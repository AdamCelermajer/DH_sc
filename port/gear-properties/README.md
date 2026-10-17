# Gear stat contributions and empty-buff character recalculation

This source C component reconstructs item/power contributions and the original
base/gear update order. It operates on explicit owned sheets and borrowed input
tables. Original Character/inventory ownership, equip requirements, random power
generation and full source gameplay remain unfinished.

## Reader evidence

The actual original array, nested record and primitive readers execute against
the complete 54,443-byte `data/pydata/item_powers_pyarray.bin`, SHA-256
`bba8cbb48cc3cc3c32b9419bbb709f253c68066500873a9f30f014a94914bc1e`.

| Table | Rows | Native record bytes | Serialized byte range, end exclusive |
| --- | ---: | ---: | --- |
| ItemPowerList | 121 | 12 | 0–12,578 |
| ItemPowerTable | 937 | 40 | 12,578–54,443 |

All native scalar/nested destinations are captured; both counts agree with the
ordered name tables. The list entries serialize an integer ID and signed byte.
Each power serializes a signed byte type, integer value, a list of three-integer
stat entries and five trailing integers. Source validates both complete tables;
its record API exposes the power records as borrowed byte spans. It does not
select/generate powers from the lists. See [reader trace](../../reports/item-power-reader-trace.json).
Stream reads, zeroed bounded allocation and disposal are modeled dependencies.

## Reconstructed behavior

Item type routes min/max damage to main or off hand. Types 4/5 also contribute
the wrapped fixed-point base element. Shields contribute damage reduction and
block; armor types contribute damage reduction. Contributions use the original
default-sentinel-aware add operation.

Power entries retain order. Opcodes 0–48 route their value to the checked stat
fields; opcode 9 sets the hand's element, others add. Multi-field opcodes affect
the original field order. Unknown opcodes are no-ops. The unused third stat
integer is retained by the reader. All arithmetic wraps to the low 32 bits.

`dh2_gear_load` adds each present slot's item stats followed by its assigned
powers. Only slot 2 is the left hand. Caller supplies slots in original order;
inventory selection and memory ownership are explicit inputs.

`dh2_character_recalc` optionally applies the class ID in base field 26 to the
base sheet with `from_final=false`, then recalculates all 224 fields in ascending
order. `update_base` resets base, loads a valid requested row, then recalculates
with class application. Negative/out-of-range row IDs retain the reset defaults
before recalculation, matching the original guarded loader. `update_gears`
resets gear stats, loads gear/power contributions, then performs class and final
recalculation. Saved stats remain retained; repeated updates can compound base
class rules. This API uses empty buffs because buff-inclusive class application
has not yet been reconstructed.

## Verification

**7,147 comparisons** pass with zero mismatches against actual original ARM32
instructions, compiled source ARM64 in Unicorn and host C:

- All 937 power record destinations on host/ARM64.
- All 1,322 real items and 937 real powers in both hands, comparing all four sheets.
- 624 synthetic opcode, sentinel, repeated add/set and signed overflow cases.
- All 260 classes with/without class application, all 448 base rows and invalid-row cases.
- 32 combined gear loads and 64 repeated reset/load/class/recalculation updates,
  using actual original inventory/instance/power getters with bounded fixtures.

Whole sheet payloads, native reserved fields, source output guards, source cache
preservation and stack restoration are checked. See [comparison report](differential-validation.json).
Fixtures contain 0–32 slots and current-set selection; original unsafe memory or
debug/assert paths are excluded. ARM64 hardware and original Lua execution are
not claimed by this comparison.

Strict ASan/UBSan checks pass 12,000 iterations, including all opcodes/item types,
wrap, truncation, damaged views, aliases, rejected inputs, atomic state preservation
and recovery. See [host build/safety](host-build-validation.json) and
[16 KiB ARM64 build](android-build-validation.json).

## Reproduce

```sh
python3 port/gear-properties/build.py --host --report port/gear-properties/host-build-validation.json
python port/gear-properties/build.py --ndk <ndk-r29> --report port/gear-properties/android-build-validation.json
python port/gear-properties/tests/differential.py --original <original-so> --oracle port/skin-payloads/build/oracle.so --cache <cache-files> --host port/gear-properties/build/gears-host.so --arm64 port/gear-properties/build/gears-arm64.so --report port/gear-properties/differential-validation.json
```

Reader/comparison drivers require Unicorn and pyelftools. Original/cache files
remain external under the repository's [rights scope](../../RIGHTS.md).
