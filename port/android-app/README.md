# Android 17 source renderer milestone

This is a new Android APK built from the repository's checked C++ BRES, scene, mesh, material and PVRTC readers. It uses a small Java file-picker UI and an OpenGL ES 2.0 shader to draw checked static triangle commands from an owner-supplied BRES file. It walks the scene hierarchy, applies each command's world transform, and combines the triangles into one bounded diagnostic draw. The native libraries are built for `arm64-v8a` and `x86_64`, and the manifest targets API 37. The APK contains no original game code, game assets, cache, or proxy engine.

From the repository root, with JDK, Android SDK platform 37, build-tools 35, and NDK r29 installed:

```powershell
python port/android-app/build.py --sdk PATH_TO_ANDROID_SDK --ndk PATH_TO_NDK_R29
PATH_TO_ANDROID_SDK/platform-tools/adb install -r port/android-app/build/dh2-source-renderer-debug.apk
```

Open **DH2 Source Renderer**. Tap **Import BRES scene** and select `data/3d/animateddecors/candle_flame.bdae` from a private copy of the supplied cache. This sample yields two static commands, eight vertices, and twelve indices; the first resolved material diffuse image is `env_crypt.tga`. Tap **Import PVRTC texture** and select `data/3d/textures/env_crypt.tga`. Android's file picker gives the app access only to those selected files; it does not require broad storage permission. The source files are read into bounded memory, and the decoded image and scene buffers are not persisted by the app.

`build.py` compiles both ABIs and checks every native `PT_LOAD` segment for 16 KiB alignment. It also checks APK ZIP alignment and signature. The output APK and local debug signing key stay in ignored `build/`. The tracked [`build-validation.json`](build-validation.json) records binary hashes; [`scene-runtime-validation.json`](scene-runtime-validation.json) records this scene build's emulator check. The earlier first-primitive build's checks remain in [`runtime-validation.json`](runtime-validation.json). The APK and screenshots remain outside Git.

The exact new scene APK was installed on the Android 17/API 37.0 x86_64 4 KiB page emulator. It loaded the real 16,856-byte candle BRES, reported two draws/eight vertices/twelve indices, decoded the matching 1024×1024 PVRTC texture, and visibly displayed the crypt background and candle flame together without a process crash. The new scene APK has not yet been run on the 16 KiB emulator or on ARM64; its native libraries passed structural 16 KiB alignment checks. The earlier first-primitive APK was separately run on both Android 17 page sizes.

This app is an asset renderer milestone. Its world X/Z projection, scene normalization, shader, camera, texture sampling, and UI are new diagnostic choices. It applies one imported texture to every command, with no per-command material/shader state or draw ordering reconstruction. A BRES with more than 256 static commands, 8,192 vertices, or 24,000 indices is rejected rather than partially drawn. It does not include the original game engine, level streaming, animation, combat, input controls, audio, saved-game handling, or gameplay. Importing a BRES and a texture does not verify that the chosen files belong together; use the filename returned by the material link. The original rights situation remains described in [`RIGHTS.md`](../../RIGHTS.md).
