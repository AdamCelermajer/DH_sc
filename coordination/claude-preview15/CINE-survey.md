# CINE survey: cinematic runner and LizardMan_Intro ambush (Preview 15)

Status: READ-ONLY survey (no file under port/, docs/ or tools/ was edited). Date 2026-10-10.
Scratch: `.local-inputs/claude-preview15/cine/` (dump.py, summ.py, all-scripts.txt, s1/*.png contact sheets).
Reference: `.local-inputs/reference-video/dh2-act1/Dungeon Hunter 2 (v1.0.3) Part 1 [720p] [z_Zky7qQdYs].mp4` (640x360, 30 fps, 1331.6 s). Video is v1.0.3; recovered code is v1.0.2.

Labels: **[obs]** = seen in frames or read in code/data; **[inf]** = inference.

## 0. Summary

- The original script data is fully decoded: `port/windows-foundation/reports/encounter-source-scripts.json` (55 level scripts, 15 common, 789 commands, 29 trigger records).
- The campaign executor exists (`OriginalCampaignRuntime` + `OriginalCampaignWorldAdapter`) and already handles Wait, ExecScript, lock/unlock, kind 30 spawn, camera target (kind 8), tutorial gate (77/78) and flush, but only if providers are bound.
- The live EXE binds 3 of about 15 providers (`main.cpp` ~1463-1467). Any cinematic command that needs an unbound provider fails. No trigger zone is constructed in `main.cpp`, so LizardMan_Intro never fires in normal play (the handoff JSON records this as the production gap).
- Reference video: the Swamp opening runs about 78-172 s; the Movement, chest and combat tutorials are prompt sequences with SKIP visible and HUD hidden; the two lizards appear at about 224-226 s after the player closes the inventory. Timings are +/-0.5 s.
- Recommended design: do not write a new runner. Reuse `SourceCinematicSession` + `SourceCampaignDispatchV1` on the existing `OriginalCampaignRuntime` (one scheduler, one clock: the app dt), add one host-provider file for HUD/skip/dialog/tutorial/save/cutscene flags, and feed authored trigger contacts. Skip input is an abstract press event.
- Size: first milestone (LizardMan_Intro + CombatTuto live, with control lock, HUD hide, skip button, spawn) is roughly M+M+M in parts; full Swamp_Intro with camera animations and actor clips is L.

## A. Existing code and data

### A1. Decoded script data
- `port/windows-foundation/reports/encounter-source-scripts.json`: `scripts[]` (level, ids 0-54), `common_scripts[]` (ids 0-14), `triggers[]` (authored TriggerZone/SpawnPoint records from `.mgp`), `command_schemas` (`port/level-loader/script_data_schemas_v52.inc`), `command_classes` (`port/level-loader/command_c1_descriptors_v59.inc`).
- Command record: `kind` = scalar key "4"; other scalars are keyed by struct offset (8, 12, 16, 20, 24...); strings by offset (12 = name, 16 or 32 = second name). Kind 30 = `Script_SpawnCharacter`, kind 26 = `Script_Wait`, kind 78 = `Script_DoTutorial`, kind 77 = `Script_LockTutorial`, kind 1/2 = Enter/ExitCutSceneMode, kind 0 = ExecScript.
- Scalar 8 of SpawnCharacter (values 16, 23, 25, 26, 45) has no decoded meaning in the descriptors [inf: template or spawn variant]. The adapter ignores it.

### A2. Campaign executor
- `port/windows-foundation/original_campaign_runtime.hpp/.cpp` (98 lines): loads `original-campaign.xml`; owns the scheduler (Wait kind 26 by app dt, ExecScript kind 0, children, blocking); `trigger_contact(key,inside,qualified,module)` where key = "source file::trigger name"; services = admit_start, command, random.
- `original_campaign_world_adapter.cpp` (57 lines): `command()` accepts kinds 1,2,24,25,30,31,32,39,69,70,77,78,79 directly; kinds 4/8 go to `CampaignCameraAdapter`; kinds 10/12 dialog and 22/23 flash go through providers; every other kind returns "Unsupported original campaign command kind". `effect()` returns "Unbound original campaign provider: <name>" when a provider is empty (line 10). So unbound providers fail, they do not no-op.

### A3. Features
- `features/cinematics/`: `source_cinematics.hpp/.cpp` (SourceCinematicCommands for kinds 4,5,6,7,8,24,25,45; SourceCinematicSession: single-use session, start/tick_scripts/input/state; needs abort provider before cancel); `source_cutscene_mode` (kind 1/2 provider, PM/UI/network effects not done); `source_camera_admission`; `source_campaign_dispatch_v1` (binds cinematics/doors/audio/remaining into the same runtime); troll jump/return slices (`mob_jump_v19`, `runtime_troll_*`).
- `reports/feature-cinematics.json`: status native pass; REMAIN lists: root must bind live providers; full Enter/Exit player/UI/network flow missing; only the authored idle camera command was exercised; "no invented camera tracks".
- `features/encounters/`: trigger constructors, activation_bindings, npc_profile. `reports/encounter-activation.json`: exact trigger-to-script-to-spawn chain recovered; `live_encounter_verified: False`.
- `features/enemy_ai/authored_intro_spawn_consumer_v1.*` (bind_authored_intro_spawn_consumer_v1: validates the two authored Lizard IDs, composes SourceWorldObjects + OriginalActorLifecycle; focused test PASS). `runtime-enemy-source-spawn-admission-handoff.json`: production_gap = main does not construct the LizardMan_Intro TriggerZone in the same session; no test covers trigger contact -> script -> spawn -> AI/death as one path.
- `features/tutorials/runtime_source_tutorial_v1.*`: SourceTutorialTimelineV1::build_movement_tutorial projects the Movement_Tuto2 dialog/wait pairs (test PASS). Not enrolled live. Prompt rendering not verified.
- `report/campaign-camera-adapter.json` + `campaign_camera_adapter.cpp` (72 lines): kind 8 target transition (duration only from scalar 8, skip -> duration 0, unresolved name -> no-op, wait scalar 20). Kind 4 is a no-op.

### A4. What main.cpp does today
- Binds only `named_character` (to the source object world), `global_controller_blocked` (sets `globalControllerBlocked`) and `character_controller_blocked`, at ~lines 1463-1467 (`campaignWorld.bind`).
- `--source-command` scheduled replay only: ~lines 2472-2476, `campaignWorld.command(execute, command, ...)`, throws on any error. Diagnostic message at ~1604: "original trigger admission and complete cutscene/UI providers remain incomplete".
- Not found in main.cpp: trigger-zone construction, SourceCinematicSession, SourceCampaignDispatchV1, HUD-hide/skip-button path, tutorial gate, dialog UI binding.

### A5. Assets
- Scripts: `assets/original-cache/data/pydata/scripts/001_swamp_pyscripts.bin` (source of the JSON above; copies in `.local-inputs/*`).
- Camera: `port/windows-foundation/assets/original-cache/data/3d/camera/animations/cs_swamp_intro/cs_swamp_intro_camera_scene01.bdae` (5428 bytes). It is a binary "BRES" container, not COLLADA XML; no decoder in the port reads it yet [obs]. Feature report uses `.local-inputs/windows-camera-assets/data/animations_pyarray.bin` (72812 bytes) for the native camera clips.
- Triggers are in the `.mgp` files under `assets/original-cache/data/3d/modules/swamp/mgp/` (listed in the JSON `triggers[]`).

## B. Original behaviour

### B1. Script structure
- Common 1 `BeginScriptedCutScene` = BlockSaveGame(70), FlushMessages(79), MarkCharacterAsScripted(32), HideFlash HUD(23), ShowFlash `menu_skipcutscene`(22), LockCharacter All(24), StopActor All(39), EnterCutSceneMode(1).
- Common 3 `EndScriptedCutScene` = PutCharacterInIdle(31), WaitDialog, ShowFlash HUD, HideFlash skip, MarkScripted off, UnlockCharacter All(25), ExitCutSceneMode(2), SaveGame(69).
- Almost every cinematic is `ExecScript(common 1)` ... `ExecScript(common 3)`. This is the SKIP and HUD-hide contract [obs: matches frames, B2].
- Command totals in the level set (kind: count): Wait 26:44, StartDialog 10:117, WaitDialog 12:110, PlayActorAnim 45:127, SetActorPosition 46:47, LookActor 41:45, PlayCamera 5:40, SetCameraTarget 8:35, LockCharacter 24:31, UnlockCharacter 25:32, SpawnCharacter 30:12, PlayEffect 20:16, ShowFlash 22:11, HideFlash 23:13, DoTutorial 78:3, LockTutorial 77:3, SetFaeryState 27:1, KillActor 44:1, SaveGame 69:1, BlockSave 70:1.

### B2. Triggers (level .mgp TriggerZone records)
| Script | Trigger prim (file) | Notes |
|---|---|---|
| LizardMan_Intro | `_prim_TriggerZone_LizManIntro` (obj_3of4_brdwalk_sw_00.mgp), pos (3102.95, -3.40, 256.74), scale (1, 5.55, 1) | triggercount 1, delay 0, no activate_cond |
| LizCaptain_Intro | `_prim_TriggerZone_BigLizIntro` (corner_ruin_ws_00.mgp) | triggercount 1 |
| LizardMan_Ambush1 | `_prim_TriggerZone_001_ChestAmbush` (obj_1of4_brdwalk_nse_00.mgp) | spawns `_prim_Monster_54_chest1/2` (kind 30, param 23) |
| LizardMan_Ambush2 | none in trigger table | spawns `_prim_Monster_3_312_355_...` (param 45): starter unknown |
| MothIntro | `_prim_MothMovieTriggerZone` (obj_3of4) | "movie" is camera 334 + sound 363 |
| Camp_Intro | `_prim_TriggerZone_Camp_Intro` (merchantcamp_ruins_swe_00.mgp) | triggercount 1 |
| enterLocation_MerchantCamp | `_prim_Zone_EnterLoc_MerchantCamp` | triggercount -1 |
| tutorial_treasure | `_prim_TriggerZone` (obj_4of4) at (-2033, 255, 288) | DoTutorial(chest_tuto) |
| TrollReturn | `_prim_TriggerZone_001` (obj_1of4 / 4of4) | |
| Swamp_Intro | none in trigger table; no ExecScript in any level script targets id 30 | starter not identified [open] |
| Movement_Tuto / Movement_Tuto2 | none; Movement_Tuto is DoTutorial(6) to Movement_Tuto2 | starter not identified [open]; Swamp_Intro has no DoTutorial |

Start conditions (`activate_cond`) and script preconditions are not evaluated by the extraction (see JSON notes).

### B3. Swamp_Intro (164 commands, script 30)
- Setup: spawn `_prim_ActorTroll`; place Player, Faery, prisoner, troll, priest at `_prim_Waypoint_5`; look-at; hide priest and Faery; cage `idle` anim; lock All; UnEquipHands; SetCameraClip 10000.
- Body: PlayCamera 359-376 with blocking flag 1 on several; PlayActorAnim 377-445 (Player, troll, prisoner, priest, Faery); PlayEffect 116-120 (flashes at `_prim_Waypoint_5`); cage PlayAnimByName `shake` / `open`; 3 dialog groups (dialog ids 2031617-2031630, "Is he already dead?", "He's dead alright", "Oh! It's a miracle", "Sorry to disappoint", "A noise like that", "Then we haven't much time", "Alright, you win").
- End (commands 137-163): ShowActor troll, camera to `_prim_Waypoint_CamTroll`, PlayActorAnim 1380 on troll, unlock, PutCharacterInIdle, teleport LocalPlayer to `_prim_Waypoint_test`, Faery/priest move, troll to `_prim_Waypoint_TrollStack` then PutCharacterInLimbus (155), **SetFaeryState(slot 0, 1) at command 156** (Celest unlock), ShowActor prison zone, StopEffect 116/117, StopSound 365/366, Unlock, EndScriptedCutScene (163).

### B4. LizardMan_Intro (12 commands, script 17)
Begin (common 1); LockCharacter All; SetCameraTarget 1000 ms to `_prim_Waypoint_NewCamSpot`; Wait 500; SpawnCharacter kind 30 `_prim_Monster_LizManIntro1`; Wait 1500; SpawnCharacter kind 30 `_prim_Monster_LizManIntro2`; Wait 2000; UnlockCharacter All; SetCameraTarget 1000 ms to LocalPlayer; End (common 3); DoTutorial(10, 7) `CombatTuto`.
- Both lizard Character declarations: ai_state Limbus, auto_spawn 0 (PreSpawn17). Spawn command: PreSpawn17 -> Spawn1 (blur in, enable, revive, collisions on); Spawn sequence; event34 -> Idle3 (`runtime-enemy-source-spawn-admission-handoff.json`, from IDA `Script_SpawnCharacter::Execute` 0x45f400).

### B5. Ambush / tutorial timing observed in the reference
Observation method: ffmpeg contact sheets at 10 s, 2 s and 0.5 s steps (`.local-inputs/claude-preview15/cine/s1/`). Times are sample times; +/-0.5 s.

| t (s) | What is on screen | HUD | SKIP | Class |
|---|---|---|---|---|
| 0-8 | Gameloft logo (black at 0 and 8) | no | no | obs |
| 10-34 | Intro movie: parchment/cartoon panels (knights, map, burning town, sorcerer). Sampled every 2 s; no skip control seen | no | none seen | obs (50.3 s repo asset vs ~25 s here: see G5) |
| 36-40 | "Dungeon Hunter" title, "(c) 2011 GAMELOFT" | no | no | obs |
| 42-48 | Main menu (Start game), Enter name (CRISR82) | no | no | obs |
| 50-60 | Choose a class (Mage / Rogue / Warrior); Warrior confirmed | no | no | obs |
| 62-68 | Main menu; Single Player / Multiplayer / Achievements sub-menu | no | no | obs |
| 70-76 | LOADING: tip "In an area is blocked by traps, look around, you should find a way to open up the path"; red bar fills | no | no | obs |
| 78 | "The Boglands - Ancient Prison" title card over the cell | no | no | obs |
| 80-172 | Swamp_Intro: cage, troll, Rene, Celeste, Crisr. Subtitles: "Is he... already dead?" (104-106), "He's dead alright" (108-112), "Oh! It's a miracle!" (116-124), "W... what happened?" (122-124), "Friends, Crisr82" (126-132), "Sorry to disappoint..." (134-136), "Now get out of here" (138), "You fool!" (148-150), "A noise like that, and every bogwomp" (152-154), "Then we haven't much time" (156-160), "Alright, you win" (162-164), "Now let's get going" (166-168). Bogwomp walks by 170-172 | hidden | yes, every frame | obs |
| 174 | HUD back; banner "New Quest: Escape the swamp with Rene and Celeste" | yes | no | obs |
| 180-190 | Movement tutorial: title "Movement Tutorial" (180); "If you've set your controls to the Virtual Stick, you can use it to move around" (182); "If you're using the Touch controls, simply tap the area you'd like your character to move to" (184-186); "You can change your control type in the Options screen, accessible from the pause menu" (188) | hidden to ~190 | yes to ~190 | obs |
| 192-204 | "Treasure Chest Tutorial" (194); "To open a chest, stand next to it and tap the chest icon" (198); "Chests contain valuable items and equipment which will help you survive the game" (200-202); "Keep an eye out for Epic Chests, which contain superior equipment" (204) | visible 192, hidden 200-204 | yes | obs |
| 206-221 | Inventory screens (Torso, Right hand); user UI | hidden / visible | varies | obs; user-driven [inf] |
| 222-223.5 | HUD visible in the dungeon (222-223); HUD gone at 223.5 | yes then no | no at 223.5 | obs |
| 224 | SKIP appears (BeginScriptedCutScene) | hidden | yes | obs |
| 224.5-226 | Lizard 1 visible at top of wall (224.5); Lizard 2 visible (~226). Lizards are small green figures above the corridor wall; no clear jump frame. | hidden | yes | obs; "jump" not seen in stills |
| 227 | "Combat Tutorial" title card | hidden | yes | obs |
| 228-234 | "Red circles will appear underneath enemies within attack range" (228-230); "When wielding a melee weapon, tap the attack icon to attack the enemy!" (232-234) | hidden | yes | obs |
| 236-258 | "You will continue to attack the target until you move or the target is destroyed" (236-238); "When wielding a ranged weapon, tap or hold the attack icon" (240-242); "To stand still while firing in different directions..." (244-246); "Some enemies may be able to break your attack with counterattack" (248-250); "Use the attack icon again to resume attack" (252); "Mix up your regular attack with special abilities..." (254-258) | hidden | yes | obs |
| ~260 | HUD back, SKIP gone, Bogwomp combat starts (260-270) | yes | no | obs |
| ~286-292 | "Both HP and MP will be replenished" (cinematic_Tuto_potionUse), HUD hidden, SKIP visible (10 s sample only) | hidden | yes | obs, coarse |

Timing check [inf]: if trigger contact is at about 223.5-224 s, Spawn1 at +0.5 s and Spawn2 at +2.0 s match the observed ~1.5 s gap. The CombatTuto caption at 227 is earlier than script time +4.0 s (the unlock point), so the script-to-video mapping is not exact. Needs a frame-accurate pass.

### B6. Control, HUD, skip
- Control lock: LockCharacter All (kind 24) at each script start; the player does not move in the cutscene frames [inf from stills]. Unlock at end.
- HUD: hidden by HideFlash HUD in BeginScriptedCutScene, restored by EndScriptedCutScene (confirmed at 174, 190, 260).
- SKIP: red X + "SKIP" bar at top-left, shown by ShowFlash `menu_skipcutscene` from BeginScriptedCutScene; hidden at End. Visible in Swamp_Intro, tutorials and the lizard ambush.
- **Skip effect: not observed.** The reference contains no tap on SKIP. Recovered source semantics (feature report): skip flag is sampled per command; PlayCamera with skip calls camera play_idle; SetCameraTarget with skip forces duration 0; LockCharacter with skip exits early; Wait skip is intentionally ignored (recovered Wait45c438). No universal end-of-scene shortcut is recovered.

### B7. Camera
- Swamp_Intro uses ~18 PlayCamera ids (359-376) plus 55 (end, idle) and `SetCameraClip 10000/900/5000`. The cs_swamp_intro BDAE holds the scene camera. Mapping of ids 359-376 to clips in cs_swamp_intro or in `animations_pyarray.bin` is NOT established [open].
- Lizard ambush camera: SetCameraTarget to `_prim_Waypoint_NewCamSpot` (1000 ms), back to LocalPlayer. The top-down framing in 224-234 is consistent [inf]; not checked against the waypoint.

## C. Gaps (production)
1. Providers: only 3 of ~15 bound in main.cpp. Unbound: cutscene_mode (kind 1/2), flash (22/23), dialog (10/12), block save / save (69/70), flush (79), mark scripted (32), idle gate and set idle (31), stop actor (39), tutorial gate / consume / save settings (77/78). Kinds 41/42/43/46 (look, show/hide actor, set position) are not accepted by the adapter at all (see C2). The first `BeginScriptedCutScene` of any level script fails with "Unbound original campaign provider: cutscene mode" (inference from effect() + main.cpp).
2. Kinds the world adapter does not accept (5 PlayCamera, 13/14 sound, 19/21 anim/stop effect, 20 PlayEffect, 27 SetFaeryState, 40 MoveActor, 45 PlayActorAnim, 46 SetActorPosition, 51/52 hands, 54/55 doors, 63, 68) must come from `SourceCampaignDispatchV1` / `SourceCinematicCommands`, which main does not construct.
3. Triggers: no TriggerZone is built or fed `trigger_contact` in main.cpp. LizardMan_Intro, Camp_Intro, MothIntro, chest/tutorial triggers are all dead in normal play.
4. SourceCinematicSession not constructed; needs an abort/restoration provider before cancel/quarantine.
5. Tutorials: no gate provider, no persistent tutorial flags (LockTutorial 6/7/8 + DoTutorial 6/7/8/10 are data only). No live prompt box in the renderer (runtime_source_tutorial_v1 is a projection). Dialog text box not bound.
6. HUD hide and SKIP button: no code path found in main.cpp or the HUD for `HUD` / `menu_skipcutscene` flashes.
7. Spawn: `OriginalActorLifecycle.spawn` path exists and is unit tested; no live spawn animation verified (`live_encounter_verified: False`). Spawn clip playback depends on the actor animation owner.
8. Camera: PlayCamera (kind 5) playback for cs_swamp_intro not wired; BRES binary of cs_swamp_intro has no decoder; only the authored idle camera command is exercised.
9. Starters unknown: Swamp_Intro, Movement_Tuto, LizardMan_Ambush2 (no TriggerZone reference, no ExecScript).
10. Skip semantics: unverified against the reference.
11. Quest banner "New Quest" (174) needs the quest runtime (not coded in Windows, per Preview 14 plan).

## D. Design (minimal, reusable, portable)

Principle: one scheduler (`OriginalCampaignRuntime`), one clock (app dt already computed in the main loop, clamped, no platform timer), abstract input (press event in virtual-screen coordinates), no Windows API outside the platform layer.

1. **Host providers** (new file, e.g. `features/cinematics/cinematic_host_v1.{hpp,cpp}`, pure C++17): fills `OriginalCampaignWorldProviders` and `SourceCampaignDispatchServicesV1` with explicit small services:
   - `hud_visible(bool)` for flash "HUD"; `skip_button(bool)` for flash "menu_skipcutscene"; these are plain flags read by the HUD draw and the input hit test.
   - `cutscene_mode(enter,skip,module)`: v1 sets the flag only (player/UI/network side effects remain explicit TODO and must fail if reached).
   - `control_lock(all|name,bool)`: reuse `globalControllerBlocked` and `characterControllerBlocked`.
   - `dialog(kind 10/12)`: reuse the existing retained dialog owner (`port/engine-ui/menu_dialog_messages_v97.hpp`, per the tutorial report).
   - `tutorial_gate` / `consume` / `save_settings`: persisted tutorial flags (save-schema dependent, see F).
   - `block_save` / `save_game`: forward to the save owner; stub with explicit error until schema v4 (F).
   - Unimplemented providers keep returning explicit errors (no no-op success), per the "no fabricated values" rule.
2. **Session**: reuse `SourceCinematicSession` (start, tick_scripts, input, state) and `SourceCampaignDispatchV1::bind`. Main owns one session per cinematic; one accepted run at a time; abort provider = restore HUD/lock/skip flags and clear dialogs.
3. **Skip input**: the skip button rect is in the HUD layout; a press inside it calls `session.input(CinematicInput::skip)`. The result sets the source skip flag for later commands only (recovered semantics). No automatic end-of-scene shortcut until the user decides (G1).
4. **Triggers**: build TriggerZone records from the level MGP (existing `trigger_constructors`, `CanonicalTriggerZoneV22`, `activation_bindings`); each frame, feed `OriginalCampaignRuntime::trigger_contact("obj_3of4_brdwalk_sw_00.mgp::_prim_TriggerZone_LizManIntro", inside, qualified, module)`. Key format is the existing one.
5. **Spawn**: kind 30 -> `named_character` -> `OriginalActorLifecycle.spawn` (exists). Spawn clip via the actor animation owner (PreSpawn17 -> Spawn1 -> Idle3). Do not schedule by frame.
6. **Camera**: kind 8 via `CampaignCameraAdapter` (exists). Kind 5 PlayCamera: idle first (exists), then authored clips after a BDAE decoder is chosen (G6).
7. **Tests** (focused, before coding): (a) LizardMan_Intro over the real 001_swamp data: trigger contact once -> spawn 1 at +500 ms, spawn 2 at +2000 ms (script time), unlock at +4000 ms, CombatTuto started once; (b) BeginScriptedCutScene sets HUD off, skip on, lock on; End restores all; (c) duplicate contact while running is ignored (triggercount 1); (d) skip press sets flag, unbound provider fails with a named error; (e) EXE: isolated save, `--frames` + `--capture` at 224-228 s equivalent, compare to reference frames.

## E. Work breakdown (S < 1 day, M 1-3 days, L > 3 days; relative sizing)

| # | Item | Size | Files | Depends on |
|---|---|---|---|---|
| E1 | Host provider file (HUD/skip/lock/cutscene flags/dialog/tutorial stub) + unit tests | M | new feature file + tests | none |
| E2 | Main wiring: bind providers, HUD draw flag, skip button draw + hit test, session construction | M | main.cpp (small hunks), HUD | E1 |
| E3 | Trigger construction for LizardMan_Intro (+ Camp_Intro, MothIntro) and per-frame contact feed | M | encounters/*, main.cpp | E2 |
| E4 | Lizard spawn in the live session, Spawn clip playback | M (L if actor anim owner must be extended) | enemy_ai, actor lifecycle | E3, actor owner |
| E5 | CombatTuto / Movement / chest tutorials: gate, persisted flags, dialog text box | M | tutorials, dialog UI, save | E1, F1 (schema v4) |
| E6 | Save points (BlockSave/SaveGame) and SetFaeryState route | S | save, faery | F1, F2 |
| E7 | Camera PlayCamera clips (cs_swamp_intro BDAE decode, ids 359-376) | L | camera asset decode | BDAE decoder decision |
| E8 | Full Swamp_Intro: actor anims 377-445, effects 116-120, cage anims, dialog groups | L | session + actor animation | E2, E4, E7 |
| E9 | Skip semantics decision and abort/quarantine tests | S | session tests | G1 |
| E10 | Verification: EXE frames at ambush, tutorial prompt frames vs reference (+/-0.5 s) | S-M | tools/verify | E4, E5 |

Recommended first milestone: E1, E2, E3, E4 (LizardMan_Intro end-to-end with CombatTuto prompts after E5). This closes the production gap in `runtime-enemy-source-spawn-admission-handoff.json`.

## F. Dependencies

- **Preview 14 (schema v4 first)**: F1 schema v4 (slot metadata, quest counters, difficulty) is needed before BeginScriptedCutScene's BlockSave and EndScriptedCutScene's SaveGame can write real saves (E6). Until then, stubs with explicit errors.
- **Quests**: "New Quest" banner (174 s) and any FirstQuest/Quest counters need the quest runtime (Preview 14 plan: not coded in Windows). Cinematic runner can ship without the banner; mark it as a gap.
- **Faery**: Swamp_Intro command 156 (SetFaeryState 0,1) is the only unlock; shared with the FAERY stream T3 provider (`set_faery_state`). Do this once.
- **Drops / chests**: chest_tuto and tutorial_treasure reference `_prim_OpenableContainer_2` and the Epic chest prompts; the chest/drop/pickup work (P14 Drops, P15 item 4) must exist for the chest tutorial to be meaningful.
- **Equipment**: UnEquipHands/ReEquipHands (kinds 51/52) in Swamp_Intro need equipment visuals (existing `equipment_visual`). Inventory is user-driven in the reference.
- **Actor animation owner**: Spawn sequence and PlayActorAnim 377-445 need the live actor FSM (`actor_state`, `actor_movement`); verify before E4.
- **Startup stream (P15 item 1)**: Swamp_Intro starts after the loading screen (70-78 s). The starter is still unknown (G4).
- **Combat (existing)**: the lizards are generic CombatSession actors (`runtime_enemy_controller_v1`); no new combat work is needed for the spawn.

## G. Questions for the user

1. **Skip**: should SKIP (a) only shorten camera transitions and skip the remaining command flags (recovered source semantics, not seen in the reference), or (b) end the cutscene at its EndScripted point? The reference never shows a skip press, so the original behaviour is unverified.
2. **Scope order**: should the first milestone be LizardMan_Intro + CombatTuto only (closes the production gap), then Movement/chest tutorials, then full Swamp_Intro? Or Swamp_Intro first?
3. **Tutorials on PC**: the recovered gate requires normal difficulty, the tutorial flag enabled, and offline. Keep that gate (tutorials show for a new normal-difficulty offline character)?
4. **Starters**: may I spend one IDA pass on who starts Swamp_Intro, Movement_Tuto and LizardMan_Ambush2 (level start, new game, quest hook)? This decides when the opening plays.
5. **Intro movie length**: the repo asset is 50.3 s; the reference shows the movie at about 10-35 s and no skip control. Is the 50 s asset the intended one? (Startup stream; flagged here because it affects the opening.)
6. **Camera fidelity**: decode the BRES camera clips for the original camera paths (more work, L), or accept an approximate camera for PC v1 with a logged limitation?
7. **Spawn look**: the stills show the lizards emerging above a wall; no clear jump frame. Is "authored Spawn clip, as recovered" enough, or do you want a frame-accurate comparison (needs a 0.1 s sample of 223-227 s)?

## H. Not verified (explicit)
- No code was built or run. All facts are from reading code, JSON reports, and frames.
- Reference timings are +/-0.5 s; the reference is v1.0.3; recovered code is v1.0.2.
- Lizard "jump": not seen in stills; spawn animation pose unverified.
- Skip behaviour: not observed.
- Camera ids 359-376 to clips: not mapped.
