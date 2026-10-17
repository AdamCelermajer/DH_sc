# `CharAISkillScript` source constructor

`character_ai_skill_script_constructor.{hpp,cpp}` reconstructs the 336-byte
constructor at original ELF `0x3cde2c`. This is the object constructor reached
by the faery/skill entry branch in `CharAI::SetSkillsAndSpells`; it establishes
the per-entry owner/name/index and arguments object used by later `DeclareSkill`
calls. It does not load Lua files or append objects to CharAI's source vectors.

## Source writes and calls

The source stores its vtable, Character pointer, and script-name pointer first.
It constructs `Arguments` at `this+0x0c`, writes the skill index at `+0x14` and
initial `lastSkillId=-1` at `+0x18`, then checks Character/name assertions in
that order. At assert level 0, both checks continue without logging; level 1
emits the corresponding diagnostic and continues; level 2 follows the source
fatal null-write boundary, represented here as an explicit status.

The final calls are `Arguments::pushString(name)` followed by
`Arguments::pushInteger(index)`. Their storage/allocation bodies remain typed
services. A missing service never reports constructor success. Service errors
preserve field writes and completed child effects, and do not roll them back.

The portable kernel uses an owned enum `DispatchTable::char_ai_skill_script`.
It does not install the original ELF vtable address into a native object.
`arguments_identity` is exposed as the stable object identity plus `0x0c`, so a
real allocator adapter can bind it to the actual embedded source projection.

## How this fits the Ghost faery setup

`Character::GetCharFaery` selects a row from the fallback FaeryList0. The
`SetSkillsAndSpells` source loop reads `FaeryTable::SpellScript` at `+0x14` and
the corresponding `Name` at `+0x18`. For a nonempty script name it loads
`_commons`, calls `DeclareSkill(name, -1)`, loads the named spell script, and
constructs this object with index `0xffffffff` if the load succeeds; otherwise
it appends a null script pointer. A final zero-argument `DeclareSkill` follows
each nonnull script iteration. Skill-tree entries use the same constructor but
their index is the slot number. The enclosing `SetSkillsAndSpells` also owns
vector reserve/growth, path changes, DebugSwitches calls, and final AIS vtable
dispatch; those are not implemented here.

## Verification

The host fixture covers valid, empty/null name, null Character, assertion-level
fatal/error boundaries, provider failures, callback mutation, ordered storage
and push calls, and identity/control overlap. Four source comparisons execute
the original constructor body and hook the three called `Arguments` methods at
their original addresses. They compare the source-owned object fields and
every argument/call in order. Embedded `Arguments` allocation/value storage is
observed at its call boundary, not reproduced. The child `Arguments` constructors and push bodies are
deliberately not claimed by these caller-boundary hooks.

```powershell
python port/level-world/tests/run_character_ai_skill_script_constructor_host.py `
  --compiler C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe `
  --original-elf C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/test_strategy/libDungeonHunter2.so
```

The report is `port/level-world/build/character-ai-skill-script-constructor/validation.json`.
This component is not Android wired and does not claim a live Lua object factory.
