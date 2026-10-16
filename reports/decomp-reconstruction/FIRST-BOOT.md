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
