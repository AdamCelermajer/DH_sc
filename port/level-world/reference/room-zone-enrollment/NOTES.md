# RoomZone enrollment boundary

This module reconstructs the source-side `RoomZone::AddInitialObject` decision
and ordered mutations, plus `GameObject::ZoneEntered` / `ZoneExited` state
leaves. It is selected into the Android library and host-tested, but no live
world session owns or calls it yet.

## Original behavior established

- `GameObject` source-plane position is the pair at `+0x160` and `+0x164`.
  They are called X/Y here to preserve the original two-dimensional query. This
audit does not assert that the second component is rendered-world Y or Z.
- `RoomZone::AddInitialObject` first asks the object's virtual `IsZonable`.
  A raw zero rejects immediately; any nonzero result proceeds.
- It reads object X once, then applies inclusive `minX <= X <= maxX`, and
  reads the second coordinate before inclusive `minY <= Y <= maxY`. The source
  `__aeabi_fcmple` comparisons reject NaN. Equality at all four bounds is
  accepted.
- The `+0x2ef` byte is a membership gate, not a test inferred from the
  `+0x2f4` pointer. A nonzero membership byte plus non-null old-zone pointer
  calls `RemoveObject` before assigning the new zone. A zero membership byte
  skips removal even if `+0x2f4` is non-null. The code then assigns `+0x2f4`,
  sets `+0x2ef`, runs the `ZoneEntered` source leaf, and appends to the target
  zone's circular object list.
- `ZoneEntered` writes `+0x2f0 = 1` first. It calls
  `VisualObject::SyncVisibility` with the pointer at `GameObject+0x2d8` only
  when `+0x2ee`, that pointer, and the visible byte `+0x80` are all set. It
  then calls virtual `IsZonable` and the vtable `+0x3c` `setUpdating` callback.
  `ZoneExited` writes `+0x2f0 = 0`; its visibility-sync gate requires `+0x2ee`
  and `+0x2d8` but not `+0x80`. The callback receives the fresh `+0x2f0` value
  when the object is zonable and zoning is enabled; otherwise it receives raw
  `1`. This preserves the distinct entry and exit gates.

The constructor defaults observed elsewhere (`+0x2ee = 1`, `+0x2f0 = 0`,
`+0x2f4 = null`) do not establish zone membership. Runtime enrollment is a
separate operation. A DACT object's authored `room` index is not the
`RoomZone*` stored at `+0x2f4`, and trigger contact is not evidence of room
membership. The runtime RoomZone XY AABB producer and its association with
authored DACT rooms are still unresolved, so this kernel must receive real
bounds from an explicit provider; it must not manufacture an in-zone flag.

## Implementation boundary

`room_zone_enrollment.cpp` uses borrowed field projections and callbacks for
virtual eligibility, old-zone removal, `VisualObject::SyncVisibility`, the
`setUpdating` callback, and target-list insertion. It preserves the original operation
order and reads fields at the points where the original body uses them. The
source trace-only debug-switch/logging calls are omitted. `RemoveObject` and
`AddObject` were executed in the ARM oracle with the list allocator boundary
instrumented; the portable callback remains the adapter boundary for owning
list storage. `SyncVisibility` and the virtual callback are intercepted
boundaries; their full VisualObject/owner effects and the RoomZone list owner
are not reconstructed here.

Callbacks may mutate the borrowed fields; later steps use those live values.
On provider failure the portable adapter returns `service_failed` and keeps
writes already performed. It does not claim transaction rollback. Separate
host-only checks cover this failure contract and are not counted as original
binary differential cases.

## Verification

Run from the `DH_sc` repository directory:

```powershell
python port/level-world/tests/run_room_zone_enrollment_host.py `
  --compiler g++ `
  --original-elf ..\test_strategy\libDungeonHunter2.so `
  --output port/level-world/build/room-zone-enrollment-host-final
```

The runner checks the original ELF SHA before execution, verifies the imported
`__aeabi_fcmple` identity, then runs the original ARM bodies under Unicorn and
compares ordered callbacks, return acceptance, and the projected state with
the C++ kernel. Current result: 22 original ARM cases, zero mismatches, plus
five host-only provider-failure cases. Cases include equality, four outside
edges, NaN, zero/nonzero IsZonable results, the two distinct `+0x2ef` paths,
mutated owner/position/bounds, and visibility-sync entry/exit gates. The report
records source and original-function hashes in `build/room-zone-enrollment-host-final/validation.json`.

The comparison proves this bounded call sequence against the pinned ELF; it
does not prove which RoomZone should own a specific game object at level
startup. Do not report runtime enrollment or full gameplay parity until the
AABB producer and owner mapping are recovered and wired.
