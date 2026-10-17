# Irrlicht Android build proof

Irrlicht is confirmed as DH2's engine family. This is an isolated build of official upstream Irrlicht OGL-ES r6038 as a porting candidate and standalone upstream sample. The exact customized DH2 Irrlicht fork/revision and game-specific layer remain under investigation; this sample does not implement DH2 gameplay.

## Pinned source

- Upstream: [`branches/ogl-es`](https://svn.code.sf.net/p/irrlicht/code/branches/ogl-es), exact SVN revision **6038**.
- Git-SVN commit: `57d48e47399c9aabe9c8b627f0aec825aaaa5492`; imported SVN ID records `branches/ogl-es@6038`.
- Version macro: **1.9.0 alpha**. The curated `upstream/` snapshot retains **876 files / 13,062,914 bytes**; its canonical SHA-256 tree-manifest hash is `d60c411e0186ca75339601219a44dee8211fe08dfbc92ef78d06329a42c17374`.
- Curation keeps all public headers and engine source/header files, all 324 translation units listed by Irrlicht's Android.mk, their required bundled dependency headers/notices, the complete 22-file shader directory, six smoke media files, and the `01.HelloWorld_Android` source/build sample. Unrelated demos, tests, tools, desktop project files, prebuilt executables/libraries, and extra media are excluded. The original 2,007-file / 42,650,831-byte export plus its original manifest are preserved locally under ignored `upstream-full/` and `upstream-full-source-manifest.json`.
- The Irrlicht and bundled third-party notices are retained with the source. Since Android NativeActivity glue is built from the NDK and licensed under Apache-2.0, its NDK notice and full Apache-2.0 license are also included in `notices/` and copied into packaged notice assets. Review all notices before redistribution; the Irrlicht license requires an IJG acknowledgement if JPEG support is kept.

`python port/irrlicht-android/build.py --verify-source-only` checks the curated manifest and every retained file hash without producing or changing build outputs. The normal build stages only manifest entries and copies APK media/shaders only when their paths appear in the same manifest.

## Build and package result

Reproduce from the DH_sc repository root with:

```powershell
python port/irrlicht-android/build.py
```

The clean build completed with NDK **r29.0.14206865**, native `APP_PLATFORM=android-35`, ABI `arm64-v8a` and `x86_64`. Build Tools **37.0.0** and Android platform **37.0** produced a standalone NativeActivity package with **min API 26 / target API 37**. NDK r29's installed sysroot tops out at API 35, so API 37 is the package target, not the native compile API.

| Output | Result |
| --- | --- |
| ARM64 `libHelloWorldMobile.so` | ELF `AArch64`; each `PT_LOAD` segment is aligned to 16,384 bytes; SHA-256 `4a81a983db8a8a5fd87935519560c58f0597f2979d07645a267ec6d66064dd05` |
| x86_64 `libHelloWorldMobile.so` | ELF `Advanced Micro Devices X86-64`; each `PT_LOAD` segment is aligned to 16,384 bytes; SHA-256 `46eb16755fd23be1ac88fa06ef226ee1442deb094811ba41d4952be4ba655025` |
| Standalone APK | `port/irrlicht-android/build/irrlicht-ogles-r6038-debug.apk`; **11,422,569 bytes**; SHA-256 `249b68998a85562174d30bfd537e953e4f4391267da63a05689ed12e2bdce506` |

The APK contains both native ABI libraries, the 28 upstream smoke media/shader assets, and the checked Android native_app_glue notice plus Apache-2.0 license. `aapt2 dump badging` reports package `org.irrlicht.ogles.r6038.smoke`, compile/target SDK 37, and launchable `android.app.NativeActivity`. `apksigner verify --verbose --print-certs` passed with v2 and v3 signatures; the local debug certificate SHA-256 is `518b0366ee269323300c4ca7ef27296aaa120e47c293eb7edaddd739d70325d0`. `zipalign -c -P 16 4` passed after packaging. Full hashes and build inputs are also recorded in ignored `build/build-report.json`; generated binaries and logs are ignored.

The build script modifies only a generated staging copy: it substitutes `ALooper_pollOnce` for two NDK-removed `ALooper_pollAll` calls, sets C-only GNU89 for bundled zlib, selects `c++_static`, removes a Unix `cp` make rule that Windows cannot execute, and changes the sample's test viewport from 300×300 to full-surface 0×0. For GLES2 it skips the fixed-function `glEnable(GL_TEXTURE_2D)` mipmap workaround and forces 32-bit source textures through the existing RGBA byte-order converter instead of the advertised BGRA extension path. It obtains Irrlicht's static archive from `NDK_OUT` and stages the upstream media files for packaging. The manifest hashes verify that the checked-in source snapshot itself stays pristine.

## Runtime status and limits

The exact APK hash above was installed and launched on Android 17/API 37 x86_64 emulators with 4 KiB and 16 KiB pages; installed `base.apk` hashes matched. The settled full-surface screenshots show the Irrlicht logo and fully textured, animating dwarf. App-PID logs on both emulators report zero `GL_INVALID_OPERATION` and zero `GL_INVALID_ENUM`, and confirm that the logo, axe, and dwarf textures and `dwarf.x` load. A screenshot taken early during cold startup was blank; the verified settled screenshots were captured after loading completed. Detailed evidence is in [`runtime-smoke-validation.json`](runtime-smoke-validation.json), with local screenshots/logs in ignored `build/smoke-api37/`.

The demo is Irrlicht's upstream HelloWorld using its own sample assets; it does not load DH_sc assets, run a game level, or integrate with DH_sc's current renderer. It does not test process death/EGL-context-loss recovery. Runtime was x86_64 emulator/SwiftShader only; ARM64 runtime and physical Android devices remain untested. DH2-specific engine integration and gameplay therefore remain unproven.

## Game scene adapter compile proof

`port/irrlicht-android/game/` contains a separate SceneMesh-to-Irrlicht `SMesh` adapter. Its compile/link check succeeds against the pinned r6038 static libraries for both ARM64 and x86_64, and the generated libraries pass 16 KiB `PT_LOAD` alignment. A synthetic pyramid now renders through the adapter in the standalone [API 37/16 KiB smoke](game-smoke/README.md) and the opt-in [API 37/4 KiB app diagnostic](../android-app/IRRLICHT-HOST.md); neither is a DH2 source asset. The cache-backed `void_maze` scene renders all 77 BRES draws through Irrlicht and confirms texture mapping on its 30 `env_voidmaze.tga` draws. That subset passes API 37 tests with 16 KiB pages in the standalone smoke and with 4 KiB pages in the main-app local-only variant; other materials and complete room composition remain unverified ([standalone report](cache-scene-smoke/README.md), [main-app report](../android-app/IRRLICHT-CACHE-SCENE-LOCAL.md)). These checks do not establish integration of Irrlicht as the app's gameplay renderer. The compile report is in ignored `game/build-compile/compile-report.json`.
