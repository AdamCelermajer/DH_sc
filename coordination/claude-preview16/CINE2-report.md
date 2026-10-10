# CINE2 report (Preview 16): camera clips, actor clips, SKIP/caption art trace, chest trigger

Branch `p16/cine2` (worktree `DH_wt/p16cine2`), base `p16/cine` (826837e5). Build `DH_wt/build-p16cine2/dh-foundation.exe`. Not pushed. Commits: 706d2225 (skeleton), 2d005cb4 (camera clips), 63db2847 (host PlayCamera test), 63b515a0 (rotated trigger zones), 1d3c774e (SKIP/caption placeholder geometry), dccd4551 (adapter test expectation).

## Status (one paragraph)
Done and verified in the real EXE: PlayCamera (kind 5) plays the original cs_* camera clip on the level camera scene with blocking, the follow camera resumes afterwards, and the chest tutorial zone (rotated Block) starts its tutorial by walking/placing the player into the zone (contact), not by a harness flag. Not done: actor clip playback (PlayActorAnim 45 and the actor show/hide/look/move verbs). The Swamp opening does not get past command 1 (SpawnCharacter of `_prim_ActorTroll`, the lifecycle branch not merged), so its camera clips and actor clips are never reached in this branch. The SKIP and caption art is still labelled placeholder art, now at the measured original layout.

## 1. Investigation (evidence)

### 1a. Camera clips (PlayCamera, kind 5)
- Visual: reference contact sheets show the Swamp intro as a cage/prisoner scene with dialogue; the cs_swamp_intro camera clips frame that scene (see section 4). Timings from `coordination/claude-preview15/CINE-survey.md` B5 (+/-0.5 s).
- IDA `Script_PlayCamera::Execute` 0x460110 (`.local-inputs/ida-apk-export-2026-10-07/.../pseudocode-all.c` line 248409): clip id = command scalar 8; with SKIP the level set's own clip plays instead. Calls `CameraLevel::PlayAnim(level, id, 0, 0)`.
- `CameraLevel::PlayAnim` 0x40f904 (line 195947): sets the playing flag CameraLevel+132 = 1. `CameraLevel::__Callback` 0x40f8f8 (line 195940): completion clears it.
- `Script_PlayCamera::IsBlocking` 0x459370 (line 244453): blocks while command byte +12 (scalar 12) is set AND the playing flag is set.
- Dictionary: scalar 8 is a direct index into `animations_dictionary` (id 359 = `cs_swamp_intro_camera_scene01`, verified by parsing the length-prefixed names blob).
- The clip BRES has no camera graph: its channels (`PlayerCamera_Default-node-translation`, `-camera-xfov`, `PlayerCamera_Default.Target-node-*`, `root_camera-*`, `upvector-*`) bind by name to the level camera scene (`data/3D/camera/CameraTests.bdae`, node `PlayerCamera_Default`). This matches the original (the animator drives the level camera nodes).
- Decoded: scene01 duration 4000 ms; eye (-3743,-1428,3559) -> (-437,-2349,878), target (-2131,111,300) -> (762,111,141).

### 1b. Actor clips (PlayActorAnim, kind 45) - decoded, not implemented
- `Script_PlayActorAnim::Init` 0x45a2ec (line 245174): actor by name (string 24); adds dictionary clips scalar 8 and 12 to the character's CharAnimator set.
- `Script_PlayActorAnim::Execute` 0x45e890 (line 247631): writes those clips into AnimTable rows (scalar 16 + 2 and + 8) and calls `CharStateMachine::SM_SetAnimState(state=row+2)`.
- `IsBlocking` 0x459628 (line 244582): blocks while the actor is still in that state.
- The clips are per-character BDAE files (e.g. `data/3D/characters/troll/animations/cs_swamp_intro_troll_scene01.bdae`, dictionary 442). The port's combat session only exposes object clips (`play_object_clip`) and attack-style actor sequences that need the clips preloaded in the actor visual. See gap G2.

### 1c. Rotated trigger zones
- `Zone::IsInside` 0x3970c8 (line 111871): with no physics shape it tests an axis-aligned box; with a shape it probes the physics block shape (the rotation applies). `TriggerZone::Update` 0x39b458 (line 114849), `TriggerZone::InitPost` 0x39c0a8 (line 115288) delegates to the base trigger.
- Data: `tutorial_treasure` = `_prim_TriggerZone` in `obj_4of4_brdwalk_sw_00.mgp`: position (-2033.31,254.846,287.618), scale (12.2957,1,1), rotation (0,0,-90). Half extent 100 x scale per axis (the source Block size, same rule as before), so the long axis runs along world Y.
- Convention: the placed transform that `actor_definitions.cpp transform()` builds (validated for every placed object) is used; the box axes are its columns, half extent 100 x |column|.

### 1d. SKIP button and caption box (ART/HUD rule)
- Art is in the package: `data/menus/dqhud_droid.swf` (FWS, uncompressed). The string `menu_skipcutscene` is a PlaceObject name on the root timeline: root depth 389, character 737 at (37.75, 74.85) px (matrix in twips, 1 px = 20 twips).
- Sprite 737 labels: `Idle` (frame 0), `Show` (frame 1). Frames 1 to 13 slide the child `btn_MENU_SKIP` (736) from about (-238, -68) px to (-43, -69) px relative (the slide-in); frame 14 and Idle rest at (-42.65, -68.85) px, i.e. absolute about (-4.9, 6.0) px.
- Sprite 736 labels `idle`, `pressed`, `release`. Children: shape 732 (DefineShape, bounds 10.2..187.3 x 3.4..35.4 px, bitmap-filled: fill kind 66, bitmap id 1), shape 734 (DefineShape2, the X icon, bounds 8.3..59 x -6..48 px, bitmap-filled), sprite 733 `flush_text`, dynamic EditText 735 `text` at (61.9, 10.3) px, 16 px, colour (255,255,204), font 7.
- Caption box: root placement `tutorials_dialog` (char 731) at (4.15, 319.8) px -> sprite 730 `dialogBox` (labels `show` 0, `hide` 18) -> sprite 729 `dialog` -> EditText 728 `text` at (2,2) px (htmlText set at run time by AS, `onTutorialMessage`/`showDialog`/`skipTutorialMessage` in the HUD ActionScript). No box background shape exists in this chain; the box look is not decoded.
- Why not drawn: the SKIP shapes and the caption use bitmap fills and runtime HTML text. The port decodes dqhud shapes into code only for the faery/potion icons (`features/generic_skills/pc_gameplay_hud_source_art_v1.cpp`). Decoding these bitmap-filled shapes needs the HUD texture atlas mapping, which is not done (gap G4).
- Reference: Part 2 contact sheet `sheets2/s010.jpg` (P2 3:36-3:56, the same control and caption style): red X at the top left, light "SKIP" beside it; a full-width translucent caption band along the bottom with a name plate above the centre for named speakers. Part 1 SKIP/caption timings from the survey B5/B6 (SKIP 80-172, 180-204, 224-260 s; captions 104-164, 192-204, 228-258 s). I did not re-sample Part 1 frames for the art in this session.

## 2. Implementation

- `port/windows-foundation/original_camera_clip.{hpp,cpp}` (new, in `foundation_data`): `CameraClipLibrary` (dictionary id -> clip bytes, level camera scene bytes; dictionary parsed once) and `OriginalCameraClip` (loads the level camera scene, `select(PlayerCamera_Default)`, loads the clip `Player` with MissingTargets::ignore, samples at start+elapsed clamped to end, eye and target from `eye_and_target`).
- `original_campaign_world_adapter.{hpp,cpp}`: new provider `camera_clip` for kind 5 (routed like the dialog provider; unbound stays an explicit error).
- `features/campaign_host/campaign_host.{hpp,cpp}`: provider implements Execute (SKIP sampled -> no clip, follow kept; otherwise load and start, a new clip replaces the old), IsBlocking (`clip active && scalar 12 && same id`), Update (no-op). `advance_camera_clip` runs in `frame()` before the executor tick (same clock). Completion clears active (follow resumes). Abort clears the clip. `source_camera_pose(follow)` returns the follow pose with the clip eye/target while active.
- `main.cpp` (small CINE2 hunks): `CameraClipLibrary` bound to the host service (scene file from `originalCamera.config().file`); the three source-camera sites use `campaignHost.source_camera_pose(originalCamera.pose())`.
- `features/campaign_host/trigger_zones.{hpp,cpp}`: `TriggerZoneBox` gains centre/axes/half; `oriented_zone_box(placement)`; `point_inside` tests the oriented box (identical result for unrotated zones); the rotation rejection is removed.
- `features/cinematic_runner/cinematic_runner.{hpp,cpp}`: placeholder SKIP (red X block + label at the measured top-left position) and caption band (full-width translucent band along the bottom), both labelled placeholders.
- CMake: `original_camera_clip.cpp` in `foundation_data`; test `original_camera_clip_tests` (features/cinematics).

## 3. Isolated tests (all pass)
- `original_camera_clip_tests` (new, real package): dictionary id negative/out-of-range rejected; 359 -> scene01; scene binds to the level scene, duration 4000 > 0; samples at start/middle/end finite, camera moves, end clamp; unloaded and non-BRES rejected.
- `campaign_host_tests`: new `camera_clip_blocks_then_releases`: execute (non-blocking start), is_blocking true, clip active, source pose differs from follow, still blocking at 2000 ms, released after 4000 ms, follow pose restored, non-blocking clip never blocks, unknown id is an explicit error and leaves no clip. Existing contracts unchanged.
- `campaign_trigger_zone_tests`: rotated box for tutorial_treasure (half extents 1229.57/100/100, long axis along Y, narrow along X, AABB bounds). Swamp and Android level checks unchanged and passing.
- `cinematic_runner`: passes with the new placeholder geometry (roles unchanged).
- `original_campaign_world_adapter`: one expectation updated (kind 5 now reports `Unbound original campaign provider: camera clip`, still an explicit error).
- ctest (118 tests): all pass except `session_skill_binding` (allowed).

## 4. Integrated runtime verification (quiet, hidden, parallel; `DH_sc/.local-inputs/claude-preview16/cine-runs/`)

| Job | What it shows | Result |
|---|---|---|
| cam-base-f300 | follow camera, no clip | knight in front of the swamp (baseline) |
| cam-f150 (replay of PlayCamera 359 at frame 100, t about 0.8 s) | `[campaign] PlayCamera id=359 ... duration_ms=4000 blocking=1`, frame `cam-f150/frame-half.png` | clip view: ruins, no follow pose |
| cam-f250 (t about 2.5 s) | `frame-half.png` | cage with figures and a pedestal, the Swamp intro framing |
| cam-f340 (near end) | `frame-half.png` | cage and glowing pedestal figure |
| cam-f420 (after end) | `PlayCamera finished id=359 frame=350`; frame matches baseline | follow camera restored |
| zone-inside (player placed at the zone centre, no harness flag) | `trigger _prim_TriggerZone -> script tutorial_treasure started`; DoTutorial -> `chest_tuto`; `frame-half.png` | caption band "To open a chest, stand next to it and tap the chest icon.", SKIP block top left, HUD hidden |
| zone-outside (player 200 units west) | zone fed (`min=-2133.31,-974.72,187.62 max=-1933.31,1484.42,387.62`), no trigger | no contact, no tutorial |
| swamp-now (`--campaign-start Swamp_Intro`) | BeginScriptedCutScene, then `cutscene aborted: Unknown original lifecycle actor` at command 1 (SpawnCharacter `_prim_ActorTroll`), restored HUD/SKIP/locks/cutscene mode, session continues | blocked at spawn (documented below) |
| lizard-zone (player at LizManIntro zone centre) | `trigger _prim_TriggerZone_LizManIntro -> script LizardMan_Intro started`, SetCameraTarget resolves, command 4 `SpawnCharacter` aborts `Unknown original lifecycle actor`, restored | blocked at spawn (documented below) |

The frames were viewed (half-size PNGs). The camera frames show the clip's framing differing from the follow camera, and the restored frame matching the baseline.

## 5. Gaps (explicit)
- **G1 Swamp opening not completed.** Command 1 of `Swamp_Intro` is `SpawnCharacter _prim_ActorTroll`, which needs the lifecycle branch (`p16/spawn`, `p16/lifecycle`), not merged here. The camera clips (command 29 onward) and actor clips (25 onward) are therefore never reached through `--campaign-start`. Proof of the camera path comes from a replay of command 29 (`--campaign-command Swamp_Intro:29:100`), which runs the same production provider. Lizard ambush is blocked at the same spawn kind (command 4).
- **G2 Actor clip playback (PlayActorAnim 45) not implemented.** Needs a per-actor clip owner: load the dictionary BDAE into the actor visual, write AnimTable rows, and play the state with the IsBlocking rule (blocks while in the state). The combat session does not expose this for actors (only object clips and attack sequences). Kinds 41/42/43/46 (look, show/hide, set position) also unimplemented; they fail explicitly as unsupported. Kind 19 (PlayAnimByName, the cage `idle`) also unsupported.
- **G3 Camera fidelity.** Only eye and target are taken from the clip; up and FOV stay with the follow camera (the clip's `camera-xfov` channel is not applied). The clip's `root_camera` channel binds to the level graph (verified to bind, not verified against a frame-accurate reference). SKIP during a PlayCamera replaces the clip with "follow kept" (recovered source: the level set's idle clip; not visually verified).
- **G4 SKIP and caption art not decoded.** Bitmap-filled shapes (fill kind 66) need the HUD texture atlas; caption box background and name plate are not in the traced chain; the caption text font (EditText font 7, htmlText face) is not mapped to the port font. Placeholders remain (section 6).
- **G5 SKIP semantics** still unverified against the reference (no SKIP press in the reference).
- **G6 Caption hold timing** remains a placeholder fit (wave-1 P4).
- **G7 PlayCamera blocking in a live run** is covered by the host test; a full live Swamp run is blocked by G1.
- **G8 Tutorial flags** remain session-only (save schema v4 pending, wave 1).

## 6. Placeholders (ART/HUD rule)
| # | Placeholder | Where | What it stands for | Reference to replicate from |
|---|---|---|---|---|
| P1 | SKIP: red 20x20 X block at (5,4) + "SKIP" label (font 5, pale yellow) | `cinematic_runner.cpp build_frame` (role `placeholder-skip`) | `menu_skipcutscene` sprite 737 -> btn_MENU_SKIP 736 (shapes 732/734, bitmap fills; text 735 font 7, 16 px, (255,255,204)) in `dqhud_droid.swf` | SKIP visible 80-172, 180-204, 224-260 s (Part 1, survey B5); Part 2 `sheets2/s010.jpg` 3:36-3:56 shows the look |
| P2 | Caption band: translucent black full-width band (0,256,480,64), alpha 0.55 (role `placeholder-box`) | `build_frame` | `tutorials_dialog` 731 -> dialogBox 730 -> dialog 729 -> text 728 (placed at (4.15,319.8) px) | captions 104-164, 192-204, 228-258 s; name plate absent in the placeholder |
| P3 | Caption/SKIP typeface: font id 5 | `main.cpp` (wave 1) | EditText font 7 (htmlText face) in the SWF | same captions |
| P4 | Caption hold: 2000 ms + 25 ms/byte, cap 6000 ms | `cinematic_runner.hpp` | the Flash dialog advance (not decoded) | Swamp 104-106 s (2 s), 108-112 s (4 s) |
| P5 | `[StrID n unresolved]` marker | `campaign_host.cpp` | error marker, not art | none |

## 7. Package files required
- Camera clips: `windows-source-clock-v19-preview-15-rc3/assets/data/3D/camera/animations/cs_swamp_intro/*.bdae` (present in the rc3 package; the test and EXE load them from there). `.local-inputs/assets-extra/android/3d/camera/animations/cs_swamp_intro/` (18 copies, staged earlier) is not needed by the Windows build.
- Level camera scene: `data/3D/camera/CameraTests.bdae` (rc3 package, node `PlayerCamera_Default`).
- Dictionary: `data/pydata/animations_dictionary_pyarray{,names}.bin` (rc3 package).
- Scripts: `port/windows-foundation/reports/encounter-source-scripts.json`, `original-campaign.xml`.
- HUD SWF (read only, for the trace): `original-cache/data/menus/dqhud_droid.swf` (rc3 package).

## 8. Verifier script (exact)
1. `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p16cine2 -Test` (only `session_skill_binding` may fail). Direct tests: `build-p16cine2/original_camera_clip_tests.exe <rc3 assets>`, `campaign_host_tests.exe <rc3 assets>`, `campaign_trigger_zone_tests.exe <rc3 assets> <rc3 assets> <android>`.
2. Jobs (quiet): `cine-runs/cam-jobs.json` (cam-base-f300, cam-f150, cam-f250, cam-f340, cam-f420), `zone2-jobs.json` (zone-inside, zone-outside), `regress2-jobs.json` (swamp-now, lizard-zone), each run by `port/windows-foundation/tools/quiet_run.ps1 -JobsFile <file> -Parallel 2..5`.
3. Expected: cam-f150/f250/f340 frames show the clip framing (cages at f250); cam-f420 matches cam-base-f300; log `PlayCamera finished id=359 frame=350`. zone-inside log `trigger _prim_TriggerZone -> script tutorial_treasure started` and frame with the chest caption and SKIP; zone-outside has no trigger line. swamp-now and lizard-zone log `cutscene aborted: Unknown original lifecycle actor` (the expected blocker).
