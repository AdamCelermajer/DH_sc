# Character native registrations and Ghost initialization dependencies

## Maintained source scope

`character_native_bindings.hpp/.cpp` reconstructs two complete registration
callers: GameObject::createBindings (2648 bytes, `0x38d7ec`) and
Character::createBindings (5332 bytes, `0x3b56bc`). Character first executes all
86 inherited registrations, then its own 179 entries: 265 ordered registrations
in total, comprising 135 functions and 130 methods, across 128 unique callback
providers. The manifest records every original name, source string address/hash,
callback symbol/address and explicit function context.

This is **two registration caller bodies, zero callback bodies and zero Binder
bodies**. The 146 pinned original ranges include external callbacks and OnInit
dependency evidence; they are not 146 rebuilt functions. Actual native VM
installation and all callback dispatch are provided by the owning adapter.

Original library SHA-256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Function and string hashes use ELF virtual addresses translated through PT_LOAD.

## Names, kinds and receiver ownership

The immutable tables preserve original registration order, duplicate aliases and
the actual underlying callback pairing. `SetActorPosition` uses the GameObject
position setter. Methods `LOCK` and `UNLOCK` have no corresponding global function
registration. The inherited final entry is method `SetMaxPath`, followed by
Character's function `SetActorPosition`. The final Character entry is method
`SetSpellCooldownTimerId__`, retaining the original suffix.

`Character::_ClearTarget` at `0x3b5690` (36 ARM bytes, SHA-256
`ce58d0a76509f4cc7d385605a5a25a935dde198dd8f4a2c18472383d9f25f9db`)
ignores Lua arguments and returns, then calls `CharAI::AI_SetTarget` on
`Character+0x3c8` with `(nullptr, false)`. The player-skill adapter projects
the existing CharAI target fields and invokes the retained `AI_SetTarget`
owner, preserving writes even if its debug-switch provider fails. It creates
no separate target state.

Global function registrations use the captured Character as userdata except for
the four original null-context functions:

- `GetDistanceBetween`
- `SetFXEndPoint`
- `RegisterSummon`
- `SetProjectileTarget`

Method registration has **no explicit userdata argument**. Its receiver must be
resolved later from the real Lua object wrapper, preserving source object-table
identity; it must not become another global closure bound to this Character.
The port passes zero for this absent argument and distinguishes the method kind.
Callback provider keys are native enum values, never callable original ELF
addresses. Registration can install an explicit unresolved closure for a callback
whose implementation is missing. Invoking it must report an unresolved error;
no missing callback is implemented as a successful no-op here.

SetCharacter writes AIS owner `+0x98` before this registration stage and assigns
the owned AI directory afterward. The separate frozen
`ais_external_initialization` unit preserves that ordering. Source AIS base/AI
names and library opens remain the separate `ais_native_bindings` stage.

## Controls, lifetime and errors

Entry Character/Binder identities and services are captured once. Providers may
change external registries and caller input facts, while retaining all Character,
Binder/VM/provider/context and retired backing through synchronous return. Control
storage remains live and disjoint; the result must not be overwritten. One owning
thread is required. Same-output reentry is forbidden; independent owned objects
and outputs can nest.

Invalid alignment, null identities and overlapping controls reject before effects.
A missing service reports unavailable when needed. Nonzero provider return or an
exception stops at that entry with earlier registrations retained. Counts include
attempted calls but only successfully completed function/method installations.
There is no rollback, destructor, cleanup or invented later registration. These
port error boundaries are not claims of source Binder exception/return handling.

## Plain monster OnInit: actual source requirements

The unchanged recovered scripts `_commons.luac` and `monster.luac` are source
evidence, with SHA-256 pinned in the manifest. Monster's registered `OnInit`
calls the empty common `OnInit`, sets Lua flags, then executes:

1. `GetPyOID("ClassTable", "Buff_Speed")`.
2. `GetPosition()` and saves the first two returned coordinates.
3. Fresh `GetPyStruct`/`GetProp`/`FromFixed` reads for LevelMax, LevelMin, LevelOffset.
4. `GetHostPlayerLevel()`, `GetHostPlayerDifficulty()`, and
   `GetCurrentLevelRange(difficulty)`, even if LevelMax later disables scaling.
5. If LevelMax is greater than -1, the exact Lua comparisons choose the supplied
   level range or the monster limits, add the offset and invoke `SetLevel(ToFixed(level))`.

Actual recovered Character rows 35 `Crypt_Ghost` and 37 `Crypt_Ghost_RE` have
raw Level/LevelMax/LevelMin all 256, LevelOffset zero, ClassID 18. Their dynamic
level branch therefore cannot be skipped by assuming LevelMax=-1. These are
authored base rows; the native provider must resolve the current property owner
after class/actor initialization and later changes. Property IDs are Level19,
LevelMax20, LevelMin21, LevelOffset22. `Buff_Speed` is row 48 of the recovered
character class names; the source `ClassTable` registry association still belongs
to the actual PyDataArrays provider, not a guessed generic name lookup.

| Callback | Original caller | Genuine producer needed |
| --- | --- | --- |
| GetPosition | `0x38e700`, 52 B | Existing maintained gameplay-object-callbacks projection, current owned GameObject world XYZ |
| GetProp | `0x3b9d8c`, 440 B | Numeric argument/type gates and current CharProperties `_GetProperty` over the selected sheet; reuse maintained property resolver where exact |
| GetPyStruct | `0x37f4a8`, 340 B | Actual PyDataArrays structure registry/schema lookup |
| GetPyOID | `0x37f5fc`, 340 B | Actual PyDataArrays `GetOID` (`0x4bd640`, 72 B), with real ClassTable association and names |
| GetHostPlayerLevel | `0x37cc00`, 60 B | PlayerManager::GetHostingPlayer (`0x36e09c`, 112 B), then player word `+0x330`; no null guard in this callback |
| GetHostPlayerDifficulty | `0x37cb8c`, 76 B | Application::GetCurrentLevel (`0x31f594`, 32 B), then current Level word `+0x118`; null Level returns zero |
| GetCurrentLevelRange | `0x37f1f0`, 356 B | Fresh current Level PyOID `+0x3c` and real 72-byte LevelTable rows |
| SetLevel | `0x3b73a4`, 260 B | Original argument/conversion/design/store/recalculation/regeneration sequence below |
| ToFixed/FromFixed | `0x37ebc4`/`0x37ee84`, 80/184 B | Existing maintained Lua numeric conversion; actual VM source-value conversion still must preserve argument behavior |

Despite its Lua name, GetHostPlayerDifficulty reads the **current Level**, not a
host Character difficulty property. GetCurrentLevelRange reads the current Level
before argument processing. PyOID -1 returns two integers -1. Other OIDs select
actual LevelTable row stride 72. Default/no argument/non-number argument or numeric
zero use min/max `+0x3c/+0x30`; numeric 1 uses `+0x40/+0x34`; numeric 2 uses
`+0x44/+0x38`; other converted numeric values return no values. The table base is
reread after the first pushInteger callback before obtaining the second value.
Underlying current-Level/hosting-Player/table ownership and bounds remain real
external producers; the native adapter cannot fabricate them from distance or
from the Ghost's own properties.

SetLevel requires a numeric first argument. It reads Value::getNumber, asks the
actual design manager for `CharacterDesign/MaxLevelDVeryHard`, shifts that integer
left eight as a 32-bit word and compares it signed against the original
`__aeabi_f2iz` conversion. If above the limit it queries the design value **again**
and stores that second shifted result. Otherwise it freshly obtains/converts the
numeric argument again. Then it stores the base Level word at Character
`+0x5b8`, calls CharProperties::RecalcProperties(true) (`0x3e0810`), calls
Character::RegenHP(-1) (`0x3bdca4`) and tail-calls RegenMP(-1) (`0x3bdbb8`).
It is not a single resolved-property overwrite or an HP/MP no-op. There is no
lower-limit check in this caller. The original PLT `0x30e4cc` was independently
resolved by executing its relocated stub: it imports `__aeabi_f2iz`. This notes
audit does not execute or implement those callback bodies.

## Validation

The host tests retain installed bindings/context records, verify function versus
method semantics, inheritance boundaries, literal aliases, snapshot behavior,
independent nested owners and control guards. Every one of the 86/265 service
phases is tested for both provider error and exception; earlier successful
registrations remain, and the failed phase does not increment success counts.

The runner validates all 146 original function ranges and registration-string
hashes, then executes actual GameObject and Character ARM caller instructions for
five independent Character/Binder identity pairs. Only Binder::bindFunction and
bindMethod installation bodies are modeled; original name/target/context/order
instructions and inherited calls execute normally. The ten original comparisons
include 1755 observed registrations and zero mismatches. No callback bodies or
soft-float helpers are modeled or executed in this registration comparison.
The source remains outside native wiring until the owning adapter installs these
bindings and genuine callback providers.

```powershell
python port/level-world/tests/run_character_native_bindings_host.py --compiler C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe --original-elf C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/test_strategy/libDungeonHunter2.so
```
