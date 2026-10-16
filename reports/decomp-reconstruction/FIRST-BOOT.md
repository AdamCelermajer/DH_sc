# First boot of the Track B reconstruction on Android (owner R, 2026-10-16)

Result: libdh2.so (arm64-v8a) links with 0 undefined symbols, installs in the x86_64 API 37 emulator (arm64 via ndk_translation),
runs appInit, the render/update loop and GSInit steps 0-5 without crashing, and the original intro movie plays (Java side).
After the movie the GL surface stays black (process alive, no crash, no log): the first game frame (splash / main menu) is not drawn yet.

## Build / package / run (scripts in tools/trackb/boot/, outputs only in build/own-r, nothing tracked)
1. `cd build/own-arm64 && ninja dh2` (configure per ANDROID-BUILD.md, DH2_ANDROID_ABORT_STUBS=OFF). Confirmed: links, undefined = libc/liblog only.
2. APK: `py -3 tools/trackb/boot/build_apk.py`. Takes the original Dungeon-Hunter-2-HD-v1-0-2.apk (its libDungeonHunter2.so is byte-identical
   to the reference .so), drops META-INF and lib/, patches minSdkVersion 8 -> 26 in the binary manifest (targetSdk then defaults to 26),
   adds lib/arm64-v8a/libDungeonHunter2.so (stripped libdh2.so) and a stub libStormGLOFT.so (storm_stub.c: the Java class loads it, the
   game .so does not link it; the stub also logs stdout and guest crashes to logcat, tags DH2STDOUT / DH2CRASH), zipalign 16K, apksigner debug key.
3. AVD: image system-images;android-37.0;google_apis_ps16k;x86_64 (has libndk_translation, abilist x86_64,arm64-v8a). Create AVD dh2_r
   (avdmanager fails on devices.xml; copying a config.ini with image.sysdir.1 fixed works), `bash tools/trackb/boot/start_emulator.sh`.
4. Data: `bash tools/trackb/boot/push_data.sh` extracts Dungeon-Hunter-2-HD-v1-0-2-cache.zip and pushes files/ to
   /sdcard/Android/data/com.gameloft.android.GAND.GloftD2SS/files (the path the Java side hardcodes), then chowns it to the app uid (otherwise the open is denied).
5. Run: `bash tools/trackb/boot/iterate.sh [secs]` (rebuild, package, install, run) or `run_game.sh`. It runs `adb root`, grants READ_PHONE_STATE and sets
   appops USE_ICC_AUTH_WITH_DEVICE_IDENTIFIER (DungeonHunter2.onCreate calls getDeviceId, refused on Android 10+), starts `.DungeonHunter2` directly
   (skips the Samsung DRM activity), captures logcat, screenshot, symbolizes. `bt.sh`/`bt2.sh`/`symbolize.sh` map pc offsets to source with llvm-addr2line on the unstripped .so.
   The "built for an older version of Android" dialog must be dismissed once: `adb shell input tap 1884 656`.

## Crashes found and fixes (each in the code, not a workaround)
1. Application::Init null CResFileManager::Inst: IDevice constructor was a stub. Now creates CResFileManager, CAnimationStreamingManager, timer, logger hookup, file system, banner, checkVersion (glitch-core-io/src/device.cpp).
2. Static `IrrFactory::s_factory` missing (original static initialiser 0x350b24): device got the engine CFileSystem instead of the game FileSystemWin32 -> bad vtable call in DebugSwitches::load (game-world pack1 + dh2/IrrFactory.hpp).
3. Data not readable: pushed files owned by shell uid (push_data.sh chown).
4. GL function table never bound: all GL calls were skipped, shaders "failed" silently, then infinite recursion createPinkWireFrameRenderPass/addRenderPass. gl_function_table.cpp binds GLES2 on Android. Also a DH2GL logcat line on shader compile failure.
5. glitch intrusive_ptr(T*) takes the reference without add-ref; ~60 sites wrapped an existing or count-0 object this way and dropped it twice (double free / freed batch buffer): all `.get()` wraps and the count-0 `new` wraps (CMeshBuffer, CImage, CAppendMeshBuffer, shaders, createFileSystem) now pass `, true`. Files: CColladaMaterial, CBatchMesh, CBatchSceneNode, IColladaSkinTechnique, ccommon_gl_driver_base, ok1/ok2-IVideoDriver, video_driver, ok2-ImageIO, IMesh, ParticleNodeCommon, CAppendMeshBuffer, quantize_buffer, irr_factory ... (see git diff). Host tests that encoded the old counts may need updating (not run by me).
6. SPassBinding table allocated 8 bytes/entry but the host struct is 16: heap overrun in bindParameter (SCreationState::addRenderPass).
7. sortParameters had the automatic/other groups reversed (listing 0x5e7b50), so binding index went negative; test ok1-ShaderNames_test updated.
8. PyData name tables never loaded: NameTable::size was never set, ReadNameTable refused every names file -> null names -> crash in SavegameManager::_initSettings. Every Arrays::X::read now sets m_memberNames.size = size (71 tables); FindMemberName guards null names.

## State / blockers
- Black GL surface after the intro; next: log GSInit step progress (step 7 splash texture getTexture of data/3d/textures/splash_final*.tga), check CTextureManager/TGA loader and the first Draw path; GL errors from driver init (GL_MULTISAMPLE/GL_LINE_SMOOTH caps, hint target) are desktop-GL leftovers, harmless on GLES2.
- libStormGLOFT is a stub; Zirconia DRM and billing are bypassed. Emulator renders through gfxstream/swiftshader.

## Round 2 (after commit 7c0fd550): intro movie to the first menu push
Furthest point now: GSInit step 12 (menu init) loads the SWF menus, character-select 3D textures load, GSFlashMenu runs and calls
MultiMenuManager::PushMenu, which crashes (game-ui/src/g1-game-ui-MultiMenuManager-1c2651-0.cpp:338: `m_menuFXStack.m_data[count] = movie`
writes through a never-allocated array, the stack container has no growth). The black screen after the movie was not a GL-state problem:
the GL trace (DH2GLT: clear/viewport/draw counts, tag in logcat) shows draws and clears issued every frame; the process died in the
interrupt-resume splash draw (below), which looked like a hang because the dialog/movie covered it.
Fixes: irr_factory_test no longer requires s_instance == nullptr (the game links IrrFactory::s_factory, which publishes itself; the binary
constructs it in a static initialiser); passes in build/own-r-host. More intrusive_ptr no-add-ref sites (the pass shader in CAppendMeshBuffer and
vertex_override destroyed the GameSWF shader mid-frame; placeholder textures; parameter getters; device createGUIAndScene; texture holds);
BGRA upload: GLES2 needs internalformat == format, the table's texImage2D wrapper maps GL_RGBA/GL_BGRA_EXT to GL_BGRA_EXT (the splash TGA failed
with GL error 0x502 and fell back to a placeholder); gameswf file_opener prefixes RES_PATH to relative names on Android (menu SWFs were not found);
CColladaFactory::getEffectName used the file name (null) instead of the effect name (listing 0x631ac4).
Debug aids left in (Android only, remove or gate before release): DH2GLT call trace (gl_function_table.cpp, DH2_GL_TRACE), DH2DBG logs in
ccommon_gl_driver.cpp (updateData, no-shader pass, upload error) and video_driver.cpp (onShaderDestroyed with a backtrace), "GSInit step N"
_DEBUG_OUT in GSInit::Update. Next: give MultiMenuManager's FX stack real storage (and check the other PARTIAL notes in PushMenu), then continue to the first menu draw.

## Round 3
- MultiMenuManager::PushMenu appended to gameswf::array storage that was never grown: PushMenu now grows m_menuFXStack and the movie's state stack the way
  gameswf::array::resize does (reserve(n + n/2)); the same helper pattern as MenuFX::GrowStateArray (those were the only unguarded appends in the tree).
- Debug aids are behind one macro, `DH2_BOOT_DEBUG`, off by default: `cmake -DDH2_BOOT_DEBUG=ON build/own-arm64` (CMakeLists option) turns on the DH2GLT trace,
  DH2GL/DH2DBG logs and the "GSInit step N" log. tools/trackb/boot/play.sh = dismiss dialog, wait, collect logcat + screenshot, symbolize.
- State: no crash through GSInit step 14 (menu init, online config, controller/HUD SWF load); the game loop runs, touches reach the game ("Touch ... 1072 540"), the
  GL trace shows depth-only clears (mask 0x100) and two 6-index quad draws per frame with GL error 0, but the screen stays black. Next: find why the SWF/menu
  quads are black or empty (texture contents of the SWF atlas, the GameSWF shader uniforms/blend, scissor), and what the state after step 14 should draw (3D background, menu_FadeFromBlackScreen).

## Round 4: main menu renders and takes taps
Reference run: not possible, the emulator image has no 32-bit ABI (ro.product.cpu.abilist32 is empty), so the original armeabi-v7a APK cannot run.
Root cause of the black menu (found with the DH2_BOOT_DEBUG GL readback + uniform dump): WorldViewProjectionMatrix was NaN and begin_display
(DH2SWF log) got a 0x0 viewport with a NaN rect. MenuFlash2DCamera::Update read the screen size only inside the limit-clip branch; the listing
(0x42ce1c) reads it unconditionally, so without a limit clip SetViewport/SetBounds got 0x0 (game-ui pack3). Fixed.
Then: CharAnimator::SetAnimationSet read Structs::SkillList / Skill through raw int32 offsets (count at "+4" hit the vptr half, skill->Anim at skill[1]):
now typed (ListSize, Anim). Android window size: appInit's default case (1280x720, the original only knew 800/854/960) now uses the real surface so the
UI fills it and touch coordinates match. FORTIFY_SOURCE is off for Android (CMake option DH2_ANDROID_FORTIFY, the original had none; a strchr abort appeared after the title).
Result: title screen ("touch to continue"), tap, main menu (save slot WOLF, Start / Settings / Information), tap on Information opens its page; no crash.
Known: a stale "touch the screen to continue" text overlaps the menu for a moment; the UI language defaults to Russian (settings save from the cache).

## Round 5: language, stale menu, FORTIFY, new game up to the loading screen
Scripts added: tools/trackb/boot/to_menu.sh (title -> main menu), to_game.sh (-> Start -> single player), cycle.sh + wait_cycle.sh (rebuild, install, drive,
poll; avoids the 2 minute tool limit), iterate.sh now stops on a failed ninja ("BUILD FAILED"; it used to run the previous .so).
- Language: the binary is followed. SavegameManager::__loadLanguageAndOrientation (0x46c7b8) always ends by taking the phone language from
  GLMediaPlayer.detectPhoneLang (Locale.getISO3Language: eng 0, fra 2, deu 1, ita 4, spa 3, jpn 5, zho/CHN 7; the emulator reports en-US -> 0 -> English), but the
  "Language" OPTION stored in dh2_settings.savegame then overrides it (getLanguage 0x46d514). The settings file in the data cache (written on the original dev's device) holds
  Language=2, and in this HD build the "german" text pack (slot 2) is a Russian translation (data/text/*.german). So Russian is correct for this data set. To see English,
  delete /sdcard/Android/data/<pkg>/files/dh2_settings.savegame on the device (user data, not code).
- Stale "touch the screen to continue": Application::_Draw called driver->setOption(3, true) where the listing (0x32af1c) calls vtable slot +0xa8 clearBuffers(3).
  The colour buffer was never cleared, so the title screen stayed behind every menu (and the "hero art" behind the main menu was that stale frame; the real main menu is black behind the panels until the 3D background draws).
- FORTIFY: the strchr abort was vendored Lua (lgc.c traversetable: strchr(svalue(mode), ..) on a TString whose tail the checker sees as 0 bytes), a false positive; dh2_vendor_lua is
  excluded. With DH2_ANDROID_FORTIFY=ON and DH2_BOOT_DEBUG=ON, app_entry.cpp replaces __strchr_chk and logs any further overread (DH2FORTIFY); a full session logged none. The crash handler in
  storm_stub.c logged with ANDROID_LOG_FATAL, which makes liblog abort; it is ANDROID_LOG_ERROR now, so SIGABRT also gets a guest backtrace.
- New game path (tap Start, Single player): fixes in order: forceBind reentrancy (animator added to a child node rebound everything against the child; getParticleSystem on a non-root);
  decoder read material-channel text only for type 86 (87..91 left a flag byte in the pointer); VoxSoundManager slot arrays used 4-byte slots (host pointers are 8);
  CMaterial::allocateProcessBufferHeap rounded the pointer inside a 4-aligned heap block (releaseProcessBuffer then read a wrong header); SVertexStream::offset is now int32_t (the original wraps
  a 32-bit negative offset, a 64-bit pointer sum saw 4 GB); PlayerManager::_ManageCharacters only handled records that already had a character: a new local record never got its class
  (savegame class/level, PlayerInfo +0x680 PlayerSavegame) and _AddCharacter was never called, so Level step 33 dereferenced a null player character.
- Now: the level loads to step 36 (100 %), the loading screen shows; the progress bar stays empty and the screen does not advance: step 36 -> 37 is NativeEndLoading, called from the
  loading menu's ActionScript ("continue_text._currentframe is:72", "IS FINISHED" traced every frame); a tap reaches the HUD "BLOCKER" button instead of the loading movie.
  Next: find what the loading movie waits for (a press on its own button, the percent/frame values written through InvokeASCallback, as_value number reads).
