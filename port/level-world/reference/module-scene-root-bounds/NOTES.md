# Module scene-root bounds producer

This adapter supplies the bounds that `Module::InitPost` passes to the
RoomZone initializer for the verified static SWAMP module scenes. It does not
construct or call the original engine objects.

## Source placement chain

The pinned original ELF is `libDungeonHunter2.so`, SHA-256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. The
reproducible runner checks function symbols, sizes, full function-byte hashes,
the `RootSceneNode` vtable address point and its `+0x30`, `+0x34`, and `+0xa0`
slots, plus the callsite byte sequences in
[`original-functions.json`](original-functions.json).

The relevant source edge is specific to the selected scene-node path:

1. `VisualObject` calls `AssetManager::loadSceneNode` with the selected node
   string. `loadSceneNode` converts that non-null node argument to the reset
   flag before calling `SceneManager::LoadScene`.
2. With a selected node, `SceneManager::LoadScene` calls
   `RootSceneNode::ResetPositionFromFile`. It resets the loaded first child to
   zero position, identity rotation and unit scale, and resets the outer
   `RootSceneNode` position to zero before refreshing its absolute transform.
3. `VisualObject::SyncPosition` passes its owner `GameObject+0x160` XYZ to
   `VisualObject::SetPosition`, which positions the scene root. The module
   loader selects the BRES root using `xrefobject + "-node"`; its root
   translation is therefore a catalogue origin that must be replaced by the
   owner position when reproducing the loaded selected scene.
4. `Module::InitPost` reads `Module+0x2d8` (`VisualObject`), then the root at
   `VisualObject+8`, and calls the root vtable's `+0x34` transformed-bounds
   method. The verified Itanium address point is `_ZTV13RootSceneNode+0x1c`;
   `+0x34` resolves to `ISceneNode::getTransformedBoundingBox` at `0x59770c`.

The adapter uses the actual selected module root and subtree from
`dh2_world_bind_module` / `dh2_world_module_records`. For a supplied live owner
position it changes the catalogue-root translation by
`owner_position - catalogue_origin`, then applies the existing
`dh2_floor_transform_bounds` kernel to each mesh local box. This is the same
composition as resetting the selected root to identity and positioning its
outer RootSceneNode at the owner. It does not apply the owner translation a
second time.

The local mesh boxes come from each type-0 BRES `SMesh` record's serialized
minimum/maximum XYZ. The host test compares those bounds with the actual
position attribute streams for every primitive in every selected mesh, then
checks that all owner-placed vertices from the nine selected module subtrees
lie inside the transformed aggregate. Each of the nine explicit owner
translation probes moves the result by exactly its X/Y/Z offset once.

## Verification

Run from `work/DH_sc`:

```powershell
python port/level-world/tests/run_module_scene_root_bounds.py --cache ../cache/files --cxx g++
```

The checked cache run passed for all nine `001_swamp.mlx` module placements.
It resolved 12–108 selected subtree scene nodes, 9–59 geometry instances and
9–62 primitive buffers per module. The generated aggregate bounds match the
existing independent all-module world-vertex extents within 0.03 source units.
The machine report is written to ignored
`port/level-world/build/module-scene-root-bounds-host/validation.json`.

## Boundary

The checked corpus contains static type-3 geometry for these module scenes.
The adapter counts and skips other instance types; the host fixture requires
that count to remain zero. It does not claim original runtime-object or
Irrlicht execution equivalence. It also does not create RoomZone objects or
establish module/object room membership. Those require the live module owner,
the source RoomZone owner identity, its room-list links and source enrollment
producer; DACT record indices are not room membership evidence.
