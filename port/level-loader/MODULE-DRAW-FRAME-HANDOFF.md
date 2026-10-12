# Retained Module rendering connection

`module_draw_frame_v1.hpp/.cpp` supplies renderer submissions from the actual
retained `RetainedModuleVisualV3` and its SAME `ModuleStaticSceneV2` root. It
reads cached instance world matrices, current effective visibility and actual
mesh attachment membership after nav-mesh/helper removal. It does not rebuild
transforms with `scene::update_world`, parse another scene, create another
manager/world, decide conditions or execute animations.

`capture_module_draw_frame_v1(visual, actual_module_id, out, error)` atomically
captures a frame. Each mesh carries its source instance index, geometry index,
cached world matrix and ordered resolved materials. The frame retains the
immutable BRES resource lease. It holds no raw scene pointers or root ownership;
it remains usable for submission after the original visual/root is released.
Capturing a released visual fails and preserves the previous frame. Invisible
roots produce an empty successful frame. Unsupported skinned geometry fails
explicitly. Callers must recapture after scene changes.

Link the adapter to the existing canonical Module graph owner. In a renderer,
open each frame mesh using `dh2_mesh_open(&mesh, &frame.bres, draw.geometry)`,
use `draw.materials[p]` for primitive p and `draw.cached_world` for its matrix.
The returned material retains its actual target ID, textures and shader fields;
do not infer material targets from primitive symbols. No loader include path is
exported ahead of the owner's authoritative headers. Consumers can include the
adapter by its existing project-relative header location.

The original complete Swamp MLX test captures all nine actual Module roots.
Its current post-PF graph submits 386 attached visible mesh instances. The test
compares every submitted matrix and geometry index to the SAME scene cache,
checks hide/restore, checks released-root rejection and reads mesh payloads
through retained frame resources after visual release. Host, ASan/UBSan and
loader emulator-5590 pass; ARM64 is compiled. Reproduce using the four platform
calls to `tools/build_module_graph_source.py`, then
`tools/run_module_draw_frame_source_checks.py`.

This builds on immutable controller source checkpoint
`canonical-module-controller-source-handoff-9ace0b047294f279.zip`, SHA256
`829cfd620b4464aa7f427533e765c8bae07456a1c86cfc5ebeebeac8adaea353`.
The updated receipt records the exact adapter source hashes and build binaries.
The tests still declare their outer platform/profile/network fixtures. They
still fail the actual first MGP at OpenableContainer construction.

The visible APK has not been updated with this source-driven scene connection.
386 submissions are not a GPU-rendering or complete-level acceptance claim.
Production class services, GSLevel publication/lifecycle and authored mob/chest
rendering remain required. The goal remains the complete generic loader.
