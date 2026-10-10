# Preview 15.1 rc1 verification report (verifier)

Status: COMPLETE (audio-cadence runs skipped by user decision)

## EXE under test
- `.local-inputs/windows-source-clock-v19-preview-15-1-rc1/dh-foundation.exe`
- SHA256 35759A8A3F2859FB880CD4D8C7E82907B09416386187C59CE8F669D0C4BBA2B8 (confirmed match)
- Source: p15/patch 233f16bb (DH_wt/p15patch)
- Baseline (A/B): `.local-inputs/windows-source-clock-v19-preview-15/dh-foundation.exe`
- Both EXEs run with the 15.1 package assets, quiet_run.ps1 (hidden desktop, DH_AUDIO_SILENT=1).
- Jobs, args, logs, captures: `.local-inputs/claude-preview15/verify-151/jobs/<name>/` (run.log, *.ppm/*.png).

## Delta checks (fix reports B053-B063)

| Item | Check | Result | Evidence |
|---|---|---|---|
| B053 | Title shows only splash + prompt; menu follows | PASS (title); menu reached | `jobs/b053-N/c12.png`: splash + "Touch the screen to continue" at ~58% height, no atlas sprites. Baseline `jobs/b053-B/c08.png` shows the whole atlas (rings, arrows, dots). 15.1 log `b053-N/run.log`: `Boot outcome=0 movie="skipped: user" presses_ignored=1`, so the title press (t=14 s) led to the menu. No menu frame captured in that job; the menu is reached in `e2e-N`. |
| B054 | Logo has no SKIP and ignores presses; story shows SKIP and a press skips | PASS | `jobs/b054-nopress-N/c03.png` logo, no SKIP. Baseline `jobs/b054-nopress-B/c03.png` logo WITH SKIP (the bug). `b054-nopress-N/c09.png` story with SKIP. `b054-logopress-N/run.log`: presses at 3 s and 5 s, `presses_ignored=2`, movie still playing at 10 s. `b054-storyskip-N/run.log`: press at 3 s ignored, press at 12 s, `movie="skipped: user"`. |
| B055 | Loading screen ornate art, tip, bar | PASS (caveat: placement) | `jobs/b055-N/load-040.png`: ornate frame, LOADING plate, filigree corners, tip panel, red bar with spark at 40%. Also `load-000/080/100.png`, `loading-end.png`. Baseline `b055-B` shows the old layout. Caveat from the branch report: stage zoom fitted to landmarks, side pillars cropped. |
| B056 | Equipment page vs original (grey/orange panels, damask list) | PASS (caveat: rail/row details) | `jobs/b056-N-rh/rh.png`: grey upper and orange lower panels, damask list, avatar shadow. Baseline `jobs/b056-B-rh/rh.png` shows the dark plate. Reference `user-shots/b056-REFERENCE-original-equipment-video-0832.png`. Not done per branch report: red X/green name in list rows (see B057), VALUE box, disabled EQUIP look, rail normal-state art. |
| B057 | Unmet requirement: red X + dark EQUIP; met item equips | NOT VERIFIED | Harness could not reproduce the unmet rows. With the bagA save and the fix057 recipe, both the 15.1 EXE (`jobs/b057p-N-torso/torso.png`, `b057-N-torso`) and the Preview 15 EXE (`jobs/b057q-B-torso/torso.png`) list only "Ceremonial Garb", no Imbued/Mystic Armor rows. The fix057 branch build (`fix057/runs/after-torso.png`) shows Imbued Armor and Mystic Armor with red X from the same inputs. Also tried with --skip-boot (rows missing) and the fix057 game save (15.1 run exited 1 with empty log, Preview 15 run unchanged). Open question: why the shipped EXEs do not list the rows. No red X or dark EQUIP observed in any packaged EXE run. |
| B060 | Helm and Belt rows: correct icons | PASS | `jobs/b060-N/eq.png`: top-right row "Conscript Helm" with helm icon, third row "Conscript Belt" with belt icon. Baseline `jobs/b060-B/eq.png`: Conscript Belt on the helm icon and Conscript Helm on the belt icon (swap reproduced). |
| B061 | Equipped gear visible on in-game hero | PARTIAL: helm PASS; torso/gloves/boots NOT VERIFIED | Helm: `jobs/b061h-N/run.log` `Player body parts: ... MC_Head_Plate_01`; crop `ch-pair.png` (left 15.1, right Preview 15): helm visible on 15.1 hero, bare hair on the baseline. Full four-piece set (PlateArmor03/Boots03/Gloves03/Helm03, `b061-N`, `b061r-N`): no "Player body parts" line in 15.1 logs; `b061r-N` logs `Equipment binding diagnostic: Shared CharacterState duplicates a source equipment slot`; no visible torso/gloves/boots change. The fix061 reference log does show the sync for this list, so the cause is unexplained (harness state or set-specific). |
| B062 | Frame times, DH_PERF=1, melee + skill, 3 runs, interleaved | INCONCLUSIVE (no verdict) | See the perf table below. The 15.1 EXE has DH_PERF output; the Preview 15 EXE has no DH_PERF code (`grep -c DH_PERF` = 0), so only wall-clock seconds for 520 frames are comparable. Machine load was visible (one 15.1 run max 3.1 s). |
| B063 | Walk-over pickup, no key; idle picks nothing; full inventory leaves item | PASS | `jobs/b063a-N-walk/run.log`: `World item target frame=148 ... ClothGloves01`, then `World item pickup frame=181 ... reason=walkover outcome=0 picked=1 stacks=4->5`. No key pressed. `b063a-N-idle`: target at 148, no pickup. `b063a-B-idle` (Preview 15): same target, no pickup, needs E (baseline). `b063a-N-full` (100 bag items): `pickup frame=181 ... outcome=4 picked=0 store=1`, item stays, retry at 215 with outcome 4. Caveat: `--move-segment` is a 15.1 test hook (Preview 15 does not accept it, so the baseline walk is replaced by the idle run). With `--audio` on, both EXEs log no drop in this harness; the walk runs were done with `--audio` off to match the fix063 reference. |

## Regression set

| Check | Result | Evidence |
|---|---|---|
| Production boot flow (movie, title, menu, loading, swamp) | PASS | `jobs/e2e-N/run.log`: Boot outcome=0 (movie skipped, presses at 6/9/14 s). `jobs/b055-N/run.log` loading tip lines. `jobs/e2e-N/swamp-end.png`: swamp HUD after loading (scripted menu actions to single player, 420 frames). |
| Save/load (F5/F9) | PASS (scripted) | `jobs/saveload-N/run.log`: `Saved live checkpoint frame=90 HP=165.098` and `Restored live checkpoint frame=150`. Live F5/F9 keys cannot be injected on the hidden desktop, so the scripted hooks `--save-frame`/`--load-frame` were used. |
| Combat run | PASS | `jobs/b063a-N-idle/run.log`: skill `BashDown` applied at frame 84, lizard `Combat cue ... event=death ... frame=148`, HP 0/47.5, drop target at 148. |
| Audio cadence run 1 (-Parallel 1, 35 s) | NOT-RUN (skipped by user decision) | Coordinator instruction: the user checks sound by hand. |
| Audio cadence run 2 (-Parallel 1, 35 s) | NOT-RUN (skipped by user decision) | Same as above. |

## B062 perf (wall-clock per 520 frames, quiet, sequential, `DH_PERF=1`)

| Scenario | Preview 15 (s) r1 / r2 / r3 | 15.1 (s) r1 / r2 / r3 | 15.1 DH_PERF avg / p50 / p95 / p99 (ms), r1 / r2 / r3 |
|---|---|---|---|
| melee | 12.7 / 20.3 / 14.0 | 20.9 / 32.9 / 10.9 | 17.7 / 15.3 / 30.9 / 54.8; 51.8 / 15.4 / 182.5 / 690.5; 16.0 / 15.1 / 17.2 / 42.0 |
| skill 1 | 15.8 / 29.6 / 14.2 | 16.5 / 38.6 / 14.1 | 19.3 / 15.3 / 36.1 / 72.0; 15.4 / 15.1 / 15.7 / 16.2; 17.7 / 15.1 / 30.3 / 30.9 |

Reading: 15.1 p50 is at the 15.1-15.4 ms floor in all 6 runs (the report's "after" value). Spikes and averages vary strongly between runs (load on this machine); the 15.1 run with 690 ms p99 is in the same batch as the others. The report's own before/after (with instrumentation on both builds, 3 interleaved runs each) is not reproduced here. Verdict: inconclusive, not a regression signal.

## Deviations from the brief
- Extra batches: the first parallel batch (-Parallel 8) had 4 B057 runs timing out (boot movie with no presses) and 3 walk runs with wrong start position, so those were re-issued in a second -Parallel 8 batch (`sum-fix.json`), then B057 and walk-over probes (`probe/`, `jobs-b057p/q`, `jobs-b061h/r`, `jobs-walk`).
- B062 ran sequentially (-Parallel 1) for timing fidelity, as a separate batch from the parallel batch.
- Audio cadence runs skipped by user decision (not run).

## Verdict (visual and behavioural fixes)

| Fix | Verdict |
|---|---|
| B053 title | APPROVE |
| B054 logo/story SKIP | APPROVE |
| B055 loading screen | APPROVE WITH CAVEATS (placement stretched, auto-continue, "touch to continue" not implemented) |
| B056 equipment page | APPROVE WITH CAVEATS (list rows, VALUE box, rail normal-state art still open) |
| B057 unmet requirement | NOT VERIFIED (no verdict; packaged EXEs do not show the rows in this harness) |
| B060 Helm/Belt | APPROVE |
| B061 hero gear | APPROVE WITH CAVEATS for helm only; torso/gloves/boots not verified |
| B063 walk-over pickup | APPROVE WITH CAVEATS (no video frame of an actual walk-onto-item; `--audio` drop issue in harness on both EXEs) |

Overall: APPROVE WITH CAVEATS. No regression seen in boot, save/load or combat. Blockers to resolve before claiming B057 and B061 fully fixed: the unmet-row case and the four-piece body sync. Audio-cadence checks not run (user decision).
