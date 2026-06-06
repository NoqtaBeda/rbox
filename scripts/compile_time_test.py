'''
Test the compilation time of each target (UT, examples, benchmarks, etc.).
The script performs the following steps:
1. Run xmake configuration command:
   "xmake f -p linux -a x86_64 -m MODE --sdk=PATH --toolchain=gcc --cxxflags="-freflection" --static-test=[yn]"
2. Run build command "xmake clean TARGET && xmake build TARGET" for each TARGET available.
   Compilation time can be obtained by xmake output: "[100%]: build ok, spent ???s" if compilation succeeds.
   Each target is repeated for R times and the average compilation time is taken.
3. Output a summary table to specified file or stdout after all the targets are tested.

Options:
  --gcc-path PATH: Sets the root directory of GCC. Default is "/usr".
  --mode (or -m) MODE: Set build mode (debug, release, etc.). Default is "debug".
  --static-test (or -S): Enables static test (with argument "--static-test=y").
  --retry-count (or -r) R: Set the number of repeated times for each target. Default is 5.
  --filter (or -f) REGEX: Sets the filter of targets to be tested.
                          A target is tested only if its name **fully matches** given regular expression.
                          All the available targets will be tested if this option is not set.
  --output (or -o) FILE: Output the final testing result to given file.
                         The testing result is formatted as a table with 2 columns.
                         The first column has title "Build Target" and the second has "Compilation Time"
                         Supported file extensions:
                         *.csv: Output as CSV format.
                         *.md:  Output as Markdown table (center-aligned).
                         The result table is written to stdout if this option is not set.
  --pcores LIST: Pin all compiler processes to the given CPU cores (taskset format, e.g. "0-15" or "0,1,2,3").
                 Mutually exclusive with --pin-to-pcores.
  --pin-to-pcores: Auto-detect performance cores and pin compiler processes to them.
                   Mutually exclusive with --pcores.
  --verbose (or -v): Shows the detailed log message to stdout.
  --help: Shows this document.
'''

import argparse
import glob
import os
import re
import subprocess
import sys
from typing import Optional

# Module-level taskset prefix, set by main() based on --pcores / --pin-to-pcores.
_TASKSET_PREFIX = ""


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Test the compilation time of each xmake target."
    )
    parser.add_argument(
        "--gcc-path",
        default="/usr",
        help="Root directory of GCC. Default: /usr",
    )
    parser.add_argument(
        "--mode", "-m",
        default="debug",
        help="Build mode (debug, release, etc.). Default: debug",
    )
    parser.add_argument(
        "--static-test", "-S",
        action="store_true",
        help="Enable static test (--static-test=y).",
    )
    parser.add_argument(
        "--retry-count", "-r",
        type=int,
        default=5,
        help="Number of repeated builds per target. Default: 5",
    )
    parser.add_argument(
        "--filter", "-f",
        default=None,
        help=(
            "Regex filter for target names. "
            "A target is tested only if its name fully matches the regex. "
            "All targets are tested if not set."
        ),
    )
    parser.add_argument(
        "--output", "-o",
        default=None,
        help=(
            "Output file for results. Supports *.csv and *.md. "
            "If not set, prints to stdout."
        ),
    )
    parser.add_argument(
        "--verbose", "-v",
        action="store_true",
        help="Show detailed log messages to stdout.",
    )
    pcore_group = parser.add_mutually_exclusive_group()
    pcore_group.add_argument(
        "--pcores",
        default=None,
        help=(
            "Pin compiler processes to the given CPU cores. "
            "Accepts taskset-style list, e.g. \"0-15\" or \"0,1,2,3\". "
            "Mutually exclusive with --pin-to-pcores."
        ),
    )
    pcore_group.add_argument(
        "--pin-to-pcores",
        action="store_true",
        help=(
            "Auto-detect performance cores and pin compiler processes to them. "
            "Mutually exclusive with --pcores."
        ),
    )
    return parser.parse_args()


def run_cmd(cmd: str, cwd: Optional[str] = None, timeout: Optional[int] = None) -> subprocess.CompletedProcess:
    """Run a shell command and return the CompletedProcess."""
    if _TASKSET_PREFIX:
        cmd = f"{_TASKSET_PREFIX} {cmd}"
    return subprocess.run(
        cmd,
        shell=True,
        capture_output=True,
        text=True,
        cwd=cwd,
        timeout=timeout,
    )


def configure_xmake(args: argparse.Namespace) -> None:
    """Run the xmake configuration step."""
    static_test_flag = "y" if args.static_test else "n"

    cmd = (
        f'xmake f -p linux -a x86_64 -m {args.mode}'
        f' --ccache=n'
        f' --sdk={args.gcc_path}'
        f' --toolchain=gcc'
        f' --cxxflags="-freflection"'
        f' --static-test={static_test_flag}'
    )
    print(f"[configure] Running: {cmd}")
    result = run_cmd(cmd)
    if result.returncode != 0:
        print(f"[configure] ERROR:\n{result.stderr}")
        sys.exit(1)
    print("[configure] OK")


def list_targets() -> list[str]:
    """Retrieve all available xmake build targets."""
    result = run_cmd("xmake show -l targets")
    if result.returncode != 0:
        print(f"[list_targets] ERROR:\n{result.stderr}")
        sys.exit(1)

    # The output is multi-column space-separated; split on whitespace.
    # Strip ANSI escape sequences that may be appended to target names.
    raw_targets = _ANSI_ESCAPE_RE.sub("", result.stdout).split()
    # Filter out any stray non-target tokens (xmake targets look like "group-name").
    targets = [t for t in raw_targets if "-" in t]
    print(f"[list_targets] Found {len(targets)} targets.")
    return targets


# Regex to extract compilation time from xmake output:
#   "[100%]: build ok, spent 1.234s"
_BUILD_TIME_RE = re.compile(r"\[100%\]: build ok, spent ([\d.]+)s")

# Regex to strip ANSI escape sequences (color codes) from xmake output.
_ANSI_ESCAPE_RE = re.compile(r"\x1b\[[0-9;]*m")


def build_target_once(target: str, warmup: bool = False, verbose: bool = False) -> Optional[float]:
    """
    Clean and build a single target once.
    Returns the compilation time in seconds, or None on failure.
    If *warmup* is True, the result is not used for averaging
    (but we still return the time for logging).
    """
    # Clean
    clean_result = run_cmd(f"xmake clean {target}")
    if clean_result.returncode != 0:
        if verbose:
            print(f"[build] WARNING: clean failed for {target}:\n{clean_result.stdout}{clean_result.stderr}")
        else:
            print(f"[build] WARNING: clean failed for {target}")

    # Build
    build_result = run_cmd(f"xmake build {'--verbose' if verbose else ''} {target}")
    if build_result.returncode != 0:
        if verbose:
            print(f"[build] ERROR: build failed for {target}:\n{build_result.stdout}{build_result.stderr}")
        else:
            print(f"[build] ERROR: build failed for {target} (use -v for compiler output)")
        return None

    # Parse timing from combined stdout+stderr
    output = _ANSI_ESCAPE_RE.sub("", build_result.stdout + build_result.stderr)
    match = _BUILD_TIME_RE.search(output)
    if match:
        return float(match.group(1))
    else:
        if verbose:
            print(f"[build] WARNING: could not parse build time for {target}")
        return None


def build_target_repeated(
    target: str,
    retry_count: int,
    warmup: bool = True,
    verbose: bool = False,
) -> tuple[str, Optional[float]]:
    """
    Build a target *retry_count* times (plus one optional warm-up build),
    returning (target_name, average_time_in_seconds).
    """
    if warmup:
        if verbose:
            print(f"[build] {target}: warm-up build...")
        t = build_target_once(target, warmup=True, verbose=verbose)
        if t is not None:
            if verbose:
                print(f"[build] {target}: warm-up done ({t:.3f}s)")
        else:
            print(f"{'' if verbose else chr(10)}[build] {target}: warm-up FAILED")

    times: list[float] = []
    for i in range(1, retry_count + 1):
        if verbose:
            print(f"[build] {target}: run {i}/{retry_count}...")
        t = build_target_once(target, verbose=verbose)
        if t is None:
            print(f"{'' if verbose else chr(10)}[build] {target}: run {i} FAILED, skipping remaining runs.")
            break
        times.append(t)
        if verbose:
            print(f"[build] {target}: run {i} finished ({t:.3f}s)")

    if not times:
        print(f"{'' if verbose else chr(10)}[build] {target}: all runs failed.")
        return target, None

    avg = sum(times) / len(times)
    print(f"{'' if verbose else chr(10)}[build] {target}: average over {len(times)} runs = {avg:.3f}s")
    return target, avg


def format_table(results: list[tuple[str, Optional[float]]], fmt: str) -> str:
    """
    Format results as a table string.

    *fmt* is one of "csv", "md", or "stdout".
    """
    if fmt == "csv":
        lines = ["Build Target,Compilation Time (s)"]
        for name, avg in results:
            time_str = f"{avg:.3f}" if avg is not None else "FAILED"
            lines.append(f"{name},{time_str}")
        return "\n".join(lines)

    elif fmt == "md":
        lines = [
            "| Build Target | Compilation Time (s) |",
            "|:---:|:---:|",
        ]
        for name, avg in results:
            time_str = f"{avg:.3f}" if avg is not None else "FAILED"
            lines.append(f"| {name} | {time_str} |")
        return "\n".join(lines)

    else:  # stdout
        # Determine column widths
        max_name = max((len(r[0]) for r in results), default=12)
        max_name = max(max_name, len("Build Target"))
        max_time = len("Compilation Time (s)")
        header = f"{'Build Target':<{max_name}}  {'Compilation Time (s)':>{max_time}}"
        sep = "-" * len(header)
        lines = [header, sep]
        for name, avg in results:
            time_str = f"{avg:.3f}" if avg is not None else "FAILED"
            lines.append(f"{name:<{max_name}}  {time_str:>{max_time}}")
        return "\n".join(lines)


def detect_pcores() -> Optional[str]:
    """
    Auto-detect performance cores and return a taskset-compatible CPU list.
    Returns None if detection fails or the CPU does not have a hybrid architecture.
    """
    # Method 1: cpu_capacity (ARM big.LITTLE / Intel hybrid on newer kernels)
    try:
        capacities = []
        for cpu_dir in sorted(glob.glob("/sys/devices/system/cpu/cpu[0-9]*")):
            cap_file = os.path.join(cpu_dir, "cpu_capacity")
            if os.path.exists(cap_file):
                with open(cap_file) as f:
                    cap = int(f.read().strip())
                cpu_id = int(os.path.basename(cpu_dir)[3:])
                capacities.append((cpu_id, cap))
        if capacities:
            max_cap = max(c[1] for c in capacities)
            if max_cap > 0:
                pcores = sorted(c[0] for c in capacities if c[1] == max_cap)
                ecores = sorted(c[0] for c in capacities if c[1] != max_cap)
                if ecores:
                    return _cpu_list_to_taskset(pcores)
    except Exception:
        pass

    # Method 2: lscpu -e (Intel Alder Lake+)
    try:
        result = subprocess.run(
            "lscpu -e=CPU,MAXMHZ", shell=True, capture_output=True, text=True
        )
        freqs = []
        for line in result.stdout.strip().split("\n")[1:]:
            parts = line.split()
            if len(parts) >= 2:
                try:
                    cpu = int(parts[0].strip())
                    mhz = float(parts[1].strip())
                    freqs.append((cpu, mhz))
                except ValueError:
                    continue
        if freqs:
            max_mhz = max(f[1] for f in freqs)
            min_mhz = min(f[1] for f in freqs)
            if max_mhz > min_mhz * 1.1:
                pcores = sorted(c[0] for c in freqs if c[1] == max_mhz)
                return _cpu_list_to_taskset(pcores)
    except Exception:
        pass

    return None


def _cpu_list_to_taskset(cpu_ids: list[int]) -> str:
    """Convert a sorted list of CPU IDs to a compact taskset range string."""
    if not cpu_ids:
        return ""
    ranges = []
    start = cpu_ids[0]
    end = cpu_ids[0]
    for cpu in cpu_ids[1:]:
        if cpu == end + 1:
            end = cpu
        else:
            ranges.append(str(start) if start == end else f"{start}-{end}")
            start = cpu
            end = cpu
    ranges.append(str(start) if start == end else f"{start}-{end}")
    return ",".join(ranges)


def main() -> None:
    global _TASKSET_PREFIX

    args = parse_args()

    # Resolve P-core pinning
    if args.pin_to_pcores:
        pcore_list = detect_pcores()
        if pcore_list is None:
            print("[pcores] ERROR: could not auto-detect performance cores.")
            sys.exit(1)
        print(f"[pcores] Auto-detected performance cores: {pcore_list}")
    elif args.pcores:
        pcore_list = args.pcores
        print(f"[pcores] Using user-specified performance cores: {pcore_list}")
    else:
        pcore_list = None

    if pcore_list:
        # Verify taskset is available
        check = subprocess.run(
            "which taskset", shell=True, capture_output=True, text=True
        )
        if check.returncode != 0:
            print("[pcores] ERROR: 'taskset' command not found.")
            sys.exit(1)
        _TASKSET_PREFIX = f"taskset -c {pcore_list}"

    # Step 1: configure
    configure_xmake(args)

    # Step 2: discover targets
    targets = list_targets()
    if not targets:
        print("No targets found. Exiting.")
        sys.exit(1)

    # Apply filter if provided
    if args.filter:
        pattern = re.compile(args.filter)
        filtered = [t for t in targets if pattern.fullmatch(t)]
        print(f"[filter] '{args.filter}' matched {len(filtered)}/{len(targets)} targets.")
        targets = filtered
        if not targets:
            print("No targets matched the filter. Exiting.")
            sys.exit(1)

    # Sort targets alphabetically for consistent ordering
    targets.sort()

    # Step 3: build each target
    results: list[tuple[str, Optional[float]]] = []

    for i, t in enumerate(targets):
        if args.verbose:
            print(f"\n[build] === Target {i+1}/{len(targets)}: {t} ===")
        else:
            print(f"[build] ({i+1}/{len(targets)}) {t}...", end=" ", flush=True)
        name, avg = build_target_repeated(t, args.retry_count, verbose=args.verbose)
        results.append((name, avg))

    # Step 4: output
    if args.output:
        ext = os.path.splitext(args.output)[1].lower()
        if ext == ".csv":
            fmt = "csv"
        elif ext == ".md":
            fmt = "md"
        else:
            print(f"[output] Unrecognized extension '{ext}', defaulting to CSV.")
            fmt = "csv"
        table = format_table(results, fmt)
        with open(args.output, "w") as f:
            f.write(table + "\n")
        print(f"[output] Results written to {args.output}")
    else:
        table = format_table(results, "stdout")
        print("\n" + table)


if __name__ == "__main__":
    main()
