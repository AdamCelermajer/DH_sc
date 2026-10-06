#!/usr/bin/env bash
set -euo pipefail
ROOT=/mnt/c/Users/adamc/Desktop/workspace/DH_sc
OUT="$ROOT/.local-inputs/authored-character-panel-v2-host"
SNAP="$ROOT/.local-inputs/character-menu-native-v1-host/snapshot"
mkdir -p "$OUT"
export LD_LIBRARY_PATH="$SNAP"
cd "$ROOT"
for MODE in SAN O2; do
 FLAGS=(-std=c++17 -g -Wall -Wextra -Werror -Wno-misleading-indentation -fsanitize=address,undefined -fno-sanitize=vptr -fno-omit-frame-pointer -fno-fast-math -ffp-contract=off -isystem "$ROOT/port/engine-ui/overlays/edit-text-v1" -I"$ROOT/port/engine-ui" -isystem "$ROOT/port/engine-ui/vendor/gameswf1714" -DTU_CONFIG_LINK_TO_JPEGLIB=0 -DTU_CONFIG_LINK_TO_LIBPNG=0 -DTU_CONFIG_LINK_TO_FREETYPE=0 -DTU_CONFIG_LINK_TO_THREAD=0)
 if [[ "$MODE" == SAN ]]; then FLAGS+=(-O1); else FLAGS+=(-O2); fi
 SOURCES=(authored_character_panel_v2 character_menu_movie_v1 character_menu_as_bridge_v1 authored_character_menu_bridge_v1 authored_character_menu_session_v1 authored_menu_lifecycle_v1 authored_menu_localization_v1 menu_stack_owner_v1 menu_stack_actions_v1 menu_stack_v1)
 FILES=();for SOURCE in "${SOURCES[@]}"; do FILES+=("$ROOT/port/engine-ui/$SOURCE.cpp");done
 g++ "${FLAGS[@]}" "${FILES[@]}" "$ROOT/port/engine-ui/tests/authored_character_panel_v2.cpp" -L"$SNAP" -Wl,-rpath,"$SNAP" -ldh2_level_world -ldh2_game_data -ldh2_engine_ui -ldh2_script_runtime -ldh2_engine_skinning -ldh2_engine_animation -ldh2_scene_materials -lz -o "$OUT/panel-$MODE"
 ASAN_OPTIONS=detect_leaks=1:abort_on_error=1 UBSAN_OPTIONS=halt_on_error=1 "$OUT/panel-$MODE" "$ROOT/port/android-native/app/src/main/assets/original-cache" 2>"$OUT/diagnostics-$MODE.txt" | tee "$OUT/result-$MODE.json"
done
