#!/usr/bin/env bash
# Automated smoke test: drives the scheduler with a heredoc and checks for expected substrings.
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"

if [[ ! -x ./scheduler ]]; then
  echo "Build ./scheduler first (run make in project root)."
  exit 1
fi

OUT="$(mktemp)"
trap 'rm -f "$OUT"' EXIT

./scheduler <<'EOF' | tee "$OUT"
2026-04-06 10:00
7
Home
1
Buy milk
2026-04-08

2
1
11
current
5
1
Remember organic
6
1
10
1
Strawberries

11
current
12
1
0
EOF

grep -q "Session started at: 2026-04-06 10:00" "$OUT" || {
  echo "FAIL: session date not shown"
  exit 1
}
grep -q 'List "Home" created' "$OUT" || {
  echo "FAIL: list creation"
  exit 1
}
grep -q "Task added to list \"Home\"" "$OUT" || {
  echo "FAIL: add task"
  exit 1
}
grep -q "marked as done" "$OUT" || {
  echo "FAIL: mark done"
  exit 1
}
grep -q "Note added" "$OUT" || {
  echo "FAIL: add note"
  exit 1
}
grep -q "starred" "$OUT" || {
  echo "FAIL: star"
  exit 1
}
grep -q "Subtask" "$OUT" || {
  echo "FAIL: subtask"
  exit 1
}
grep -q "removed from" "$OUT" || {
  echo "FAIL: delete task"
  exit 1
}
grep -q "Goodbye" "$OUT" || {
  echo "FAIL: quit"
  exit 1
}

echo "smoke_test.sh: all checks passed."
