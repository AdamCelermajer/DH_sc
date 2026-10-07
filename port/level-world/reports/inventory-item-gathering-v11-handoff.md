# Inventory gathering and bounded Item interaction V11

This successor supplies one missing source authority on the existing inventory, one bounded host callback scope, and actual chest table/dictionary bindings. It does not publish a second inventory, world, item pool, character, or Level. Shared production files are frozen after the tests below; rebuild every FreshInventory/Gear/Item consumer together.

## Source gathering list

Original ItemInventory C1/C2 `3ff200/3ff330` initialize the empty list at `+30/+34`. ObjectiveGatherLoot::Register `47eaec` calls its real base Register first, then rereads enabled byte8, character10, and objective-data item ID24. Its inlined registration uses the character's embedded inventory (`character+37c+30`): existing IDs increment byte references; new IDs append a native16 node in order. Unregister `47d6fc` delivers base Unregister first and then `47d5fc`; a byte decrement reaching0 unlinks/frees the node. Both increment and decrement wrap modulo256. Presence remains true with reference byte0. Missing unregister follows the actual assertion policy; the modern receiver requires that policy callback and refuses the legacy fatal NULL-store branch.

`FreshInventoryOwnedV4::gathering_ids_v11()` is the ONE member corresponding to that list. Its empty constructor is a genuine source producer, including the NULL-character temporary constructor. No source binary-layout projection changed; native C++ size/layout did change. The list is neither the bag contents nor a separate quest mirror.

Guarded Gear endpoints on the same existing mutable inventory:

```cpp
const data::InventoryGatheringIdsV11* gathering_ids_v11() const noexcept;
bool register_gathering_id_v11(int32_t id, std::string& error);
bool unregister_gathering_id_v11(int32_t id,
    const data::InventoryGatheringAssertServicesV11& actual_assertion,
    std::string& error);
```

`objective_gathering_register_v11` / `objective_gathering_unregister_v11` provide the whole reached wrapper order through required base Register/Unregister and character-inventory callbacks. Bind those callbacks to the selected character's real inventory/Gear; registration is not triggered by merely querying loot. The base Objective constructor/registration and positive quest lifetime remain required external owners. Do not register arbitrary IDs just to enable events.

Bind `LootPickupQuestServicesV10::registered_gathering_id` to `same_gear.gathering_ids_v11()->contains(item_id)` after validating its character identity. Its empty-list branch is now constructor-backed. Positive pickup preserves existing source order: AddItemInstance(force=false,convertGold=true) → registered GatherLoot async event → delete the transferred source slot → final AddGold, including zero. Actual GSLevel, constants, and owned async delivery remain mandatory when reached.

## Bounded synchronous Item interaction

Original GameObject.Update `38cc78..84` invokes Item's Interact virtual synchronously while outer Item.Update is still running. `RetainedWorldItemObjectV1::interact_from_update_v11(peer,error)` is permitted only inside that same object's `game_update` delivery scope. The historical public `interact()` guard and the inner destructive Interact guard remain intact. The host scope is RAII-reset on return/failure/exception; it is not a source lifecycle/FSM field.

Connect the retained frame directly:

```cpp
frame_services.collision_interact = [weak_item](uintptr_t peer, std::string& e) {
    auto same = weak_item.lock();
    if (!same) { e = "Required retained SAME Item owner"; return false; }
    return same->interact_from_update_v11(peer, e);
};
```

Deliver the same `WorldItemFrameV5::route` for `is_at_destination`, `stop`, and `game_update`; the outer Item.Update performs these calls. The existing frame clears collision target2e4 only after successful interaction. Failure preserves the actual target/inventory/body prefix and the Item owner prevents retry. Required pickup sound, tooltip/FX/trophy/network/despawn/quest receivers are still required. Inventory Drop does not auto-pickup.

## Chest data

See `openable-container-data-v11-handoff.md` for the ready shared `OpenableContainerDataConnectionV11` wiring. Actual source `Arrays::GameObjectDict`, rather than a guessed Visuals namespace, supplies the visual URI. Real cache has68 OpenableContainers and85 dictionary rows. Swamp_Normal_Chest resolves visual47/loot227/sound33 and `data/3D/GameObjects/go_chest_swamp.bdae`; Big/Rotten use actual48/49. The receiver writes SAME base CString290. Genuine canonical graph/scene/PODecor/condition/Spawn/PF/light/audio/DropLootTable callbacks remain the existing external contract.

## Production build additions

Add once to existing targets; root owns CMake:

* game-data: `inventory_gathering_ids_v11.cpp`, `game_object_dictionary_v11.cpp`.
* level-world: `objective_gathering_registration_v11.cpp`, `openable_container_data_connection_v11.cpp`.

Existing FreshInventory CPP must rebuild against the new header; existing Gear and Item owner CPP must rebuild too. Rebuild all callers in the APK, including retained visual consumers changed in the prior coherent closure. No bundled vendor snapshots or duplicate target implementations are needed. `world_item_physical_contact_v4.cpp` remains the separately requested existing support TU for root's next live world integration.

## Verification and limits

* Original ARM whole Register/Unregister sequence:1763 cases,65 native-node allocations/58 frees, byte-wrap/extreme/disabled/absent-ID paths. Original Objective base/allocator/assertion-mode0 receivers are declared fixtures.
* Native list owner:1763 cases/38,281 checks against those original snapshots.
* Native Objective wrapper:7 checks including post-base field rereads and failure prefix.
* SAME FreshInventory constructor/list plus real transferred item identity/property/RNG and positive registered GatherLoot event:80 checks. Current FreshInventory CPP compiled inline. GS/constant/async/notification receivers are declared fixtures, not live app claims.
* SAME retained Item/BRES/Box2D body/frame successor:83 checks. Current Item/visual constructor closure compiled inline; public guard, internal scope, success clear, failed target retention and retry rejection all checked. Cast/IsPlayer/profiler/Debug/camera/online deliveries are explicitly fixture services. This validates the real nonplayer Interact early-return branch; it does not claim full player presentation/quest/FX completion.
* Actual chest table/dictionary bindings:105 native checks using immutable original68/85-row cache.
* Strict ARM64 syntax:all seven changed/new owner TUs PASS.

Receipts are under `reports/android-native-owner-tests/{inventory-gathering-ids-v11,objective-gathering-registration-v11,inventory-gathering-same-owner-v11,world-item-update-interact-v11,openable-container-data-v11}`. The latter successor fixtures link against APK11c8219e but compile affected owners locally; they are isolated native executables, not app/live-pickup acceptance. Source hashes and complete current translation-unit closures are recorded in each receipt. No app install, gameplay input, Level150 override, fake empty quest callback, or extra RNG was introduced.
