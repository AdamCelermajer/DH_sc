# verify-p16c: Preview 16 rc3 independent verification

## Setup
- EXE: `.local-inputs/windows-source-clock-v19-preview-16-rc3/dh-foundation.exe`, SHA256 `3ACF50D2A249962156583C3F13CE010443F6AB65A94A7E0DBE423D71CF4D3D1D`. Matches manifest.json (17928192 bytes).
- One quiet batch: `port/windows-foundation/tools/quiet_run.ps1 -JobsFile jobs.json -Parallel 8`, hidden desktop, DH_AUDIO_SILENT=1. 37 jobs, all exit 0, all `Rendered frames=N; clean shutdown`, no `aborted`, `Foundation error`, `Asset not found`, `refused` or `rejected` lines. No audio-cadence run (user decision). No follow-up batch was needed.
- Folder: `.local-inputs/claude-preview16/verify-p16c/` (generator `gen/gen.js`, `jobs.json`, `summary.json`, `jobs/<name>/` with run.args, run.log, caps/). Captures are PPM and PNG. Package not written.
- Inputs: post-opening profile P0 from `claude-preview16/verify-p16/recipes/p0-profile`, levelup profile, and `claude-preview15/fix061/saves/bagTorsoHelm.save`. Combat recipe from `claude-preview15/audiostall/p3-rc2-1/run.args` with the audio options removed.

## Results
| # | Item | Result | Evidence |
|---|------|--------|----------|
| 1a | SKIP by mouse click at the drawn SKIP (frame 300) | PASS | skip-click300: `SKIP pressed`, `SKIP fast-forward passes=2 frame=302 ok=1`, 8 `EndScriptedCutScene ... skip=1 frame=302`, 0 aborts. `caps/f299.png`: SKIP drawn, no HUD. `caps/f320.png`: HUD back, "New Quest: Escape the swamp with Rene and Celeste" banner. `caps/f1100.png`: HUD and play, no SKIP. |
| 1b | Mouse click at SKIP at frames 1000 and 2500 | PASS | skip-click1000 and skip-click2500: same markers at 1002 and 2502. Final state lines (Actor, Lifecycle, Quest banner, Quest runtime) identical to skip-played at 4700 (0 differing lines). |
| 1c | Enter at frames 500 and 1500 | PASS | skip-enter500 and skip-enter1500: `SKIP pressed`, fast-forward in the press frame (501, 1501), 8 End commands with skip=1. Final state identical to played (0 differing lines). |
| 1d | Click at (300,200), outside SKIP | PASS (negative control) | skip-miss600: no `SKIP pressed`, no End markers. |
| 1e | Post-skip state vs the played opening (4700 frames) | PASS, one caveat | skip-click300 differs from played in 2 lines only: player Z 258 grounded=0 (played) vs 255 grounded=1 (skip). Quest, lifecycle and actor lines otherwise equal. |
| 1f | F5 save and F9 load after the skip, no replay | PASS | skip-f5f9: `Saved live checkpoint frame=1000 HP=165.098`, `Restored live checkpoint frame=1200 HP=165.098`, no rejection. `Swamp_Intro` commands last at frame 302 (the skip frame), none after the load. Caveat: that run ends at 1800 frames, so the end-state diff against played is not like-for-like. |
| 1g | Controls unlocked after the skip | PARTIAL | skip-click300 frame 35 player events show `controllerBlocked=1`. Frame 351 player event shows `controllerBlocked=0`. Space presses were never scheduled (`Semantic Space ... scheduled=0`), so no attack-based proof. |
| 1h | Fresh-process continue after a skip | NOT-RUN | Needs a chained job (skip run, then continue). Not possible in one batch. |
| 2a | Witch quest zone, player inside, no error, NEW QUEST | NOT VERIFIED (blocked) | witch-inside: player at (-11800,12000,255), inside `_prim_WitchQuestStart` (x -12552..-11090, y 11676..12817). Merchant-camp safe zone entered at frame 2 with a StartDialog ("Bogwalker Camp"), so `Swamp_Witch_Activate` never ran. `caps/f200.png`: a scripted scene with SKIP drawn. No witch dialogue, no witch NEW QUEST banner. |
| 2b | Kill advances a quest objective | NOT VERIFIED | kill-quest (P0, seed 1234): 2 lizard kills at frames 137 and 215 (`Source death reward`). `Character menu Quest Log selected frame=2060`. `caps/f2080.png`: "Prison Break" selected, COMPLETED header, detail pane empty. The build logs no objective-progress line, and the victims were lizardmen, not Bogwomps. |
| 2c | Lizard ambush after the opening | NOT VERIFIED (not reached) | ambush-after-opening: `trigger zone fed: _prim_TriggerZone_001_ChestAmbush ... LizardMan_Ambush1` is logged, but the player ended at (-6382.69, 5063.52). The zone's x range is -6226..-5975, so the player never entered it. `LizardMan_Ambush1` never ran; no CombatTuto captions. |
| 2d | Helm visible on the hero after equipping | NOT VERIFIED | helm-on and helm-off with `--equipped-item PlateHelm01`: no `PlateHelm` line in either log, so the item was not applied. `caps/f100.png` (both): hero under the NEW QUEST banner, identical frames. |
| 2e | Red X or dark EQUIP on an unmet requirement | NOT-RUN | equip-unmet (bag from `bagTorsoHelm.save`): `caps/f95.png` shows only Ceremonial Garb in the torso list, so PlateArmor01 did not load. The Imbued Armor REQ 7 ENG item id was not found in the package. |
| 3 | Combat A/B vs 15.2, 6 seeds, identical args | PASS on counts, CAVEAT on HP and minions | Seeds 1234, 99, 1, 2, 7, 42. Both EXEs exit 0 with clean shutdown. Hit counts equal (7, 6, 7, 6, 6, 8). Death rewards equal (xp 8 and 16). World item counts equal. See details below. |
| 4a | Boot to menu, NEW GAME starts the opening | PASS (log) | spot-boot-newgame: `caption frame=2 ... The Boglands - Ancient Prison`, `Swamp_Intro` runs. Frame images not viewed in this pass. |
| 4b | Walk-over pickup, no key | PASS | spot-walk-pickup: `World item pickup frame=50 ... Bow02 reason=walkover picked=1` |
| 4c | Space opens chest | PASS | spot-space-chest: `Context button frame=20`, `Container opened ... loot=227 frame=34` |
| 4d | Space breaks barrel, moth summon | PASS | spot-barrel-moth: `Container opened ... loot=9 frame=36`. `caps/f120.png`: "Muck Fly" with HP bar and "1 GOLD". |
| 4e | Quest Log with Assigned and Completed | PASS (log, plus same layout viewed) | spot-quest-log: `Quest Log selected frame=150`. Layout viewed in kill-quest `caps/f2080.png`: Prison Break in the list, COMPLETED header. spot-quest-log image not viewed. |
| 4f | Map tab | PASS (log) | spot-map: `Map selected frame=150 level=data/scene/001_swamp.mlx zones=9`. Image not viewed. |
| 4g | Level-up placeholder | PASS (log) | spot-levelup: `Level up presentation frame=143 ... text=LEVEL UP! drawn=0`. Image not viewed. |
| 4h | Despawn about 3 s after kill | PASS | spot-despawn: kill frame 120, `DESPAWN death end ... delay_ms=2000 frame=183`, `delay expired ... hidden frame=307`. 187 frames at 0.016 s = 2.99 s. Visual not checked. |
| 4i | Equipment page | PASS | equip-unmet `caps/f95.png`: Torso page, AUTO-EQUIP, UNEQUIP and EQUIP panels, Ceremonial Garb. spot-equipment has the same markers, image not viewed. |
| 4j | Fresh continue, no intro replay | PASS | spot-continue: `Quest runtime bound rows=64 actors=42 current=50`, 0 `Swamp_Intro` commands. |
| 4k | F5 save and F9 load | PASS | spot-f5f9: `Saved live checkpoint frame=120 HP=165.098`, `Restored live checkpoint frame=200 HP=165.098`, no rejection. |

## Combat A/B detail (6 seeds, identical args, RC3 vs 15.2)
| Seed | Hits RC3 / 15.2 | Deaths | Death xp | World items RC3 / 15.2 | Player HP RC3 / 15.2 | Hit frames identical | Moth-minion lines RC3 / 15.2 |
|---|---|---|---|---|---|---|---|
| 1234 | 7 / 7 | 2 / 2 | 8, 16 | 3 / 3 | 92.39 / 123.67 | yes | 7 / 0 |
| 99 | 6 / 6 | 2 / 2 | 8, 16 | 1 / 1 | 101.45 / 140.77 | no (1-3 frames) | 7 / 0 |
| 1 | 7 / 7 | 2 / 2 | 8, 16 | 1 / 1 | 140.09 / 140.09 | no (1-3 frames) | 7 / 0 |
| 2 | 6 / 6 | 2 / 2 | 8, 16 | 3 / 3 | 100.95 / 149.86 | yes | 7 / 0 |
| 7 | 6 / 6 | 2 / 2 | 8, 16 | 4 / 4 | 139.44 / 139.44 | no (1-3 frames) | 7 / 0 |
| 42 | 8 / 8 | 2 / 2 | 8, 16 | 1 / 1 | 122.98 / 122.98 | yes | 7 / 0 |

- The RC3 runs log 7 `Swamp_Moth_Minions` spawn-pool actor lines (HP 36, position 0,0,0). The 15.2 runs log none. They are not hits or deaths.
- Player final HP differs in 3 of 6 seeds (1234, 99, 2), always lower in RC3. This is an unexplained difference from the 15.2 build under identical args. The verifier cannot attribute it. No crash, no abort.
- Drop counts and XP match. The combat result is not identical frame-for-frame, so this is a caveat, not a clean pass.

## Verdict per item
- 1 SKIP (click, Enter, HUD, unlock, state, F5/F9 no replay): APPROVE. Controls unlock is supported by the `controllerBlocked` flag only, not an attack. Post-skip continue not run.
- 2 Witch zone, kill objective, ambush, helm, red X: NOT VERIFIED or NOT-RUN. Nothing failed; none of them were reached.
- 3 Combat A/B: APPROVE WITH CAVEATS on counts; the RC3 player-HP and minion differences need a root explanation.
- 4 Spot re-checks: APPROVE WITH CAVEATS. Log PASS everywhere. Images viewed for pickup/chest/barrel/Quest/despawn/equipment layout, not for boot, map, level-up or continue.

## Overall verdict: REJECT
Reasons:
1. Unexplained RC3 vs 15.2 player-HP difference in 3 of 6 identical-args combat seeds, plus extra moth-minion pool actors in RC3. Root must explain or fix this before the combat regression is signed off.
2. Witch zone (blocked by the merchant-camp cutscene), kill-objective progress, lizard ambush (player outside the zone), helm on the hero, and red X on an unmet item were not verified in this batch.

The SKIP fix itself is verified (approved above).

## Not verified / gaps (explicit)
- Witch dialogue and NEW QUEST from the witch: blocked by the merchant-camp safe zone at frame 2.
- Kill advancing a quest objective: no progress log in the build; the Quest Log detail pane is empty.
- Lizard ambush and CombatTuto captions: player did not enter the zone.
- Helm on hero: the equip argument was not applied; the frames are identical.
- Red X / dark EQUIP: unmet item not reachable from the bag used here; Imbued Armor id not found.
- Fresh continue after a skip: needs a chained job.
- Controls unlock after skip: flag-based evidence only.
- Hardware touch on SKIP, gamepad, audio cadence (no run by user decision).
- Combat HP and minion difference from 15.2: cause not isolated.
