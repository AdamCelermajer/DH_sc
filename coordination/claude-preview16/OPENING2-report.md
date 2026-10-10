# OPENING2 report (Preview 16, wave 2): caption timing, quest-driven opening start, named conditions, block-save rule

Branch `p16/opening2` (worktree `DH_wt/p16opening2`), base `p16/integrate` (29bcec59, includes `p16/opening`). Build `DH_wt/build-p16opening2/dh-foundation.exe`. Not pushed. HUD art files not edited.
Commits: d94ba82f (caption timing), d233c040 (skeleton), 44e87c11 (quest script start, named conditions), 9521afef (Script_CONSOLE no-op), cd49c8d6 (block-save rule), b1499032 (tests), 4b8a9085 (nested cutscene mode), ef4dfcf9 (named conditions use the authored initial quest states for a new game).

## Status
Done and verified in the EXE (quiet, hidden, silent):
- (1) Caption model = the original Flash dialog-box rule (tap-wait and auto styles, fixed step timings). Placeholder hold removed.
- (6) A new game starts the Swamp opening through the authored quest scripts (`Quest::SetState` slots). `GameStartOnly` and the other named conditions are evaluated from the saved quest states, not from command-line flags. The production startup configs enable campaign triggers.
- (7) The opening runs to `EndScriptedCutScene`; the controller is unlocked and the final frame shows the normal HUD and attack control. F5 and F9 are refused during scripted cutscenes (block-save rule, logged).

NOT done: (2) cage and scene-object animation, (3) camera up and FOV, (4) equipment hand switching and SetActorPosition followers. These remain logged stubs. (5) Only the FX playback path is confirmed (no error, sets play); no flash frame has been compared with the video.

Open production defects found in this wave: D1 the Movement tutorial starts at game start, before the intro (the reference shows it after the intro, at 180 s); D2 during the opening the HUD is still visible and SKIP is not drawn (the reference hides the HUD and shows SKIP); D3 saves after the opening are refused by the checkpoint validator.

## 1. Caption timing (investigation, implemented)
- Logic (IDA `pseudocode-all.c`): `Script_StartDialog::Execute` (line 248929) enqueues `DialogMsg(text id = scalar 16, style = scalar 12, actor = scalar 8)`. `Script_WaitDialog` blocks while `Level::hasActiveDialog` (line 173807). No C++ timer exists. The Flash box advances a line through the `StopDialog` FS command (`MenuBase::FS_StopDialog`, line 212363), which pops the line and starts the next one.
- Flash box (`dqhud_droid.swf`, 30 fps; decoded with `.local-inputs/claude-preview16/opening2/tools/swfdis.py`): DialogBox = sprite 727, one frame label per DialogStyles value, which sets `isSkipable` and `btn_next` per frame. dialogBox = sprite 730: show = frame 1, hide = frame 19, `onShowAnimEnd` on frame 12, `onHideAnimEnd` on frame 39, no Stop inside. `btn_next.onRelease` hides only when `isSkipable`. `onShowAnimEnd` stops a skippable box at its show end.
- Styles (values from `data/dialogs_pycst.bin`): non-skippable = ScrollingDialogFull 6, TopBubble 7, TopBubbleAvatarLeft 8, TopBubbleAvatarRight 9, Warning 10, QuestCompletedMsgDialog 19. Every other style waits for a tap. Swamp intro captions use style 4 (MiddleBubble, tap). CombatTuto uses styles 4, 11 and 12 (all tap).
- Original rule (implemented in `features/cinematic_runner`): show = 11 steps (367 ms at 30 fps). A tap-wait line stops there until the player taps. A tap starts the hide (20 steps, 667 ms), then the next line starts. A non-skippable line plays on by itself (38 steps, 1267 ms). The placeholder hold (2000 ms + 25 ms per byte, cap 6000 ms) is removed.
- Input: `CampaignHost::caption_tap()` on any pointer release while a caption is up. This is a PC adaptation of btn_next; the button art is HUD art and is not drawn here. Verification input: `--caption-auto-tap-ms N` (default 0 = the player taps).
- Reference alignment: the reference spacing (2-8 s per line) is the viewer's tap timing. It cannot be matched by the rule alone, so section 4 compares structure only.

## 2. Scripted objects, camera, hands (NOT implemented)
- (2) `Script_PlayAnimByName` (kind 19: cage `idle`, `shake`, `open`) remains a logged stub (count=4 in the production run). The cage is not instantiated as a scene object (OPENING G1).
- (3) Camera up vector and FOV from the clips (CINE2 G3): unchanged, the follow camera values are used.
- (4) `UnEquipHands` / `ReEquipHands` (kinds 51/52) are logged stubs. The `SetActorPosition` party followers (+/-200 around the waypoint, IDA `Script_SetActorPosition::Execute`) are not moved (OPENING G3).

## 3. Opening start, named conditions, production config (item 6)
Investigation:
- The starter is a quest script. v2quests row 50 `Swamp_Escape` has prerequisite `IsPlayerInLevel(41)`, an automatic accept objective (type 6), and script slot 1 = `SwampScripts.Swamp_Intro`. IDA `Quest::SetState` (0x480c78, line 273104) runs `Quest::ExecScript(this, slot)` (line 272603, which calls `ScriptManager::StartScript`). Entering state 3 (available) uses slot 1, or slot 4 when the quest comes from Active.
- Slot map from the IDA switch: post_locked 9, pre_available 11, available 1 or 4, post_available 6, pre_active 10, active 0, post_active 5, pre_completed 13, completed 3, post_completed 8, pre_closed 12, closed 2, post_closed 7.
- `GameStartOnly` is v2conditions record 4: type 0 (`Condition_IsQuestInState`), quest row 1 (`Abbey_Rescue`), state == 0. The type-to-class map is `s_conditionImplDataMap` at 0x969258 (relocated pointers: 0 IsQuestInState, 1 IsQuestStateLower, 2 IsQuestStateHigher, 3 IsPlayerInLevel, 4 IsLevelInState, 5 IsPlayerAtLevel, 6 IsEventInState). `Condition_IsQuestInStateGeneric::Eval` (line 267056) reads the quest state from the save; no save record evaluates false.
- Consequence: a new game (all quests locked) runs the opening when the player is in level 41. A save whose quest 50 has already passed availability does not replay it, because the state is in the CQPG envelope.

Implementation:
- `features/quest_runtime/quest_runtime_v1.{hpp,cpp}`: `quest_script_slot_v1` (the IDA slot map). `set_state` starts the state's script through `services.start_script`, with the group prefix stripped from the name. A missing owner or a failed start is reported once in diagnostics.
- `features/quest_runtime/quest_conditions_v1.{hpp,cpp}` (new): decodes v2conditions (names plus 4-word records) and evaluates types 0-3 from the saved quest states and the current level. Types 4-6 are reported once and evaluate false.
- `main.cpp`: the population policy for `activate_cond` / `deactivate_cond` uses the named evaluation; command-line names still count. `questServices.start_script` starts the authored script through `sourceCampaign` (requires `--campaign-triggers`).
- Production configs: `.local-inputs/claude-preview16/opening2/production/startup.args` and `swamp.args` = the rc3 package configs plus `--campaign-commands original-campaign.xml --campaign-triggers` (no `--campaign-start`, no `GameStartOnly` flag). The package files in `windows-source-clock-v19-preview-15-rc3` are not modified; root copies these two files into the package.
- Script_CONSOLE (kind 3): IDA `Script_CONSOLE::Execute` (line 248892) shows its text only when the DisplayScriptConsoleAsDialog debug switch is on (release default off). It is now a no-op that never blocks. Before this change it aborted the first quest script.

Verification in the EXE (quiet, hidden, silent, `opening2/run2.sh`):
- `fresh8` (`base-prod.args` without any `--condition-active GameStartOnly`, 2400 frames, auto-tap 300): the population evaluates `_prim_ActorTroll` activate_cond true from the authored initial state of quest 1 (`fresh7`, 300 frames, no "Condition policy returned unknown" for the troll), the intro spawns the troll and ends with `EndScriptedCutScene`. The first evaluator version returned false here (a new character has no CQPG yet), which the CLI flag had masked; fixed in ef4dfcf9.
- `fresh5` (`base-prod.args`: `--fresh-player`, no campaign-start, no game-save; 2400 frames, with the CLI flag): the quest chain runs at bind (`Quest runtime bound rows=64 ... current=50`). `Movement_Tuto` (DoTutorial) and `Swamp_Intro` start at frames 1-2. The intro plays PlayCamera 359-376 and 55, `EndScriptedCutScene` at frame 3946, `Rendered frames=2400; clean shutdown`, no abort.
- `prod1` (9000 frames, `--caption-auto-tap-ms 300`): the same chain to the end, exit 0. The final frame (`opening2/prod1/frame.png`) shows the normal HUD (portrait, bars, skill slots, Attack) and free play.
- `cont1` and `cont2` (continue from `opening/run0` and the staged package saves): both have the same CQPG size as a fresh save (quest 50 unprogressed), so the intro starts again. That is correct for those saves.
- A true continue test (a save taken after the opening) is NOT possible yet. F5 after the opening is refused by the checkpoint validator (`Save checkpoint rejected ... Campaign lifecycle/controller providers are not persisted`, `runA`). F5 during the opening is refused by the block-save rule (`save1`, `saveB1`). Continue-without-replay is therefore covered only by the quest-state logic and the condition unit tests. It is not verified in the EXE.

## 4. Captions in the production run (`prod1`, auto-tap 300 ms, structure only)
Intro lines (strid 2031617-2031630). Spacing from the first line, ours vs the reference (the reference is the viewer's taps, so this is not a line-by-line match):

| line | ours (s) | reference (s from the same first line) |
|---|---|---|
| 1 "...Player!" | 0.0 | 0 (98 s) |
| 2 "Lord Player!" | 1.4 | 4 |
| 3 "Is he... already dead?" | 2.8 | 6 |
| 7 "Oh! It's a miracle!" | 8.7 | 18 |
| 14 "Now let's get going..." | 35.5 | 68 |

Ours is shorter because the auto-tap fires 300 ms after each show end. The Movement tutorial captions (`Movement_Tuto2`) run at frames 315-567, before the intro's first caption at frame 1440 (defect D1).

## 5. Completion and save/load (item 7)
- Completion: the intro ends with `EndScriptedCutScene` (PutCharacterInIdle, ShowFlash/HideFlash, UnlockCharacter, ExitCutSceneMode). `globalControllerBlocked=0` in the run log. The final frame shows the normal HUD and attack control.
- Block-save rule (implemented, `main.cpp` `campaignSaveRefused`): F5 is refused while `Script_BlockSaveGame` is active (`Save refused: cutscene blocks saving`). F9 is refused while a cutscene is running (`Restore refused`). Verified in `save1` at frames 400 and 500.
- Cutscene mode nests (`campaign_host.cpp`, `cutscene_depth_`): a tutorial that ends inside the opening no longer ends cutscene mode by itself. This did not change the HUD in the frame at 1800 (defect D2).
- Post-opening save: refused by the checkpoint validator, because campaign lifecycle providers are not persisted. The original's `Script_SaveGame` at the intro end is a stub (nothing written). Gap G-SAVE.

## 6. Open defects (production path)
- D1 Movement tutorial at game start. `Movement_Tuto` is started by the `pre_active` slot 10 during the bind update, because the automatic accept objective lets the quest reach active at once. Its captions run before the intro; the reference shows them after the intro (180 s). Cause not isolated. Next: find which state change the original waits for, or gate DoTutorial on the level's start.
- D2 HUD during the opening. At frame 1800 of the production run the HUD is visible and SKIP is not drawn (`f1800/frame.png`, `f1800b/frame.png`). The reference hides the HUD and shows SKIP throughout the intro (sheet A, 78-172 s). Nesting cutscene mode did not change this frame. Next: trace `hud_visible_` and the ShowFlash/HideFlash of `menu_skipcutscene` in the production order (the Movement tutorial's EndScriptedCutScene at frame 684 is one candidate).
- D3 F5 after the opening refused (G-SAVE).

## 7. Gaps (explicit)
- G1 (2) Cage `idle` / `shake` / `open` (kind 19): not drawn, stub.
- G2 (3) Camera up and FOV from clips: stub (follow camera).
- G3 (4) UnEquip / ReEquipHands visuals and SetActorPosition followers: stubs.
- G4 (5) FX: PlayEffect sets 116-120 run through the effects owner without error. The flash was not compared with the video. The `f1800` frame (PlayEffect 73 at frame 1776) shows the priest and the cage, with no flash visible. Open.
- G5 SKIP semantics and the caption band remain placeholders (HUD art not drawn; HUDART owns them).
- G6 Caption timing follows the rule; the reference spacing depends on taps. A tap schedule taken from the reference (verification input) is not implemented.
- G7 Named conditions types 4-6 (level state, player at level, event state) are not evaluated (logged once); maps that use them keep those objects inactive.
- G8 Save and continue: see section 3 (not verified in the EXE).
- G10 Ambush after the opening is not re-verified. In a fresh production run the intro locks the player at frame 2, so the walk-in trigger (`lizard4`, 1500 frames) cannot fire; no abort was logged. A run that reaches the ambush needs the opening finished first (about 4000 frames) or a position after it.
- G9 `character_menu` ctest (`Tab overlap left part not answered by the nearer Quest Log`) fails on the base branch as well: this branch does not touch `features/character_menu` (checked with `git diff 29bcec59`).

## Placeholders (ART/HUD rule)
- SKIP red block and "SKIP" label (existing, CINE2 P1); caption band (existing, CINE2 P2). No new placeholder art in this wave. The named conditions, the script start and the caption timing are data-driven and need no art.

## Package files required
- Production configs: `.local-inputs/claude-preview16/opening2/production/startup.args` and `swamp.args` (copy into the package root).
- Unchanged from OPENING: the Android-staged cs_* clips and camera scenes (see OPENING-report).

## Verifier script (exact)
1. Build: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p16opening2 -Jobs 6 -Test`. Only `session_skill_binding` and the pre-existing `character_menu` may fail.
2. Fresh production-style opening (quiet): `cd .local-inputs/claude-preview16/opening2 && ./run2.sh prod1 9000 --base=base-prod.args --condition-active GameStartOnly --caption-auto-tap-ms 300`. Expect `EndScriptedCutScene` at about frame 3946, `Rendered frames=9000; clean shutdown`, no `cutscene aborted`.
3. Block-save: `./run2.sh save1 700 --base=base-prod.args --condition-active GameStartOnly --caption-auto-tap-ms 300 --save-frame 400 --load-frame 500`. Expect `Save refused` and `Restore refused`.
4. Frames: `py -3 ../opening/tools/ppm2png.py <job>/frame.ppm <job>/frame.png`, then view.
