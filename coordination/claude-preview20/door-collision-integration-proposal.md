# Door collision integration proposal for v20

Feature files: `port/windows-foundation/features/doors/door_collision_v1.{hpp,cpp}` and `door_collision_geometry_v1.{hpp,cpp}`. Main/CMake remain owned by `/root/v20_integration`.

## CMake delta

Add `features/doors/door_collision_v1.cpp` and `features/doors/door_collision_geometry_v1.cpp` to `foundation_doors`. Add `door_collision_v1_tests.cpp` and `door_collision_geometry_v1_tests.cpp`, both linked to `foundation_doors`, and register them with `add_test`; the geometry test takes the isolated v20 asset-stage root as its argument.

## Runtime boundary

After `DoorRuntimeV1::adopt()` and every condition/state restore or door transition update, read `DoorRuntimeV1::physical_bindings()`. Each snapshot carries stable ID, authored collision gate, presence, latched source filter, source ApplyMeshBox absolute XY bounds in game units, and a validity bit. The geometry is captured during adopt before idle-state selection, matching source Door InitPost/ApplyMeshBox before InitFinal. A colliding, present snapshot with `source_bounds_valid == false` must surface the geometry error rather than substitute renderer bounds.

Convert each snapshot with `collision_enabled && present && filter_enabled` to a `DoorCollisionVolumeV1`, copying `source_bounds_xy` into `bounds_xy`; set source physical fixture group/category/mask from the same recovered static door body (group 0, category 2, mask `0xffff`, unless source MP_NoCollisions overrides the group). Use `plan_door_physical_reconcile_v1` with last-applied snapshots to issue create/retire/filter updates idempotently; commit the snapshot only after those operations succeed.

At the common actor segment-admission point before any source root-motion position is accepted, construct `DoorActorCollisionV1` from that actor's existing physical radius (game units), source group/category/mask, physical-collision enabled state, and the original MoveActor ignore-collision policy. Pass all current volumes, the actor's current XY and desired XY to `constrain_door_motion_v1`; accept only its returned position. Apply this same path for player, enemies/followers, and context-approach movement. An opened marker or clip-end changes the stored filter; an opening/closing clip before that source terminal event retains the prior filter.

The source geometry adapter and solver do not create a native body or invoke a private player-only collision path. The recovered native owner remains the lifecycle/filter authority; the shared movement gate consumes its source snapshot where P17's current mover has no native Box2D step admission.
