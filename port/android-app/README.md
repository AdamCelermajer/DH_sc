# Android 17 source renderer milestone

This is a new Android APK built from the repository's checked C++ BRES, mesh, material and PVRTC readers. It uses a small Java file-picker UI and a new OpenGL ES 2.0 shader to draw the first textured triangle primitive of an owner-supplied BRES file. The native libraries are built for `arm64-v8a` and `x86_64`, and the manifest targets API 37. The APK contains no original game code, game assets, cache, or proxy engine.

From the repository root, with JDK, Android SDK platform 37, build-tools 35, and NDK r29 installed:

```powershell
python port/android-app/build.py --sdk PATH_TO_ANDROID_SDK --ndk PATH_TO_NDK_R29
PATH_TO_ANDROID_SDK/platform-tools/adb install -r port/android-app/build/dh2-source-renderer-debug.apk
```

Open **DH2 Source Renderer**. Tap **Import BRES mesh** and select `data/3d/animateddecors/candle_flame.bdae` from a private copy of the supplied cache. The app resolves the first primitive's material diffuse image to `env_crypt.tga`. Tap **Import PVRTC texture** and select `data/3d/textures/env_crypt.tga`. Android's file picker gives the app access only to those selected files; it does not require broad storage permission. The source files are read into bounded memory, and the decoded image and mesh are not persisted by the app.

`build.py` compiles both ABIs and checks every native `PT_LOAD` segment for 16 KiB alignment. It also checks APK ZIP alignment and signature. The output APK and local debug signing key stay in ignored `build/`. The tracked [`build-validation.json`](build-validation.json) records binary hashes; [`runtime-validation.json`](runtime-validation.json) records the separate emulator checks. The user-facing APK and screenshot are supplied outside Git.

The exact exported APK was installed on the Android 17/API 37.2 x86_64 16 KiB page emulator. It loaded a real 16,856-byte candle BRES (4 vertices, 6 indices), decoded the matching 1024×1024 PVRTC texture, and displayed the textured mesh without a process crash. The same rendering path also passed on a 4 KiB Android 17 emulator using an earlier APK whose DEX and native libraries are byte-identical; only manifest version fields changed afterward. The ARM64 build was checked structurally but was not run on an ARM64 device or emulator.

This app is an asset renderer milestone. Its mesh normalization, shader, camera, texture sampling, and UI are new diagnostic choices. It does not include the original game engine, level streaming, animation, combat, input controls, audio, saved-game handling, or gameplay. Importing a BRES and a texture does not verify that the chosen files belong together; use the filename returned by the material link. The original rights situation remains described in [`RIGHTS.md`](../../RIGHTS.md).
