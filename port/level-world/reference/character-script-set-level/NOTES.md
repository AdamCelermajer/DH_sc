# Original Character SetLevel callback caller

## Scope and source evidence

`character_script_set_level.hpp/.cpp` reconstructs the complete 260-byte
Character::_SetLevel caller at `0x3b73a4`. It does not reconstruct its Value,
design-constant lookup, floating conversion, property recalculation or HP/MP
dependency bodies. The manifest pins seven exact original function ranges,
literal strings and Application singleton GOT identity. This is **one new
callback caller body, zero new full dependency bodies**. Native VM wiring is
owned by the integration adapter and is not claimed here.

Original ELF SHA-256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Hashes use ELF virtual addresses translated through PT_LOAD segments.

## Exact source call and store order

1. Empty argument vectors and initial first-argument types other than source
   numeric type 3 return without source actions. Extra arguments are ignored.
2. Fresh Arguments::operator[](0) and Value::getNumber obtain the first binary32
   number. The original 88-byte indexing instructions execute in the ARM oracle.
3. Capture Application through its original singleton GOT identity after that
   number query. Query the current captured Application's design manager `+0x2c`
   for `CharacterDesign/MaxLevelDVeryHard`.
4. Shift that returned signed integer left eight as an unsigned 32-bit word,
   retaining ARM wrap, then convert the **captured first number** through the
   original `__aeabi_f2iz` dependency. Compare limit and converted word as signed32.
5. When limit is strictly less than the converted word, reread the retained
   Application's current design manager and query the constant again. Store the
   second constant shifted eight. There is no positivity or second-limit recheck.
6. Otherwise freshly index argument zero, obtain its number and convert again.
   Store the second converted word, without repeating the level-limit comparison.
7. Store the raw base Level word at Character `+0x5b8`. This is property 19
   in the base sheet: Character `+0x560` properties, base Struct `+8`, four-byte
   Struct header and `19*4` byte field offset. Saved-sheet Level is `+0x8f8`;
   this caller does not write that field.
8. Call CharProperties::RecalcProperties(true) at `0x3e0810` with the fixed owned
   properties receiver corresponding to Character `+0x560`.
9. Call Character::RegenHP(-1) at `0x3bdca4`, then tail-call RegenMP(-1) at
   `0x3bdbb8`, with the captured Character receiver.

No lower-limit clamp exists in this caller. Equality takes the fresh second
argument path. Negative numbers and a changed second number are retained as raw
integer words. Large design values wrap on shift before the signed comparison.
A global Application selection change during the first number query affects
which Application is captured. A change after the first design query does not
replace it, while mutation of that retained Application's manager is observed by
the second query. Provider writes to base Level after its store remain visible;
the caller does not restore or replay the store after recalculation/regeneration.

## Real providers and authored cache

The source design method is PyDataConstants::getConstant (84 bytes,
`0x4c4bdc`), not a callback that returns a guessed maximum. The actual unchanged
cache `data/pydata/design_pycst.bin` contains
`CharacterDesign/MaxLevelDVeryHard=100`. The runner verifies that file's SHA-256
and full-consumption evidence in the existing actual original constant-reader
report. That existing reader report executed the 660-byte reader and primitive
readers with named stream/map insertion services; the new setter runner does not
rebuild or rerun the constant lookup body. Synthetic mutation cases supply
different first/second values to verify the caller's genuine freshness.

Raw base Level is not the resolved property's final value. RecalcProperties and
RegenHP/RegenMP remain real owned service providers. A native adapter cannot
replace them with successful no-ops or treat this callback as only a direct
property overwrite. Existing maintained property/class/vitals kernels should be
reused where their accepted owner and sheet semantics match. An unresolved
provider must fail explicitly; this module does not manufacture derived stats.

The float conversion at original PLT `0x30e4cc` is independently resolved by
executing its relocated stub and verifying the import `__aeabi_f2iz` before the
comparison models it. The original numerical helper body is not implemented
here. The ARM fixtures model finite binary32 values representable as signed32
with truncation toward zero; they include signed zero, negatives, fractional
values, signed minimum and the largest representable binary32 value below signed
maximum. A test backend rejects NaN, infinities and unrepresentable values as
explicit dependency failures. Those host error cases are **not claims of the
original helper's result on those inputs**. Production must provide the genuine
helper or explicitly report an unresolved conversion domain.

## Lifetime and port errors

Character/properties/Arguments identities and services are captured at entry;
Application is captured only when reached, after the first number query. All
owners, backend contexts and retired Application backing must stay live until
synchronous return on one owning thread. Scalar argument contents, base Level,
Application selection and the retained manager may change through providers.
Fresh reads and stores follow the source order above. Same Character/output
reentry is forbidden; independent owners and outputs may nest.

Control alignment, range overflow and pairwise alias guards reject before effects.
Application validity/overlap is checked only after the first number service, so
skipped argument branches do not invent an Application read. Missing, failing or
throwing services stop at that phase, retaining earlier source/provider effects.
There is no rollback, added regeneration or cleanup. Applied/clamped metadata
records a completed source store and the branch taken; it does not promise that
later service failures undo that store. These are port error boundaries, not
claims of original exception handling or invalid-pointer diagnostics.

## Verification

The host gate checks argument gates, both branches, equality/fresh number changes,
two design reads, retained Application/fresh manager semantics, signed/shift wrap,
store-before-services order, provider mutation, failure/exception at every phase,
unresolved numeric domains, independent nesting and alias/alignment guards.

The original comparison executes the complete 260-byte callback and real 88-byte
Arguments indexing instructions; named services model Value number reads, design
lookup, f2iz conversion and the three final dependency calls. Actual source
instructions perform the type/count gates, calls, signed comparison, wrapping
shift, base-word store and tail call. Original soft-float import identity is
verified independently. No whole VM, class recalculation or HP/MP body claim is
made by this comparison.

```powershell
python port/level-world/tests/run_character_script_set_level_host.py --compiler C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe --original-elf C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/test_strategy/libDungeonHunter2.so --cache C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/cache/files
```
