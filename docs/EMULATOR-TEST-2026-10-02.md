# Android emulator test — 2026-10-02

This is an emulator-only development test on an x86-64 Windows PC. No phone was used. The supplied cache ZIP (`3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`) was CRC-checked and its 6,833 files were copied into each disposable AVD's app-specific files directory. The Prince model in the Android 9 AVD matched the source SHA-256 `7e998e60bdfcbc9237c9867bd8de146f6d59bc9f8c9c028cd4d2391f460f0b3e`.

## Development toolkit

The official Android SDK command-line tools, platform tools, build tools 35.0.0, emulator 37.2.12, Android 9 Google APIs x86 system image revision 12, Android 11 Google APIs x86_64 image, and NDK r29 (`29.0.14206865`) were installed locally. The Android 9 AVD boots and reports `x86,armeabi-v7a,armeabi` plus `libndk_translation.so`. This is the same official Android Emulator used by Android Studio, managed here through the SDK commands. The tools, system images, cache, APKs, screenshots and raw logs are local test inputs/results and are not committed to Git.

The separate Android 11 x86_64 AVD can run the Test 5 launcher but its x86_64 process cannot load the wrapper's AArch64 proxy (`EM_AARCH64` versus `EM_X86_64`). That check is documented in [LOCAL-TEST-2026-10-02.md](LOCAL-TEST-2026-10-02.md). The Android 9 test below directly runs a signed ARM32 *guest APK* inside the x86 AVD's native bridge. It does not exercise the released ARM64 ZettaBridge host.

## Original APK baseline

The exact supplied original APK (`32c2d027b585a42547311cd95da6a3975fdb3174e663e513d42a7f49d1a4c200`) installed on the Android 9 AVD. With the full cache copied in, the original ARM32 game passed initialization and played its introductory video. After the video, the engine printed `Could not open file` for `Defaulteffects.bdae` and the process exited with signal 11. The file was present and readable through the AVD's case-insensitive `/sdcard` view. The log alone does not establish why the engine rejected it. This baseline did not reach the title menu.

## Direct Test 5 guest diagnostic

The published Test 5 wrapper contains an unsigned `assets/dh2/game.apk` guest (`84efe6018bcabb5c7e7bfb1953911dc67a88af19425816a8d55bc81dbd03a79b`). It was extracted, aligned and signed with a new local **emulator-only** development key, then installed into the Android 9 AVD. The signed local APK had SHA-256 `dab562301977d9225d9a1ecf6643939ad976c4caa9e1ec818082c4b43d905bc5`. Android installed the embedded patched engine with the expected Test 5 SHA-256 `45891aad9e7a5b1d84a5218f91926a04bd13a62ce104cb29beca7c70228c93c4`. No signed APK or key is committed.

With the full cache, this direct guest played the cinematic, displayed the title screen and accepted a tap into the main menu path. Its log read the supplied saved character (`wolf`, warrior level 1), successfully opened the Prince model once, then failed to reopen it through a path containing **two absolute cache roots**. The `DH2FileGuard` log records the first successful model open and second failed open; the process then ended with SIGSEGV at address zero. This is a concrete loader failure in the emulator test.

As a diagnostic only, copying the complete cache a second time under the nested path requested by that bad reopen let Test 5 display the animated main menu and its saved character. The Start game and Single Player buttons responded, but level loading remained at 31% (load step 12) for over three minutes. This duplicate cache was removed from the active path before the Test 6 run below.

## Direct Test 6 guest: source-level path repair

Test 6 changes the Storm file-open guard to retry a failed read-only request only when it contains two identical absolute app-cache roots. The retry keeps the first root's spelling. Its host regression test, pinned ARM32 patch build, APK ZIP check and local signature verification passed; see [the build note](../compatibility/work/fold7-build/TEST6-EMULATOR.md). The locally signed emulator APK has SHA-256 `21987bbdbd10edac201e1e3fe26e06aa9caa22da346305522648dd87f2255189`; the patched Storm library SHA-256 is `98eecf6d8b13be98a3d615db4584d67f2f59843deb0396516e72fe99601f868a`. Neither APK nor test key is committed.

With only the original cache root present, Test 6 played the cinematic, displayed the title, and accepted a tap into the **animated main menu**, showing the saved character and responding to Start game and Single Player. The emulator log contains successful `DH2FileGuard recovered repeated root:` and model-open entries. The duplicate `files/storage` workaround directory was absent. Thus the source-level fix reproduces the menu result without a second 648 MB cache tree.

Single Player has **not** reached gameplay. During this run the original ARM32 game made 3,188 failed thread-creation attempts (`pthread_create ... Out of memory`) while at the menu. Level loading advanced to step 6, then Android's x86 ARM translator aborted with `Memory exhausted: requested 131072 bytes`. This is an emulator/runtime failure, not evidence that level content is playable. Four texture requests used `qata/3d/textures/` while those names exist under `data/3d/textures/` in the supplied cache. Their relationship to the translator failure is still under investigation. The game is currently verified through menu navigation only.

These direct guests use Android's `libndk_translation.so` and locally signed packages. Their results must not be read as validation of the released ARM64 wrapper or of a fully reconstructed engine. The source branch separately includes host-rendered BRES/GLSL previews and independently built asset readers; those components are not installed in these APKs.

## Reproduce on this host

1. Install the official SDK packages `emulator`, `platform-tools`, `build-tools;35.0.0`, and `system-images;android-28;google_apis;x86`, then create an x86 AVD for that image. This test used a 10 GB data partition and 2 GB RAM.
2. Start the AVD with software graphics and confirm `sys.boot_completed=1`, `ro.product.cpu.abilist=x86,armeabi-v7a,armeabi` and `ro.dalvik.vm.native.bridge=libndk_translation.so`.
3. Extract `assets/dh2/game.apk` from the published Test 5 APK. Align with `zipalign`, sign with `apksigner` using a disposable local development key, verify its signature, then install it. Do not install it over a differently signed package without preserving that package's data separately.
4. Copy the *contents* of the validated cache's `files/` directory into `/sdcard/Android/data/com.gameloft.android.GAND.GloftD2SS/files/`. Check at least one file hash on the AVD against the input cache before launch.
5. Launch `com.gameloft.android.GAND.GloftD2SS/.Zirconia_DRM`; collect `logcat`, a screenshot, and any tombstone. The cinematic lasted about 50 seconds in this test, followed by the title screen and a tap to continue.

The original APK and the direct signed guest were tested as separate installs in the same disposable AVD. Reinstalling a differently signed package may remove its app data, so the cache was copied again and verified after switching packages.
