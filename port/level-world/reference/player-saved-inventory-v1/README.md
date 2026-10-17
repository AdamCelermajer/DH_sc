# Borrowed saved Player inventory V1

## Source and attribution

The complete 1024-byte original `PlayerSavegame::__LoadInventory` at `0x46a3a0` is pinned against ELF SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. The capture executes 221 distinct instruction words across the caller and the real eight-byte `ItemInstance::SetValue` at `0x3fbc58`. Its store at `+0x54` precedes the tail call to `_UpdateName` at `0x3fbc5c`.

The original belongs to Gameloft. This composition reuses the selected V4 inventory and V5 ItemPower/ItemPresentation/text/equipment services, preserving existing Adam contribution attribution and the audited upstream `791e961b12233100b303038c961666834f4beb9d`. It adds no inventory, property, Save, VM, timer, progression or item-ID owner.

## Exact borrowed API

`Runtime(Bindings)` borrows the canonical Save, sole `FreshInventoryOwnedV4`, its grouped `PropertyView`, stable equipment descriptor, immutable ItemPower resources and an existing lifetime owner's stable `unique_ptr<ItemInstanceV1>` incoming slot. `load(Bytes, Result*, error)` consumes only the GEAR payload. It reads the Save's Character freshly at original mutation boundaries and requires it to match the inventory's full-width Character identity.

Order: read gold / selection word / item count; SetGold; low-byte selection store; read an Item identifier and both slots, quantity, value, identified byte and power count; create through V4; SetValue / mandatory UpdateName; normalize identified; read each Power name / AddPower with source mode -1; AddItemInstance with both booleans true; set selection zero and equip first slot; restore; set selection one and equip second slot; restore. Item and Power lookup uses the source first `strcmp` match. There is no additional property recalculation, Skin, vitals or random tail.

The narrow V4 `saved_item_effect` routes the two direct Item effects through V4's existing callback guard. `project_current_equipment(uint8_t)` writes its one existing byte. Source temporary 0/1 selection is used before weapon indexing; a raw corrupt saved selection can be retained but is not authorization for another query to index an invalid set.

A failed SetValue/AddPower retains the actual incoming Item; failed AddItem can retain an already transferred Item in V4; failed Equip retains its preceding temporary selection and Item prefix. The borrower does not erase or destroy incoming failure state. Its existing lifetime owner must call Presentation retirement before destruction or text/resource teardown. Existing V4 constructor failure does not publish its local allocation; this is a declared native callee boundary, not a claim about original C++ exceptions.

## Evidence and limits

The actual selected world/game-data/text DSO gate passes 19 original cases (18 synthetic plus the private actual GEAR), 57 comparisons over Knight/Mage/Rogue actual base rows, 26 required/source failure prefixes, eight truncated/assertion read prefixes, 16 guards and 2495 checks. Source TU and VM TU occur once; five text TUs are shared once. The clean gate guards 575 compiler/configuration inputs and 21 reached cache inputs with equal before/after hashes, commands and results.

Actual private GEAR: 250 bytes, six items, one power; payload SHA256 `1924e252a1893717a15149bcbd410aba68b46df8f35de2fd900885218f86269a`. The private campaign file is 9303 bytes, SHA256 `d4d947e32feb7b2ed5b64038195e565b8d5978adbaa86bf58027b47cfe2b43e2`. Actual equipped quantities are one. No private payload, profile name or item names are copied into this reference or repository fixture. Original case bytes stay under the private host-output directory.

Stream transports, string allocation, strcmp, allocation and Item/SetGold/AddItem/Equip callees are explicit original ARM fixtures; only SetValue's actual nested body executes. Native tests use real selected V4 algorithms, V5 Presentation, text formatters and actual cached tables; external Debug and empty visual/world hooks remain declared fixtures. The source null/assert/unsafe-pointer and bounded malformed-string/count domains fail at reached native boundaries. Not all original debug assertion continuations execute.

Full SG_Load4 section orchestration, actual profile file service, genuine startup/locality predicates, InitPost invocation and native integration remain open. This is isolated selected-source proof, not live saved-profile startup or complete campaign loading.

### Existing powered split lifetime dependency

V4 `equip_to_slot` keeps a local remainder. If a powered stack split reaches a failing AddPower localization provider after a successful remainder constructor, the local remainder may be destroyed without calling Presentation retirement. This caller's tested quantity-one saved equipment avoids that path. Preserve the current production checkpoint; do not claim arbitrary powered split failure lifetime closure. The subsequent remediation should borrow the existing caller lifetime owner's stable split remainder slot, preserve quantity/unequip prefixes, and retire full Presentation state before a real destruction. It must not add an inventory/property/power-ID mirror or blanket rollback.
