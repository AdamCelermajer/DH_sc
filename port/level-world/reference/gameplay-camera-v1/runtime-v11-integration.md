# Original gameplay camera runtime V11

This is a resource-backed portable composition, not an installed camera change. No guessed horizontal offset is introduced.

## Concrete owner

`GameplayCameraRuntimeV11` owns one `GameplayCameraSceneV3`, one source camera `AnimatorSet` and one `GameplayCameraLevelV4`. `load(CameraLoadV11)` borrows the existing `AnimationTables` and resolves its first exact `camera_names` match. The same graph supplies authored parent/target nodes, registration targets, animation pose and submitted view. No scene copy, Character scheduler, forced animation event or duplicate player is used.

Camera resources and dictionary IDs are actual bundled data: Default row0 template57/idle55/shake44/crit45. These are AnimDict resource IDs, not AnimTable sequence indexes. The Application retains one `GameplayCameraAnimationManagerV10`; source constructor counter=-1 and Create decrements before insertion (first key=-2). This camera-domain directory must be shared with future original Character registration, not recreated per World/GL restore or populated with cloned actor ClipBanks.

`CameraLoadV11` requires actual LevelConfig file/animset/node/near/far. It does not manufacture defaults or a current level. Existing source LevelConfig InitPost may provide CameraTests.bdae/PlayerCamera_Default/Default/600/10000; borrow that actual produced receiver. Source `_LoadCamera3f1008` also constructs Overview, binds ZoomHandler and adds skybox; those outer owners remain separate requirements.

## Exact caller bindings

1. Read resources through the existing APK/cache retained resource owner.
2. `attach_graph` performs genuine Visual constructor SceneManager root publication before camera selection/animation registration; `attach_animator` performs the original controller root membership/onBind over the same graph after its real set is created. Both retain leases. `detach` must examine actual membership, including failed-candidate prefixes, and remove the same root/animator; it must not be a successful no-op. The graph/animator shared pointers provide lifetime, not fabricated original SceneManager identities.
3. `activate(identity,scene,index)` must use one process CameraBase active slot: previous virtual Deactivated, publish self, actual SceneManager.setActiveCamera, new Activated. `is_active` reads that same slot. `release_active` clears only when identity matches; it is not an independent renderer active flag.
4. Actor services borrow the same World/Character source position160, camera-anchor2e0, disabled81, local-player list/currentLevel and actual Application.GetDt. NPC anchor storage is Combat's sole `CharacterPositionFieldsV7.attached2e0`; no parallel camera field. Player needs its sole base-position authority extended at the actual constructor/adoption boundary, not a per-frame position fallback. Actual anchor object resolves position+c and source setter invokes its destructor before replacement.
5. Scene animation calls `scene_phase(actual_source_timestamp)` once through the retained SceneManager phase. Camera update calls `update()` at original camera update ordering; dt comes from Application, not timestamp subtraction. `NewAnim(false)` only disables displacement byte1ec and performs no immediate pose/callback. The actual finite timeline callback clears CameraLevel.shake84.
6. Level `_LoadCamera` caller order is load, SetData (inside load after controller), activate, play_idle, resolve actual local player0/Character660, set_target with0ms, ZoomHandler connection and autozoom fields. Source `HandleCentering` single-local-player branch is a proven no-op only after genuine currentLevel/party-count service succeeds. Multi-local-player centering/unprojection and automatic zoom remain required source branches; do not bind unconditional success.

## View contract

Default distance uses parent and target-node **local getPosition** (source virtual+a0), not world positions and not camera child position. `view()` uses the same current graph world matrices and actual child camera local position. Source Level SetData FOV bits3edbf877=0.429630011rad and aspect bits3fd578e9=1.667752385 override constructor viewport aspect. Near/far are actual LevelConfig integer conversion. Up=(0,0,1). Zoom writes child local(0,0,-zoom*local_distance), preserving its authored parent pose.

Returned matrices are original positive-forward/positive-W, depth0..1 engine matrices. Their ARM differential proof is bit exact (316 cases). The new `gameplay_camera_gpu_projection_v12` recovers the actual original GLSL driver's conversion: setTransform state2 copies the source matrix, calls `fixUpProjectionMatrix6dd9d0`, then stores its shader copy; `getTransformForShader5af054` reads that copy. V12 matches depth conversion, raw sign-bit Y flip and orientation permutation in768 bit-exact ARM differential cases. Its inputs must borrow actual driver render-target count, flip_y4a0 and orientation13c, not guessed landscape constants. Original IVideoDriver constructor initializes count0/orientation0; CommonGL constructor initializes flip_y0; later actual target/orientation producers must be composed before live use. Driver field lifecycle/upload is still a production binding requirement. Do not feed these through the development renderer's existing RH camera builder, recompute FOV45 or overwrite fixed source aspect every frame. SceneManager onChanged sets width/height aspect at lifecycle; subsequent Level SetData wins unless another actual lifecycle change occurs.

## Lifetime and source ordering

`close()` is explicit while real services live. CameraLevel D1 first destroys Visual/controller, then CameraTarget D2 reaches CameraBase D2 active-slot clearing and root/camera drops. V11 therefore detaches first, invalidates animator callback, clears matching active identity, then releases owned graph fields. Failed detach keeps active identity and all reached resource/pose fields. Source AnimSetManager directory/resources remain Application-owned.

## Verification and remaining production boundaries

Actual-cache native runtime test covers complete resource/table/load/target/frame/pose/view/teardown composition, exact registration ordering and rejected attach/discard prefixes. SceneManager/actor/Application/device/Debug services are explicitly declared fixtures; this is not live camera acceptance. Receipt: `reports/android-native-owner-tests/gameplay-camera-runtime-v11/receipt.json`, APK11c8219e. Animator receipt additionally verifies Play does not immediately change any graph transform or callback count. Damping original differential288 and matrix differential316 are separate source oracle receipts.

Remaining production work: real camera instance/SceneManager registration projection over the shared roots owner; process active slot owner; actual player anchor/destructor storage; LG fact from the already retained JNI manufacturer string; same Application manager integration with Character registrations; original automatic-zoom/centering/offset services when their genuine branches are reached; actual driver field lifecycle/matrix upload and viewport binding; Overview/ZoomHandler/skybox outer load lifecycle. Missing selected camera/target URI/type is an explicit supported-domain boundary, not substituted development geometry.

## Build closure (new units only)

LevelWorld: gameplay_camera_damping_v1.cpp, target_v2.cpp, scene_v3.cpp, level_v4.cpp, design_v5.cpp, anchor_v6.cpp, matrix_v8.cpp, device_v9.cpp, animator_v10.cpp, runtime_v11.cpp, gpu_projection_v12.cpp. Existing dependencies: actual BRES/scene/math; animation table/dictionary/design owner; RegistrationSet/TransformSet/Player; visual_timeline kernels. Compile source arithmetic with `-ffp-contract=off`. No existing shared class layout changed by this camera package.
