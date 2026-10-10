# Verify report: combat (B037, B004/B029) - verifier `combat`

Candidate EXE: `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/windows-source-clock-v19-preview-12-candidate/dh-foundation.exe`
SHA256 `4e872fe0bc27112b45aa84a7f5fe3196235942a534820c808e8dcd3f9913dabe` (matches brief).
Baseline: `.local-inputs/windows-source-clock-v19-preview-11/dh-foundation.exe` (same `swamp.args` content, verified by diff).
Working folder (all outputs, logs, saves, PPM/PNG): `.local-inputs/claude-preview12/verify-combat/{cand,p11,video}/`.
No file in the candidate or Preview 11 folders was written; saves are copies in the working folder. The user's live game was not touched; every process was started and exited by me.

## Method

Runner: `.local-inputs/claude-preview12/verify-combat/run.sh <cand|p11> <case> <frames> [startup args...]`.
It copies `swamp.args` to a per-case args file, makes asset/save paths absolute, and appends `--fixed-step .016 --frames N --capture <case>.ppm`. Runs are deterministic (`--combat-seed 1234`), so a capture at `--frames N` is the state at frame N. Captures only happen at the last frame, so each timepoint is its own run.

Scenario: Knight (`--position -6752.641,938.285,250`, `--combat-source-combo`, `--combat-ai-gate Swamp_LizadMan_Type1=Limbus`) next to a lizard-type Monster (id 7118915781085668844) about 100 units away. The position and gate come from the existing combo checkpoint args. Skill key 2 = BashDown (slot 0). Key 1 maps to an empty hotbar slot in both builds (diagnostic "Selected source hotbar slot has no valid saved skill row"), so key 2 is used.

Log line references below are from `cand/*.log` and `p11/*.log`. "Boundary" means `Source combo boundary` lines, which are combo-step starts.

## (1) B037: skill admission during Space-held swing, release

| Check | Command (args added to run.sh) | Candidate | Preview 11 | Result |
|---|---|---|---|---|
| Skill mid-swing, Space held | `--space-key-interval 30:400 --skill-key-frame 50:2`, 260 frames | `Source skill key=2 slot=0 frame=50 generation=1 phase=1 skill=BashDown diagnostic=` (empty). Group-1 strike began 43, hit at 46, then **no group-2 boundary at 61**; `Source skill Use geometry ... do_skill` at 84; `Damage ... marker=do_skill` at 84. | `Source skill key=2 ... generation=0 phase=0 skill= diagnostic=Current source lifecycle action rejects a new skill cast`. Swing continues: group 2 boundaries at 61, 74, 93. | PASS |
| No second basic attack while skill runs | same run | No boundary between 61 and 132. Next attack boundary at 132 (generation 2), with Space still held (Semantic Space `held=1` through the run). | n/a | PASS (no restart during skill; restart after skill) |
| Held Space resumes after skill Post | same run | Attack resumes at 132 while Space is held (B003 behaviour). Exact Post frame not logged; resume is the observable. | n/a | PASS (observed resume); exact Post frame NOT-RUN |
| Release on strike, before hit | `--space-key-interval 30:14`, 130 frames | Release at 44 (Space held 30-43). Strike 43-61 completes; hit at 46 with `continued=0`; idle at 61 (no group 2). Final Player `action=0`. | Identical (same boundaries, action=0). | PASS, identical to P11 |
| Release on strike, after hit (continuation latched) | `--space-key-interval 30:21`, 130 frames | Release at 51. Group 2 starts at 61 (hit at 46 had `continued=1`). Group 2 hits at 79. Final `action=0`. | Identical. | PASS (source-accepted continuation), identical to P11 |
| Release on group-2 pre-step | `--space-key-interval 30:35`, 150 frames | Release at 65 (pre step 61-74). Group 2 strike 74-79 completes. Final `action=0`. | Identical. | PASS, identical to P11 |

Notes:
- Release never cut a swing in the tested cases. The combo-fix report says the recovery-window cut (`finisher`, CharAI+122) only fires when the target is dead at the final step. That cannot be produced with a living target in this fixture, so the recovery cut is NOT-RUN here.
- Hurt admission is still rejected in the candidate (B037 report, left as flagged). Not exercised.
- "Returns to Idle": final player `action=0` is `CharacterAction::idle` (`port/windows-foundation/character_state.hpp:97`) after the swing ends.
- The combo CMake test `combat_session_combo` was not run by me (the combo-fix report says it passed in the integrated build).

## (2) B004/B029: target marker and HUD after skill Post and Space release

Image paths are under `verify-combat/`.

| Frame (run) | Situation | Candidate | Preview 11 |
|---|---|---|---|
| `cand/b4-f99.png`, `p11/b4-f99.png` (`--space-key-interval 30:14 --skill-key-frame 100:2`) | Before skill; Space released at 44; swing finished; Idle | Red ring under Bogwomp, "BOGWOMP" name, red HP bar with level badge. | No ring, no name/HP frame. |
| `cand/b4-f130.png`, `p11/b4-f130.png` | During BashDown (do_skill at 134) | Ring + name/HP frame still shown. | No ring, no HUD. |
| `cand/b4-f200.png`, `p11/b4-f200.png` | After skill; no Space held | Ring + name/HP frame still shown on the in-range enemy. | No ring, no HUD. |
| `cand/tab-f99.png`, `p11/tab-f99.png` (`--target-frame 60`, then as above) | Tab explicit selection (far lizard 4141..., ~1190 units away); after Space release, before skill | Ring + HUD on Tab-selected lizard. | Ring + HUD on Tab-selected lizard. Tab works in both. |
| `cand/tab-f200.png`, `p11/tab-f200.png` | After BashDown on Tab-selected target | Post cleared the Tab target; marker fell back to the nearest eligible enemy in range (the lizard beside the player); ring + HUD shown. | No ring, no HUD. |
| `cand/death-f142.png` (`--space-key-interval 30:400`) | Enemy alive, swing in progress | Ring + HUD on Bogwomp. | (not captured) |
| `cand/death-f146.png` | Monster killed at 143 (`Damage ... dead=1`) | Ring and HUD gone within 3 frames. | `p11/death-f146.png`: none. |
| `cand/death-f175.png` | ~32 frames (~0.5 s) after death | Marker moved to the next living lizard in range; dead one not marked. | not captured |
| `cand/range-near.png` (player 150 units from enemy, no Space, no skill) | In range | Ring + name/HP frame. | not applicable (no target). |
| `cand/range-far.png` (250 units) | Out of range | No ring, no HUD. | not applicable. |

Verdicts for (2):
- Post and release persistence: PASS in candidate (f99, f130, f200). Preview 11 loses the marker in the same runs. The exact Post frame was not logged. The f130 capture is mid-skill, and f200 is about 66 frames after do_skill.
- Tab explicit selection: PASS (tab-f99, `Source target command frame=60 selected=4141...`).
- Death hides within ~0.5 s: PASS (hidden at +3 frames; moved to next eligible at +32 frames).
- Range: 150 shown, 250 hidden: PASS. The exact 200-unit boundary was NOT tested (no run placed the enemy at 195-205).
- Live Space-held sequence with marker: f142/f146/f175 captured with Space held.

Observation not attributed to B004: in the candidate the top-left player portrait is blank in every frame I looked at, including `cand/smoke-hud.png` from a plain no-input 120-frame run (no target, no combat). Preview 11 shows the Knight portrait (`p11/smoke-hud.png`). The reference video shows the portrait (sheets s012/s013). The target-HUD code change does not touch the player portrait path (`compose_original_hud`, main.cpp ~2994). I did not find the cause; this needs a root check of the candidate's HUD/asset set.

## (3) Reference video

Source: `.local-inputs/reference-video/dh2-act1/Dungeon Hunter 2 (v1.0.3) Part 1 [720p] [z_Zky7qQdYs].mp4`. Contact sheets used to locate: `dh2_video_research/sheets1/s012.jpg` (4:24-4:44) and `s013.jpg` (4:48-5:08). Frames extracted with ffmpeg (`fps=2` tile, 307-312 s): `verify-combat/video/tileD-307.png`.

Observed:
- 307.0-307.5 s: no marker shown.
- 308.0 s: Bog Moth ring + name/HP frame appear; player swinging with a blue arc.
- 308.5-310.5 s: ring + name/HP frame persist through consecutive swings and "MISS" feedback. Damage/EXP numbers float.
- 311.0 s: ring + frame still shown. 311.5-312.0 s: "EXP" text, frame and ring gone (enemy dead).

So in the reference, the marker persisted across attack swings while the target was alive and cleared on the kill. This is consistent with the candidate's behaviour.

Not established from video: a skill press followed by a Post, or a release with an enemy nearby. The blue arc at 308 s may be a skill, but I could not confirm the skill key or the Post frame from the frames. Video is 640x360 native; the markers are readable at 2 fps.

Also visible in the video: the player portrait is present top-left throughout, which matches Preview 11 and not the candidate (see above).

## Verdicts

- **B037: APPROVE** (for the tested scenarios). Mid-swing skill is accepted (empty diagnostic) and cuts the swing once, with no second basic attack until the skill finishes. Held Space resumes attacking after the skill (B003 path). Preview 11 rejects the same press with the expected diagnostic. Release before/after a hit leaves the swing to the source-allowed step and ends in Idle, identical to Preview 11. Residual, not verified: recovery-window release cut (needs a dead target at the final step); the exact Post frame; hurt admission (still rejected). The CMake `combat_session_combo` test was not run by me.

- **B004/B029: APPROVE** (for the tested scenarios). Candidate shows the marker and name/HP frame after the skill and after Space release; Preview 11 does not. Tab works; the HUD hides on death within ~3 frames and moves to the next in-range enemy by ~0.5 s; range check 150 shown / 250 hidden. Residual: exact 200-unit edge; long-run persistence across many skills not tested. Video supports the marker persisting while the target lives (not the post-skill case).

- **Separate observation (not a verdict for B004 or B037): INCONCLUSIVE.** Candidate player portrait is blank in the HUD in a no-combat run, while Preview 11 and the reference video show it. The cause is not in the combat change I checked. Root should check the candidate's HUD/asset set before the preview.

## Not run / limits
- `combat_session_combo` CMake target: not run by me.
- Exact Post frame for BashDown: not logged; resume and marker checks used frames 130/200.
- Range boundary at exactly 200 units: not tested.
- Video: only the 307-312 s window was examined in detail; skill-followed-by-release case in the reference not found.
