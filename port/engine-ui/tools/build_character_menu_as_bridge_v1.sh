#!/usr/bin/env bash
set -euo pipefail
ROOT=/mnt/c/Users/adamc/Desktop/workspace/DH_sc
OUT="$ROOT/.local-inputs/character-menu-as-bridge-v1-host"
SOURCE="$ROOT/.local-inputs/character-menu-native-v1-host/snapshot"
SNAP="$OUT/snapshot"
mkdir -p "$OUT"
if [[ ! -d "$SNAP" ]]; then mkdir "$SNAP"; cp "$SOURCE"/*.so "$SNAP/"; fi
export LD_LIBRARY_PATH="$SNAP"
LIBS=(-L"$SNAP" -Wl,-rpath,"$SNAP" -ldh2_level_world -ldh2_game_data -ldh2_engine_ui -ldh2_script_runtime -ldh2_engine_skinning -ldh2_engine_animation -ldh2_scene_materials -lz)
cd "$ROOT"
for MODE in SAN O2; do
 FLAGS=(-std=c++17 -g -Wall -Wextra -Werror -fsanitize=address,undefined -fno-sanitize=vptr -fno-omit-frame-pointer -fno-fast-math -ffp-contract=off -isystem "$ROOT/port/engine-ui/vendor/gameswf1714" -DTU_CONFIG_LINK_TO_JPEGLIB=0 -DTU_CONFIG_LINK_TO_LIBPNG=0 -DTU_CONFIG_LINK_TO_FREETYPE=0 -DTU_CONFIG_LINK_TO_THREAD=0)
 if [[ "$MODE" == SAN ]]; then FLAGS+=(-O1); else FLAGS+=(-O2); fi
 g++ "${FLAGS[@]}" "$ROOT/port/engine-ui/character_menu_as_bridge_v1.cpp" "$ROOT/port/engine-ui/tests/character_menu_as_bridge_v1.cpp" "${LIBS[@]}" -o "$OUT/bridge-$MODE"
 ASAN_OPTIONS=detect_leaks=1 "$OUT/bridge-$MODE" "$ROOT/port/android-native/app/src/main/assets/original-cache/data/menus" | tee "$OUT/result-$MODE.json"
done
