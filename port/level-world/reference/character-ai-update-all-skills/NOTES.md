# `CharAI::UpdateAllSkills` source caller

`character_ai_update_all_skills.{hpp,cpp}` implements the complete 180-byte
caller at original ELF `0x3d8894`. This is a small but useful part of
`CharAI::InitScriptProcess`: it performs the source FSM gates and updates the
already-created `CharAISkillScript*` entries. It does not construct the lists,
execute `CharAISkillScript::OnSkillUpdate`, or make the source skill/faery
records available to the native actor.

## Source order

1. Query `SM_IsUsingSkill` against the current owner's embedded state machine.
   Any nonzero word returns immediately.
2. Query `SM_IsCasting` against a freshly read owner. Any nonzero word returns.
3. Capture the skill-vector count from `CharAI+0xb4/+0xb8`. For every slot,
   read the current `begin[index]`, skip null, and call
   `CharAISkillScript::OnSkillUpdate` for nonnull entries.
4. Capture the faery-vector count from `CharAI+0xc0/+0xc4` after all skill
   callbacks. Iterate it with the same null check and child call.

The original loop keeps each phase's count fixed, but reloads that vector's
`begin` before each slot after the first. The faery count is read only after
the skill phase, so a skill callback can change the faery list used this frame.
These callbacks are synchronous. Replaced vector storage and script objects
must remain alive for the whole update call. The adapter fails closed if a
callback shrinks a vector below the already captured iteration count; the
original code would read beyond that vector, so malformed/reentrant storage is
not presented as a supported source case. Callback effects completed before a
provider error remain applied.

`OnSkillUpdate` returns void. Its body is an explicit service and is neither a
no-op nor part of this caller's completion claim. Missing callback providers
fail when a nonnull script slot is reached. The two FSM predicates also remain
typed services; their exact leaf hashes are supporting evidence, while the
original caller's branch instructions execute in the differential runner.

## What the authored Crypt Ghosts select

The cached CharacterProperties rows 35 (`Crypt_Ghost`) and 37
(`Crypt_Ghost_RE`) both have raw `SkillTree` property 28 = `-1` and raw
`FaeryList` property 29 = `-1`. There is no `SpellsID` property in the
CharacterProperties schema. The source obtains the two lists through separate
runtime IDs: `Character+0x1068` for `GetCharSkillListId` and `Character+0x106c`
for `GetCharFaeryListId`.

The original ID getters range-check these words. Invalid SkillTree IDs select
SkillListTable row 3; its actual cached members are empty. Invalid FaeryList
IDs select FaeryListTable row 0; the cached list is `[2, 4, 5, 6, 3]`, named
`Fake_Celest`, `Fake_Rocky`, `Fake_Wetty`, `Fake_Windy`, and `Fake_Hotty` in
FaeryTable. Thus the Ghost has no ordinary skill-list entries at the authored
base-property projection, while its faery/spell side can still use the default
faery list. The CharAI setup caller `SetSkillsAndSpells` decides which entries
become script objects after resolving the live Character state; this audit
does not assert the resulting native vectors.

This differs from the Monster Lua global `SKILLTREE_ID`, which reads the same
raw `SkillTree` property and applies `FromFixed`. For `-1`, the source script's
`IsHavingSkillTree()` is false, gating its skill-tree-specific combat branches.
That is not proof that `SetSkillsAndSpells` skips the separate FaeryList path.
Animation table `Spells` arrays are a third, unrelated schema.

Data provenance and row/fallback projection are pinned in
[`original-functions.json`](original-functions.json). No cache binary or script
was copied into this new source module.

## Verification

The C++ host fixture covers each query short circuit, noncanonical truthy words,
owner replacement between the two FSM queries, null entries, empty vectors,
replaced skill `begin`, faery replacement during the skill phase, faery `begin`
replacement, and explicit provider failure. The runner also executes the
original ARM `UpdateAllSkills` instructions for nine cases and compares every
query/callback identity and order against the compiled caller. At the three
called source-function boundaries it supplies controlled predicate words and
records the actual `OnSkillUpdate` receiver; those callees do not execute in
this test.

```powershell
python port/level-world/tests/run_character_ai_update_all_skills_host.py `
  --compiler C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe `
  --original-elf C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/test_strategy/libDungeonHunter2.so
```

The result is written to `port/level-world/build/character-ai-update-all-skills/validation.json`.
This is a host/source-caller comparison only; the module is not wired into
Android, and it does not validate live pending-AIS promotion or the script
construction path.
