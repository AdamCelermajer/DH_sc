# Typed character property methods and owned Lua state

## Current class-rule extension

Objects can now retain owned class datasets and call the authored `ApplyClass`
diagnostic method. All 260 real classes / 116,480 final-field queries pass on
host, strict sanitizers and both Android 17 page sizes. See [class binding and
limits](CLASS-BINDING.md). Earlier numeric/boolean callback evidence below keeps
its original scope; full Character and gameplay lifecycles remain unfinished.

This component reconstructs the numeric/boolean `Character:GetProp` and
`Character:SetProp` paths. It also supplies an authored, owned Lua property
userdata. It does not implement the original Character constructor, derived
base-stat lifecycle, buffs, combat, navigation or the game loop.

## Original callback evidence

[Differential evidence](arm-differential-validation.json) records **1,586**
original ARM32 / compiled source ARM64 / host comparisons: 905 getters and 681
setters, covering all 224 properties. Whole four-sheet state, input preservation,
output guards and stack restoration match. Original callback bodies, argument
indexing, numeric/boolean accessors, unsigned float conversion and native property
operations execute. Integer return pushes, stream/allocation dependencies and
finite signed float conversion have explicit models.

Property IDs use the original unsigned conversion: finite negative values clamp
to zero. Property write values use signed truncation. Getter optional boolean
true selects the original global temporary/default sheet; the test resets that
sheet through the actual original reset body to serialized defaults. Its other
possible global lifetimes are untested. Pointer overloads, original unsafe casts,
empty-setter diagnostic handling, buff-inclusive callbacks and original Lua
interpreter equivalence are untested. ARM64 execution here uses Unicorn.

[Host build and strict sanitizers](host-build-validation.json) pass 12,000
safety iterations, including aliasing, malformed arguments and unsafe casts.
[ARM64 build](android-build-validation.json) has 16 KiB ELF alignment.

## Owned Lua binding

`dh2_lua_import_character_properties` validates the complete three-table file,
copies it into Lua-accounted storage and atomically installs the dataset. Invalid
input and memory exhaustion retain the prior dataset. Each object keeps its own
dataset generation after replacement and caller buffer release.

`DH2CreatePropertyState(row)` is an **authored diagnostic factory**. It initializes
four owned sheets, loads a base record and recomposes all fields with no buffs.
This initial checkpoint exposes numeric/boolean `GetProp` and `SetProp`; the
newer class extension is described above. It is not an
original Character object. Unsupported pointer arguments and unsafe casts raise
controlled errors. Global `GetProp`/`SetProp` callbacks remain absent.

```lua
local state = DH2CreatePropertyState(2)
local hp = GetPyStruct("CharacterProperties", "HP")
local value = state:GetProp(hp)
state:SetProp(hp, value)
local serializedDefault = state:GetProp(hp, true)
```

The current runtime checks all **448 real rows / 100,352 composed final fields**,
plus 224 serialized defaults. Expectations are an authored reference of
separately original-tested composition rules; this is not execution of all rows
through the original Lua interpreter. Selftests cover typed calls, malformed/OOM
import retention, dataset replacement and old-object lifetime.

| Runtime | Evidence |
| --- | --- |
| Host | [execution](../lua-runtime/properties-host-execution-validation.json) |
| Strict host sanitizers | [execution](../lua-runtime/properties-host-sanitizer-execution-validation.json) |
| Android 17, 4 KiB | [execution](../lua-runtime/properties-android-4k-execution-validation.json) |
| Android 17, 16 KiB | [execution](../lua-runtime/properties-android-16k-execution-validation.json) |

All current parse/arithmetic, ordered-name, constant and shared-script regressions
also pass on host, strict host sanitizers and both Android page sizes. Reports in
`port/lua-runtime` use the `properties-` prefix. Native gameplay objects remain
unfinished. The Android app integration is documented separately.

## Reproduce

```sh
python port/lua-character/build.py --help
python port/lua-character/tests/differential.py --help
python port/lua-runtime/build.py --help
python port/lua-runtime/tests/properties_corpus.py --help
```

Original engine and cache inputs are owner supplied. See [rights](../../RIGHTS.md).
