# Original gameplay camera recovery V1

Read-only recovery; the production renderer camera is unchanged. Original ELF
SHA256 is recorded in damping-original-receipt.json. Full source bodies are in
original-source.asm; selected camera virtuals and literals in camera-vtable.json.

## Concrete source facts

* Level::_LoadCamera3f1008 creates CameraOverview and CameraLevel, stores the
  latter at Level128, loads actual LevelConfig camera_file24c / animset264 /
  name27c, and calls SetActive. The string buffer reads260/278/290 are those
  same string objects, not three additional fields.
* LevelConfig defaults are empty file/name, Default animset, near600/far10000;
  its genuine initialization supplies data/3D/camera/CameraTests.bdae and
  PlayerCamera_Default when the corresponding strings are empty. A selected
  authored level's overrides must be borrowed; default fallback is not proof
  that Crypt or Swamp chose that resource.
* CameraLevel::Load41068c constructs VisualObject with parent NULL, finds the
  configured node and camera scene type, retains the real authored root and
  camera, and constructs its AnimSet/timeline. Full cache contains both
  CameraTests.bdae and playercamera.bdae plus camera animation resources.
  cache-manifest.json records exact original archive entries/hashes.
* Level load's SetData values are FOV 0x3edbf877 = 0.429630011 radians,
  aspect 0x3fd578e9 = 1.667752385, and actual LevelConfig near/far. The scene
  camera address point is vtable+1c; slot13c=setFOV,138=setAspect,130=setNear,
  134=setFar. This initial source aspect is constant; a later viewport-change
  writer has not been proven absent and must be audited before claiming the
  whole viewport policy.
* CameraLevel::Update410390 only runs the follow pipeline for the active
  camera with target, root and camera present. Order is transition, target
  position, offset, local-player centering, source offsets98/9c/a0 (optional
  XY45-degree rotation), zoom/shake, ghost camera, damping, root SetPosition,
  target disabled81 cleanup. No inferred rightward screen offset exists.
* HandleCentering40fa68 returns unchanged for one local player when actual
  current Level exists. Two/more player layout reads Level190..193 and uses
  actual camera unprojection. It does not imply a single-player 60% anchor.
* CameraTarget constructor411cdc enables damping25, ratio28 bits3f333333,
  velocity2c..34=the original Point3D ZERO. Offset24=0; targetc=NULL;
  transition1c/20 and ghost velocity38..40 initialize zero.
* HandleDamping41165c is not fixed interpolation. For each axis it stores
  v=(v+desired-firstRootPosition)*ratio, then rereads the same root and obtains
  unsigned Application dt, finally desired=secondRootPosition+float(dt)*.001*v.
  There is no clamp of large dt in this function. Application's outer update
  policy belongs to the shared Application, not this owner.
* HandleZoom410218 queries actual Debug InfiniteZoom first. Normal bounds
  come from source config singleton a8/ac or48/4c according to mode85.
  These must be borrowed from the actual configuration; no invented zoom
  min/max is included in V1.
* CalculateDefaultTargetDistance40fd18 obtains the Euclidean separation of
  actual camera parent and camera target-node positions, stores it at94;
  it does not derive zoom distance from the world's enclosing mesh radius.

## Portable verified portion

gameplay_camera_damping_v1.hpp/.cpp provides retained DampingStateV1 and
handle_damping_v1. Bind root_position to the SAME actual scene root GetPosition
and application_dt to the shared Application. Two reads are intentional.
Disabled/missing-root is the genuine no-call leaf. Required callback failures
retain velocity mutations reached before the second read/dt boundary.

Compile with -ffp-contract=off. test_gameplay_camera_damping_v1.py executes the
whole original ARM method and optimized ARM64 kernel: 288 bit-exact cases,
0 mismatches, varied ratios/positions/velocities and dt through UINT32_MAX.
Root-position and GetDt receivers are explicitly modeled boundary fixtures;
this does not prove a production camera graph or whole gameplay rendering.

## Remaining integration requirements

Do not swap the live camera for the damping kernel alone. The shared Level
must publish its actual selected camera config/table and retained camera
Visual/Scene/target-node/timeline, then supply the same actor's camera-anchor
position and same Application/viewport. Complete transition, GetCenterOffset,
ghost, source zoom configuration, shake and later aspect writers still need
portable composition/proof. Character target height comes from those original
anchors/offsets/resources; the development renderer's +90 is not established
as the original value. No deadzone/lookahead claim is made without their
reached original bodies and actual configuration producers.

## V2-V6 continuation (2026-10-06)

New `GameplayCameraTargetV2` ports source SetTarget, HandleTransition,
HandleOffset, HandleGhostCam, HandleDamping and Target::Update. NULL SetTarget
is ignored. Positive transition from a NULL prior target borrows the genuine
Vec3f_Origin (GOT99f854); it does not invoke GetCameraAnchor on NULL. Immediate
selection clears transition and ghost fields while preserving follow velocity.

New `GameplayCameraSceneV3` reads actual serialized tag1 camera instances,
SCamera28 records and explicit target-node bindings into one graph. It supports
the inspected camera-only local visual scene domain; external/non-camera
instances and missing target URI remain explicit boundaries. It does not
change the historical scene::load geometry requirement or shared scene ABI.
Root wrapper position is separate from authored top-level node translation.
Camera instance local position is separate from its serialized parent node
(source CCameraC1 begins at0,0,100). CameraLevel changes that child position;
overwriting the authored parent would lose the original angle/transform.

New `GameplayCameraLevelV4` ports active/target/root/camera gates and complete
Update stage ordering, source PlayAnim field prefix, shake decay, zoom bounds
and disabled81 target cleanup. Real controller/root/World/Debug/config services
are required when reached. `source_animation_callback` is only to be called
by an actual finite timeline callback; it is not an animation completion timer.
`GameplayCameraDesignV5` borrows the existing first DesignSettings row:
ZoomMin/Max at ac/a8, MiniMapZoomMin/Max4c/48; autozoom c/10/18/1c/20.
Current real design table gives normal bounds0..0.35. Float comparison import
identification proves30e2f8=fcmpgt and30e70c=fcmplt.

New `CameraAnchorBorrowV6` reads/writes the SAME source2e0 slot. Getter3943b8
selects AnchorBase+c when nonnull, otherwise actor160. Setter394378 destroys
the old receiver before zeroing and publishing the new one. It never casts
an opaque context to an assumed raw native layout. Generic canonical bases
already own2e0; live RetainedCharacter/player facets need one missing storage
producer, not a second CanonicalGameObjectBase or perpetual position fallback.

Actual-cache isolated native receipts:

* `reports/android-native-owner-tests/gameplay-camera-target-scene-v3/receipt.json`:
  both original CameraTests/playercamera graphs, 34 checks, target lifecycle and
  required offset/origin boundary. Application/actor endpoints are fixtures.
* `reports/android-native-owner-tests/gameplay-camera-level-design-v5/receipt.json`:
  actual camera/design tables; source frame call ordering, child camera pose,
  mode85 rotation, shake decay and disabled target cleanup. World/Debug/current
  camera selection/one-party centering endpoints are explicit fixtures.
* `reports/android-native-owner-tests/gameplay-camera-table-v4/receipt.json`:
  actual complete animations table yields3 CamAnimSet rows. Camera IDs address
  AnimDict resources, NOT AnimTable sequences. Default row Template57,
  Idle55, Shake44, Crit45 resolve camera_template/camera_idle/
  cam_shake_horiz/camera_crithit_0. SwampCam extras contain three repeated1040
  dictionary IDs exactly; they have not been replaced with invented camera art.

Remaining constructor closure: source AnimSetManager and AnimSetController
must share actual camera graph and canonical resources. ManagerC14753b4
counter1c=-1; Create476464 decrements first, thus fresh first key=-2. Its source
skip-load field is actual LG_DEVICES9f6406, not a generic performance guess.
Camera Load replaces generic BaseNamed with AnimSetController4751b4, attaches
the real root animator, and sets callback to CameraLevel::__Callback (clears84).
Those production services, matrix/viewport continuation and original current
LevelConfig selection are not yet claimed integrated. Live renderer untouched.
