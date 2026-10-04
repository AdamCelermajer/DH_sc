# Owned player savegame and inventory v1

This batch reconstructs the source-owned fields used by the original HUD and the actual named profile readers that produce them. It is a native 64-bit owner, not an ARM32 runtime. It does not manufacture a newly created campaign player.

## Source and executed evidence

Original ELF SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. Focused function manifests and exact disassembly are under `captures/{core,producers,queries,readers,faeries,destructors}`. Each manifest binds original function bytes, address and size.

The actual original constructors, STL saved-skill map algorithms, stream/string readers, faery field operations, inventory insertion/removal and campaign index map execute in the Python gold producers. Allocator/stream storage is an explicit service. Item name/stats/requirements/powers, equipment, fullness and notification effects are controlled required services; no successful inventory load claims their deeper game behavior was reconstructed.

Final-source sanitizer replay compares 3,720 original-derived snapshots/readers: skills 1,536, saved faeries 864, inventory loads 72, potion operations 768, campaign indexes 96, name/level/class readers 384. Inventory/potion service ordering compares 2,979 requests. Skill/faery query checks total 71,784. Host reports bind each actual executable, all tested source and input bytes. Optimized standalone ARM64 executes 3,406 exported skills/field/scalar cases and 96 campaign-index cases. Both standalone NDK ABIs are ELF64 with 16 KiB LOAD alignment. Whole STL owners are host-executed; this is not packaged instruction or device proof.

## Constructor and caller ownership

`PlayerSavegame` blank C1 `465ae0`/C2 `465c40` starts slot -1, level 0, class -1, empty name, no saved skill rows, two empty slot-map headers, three null faery arrays/counts, current faery IDs 0, unlocked difficulty 0. The indexed/loading constructor `4655ac` instead writes level 1 and enters `SG_Load`; its outer file/creation behavior must not be replaced by the blank constructor plus guessed defaults.

`Character::InitializePlayerSavegame 3b36b0` allocates 0x198 and calls the blank constructor, stores it at Character+14e8 and invokes `SG_SetPlayer 3bb754`. That method stores the actual Character in savegame+10/+114/+174. Native `set_character` projects this identity using `uintptr_t`; the caller owns the real Character lifetime.

`_InitSkills 469764` only initializes when source rows+80 are null, obtains the real `GetCharSkillList 3bc5fc`, allocates eight-byte rows with ID/level0/flag0 and clears both maps. The Character getter reads its cached skill-list property and uses source fallback list 3 on invalid ID. Native `initialize_skills` requires the caller's genuine decoded list; it does not choose a default list. Slot values are saved row indices, not skill dictionary IDs. Source `GetCurrentSkillSet 3fc6a0` is literal 0, so lookup/assignment uses map0. Assignment removes all previous entries with the same row, writes the signed slot key, then requires `CharAI::UpdateSkills 3d8a04`; deleting with row UINT_MAX skips that callback. Provider failure preserves the reached map write.

`_InitFaeries 4694c8` allocates literal five four-byte rows for each of three difficulties, only for missing arrays. Rows store state at byte0 and u16 level at +2. Constructor IDs are zero; this does not mean an authored ready faery has been selected. Explicit difficulty is required at the native owner boundary. Character wrappers that accept -1 use the separately owned source global CurrentDifficulty; missing savegame has its own original wrapper behavior, which this owner does not replace.

Player death is a live Character/CombatActorState field, not an invented saved-profile boolean.

## Genuine campaign bytes and ordered loading

`Savegame::_cacheFile 315ad0`, false flag, reads count then repeated `[u32 payload-size][4-byte tag][payload]`. `PlayerProfileIndexV1` owns the bytes and all raw ordered records, plus the source C-string tag identity map with duplicate last-write behavior. Embedded NUL in the four-byte tag is retained raw but terminates lookup identity. Borrowed snapshots remain alive after the facade dies, and reject reload while borrowed. Malformed spans reject atomically; the C ABI preserves a reached delivery prefix. Invalid seeks are explicitly rejected before publishing an unsafe native span, rather than pretending to reproduce the original unchecked continuation.

The original valid-file corpus executes the actual original STL index. Corruption marker -1, source short-file handling, `.bak` fallback, filename/slot selection and file/save ownership remain separate required services. Settings `dh2_settings.savegame` is a different raw option stream and must not be used for these campaign sections.

`load-order-original.json` executes `_Load 464f4c` for nine flag combinations with named reader dispatch and initialization as explicit services. Exact order:

- bit1: PNAM, PLVL, PCLS, PDFL, LNAM, LEPT, LUSP.
- bit2: InitLevelStates, InitSkills, InitFaeries, quest initialization at +b8, then +118.
- bit4: LVLS, SKIL, FAES, CFEE, QEST, PROP, GEAR, FTVL.
- bit8: SKIL again; bit32: PROP; bit16: both quest initializations then QEST.

GetOnline `7fd794` returns a borrowed source object. The probe supplies byte+5 false and records a genuinely null CFEE load callback; it does not supply an online manager. The nonnull branch also depends on the source Application player byte+71b. Do not turn that predicate into a constant online capability.

PNAM `4698dc` and PCLS `469d88` use the original stream string format: length includes terminal NUL; positive payload copies all preceding bytes including embedded NUL, while class lookup uses C-string comparison against actual cache names. PLVL `4689a0` copies a signed 32-bit word. PDFL `468968` writes the first word to shared CurrentDifficulty BEFORE reading the second word into this savegame's unlocked difficulty; the native loader requires that store service. FAES `4691d0` writes current ID then checks count against five; mismatch returns immediately with that prefix preserved. Valid rows consume u16 level followed by one state byte. CFEE loader `468cd4` is available only when the source dispatch actually selects it.

## Inventory ownership and reached services

`ItemInventory C1 3ff200`: two nine-slot equipment sets, no potion, gold0, gold limit INT_MAX, potion capacity signed8 -1, selected equipment0. `ItemInventoryV1` owns an immutable copy of the actual ItemTable, heap-owned ItemInstance/ItemSlot objects and stable equipment/potion pointers. A blank inventory is not a ready Player inventory.

`__LoadInventory 46a3a0` reads all three header words before SetGold effects: raw gold, selected equipment, item count. It then reads each identifier, both saved equipment slots, quantity, value, identified byte and power count. Native item lookup uses the actual decoded identifier table. Construction `3fc26c/3fc494` stores low16 quantity and requires Name -> Stats -> Requirements. SetValue `3fbc58` performs another Name refresh. AddPower remains a required provider. `_AddItemInstance 3ff5d4` receives force=true/notifications=true: no fabricated stacking. Potion-capacity0/type14 destroys; type13 credits gold and destroys; other types append a stable slot, then require fullness/notification services. Equipment restore temporarily selects set0/set1, invokes the source equip service, then restores the previous selected byte. Only that real provider may call `record_delivered_equipment` after genuine checks/recalc/visual work.

`SetPotionQty 3ffc40` missing-potion branch resolves literal `Potion0` in the real ItemTable and constructs it EVEN for quantity0. Existing nonzero input stores low16 (thus 65536 can retain a quantity0 item); input0 clears potion before required destruction/removal. `RemoveOnePotion 3fe878` compares SIGNED16 quantity: >1 subtracts, otherwise removes, including negative stored quantities. No potion99 or default potion grant is supplied. Gold/property cap producers, generic loot, equipment checks/recalc, item text/stat/power computation, online notifications and source Debug continuation remain mandatory external backends.

Guarded malformed indices/counts, negative Debug-continuation setters, unsupported reentrant inventory mutation and equipped-potion dangling removal fail explicitly. Caller services may inspect live state and preserve reached writes, but unsupported reentry is not silently accepted.

## Integration and lifetime

Add `player_savegame_v1.cpp`, `item_inventory_v1.cpp`, `player_profile_index_v1.cpp` to `dh2_game_data`; existing `items.cpp` and `skill_tables.cpp` are dependencies. All three owner types are isolated from GL and Android. Keep the actual Character, savegame/inventory owners and required service contexts alive while HUD borrows their rows/maps/item pointers. Profile `Borrow` owns its immutable bytes.

Six standalone audit targets each link `dh2_game_data`:

| test source | command arguments |
| --- | --- |
| tests/player_savegame_v1.cpp | reference/player-savegame-v1/fixtures.bin .local-inputs/skill-tables |
| tests/player_faeries_v1.cpp | reference/player-savegame-v1/faery-fixtures.bin |
| tests/player_metadata_v1.cpp | reference/player-savegame-v1/metadata-fixtures.bin .local-inputs/combat-data/character_classes_pyarraynames.bin |
| tests/item_inventory_v1.cpp | reference/item-inventory-v1/fixtures.bin .local-inputs/items-discovery |
| tests/inventory_potions_v1.cpp | reference/item-inventory-v1/potion-fixtures.bin .local-inputs/items-discovery |
| tests/player_profile_index_v1.cpp | reference/player-profile-index-v1/fixtures.bin |

Paths in the table are relative to repository root except abbreviated `reference/` and `tests/`, which mean `port/game-data/reference/` and `port/game-data/tests/`. Exact executable paths, CLI arrays and hashes are in `reports/player-inventory-v1-host-audit.json`.

The next connected prerequisite is actual fresh-player/campaign creation: requested class/name/level, source skill list and slot assignments, actual initial equipment and potion grants, profile slot/file selection and remaining named sections. Those producers must feed this owner; neither constructor nor controlled test service supplies them.
