# Live ordered Character loot composition V22

This package composes recovered source owners; it is not a second inventory,
registry, player manager or ItemManager. Hit8 remains disconnected until the
real Level, loot, XP and quest continuations have all been bound.

## Construction and lifetime

1. Retain one `CharacterLootLiveV22` on the same Player/World runtime. Pass the
   existing `CharacterWorldRuntimeV1`, `PlayerManagerOwnerV1`, immutable Gear
   LootTables/ItemPowerTables, actual LootPowerResources and the same Application
   `LootRandom8V2`. `RendererCharacterLootLiveV22::bind` borrows Gear's actual
   tables, text provider and RNG, without allocating another Gear.
2. Its actor callback returns the actual registered Character's property view,
   AiProps row, source type-f4 pointer, position160, initialized table101c pointer
   and canonical receiver lease. The table pointer must equal resolved+9:
   source Structs header ff4, first property ff8, cached table field101c. The
   source getter is a raw declaration ID, not a fixed-point number.
3. Build the existing `WorldItemLiveOwnerV5` against the same canonical
   ObjectManager/PropertyMap, physics world, floor collision graph, obstacle
   registry and scene-root registry. Give its services `drop =
   loot.drop_services()` BEFORE construction. Supply the genuine Item factory
   resolve/condition services, actual graph/cache/device/material/color/audio
   services and actual interaction/outer-frame services.
4. Call `loot.bind_pool(item_live.pool(), error)` once. There is exactly one
   145-slot ItemManager and category cursor. V22 borrows it; it does not recreate
   or clone it. Execute `precache` only at the original ItemManager initialization
   boundary, not implicitly at the first kill.
5. Root's source Kill backend delegates only `kill_drop_loot` to
   `loot.route(actor, request, response, handled, error)`. Keep original
   Character Kill ordering: dead/property prefix -> DropLoot -> contributor
   events/stats -> XP -> quests -> outer Ctrl_Kill OnDied2. Do not attach a
   second alive/dead/animation callback. The same source callback receives the
   credited killer; no independent nearest-player or automatic Gear insertion.
6. Flush the same pool before canonical Item deletion. Actual Item graph
   detach must precede physics/floor teardown; rebind the same retained owners
   on level replacement. Destroy V22 only after Item pool/drop callbacks cease.

V22 latches failed destructive routes per actor and leaves the existing pending
temporary inventory/item prefix intact. A subsequent route returns the original
failure without another selection, roll or transfer. Successful duplicate-call
suppression belongs to original Kill's dead gate; V22 does not invent a second
reward ledger. Source empty/ineligible branches remain genuine early returns.

## Creation providers already composed

`CharacterLootCreationBindingsV22` is an optional production binding for actual
Debug singleton/files, fresh PM Warrior/Mage/Rogue counts (base IDs263/290/325),
and actual `ItemPresentationOwnerV5::add_power` using the same ItemInstance and
text provider. It returns `WorldItemLootCreationServicesV10` for V22. The caller
must pass the actual presentation owner and deliver its `forget` before item
deletion or destination merging; it is not an inventory mirror.

The source `GetPlayersCount4043a8`, current-Level/difficulty query, reached
assertion and full notifications still require actual callbacks. No count1,
false-Level or successful assertion is supplied. Existing retained PM count0
therefore remains truthful; class queries execute the source empty-count path.

## Item factory, physical graph and audio audit

Existing `WorldItemLiveOwnerV5` composes `CanonicalItemFactoryV2`, actual
ObjectManager Spawn, the canonical shared Handle, PropertyMap/default/template,
Item InitPost, same-scene visual, NativeBody/PODecorItem and PF connection.
`CanonicalItemFactoryV2::pickup_override58` borrows the real ItemInstance
signed16 field (frozen V1 called it requirement), initialized -1; it does not
derive an override from ItemTable. Round-robin reuse and ownership transfer use
the same `WorldLootItemRuntimeV1`/ItemManager.

Root's current `renderer_item_graph_v4.inc::vox_service` and
`renderer_combat_sound_v2.inc` provide only genuine sound-disable and current
Level queries. They explicitly fail reached positive Play3D operations. No
sound bank/device/playback owner is therefore available to accept a positive
Item drop or pickup sound. Actual source level phase may take a proved early
sound gate; it must not be changed solely to bypass audio.

The current canonical Level C1 word150 is zero and requires DropLoot. The
development Level cannot be given gate1 to skip it. Real Level/currentLevel,
nonempty canonical Item/template initialization, all reached pickup/quest/UI
tails and positive audio are required for whole lethal skill acceptance.

## Build and evidence

New production level-world TUs:
- `character_loot_live_v22.cpp`
- `character_loot_creation_bindings_v22.cpp`

Additional dependency missing from the tested APK link closure:
`point3d_normalize_v2.cpp`. Existing source `character_loot_scatter_v8.cpp` and
`canonical_point3d_globals_v1.hpp` supply the original scatter and Vec3f_K.
All existing loot, V10 creation/drop, Item V5 graph/factory, PM and Debug TUs
remain unchanged. Include the new renderer helper after Item/Kill adapters;
root must add its member/header and explicit real-provider construction.

Strict two new TUs x ARM64/x86_64 PASS4. Temporary complete renderer closure
with the new include PASS2. Isolated emulator5554 test links current APK
55164f7836de89b7740887e0e34e830acd8d6717ec1033ff7f38b94aa7c7b97a,
compiles new TUs plus normalization explicitly, and passes23 checks. It loads
actual bundled339-table cache bytes; actor/PM initialization and absent debug
file are declared fixtures. It proves invalid-table empty drop, ineligible
source branch, SAME pool identity, PM local record, shared RNG scatter, property
and position identity rejection, retained pending failure and no retries,
source Debug and actual count0 class-query behavior. It does NOT prove positive
nonempty live world drop, GPU/audio, XP or lethal gameplay. No game lifecycle,
APK install, touch or production source shared-owner edit was performed.
