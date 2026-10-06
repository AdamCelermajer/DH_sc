# Authored animation step FX V4 / draw V5

The production insertion snippets are now connected in the actual renderer
and both CMake targets. The latest actual renderer and eight UI/session source
dependencies pass strict compilation for both ABIs via
`.local-inputs/compile_authored_character_panel_v2.py`. They have not yet been
packaged, installed or visibly accepted in the game.

## Same-owner renderer changes

Add headers `character_combat_fx_runtime_v4.hpp`,
`character_authored_fx_forces_v4.hpp`, `character_animation_step_fx_v2.hpp`,
`character_animation_swoosh_v4.hpp`, `canonical_point3d_globals_v1.hpp`.

Replace the three declarations/ownership slots with:

```cpp
std::unique_ptr<dh2::character::skills::CharacterCombatFxRuntimeV4> combat_fx;
std::unique_ptr<dh2::fx::CharacterAuthoredParticleFxFactoryV4> combat_fx_particles;
std::unique_ptr<dh2::fx::CharacterAuthoredFxForceFactoryOwnerV4> combat_fx_forces;
```

Replace `renderer_effect_scene_v4.inc` with
`renderer_authored_effect_scene_v5.inc`. This successor includes the new draw,
program-connection and actual mixed-source adapters; it reuses the actual
texture owner, shader collection, reflection, full material directory and
source transparent heapsort. Do not include both scene owners.

Replace `renderer_combat_fx_runtime_v2.inc` with
`renderer_combat_fx_runtime_v4.inc`, followed by
`renderer_animation_sound_v4.inc` and
`renderer_player_animation_step_fx_v2.inc`.

The V4 runtime include constructs forces before resources before manager,
binds the source step callback only after manager initialization, and converts
both real source lists into ONE mixed GPU/queue submission. It uses the actual
registered player anchor, same rotation16c, source InitPost scale120 and sole
canonical raw81/raw84 fields. NPC position/rotation/life borrows work; NPC raw
81/84/scale anchor continuations remain explicitly required.

The retained PlayerSkillsRuntime destructor now clears its borrowed
`prince_locomotion.selection_fx_v2` hook before member destruction. Reload
binding clears the old hook before replacing FX
owners. The force factory must outlive resources, and resources must outlive
manager teardown: explicit reset order is manager, resources, force factory.

## Translation units

Add to level-world: `character_animation_step_fx_v2.cpp`,
`character_animation_swoosh_v4.cpp`, `source_fx_node_matrix_v4.cpp`,
`authored_fx_mesh_graph_v4.cpp`, `character_authored_particle_fx_v4.cpp`,
`character_authored_fx_forces_v4.cpp`, `character_mesh_fx_owner_v4.cpp`,
`character_combat_hit_fx_v4.cpp`, `character_combat_fx_runtime_v4.cpp`.

Add to engine-animation if absent: `particle_cloud_runtime_v3.cpp`,
`particle_deflector_v1.cpp`, `particle_force_scene_v2.cpp`,
`particle_bound_forces_v4.cpp`. Existing source emission/models/baker/math/
material/color/texture kernels remain dependencies. `actor_blended_playback.cpp`
has the additive optional selection callback; historical callers leave it null.

## Proven scope and limits

Actual BashDown and Charge cache resource test: 21,386 native checks. Whole
manager BashDown test: 1,069 checks, 107 positive mesh frames, actual timeline/
completion/pool/warm reuse; declared camera/anchor/floor fixtures. Source Swoosh
branch test: 163 ordered checks, source-ASM derived rather than an ARM oracle.
Actual material GLES test: 210 checks across two context cycles, source missing
Color0 default versus present black stream and state cleanup; quad/white-texture
fixtures, not live geometry or texture acceptance. Deflector has separate 160
bit-exact original ARM/compiled ARM64 cases and actual-cache force binding test.

The mesh GPU receiver supports unskinned one-buffer nodes and actual transparent
ProfileCOMMON passes. It explicitly rejects multi-buffer nodes, opaque mesh
queue continuations, unsupported shader profiles, missing texture/anchor/audio
providers and pure-mesh manager metadata. No skill ID, FX set or file is hardcoded
in the runtime. Actual Bash has three such mesh nodes plus debris BOX and dust
SPHERE; debris binds Deflector then Gravity, dust binds zero forces. Charge has
two mesh nodes plus a BOX cloud. Other authored families must satisfy the same
supported source contracts or fail precisely; the 45-file inventory is not a
claim of whole live support.

Original missing Color0: guessShaderVertexAttribute6dbb50 registers color0 as
semantic18; setupArrays5b6584 reads table8e018c[18]=(1,1,1,1), disables its array
and supplies the source constant. V5 preserves and restores current attribute
values. CMesh IMesh prepare slot38 is literal16 at6676b8. Source onRegister
6463c8 emits buffer ordinal+1, flags bit10000→renderkind8, NULLposition,
priorityINT_MAX. Source getRenderVertexCount64635c uses that same buffer's vertex
count. Mesh and cloud metadata never share fictitious particle identities.

## Authored skill Use timing

Actual animations over actual bundled prince model dispatch do_skill once:
BashDown clip1234 range0..1666/event700ms; Charge clip1238 range0..1066/event433;
GroundSlam clip1239 range0..1500/event566. This isolated cache test does not
unlock any skill or prove live damage. Live verification must log actual
compiled clip range/event count, source Use receipt and target HP at event time;
PreSearch nativeTargets1/state6→3 alone is insufficient.

An empty-scene MissingTargets::ignore fixture returns range0..0 while retaining
the event700ms, because all transform bindings are absent. This is not evidence
that the live scene has that defect; the real model fixture gives correct ranges.
