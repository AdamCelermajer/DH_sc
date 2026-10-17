# `CharAI::SetSkillsAndSpells` caller reconstruction

This bounded caller reconstruction is based on `libDungeonHunter2.so` SHA-256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. The
caller is ELF `0x3ce044`, 1,916 bytes. The pinned body hash and directly
observed callee hashes are in `original-functions.json`.

## Reproduced source flow

The source routine checks the active AIS, performs the `Lua_LoadMemUsage`
debug load/query, saves `AIS+0x68`, switches to `data/scripts/skills/`, then
populates the skill vector before the faery vector only when each destination
is empty. It reserves each source count, constructs one temporary `Arguments`
object per list, pushes `""` and `-1`, and processes rows in index order.
Skill row eligibility/name come from `+0x24/+0x28`; faery row eligibility/name
come from `+0x14/+0x18`. A false row gate appends null without declaring a
script. For an eligible skill row, `_commons` is loaded, `DeclareSkill` receives
the row name and **the current loop slot converted to float**, the named script
is loaded, a `CharAISkillScript` is constructed only if that load succeeds,
the result (or null) is appended, and zero-argument `DeclareSkill` is called.
For an eligible faery row, `DeclareSkill` receives the row name and `-1.0`,
and the child constructor receives index `0xffffffff`; `-1` is also the second
initial value pushed into the temporary argument list.

After both vectors, source restores the saved AIS path, repeats the debug
load/query, dispatches the **fresh active AIS** virtual at `+0xcc` (the
`InitVCB` slot for the observed default/external tables), then releases the
saved path. The caller reads `AIS+0x68` as pointer/end at `+0x14/+0x10`.

## Validation

Run the maintained portable caller and its original ARM oracle from the
repository root:

```powershell
python port/level-world/tests/run_character_ai_set_skills_and_spells_host.py `
  --original-elf C:\path\to\libDungeonHunter2.so
```

The runner compiles the caller and host fixture with C++17, warnings-as-errors,
and `g++`; it executes the original ARM caller through Unicorn with typed
fixtures at the named direct-callee boundaries. The same seven high-level
scenarios are compared against the C++ implementation by operation order,
vector null/non-null pattern and final InitVCB dispatch. The normal case also
checks skill slot conversion/constructor indices and the faery `-1.0` /
`0xffffffff` arguments against the original ARM caller.

The child `Character::GetCharFaery` body is intercepted at its exact call
boundary here; its selector has an independent source-tested kernel. The
Arguments/Value bodies, debug persistence, Lua VM, script allocation provider,
and child constructor are not credited as complete bodies by this caller
comparison. The constructor itself is covered separately in
`reference/character-ai-skill-script-constructor/`. The result is a bounded
source caller and host-validated port kernel, not a claim that all script
dependencies or native Android lifecycle integration are complete.
