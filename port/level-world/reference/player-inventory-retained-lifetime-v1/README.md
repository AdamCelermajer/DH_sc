# Retained Item lifetime repair

This cut borrows a stable `unique_ptr<ItemInstanceV1>` slot from the existing Item lifetime owner. It adds no inventory, Item registry, property, Presentation, RNG, Save or VM owner. The sole selected `FreshInventoryOwnedV4` still owns storage and both equipment sets; the existing `ItemPresentationOwnerV5` still owns full Power entries.

## Original order

- `ItemInstance::ItemInstance` at `0x3fc26c` (372 bytes) initializes the Item, then calls Name, Stats and Requirements at `0x3fc36c`, `0x3fc374` and `0x3fc37c`. The retained overload publishes the actual allocation before these callbacks. Required failure keeps that allocation in the caller slot.
- `ItemInstance::Split` at `0x3fc3e0` (180 bytes) decrements the original quantity before allocation/construction, calls SetValue at `0x3fc44c`, sets clone Identified to zero, copies Powers through `AddPower(-1)` at `0x3fc468`, then restores Identified. Source `GetNumPowers` at `0x3f9e80` uses 32-byte Power entries. A failed text callback after Power append retains both the Item and the full Presentation prefix.
- `ItemInventory::_EquipItemToSlot` at `0x400634` (956 bytes) reaches unequip before Split at `0x400808`. Successful Split precedes equipment attachment at `0x400830`/`0x400848` and force-add at `0x400854`. A failed split preserves the preceding unequip and original quantity prefix. A failed force-add may already have transferred the clone to the inventory.
- The existing fixed-loot caller can borrow the same stable slot for construction. It retains its source RNG and query prefixes. Its supported domain remains the existing fixed, unpowered starter transport; random/subloot/powered valuation continuations remain required failures.

## Native lifetime boundary

`RetainedItemSlotV4` is a borrowed control pointer. Constructor, Split, Equip and fixed-loot overloads require an empty, disjoint slot. Saved GEAR reuses its existing incoming slot after primary AddItem transfer. Required failures never erase the pending Item.

`retire_item` delivers the synchronous storage observer while the actual Item is alive, then resets only after successful retirement. Missing or throwing retirement keeps the pointer. Existing merge/delete branches likewise require a retirement observer for stateful services before actual destruction, retaining earlier source quantity/slot writes. The live equipment facade already requires this observer and remains stateful.

Legacy temporary wrappers require the explicit default-false `stateless_temporaries` contract: callbacks retain no Item pointer and create no external per-Item state. Missing retirement does not imply this contract. Reached constructor/split allocation is rejected without it; quantity-one Equip still follows the source branch without a temporary. Legacy live-facade positive splits require a retained caller integration when reached. They are not silently accepted as stateless.

## Proof limits and attribution

The ARM runner executes original valid constructor, Split and Equip paths and the real SetValue store/tail. Text, full 32-byte Power append, Debug, allocation/string/libc transports are declared fixtures. Failure injection proves caller prefixes at required callee boundaries, not original C++ exception behavior. The pinned ELF SHA-256 is `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

Actual stock metadata has no stackable equippable Item. Only row 664's Stackable field is set to one for this source branch; all other metadata and the Knight, Mage and Rogue property/class tables remain actual. The selected host gate also replays saved GEAR (including the private campaign fixture), initial equipment, V4, V5 gear, V7 loot, live equipment and shared text regressions. Private profile/GEAR payload bytes stay outside the repository.

Gameloft original behavior and Adam's pinned `791e961b12233100b303038c961666834f4beb9d` inventory/equipment/power/text contributions retain their attribution. This is selected source composition evidence. It makes no new Android, menu, campaign SG4/InitPost or live gameplay claim.
