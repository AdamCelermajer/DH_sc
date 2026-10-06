# Native performance checkpoint V35

Scope: improve the playable native Crypt renderer while preserving reconstructed
animation clocks, events, AI, physics, target bounds, original SWF commands and
original artwork. This does not complete the Swamp chapter or player/save/loot/audio
integration. The performance goal remains active until final validation below.

## Implemented systems

- Native phase timers: fixed 120-frame storage, aggregate CPU timing and draw,
  culling and upload counters. No per-frame log or timing heap allocation.
- Optimized native build across the dependency tree, including the UI/VM and
  animation libraries. Debug symbols retained; `-fno-fast-math` and
  `-ffp-contract=off` retained. `DH2_OPTIMIZE_NATIVE=OFF` remains available for
  deliberate unoptimized debugger experiments.
- World GPU submission caches immutable uniform locations and adjacent material
  state. Draw order and all authored blend/depth/culling selections remain intact.
- Conservative camera culling uses current geometry bounds and homogeneous clip
  planes. Rigid and animated scenery/NPC geometry outside the camera avoids GPU
  draw/upload work. Source animation, CPU pose, AI, physics and target bounds still
  run. Invalid bounds submit conservatively. Shared NPC GPU buffers track actor
  owner and uploaded pose revision, including offscreen reentry and context reload.
- Skinning retains palette and position storage, invalidated by bit changes in
  the contributing joint matrices. Equipment uses synchronous borrowed views with
  pinned immutable topology/material ownership; rigid weapon positions are borrowed.
- Numeric animation sampling avoids copying full scene metadata and materials,
  with transactional failure and exact source interpolation/matrix order retained.
- FX retains bounded GPU buffers/topology, skips exact unchanged uploads, and
  captures/restores external GL state once per ordered effect batch.
- Original HUD geometry cache and render-resource work are coordinated separately;
  final accepted implementation and measurement are recorded below when frozen.

## Verification completed so far

- Both Android ABIs build: ARM64 and x86_64, one APK with embedded original cache.
- Conservative visibility: 46,038 host checks, perspective/mirrored/singular/
  clip-boundary/nonfinite cases and randomized interior-point rejection checks.
- Actor/cache source parity: 28,291 ASan/UBSan checks, 172 actual modules,
  animation/rigid fallback and weapon borrow lifetimes; 10 strict Android compiles.
- FX geometry: 521 ASan/UBSan checks. Warm dynamic packet test eliminates
  8,000 allocations across 4,000 updates; this is an isolated CPU test, not FPS.
- Actual GLES FX batch: 90 checks, original FIRE resources, bit-identical rendered
  pixels and restored external GL state. Eight draws reduce state queries 360→45.
- Real main menu → character selection → Crypt, movement, attack, target glyph,
  sword trail and targeted skill damage tested. Portrait click opens the original
  character statistics screen. Final authored-Back and tab acceptance is still
  required; a subsequent tab capture occurred with another app foreground and is
  excluded from menu acceptance.

## Measurement method and intermediate observations

Visible `emulator-5554`, x86_64 API37, 2400×1080, software SwiftShader graphics.
SurfaceFlinger presentation timestamps, 12-second stationary live Crypt workload,
normal AI enabled, no screen recording during measurement. Intermediate runs may
overlap other worker compilation; final repeated runs will be quiet. These numbers
do not establish ARM64 physical-phone performance.

| Candidate | Samples | Mean interval | Approx. FPS | p95 | p99 | >50 ms |
|---|---:|---:|---:|---:|---:|---:|
| Previous accepted 2334 APK | 375 | 37.43 ms | 26.72 | 49.93 ms | 52.28 ms | 18 |
| First O0 instrumented/caching stage | 380 | 36.89 ms | 27.11 | 50.45 ms | 52.83 ms | 25 |
| Optimized native/tree + pose stage | 408 | 33.98 ms | 29.43 | 36.33 ms | 50.93 ms | 8 |
| Full visible-geometry + initial UI cache stage | 474 | 28.40 ms | 35.22 | 36.90 ms | 38.03 ms | 0 |

Full visible-geometry stage warm native timings: actor rendering fell from roughly
7.1 ms to 0.4 ms; complete native frame about 21 ms. HUD remains about 20 ms,
with packing/submission about 1.5/2.3 ms. Empty display/coverage passes and source
HUD update need deeper attribution before claiming a complete smoothness fix.

Raw evidence: `port/android-native/reports/performance-v35/*.json`, logs and
SurfaceFlinger timestamp captures. Intermediate source hashes describe the mutable
worktree, not an exact compiled-source manifest. APK hashes identify measured binaries.

## Final acceptance

Pending coherent HUD source freeze, final build, repeated pacing measurements,
normal targeted-skill and original-menu visual checks, and final APK receipt.
No 60 FPS claim yet. Physical ARM64 and complete Swamp workload remain untested.

Update: V36 HUD source is frozen and its standalone shader/pixel/cache tests pass.
Controlled software-backend observations of the combined build show mean25.69ms
(38.92FPS) versus35.06ms (28.52FPS) for the old accepted build, p99 36.91ms versus
51.82ms. These are short Crypt observations, not full smoothness/stability proof.

Windows resource exhaustion then became the priority. The guarded launcher and
independent watchdog now stop the owned emulator before runaway host commitment.
Actual API37 startup tests hit early5/5.5GiB thresholds before a requested game
launch, including hardware/GLES-only. No new combined APK is marked accepted and
no new unbounded emulator test is permitted. See memory-protection report. The
subsequent inventory/skills tab captures showed another screen and are excluded
from acceptance; absence of errors alone is insufficient visual proof.
