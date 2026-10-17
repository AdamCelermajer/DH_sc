# Crypt `GhostAmbush01` trigger contact

This host-only adapter reconstructs the direct Crypt ambush trigger's source
geometry and ordinary one-player activation path. It is separate from the
four-template `GhostAmbushHallway` and does not replace the Irrlicht renderer.

## Authored trigger and canonical layout fixture

The original cached
`data/3d/modules/crypt/mgp/crypt_straight_c_ns_01.mgp` record is
`_prim_TriggerZone_GhostAmbush01`, `gametype=TriggerZone`,
`script=GhostAmbush01`, `triggercount=1`, and `triggerdelay=0`. The move-out,
all-player, all-player-move-out, and one-player-effect fields are empty, and the
record has no door binding or dimensions override. `Zone::DeclareProperties`
registers inherited dimensions `(200,200,200)`.

The cache's authored `x07_crypt_backup.mlx` places its matching module at
`(0,19200,0)`, unit scale, and zero rotation. That gives a *fixture* trigger
center of `(-1405.23,19500.262,608.062)`. It is not asserted as the generated
level's current position: runtime layouts must pass the final GameObject
position and scale resolved by their loader.

`Zone::InitPost` at `0x39771c` scales the dimensions and forms relative
half-extents. Its virtual `SetRelativeAABB` call reaches
`GameObject::SetRelativeAABB` at `0x38b110`, which stores those six relative
values and calls `GameObject::UpdateAbsoluteAABB` at `0x38aac8`. That routine
adds the GameObject position componentwise into absolute bounds
`+0x12c..+0x140`; it does not rotate the endpoints. The Crypt trigger's extents
are all above the small-box padding threshold in `SetRelativeAABB`, so the
observed path preserves the computed dimensions exactly. With the x07 fixture
transform, the absolute bounds are:

| Axis | Absolute bounds |
|---|---:|
| X | `[-1911.319, -899.141]` |
| Y | `[19356.985, 19643.539]` |
| Z | `[508.062, 708.062]` |

The module API computes the local box from the resolved object scale, then
translates it by the resolved owner world position. It does not infer the
generated module position, parent rotation, collision selector, or scene node.

## Actual TriggerZone contact path

The direct call chain in the original engine is:

1. `TriggerZone::Update` (`0x39b458`) calls `Trigger::GetNumPlayerActivating`
   (`0x3987f8`).
2. That thunk reaches `GameObject::GetNumPlayerTouching` (`0x38b6d4`), which
   loops the player Characters, skips null Character objects, and calls
   `GameObject::IsTouching(GameObject const*)` (`0x38b518`).
3. `IsTouching` compares the two objects' **absolute AABBs** on all three axes
   with inclusive faces. It does not test actor center position or call
   `Zone::IsInside`.

Therefore this trigger adapter accepts caller-provided, already-updated
absolute Character AABBs and an explicit `has_character` flag. It follows the
ordinary `script` path with one offline player. It does not require the
trigger's `_colzone` visual node or triangle selector.

`Zone::IsInside` (`0x3970c8`) remains a distinct recovered predicate. Its null
PhysicalObject guard is preserved by the existing
`port/zone-contact-runtime/zone_geometry.cpp` projection and is checked in the
host fixture. That guard is not added to `TriggerZone::Update`, whose contact
call chain uses AABB overlap instead.

Before contact sampling, `TriggerZone::Update` checks the local player's
scripted flag, the optional associated-door gate, `Trigger::CanActivate`
(`0x3987ac`), and `GameObject::MeetCondition` (`0x38ab60`, which returns true
in the recovered body). `CanActivate` gates the configured activation count,
timer, and enabled flag. The authored record has no associated door, count one,
and delay zero. Online-only behavior is rejected as unsupported by this
offline slice. The ordinary script fields select the one-player path; other
trigger script/effect modes are outside the adapter.

The `qualifying_contact` latch models the one-player rising edge. Once the
first edge starts the script, count one causes `CanActivate` to return before
later frames sample contact. As in the source, a blocked early return leaves
the prior latch unchanged. `Trigger::Activate` increments the count, while
`TriggerZone::SafeStartScriptOnlyOnce` (`0x39b2a0`) is represented through the
checked scheduler's `enter_trigger()` path.

## Implementation and validation

`port/level-world/crypt_spawn_trigger.{hpp,cpp}` exposes
`make_world_bounds()` and `update()`. The latter validates the trigger name,
source script ID, and one-shot count, then delegates the proven offline gates
and edge handling to `port/trigger-contact/trigger_contact.cpp`.

The cache-backed host runner checks the original Crypt MGP and x07 layout,
decodes both original common and `007_crypt_01` PyData tables, and starts the
actual `GhostAmbush01` script (local index 2, global ID 17) on first contact.
It checks bounds, a miss, the null-Character skip, inclusive AABB contact,
one-shot edge/count behavior, and the separate Zone PhysicalObject guard:

```powershell
python port/level-world/tests/run_crypt_spawn_trigger_host.py
```

The report and executable are written to
`port/level-world/build/crypt-spawn-trigger/`. This proves the host adapter and
checked scheduler activation against the original table bytes. It does not by
itself prove Android integration, renderer/physics update order, or full
Crypt-level completion. Root's runtime integration must pass the live
GameObject world position/scale and Character absolute AABB after the native
world update. The separate `GhostAmbushHallway` four-template route, online
activation, other player counts, and trigger modes remain outside this slice.

## Original source functions

The pinned native function ranges, hashes, bounded adapter mappings, and
unsupported calls are in `original-functions.json`. The engine file is
identified by SHA-256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
