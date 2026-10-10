# POINTS (BUG B049): all granted Stats points spendable in one visit

Status: investigation, implementation, isolated tests and in-EXE quiet verification done.
Not done: skill-side confirmation (SettingSkills/SettingBoth), see open risks.

## Evidence: original behaviour

Visual (reference video `.local-inputs/reference-video/dh2-act1/Dungeon Hunter 2 (v1.0.3) Part 1 [720p] [z_Zky7qQdYs].mp4`):
- Stats page, t ~778-781 s (contact sheet `.local-inputs/claude-preview14/skills/video/sheet-766-782.png`, read directly):
  "Points left 2 -> 1 -> 0" while STRENGTH goes 12 -> 13 -> 14 in one menu visit; the
  + buttons stay active; Dex 5, End 8, Energy 3 unchanged. Observed, not inferred.
- t=789-797 s Skills page, dialog "Confirm character point allocation?" (green check / red X)
  at t=798 s over the menu; t=799 s is the HUD (menu closed). Observed on the Skills page.
- Part 2 is not in the local inputs.

Logic (`.local-inputs/ida-apk-export-2026-10-07/.../pseudocode-all.c`, `port/engine-ui/reference/character-menu-flow-v1/authored-actions.txt`):
- btn_train_* onRelease (SWF 0x149de..0x14a4e and the other three stats): `AddedStatsThisTurn`
  is tested only to decide whether to save: when it is false and `useSkillPoint` is false,
  `NativeSaveGame` runs (the pre-batch save). The spend itself always runs
  (`StartingX += 1`, `AddedStatsThisTurn = true`, `NativeStatsAssignPoint(stat)`, sound).
  Decoded from the jump structure (the branch at 0x149f9 targets the `not` at 0x14a13, which
  jumps to the spend block when the flag is already true). An earlier reading that treated
  the flag as a spend gate was wrong and is withdrawn.
- NativeStatsAssignPoint (IDA 0x43eb7c) -> Character::IncStatStr/Dex/End/Nrg: debit property
  148 by 1, credit stat by 1, UpdateBaseProperties. No gate in native code.
- CharacterMenu onPush (0xe265): `AddedStatsThisTurn=false`, `Starting*=0`, `useSkillPoint=false`.
- CharacterMenu btnBack onRelease (0xe55b..0xe5c9): with `AddedStatsThisTurn` true and
  `useSkillPoint` false, WarningType "SettingStats" and the menu_confirm2 box
  (GAMEPLAYMENUS_POINTS_CONFIRM) instead of closing.
- Confirm yes (0x345dc, btn_yes; SettingStats branch 0x346c8..0x34825): flag false, NativeSaveGame,
  NativePopAllMenus, NativeBackToHud. Commit and close.
- Confirm no (0x34a47, btn_no; SettingStats branch 0x34af3): NativeReloadSkills (Character::ReloadSkills
  -> SG_Load of the saved file, i.e. the pre-batch save), NativePopMenu, flag false, btnBack again. Undo the
  whole batch and close.
- Level-up: Character::LevelUp (0x3beb88) raises level and calls UpdateBaseProperties; the 2 points come
  from the class recalc data (no hard-coded grant in code). The fixture level-up grant was not re-derived.

Conclusion: the original stages all spends of one visit and asks Confirm on exit (Yes = save and
close, No = undo the batch). The port's one-per-visit gate (Preview 14) was the bug. No PC deviation
on the gate; the confirm model is the original's.

## Changes (branch p15/points, commit 4f8f96a3)

- `features/character_menu/stat_training_v1.hpp/.cpp`
  - `StatTrainingVisitV1`: per-stat staged counts (`staged[4]`, `has_staged`, `staged_total`), cleared by
    `open_visit`. The one-per-visit gate is removed.
  - `train_stat_in_session_v1`: spend; persistence is optional (empty = stage in memory).
  - `refund_stat_in_session_v1` (new): inverse of a spend (points +1, stat -1, same class recalculation;
    as NativeStatsRemoveAssign / Character::ResetStatsChange).
  - `commit_stat_visit_v1` (new): one save of the staged CharacterState (Yes); failure keeps the batch.
  - `cancel_stat_visit_v1` (new): refunds every staged spend on the live Session, saves the reverted state
    once (No). Failure restores the Session sheet, actor vitals and CharacterState exactly.
  - The + route stages the spend and increments the visit count (no save until Yes).
- `features/character_menu/character_menu.hpp`: `Presenter::close_guard` (std::function<bool()>),
  consulted by `close()`. Back, Escape, profile key and composition release close paths all pass it.
- `main.cpp`
  - Guard: closing with staged spends sets `statConfirmOpen` and keeps the menu open.
  - Confirm box = existing pause confirmation frame (`pauseConfirmArt`, `WarningBox`/`btn_yes`/`btn_no` art
    and hit regions), with `confirm_msg` text replaced by `GAMEPLAYMENUS_POINTS_CONFIRM`.
    `drawPauseArt` takes an optional message symbol; the text cache key differs for the message.
  - Click routing: btn_yes -> `statConfirmYes` (commit, close); btn_no -> `statConfirmNo` (cancel, close).
  - Escape with the box open dismisses only the box (staged points stay).
- `features/character_menu/stat_training_v1_tests.cpp`: test 8 (two staged spends, third refused, nothing saved
  before Yes, Yes saves once and clears), test 9 (No refunds, sheet and vitals restored exactly, one save of
  the pre-visit state), test 10 (failed No and failed Yes keep the batch and states). Tests 1-7 unchanged.
- Bug found and fixed during the test: the rollback reused the caller's `error` string (overwritten by the
  Session update); it now uses a local message.

## Tests (real output)

- `ctest -R stat_training` : `stat_training_v1 ... Passed`; prints `stat_training: two-points visit strength=12
  endurance=10 saved points=0`.
- Full suite: 110/111 pass; the only failure is `session_skill_binding` (known: .local-inputs junction in worktrees).
- Build: `p14_build.ps1 -Name points` build exit 0.

## In-EXE verification (quiet runner, EXE `DH_wt/build-points/dh-foundation.exe`, Preview 14 rc1 assets)

Fixture: `prof/stat2.save` (level 1, Stat_Points 2, Str 10, Dex 5, End 8, Energy 3), made from
`claude-preview14/skills/prof/stat3.save` with `tools/set_profile.exe ... 2 1`.

Batch 1 (`jobs-b1.json`, 9 jobs, all exit 0, not timed out):
- `two-f105` (click Str at 100, capture 105): log `staged stat=0 points=2->1 value=10->11`. Capture: Points left 1,
  Strength 11, Dex 5, End 8, Energy 3 (matches the user's screenshot values).
- `two-f125` (Str at 100 and 110, capture 125): log `points=2->1` then `points=1->0 value=11->12`. Capture: Points left 0,
  Strength 12.
- `refuse-3` (3 clicks): third log `Character menu action diagnostic: Source stat training refused: no Stat_Points remain`.
- `yes-open` (2 clicks, Escape at 125, capture 140): log `Stats confirmation opened staged=2`. Capture: confirm box
  "Confirm character point allocation?" with Yes / No over the Stats page.
- `yes-final` (Yes at 150:150:185): log `Stats confirmed frame=150 points=0`; capture 175 is the HUD (menu closed).
- `no-open` (capture 140): same box.
- `no-final` (No at 150:275:185): log `Stats cancelled frame=150 points=2`; capture 175 HUD.
- `esc-dismiss` (Str at 100, Escape 115 then Escape 125 via `--pause-close-frame`): log `Stats confirmation opened staged=1`,
  then `Stats confirmation dismissed frame=125 via Escape`; menu stays open.
- `nostage-close` (open and Escape, no spend): no confirmation; closes.
- Save files: `yes-final/character.save` is schema v4 (old `dump_profile` cannot read it: "Unsupported character
  schema version"; not a game error). `no-final` and `esc-dismiss` / `two-f125` / `refuse-3` leave the fixture
  unchanged on disk (staged spends are not saved before Yes).

Batch 2 (`jobs-b2.json`, reload from the batch-1 saves, capture at 90 after profile click at 60):
- `reload-yes` (from yes-final save): Points left 0, Strength 12, Attack 75, R-Dam 14-18. Persistence after Yes confirmed.
- `reload-no` (from no-final save): Points left 2, Strength 10, Attack 69, R-Dam 13-17.
- `reload-fixture` (fixture): identical to reload-no pixel readouts (Points 2, Str 10, Attack 69, R-Dam 13-17).
  Cancel restores the pre-visit state exactly.

Control: the old Preview 14 Skills EXE (`build-skills`) with the same args gave `points=2->1` then refused the second click
("Stat point already assigned in this menu visit"), which confirms the one-per-visit gate was the bug.

Images: `.local-inputs/claude-preview15/points/frames/` (two-f105.png, two-f125.png, yes-open.png, yes-final.png,
no-final.png, reload-yes.png, reload-no.png, reload-fixture.png). Read directly: yes-open shows the box; reload-no equals the fixture.

## Package files required

- No new assets. The confirmation frame and text use the pause UI art already in the package (`pause_ui`
  `menu_hud_confirm` WarningBox) and the localization symbols `GAMEPLAYMENUS_POINTS_CONFIRM`, `GAMEPLAYMENUS_ACCEPT`,
  `GAMEPLAYMENUS_REFUSE` (rendered in the rc1 package).

## Verifier script

- Scripts (not in the repo): `.local-inputs/claude-preview15/points/scripts/mkrun.sh` (run folder + args),
  `mkjobs.sh` (jobs JSON). Batches: `jobs-b1.json`, `jobs-b2.json` (after batch 1). Run:
  `port/windows-foundation/tools/quiet_run.ps1 -JobsFile <jobs> -Parallel 9 -Summary <json>`.
- Expected observations: see the in-EXE section above (log lines and capture readouts).

## Open risks

- Visual: the original confirm (video 797-798 s) shows check/X icons; the port shows the existing Yes/No
  WarningBox art with the same message text. Art difference, not a logic difference.
- Skill side: SettingSkills / SettingBoth confirmation is not implemented (open item from the Skills stream).
  A mixed batch (stats staged, then a skill spend) saves the skill spend immediately; No undoes only the stats.
  The original would undo both (NativeReloadSkills).
- Undo mechanism: No refunds via the inverse recalculation, not a disk reload. Current HP/MP are clamped to the
  restored maxima; the original reloads the saved HP/MP.
- Persistence: staged spends are not written until Yes (the original writes the pre-batch save at the first spend).
  A crash or quit with staged points loses those points, which matches the original's disk state.
- Pause (`Pause`/Escape-HUD) can open over a character menu with staged points (pre-existing flow, untested).
- Stats `+` buttons stay visible at zero points (matches the reference video frames); refusal is logged only.
- Test window is a hidden desktop run; no live window was controlled.
