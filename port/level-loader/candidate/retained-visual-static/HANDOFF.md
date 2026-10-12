# Retained Visual static/hidden graph integration

Source-only candidate for the first SWAMP milestone. No compilation, test, runtime, hash, or freeze was performed. Shared Root remains untouched. Adopt the seven files together or apply retained-visual-static.patch to their matching current Root sources. Rebuild every affected owner/consumer coherently at the integrated milestone: the retained class storage changes and authored_scene_subtree_v2 has paired optional visibility arguments.

## Source files

- port/scene-materials/scene.hpp and scene.cpp
- port/level-world/retained_gameobject_visual_v1.hpp and .cpp
- port/level-world/authored_scene_subtree_v2.hpp and .cpp
- port/level-world/retained_visual_draw_v27.cpp

Existing scene::load remains the filtered API. load_authored_v76 uses the SAME Loader/parser to construct ONE retained Scene with every authored mesh child, including hidden ones, plus local/effective node/mesh visibility sidecars. The existing xref selection/reset control remaps the sidecar and recomputes effective visibility relative to the selected subtree.

RetainedGameObjectVisualV1 now follows captured SetParent47295c: dynamic parents Sync then recursively enable bit200; static parents call actual IsAnimated first, then Sync, and either enable200 or run OptimizeStatic50f220. Static bake uses the existing ModuleStaticSceneV2 source control on this SAME graph/cache and geometry children, without constructing a ModuleStaticScene owner. Position and quaternion come from cached world matrices; geometry child local scale remains C1 identity. Later root Sync/position-only updates preserve already-baked child caches. Direct animation sampling on a baked graph fails with the required explicit source reparent/update boundary; the sole V69 scene clock is preserved and already skips roots with200 clear.

## Parent PF adapter interface

- source_mesh_pose_v76(unsigned, std::array<float,4>& quaternion, std::array<float,3>& scale, std::string&) const
- mesh_attached_v76(unsigned) const noexcept
- node_attached_v76(unsigned) const noexcept
- mesh_visible_v76(unsigned) const noexcept
- remove_floor_mesh_v76(unsigned, std::string&)
- remove_node_v76(unsigned, std::string&)
- resource_owner_v76() const noexcept -> std::shared_ptr<void>

Indices remain original graph/instance indices. Detach clears actual membership/effective visibility and draw eligibility without erasing/reindexing the source graph, caches or immutable geometry. Node name lookup skips detached nodes. Existing held node handles can still read cached positions for authentic search snapshots. Mesh hide/parent visibility changes reach the SAME mesh fields used by rendering; permanently detached meshes cannot become visible again. The renderer binds attached meshes and refreshes using mesh_visible_v76, retaining existing geometry, skinning, material and shader paths.

resource_owner_v76 pins the independent actual read-byte block. read_asset fills that shared vector directly: no second byte buffer/copy. BRES/model data and animation load read the same data/size. Visual release destroys its own animation/scene borrows before dropping its resource ownership; existing PF clone pins preserve the original block independently, without retaining Visual, base C1, World, Level, or a manager. Parent PF must put this resource_owner_v76 into its existing sourceNativeMesh/floor clone ownership, rather than using a whole Visual alias.

## Preserved ownership and remaining integration

Existing actual register_root, force_register, release_root, parent_is_animated and UpdatePF leaves remain required. No new Main leaf or replacement world/cache/visual was added. generic_callbacks_v69, Character animator replacement, source_scene_phase_v69, skinning and current Root material/renderer behavior are retained. Parent owns its SAME-floor-world PFRooms retained-visual overload.

Existing unsupported domains remain explicit: external BRES scene/geometry/material bindings, unsupported controller/source instance tags, and nonempty-xref multi-visual-scene lookup. They are not broadened by this batch. Static reparent/re-enable animation after baking requires its genuine original producer. No claim of SWAMP draw/runtime success is made; admission remains the BIG integrated menu-to-selected-saved-Character-to-native-SWAMP movement/camera/HUD/profile-menu milestone.

Source basis: retained reference/visual-root-policy/0047295c.asm and visual-root-construction/0035d824.asm; existing Root ModuleStaticSceneV2 optimize_static/update_absolute/mesh C1/removal source bodies. SetParent proof includes actual static84 branch and IsAnimated virtual80 before Sync, followed by source root getPositionA0/setPositionA4 and OptimizeStatic.
