#!/usr/bin/env python3
"""
Analyze all headers under include/rbox/ and report which C++ standard library
headers each one directly or indirectly (transitively) includes.

Usage:
    python3 dev/analyze_stdlib_includes.py [--detailed] [--direct-only]
"""

import re
import sys
from collections import defaultdict
from pathlib import Path

# ---------------------------------------------------------------------------
# Configuration
# ---------------------------------------------------------------------------

PROJECT_ROOT = Path(__file__).resolve().parent.parent
INCLUDE_DIR = PROJECT_ROOT / "include" / "rbox"

# Regex:   #include <...>
INCLUDE_RE = re.compile(r'^\s*#\s*include\s+<([^>]+)>')

# Project-local includes start with "rbox/"
PROJECT_PREFIX = "rbox/"

# ---- ANSI color helpers ----

_RBOX_COLOR = "\033[36m"     # cyan
_STDLIB_COLOR = "\033[32m"   # green
_RESET = "\033[0m"


def _noop_color(s: str) -> str:
    return s


def _color_rbox(s: str) -> str:
    return f"{_RBOX_COLOR}{s}{_RESET}"


def _color_stdlib(s: str) -> str:
    return f"{_STDLIB_COLOR}{s}{_RESET}"


# Mutable so --colored can swap them in
color_rbox = _noop_color
color_stdlib = _noop_color


def _apply_colors(colored: bool) -> None:
    global color_rbox, color_stdlib
    if colored:
        color_rbox = _color_rbox
        color_stdlib = _color_stdlib
    else:
        color_rbox = _noop_color
        color_stdlib = _noop_color


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def resolve_include(inc: str) -> Path:
    """Resolve a project-local include like 'rbox/utils/meta_pair.hpp'
    to an absolute filesystem path."""
    return PROJECT_ROOT / "include" / inc


def parse_includes(filepath: Path):
    """Return (project_includes: list[str], stdlib_includes: list[str])
    for a single header file."""
    proj = []
    std = []
    try:
        text = filepath.read_text(encoding="utf-8")
    except OSError:
        return proj, std

    for line in text.splitlines():
        m = INCLUDE_RE.match(line)
        if not m:
            continue
        inc = m.group(1)
        if inc.startswith(PROJECT_PREFIX):
            proj.append(inc)
        else:
            std.append(inc)
    return proj, std


# ---------------------------------------------------------------------------
# Main analysis
# ---------------------------------------------------------------------------

def collect_all_headers() -> dict[str, Path]:
    """Walk include/rbox/ and return {logical_name -> absolute_path}."""
    headers = {}
    for fpath in sorted(INCLUDE_DIR.rglob("*.hpp")):
        rel = fpath.relative_to(INCLUDE_DIR)
        logical = f"{PROJECT_PREFIX}{rel.as_posix()}"
        headers[logical] = fpath
    return headers


def build_dep_graph(headers: dict[str, Path]):
    """Return (proj_deps, direct_stdlib) where:
    proj_deps:  logical_name -> set of logical_names (project deps)
    direct_stdlib: logical_name -> set of stdlib header names
    """
    proj_deps = defaultdict(set)
    direct_stdlib = {}

    for logical, fpath in headers.items():
        pi, si = parse_includes(fpath)
        proj_deps[logical] = set(pi)
        direct_stdlib[logical] = set(si)

    return proj_deps, direct_stdlib


def transitive_stdlib(logical: str, proj_deps, direct_stdlib, memo: dict):
    """Return the set of all stdlib headers reachable from `logical`."""
    if logical in memo:
        return memo[logical]

    result = set(direct_stdlib.get(logical, set()))
    for dep in proj_deps.get(logical, set()):
        result |= transitive_stdlib(dep, proj_deps, direct_stdlib, memo)
    memo[logical] = result
    return result


# ---------------------------------------------------------------------------
# Output formatting
# ---------------------------------------------------------------------------

def print_results(headers, proj_deps, direct_stdlib):
    all_stdlib = transitive_stdlib_for_all(headers, proj_deps, direct_stdlib)

    for logical in sorted(headers):
        fpath = headers[logical]
        rel = str(fpath.relative_to(PROJECT_ROOT))
        libs = sorted(all_stdlib[logical])

        print(f"{'='*70}")
        print(f"File:  {color_rbox(rel)}")
        print(f"{'-'*70}")

        if libs:
            print(f"  ({len(libs)} stdlib headers)")
            for h in libs:
                print(f"    <{color_stdlib(h)}>")
        else:
            print(f"  (0 stdlib headers)")

        print()


def print_results_detailed(headers, proj_deps, direct_stdlib):
    """Show the include chain for each stdlib header with multi-line indentation."""
    all_stdlib = transitive_stdlib_for_all(headers, proj_deps, direct_stdlib)

    for logical in sorted(headers):
        fpath = headers[logical]
        rel = str(fpath.relative_to(PROJECT_ROOT))
        libs = sorted(all_stdlib[logical])

        print(f"{'='*70}")
        print(f"File:  {color_rbox(rel)}")
        print(f"{'-'*70}")

        if libs:
            print(f"  ({len(libs)} stdlib headers)")
            for h in libs:
                chain = find_chain(logical, h, proj_deps, direct_stdlib, headers)
                if len(chain) == 1:
                    print(f"    <{color_stdlib(h)}>  directly")
                else:
                    print(f"    <{color_stdlib(h)}>  via")
                    # chain[0] is the starting file
                    for step in chain[1:]:
                        step_rel = str(headers[step].relative_to(PROJECT_ROOT))
                        print(f"       -> {color_rbox(step_rel)}")
        else:
            print(f"  (0 stdlib headers)")

        print()


def find_chain(logical, target_stdlib, proj_deps, direct_stdlib, headers):
    """Find a chain of project includes from logical to a file that directly
    includes target_stdlib. Returns list of logical names."""
    from collections import deque

    if target_stdlib in direct_stdlib.get(logical, set()):
        return [logical]

    visited = {logical}
    queue = deque()
    queue.append((logical, [logical]))

    while queue:
        current, path = queue.popleft()
        for dep in proj_deps.get(current, set()):
            if dep in visited:
                continue
            visited.add(dep)
            new_path = path + [dep]
            if target_stdlib in direct_stdlib.get(dep, set()):
                return new_path
            queue.append((dep, new_path))
    return [logical, "???"]


def transitive_stdlib_for_all(headers, proj_deps, direct_stdlib):
    memo = {}
    result = {}
    for logical in headers:
        result[logical] = transitive_stdlib(logical, proj_deps, direct_stdlib, memo)
    return result


def print_summary(headers, proj_deps, direct_stdlib):
    all_stdlib = transitive_stdlib_for_all(headers, proj_deps, direct_stdlib)

    # Collect reverse index: stdlib -> list of headers that include it
    stdlib_to_headers = defaultdict(set)
    for logical, libs in all_stdlib.items():
        for lib in libs:
            stdlib_to_headers[lib].add(logical)

    print(f"{'='*70}")
    print("SUMMARY: C++ Standard Library headers used in rbox")
    print(f"{'='*70}")
    print()

    for lib in sorted(stdlib_to_headers):
        consumers = sorted(stdlib_to_headers[lib])
        print(f"  <{color_stdlib(lib)}>  ({len(consumers)} file(s)):")
        for c in consumers:
            rel = str(headers[c].relative_to(PROJECT_ROOT))
            print(f"      {color_rbox(rel)}")
        print()


# ---------------------------------------------------------------------------
# Entry point
# ---------------------------------------------------------------------------

def main():
    direct_only = "--direct-only" in sys.argv
    detailed = "--detailed" in sys.argv
    colored = "--colored" in sys.argv

    _apply_colors(colored)

    headers = collect_all_headers()
    proj_deps, direct_stdlib = build_dep_graph(headers)

    if direct_only:
        print("=== Direct stdlib includes only ===\n")
        for logical in sorted(headers):
            fpath = headers[logical]
            rel = str(fpath.relative_to(PROJECT_ROOT))
            libs = sorted(direct_stdlib.get(logical, set()))
            if libs:
                print(f"{color_rbox(rel)}:")
                for lib in libs:
                    print(f"  <{color_stdlib(lib)}>")
            else:
                print(f"{color_rbox(rel)}: (none)")
    elif detailed:
        print_results_detailed(headers, proj_deps, direct_stdlib)
        print_summary(headers, proj_deps, direct_stdlib)
    else:
        print_results(headers, proj_deps, direct_stdlib)
        print_summary(headers, proj_deps, direct_stdlib)


if __name__ == "__main__":
    main()
