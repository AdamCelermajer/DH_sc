# B041 implementer report 2 (continuation of B041-B005-report.md)

Status: **B041 NOT closed.** Re-investigation disproves the previous root-cause claim (the timeline does not jump
to its end in production). The swoosh IS drawn with visible pixels in the isolated WGL renderer at mid-life phases.
No production code was changed. Remaining gap: the live EXE draw path and the live-vs-reference trail
shape/duration were not verified. Nothing here closes B041 or B005.

## 1. Evidence

### Logic (verified by native probe on the real session/factory path)
- Probe copy: `.local-inputs/claude-preview12/B041/probe/probe_test.cpp` (copy of the factory native test, git-ignored),
  run by `probe/probe_runner.py`. The swoosh view was created at source absolute 256 ms (current 0, start 0, end 333).
  Then 16 ms steps through `CombatSession::update(0.016)` and `RuntimeEffectsFactoryV1::runtime().update(frame, abs, 16)`:
  - `B041PROBE` output: step 0 (abs 272) current 20, step 1 41, step 2 62, ... step 14 311, step 15 332, then 332
    (end 333). Advance is about 20.8 timeline ms per 16 app ms (ratio about 1.3).
  - So the first update after creation does NOT jump. `dh2_timeline_update` takes the `!initialized` branch
    (`port/level-world/visual_timeline.cpp:23`), elapsed=0, current stays 0. The timeline reaches end after about 16
    steps (about 320 app ms).
- Where the "current=333 at every phase" came from: the test's phase loop (old `runtime_effects_factory_native_tests.cpp`
  ~746-778) called `scene_frame(timeline_current + offset, dt)`. Timeline-relative ms were used as the absolute source
  clock, while the timeline had already been updated at about 256..448 ms. The first call (`scene_frame(1, 1)`) moved the
  clock backwards; `B041PHASE` probe output: `phase_abs=1 dt=1 current=333`. Later phases stayed at 333. This is a
  harness defect; the production clock is monotonic.
- Production clock: `main.cpp:2927-2931` accumulates `sourceEffectsMs` and passes `integerMs` as dt to
  `runtime().update(...)`; `runtime_combat_effects_v1.cpp:100-101` calls `scene_phase` then `manager_phase`.
- Timeline scale ratio of about 1.3 is consistent with the attack-speed scaling of FX speed:
  `port/level-world/character_state.cpp:50` calls `speed(s,c,f.attack_speed)` and
  `character_fx_state_v1.cpp:8/19/51` writes it to `dh2_timeline_scale` (`character_mesh_fx_owner_v4.cpp:129`). I did not
  trace the Prince's attack_speed value; the 1.3 is measured, its source is inferred.

### Pixels (factory native test, sampled texels) and WGL renderer (real GL, isolated process)
- Factory native test (fixed, monotonic phases, 16 ms steps from source 256 ms): `nonblack_uv_samples` per phase
  (source absolute ms): 272:0, 288:9, 304:22, 320:37, 336:46, 352:49, 368:47, 384:39, 400:28, 416:23, 432:14,
  448:5, 464+:0. So the authored bright band is sampled from about 288 to 448 source ms (about 32 to 192 app ms after
  creation), peaking at 352 ms source. (`features/effects/runtime-effects-factory-native-report.json`, regenerated.)
- WGL renderer (copy of `runtime_effects_renderer_wgl_smoke.cpp` with an env hook `B041_STEPS=N` that advances the real
  clock N x 16 ms before the frame is prepared; `.local-inputs/claude-preview12/B041/wgl/`). Framebuffer
  `changed_pixels` (threshold >10) and the PPM / PNG (`.local-inputs/claude-preview12/B041/wgl/ppm/steps_N.png`):

  | steps (x16 app ms after creation) | changed_pixels | result |
  |---|---|---|
  | 0 (creation) | 7 | black (same as the earlier FAIL) |
  | 2 | 3101 | PASS |
  | 4 | 7174 | PASS |
  | 6 | 7308 | PASS |
  | 8 | 5725 | PASS |
  | 12 | 0 | black |

- I LOOKED at `steps_4.png` and `steps_8.png`: a saturated blue curved trail, a crescent of about one blade length
  with streaks, dark background (clear rgb 9,11,15). At step 8 it has moved and faded. Texture max rgb (132,214,255),
  material colour 0.584 grey, additive blend (1,1), depth write off (status JSON).
- Canonical WGL smoke (`runtime_effects_renderer_wgl_smoke.py`) was NOT changed and was NOT rerun: it captures at
  creation (step 0), which is black by design of the track (offset 0). Its `changed_pixels=7` FAIL is therefore an
  expectation problem for that capture point, not evidence of an invisible trail. Do not treat the canonical WGL FAIL as
  the B041 result.

### Reference video (previous worker's frames, looked at again: `.local-inputs/claude-preview12/B041/zoom_263_265.png`)
- Observed in several 6 fps frames (about 4:23.0 to 4:25.4, Knight, Longsword): a pale blue, translucent, broad
  arc/disc under and beside the character, with a whiter edge, larger than the blade. Our render is a narrower, more
  saturated blue crescent. The reference may be a different geometry or a different FX; not resolved in this pass.
  This is a fidelity question for the fidelity agent, not a visibility bug.

## 2. Expected behaviour and fix status
- Expected: a translucent blue-white trail during the swing (reference), drawn additively, visible for part of the swing.
- Measured in the isolated renderer: visible at about 32..176 app ms after creation (steps 2..8), black at creation and
  after about 192 ms. The earlier "invisible" claim was based on the test harness clock bug.
- Fix: **none in production.** `visual_timeline.cpp`, the effects executor and the factory are unchanged. Changing them
  would be a guess. The previous report's suspected cause (first update jump) is refuted (see Logic).

## 3. Changes
1. `port/windows-foundation/features/effects/runtime_effects_factory_native_tests.cpp` (source-timeline block,
   ~lines 746-790): replaced the timeline-relative phase feed with a monotonic sweep (`scene_frame(source_absolute_ms +
   16*step, 16)` until the timeline reaches its end, max 64 steps). After the loop, asserts at least one phase has
   `nonblack_samples > 0` (`check(bright_phases>0, "B041: no source FX timeline phase samples the bright swoosh band")`).
2. Regenerated tracked evidence: `features/effects/runtime-effects-factory-native-report.json` (by the canonical runner;
   phases now 272..528 ms source, nonblack 0..49). `runtime-swing-fx-observer-report.json` was also rewritten by that
   runner.
3. No change to `visual_timeline.cpp`, `main.cpp`, `CMakeLists.txt`, the WGL smoke files, docs or saves. The earlier
   one-line runner link fix in `runtime_effects_factory_native_tests.py` and the WGL py files are from the previous
   worker (still in the diff, not reverted).

Copy-only probes (git-ignored, under `.local-inputs/claude-preview12/B041/`): `probe/` (16 ms session probe and phase
mode), `run1/` (run of the fixed test), `wgl/` (phase-hook WGL copy and runner), `ppm/` (captures), `tools/ppm2png.py`.

## 4. Tests (commands and real results)
- `"$PY" -I .local-inputs/claude-preview12/B041/probe/probe_runner.py` (16 ms probe): `B041PROBE step=0..29`, exit 1 by
  design (probe `std::exit(3)`); output above.
- `"$PY" -I .../B041/probe/probe_runner.py` with `B041_MODE=phase` (test's old phase feed): `B041PHASE phase_abs=1 dt=1
  current=333`; exit 4 by design.
- `"$PY" -I .../B041/run1/runner.py` (copy of the native runner, fixed test, own build folder): stdout contains
  `"validation": "PASS"` x3, `EXIT=0`.
- Canonical: `"$PY" -I port/windows-foundation/features/effects/runtime_effects_factory_native_tests.py` (from repo root):
  `"validation": "PASS"` x3, `EXIT=0`. Log: `.local-inputs/claude-preview12/B041/canonical_stdout.log`.
- WGL copy compile (`wgl/runner.py --compile-only`): `{"validation": "COMPILED"}`, `EXIT=0` (the copy needed
  `platform_context_identity.cpp` added to its source list; the canonical WGL runner has the same gap, not changed).
- WGL phase captures: `B041_STEPS=0,2,4,6,8,12` runs, results in the table above (exit codes in `wgl/ppm/run_N.json`).
- Not run: `run_tests.py` for the whole effects folder, the canonical WGL smoke, and any live game run.

## 5. Uncertainties / not verified (for the verifier and root)
- The live EXE was not run. Verify in an isolated window: a Longsword basic swing, then capture several frames across
  about 0.05..0.2 s after the swing starts. Expect visible blue pixels. If the live window is black at those times, the
  gap is in the live draw path (`main.cpp` around the source FX draw, camera, or draw order), not the timeline.
- Whether the live first frame after creation is black (WGL step 0 is black, which matches the track). The swing
  creation-to-visible delay is about 32 ms of app time.
- Trail duration: the bright window is about 150 ms of app time (1.3x scale). The reference shows a translucent arc
  for about 1 s. This may be a different FX or a longer set; not resolved.
- Trail geometry and colour versus the reference (narrow saturated crescent vs. pale translucent wide arc).
- Skill swings (Prince/Knight) and the Knight ground impact (B005): not checked here.
- The 1.3 timeline scale source (attack speed) is inferred from `character_state.cpp:50`; not traced to the value.
