# VERIFY17 report (Preview 16+17 merged EXE, verification)

Branch `p17/verify17` (worktree `DH_wt/p17verify17`), started from `p17/fix` (`a78a882f`). Build `DH_wt/build-p17verify17/dh-foundation.exe` (build exit 0). Assets: rc3 (`.local-inputs/windows-source-clock-v19-preview-15-rc3/assets`) for save, quest and drop runs; the witch-cave runs use the `procgen2/asset-root3` overlay as in P17INT. Not pushed. Evidence folder: `.local-inputs/claude-preview17/verify17/`.

## Investigation
- Continue path: `--resume-game` (`loadFrame=0`) already runs the F9 restore at frame 0, so a fresh process does restore doors, containers, quest state and lifecycle. Two defects found in that path (below).
- Defect A: the campaign trigger feed runs before the restore block in the same frame. The restored character is inside `_prim_Zone_EnterLoc_Prison`, so `enterLocation_StartRoom` (the opening) replayed on continue; the restore only replaced the edge state afterwards.
- Defect B: bind-time quest banners were shown on a continued start (startup `bindQuestRuntime()`).
- Drop roll: `Source death reward` draws from the shared `gameplay_rng` (`features/loot/runtime_death_rewards_v1.cpp` ~L418-423), the same victim loot table on both builds.

## Implementation
- Commit (VERIFY17 fix): `--resume-game` sets `resumeGame`; startup bind uses `bindQuestRuntime(!options.resumeGame)`; a restore frame (`drawn==loadFrame`, `reloadFrame`, or F9/R held) skips `campaignHost.frame` so the restore replaces the trigger edge state before any zone fires. Nothing else changed.

## Isolated tests
- ctest (`-j 6`, llvm-mingw on PATH via the Android SDK ctest): final run 146 of 147 pass; only `session_skill_binding` fails (allowed). Log `verify17/ctest-final2.txt`. One earlier full run also failed `game_save`; that test writes a fixed temp path and passed alone and in the next full run, so it is a parallel-run race, not a regression.

## Integrated runtime verification (real EXE, quiet_run, rc3 package)

### 1. Save/reload with doors, containers, seed, entry, lifecycle
Run `save/s4` (Swamp, campaign triggers, `GameStartOnly` for the opening, forced `IsAfter_Swamp_KillWitch2`, `open_SwampKingDoor`, start beside `_prim_DestructibleContainer`):
- Opening and cutscenes complete (Swamp_Intro, Movement tutorial, cutscene ended at frame 4577). Save refused while a cutscene runs (`Save refused: cutscene blocks saving`, frames 1700, 2400, 4300), so the save is taken at 5400 after they end.
- Save at 5400 (`Saved live checkpoint frame=5400 HP=165.098`), reload at 5460 (`Restored live checkpoint frame=5460`). `Door view _prim_SwampDoor state=1` before the reload and after it (open stays open); `_prim_SwampDoor_FIRE state=1`.
- Container broken before the save (`Context container ... status=accepted state=2->3`, `Container opened ... loot=9`); after the reload a Space press is `rejected_state state=4->4` (opened state kept).
- Save file (`s4-gameplay.save`) contains `dh2.world-gate.door.v1`, `dh2.container.hits.v1`, `dh2.source-container.objs.v1`, `campaign_lifecycle_v1` (`campaign.lifecycle` object).
- No replay after the reload: no `caption`, `script ... started`, `trigger`, banner or tutorial line after `Restored` (305 lines checked). `Quest runtime bound ... current=50` before and after.
- Procedural seed and entry record (run `witch/b1`, Swamp to Bogwitch cave by Space at 60, save at 120, reload at 140): `Transition procedural seed ... seed=4058064753`, `Transition character saved ... visited_records=2`, `Saved live checkpoint frame=120`, `Restored live checkpoint frame=140`, no regeneration on reload (the seed is per run; each run draws its own seed).
- Caveat: with campaign triggers on, the Swamp prison zone fires the intro and the Movement tutorial blocks the cave Space transition. So the procedural seed/entry test (cave) and the lifecycle/door/container test (Swamp) are two runs, not one combined run. The lifecycle component is written only with `--campaign-triggers`.

### 1b. Fresh process continue (`--resume-game --game-save`)
- Before the fix (`save/s5`, `s6` pre-fix): door state 1 and quest current=50 restored at frame 0, but the opening replayed (`caption frame=2 ... The Boglands - Ancient Prison`), i.e. Defect A.
- After the fix (`save/s6`, no forced `GameStartOnly`): door state 1, quest current=50, no caption, no script start, no banner. Container: `--interact-at _prim_DestructibleContainer@300` gives `rejected_state state=4->4` (opened state persisted, `save/s6b`).
- Fresh continue into the cave (`witch/b2`, `--level 002_swamp_witchcave --resume-game`): `Procedural level generated ... seed=4058064753` (the saved seed, not a new one).
- Not verified: a Space press on the opened destructible in the continued process offered no object of interest (the interactable provider returns -1 for an opened destructible, so this matches the code; the in-process reload did log a context call). Left as a note.

### 2. Quest Log tab (merged EXE vs P16 and Part 2 reference)
Frames in `verify17/quest/`:
- `qq-220.png` (merged, `--quest-page-frame 150`, accept 53 at 60, 8 kills template 94 at 100): Assigned row "Prison Break" (Main quest, current), COMPLETED header with "Bogwalker's Delight 2". `qq16-220.png` (`build-p16int`, same args): identical frame.
- `d-detail-200.png` (click Prison Break, `--menu-release 170:154:81`): MAIN QUEST tag, title "Prison Break", two-line description "Escape the swamp with Rene and Celeste.", MAKE ACTIVE button, orange selection.
- `d-completed-200.png` (click the completed row, `154:136`): SIDE QUEST tag, "Bogwalker's Delight 2", "Kill 8 Bog Moths.", no MAKE ACTIVE.
- Reference `ref-523.png` (Part 2 video, t=523 s): Quest Journal, tab highlighted at the same position, Assigned list (Buried Alive selected, SIDE QUEST, description, MAKE ACTIVE), COMPLETED list with four rows.
- Differences: the reference has four Assigned and four Completed rows (our save is an earlier point in the game, so we have one of each); the reference opens with the first row selected, ours opens with nothing selected (P16 QUESTUI2 gap, unchanged). Visual layout (tab row, row art, detail panel, typography) matches.

### 3. Drop rate (100 combat seeds per build, same seeds 1-100)
Recipe: the P17FIX walk args (`sweep-walk-10/walk.args`, 300 frames, `--combat-seed N`), quiet runs, 6 parallel. Drop = `spawned>=1` on the `Source death reward` line (the P16 build does not print `selected=`, so `spawned` is used for both builds).
- merged (`build-p17verify17`): 100 seeds, 74 kills, 25 drops (23 with 1 item, 2 with 2).
- p16 (`build-p16int`): 100 seeds, 74 kills, 21 drops.
- Drop rates 33.8% (merged) vs 28.4% (p16). Fisher exact two-sided p = 0.59. Difference 5.4 points, 95% CI about -9.5 to +20 points.
- Conclusion: no significant difference. The earlier 2/18 vs 7/19 (p about 0.07) was sampling noise. No fix made (no cause found; the RNG stream still shifts with the extra actor, but the marginal drop rate does not change).

### 4. ctest
See Isolated tests.

## Gaps
- Procedural seed/entry and lifecycle/door/container were verified in two runs, not one (campaign triggers block the cave transition, see 1).
- Despawn owner state is not in the continue path (transient by design, P16 DESPAWN2 rule); not tested.
- Continued process: Space offers no object of interest on the opened destructible (see 1b); the in-process case logged a context call. Not resolved.
- Quest: reference has more rows; the initial selection rule on opening is still unknown (P16 gap).
- Drop test: 74 kills per build; a difference of about 15 points would not be excluded.

## Placeholders
- `--resume-game` now implies no bind banners (test/continue aid, documented in code).
- The `--interact-at` and `--quest-debug-*` flags are test aids.

## Package files required
- rc3 package (`windows-source-clock-v19-preview-15-rc3`) for save, quest and drop runs.
- `procgen2/asset-root3` overlay for the witch-cave runs (as P17INT).

## Verifier script
1. Build: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p17verify17 -Jobs 6`.
2. Save/reload: `quiet_run.ps1 -JobsFile verify17/save/jobs-s4.json -Parallel 1`; grep `Saved live checkpoint frame=5400`, `Restored live checkpoint frame=5460`, `Door view _prim_SwampDoor state=1`.
3. Continue: `jobs-s6.json` (after `s4-gameplay.save` copied to `s6-in-gameplay.save`); grep `Restored live checkpoint frame=0`, `Door view _prim_SwampDoor state=1`, and no `caption frame=2`.
4. Cave: `witch/jobs-b1.json` then `witch/jobs-b2.json`; grep `Procedural level generated ... seed=4058064753`.
5. Quest: `quest/jobs-qq17.json` and `quest/jobs-qq16.json`; `quest/jobs-d-detail.json`, `quest/jobs-d-completed.json`.
6. Drops: `drops/jobs-merged.json`, `drops/jobs-p16.json`; `drops/analyse.sh merged|p16`; `drops/fisher.awk`.
