#!/usr/bin/env bash
# Compile-check single files the way the Track B build compiles them, without a full build.
#   tools/trackb/check_tu.sh port/decomp-reconstruction/game-world/src/Door.cpp
#   tools/trackb/check_tu.sh port/decomp-reconstruction/game/include/dh2/Door.hpp   # header on its own
#   tools/trackb/check_tu.sh --test port/decomp-reconstruction/game-world/tests/door_test.cpp src1.cpp ...
# A .hpp is checked by compiling a translation unit that includes only it (headers must be
# self-contained). --test links the test with the listed sources and runs it.
# Exit status 0 means every file compiled (and the test passed).
set -uo pipefail
root=$(cd "$(dirname "$0")/../.." && pwd)
dr=$root/port/decomp-reconstruction
cxx=${CXX:-g++}
flags=(-std=c++17 -Wall -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unknown-pragmas)
engine=(core-platform pydata-accessors net-transport net-session-online vox-audio tinyxml-and-c-libs glitch-core-io glitch-video glitch-scene-collada-gui)

includes_for() {  # category of a path -> -I flags
  local rel=${1#"$dr"/}; rel=${rel#port/decomp-reconstruction/}
  local cat=${rel%%/*}
  if [[ $cat == game || $cat == game-* ]]; then
    printf -- '-I%s\n' "$dr/game/include" "$dr/shared/include"
    for c in "${engine[@]}"; do printf -- '-I%s\n' "$dr/$c/include"; done
  else
    printf -- '-I%s\n' "$dr/$cat/include" "$dr/shared/include"
  fi
}

tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT
test_mode=0
if [[ ${1:-} == --test ]]; then test_mode=1; shift; fi
status=0
if ((test_mode)); then
  t=$1; shift
  mapfile -t inc < <(includes_for "$(cd "$(dirname "$t")" && pwd)/$(basename "$t")")
  if "$cxx" "${flags[@]}" "${inc[@]}" "$t" "$@" -o "$tmp/t.exe" -lws2_32 2>&1; then
    "$tmp/t.exe"; status=$?
    echo "test exit $status"
  else
    status=1
  fi
  exit $status
fi
for f in "$@"; do
  abs=$(cd "$(dirname "$f")" && pwd)/$(basename "$f")
  mapfile -t inc < <(includes_for "$abs")
  case $f in
    *.hpp|*.h) printf '#include "%s"\n' "$(cygpath -m "$abs" 2>/dev/null || echo "$abs")" > "$tmp/h.cpp"; src=$tmp/h.cpp ;;
    *) src=$abs ;;
  esac
  if "$cxx" "${flags[@]}" "${inc[@]}" -c "$src" -o "$tmp/o.o" 2>&1; then
    echo "OK   $f"
  else
    echo "FAIL $f"; status=1
  fi
done
exit $status
