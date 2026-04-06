#!/usr/bin/env bash
# Exercises reschedule, mark undone, second list, delete list, add task to named list.
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"
[[ -x ./scheduler ]] || {
  echo "Build ./scheduler first (make)."
  exit 1
}

OUT="$(mktemp)"
trap 'rm -f "$OUT"' EXIT

./scheduler <<'EOF' | tee "$OUT"
2026-04-06 12:00
7
Work
1
Task A
2026-04-10

4
1
2026-04-15

3
1
7
Personal
8
Work
1
Task B


8
Personal
0
EOF

grep -q "rescheduled" "$OUT" || {
  echo "FAIL: reschedule"
  exit 1
}
grep -q "marked as not done" "$OUT" || {
  echo "FAIL: mark undone"
  exit 1
}
grep -q 'List "Personal" created' "$OUT" || {
  echo "FAIL: second list"
  exit 1
}
grep -q 'Active list is now "Work"' "$OUT" || {
  echo "FAIL: switch list"
  exit 1
}
grep -q "Task added to list \"Work\"" "$OUT" || {
  echo "FAIL: add while on other list (target Work)"
  exit 1
}

echo "extra_test.sh: all checks passed."
