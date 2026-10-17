# Original class rules for derived character properties

The original class reader fully consumes the **34,224-byte** owner cache file,
containing **260 classes / 1,659 rules**. Actual ClassTable, ClassFuncList and
ClassFunc readers execute; all five-integer rule payloads match their original
destinations and ordered name counts agree. Stream reads, zeroed bounded
allocation/disposal and copying are explicit dependency models.
[Reader evidence](../../reports/character-class-reader-trace.json).

The portable C module loads the complete class table and applies its ordered
rules to four owned character sheets or a separate temporary sheet. Its
composition inputs have empty buffs. This is not the complete original Character
constructor, property lifecycle or source game.

## Original application rules

| Opcode | Operation |
| ---: | --- |
| 0 | Apply up to three referenced groups, in order; skip `-1` |
| 1 | Base value plus coefficient times arithmetic-shifted source property |
| 2 | Signed minimum/maximum clamp |
| 3 | No operation |
| 4 | Add another property |
| 5 | Scale by another fixed-point property |
| 6 | Scale using target or final, then execute the actual dispatch's AddValue fallthrough |
| 7 | Add a literal |
| 8 | Empty original helper, then return early from the current class |
| 9 | Set a literal ID/value |

Unknown opcodes and negative/out-of-range class IDs are original no-ops. Linear
rules use the destination property as the base when the serialized value is
`-666`. With final-source disabled, non-temporary destinations recalculate the
source property first; temporary destinations read their own source sheet. With
final-source enabled, linear/buff-scale rules read current final values. Arithmetic
uses low-32-bit wrap and an arithmetic eight-bit shift, matching the ARM bodies.

## Comparison and safety

**3,270 original ARM32 / compiled source ARM64 / host comparisons pass**:

- 2,600: all 260 real classes, all five destination sheets, both final-source flags.
- 620: authored opcode, integer-overflow, sentinel, no-op and early-return cases.
- 50: original invalid-class no-ops.

Original application, group, linear/clamp/scale/add/set and property recalculation
bodies execute. All four state sheets and the temporary sheet match exactly.
Original reserved bytes, actual class records, property inputs, source output
guards and stack restoration are checked. [Comparison](arm-differential-validation.json).
Source ARM64 runs in Unicorn; hardware execution is unverified.

The source wrapper validates complete input, rejects output/input aliases, and
stages changes before committing. It caps data at 4 MiB, 4,096 classes and 65,536
rules; application caps recursion at 64 levels and 100,000 rule/group visits.
Cyclic/unsafe original behavior is not executed; these source guards and atomic
failure recovery are authored. [Strict host sanitizers](host-build-validation.json)
pass 12,000 safety iterations. [ARM64 build](android-build-validation.json)
has 16 KiB ELF alignment.

Buff-inclusive application, original Character construction, complete derived-stat
lifecycle, Lua class binding, APK integration and full gameplay remain unfinished.

## Reproduce

```sh
python tools/trace_character_classes.py --help
python port/character-classes/build.py --help
python port/character-classes/tests/differential.py --help
```

Original engine and cache inputs are supplied by the owner. See [rights](../../RIGHTS.md).
