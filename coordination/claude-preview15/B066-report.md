# B066 performance report ("still not great" on the user's machine after 15.1 candidate: skills and general play)

## Honest status
The slowness was NOT reproducible on the test machine (RTX 4070 SUPER, hardware GL): with the B062 fixes it already ran
about 15.4 ms/frame, and in a real visible 1280x720 window `SwapBuffers` returned in 0.15 ms (no vsync blocking observed
here), GPU time was 0.09-0.23 ms per frame even at 1920x1080. So the causes below are structural findings on code paths
that cost much more on a slower GPU/driver or with a vsync-blocking swap. They are measured on this machine as CPU/driver
savings; the benefit on the user's machine is expected but unverified. The user's own DH_PERF log (instructions at the
end) is the way to confirm.

## Method / evidence
No original-video or IDA evidence applies (a smoothness complaint, port-side rendering code; no gameplay behaviour changed).
Instrumented the real EXE (`DH_PERF=1`) and ran Swamp scenarios (idle, melee, skill keys 1/2/3, Faery 4/5; knight save with
skills, lizards/moths with AI; 420-520 frames) through `quiet_run.ps1` (hidden desktop, silent; GL still runs on the real
driver), 3 interleaved before/after rounds, plus brief real visible windows (900 frames, 1280x720 and 1920x1080,
`DH_AUDIO_SILENT=1`, no input, about 15 s each). Not measured: Mage/Rogue skills (class saves are rejected by the
fresh-player harness, same FX path scales with packet count) and menus beyond a pixel-equivalence check.

## Findings (extended DH_PERF, see "Instrumentation")
1. Frame pacing: the main loop and the front-end loop slept `sleep_for(1 ms)` per frame. On Windows that is a
   15.6 ms scheduler tick. Measured: sleep averaged 9-10 ms of every 15.4 ms frame. On a machine where the swap blocks on
   vsync (frame ends at a 16.7 ms vblank) the extra tick-rounded sleep adds up to 15.6 ms: a 60 Hz display falls to
   about 30 fps, a 144 Hz display to about 45 fps. Never explicitly set: no swap interval anywhere (driver default).
   This matches "not great in general" and is the most likely cause on a real display, but I could not trigger a
   blocking swap here, so it is a code-proven, not machine-proven, cause.
2. Level geometry drawn through client arrays every frame: 392 level ranges = about 448 `glDrawElements` per frame, each with
   `glVertexPointer` into the full scene vertex array and a colour array of the whole scene. The driver must scan indices and
   copy the referenced vertex span for every call (cost scales with driver/CPU; on NVIDIA 1.6-2.6 ms world draw).
   No culling: the whole swamp level (24.7k triangles) was submitted each frame although the camera sees about 5% of it
   (only 22 of 392 ranges are visible).
3. CPU scans of the whole level every frame: vertex-alpha scan (`isTransparent`, `drawRange`, `isBlended`) and bounding-box
   scan (`rangeDepth`) for every range, three to four times per frame.
4. `RuntimeEffectsRendererV1::finish_and_drain` called `glFinish()` on every frame with an FX packet on screen: a full CPU/GPU
   round trip (CPU time + GPU time per frame instead of the maximum, worst on slow GPUs, exactly "skills"). Measured
   fxDrain 0.3-1.4 ms here (swap+drain), now 0.003-0.08 ms. Client-array draws are consumed when the draw call returns, so the
   finish was not needed for memory safety (kept under `DH_GL_FINISH=1` for diagnostics).
5. HUD overlay `glPushAttrib(GL_ALL_ATTRIB_BITS)` + `glPushClientAttrib(ALL)` every frame (cheap on NVIDIA, heavier on
   Intel/AMD drivers). Narrowed to the state groups the overlay can change.
6. Checked and NOT a problem (measured): per-frame logging (about 1 line per 0.4 frame, per the B062 measurement), file
   IO, per-frame texture uploads (only on first use: 18 at start, 0 afterwards), immediate mode (HUD: 42 batches, 996 vertices
   per frame, 0.07-0.14 ms), CPU skinning (`visual.update` 0.26-0.38 ms per frame for 9 actors/frame), AI/physics
   (`sim.*` probes 0.01-0.09 ms each), `combatSession.update` total 1.2-1.9 ms (animation retained playback; scales with
   actor count), HUD scale (hud 0.07 ms), shadow/blob passes (none separate), resolution (GPU 0.09 ms at 720p, 0.17 ms at 1080p).

## Change
- `platform_sleep.hpp` + platform layer: `FramePacer` (portable `std::chrono`): sleeps only the REMAINDER of a 60 fps period
  (zero when a vsync-blocking swap already used it), 1 ms timer resolution on Windows (`timeBeginPeriod(1)` loaded
  dynamically in `platform_win32.cpp`, no-op on Linux), yield-spin for the last 2.5 ms. `DH_FPS_CAP=<n>` (0 = uncapped).
  Used by the game loop (`main.cpp`) and the front-end/menu loop.
- `Window::set_swap_interval/swap_interval/gl_proc` (WGL_EXT_swap_control, SDL_GL_*): vsync is now explicit (on, `DH_VSYNC=0` off)
  and logged ("Swap interval requested=1 supported=1 effective=1").
- Static level geometry (`Mesh::staticGeometry`, set by main for `scene.mesh`): vertices and indices in VBO/IBO (GL 1.5 entry
  points through `Renderer::setGlLoader`, client-array fallback when absent), per-range bounds/vertex-alpha cached
  (`Renderer::rangeStats`), conservative view-frustum culling in `RenderQueue::flush` (8 corners vs 6 planes), invalidated
  when the level is replaced (`invalidateStaticGeometry`). Tinted ranges (material colour != 1) keep the client colour
  array, so output is bit-identical. Skinned actors and FX remain dynamic client-array draws.
- `runtime_effects_renderer_v1.cpp`: no per-frame `glFinish` (see finding 4). `overlay_renderer.cpp`: narrowed attrib masks.

## Instrumentation (DH_PERF, off unless `DH_PERF=1`)
`frame_perf.hpp`: per-second line `Perf second ...` (all phases incl. new `fxDrain`, `swap`, `sleep`), `Perf gl | world|fx|hud
calls tris binds states imVerts imBatches clientKB vboDraws culled | textureUploads | gpuMsPerFrame`, spike lines (>33 ms), a
final `Perf summary` (frames, avg, p50/p95/p99, max, `workMs` = frame minus sleep and swap, `gpuAvgMs/gpuMaxMs`
from `GL_TIMESTAMP` queries read back asynchronously, per-phase averages, `over33ms`), `Perf probe` lines (skin, sim sub-steps,
FX), `Perf GL vendor/renderer/version/maxTexture/extensions/viewport`, and the swap-interval line. Everything is also appended to
a log file (`DH_PERF_LOG`, default `dh-perf.log` in the working directory).

## Before / after (interleaved B,A x3 per scenario, `ab1.csv`; work = ms/frame excluding sleep and swap)
Before = Preview 15.1 candidate + instrumentation; after = this branch. Avg frame: before 15.4 (65 fps, set by the 15.6 ms
sleep tick; capped by nothing), after 16.7 (60 fps cap by design; with a blocking vsync the old loop would be 30-45 fps).
| scenario | work before (avg of 3) | work after (avg / min of 3) | world draw ms before -> after |
|---|---|---|---|
| idle | 4.81 | 2.82 / 2.54 | 2.65 -> 0.90 |
| melee | 3.92 | 3.36 / 2.69 | 1.68 -> 0.99 |
| skill 1 | 4.21 | 3.73 / 2.69 | 1.90 -> 1.18 |
| skill 2 | 4.22 | 4.35 / 3.02 | 1.79 -> 1.43 |
| skill 3 | 4.04 | 3.43 / 2.98 | 1.86 -> 1.03 |
| Faery | 4.07 | 3.40 / 3.03 | 1.83 -> 0.98 |
(Run 1 of the "after" rounds ran while another session built on this machine; min column is the undisturbed figure.)
Per frame: draw calls 448 -> 78-83, level triangles 39.6k -> 17.5k, client-array bytes/frame about 10x lower for the level,
HUD 42 batches unchanged, FX drain 0.3-1.4 -> 0.003-0.08 ms. Variance: p50 16.67 (cap), p95 16.7, p99 16.8-17.3
(quiet) and 20-34 in disturbed rounds; the one >33 ms frame is frame 1 (about 60 ms first upload).
Real visible window 1280x720, 900 frames, idle: before avg 15.18 / world 2.37 / swap 0.15; after avg 16.69, p99 16.76,
work 2.41, world 0.75, swap 0.06, gpu 0.09 ms. 1920x1080: avg 16.73, work 2.71, world 0.82, gpu 0.17 ms.
Pixel check: deterministic fixed-step captures (330 frames, skill 2 mid-cast, idle, Skills page, Equipment page, Pause menu)
before vs after are byte-identical PPMs (`cmp` 0 differences).

## Tests
- `renderer_static_geometry` (new, real GL context): static mesh (VBO + culling + range cache) renders pixel-identical to the
  client-array path for 9 camera poses (inside, edge, looking away, everything outside), proves culling happened
  (1239 ranges) and VBO draws used (525), non-static meshes never use buffers, and an invalidated mesh follows geometry edits.
- `frame_pacer` (new): a 2 ms frame is released at 10.000 ms (target 10), not at a timer tick; a frame already over the period gets no
  added sleep.
- Perf regression guard: `port/windows-foundation/tools/perf_budget.ps1 -Exe <dh-foundation.exe> -Package <package root>
  -SaveDir <knight save dir>` runs the six scenarios with DH_PERF through `quiet_run.ps1` and fails (exit 1) over budget:
  workMs 9, p50 18.5, p99 45, over33 6, level draw calls/frame 160, client-array KB/frame 4000, FX prep 1.5 ms, HUD 2 ms,
  FX drain 3 ms (about 2.5x headroom over the numbers above). Verified OK on this build (all 54 checks); it fails on the
  15.1 binary because the new metrics are missing. Needs the local package, so it is a script, not a ctest.
- ctest (build-fix066): all pass except `session_skill_binding` (known) and `frontend_music_v1`, `winmm_pump_priority_v1`,
  `startup_loading_art_v1` (exit 0xc0000135 DLL load at process start; the same executables fail identically in the
  fix064/fix065/p15patch builds, so pre-existing in this environment).

## Remaining / not fixed
- First cast of each skill still loads and uploads its FX textures synchronously (about 16 ms average, 44 ms worst, once per
  texture). A pre-cache at level load needs the skill-to-effect-resource mapping; not done.
- Skinned actors (about 56 draws, 1.3 MB/frame of client arrays) are still client-array draws; a streaming VBO would help
  a slow driver further. FX packets likewise.
- `combatSession.update` (1.2-1.9 ms, retained animation playback) scales with actor count; no culling of off-screen actors' animation
  (original behaviour; would need evidence).
- Per-material GL state shadowing (redundant state elision) not done; negligible here.
- The whole result is unverified on the user's machine; see below.

## How the user captures a DH_PERF log (send the file back)
In the package folder (where `Play-swamp.cmd` is), open a Command Prompt and run:
```
set DH_PERF=1
set DH_PERF_LOG=%USERPROFILE%\Desktop\dh-perf.log
Play-swamp.cmd
```
Play 1-2 minutes (idle, fight, cast each skill a couple of times, open the menu), then close the game normally with
the window's X (the summary line is written at exit). Send `dh-perf.log` from the Desktop (without `DH_PERF_LOG` it is
`dh-perf.log` next to the exe). Useful variants to try for comparison: `set DH_VSYNC=0` (vsync off), `set DH_FPS_CAP=0`
(no frame cap), `set DH_FPS_CAP=30`. The log contains the GPU model/driver, swap interval, per-second phase timings
(worldDraw, swap, sleep), draw/state counts and `gpuMsPerFrame`.

## Package files required
None (code only).
