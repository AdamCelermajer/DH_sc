# Original gameplay camera outer composition V19

`GameplayCameraLoadV19` now composes the recovered source order:
Overview construction through the actual first DefaultSceneNodeFactory cam_
branch, CameraLevel visual selection/grab, AnimSetManager registration,
CameraLevel SetData, process/SceneManager activation, idle Play(false), actual
local Character target, Application ZoomHandler setCamera, source zoom defaults,
and the optional nonempty skybox tail. No constant horizontal displacement is
added. The actual source camera asset, its target and its matrices determine
framing.

## Production bindings

1. Borrow `ApplicationServicesOwnerV5` through
   `model_renderer::borrow_actual_application_services_v5`. Use its actual
   `events14()` heap EventManager and `active_camera()` process owner. Neither
   Level EventManager nor a camera-private replacement is valid.
2. Borrow the SAME World's `GameObjectSceneRootRegistryV1`. Retain one actual
   first `CameraDefaultFactoryV16`, genuine filesystem/cursor constructor inputs,
   and actual configured driver viewport service. Create/initialize one actual
   Application ZoomHandler50 over events14. Retain these across the appropriate
   Application lifecycle; setCamera(NULL) on World teardown.
3. Supply actual `AnimationTables::cameras/camera_names`, matching Dictionary
   lease, DesignSettings borrow, resource reader and Application AnimSetManager.
   The manager dictionary pointer must remain pinned for the manager lifetime.
   `CameraLevelConfigV19` fields are actual LevelConfig camera24c, animset264,
   node27c, near294, far298 and skybox234. Do not select config from the footage.
4. World actor target services read SAME position160 and constructed
   `CharacterPositionFieldsV7::attached2e0`. A NULL anchor produced by fresh C1
   uses the original position branch; an unavailable adoption projection fails.
   Positive AnchorBase+c lookup remains a genuine required borrowed endpoint.
5. `runtime.actor_services.handle_centering` must use actual V14 local count,
   PlayerInfo flags and World screen/world projection. One local player returns
   before centering transforms; this is a proved source leaf, not a generic
   no-op for every player count. Actual application dt, Debug zoom and actor81
   are required; do not use elapsed GL frame time as an invented substitute.
6. Call `update`, real scene timestamp `scene_phase`, and `view` in their actual
   source frame phases. Animation Play(false) does not fabricate an immediate
   pose or animation event. Apply V12 driver projection conversion to this
   actual engine matrix; orientation/flip/FBO policy comes from configured
   backend fields, not screenshots. Draw and unprojection/input must share it.

## Ordered teardown and failure discard

After Root's original skills/AI/item/FX/ObjectManager cleanup:
`source_unbind_zoom_v19`, then `source_clear_camera_roots_v19`, then
`source_clear_scene_active_v19`. Root performs physical clear and PF Flush,
then `source_flush_animation_sets_v19`, then its actual Level events/App cleanup.
`release_native_owners_v19` is the explicit deterministic native lifetime repair
after those phases; the captured original LevelD1 has no explicit deletion of
its128/12c fields. It also discards genuinely reached failed-load graph/camera
references. Never destroy the outer receiver while callbacks referencing it are
still registered. These methods remove camera-owned roots only: Root's full
scene clear must also process all non-camera roots.

## Proof and limits

`tests/gameplay_camera_load_v19.cpp` loads actual cache CameraTests, camera clips,
animation tables/dictionary and design data, and uses actual Application,
EventManager, RootRegistry, factory, Overview, active camera, membership,
timeline, target, matrices and Zoom owners. World actor42, viewport960x540 and
Debug reads are explicitly declared fixture boundaries. A nonempty skybox with
no real provider fails after target/Zoom publication, preserves that prefix and
passes ordered discard. The skybox name in that negative test is a named missing
service fixture, not an authored level selection.

Source math differential receipts prove damping288, matrix316, driver
projection768 and layout224 original cases. The whole Overview/gamepad and
Zoom touch/mouse arithmetic owners have not yet received a full original
differential receipt. Nonempty skybox resource/mesh wrapper construction,
optional CameraBase GetCenterOffset and general normalized screen/world ray
math remain concrete required production endpoints. Normal source offset24
false legitimately bypasses GetCenterOffset, but no full camera-feature claim
is made for the missing branch. Native acceptance is a composed subsystem test,
not live player-camera acceptance; Root still owns production integration.

## Coherent rebuild

Rebuild all consumers of the additive registry active-camera fields/root borrow
callback, and `CameraRuntimeServicesV11::retain_selected_v19` before execution.
No new Character camera-anchor storage is introduced. Existing actor API and
SAME source2e0 authority remain untouched.
