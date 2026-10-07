# Camera projection, basis and Application services successor

New TUs: `gameplay_camera_picking_v20.cpp`,
`gameplay_camera_basis_v21.cpp`, `authored_camera_basis_v22.cpp`,
`gameplay_camera_application_v23.cpp`. Frozen V19 is unchanged.
Strict native syntax passes ARM64 and x86_64, the current modern engine ABIs.
The inherited engine headers deliberately assert 64-bit owner layouts; ARMv7
compilation is not supported by this successor and was not repaired here.

## One Application context, one actual World session

Include `renderer_gameplay_camera_services_v20.inc` after the real
EquipmentPlatform/current `equipment_platform` declarations and include
`gameplay_camera_application_v23.hpp` and `<map>`. Call
`borrow_gameplay_camera_services_v20(app,bindings,out,error)`. Its only resource
reader resolves the CURRENT actual OriginalCacheAsset for every request. A
required missing current provider or absent resource fails explicitly.

The SAME Application's native integration lease slot owns the context; the
typed weak directory validates that opaque slot before borrowing it. It is
native type-safety bookkeeping, not a second SceneManager or camera authority.
Context holds weak App, the real events14 lease, SAME RootRegistry, first
DefaultSceneNodeFactory, source ZoomHandler50 and AnimSetManager. Actual
dictionary is the process-static actor_clip_table or another separately owned
immutable metadata object. Its lease must not retain App through its own slot.
The same applies to filesystem/backend leases and captured callbacks: avoid
capturing a strong App or previous World/equipment_platform. Root's actual
viewport callback reads current native surface state. The configured modern
landscape orientation0/flip0/targetcount0 policy must be stated as backend
configuration, not inferred from screenshots.

The actual World field must own
`shared_ptr<dh2::camera::CameraWorldSessionV23>`. Call
`context.load(config,world_bindings,world_slot,error)`. Session is published
before Load, preserving a reached failure prefix. App context only weakly
observes it: storing V19 strongly inside App would create App→V19→App. Runtime
and session World leases require explicit ordered release before resetting
the World owner; no metadata defaults or player constructor are replayed.

`update(error)` performs whole source V14 autozoom BEFORE CameraLevel Update.
Its centering callback uses whole V14 and the same supplied layout services.
Party count1 and local count1 are original early branches; general counts
require actual online/player/dead/screen/world services. Application dt comes
from the real supplied actor target callback, not from an invented helper
clock. `scene_phase(actual_world_stamp,error)` then `view(engine_view,error)`
provide the actual scene animation and view transport. For GPU draw copy the
engine projection and invoke V12 with actual configured driver fields. Retain
the engine matrix separately for source projection and picking.

Release sequence: `begin_world_release` unbinds Zoom, removes camera roots,
clears SceneManager active camera. Root then runs actual Physical/PF cleanup;
`flush_animation_sets` follows; Root runs actual Level events/App cleanup;
`release_world_owners` releases native camera owners/World bindings. Reset the
World slot afterward. `close_application` rejects a surviving camera prefix,
then detaches Zoom events and releases context services. Preserve errors and
reached mutations; do not substitute successful empty cleanup callbacks.

The required AddAnim Debug endpoint is `Debug.load` then
`GetSwitch("isTracingAnimSetManager")`: source476608/476630, literal8cd850.
Original ignores its returned switch value but requires the callback to run.

## Exact projection/picking conventions

`GameplayCameraPickingV20::bind` receives actual CameraViewV11 plus actual
viewport rect width/height, render-target width/height and genuine scene/
active-camera presence. Source ray uses viewport rect differences; normalized
GetWorldCoord uses separate target+c/+10 dimensions. Never silently alias the
two when the actual configured values differ. Original ray/pixel projection
do not subtract viewport rect origin; oracle tests exercise nonzero origins.

* `screen_coord` reproduces CameraBase40f714 projection-state2 × view-state0,
  homogeneous divide, output normalized +Y-up.
* `screen_pixels` reproduces CollisionManager6c57d8 camera projection/view,
  integer half viewport, floor(value+.5), top-down Y. Behind-camera W<0 gives
  (-10000,-10000); genuinely absent manager/camera gives (-1000,-1000).
* `ray(integer pixels)` reproduces CollisionManager6c59c0, actual normalized
  frustum plane intersections and perspective/orthographic branches. No
  inverse of the GPU depth/orientation-adjusted matrix is used.
* `world_coord(normalized,height,...)` preserves source float→integer viewport
  quantization and LIMITED line/plane intersection. Source failure can write
  the infinite intersection before rejecting the segment. Delivery success
  and actual intersection boolean are separate. Normalized +Y maps down here;
  it is not directly the inverse of CameraBase's +Y-up output.

No guessed joystick axes, camera offsets, clipping clamps or nearest target
selection were added. Root must use its actual touch coordinate transport and
same submitted view for World/HUD; V12 GPU orientation is a separate boundary.

## Authored basis and optional offset

CameraBase GetCameraLookAtVec reads actual absolute transformation column1
[4,5,6]; GetCameraUpVec reads column2 [8,9,10]. These are not the view matrix's
forward/up vectors. `borrow_authored_camera_basis_v22` borrows the SAME V3
authored parent world transform and camera instance eye, plus actual repeated
FOV reads and source initialized Vec3ZERO/UP. CCamera instance C1 has identity
local rotation; positive future local-rotation producers require extending
that same instance authority, never substituting the target direction.

`source_camera_center_offset_v21` preserves absent-camera false/output unchanged,
initial output XY/Z0 prefix, exact horizontal threshold, actual absolute eyeZ,
two FOV reads and source angle/tangent/normalize arithmetic. It is optional
only under the genuine original offset24 flag. No horizontal shift is guessed.

## Source frame evidence and acceptance

Level.Update3f82d8: PhysicalWorld→ObjectManager→UpdateCameraZoom3f9870→VisualFX→
CameraLevel Update3f8534, then node virtual+b8 `updateAbsolutePosition(false)`
and absolute eye/parent reads for fog/listener. That virtual is not animation.
Camera scene vptr address point is table+1c due virtual-inheritance prefix.
Actual scene animator timestamp runs separately before view submission.

Original ARM vs optimized ARM64: V20 PASS896 exact cases (planes, frustum
extraction, full ray, normalized projection, quantization and pixel projection),
V21 PASS160 exact offset cases/read ordering. Imported softfloat/math and actual
virtual/driver endpoints are declared boundaries. Frustum bounding-box side
effects are excluded because picking never reads them; no full SViewFrustum
bounding-box owner is claimed.

Android actual-cache tests PASS camera view→source HUD pixel→limited ray and
authored basis/offset; persistent Application context→two World loads→ordered
release→no App ownership cycle. Dictionary lease and resource bytes are real;
World/driver/device/Debug/PM services are explicit fixture boundaries. These
receipts are composed native proof, not live player-camera or input acceptance.

Positive skybox mesh/root wrapper remains an explicit required provider when
actual LevelConfig skybox is nonempty. Overview PF singleton and minimap input
services remain required if those actual branches are reached. No full Level
constructor, quest lifecycle, SceneManager draw or network claim is made.
