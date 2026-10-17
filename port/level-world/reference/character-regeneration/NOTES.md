# Original HP/MP regeneration callers

## Scope

`character_regeneration.hpp/.cpp` reconstructs the complete 236-byte
Character::RegenHP caller at `0x3bdca4` and 236-byte RegenMP caller at `0x3bdbb8`.
This is **two complete caller bodies, zero new complete dependency bodies**.
The manifest pins ten exact original ELF ranges, the debug literal and singleton
GOT, and actual cache configuration. Native VM/OnInit adapters remain external.

Original ELF SHA-256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

## Exact caller behavior

Each caller captures its properties receiver (Character `+0x560`) and resolved
Struct receiver (Character `+0xff4`). HP reads property 36 then maximum 38;
MP reads property 41 then maximum 43 through `_GetProperty`. This is a direct
resolved-sheet read, not an inserted RecalcProperty operation.

A negative signed request selects the captured maximum. The caller compares
wrapping 32-bit `current+amount` to maximum as signed integers. A greater sum
replaces amount with wrapping `maximum-current`. A nonpositive resulting amount
returns without reading the debug identity, constructing a string, querying a
switch or adding a property. There is no generic saturating clamp.

A positive amount captures DebugSwitches::s_inst from GOT `0x99531c` (defined
48-byte object `0x9a1d18`), calls its real `load`, constructs the exact string
`isTracingChar_Stats` (`0x8c4898`), calls `GetSwitch`, destroys that string, then
calls real `PROPS_Add(current_id, captured_amount)`. The query return word is
discarded. The same captured debug object and constructed string are retained
across subsequent provider changes; current, maximum and selected positive
amount are not reread or recomputed. Original stack protection is compiler
housekeeping, not a new game-state action.

The `-1` request used by SetLevel reaches this debug branch only when its computed
amount is positive. At current==maximum it computes zero and skips debug/add.
At current==-1 and maximum==12160 it adds 12160, producing 12159 for an ordinary
saved-contribution HP sheet; this does not forcibly set current to maximum.
Existing InitPost's second fill pass differs from SetLevel's single HP/MP pass.

## Real maintained dependencies and ownership

Reuse existing `dh2_property_add` with an actor-owned live PropertyView for the
adder; do not replace it with arbitrary resolved-sheet writes. Existing
`dh2_vitals_regen` has the same verified cap arithmetic, but omits debug side
effects and cannot be called after a mutating debug callback to recompute a
captured amount. This new caller retains that verified arithmetic and exact
debug phase, with typed read/add services. Host tests compare 512 full real
sheet states through both maintained arithmetic paths without debug mutation.

For SetLevel's preceding true RecalcProperties call, existing live
`dh2_class_recalc_base` already composes `_LoadClass(base, base[26], false)` and
ordered RecalcProperty IDs 0 through 223. The C++ copy/commit convenience wrapper
is not an equivalent provider for partial failure or supplied buff groups.
SetLevel writes base Level property 19 at Character `+0x5b8`; saved-sheet Level
is `+0x8f8`. Retain separate actor-owned base/saved/gear/resolved sheets and real
ordered buff views rather than merging them or inventing a direct derived stat.

The actual `DebugSwitches.savegame` is 665 bytes, SHA-256
`51a3827f0109e16d1520e76d5b19736df3afe38375b91b955ac519f954234d6b`.
The runner fully consumes its 23 length-prefixed names/byte values as an audit
fixture. **It lacks `isTracingChar_Stats`.** In the original GetSwitch missing-key
branch, `_M_find` returns the map sentinel; `operator[]` inserts/defaults false,
then the function loads the global debug owner, queries `isTracingDebugSwitches`
and finally reads the requested map entry. That unused caller result does not
make load/query inert. File access, map allocation, string storage and actual
configuration loading remain real named providers; no parser/load/map body is
claimed here. The inspected source debug load includes a global one-time guard
and real filesystem access. A successful empty provider is not a substitute.

## Error and lifetime contract

All owner tokens/backing, control records and backend contexts stay retained on
one owning thread until synchronous return. State/services are captured at entry;
Globals.debug_switches is read only when the positive phase is reached. Providers
may change actor scalar values and global debug selection. Same owner/output or
provider string reentry is forbidden; independent owners and backends may nest.

Aligned nonoverlapping control storage and nonzero entry owner tokens are port
requirements. A zero debug token is rejected only when the debug read is reached.
String construction must return real retained nonzero string storage. Errors and
exceptions stop at that phase, retaining prior effects. No rollback, property
add, extra string destructor or cleanup is invented. Providers manage backing
left after a port error. Original C++ exception unwinding/invalid pointer behavior
is not asserted by these error cases. Added metadata means an adder returned
normally; it does not assert an adder cannot itself partially mutate then fail.

## Verification

The ARM comparison executes both complete caller ranges, including their debug
blocks, with explicitly named property/debug/string dependency entry fixtures.
It checks captured reads, signed/wrapping arithmetic, both refill/no-op branches,
query return discard, global/owner/property mutation and all call order against
compiled source. No floating numerical import is modeled. Host tests additionally
cover failure/exception phases, missing providers, null string output, alias and
alignment guards, independent nested owners and 512 maintained-property states.

```powershell
python port/level-world/tests/run_character_regeneration_host.py --compiler C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe --original-elf C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/test_strategy/libDungeonHunter2.so --cache C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/cache/files
```
