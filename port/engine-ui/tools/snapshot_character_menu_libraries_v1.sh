#!/usr/bin/env bash
set -euo pipefail
OUT=/mnt/c/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/character-menu-native-v1-host/snapshot
BASE=/home/adampalace/dh2-world-build
mkdir -p "$OUT"
for rel in libdh2_level_world.so engine-skinning/libdh2_engine_skinning.so script-runtime/libdh2_script_runtime.so engine-ui/libdh2_engine_ui.so engine-skinning/engine-animation/libdh2_engine_animation.so engine-skinning/engine-animation/scene-materials/libdh2_scene_materials.so game-data/libdh2_game_data.so libdh2_zip_asset_pack_v1.a; do
 cp "$BASE/$rel" "$OUT/$(basename "$rel")"
done
find "$OUT" -maxdepth 1 -type f ! -name SHA256SUMS -print0 | sort -z | xargs -0 sha256sum > "$OUT/SHA256SUMS"
