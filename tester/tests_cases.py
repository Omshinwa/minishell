#!/usr/bin/env python3
"""
Mini test runner for minishell using test_cases.csv.
Doesnt check for leaks, not closed fds etc...

Rules implemented:
- Each row is executed as if:
      ./minishell < INPUT_FILE
  where INPUT_FILE contains the literal string from the `input` column.
- Convert the \n chars in `input` to newline in stdin. (useful for
    heredoc/multiline simulations).
- Exit status expectation:
    - If `error?` is *: exit code must be non-zero (returned an error).
    - If `error?` is 0: exit code must be 0.
Notes:
- This runner compares only stdout.
- minishell often prints prompts; if your minishell prints a prompt to stdout,
  this comparison will fail unless the prompt works like this:
    ignores every thing that starts with a color or ghost emoji
"""

from __future__ import annotations

import csv
import os
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import List, Optional, Tuple


BOLD = "\x1b[1m"
RED = "\x1b[31m"
GREEN = "\x1b[32m"
RESET = "\x1b[0m"


def _decode_backslash_n(s: str) -> str:
    """Convert user-friendly newline notation into real newlines.

    Encoding used by tests_cases.csv:
    - ' @ ' means Enter (a real newline)

    Notes:
    - We intentionally do NOT decode "\\n" anymore.
    """

    if "@" not in s:
        return s
    out: list[str] = []
    i = 0
    while i < len(s):
        if i + 2 < len(s) and s[i] == " " and s[i + 1] == "@" and s[i + 2] == " ":
            out.append("\n")
            i += 3
            continue
        out.append(s[i])
        i += 1
    return "".join(out)


def _ensure_trailing_newline(s: str) -> str:
    """Ensure stdin ends with a newline, so the last line is 'entered'."""
    if s == "":
        return s
    return s if s.endswith("\n") else (s + "\n")


def _normalize_for_compare(s: str) -> str:
    """Normalize outputs for comparison.

    Keep it conservative: only normalize CRLF -> LF.
    Do not trim trailing spaces/newlines; the CSV can encode them explicitly.
    """
    return s.replace("\r\n", "\n")


def _strip_special_leading_lines(s: str) -> str:
    """Remove lines that are clearly prompts / decorations.

    User requirement: remove any line starting with a special character like
    ANSI colors (ESC) or the 👻 emoji before comparing outputs.

    Heuristic implemented:
    - Drop lines whose first character is the ESC control char (\x1b)
    - Drop lines whose first character is non-ASCII (ord > 127) (covers 👻)

    This keeps the rest of stdout intact.
    """
    out_lines: List[str] = []
    for line in s.splitlines(keepends=True):
        if not line:
            continue
        first = line[0]
        if first == "\x1b":
            continue
        if ord(first) > 127:
            continue
        out_lines.append(line)
    return "".join(out_lines)


def _equal_ignoring_single_trailing_newline(a: str, b: str) -> bool:
    """Return True if a == b, or they differ only by one trailing '\n'.

    This is intentionally conservative: it only ignores ONE final newline.
    """
    if a == b:
        return True
    if a.endswith("\n") and a[:-1] == b:
        return True
    if b.endswith("\n") and b[:-1] == a:
        return True
    return False


def _fmt_visible(s: str) -> str:
    """Pretty-print a string with newlines visible for debugging."""
    return s.replace("\n", "\\n\n")


@dataclass
class TestCase:
    idx: int
    raw_input: str
    raw_expected_stdout: str
    error_flag: "Union[int, str]"

    @property
    def stdin_text(self) -> str:
        # For interactive behavior like heredoc, we must terminate the final
        # line with a newline so minishell receives an actual Enter.
        return _ensure_trailing_newline(_decode_backslash_n(self.raw_input))

    @property
    def expected_stdout(self) -> str:
        return _decode_backslash_n(self.raw_expected_stdout)


def read_cases(csv_path: Path) -> List[TestCase]:
    # Your file is tab-separated, with header.
    cases: List[TestCase] = []
    with csv_path.open("r", encoding="utf-8", newline="") as f:
        # Preserve quotes as literal characters in fields.
        # Default csv parsing treats double quotes as quoting delimiters and strips
        # them (e.g. "haha" becomes haha). Your tests want the literal quotes.
        reader = csv.DictReader(
            f,
            delimiter="\t",
            quoting=csv.QUOTE_NONE,
            quotechar=None,
        )
        required = {"input", "expected output", "error?"}
        if reader.fieldnames is None:
            raise ValueError("CSV/TSV file has no header")
        missing = required - set(reader.fieldnames)
        if missing:
            raise ValueError(
                f"Missing required column(s): {', '.join(sorted(missing))}. "
                f"Found columns: {reader.fieldnames}"
            )

        for row in reader:
            # csv.DictReader tracks the *physical* line number in the file,
            # including the header and comment lines. This matches the line
            # numbers displayed in editors.
            line_no = reader.line_num
            # Allow comment lines in the TSV: if the first field of the row
            # starts with '#' we treat the whole row as a comment and skip it.
            first_cell = next(iter(row.values())) or ""
            if first_cell.lstrip().startswith("#"):
                continue

            raw_in = (row.get("input") or "")
            raw_exp = (row.get("expected output") or "")
            raw_err = (row.get("error?") or "").strip()

            if raw_err == "":
                raise ValueError(f"Line {line_no}: empty 'error?' field")
            if raw_err == "*":
                err_flag = "*"
            else:
                try:
                    err_flag = int(raw_err)
                except ValueError as e:
                    raise ValueError(f"Line {line_no}: invalid 'error?' value: {raw_err!r}") from e

            cases.append(TestCase(idx=line_no, raw_input=raw_in, raw_expected_stdout=raw_exp, error_flag=err_flag))

    return cases


def run_one(minishell_path: Path, tc: TestCase, timeout_s: float = 5.0) -> Tuple[int, str, str]:
    """Run minishell once with given stdin, return (exit_code, stdout, stderr)."""
    proc = subprocess.run(
        [str(minishell_path)],
        input=tc.stdin_text,
        text=True,
        capture_output=True,
        timeout=timeout_s,
        env=os.environ.copy(),
    )
    # stdout/stderr are strings due to text=True
    return proc.returncode, proc.stdout, proc.stderr


def check_exit_code(error_flag, code: int) -> bool:
    # Special wildcard: '*' means any non-zero exit code
    if error_flag == "*":
        return code != 0
    # Numeric 0: success only
    if error_flag == 0:
        return code == 0
    # For any other integer expectation, require exact match
    try:
        return code == int(error_flag)
    except Exception:
        return False


def main(argv: List[str]) -> int:
    here = Path(__file__).resolve().parent
    repo_root = here.parent  # .william/.. => repo

    csv_path = Path(argv[1]).resolve() if len(argv) >= 2 else (here / "tests_cases.csv")
    minishell_path = Path(argv[2]).resolve() if len(argv) >= 3 else (repo_root / "minishell")

    if not csv_path.exists():
        print(f"ERROR: test case file not found: {csv_path}", file=sys.stderr)
        return 2
    if not minishell_path.exists():
        print(f"ERROR: minishell binary not found: {minishell_path}", file=sys.stderr)
        return 2

    cases = read_cases(csv_path)

    total = len(cases)
    passed = 0

    for tc in cases:
        try:
            code, out, err = run_one(minishell_path, tc)
        except subprocess.TimeoutExpired:
            print(f"{BOLD}{RED}FAIL{RESET}  case {tc.idx}: timeout")
            print(f"  input: {tc.raw_input!r}")
            continue

        got_out = _strip_special_leading_lines(_normalize_for_compare(out))
        exp_out = _strip_special_leading_lines(_normalize_for_compare(tc.expected_stdout))

        ok_out = _equal_ignoring_single_trailing_newline(got_out, exp_out)
        ok_code = check_exit_code(tc.error_flag, code)

        if ok_out and ok_code:
            passed += 1
            print(f"{BOLD}{GREEN}PASS{RESET}  case {tc.idx}")
            continue

        print(f"{BOLD}{RED}FAIL{RESET}  case {tc.idx}")
        print(f"  input (raw): {tc.raw_input!r}")
        if tc.raw_input != tc.stdin_text:
            print("  input (decoded):")
            print("    " + _fmt_visible(tc.stdin_text).replace("\n", "\n    ").rstrip())

        if not ok_code:
            print(f"  exit code: got {code}, expected: {tc.error_flag}")
        if not ok_out:
            print("  stdout mismatch:")
            print(f"  expected raw field (repr): {tc.raw_expected_stdout!r}")
            print(f"  got raw stdout (repr):     {out!r}")
            print(f"  expected (repr): {exp_out!r}")
            print(f"  got (repr):      {got_out!r}")
            print("  expected:")
            print("    " + _fmt_visible(exp_out).replace("\n", "\n    ").rstrip())
            print("  got:")
            print("    " + _fmt_visible(got_out).replace("\n", "\n    ").rstrip())

        # Helpful extra context, but only on failure.
        if err:
            print("  stderr (captured):")
            print("    " + _fmt_visible(_normalize_for_compare(err)).replace("\n", "\n    ").rstrip())

    print(f"\nSummary: {passed}/{total} passed")
    return 0 if passed == total else 1


if __name__ == "__main__":
    main(sys.argv)
    # raise SystemExit()
