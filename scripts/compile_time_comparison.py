'''
Compares two CSV files (written by compile_time_test.py).
Outputs a table (CSV, Markdown, or stdout) showing per-target compilation
time differences, sorted by relative improvement (most improved first).

Usage: python compile_time_comparison.py BEFORE AFTER
       BEFORE and AFTER are paths to the CSV files to be compared.
'''

import argparse
import csv
import re
import sys


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Compare two compile-time CSV results."
    )
    parser.add_argument(
        "before",
        help="Path to the baseline (before) CSV file.",
    )
    parser.add_argument(
        "after",
        help="Path to the new (after) CSV file.",
    )
    parser.add_argument(
        "--output", "-o",
        default=None,
        help=(
            "Output format. Supported extensions: *.csv (CSV), *.md (Markdown table). "
            "If not set, prints a human-readable table to stdout."
        ),
    )
    parser.add_argument(
        "--sort-by",
        default="ratio",
        choices=("ratio", "delta", "name"),
        help="Sort key for results. Default: ratio (biggest improvement first).",
    )
    parser.add_argument(
        "--colored", "-c",
        action="store_true",
        help="Color-code the speedup column with ANSI escape codes.",
    )
    parser.add_argument(
        "--filter", "-f",
        default=None,
        help=(
            "Regex filter for target names. "
            "A target is compared only if its name fully matches the regex. "
            "All common targets are compared if not set."
        ),
    )
    return parser.parse_args()


def parse_csv(path: str) -> dict[str, float]:
    """Read a CSV file and return a dict mapping target_name -> time_in_seconds."""
    data: dict[str, float] = {}
    with open(path, newline="", encoding="utf-8") as f:
        reader = csv.reader(f)
        header = next(reader, None)
        if header is None:
            print(f"ERROR: {path} is empty.", file=sys.stderr)
            sys.exit(1)
        # strip BOM / whitespace from header
        header = [h.strip().lstrip("\\ufeff") for h in header]
        try:
            name_idx = header.index("Build Target")
            time_idx = header.index("Compilation Time (s)")
        except ValueError:
            print(
                f"ERROR: {path} missing required columns. "
                f"Expected 'Build Target' and 'Compilation Time (s)'.",
                file=sys.stderr,
            )
            sys.exit(1)
        for row in reader:
            if not row or all(cell.strip() == "" for cell in row):
                continue
            name = row[name_idx].strip()
            time_str = row[time_idx].strip()
            if time_str.upper() == "FAILED":
                continue  # skip failed builds
            try:
                data[name] = float(time_str)
            except ValueError:
                print(f"WARNING: cannot parse time '{time_str}' for target '{name}'. Skipping.", file=sys.stderr)
    print(f"[parse] {path}: {len(data)} targets read.")
    return data


def compute_comparison(
    before: dict[str, float],
    after: dict[str, float],
) -> list[dict]:
    """
    Join before and after data. Returns a list of dicts with keys:
      name, before, after, delta, ratio
    where delta = after - before (negative = improvement in seconds),
    and   ratio = before / after - 1 (positive = speedup, negative = regression).
    """
    common = set(before.keys()) & set(after.keys())
    only_before = set(before.keys()) - set(after.keys())
    only_after = set(after.keys()) - set(before.keys())

    if only_before:
        print(f"[info] Targets only in BEFORE: {', '.join(sorted(only_before))}")
    if only_after:
        print(f"[info] Targets only in AFTER:  {', '.join(sorted(only_after))}")

    results: list[dict] = []
    for name in sorted(common):
        b = before[name]
        a = after[name]
        delta = a - b
        ratio = (b / a - 1.0) if a != 0.0 else float("inf")
        results.append({
            "name": name,
            "before": b,
            "after": a,
            "delta": delta,
            "ratio": ratio,
        })
    return results


def sort_results(results: list[dict], key: str) -> list[dict]:
    """Sort results by the given key."""
    if key == "ratio":
        # biggest speedup first (descending)
        return sorted(results, key=lambda r: r["ratio"], reverse=True)
    elif key == "delta":
        return sorted(results, key=lambda r: r["delta"])
    else:  # "name"
        return sorted(results, key=lambda r: r["name"])


# ANSI color codes
_ANSI_RESET = "\033[0m"
_ANSI_DEEP_GREEN = "\033[38;5;22m"
_ANSI_LIGHT_GREEN = "\033[38;5;40m"
_ANSI_LIGHT_RED = "\033[38;5;203m"
_ANSI_DEEP_RED = "\033[38;5;160m"


def _color_speedup(ratio: float, text: str, colored: bool) -> str:
    """Wrap *text* in ANSI color if *colored* and ratio crosses a threshold."""
    if not colored:
        return text
    if ratio > 0.10:
        return f"{_ANSI_DEEP_GREEN}{text}{_ANSI_RESET}"
    elif ratio > 0.05:
        return f"{_ANSI_LIGHT_GREEN}{text}{_ANSI_RESET}"
    elif ratio < -0.10:
        return f"{_ANSI_DEEP_RED}{text}{_ANSI_RESET}"
    elif ratio < -0.05:
        return f"{_ANSI_LIGHT_RED}{text}{_ANSI_RESET}"
    return text


def format_stdout(results: list[dict], colored: bool = False) -> str:
    """Format results as a human-readable aligned table."""
    if not results:
        return "(no common targets to compare)"

    header = ("Build Target", "Before (s)", "After (s)", "Δ (s)", "Speedup")
    rows: list[tuple[str, str, str, str, str]] = []
    for r in results:
        speedup_str = f"{r['ratio']:+.1%}"
        rows.append((
            r["name"],
            f"{r['before']:.3f}",
            f"{r['after']:.3f}",
            f"{r['delta']:+.3f}",
            _color_speedup(r["ratio"], speedup_str, colored),
        ))

    # determine column widths (strip ANSI codes from speedup column for accurate width)
    widths = [len(h) for h in header]
    _ansi_re = __import__("re").compile(r"\x1b\[[0-9;]*m")
    for row in rows:
        for i, cell in enumerate(row):
            widths[i] = max(widths[i], len(_ansi_re.sub("", cell)))

    def fmt_row(cells: tuple[str, ...]) -> str:
        parts: list[str] = []
        for c, w in zip(cells, widths):
            visible_len = len(_ansi_re.sub("", c))
            pad = w - visible_len
            parts.append(c + " " * pad if pad > 0 else c)
        return "  ".join(parts)

    lines = [fmt_row(header), "-" * (sum(widths) + 2 * (len(widths) - 1))]
    for row in rows:
        lines.append(fmt_row(row))

    # summary
    total_before = sum(r["before"] for r in results)
    total_after = sum(r["after"] for r in results)
    total_delta = total_after - total_before
    total_speedup = (total_before / total_after - 1.0) if total_after != 0.0 else float("inf")
    speedup_str = f"{total_speedup:+.1%}"
    lines.append("")
    lines.append(f"Summary: {len(results)} targets, total {total_before:.3f}s → {total_after:.3f}s "
                 f"(Δ={total_delta:+.3f}s, speedup={_color_speedup(total_speedup, speedup_str, colored)})")

    return "\n".join(lines)


def format_csv(results: list[dict]) -> str:
    """Format results as CSV."""
    lines = ["Build Target,Before (s),After (s),Δ (s),Speedup"]
    for r in results:
        lines.append(f"{r['name']},{r['before']:.3f},{r['after']:.3f},{r['delta']:+.3f},{r['ratio']:+.1%}")
    return "\n".join(lines)


def format_md(results: list[dict]) -> str:
    """Format results as a Markdown table."""
    lines = [
        "| Build Target | Before (s) | After (s) | Δ (s) | Speedup |",
        "|:---|:---:|:---:|:---:|:---:|",
    ]
    for r in results:
        lines.append(
            f"| {r['name']} | {r['before']:.3f} | {r['after']:.3f} "
            f"| {r['delta']:+.3f} | {r['ratio']:+.1%} |"
        )
    return "\n".join(lines)


def main() -> None:
    args = parse_args()

    before_data = parse_csv(args.before)
    after_data = parse_csv(args.after)

    results = compute_comparison(before_data, after_data)
    if not results:
        print("No common targets to compare.", file=sys.stderr)
        sys.exit(0)

    # Apply filter if provided
    if args.filter:
        pattern = re.compile(args.filter)
        filtered = [r for r in results if pattern.fullmatch(r["name"])]
        print(f"[filter] '{args.filter}' matched {len(filtered)}/{len(results)} targets.")
        results = filtered
        if not results:
            print("No targets matched the filter. Exiting.")
            sys.exit(0)

    results = sort_results(results, args.sort_by)

    if args.output:
        ext = args.output.rsplit(".", 1)[-1].lower()
        if ext == "csv":
            formatted = format_csv(results)
        elif ext == "md":
            formatted = format_md(results)
        else:
            print(f"ERROR: unsupported output format '.{ext}'. Use .csv or .md.", file=sys.stderr)
            sys.exit(1)
        with open(args.output, "w", encoding="utf-8") as f:
            f.write(formatted)
        print(f"Written to {args.output}")
    else:
        print(format_stdout(results, colored=args.colored))


if __name__ == "__main__":
    main()