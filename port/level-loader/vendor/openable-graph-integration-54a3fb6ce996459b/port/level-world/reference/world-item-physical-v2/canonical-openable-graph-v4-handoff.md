# Canonical OpenableContainer graph V4

Production files: `canonical_openable_graph_v4.hpp/.cpp` (new). No renderer,
native-app, CMake or existing menu source changes are included.

`CanonicalOpenableGraphV4` owns one RuntimeState, one source GO_ID7
CanonicalOpenableContainerV1, the same canonical base fields, a V3 retained
scene/visual connection, the original generic InitPost/InitFinal owners and
actual fresh PODecor receivers. `factory_receiver()` returns an aliasing lease
to that receiver, with actual class InitPost/position/SetPosition callbacks.
Supply this receiver from the existing `CanonicalReceiverConstructionV1`
OpenableContainer callback; retain the graph in the same world class-owner map.
The original factory address is 340da4, class catalog index25, GO_ID7. These
three identifiers are not interchangeable. Do not add a second source catalog.

The returned callbacks intentionally leave canonical manager Add, source
properties/defaults/overrides/module offset to the existing factory pipeline.
They do not replay initialization or assign a Handle themselves.

## Source-owned concrete continuations

* Generic visual LoadVisual/Sync/light-set/target-node lookups use the attached
  retained visual identity at the same base2d8.
* Source SetPosition uses game_object_set_position_v2 with the same source pose,
  destination1a8, actual root Sync and actual native-body SetPosition.
* Original base AnimController::SetCallbacks474d10 is bx lr. No callbacks,
  synthetic `opened` events or synthesized completion are installed.
* Container named PlayClip calls pass loop0 (39fa0c/10,39faa0,39fac8). This is
  distinct from AnimatedDecor's looped idle path.
* State changes mutate the attached source scene11c before storing state394.
* Source ApplyMeshBox uses the same actual visual mesh bounds.
* PODecor construction allocates a fresh actual owner/body for every reached
  call. Previous-body destruction is invoked by the source assignment tail;
  failed candidates remain retained for ordered release.
* Teardown releases bodies before detaching/destroying visuals, then uses the
  actual V3 scene-root registry to discard unattached candidates.

## Required outer production borrows

`CanonicalOpenableGraphServicesV4` requires the SAME world lease, scene-root
registry and loaded PhysicalWorld, actual cache.read, actual UpdatePFObject,
generic PF InitObject, the actual source Device facts, condition compilation
and evaluation, CheckSpawnProbability using the same LootRandom0, LightSet
name owner, sound manager, object-script load/dispatch, key inventory query,
actual GameObject.Update and ItemObject.DropLootTable. Reached absent callbacks
remain explicit errors. No test success callback is installed in production.

The `visual_asset` callback is the source Visuals table lookup followed by a
write to this graph's base string290. Actual dictionary rows47/48/49 are
`data/3D/GameObjects/go_chest_swamp.bdae`, `_big.bdae`, `_rotten.bdae`.
Original68 Container rows: Swamp_Normal_Chest is id58/visual47/loot227;
SwampCave_Normal_Chest is id55/visual47/loot223. All five selected SWAMP
declarations use those two rows. The test also exercises the genuine Big and
Rotten rows to verify all three source resource domains.

Do not use an audio NULL fixture in production unless the actual manager is
absent. Do not replace script loading or generic Update with empty callbacks.
Do not force scene visibility, invent PF initialization or deliver completion
events outside the original controller/frame producers.

## Verification

Strict Android syntax PASS arm64-v8a and x86_64.
`tests/canonical_openable_graph_v4.cpp` executed on isolated5554 and PASS:
three actual BRES complete graphs/skins/colbox markers, nonloop named
controller, same class/base/runtime, original InitPost/InitFinal, native
Box2D/PODecor assignment and release. Same source68 records and exact Visuals
rows are used. PF/RNG/device/condition/Debug/light/script/Update boundaries are
explicit fixtures. This is a meaningful owner composition proof, not full
live loader acceptance.

Receipt: `reports/android-native-owner-tests/canonical-openable-graph-v4/receipt.json`.
APK library SHA256: da4aede0d4415f85ecc1abf84e689ab55124401c2747f215d673c1f437d8b948.
New graph and game_object_set_position_v2 are compiled into the isolated test;
the former is not yet in the APK.

CMake additions when accepted: canonical_openable_graph_v4.cpp and
game_object_set_position_v2.cpp (the latter was absent from current APK exports).
Other dependencies are the existing canonical OpenableContainer, V3 retained
scene/visual, generic initialization/visual-asset and retained PODecor owners.
