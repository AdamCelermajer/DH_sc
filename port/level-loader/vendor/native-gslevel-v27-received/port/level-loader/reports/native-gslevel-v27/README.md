# Native GS/current Level/shared loading menu V27

The source GS constructor, complete source Level C1, Loading callbacks and
player-independent native MenuManager now have a coherent composition. This is
a staged launch owner; the current Crypt demo retains its existing C1 launch.

## Implemented

- `NativeGSLevelRuntimeV27` retains one GS receiver and one actual global
  `s_level` cell. The GS kernel flushes the actual Application animation-set
  manager before allocating/constructing C1, publishes the same allocation,
  then looks up and invokes the original Loading menu.
- Loading callback bindings are created before synchronous `onProgress`.
  Nonnull-Level progress reads its actual field30. Missing Online34 remains
  required only on the reached absent-Level/online branches.
- `AuthoredSharedMenuRosterV27` registers MenuBase receivers discovered from
  the actual base, main and HUD movies without requiring a player. It shares
  one MenuStackOwner with the later character panel. Source Show/Hide,
  localization, dead zones and weak-proxy checks address those same movies.
- MenuStackOwner can register a source receiver after initial publication
  while retaining live occurrences and republishing its directory views.
- Native camera Application preparation is separate from camera loading.
  The GS path prepares its same animation manager before Flush; the existing
  demo camera still loads at its existing point after actor initialization.
- `borrow_current_native_level_v27` reads the actual GS global independently
  of the renderer's demo C1. Before a GS launch, its genuine BSS value is NULL.

## Current demo and missing contract

`construct_native_level_c1_v25(..., native_gs_launch_v27=false)` remains the
existing `load_world` hook. It constructs the complete real C1, preserves the
demo's rendering flow and does not push a Loading screen or invent a GS
publication. The staged `true` path must be adopted by a genuine game-launch
contract together with original Level.Init, LoadProcess and Unload/destruction.

The actual completed C1 leaves loading field30 and field130 at zero and
loot gate150 at zero. GS construction itself does not call Level.Init.
Enabling the staged GS Loading path now would leave its original progress
unfinished. No timer progress, phase38 assignment, dummy Unload success or
replacement current-Level owner has been added. Replacing a published GS
World cannot bypass required Unload by clearing its global.

The menu composition has compiled, but its actual Android PostLoad/Loading
push has not been runtime accepted. The front-session Loading callback's raw
context must stay pinned through use; a future launch/destruction contract must
unbind it before releasing the GS runtime. GL recreation must preserve the
native CPU menu roster rather than recreate/dangle registered receivers.
The current front menu still uses its V87 development navigation slice;
its preceding navigation history has not been adopted into this native GS/
character stack. The final launch contract must connect that same Application
menu owner and history, rather than claim two stacks are one original manager.

## Verification

- `compile.json`: six changed consumers compiled strictly for ARM64 and
  x86_64 (12 successful compiles), including shared stack, character panel,
  native app and renderer. This precedes subsequent parent FX/target work;
  the next whole APK build must compile the final combined source again.
- `host-receipt.json`: **493 checks passed with ASan/UBSan**. Uses actual
  51-row cache data, original Swamp selection and original combat/death Lua,
  coherently rebuilt source Lua runtime, actual private VMs/shared script
  cache/Save owners, actual GS/current-level/loading owner composition.
- Menu and animation manager backends in that host test are explicitly
  declared deeper fixtures. It proves publication/callback order, identity,
  same field reads, no false EndLoading transition, required Unload failure
  retention, no replay and lifetime teardown. It does not prove live menu
  display, whole Level.Init, map gameplay or campaign completion.
- Neither emulator5554 nor5590 was touched; no APK was built or installed.

## Integration dependencies

Existing shared root files contain the surgical hooks; preserve other agents'
edits to them. Root level-world CMake compiles `native_gslevel_runtime_v27.cpp`
alongside V25 Application/context and V26 Loading. Root engine-ui CMake compiles
the V27 roster alongside the existing source MenuStack/character panel.

This uses the existing full C1/GS lifecycle/CanonicalLevelContext/Loading
owners and actual character-panel platform/localization/movie projections.
The loader's module and family successor packets remain separate dependencies;
this work does not certify their whole InitPost/LoadProcess bodies or merge the
private loader checkout.
