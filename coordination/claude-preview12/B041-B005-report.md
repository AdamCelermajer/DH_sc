# B041 / B005 implementer report

Status: **B041 OPEN, not fixed.** Root cause localized to the source-FX timeline phase path (last working stage
identified below). B005 only partly checked. Nothing here closes either bug.

## 1. Evidence

### Visual (reference video, v1.0.3 Part 1, `.local-inputs/reference-video/dh2-act1/...mp4`)
- Observed (direct, frames looked at): BogWomp fight from about 4:18. Frame grid 4:20 to 4:28 at 4 fps
  (`.local-inputs/claude-preview12/B041/crop_260_268.png`). Zoomed 6 fps run 4:23.0 to 4:25.4
  (`.local-inputs/claude-preview12/B041/zoom_263_265.png`). Knight, Longsword, basic swing.
- Observed trail: a pale blue / white-blue translucent crescent that follows the blade through the swing, about
  one blade length in radius, with a brighter white edge. Tiles remain visible through it (additive-like). It
  appears around 4:23.5 mid-swing, is strongest near 4:24.0 to 4:24.2, and is gone by about 4:24.7. Distinct from
  the red enemy ring and the green/red hit splashes. The blade itself also has a white glow.
- Not separately verified: skill swings (Prince/Knight skill) and the Knight ground impact (B005). Not sampled
  in this pass.

### Logic (verified in this pass)
- The swoosh texture `data/3d/textures/fx_weapon_trail_blue_gradual.tga` is a `BTEXpvr` PVRTC4 (64x64, 2048
  bytes, flags 0x219, no alpha flag). Decoded with the engine's own decoder (`port/engine-textures`, validated
  against original ARM32 in its README) into `.local-inputs/B041-build/tex_x6.png`. Bright blue occupies texel
  columns about 3 to 33 (u 0.05 to 0.52); columns 34 to 63 are black.
- Authored texture track (`TextureTransform20V1`: offset_u, offset_v, rotation, scale_u, scale_v). Sampled through
  the engine's own `texture_sample_v1` on `swoosh_prince_1hand_combo_01.bdae`, anim 3 seg 0 (the one accessor that
  matches), interpolation on (`.local-inputs/B041-build/h/track_probe.cpp`):
  offset_u = 0.000 at 0 ms, 0.141 at 33, 0.353 at 83, 0.652 at 167, 0.870 at 250, 1.000 at 333. A linear 0 to 1
  U ramp over the 333 ms life. offset_v = 0, scale = 1.
- Mesh graph path: `port/level-world/authored_fx_mesh_graph_v32.cpp:77-81` (`sample`) samples that track at the
  `ms` it is given and writes the 2D matrix (`dh2_fx_texture_matrix_v1`, translation = offset_u for rotation 0).
  `draw_sources` (line 87-88) applies the matrix once to the copied UV stream.
- **Live-path timeline defect (observed in the real session code, not a sampler artifact):**
  `port/windows-foundation/features/effects/runtime_effects_factory_native_tests.cpp` (factory session, same
  `RuntimeEffectsFactoryV1` as the production path). After `manager().scene_frame(phase_ms, dt)` for phase 1 ms,
  the source view reports `current_ms=333 end_ms=333` (stderr debug lines in the test run log, all phases
  1 to 330 ms: current=333). The timeline is at its end from the first update after the step.
  Consequence: the sampled track is at 1.0 (offset 1 U, which under GL_REPEAT is the same texel column as 0) for
  every phase. Texel columns sampled: u 0.711 to 0.991 (+1). Black half. Result: additive blend of black, so the
  trail is invisible in this path. This matches the earlier WGL result (7 changed pixels) and the CPU projection
  (7524 of 7534 black).
- The packet UVs are black at every phase 1 to 330 ms (dense sampling), so the "test samples only the first
  instant" hypothesis from the investigation doc is **refuted**: the window is black throughout, not only at t=0.
- Suspect for the timeline jump (not yet proven): `port/level-world/visual_timeline.cpp:17-35`
  `dh2_timeline_update`. The first call after `initialized` uses `last_seconds` from the previous call. If that
  base is stale or far from the new absolute time, `elapsed` is large and positive, `current` crosses `end`, and
  `ended=1` fires the callback (`dh2_fx_handle_loop_end_v1`) then `end_sample` (which jumps to end and samples
  there, `character_mesh_fx_owner_v4.cpp:133`). Not verified; needs the input values of the first two calls.
- Live-path claim checks: `main.cpp:1766` constructs `RuntimeSwingFxObserverV1` and registers it (line 1779-1787).
  The tracker claim "not registered" is stale. The factory's `read_asset` (`runtime_effects_factory_v1.cpp:58-63`)
  routes `is_runtime_source_fx_uri_v1` URIs to `read_runtime_source_fx_asset_v1` (the exact-URI/hash resolver), so
  the B005 resolver is wired in the factory read path in code. Not run in a live game.

## 2. Expected behaviour and fix status
- Expected (from video): a translucent pale blue / white crescent, about one blade length, following the blade
  through the swing (roughly 1 s visible per swing, timing approximate from 6 fps frames), drawn additive with
  depth write off.
- Fix: **not made.** The defect is the source-FX timeline phase advance (or its first update), which is not
  yet pinned to a line. Changing the UV or the texture would be a guess and was not done.

## 3. Changes
1. `port/windows-foundation/features/effects/runtime_effects_factory_native_tests.cpp`:
   - Phase sampling in the source-timeline block (around line 746-760): replaced the four fixed fractions
     `{0.25,0.5,0.75,0.95}` by a dense list (1 ms steps to 20 ms, then 5 ms steps, plus 0.95). Comment added.
     This makes the test record the real phase curve. It does NOT yet assert visibility (the test still passes
     while the trail is black, see Tests).
2. `port/windows-foundation/features/effects/runtime_effects_factory_native_tests.py`: added
   `FEATURE / "runtime_source_fx_asset_v1.cpp"` to `source_files`. Without it the runner failed to link
   (`read_runtime_source_fx_asset_v1` undefined, since `runtime_effects_factory_v1.cpp` already calls it).
   The production CMake already lists this file (`port/windows-foundation/CMakeLists.txt:182`).
3. `port/windows-foundation/features/effects/runtime_effects_renderer_wgl_smoke.py` and
   `runtime_effects_factory_wgl_smoke.py`: same one-line addition after `runtime_effects_factory_v1.cpp`
   (same link dependency). Only `py_compile` checked; not run (GUI).
4. Regenerated tracked evidence by running the runner: `features/effects/runtime-effects-factory-native-report.json`
   and `features/effects/runtime-swing-fx-observer-report.json` (now contain the dense phase list).
5. Not touched: `main.cpp`, `CMakeLists.txt`, `docs/`, saves. No `git` commands.

Diagnostic files (git-ignored, under `.local-inputs/B041-build/`): `h/dump_tex.cpp` (texture decode harness),
`h/track_probe.cpp` (track sampler), `tex_x6.png`, `phases.py`, `factory_dense_run.log`, `factory_final_run.log`.

## 4. Tests (run in this pass)
- `"$PY" -I port/windows-foundation/features/effects/runtime_effects_factory_native_tests.py` (before my runner
  fix): FAIL at link (`read_runtime_source_fx_asset_v1` undefined). After the one-line runner fix: **EXIT=0,
  validation PASS x3**; report phases: 1 to 330 ms, `uv_min=1.711` at every phase, `nonblack=0/74`,
  `maxtex=0`. (`factory_final_run.log`).
- `py_compile` of `runtime_effects_renderer_wgl_smoke.py` and `runtime_effects_factory_wgl_smoke.py`: OK.
- Texture decode harness: `open=ok w=64 h=64 format=1`, `decode=ok`; rgb max (132,214,255); 1782 texels with rgb>8.
- Track probe: offset_u series above (engine sampler, anim 3 seg 0 only valid accessor among 40x4 probes).
- Not run: `run_tests.py` (full effects suite), the WGL renderer/factory smoke (GUI, not launched; the earlier
  terminal WGL run was 7 changed pixels, FAIL), and any live game run. No WGL phase readback was produced in this
  pass.

## 5. Uncertainties / what the verifier and root must check
- Why `dh2_timeline_update` reaches `end` on the first update (inputs `last_seconds`, `scale`, `elapsed`,
  `initialized` on the first two calls). Also whether the live `scene_frame` absolute time is consistent with the
  creation time (the test passes absolute 1..330 ms from a fresh manager; the live game passes real times).
- Whether the live EXE also has the timeline at end. Check: during a Longsword swing, the "Source player step FX
  frame=" line (main.cpp:1769) or a temporary print of `view.current_ms` per frame. If it reads 333 at the first
  frame, the trail is dead in production and B041 is the timeline bug, not the texture.
- The swoosh track might be intended to saturate only after a key; the probe shows a linear ramp to 1.0 at 333 ms.
  The fix must follow the original timeline semantics (IDA `dh2_timeline` / `FX` update), not a clamp.
- Skill swings (Prince/Knight) and the Knight ground impact (B005): not sampled in video or code in this pass.
- B005: `RuntimeSwingFxObserverV1` is registered (main.cpp:1766-1787) and the factory uses the exact-URI resolver,
  but asset staging, the live BashDown FX draw, and the ground impact timing were not verified.
- Tracked report JSON files were rewritten by the runner (dense phase list); the verifier should re-run before
  relying on them.
