# Door collision integration proposal for v20

Feature files: `port/windows-foundation/features/doors/door_collision_v1.{hpp,cpp}` and `door_collision_geometry_v1.{hpp,cpp}`. Main/CMake remain owned by `/root/v20_integration`.

## CMake delta

Add `features/doors/door_collision_v1.cpp` and `features/doors/door_collision_geometry_v1.cpp` to `foundation_doors`. Add `door_collision_v1_tests.cpp`, `door_collision_geometry_v1_tests.cpp`, and `door_runtime_restore_v1_tests.cpp`, all linked to `foundation_doors`, and register them with `add_test`; geometry and runtime-restore tests take the isolated v20 asset-stage root as their argument.

## Runtime boundary

After `DoorRuntimeV1::adopt()` and every condition/state restore or door transition update, read `DoorRuntimeV1::physical_bindings()`. Each snapshot carries stable ID, authored collision gate, presence, latched source filter, source ApplyMeshBox absolute XY bounds in game units, and a validity bit. The geometry is captured during adopt before idle-state selection, matching source Door InitPost/ApplyMeshBox before InitFinal. A colliding, present snapshot with `source_bounds_valid == false` must surface the geometry error rather than substitute renderer bounds.

Convert each snapshot with `collision_enabled && present && filter_enabled` to a `DoorCollisionVolumeV1`, copying `source_bounds_xy` into `bounds_xy`; set source physical fixture group/category/mask from the same recovered static door body (group 0, category 2, mask `0xffff`, unless source MP_NoCollisions overrides the group). Keep source snapshot/filter lifecycle coherent and idempotent. Native body create/retire/filter reconciliation may use `plan_door_physical_reconcile_v1` where the recovered world owner is available, but native-body plumbing is optional because P17 does not step native Box2D for root-motion admission. The mandatory consumer is the shared actor motion gate below.

At the common actor segment-admission point before any source root-motion position is accepted, construct `DoorActorCollisionV1` from that actor's existing physical radius (game units), source group/category/mask, physical-collision enabled state, and the original MoveActor ignore-collision policy. Pass all current volumes, the actor's current XY and desired XY to `constrain_door_motion_v1`; accept only its returned position. Apply this same path for player, enemies/followers, and context-approach movement. An opened marker or clip-end changes the stored filter; an opening/closing clip before that source terminal event retains the prior filter.

On save restore, map serialized 0/2 to terminal closed and 1/3 to terminal opened before snapshot reconciliation; restore clears transition-busy/dirty state and does not replay transition clips or sounds. Persist/checkpoint the normalized terminal runtime state (0 or 1), not the loaded transitional byte. Reject malformed values before mutating the corresponding runtime state.

The shared movement gate consumes exact source XY snapshots where P17's current mover has no native Box2D step admission. Do not treat optional native-body reconciliation or a helper pass as a substitute for this effective movement consumer.
