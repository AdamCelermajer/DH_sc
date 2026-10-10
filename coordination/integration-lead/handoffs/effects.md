Effects worker checkpoint for exclusive GPT-6 Luna HIGH successor.

Owned implementation: `port/windows-foundation/features/effects/` and
`reports/feature-effects.json`. Root main/renderer/core/CMake belong to root.
This checkpoint additionally writes this requested handoff. No commits made.

Implemented `EffectsExecutor` borrows `CharacterMeshFxOwnerV4`: original fx_
name lookup, actual AnimationStep FX kernel, source Swoosh gate, explicit selected
set/actor/socket, separate source absolute/App dt phases, pool reuse, release.
`RetainedEffectsAdapter` consumes the same retained audiovisual batch after
actor event0x28 and forwarding, with token actor/clip/slot/generation/wall/lag/
ordinal. It does not suppress a new frame/loop batch in the same generation.
Audio worker is coordinated: original Swoosh prefix executes once, returning
both fallback decisions; do not rerun it independently from audio and FX.

`EffectsRenderBridge` emits root Mesh/Mat4 from source baked/skinned/billboard
positions, exact indices and already baked UV/color. Source provenance and frame
lease reach root through EffectRenderServices.material/submit callbacks.
`OriginalEffectMaterialBinding` resolves the actual selected COMMON unlit pass,
decodes original textures and delegates root GPU upload/release. Other shader/
lighting/separate-alpha/richer-state branches reject explicitly. No generated
billboards, textures, glow, recoloring or fades. `EffectsAnchorBinding` is only an
identity/lifetime/read callback registry, delegates detach to the same manager.

Root landed `CharacterVisual::retained_scene_borrow() const noexcept`, returning
the actual loaded impl_->scene. Borrow expires on load/move/destruction; drain
render loans and release manager/anchors before replacing it. Native test
`effects_scene_borrow_tests.cpp` passes 29 checks: actual Prince35-node scene,
posed source right-hand attachment changes without a second clock, repeated
actual field reads, lifetime detach fixture, failed reload preservation.
It also proves original faery rigid glow geometry8 vertices/12 indices and
three embedded particle cloud nodes remain on the SAME CharacterVisual Scene.
No cloned scene/pose added to runtime.

Faery source finding: actual `faeries_02_celeste.bdae` has rigid
`_mesh_glow_nobatch-node_PIVOT` geometry1 using material `fx_particles_additive`
and original `atlas_fx_particles_001.tga`; nodes GL_PCloud04/06/07 and animated
bip_faerie_GLOW are embedded actor content. No separate named EffectsTables
FaeryGlow set is established. `include_static_instances=true` reaches the
original rigid glow through CharacterVisual (native tested). Embedded clouds
still need a receiver borrowing this SAME actor scene/pose/source clip time.
General V32 factory ignores its scene argument and creates a resource-owned
scene; using it on the entire actor would create a forbidden second pose.

Validation: old source-domain audit passes 71,630 checks, 73 set bindings/61
distinct original resources, 7,636 mesh/2,235 particle draw sources, ASan/UBSan.
It originally exposed old host snapshot Node ABI mixing; current scene.cpp,
skinning.cpp, animation.cpp and component_applicator.cpp are rebuilt into test.
Only actual skill_dh2_prince_rogue_vicious_strike.bdae reports no authored nodes;
the different actual skill_prince_rogue_viciousstrike asset passes.

Current final render-stream test is running via exec session7853, command:
`.local-inputs/ida-ghidra-review/venv/Scripts/python.exe
port/windows-foundation/features/effects/run_tests.py --render`.
All new bridge/material/anchor files and the native Renderer smoke compile with
Windows -Wall -Wextra -Werror. First WSL build found missing <algorithm> in
effects_material_binding.cpp; fixed. Final result will be appended below.
This test uses actual sets164/150/160/253/254/255, real texture decoder and source
pass resolver, exact stream comparisons, no-tick submission, retained marker
batch tokens, texture cleanup. Emits `.local-inputs/windows-effects-feature/
render-fixture.bin` for the compiled native WGL test executable.

Native WGL test binary is already compiled at `.local-inputs/windows-effects-
feature/effects-native-renderer-tests.exe`. After a PASS render fixture exists,
run it with fixture path and output PPM path. It uploads original decoded RGBA
through root Renderer, draws exact source streams/Mat4/pass and counts actual
framebuffer changes. Camera is diagnostic framing; this is NOT live campaign or
source scene-wide ordering acceptance. Root queue/source callback remains to be
bound; frame loans must survive through flush but drain before source unload.

FINAL TOOL-SAFE CHECKPOINT: ASan/UBSan render-stream cell7853 finished PASS:
14,909 checks, six actual skill/swoosh sets,185 packets,6,372 exact vertices,
14,424 exact indices,six original decoded texture uploads,12 retained events.
Native WGL smoke had already completed before lead's later stop-before-WGL
message arrived: PASS,3 actual packets/5 original textures,108 vertices/294
indices,2,030 changed framebuffer pixels,no GL error. Original framebuffer
receipt `.local-inputs/windows-effects-feature/native-fx-frame.png` inspected;
this is a diagnostic source effect frame, not a campaign acceptance screenshot.
Report now contains render_stream_validation,same_actor_scene_native_validation,
faery_source_continuation,native_renderer_smoke and worker_state stopped.
All tool cells/processes owned by this worker are finished. No pending WGL run.
Worker stopped; successor may exclusively edit effects files/report.

Immersion evidence sent video_fidelity and lead: F_ApplyResult0x3b1648/3b1cd0
routes skill_apply_hit_fx_v6 through character_combat_hit_fx_v4, category+124 or
cached resolved[7] CharacterEffects blood/death row, actual target position and
Character rotation+16c before same manager unanchored PlayAnimFXSet. Source
_SetAnimStep0x3ca8e4 selects actual AnimationStep.fx+anchor_fx; original rows
BashDown164/ColdRay150/JumpKick160. CharAI _OnAnimEvent0x3d4434 does fx_ lookup.
No new full-screen flash/freeze/hitpause is proven by these selected kernels.
Camera shake source investigation belongs to cinematic/camera worker.

Next bounded task after current tools finish: integrate these callbacks with
root's real actor/scene/clock and Renderer texture context, compile a native FX
backend from current recovered sources (sources in run_tests.py), and run an
actual campaign FX draw receipt with original queue ordering. Keep original
App-owned VisualFxManagerLibrariesV63/pools; do not substitute a manager/FSM.
Then implement embedded faery particle receivers borrowing actual actor Scene,
not a second factory instance. Current renderer alphaReference heuristic even
with sourcePass was reported to lead; exact authored0 must stay0.
