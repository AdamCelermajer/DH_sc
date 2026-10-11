# OPENING3 report (Preview 16): quest script gating (D1), intro HUD (D2), continue after the opening (D3), ambush run

Branch `p16/opening3` (worktree `DH_wt/p16opening3`), base `p16/integrate` (ef8b59f4). Build `DH_wt/build-p16opening3/dh-foundation.exe`. Not pushed.
Commits: 66c7e9db (D1 quest script gating, D2 pause button with the HUD), d1d5aed4 (D3 lifecycle component, F5/F9 and continue).

## Status
- D1 fixed: the Movement tutorial no longer starts at game start. It starts after the intro ends, as the authored order requires.
- D2 fixed: during the intro the HUD is hidden and the original SKIP art is drawn (frame 1200 checked against the reference). The pause button was still drawn; it is now hidden with the HUD.
- D3 fixed: F5 after the opening writes the save, F9 restores it, and a new process started from the saved profile continues at quest 50 without replaying the intro or the Movement tutorial (all verified in the EXE).
- The lizard ambush runs after the opening in one fresh production-style run (walk-in test aid), with a timeline comparison below.
- Not done: item 2 (cage `shake`/`open`), item 3 (camera up and FOV), item 4 hand switching (SetActorPosition followers not applicable, see below), item 5 (flash not confirmed on screen).
- ctest: 133 of 134 pass. `session_skill_binding` fails (allowed). Three tests need the toolchain DLLs on PATH (see Verifier script).

## 1. D1: Movement tutorial at game start (investigation and fix)
Cause (IDA `pseudocode-all.c`): `Quest::Update*` does not advance a state until the script of that state's slot has finished: `UpdatePostLocked` waits on slot 9, `UpdatePreAvailable` 11, `UpdateAvailable` 1 (only once the accept objective is complete), `UpdatePostAvailable` 6, `UpdatePreActive` 10, `UpdateActive` 0 (objectives done), `UpdatePostActive` 8 (IDA waits on 8, not on its own slot 5), `UpdatePreCompleted` 13, `UpdateCompleted` 3 (end objective), `UpdatePostCompleted` 8, `UpdatePreClosed` 12, `UpdateClosed` 2. Helper: `Quest::TestIsScriptRunning` (line 272560) -> `ScriptManager::IsScriptRunning`.

Authored chain for `Swamp_Escape` (row 50): prerequisite `IsPlayerInLevel(41)`, slot 1 `Swamp_Intro`, slot 6 `Flushit`, slot 10 `Movement_Tuto`, slot 11 `Flushit`. The accept objective is automatic, so the old runtime moved Available -> PostAvailable -> PreActive -> Active in the bind update and started `Movement_Tuto` at frame 2. The original waits on the intro script before PostAvailable, and on `Flushit` and then `Movement_Tuto` before Active.

Fix (`features/quest_runtime/quest_runtime_v1.{hpp,cpp}`): `quest_wait_slot_v1` (the table above) and `slot_script_running`; each transition is gated on the wait slot; the new service `script_running` is bound to `OriginalCampaignRuntime::running`. `main.cpp` runs `questRuntime->update` every frame after the campaign executor (`Quest::Update` runs every frame) and saves the CQPG after each update.

Result (prod3, production config, auto-tap 300 ms): `Swamp_Intro` at frame 3, its `EndScriptedCutScene` at 3725, `Flushit` at 3726, `Movement_Tuto` at 3727 (was frame 2 before the fix). In the ambush run (start position east of the zone): intro end at 4078, `Movement_Tuto` at 4080.

## 2. D2: HUD during the intro (investigation and fix)
Cause: the Movement tutorial's `ExecScript` (slot 3) runs `EndScriptedCutScene`, which shows the HUD and hides SKIP. Because D1 ran the tutorial during the intro, the HUD came back and SKIP disappeared (the old frame 1800 defect). With D1 fixed, the HUD stays hidden and SKIP is drawn for the whole intro (`ShowFlash menu_skipcutscene` / `HideFlash HUD` from `BeginScriptedCutScene`).
Remaining item in the same path: the round pause button is part of the HUD sprite (the original hides it with HUD). It was drawn unconditionally; now `campaignHost.hud_visible()` gates it (`main.cpp`, pause draw).
Verification: `f1200` frame (intro caption-free moment, `opening3/f1200/frame.png`): no bars, no portrait, no pause button, SKIP drawn. Reference sheet A (78-108 s) shows the same HUD state with SKIP.
Note: the reference also shows a "Crisr82" name tag at the bottom right during the cutscene. Not reproduced (not identified in the HUD art yet; no change made).

## 3. D3: continue after the opening (investigation and fix)
Cause: `validate_lifecycle_checkpoint` refused every F5/F9 once campaign triggers were on, because `retainHiddenActors` (implied by `--campaign-triggers`) installs a combat permission provider and lifecycle registration sets `lifecycleRegistered`. The host did not serialize its lifecycle. Two further checks then refused: physical presence (lifecycle body changes did not update `source_physical_present`), and the body source-flag table (PreSpawn17 and Limbus were missing). Restore also could not normalize PreSpawn17 and Limbus.

Fix:
- Save component mechanism (no schema bump): a neutral world object (fixed ID `0x43414d504c494650`, name `campaign.lifecycle`, bound at load next to the containers) carries component `campaign_lifecycle_v1` = trigger once-activation counts (`OriginalCampaignRuntime::trigger_activations` / `restore_trigger_activations`). `CampaignHost::bind_lifecycle_object`, `persist_lifecycle` (before every GameSave capture: F5, reload) and `restore_lifecycle` (after `restore_game_save` in F9).
- `CombatSession::set_lifecycle_serialized_by_host(true)` declares that the host serializes its lifecycle; the validator then accepts the provider state. Without it the old refusal stays.
- F9 clears the lifecycle records after the world is replaced (they hold pointers into the old world, as the reload path already did).
- Lifecycle body changes (`remove_physical`, `initialize_physical`, authored physical) update `source_physical_present` from the native body.
- Checkpoint table: PreSpawn17 = 0x1300 (CSPreSpawn::OnFocus, documented in `session_actor_transition_v1.cpp`), Limbus = 0 (`OriginalActorLifecycle::change` sets flags 0). Restore normalizes the same two states.
- Quest runtime saves the CQPG after each per-frame update. Before this, transitions made in the frame loop never reached the profile or the snapshot (the continue test showed `current=-1` and a replayed intro).

Verified in the EXE (production config, `--caption-auto-tap-ms 300`):
- `save6`: F5 at frame 5000 accepted (`Saved live checkpoint frame=5000 HP=165.098 RNG=8504954/20`); F9 at 5100 restores (native bodies rebound, no `checkpoint rejected`, no `Foundation error`), `Rendered frames=5200; clean shutdown`.
- `cont4`: new process with the F5 profile (`--fresh-player` path loads `character.save`): `Quest runtime bound rows=64 actors=42 current=50 cqpg=1631`, zero `Swamp_Intro` and zero `Movement_Tuto` commands, free play with the normal HUD (frame 2600 viewed).
- Known after F9 (not a replay of the opening): the restored quest rows in post_active/pre_completed advance (`Flushit` at 5102, rows 51 and 53), and the zone `enterLocation_StartRoom` fires again for a player who is inside it (zone contact is not persisted). Both are gaps (section 5).

## 4. Ambush after the opening (one fresh production-style run)
Run `amb2` (production config, 6000 frames, `--position -2750,-3.4,255 --move-from-frame 4950 --move-axis -1,0,0 --move-run`): the intro runs with the player locked at the start (frame 2 lock), the player is placed just outside the `LizardMan_Intro` zone (x -2997..-2797), and the walk west after the opening enters it. Earlier walk-in from the start position (`amb1`, no position) stopped at a wall (-1278,-1271); the zone was not reached.

Timeline (frames, 60 Hz; reference from `coordination/claude-preview16/OPENING-report.md` section 3-4, sheet B, 1 s steps):

| Event | Ours (frame) | Ours (s from lock) | Reference (video s from lock 223.8) | Diff |
|---|---|---|---|---|
| Walk-in zone enter, `LizardMan_Intro` starts | ~4955 | ~0 | HUD hidden 223.5-224 | aligned by construction (trigger inferred) |
| `LockCharacter`, camera target | 4962 | 0.0 | HUD hidden, SKIP 224 | ~0 |
| Lizard 1 spawn | 4996 | 0.6 | visible 224.5 (+0.7) | -0.1 |
| Lizard 2 spawn | 5099 | 2.3 | visible 226 (+2.2) | +0.1 |
| `UnlockCharacter`, `DoTutorial` CombatTuto | 5234-5235 | 4.5 | "Combat Tutorial" title 227 (+3.2) | +1.3 |
| CombatTuto line 1 (title) | 5237 | 4.6 | 227 | +1.4 |
| CombatTuto line 2 | 5328 | 6.1 | 228-231 | auto-tap spacing |
| CombatTuto lines every ~91 frames (1.5 s) | 5419, 5510, 5600, 5691, 5783, 5874, 5966 | 7.6 ... 16.7 | 232-244 at 2-4 s spacing | spacing differs: our auto-tap 300 ms, reference is the viewer's taps |

Frames compared: `montage_fx.png` (in `DH_sc/.local-inputs/claude-preview16/opening3/`): `amb-5000` (HUD hidden, SKIP, lizards arriving), `amb-5240` and `amb-5330` (CombatTuto caption with the "Bogwomp" name plate and health bar), reference sheet B (`opening/ref/sheetB_222_238.png`).

Observation: the intro end moves with the start position (3725 for the default start, 4078 for the walk-in start). The caption timing is therefore not fixed frame to frame; the cause was not isolated.

## 5. Gaps (explicit)
- G-CAGE (item 2): `Script_PlayAnimByName` (kind 19: cage `idle` / `shake` / `open` on `_anim_cage_001`) is still a logged stub. The cage is not a scene object in this build. Not implemented.
- G-CAM (item 3): camera up vector and FOV from the clips: not implemented (the follow camera values are used).
- G-HANDS (item 4): `UnEquipHands` / `ReEquipHands` (kinds 51/52) are logged stubs. SetActorPosition followers (IDA `Script_SetActorPosition::Execute`): the party followers are PlayerManager players 1-3 and are moved only for the local player's waypoint move. This build has one player, so there are no followers to move; the rule is recorded, no change made.
- G-FX (item 5): PlayEffect 73 (frame 1927) and 101 (frame 2855) play through the FX owner without error. Frames at 1935 and 2862 show no flash (the camera frames a different part of the scene than the reference at 92-100 s). Flash not confirmed on screen.
- G-RESTORE: after F9 the quest rows in post_active/pre_completed advance again (`Flushit` at 5102), because the script running at save time is not persisted (the wait is on a running script). Zone contacts are not persisted either, so `enterLocation_StartRoom` fires again while the player stays inside. Trigger once-counts are persisted.
- G-CONTINUE: the startup continue reads the profile only (`--fresh-player` path or menu slots). `gameplay.save` (world, combat actors, lifecycle component) is not loaded at startup; F9 is the world restore path. Combat-session lifecycle originals (`set_actor_original_state`) are not serialized; restored actors use population defaults.
- G-TIMING: the intro end varies with the start position (3725 vs 4078); the caption timing is still auto-tap (300 ms), not the viewer's taps.

## Placeholders
- None added. The campaign lifecycle object is a neutral object without visuals.

## Package files required
- None new. Production run configs: `.local-inputs/claude-preview16/opening3/base-prod3.args` (= `opening2/production/swamp.args` with absolute asset paths; the package root is not written to).

## Verifier script
1. Build and ctest: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p16opening3 -Jobs 6`. Then `PATH="<toolchain bin>;$PATH" ctest --test-dir build-p16opening3 -j 6` (the toolchain on PATH is needed for `despawn_after_death_v1`, `container_open_script_v1`, `winmm_pump_priority_v1`). Expect only `session_skill_binding` to fail.
2. Opening and tutorial order (quiet): `cd .local-inputs/claude-preview16/opening3 && ./run3.sh prod3 4600 --base=base-prod3.args --caption-auto-tap-ms 300`. Expect `script=Swamp_Intro index=1` at frame 3, `EndScriptedCutScene index=0` at about 3725, `Movement_Tuto index=0` at about 3727 (not at frame 2).
3. HUD and SKIP in the intro: `./run3.sh f1200 1200 --base=base-prod3.args --caption-auto-tap-ms 300`, then `py -3 ../opening/tools/ppm2png.py $PWD/f1200/frame.ppm`. Expect SKIP, no HUD.
4. Save, restore, continue: `./run3.sh save6 5200 --base=base-prod3.args --caption-auto-tap-ms 300 --save-frame 5000 --load-frame 5100` (expect `Saved live checkpoint` and no `checkpoint rejected`). Copy `save6/character.save` to a new folder and run `./run3.sh cont4 2600 --base=base-prod3.args --caption-auto-tap-ms 300` there (expect `current=50` and no `script=Swamp_Intro index=1`).
5. Ambush after the opening: `./run3.sh amb2 6000 --base=base-prod3.args --caption-auto-tap-ms 300 --position -2750,-3.4,255 --move-from-frame 4950 --move-frames 6000 --move-axis -1,0,0 --move-run` (expect `trigger _prim_TriggerZone_LizManIntro -> script LizardMan_Intro started`, spawns about 35 and 137 frames after the lock, `CombatTuto` lines).
