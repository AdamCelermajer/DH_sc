# GameObject zoning and VisualObject visibility

This bounded source unit reconstructs the complete original ARM bodies for
`GameObject::DisableZoning`, `GameObject::EnableZoning`, and
`VisualObject::SyncVisibility`. The exact original ranges, SHA-256 hashes,
supporting functions, and vtable slot evidence are in
[`original-functions.json`](original-functions.json). The source ELF is pinned
there by SHA-256.

## Recovered behavior

`DisableZoning` conditionally removes the object from its current `RoomZone`
and adds it to `ObjectManager`'s no-room collection. It clears byte `+0x2ee`
after those calls, captures the virtual target at `+0x3c`, then invokes the
current virtual `IsZonable` target at `+0xc4`. The `setUpdating` argument is
the fresh raw byte at `+0x2f0` only when the raw `IsZonable` result is nonzero
and the fresh `+0x2ee` byte is nonzero; otherwise the argument is canonical
one. The saved `+0x3c` target is used even if `IsZonable` changes the vtable
projection.

`EnableZoning` has separate already-enabled and disabled paths. On the
disabled path it first sets `+0x2ee`, removes the object from the no-room
collection, then reads `+0x2f4`. If that room pointer is non-null, it calls
`RoomZone::AddObject`, reloads `+0x2f4`, reads that room's byte `+0x389`, and
calls `ZoneEntered` for nonzero or `ZoneExited` for zero. The source does not
perform another null check after `AddObject`. Both paths then check the
current `IsZonable` target and, when zonable and the fresh visual pointer
`+0x2d8` is non-null, call `SyncVisibility`. Finally, as in
`DisableZoning`, it uses the captured `+0x3c` target and chooses the update
argument from fresh source bytes.

`SyncVisibility` resolves its owner through `VisualObject+4`; a null owner
returns without changing visibility. If owner byte `+0x80` is zero, it calls
`SetVisible(false)` immediately. Otherwise it calls the current virtual
`IsZonable`; visibility is false only when that result is nonzero, fresh
`+0x2ee` is nonzero, and fresh `+0x2f0` is zero. Every other case calls
`SetVisible(true)`.

The vtable evidence pins `ObjectBase::setUpdating(bool)` at slot `+0x3c` for
`GameObject`, `Character`, and `RoomZone`. `GameObject`, `Character`, and
`RoomZone` have distinct `IsZonable` targets at slot `+0xc4`. The target
addresses and complete vtable hashes are recorded separately in the manifest.

## Field writer and ownership evidence

The bounded writer/caller audit establishes these producers without treating
an authored room number as a runtime pointer:

* Constructors set `+0x2ee = 1` and `+0x2f0 = 0` (see the existing
  `room-zone-enrollment` source notes).
* `ZoneEntered` writes `+0x2f0 = 1`; `ZoneExited` writes `+0x2f0 = 0`.
  Those bodies have a separate source mapping and are synchronous service
  boundaries here.
* `Character::InitSpawned` at `0x3b379c` writes `+0x2f0 = 1` and the
  character spawn marker at `+0x1481`; it does not assign `+0x2f4`.
* `RoomZone::AddInitialObject` assigns the actual runtime `RoomZone*` to
  `+0x2f4`, sets its room-list byte, and invokes `ZoneEntered` when the source
  IsZonable and XY-bounds checks accept the object. The bounded implementation
  and checks are in `room_zone_enrollment`.

`RoomZone` bounds are produced from a live module's root visual bounding box.
The DACT/PFRoom room index does not identify a `RoomZone*`, and a trigger
contact does not establish room membership. This unit therefore accepts the
actual room pointer as a runtime fact and never derives it from authored room
indices or proximity.

## Port boundary and validation

The kernel owns the source branch order, byte reads/writes, target capture,
and argument selection. Calls that mutate room/no-room collections, run
`ZoneEntered`/`ZoneExited`, execute `SetVisible`, or dispatch source virtuals
remain typed synchronous services. On a provider failure, earlier source
effects remain; the port does not roll them back. The full visual scene-node
behavior inside `VisualObject::SetVisible`, source container allocators, and
native actor/renderer wiring are not claimed by this module.

Run the host and original ARM differential gate from the repository root:

```powershell
python port/level-world/tests/run_game_object_zoning_visibility_host.py `
  --original-elf ../test_strategy/libDungeonHunter2.so `
  --output port/level-world/build/game-object-zoning-visibility/host.exe `
  --report port/level-world/build/game-object-zoning-visibility/validation.json
```

The verified run executes 15 source scenarios with zero mismatches and covers
all 120 active instructions in the three bodies. Twelve host-side malformed
input, alias, full-width identity, and failure-retention guards pass. The
report is generated under the ignored build directory; no emulator or Android
build is part of this validation.
