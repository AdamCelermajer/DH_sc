# Gear stats and powers in the Android 17 source preview

The tested source APK is **836,515 bytes**, SHA-256
`d0005277e5839989c63a38cb2e86add55347b99150181e0baf944ca9db26e40f`.
It targets SDK 37 (minimum 26), with source-built ARM64/x86_64 libraries aligned
for 16 KiB ELF/ZIP loading. It contains no original engine or ARM translator.
Full source gameplay remains unfinished.

## Use

Import these files from your own cache through the standard document picker:

1. **Import character properties**: `data/pydata/character_properties_pyarray.bin`.
2. **Import character classes**: `data/pydata/character_classes_pyarray.bin`.
3. **Import item data**: `data/pydata/loot_table_pyarray.bin`.
4. **Import item powers**: `data/pydata/item_powers_pyarray.bin`.
5. **Import script source**: your plaintext diagnostic Lua source.

```lua
local c = DH2CreatePropertyState(2)
c:EquipGear(0, 1, 0) -- sample set, slot and item row
c:SetItemPowers(0, 1, 0) -- sample power row
c:UpdateGearsProperties()
local damage = c:GetProp(GetPyStruct("CharacterProperties", "Damage_Min_Main_Hand"), false)
c:UpdateBaseProperties(2)
c:RecalculateProperties(false)
assert(type(damage) == "number")
```

Create objects after all imports. Each retains its exact four dataset generations.
Import replacement affects new objects; existing ones retain their earlier data.
Closing the activity destroys the runtime. Each data import is limited to 4 MiB;
the script session has an 8 MiB memory budget and the earlier [execution limits](SCRIPT-INTEGRATION.md).

These are authored controls on diagnostic property objects. There are 16 slots
in two sets, up to 32 power IDs per slot, and explicit gear updates. Only slots
1/2 use the selected set; other slots use set zero. Equipping a row clears its
assigned powers. Updates reconstruct item/power contributions, class application
and all-field recalculation with empty buffs. They do not enforce equip
requirements, create an original inventory, save gameplay or change the rendered
character/armor. See [binding scope](../lua-character/GEAR-BINDING.md).

## Exact-package evidence

On both Android 17 x86_64 page sizes (4 KiB and 16 KiB), the exact installed APK
hash matches the build. Each run performs **20 imports in one process**, including
all four real datasets and **70 selected item/power configurations / 15,680
final-field queries** from the checked host corpus. Synthetic cases check armor,
shield/powers, main/off-hand set selection, base reset/reload, repeated gear
updates, retained generations, typed/index rejection, malformed/oversized input
and recovery. No current-run fatal error occurs.

- [4 KiB gear integration](gears-4k-runtime-validation.json)
- [16 KiB gear integration](gears-16k-runtime-validation.json)
- [Build, signatures and alignment](build-validation.json)

Both final ready screenshots show the textured source character and all import
buttons. The same exact package separately passes two-animation import, midpoint
seek, paused 0/50/100% mixing, advancing Play at 100% and stable Pause. All six
paused mixing screenshots were inspected. See [animation evidence](gears-animation-runtime-validation.json).

The component runner passes **all 1,322 items and 937 powers in both hands /
1,012,032 final queries** on host, strict sanitizers and both Android page sizes.
Existing class, equipment-bonus, name, constant, script and property component
corpora also pass on this runtime. Standalone C matches actual original ARM32
instructions versus source ARM64/host in **7,147 comparisons**. These are component
and diagnostic app checks; original Lua combat/gameplay equivalence, ARM64
hardware and a complete source-built game remain unverified. No Fold7 was tested.

## Remaining work and reproduction

Original Character ownership, inventory mutation/requirements, random power
generation, buff-inclusive class application, AI, combat dispatch, level loading,
save lifecycle and source gameplay remain unfinished. The independently tested
Test11 compatibility APK runs the original ARM32 engine through translation.

Build using `port/android-app/build.py` with the configured SDK/NDK. Reproduce
the package check with `port/android-app/tests/gears_runtime.py`; the full
component corpus is `port/lua-runtime/tests/gears_corpus.py`.
Original cache inputs remain external under [rights](../../RIGHTS.md).
