# Character::_InitHpMp

`character_init_hp_mp.hpp/.cpp` reconstructs the complete original32B caller
`_ZN9Character9_InitHpMpEv`, `0x3b3a70`. It captures this in r4, invokes mandatory
RegenHP(-1), restores the same captured identity, reloads -1, restores its own
stack/register frame, and tail-branches to mandatory RegenMP(-1).

Count **1 new complete caller body, 0 new complete dependency bodies**. Existing
HP/MP236B callers are frozen in `character_regeneration`; their property, Debug,
string and write providers remain separate. No direct HP/MP write, arithmetic,
class recalculation, SetLevel, skill initialization, post stage or final/frame
stage is invented in this32B caller. It does not prove full InitScriptProcess.

The API borrows a live Owner projection, Services and Result. It captures the
Character identity, both callbacks and context once. Providers can change
Owner.character during HP, but must retain the originally selected Character's
backing through MP. Both requests use captured identity and raw amount
`0xffffffff`. Requests are read-only. Providers implement the actual source
regeneration caller/effects; successful no-op callbacks are invalid.

Provider status zero/nonzero is a separate **port** success/error channel, not
the unspecified register left by an original void callback. The original HP
register return is discarded, and MP's tail register is likewise not a semantic
return from the void initializer. Missing MP is checked when reached, preserving
completed HP effects. Returned errors/exceptions stop without added MP,
rollback or extra cleanup. Controls are aligned/disjoint and checked before
source reads/writes. One owning thread retains them, provider contexts and
retired actors. Same captured Character/output reentry or destruction is
forbidden; independently owned calls may nest.

The runner SHA-pins original ELF, caller32B and two source regeneration236B
ranges, compiles C++17 with warning errors, and compares **32 actual original
ARM cases** (two retained Character identities,16 owner/binding/context mutation
combinations). All eight instructions are observed, including tail transfer
after restoring the original caller frame. Arbitrary opaque HP and MP register
returns do not alter dispatch. No float/import behavior is modeled. Regeneration
callees are explicit observing fixtures in this proof, not executed/credited
bodies. Their real frozen runners and root-owned Runtime composition provide
that independent dependency closure.

Host tests pass **17 behavior,10 guard,6 failure/exception/missing-provider
cases**. Failure callbacks commit partial test effects before error/throw;
previous effects remain and no following callback runs. Host owner mutation to
a different identity or zero, replacing MP callback/context after HP, and
independent nested calls all preserve the original capture rule.

Run from repository root:

```powershell
python port/level-world/tests/run_character_init_hp_mp_host.py --compiler C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe --original-elf C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/test_strategy/libDungeonHunter2.so --output port/level-world/build/character-init-hp-mp-parent/host.exe --report port/level-world/build/character-init-hp-mp-parent/validation.json
```

Native renderer/CMake and previously frozen source are unchanged. Root owns the
real Runtime initialization adapter and the surrounding lifecycle stage order.
