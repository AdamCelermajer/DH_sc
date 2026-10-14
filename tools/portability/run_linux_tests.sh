#!/usr/bin/env bash
# Reproduces the Linux (non-MSVC) build and the full CTest run of port/windows-foundation.
#
# Why this script exists: the Windows build is the only fully tested target. This script builds
# every target (including all tests) on a case-sensitive file system, with the assets the tests
# name, and runs CTest. It is meant for WSL Ubuntu (or any Linux host) with cmake, ninja, SDL2
# and OpenGL development packages installed.
#
# Usage:
#   tools/portability/run_linux_tests.sh [--compiler gcc|clang] [--sanitize] [--jobs N]
#                                        [--work DIR] [--local-inputs DIR] [--ctest-args "ARGS"]
#                                        [--no-copy] [--build-only]
#
#   --compiler gcc|clang   host compiler (default gcc; clang must be installed as clang++)
#   --sanitize             build with -fsanitize=address,undefined (ASan + UBSan)
#   --jobs N               build/ctest parallelism (default: nproc)
#   --work DIR             ext4 work directory (default: $HOME/dh-portability/<compiler>[-asan])
#   --local-inputs DIR     directory holding the .local-inputs data (default: the repo's own
#                          .local-inputs, which in a git worktree is made of junctions to the
#                          primary checkout's subdirectories)
#   --ctest-args "ARGS"    extra arguments for ctest (e.g. "-R path_case")
#   --no-copy              reuse the existing work tree without re-mirroring sources/assets
#   --build-only           stop after the build
#
# The work directory must be on a case-sensitive file system. /mnt/* (drvfs) is refused,
# because it hides case-mismatch bugs in asset and include paths.
#
# Tests that are Windows-only are not built on Linux (see port/windows-foundation/CMakeLists.txt,
# e.g. private_save_transport_win32, window_probe, renderer_visual_quality, winmm_pump_priority).
# Those are reported by CMake at configure time; they are never silently passing.
set -euo pipefail

COMPILER=gcc
SANITIZE=0
JOBS=""
WORK=""
LOCAL_INPUTS=""
CTEST_EXTRA=""
COPY=1
BUILD_ONLY=0

while [ $# -gt 0 ]; do
  case "$1" in
    --compiler) COMPILER="$2"; shift 2 ;;
    --sanitize) SANITIZE=1; shift ;;
    --jobs) JOBS="$2"; shift 2 ;;
    --work) WORK="$2"; shift 2 ;;
    --local-inputs) LOCAL_INPUTS="$2"; shift 2 ;;
    --ctest-args) CTEST_EXTRA="$2"; shift 2 ;;
    --no-copy) COPY=0; shift ;;
    --build-only) BUILD_ONLY=1; shift ;;
    -h|--help) sed -n '2,25p' "$0"; exit 0 ;;
    *) echo "unknown option: $1" >&2; exit 2 ;;
  esac
done

case "$COMPILER" in
  gcc) CC_BIN=gcc; CXX_BIN=g++ ;;
  clang) CC_BIN=clang; CXX_BIN=clang++ ;;
  *) echo "--compiler must be gcc or clang" >&2; exit 2 ;;
esac

REPO="$(cd "$(dirname "$0")/../.." && pwd)"
SRC_PORT="$REPO/port/windows-foundation"
[ -f "$SRC_PORT/CMakeLists.txt" ] || { echo "not a repo checkout: $SRC_PORT" >&2; exit 1; }
LOCAL_INPUTS="${LOCAL_INPUTS:-$REPO/.local-inputs}"
JOBS="${JOBS:-$(nproc)}"
SUFFIX="$COMPILER"; [ "$SANITIZE" = 1 ] && SUFFIX="$COMPILER-asan"
WORK="${WORK:-$HOME/dh-portability/$SUFFIX}"

case "$(readlink -f "$WORK")" in
  /mnt/*) echo "refusing a drvfs work directory ($WORK): use an ext4 path such as \$HOME" >&2; exit 1 ;;
esac

MIRROR="$WORK/repo"
BUILD="$WORK/build"
LOG="$WORK/logs"
mkdir -p "$MIRROR" "$LOG"

# Assets: the .local-inputs trees named by add_test() arguments and by the runtime path literals of
# the tests (found with: grep -rhoE 'local-inputs/[A-Za-z0-9._-]+' port/windows-foundation).
# Whole trees are copied; the three very large trees are copied only where the tests need them.
# Keep this list in sync with CMakeLists.txt when a test adds a new asset root.
ASSET_DIRS=(
  actors assets-extra character-skill-session-v2 design-settings player-loot-v7
  runtime-session-projectile-v1 windows-encounter-source windows-main-frontend-v1 windows-population-assets
  windows-shared-assets windows-melee-bindings windows-equipment-assets windows-camera-assets
  windows-source-floor-fixture
  windows-source-clock-v19-preview-9 windows-source-clock-v19-preview-12 windows-source-clock-v19-preview-14-rc1
  windows-source-clock-v19-preview-15-rc3 windows-source-clock-v20 windows-source-clock-v21.6
  windows-source-clock-v21.8-staged windows-source-clock-v21.8-staged3
)
ASSET_SUBPATHS=(
  claude-preview15/startup/converted-media-v2
  claude-preview16/levels-root
  claude-preview16/map/assets-darkwood
  v20/asset-stage
)

if [ "$COPY" = 1 ]; then
  echo "[mirror] sources -> $MIRROR"
  rsync -a --delete --exclude .git --exclude .local-inputs --exclude .claude --exclude '*.zip' "$REPO"/ "$MIRROR"/
  mkdir -p "$MIRROR/.local-inputs"
  for d in "${ASSET_DIRS[@]}"; do
    [ -d "$LOCAL_INPUTS/$d" ] || { echo "[assets] missing $LOCAL_INPUTS/$d" >&2; exit 1; }
    echo "[assets] $d"
    rsync -rL "$LOCAL_INPUTS/$d" "$MIRROR/.local-inputs/"
  done
  for p in "${ASSET_SUBPATHS[@]}"; do
    [ -d "$LOCAL_INPUTS/$p" ] || { echo "[assets] missing $LOCAL_INPUTS/$p" >&2; exit 1; }
    echo "[assets] $p"
    mkdir -p "$MIRROR/.local-inputs/$p"
    rsync -rL "$LOCAL_INPUTS/$p/" "$MIRROR/.local-inputs/$p/"
  done
fi

CMAKE_EXTRA=()
if [ "$SANITIZE" = 1 ]; then
  SAN="-fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=undefined -g"
  CMAKE_EXTRA+=("-DCMAKE_C_FLAGS=$SAN" "-DCMAKE_CXX_FLAGS=$SAN" "-DCMAKE_EXE_LINKER_FLAGS=$SAN")
fi

echo "[configure] $BUILD ($CXX_BIN, sanitize=$SANITIZE)"
cmake -S "$MIRROR/port/windows-foundation" -B "$BUILD" -G Ninja -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_COMPILER="$CC_BIN" -DCMAKE_CXX_COMPILER="$CXX_BIN" \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON "${CMAKE_EXTRA[@]}" > "$LOG/configure.log" 2>&1 \
  || { tail -40 "$LOG/configure.log"; exit 1; }

echo "[build] ninja -k 0 -j$JOBS (log: $LOG/build.log)"
set +e
ninja -C "$BUILD" -k 0 -j"$JOBS" > "$LOG/build.log" 2>&1
BUILD_RC=$?
set -e
if [ "$BUILD_RC" != 0 ]; then
  echo "[build] FAILED steps:"; grep "^FAILED" "$LOG/build.log" | head -50
  exit "$BUILD_RC"
fi
[ "$BUILD_ONLY" = 1 ] && { echo "[build] OK"; exit 0; }

echo "[ctest] -j$JOBS (log: $LOG/ctest.log)"
set +e
ctest --test-dir "$BUILD" -j"$JOBS" --timeout 900 --output-on-failure $CTEST_EXTRA > "$LOG/ctest.log" 2>&1
CTEST_RC=$?
set -e
tail -15 "$LOG/ctest.log"
grep -E "\*\*\*(Failed|Timeout|Exception|Not Run)" "$LOG/ctest.log" || true
exit "$CTEST_RC"
