#!/usr/bin/env bash
set -euo pipefail
ROOT=/mnt/c/Users/adamc/Desktop/workspace/DH_sc
OUT="$ROOT/.local-inputs/character-menu-native-v1-host"
SNAP="$OUT/snapshot"
SOURCES=("$ROOT/port/engine-ui/character_menu_stats_owner_v1.cpp" "$ROOT/port/engine-ui/character_menu_actions_owner_v1.cpp" "$ROOT/port/engine-ui/character_menu_queries_owner_v1.cpp" "$ROOT/port/engine-ui/character_menu_font_palette_v1.cpp" "$ROOT/port/engine-ui/character_menu_inventory_order_v1.cpp" "$ROOT/port/engine-ui/tests/character_menu_composite_v1.cpp")
LIBS=(-L"$SNAP" -Wl,-rpath,"$SNAP" -ldh2_level_world -ldh2_game_data -ldh2_engine_ui -ldh2_script_runtime -ldh2_engine_skinning -ldh2_engine_animation -ldh2_scene_materials -ldh2_zip_asset_pack_v1 -lz)
SOURCES+=("$ROOT/port/engine-ui/character_menu_inventory_mutation_v1.cpp")
SOURCES+=("$ROOT/port/engine-ui/character_menu_item_actions_v1.cpp")
SOURCES+=("$ROOT/port/engine-ui/character_menu_save_actions_v1.cpp" "$ROOT/port/engine-ui/character_menu_faery_actions_v1.cpp")
SOURCES+=("$ROOT/port/game-data/player_save_load_owner_v1.cpp" "$ROOT/port/game-data/player_save_named_writer_v1.cpp")
export LD_LIBRARY_PATH="$SNAP"
cd "$ROOT"
for mode in SAN O2; do
 FLAGS=(-std=c++17 -g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-fast-math -ffp-contract=off)
 if [[ "$mode" == SAN ]]; then FLAGS+=(-O1); else FLAGS+=(-O2); fi
 g++ "${FLAGS[@]}" "${SOURCES[@]}" "${LIBS[@]}" -o "$OUT/menu-$mode"
 ASAN_OPTIONS=detect_leaks=1 "$OUT/menu-$mode" "$ROOT" /mnt/c/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip
done
