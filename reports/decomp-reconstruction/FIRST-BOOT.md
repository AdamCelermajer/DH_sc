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

## Round 6 (owner R2): loading screen -> first level, hero visible and running
Furthest point: new game loads, the loading movie finishes, tap-to-continue ends loading (step 36 -> 37 -> 38), the first level renders in 3D with the HUD, the hero is visible and runs with the virtual joystick,
dead bodies/blood/torches render, the attack button is accepted; no crash. Not verified: enemies fighting back (none met in the first room), the attack hit path, item/monster textures (see remaining).
Root causes (all against the binary):
1. Loading never finished: the loading movie calls `loading_anim.gotoAndStop(percent + 1)` and `continue_text.gotoAndPlay(1)` with NUMBERS. The binary's sprite_goto_and_stop (0x7c1adc) converts with
   as_value::to_string(tu_string&); the upstream gameswf_as_sprite.cpp used to_tu_string(), which in DH2's as_value (0x420a84) does not convert numbers, so the frame was "" and goto never ran: the bar stayed empty
   and the continue button/text (frames inside loading_anim, sprite 135) never appeared. gotoAndPlay/gotoAndStop in upstream/gameswf-r1714/gameswf/gameswf_as_sprite.cpp now use to_string(buf). Taps then hit btn_loading_continue,
   whose onRelease calls NativeEndLoading. (The HUD "BLOCKER" button sits below it in the loading movie; the stacking was correct.) Other upstream sites that pass numbers to to_tu_string() may still be wrong (128 uses in upstream/).
2. 3D never drawn: Application::_Draw called scene->registerSceneNodes(null) for Hex-Rays "vtable +0x3c". The object's vptr is 5 entries into the symbol table, so +0x3c is SceneManager::drawAll(ISceneNode*) (listing +0x50, 0x359338;
   GSViewer already used it). Now `scene->drawAll(nullptr)`.
3. Hero invisible and stuck: PlayerManager::_ManageCharacters computed the wanted visibility as `!m_ingame`; the listing (0x373580..0x3735c8) is `ingame && !ScriptManager::m_inCutScene`, and a visibility mismatch
   resets the state machine to idle (SM_SetIdleState). Inverted, every in-game character was hidden and reset to idle each frame (Move state left after one frame, root-motion walk anim restarted every frame, no movement).
4. CMakeLists: dh2_vendor_stb_vorbis got `vox-audio/include` (another owner's stb_vorbis.c edit includes vox-audio/vox/stream.hpp; the Android build failed without it).
Debug aids used and removed again (all were behind DH2_BOOT_DEBUG): per-native logs, goto_frame, HUDControls::OnEvent, state-machine/animation traces. Findings that are still useful: joystick events arrive as RenderFX type 4/5/6 on HUDControls::m_joystickStick;
movement is root motion (RootSceneNode::_HandleDisplacement, AnimApplicator delta of the "root_camera-node" position track); `adb shell input swipe 420 830 520 720 5000` drives the stick, a tap at (1200,540) ends loading.
Remaining / next: (a) a green hooded model beside the first torches with a wrong (streaked marble) texture: probably a UV or texture slot fault of a skinned/modular mesh (the hero's own textures are right); (b) the level is very dark (lighting,
ambient/dynamic lights or fog; compare LevelConfig colours); (c) find an enemy and check attack, hit reaction, enemy AI and damage numbers; (d) the loading "touch to continue" text blinks (frame 1..54 visible) as in the original.

## Round 7 (owner R3): playing the first level (combat, AI, zoning)
Build/run: own build dir build/own-r3/arm64 (Release, DH2_BOOT_DEBUG=OFF: the debug build ran at ~1 fps because of the GL readbacks/DH2SKIN logs; turn it on only for rendering work). adb -s emulator-5554 only
(two emulators exist), `svc power stayon true` + keyevent 224 (a sleeping screen makes screencap hang), debug.keystore must be the same as the one that installed the package (INSTALL_FAILED_UPDATE_INCOMPATIBLE otherwise).
Stick: `input swipe 415 830 X Y ms`; attack hold: `input swipe 2052 846 2052 846 3000` (a plain tap is too short to be seen by HUDControls::Update); skills (1824,696) and (2226,510); potion (2226,108); pause (294,322).

Root cause of "the level is very dark / black void / monsters never wake" (it was NOT lighting): RootSceneNode::RefreshBoundingBox called the parameterless virtual CSceneNode::computeBoundingBox() (K 0xf4, union of RELATIVE transforms).
The listing (0x35c854) calls the const overload computeBoundingBox(aabbox3d&) with R1 = this+0x130, the union of the mesh children's ABSOLUTE (transformed) boxes, then subtracts the node position.
VisualObject::SetParent of a static owner runs OptimizeStatic (0x50f220: every node's position := its absolute position), so the relative transforms of a Module's children carry the module offset again; the boxes that Module::InitPost turns into the generated RoomZones
were shifted by (module offset) for every module not at the origin (module (-6000,0) got x -19830..-12236 instead of -9000..-3000). Consequences: the decor batch (GOBatchSceneNode::isSegmentVisible hides segments of objects with zoning on and m_inZone == 0) was invisible
outside the start module, and every zonable Character/Item standing in those modules had m_inZone == 0, so CharAI::Update returned at once (never aggro, never attack, never even drawn). Fixed in game-world/src/g1-game-world-RootSceneNode-e402f1-0.cpp. After it the 9 rooms are exact 6000 tiles,
the whole swamp renders with floor/walls/skulls/torches, monsters wake when their room enters the camera frustum (RoomZone::Update) and fight.
Verified in play (hero level 1, savegame slot WOLF): basic attack (hold) with hit/miss floating numbers ("Промах"), enemy name + level + HP bar + target ring, enemy melee on the hero (HP bar drops), skill 1 (slash FX, cooldown grey-out) and skill 2 (cast, mana bar drops, cooldown arc),
XP bar fills on kills, enemy death (corpse despawns), the follower NPC (green hooded model, ai 50) fights and draws aggro, prespawn (state 17) monsters wake on approach, pause menu (continue / help / settings / main menu), "return to main menu?" Да -> main menu with the save slot (no crash).
Not verified / blocked: hero death (the hero only lost ~8% HP in all fights), potion use (HP was always ~full; count stays 5), loot drops (the 6 monsters killed have no drop or none rolled; the save slot has the start chest (OpenableContainer state 4 = OPENED) and the barrel already opened/destroyed,
the two "corpses" near the start are ai-30 characters with HP 0 saved as dead: this is the persisted state of the WOLF save, not a bug), inventory/equipment screens (no HUD button found), doors/triggers/cut scenes (none reached), checkpoint/save write, level-up (needs more XP).
Visual bugs seen, left for V1: swamp monsters (zombie "Болотник", follower NPC) have a streaked green marble texture; the moths (ai 41) render as black silhouettes; DH2SKIN debug lines in IColladaSkinTechnique.cpp show outOff=4294965880 (a negative offset printed as unsigned, check the 32-bit wrap).
Debug aids used and removed again (the combat/zone traces were behind DH2_R3_DEBUG): per-frame character/zone dump in ObjectManager::Update, F_ApplyResult trace, Module::InitPost box log.

## Round 8 (owner R4): genuinely new game, deeper play (loot, inventory, skills, save/continue)
Setup: the repacked APK's SavesRestoringPortable.DoSmth runs only from Zirconia_DRM.onCreate, which run_game.sh/am start skip (it launches .DungeonHunter2 directly), so the WOLF save came from the pushed data cache, not from the restorer. The device saves were backed up to /sdcard/r4_save_backup and removed (dh2_*.savegame/.checkpoint, dh2_settings.savegame, DebugSwitches kept);
with dh2_settings gone the language is English (phone language), the new game starts at slot 000 with hero "ray", Warrior L1, HP 165, MP 27, 5 potions, empty chest/barrels/corpses as in the real start. Build dir build/own-r4 (arm64, Release, DH2_BOOT_DEBUG off, same debug key), device emulator-5554, helper build/own-r4/a.sh (tap/sw/shot/togame).
Fixed: NativeInvGetItemsListForSlot (0x44bb94, game-app/src/g1-game-app-global-f216b6-13.cpp) never pushed the item entries (the earlier comment said the Hex-Rays text has no push after the sort; the listing 0x44be5c..0x44c1a8 has the loop). Every slot list in the inventory/equip screen was empty, so nothing could be equipped, shown or transmuted.
Now per entry: ItemName ("<font color='#%000000X'>name</font>", item colour = rarity colour), ItemIndex, ItemEquippable, ItemEquipped, ItemEquippedOtherHand (other hand / other ring finger), ItemQuantity (count in valuables mode, " " otherwise), pushed onto the array.
Verified in play after the fix: HUD menu button (372,144) opens the character menu, tabs Stats(1104,54)/Inventory(1338,54)/Skills(1584,54)/Faeries(1800,54)/Quest journal(2028,54)/Map(2256,54) all draw; slot lists show the items in the item colour (green = magic), detail pane (armor, resist, req, value), EQUIP/UNEQUIP buttons, Auto-equip All; the hero model changes (helm, buckler, boots, ring), stats pane updates;
chest opened by attacking it (opens, drops "Conscript Helm"/"Broken Ring" with the coloured pickup text), gold pickup text ("2 GOLD") and counter, potion count 5 -> 6 from a drop, XP bar fills from kills, the two tutorials (treasure chest, combat counterattack) appear as trigger volumes with SKIP and tap-through, quest journal "Prison Break", skills page (Headsplitter L1, locked slots, skill points 0), world map page.
Save/continue: pause -> Main Menu -> Yes writes dh2_000.savegame + level savegame; the character-select screen shows ray / Warrior / Level 1 / Act 1 / The Boglands / Last Save, Single Player -> loading -> the level restores helm + gold + potions + XP and the killed enemies stay dead.
Not reached: level-up (needs 100 XP, bar at ~55 % after the first rooms), hero death and potion use (hero HP never dropped visibly: level 1 hero with 165 HP, the monsters meet the follower first; a potion tap at full HP leaves the count unchanged), doors/level exit, next level, checkpoint.
Blocker: SIGSEGV after ~21 minutes of play (GLThread, memmove of 0x1302 bytes into a non-writable page, fault addr 0x797e8ce1cd40, SEGV_ACCERR, write at the first byte of the copy; ndk_translation gives no guest pc and the guest crash handler printed nothing; tombstone data: rdx=0x1302). Looks like a stale/guard-page destination (use-after-free of a large scudo allocation), not reproduced yet; needs a DH2_BOOT_DEBUG run with a guest backtrace or an ASAN-like heap check.
Rendering (owner V1, not touched): follower and Bogwomp/zombie textures are green marble, moths are black silhouettes, the "Rene" corpse model on the plank bridge stands upright.

## Round 8 (owner R4): fresh new game, loot, inventory/equipment
Setup: build/own-r4 (arm64 Release, DH2_BOOT_DEBUG=OFF), emulator-5554, same debug key. The injected save restorer (com.savegame.SavesRestoringPortable, called from Zirconia_DRM.onCreate) never runs here because run_game/am start launch DungeonHunter2 directly; the WOLF save was only the cache's dh2_* files, backed up to /sdcard/r4_save_backup* and deleted, dh2_settings removed too (UI is now English).
Verified from a genuinely new game: name entry keyboard, class select, hero slot (RAY, Warrior, Act 1 The Boglands), Start -> Single player, loading, intro cut scene with Skip and dialogue (Rene, "Lord Ray!"), tap-to-advance, follower NPC, chest tutorial ("To open a chest, stand next to it and tap the chest icon") that is triggered by walking to the chest, attack button turns into the chest icon next to a chest, chest opens and drops gold + items with floating names (rarity colours: white/blue/green names), pickup status line (e.g. "Broken Ring", "Vagrant Bracers", "Imbued Boots"), inventory screen (pause screen tab 2: slot list, Auto-equip All, Swap Weapon, gold, potions), per-slot item list (a scroll wheel: drag the list to bring an item into the centre row, taps do nothing by design), item details (armor, magic powers, Req, value), Equip/Unequip/Transmute (confirm dialog -> gold 0 -> 20)/Drop buttons, combat on the Bogwomp, Continue after quit-to-menu (the checkpoint state is restored, same position, potions 5, chest closed again).
Bug fixed (root cause against the binary): NativeInvGetItemsListForSlot (0x44bb94, game-app/src/g1-game-app-global-f216b6-13.cpp) never pushed the entries onto the output array (the earlier port read the Hex-Rays text, which drops the loop; the listing 0x44be5c..0x44c1a8 builds one object per entry with ItemName "<font color='#%000000X'>name</font>", ItemIndex, ItemEquippable, ItemEquipped, ItemEquippedOtherHand, ItemQuantity and pushes it). Symptom: every slot list in the inventory was empty, no item could be equipped; now the list, the green/blue rarity names and Equip work.
Not verified / open: potion use (hero HP never dropped in ~10 fights: enemies seem passive or too weak; a Bogwomp stood in an attack pose without approaching), hero death/respawn, level-up and skill tree (XP bar only ~2%), doors/world map/next level, save timestamp ("Last Save 11:59" never changes after saves at 12:12: check PlayerSavegame m_saveDate update), one SIGSEGV in memmove on the GL thread (guest pc 0x218ae in a trampoline, after ~21 minutes of play with skills spam; no symbolised stack: tombstone_16 on the emulator; ctx: pressing skill 1 repeatedly + attack). "Req: 7 eng" in item details looks like an unresolved stat abbreviation: check the Req string table. Rendering (V1's): green marble follower/monster textures, white silhouette fairy in the intro, level-geometry texture seams.

## Round 7 (owner V1): lighting, texture binding and vertex colours of the first level
Verdict on the three visual bugs, checked with a GL trace on a second emulator (own AVD, own build dir build/own-v1-arm64, APK/scripts under build/own-v1, untracked):
1. **Dark level: not a lighting fault.** The light path is right end to end: the level's `PlayerLight` (swamp.lightset_xml, attenuation 0.265625/0/0.132813, ambient and diffuse 1) becomes a `CLightSceneNode`, `LightSetManager::SetLight` and
   `IVideoDriver::addDynamicLight` run every frame, and `commitLightParameter` sends Light0Position/Attenuation/Ambientcolor/Diffuse (-4291,-44,455 / 0.2656,0,1.3e-7 / 1,1,1 / 1,1,1) to every GL_Diffuse draw (Level::UpdateLightSet(bool,int) is empty in the binary, too: 0x3ef280).
   The darkness was the decor batch hiding every module outside the start room (RootSceneNode::RefreshBoundingBox, fixed by R3); a framebuffer dump after every draw showed the lit floor batches painting nothing. With that fix the level is lit as the data intends (warm player light over the teal tiles).
2. **Green hooded figure at the first torches: not a texture fault.** It is a low-poly jade statue batched with the tiki torches (draw with env_swamp.tga, 438 triangles; UVs all inside the atlas, vertex colours baked). Its flat facets and the tiny UV swatches give the "streaked marble" look; geometry and UVs
   dumped from the draw are consistent. No change made. Zombies and the follower NPC were not met again in my runs, so that claim is unverified.
3. **Fixed: black moths.** `overrideVertexStreams<true,false,true>` (glitch-core-io/src/vertex_override.cpp, 0x60a384) fills a missing colour stream (attribute 18) with the material's diffuse colour. The listing initialises the four colour bytes to 0xFF and `getParameter<SColor>` only overwrites them when the
   stored value really is a byte colour; the port started from a zeroed SColor and copied it back unconditionally, so every skinned, batched mesh whose DiffuseColor is stored as float colour got vertex colour (0,0,0,0) and drew black. Now the bytes start white. Before/after: build/own-v1/full3.png (black moth) / build/own-v1/moth1.png.
4. **DH2SKIN "offset 4294965880":** only the log cast (`(unsigned)offset`); `SVertexStream::offset` is int32 and every use (skin, copyStream, setupArrays, vertex_override) sign-extends in pointer arithmetic, so there is no 32-bit wrap in the skinning path. The log now prints `%d`.
Other findings: (a) the level materials come as `Multilight-fx` techniques (GL_Diffuse_L1_VC_iPhone) baked by CGenericBaker/CAppendMeshBuffer into `ZeTechnique` batch renderers; the unlit ProfileCOMMON batches carry baked vertex colours; (b) the "Could not open" textures
(`tex_prince.tga`, `env_swamp_alpha.tga`, `char_faerie.tga` ...) are expected misses: the cache only has the atlas and `pvr2_*_alpha` files; (c) PVRTC (4bpp and 2bpp alpha) textures decode correctly with PVRTCDecompress (checked offline on the swamp atlases).
Test-rig notes: a second AVD (dh2_v1, port 5556) avoids touching another owner's app; use `adb logcat -G 32M` (the game floods the log and the step markers of the scripts rotate out), `settings put global hide_error_dialogs 1` and `svc power stayon true`; the intro movie only ends the loading step
through appPause/appResume, so with DH2_BOOT_DEBUG a file `files/dh2_skipvideo` skips the movie, and `files/dh2_shpatch.txt` (lines `FROM=>TO`) patches GLSL text at load for bisecting shading without a rebuild.
