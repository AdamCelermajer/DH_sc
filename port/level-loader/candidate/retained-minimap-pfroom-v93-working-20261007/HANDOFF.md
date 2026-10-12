# PFRoom positive minimap resource V93 — source only

Candidate: C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/candidate/retained-minimap-pfroom-v93-working-20261007

`minimap-pfroom-resource-v93.patch` is one coherent delta against current Root PF/Module/Map APIs and the pending V91 Scene/child/Visual ABI. It does not overwrite shared Root or active private selected files. Preserve concurrent unrelated changes when Main adopts the narrow additions.

New `retained_map_mesh_v93.hpp/.cpp` lends SceneMapNodeBorrowV2 over a genuine source-created RetainedMeshNodeV91. Its owner is the independent actual mesh resource and SAME InstanceStorageV91 cells. It retains the real immutable source resource independently for geometry/material-index projection; that BRES pin never substitutes for node ownership. Original parent, Scene and map authority remain weak. Link ONE new CPP into the existing level-world owner beside V91, not another Scene/World/PF target.

Source producer changes:

- `ModuleSelectedSceneV2` carries the SAME already decoded BRES view. Its actual RetainedModuleVisualV3 producer already pins the original source byte block before ModuleStaticScene.initialize; no new file read/cache exists.
- `ModuleStaticSceneV2` owns real V91 node/mesh resources at its existing constructor path. Its old inline flag/TRS/detach arrays become views over the SAME source-created Node/Instance cells. Scene cloning still deep-copies storage. Existing TRS/cache/OptimizeStatic controls remain; no second C1/visual/floor record is constructed.
- Generic `RetainedGameObjectVisualV1` lends its existing V91 mesh receivers, with stable native mesh pins so repeated adds borrow the same child after its previous parent unlink. It creates no ModuleStaticScene alternative.
- SceneMapNodeBorrowV2 adds the exact typed `source_mesh_v93` receipt. Map owner `source_children_v93()` exposes existing retained entries for the actual renderer/resource consumer; no copied scene is supplied.

Actual source AddNodeToMap3524a0 remains the SAME existing SceneManagerMapOwnerV2 algorithm. New real child methods lend actual parentec, flags11c, cached absolute position, source setPosition, old-parent detach, original single static mesh OptimizeStatic, parent visibility and scene-phase/cache delivery. The original source owner survives root/visual storage destruction after transfer. Callbacks retain the child only and weak map; no containing World, actor or Visual/root ownership cycle.

Original PFWorld.LoadRoom523c14 order is preserved:

1. Publish the SAME freshly constructed PFRoom, capture SearchByType's stable attached mesh indices INCLUDING hidden meshes, and capture the LAST original minimap-name match.
2. For each reached floor, complete existing PFRoom._LoadFloor521eb8: same record/geometry, actual floor clone, original hide/removal, nav graph and first clone map add.
3. Only after that completion does523e00 select the captured positive minimap mesh at523e24. It now borrows the SAME native mesh and performs actual map transfer. Each reached floor repeats this same-node reparent, matching source.
4. With no captured minimap,523eb4..ed4 reads this room's CURRENT LAST freshly copied floor mesh. It performs the second map add after EACH reached floor, not a room-loop-only final add. Existing ModuleFloorCloneV3 storage/body/release order is unchanged.
5. A room with no floors takes the existing original NULL-room branch; a minimap alone does not transfer or publish a nonNULL room.

Current names are read from the actual native mesh name/parent fields at both original lookup sites. The source mesh C1-empty name falls back to its current parent name; after minimap transfer that parent is the SAME actual CSceneNode map group with its real empty CString, not the old recorded scene-index parent. Own-name exit operands and parent-name operands remain distinct. No XML or global registry is rescanned.

Ownership after transfer:

- Actual old-parent membership is erased, original visual membership is marked detached, and parentec names the real map group after source publication.
- Old Scene/Generic Visual/Module root matrix and visibility delivery skip that detached child. Its live map cache and visibility are not overwritten through stale original vector indices.
- `minimap_prefix_v93/exit_prefix_v93` retain the actual reached child receipt when a transfer/removal fails. Success clears only the extra room prefix after the genuine map owner accepted the child. Room/floor/clone/PF/map prefixes remain retained on failure.
- Existing map remove/release and qualified floor-clone retirement remain the native lifecycle entry points. No passive Visual/base/BRES-byte alias claims to execute native node teardown, and no new floor/PF authority or release order is introduced.

Production includes/dependencies: V91 scene.hpp +retained_visual_child_v91.hpp/CPP +generic Visual pair must be adopted coherently; this batch adds their typed parent/name/resource receipt fields. Module selected/static scene HPP/CPP, ModulePFRoom HPP/CPP, SceneManagerMap HPP, plain Scene.cpp and GameObjectSceneBinding CPP are surgical successors. Public vector views change ModuleStaticScene's accessor ABI, so dependent production TUs need the ONE coherent milestone rebuild. The original V91 same-storage assignment guard correction is retained.

Supported positive receiver domain is the existing actual static triangle MeshSceneNode resource domain, independent of level/map name. Actual skinned minimap children or malformed/unsupported geometry layouts fail explicitly; no alternative/fake minimap node is supplied. Existing manual inspection graphs keep their historical constructor API but do not advertise a native positive map receiver without genuine resource ownership. No generic all-map runtime success or GPU draw readiness is claimed by source composition.

Main must feed the existing actual map renderer/resource registration from `SceneManagerMapOwnerV2.source_children_v93()` and the typed SAME child receipt. It has actual node fields, current matrix, source material indices, owned geometry and independently retained original source bytes; no second Scene or shader owner is needed. Main owns the existing coordinated native unload/Scene.Clear and GPU lifecycle gates.

No tests, compilation, syntax/build/apply checks, source audit jobs, hashes, fixture growth, emulators or runtime/native jobs were run. Main adoption and BIG integrated milestone validation remain pending.
