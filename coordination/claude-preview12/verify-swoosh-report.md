# Swoosh verification report (verifier: swoosh)

Candidate: `.local-inputs/windows-source-clock-v19-preview-12-candidate/` (not modified).
EXE SHA256 `4E872FE0BC27112B45AA84A7F5FE3196235942A534820C808E8DCD3F9913DABE` (checked, matches brief).
Swoosh resource `assets/data/3d/interface/swoosh_prince_1hand_combo_01.bdae` present, SHA256 `af8ef8a2...6180b` (checked).
Work folder (git-ignored): `.local-inputs/claude-preview12/verify-swoosh/`. It holds a copy of the candidate package (`cand-pkg/`), so the candidate folder was not written to. Every run was foreground and exited with `EXIT=0`. No `dh-foundation.exe` process was started outside these runs, and the user's preview-11 process was not touched.

## What was run

Scripts: `verify-swoosh/scripts/run.sh` (copy of the previous verifier's runner, adapted to the copied package), `trail.py`, `sheet.py`.

- Full fight: `POS="-6750,938,250" scripts/run.sh full330 330 5 "60:20 130:25 200:25 270:25"` (Knight near the Bogwomp, `--target-frame 5`, Space intervals as in the previous verifier).
- Swing series, one run per frame (capture is written only at the final frame): `POS=... scripts/run.sh sw$n $n 5 "60:20 130:25 200:25 270:25"` for n = 70..90 and 100..118.
- BashDown: `SKILL="90:2" POS=... scripts/run.sh skill120 120 5 "60:20 130:25 200:25 270:25"` and `SKILL="90:2" ... bd$n` for n = 90..112 (`--skill-key-frame 90:2`).
- Reference: `ffmpeg -ss 262.5 -t 3 -vf fps=8` on `Dungeon Hunter 2 (v1.0.3) Part 1 [720p] [z_Zky7qQdYs].mp4`, frames in `verify-swoosh/ref/s/`.

## (1) Log check: dispatch

Every run in the series shows (full330 log, identical in each frame-series run):
- `Source player step FX frame=73 actor=... sequence=470 step=1 occurrence=7 dispatched=1 sets=253, diagnostic=` (no resource error). Previously `dispatched=0`.
- `Source player step FX frame=143` and `frame=160`, `frame=213`, `frame=283` (sequence 470) also `dispatched=1`.
- Second basic swing, sequence 471: `Source player step FX frame=104 ... sequence=471 step=1 occurrence=10 dispatched=0 ... Exact authored FX resource not found (basename fallback disabled): data/3D/interface/swoosh_prince_1hand_combo_02.bdae`. The same error recurs at frames 244 and 314 (sequence 471, combo_02).

Finding: `swoosh_prince_1hand_combo_02.bdae` is still missing from the package. It exists in the repo cache (`.local-inputs/character-fx-owner-v1/cache/swoosh_prince_1hand_combo_02.bdae`), so the root must also stage `combo_02` (and `combo_03`, which the cache has too, if the Knight's chain uses it).

## (2) Visual check, first basic swing (frames as log steps; 1 frame = 0.016 s fixed step)

Crop at 420..740 x 80..390 of the 1201x720 capture. Sheet: `verify-swoosh/img/swing1-74-90.png`. Blue count (B>120, B-R>60, B>G) in crop 400..760 x 60..400, from `img/` frames:

| frame | 74 | 75 | 76 | 77 | 78 | 79 | 80 | 81 | 82 | 83 | 84 | 85 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| blue px | 0 | 0 | 59 | 1108 | 3266 | 8225 | 8800 | 8584 | 6965 | 4890 | 1961 | 0 |

- Observed: a blue crescent appears at frame 76 (thin, top right of the Knight, next to the blade), widens to a broad arc around the Knight at 79..82 (sweeping from upper-right round to lower-left, passing the Bogwomp), then fades 83..84. It is gone from frame 85. The FX is dispatched at frame 73 and the hit lands at frame 76; the arc first becomes visible at 76, on the hit.
- Shape/colour: a translucent arc with a bright rim, saturated mid-blue, not the pale white-blue of the reference. Radius is roughly the Knight's reach.
- Duration: about 8 to 9 frames above threshold (frames 76..84), so about 0.13 s of game time at the fixed step.

Note: the previous verifier's pixel count (B>170, B-R>50, G>120) gave 0 in every frame. That threshold excluded the darker blue of this trail, so their "no trail in any frame" was a measurement artifact. Their P11 comparison is still valid (no trail in P11 per their report), but I did not re-run P11.

## (2b) Second basic swing (frames 100..118, combo_02)

Blue count 0 in frames 104..118, in the same crop. Sheet not needed: no arc visible in the captures. Dispatch was `dispatched=0` (combo_02 missing), so no trail is expected. This is the expected failure.

## (3) Reference comparison (v1.0.3 video, 8 fps from 262.5 s)

Sheet: `verify-swoosh/img/ref-zoom-09-16.png` (crop of frames s_09..s_16).
- Observed: a pale, translucent white-blue disc/arc sweeps around the Knight. It is visible at s_09 (263.5 s, over the red hit splash) through s_14 (264.1 s), with the largest arc at s_13..s_14. Its radius is larger than the candidate's (it reaches well past the Bogwomp). The colour is paler (white-blue) than the candidate's mid-blue.
- Second swing in the reference: a further arc at about s_21..s_22 (265.0..265.1 s), at the next swing.
- Duration in the reference: about 6 frames at 8 fps, about 0.6 to 0.8 s (video timing), roughly 5 to 6 times longer than the candidate's 0.13 s.
- Timing sync to the swing hit is not exact, because the video and game clocks are not aligned; the video shows the arc starting at the same moment as the hit splash, which matches the candidate's start at frame 76.

Judgement: the candidate's first-swing trail roughly matches the reference in position and sweep direction, but it is too short (about 1/5 of the reference duration), too saturated/dark in colour, and possibly narrower at its ends. The second swing has no trail at all (missing combo_02).

## (4) BashDown (skill key 2) and ground impact (B005)

- Log: `Source skill key=2 slot=0 frame=90 generation=1 phase=1 skill=BashDown MP=21.25`, `Source skill lifecycle frame=90 ... skill=BashDown hits=0`, and `Source player step FX frame=90 ... sequence=347 step=0 occurrence=8 dispatched=1 sets=164` (no resource error). So the BashDown effect is dispatched; `skill_dh2_prince_warrior_bash_down.bdae` resolves.
- Visual: sheet `verify-swoosh/img/bashdown-90-112.png` (frames 90..112). The Knight performs a sword-arc animation with the blade visible (frames 91..95, 99..105); a `Miss` label shows at about frame 95. No `Damage frame=` lines were checked for this run. No pale-blue or bright ground ring is visible in any frame: blue count 0 in all frames 90..112 (same threshold as above), and the frames show no ground impact effect.
- Not verified: the reference's BashDown/ground-impact frames were not compared (B005 notes the v1.0.3 streak is unidentified), so it is not known whether a ground impact should be visible at this point.

## Verdicts

- **B041 (swing trail): REJECT.**
  - Pass: the first basic swing now dispatches (`dispatched=1`, no resource error) and draws a blue trail at frames 76..84 (peak 80..82), positioned like the reference.
  - Fail: the second basic swing still dispatches `dispatched=0` (missing `swoosh_prince_1hand_combo_02.bdae`) and draws no trail.
  - Fail: the trail is about 0.13 s versus about 0.6 to 0.8 s in the reference, and its colour is darker than the reference's pale white-blue.
- **B005 (ground impact): INCONCLUSIVE.**
  - Pass: `BashDown` is selected (`Source skill key=2 ... skill=BashDown`) and its authored FX is dispatched (`sequence=347 dispatched=1`, no resource error).
  - Not verified: no ground impact is visible in captured frames 90..112, and the reference was not compared at the skill time, so it is unknown whether the visual is missing or never expected in this capture.

## Open items for the root

1. Stage `data/3D/interface/swoosh_prince_1hand_combo_02.bdae` (and `combo_03` if used) in the package, then re-run the second-swing series (frames 104..118).
2. Compare trail duration: the candidate's 8-frame life vs the reference's ~0.6 s. The authored track is 333 ms (B041 report), so an 8-frame life (about 130 ms at 16 ms per frame) suggests the source timeline is not advancing for the full life. Check this against the B041 report's "current_ms=333 end_ms=333" finding.
3. For B005, locate the reference BashDown moment in the video (not checked in this pass; the 262..266 s window shows only the basic swings) and compare.
