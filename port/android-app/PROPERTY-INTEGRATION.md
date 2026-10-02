# Property data and scripts in the Android 17 source preview

The current APK is **775,075 bytes**, SHA-256
`31c8e586004ce0932cc9853ec6a03b3140c6752f9b29d4ebcc1c16cfc3e61be7`.
It builds ARM64 and x86_64 renderer/Lua libraries with 16 KiB ELF/ZIP alignment,
target SDK 37 and minimum SDK 26. It contains no original engine or ARM translator.
Full source gameplay remains unfinished.

## Use

Install the APK. **Import character properties** opens Android's document picker.
Select `data/pydata/character_properties_pyarray.bin` from your own cache. The
status should say **Properties loaded. Script property objects are ready;
gameplay is unfinished.** The file is copied into the activity's current Lua
runtime; closing the activity destroys that runtime. The import limit is 4 MiB.

**Import script source** executes a plaintext file in the same runtime:

```lua
local state = DH2CreatePropertyState(2)
local hp = GetPyStruct("CharacterProperties", "HP")
local value = state:GetProp(hp)
state:SetProp(hp, value)
assert(state:GetProp(hp) == value)
```

This is an authored property object with four owned sheets and numeric/boolean
methods, not the original Character. It loads a record and recomposes properties
with no buffs. Derived class stats, actual entities, combat and gameplay remain
unfinished. See [binding evidence and limitations](../lua-character/README.md).
The three packaged shared scripts and execution limits retain their scope in
[the prior script checkpoint](SCRIPT-INTEGRATION.md).

## Exact-package checks

The installed hash matches on Android 17 x86_64 with both 4 KiB and 16 KiB pages.
Each run checks real cache import; 1,344 final/default queries from three records;
typed getter/setter behavior; two dataset generations; old-object retention;
malformed and oversized rejection; and successful recovery in the same process.
No current-run fatal error occurred.

- [4 KiB property evidence](properties-4k-runtime-validation.json)
- [16 KiB property evidence](properties-16k-runtime-validation.json)
- [4 KiB script regression](properties-scripts-4k-runtime-validation.json)
- [16 KiB script regression](properties-scripts-16k-runtime-validation.json)
- [Textured character / animation regression](properties-animation-runtime-validation.json)

The same package passes textured warrior import, two animation imports, midpoint
seeking, paused 0/50/100% mixing, advancing Play at 100% and stable Pause. All six
paused mixing screenshots across both page sizes were visually inspected.

The component runner separately checks all 448 real records / 100,352 composed
final values, plus 224 serialized defaults, on host, strict sanitizers and both
Android page sizes. APK queries use an authored reference of previously checked
composition rules; they do not establish original Lua gameplay equivalence.
ARM64 hardware execution is unverified. No Fold7 was tested.

## Reproduce

```sh
python port/android-app/build.py --help
python port/android-app/tests/property_runtime.py --help
python port/android-app/tests/script_runtime.py --help
python port/android-app/tests/animation_runtime.py --help
```

Cache assets/metadata retain the repository's [rights statement](../../RIGHTS.md).
