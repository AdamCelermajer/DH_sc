# LEVELUP4 report (level-up white column: hypothesis checks, LEVELUP_PLACEHOLDER_COLUMN)

Branch `p16/levelup4` (worktree `DH_wt/p16levelup4`, started from `p16/levelup3` 89537b94). Build `DH_wt/build-p16levelup4`.
Scratch, captures, timelines, contact sheets and tools: `.local-inputs/claude-preview16/levelup4/`
(`sheet/sheet3.png` contact sheet, `ref-769-timeline.txt`, `ours-baseline-timeline.txt`, `ours-placeholder-timeline.txt`,
`tools/white_bbox.py`, `exe/baseline`, `exe/placeholder3`). Captures: `.local-inputs/claude-preview16/levelup2/jobs/l4base-f140..f176`
(baseline) and `l4ph3-f140..f176` (placeholder, final).

Status: the original white column is still NOT decoded. A clearly labelled placeholder (`LEVELUP_PLACEHOLDER_COLUMN`)
now produces a white column in the level-up FX: white from +100 ms to +430 ms after the FX play, gold sheet suppressed,
width x2.5. It matches the reference in colour, timing and rough position. It does NOT match the soft bloom falloff or
the body-height flash (section 5).

## 1. Investigation (evidence, bounded hypothesis checks)

Visual evidence (direct observation, reference `.local-inputs/reference-video/dh2-act1/Dungeon Hunter 2 (v1.0.3) Part 1 [720p] [z_Zky7qQdYs].mp4`,
640x360 30 fps, extracted at 769.0 s, `sheet/ref769.rgb`, `ref-769-timeline.txt`):
- 769.03-769.10 s: small white-blue flash at body height (flash start is at 769.03 in LEVELUP2, 769.10 in LEVELUP3 frame r034).
- 769.167 s (i05): white column appears, white_frac in the 200x200 box 0.21, top-5% (246,246,248).
- 769.167-769.500 s (i05-i15): white column/bloom from above the hero to the feet, box white_frac 0.25-0.35.
  Column span on the mid-height row (y~170 at 640 scale) about x 251-405 (~150 px), ~90 white px per row.
- 769.533 s (i16): hard cut, back to the green halo (top-5% (196,238,213)). No gold anywhere in the white phase.

Hypothesis (a): sheet additive colour combine saturates to white at high intensity. REJECTED on the authored data.
- Authored sheet `_7_-_Default1` diffuse track (`bres_anim_keys.py`, level_up.bdae): peak (172,122,24)/255 at keys 4-18,
  fades to 0 by key 28. Blue peaks at 24/255 = 0.094.
- Additive ONE/ONE of colour x texture (texture <= 1) cannot saturate the blue channel. It would need a gain of about
  10.6x on the authored colour. The asset has no intensity track: the only tracks are colour (sheet, halo), birth rates
  and one translation. EffectsTables set 135 play params are speed 1.0, loop 0, play_time -1.
- Our render agrees with the additive model (no clamp to white): baseline top-5% (244,236,114), the authored gold.
- Result: the gold colour is the authored colour. No authored path makes it white.

Hypothesis (b): the halo texture and star particles at the right scale.
- Halo: `_mesh_glow_floor_nobatch` is a flat 520x520 quad at z +27.3 with material `gloow` (diffuse peak (222,181,32), B 0.125).
  Gold, not white, so it cannot be the white column.
- Stars: emitter `stars` (maxParticles 100, birth 90/s to key 30). Its translation track rises from z ~0 to z ~395 units
  over 1 s (X drifts to +157, Y swings about +-140). Particle template default colour is 0xffffffff (white), scale 1.0,
  material `fx_particles_additive_000`, no colour track. Streaks: maxParticles 5, birth 30/s to 0.5 s,
  `fx_particles_additive_streak`, white by the same default. So the ONLY white source in the authored effect is the
  star/streak particle layer. The rising stars match the reference thin white streaks rising above the hero.
- Measured in our baseline: white_frac 0.001-0.010 for the whole window, no white layer (ours_timeline).
  Whether the port GENERATES and DRAWS these particles for set 135 is NOT verified: the trace needed a per-frame particle
  count, which was not run (time box). This is the first thing to check in the next pass.

Conclusion of the one-hour check: the white is most plausibly the star/streak particle layer (white, rising, 0.5-1.0 s),
not the sheet. Neither the sheet nor the halo can produce it, and the port shows no white particle layer. The original
mechanism is not decoded, so the placeholder below was implemented (rule: simple labelled placeholder).

Billboard/geometry (carried from LEVELUP2/3, unchanged): the sheet is a glitch CBillboardSceneNode (camera-facing,
200 x 1140 units from the feet upward). The placeholder keeps that geometry.

## 2. Implementation (LEVELUP_PLACEHOLDER_COLUMN)
Commit `d71e910c` on `p16/levelup4`.
- `port/level-world/level_up_placeholder_column_v1.hpp` (NEW): data record `kLevelUpPlaceholderColumnV1`
  (fx_uri `data/3d/interface/level_up.bdae`, sheet material `_7_-_Default1`, white window 100..430 ms after the FX play
  start, white (1,1,1,1), width_scale 2.5), plus `level_up_placeholder_keep_part_v1` (per draw part: keep, recolour to a
  white snapshot material table, or drop) and `apply_level_up_placeholder_v1` (vector form).
  The snapshot keeps the previous retention alive and copies the geometry before the x scale (no shared mutation).
- `port/level-world/character_mesh_fx_owner_v4.cpp` (2 one-line hunks, CRLF kept): the placeholder runs in
  `CharacterMeshFxOwnerV4::draw_parts` (particle composite branch) and in `mesh_draw_sources_v4` (the mesh source
  consumed by the Windows render bridge `effects_render_bridge.cpp`). It matches the FX record by its uri and uses
  `timeline.current_ms - timeline.start_ms`.
- `port/windows-foundation/features/effects/runtime_level_up_presentation_v1.hpp`: `kLevelUpSet135PlaceholderV1` next to
  the set 135 binding (`kLevelUpFxSetV1`), referencing the record above. No renderer or main.cpp change.
- Suppression: while the placeholder is active the gold sheet part is not drawn outside the white window
  (before 100 ms and from 430 ms). Other FX parts are unchanged.

## 3. Tests
- Build `p14_build.ps1 -Name p16levelup4 -Jobs 6` green (warnings fixed).
- New host checks in `runtime_level_up_presentation_v1_tests.cpp` (ctest `runtime_level_up_presentation_v1`, passed):
  window length 330 ms and bounds 100/430; sheet dropped at 0 ms and at 430 ms; sheet kept at 200 ms with a white snapshot
  table; authored table not modified; other part untouched; other FX uri is a no-op.
- ctest (full, toolchain on PATH): 130 passed, 2 failed out of 132.
  - `session_skill_binding`: failed, exempt per task.
  - `character_menu`: failed ("Tab overlap left part not answered by the nearer Quest Log"). PRE-EXISTING, not caused here:
    its test binary links only `foundation_data` and its own source; none of the changed files are in its link.

## 4. Runtime verification (EXE, quiet runs, rc3, no skill key, frames 140-176 at 1 frame step)
- Baseline EXE (levelup3 code, `exe/baseline`) and placeholder EXE (`exe/placeholder3`) both log
  `Level up presentation frame=143 ... fx=135:level_up:played`.
- Timelines (`ours-baseline-timeline.txt` vs `ours-placeholder-timeline.txt`; ours_timeline.py, same metric):
  - Baseline: gold ramp from f151, gold_frac 0.04-0.05, white_frac 0.001-0.010 (no white at all).
  - Placeholder: white_frac jumps at f151 to 0.098-0.112 (top-5% (255,255,255)), white until f170 (inclusive); cut at f171
    (white_frac 0.001); gold_frac falls from 0.011 to 0.002-0.011 during the white phase (sheet suppressed).
- Timing mapping (inference, not measured): our 60 fps FX clock, f143 = flash. White starts f151 = +133 ms (ref +140 ms
  from 769.03), white cut f171 = +467 ms (ref +500 ms from 769.03). Matches within about one frame.
- Visual: `sheet/sheet3.png` (rows: reference 769.0-769.5 s, baseline f143-f172, placeholder f143-f172; crop 190x300 at 640
  scale). `sheet/full_ph3_160.png`: full placeholder frame at f160, white column from the feet up to the top of the frame.
- Quantitative width (640 scale, row y170): reference white span x 251-405 (~150 px, ~90 white px); baseline gold ~60 px span;
  placeholder (x2.5) x 268-368 (~100 px, 78-91 white px). So the x2.5 factor is too small for the span (the rough 60 x 2.5
  estimate gave 150 px); the white pixel count matches. Width is a gap (section 5, item 7).
- Note: after the cut (f171+) our gold_frac is still 0.014 (baseline 0.040). That gold is the halo/glow floor and emitters
  (`gloow`), which are NOT the sheet and are intentionally not suppressed. The reference shows only the green halo here.

## 5. Gaps (not matched)
1. Soft bloom falloff: the placeholder column has the sheet's hard edges. The reference bloom is soft and wide.
2. Body-height flash at 769.03-769.10 s (small white-blue flash at hero body height): NOT implemented. No geometry for it
   in the placeholder; a later pass needs a billboard or particle source at body height.
3. Star/streak particle layer: not verified as generated or drawn for set 135 (per-frame particle count not traced).
   Probable true source of the white (section 1b). Next pass: count `stars`/`streaks` particles alive per frame.
4. The original mechanism for the 0.43 s hard cut is not decoded. The placeholder uses a fixed window.
5. Clock: 60 fps FX-clock mapping is inferred (LEVELUP3), not measured from the game frame rate.
6. Remaining gold after the cut (gloow halo/glow floor) is not suppressed (see section 4).
7. Column width: placeholder span ~100 px at mid-height vs reference ~150 px (section 4). The x2.5 factor was an estimate;
   tune it (or replace the sheet) in the next pass.

## Placeholders
- `LEVELUP_PLACEHOLDER_COLUMN` (level-up FX, set 135 only). Code: `port/level-world/level_up_placeholder_column_v1.hpp`,
  binding `port/windows-foundation/features/effects/runtime_level_up_presentation_v1.hpp` (`kLevelUpSet135PlaceholderV1`).
  What it is: a white recolour of the authored sheet, with a 100-430 ms window, a x2.5 width, and the gold sheet dropped
  outside the window. It is NOT the original art or mechanism.
  Reference timestamps for the later pass (video v1.0.3, Part 1, 640x360 30 fps):
  - flash at body height: 769.03-769.10 s (not implemented, gap 2);
  - white column in the window: 769.13-769.50 s (white at 769.167 first full frame; cut at 769.533);
  - column span: about 150 px at 640 scale (mid-height row), from the top of the hero to the feet; placeholder ~100 px.
  Replace with the decoded star/streak particle (or bloom) behaviour and remove the record when that lands.

## Package files required
- None new. The verifier needs the rc3 package (`windows-source-clock-v19-preview-15-rc3`): rc4 lacks the v2quests table.

## Verifier script
1. Build: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p16levelup4 -Jobs 6 [-Test]`.
2. Jobs (Windows paths only, `C:/...` for the package): `SRC=.../levelup2/jobs/nsclean-f150/run.args PKG=C:/.../windows-source-clock-v19-preview-15-rc3 bash .../levelup2/tools/mkjobs_ns.sh <EXE> <tag> $(seq 140 176)`.
3. Run: `port/windows-foundation/tools/quiet_run.ps1 -JobsFile .../levelup2/jobs/<tag>.json -Parallel 10 -Summary <json>`.
4. Timeline: `py -3 levelup3/tools/ours_timeline.py levelup2/jobs/<tag>-f1*/cap-*.ppm`. Reference:
   `ffmpeg -ss 769.0 -i <ref mp4> -t 1 -vf fps=30,scale=640:360 -f rawvideo -pix_fmt rgb24 ref769.rgb`, then
   `py -3 levelup4/tools/white_bbox.py ref ref769.rgb`.
5. Expect: `fx=135:level_up:played` at frame=143; placeholder: white_frac > 0.04 for f151-f170, gold removed in that window;
   baseline: no white layer.
