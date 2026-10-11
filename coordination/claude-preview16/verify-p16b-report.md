# verify-p16b: Preview 16 rc2 independent verification

## Setup
- EXE: `.local-inputs/windows-source-clock-v19-preview-16-rc2/dh-foundation.exe`, SHA256 `1283C3D36D38FE659443652020E698D9F841A28DF59B7B1904FB376C53E46912` (matches manifest.json).
- One quiet batch: `port/windows-foundation/tools/quiet_run.ps1 -JobsFile verify-p16b/jobs.json -Parallel 8` (hidden desktop, DH_AUDIO_SILENT=1). 24 jobs, all finished. Summary: `verify-p16b/summary.json`. No audio-cadence run (user decision).
- Jobs: `.local-inputs/claude-preview16/verify-p16b/jobs/<name>/` (run.args, run.log, caps/). Generator `verify-p16b/gen/gen.js`, PPM to PNG `verify-p16b/gen/ppm2png.js`. Each job has its own copies of configs and saves, and junctions to the package asset folders. The package was not written (0 files newer than the generator).
- Quest/map/level-up/F5/F9/continue inputs: the P0 post-opening save and levelup save from the fix16 recipes (`verify-p16/recipes/`), produced by the RC2 build. Not re-produced in this batch (one-batch rule).

## Results
| # | Item | Result | Evidence |
|---|------|--------|----------|
| 1a | Boot, title, loading art | PASS | `boot-newgame/caps/b12.png` ("Touch the screen to continue", title art); `load-040.png` (LOADING, tip, bar) |
| 1b | NEW GAME opening: captions, HUD hidden, SKIP drawn | PASS | Captions from frame 2 ("The Boglands - Ancient Prison"), 1441 ("...Hero!"), 3205-3573 (boot-tap). `boot-newgame/caps/f300.png`: SKIP drawn, no HUD |
| 1c | Opening reaches its end | PASS | boot-tap: `EndScriptedCutScene` at frame 3947 (with auto-tap 300 ms; the taps are a harness input) |
| 1d | SKIP press ends/advances cutscene | FAIL | Campaign SKIP at 600 (`boot-skip-action`): commands sampled with skip=1, but at 890 (`f890.png`) the cutscene is still on screen with SKIP drawn; no end marker |
| 1e | SKIP touch at (60,25) at 900 | FAIL (harness) | `boot-skip-touch`: no skip=1 sampling, scene continues (commands at 1066 normal); `f960.png` SKIP still drawn. Hardware touch not tested |
| 1f | Movement tutorial after opening | PASS | boot-tap: `Movement_Tuto` at 3949, `Movement_Tuto2` dialog 4263, ends 4631; `f4300.png` "Movement Tutorial" panel with original HUD art. SKIP is still drawn during this dialog (observed, not judged) |
| 1g | Lizard ambush after opening | NOT-RUN | No walk into the ambush zone in boot-tap; `LizardMan_Ambush1` never started |
| 2 | Play-swamp.cmd path | CAVEAT | play-swamp: exit 0, 900 frames, clean shutdown; the production swamp.args start the opening at frame 2 (root decision, as RC1) |
| 3 | Walk-over pickup, no key | PASS | walk-pickup: `World item pickup frame=50 id=Bow02 reason=walkover picked=1`; chest opened by an interact request (not a key) |
| 3b | Space does not pick up | PASS (weak) | space-nopick: two Space presses (frames 20, 60), no pickup line. Player stood still, so this is not a walk-over test |
| 4a | Space opens chest | PASS | space-chest: `Context button frame=20`, `Container opened ... loot=227 frame=34` |
| 4b | Space breaks barrel, moth summon (seed 4) | PASS | barrel-moth: barrel opened frame 36 (loot=9); `barrel-moth/caps/f120.png` shows a "Muck Fly" with HP bar and "1 GOLD" |
| 4c | Enemy priority | PASS (one case) | enemy-priority: Space targets Bogwomp (`f60.png`, target ring, MISS), barrel not hit. No contrast case |
| 5a | Witch zone dialogue, error-free, NEW QUEST | NOT-RUN | witch-zone: player stayed at (-11800,11500), outside the witch zone box (y from 11676); a merchant-camp dialogue (frame 2) blocked movement. No witch dialogue, no error. Not re-verified here |
| 5b | NEW QUEST banner | PASS | levelup `f151.png`: "New Quest - Escape the swamp with Rene and Celeste." (quest row 50) |
| 5c | Kill objective | NOT VERIFIED | quest-kill: Space attacks, no objective progress line |
| 5d | Quest Log: Assigned and Completed rows | PASS | quest-log `f235.png`: "Prison Break" in the list (selected), COMPLETED header; detail pane empty |
| 6 | Map tab | PASS | map `f235.png`: The Boglands, visited area, player marker, Show legend, Reset zoom. Log: `Character menu Map selected frame=150 ... zones=9` |
| 7 | Level-up placeholder | PASS (presentation) | levelup: `Level up presentation frame=143 level=2 ... text=LEVEL UP! drawn=0`; `f151.png` light column behind banner, no LEVEL UP text. XP from hand-set save |
| 8 | Despawn about 3 s after kill | PASS (log) | despawn-a and despawn-b: kill frame 120, death end 183 (delay_ms=2000), hidden 307. 187 frames at 0.016 s = 2.99 s. Visual not checked |
| 9a | 15.2 fix: title | PASS | 1a |
| 9b | 15.2 fix: loading art | PASS | 1a |
| 9c | 15.2 fix: equipment page | PASS | equipment `f95.png`: Torso page, AUTO-EQUIP, UNEQUIP/EQUIP panels, Ceremonial Garb |
| 9d | Helm on hero | NOT-RUN | No helm item id found in the package |
| 9e | Red X on unmet requirement | NOT-RUN | Not reached |
| 10a | F5 save / F9 load | PASS | f5f9: `Saved live checkpoint frame=120 HP=165.098`, `Restored live checkpoint frame=200 HP=165.098`, no rejection |
| 10b | Fresh-process continue, no intro replay | PASS | continue-fresh: `Quest runtime bound rows=64 actors=42 current=50`, 0 Swamp_Intro commands, `f600.png` HUD and play. Continue is the menu slot then Start (`btn_MENU_CONTINUE` is not an enabled button); saved position not restored |
| 11 | Regression combat vs 15.2 (same seeds) | NOT-RUN | Both 15.2 jobs exit 1: `Unknown argument: --campaign-triggers` (my args error: the 15.2 jobs kept the campaign lines). RC2 combat jobs (seeds 1234, 99) exit 0, 700 frames, clean shutdown, but 0 combat hits and 0 damage lines, so no drop/XP comparison |
| x | Asset gap / save rejections | PASS | 0 "Asset not found", 0 "Save refused"/"checkpoint rejected", 0 "cutscene aborted", 0 Foundation errors in RC2 jobs |

## Verdict per item
- 1 boot/title/loading/opening/HUD/tutorial: APPROVE WITH CAVEATS (ambush not reached)
- 1 SKIP: REJECT (action advances commands but cutscene stays on screen; touch path no effect in harness)
- 2 Play-swamp: CAVEAT (starts the opening)
- 3, 4: APPROVE WITH CAVEATS (enemy priority and Space-no-pickup thin)
- 5: NOT VERIFIED for witch dialogue and kill objective; Quest Log and banner PASS
- 6, 7, 8, 10: APPROVE WITH CAVEATS
- 9: title, loading, equipment PASS; helm and red X NOT-RUN
- 11: NOT-RUN

## Overall verdict: REJECT
Reasons: SKIP does not end the opening cutscene (item 1). Witch dialogue and kill objective were not verified in this batch. Regression A/B not run (my args error on the 15.2 side).

## Not verified / next batch (needs user decision, one-batch rule)
1. 15.2 A/B: rerun the two 15.2 jobs with `swamp-nocamp` args (drop the three campaign lines). Keep RC2 jobs.
2. Witch: position inside `_prim_WitchQuestStart` (x -12552..-11090, y 11676..12817), e.g. (-11800, 12000, 255), with no merchant-camp dialogue.
3. Lizard ambush: walk into `_prim_TriggerZone_001_ChestAmbush` (x -6226..-5975, y 4887..5119) after the opening.
4. Helm and red X: find the helm item id and an unmet requirement.
5. Hardware touch on SKIP is untested.
