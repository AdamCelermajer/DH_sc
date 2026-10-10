# CINE report (Preview 16): cinematic runner on the campaign script host

Branch `p16/cine` (worktree `DH_wt/p16cine`), base `p16/host` (2dc07f37). Commits: 46081d8b (runner, caption path, SKIP), dbaa1636 (DoTutorial binding, camera anchor, timing). Not pushed. Build `DH_wt/build-p16cine/dh-foundation.exe`.

## Status (one paragraph)
The generic runner exists and runs inside the real EXE: BeginScriptedCutScene/EndScriptedCutScene contract (HUD hide, SKIP control, controller lock, cutscene mode), authored dialogue lines (StartDialog/WaitDialog/FlushMessages) drawn as caption text with the original StrID text, a tappable SKIP control, and the tutorial scripts (`chest_tuto`, `Movement_Tuto` -> `Movement_Tuto2`) run to completion from level data with no per-level code. NOT done: the Swamp opening and the LizardMan ambush do not complete. Both stop at the lizard/actor spawn, which needs the lifecycle owner from the spawn stream (not on this branch), and the Swamp opening also needs camera clip playback (kind 5) and actor clips (kind 45), which are unsupported. The art for the caption box and SKIP is labelled placeholder art (see Placeholders).

## 1. Investigation (evidence, AGENTS.md rule)

### Visual (reference video, contact sheets, survey B5/B6)
Reference timings are from `coordination/claude-preview15/CINE-survey.md` section B5/B6 (+/-0.5 s, v1.0.3). Frames I looked at in this session are my own captures (section 4). I did not re-sample the reference video in this session, so the timings below are the survey's, not re-observed.
- SKIP: red bar, top-left, visible in the Swamp intro (80-172 s), movement/chest/combat tutorials (180-260 s); hidden in the HUD gameplay. HUD hidden in the same spans.
- Captions: Swamp intro subtitles 104-164 s (about 2-4 s per line, one line at a time); chest tutorial 192-204 s (title, then 3 body lines, about 2-4 s each); movement 180-190 s; combat 227-258 s.
- No SKIP press is visible in the reference (skip semantics unverified, see Gaps 1).

### Logic (IDA `libDungeonHunter2.so/pseudocode-all.c`, line numbers in that file)
- `Script_StartDialog::Execute` (line 248929): `DialogMsg(0, text=scalar16, style=scalar12, actor=scalar8)` enqueued; style `EnterLocationDialog` flushes instead.
- `Script_WaitDialog::IsBlocking` (line 244736) = `Level::hasActiveDialog` (line 173808): blocks while the dialog queue head != tail.
- `Script_DoTutorial::Execute` (0x460950, line 248676): gate (player, difficulty 0, tutorial flag, offline) then `ScriptManager::StartScript(GetIDFromName(name))` (name from scalar 12), then consume flag and save settings. The tutorial prose is therefore dialog commands inside the named script, not a separate table.
- `Script_SetCameraTarget::Execute` (line 248361) -> `CameraTarget::SetTarget(object)` (line 197193) -> `GameObject::GetCameraAnchorPosition` (0x3943b8, line 109899): returns the object's own position (+352) unless an auxiliary anchor node is attached (+184; not decoded). Used for the anchor rule in section 2.
- `Level::Update` (line 179830) starts the level start script from level field +328 (script id); the id source is not established (see Gaps 6).
- StrID: the dialog ids look like `(group<<16)|index` values (2031617 = 0x1F0001 swamp group; 2097213 = 0x1FFFFD tutorial) [inf; the bit layout is not verified in IDA]. They are resolved through the existing `MenuLocalization::string_id` (the original numeric StrID corpus), not a new parser; the EXE shows the correct text for the tutorial ids, which is the evidence used.

### Decoded data
- `reports/encounter-source-scripts.json`: CombatTuto (9 dialog pairs, ids 2097213..), chest_tuto (4 pairs, title first, SetCameraTarget `_prim_OpenableContainer_2`), Movement_Tuto (DoTutorial 78 -> `Movement_Tuto2`, 4 pairs), Swamp_Intro (script 30; PlayCamera 5, PlayActorAnim 45 present).

## 2. Implementation

- `features/cinematic_runner/cinematic_runner.{hpp,cpp}` (new, pure C++17, no platform/frontend dependency): caption queue (enqueue, waiting, flush, update on the executor clock), cutscene contract flags (active, SKIP visible), SKIP hit test with the same letterbox mapping as the PC HUD (`viewport_for`), flat frame description (solid rects + text items in the authored 480x320 space).
- `features/campaign_host/campaign_host.{hpp,cpp}`:
  - flash `menu_skipcutscene` sets the runner's SKIP state;
  - dialog provider: kind 10 enqueues `CaptionLine{text_id=scalar16, style=scalar12, actor=scalar8, text}`; the text comes from `set_caption_text` (main binds `MenuLocalization::string_id`); an unresolved id shows `[StrID n unresolved]` and is listed once as unsupported. Kind 12 blocks while `waiting()`;
  - flush provider -> runner flush (no longer a stub);
  - cutscene mode enter/exit and abort -> runner active/flush;
  - `frame()` advances captions on the executor clock;
  - `bind_executor` now calls `world.bind_runtime(runtime)` so DoTutorial can start its named script (was unreachable: "needs bound ScriptManager runtime").
- `source_world_objects.cpp`: camera anchor for any non-Character authored object = its placement translation (IDA rule in section 1). Before this, `chest_tuto` (target `_prim_OpenableContainer_2`) killed the EXE with "Unsupported source camera anchor receiver".
- `main.cpp` (P16 CINE hunks only): options `--campaign-skip-frame N` (scripted SKIP press, verification input) and `--campaign-start NAME` (harness: start an authored script by name through the runtime, same start as DoTutorial; not a production starter); caption StrID resolver after the localization load; a release on the SKIP rect presses SKIP (mouse); caption/SKIP draw after the PC HUD (placeholder quads through `overlay.drawTriangles`, text through `FrontendText`, font id 5); the text is rebuilt only when its signature changes.
- `CMakeLists.txt`: one P16 CINE block (runner + test).

Unit/focused tests (all pass):
- `cinematic_runner` (new): duration placeholder, queue order and waiting, flush and end, SKIP hit across viewports, frame content.
- `campaign_host` (updated): Begin/End contract (unchanged checks), lizard spawn aborts explicitly and restores flags, session continues, SKIP press ignored when hidden / sampled when visible, `captions_block_then_release` (CombatTuto 9 lines, WaitDialog blocks, script finishes), `unresolved_caption_is_explicit`, `do_tutorial_starts_named_script` (Movement_Tuto -> Movement_Tuto2, 4 lines), unsupported kinds named.
- `campaign_trigger_zones`: passes unchanged.
- ctest: 116/117 pass; only `session_skill_binding` fails (allowed in worktrees).

## 3. Integrated runtime verification (quiet EXE, hidden desktop; logs and frames under `.local-inputs/claude-preview16/cine-runs/`)

Base args: `host-runs/base.args` (rc3 Swamp package), `--campaign-triggers`, `--fixed-step 0.0166667`, isolated copies of gameplay.save and character.save per job.

| Job | What it shows | Result |
|---|---|---|
| chest-f150 (frame 150) | Chest tutorial running | HUD hidden, SKIP placeholder top-left, caption box "Treasure Chest Tutorial" (frame.png) |
| chest-f300 | Second line | "To open a chest, stand next to it and tap the chest icon." (frame.png); matches the reference line at 198 s |
| chest-f1300 | After the script | Cutscene ended, HUD back, SKIP gone; log: `LockTutorial`, `UnlockCharacter`, `EndScriptedCutScene` ExitCutSceneMode |
| chest-skip (press at 150) | SKIP press | `[campaign] SKIP pressed` logged at press; run completes |
| movement-f420 | Movement tutorial | "Movement Tutorial" caption, SKIP, HUD hidden (frame.png). Movement_Tuto -> Movement_Tuto2 started through DoTutorial, 4 lines |
| movement-f200 | Before the first line | SKIP and HUD hidden, no caption yet (Movement_Tuto2 waits 1 s + 4 s before its first line) |
| swamp-intro (harness start) | Swamp opening | Runs BeginScriptedCutScene, then `SpawnCharacter` (kind 30, `_prim_ActorTroll`) fails: "Unknown original lifecycle actor"; HUD, SKIP, cutscene mode, save block and controller locks restored |
| lizard-walk (walk west, final build) | LizardMan ambush via trigger | `trigger _prim_TriggerZone_LizManIntro -> script LizardMan_Intro started`; SetCameraTarget `_prim_Waypoint_NewCamSpot` now resolves; SpawnCharacter fails "Unknown original lifecycle actor"; cutscene aborted and restored; session continues |
| zones | Trigger list | 8 zones fed, 10 not fed (reasons logged, incl. `tutorial_treasure`: rotated shape) |

Player control during the cutscenes: player position stays at the start position across frames (chest run), so the controller lock holds.

## 4. Visual check

Frames (PNG beside each PPM): `cine-runs/chest-f150/frame.png`, `chest-f300/frame.png`, `chest-f1300/frame.png`, `movement-f200/frame.png`, `movement-f420/frame.png`. I looked at each.

## 5. Gaps (explicit)

1. **Skip semantics**: recovered source semantics only (sampled per command: camera duration 0, lock early exit, Wait ignores skip). A SKIP press in the chest run does not shorten the dialogue (no recovered dialogue skip). The reference shows no SKIP press, so the original behaviour is unverified. User decision still open (survey G1).
2. **Lizard ambush does not complete**: spawn needs the lifecycle owner. Blocker recorded on `p16/spawn` (f4bec88f, section 5: live spawn under AI refused; needs an owner for physical transitions) and `p16/lifecycle` (0fb95dc0, partial). Not merged here; the root must merge those streams, then `LizardMan_Intro` -> `CombatTuto` should run to the end.
3. **Swamp opening does not complete**: (a) its first failing command is the kind 30 spawn of `_prim_ActorTroll` (gap 2); (b) PlayCamera (kind 5) clip playback is unsupported (cs_swamp_intro is a BRES container; no decoder in the port); (c) PlayActorAnim (kind 45), SetActorPosition (46), LookActor (41), Show/HideActor (42/43) are unsupported. The starter of `Swamp_Intro` is not identified (`--campaign-start` is a harness only).
4. **Movement_Tuto and chest_tuto starters**: not identified in the scripts. Both were started by name through the harness. The `tutorial_treasure` zone (chest) is not fed: "rotated shape not supported (source rotation convention not verified)".
5. **Tutorial flags**: session-only (save schema v4 pending). The save/settings calls are logged stubs ("nothing written").
6. **Level start script**: `Level::Update` starts a script from level field +328; I did not establish which authored script that is for Swamp (IDA line 179830).
7. **Camera errors kill the session**: `main.cpp` throws "Source camera target: ..." from the camera tick, outside the host abort policy. The anchor case is fixed; other camera failures still end the process. Needs routing through the host abort.
8. **Captions**: `$player` in authored text is not checked (the chest and movement lines have none). The resolver output is taken as is.
9. **Dialog advance**: the original advances inside the Flash dialog box (not decoded). The placeholder hold is a timing fit (Placeholders), not the original mechanism.
10. **Quest banner ("New Quest", 174 s)** and the combat-tutorial red circles are out of scope here.

## 6. Placeholders (ART/HUD rule)

IDA trace status: **incomplete**. I traced the script side (ShowFlash/HideFlash menu names, StartDialog/WaitDialog/Level::hasActiveDialog, DoTutorial), and found the SKIP and dialogue art is inside the Flash HUD/menu SWFs (`dqhud.swf` frame label `menu_skipcutscene`; dialogue box in the menu SWFs). I did NOT follow the Flash draw path to its primitives, so the placeholders below are used because the trace is not done, not because the art is proven absent. A later pass must do that trace.

| # | Placeholder | Where in code | What it stands for | Reference timestamps to replicate from |
|---|---|---|---|---|
| P1 | SKIP control: solid dark red rect at authored (8,8) 72x20 with the label "SKIP" in font 5 | `cinematic_runner.cpp` `build_frame` (role `placeholder-skip`), `main.cpp` cinematic draw | the original `menu_skipcutscene` art (dqhud.swf) | SKIP visible 80-172 s, 180-204 s, 224-260 s; HUD hidden in the same spans |
| P2 | Caption box: translucent black rect at authored (32,238) 416x64 | `build_frame` (role `placeholder-box`) | the original dialogue box | captions 104-164 s, 192-204 s, 228-258 s |
| P3 | Caption/SKIP typeface: font id 5 (Fontin SmallCaps, the only mapped source font) | `main.cpp` (`field.font_id=5`) | the original dialogue font, not established | same captions as P2 |
| P4 | Caption hold: 2000 ms + 25 ms per byte, cap 6000 ms | `cinematic_runner.hpp` (`kCaption*MsPlaceholder`) | the original dialog advance (Flash-driven); fitted to the reference | Swamp 104-106 s (2 s), 108-112 s (4 s); chest 192-204 s (about 2-4 s per line) |
| P5 | Diagnostic text `[StrID n unresolved]` (only when the StrID does not resolve) | `campaign_host.cpp` dialog provider | not art; an explicit error marker | none |

## 7. Package files required

- `.local-inputs/assets-extra/android/3d/camera/animations/cs_swamp_intro/*.bdae` (18 clips, copied with `cp -n` from the Android device folder `.../files/data/3d/camera/animations/cs_swamp_intro`). Not used yet (PlayCamera unsupported, gap 3b).
- Existing package `windows-source-clock-v19-preview-15-rc3` (assets, `swamp.args` base, `gameplay.save`, `character.save`). Not modified; per-job save copies live in the run folders.

## 8. Verifier script (exact)

1. `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p16cine -Test` (ctest: only `session_skill_binding` may fail).
2. Jobs file `.local-inputs/claude-preview16/cine-runs/jobs.json` (chest-f40..f1300, chest-skip, zones, movement-tuto, swamp-intro, movement-f200, movement-f420): `powershell -NoProfile -File port/windows-foundation/tools/quiet_run.ps1 -JobsFile <jobs.json> -Parallel 10`.
3. Expected: `chest-f150` and `movement-f420` frames show HUD hidden, SKIP, and the title caption; `chest-f300` shows the "To open a chest..." line; `chest-f1300` shows the HUD back; `chest-skip` log has `SKIP pressed`; `swamp-intro` log has `cutscene aborted: Unknown original lifecycle actor` after `kind=30`; `lizard-walk` (`lizard-jobs.json`) has `trigger _prim_TriggerZone_LizManIntro -> script LizardMan_Intro started`.

## Notes for the root
- Branch `p16/cine` depends only on `p16/host`. The lizard ambush needs the `p16/spawn` + `p16/lifecycle` work merged; I did not merge them.
- Other branch-touching hunks: `source_world_objects.cpp` (camera anchor rule) is a shared file; the change is small and commented with the IDA address.
