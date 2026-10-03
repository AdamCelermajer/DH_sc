# Borrowed actor-phase integration

`actor_runtime.hpp/.cpp` composes verified native modules for one eligible actor. The caller owns all storage, scene animation, the genuine physics world, body creation, FSM/AI and clip selection. The coordinator performs no animation sample or world Step. The original ELF identity is SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`; the local manifest and assembly preserve GameObject::Update, UpdateTargetPosition, the absolute-position getter and the base GetSpeed routine.

## API and authoritative state

`dh2::actor::update_actor(RuntimeResult&, const RuntimeRequest&, std::string&)` returns0 when complete,1 for malformed input,2 for navigation capacity failure, or3 for a service failure. RuntimeState is472 bytes, RuntimePolicy36, RuntimeRequest128 and RuntimeResult104 on the native64-bit ABI. Static assertions enforce these layouts.

`RuntimeState::subobjects.position`, destination and rotation are authoritative between frames. The coordinator synchronizes controller position/destination around UpdatePath and transfers its heading/path result into the subobject view. RuntimeState::rotation retains Euler X/Y, heading target and last incremental turn direction. The caller initializes its heading target consistently with the existing heading producer. Path segments, scratch arrays, registry entries, geometry and graph are borrowed storage; their normal module contracts still apply.

RuntimeState::previous_position and previous_rotation are the GameObject pre-update snapshots. They are distinct from subobjects.previous_position, which represents the PF cached position used by camera rejection. The real floor bridge refreshes that PF cache from NavigationObject::motion.position after validation.

Character flags decode the actual position/rotation/floor/update-path policy through `dh2_move_policy`; no physics-position flag is forced. Rotation speed uses the recovered Character getter/property payload index47 through `dh2_move_rotation_speed`. RuntimePolicy supplies explicit obstacle avoidance, boundary debug, physical Stop, camera and auxiliary virtual facts. Its virtual_speed is explicit; `base_virtual_speed=4` is the original base GameObject::GetSpeed `0x3400e4`, whose instructions return float bits0x40800000. Authored animation/root-motion speed remains separate and belongs to the caller's scene/timeline phase.

## Ordered composition

The source order from GameObject::Update `0x38cbe8` is:

1. Snapshot current position and Euler rotation at`0x38cc90..0x38ccbc`.
2. UpdatePath at`0x38ccc4`. Execute genuine NativeBody Stop only for its physical_stop_requested result. With no supplied avoidance scene, a single actor presence view represents the borrowed native body's existence. When an avoidance scene is supplied, its own actor/body presence must agree with the native body. The scene remains explicit for actual neighbors and avoidance.
3. UpdateRotation at`0x38cccc`. Write final GameObject Z and perform any visual rotation synchronization through SceneBinding::set_rotation and update_world.
4. UpdateSubObjects at`0x38ccd4`, with native physical, visual and floor services attached synchronously.
5. UpdateTargetPosition at`0x38ccdc`. A nonnull target_absolute_position supplies the target node's existing absolute XYZ cache; null retains target_position. This is a cache read, not actor/controller position propagation or a new target scene update.

The parent frame dispatcher performs early scene/timeline/root displacement, then genuine world Step, then this eligible actor phase. Velocity changes issued here affect the caller's subsequent Step. Full load/pause/input/eligibility behavior remains outside this adapter; the traced source gates are documented in `../frame-order/NOTES.md`.

## Genuine and explicit services

Physical update, transform, linear velocity and wake use `dh2_native_body_subobject_service` and the actual source-built Box2D2.0.1 body. Stop uses `dh2_native_body_stop`, then reflects its verified direct logical resets in the BodyState view. A false SetXForm result is retained as genuine frozen/outside-world behavior and is ignored by original callers. Public view refresh does not invent private native force/torque/sleep-time observations.

Visual position apply/sync use `dh2_visual_apply_position` and `dh2_visual_sync_position`. Their absolute-position boundary rebuilds the bound scene through SceneBinding::update_world. Rotation sync uses the recovered Euler/quaternion service through SceneBinding::set_rotation and updates world matrices. The explicit caller service handles visual update hooks, base/override ApplyRotation, scaling and camera/auxiliary operations. The original base ApplyRotation is empty; a caller override remains its own service.

Floor validation calls `dh2_nav_validate_object_position` against the real floor CollisionWorld, MotionPolicy, NavigationObject and ObstacleRegistry. Its result controls acceptance and updates the PF rollback cache. The bridge does not use synthetic floor heights or a generic radius controller.

RuntimeRequest::services is always explicit, including when camera and auxiliary objects are absent. Camera return0 is a valid absence or rejection. UINT_MAX reports an external service failure. Native/visual/floor service failures record the reached phase and failed event. Top-level malformed requests reject before state/output writes and callbacks. Later failures are reported after already completed phases; native world, registry and scene side effects are not rolled back. No whole-frame transaction or full original FSM behavior is claimed.

## Composite sanitizer fixture

`tests/actor_runtime.cpp` takes four files: Crypt BRES, derived DWLD room descriptor, Prince BRES and walk BDAE. The tested inputs are `.local-inputs/world/crypt01/crypt.bdae`, `crypt01.dwld`, `.local-inputs/prince_modular.bdae` and `.local-inputs/world/prince_walk_1hand.bdae`.

The host test loads the authored Crypt335-node/838-edge graph and floor selectors, Prince35-node scene and authored walk clip. It performs12 separate scene samples/Steps followed by actor updates, covering146.154 units of real root displacement, path heading/boundary queries, real floor/registry changes, PF rollback cache, visual transforms and genuine body transforms. An explicit physical-position case verifies base virtual speed4 and movement on the caller's next Step. A controller arrival requests genuine Stop, including accumulated-force clearing verified by a later Step. Total14 successful actor frames and14 caller Steps pass. An absent-camera service is observed12 times, five malformed cases preserve state/body/output without callbacks, and an external visual-update failure reports its phase/event.

ASan, UBSan and leak detection pass. This is a functional composite adapter audit. Original instruction parity belongs to the underlying path, rotation, subobject, visual, floor and genuine physics module audits; this fixture is not an end-to-end original frame oracle. Renderer/CMake/world initialization and APK packaging are parent-owned and were not changed by this task.
