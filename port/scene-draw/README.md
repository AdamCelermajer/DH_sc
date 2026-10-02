# Static scene draw descriptors

This C++17 module joins the checked BRES scene hierarchy, world matrices, geometry views, triangle primitives and material IDs into renderer-neutral static draw descriptors. A caller opens a BRES once, then calls `dh2_static_scene_draws` with node and draw budgets. Each callback receives geometry/primitive/material indices, original node visibility, vertex/index counts and the node's column-major local-to-world matrix. The callback can reopen the checked mesh and material views to prepare a GPU draw. Strings and indices borrow the original BRES; the callback must copy anything it retains beyond the walk.

The bridge follows type-6 local visual-scene references and type-3 local geometry instances. It records unresolved geometry links, unsupported type-1 geometry, nontriangle or empty primitives, and unmatched material IDs. It emits hidden nodes with their raw visibility flag so a renderer can choose its own visibility policy. The bridge does not guess texture samplers, effect programs, coordinate conversion, animation, skinning or scene composition across multiple files. It is new port code, not a recovered original draw routine.

## Verification

The full private-cache audit visited all 2,904 BRES files. It walked 1,563 visual scenes and 21,472 nodes, resolved 10,403 of 10,444 geometry instances, and emitted **11,311 draw descriptors covering 995,715 triangles**. Of these, 9,912 commands have a nonzero world translation. It recorded 41 unresolved geometry links, nine unsupported geometry records and 451 unmatched material IDs. Bounds, stop-callback and input-immutability checks passed. The [validation report](validation.json) has counts and small metadata samples; no game asset bytes are committed.

The module builds as a host DLL and Android x86_64/ARM64 libraries using NDK r29. All Android load segments are 16 KiB aligned ([build report](build-validation.json)). An x86_64 command-line executable ran on the Android 17/API 37 emulator with 16 KiB pages, read the owner's private `candle_flame.bdae`, and returned two draw commands/four triangles across seven nodes with exit code zero. The on-device BRES and executable hashes matched the local inputs ([runtime report](runtime-validation.json)). ARM64 code was cross-built but has not run on an ARM64 device or emulator.

From the repository root:

```text
python port/scene-draw/build.py --ndk PATH_TO_NDK_R29 \
  --sample PATH_TO_CACHE/files/data/3d/animateddecors/candle_flame.bdae
python port/scene-draw/tests/audit.py --cache PATH_TO_CACHE/files \
  --library port/scene-draw/build/libdh2_scene_draw_host.dll \
  --report port/scene-draw/validation.json
python port/scene-draw/tests/run_android.py --adb PATH_TO_ADB \
  --serial EMULATOR_SERIAL \
  --sample PATH_TO_CACHE/files/data/3d/animateddecors/candle_flame.bdae
```

Use `.so` for the host library on Linux. Generated binaries and private BRES files stay outside Git. This module emits checked static descriptors but does not itself draw pixels or run the game. The separate Android source renderer now consumes the descriptors for bounded static scene previews, including a 77-command cached maze sample. It does not run gameplay.
