# CharProperties::HandleDots source caller

This reconstructs one complete original caller, `CharProperties::HandleDots()`
at `0x3df3f0`, 144 bytes. The manifest pins the original ELF and seven symbol
ranges. Only the first range is credited as a new body; the other six document
dependencies and the event route. Nothing is wired into the APK by this unit.

## Exact source order

At entry the source captures its properties object in `r5` and its resolved
sheet `properties+0xa94` in `r6`. It loops element `r4=-1..4`, calling
`_GetProperty` with property index `r4+127` (126..131). These are the original
Normal, Fire, Water, Lightning, Earth and Air tick-damage fields, verified from
the unchanged cache's field-name table.

The returned amount is captured in `r8` and interpreted as a signed word.
Nonpositive values skip all owner and combat calls. A positive value reads the
live owner at `properties+4` and calls its virtual `IsDead` at vtable byte0x34.
Any nonzero predicate word skips that element. Otherwise:

1. `0x3df448` rereads the live owner; one captured owner is supplied as both
   attacker and defender to `F_DotAttack` at `0x3df454`. The positive captured
   amount and element word are preserved across the IsDead callback.
2. `0x3df458` rereads the live owner again; one captured owner is supplied as
   both attacker and defender to **`F_ApplyResult`** at `0x3df468`. Its boolean
   mode is the previously captured zero IsDead return (`r8`). It is not a new
   dead query and is not the preceding attack provider's return word.
3. The next element obtains a fresh cached-sheet property value, so previous
   provider effects can change the subsequent tick.

The source reuses the same local AttackResult storage for all elements. The
native adapter uses the existing 40-byte `data::CombatResult` projection. A
successful attack provider must produce every field its application provider
consumes. The native type's defaults are not credited as an original result
constructor. Requests and local result storage are synchronous borrows.

This caller contains no delta read, duration decrement, damage approximation,
target selection, RNG or floating-point operation. The source timer's interval
and repetition belong to CharAI::OnInit/Coordinator. Existing
`character_ai_events.cpp` already maps event0x34 to this producer before the
ordinary AI-state gates; that dispatcher is not duplicated here.

## Providers and native boundary

`read_property` is the real cached `_GetProperty(sheet,id)` operation. With the
verified normalized PropertyView it can read `resolved[id]`; it must not run
an uncached resolver/recalculation in place of that read. `is_dead` must query
the selected owner's current virtual source facts. Callbacks may change the
live owner; all old and new owner backing must survive the synchronous call.

`F_DotAttack` has its own mandatory DebugSwitches load/string/query/destruction
before `_F_CalculateResult(mask=0x20080000, weapon_category=-1, element, amount)`.
The maintained combat result arithmetic can be reused by that separate
provider, but does not cover the caller's debug side effects. `F_ApplyResult`
is the actual 3388-byte symbol at `0x3b10b4`, with property/health/aggro/state,
status, UI and other effects. The maintained combat application projections
document external effects; they are not interchangeable with a fully bound
application provider. These two positive-path providers remain explicit here.

The three authored Crypt Ghost rows have zero base words in all six tick
fields. This permits a real live read loop to complete without combat calls
when the **current resolved** words are nonpositive. Authored zero values do
not justify a permanent successful timer no-op: buffs and gameplay can change
the resolved words, and a reached unsupported positive provider must fail.

## Failure and lifetime

State identities and service bindings are captured once. Owner.character is
read fresh at the three source points. One owning thread retains aligned,
disjoint controls, source sheet backing, provider context and retired owner
backing. Reentry using the same owner/output, destroying borrowed owners or
letting the local result escape is outside the contract. Independent owners
and outputs may nest.

Missing providers are checked only when their source call is reached. A null
owner likewise fails only when a positive entry reaches that source read.
Returned provider errors and exceptions preserve already completed effects
and stop immediately; there is no added application, iteration, cleanup or
rollback. These explicit port guards are not claims about original exception
or invalid-pointer behavior.

## Verification

The runner executes all 36 instructions of the complete 144-byte original
caller under the relocated ELF loader. Four named provider hooks model live
cached-property values, raw dead words, an explicit result fixture and the
application observer. Their complete bodies are not executed or credited.
The compared traces include owner identity, sheet, property, amount, element,
mode, result-storage reuse, and mutation of subsequent properties/owner.
The providers return unrelated raw words to prove the caller does not use
their returns as its next boolean mode. No guessed PLT/soft-float hook exists.

149 original ARM comparisons cover every caller instruction. The independent
host gate has 36 behavior cases, 19 guard cases and 48 failure/exception cases.
The cache audit hashes all three original Character tables, checks the six
field names and verifies the three Ghost rows (35,36,37). The array-name table
has 83 bytes of additional class-name metadata after its 448 row names; that
metadata is hashed with the file rather than misread as a property row.
The property table likewise retains 196 bytes after its 448 fixed rows; the
maintained CharacterTable reader already reports those consumed boundaries.

Host cases additionally exercise reached missing providers, raw signed
boundaries, all six elements, above4GiB identities, independent nested owners,
null/alignment/alias guards, and failure/exception at every call in the maximal
24-call path. These failure cases are port-contract checks, not original ARM
differential cases.

Run:

```powershell
python port/level-world/tests/run_character_dot_tick_host.py --compiler C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe --original-elf C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/test_strategy/libDungeonHunter2.so --cache C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/cache/files --output port/level-world/build/character-dot-tick/host.exe --report port/level-world/build/character-dot-tick/validation.json
```
