# pydata ground truth: reconstructed Structs vs the original generated headers (owner P, 2026-10-16)

Ground truth: `reference-binaries/dh2-scons-dev/gameloft/games/GloftD2HP/scons/pydata/*_pystruct.h`, `*_pystruct_read.h`,
`*_pystruct_impl.h`, `*_pyarray_readers.h` (iPhone 1.0.0 SCons tree). Ours: `port/decomp-reconstruction/pydata-accessors`.

## Struct definitions (343 original classes)

* Every one of the 343 original classes exists in our headers.
* Member COUNT and ORDER match for all 343. Member offsets were checked as well: the 32-bit ARM layout of the original
  member list (vptr at 0, natural alignment, embedded structs by their computed size) gives exactly the `// +0xNN` offsets
  of our headers for all 343. Member types agree (int32/uint32/float/bool/int8/int16 exact; original `T*` members are
  `void*` or `char*` in ours by design: element tables are host objects, see NOTES.md "Element tables").
* So there is **no struct** whose count or order differs, and nothing had to be left unrenamed. After this pass no
  `m_wN`, `m_offN` or `m_subN` member remains in any pydata header (132 structs were renamed by position, plus `LangSheet`).
* Representation differences (not defects, recorded for completeness):

| struct | header | ours | original |
|---|---|---|---|
| `Item` | w7_structs_3.hpp | `kCount = 38` (ItemBase 15 + Item 23) | `m_dataNames` of `Item` lists its own 23 names only |
| `LangSheet` | w7_structs_1.hpp | `: public pydata::ArrayElement` (vptr at 0, members from +0x4) | `class LangSheet` with no parent (same offsets) |
| `v2Quest` embedded members | w7_structs_1.hpp | `AcceptType`/`EndType` are `v2QuestObjectiveStub`, `Scripts` is `v2QuestScripts`, same offsets | same names and types |
| `ExecScript::ScriptAlts` | script_cmd.hpp | `std::int32_t*` | `int32*` |

## Readers, finalize, destructors

* Registration table (142 addFuncsForFile + 142 registerClassByName): 284/284 identical, see GROUNDTRUTH-READERS.md.
* `read()` token sequence (readAs<T> type, readStringEx, nested read) per class, original `_pystruct_read.h` vs our bodies
  (helpers `ReadStringField`, `ReadBuffer`, `ReadInt32Array`, `ReadObjectiveWords`, `ReadItemRefWords`, ... expanded):
  **343/343 match** (one tooling false positive: a comment in `ItemAudioVisual::read` mentions readStringEx).
* Read ORDER of member names (original `// Name` comments vs the order our body assigns members): 335 match; the other 8 are
  the ItemBase family whose shared helper now goes through `ItemRefMembers(T&)` (per-class names, same positions).
* Free order of `~X()` and `finalize()` (`if(Member)` list of the original vs ours): all match; 17 ItemBase-family
  bodies go through `ReleaseItemRefWords` / `FinalizeItemRefWords` (IconName, ModularModule, same order).
* `GetMemberIDByString<Arrays::TrophyTable|CharacterTable|ItemTable>`: defined once each, as explicit specializations in
  `src/tb_accessors-Arrays-0.cpp`. An uncommitted edit had also instantiated them in `src/member_ids.cpp` (two strong
  definitions of each symbol); that edit was removed. `nm` of the archive: all three `T` in `tb_accessors-Arrays-0.cpp.obj`.

## Fixes made

* ItemBase-family helper (`w7_structs_2.cpp`): the three `ReadItemRefWords` field names are class specific in the original
  (e.g. `WeaponRef.minDamage`, `Potion.HP_Recovery_Percentage`, `GoldItem.MinValue`); a single hand-named template could not
  be right for all nine. Replaced with `ItemRefMembers(T&)` overloads returning `std::tie` of the class's 24 members in file order.
* Rename applied tree-wide (game*, glitch*, net*, vox-audio, tests); full build of `build/own-p` has zero compile errors from it.
