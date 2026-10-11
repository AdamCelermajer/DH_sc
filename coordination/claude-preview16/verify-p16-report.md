# verify-p16 report: Preview 16 RC1 verification (verifier)

Status: COMPLETE for one quiet batch (20 jobs, -Parallel 8, hidden desktop, DH_AUDIO_SILENT=1). No corrective second batch was run (ONE batch rule). Audio-cadence not run (user decision).

## EXE and setup
- EXE: `.local-inputs/windows-source-clock-v19-preview-16-rc1/dh-foundation.exe`, SHA256 `05B69C44E7139C0F4502187738CC618B4E39B109C394ED37DD6250286A1472CE` (matches manifest.json).
- Package untouched after the run: 0 files modified (checked by mtime), no saves in the package.
- Jobs: `.local-inputs/claude-preview16/verify-p16/jobs/<name>/` (run.log, caps/*.ppm, *.png). Batch: `verify-p16/jobs.json`, `verify-p16/summary.json`. Generator: `verify-p16/gen/gen.sh`. Each job hard-links the package, saves are independent copies.
- Batch summary: 18 jobs exit 0, `quest-log` exit 1 (Foundation error), `continue-fresh` exit 1 (harness: no character.save), `combat-152` exit 1 (harness: 15.2 EXE does not know `--capture-frames`).
- Harness finding that blocked most checks: the swamp.args campaign lines (`--campaign-commands original-campaign.xml --campaign-triggers`) start the opening at frame 2 because the start position is inside the prison start-room zone (`enterLocation_StartRoom`). The opening locks the controller and blocks saving (`Script_LockCharacter` frame 3, `Save refused: cutscene blocks saving`). The packager avoided this by dropping the campaign lines for Space/quest tests.

## Results

| # | Item | Result | Evidence |
|---|------|--------|----------|
| 1 | Production boot -> menu -> NEW GAME opening | PARTIAL | boot-newgame: title "Touch the screen" (`caps/b12.png`), loading tip and bar (`caps/load-040.png`), SKIP red X drawn and HUD absent in the cutscene (`caps/f300.png`, `f600.png`). Captions: frame 2 "The Boglands - Ancient Prison", frame 1441 "...Hero!" (`f1200.png`, `f2400.png` show the caption box). Script then waits for a tap (no `--caption-auto-tap-ms`) through frame 5000. Not verified: opening end, movement tutorial after opening, troll/Celeste scene identification. |
| 1b | SKIP works | NOT VERIFIED | skip-hit: authored click (60,25) at frame 900 via `--menu-release`; SKIP still drawn and scene continued at 905 and 960 (`skip-hit/caps/f905.png`, `f960.png`). Unclear whether the semantic click reaches the cinematic hit box (x 3-196, y 0-54). Action path only is covered by `--campaign-skip-frame` (not run). |
| 2 | Play-swamp.cmd path | CAVEAT | play-swamp exit 0, 900 frames, clean shutdown. Finding: the production swamp args start the opening (SKIP visible at `play-swamp/caps/f120.png`). Root to decide whether Play-swamp should start the opening. |
| 3 | Walk-over pickup, no key; Space does not pick up | NOT VERIFIED | walk-pickup: chest opened by scripted `--interact-at` (state 2->3 frame 20, opened frame 34, loot 227), Bow02 target at frame 35, but no walk-over pickup in 200 frames (controller locked). space-nopick: Space at 20 and 60 refused. |
| 4 | Space opens chest, breaks barrel (moth 25%), enemy priority | NOT VERIFIED | chest-space, barrel-moth, enemy-priority: every Space press `status=refused_controller_locked` (frame 20). Moth summon not reached. barrel-moth and enemy-priority both show `type=8` (enemy) at the barrel position, but refused, so priority is not verified. |
| 5 | Quests: witch zone accept, NEW QUEST banner, kills, Quest Log Assigned row, opening quest in log | FAIL | witch-zone: `Quest zones built=1 _prim_WitchQuestStart level=41`, no zone entry, and `cutscene aborted: Unsupported original campaign command kind 16 (Script_EnterSafeZone)`. quest-log: process exit 1, `Foundation error: Source camera target: Live Character camera anchor provider is required` during the witch dialogue (frame 56). quest-kill: accept of row 53 `refused: quest is not Available`, debug kills `current=-1` (no effect). Only banner seen: `NEW QUEST row=50` (not the witch). Quest Log page never rendered, so the opening-quest-missing-from-list claim is neither confirmed nor refuted here. Reference for the Quest Log: Part 2 t=523 (Assigned/Completed lists, SIDE QUEST tag, MAKE ACTIVE; see `coordination/claude-preview16/QUESTUI2-report.md`); reference frame `.local-inputs/claude-preview14/QUESTS/p2/f523.png` not viewed. |
| 6 | Map tab (visited rooms, level name, zoom) | NOT VERIFIED | map-a and map-b: no map-page log line; captures show gameplay (`map-a/caps/f150.png`). |
| 7 | Level-up (white column placeholder, no LEVEL UP text) | NOT RUN | levelup: Space kills blocked (controller lock); no level-up event. |
| 8 | Lizard ambush after opening; CombatTuto captions | NOT RUN | Opening did not finish (boot-newgame stalls at "...Hero!" waiting for taps). |
| 9 | Despawn after kill (corpse hidden ~3 s) | PASS (log) | kill-despawn: `KILL test ... _prim_MothTemplate_02 frame=120`, `DESPAWN death end ... delay_ms=2000 frame=183`, `DESPAWN delay expired ... hidden frame=307`. 187 frames x 0.016 s = 3.0 s. Visual not conclusive (cutscene camera moves between `f240.png` and `f330.png`). |
| 10 | B063-B067 and 15.2 fixes (title, loading art, equipment page, red X, helm) | PARTIAL | Title PASS (`b12.png`). Loading art PASS (`load-040.png`). Equipment page PASS: Torso page with rail, AUTO-EQUIP, UNEQUIP/EQUIP panels, Ceremonial Garb (`equipment/caps/f95.png`). Red X on unmet requirement: NOT RUN. Helm on hero: NOT RUN. |
| 11 | F5/F9 save/load; fresh-process continue (no intro replay) | NOT VERIFIED | save-load: `Save refused: cutscene blocks saving (campaign frame=90)`; F9 not exercised. continue-fresh: exit 1 `Source button is not active on the current menu` (no character.save, so Continue is inactive). Harness, rerun needed. |
| 12 | Regression combat vs 15.2 EXE, same seeds | PARTIAL | combat-p16 exit 0 (700 frames, seed 1234, no crash). combat-152 config error (`--capture-frames` unknown to 15.2), NOT RUN. No A/B comparison done. |
| extra | Asset gap | FINDING | Every swamp-start run logs `Asset not found: data/3D/AnimatedDecors/CS_wallbreak_death_cutscene.bdae`. Check the package asset list. |

## Verdict per item
- 1 boot/title/loading/HUD-hidden: APPROVE WITH CAVEATS (opening end and tutorial not reached)
- 1b SKIP: NOT VERIFIED
- 2 Play-swamp: CAVEAT (starts the opening; root decision)
- 3, 4, 6, 7, 8, 11 (save/continue): NOT VERIFIED / NOT RUN (harness controller lock and campaign blocks)
- 5 quests: REJECT (exit 1 Foundation error in quest path; witch cutscene aborts; Quest Log not seen)
- 9 despawn: APPROVE (log timing; visual caveat)
- 10 title/loading/equipment: APPROVE WITH CAVEATS (red X and helm not seen)
- 12 regression: NOT RUN (A/B not done)

## Overall verdict: REJECT (RC1 as packaged)
Reasons: the Quest path exits the process with a Foundation error and the Witch-zone campaign cutscene aborts on an unsupported command. Most gameplay checks (Space, pickup, map, level-up, F5/F9, continue) were blocked by the opening lock and therefore are not verified. Re-verify after the quest fix and a corrective batch.

## Corrective batch needed (for the next verifier pass, not run here)
1. Space, pickup, quest, map, level-up jobs: remove the two `--campaign-*` lines (as the packager did) or pass `--campaign-skip-frame`.
2. Opening runs: add `--caption-auto-tap-ms 300` so the opening reaches its end and the tutorial/ambush.
3. Continue: copy a `character.save` into the job and pass `--save character.save`.
4. 15.2 run: drop `--capture-frames`, use `--capture` only.
5. Investigate the Foundation error (`Source camera target: Live Character camera anchor provider is required`) on the witch dialogue and `Script_EnterSafeZone` (kind 16).
