# LEVELUP3 report (level-up column: colour and timing evidence, billboard basis test)

Branch `p16/levelup3` (worktree `DH_wt/p16levelup3`, started from `p16/levelup` e4fcc98d). Build `DH_wt/build-p16levelup3`.
Scratch, frames, timelines and tools: `.local-inputs/claude-preview16/levelup3/` (ref/, tools/, ref-timeline.txt,
ours-timeline.txt, NOTES.md). Captures: `.local-inputs/claude-preview16/levelup2/jobs/dense3-f140..f176`.

Status: the vertical billboard basis is now unit-tested. The COLOUR and TIMING mismatch is NOT resolved. The evidence
below narrows it down. No visual match is claimed.

## 1. Investigation (evidence)

### 1a. Reference timeline (dense 30 fps, `ref-timeline.txt`)
- Reference file: `.local-inputs/reference-video/dh2-act1/Dungeon Hunter 2 (v1.0.3) Part 1 [720p] [z_Zky7qQdYs].mp4`,
  640x360 30 fps. Frame r_NNN = 768.0 + (NNN-1)/30 s.
- r034 (769.10 s): first small white-blue flash at body height. r035 (769.13 s): bloom begins at the feet.
- r036-r046 (769.17-769.50 s): saturated white bloom around the feet with thin white streaks rising above the hero.
  Top-5% colour of the bloom box = (246-248, 247-249, 248-250): white, no gold tint.
- r047 (769.53 s): hard cut. Only the green halo remains. No LEVEL UP text in these frames.
- Measured white phase: about 0.33-0.37 s, starting 0.07-0.10 s after the flash and ending about 0.43 s after it.

### 1b. Our timeline (dense quiet run, rc3, billboard EXE, no skill key, `ours-timeline.txt`)
- Level-up presentation at frame 143 (`fx=135:level_up:played`). The run has no warrior-bash FX, so the level-up is
  isolated in this fixture.
- Gold ramp from f151 (8 frames after the presentation). gold_frac peaks f153-f158 (about 0.05 of the box), then stays
  about 0.04 through f176 (end of capture). Box top-5% colour (244-249, 236-242, 114-129): gold.
- white_frac is 0.001-0.009 for the whole window. There is no white layer in our render.
- Assumed clock: 60 fps game frames, so f151 = 133 ms after the presentation. This matches the sheet colour track
  (peak at 133-167 ms). The 60 fps value is an inference from that match, not a measured frame rate.

### 1c. Authored data (`tools/bres_dump.py`, `tools/bres_effects.py`, `tools/bres_anim_keys.py`)
- Sheet material `_7_-_Default1`: `__irrlicht_Diffuse_color` track (track 86, RGBA bytes). Keys (30 fps frames):
  f3 (85,50,12), f4 (172,122,24), f5 (172,122,24), f18 (172,122,24), f20 (137,93,19), f22 (102,64,14),
  f24 (68,36,10), f26 (33,7,5), f28+ black. Sheet diffuse default (255,235,0). The authored colour is gold, not white.
- Glow material `gloow`: diffuse track peaks at (222,181,32), fades to 0 at key 33.
- Emitters: glow maxParticles 5, birth 30/s, stop key 20; stars maxParticles 100, birth 90/s, stop key 30;
  streaks maxParticles 5, birth 30/s, stop key 15 (0.5 s).
- Five streak particles alive at once cannot produce the broad white bloom of the reference. The bloom must come from
  the large sheet billboard or from a post pass.
- IDA search (`pseudocode-all.c`): glitch::ps has only billboard renderers (PRenderDataBillboardModel,
  PSBillboard*Baker). No stretched/streak renderer. The only glow/blur code is gameswf::filter_engine (`apply_glow`,
  `apply_blur_h/v`), which is the Flash UI filter. No 3D bloom or post-process pass was found by name.

### 1d. The three questions
1. COLOUR. The authored sheet colour is gold, and the port renders it as gold. The reference is white with no gold
   tint, saturated in all channels. Our render has no white layer. Candidates, none verified: (a) a white additive
   layer (stars at 100 max, default white particle colour) that our port does not show; (b) a bloom/exposure stage
   that whitens bright additive regions, which no IDA name confirms; (c) the sheet material's colour combine
   differing from ours. Not decided. The port's particle output for this FX is very sparse (earlier LEVELUPFX note:
   5 particles per run), which is consistent with (a) being absent.
2. TIMING. Our sheet ramp (f151, peak f153-f158) matches the authored colour track at a 60 fps FX clock. Our sheet
   stays bright past 0.6 s and fades to 0 at 0.93 s. The reference white ends about 0.43 s after the flash with a hard
   cut, which the authored sheet envelope does not explain. The streak birth stop (0.5 s) and the glow stop (0.67 s)
   are close to it but not equal. The cut mechanism is NOT decoded. The clock-rate question (speed factor or node
   range) is NOT resolved, because the reference game-frame rate is not known.
3. ISOLATION. The no-skill quiet run contains only the level-up FX. Its sheet is gold and no white appears. So the
   warrior-bash shockwave is not the source of the reference white in our capture, and the level-up alone does not
   reproduce the reference white in our port.

## 2. Implementation
- `port/level-world/source_fx_billboard_basis_v1.{hpp,cpp}`: the CBillboardSceneNode camera basis (decoded in the
  LEVELUP2 report) moved out of `source_fx_node_matrix_v4.cpp` into its own unit, with matrix helpers
  (`source_fx_mat16_multiply_v1`, `source_fx_mat16_inverse_affine_v1`). Behaviour is unchanged (refactor only).
- `source_fx_node_matrix_v4.cpp` calls the new unit (CRLF preserved).
- CMake: `port/level-world/CMakeLists.txt` and `port/windows-foundation/CMakeLists.txt` (source list and new test).
- Commit `2ecf91d2`.

## 3. Tests
- New `port/windows-foundation/tests/source_fx_billboard_basis_v1_tests.cpp` (ctest `source_fx_billboard_basis_v1`):
  - identity parent, tilted camera (eye (0,-800,800)): local X -> screen right, local Y (sheet length 1140) -> camera
    up U = (0,0.707,0.707) (vertical on screen), normal -> eye direction, unit columns, sheet top at 1140 along U.
  - parent rotated 90 degrees about Z: U is unchanged (parent rotation ignored); translated parent sets the origin.
  - mode 2 and sub 2 are rejected (plain-node fallback); eye at the origin is rejected; inverse round trip.
  - Standalone compile and run: "all checks passed".
- Colour/timing rule: NOT established, so no colour/timing unit test was added (section 1d).
- ctest: see section 6.

## 4. Runtime verification (quiet runs)
- Captures: dense 140-176 (`levelup2/jobs/dense3-f140..f176`, rc3, billboard EXE from e4fcc98d, no skill key).
- An rc4 run failed for every job with `Asset not found: original-cache/data/pydata/v2quests_pyarray.bin` (exit 1).
  rc3 works. Use rc3 for the verifier.
- Reference comparison: `ref-timeline.txt` vs `ours-timeline.txt` (section 1).

## 5. Gaps (not matched)
1. Colour: the reference column is white; our sheet is gold; no white layer in our render. Cause not decided (streak or
   star particles, or bloom; neither verified).
2. Duration/cut: the reference white ends with a hard cut about 0.43 s after the flash; our sheet is still bright at
   0.6 s. The cut mechanism is not decoded.
3. Game-frame rate for the FX clock is inferred (60 fps), not measured.
4. Particle output: the emitters have maxParticles 5/100/5. The port's particle output for this FX is very sparse
   (earlier measurement). Why is not decided.
5. Warrior-bash shockwave: not compared with a reference bash frame in this pass.

## Placeholders
- No placeholder coded. The reference column (white, from the feet, visible about 0.07-0.43 s after the flash) is not
  invented, because the mechanism (particles or bloom) is not decoded. The target is recorded in section 1a and
  NOTES.md for the next pass. A fixed-time white override would be invented art and was not added.

## Package files required
- None new. The verifier needs the rc3 package (`windows-source-clock-v19-preview-15-rc3`) for the original-cache quest
  asset. rc4 fails the quiet run with the pyarray asset error.

## Verifier script
1. Build: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p16levelup3 -Jobs 6`.
2. Jobs: `SRC=<levelup2>/jobs/nsclean-f150/run.args PKG=<rc3> bash <levelup2>/tools/mkjobs_ns.sh <EXE> dense3 $(seq 140 176)`.
3. Run: `port/windows-foundation/tools/quiet_run.ps1 -JobsFile <levelup2>/jobs/dense3.json -Parallel 10 -Summary ...`.
4. Timeline: `py -3 <levelup3>/tools/ours_timeline.py <levelup2>/jobs/dense3-f1*/cap-*.ppm`.
   Reference: `ffmpeg -i <ref mp4> -ss 768.0 -t 2.5 -vf fps=30 -f rawvideo -pix_fmt rgb24 ref/raw640.rgb`, then
   `ref_timeline.py ref/raw640.rgb 280 40 420 330`.
5. Expect: `Level up presentation ... frame=143`, gold ramp from f151, no white layer.
