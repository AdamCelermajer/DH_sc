# Character combat queries

This module reconstructs read-only `Character::CanRangeAttack`, the equipment query it calls, and `Character::HasComboAttack`. Original instruction execution against optimized ARM64 passed 4,401 comparisons. The isolated ASan/UBSan replay passed the same corpus, 448 additional comparisons through the native decoded animation-table bridge, and 15 malformed-input guards, with zero mismatches or sanitizer findings.

The original oracle is `libDungeonHunter2.so`, SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. [original-functions.json](original-functions.json) binds eight captured routines to their bytes; [original-functions.asm](original-functions.asm) contains their disassembly. Seven routines execute in the query corpus. `GetAnimStance` was captured for discovery and is not called by the actual combo query.

## Range capability and equipment lookup

`Character::CanRangeAttack` at `0x3a4d3c` reads the signed word at Character+`0x1078`. The actual 224-word property schema names index32 `RangeProjectileID`; the cached payload starts at +`0xff8`. Every value except `-1` returns true immediately, including zero and other negative values. This branch does not inspect equipment or the item table.

For `-1`, it passes the embedded inventory at Character+`0x37c` to `0x400014`. The original symbol here is **ItemInventory::CanRangeAttack**, whose body tailcalls `ItemInventory::HasRangedWeapon` at `0x3fffa4`; it is not an EquipSet method returning a supplied capability flag.

The equipment path is:

1. Call `ItemInventory::GetCurrentEquipSet(1)` at `0x3fc6a8`, which reads the signed byte at inventory+`0x2e` for this argument.
2. Select the 12-byte inventory vector entry from inventory+`0x14`, then read its pointed-to equipment-set main-hand reference at +4.
3. An absent main-hand reference returns false. Otherwise, dereference it to an `ItemInstance` and call `GetItem` at `0x3f9e08`.
4. `GetItem` reads the item ID at instance+4 and selects the global ItemTable row with stride164.
5. Read row+`0x58`, word22. Type4 returns true. Other types make a second original `GetItem` call and return true only for type5.

The native query uses a borrowed read-only inventory, reference, instance-ID and ItemTable projection. It tests the same type4/type5 predicate without repeated global lookup because these inputs are immutable. It does not assign an inferred weapon name to either numeric type. The test constructs genuine original pointer levels and global ItemTable storage; no equipment result or `GetItem` return is mocked. Its ten item types are explicit resolved fixtures, not an asserted decoding of the original serialized item database.

## Combo table lookup

`Character::HasComboAttack` at `0x3a346c` calls `GetCharAnimTableId` at `0x3a3228`. That getter reads Character+`0x1000`, cached property index2 `AnimTable`. A nonnegative value below the original character-animation-bank count is used; any other value falls back to bank17.

The query uses a 160-byte character-animation-bank stride and reads bank+4. This is the first decoded character-animation field, `Attack`, rather than a stance-adjusted field. A negative attack-sequence ID or one at least the sequence count returns false. Otherwise the sequence row has stride20 and its type at +`0x10` must equal1. No `GetAnimStance`, runtime scheduler depth, live step index, or selected clip participates in this query.

`dh2::character::has_combo_attack` connects this lookup directly to the existing `data::AnimationTables` loader: it reads `characters[bank].fields[0]` and `sequences[attack].type`. The host fixture loads the exact bundled original assets and checks this bridge for all 448 resolved character-property rows.

## Proof and input boundaries

[query-fixtures.bin](query-fixtures.bin), SHA256 `341a2ea3b6d9af64a3e3354e6b5921c56bb8e654ab925be61cc236d6e705ebb8`, stores the original results. Its CQC1 header contains case/item/bank/sequence counts, followed by resolved item types, actual bank Attack indices, actual sequence types, and nine-word input/result records. `INT_MIN` override words retain the real table value. The corpus includes:

- 2,142 range queries, including property short-circuits, both equip-set selections, absent main hands and both accepted item types.
- 1,694 direct inventory queries over the same equipment fixtures.
- 565 combo queries over all real bank IDs, fallback17, bounds/type overrides and all real character rows.

The actual source functions execute against concrete memory for the cached properties, ItemTable, CharAnimTable and AnimTable globals. The ARM64 comparison executes the compiled native exports. The reports bind source, manifest, oracle, corpus and asset hashes. The host binder also snapshots all its compiler inputs before and after an isolated build, records the executable hash, checks all eight table assets byte-for-byte against the authorized cache, and runs with ASan/UBSan enabled.

The native C API returns source0/1 for valid borrowed inputs and `-1` for malformed pointer/alignment/index/count contracts. These checks are new native boundary behavior; the original unchecked invalid pointers/indices are not a parity domain. Short-circuited unused spans remain permitted. The caller must still provide live, sufficiently large read-only storage; the API does not own inventory or items.

Original item serialization, inventory construction/equipment mutation, property resolution timing, and complete Character/AI lifecycle are outside this query module. The provided property words and resolved item rows make that boundary explicit. This evidence establishes the query algorithms and native table bridge, not full inventory lifecycle or combat/application parity.

## Reproduction

Build the optimized ARM64 oracle with `tools/build_character_combat_queries_oracle.ps1`, then run `tests/character_combat_queries_differential.py` with `--engine`, `--library`, `--manifest`, `--assets`, `--report` and `--reference-output`.

Run `tests/character_combat_queries_host.py --rebuild` to compile and bind the isolated sanitizer proof. The CMake-compatible test source is `tests/character_combat_queries.cpp`, linked with the new query source and `dh2_game_data`; its arguments are `<query-fixtures.bin> <assets-directory>`.

Reports: `reports/character-combat-queries-arm64-differential.json` and `reports/character-combat-queries-host-audit.json`. No renderer, CMake, APK, emulator, or existing coordinator changes were made for this bounded module.
