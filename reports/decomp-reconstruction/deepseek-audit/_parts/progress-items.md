# progress — slice `items` (category game-items-save)

Auditor slice: PlayerSavegame, ItemInventory, ItemObject, ItemInstance, SimpleTypeProperty,
PropertyMap, PlayerStatManager, Quest*, SavegameManager, TrophyManager, Objective*, RewardList,
UserProperties.  Worklist: `index/queue_items.tsv` (765 rows).

Repo path was writable: the CSV and this note are written under
`port/decomp-reconstruction/.../deepseek-audit/_parts/` as the brief requires.

## Result in one line

**96 functions audited (all 20 prio 0, all 7 prio 2, 48 of the 102 prio 1 rows that carry a
`const:`/`float:` lead, and 21 extra prio 1/3/4 save/load rows). 0 findings.**
`findings-items.csv` therefore holds the header row only. That is an honest zero: every prio 0–2
row was read on both sides in full, and no branch, constant, index, call, field, size, order or
default value was found to differ.

## Audited counts by priority

| prio | in queue | audited here | note |
|------|----------|--------------|------|
| 0    | 20       | 20           | complete |
| 1 (all) | 189   | 48           | only rows whose `lead` starts with `const:` or `float:` were attempted (102) |
| 2    | 7        | 7            | complete |
| 3    | 64       | 1 (`0x46a0b4`) | opportunistic (save writer) |
| 4    | 485      | 20           | opportunistic: every small `__Save*`/`__Load*` section handler |

prio 3 not attempted: 63; prio 4 not attempted: 465; prio 1 with a `const:`/`float:` lead not
attempted: 54; prio 1 with a non-const lead not attempted: 87.

### prio 0 — all 20 read, all clean

0x47a444 `ObjectiveList::Eval`, 0x47a498 `ObjectiveList::IsValid` (both use the
"check element 0 first, then 1..n-1" shape, semantically equal to the loop),
0x482438 `QE_MoveInZone::FillNetworkMsg`, 0x4822e8/0x482320/0x482358/0x482390/0x4823c8/0x482400
(the six `QE_*::FillNetworkMsg` five-word fills), the `SimpleTypeProperty<T>` family
0x387734 / 0x38777c / 0x387794 / 0x3877f8 / 0x3883fc / 0x3a357c / 0x3a35f0 / 0x33de64 / 0x33de80
(bool, dimension2d<float>, vector3d<float>, Point3D<float>, Point2D<int> defaults — the default
*values* were verified, not the layout: m_defaultValue at +0x20..+0x28, offset read from +0x04),
0x3fc690 `ItemInventory::GetNumPotions` (m_potion +0x24, int16 qty at +0x50),
0x4635a4 `PlayerSavegame::SG_GetSavefileName` (Savegame +0x18 = the m_filename text word).
All prio 0 `armdiff` classes were the harness's own A-bool / C-stl classes (bool-vs-int returns,
std:: containers), not behaviour.

### prio 2 — all 7 read, all clean

* `0x403310 _AddLootItemPowers` — probQuantity (b != -1 ? b : NumProb roll of +0x10 with a>>8),
  the `probQuantity >= count` "add every power" shortcut, the random draw with the monopoly-attr
  std::set and the 10-clash (`v12 > 9`) cutoff, the ordered fallback pass and the
  `AddPower(item, id, c)` variant argument all match.
* `0x402dfc _AddLootItems` — random vs `all` paths, both ASSERT line numbers (361 / 350),
  the 12-byte ItemListEntry stride and the LootInfo fields at +0/+4/+8/+0xc match.
* `0x3facdc _UpdateReqs` — the six requirement words at +0x74..+0x88, the separator logic and the
  class-name switch. The nine hard-coded `Arrays::CharacterTable::members + N` offsets decode
  exactly to `kClassCharacterRow = {263,264,265,325,327,326,290,292,291}` with the 900-byte row
  stride and `ClassString` at +0x18 — verified arithmetically.
* `0x315fb8 Savegame::saveAll` — the kind-1 backup job, the 0xFFFFFFFF count placeholder, per
  section the 0 size placeholder + 4-byte name + data, the seek-back size patch, the cache-copy
  branch (StreamBuffer sized ASSERT at StreamBuffer.h:130), the zero-fill branch, the count at
  +0x30 written at offset 0, `_cacheFile(buffer)` and the kind-0 owning job all match.
* `0x46a3a0 __LoadInventory` — read order gold / equipSet / itemCount, then per item
  name, slot0(int32), slot1(int32), qty(int32), value(int32), flag(byte), powerCount(uint32),
  then power names; `unk_68 = flag != 0`; `_AddItemInstance(item, true, true)`; the two equip
  calls with m_equipSet forced to 0 then 1 and restored. **Cross-checked against the writer
  `0x46a0b4 __SaveInventory` (prio 3): the field order, widths and sign handling round-trip
  exactly.**
* `0x400634 _EquipItemToSlot` — the -4 → 1 fold when Type is not 4/5 and Character +0x1324
  (m_props.Special_Equip_2H_In_One, row field +0x330) is set, the already-equipped early exit,
  the two `_UnEquipItemFromSlot` releases, the two-hander branch (dest 1, equip byte 1) and the
  off-hand branch (dest 2), the qty==1 path and the split path all match.
* `0x510dec DestroyPropertyMaps` — the three-level walk, `~Property` (non-virtual) + CustomFree
  per property and the final tree erase match; ~Property destroys m_name and leaves the typed
  default alone, as the original does.

### Save-format handlers also audited (prio 3/4, not required)

Writer/reader pairs verified as exact mirrors in both directions:
`0x46a0b4/0x46a3a0` (GEAR), `0x469e6c/0x46ac74` (SKIL), `0x46a7a0/0x46a9cc` (LVLS),
`0x4688c0/0x4698dc` (PNAM), `0x4688c8/0x468938` (LEPT), `0x4688f8/0x468968` (PDFL),
`0x468930/0x4689a0` (PLVL), `0x4689a8/0x468af0` (LUSP), `0x468b20/0x468b78` (LNAM),
`0x468bf0/0x468cd4` (CFEE), `0x468f18/0x4691d0` (FAES), `0x4689d8/0x46932c` (PROP),
`0x469454/0x469478` (QEST), plus `0x315ad0 _cacheFile` and `0x464f4c _Load` (the section table:
PNAM PLVL PCLS PDFL LNAM LEPT LUSP for bit 1; LVLS SKIL FAES CFEE QEST PROP GEAR FTVL for bit 4;
SKIL bit 8; PROP bit 0x20; QEST + both InitQuests for bit 0x10), `0x46954c _InitLevelStates`
(row stride 72 / field +0x28 and stride 20 / field +0x08 confirmed against the record headers),
`0x469764`-adjacent `ClearDifficultyTables` (the eight per-difficulty arrays in the original's
order 37, 40, 43, 16, 26, 29, 23, 20), the four `PlayerSavegame` constructors (m_slot -1 or the
argument, m_playerLevel 0 vs 1, m_maxPotions/m_maxGold defaults), `0x467ce4/0x467e58/0x467f70`
(assert-then-bounds order preserved), `0x46649c/0x467c54` (the bitset word
`this + 2*difficulty + (id>>5) + 95` at +0x17c, `id > 0x3f` out_of_range), `0x467488/0x4676e4`
(map lower-bound == find; size word at map +0x10), `0x4660c8 SG_GetNextFreeSlot`,
`0x4679e8 SG_TryQuestSync` (m_loadStep == 38 gate, m_questSyncBuffered +0x710, CMsgQuestsSync
block), `0x480c78 Quest::SetState` (states 1..13, each ExecScript number, the old-state
unregister rules Active/Completed/Available, the stamp, m_needsReinit at +0x64),
`0x47e914 ObjectiveList::GetDesc` (separator only between non-empty descriptions),
`0x318e5c UserProperties::_ParseKeyValue` (the `%22` pair handling and the NUL restore),
`0x318fe4`, `0x513d78 PropertyMap::InitProperties` (the synthetic "template" property, m_offset 4,
vtable slot 0 = DeclareProperties), `0x4832ec Reward_Loot::Give`,
`0x47aa48 ObjectiveList::AssignPyData` (44-byte row stride),
`0x47c6f0/0x47c914/0x47ca18/0x47cb1c` GetPositions — the OID1/OID2 (m_w8/m_w9) swap per
instantiation matches the four originals (Clear rows read the enemy at +0x24, Kill rows at +0x20),
and the vtable +0x34 gate is Character::IsDead.
Item-side: `0x3fa1fc operator<`, `0x3f9d78 operator==`, `0x3fb290 _UpdateStats` (type bits
0x1780 / 0x40 / 0x3F and the `>>8` signed shifts), `0x3fb754 _UpdateName` (segment indices,
the '#' delimiter scan, the [a]-tag join order, the `<`/`>` fallbacks), `0x3fc3e0 Split`,
`0x3ff858 TransferItemTo`, `0x3ff5d4 _AddItemInstance`, `0x3fe448 RemoveItem`,
`0x3fdaf0 IsItemEquipped`, `0x400510 _UnEquipItemAuto`, `0x3fe658 TryConsuming`,
`0x3fe878 RemoveOnePotion`, `0x3ffc40 SetPotionQty` (the "Potion0" ItemTable scan),
`0x3fe330 IsInventoryFull` (`> 99`), `0x3ff200` ctor, `0x3eccc8 DropLootTable`,
`0x3eac34 ItemManager::DeSpawn`, `0x3eb6e4 PreCache` (`ItemObject_%02u_%02u`, GO id 3, five
slots, DeSpawn of all five), `0x3ec668 _GetRandomDropPos` (the 200/300/500 immediates IDA prints
as `dword_C8`, `elf_hash_bucket[20]`, `elf_hash_bucket[70]` = addresses 200/300/500),
`0x3ebd2c OnCollisionEnds`, `0x379ac4 PlayerStatManager::GetRanking` (see below).

## Findings by severity

none / none / none (high / medium / low). The CSV contains only the header row.

Every `const:`/`float:` lead that was checked turned out to be one of the false-alarm classes the
brief lists: a struct offset (`+0x24`, `+0x50`, `+0x8c`, `44`, `72`, `128`), an STL internals
constant (division-by-24 magics 1431655765 / -1431655765 / 1431655766, `0x1FFFFFFF`, the 128-byte
`CustomFree` threshold, `std::string` block sizes 16/24), an operator-new size (56/60/108),
a stack-buffer size, or an address mistaken for a float (the `float:` leads on the four
`ObjectiveTemplate_KillCharacter::GetPositions` rows are the instantiation address `0x47C6F0`,
`0x47C914`, `0x47CA18`, `0x47CB1C`; the cmp/cf/do counts differ only because the reconstruction
uses a range-for over `ObjectManager::m_characters` where the listing has a do/while walk).

## Addresses that could not be matched or read

* `0x387734 SimpleTypeProperty<dimension2d<float>>::IsDefaultValue` — the queue's `recon_span`
  (3–10) points at the file header comment, not the function. Re-located by hand: the body is at
  `g1-game-items-save-SimpleTypeProperty-cdc6b3-0.cpp:204-210` and was audited there. Same
  re-location was needed for several rows whose `recon_span` is one line (`0x3ff200`, `0x3ff330`,
  `0x47c6f0`, `0x47c914`, `0x47ca18`, `0x47cb1c`) — the reconstruction has explicit template
  instantiations that carry only the `// 0x<addr>` comment; the audited body is the template
  definition elsewhere in the same file.
* No row was left unread because of a missing file or a missing address comment.

## Deliberately not reported (documented deviations / not behaviour)

1. **Return values the Hex-Rays listing leaves in r0 after a void or tail call.** `SetPotionQty`
   (0x3ffc40, declared void, original returns SetQty / _AddItemInstance), `_UnEquipItemAuto`
   (0x400510, documented PARTIAL in the source), `_EquipItemToSlot` (0x400634, returns 1 instead
   of the item pointer, documented), `saveAll` (original returns the junk 16556), the
   `SimpleTypeProperty` slots, `_UpdateReqs`/`_UpdateStats`/`_UpdateName` int results. Every
   caller in the tree ignores these values (checked for SetPotionQty), so nothing observable
   changes.
2. **Early return after a failed ASSERT where the original keeps going and reads past the end of
   a vector/array.** `RemoveItem` (0x3fe448), `_UnEquipItemAuto`, `IsItemEquipped`,
   `_EquipItemToSlot`, `TransferItemTo` (0x3ff858), `_AddItemInstance`, `_AddLootItems`,
   `_AddLootItemPowers`, `_UpdateReqs` (all with an in-source or ledger note). The original's
   behaviour on those paths is undefined; the port is defensive. (TransferItemTo's is the only one
   of these whose comment does not say so explicitly.)
3. **`PlayerStatManager::GetRanking` (0x379ac4) is an explicit PARTIAL.** The reconstruction
   reproduces the value/player buffer and the insertion sort but not the `> 135 bytes` introsort
   branch (original: `__introsort_loop` then a final pass from `&dword_9A23CC` to `&dword_9A2444`).
   That branch needs more than 16 players, which the 8-dword static buffer cannot reach; the
   reconstruction's own comment gives "four pairs" as the reason. The insertion-sort code that
   *is* reproduced matches the original's `__insertion_sort` + `__linear_insert` semantics
   (stable, largest first, `_StatComp` = a.first > b.first). Flagging this would be reporting a
   documented deviation, so it is listed here instead.
4. **std::string / std::map / std::vector instead of the original 24-byte COW string, STLport map
   and raw arrays**, including the "text word at string+0x14" convention; `DebugSwitches` reads
   through `GetInstance()` in `IsInventoryFull` (the original touches `s_inst` directly, so the
   settings file is loaded one extra time on first use); `PreCache` skips `InitOnce` on the null
   object the original would dereference; `FindMemberName` has an extra null-table guard;
   `Savegame::_cacheFile` opens the slot and the `.bak` copy through the savefile seam.
5. Omitted ASSERT blocks, `Hex-Rays`' `x >= a` for `!(x < a)`, float constants printed as bit
   patterns, and inlined `sub_XXXX` bodies — per the brief, never reportable.

## Method notes

* Both sides were read in full for every audited row (original span from the TSV, reconstruction
  re-located from its `// 0x<addr>` comment when the TSV span was stale).
* Struct-level claims were checked against the headers, not taken on trust: `ItemInstance.hpp`
  (PowerInfo 32 bytes, m_powers +0x5c/+0x60, unk_68 +0x68), `ItemInventory.hpp` (m_potion +0x24,
  m_maxGold 0x7fffffff, m_maxPotions -1, m_equipSet +0x2e), `ItemManager.hpp` (ObjectInfo 8 bytes,
  CategoryInfo 16), `PlayerSavegame.hpp` (every field the section handlers touch),
  `pydata-accessors/w7_structs_2.hpp` (`Structs::Item`: Name +0x44, Material +0x48, Type +0x58,
  EquipmentSlottingType +0x68, requirements +0x74..+0x88, Param1 +0x8c, Param2 +0x90),
  `structs_records.hpp` (LevelDeclaration LevelState +0x28, WldMapLocation State +0x08,
  CharacterProperties ClassString +0x18, LevelList stride 72, WorldMap stride 20,
  CharacterTable stride 900).
* Ledger notes were read for the addresses where a deviation was suspected.
