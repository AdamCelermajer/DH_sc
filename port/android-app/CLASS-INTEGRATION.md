# Character class calculations in the Android 17 source preview

The current APK is **779,171 bytes**, SHA-256
`7d2c82ab3ba433e06c006bbe962f35b78f8266b4cd030d2be2440942a8231c79`.
Its source-built ARM64/x86_64 renderer and Lua libraries have 16 KiB ELF/ZIP
alignment, target SDK 37 and minimum SDK 26. It contains no original engine or
ARM translator. Full source gameplay remains unfinished.

## Use

Use the standard Android document picker to import both files from your own cache:

1. **Import character properties**: `data/pydata/character_properties_pyarray.bin`.
2. **Import character classes**: `data/pydata/character_classes_pyarray.bin`.
3. **Import script source**: select your plaintext Lua script.

```lua
local state = DH2CreatePropertyState(2)
state:ApplyClass(225)
local field = GetPyStruct("CharacterProperties", "Level")
local value = state:GetProp(field)
assert(type(value) == "number")
```

Create property objects after class import. Each retains its property and class
dataset generations. Import replacement affects newly created objects; old
objects retain their datasets. Closing the activity destroys the runtime.
Both data imports have a 4 MiB limit. Script limits and packaged shared sources
retain the earlier [script integration scope](SCRIPT-INTEGRATION.md).

`ApplyClass` is an **authored diagnostic method**. It applies the original checked
class rules to base and recomposes all final fields with empty buffs. Its optional
boolean selects current final sources; default is false. Repeated application
can compound base values. It does not reset/reload base or implement the original
Character lifecycle. See [binding scope](../lua-character/CLASS-BINDING.md).

## Exact-package checks

The installed APK hash matches on Android 17 x86_64 with 4 KiB and 16 KiB pages.
Each run imports the real property and class files, checks 40 real class
applications / 8,960 final values, and exercises retained generations, typed
arguments, malformed/oversized rejection, cyclic-application atomic failure and
recovery in the same process. No current-run fatal error occurred.

- [4 KiB class integration](classes-4k-runtime-validation.json)
- [16 KiB class integration](classes-16k-runtime-validation.json)
- [4 KiB script regression](classes-scripts-4k-runtime-validation.json)
- [16 KiB script regression](classes-scripts-16k-runtime-validation.json)
- [Textured character / animation regression](classes-animation-runtime-validation.json)

The same APK passes textured warrior/two-animation import, midpoint seeking,
paused 0/50/100% mixing, advancing Play at 100% and stable Pause on both page
sizes. All six paused mixing screenshots were visually inspected.

The component runner separately checks all 260 classes / 116,480 final values on
host, strict sanitizers and both Android page sizes. APK expectations come from
the checked host corpus and standalone source C, which was separately matched
to actual original ARM bodies. Original Lua gameplay equivalence and ARM64
hardware execution are unverified. No Fold7 was tested.

## Remaining work and reproduction

Actual Character construction, equipment/buffs, complete derived-stat lifecycle,
real combat, AI, navigation, level progression and the game loop remain unfinished.
The renderer's diagnostic timing and mixing retain their previous scopes.
Cache metadata/assets retain the [rights statement](../../RIGHTS.md).

```sh
python port/lua-runtime/tests/classes_corpus.py --help
python port/android-app/tests/class_runtime.py --help
python port/android-app/tests/script_runtime.py --help
python port/android-app/tests/animation_runtime.py --help
```

Generate the host class corpus before APK integration tests; pass its staging
directory as `--assertions`. The component corpus requires the checked host
reference library and Python Unicorn/pyelftools. With Windows adb from WSL, the
driver translates local push paths with `wslpath` and checks all remote hashes.
