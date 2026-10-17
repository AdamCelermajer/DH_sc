# Authoritative item/inventory owner V4

`FreshInventoryOwnedV4` owns the actual item vector, stable heap slots/items, both nine-slot equipment arrays, potion pointer, gold and source constructor fields. Its loot selection is reproduced from frozen V2 on immutable `LootTablesV2`; equipment bodies are directly implemented on that SAME owned graph, not a borrowed V3 mirror. V2/V3 production, tests, gold and reports remain unchanged.

## Native integration

Add only `port/game-data/fresh_inventory_owned_v4.cpp` to the existing data target. It reuses existing `items.cpp`, `loot_tables_v2.cpp`, `item_inventory_v1.cpp`, `player_savegame_v1.cpp` and `skill_tables.cpp`; do not compile duplicate dependencies. Include its header. Construct with the actual Character identity, retained LootTables borrow, live RNG, genuine signed potion capacity and the SAME `shared_ptr<PropertyState>` used by the Character. One initial owner has empty inventory/equipment, selected set0 and gold0; this constructor does not claim campaign initialization.

The actual cached Character sheet begins at Character+ff8 after header+ff4. Source equipment reads Character+1320 and+1324 directly: native resolved[202] `Special_Equip_Main_In_Offhand` and[203] `Special_Equip_2H_In_One`. These are live RAW cached values, not scaled `GetInt`. No source caller flag values are invented. Explicit project_* setters represent caller field writes only. Constructor/gear producers and class selection remain separately proven modules.

`OwnedInventoryServicesV4::invoke` must deliver the reached genuine effects. Names/stats/requirements, AddPower, player/difficulty discovery, gold notifications, full-inventory notifications and Character gear/Skin/HP-MP effects are REQUIRED. The context can read `table()`, item fields, the live shared properties, equipment and stable identities. Missing/failed effects return false with the source-reached storage/scalar prefix retained. Native item construction/destruction, vectors, splits, matches, merges, fullness count and equipment writes are executed here. `observe_storage` is optional, cannot veto/replace any of those bodies, and is for diagnostics. Its caller field is a source branch label, not a promise of normalized call-LR proof for every storage instruction.

The service must actually apply AddPower effects to its supplied item; V4 does not fabricate full32-byte Power instances from IDs. Actual starter cache rows have no powers. Names/stats/requirements need the real StringManager/design/effect callers, not blank formatting. Character auto-equip ALWAYS performs UpdateGearsProperties3e08a8→Skin3a999c→ValidateHPMp3bd140 after capturing its inventory return, even return0. Those bodies remain mandatory providers.

`create_item`, `split_item`, `add_item`, `equip_to_slot`, `unequip_from_slot`, `auto_equip`, `character_auto_equip`, queries and gold operate on the one graph. AddItem input ownership is consumed by native append/merge/deletion; unconsumed input remains with caller on failure. A split updates the original BEFORE constructing its clone. Clone value is copied after constructor effects, identified0 during powers, then original identified reread. Equip captures its set before effects, and writes that captured set even if callbacks change selected. Slots store two independent signed indices, one per set (source bytes+4/+5), not a set/slot pair.

Source-valid indices and live identities are required. Unrecovered original assertion/Debug continuations fail explicitly. Native destructive provider reentry rejects before mutation; read-only queries and synchronous selected/cached-property changes are supported and tested. Callers must keep the owner alive through callbacks; source-invalid deletion of still-equipped slots is rejected. There is no claim of arbitrary allocator/destructive-reentry parity. Owned native RAII cleans unreturned transient allocations on provider failure; that native safety policy is separate from source normal-path parity.

## Original evidence

The ELF is SHA36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80. New hash-bound captures under `original/` contain:

- Auto4009f8, Character wrapper3a9fa8, Equip400634, UnEquip4003a4, HasTwo4001a0, IsEquipped3fdaf0, HasLike3fe1cc and current-set3fc6a8.
- Split3fc3e0, AddQty3fa17c/SetQty3fa0e4, AddItemInstance3ff5d4, DeleteItem3fe7d8 and ItemCtor3fc26c from the prior exact-source captures.
- New fullness3fe330: DebugLoad337888@3fe364 then GetSwitch337a88@3fe3a8 `InfiniteInventory`; truthy switch or source unlimited byte skips limit, otherwise actual slot count>99.
- SetGold3fdfd8/AddGold3fe164 with explicit wrapping signed comparisons. Notifications stay required after scalar store.
- Equality3f9d78 compares ID plus ordered Power IDs, not quantity/value/name; D1 3faa64 destroys owned power/string storage, D0 3faab0 then operator delete310440. No external game effect is replaced by an accepted destructor fixture in V4.
- Item Name3fb754, Stats3fb290, Requirements3facdc, AddPower3fbc60 and StringManager::parse508ef4 are captured as remaining actual effects. parse508ef4 is not labeled identical to recovered parseEx509aec merely because both parse strings.

Original execution corpus:84 sessions/1720 steps/10828 ordered explicit effect requests.56 cases use entirely actual metadata;28 explicitly modify ONLY Longsword01 ID664 Stackable for branch testing. Actual cache has no stackable equippable rows, so this synthetic branch is not an authored gear claim. All three original starter lists are retained, including duplicate Rogue daggers. Corpus covers potion split/merge, force vs merge, gold clamps/debits, source signed16 quantity truncation, count99/100/102 fullness, unlimited/Debug gates, both sets and raw capability predicates. Four callback cases change the live selected set/cached flag during clone construction; captured equipment set ordering is compared to original.

Optimized standalone ARM64 executes the owned C++ graph and libc/STL byte/allocation services;419096 allocations and419096 frees. All1720 state/request comparisons PASS. Host ASan/UBSan/LSan:78265 checks,1720 original steps,240 actual native item deletions,952 actual fullness queries,1461 native guards/readonly callback probes, zero findings. It additionally replays all60 unchanged original V2 loot gold cases. Only that historical regression omits newly genuine fullness Debug calls and logs the corresponding old controlled-fullness storage observation; the NEW V4 corpus compares the original real fullness body and Debug order. Name/Stats/Req/Character effects are controlled fixture providers in these reports, not completed game services.

## Reproduce

Standalone sanitizer build/report:

`python port/game-data/tools/run_fresh_inventory_owned_v4_host.py --output NEW-REPORT.json`

Host target `fresh_inventory_owned_v4_audit`: source `port/game-data/tests/fresh_inventory_owned_v4.cpp`, link the data target containing V4. Arguments:

`port/game-data/reference/player-inventory-owned-v4/fixtures.bin port/game-data/reference/player-creation-v2/fresh-fixtures.bin .local-inputs/items-discovery`

Original gold generation (writes this V4 reference only): `tests/fresh_inventory_owned_v4_original.py`. Optimized oracle uses NDK29 targetaarch64-linux-android28, O2/shared/PIC/static-libstdc++/no-fast-math/ffp-contract=off, test wrapper `tests/fresh_inventory_owned_v4_arm64_fixture.cpp` plus V4 and listed source dependencies; pass `-Wl,--pack-dyn-relocs=none` and16KiB max-page-size. Replay:

`python port/game-data/tests/fresh_inventory_owned_v4_arm64.py --library .local-inputs/player-inventory-owned-v4/libfresh_inventory_owned_v4_arm64.so --output NEW-REPORT.json`

This is an isolated native owner proof, not an APK/device or completed menu-created Player proof. The remaining coherent next connection is real Item presentation/powers and Character gear/property/Skin effects; current APIs cannot silently accept those as done.
