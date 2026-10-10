# Visual verification report (verifier: visual)

Candidate: `.local-inputs/windows-source-clock-v19-preview-12-candidate/dh-foundation.exe`, SHA256 `4E872FE0BC27112B45AA84A7F5FE3196235942A534820C808E8DCD3F9913DABE` (verified, matches brief).
Baseline: `.local-inputs/windows-source-clock-v19-preview-11/dh-foundation.exe` (differs from candidate; not re-hashed, receipt lists `FD850478...`).
Work folder (git-ignored, private): `.local-inputs/claude-preview12/verify-visual/` (copies of both packages, `runs/`, `img/`, `sw/`, `ref/`, `scripts/`). Neither package in the coordination/candidate/preview-11 folders was written to. No `dh-foundation.exe` process was left running (every run was foreground, exited with `EXIT=0`).

## Method

Scripted fight (`verify-visual/scripts/run.sh`), based on preview-11 `swamp.args` (Knight, Swamp, `--hud`, `--combat-text`, fixed seed 1234, `--fixed-step .016`):
- `--position -6750,938,250` (Knight placed ~100 units from Swamp lizard `7118915781085668844`, the default start position is too far to fight anything)
- `--target-frame 5` (selects lizard `7118915781085668844`; log: `Source target command frame=5 ... selected=7118915781085668844`)
- `--space-key-interval` at `60:20 130:25 200:25 270:25`
- `--capture <frame>.ppm` (capture is only written at the final frame, so each frame in a series is a separate run with `--frames N`)

Both builds show the same combat sequence for the first ~146 frames: lizard hits at 14/44/136, Knight hits at 76 and 109, and kill of `7118...` at frame 146 (`dead=1`). The second lizard `4141...` dies at frame 249 in P11 and in the candidate's full 330-frame run.

Known run-to-run jitter (candidate only in one case): some candidate runs with `--frames 256` gave a different hit at frame 76 (`removed=0`) and the lizard kill moved to frame 179 (`cand-post249`, `cand-post249b`). The log shows `Audio ... No valid caller-paired WinMM TIME_SAMPLES/QPC` lines differing between runs, so audio timing feeds into combat timing. Such runs were not used for conclusions except as noted.

## (1) B038 XP bar

### Log evidence
- `Character name=Player class=KnightPlayerBase xp=0` (start of every run).
- The engine does NOT print XP after a kill. The only XP-related lines are `Selected profile source projection ... XPraw=` (session creation only) and `Character name ... xp=` (startup only). The brief's assumption that "the log prints XP and level" is not true for this build, so exact XP values could not be read from the log.
- Kill lines: `Damage frame=146 ... removed=12.7656 dead=1` (cand-fight2 / cand-end330 / p11-fight2), `Damage frame=249 ... removed=14.1914 dead=1`.
- The save-frame option (`--save-frame 160`) did not write a checkpoint in combat (no `Saved live checkpoint` line), so no XP was read from a save.

### Pixel measurement
Capture 1201x720. XP bar is the thin strip above the red HP bar, rows y=48..51, x about 116..235 (bar span about 120 px; left end partly hidden under the portrait orb). Green fill counted per row (G > R+20, G > B+15, G > 50):

| Capture | State | Green px (row 48, 49, 50, 51) | Fill extent (x) |
|---|---|---|---|
| cand-pre146 (frame 140) | 0 kills | 0 / 0 / 0 / 0 | none |
| cand-post160 (frame 160) | 1 kill (146) | 7 / 5 / 4 / 4 | 116..122 |
| cand-pre249 (frame 245) | 1 kill | 7 / 5 / 4 / 4 | 116..122 (identical) |
| cand-post249b (frame 256, diverged run) | 1 kill (179) | 7 / 5 / 4 / 4 | 116..122 (identical) |
| cand-end330 (frame 330) | 2 kills (146, 249) | 17 / 15 / 14 / 14 | 116..132 |
| p11-pre146 (140) | 0 kills | 0 | none |
| p11-post160 (160) | 1 kill | 0 | none |
| p11-pre249 (245) | 1 kill | 0 | none |
| p11-post249 (256) | 2 kills (146, 249) | 0 | none |

So the candidate fill grows left to right: about 7 px (about 6% of the bar) after the first kill and about 17 px (about 14%) after the second. P11 stays empty after the same kills. Visual check: `verify-visual/img/xp-stack.png` (candidate empty, candidate 1 kill, candidate 2 kills, P11 empty, P11 2 kills).

Expected percentage (100*XP/XP-for-level) could NOT be checked: XP per kill and the level threshold (`resolved[33]/[34]`) are not logged, and the save does not expose them without a checkpoint. Only the growth is measured.

Side note (observation, not a verdict item): in the candidate the bar's right end cap (x=236, y=48..51) is green-tinted even when empty (66,92,54), while P11 shows grey (63,60,53). Could be the new XP frame's background; flag for fidelity.

### Reference video
Frames: `verify-visual/ref/xp/x_NN.png` (2 fps, from t=355 s), enlarged crops `ref/xp-bar-crops.png`, `ref/xp-sheet.png`. Observed: a green XP strip above the red HP bar, growing left to right, dark remainder at right end. Roughly 125 px of 145 px at t=355..363, about 140 px at t=364..375 (approx 84 to 95 percent, same as the implementer's visual estimate). The reference bar was already partly full, so only the shape and growth direction are comparable to the candidate, not the amount. No level-up was seen in the reference window (t=355..380; the SKIP overlay covers the HUD from about t=377).

### Level-up
NOT RUN. A level-up needs more kills than this scenario provides (about 17 kills at the observed per-kill fill), and the log does not report the level.

### B038 verdict: APPROVE (for the reported defect: the XP bar now fills after XP is awarded)
- PASS: bar empty before kill, grows after each kill, grows left to right like the reference; P11 stays empty.
- INCONCLUSIVE: exact 100*XP/need percentage (no XP values in log or save), and level-up carry/reset (not reached).

## (2) B041 swoosh

### Log evidence (candidate; P11 identical)
Every swing run (frames 73..82) contains:
`Source player step FX frame=73 actor=18446744073709551615 sequence=470 step=1 occurrence=7 dispatched=0 sets= diagnostic=Required source animation step FX anchored PlayAnimFXSet: Exact authored FX resource not found (basename fallback disabled): data/3D/interface/swoosh_prince_1hand_combo_01.bdae`

All 6 candidate runs in frames 80..82 and the P11 runs show `dispatched=0`. The package has no such file: `assets/data/3d/interface/` contains only `quest_marker.bdae`, `skill_dh2_faery_lightning.bdae`, `skill_dh2_prince_warrior_bash_down.bdae`, `spell_dh2_faery_lightning.bdae`, `target_circle.bdae` (and no swoosh). The file exists only in the repo cache (`.local-inputs/character-fx-owner-v1/cache/swoosh_prince_1hand_combo_01.bdae`) and in the B041 build folder.

### Pixels
Frames 58..82 (one run per frame, after the first Space press at frame 60 and the first basic-swing hit at 76/77), candidate and P11, contact sheets `verify-visual/img/sw-cand-58-82.png` and `img/sw-p11-58-82.png`. Crop (400..760, 80..400) per frame. Count of pale blue/white-blue pixels (B>170, B-R>50, G>120): **0 in every frame, in both builds.** The sword moves through the swing but no trail is drawn.

Reference (`ref/swoosh/s_NN.png`, 6 fps from t=262.5 s; sheet `ref/swoosh-sheet.png`): a pale blue translucent arc/disc under and beside the Knight is visible at about t=264.0 s (s10) and t=264.5..265.0 s (s13..s16). That is the expected trail, and the candidate does not draw it.

### B041 verdict: REJECT
- The swoosh is not dispatched in the candidate: the authored FX resource is not found by the live path (`dispatched=0`).
- No pale blue trail in the candidate in any of 25 frames around the first swing, identical to P11 (no regression, but not fixed).
- This matches the B041 report 2, which states that no production change was made.

## Other observations (not verdicts)
- Combat is reproducible in P11 but not always in the candidate with audio on (WinMM timing changes the hit at frame 76 and the lizard kill frame). Use `--frames` values that match the earlier runs when comparing.
- The candidate and P11 differ in XP-bar right-cap tint (see above).

## Commands (examples; all via `verify-visual/scripts/run.sh`)
- Candidate, two kills: `POS="-6750,938,250" scripts/run.sh cand end330 330 5 "60:20 130:25 200:25 270:25"`
- Candidate, pre/post first kill: `... cand pre146 140 ...`, `... cand post160 160 ...`
- Preview 11 equivalents: `... p11 ...`
- Swoosh series: `for n in $(seq 58 82); do scripts/run.sh cand sw$n $n 5 "60:20 130:25 200:25 270:25"; scripts/run.sh p11 sw$n $n 5 "60:20 130:25 200:25 270:25"; done`
- Frame conversion/measure: `scripts/conv.py`, `scripts/crop.py`, `scripts/fill.py`, `scripts/blue.py`, `scripts/swsheet.py` (Python: `C:/Users/adamc/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe -I`).
- Reference frames: `ffmpeg -ss 262.5 -t 3 -vf fps=6` and `ffmpeg -ss 355 -t 26 -vf fps=2` on `.local-inputs/reference-video/dh2-act1/Dungeon Hunter 2 (v1.0.3) Part 1 [720p] [z_Zky7qQdYs].mp4`.

## Final verdicts
- B038 (XP bar fills): **APPROVE** for the reported defect. Open: numeric scale vs 100*XP/need and level-up reset not verified (log does not print XP; not reachable with the scenario).
- B041 (Knight swoosh): **REJECT**. Trail not visible in candidate, FX dispatch fails on a missing authored resource (`data/3D/interface/swoosh_prince_1hand_combo_01.bdae`), identical to Preview 11.
