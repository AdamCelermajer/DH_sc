# `Character::GetCharFaery(int)` bounded source selector

`character_faery_selection.{hpp,cpp}` reconstructs the 540-byte source body at
ELF `0x3aeac0`. It selects a FaeryTable row through the Character's live
FaeryList ID and includes the original consistency checks. It is the leaf
selector used by `CharAI::SetSkillsAndSpells`; it does not build the
`CharAISkillScript` vectors or run a spell script.

## Source decision and order

1. `Character::GetCharFaeryListId` reads `Character+0x106c`. Negative or
   out-of-range IDs select list 0, after comparing against the live
   `FaeryListTable::size`.
2. The FaeryList row pointer is selected before the first `FaeryTypes/COUNT`
   query. A negative requested faery slot skips that first query; otherwise it
   queries and checks `COUNT > slot`.
3. It reads the selected list's `ListSize`, performs a second fresh
   `FaeryTypes/COUNT` lookup, and checks exact equality.
4. It indexes the list's members with the requested slot, maps that integer to
   a 36-byte FaeryTable record, and compares the record's word at `+0x20` to
   the requested slot. A failed consistency check is diagnostic-only at
   assert level 0 and continues returning the selected table row.

The two constants queries are separate, ordered calls. The first may return a
different value from the second. The implementation keeps that order. It
also models assert-level-1 diagnostics through a typed provider and represents
assert-level-2's original null-store fault with `fatal_source_assertion`.
Malformed table bounds are a named port guard because reproducing the original
out-of-bounds read would not be a usable native contract.

## Why this matters for the Crypt Ghosts

The cache's `Crypt_Ghost` and `Crypt_Ghost_RE` rows both author raw FaeryList
property `-1`. The earlier Character initialization path therefore chooses
FaeryList row 0. Its five members map to FaeryTable indices
`[2, 4, 5, 6, 3]`, named `Fake_Celest`, `Fake_Rocky`, `Fake_Wetty`,
`Fake_Windy`, and `Fake_Hotty`.

`CharAI::SetSkillsAndSpells` uses this getter while constructing entries in
the faery vector. Its full 1916-byte body also depends on Lua `DeclareSkill`,
script-object constructors, vector allocation, and final AIS dispatch. Those
dependencies are not replaced by this selector. The authored `SkillTree`
property remains a distinct field; there is no `SpellsID` field in the
CharacterProperties schema.

## Validation and limits

The host fixture covers the explicit list and invalid-ID fallback, the two
fresh count results, count/list inconsistencies, the Type assertion, source
diagnostic ordering, and bounded malformed-array rejection. The differential
runner executes the original `GetCharFaery` and `GetCharFaeryListId` ARM
instructions for five list/slot cases. It intercepts the real
`PyDataConstants::getConstant` call and supplies test `COUNT` words; the
constant-map body and diagnostic `fprintf` path are not part of that
comparison.

```powershell
python port/level-world/tests/run_character_faery_selection_host.py `
  --compiler C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe `
  --original-elf C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/test_strategy/libDungeonHunter2.so
```

The report is written to
`port/level-world/build/character-faery-selection/validation.json`. This is a
host/source-body comparison, not Android wiring or proof that the source
CharAI lists are active in the rebuilt app.
