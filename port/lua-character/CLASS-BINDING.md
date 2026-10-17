# Owned class rules on script property objects

`dh2_lua_import_character_classes` now atomically installs an owned class table
inside the source Lua runtime. Newly created `DH2CreatePropertyState` objects
retain both their property dataset and the current class dataset generation.
Older objects keep their own datasets after replacement. Objects created before
class import must be recreated to acquire class data.

```lua
local state = DH2CreatePropertyState(2)
state:ApplyClass(225)
local level = state:GetProp(GetPyStruct("CharacterProperties", "Level"))
```

**ApplyClass is an authored diagnostic method**, not a reconstructed original
Lua callback. It applies the checked native class rules to base, then recomposes
all 224 final fields with empty buffs. An optional boolean selects current final
sources during class application; its default is false. Repeated applications
can compound base values. It does not reset/reload base, construct an original
Character or implement the complete derived-stat/gameplay lifecycle.

Class indices must be exact nonnegative integers inside the imported table.
Malformed arguments, unsafe indices and cyclic/oversized application reject
without changing object state. Class import failure retains the previous table.
Storage is charged to the existing Lua allocator; caller bytes are released
before querying. Userdata metatables and retained datasets remain private.

## Checks

All **260 real classes**, both final-source modes and **116,480 final-field
queries** pass against the standalone C implementation, which was separately
matched to actual original ARM32 bodies in 3,270 checks. This validates the new
Lua binding; it does not execute those queries in the original Lua interpreter.

| Runtime | Evidence |
| --- | --- |
| Host | [execution](../lua-runtime/classes-host-execution-validation.json) |
| Strict host sanitizers | [execution](../lua-runtime/classes-host-sanitizer-execution-validation.json) |
| Android 17, 4 KiB | [execution](../lua-runtime/classes-android-4k-execution-validation.json) |
| Android 17, 16 KiB | [execution](../lua-runtime/classes-android-16k-execution-validation.json) |

Selftests check buffer release, retained generations, malformed/OOM class import
recovery and atomic cyclic-application failure. Existing property, parse,
arithmetic, ordered-name, constant and shared-script checks also pass on all four
runtime configurations; reports use the `classes-` prefix.

No buffs, original Character lifecycle, real combat or full gameplay are added.
See [native class evidence](../character-classes/README.md),
[property binding](README.md) and [rights](../../RIGHTS.md).
