# Lane 20 handoff — cutscenes and cinematic cameras

Status: camera plane receiver support added; command 6 binding remains with lane 17. Source state is not gameplay verification.

## Changes in this lane

- `port/level-world/gameplay_camera_level_v4.hpp/.cpp`: added per-plane `set_clip_start(float)` and `set_clip_end(float)` receiver calls, mapped to source `CameraLevel` virtual offsets +304/+308. The separate calls preserve the original command's near-then-far ordering and partial prefix.
- `port/level-world/gameplay_camera_runtime_v11.hpp/.cpp`: retained the actual loaded `CameraLoadV11` near/far integer defaults through `source_clip_defaults_v120(...)`; binds both setters to the SAME loaded runtime's view plane state, which is used to rebuild the projection on the next camera view. Facade methods enforce the same loaded camera and Level owner.
- `port/level-world/gameplay_camera_factory_v16.hpp/.cpp`: exposed matching per-plane setters on the existing retained procedural camera node. Each changes only its own plane on that same node.

Lane 17 was given the exact interface for its command 6 implementation in `source_campaign_script_execution_v96.cpp`: preserve current-Level NULL and camera NULL early returns; read command `+12` near then `+8` far; positive values use these setters; `-2` uses `source_clip_defaults_v120` values; all other nonpositive values skip that plane.

## Existing shared cutscene wiring

`port/android-native/app/src/main/cpp/source_campaign_script_runtime_v99.cpp` binds the actual current World/Application lifetime to the existing UI/runtime callbacks. It resolves the real `MenuBase::s_igmOpened`, whole `MultiMenuManager::PopMenu` receiver, `AnimController::s_scalingEnabled`, `PlayerManager` network `PlayerInfo::SetInCutscene`, and online dead-player reset. It preserves the same World/Application lease and delegates enrollment to `bind_source_campaign_script_execution_v96`.

The source command owner `port/android-native/app/src/main/cpp/source_campaign_script_execution_v96.cpp` already routes Enter/ExitCutSceneMode (kinds 1/2), PlayCamera (5), SetCameraTarget (8), WaitCamera (7), MoveActor (40), LookActor (41), PlayActorAnim (45), visibility, and actor state to existing providers. This file is outside lane 20's writable paths and lane 17 owns the execution/actor providers. Camera Wait/Play wait state uses the actual `CameraLevel` state. Enter/Exit cutscene UI and player/network state callbacks are supplied by runtime v99.

`port/level-loader/script_command_execution_v96.cpp` remains the generic actual-receiver adapter. It binds supplied owner callbacks after checking identity/lifetime and dispatches original Execute/IsBlocking/Update; it does not own App/World camera behavior.

## IDA evidence and remaining integration

- `Script_EnterCutSceneMode::Execute(bool,int)` 0x45a384 and `Script_ExitCutSceneMode::Execute(bool,int)` 0x4599ac perform HUD callbacks, toggle `AnimController::s_scalingEnabled`, and update online player cutscene state. Enter also uses the current Level cutscene-UI field, menu PopMenu, skill reload and online dead-player reset. Existing native code routes these effects through the same current owners.
- Both functions also store/read process byte `0x9A600C`. IDA xrefs show reads from `RootSceneNode::onAnimate(unsigned int)` 0x35d168, `CharTimers::Update()` 0x3db640, `HUDControls::Update()` 0x41a780, `InfoHUDManager::HideLocalDeathTimer()` 0x41d570, `NativeBackToHud(...)` 0x444990, and scene registration/update functions at 0x357ea4. Root is investigating the shared native publication/owner for this source global; do not invent a duplicate boolean or map the 32-bit address into 64-bit process memory.
- `Script_SetCameraClip::Execute(bool,int)` 0x45c670 consumes command data `+12` (near) and `+8` (far). Positive values call CameraLevel virtual methods at +304/+308. `-2` selects current LevelConfig near/far fields +660/+664; other nonpositive values leave that plane unchanged. Native per-plane receiver APIs are now ready; lane 17 owns adding the command dispatch.
- IDA camera behavior: `Script_SetCamera::Execute` 0x455678 is a literal no-op; `Script_PlayCamera::Execute` 0x460110 starts authored idle animation on skip or the command animation otherwise; `Script_SetCameraTarget::Execute` 0x460004 sets the actual named-object/local-player target and duration (zero on skip); `Script_WaitCamera::IsBlocking` 0x45932c reads CameraLevel byte +132; `Script_PlayCamera::IsBlocking` 0x459370 gates that byte on its original wait operand.
- Actor execution and cutscene skip/return wiring remain in lane 17's command providers. No parallel actor/camera runtime is introduced.

No build, tests, emulator, ADB or runtime proof was run, as required by the lane assignment. Assignment requested GPT-6 Luna/high; the selected live runtime is not exposed to this worker for independent confirmation.
