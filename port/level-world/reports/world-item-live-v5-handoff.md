# Canonical world Item graph and pickup handoff V5/V10

This is a callable native composition over the **existing** World, CanonicalObjectManager, PropertyMap, Scene root registry, PhysicsWorld, floor/PF graph, Application RNG and player Gear. It does not create another manager, Player, inventory, Save or current-Level global. App callbacks below remain required when reached; this package does not claim live Swamp loading, live drops, completed lethal Kill or GUI pickup.

## Production files for root CMake

Add to `dh2_level_world`: `world_item_live_owner_v5.cpp`, `world_item_frame_v5.cpp`, `world_item_pickup_inventory_v10.cpp`, `world_item_loot_creation_v10.cpp`, `character_kill_loot_connection_v10.cpp`, `loot_pickup_quest_tail_v10.cpp`, `gameobject_online_update_v5.cpp`.

Add to `dh2_game_data`: `loot_inventory_transfer_v10.cpp`.

Rebuild existing `player_equipment_render_owner_v1.cpp` for its approved `loot_inventory_full_v10(bool&,error)` endpoint. Its header adds only a method; no member layout changes. `LootTemporaryInventoryV8` likewise adds only a method. Link existing canonical factory/Spawn/manager/property/base, retained visual/Scene registry, PF/physics/actor runtime, ItemManager/Interact/drop/creation/XP modules. Do not compile duplicate implementations from loader archives.

## Retained ownership and initialization

Keep one `WorldItemLiveOwnerV5` with the World. Construct it with the **same** canonical manager/map, NativeWorld, current collision geometry and obstacle registry, immutable LootTables/audiovisual borrows and `WorldItemLiveServicesV5`. The owner's delivery maps retain canonical Item leases; keys remain produced by the actual manager. Its factory invokes source `Spawn("Item",name,false,true)` with exact Item type3 constructor, declared property defaults, Add, actual condition test and source pending-list append.

`factory.resolve` must call that same manager's `resolve_handle_v4` with the genuine reached NULL-assertion Debug service. `factory.test_enable_condition` uses the same Item base fields and whole `object_enable_condition_v2`: real direct Save+14 quest-sync readiness and real current-Level difficulty if those branches are reached. No independent Level-ready flag is accepted.

`graph_services` fills genuine cache/device/condition/spawn-probability/physical-peer/color/Vox services. The owner installs `world_item_scene_services_v3` over the supplied **one** root registry and `WorldItemPfConnectionV4` over the supplied **one** floor/obstacle graph. It pins each actual receiver/visual/body. `precache` executes all audiovisual rows ×5 (actual cache:145) through source InitOnce and DeSpawn. A failed construction prefix cannot be retried as success.

Do not retain both `RendererItemGraphAdaptersV4` and this owner as independent graphs for the same Item. Its former callbacks are useful inputs for `graph_services`, but replace its record ownership with this owner. Its old Vox adapter was only a prefix and must not be presented as complete Play3D.

## Frame, draw and physical contacts

Create one `WorldItemFrameV5` for each `owner.graph(identity)` or retain them in the root delivery map. Pass actual geometry/path/obstacle/motion/workspace/avoidance borrows, **actual RuntimePolicy facts**, same roots and required subobject world services. Set source absolute application milliseconds before routing. The helper handles IsAtDestination, source Stop and generic GameObject Update without a Character sheet, flags or actor-state string.

The Update order is required begin/profiler/Debug/Application counter → source collision target2e4 Interact, then clear2e4 → path/rotation/subobjects/target → whole RequireOnlineUpdate → idle audio when signed370≥0 → profiler end. `begin_update` and `end_update` are required production receivers. Source constructor-null target node180 is read from the actual base; positive node targets require their genuine position backing. Positive collision Interact requires a bounded same-owner Update→Interact delivery; the historical public Item `interact` guard alone rejects this nested path, so do not swallow it. A source successor for that bounded reentry remains an app integration requirement.

`gameobject_require_online_update_v5` preserves both fresh GetOnline queries, real NetStruct100 NULL gate, hosting/virtual54 branch and sole byte119 write. It supplies no offline value. PhysicsWorld stepping remains the root frame owner. For meshes, borrow `graph.visual().visual()->bres()/scene()/binding()/node_flags()/skinned_meshes()` and root flags from the actual sampled visual. World-projected picking/contact eligibility uses actual `world_item_physical_contact_v4`, canonical type3/Handle and produced byte80; do not add a pickup radius or visible marker substitute.

Physical peer services must map actual WorldObject contexts to canonical published owners, resolve their shared Handle/type, use genuine AsCharacter, produced peer-visible80 and same Character OOI. Unknown contexts fail. No NULL peer owner or invented obstacle flags.

Before a floor/PhysicsWorld replacement, call `detach_physics`; only then destroy the old physics storage. Rebind the same retained owner to new floor/registry. This detaches bodies, and deliberately does **not** reconstruct/re-enable active bodies automatically. Deliver actual source InitAgain/SetPhysical if that lifecycle is reached. On whole World teardown, source manager unpublishes objects; flush the ItemManager before `erased(id)`, which releases physical then visual root and retained delivery leases. Keep the owner/service context alive until all canonical receiver leases are gone. `erased` is not an unpublish substitute.

## Original Inventory Drop and pickup

`bind_menu_drop_world_v9(menuServices,worldLease,owner.pool(),actualDropServices)` installs existing original DropInventory3ec974 transport. Its source is the already transferred real NULL-character inventory from the authored menu operation. It does not clone items or automatically pick them up. Position/destination/scatter use the actual source positions and one Application RNG. Friendly-ID/drop lock stores remain source5000/PlayerInfo678; no inferred player index.

Route `LootInteractRequestV8` first through `WorldItemPickupInventoryV10`. Its fresh `player` callback must return the same live Gear whose inventory.character matches the actual Character. It provides real fullness, potion count/capacity and storage transfers. Normal pickup and transmute transfer use **force=false, convertGold=true**, from original3ed480..490/3ed834..84c, not drop insertion's force=true/convertGold=false.

The new whole NULL-character `transfer_all_source_v10` runs each destination Add → actual gathering quest tail → deletes that source slot, preserving declaration order and vector-end reset after all slots. It then delivers destination AddGold(0), which still performs the source gold notification tail. Failure preserves consumed destination ownership and source slot/deletion prefix; repeat is rejected. Modern safety details are explicit: item ID is captured before destination Add may merge/destroy it, and a moved potion cache is cleared rather than left dangling. Neither is claimed as byte-for-byte legacy behavior.

Connect `after_add_all` to `loot_pickup_quest_tail_v10`: actual IsPlayer → SAME inventory registered gathering-ID list+30 → actual current GS nullable query → literal r1=`v2QuestObjectiveType`, r2=`GatherLoot` → actual AsyncCall with copied event fields. The registered-ID list is **not** inventory contents. FreshInventoryOwnedV4 currently has no live gathering-list producer in its portable projection; retain a genuine source owner/registration lifecycle or return a named required failure. Do not assume empty solely because the projection omitted it.

Unhandled Interact operations remain mandatory app services: actual cast/player/friendly/local queries, AutoTransmute setting and transmutation, font palette/localized text/ShowText, tutorial owner, Debug, trophies, network message, pickup Play3D, tooltip lifetime, `loot_orb_fx`, stat4. `despawn` is owned by this composition and routes to the same145 manager. No accepted empty quest/FX/audio callback is supplied. Successful full-inventory/potion-cap early returns are preserved by the existing whole Interact body.

## Death, authored drops and XP

`WorldItemLootCreationV10` composes existing whole LootCreationV8 into the same pending embedded temporary inventory, using real immutable loot/power/text tables and caller Application RNG. Creation uses actual name/stats/requirements effects; forced non-converting storage retains mandatory Debug/full notifications. Player-count/current-Level queries remain genuine services. `CharacterLootDropV8` still owns source eligibility, fresh World handles, AI flags and killer properties195/196.

Bind its create service to this creation helper and its drop service to `owner.pool().drop(...)`. Then `CharacterKillLootConnectionV10` handles only `kill_drop_loot` from the existing whole CtrlKill owner: borrow actual Character GetLootTable+101c, call DropLootTable(table,victim,attacker,−1,false). No actor-name table fallback. Original Kill calls this when actual Level.loot_gate150 is0 **before contributor events and XP**, then later quest/death tails and outer AI OnDied. Keep XP in the original `kill_distribute_xp` callback, not an independent alive→dead hook that reorders/doubles drops. Real GSLevel/current-Level lifetime, quest queue and all later providers must be present before claiming complete lethal Kill. Never set gate150=1 to suppress unavailable loot.

## Verification

* Original ARM whole TransferInventoryTo:4 vector-size cases plus nonplayer/unregistered/no-GS/positive gathering quest paths. Literal order, copied fields, source deletion and zero-gold tails checked against original instructions with explicitly declared external receiver observers.
* Android native same transfer:71 checks, including exact original item pointer into one real FreshInventoryOwnedV4, same properties/RNG, observed gold notification and failed quest prefix.
* Canonical145 graph:2200 checks with actual itemdrops BRES, source Handle keys/manager/pending list, real Scene/Box2D/PF graph and release/rebind; device/condition/Debug services explicitly fixtures.
* Reusable Item frame: final receipt `android-native-owner-tests/world-item-frame-v5/receipt.json`; actual Crypt BRES/DWLD and body, eight speed6 frames, source Stop/IsAtDestination; external camera/profiler/device/condition/Debug/offline queries explicitly fixtures.
* Pickup quest kernel:20 checks for all gates, exact literal/event fields and required failure prefixes.
* Whole RequireOnlineUpdate:29 native checks covering actual ctor-null NetStruct, both fresh queries, host/ownership/write order and explicit required-provider failures.

Native fixtures compile the current retained visual TU, registry and Item callers coherently. `item-visual-inline-closure-v5.json` records zero imported old RetainedVisual methods, current executable exports and no symbolic DSO binding. These are isolated `/data/local/tmp` fixtures, no app install/input. Full live Item drawing/touch, menu Drop, Swamp and lethal Kill remain integration validations.
