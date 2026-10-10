# Lane 17: Level scripts

## Delivery

Added `Script_SetCameraClip` command kind 6 dispatch to `source_campaign_script_execution_v96.cpp`. It uses the current World/Application camera and lane 20's same-camera interface. The command applies near Data+12 before reading far Data+8; positive integers are converted to floats, `-2` selects retained LevelConfig defaults, and other nonpositive values leave that plane unchanged. The original NULL current-Level camera and NULL CameraLevel exits are preserved. Kind 6 blocking returns false, matching the literal body.

The other assigned script owners already provide the retained actor and movement operations, AI command bodies, environment/door/trap operations, and the shared execution composition. The dispatcher already routes commands to the existing environment v120, World v117, audio v117, AI v118, tutorial v118, and tutorial-control v118 services. Those paths remain unchanged.

## Scheduling and evidence

Scheduling remains on the Application's retained `ScriptManagerOwnerV52`; no second VM was introduced. `bind_source_campaign_script_execution_v96` binds the existing parsed receivers and scheduler. The retained manager implements Start/Stop/Skip/IsRunning, ExecuteScript, ExecuteAllScripts, ExecScript child waiting, and Wait duration accumulation/update. Relevant IDA addresses are StartScript `0x4605c0`, ExecuteScript `0x45c16c`, ExecuteAllScripts `0x45c37c`, ExecScript Execute `0x4607f4`, SetCameraClip Execute `0x45c670`, and SetCameraClip IsBlocking `0x455684`. Original ARM assembly and IDA pseudocode were used; no Ghidra output was consulted.

SetCameraClip's IDA body confirms Data+12 near and Data+8 far. Positive values call the same CameraLevel clip-start/clip-end virtuals at vtable byte offsets `+304` and `+308`; `-2` reads LevelConfig integer fields `+660` and `+664`. Lane 20 exposes these through `GameplayCameraRuntimeV11::set_clip_start`, `set_clip_end`, and `source_clip_defaults_v120`.

## Root wiring and limits

Root must bind the service after the current Application's ScriptManager and parsed command receivers exist, then drive that same manager from the original gameplay update. The entry point is `bind_source_campaign_script_execution_v96(actual_world, ui_leaves, error, actor_leaves)`. UI and actor leaves must be actual current owners. Root retains all cross-owner wiring and integrated validation.

Source review and `git diff --check` passed. No build, tests, APK, or gameplay verification were run. This is source delivery only. The worker runtime did not expose a model/reasoning setting, so GPT-6 Luna/high could not be independently confirmed.
