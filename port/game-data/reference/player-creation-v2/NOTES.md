# Fresh-player grants and owned inventory, version 2

This batch executes the original initial-equipment/skill-slot/skill-increment callers and the fixed starting-loot producer. It supplies immutable decoded loot backing and stable owned item/slot identities. It does **not** claim complete campaign character creation, native item effects, automatic equipment effects, or live skill scripts.

## Source and initialization order

All captures bind `libDungeonHunter2.so` SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. The canonical cache ZIP is SHA256 `3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`. Captures under `original/` retain function manifests and instructions; executed probes and original-derived fixtures are separately bound by the freeze manifest.

`Character::_InitEquipment` 0x3b395c checks online host record first. Existing items or Character+0x39c gold take the source Skin continuation. Otherwise it reads property 9, calls AddLoot with `(loot,0,0,-1,false)`, captures the resulting item count once, and tests/auto-equips each index. A callback changing the live item count does not change this captured loop bound.

`Character::_InitSkillsSlots` 0x3b3a90 checks actual saved slot map presence. On the empty path it sets saved row 0 into slot 0, swaps, sets the same saved row into slot 0, swaps back, then increments saved row 0 only if its saved level is zero. A saved row index is distinct from a skill dictionary ID. Both source GetCurrentSkillSet queries return literal zero; that does not prove equipment sets are globally equivalent.

`Character::IncSkill` 0x3bcc58 reads genuine integer property 157, availability and CharacterDesign caps. Its second difficulty query is live. On acceptance it deducts one point, reloads the live savegame/row after that callback, wraps the uint16 saved level, updates all skills, recalculates properties, and stores the low byte of max(GetInt(194),0) as potion capacity. Required-native failure is distinct from normal source false. Missing-save/row Debug continuation remains unsupported.

The executed original cache producer gives all three base classes Level 1, one skill point and potion limit 12: resolved raw values 256, 256, 3072 and source GetInt arithmetic shift by 8. Raw authored Knight skill points zero must not be confused with the resolved class-property result.

| Source character row | Starting Loot | SkillList | First saved row 0 | FaeryList |
|---|---:|---:|---|---:|
| Knight 263 | 165 | 14 | dictionary 7, BashDown | 1 |
| Mage 290 | 174 | 21 | dictionary 14, ColdRay | 2 |
| Rogue 325 | 213 | 27 | dictionary 56, JumpKick | 3 |

Specialized rows 264/265, 291/292, 326/327 have distinct skill lists and raw Loot -1. No fallback is inferred. Source rogue fixed loot contains two separate dagger entries; they must not be deduplicated. Potion quantity 5 comes from the authored item-list entry, not a global default. Random runs once for each weighted list even when it has a single probability-1 choice.

## Owned source path and explicit providers

LootTablesV2 retains the first six actual cache sections, 1322 item rows, 186 item lists and 339 loot rows. NumPowerProbs is a vector of byte vectors. Names/schema are retained; reload while borrowed rejects atomically. Borrow survives destruction of the original loader and caller byte buffers.

FreshInventoryV2 owns stable heap items/slots and supports the genuine fixed/no-subloot/no-power starting tables. Item name, stats and requirements are mandatory ordered services. ItemID is reread after providers before valuation and force-add. Capacity zero genuinely destroys rejected potion instances; signed quantity behavior and minimal-random Debug gate are proved. Unsupported random/powered/subloot/gold/name-variant continuations fail explicitly with the reached prefix preserved. Application RNG seed initialization is caller-owned and unproved here.

Source Character auto-equip 0x3a9fa8 invokes inventory 0x400c84, preserves its result, then property refresh, Skin and status. Inventory 0x400c84 tails 0x4009f8; actual slot selection invokes 0x400634 `_EquipItemToSlot` and 0x4003a4 `_UnEquipFromSlot`. Their requirements/stats/notification/visual effects are **not** supplied by `record_delivered_equipment`, which records only an already delivered genuine effect. The next batch must reconstruct this caller system.

Menu FSStartGame 0x4220a0 New uses the indexed save constructor 0x4655ac, selected class/name, level 1, SaveDate, real SGSave and ApplicationLoadLevel. It differs from Character.InitializePlayerSavegame 0x3b36b0's blank constructor 0x465ae0. Full profile file/campaign/progress loading is not replaced by a synthetic section. In InitPost, SG_Load precedes host `_InitEquipment`; property recalculation/cache precedes host `_InitSkillsSlots`. Other original InitPost effects remain required.

## Proof scope

- Original versus optimized ARM64: 979 loot/reader/RNG cases; 60 complete owned starting-loot cases and 3962 requests; 768 initial caller cases and 2697 requests; 768 IncSkill cases and 5428 requests.
- ARM64 owned inventory runs genuine C++ constructors, decoder, RNG and containers. Heap/libc imports are explicit byte-storage services. Both allocation/free totals are 144222; native effect responses are controlled fixtures.
- ASan/UBSan/LSan isolated replay: 45049 checks, 828 malformed/reentry/failure guards, zero findings. Three actual saved skill-list compositions execute owned storage while update/availability effects are explicitly fixtures.
- The original class-cache producer is an executed getter/reader projection for nine rows, not a complete original menu or InitPost execution.
- No APK, GPU, live inventory, full skill-script, or campaign parity claim.

## Central integration and reproducibility

Add `loot_tables_v2.cpp` and `fresh_inventory_v2.cpp` to `dh2_game_data`, which already owns `items.cpp`. Add `player_initial_grants_v2.cpp` to `dh2_level_world`. Do not add fixture wrappers to production.

Host targets/arguments:

1. `loot_tables_v2_audit`: tests/loot_tables_v2.cpp, links dh2_game_data; arguments `port/game-data/reference/player-creation-v2/fixtures.bin .local-inputs/items-discovery`.
2. `fresh_inventory_v2_audit`: tests/fresh_inventory_v2.cpp, links dh2_game_data; arguments `port/game-data/reference/player-creation-v2/fresh-fixtures.bin .local-inputs/items-discovery`.
3. `player_initial_grants_v2_audit`: level-world/tests/player_initial_grants_v2.cpp, links dh2_level_world and dh2_game_data; arguments `port/level-world/reference/player-initial-grants-v2/fixtures.bin port/level-world/reference/player-initial-grants-v2/skill-fixtures.bin .local-inputs/skill-tables`.

Reproduce isolated host proof with `python port/game-data/tools/run_player_creation_v2_host.py --output NEW_REPORT_PATH`. Exact source lists/compiler/run commands and corpus/input/executable bindings are recorded in the frozen host report. Use a new report path after freeze.

ARM64 owned fixture uses NDK29 `-O2 -static-libstdc++ -fno-fast-math -ffp-contract=off -Wl,-z,max-page-size=16384 -Wl,--pack-dyn-relocs=none` on loot_tables_v2.cpp, fresh_inventory_v2.cpp, items.cpp and tests/fresh_inventory_v2_arm64_fixture.cpp; replay via tests/fresh_inventory_v2_arm64.py. The oracle executes ordinary RELA relocations; disabling packed relocations is a test-link choice, not a source algorithm change.
