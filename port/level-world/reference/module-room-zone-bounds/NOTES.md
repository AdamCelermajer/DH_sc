# Module-created RoomZone bounds

This bounded producer implements the `RoomZone` path through the original
`Zone::InitWithBoundingBox` ARM function. It is a small source kernel plus a
host test that executes the pinned ARM routine as an oracle.

## Original call chain

`Module::InitPost` (`0x388b20`, 312 bytes) reads its visual-object pointer from
`Module+0x2d8`, then the scene root from `VisualObject+8`. It calls the root's
vtable slot `+0x34` for the box pointer. The pinned ELF constructor and
relocations establish the root vptr address point as `_ZTV13RootSceneNode+0x1c`;
therefore slot `+0x34` is `ISceneNode::getTransformedBoundingBox()` at `0x59770c`.
That method obtains the root-local box through vptr slot `+0x30`, whose
`CEmptySceneNode::getBoundingBox() const` body (`0x5839b0`, 8 bytes) returns
`this+0x130`; it copies the six faces to `+0xd4` and transforms the box through
the scene node's absolute matrix at `+0x24` when the cached result is dirty.
So the input to `Zone::InitWithBoundingBox` is the engine's transformed
scene-root bounds, not an explicit module XML box.

`RootSceneNode::RefreshBoundingBox` (`0x35c854`, 156 bytes) calls the aggregate
scene-node bounding computation, then uses vptr slot `+0xa0` to get the root
position (`ISceneNode::getPosition`, `0x597124`, returning `this+0xac`) and
subtracts it from all six root-local faces. The absolute matrix used by
`getTransformedBoundingBox` supplies the next transform. This traces the
geometry-to-scene-node box production, but does not prove whether the
scene-node absolute transform has already incorporated the MLX module
placement or how that placement is bound to the visual object.

If the spawned object has source kind `0xb`, `Module::InitPost` passes the
transformed box to `Zone::InitWithBoundingBox` (`0x397594`) and writes the parent
`Module*` to the resulting zone's `+0x38c`. This is the runtime producer: no
separate AABB or RoomZone record is present in the module XML.

The authored `001_swamp.mlx` lists nine `Module` instances. Each names a DAE,
an MGP, an MVP, and a module placement. Module zero is
`obj_4of4_brdwalk_sw_00_0` at `(0,0,0)`, using `swamp.bdae`,
`obj_4of4_brdwalk_sw_00.mgp`, and `obj_4of4_brdwalk_sw_00.mvp`. Those records
describe geometry and child objects; they do not store the six-float box passed
to the constructor. The exact live box depends on the original scene-root
bounds producer.

The MLX module placement is a separate input from its DAE/MGP/MVP child data.
Although the bounding-box getter transforms by the scene node's absolute
matrix, the current evidence does not establish whether that absolute matrix
already contains the top-level MLX module placement, which rendered children
contribute to the scene-root aggregate, or how the original object manager
binds a Module instance to the visual root. Do not apply the MLX position a
second time or infer a Module-to-RoomZone owner from a DACT room index.

## `Zone::InitWithBoundingBox` behavior

`RoomZone::RoomZone` (`0x396558`) calls `Zone::Zone(kind, false, true)`.
`Zone::Zone` (`0x397ca0`) copies the first boolean to `+0x380`, so the optional
visual/debug branch after the bounds initialization is disabled for this
producer.

The bounded body performs these effects in source order:

1. `maxX-minX`, `maxY-minY`, `maxZ-minZ`, stored to dimensions `+0x374`,
   `+0x378`, `+0x37c` (the writes occur Y, Z, X after the three arithmetic
   calls).
2. Copies the raw minimum and maximum triples to relative faces `+0x144` and
   `+0x150`.
3. Computes `(min+max)*0.5` on each axis and calls
   `GameObject::SetPosition(center, true)` (`0x393db4`).

`SetPosition` is represented here as an owner callback. The original function
identity, object argument, center vector, and `true` flag are captured by the
ARM harness. Its internal position, absolute-AABB, and optional physical or
visual-object effects are deliberately not claimed by this kernel.

## Verification

From `work/DH_sc`:

```powershell
python port/level-world/tests/run_module_room_zone_bounds.py `
  --compiler C:\Users\noamc\.local\mingw\mingw64\bin\g++.exe
```

The checked run used the original ELF SHA-256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80` and
compared 105 deterministic/seeded float-box cases with zero mismatches.
Arithmetic helper identity and call order were checked as three
`__aeabi_fsub` calls followed by three `__aeabi_fadd`/`__aeabi_fmul` pairs.
The `SetPosition` call was intercepted at the exact ELF entry `0x393db4` and
asserted to receive `r2=1`.

The runner writes `build/module-room-zone-bounds-host/validation.json`.
RoomZone pointer creation/handle resolution, RoomZone-to-native-level-room
identity, full RootSceneNode bound production, coordinate composition,
`SetPosition` implementation, and native actor enrollment remain separate
integration work. The generic `Zone::InitPost` scaled-dimensions path is not
part of this kernel.
