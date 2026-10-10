#!/usr/bin/env bash
set -euo pipefail
ROOT=/mnt/c/Users/adamc/Desktop/workspace/DH_sc
OUT="$ROOT/.local-inputs/character-menu-native-v1-host"
mkdir -p "$OUT"
g++ -std=c++17 -O2 -fno-fast-math -ffp-contract=off -c "$ROOT/port/engine-ui/character_menu_actions_owner_v1.cpp" -o "$OUT/actions-O2.o"
g++ -std=c++17 -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-fast-math -ffp-contract=off \
 "$ROOT/port/engine-ui/character_menu_stats_owner_v1.cpp" \
 "$ROOT/port/engine-ui/tests/character_menu_stats_owner_v1.cpp" \
 "$ROOT/port/game-data/data.cpp" "$ROOT/port/game-data/properties.cpp" "$ROOT/port/game-data/class_tables.cpp" \
 -o "$OUT/stats-sanitized"
ASAN_OPTIONS=detect_leaks=1 "$OUT/stats-sanitized" "$ROOT/port/android-native/app/src/main/assets/data" "$ROOT/port/engine-ui/reference/character-menu-native-v1/stat-full-gold-v1.bin"
g++ -std=c++17 -O2 -fno-fast-math -ffp-contract=off \
 "$ROOT/port/engine-ui/character_menu_stats_owner_v1.cpp" \
 "$ROOT/port/engine-ui/tests/character_menu_stats_owner_v1.cpp" \
 "$ROOT/port/game-data/data.cpp" "$ROOT/port/game-data/properties.cpp" "$ROOT/port/game-data/class_tables.cpp" \
 -o "$OUT/stats-O2"
"$OUT/stats-O2" "$ROOT/port/android-native/app/src/main/assets/data" "$ROOT/port/engine-ui/reference/character-menu-native-v1/stat-full-gold-v1.bin"
