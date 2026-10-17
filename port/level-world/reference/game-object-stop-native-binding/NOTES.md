# Native ownership plan for GameObject::Stop

This note maps the frozen `game_object_stop` caller onto the retained native
Crypt actor owners. It is a read-only integration plan: it adds no callbacks,
APK wiring, or live Stop behavior. The source proof is
[`game-object-stop`](../game-object-stop/NOTES.md); native owners are in
`port/android-native/app/src/main/cpp/model_renderer.cpp` and their portable
backends are under `port/level-world`.

## Stable owner mapping

For a live gated Character, `ObjectActor::spawn_owner` is a `shared_ptr` to the
stable `SpawnOwner`. Its `character` owns the current `Coordinator::state`;
`runtime` owns the logical GameObject fields and `PathObject`; `route_storage`
owns the segment array borrowed by `runtime.path`; `body` owns the current
`NativeBody` wrapper; and `body_owner` owns the `WorldObject` callback context
used by the Box2D body and shapes. `NativeMonsterInitialization::owner` also
retains the same `SpawnOwner` while the native VM is alive. The source actor ID
is the full-width `ObjectActor::identity`.

Build each `game_object_stop::State` at the stop call from these live owners:

| Stop projection | Native owner |
| --- | --- |
| `object_identity` | `ObjectActor::identity` |
| `path_identity` | identity of this owner's `runtime.path` (`GameObject + 0x1c8` projection) |
| `physical_identity` | stable identity of this owner's physical wrapper, only when `body.body` exists (`GameObject + 0x2dc` projection) |
| `position`, destination, heading, moving flags | live `runtime.subobjects` fields and the corresponding source Character/controller state |
| virtual `+0x64` result | `character_physics_position` over `character.state.flags`, with the same full-width actor identity |

The portable Stop adapter is a typed projection, not a binary overlay. Resolve
every subject identity against the selected `SpawnOwner`; reject an identity
that names another actor or a stale body. Resolve `NativeBody` anew for each
physical callback because the wrapper can point at a replacement `b2Body`.
The final `resolve_body` callback must return this owner's `runtime.body` only
when the physical identity still matches.

## Callback sequence and backends

`dh2_game_object_stop` already owns the recovered source sequence. Bind its
synchronous services to these exact resources:

1. `drop_path`: validate the owner and path identity, then call
   `dh2_nav_drop_path(&owner.runtime.path)`. This clears the owned route head
   using the existing navigation implementation. The segment vector remains
   alive and at the same address for the full `PathObject` lifetime; Stop is a
   route clear, not a storage teardown.
2. `is_updating_position_from_physics`: construct a fresh
   `character_physics_position::CharacterView` from the live Character identity
   and `&owner.character.state.flags`, then call the verified Character
   override adapter. The Crypt Ghost's source class is `Character`; the base
   GameObject return value is not its policy.
3. `set_linear_velocity`: call `dh2_native_body_set_linear` with the requested
   zero XY on the current `owner.body`, then refresh the logical
   `owner.runtime.body` view if subsequent source observers require it.
4. `set_angular_velocity`: call `dh2_native_body_set_angular` with zero on the
   current body, then refresh the logical view as needed.
5. `set_position`: call `dh2_native_body_set_position` with the live GameObject
   XY from the request. That API converts game units to Box2D units (`* 0.01`),
   retains the body's current angle and applies `SetXForm`. Source Stop ignores
   the transform's frozen/out-of-world boolean; the adapter should distinguish
   that ordinary false result from malformed/stale owner failure.
6. `resolve_body` runs after the setters and rechecks the physical identity.
   Then apply the original final body reset to the genuine backend and logical
   view. `dh2_physical_stop_finish` resets only the portable `BodyState`; it
   does not sleep or clear the Box2D body. The pinned Box2D `b2Body::PutToSleep`
   writes sleep flag/time and clears linear/angular velocity, force and torque,
   matching the final field effects used by the existing fused
   `dh2_native_body_stop` adapter.

Do not call the fused `dh2_native_body_stop` from the `set_position` callback:
it repeats the linear and angular setters and sleeps immediately, collapsing
the separately observed source callback sequence. The existing fused API
remains appropriate for the separate path-controller physical-stop boundary in
`actor_runtime.cpp`; it is not a substitute for individually bound services
in `dh2_game_object_stop`.

`dh2_native_body_refresh_view` can copy Box2D's public position, angle,
velocities, radius and selected flags. It cannot observe the backend's force,
torque or sleep-time fields, so those fields in `BodyState` must not be
presented as queried native values. After the final backend sleep/reset, call
`dh2_physical_stop_finish` to synchronize that logical projection.

## Lifetime and frame integration gates

- Keep `SpawnOwner`, its nested `BodyOwner`, the Box2D world, and all native
  callback contexts alive through every Stop callback. The Box2D `userData`
  points at `BodyOwner`; contact/query callbacks must never retain a temporary
  wrapper or a moved `SpawnOwner` address.
- `route_storage` is borrowed by `runtime.path.segments`. Do not resize or clear
  it during Stop. The world teardown currently destroys/clears world ownership
  and then invalidates body/runtime pointers before clearing route storage;
  Stop must be unavailable once `runtime_ready` or `body.body` has been
  invalidated.
- The source body may be absent. A zero physical identity must make the source
  caller take only the path/logical Stop branch. A nonzero identity with a
  missing or mismatched `NativeBody` is an explicit provider failure, not a
  successful no-op.
- The source virtual getter reads Character flags at the moment of the call.
  Do not cache a prior `position_from_physics` result across FSM transitions.
- On success, propagate the portable Stop projection back into the same live
  `runtime.subobjects` owner before the next `update_actor` call. Keep the
  distinction between GameObject `moving`/heading fields and
  `PathController::path_requested` explicit; the current source Stop proof
  establishes DropPath and GameObject field writes, but does not prove that an
  adapter may manufacture unrelated controller state.
- Preserve the source frame order: dispatch Stop from the relevant Character
  state/controller event before later actor-path/subobject consumers in that
  frame. A following controller update must consume the cleared source path
  and freshly synchronized owner state rather than a cached route request.

## Evidence and remaining work

The frozen source caller proof records 246 Stop cases, covers all 59
executable Stop instructions, and executes the Character `+0x64` vtable
override through the pinned Character address point. The
`character_physics_position` proof compares 67,594 Character flag words against
the original override. A separate `native_body_differential.py` runner is
available to compare the fused native body operation against original Stop and
the pinned Box2D backend. These separate proofs do not compose
`dh2_game_object_stop` with live `SpawnOwner`, `NativeWorld`, contact owners
and VM/controller callbacks; this note does not claim that integration test
passed.

Before claiming live Ghost Stop, add a focused composition gate that starts
from a nonempty route and nonzero body velocity/force, dispatches Stop through
the exact synchronous callbacks above, and asserts: route count/owned become
zero without freeing route storage; the Character policy is queried from
current flags; source service order is preserved; actual Box2D XY/angle,
sleeping, velocities, force, torque and sleep time match Stop; the logical
projection matches; body absent/identity mismatch fails closed; and an
immediate subsequent actor frame does not resume the cleared path. That gate
and the actual renderer integration are pending.
