# Lane 13 handoff — containers and interactions

## Source delivered

The assigned code already had a coherent canonical container path. Source `Container` instances enroll into the existing target runtime after the real `InitPost`; openable chests run through the retained canonical openable receiver and generic animator; destructibles use their own canonical receiver and preserve staged interactions; both drop authored loot through the existing loot creation and world-item owners. `source_campaign_item_container_interact_v114` only claims a handled interaction after a registered container or prepared item owner accepts it.

I corrected the locked-chest consumption contract in `openable_container_owner_v1.hpp/.cpp`: the recovered source calls `ItemInventory::TryConsuming(key_id, key_qty)`, so the owner now requests a same-inventory consumer with both values instead of `RemoveItemByID(key_id)`. The service still needs to be connected by the constructor/provider owner; no provider for the previous `find_key`/`remove_key` callbacks existed in the searched production source. Until `consume_key` is wired to the actual Character inventory owner, locked chests correctly fail with a missing-service error. Do not treat them as playable yet.

## IDA evidence

- `0x3a18bc` `OpenableContainer::TryUnlocking` delegates locked containers to `Unlock` only for a `GameObject::IsCharacter` actor; unlocked containers pass through.
- `0x3a1860` `OpenableContainer::Unlock` reads key id at source word 452, checks `FindItem` quantity against word 450, requires byte 1804, then tail-branches to `ItemInventory::TryConsuming(int,int)` with `(key_id,key_qty)`. `TryConsuming` is at `0x3fe658` (not `0x3ffa44`, which is `TransferItemTo`). The ARM assembly at `0x3a18b8` confirms this two-argument call; the previous owner interface omitted quantity.
- `0x3a1904` `OpenableContainer::Interact` performs unlock first, captures CurrentLevel and room/data/actor into a synchronous stack event, raises it only for the local host, calls `Container::Interact`, then increments the actual Character property 218 and checks local-player trophy `open_100_chests` after count 99.
- `0x3a0da0` `DestructibleContainer::Interact` decrements staged timeline hits first; the final interaction raises `DestroyGameObject`, delegates to `Container::Interact`, and updates Character property 217 / trophy `destroy_200_breakables` after 199.
- `0x3a1274` `DestructibleContainer::__EventCallback` raises a second authored destruction event on `opened` with actor 0, then delegates to `Container::__EventCallback`; `fx` calls `DoEffects`, whose body at `0x3a0d68` is a return. The current receiver follows these branches.
- Original ARM bodies and xrefs are in `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/assembly-functions.asm` and `xrefs.jsonl`; pseudocode is under `pseudocode/003a/`. The ABI conclusion above is supported by the callsite registers and callee body, not just inferred types.

## Proposed root binding

The concrete composition point is the openable service lambda in `renderer_campaign_noncharacter_v69.inc`, immediately after `bind_source_openable_interaction_v116(actual_world,get,output.interaction,e)`. Capture the existing provider's `weak` handle. Add the following to `output.container`; this reads and mutates only the same Character record's prepared Gear and its `inventory37c` owner:

```cpp
output.container.find_key=[weak](std::uintptr_t character,std::int32_t id,
 bool& found,std::int16_t& quantity,std::string& e){
 found=false; quantity=0;
 auto self=weak.lock(); auto world=self?self->scope.actual_world.lock():nullptr;
 SourceCampaignCharacterBorrowV62 loan;
 if(!self||!self->current(e)||!world||
    !borrow_source_campaign_character_v62(world,character,loan,e)||!loan.character)return false;
 auto* gear=loan.character->prepared_equipment_v60;
 if(!gear||!gear->ready()||gear->inventory()!=loan.character->inventory37c){
  e="Required SAME actual Character Gear/Inventory37c for Openable key lookup"; return false;
 }
 const auto* inventory=gear->inventory();
 auto match=[&](const dh2::data::ItemInstanceV1* item){
  if(!item||item->id!=id)return false;
  quantity=static_cast<std::int16_t>(item->signed_quantity()); return true;
 };
 if(match(inventory->potion())){found=true;e.clear();return true;}
 for(const auto& slot:inventory->items())
  if(slot&&match(slot->item.get())){found=true;e.clear();return true;}
 e.clear(); return true;
};
output.container.consume_key=[weak](std::uintptr_t character,std::int32_t id,
 std::int32_t quantity,bool& consumed,std::string& e){
 consumed=false;
 auto self=weak.lock(); auto world=self?self->scope.actual_world.lock():nullptr;
 SourceCampaignCharacterBorrowV62 loan;
 if(!self||!self->current(e)||!world||
    !borrow_source_campaign_character_v62(world,character,loan,e)||!loan.character)return false;
 auto* gear=loan.character->prepared_equipment_v60;
 if(!gear||!gear->ready()||gear->inventory()!=loan.character->inventory37c){
  e="Required SAME actual Character Gear/Inventory37c for Openable key consumption"; return false;
 }
 return gear->try_consuming_source_v108(id,quantity,consumed,e);
};
```

`ItemInventory::FindItem` at `0x3fd15c` checks the special potion slot first, then the ordinary item vector in order; the query above preserves that order. `PlayerEquipmentRenderOwnerV1::try_consuming_source_v108` is the existing mutation owner for the same `FreshInventoryOwnedV4`; it mirrors the source `TryConsuming` lookup/quantity checks and delivers actual inventory removal/effects when quantity reaches zero. The callbacks deliberately fail when the character has no prepared Gear/inventory identity match; they do not substitute an empty inventory or a second owner.

## Remaining integration

The changed `consume_key` callback needs a production binding to the same Character `ItemInventory` and must call its source-equivalent quantity consumer, preserving item-removal/effect delivery. The relevant construction/provider files are outside lane 13's writable assignment, so I did not edit them. The current openable interaction adapter also explicitly lacks the original online `IsLocalPlayerHosting` source; offline/local-host behavior is the only evidenced interaction path here.

No build, test, emulator, APK or runtime verification was performed per lane instructions. This is source work only; gameplay remains unverified. Root owns the shared renderer/native-app/CMake/build/APK/ADB integration.
