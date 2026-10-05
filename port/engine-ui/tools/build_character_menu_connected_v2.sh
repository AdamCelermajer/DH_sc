#!/usr/bin/env bash
set -euo pipefail
ROOT=/mnt/c/Users/adamc/Desktop/workspace/DH_sc
OUT="$ROOT/.local-inputs/character-menu-connected-v2-host"
SNAP="$ROOT/.local-inputs/character-menu-native-v1-host/snapshot"
mkdir -p "$OUT"
SOURCES=("$ROOT/port/engine-ui/character_menu_stats_owner_v1.cpp" "$ROOT/port/engine-ui/character_menu_actions_owner_v1.cpp" "$ROOT/port/engine-ui/character_menu_queries_owner_v1.cpp" "$ROOT/port/engine-ui/character_menu_font_palette_v1.cpp" "$ROOT/port/engine-ui/character_menu_inventory_order_v1.cpp" "$ROOT/port/engine-ui/character_menu_inventory_mutation_v1.cpp" "$ROOT/port/engine-ui/character_menu_as_bridge_v1.cpp" "$ROOT/port/engine-ui/tests/character_menu_connected_v2.cpp")
LIBS=(-L"$SNAP" -Wl,-rpath,"$SNAP" -ldh2_level_world -ldh2_game_data -ldh2_engine_ui -ldh2_script_runtime -ldh2_engine_skinning -ldh2_engine_animation -ldh2_scene_materials -ldh2_zip_asset_pack_v1 -lz)
SOURCES+=("$ROOT/port/engine-ui/character_menu_item_actions_v1.cpp" "$ROOT/port/engine-ui/character_menu_save_actions_v1.cpp" "$ROOT/port/engine-ui/character_menu_faery_actions_v1.cpp")
export LD_LIBRARY_PATH="$SNAP"
cd "$ROOT"
python3 "$ROOT/port/engine-ui/tools/audit_character_menu_connected_v2.py" --capture-sources
for MODE in SAN O2; do
 FLAGS=(-std=c++17 -g -fsanitize=address,undefined -fno-sanitize=vptr -fno-omit-frame-pointer -fno-fast-math -ffp-contract=off -isystem "$ROOT/port/engine-ui/vendor/gameswf1714" -DTU_CONFIG_LINK_TO_JPEGLIB=0 -DTU_CONFIG_LINK_TO_LIBPNG=0 -DTU_CONFIG_LINK_TO_FREETYPE=0 -DTU_CONFIG_LINK_TO_THREAD=0)
 if [[ "$MODE" == SAN ]]; then FLAGS+=(-O1); else FLAGS+=(-O2); fi
 g++ "${FLAGS[@]}" "${SOURCES[@]}" "${LIBS[@]}" -o "$OUT/menu-$MODE"
 ASAN_OPTIONS=detect_leaks=1:abort_on_error=1 UBSAN_OPTIONS=halt_on_error=1 "$OUT/menu-$MODE" "$ROOT" /mnt/c/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip | tee "$OUT/result-$MODE.json"
done
