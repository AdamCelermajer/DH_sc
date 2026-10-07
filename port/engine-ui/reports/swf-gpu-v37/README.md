# HUD submission V37: source freeze, runtime acceptance pending

This bounded successor retains the accepted V36 exact geometry cache, scratch
reuse and lazy display materialization. It changes only the modern SWF GPU
submission owner; native movie calls, input graph, source callbacks, timelines,
HUD updates and original supported blend selections remain in place. No batch,
framebuffer fusion, resolution change or forced gameplay state is added.

## Completed source system

- Every primitive follows its existing CPU validation and exact original color
  conversion before an optimization decision. Only exact **final byte alpha
  zero**, normal `SRC_ALPHA / ONE_MINUS_SRC_ALPHA`, outside all masks skips GPU
  work. Invalid texture owners, dimensions, coordinates, color transforms and
  blend producers still fail. A transparent-only display therefore needs no
  FBO clear or fullscreen composite. Source command accounting still advances.
- Transparent lines perform their finite width/matrix conversion but preserve
  the incoming GL line-width state when suppressed. Real lines clamp using the
  current context's cached capability range.
- Mask geometry, active-mask color geometry and non-normal bitmap passes still
  submit even with final alpha zero. Their stencil/blend behavior cannot be
  inferred from alpha alone.
- Warm primitive submission defers GL error queries to mandatory display and
  mask entry/exit, stencil query, scene-pane entry/exit and resource transitions.
  Buffer/texture/FBO allocation and upload checks remain immediate. Required
  failures include the source command range since the preceding successful
  barrier; errors are reported, not discarded or replaced with success.
- Maximum texture size and line-width range are queried once per actual context.
  The owned DEPTH24_STENCIL8 FBO capability is checked after its first allocation
  per context. Same-format resizing reuses that capability. Context initialization
  resets all capability values and abandons old GL names as V36 requires.
- `stats_v37()` exposes transparent no-ops, GL error queries, capability queries
  and source command serials. Existing V36 stats/timing hooks remain available.

## Evidence and acceptance boundaries

| Row | Evidence | Status |
| --- | --- | --- |
| Current GPU/cache/OriginalUiSession source | Six strict `-Wall -Wextra -Werror` ARM64/x86_64 compiles, `compile.json` | PASS |
| Production GPU command/state contract | 1,085 host assertions with ASan/UBSan, `host-receipt.json` | PASS with explicit mock GL/shader transport |
| Exact immutable cache | V36 frozen cache implementation unchanged; V37 host also exercises warm reuse and lost-context reupload | Existing V36 cache acceptance retained |
| Actual GLES pixel fixture | Both ABI link against EGL/GLES with production GPU, shader/cache/color code; `pixel-compile-receipt.json` | COMPILE PASS; execution PENDING |
| Pixel/state/error parity on a real driver | Updated fixture covers transparent mask, normal zero, visible/cache output, scene pane, non-normal zero passes, deferred error ranges and actual context recreation | PENDING safe QA environment |
| Original app menus/HUD/skills and performance | Root owns coherent app build and guarded runtime checks | PENDING; no V37 FPS claim |

The host fixture is explicitly a GL call/state transport, not a pixel renderer.
It executes current production SwfGpu and exact color/cache logic and checks
1,000 warm primitives issue no per-primitive error query; zero-alpha normal/line
draws touch no GPU state; mask/non-normal work is retained; malformed transparent
commands still fail; injected errors reach display/mask/stencil/scene/resource
barriers with command ranges; context cache/reset/reupload occurs.

The actual GLES fixture is compiled only. No emulator, app, ADB operation or
GPU fixture was launched for V37. Runtime is deferred because API37 startup
currently trips the parent safety guard following host commit exhaustion.
V36's earlier actual pixel PASS does not establish V37 pixel parity. The parent's
earlier combined V35/V36 FPS result also does not establish a V37 improvement.

## Integration and next safe test

There are no new production CPP or app integration hooks: the existing engine
UI CMake cache entry and `frame_perf_v35.hpp` timing phases are reused. Parent
must preserve current SwfGpu header/implementation together. The compile-only
helper `.local-inputs/compile_swf_gpu_pixel_v37_bothabi.py` contains no device
operations. Only after a safe device lease, the fixture binaries may be run in
an isolated pbuffer with the actual hash-verified shader pack, then original
profile/three character panels/HUD/skills exercised and FPS measured separately.
