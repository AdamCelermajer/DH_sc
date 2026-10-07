# Source equipment caller graph, version 3

This additive kernel recovers the complete selection, equipment-cell and per-set slot-byte caller graph. It borrows actual stable owner/item/slot projections. It does not allocate a second inventory, fabricate native game effects, or modify the frozen v1/v2 owners.

## Recovered source functions

Original ELF SHA256: `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

| Function | Address | Native export |
|---|---|---|
| Character.EquipItemAuto | 0x3a9fa8 | dh2_equipment_character_auto_v3 |
| Inventory.EquipItemAuto tail / _EquipItemAuto | 0x400c84 / 0x4009f8 | dh2_equipment_auto_v3 |
| _EquipItemToSlot | 0x400634 | dh2_equipment_to_slot_v3 |
| _UnEquipItemFromSlot | 0x4003a4 | dh2_equipment_from_slot_v3 |
| HasTwoHander | 0x4001a0 | dh2_equipment_has_two_hander_v3 |
| IsEquipmentSlotTaken | 0x4002d8 | dh2_equipment_slot_taken_v3 |

Source Item.GetItem 0x3f9e08 indexes the immutable 164-byte original row by live ItemID. The native caller receives the exact `type` (+0x58), `slotting` (+0x68), `stackable` byte (+0x1c) projection. The actual cache decoder is independently proven; native callers must retain its immutable backing and provide genuine live IDs/quantities.

GetCurrentEquipSet 0x3fc6a8 uses the signed selected byte for slots 1/2 or a negative request. Every other nonnegative slot uses set 0. Source slot-object bytes +4 and +5 are **slot indices for set 0 and set 1**. The frozen historical ItemSlotV1 field names do not establish those bytes as a set number plus slot number. New EquipmentSlot16V3 names this distinction explicitly; caller mapping must follow it.

## Ordered behavior

Auto-equip captures the source item slot, checks equippability and captures slotting before selecting a destination. Source Character fields +0x1320 and +0x1324 are explicit real caller inputs; the kernel does not invent their class/property producers. Negative slotting -3 selects the first free weapon slot 1/2; -2 selects ring slot 5/6. A -4 two-hander handles off-hand removal, subject to the source capability/type branches. Other invalid slotting returns source false. Normal in-range slotting selects the original destination.

EquipToSlot captures the source equipment set, un-equips the requested and prior matching slots, then applies the two-hand/forced branch. It rereads live item quantity after those callbacks. Quantity 1 stores the actual slot pointer and its captured-set byte. A stacked item invokes Split(quantity-1), writes the original unit into equipment, then tail-calls ForceAdd(remainder,true,true). The source tail branch is 0x400854; its LR belongs to the incoming EquipToSlot caller. Report request `caller=0x400854` denotes that source branch, not a fabricated BL return address.

UnEquip clears the equipment cell before touching the retained old slot. It writes that set's byte -1; only the packed signed16 pair -1/-1 allows stack merging. Stackability is the source low byte. The ordered required path is HasItemInstanceLike -> IsItemEquipped -> live target read -> AddQty(source signed quantity) -> DelItemInstance(old slot's live item). Source callbacks may change selected set or item/array state; already captured slots and items must remain alive until source return. The controlled corpus changes the selected set during callbacks and compares the original resulting captures/writes.

Character.EquipItemAuto always executes UpdateGearsProperties -> Skin -> ValidateHPMp after delivered inventory auto-equip, including normal source false. Its return preserves the inventory result. Required-provider failure is distinct from this source false. Source UpdateGearsProperties itself is 0x3defac -> 0x3df480 -> Recalc(true); those deeper gear/stat providers remain required. ValidateHPMp's exact property branch is captured but is not silently supplied by this module.

## Required integration services and ownership

EquipmentState72V3 is borrowed mutable storage, not a constructor or detached inventory. It references genuine stable EquipmentSlot16V3/EquipmentItem16V3 projections, the live item list/count and two genuine equipment arrays. A future owned-inventory binder must make these writes authoritative on the same native inventory and retain all identities. Merely filling a temporary graph and calling `record_delivered_equipment` is not a ready live equipment backend.

Services are mandatory for reached Split, ForceAdd, HasLike, IsEquipped, AddQty, DeleteItem, UpdateGearsProperties, Skin and ValidateHPMP. Split must genuinely reduce the original quantity and return an owned remainder; ForceAdd must transfer that remainder to real inventory ownership. Match/merge/delete must perform the original owned-container effects. No provider may report success for an unimplemented effect. Fixed starting gear has quantity 1 and does not need Split/ForceAdd, but still requires genuine property/visual work.

The source invalid-index assert/Debug modes are not reconstructed here. These continuations fail -3; malformed native ABI calls fail -1 before writes/providers. Missing/failed required providers fail -2, preserving reached source effects. Output/reserved-field guards are additional native boundary checks, not invented original branches. Pointer identities stay 64-bit. Generic ownership teardown, source raw pointers, campaign save files and complete skill/item scripts remain separate.

## Proofs

- 1200 actual original ARM32 versus O2 native ARM64 cases execute all six source entrypoints. Original GetItem/GetCurrentEquipSet run actual source instructions. The corpus covers captured-set writes, paired-slot guards, stacked splitting, merging/deletion, source false, callback selection mutation and Character effect ordering: 1229 requests.
- 24 additional original/native compositions execute actual starting AddLoot tables 165/174/213 using genuine cached metadata and preserve all 5/5/6 items, including both Rogue dagger occurrences. Their 312 Character-effect requests match. Capability words and selected set are explicit fixture projections, not claimed class constructor proof.
- Sanitized host replay: 78977 checks, 22 atomic/failure guards, 1229 synchronous read-only helper reentries, zero ASan/UBSan/LSan findings. Native storage/effect services remain controlled fixtures; this is not complete original callback-reentry or live GPU parity.
- Frozen production, exact corpus, script, source capture and compiled binary hashes are in freeze-manifest.json. No central build or APK/device was changed.

## Integration / run

Add `port/game-data/player_equipment_v3.cpp` to dh2_game_data. Host target `player_equipment_v3_audit` uses `tests/player_equipment_v3.cpp`, links dh2_game_data and takes `port/game-data/reference/player-equipment-v3/fixtures.bin`.

Reproduce an isolated sanitizer proof with `python port/game-data/tools/run_player_equipment_v3_host.py --output NEW_PATH`. The frozen report contains complete build/run commands.

ARM64: NDK29 clang++, `--target=aarch64-linux-android28 -std=c++17 -O2 -shared -fPIC -fno-fast-math -ffp-contract=off -Wl,-z,max-page-size=16384`, new production cpp only. Replay tests/player_equipment_v3_original.py and player_equipment_v3_starters.py with `--library PATH`. Use successor report paths/scripts after freeze rather than replacing evidence.
