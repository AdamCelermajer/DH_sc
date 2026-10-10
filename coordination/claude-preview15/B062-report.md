# B062 performance report ("huge performance issue in skills and in general")

## Evidence / method
- Video: not applicable (a smoothness complaint); measured the real EXE instead. Logic: no IDA needed; the cause is port-side.
- New debug facility (portable, std::chrono, off unless env `DH_PERF=1`): `frame_perf.hpp`. Unclamped per-frame total and per-phase
  ms (poll, input, audio, sim update, post update, camera, FX update, FX render prep, scene build, world draw, FX draw, HUD/UI,
  drain+swap, sleep), draw calls, triangles, texture uploads, spike lines (>33 ms) and a summary with p50/p95/p99/max. `DH_PROBE("name")`
  adds named sub-probes. Main loop is instrumented in main.cpp (marks), renderer.cpp (draw/texture counters), FX bridge/material binding (probes).
- Scenarios (Swamp, Preview 15 swamp.args + saved knight with skills, melee held from frame 60, lizards/moths with AI): idle, melee,
  skill keys 1/2/3, Faery (keys 4,5), 420-520 frames; plus a 2500-frame long run. Run only via quiet_run.ps1 (hidden desktop, silent).
  Not measured: Mage/Rogue skill sets (the class saves are rejected by the fresh-player harness: "Profile vital exceeds prepared source maximum"); same code path, the cost scales with FX packet count.
- GPU in the test machine: RTX 4070 SUPER (hardware GL), so draw time here is a floor for the user's machine.

## Root cause (measured)
`EffectsRenderBridge::prepare` runs every frame and for every live FX packet calls `OriginalEffectMaterialBinding::bind`, which called
`resolve_content_path()` for the packet's texture EVERY frame. `resolve_content_path` builds ~15-30 candidate paths and, for each path
component, re-enumerates the whole directory with `directory_iterator` and case-folds every entry (the texture directories hold
thousands of files). Measured 4.5 ms per call (worst 7-45 ms), ~1.5 calls/frame in melee (weapon-swing FX), more with skills:
FX render prep averaged 5.5-8 ms per frame (up to 60 ms) in every combat/skill run, and in the 2500-frame run reached 24 ms avg with
multi-second frames (3.5 s worst). It scales linearly with the number of FX packets, so heavy skills (many particles) are the worst
case. Idle (no FX) was fine, which is why "skills" feel worst, but plain melee also paid it ("in general").
Other findings (not the cause): logging is negligible (about 1 line per 0.4 frame); no per-frame file/asset loads outside this path;
draw calls 448-455 per frame (~39.6k triangles) constant, texture uploads only on first use; audio phases < 0.04 ms/frame.

## Change
1. `OriginalEffectMaterialBinding`: cache the resolved path per (diffuse uri, owner resource uri) (cleared with the texture cache).
2. `content_paths.cpp`: generic directory-listing cache (per directory, validated by the directory write time, racy-timestamp guard:
   directories modified in the last 2 s are not cached), mutex protected, portable `std::filesystem`. Speeds every caller of
   `resolve_content_path` (world-item drops, etc.), semantics unchanged (case-insensitive match, ambiguity rejection).
3. `RuntimeCombatEffectsV1::prepare_render_frame`: prune expired frame-loan weak_ptrs every frame (the vector grew by one per frame for the whole session, keeping every frame's control block alive).
4. `DH_PERF` instrumentation (above).

## Tests
`content_paths` test extended: settled directory lookup, cache-hit answer unchanged, file added after a cached listing is found,
removed file no longer resolves. ctest: 117/119 pass; failing `session_skill_binding` (known worktree failure) and
`winmm_pump_priority_v1` (exit 0xc0000135, DLL load failure at process start, not touched by this change; not rerun on the baseline).

## Before / after (same time window, interleaved B,A per scenario, 3 runs each; frame ms incl. the 1 ms sleep, which Windows rounds to the 15.6 ms timer tick, so ~15.3 is the floor here)
Final build, `ab2` (before = Preview-15 code + instrumentation, after = fix):
| scenario | before avg / p95 / p99 / frames>33ms (3 runs) | after avg / p95 / p99 / frames>33ms (3 runs) |
|---|---|---|
| idle | 15.4 / 16.0 / 16.2 / 1 | 15.4 / 16.0 / 16.2 / 1 |
| melee | 17.0-17.8 / 30.5-30.7 / 31.1-31.4 / 1 | 15.35-15.44 / 15.8-16.0 / 16.1-16.2 / 1 |
| skill 1 | 17.8-20.0 / 30.7-31.1 / 31.4-31.6 / 1 | 15.35-15.40 / 15.8-16.0 / 16.1-16.4 / 1 |
| skill 2 | 18.8-20.0 / 31.2-46 / 45.5-47 / 8-48 | 15.42-15.44 / 15.8-16.1 / 16.2-16.3 / 2 |
| skill 3 | 17.1-23.0 / 30.4-32.5 / 31.2-52 / 1-22 | 15.35-15.37 / 15.96-16.07 / 16.2-16.3 / 1 |
| Faery | 18.2-20.5 / 30.6-31.1 / 31.5-68 / 1-8 (max 924) | 15.34-15.38 / 15.7-16.0 / 16.2 / 1 |
FX render prep per frame: before 5.5-8.2 ms, after 0.04-0.13 ms. The one >33 ms frame in every run is frame 1 (first-frame texture/model upload, about 60 ms).
Earlier interleaved run (`ab1`, machine busier) showed before up to avg 42 ms / p99 214 ms / 220 slow frames; after identical to the table.
2500-frame run with 8 casts: before avg 52.8 ms (p99 432, max 3786 ms); after avg 16.4 ms (p99 44, max 180).
Variance: "after" runs agree within 0.1 ms avg; "before" varies 17-42 ms with machine load (other jobs build on this machine), so the fix also removes the sensitivity to load.

## Remaining (not fixed, measured small here)
- Remaining real work per frame is about 5 ms (sim/AI/animation 1.3-1.6, world draw 1.8-2.8 with 450 immediate-mode draw calls and per-frame CPU
  transparency/vertex-colour scans of the whole scene mesh, swap 0.5). A slower user CPU/GPU will scale this part; a static-mesh transparency/colour cache would be the next step.
- First cast of each skill loads and uploads its FX textures synchronously (12-46 ms hitch once per texture; `fxmat.texture_load_upload`); pre-caching at level load would remove it.
- Isolated sim spikes of 20-50 ms in combat runs coincide with scheduler stalls in other phases (sleep up to 113 ms) so are attributed to machine load, not confirmed as game cost.
- Main loop uses `platform_sleep_milliseconds(1)`, which on Windows with default timer resolution is a 15.6 ms tick: frames cluster at 15/31 ms. Not changed.
- Not verified on the user's actual machine.

## Package files required
None (code only).
