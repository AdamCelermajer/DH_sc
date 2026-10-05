#!/usr/bin/env bash
set -euo pipefail
ROOT=/mnt/c/Users/adamc/Desktop/workspace/DH_sc
OUT="$ROOT/.local-inputs/character-menu-reload-action-v1-host"
mkdir -p "$OUT"
for MODE in SAN O2; do
 FLAGS=(-std=c++17 -g -Wall -Wextra -Werror -fsanitize=address,undefined -fno-omit-frame-pointer)
 if [[ "$MODE" == SAN ]]; then FLAGS+=(-O1); else FLAGS+=(-O2); fi
 g++ "${FLAGS[@]}" "$ROOT/port/engine-ui/character_menu_reload_action_v1.cpp" "$ROOT/port/engine-ui/character_menu_reload_v1.cpp" "$ROOT/port/engine-ui/tests/character_menu_reload_action_v1.cpp" -o "$OUT/reload-action-$MODE"
 ASAN_OPTIONS=detect_leaks=1:abort_on_error=1 UBSAN_OPTIONS=halt_on_error=1 "$OUT/reload-action-$MODE" "$ROOT/port/engine-ui/reference/character-menu-flow-v1/reload-action-gold-v1.bin" | tee "$OUT/result-$MODE.json"
done
