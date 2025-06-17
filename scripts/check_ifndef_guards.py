#!/usr/bin/env python3
"""
Check that the #ifndef / #define include guard in each .hpp file under include/
matches the file's path relative to the include/ directory.

Convention:
  include/rbox/utils/ctype/ascii_test.hpp
  -> expected guard: RBOX_UTILS_CTYPE_ASCII_TEST_HPP

Rule:
  relative_path = path.relative_to(include_dir)   # e.g. rbox/utils/ctype/ascii_test.hpp
  expected = 'RBOX_' + relative_path.upper()       # uppercase, / -> _ implied by stem/suffix
             .replace('/', '_')
             .replace('.HPP', '_HPP')               # .hpp -> _HPP
"""

import sys
from pathlib import Path


def expected_guard(header_path: Path, include_dir: Path) -> str:
    """Compute the expected include guard for a header file."""
    rel = header_path.relative_to(include_dir)
    # Build guard: replace path separators with _, replace .hpp extension with _HPP, uppercase
    guard = str(rel).replace('/', '_').replace('\\', '_')
    if guard.endswith('.hpp'):
        guard = guard[:-4] + '_HPP'
    return guard.upper()


def extract_guard(header_path: Path) -> tuple[str | None, str | None, int, int, str | None, int]:
    """
    Extract the #ifndef, #define guard names and #endif trailing comment
    from a header file.
    Returns (ifndef_name, define_name, ifndef_lineno, define_lineno,
             endif_comment, endif_lineno).
    """
    ifndef_name = None
    define_name = None
    ifndef_lineno = 0
    define_lineno = 0
    endif_comment = None
    endif_lineno = 0

    try:
        with open(header_path, 'r', encoding='utf-8') as f:
            lines = f.readlines()
    except Exception as e:
        print(f"  ERROR reading file: {e}", file=sys.stderr)
        return None, None, 0, 0, None, 0

    for i, line in enumerate(lines, start=1):
        stripped = line.strip()
        if stripped.startswith('#ifndef ') and ifndef_name is None:
            ifndef_name = stripped[len('#ifndef '):].strip()
            ifndef_lineno = i
        elif stripped.startswith('#define ') and define_name is None:
            if ifndef_name is not None:
                define_name = stripped[len('#define '):].strip()
                define_lineno = i
        elif stripped.startswith('#endif'):
            # Extract trailing comment after //
            comment = ''
            if '//' in stripped:
                comment = stripped.split('//', 1)[1].strip()
            endif_comment = comment if comment else None
            endif_lineno = i
            # Only take the first #endif (in simple headers there's only one)

    return ifndef_name, define_name, ifndef_lineno, define_lineno, endif_comment, endif_lineno


def main():
    workspace = Path(__file__).resolve().parent.parent
    include_dir = workspace / 'include'
    if not include_dir.is_dir():
        print(f"ERROR: include/ directory not found at {include_dir}", file=sys.stderr)
        sys.exit(1)

    hpp_files = sorted(include_dir.rglob('*.hpp'))
    if not hpp_files:
        print("No .hpp files found.")
        return

    errors = 0
    ok_count = 0

    for hpp in hpp_files:
        expected = expected_guard(hpp, include_dir)
        ifndef_name, define_name, ifndef_ln, define_ln, endif_comment, endif_ln = extract_guard(hpp)

        issues = []
        if ifndef_name is None:
            issues.append("missing #ifndef guard")
        else:
            if ifndef_name != expected:
                issues.append(
                    f"#ifndef '{ifndef_name}' (line {ifndef_ln}) "
                    f"does not match expected '{expected}'"
                )
        if define_name is None:
            issues.append("missing #define guard")
        else:
            if define_name != expected:
                issues.append(
                    f"#define '{define_name}' (line {define_ln}) "
                    f"does not match expected '{expected}'"
                )
            elif ifndef_name is not None and define_name != ifndef_name:
                issues.append(
                    f"#ifndef '{ifndef_name}' (line {ifndef_ln}) "
                    f"and #define '{define_name}' (line {define_ln}) do not match"
                )

        if endif_ln == 0:
            issues.append("missing #endif")
        elif endif_comment is None:
            issues.append(
                f"#endif (line {endif_ln}) missing guard comment, "
                f"expected '// {expected}'"
            )
        elif endif_comment != expected:
            issues.append(
                f"#endif comment '// {endif_comment}' (line {endif_ln}) "
                f"does not match expected '// {expected}'"
            )

        if issues:
            rel = hpp.relative_to(workspace)
            print(f"❌ {rel}")
            for issue in issues:
                print(f"     {issue}")
            errors += 1
        else:
            ok_count += 1

    print()
    print(f"Checked {len(hpp_files)} header(s): {ok_count} OK, {errors} error(s).")

    if errors:
        sys.exit(1)


if __name__ == '__main__':
    main()
