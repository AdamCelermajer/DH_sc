#!/usr/bin/env bash
set -euo pipefail
ROOT=/mnt/c/Users/adamc/Desktop/workspace/DH_sc
OUT="$ROOT/.local-inputs/player-save-write-owner-v1-host"
mkdir -p "$OUT"
for MODE in SAN O2; do
 FLAGS=(-std=c++17 -g -Wall -Wextra -Werror -Wno-misleading-indentation -ffunction-sections -fdata-sections -Wl,--gc-sections -fsanitize=address,undefined -fno-omit-frame-pointer)
 if [[ "$MODE" == SAN ]]; then FLAGS+=(-O1); else FLAGS+=(-O2); fi
 g++ "${FLAGS[@]}" "$ROOT/port/game-data/skill_tables.cpp" "$ROOT/port/game-data/player_savegame_v1.cpp" "$ROOT/port/game-data/player_save_load_owner_v1.cpp" "$ROOT/port/game-data/player_save_write_owner_v1.cpp" "$ROOT/port/game-data/player_save_named_writer_v1.cpp" "$ROOT/port/game-data/tests/player_save_write_owner_v1.cpp" -o "$OUT/write-$MODE"
 ASAN_OPTIONS=detect_leaks=1:abort_on_error=1 UBSAN_OPTIONS=halt_on_error=1 "$OUT/write-$MODE" "$ROOT/port/engine-ui/reference/character-menu-native-v1/save-sections-gold-v1.bin" | tee "$OUT/result-$MODE.json"
done
