# Equipment calculations in the Android 17 source preview

The current APK is **807,843 bytes**, SHA-256
`2974a802e258582a13c20a0d38aad19a44ceff313170473a85c0ff68322cafdd`.
Its source-built ARM64/x86_64 renderer and Lua libraries have 16 KiB ELF/ZIP
alignment, target SDK 37 and minimum SDK 26. It contains no original engine or
ARM translator. Full source gameplay remains unfinished.

## Use

Use the standard Android document picker to import files from your own cache:

1. **Import character properties**: `data/pydata/character_properties_pyarray.bin`.
2. **Import character classes**: `data/pydata/character_classes_pyarray.bin`, when using class rules.
3. **Import item data**: `data/pydata/loot_table_pyarray.bin`.
4. **Import script source**: select your plaintext Lua script.

```lua
local state = DH2CreatePropertyState(2)
state:EquipItem(0, 1, 0) -- sample set, slot and item-row IDs
state:SelectEquipmentSet(0)
local attackBonus = state:GetAttackRatingBonus(false)
local critBonus = state:GetCritRatingBonus(false)
local damageBonus = state:GetDamageBonus(false)
local shield = state:HasShield()
assert(type(attackBonus) == "number" and type(shield) == "boolean")
```

Create objects after data imports. Each retains its exact property, class and
item dataset generations. Replacement affects new objects; old objects retain
their data. Closing the activity destroys the runtime. Data imports have a
4 MiB limit. Scripts retain the earlier [execution limits](SCRIPT-INTEGRATION.md).

`EquipItem` and `SelectEquipmentSet` are authored snapshot controls. Sets are
0/1; slots are 0..2; item row -1 clears a slot. Invalid calls preserve the
snapshot. These controls do not enforce original equip requirements, mutate an
inventory, contribute gear properties or change the rendered character model.
See [binding details](../lua-character/EQUIPMENT-BINDING.md).

## Exact-package evidence

Installed APK hashes match on Android 17 x86_64 with 4 KiB and 16 KiB pages.
Each run performs 15 imports in one process: real item data, 106 actual item
configurations in both sets / 1,484 bonus and shield assertions, supported typed
arguments, old-data retention after replacement, malformed/oversized rejection
and recovery. The source character model, texture and 25-track walking animation
also import successfully. Both final preview screenshots were inspected.
No current-run fatal error occurred.

- [4 KiB item integration](equipment-4k-runtime-validation.json)
- [16 KiB item integration](equipment-16k-runtime-validation.json)
- [Build, signatures and alignment](build-validation.json)

The component runner separately checks all 1,322 items / 18,508 bonus and shield
queries on host, strict sanitizers and both Android page sizes. The standalone
reader/query comparison passes 24,670 original ARM32 versus source ARM64/host
checks; 312 supported original bonus/shield callback cases also match host Lua.
Original Lua gameplay equivalence and ARM64 hardware execution are unverified.
No Fold7 was tested.

The same exact APK also passes the existing class integration (40 actual class
applications / 8,960 final queries), seven shared-script rejection/recovery
imports, and the textured two-motion animation regression on both page sizes.
All six paused 0/50/100% mixing screenshots were inspected; Play advances and
Pause remains stable. These checks verify the diagnostic preview, not gameplay.

- [4 KiB class regression](equipment-classes-4k-runtime-validation.json)
- [16 KiB class regression](equipment-classes-16k-runtime-validation.json)
- [4 KiB script regression](equipment-scripts-4k-runtime-validation.json)
- [16 KiB script regression](equipment-scripts-16k-runtime-validation.json)
- [Animation regression and visual inspection](equipment-animation-runtime-validation.json)

## Remaining work and reproduction

The full original Character lifecycle, inventory mutation and requirements,
gear contributions/powers, buffs, AI, combat dispatch, level loading and source
gameplay remain unfinished. The independently tested Test11 compatibility build
still runs the original ARM32 engine through translation.

Build with `port/android-app/build.py` using the configured SDK/NDK. Reproduce
the owned-item checks with `port/android-app/tests/equipment_runtime.py`; use
the checked component corpus from `port/lua-runtime/tests/equipment_corpus.py`.
Original cache inputs remain external; see [rights](../../RIGHTS.md).
