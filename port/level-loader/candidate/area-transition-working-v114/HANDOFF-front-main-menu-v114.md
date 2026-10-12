# Retained Front / MenuManager main-menu source integration V114

Completed source-only. No tests, compiler, build, hash, emulator, or runtime claim.

Adopt menu_main_menu_state_v114.hpp/.cpp into the existing portable source target (one additional TU). Adopt native_main_menu_front_v114.inc beside native_app fragments, included AFTER native_menu_fs_command_v114.inc and BEFORE native_source_main_menu_v114.inc. Apply front-main-menu-state-v114.patch to the CURRENT Front pair; the candidate Front pair is context only and must not replace newer Root edits wholesale.

Public actual Front APIs:
- bind_main_menu_state_services_v114(ui::GSFlashMenuServicesV114,error)
- borrow_main_menu_state_v114(loader::MainMenuStateBorrowV114&,error)
- enter_main_menu_state_v114(expectedState,error)
- resume_main_menu_state_v114(error), explicit same failed entry continuation
- update_main_menu_request_v114(error), pending-menu prefix BEFORE sole existing MenuManager frame Update, ONLY while SAME GSFlashMenu is active.

SAME Front Impl embeds native-width manager4/current8/request_c, empty inline CString10[16], zero source28/source29. Source C1 3841b4 is 104 bytes. No competing singleton, process App, menu directory, World, movie, or clock is created. Borrow pins only SAME Front Impl. Independent services/native provider retain weak containing process App/NativeCharacterMenu; resource authority may be strong only if genuinely independent. No old World is captured.

Native fragment:
- bind_source_main_menu_front_v114(independent_resource_owner, actual_LoadMainMenu_callback,error): enroll once on actual existing process directory before return requests.
- reset_source_main_menu_manager_v114(manager_identity,error)
- get_source_main_menu_v114(manager_identity,name,identity_out,error), NULL is authentic successful miss.
- resume_source_main_menu_reset_v114(error): explicit recovery; no-op if no failed Reset.
Parent continues owning request queue/frame, V88 old-GS retirement barrier, and native state activation. Explicit application-resume should resume a failed Reset/Front entry before resuming the SAME command. Completed native callback effects are retained before post-callback current-owner failure gates, preventing replay.

Authentic source:
MenuManager.Reset begins 42d73c (152 bytes); 42d7c8 is ONLY final listener88=1 store. For slots0..3 reborrow actual MultiMenu134[i]; positive slots MenuFX.PopAll7abb20, absent slots skipped. Capture signed GetNumMenus42cb38 once. For each reached live registry64[index], read actual MenuBase.IsVisible74 (41f3f4); if visible RELOAD live registry index and dispatch selected virtual10 Hide. Then actual listener88=1. No Unload/deleting callback/movie recreation/registry erase. Existing PopAll helper preserves source stack fields and SetContext.

GSFlashMenu.Ctor3846fc: DebugSwitches.Load -> GetSwitch("isTracingGSFlashMenu") ignored native switch result -> MenuManager.GetInstance -> manager4 store. If29 positive: original LoadMainMenu4324dc -> fresh GetInstance -> GetMenuByName("menu_MainMenu")42d1f0 -> request_c store (NULL valid) ->29=0. Positive current8 then SAME MultiMenu virtual34 PushMenu(actual MenuBase*), finally28=0. Native fragment uses direct dh2_menu_stack_push_v1, not extra MenuManager.PushMenu wrapper.

GSFlashMenu.Update384460 request prefix: if c positive and differs8, positive old8 MultiMenu virtual3c PopMenu(old.name8,false); RELOAD c after callback; c=0,8=reloaded; direct MultiMenu PushMenu. Fragment uses pop_named_direct (439270), not MenuManager.PopMenu wrapper. Existing frame owns remaining GS update/online/base-state/pop logic; this helper neither certifies state top nor synthesizes it. Request failure latches on SAME Front; no later frame silently treats cleared c as completion.

Remaining genuine reached Main resource leaf:
actual_LoadMainMenu_callback(manager,error) must complete MenuManager.LoadMainMenu4324dc. Front.load_main_movie_slot_v93 alone is insufficient. Original MultiMenu.LoadSWFFile437d68 DOES return retained positive slot2 directly, but first writes its actual process loading-main flag; missing slot requires real RenderFX C1/Load/camera publication. LoadMainMenu selects authored URI using source Width_Screen (854/960/800), real HTC/noIGP and SAME Save.getLanguage; sets RenderFX flags84 and calls virtual10(1,false); repairs original weak singleton roots in order Debug, Debug again (byte9b=0), MainMenu, CharacterSelect, EnterName, MultiplayerLobbyMulti, Leaderboard; runs manager PostLoad and RegisterDragAndDrops(NULL)/selected Hide for newly appended live registry suffix. Missing MenuLeaderboard or weak release/resources must remain a reached required leaf. Do not substitute movie presence, legacy Front.load_front_screen, or reset_failed.

Shared sources were only read/copied to private context. Automatic review first misclassified the combined read/write command as shared writes; that command did not execute. Explicit read-only copy followed by explicit private-only edits succeeded.
