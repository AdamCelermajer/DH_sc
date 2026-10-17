# Adam core source import

This source import is based on Adam Celermajer’s public `DH_sc` repository at commit `45c5348e807607a2825211bb8f26248067ba9106` (2026-10-04). The reference checkout used for the copy is `work/DH_sc-adam-audit`.

## Scope

Imported modules: `engine-textures`, `scene-materials`, `engine-animation`, `engine-skinning`, `game-data`, `physics-backend`, `level-world`, and Adam’s separate `script-runtime` under `adam-script-runtime`. The source-runtime import contains 158 tracked source/license/build files (1,064,065 bytes), copied byte-for-byte; 65 tracked research/report/binary/non-source files were excluded. The other modules contain C/C++ sources and headers, source test/tool scripts, top-level module READMEs, root `original-functions.json` maps, and the Box2D 2.0.1 source and license/readme files. Adam’s Android app and bundled assets are recorded as a separate project import below.

Generated reports, reverse-engineered reference dumps/corpora, binary fixtures, compiled objects/libraries/APKs, local cache inputs, and bundled game assets were excluded from the core module import. Separately, Adam’s tracked `port/android-native` project was imported with its bundled app assets intact. The initial import added a copyright footer; the subsequent continuation adds a validated mod asset loader and attack-status synchronization. The project remains Adam’s custom GLES2 renderer and is currently a runnable reference baseline, not the Irrlicht renderer. Build/gameplay evidence and the combined roadmap are recorded in `docs/COMBINED-RECONSTRUCTION-STATUS.md` and `docs/ADAM-WORK-COMPARISON.md`.

The shared `port/asset-payloads`, `port/engine-math`, and `port/engine-resources` modules are byte-identical to Adam’s versions and already exist in this branch’s base; they remain unmodified. This repository’s `port/script-runtime` was left untouched. `port/level-world/CMakeLists.txt` and `port/level-world/character_script_timers.hpp` now point to the copied `port/adam-script-runtime`, keeping Adam’s `dh2_script_runtime` target and header isolated from the pre-existing runtime. These two relative-path adaptations are the only changes made to the imported level-world module. The Android NativeActivity/renderer remains a separate integration concern.

This import attributes the work to its source repository and commit; it does not assert a blanket license for recovered game code, binary-derived data, or original assets. See this checkout’s `RIGHTS.md` and the source repository’s `RIGHTS.md`.
