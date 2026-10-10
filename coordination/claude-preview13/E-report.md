# Preview 13 Group E report: B041 (sword swing trail) and B005 (BashDown ground impact)

Status: **B041 NOT closed, no production change. B005 NOT closed (reference shows a candidate dust burst, attribution unverified).**
Repo HEAD f6b7f134. No tracked file was edited. All scratch frames and tools are under
`.local-inputs/claude-preview13/E/` (git-ignored).

## 1. Evidence

### Visual, reference (v1.0.3, 30 fps frames extracted with ffmpeg; frame n at t0 + (n-1)/fps)

- **Basic swing trail (Knight, Longsword).** I looked at 30 fps frames s_013..s_039 (`E/swing_sheet.png`, t = 263.0 + (n-1)/30).
  A pale white-blue translucent disc/arc around the Knight is first visible at s_019 (263.57 s), grows (largest about s_031..s_035, 263.97..264.10 s), and is still faintly visible at s_037 (264.17 s). Visible span about 0.6 s. This agrees with the verifier's 8 fps reading (0.6..0.8 s). It is wider than the blade and paler than our render. Observation is at sheet resolution, so the exact edge is not measured.
- **BashDown window (6 fps, t = 281 + (n-1)/6, `E/bd_sheet.png`, `E/bd_zoom.png`).** The target is the Bogwomp (lure ring on its feet). Pale blue sweeping arcs around the Knight at about 283.8 s and 284.8 s; a yellow-green flash on the Bogwomp at 283.0..283.5 s with a "Miss" label; a red-tinted floor patch under the Knight from about 282.8 s onward. No blue shockwave ring is visible in any frame I looked at.
- **BashDown ground impact (30 fps, t = 282.6 + (n-1)/30, `E/bd30_sheet.png`, `E/z_sheet2.png`).** c_01 (282.60 s) to c_03 (282.67 s): a bright orange/yellow dust cloud at the Knight's front feet, fanning out and fading to a red glow by c_03. c_04 (282.70 s) onward: only the red floor patch remains, plus the target flash. The dust burst lasts about 3 frames (about 0.1 s). It is a dust/impact puff, not a blue ring.
  - **Attribution to BashDown is inference.** The 281..284 s window is the BashDown area named in the previous worker's note (`warrior-bashdown-fx-evidence.md`: 4:41.995..4:43.995, "no clear ground shockwave"). The footage does not show the pressed key. I did not confirm the Knight's skill key at that moment.

### Visual, ours (candidate EXE, verifier captures)

- Verifier captures `.local-inputs/claude-preview12/verify-swoosh/runs/bd90..bd112/frame.ppm` (one frame per run, final frame only). I viewed them as a 12-frame sheet (`E/ours_sheet.png`, frames 90..112 even): the Knight sword-swings; a "Miss" label appears at about 96; no orange dust burst at the feet and no ground ring in any of the 12 frames.
- Limitation: the verifier's "no ground impact" used a **blue-only** threshold. An orange dust burst would not register there. My sheet check is visual and covers only the even frames 90..112.
- Basic swing (verifier, blue threshold): trail visible at frames 76..84 (about 8 frames, 0.13 s) in the candidate, darker saturated blue than the reference.

### Logic (authored data, verified by reading the cached BDAE files)

- `CharAnimator::_PlayItemSwooshFX` (IDA `pseudocode-all.c` line 145051) calls `VisualFXManager::PlayAnimFXSet(item+24 set, actor or target, ...)` once per weapon per animation step. The caller (line ~145704) plays it only when step flag `v7+52` is set. There is **one set per step per weapon, not several sets**, so "several FX instances per swing" is not supported by IDA.
- Authored clip ends (BRES library 0x34/0x38 walk, `.local-inputs/claude-preview13/E/tools/clips.py`, all three files have one animation clip, `AUTO_Current_Range`):
  - `swoosh_prince_1hand_combo_01.bdae` (8332 B, sha af8ef8a2...6180b): clip 0..**333 ms**.
  - `swoosh_prince_1hand_combo_02.bdae` (8668 B, sha 896e1c52...3f84): clip 0..**366 ms**.
  - `swoosh_prince_1hand_combo_03.bdae` (7980 B, sha 7c372201...d3dc): clip 0..**2500 ms**.
  - None of the three has emitters or GNPS emitters in the BRES library counts (`tools/libs.py`), so the trail is a mesh track only.
- Lifetime in our code: `character_mesh_fx_owner_v4.cpp:78` uses clip 0's end as the set end.
- From the factory native test (B041 report 2, 16 ms steps), the bright UV band is sampled from timeline about 32..192 ms (source 288..448 ms), peaking near 352 ms source. So the authored combo_01 texture is visible for about 160 ms of its 333 ms clip, then fades. The timeline runs about 1.3x (measured; attack-speed source is inferred, not traced). 160 / 1.3 is about 123 ms of app time, about 8 frames at 16 ms. **This matches the verifier's 8-frame result.** So our short trail is what the authored combo_01 clip produces under our timeline, not a lost clip.

## 2. Diagnosis of "shorter and darker"

- Duration: the authored combo_01 texture is visible about 160 ms. Our render reproduces that. The reference shows about 0.6 s. Reference swing identity is unknown, so two hypotheses remain. Neither is verified:
  - (a) The reference swing uses a longer set (combo_03 clip is 2.5 s, so its visible band may be longer; not measured).
  - (b) The original does not scale the swoosh timeline by 1.3, which would give about 10 frames at 1.0x. This does not reach 0.6 s, so it cannot be the whole gap.
- Colour: material colour 0.584 grey, additive (1,1), texture max rgb (132,214,255). Pale white-blue in the reference is brighter. I did not find a source-backed change (vertex colour, alpha blend, or tint) in the logic I read. No change is made.
- v1.0.3 video vs v1.0.2 recovered data: the video is v1.0.3, our authored data is v1.0.2 (per brief). Any difference in swoosh content between versions is **uncertainty**; I have no evidence either way.

## 3. Fix decision

**No production change.** The evidence does not identify a code defect within `features/effects/*`:
- The short trail matches the authored combo_01 clip under our timeline.
- The 0.6 s reference requires a different set (combo_03?) or a different version's data. Changing lifetime or colour without that would be inventing behaviour.
- A regression test asserting 0.6 s would fail against the authored combo_01 data and would be wrong.

Recommended next step (not done here, needs a probe that forces dispatch of combo_02/combo_03 through the real Session): measure the visible bright window of combo_03 and combo_02 in the factory native harness, and check which combo the reference swing uses (for example by matching the arc's radius and timing to a Knight combo step).

## 4. Package files required

| file (under `assets/data/3d/interface/`) | source | SHA256 | in candidate `.local-inputs/windows-source-clock-v19-preview-12-candidate/` |
|---|---|---|---|
| swoosh_prince_1hand_combo_01.bdae | `character-fx-owner-v1/cache/` | af8ef8a2dc4d6da7c369964aa7d1fed08859ea4ba1d6062ac6796f11a936180b | present, hash matches |
| swoosh_prince_1hand_combo_02.bdae | `character-fx-owner-v1/cache/` | 896e1c5247fd596086b855f2ba1ba278f7a7f5899af7bc947b43207255083f84 | present, hash matches (timestamp 13:44) |
| swoosh_prince_1hand_combo_03.bdae | `character-fx-owner-v1/cache/` | 7c3722015b676caf02a2b8fa9c1b546694cbe0e0e46f36697a72b989ea0d3dcd | present, hash matches |

**Correction to the verifier report:** its finding that combo_02 is missing applies to the verifier's own copy `claude-preview12/verify-swoosh/cand-pkg/` (which lacks it). The candidate folder contains combo_02 with the cache hash. The root should confirm the candidate was not changed after the verifier copied it.

BashDown asset: `data/3D/interface/skill_dh2_prince_warrior_bash_down.bdae` (sha 21a31d37...e483 per the B005 note). Verifier log shows it resolves (`sequence=347 dispatched=1`). Presence in the candidate package was not re-checked by me.

## 5. Changes

None. No file under `port/` or `coordination/` was edited by this pass, other than this report. Scratch folder `.local-inputs/claude-preview13/E/` contains frames (`swing/`, `bd/`, `bd30/`, `ours/`), the contact sheets, and tools (`sheet.py`, `clips.py`, `libs.py`).

## 6. Tests

- No code touched, so no runner was rerun. Earlier results (from the B041 worker): canonical `runtime_effects_factory_native_tests.py` gives `"validation": "PASS"` x3, exit 0.
- Tool checks run: BRES clip dump (`clips.py`) for the three swoosh files, BRES library count dump (`libs.py`) for the same files.

## 7. Uncertainties / not verified

- Whether the reference's 0.6 s swing uses combo_03 (2.5 s) or a different set. Not tested.
- The 1.3x timeline scale source (attack speed vs step Speed field). Not traced.
- Reference colour (pale white-blue) vs our saturated blue: observed visually only; no colour measurement.
- Reference BashDown attribution: the dust burst at 282.6 s is not tied to the skill key.
- BashDown in the candidate: only even frames 90..112 were viewed, and the verifier checked blue only. An orange dust burst cannot be ruled out from the existing captures.
- Live EXE not run by me.

## 8. Verifier script

Not applicable to a code change, because none was made. For B005, a verifier should:
1. Start the candidate EXE in an isolated folder with `SKILL="90:2"` (`--skill-key-frame 90:2`) and `--frames` 100..112, capturing each frame with `--capture` (one capture per run; the verifier's `scripts/run.sh` pattern works).
2. Check for an orange/yellow dust burst at the Knight's feet in the frames near the skill's impact time. Use a colour test that is not blue-only.
3. Compare with reference frames 282.60..282.70 s (30 fps) from `Dungeon Hunter 2 (v1.0.3) Part 1 [720p] [z_Zky7qQdYs].mp4`.
4. Expected: if the burst is BashDown's ground impact, it should appear in our render at a similar time after key press (not measured here). If it does not appear, B005 stays open. If it does appear and the reference burst is confirmed as BashDown, B005 can close.

Summary: no production change; B041 remains open because the 0.6 s reference swing is not explained by authored combo_01 (333 ms, visible 160 ms). B005: reference shows a brief orange dust burst at the Knight's feet at 282.6..282.7 s; ours shows none in the even frames 90..112 (orange not measured by the verifier). Attribution to BashDown is inference.
