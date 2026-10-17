# Skill and Faery table cache reader

`skill_tables.{hpp,cpp}` owns the cached `SkillListTable`, `SkillTable`,
`FaeryListTable`, and `FaeryTable` rows as ordinary host-width C++ data. It
does not overlay ARM32 row memory and it does not reinterpret serialized
strings as pointers.

## Serialized schema and source row layouts

The cache has 36 SkillList names/rows and 127 Skill names/rows, followed by
four FaeryList names/rows and 16 Faery names/rows. The associated schema
sections are checked exactly:

* SkillList: `List`; Skill: `Anim`, `AnimIsMoving`, `DisplayProps`,
  `ElementalType`, `FairieDependantText`, `Flags`, `Level`, `Script`,
  `SkillAssignable`, `SkillCurrLevel`, `SkillDescription`, `SkillIcon`,
  `SkillName`, `SkillNextLevel`, `Type`.
* Faery: `Description`, `Elemental`, `ModelFile`, `Name`, `SpellScript`,
  `SpellType`, `Type`; both serialized list structures have `List`.

The cache list format is a u32 count followed by signed 32-bit members. The
booleans in Skill are single bytes. `Script`, `SkillIcon`, and `SpellScript`
each serialize one signed source length followed by exactly that many bytes;
there is no second string-length word. The loader retains each length
separately from its owned `std::string`. This matters for Faery rows because
`SetSkillsAndSpells` uses the source `SpellScript` length as the null-script
gate. An empty script and a faery's table name are different fields.

The original 32-bit object strides are Skill 76 bytes, Faery 36 bytes, and
both list structures 12 bytes. Their strings are pointer-bearing runtime
members, so those ARM layouts are evidence only. The native rows instead use
host-width `std::string` and `std::vector` storage. The serialized Faery
`Name` field is a numeric source value and is exposed as `name_id`; the
cache's row label is exposed independently as `table_name`.

## Crypt Ghost source projection

The cached CharacterProperties `Crypt_Ghost` and `Crypt_Ghost_RE` have raw
`SkillTree=-1` and `FaeryList=-1`. The source's invalid-list fallback selects
SkillList row 3 (`DEFAULT`, empty) and FaeryList row 0 (`DEFAULT`, members
`[2,4,5,6,3]`). Those faery indices resolve to `Fake_Celest`, `Fake_Rocky`,
`Fake_Wetty`, `Fake_Windy`, and `Fake_Hotty`. Their serialized
`SpellScript` lengths are all zero. Preserve that length gate independently;
the row labels are not spell script names and must not be used to load Lua.

This is table/fallback source data only. The module does not produce runtime
Character IDs, create CharAI vectors, load scripts, or wire
`SetSkillsAndSpells`. The separate `character_faery_selection::FaeryRow`
models a source ARM32 row for its selector proof; native consumers must use
these typed rows and must not narrow `std::string::data()` to a 32-bit word.

## Original reader evidence

The pinned ELF is `libDungeonHunter2.so`, SHA-256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
`original-functions.json` records symbol addresses, sizes, and code hashes.
The original `Structs::SkillList::read`, `Structs::FaeryList::read`,
`Structs::Skill::read`, and `Structs::Faery::read` ARM instructions execute
for all 183 actual cache rows. Their read primitive, bounded array allocator,
free function, and `readStringEx` are named modeled dependencies. The four
`Arrays::*Table::read` bodies are also hash-pinned and disassembly-reviewed;
the comparison executes each corresponding row reader directly and does not
claim to execute table finalization or global table installation.

The runner independently decodes the cache, compares every owned row field,
then compares the same row bytes against the original ARM reader's populated
object. It also checks malformed/truncated records, mismatched schema/count,
suffix bytes, and that failed loads leave previously-owned output intact.

```powershell
python port/game-data/tests/run_skill_tables_host.py `
  --compiler C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe `
  --original-elf C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/test_strategy/libDungeonHunter2.so
```

The local cache is read from `work/cache/files/data/pydata`; it is not copied
into this source package. The report and host executable are written under
`port/game-data/build/skill-faery-tables/`. This result is not Android wiring
or proof of active runtime skill/faery ownership.
