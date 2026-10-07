# HUD/SWF backend performance V36

The first measured system optimizes rendering work while preserving source
HUD/movie/native calls, updates, timelines, geometry, colors, masks and input.
No reduced resolution, hidden HUD, animation/callback skipping, fullscreen
target fusion or adjacent draw batching was introduced.

- GPU conversion reuses a persistent vertex scratch vector and reads the
  existing command XY directly; bitmap quads use stack storage.
- A bounded cache (8MiB / 1024 entries) retains immutable exact packed vertex
  bytes and GPU buffers. Hash matches are checked against every byte and draw
  mode. Matrix, tint, blend, wrap and source draw calls are still submitted.
  Oversized geometry renders through the original stream-buffer fallback.
- Eviction releases only owned buffers. GL recreation abandons old names and
  uploads the retained CPU bytes once when needed; image reset clears buffers.
- Texture wrap is changed only when its actual owned texture needs a new mode.
- Display begin remains a logical source operation. The private coverage FBO
  is materialized on the first actual primitive, mask, stencil query or scene
  pane. Empty end performs no fullscreen clear/composite and leaves incoming
  output/state untouched. Alpha-zero actual primitives remain delivered.
- Existing glGetError checks remain, including immediate resource-upload
  checks. Invalid nesting, masks, required resources and scene-pane calls
  preserve required error reporting/cleanup. Error-query aggregation was not
  added to this frozen implementation.
- Root's fixed performance phases attribute `ui_update` to HUD source queries,
  setters and real timelines, `ui_geometry` to vertex conversion, and
  `ui_submit` to primitive/control GL work and error queries. Cumulative backend
  stats expose primitive count, scratch growth, cache hits/uploads/bytes and
  logical/empty/materialized display counts.

## Verification

- Strict current source compile: **6 successful compiles** across ARM64 and
  x86_64 (`compile.json`), covering GPU, cache and OriginalUiSession.
- Standalone cache test: **1049 checks passed with ASan/UBSan**. Covers 1000
  warm hits, mode/data keys including signed zero, bounded LRU/budget fallback,
  lost-context reupload, release counts and failed-upload cleanup. The small
  triangle expansion helper is tested but not enabled in the renderer.
- Actual isolated Android EGL/GLES2 pixel test: **276 checks passed**
  (`pixel-receipt.json`), using current production GPU/cache code and the
  original shader pack with its production SHA256 verification. Tests 100
  empty displays with zero materializations and unchanged pixels, alpha-zero
  primitive delivery, repeated cached pixels without additional uploads,
  mask inside/outside output, nested-display error cleanup and scene-pane
  materialization/pixels. Its disk asset transport is explicitly a fixture.
- The pixel test only used a separate pbuffer process and unique
  `/data/local/tmp/dh2-swf-gpu-v36`; it did not install/start/touch the app,
  alter a movie or restart the device/server.

The parent will build/install the final combined source and measure original
menu/HUD/skill flow and FPS. No live performance improvement or full menu
acceptance is claimed from the isolated proofs alone. Parent frame/culling and
O2 changes are separate contributors to any combined measurement.
