# Completed offline LoadMainMenu source V114

Source only; no tests/compiler/build/hash/emulator/runtime verification. Network/CX/WiFi/score/login remain outside this delta.

New complete files:
- menu_main_menu_load_v114.hpp/.cpp: original MenuManager.LoadMainMenu4324dc (one TU on EXISTING portable target).
- native_main_menu_load_v114.inc: genuine binder on existing NativeCharacterMenu/Front/source singletons; include AFTER native_main_menu_front_v114.inc and BEFORE native_source_main_menu_v114.inc.

Narrow integration deltas (apply to current sources; successors are context only):
- front-main-menu-load-v114.patch
- main-menu-weak-singletons-v114.patch
- main-menu-explicit-resume-v114.patch

Earlier accepted Front/Reset packet remains required; latest candidate menu_main_menu_state_v114.hpp/.cpp include additive explicit-resume arm methods. No MenuStackOwner, nativeFS, runtime v61, Root CMake or shared source edits.

Enrollment:
After actual process MenuManager, existing primary1 display/device-facts provider, Front Init0 font/string prefix and local settings owner are established, call bind_source_main_menu_load_v114(error) ONCE. It installs the completed actual resource callback through bind_source_main_menu_front_v114. Parent queue continues using enter_main_menu_state_v114(expected,error).
For explicit retry of SAME failed command, call arm_source_main_menu_front_resume_v114(error) BEFORE parent ApplicationGoToMainMenu.resume. Arming clears terminal failure metadata without eagerly running nested bodies; original retained phases resume through the existing callbacks. Do not eagerly call the old resume helpers and then re-enter completed Ctor/Load.

Whole source kernel:
- Width854 -> dqmenus_droid.swf;960 -> dqmenus.swf.
- Width800: HTC positive -> dqmenus_i9000.swf; otherwise No_IGP positive -> dqmenus_i9000_lg.swf, else i9000.
- Other widths: actual SAME settings getLanguage once for Korean5; otherwise original second call for Japanese4; else dqmenus.swf.
- Actual MultiMenu.LoadSWFFile(slot2), behavior84, RenderFX.virtual10(1,false).
- Debug Get/real weak query; missing/dead -> genuine Init. Fresh Debug Get/weak query, then store0 into actual weak root gameswf.character.visible9b (Root get/set-visible774428/77441c confirms offset).
- MainMenu, CharacterSelect, EnterName, Lobby, Leaderboard in original order. Missing/dead CharacterSelect calls actual Init; others actual RegisterMenu.
- Capture registry count AFTER singleton repair, actual four-resource PostLoad, then live appended suffix RegisterDragAndDrops(NULL), RELOAD same live index, selected Hide. No extra MenuManager Push/Pop wrapper.
- First native failure retained; successful effect completion precedes current-owner postguards. Continuation loan metadata is weak between calls, avoiding Front->services->kernel->movie cycle. Actual existing slot/registry/singleton authorities retain resource prefixes.

Native resource behavior:
Front source_load_main_movie_v114 writes existing Game_Text/menu_string_flag BEFORE slot2 lookup. Positive retained resource returns actual SAME movie lease. Missing slot2 uses existing font/movie/resource/camera owner with original source parser load_source_resource_v98 (no eager shared preload/advance), actual source text-buffer85=true BEFORE camera C1, fresh display borrower, source flags0/root input, and existing viewport/input providers. It does not run legacy show_main, hide-all states, metadata/settings reload, base0 movie creation, menu background reload or reset_failed. Raw main slot is already published before parsing as in existing source resource owner; native binder publishes only SAME render/root projection, deferring original PostLoad to its authentic later point. New resource failure stays pinned and terminal; no synthetic repair/native D0.

Weak/singleton deltas:
Actual weak_ptr.get_ptr supplies source_live_v114, including genuine dead-proxy release/clear, without movie-generation search or invented liveness.
Existing source_singleton_v67 storage becomes shared (same records), so immediate typed receiver loans pin actual class storage.
Leaderboard is added to SAME existing singleton set; exact C142b8a0 is inherited MenuBase+RegisterMenu, name Multiplayer_Leaderboard.
CharacterSelect Init uses SAME process SourceProcessArrays.get_member_id("CharacterTable","KnightPlayerBase") after old World retirement; no second table/decoder. Exact source description cache path is s_description.text (428cc0), replacing old incorrect class_description.text.

Genuine reached requirements stay explicit:
Missing actual Init0 metadata/local settings/display/device source producer, missing selected authored movie/file, incomplete native resource prefix, absent registered positive Debug character (original would NULL-store9b), replaced source class/resource or required selected Hide fails. No fallback to droid URI or empty resource, passive expiry D0, alternate App/manager or readiness success.

Parent owns shared adoption/linkage, current queue/frame/state activation and remaining GS.Update branches, local-player cleanup and Android leaves. This packet supplies only offline original movie/local registration. Hold for Root integrated milestone after adoption.
