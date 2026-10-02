# Android 17 source renderer milestone

## Character animation preview milestone

Import `data/3d/characters/prince/prince_low_poly_warrior.bdae`,
`data/3d/textures/prince-warrior.tga`, then
`data/3d/characters/prince/animations/prince_walk_1hand.bdae` from your own
cache. **Play animation**, **Pause animation** and the time slider show a
27-track, 799 ms walk preview on the 18-bone warrior. Every animation target
must resolve once in the imported model; mismatched clips are rejected.

The exact 221,728-byte APK, SHA-256
`183b488b1c6b6071920c71d4600e44ef70a3976ac0031957e080e189c6f88a37`,
was installed and pulled back with matching hashes on Android 17 x86_64
emulators with both 4 KiB and 16 KiB pages. Import, midpoint seek, Play and
Pause passed, with visibly different textured leg/arm poses and no fatal
error during each run. [Runtime evidence](animation-runtime-validation.json)
records this build. [The pose component](../animation-pose/README.md)
documents sampling and safety checks.

This previews absolute stored keys. Original default-relative blending,
animation transitions, root motion and gameplay are unfinished. Each frame
uses diagnostic bounds normalization, one texture and one resolved skin
controller. ARM64 libraries are structurally checked but have not run on
ARM64 hardware. Selected input files are bounded to 32 MiB each.

## Earlier character pose milestone

The current build adds a checked software-skinning fallback when a BRES has
no static scene draws. Import
`data/3d/characters/prince/prince_low_poly_warrior.bdae`, then
`data/3d/textures/prince-warrior.tga`. It resolves 18 bone scope IDs and
renders 335 vertices/1,092 indices. Drag rotates the textured pose. The
diagnostic camera maps character Z-up coordinates to the display's Y-up
basis; recovered data stays unchanged.

The exact 213,536-byte APK, SHA-256
`664595b19de049d9a745d6e3b8f629ff1d65c9121abf927de895f6ad4be8fdb8`,
was installed and pulled back with matching hashes on both Android 17 page
sizes. Both previews visibly showed the warrior and changed angle after a
drag. [The runtime record](character-runtime-validation.json) records this
check. [Skin payloads](../skin-payloads/README.md) documents the parser,
original ARM palette comparison and safety checks.

This fallback selects one locally resolved controller. It does not assemble
modular equipment, skin normals or provide gameplay. The
older scene runtime records below belong to earlier APKs.

This is a new Android APK built from the repository's checked C++ BRES, scene, mesh, material and PVRTC readers. It uses a small Java file-picker UI and an OpenGL ES 2.0 shader to draw checked static triangle commands from an owner-supplied BRES file. It walks the scene hierarchy, applies each command's world transform, retains all three world coordinates, and combines the triangles into one bounded diagnostic draw. The preview has an oblique 3D camera, depth buffer, and touch rotation. The native libraries are built for `arm64-v8a` and `x86_64`, and the manifest targets API 37. The APK contains no original game code, game assets, cache, or proxy engine.

From the repository root, with JDK, Android SDK platform 37, build-tools 35, and NDK r29 installed:

```powershell
python port/android-app/build.py --sdk PATH_TO_ANDROID_SDK --ndk PATH_TO_NDK_R29
PATH_TO_ANDROID_SDK/platform-tools/adb install -r port/android-app/build/dh2-source-renderer-debug.apk
```

Open **DH2 Source Renderer**. Tap **Import BRES scene** and select `data/3d/modules/void_maze/void_maze.bdae` from a private copy of the supplied cache. This larger sample yields 77 static commands, 3,140 vertices, and 4,695 indices; its first resolved material diffuse image is `env_voidmaze.tga`. Tap **Import PVRTC texture** and select `data/3d/textures/env_voidmaze.tga`, then drag the preview to rotate the view. The smaller `data/3d/animateddecors/candle_flame.bdae` plus `data/3d/textures/env_crypt.tga` sample also remains usable. Android's file picker gives the app access only to selected files; it does not require broad storage permission. The source files are read into bounded memory, and the decoded image and scene buffers are not persisted by the app.

`build.py` compiles both ABIs and checks every native `PT_LOAD` segment for 16 KiB alignment. It also checks APK ZIP alignment and signature. The output APK and local debug signing key stay in ignored `build/`. The tracked [`build-validation.json`](build-validation.json) records the current binary hashes; [`animation-runtime-validation.json`](animation-runtime-validation.json) records this build's emulator check. The earlier scene check is in [`scene-3d-runtime-validation.json`](scene-3d-runtime-validation.json). Earlier builds' evidence remains in [`scene-runtime-validation.json`](scene-runtime-validation.json) and [`runtime-validation.json`](runtime-validation.json). The APK and screenshots remain outside Git.

The earlier 3D scene APK was installed on Android 17 x86_64 emulators with both 4 KiB (API 37.0) and 16 KiB (API 37.2) pages. On each it loaded the real 199,300-byte void maze BRES, reported 77 draws/3,140 vertices/4,695 indices, decoded its 256×256 PVRTC texture, and visibly displayed textured stone geometry. A touch drag changed the 3D view angle, and the process remained alive. The candle sample also loaded and displayed on the 4 KiB emulator. ARM64 was checked structurally but has not run on an ARM64 device or emulator.

This app is an asset renderer milestone. Its coordinate normalization, orthographic camera, shader, texture sampling, and UI are new diagnostic choices. It applies one imported texture to every command, with no per-command material/shader state, transparency rules, or draw ordering reconstruction. A BRES with more than 256 static commands, 8,192 vertices, or 24,000 indices is rejected rather than partially drawn. It does not include the original game engine, level streaming, original animation state control, combat, input controls, audio, saved-game handling, or gameplay. Importing a BRES and a texture does not verify that the chosen files belong together; use the filename returned by the material link. The original rights situation remains described in [`RIGHTS.md`](../../RIGHTS.md).
