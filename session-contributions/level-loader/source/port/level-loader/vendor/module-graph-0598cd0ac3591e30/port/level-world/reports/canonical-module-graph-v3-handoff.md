# Canonical SWAMP Module graph V3

The callable composition now executes the supported static Module visual/room
path over the actual SWAMP BRES and one canonical manager/floor world. This is
not full GSLevel runtime readiness: the remaining platform/global providers
below must be connected to their actual owners.

## Initialization recipe

1. Retain one candidate lease, `ModuleRuntimeGlobalsV1`,
   `GameObjectSceneRootRegistryV1`, `SceneManagerMapOwnerV2`, `floors::World`,
   `ModulePFRoomsV3`, `CanonicalObjectManagerV1` and `CanonicalPropertyMapV1`.
   Use the existing shared targets; do not compile duplicated vendor owners.
2. Construct `CanonicalRoomZoneFactoryV3` with services capturing a weak
   `CanonicalRoomZoneRecordV3`. Each factory record owns its sole RuntimeState
   and canonical base. Reuse whole CheckSpawnProbability, ConditionData Init,
   SetPosition, LoadVisualObject and UpdatePFObject services for that receiver.
   An empty model goes through actual `GameObjectVisualAssetOwnerV1::load_visual`.
3. Construct `ModuleRoomZoneSpawnV3` over that factory and the SAME manager/map.
   Its `CanonicalSpawnServicesV1` input requires the actual Handle resolver and
   `TestEnableCondition` continuation. It composes the genuine RoomZone factory,
   original Spawn(false,true), property/default/name/archetype order, actual
   virtual38, same-manager pending append and InitBounds/Module backlink.
4. Construct `CanonicalModuleGraphV3`. Its services borrow that same PF rooms
   owner, immutable asset transport, source SceneManager root registry, global
   modular-mesh search, video driver kind and Root absolute-update counter.
   Bind `spawn_zone` and `zone_init` to the retained adapter above.
5. In `CanonicalLevelModuleConstructionV2::module_services`, prepare actual
   outer GameObject services, then call `graph->bind(record, init, module, error)`
   BEFORE the Module constructor. This is the required existing V2 dispatcher
   contract. Graph callbacks resolve the weak record after construction.
6. Run the existing same-manager canonical factory/Add/property/default/XML
   sequence. The actual Module receiver owns its typed fields, including XML
   selections and the single shared counter-produced Module ID. Do not replace
   source room64 with a diagnostic occurrence ID.
7. Execute canonical `init_post`. The graph supplies real ConditionData Init,
   SetPosition/Sync, declared BRES/xref selection, source VisualObject root
   publication/204 association, static optimization, bounding boxes, floor
   clones/map publication, room solid bit and ordered room/world bounds unions.
   Module then performs its source generated RoomZone spawn and InitBounds.
8. After all actual rooms load, call existing `floors::post_load` on this SAME
   world, then `rooms->publish_collision_bounds(error)`. This publishes retained
   PFRoom boxes including the Decor owner extensions instead of dropping them
   when creating the collision query projection. Membership is verified first.
9. Run actual InitFinal and source pending scheduling with the providers below.
   Retain the SAME navigation/physical world used by Character/controller peers.

## Current loader receiver transport

Use the imported `CanonicalReceiverTransportV1` with the SAME property map.
Its catalog constructor callback routes LevelConfig/Module/Block to
`CanonicalLevelModuleBindingsV2::construct_receiver`; route RoomZone to
`CanonicalRoomZoneFactoryV3` and other classes to their actual retained owners.
The transport retains receiver leases and typed callbacks; it supplies neither
a replacement manager nor class implementations. Do not use the older fixed
three-class dispatcher for the LevelConfig/Module requests.

For InitFinal retain one `ModulePFActorConnectionV3` per actual Module, borrowing
its SAME base/runtime.object, the candidate collision world and obstacle registry.
Bind InitPFObject to `init` and UpdatePFObject to `update`. Null user follows the
original kernel prefix; a reached unsupported physical producer fails explicitly.
`LightSetNameOwnerV3` supplies the proven constructor-backed four-name/GetID
subset. It does not supply full light settings/filter/update or replace a future
retained LightSetManager; that manager must adopt these same name fields.

## Supported resource domain and source order

All nine selected SWAMP module roots are actual static geometry-only resources,
with empty animation libraries and no colbox marker. Consequently the source
Decor PODecor branch is skipped because real VisualObject physical28 is false;
no empty physical constructor is accepted. Resources outside that domain fail
with an explicit required continuation. Hidden floor meshes remain searchable,
are cloned with their actual cached transforms and removed from the visual;
their leases survive in the SAME PF room/map world.

Module virtual80 resolves to original GameObject::IsAnimated3883a8, whose whole
body returns zero. Source SetParent therefore takes Sync then OptimizeStatic.
Both invoke the Root absolute-position counter once. Later SyncPosition always
increments it again and updates the root cache before rotation/scaling checks.
The actual Module has no named animation callback to swallow or replace.

Decor::LoadFloorMap388730 updates only PFRoom flags24 mask 1 from solid376, then
PFRoom::ExtendBoundingBox388218 unions room bounds first and its parent world
bounds second. Both solid values are supported; unrelated flag bits survive.
The floor-load pending byte is cleared by the canonical Module only after this
whole continuation succeeds. Failure leaves its original mutation prefix.

## Required real outer providers

These remain required bindings, not defaults supplied by this module:

- Shared CheckSpawnProbability RNG/network/Handle classification and failed
  spawn lifecycle; no constant roll provider in production.
- Actual Device::IsHighPerformance and selected GameObject visibility method.
- Same SceneManager modular-mesh search, driver kind and Root global counter.
- Same manager Handle resolver, network Add and outer TestEnableCondition:
  actual GetLocalPlayer(0,true), Character14e8 profile and current Level118.
  Empty compiled conditions do not bypass these outer gates.
- Source PFWorld InitObject/UpdatePFObject can use ModulePFActorConnectionV3 over
  the same retained actor objects, room geometry and obstacle registry. Bind the
  actual retained constructor-backed light-name owner for InitFinal lookup.
  Generic successful empty callbacks are not valid adapters.
- Full GSLevel/Level constructor EventManager/Lua/online/save/event/async
  lifecycle and genuine current-level publication. Existing canonical context
  is a connected constructor-field owner, not a replacement for that graph.

## Destruction / reload

Stop callbacks and unpublish source pending/room/manager membership first.
While records and candidate still live, invoke `graph->release(record.get(), e)`
to remove source animators/visual root, destroy retained visual/resource owners
and clear the SAME ConditionData allocations through their real destructors.
Then release dispatcher records. PF clones/map children retain their own real
resource leases: remove them through the actual map owner and release the map
before destroying SceneManager/floor/candidate owners. A failed candidate uses
this explicit teardown; do not retry its already-mutated initialization.
The graph release method does not claim to perform manager/network/pending
unpublication or whole source Level destruction.

## Evidence

Current native receipt:
`android-native-owner-tests/canonical-module-graph-v3/receipt.json`, binary SHA256
`ccd011d186fbdf0dfddfc36207555d0e6468ae2c2f3747f3cb496eaad91dc58f`.
The executable ran on emulator5554 through isolated /data/local/tmp transport.
It exercises nine real Module InitPost graphs, sixteen real floor clones,
nineteen exits, nine canonical generated zones/pending entries, source backlinks,
same-world post-load, collision bounds, all nine InitFinal calls through the
same PF object/obstacle graph, constructor-backed light-name selection, and
explicit visual/map cleanup. The retained source PFRoom label remains UINT32_MAX
for source room -1; portable collision-room indices are separate ordered indices.
Platform/profile/network/Handle transports are declared fixtures; this receipt
does not establish their production providers or whole-level acceptance.

Source captures: `reference/module-zone-v3/module-visual-tail.asm`,
`module-virtual80.json`, `zone-virtual38.json`, and
`reference/level-config-module-connection-v2/original-source.asm`.
Actual asset SHA256:
`89da80c60a7ebecd0e8a27a9d46f2625e3a5ec112933e5aa7cab251412b8364d`.
The independent ConditionData original ARM oracle has six passing constructor,
empty and Invalid-name cases; its native same-field suite has twenty cases.
