# Windows host runner (owner W, 2026-10-16)

The reconstructed game runs natively on this Windows machine (MinGW-w64 GCC 16.2, 64-bit), headless or in a window, with a fixed timestep,
scripted input, a state log, gdb, and a guard-page heap that finds memory errors where the toolchain has no AddressSanitizer.

Result on 2026-10-16 (all four builds: Release, -O0 with asserts, guard-page heap, UBSan-trap plus guard-page heap): boot -> GSInit -> menus -> title tap -> main menu -> Start -> Single player -> loading -> first level (`001_swamp.mlx`, 51
characters, player 165/165 HP) -> hero walking, 2500 frames, exit code 0, in the Release build and in the guard-page build. The windowed runner
draws the same session through ANGLE (GLES2 on D3D11): title, main menu, loading and level 1 render correctly (1200x540 screenshots taken by the script).

## Layout

| Path | What |
| --- | --- |
| `port/decomp-reconstruction/host-runner/` | the runner sources, `CMakeLists.txt`, `scripts/*.txt` (input scripts), `symbolize.py` |
| `CMakeLists.txt` option `DH2_HOST_RUNNER` (OFF by default) | adds the `host-runner` subdirectory and defines `DH2_HOST_RUNNER=1` for every library |
| `build/own-w`, `build/own-w-dbg`, `build/own-w-ubsan` | the three build directories used here (untracked) |
| `build/own-w/data/files` | the game data (a copy of the emulator data folder, made with `robocopy`; the game writes savegames into it). Never tracked. |

Targets (all link every library whole-archive, like `libdh2.so`, because the game relies on static initialisers such as `IrrFactory::s_factory`):

* `dh2_win_headless`: no window, GL calls go to a table of stand-ins (`host_gl_null.cpp`), fixed clock.
* `dh2_win_headless_ph`: the same on a guard-page heap (`host_pageheap.cpp`), the AddressSpace-style memory checker.
* `dh2_win_window`: Win32 window plus an EGL/GLES2 context from ANGLE, wall clock by default, mouse = touch.

## Build

```
cmake -S port/decomp-reconstruction -B build/own-w -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS=-g -DCMAKE_C_FLAGS=-g -DDH2_HOST_RUNNER=ON
cd build/own-w && ninja dh2_win_headless dh2_win_headless_ph dh2_win_window      # about 5 minutes cold on 16 cores
# gdb build (-O0): same with -B build/own-w-dbg -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-g -O0" -DCMAKE_C_FLAGS="-g -O0"
```

Never run ninja in `build/tb-int`. Do not build `all` (tests) in these directories.

## Data

The data folder is the one `tools/trackb/boot/push_data.sh` pushes to the emulator (`Dungeon-Hunter-2-HD-v1-0-2-cache.zip`, extracted to
`build/own-r/data/com.gameloft.android.GAND.GloftD2SS/files`). The runner takes it as `--data <dir>`; point it at a copy, because the game saves into it:

```
robocopy build/own-r/data/com.gameloft.android.GAND.GloftD2SS/files build/own-w/data/files /E /MT:16
```

(Language: the saved "Language" option in this data selects the Russian text pack, as on the emulator. `--lang N` only sets the phone language the Java side
reports.)

## Run

```
build/own-w/host-runner/dh2_win_headless.exe --data build/own-w/data/files --size 2400x1080 --frames 2500 --out build/own-w/run1 --log-every 100 \
    --script port/decomp-reconstruction/host-runner/scripts/new_game.txt
```

Options: `--data` (required), `--script`, `--frames`, `--dt <ms of game time per frame, default 33>`, `--log-every N`, `--out <dir>`, `--size WxH`,
`--script-scale f` (scripts are written for 2400x1080, the emulator surface), `--lang N`, `--realtime` / `--fixed`, `--verbose` (game chatter on the console),
`--jni-log` (print every Java method the natives call), `--angle <dir>` (window only). Environment: `DH2_GL_DUMP=1|2` prints every linked GL program with its
active attributes/uniforms (2 also dumps the sources of programs with no attributes).

Output: the runner's lines on the console (`[runner]`, `[script]`, `[state]`), the game's own `_DEBUG_OUT`/stderr chatter in `<out>/game.log`, crash reports in both.

State line, every `--log-every` frames: `[state] f=<frame> t=<game ms> dt=<last frame ms> states=<stack depth> top=<state class> level=<file> chars=<live characters> player pos=(x,y,z) hp=hp/total dead=<0|1> csm=<character state id>`.

### Input script

One command per line, `#` comments, frame = absolute frame index (0-based). Coordinates are surface pixels of a 2400x1080 surface unless `--script-scale` is given.

```
<f> tap x y [hold]          touch down, up after hold frames (default 2)       <f> down|move|up x y [slot]
<f> drag x1 y1 x2 y2 n [hold]   down, n linear moves, hold frames at the end, up   <f> key code      <f> log text      <f> shot name (window)      <f> quit
```

`scripts/new_game.txt` (title -> Start -> Single player -> loading -> walk) and `scripts/new_game_shots.txt` (the same with screenshots for the window).
Touches reach the game through `appOnTouch`, the function `GameGLSurfaceView.nativeOnTouch` calls; keys through `DungeonHunter2.nativeKeyDown/Up`.

## Windowed

```
build/own-w/host-runner/dh2_win_window.exe --data build/own-w/data/files --size 1200x540 --script-scale 0.5 \
    --angle "<android-sdk>/emulator/lib64/gles_angle" --out build/own-w/runw --script .../scripts/new_game_shots.txt
```

Without `--script` it is interactive: left mouse = touch (drag = stick or swipe), F12 = BMP screenshot in `--out`, Esc = quit. `--fixed` uses the fixed timestep instead of the wall clock.

GLES2 comes from ANGLE (`libEGL.dll`, `libGLESv2.dll`, `d3dcompiler_47.dll`) loaded at run time and never copied into the tree. Source used: the Android SDK
emulator folder, `<sdk>/emulator/lib64/gles_angle` (here `C:\Users\nacworkstation\Documents\Codex\2026-10-09\find\work\tools\android-sdk\emulator\lib64\gles_angle`),
ANGLE "2.1.22762 git hash 533fa5cedc61", Authenticode status Valid, signer Google LLC (the `d3dcompiler_47.dll` next to it is signed by Microsoft). The runner binds the
game's `GLFunctionTable` (`shared/include/glitch-shared/gl_function_table.hpp`) to the `glXxx` exports of `libGLESv2.dll`, exactly the binding
`glitch-video/src/gl_function_table.cpp` does for Android (with the same BGRA and 64-bit-size wrappers). Not used: desktop GL.

## What the platform seams are

| Android side | Host replacement | Where |
| --- | --- | --- |
| JNI: `GLMediaPlayer` (sound, movie, `getSDFolder`, `detectPhoneLang`), `DungeonHunter2` (phone model/manufacturer/language, wifi, demo, VZ purchase), `GLResLoader`, `Musicplayer` | a fake `JavaVM`/`JNIEnv` whose method table answers the static calls from the host (the data folder, a generic phone, no vendor flag, sound calls ignored, the intro movie reported finished by setting `videoDone`); the real `*_nativeInit` natives run against it, in the Java classes' order | `host_jni.cpp` |
| EGL/GLES2 | `GLFunctionTable` of stand-ins (headless: object names, viewport, shader source scan with a small GLSL preprocessor so active uniforms/attributes match what a GLES2 compiler keeps, caps of a 1024-vector GLES2 device, framebuffers complete) or ANGLE (window) | `host_gl_null.cpp`, `host_window.cpp` |
| wall clock (`gettimeofday`) | `glitch::os::Timer` reads `g_dh2HostMicrosecondsOverride` (added to `glitch-core-io/src/timer.cpp`), `TimeNow`/`_GetTime`/`OS_GetTime` come from the runner; the fixed clock advances `--dt` ms per frame | `host_runner.cpp` |
| touch / keys from Java | `appOnTouch`, `nativeKeyDown/Up` called from the script or the window procedure | `host_script.cpp`, `host_window.cpp` |
| surface size | `appInit` already sized the device from `Width_Screen` only on Android; `core-platform/src/app_entry.cpp` now does it when `DH2_HOST_RUNNER` is defined | app_entry.cpp |
| resource root | the SWF file opener (`game-ui/.../RenderFX-849cdc-2.cpp`) prefixes `RES_PATH` for the host as for Android; `FileSystemBase::createAndOpenFile` treats a name that already starts with `RES_PATH` as absolute (the binary tests for `/sdcard/`, which only an Android `RES_PATH` has) | both files, `DH2_HOST_RUNNER` only |
| `lrand48` (POSIX, used by `ok2-Clustering.cpp`) | the drand48 generator in the runner | `host_runner.cpp` |

Sound: the vox sound manager initialises without a device and nothing is played. Audio output on Windows is not done.

## Debugging

* A crash filter prints the exception, the fault address, and a backtrace of RVAs: `python port/decomp-reconstruction/host-runner/symbolize.py <exe> <out>/game.log <path to addr2line>` turns them into functions and lines (use the `-g` Release build or the `-O0` build).
* `gdb -batch -ex run -ex bt --args <exe> ...` works as is (`gdb` and `addr2line` are in the MinGW bin folder). For a hang, `gdb -ex run` and Ctrl+C.
* The `-O0` build (`build/own-w-dbg`) keeps the `assert`s of the vendored gameswf code (the Release build compiles them out) and gives exact lines and locals; it is the one to use with `gdb` watch/condition breakpoints.

## Sanitizers

The WinLibs GCC here has no AddressSanitizer or UBSan runtime (`cannot find -lasan`, `-lubsan`), and no clang-cl/MSVC/llvm-mingw is installed (downloading one needs the
owner's consent, so none was fetched). What is configured instead:

1. `dh2_win_headless_ph` (`host-runner/host_pageheap.cpp`): operator new/delete and malloc/calloc/realloc/free (through `-Wl,--wrap`) allocate every block on its own pages with the end of the block
   against a NOACCESS guard page; a freed block is decommitted for good. An overrun or a use after free faults on the exact instruction, and the crash report adds
   "address is in a FREED block of N bytes" with the allocation and the free stack of that block (`symbolize.py` prints both). Fixed arena address, so runs repeat.
   Cost: one 4 KiB page per live allocation (700 frames, to the first level: peak working set 1.4 GB, no slower than the Release build by more than a few times).
2. `build/own-w-ubsan` (RelWithDebInfo -O1, plus the guard-page heap in `dh2_win_headless_ph`): `-fsanitize=bounds,shift,integer-divide-by-zero,unreachable,bool,enum,object-size,pointer-overflow,nonnull-attribute,returns-nonnull-attribute,float-cast-overflow -fsanitize-undefined-trap-on-error`
   (the UBSan runtime is not installed, but the trap form needs none: a failed check is an illegal-instruction exception, reported by the crash filter with its backtrace).
   Left out on purpose: `alignment` (finding 6 traps at once), `null` (finding 9, benign), signed-integer-overflow and `vptr` (the reconstruction deliberately relies on 32-bit wrap and raw casts).
3. `build/own-w-dbg`: `-O0 -g` with asserts.

Each build also runs under `gdb`.

## Findings

All found with the headless boot to the first level. Fixed ones are changes to the engine sources, each checked with `bash tools/trackb/check_tu.sh` (OK). They change Android behaviour too (every one is a real bug there, hidden by the allocator).

| # | Where | Finding | Status |
| --- | --- | --- | --- |
| 1 | `glitch-video/src/ok2-IVideoDriver.cpp` `initImplementationDependentData` | `intrusive_ptr<CVertexAttributeMap>(new CVertexAttributeMap(...))` without the add-ref flag: the map is born with count 0, the first copy takes it to 1, the temporary's release then deleted it. The full-screen quad map `unk_144[0..3]` was freed memory (Windows heap: `0xc0000374` heap corruption at the first big allocation after the driver init). | fixed (`, true`, like every other `new CVertexAttributeMap`) |
| 2 | `game-ui/src/g1-game-ui-MenuBase-8948cc-3.cpp` `~MenuBase` | released the saved-focus and character weak proxies with `weak_proxy::drop_ref()` and left the fields set; the C++ base destructor `MenuFX::State::~State` then released them again. Count below zero, proxy freed while other `weak_ptr<character>`s still pointed at it; the next gameswf object destructor read freed memory, and in the Release heap `RenderFX::Unload` during the level load ended in a heap corruption fast-fail. (`assert(m_ref_count > 0)` in `weak_ptr.h:41` fires in the -O0 build.) | fixed (fields cleared after the release) |
| 3 | `glitch-scene-collada-gui/src/r3a-...CColladaDatabase-056212-2.cpp` `getAnimation` | compared the bind URI (`channel->extra`, not a string for other channel types) with `_stricmp` before testing the channel type: a jump through a bad pointer for the fairy particle effect (`bolt2`, `faeries_02_celeste.bdae`). | fixed (type tested first) |
| 4 | `glitch-core-io/src/material_copy.cpp` `copyMaterialParameters` (0x6a1d9c) | walks `count` ids per shader stage but a pass keeps `count - field04` (12 vs 8 in the first batch of level 1): reads `field04` ids past the list, and starts the second stage late. The binary does the same (UB hidden by the next heap block); the guard-page heap faults on it. | fixed on the host semantics (walk and advance by `count - field04`, as `CGenericBaker::initParameters`); worth an explicit check against Hex-Rays by the owner of the batch code |
| 5 | `glitch-video/src/res_address_space.cpp` | the registry was a function-local static destroyed before the static destructors of other files (`VisualFXManager` dictionary) freed `.bdae` blocks: reads of freed map nodes at exit. | fixed (registry is never destroyed) |
| 6 | `glitch-core-io/src/process_buffer_heap.cpp` | the heap hands out blocks on 4-byte words; objects placed in it (`SCreationState` in `CMaterialRendererManager::beginMaterialRenderer`, `CMaterial` heap copies, ...) have 8-byte alignment on a 64-bit host. x86-64 and ARM64 tolerate scalar misaligned access, but a vectorised initialiser (`movaps`) would fault; `-fsanitize=alignment` traps at the first one. | open (owner of core-io: round the block start to 16 bytes and the header accounting with it) |
| 7 | `glitch-core-io/src/vertex_override.cpp` `overrideVertexStreams<true,false,true>` (batch baking) | the destination stream for an attribute in the shader's mask but not configured by `CGenericBaker::configureAppendBuffer` (attribute 28 here, 0x1c, which `configureAppendBuffer`'s switch has no case for) has a null buffer, `->map(5)` on it is a null call. It showed up only when the headless GL stand-in reported unused attributes as active (a real GLES2 compiler drops them); fixed in the stand-in. | open, latent: guard `destinationStream.buffer` or add the case if the binary has one |
| 8 | data set | `data/3d/characters/prince/weapons/mc_torso_default_warrior.bdae` is not in the data folder; `FileSystemBase::createAndOpenFile` logs "Could not open file" and the character-select screen carries on. | note |
| 9 | `game-ui/.../MenuCharMenu_Map-2d6327-0.cpp` `Init` | calls `m_menu->Find(...)` with `m_menu` null (the `menu_MapSheet` movie is not registered when the singleton is built in `MenuManager::Init`). Harmless: `RenderFX::Find` returns before touching `this` when the context is null. `-fsanitize=null` traps on it. | note (UB, benign) |
| 10 | `game-app/src/n2-game-app-sfc_script_lua_Value-c98984-1.cpp` `Value::getUInteger` | `static_cast<unsigned>(float)` of a negative number (first hit: `Character::_SetProp` from a skill script): undefined in C++ and wraps on x86-64 (-1.0f gave 0xffffffff), the binary's `__aeabi_f2uiz` / arm64 `fcvtzu` saturate to 0. | fixed (saturating conversion). The same class (`(unsigned)` or `(int)` casts of floats that can leave the range) may exist elsewhere: only reached paths were checked |
| 11 | `vendor-gameswf/src/v1-...sprite_instance-d5206d-0.cpp` `goto_frame(const tu_string&)` | `(int) number_value` of NaN/huge doubles from the loading movie's `gotoAndStop(...)`: `__aeabi_d2iz` saturates (NaN gives 0), x86-64 gives INT_MIN. | fixed (saturating conversion) |
| 12 | `glitch-video/src/material_parameters.cpp` `matrixPool()` | a function-local static vector destroyed before the static destructors of other files (`VisualFXManager::_FlushAnimDict`) released materials into it: use after free at exit. | fixed (pool is never destroyed) |

## Open points for the owners of gameplay and rendering

* In the 2500-frame session the hero walks about 30 frames after the stick drag and then stands still at (-4393, 555) though the stick is held until frame 621; no enemy comes
  into range, so no combat was exercised. State lines are cheap now: `scripts/new_game.txt` is the starting point for a combat script.
* Every `GL_*` capability the driver reads at start-up is answered by the headless stand-in (`host_gl_null.cpp`) with a mid-range GLES2 device; the windowed run uses ANGLE's real answers.
