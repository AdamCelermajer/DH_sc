# Preview 14 rc1 regression verification (verifier `regress`)

Verdict: **APPROVE WITH CAVEATS**. No regression found against the Preview 13 baseline or the accepted rc2 traces. Caveats: new-profile creation NOT-RUN; the save point is the scripted `--save-frame` (not a physical key press); baseline logs for kill-damage and some RNG-dependent timings diverge from the accepted rc2 traces (cause not isolated, see Caveats).

EXE under test: `.local-inputs/windows-source-clock-v19-preview-14-rc1/dh-foundation.exe`, SHA256 EA2DDB51...29541 (matches brief).
Baseline: `.local-inputs/windows-source-clock-v19-preview-13/dh-foundation.exe` (SHA256 418FDB56..., identical to the rc2 EXE; audio options stripped from the baseline args).
Runner: `port/windows-foundation/tools/quiet_run.ps1` only (hidden, silent).
Folder: `.local-inputs/claude-preview14/verify-regress/` (jobs, runs, images, scripts).
Batches: main `jobs-main.json` at `-Parallel 12` (38 jobs, all exit 0, `summary-main.json`); menu after-stage `jobs-after.json` at `-Parallel 2` (depends on the play-stage saves, exit 0); audio `jobs-audio.json` at `-Parallel 1` (B039 only, exit 0).
Job args: the Preview 13 `*-R` templates, adapted (package assets, own capture and save paths). Both EXEs use identical args; audio flags kept for P14 only.

## Results

| # | Check | Verdict | Evidence |
|---|---|---|---|
| 1 | Smoke Swamp 130 frames, exit 0, HUD intact | PASS | `runs/P-smoke/run.log`: `Rendered frames=130; clean shutdown`, exit 0. `images/P-smoke.png`: portrait, HP red, MP blue, keys 1-5 with Faery and Potion. R-smoke also exit 0. |
| 2 | B037 skill mid-swing accepted, swing cut | PASS | P-b037-hold36: `Source skill key=2 ... frame=36 generation=1 ... skill=BashDown MP=21.25`, FX frames 36/131/162/194/213/244, all `dispatched=1`. This is identical to the accepted rc2 trace `claude-preview13/verify-final/runs/b037-hold36-R`. P-b037-hold50: skill at 50 after combo 470 at 43, then 470/471/472 chain; identical to R-b037-hold50 frame for frame. rel8/rel20 exit 0. |
| 3 | B004 marker persists after skill/release | PASS | `Source skill key=2 frame=100`, `Retained state6 source Use/Post completed and authored ClearTarget` at 181; identical in P and R. `images/P-b004.png` (frame 200): target ring under Bogwomp, name, enemy HP bar present. |
| 4 | B038 XP bar grows after kills | PASS | P-kxp-200: `Source death reward frame=143 ... xp=8`. P-kxp-300: second kill `frame=240 ... xp=16`. `images/P-kxp-130.png` XP strip empty; `images/P-kxp-300.png` small fill visible (eye estimate, no pixel measurement). |
| 5 | B041 three combo swing FX dispatched=1 | PASS | P-b041-f260: `sequence=470 step=1 frame=43 dispatched=1`, then 471 at 74, 472 at 106, 471 at 138, 470 at 157/172, 471 at 203, all `dispatched=1`. Identical to R-b041-f260. |
| 6 | B043 Rogue Celest then potion works | PASS | P-rogue-potion: `Source Faery key=4 frame=30 ... MP=30.25`, `Source potion key=5 frame=60 result=1 consumed=1 quantity=5->4 MPraw=7744->10304`. Identical to R. The log labels the key-4 ability "Faery"; `Source Celest FX bound` is present. |
| 7 | B044 Rogue kill | PASS | P-b044-rogue-swamp: `Damage frame=175 ... dead=1`, `Source death reward ... xp=8`; second kill `frame=289 ... xp=16`; `Rendered frames=300; clean shutdown`. |
| 8 | B040 return-to-menu stop | PASS | P-b040-return (audio on): `Level music transition: kind=start track=SwampHubAmbientMusic ... ordinal=211` (one line), `Pause menu confirmed main menu frame=80`, `Level music transition: kind=return-stop ... fadeMs=1000` (one line), `Rendered frames=80; clean shutdown`. `Audio host is unavailable` count 0. |
| 9 | B039 35 s real-time WinMM 0 underruns | PASS | P-b039-rt (`-Parallel 1`): `WinMM pump: underruns=0 refills=3488 renderedSeconds=37.2053 wallSeconds=37.1427 maxGapMs=33.1 gapsOver40ms=0`, `Rendered frames=2100; clean shutdown`, exit 0, 40.1 s wall. |
| 10 | No hit pushes the hero | PASS (limited) | `images/crop-hit-13-P-hit-13.png` vs `crop-hit-17-P-hit-17.png`: ring, hero feet and sword stay at the same screen position across frames 13 to 17; damage "8" appears at 17. Same R. Limit: 4-frame gap, screen-space only, no push-bearing hit tested. |
| 11 | Menu slot metadata, Knight, before/after save point | PASS | Before (legacy 967 B, `P-menu-knight-before/frames/frame-000120-*`, `images/menu-knight-before.png`): `QA / Warrior / LEVEL 1`, Act/location/difficulty/last-save rows blank. Save point: `Saved live checkpoint frame=40 HP=165.098`, slot rewritten to 1008 B (schema 4 header `04 00 00 00`). After (`images/menu-knight-after.png`): `QA / Warrior / LEVEL 1 / Act 1 / The Boglands / Difficulty: Normal / Last Save 10/10 18:05`. |
| 12 | Menu slot metadata, Mage, before/after save point | PASS | Before (legacy 944 B, `images/menu-mage-before.png`): `QA / Mage / LEVEL 1`, rest blank. Save point: `Saved live checkpoint frame=40 HP=141` (985 B). After (`images/menu-mage-after.png`): `QA / Mage / LEVEL 1 / Act 1 / The Boglands / Difficulty: Normal / Last Save 10/10 18:05`. |
| 13 | New-profile creation stamps metadata | NOT-RUN | No scripted creation route. The schema report says the Start-game creation UI was not driven, and `main.cpp` has no creation option. |

## Caveats
- Save point: `--save-frame 40` takes the same branch as the F5 key (`main.cpp` L2746: `pressed(VK_F5)||drawn==options.saveFrame`), so the stamp path is the same. It is not a physical key press.
- Schema after-stage: the menu-after jobs depend on the play-stage saves, so they ran as a second small batch rather than in the main batch.
- Baseline divergence: the P13 EXE is byte-identical to the rc2 EXE, and the P13 package assets are identical to rc2 assets (`diff -rq` empty). Even so, the P13 baseline run in this batch diverges from the accepted rc2 traces: hold36 combo at 181/212 (accepted 194/213), kill damage `removed=2.05` (accepted 12.77), B044 kills at 143/272. The P14 runs reproduce the accepted rc2 traces exactly (hold36 36/131/162/194/213/244, kill `removed=12.7656`). The cause was not isolated. Likely the rc2 folder's root `character.save`/`startup.args` (absent from the P13 folder) affects the run. This is an environment difference, not a P14 regression.
- Audio: the `Unavailable original audio asset: data/sounds/sfx_lizardman_attack_1.wav` line appears in P14 audio runs, and also in the accepted rc2 audio runs (`claude-preview13/verify-final/runs/b039-rt-R`, `b037-*-R`). It is baseline, not a regression.
- Schema 4 is still not readable by the Preview 13 EXE (per the schema report).
- Brief names "Celest": the log labels the key-4 skill "Faery" in both builds.
