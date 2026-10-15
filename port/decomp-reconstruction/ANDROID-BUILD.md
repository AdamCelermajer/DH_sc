# Building the reconstruction for Android ARM64

The reconstructed tree builds for `arm64-v8a` (android-26 and later) with the NDK's own CMake toolchain file. This is
the 64-bit modern-Android build; `android-armv7a.cmake` next to this file is the 32-bit soft-float build that the
armdiff differential tester uses and is a different thing.

## How to build

NDK r29 (29.0.14206865) was used. From the worktree root:

```
NDK=<sdk>/ndk/29.0.14206865
cmake -S port/decomp-reconstruction -B build/own-arm64 -G Ninja \
      -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
      -DANDROID_ABI=arm64-v8a -DANDROID_PLATFORM=android-26 -DCMAKE_BUILD_TYPE=Release
cd build/own-arm64
ninja -k 0 $(ninja -t targets all | grep -oE '^libdh2_[a-z0-9_]*\.a' | grep -v test_link_stubs)   # every library, tests not built
```

Notes:

- Do not build `all`: the per-category test executables are host programs (they link `dh2_all_tests`, run on the
  machine) and are only configured, never built or run, for Android. CMake prints a path-length warning for a few test
  object files on Windows; it is harmless because they are not built.
- `-DDH2_SYSTEM_ZLIB=OFF` (default) builds the vendored zlib; Android has `libz`, so `-DDH2_SYSTEM_ZLIB=ON` also works.
- `-Wno-format-security` is added for `ANDROID` (the NDK passes `-Werror=format-security`; `sys::printNoFormat` forwards
  the original's `printf(text)` thunk on purpose).
- Result on 2026-10-15: all 24 library targets compile (about 3.5 minutes cold, 16 vendored plus 9 engine libraries
  plus `dh2_game`). The compile is clean of errors; the warnings are clang-only
  (`-Winconsistent-missing-override`, `-Woverloaded-virtual`, `-Wmismatched-tags`), none about pointer width.

## What had to change for ARM64/Android (the host build is unchanged)

Most of the tree was already portable: every `_WIN32` / MinGW block (`filesystem_win32.cpp`, `filesystem_base.cpp`,
`misc_util.cpp`, `w7_global_04.cpp`, `file_system.cpp`, `xp_api.cpp`, `socket_platform.hpp`, `csocket.cpp`,
`transport_manager_impl.cpp`, `vox_thread.cpp`, `glx_player_ping.cpp`, ...) has a POSIX `#else` branch that Bionic
accepts, `core-platform` already carries the Android JNI / `ADevice` / `FileSystem` code (`__ANDROID__` blocks in
`debug_out.cpp`, the `p1-tinyxml-and-c-libs-global-*` unit and `android_jni_platform.hpp`), and the `-static` link
flags are guarded by `if(MINGW)`. The fixes that were needed were clang-vs-GCC and NDK-sysroot differences:

| Where | Problem | Fix |
| --- | --- | --- |
| `CMakeLists.txt` (zlib, `if(ANDROID)`) | `gzread.c`, `gzwrite.c`, `gzlib.c` call `read/write/close/lseek` without `<unistd.h>` (clang 21 makes the implicit declaration an error) | `Z_HAVE_UNISTD_H` for `dh2_vendor_zlib` on Android |
| `CMakeLists.txt` + `android-compat/GL/gl.h` | libtess's `glu.h` includes `<GL/gl.h>`, absent from the NDK | shim header that includes `<GLES2/gl2.h>` and declares `GLdouble`/`GLclampd`, on the include path of `dh2_vendor_libtess` only, Android only |
| `CMakeLists.txt` | `-Werror=format-security` on `sys::printNoFormat` | `-Wno-format-security` (Android) |
| `glitch-core-io/src/xml_reader.cpp` | destructor defined through the alias `WideReader` (clang `-Wdtor-typedef` error) | spelled `CXMLReaderImpl<wchar_t, glitch::IReferenceCounted>::~CXMLReaderImpl()` |
| `glitch-scene-collada-gui/src/tb_scene-collada-gui-glitch_scene-0.cpp` | explicit instantiations of the anonymous-namespace template `createTriangles` were outside that namespace | instantiations wrapped in `namespace { }` |
| `vendor-gameswf/src/v1-vendor-gameswf-pack0-157e56-0.cpp`, `...-pack6-a8bd9d-0.cpp` | `template<> void ::hash<...>::set_raw_capacity` written inside `namespace gameswf` (specialisation of the global `::hash`) | defined after closing `namespace gameswf` |

All five touched sources still pass `bash tools/trackb/check_tu.sh` (GCC, host). No ledger row changed.

### 64-bit layout caveat (not a build error)

The original is 32-bit ARM. On arm64 `sizeof(void*)` is 8, so every reconstructed class with pointer or `long` members
is larger than the original and the member offsets in the header comments (`// +0x04`) do not hold. Nothing in the
build trips on this (no pointer-to-int cast warnings, no failing `static_assert`), but code that reads original data
files through the class layout (savegames, `.bin`/`.res` records parsed by overlaying a struct) or that jumps on
`vtable +0x..` offsets must use explicit fixed-width fields. The host MinGW build is already 64-bit, so the same
issues apply there and the tests cover the data-file paths that matter; the Android build adds no new layout case.

## Producing libdh2.so (libDungeonHunter2.so)

The `dh2` target (added to `CMakeLists.txt`, `if(ANDROID)` only) links all libraries whole-archive into one shared
object, with `-llog -landroid -lEGL -lGLESv2 -lOpenSLES -ldl -lm` and the NDK's default static libc++:

```
ninja dh2                                    # -Wl,--no-undefined: fails, listing the undefined symbols
cmake -DDH2_ANDROID_ABORT_STUBS=ON . && ninja dh2    # aborting stubs for those symbols: libdh2.so links
```

The first form is the honest one: with `--no-undefined` the link stops with 160 undefined symbols (lld, 2026-10-15 22:56), listed with the
library object that references each one in `reports/decomp-reconstruction/android-link-missing.csv`. The second form
defines them (functions that print `DH2 android link stub called (not reconstructed yet): <symbol>` and abort, zeroed
4 KiB for data) from that same CSV, and produces a 105 MB (unstripped,
`-g`) `libdh2.so` that is a valid AArch64 ELF exporting `JNI_OnLoad` and the 35 `Java_com_gameloft_android_GAND_GloftD2SS_*`
natives. It is a link proof, not a runnable game: calling any stubbed row aborts.

### What is still missing (160 symbols, see the CSV)

| Category (`android_category`) | Rows | Meaning |
| --- | ---: | --- |
| `original-not-reconstructed` | 64 | ledger rows still `skipped`: `Console::*` (debug console menu), `DBG_*` tracing helpers, `VoxSoundManager`, `CResFileManager`, `CTextureManager`, `C2DDriver::draw2D*`, `IVideoDriver::draw/setMaterial/...`, `get_fontfile`, `read_filter_list`, `fillLeaderBoard`, `TouchScreenBase::isRegionPressed`, `CMatrix4::operator*`, `CNullDriver` ctor |
| `port-seam-placeholder` | 49 | `glitch::collada::recon_v2_10::*`, `recon_v2_04::*`, `recon_31::*`: seams of the collada layout recovery that stand in for a layout-dependent original body. They have no ELF symbol; they go away when the layout owner replaces the call |
| `port-stand-in` | 13 | names the port invented (no ELF symbol) |
| `third-party-source-not-linked` | 9 | `gluTessCallback`, `gluTessProperty` (the binary's references are C++-mangled, while the vendored libtess C units, built as C++, do not provide these two names) and Musepack `mpc_demux_*`, `mpc_crc32`, `vox::DecoderMPC8Factory`. The vendored `mpc_demux.c` is compiled as C, so its symbols are unmangled; the binary's references are C++-mangled |
| `jni-app-entry-glue` | 9 | `appInit appUpdate appPause appResume appDestroy appOnTouch appKeyPressed appKeyReleased appTrackBall`: the C entry points the `Java_..._nativeXxx` natives forward to (owned by the app/menu session) |
| `original-partial-row` | 7 | rows reconstructed only in part, no symbol-compatible body yet (`CLight` attribute (de)serialisation, `CMaterialVertexAttributeMap::allocate`, `CVertexStreams::setStreamBuffer`, ...) |
| `other` | 3 | no ledger row |
| `android-libcxx-mangling-or-new` | 5 | two are rows of the host report spelled with libc++ (`__ndk1`) instead of libstdc++ (`AlgoAStar::findNode`, `setParameter<intrusive_ptr<CLight>>`); three appeared after the host report was written (`CMaterialRendererManager::clearUnusedInstances`, `CResFileManager::unloadAll`, `core::createQuantizedBuffer`) |
| `android-key-function` | 1 | `typeinfo for RenderFX::EventListener`: clang emits it only in the unit that defines the key function (`OnEvent`), which no unit defines (only a test stand-in does). GCC on the host leaves it a weak symbol. Reconstruct `EventListener::OnEvent`/`OnFSCommand` (or make them inline) |

The list changes as the owners finish rows. `py -3 tools/trackb/android_link_report.py build/own-arm64` relinks the built
libraries and rewrites the CSV (classified with the host `link-missing.csv`). Only the toolchain-specific rows
(`AlgoAStar::findNode`, `setParameter<CLight>`, the `RenderFX::EventListener` typeinfo) differ from the host's gaps. Everything the Android shared object needs from the platform links against the NDK
(`log`, `android`, `EGL`, `GLESv2`, `OpenSLES`); no other system library is undefined.

### Entry points

- `JNI_OnLoad` is defined (core-platform, `p1-core-platform-global-f216b6-2.cpp`) and exported. There is no
  `JNI_OnUnload` in the original.
- All 35 `Java_com_gameloft_android_GAND_GloftD2SS_*` natives are defined and exported (`DungeonHunter2_native*`,
  `GameRenderer_native*`, `GLMediaPlayer_native*`, `GLResLoader_nativeInit`, `GameGLSurfaceView_nativeOnTouch`).
  Nine of them forward to the missing `app*` glue above, which is what actually drives the game loop.
- `Application` start-up, the renderer (`glitch::video::createOpenGLES2Driver`) and the resource/texture managers
  (`CResFileManager`, `CTextureManager`) are the blockers between this library and a game that starts: they are the
  rows above, not a build-system problem.
- The Java side (`com.gameloft.android.GAND.GloftD2SS.*` classes and the GLSurfaceView that calls `nativeOnDrawFrame`) is
  not part of this tree and must be supplied by the APK.
