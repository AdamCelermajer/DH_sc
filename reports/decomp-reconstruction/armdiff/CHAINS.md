# armdiff function chains (owner C, 2026-10-16)

`tools/trackb/armdiff/chain.py` runs whole call sequences of the game logic on real data in Unicorn, once on the original
`libDungeonHunter2.so` and once on the ARM32 build of the reconstruction, and compares the results.

## Harness design

- **Image.** Every ARM32 object of the game, pydata, core, net and glitch-core-io categories (`build_arm.py` output) plus the NDK
  `libc++_static.a` is linked into one shared object (`_chain/chain.so`). Machine mode `callee="chain"`: a symbol the reconstruction
  defines stays in the reconstruction, anything else binds to the original (Thumb bit preserved). Run with
  `DH2_ARMDIFF_OUT=<dir>` to keep the build out of `build/tb-arm32`.
- **Real state.** `boot` (original only) runs the original's `.init_array` (539 initialisers) and reads all 70 real pydata files
  (142 `Arrays::X::read/readNames/skipNames`) through a `StreamBuffer`; the heap/arena/data snapshot is cached. For the
  reconstruction side the same snapshot is restored, every pointer into the original image whose symbol the reconstruction also
  defines (vtables, functions, globals) is re-pointed at the reconstruction's copy, shared-by-name globals are copied in, and the
  member-name tables (`pydata::NameTable`) are filled from the original's.
- **Services per side** (`chains/services.py`): fake `FileSystem` (vtable slots found per side) serving the game data folder,
  `PyDataConstants` loaded with the original `Load()` loop, `StringManager` with the English pack, `SavegameManager`,
  `ObjectManager`; the original `Character::Character` constructor builds real Characters. Engine services without a model are
  `neuter`ed (`DebugSwitches::load`, `SG_Save`, `GrabAnimFX`).
- **Comparison.** Return values (with optional captured out-structs), non-allocation library calls (file names asked for), named
  scenario values, and the reachable object graph: heap blocks are matched by following pointers through both sides, words are
  compared at the original's offsets (STLport/libc++ containers are skipped with the size deltas of `run.stl_expected_delta`),
  differences are reported with block, class (vptr/tag) and member from the reconstruction's clang record layout, and the pointer path
  from the root. Only words that changed during the chain are compared.
- **Controls.** `control_flipped_byte` (one input byte flipped on the reconstruction side) and `control_mutated_function`
  (`RecalcProperty` replaced by `return 0` in the reconstruction only) must be reported. They are, and are `WILL_FAIL` ctests.
- ctest: `cmake -S tools/trackb/armdiff/chains -B build/own-chain/ctest && ctest --test-dir build/own-chain/ctest`
  (standalone project; the port CMakeLists was being edited by others).
- Small machine/tool changes: `machine.py` (`callee="chain"`, Thumb bit, `track_writes` flag), `build_arm.py` (tinyxml include
  and `-DTIXML_USE_STL`, which fixed 13 units that did not compile for ARM32).

## Chains

| chain | what runs |
|---|---|
| pydata_ai_factions, pydata_read_all | all 70 real pydata files through all 142 Arrays readers; whole object graph + 8,863 member names in 71 tables |
| charprops_load_base, charprops_all_rows | CharProperties ctor, `LoadBaseProperties` + `RecalcProperties` for all 447 CharacterTable rows, every ClassTable row applied, property getters |
| combat_cf | `CF_SetCombatants`, `CalcMissOrDodge`, `CalcCrit`, `CalcBlock`, `CalcDamage` (4 modes), `CalcDotDamage`, Slow/Fear/Push/Hurt/Stun, `GetLevelScaledXP` on 24 x 24 real sheets (seeded `Random`) |
| char_stats | real Character: stat points, `ResetStats`, mana/HP regeneration, `HasMana/UseMana` |
| char_buffs | `PROPS_AddBuff`, `AddDot`, `HandleDots`, `RemoveAllBuffs` for ClassTable rows |
| char_skills | real Character + PlayerSavegame: skill availability, `IncSkill`, skill rows, saved bytes |
| ai_aggro | CharAI aggro table, friend/enemy/attack relations (AIFactions) between real Characters registered in an ObjectManager |
| item_instances, loot_tables, loot_deep | ItemInstance names/stats/requirements from the real text pack; `AddLoot` over every LootTable row (3 seeds/levels/flags in loot_deep) |
| equip_gear | AddLoot + `EquipItemAuto` + `UpdateGearsProperties` on real Characters |
| savegame_sections | PlayerSavegame `__Save*`/`__Load*` sections: bytes, and save-load-save round trip |
| quest_savegame | `QuestSavegame::InitQuests/CompileQuests/PackQuests` on the real v2Quests table (942 bytes per difficulty) |

## Results

See the table printed by the last run in the section below (copied from `build/own-chain/all.log`).

RESULTS_PLACEHOLDER

## Mismatches found

None in the reconstruction. The mismatches the first runs showed were all harness or input errors, each root-caused: wrong offsets I
had assumed for fake objects (`PlayerManager::m_numCharacters`, `Character::m_savegame` 0x14e8), calls on blank objects the original
dereferences (`PlayerSavegame` tables before `_Init*`, `AddDot` without an effect manager, out-of-range `SG_SetMapLocState`; the
reconstruction guards these, documented deviations), an `strncmp` model that demanded a NUL inside `n` bytes, STLport allocation
differences (pool allocator), and a few return values that are void/leftover registers.

## Limitations

- Pathfinding (`PFWorld`) needs scene-graph floors built from the level `.bdae`; not run. `GraphSparse` A* alone has no entry points.
- AI scripts (Lua/Update loops), Character `LevelUp/_GiveXP` (script manager, FX, menus) and `TargetList` searches (needs positioned
  objects) are not covered; `ObjectManager` and real Characters are in place for a follow-up.
- Heap blocks of classes with STL members are compared outside the containers only; allocation order is not compared (STLport pool).
- Full-file savegame load/save (`Savegame::load`, jobs thread) needs the file-system write seam; sections are covered.
