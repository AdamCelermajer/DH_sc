# Original character stat composition

Source C projects the original `CharProperties::RecalcProperty` calculation onto
explicit base, saved, equipment and ordered buff sheets. It uses the checked
[character property loader](../character-properties/README.md). This standalone
module does not own characters, create the original buff containers, schedule
buff lifetimes, install Lua object callbacks or run the Android game.

## Preserved rules

The serialized property type selects the first matching rule in this order:

| Type bit | Calculation |
| --- | --- |
| 4 | Default, then each set base/saved/equipment value, then all set buffs in ascending group/deque order |
| 2 | First set base/saved/equipment value, then first qualifying ascending buff group; last set sheet within that group wins |
| 1 | First qualifying descending buff group; last set sheet within that group wins, then equipment/saved/base/default fallback |
| 32 | Base plus saved, using the original sentinel-aware addition |
| 16 | Base |
| None | Retain the previous final value |

An unset value equals its serialized default. Addition replaces an unset result
with its delta; otherwise it wraps in 32 bits. Cancellation back to the default
therefore changes the next addition. A serialized type of -1 resolves to 16.
Multiple flags retain the original branch priority.

Caller groups are already ordered by the original map order and retain their
deque order. Source bounds are 32 groups, 64 sheets per group, 1,024 sheets total.
Only the selected final field changes. Invalid input/index/table/count or output
aliasing preserves output.

## Original instruction comparison

**585** original ARM32/compiled source ARM64/host comparisons pass, with exact
integer bits and whole-sheet state. They cover all 224 original cache types,
325 synthetic flag/group cases and 36 sentinel/priority edge cases. Actual
original ordered tree traversal and both deque iterator bodies execute, including
33- and 64-entry groups crossing the original 32-pointer block boundary.
No tree/deque getter or property calculation is stubbed.

The original 1,972-byte calculation, iterator bodies and property helpers are
hash pinned in [comparison evidence](arm-differential-validation.json). Fixtures
construct the original memory layouts directly; original map/deque construction,
insertion/removal and buff lifecycle are not exercised. Virtual cache reads,
bounded allocation/disposal and libc copy/compare dependencies use explicit
models. The test verifies source input preservation, original container
preservation, output guards and stack restoration. ARM64 executes in Unicorn,
without physical hardware or Android APK execution.

Strict ASan/UBSan passes **10,000** valid/damaged-input iterations, including
invalid group/sheet counts, aliases, indexes and corrupted table metadata.
[Host build/safety](host-build-validation.json),
[ARM64 16 KiB build](android-build-validation.json).

## Remaining integration

Full character ownership, property write routing, buff management, Lua entity
methods and Android integration remain unfinished. This calculation does not
provide combat, navigation, level progression or full source-built gameplay.
Recovered metadata remains subject to [the rights statement](../../RIGHTS.md).

## Reproduce

```sh
python3 port/property-composition/build.py --host --report /path/to/host.json
python3 port/property-composition/build.py --ndk /path/to/ndk --report /path/to/android.json
python3 port/property-composition/tests/differential.py --help
```

The differential check requires Unicorn/pyelftools, the original pinned library,
owner cache, arithmetic oracle and the two compiled source modules.
