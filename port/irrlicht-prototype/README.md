# Irrlicht 1.8.5 isolated Null-driver prototype

This directory is a reproducible, isolated build of the official Irrlicht **1.8.5** source release as a feasibility candidate. It is not evidence that DH2 shipped this exact release, and it is not connected to the Android app or its current renderer.

## Source and license

- Upstream: [Irrlicht Engine SourceForge SVN tag `release-1.8.5`](https://svn.code.sf.net/p/irrlicht/code/tags/release-1.8.5/).
- Version: the tag's `include/IrrCompileConfig.h` identifies `1.8.5`; its `source/Irrlicht/Makefile` has major/minor/release `1.8.5`.
- Source mirror: `upstream/irrlicht-1.8.5/`, limited to `include/`, `source/Irrlicht/`, and upstream release notes. The source files are the bytes fetched from that tag. `upstream-source-manifest.json` records each file's size/SHA-256; its canonical tree-manifest SHA-256 is `986c189e13b8bd19c74b2552a61e60febdc0e68f8e7f54cf9763249e14e4306e`.
- `LICENSE.txt` preserves the Irrlicht license and its acknowledgment about Independent JPEG Group, zlib, and libpng. The original upstream `readme.txt` and third-party notices remain in the source mirror.
- `build.py` verifies every manifest hash before and after building. It does not edit upstream files. **Upstream source patches: none.**

## What this prototype verifies

`host/scene_mesh_null_host.cpp` reads the local cache's original `data/3d/animateddecors/candle_flame.bdae` through the repo's checked BRES reader and existing `SceneMesh` assembler. It transfers each draw's positions, UVs, and indices into Irrlicht `SMeshBuffer` objects, carries the repo's material and texture-reference labels through the handoff, then sends the triangles to Irrlicht's official `CNullDriver`. It checks the known four-triangle and `env_crypt.tga` fixture results and prints the resolved material/texture metadata. The host and Android probes omit upstream `Irrlicht.cpp` because its device factory chooses Win32 on the host and Linux/X11 on Android. `core_globals.cpp` supplies the three engine-wide constants needed by this Null-driver-only target; it does not modify any upstream source. Consequently, these probe libraries do not export Irrlicht's general `createDevice` factory.

The Null driver does not rasterize pixels or open textures. This is a mesh/API integration check, not a rendered-game test. The local cache input is used in place and is not copied into this directory.

The Android outputs compile the same official common engine/core source selection, with the Null driver and a tiny exported triangle probe. They use NDK r29's highest installed native API target (**API 35**) for `arm64-v8a` and `x86_64`; the app/runtime target remains **API 37**. The NDK r29 sysroot has no API 36 or 37 native stubs or compiler wrappers. API 35 is the native target used by the existing app build and is suitable for loading on Android 37, but this prototype is not compiled against API 37 native stubs. The linker requests 16 KiB maximum/common page size and the build script rejects any `PT_LOAD` with less than 16 KiB alignment. The shared libraries depend on NDK `libc++_shared.so`; a future APK using them must package that runtime or switch to an agreed static-runtime strategy.

### Platform limitation

The official 1.8.5 tag includes a Null driver and desktop OpenGL/Direct3D/software drivers, but no Android device implementation or GLES2 driver. This prototype excludes the desktop renderers and desktop window devices. Its Android `.so` proves that the selected official engine/core source builds for both requested Android ABIs with 16 KiB segment alignment; it does **not** provide Android rendering or prove that the app can run on Android. A real Irrlicht rendering path would still need an Android `ANativeWindow`/EGL device and GLES renderer (or a separately evidenced upstream fork).

## Build and run

From the repository root, the default paths use the local NDK and private cache already present beside this checkout:

```powershell
python port/irrlicht-prototype/build.py
```

The full command runs the host SceneMesh/Null-driver check, builds both Android shared libraries, verifies the ELF load alignment, and writes `build-report.json`. To run only one half:

```powershell
python port/irrlicht-prototype/build.py --host-only
python port/irrlicht-prototype/build.py --android-only
```

Pass `--ndk PATH` or `--cache PATH` to select other local inputs. Host output and Android objects/libraries are under the ignored `build/` directory and are reproducible from the source mirror; no emulator is used by this prototype.

## Build selection

The compiled source list is derived from the official upstream `source/Irrlicht/Makefile` object groups for mesh loading/writing, scene nodes, animation, GUI, IO, images, and bundled codecs. The `CNullDriver` is retained. OpenGL, Direct3D, software renderers, and SDL/X11 window devices are excluded because this is a headless smoke target. No app source or renderer build configuration is changed.
