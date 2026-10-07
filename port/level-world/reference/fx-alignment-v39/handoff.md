# Authored skill/weapon alignment V39

## Conclusion

No new source-proven camera-angle/slope transform defect was found in the current inspected V32/V34 draw path. No rotation compensation, actor/camera movement, shader change, socket substitution or reduced effects were applied. V39 adds an explicit reusable draw-space contract and meaningful actual-cache regression coverage. It is not a declaration that all skills now visually match the reference.

Current source composition:

- Anchored AnimatedFX consumes the actual visual-root rotation (V28), actual source GetTargetPosition and scale. Unanchored authored step FX consumes source rotation16c and current source position. Source Euler(Y,-X,-Z) mapping happens at the VisualObject owner boundary; authored node quaternion tracks do not receive it again.
- Rigid authored mesh: owner outer × authored node. Skinned authored mesh: deformed source palette positions × owner outer; no second mesh-node transform.
- Particle factory: outer × emitter node initializes actual world-space particle positions. Source billboard baker offsets these world centers using camera view basis. Its draw part.world is identity. An additional outer/emitter multiplication here would be a real displacement bug.
- Equipped weapon: actual retained prince socket world × actual weapon instance local world. Selection chooses `anchor_shield_left_offset`, `anchor_weapon_right_offset`, or `anchor_weapon_left_offset` from the original mode producer. Camera does not change those matrices.
- Renderer final WVP: submitted camera projection/view × part.world, once. Packet conversion copies positions, not a second world transform.

Inspected active files: renderer_player_animation_step_fx_v2.inc; renderer_authored_fx_anchor_v4.inc; character_mesh_fx_owner_v4.cpp; character_authored_resource_v32.cpp; authored_fx_mesh_graph_v32.cpp; source_fx_node_matrix_v4.cpp; visual_skin_owner_v6.cpp; authored_fx_geometry_packet_v7.cpp; renderer_authored_effect_scene_v5.inc. V38 normal-focus forwarding integration is owned by root; its original registered-target position contract is separate from camera follow.

## New production helper

`authored_fx_alignment_v39.hpp/.cpp`:

- `authored_fx_draw_world_v39(out,space,owner,node_or_socket,error)` accepts an explicitly source-produced position-space enum. It composes only the appropriate transforms. World-particle input requires NULL owner/node, and skinned owner-local input rejects an extra node. Output is unchanged on failure.
- `authored_fx_wvp_v39(out,camera_projection_view,draw_world,error)` keeps camera separate and composes it once using the captured source matrix multiplication kernel.
- Exact enum values identify node-local, owner-local skinned, particle-world and weapon-socket-local producers. Do not infer them from asset names or appearance.

The helper is additive and not connected into the app by this package. Existing active producers already implement these contracts. Root may use it as an assertion/centralized construction boundary once actual source metadata selects the space. It is not necessary to alter working production transforms merely to use the helper. If compiled into level-world, add only `authored_fx_alignment_v39.cpp`; it depends on existing source_fx_node_matrix_v4 and engine math.

## Evidence

| Coverage | Result and limits |
|---|---|
| Original matrix operator35e998 |160 original ARM cases vs native O2 sanitized output, exact65 defined bytes including identity marker. Bytes65..67 are native normalized padding, not claimed original-defined data. |
| Actual skill resource geometry | BashDown asset38, Charge42 and GroundSlam43; actual URI/SHA checked against manifest before running. Eight owner headings, pitch/roll and nonuniform scale fixtures; authored sampled geometry and camera orientation changes. Actual resource/animation/material/force producers run. No actor script/skill activation/GPU/video proof. |
| Actual weapon/socket geometry | Actual prince_modular + prince_walk_1hand animation, actual longsword01 and twohandssword01; two slots × three original attachment modes × eight heading/slope fixtures. PASS2828 checks /96 actual weapon parts. Current scene/animation/selection/skin owner sources compiled coherently. |
| Camera-independent mesh/trail | Geometry and owner/node matrices retained exactly across camera-only changes in current source fixture. This is correct mesh behavior, not proof that every named billboard mesh has the appropriate unrecovered source node subtype. |
| Particle world-space draw | Identity part.world checked; camera billboard offsets leave same world centers at same source time. No inferred fixed facing or arbitrary world normal. |
| Required failures | Extra outer on world particles, repeated node on skinned positions and nonfinite/missing required matrices fail explicitly. No silent FX omission. |
| Compiler | New production helper strict ARM64 and x86_64 PASS. No root app build/install. |

The first standalone weapon attempt mixed a historical Scene::Material producer DSO with its current extended header and ASAN rejected the resulting invalid fixture allocation. The runner was corrected to compile current scene/animation sources. This is a fixture ABI mismatch, not evidence of an app/native leak. Its failed run was not used as acceptance.

## Remaining checklist boundaries

Live guarded verification still needed: same selected target moving/retarget/death, actual targeted and area/directional casts, actual learned skill scripts and authored Use timing, camera angles/slopes and source weapon equipment changes, source FX phase/visibility and render-state parity. Planar source sword mesh can project thinly at some camera angles; that alone does not justify changing source orientation. A mesh called `billboard` is not sufficient proof of a dynamic billboard receiver. Camera follow/focus is a distinct system.

All45 assets parsing and these three actual resource tests do not establish all86 skills. No force unlock, life/RNG edit, target replacement, forced player count/Level phase or framebuffer/emulator operation occurred.
