# Irrlicht OGL-ES r6038 Android proof

Irrlicht is confirmed as DH2's engine family. This isolated proof builds official upstream Irrlicht OGL-ES r6038 for Android as a porting candidate. The exact customized DH2 Irrlicht fork/revision and game-specific layer remain under investigation; the recovered ELF references game-specific `glitch::` classes that this upstream sample does not implement.

## Upstream source and provenance

- Canonical upstream: [`branches/ogl-es`](https://svn.code.sf.net/p/irrlicht/code/branches/ogl-es), fetched at SVN revision **6038**.
- Git-SVN import commit: `57d48e47399c9aabe9c8b627f0aec825aaaa5492`; commit metadata records `https://svn.code.sf.net/p/irrlicht/code/branches/ogl-es@6038 dfc29bdd-3216-0410-991c-e03cc46cb475`.
- Upstream version macro: **1.9.0 alpha**. The official project marks OGL-ES experimental; the r6038 commit log says the GLES drivers were adapted but only make compile-tested.
- `upstream/` is a curated, source-only snapshot: **876 files / 13,062,914 bytes**, with canonical tree-manifest SHA-256 `d60c411e0186ca75339601219a44dee8211fe08dfbc92ef78d06329a42c17374`. Every retained file keeps its exact upstream SVN-r6038 bytes and per-file hash. The snapshot contains the engine's source and headers, all 324 Android.mk compile inputs, the Android sample, all 22 shaders, six smoke assets, and applicable notices. `build.py` verifies the manifest, the complete file set, and all file hashes before building.
- The original 2,007-file / 42,650,831-byte upstream export and its manifest are preserved locally as ignored `upstream-full/` and `upstream-full-source-manifest.json`; they are not copied into the source package. Unrelated demo/test/tool trees, desktop project files, prebuilt outputs, and unrelated media are excluded from the curated vendor input. `curate_upstream.py` recreates this snapshot from a separately verified full export.
- Irrlicht and bundled third-party notices remain in `upstream/doc/` and the relevant source directories. NDK `native_app_glue` is separately licensed under Apache-2.0; its module notice and the complete Apache-2.0 license are checked in under `notices/` and packaged with the APK. AES Gladman source retains its license headers.

The Irrlicht license is zlib/libpng-derived. Keep the license, mark altered sources, and do not misrepresent the origin. The upstream notice requires an IJG acknowledgement if JPEG support is retained (or rebuild with JPEG disabled). Check each included third-party notice before redistribution.

## Build and smoke package

From the repository root:

```powershell
python port/irrlicht-android/build.py
```

To verify the checked-in source snapshot without touching build outputs or invoking Android tools:

```powershell
python port/irrlicht-android/build.py --verify-source-only
```

The script stages only the paths listed in the curated manifest under the ignored `build/` folder, applies the recorded Android NDK and full-surface smoke patches to that staged copy, builds Irrlicht's static library and the upstream `01.HelloWorld_Android` NativeActivity for `arm64-v8a` and `x86_64`, then packages a signed standalone smoke APK. The staged tree cannot pull in files outside the reviewed manifest. It does not modify the DH2 application renderer.

- Android NDK r29's highest installed native sysroot target is API 35; both native ABIs are compiled for API 35.
- The smoke APK uses **min API 26 / target API 37**, following this repository's Android target conventions. This distinguishes native compile API from the app's target API.
- Shared native libraries are linked with 16 KiB max/common page sizes. The script checks every `PT_LOAD` alignment and uses `zipalign -P 16` on the APK. It verifies the APK signature and alignment after packaging.
- On Windows, the build temporarily maps the generated `build/` output to an unused drive letter so NDK r29's `llvm-ar` receives short object paths. The mapping is removed even when a build step fails.
- Outputs and full per-step logs are in ignored `build/`; `build/build-report.json` records curated and original snapshot sizes/hashes, library/APK hashes, ABIs, alignment, and patch details.
- `python port/irrlicht-android/build.py --no-apk` builds the static and shared native libraries and performs ELF checks without packaging.

### Recorded patch

The pinned upstream `CIrrDeviceAndroid.cpp` calls `ALooper_pollAll` twice. NDK r29/API 35 headers mark that API unavailable and require `ALooper_pollOnce`. `build.py` replaces those two calls in the staging copy only. It also changes the upstream sample's 300×300 `WindowSize` to 0×0 so Irrlicht uses the full Android surface. The checked-in curated `upstream/` tree remains byte-for-byte unchanged and manifest-verified.

The API 37 SwiftShader smoke test exposed two more GLES2 portability issues. The common texture helper tried an obsolete fixed-function `glEnable(GL_TEXTURE_2D)` workaround before mipmap generation; the staging copy now excludes that call from GLES2 while retaining it for desktop GL and GLES1. The GLES2 driver also preferred its advertised BGRA texture extension, but SwiftShader rejected that format while generating mipmaps. The staging copy now uses the driver's existing A8R8G8B8-to-RGBA converter for GLES2, which supplies the standard `GL_RGBA` internal format. Both changes are applied to generated staging files only; `upstream/` and its manifest are unchanged.

The bundled zlib 1.2.8 C sources also assume the GNU89 default of the original Android NDK. NDK r29 defaults C to GNU17, which rejects their implicit `read`/`close` declarations. The staging copy's Irrlicht `Android.mk` receives `LOCAL_CONLYFLAGS += -std=gnu89`, which affects C files only and leaves C++ mode unchanged.

### Compatibility issues seen during the initial build

1. The unmodified Android device implementation failed at `ALooper_pollAll` because NDK r29 marks it unavailable. Fixed in the generated staging copy with the `ALooper_pollOnce` substitution above.
2. The first zlib build failed in `gzread.c` because GNU17 treats legacy implicit declarations of `read` and `close` as errors. A first attempt to pass `APP_CFLAGS=-std=gnu89` also reached C++ and was rejected; fixed by adding the NDK's C-only `LOCAL_CONLYFLAGS` in the staging `Android.mk`.
3. Windows `llvm-ar` initially failed with error 87 because the full checkout path made the archive command exceed Windows' process parameter limit. Fixed by routing NDK object/library outputs through a temporary drive mapping to the same ignored build folder.
4. After producing the archive, the upstream makefile's custom `all` rule invokes Unix `cp`, which Windows `ndk-build` cannot execute. The harness removes only that custom copy rule from its generated staging copy, collects the archive from `NDK_OUT`, and stages it where the upstream example expects its prebuilt library; source code and build rules in the pristine source snapshot remain untouched.
5. Upstream omits `APP_STL`; NDK r29 then selects the legacy system STL, which cannot resolve sized-delete symbols from current Clang output. The staging copy sets `APP_STL := c++_static` for both the library and example.
6. The upstream example's makefile also stages demo assets with Unix shell `cp`; the APK packager copies the same named Irrlicht media and shader assets from the pinned source tree directly.
7. Legacy override/deprecation warnings (including `ASensorManager_getInstance`) are expected from this old branch; they are not promoted to errors.

## API 37 runtime smoke result

The exact APK SHA-256 `249b68998a85562174d30bfd537e953e4f4391267da63a05689ed12e2bdce506` was installed and launched on Android 17/API 37 x86_64 emulators with 4 KiB and 16 KiB pages. In both runs, the installed `base.apk` hash matched the build. App-PID logcat reported zero `GL_INVALID_OPERATION` and zero `GL_INVALID_ENUM`; Irrlicht loaded the logo, axe, and dwarf textures plus `dwarf.x`. The settled screenshots show the logo and fully textured, animating dwarf. A first capture taken during cold startup was blank, so the recorded screenshots were taken after the scene finished loading. See [`runtime-smoke-validation.json`](runtime-smoke-validation.json); local screenshots and logs are under ignored `build/smoke-api37/`.

This proves only that the upstream sample package launches, creates an EGL/GLES context, loads the model and renders a frame on API 37 with SwiftShader on both page sizes. It does not integrate Irrlicht with DH_sc's current `GLSurfaceView` renderer, test process-death/EGL-context-loss recovery, or demonstrate DH2 gameplay. Runtime used x86_64 emulators; ARM64 runtime and physical devices remain untested. The historical OGL-ES device owns its native Android/EGL lifecycle, so app integration needs an explicit single-EGL-owner design.
