# Checked equipment queries and weapon bonuses

`equipment.c` owns a bounded snapshot of slots 0, 1 and 2 in two equipment sets.
It reads item IDs from the checked loot cache, stores the needed item fields,
and projects these original functions:

- `GetCurrentEquipSet`, const/mutable `GetEquippedItem`, and `ItemInstance::GetItem`.
- `HasShield`, `HasOffHandWeapon`, `IsDualWielding` and `HasTwoHander`.
- Critical, attack and damage bonuses for either hand.
- `PROPS_GetIntWithBonus`, using explicit current final or temporary properties.

This snapshot is an authored ownership boundary. It does not implement original
ItemInventory allocation/mutation, item instances, gear-sheet contributions,
equip restrictions or the full Character lifecycle.

## Reconstructed rules

Slots 1/2 use the current set; slot 0 uses set 0. An absent item gives zero bonus.
Offhand type 6 is a shield; any other present offhand counts as dual wielding.
Weapon kind selects its critical/attack/damage property. Attack also adds dual
bonus when dual wielding; damage also adds two-hand and dual bonuses. Additions
wrap as 32-bit integers before arithmetic shifts, matching the original order.

Two-hand checks use main-hand slotting -4. Normal checks treat types 4/5 as
two-handed directly, otherwise consult `Special_Equip_2H_In_One` (final property
203 at original owner+0x1324). Ignore-rule checks omit that property. Damage bonus
uses the ignore-rule path. The snapshot caller refreshes this input after
property changes. Source supports weapon kinds -1 (no bonus) and 0..6.

`GetIntWithBonus` first shifts its selected property, then adds a separately
shifted main attack bonus for field 50; main damage bonus for fields 79/80;
offhand damage bonus for fields 81/82. Bonus calculations always use current
final properties, even when the initial value comes from temporary storage.

## Evidence

[differential-validation.json](differential-validation.json) passes **24,670
checks with zero mismatches** against original ARM32 and source ARM64/host:
all item/other-record destinations, all 1,322 actual item configurations,
synthetic shield/slot/kind/overflow cases, both set routing, and all 224 integer
fields with both temporary flags. Sixteen source snapshot loads execute on
ARM64; remaining bonus cases copy checked host snapshots into ARM64. Guards,
input preservation and stack restoration are checked. Source ARM64 runs in
Unicorn; this is not ARM64 hardware execution.

The strict build passes **12,000 ASan/UBSan safety iterations** covering bounded
reader mutations, truncation, aliases, invalid snapshots, preservation and
overflow. Build reports record source/artifact hashes and 16 KiB ARM64 alignment.

The [callback trace](../../reports/equipment-callback-trace.json) separately
executes 312 actual original bonus/HasShield callbacks and numeric/boolean/nil
`Value.getBool` paths. Captured return counts/types match owned source Lua
userdata, including zero, nonzero, NaN/infinity, missing/extra arguments and
both sets. ReturnValues pushes and int-to-Lua float32 conversion are explicit
boundaries. Original Lua interpreter and string/pointer coercion are untested.

See [owned script binding](../lua-character/EQUIPMENT-BINDING.md) and
[Android integration](../android-app/EQUIPMENT-INTEGRATION.md).

## Reproduce

From the repository with a C compiler and the configured WSL Unicorn/pyelftools
environment:

```text
python port/equipment-bonuses/build.py --host --report port/equipment-bonuses/host-build-validation.json
python port/equipment-bonuses/build.py --ndk PATH_TO_NDK --report port/equipment-bonuses/android-build-validation.json
python tools/trace_loot_tables.py --original PATH_TO_ORIGINAL_SO --oracle port/skin-payloads/build/oracle.so --cache PATH_TO_CACHE --report reports/loot-table-reader-trace.json
python port/equipment-bonuses/tests/differential.py --original PATH_TO_ORIGINAL_SO --oracle port/skin-payloads/build/oracle.so --cache PATH_TO_CACHE --host port/equipment-bonuses/build/equipment-host.so --arm64 port/equipment-bonuses/build/equipment-arm64.so --report port/equipment-bonuses/differential-validation.json
```

Private original/cache fixtures are not packaged by this module. Full source
gameplay remains unfinished. See [rights](../../RIGHTS.md).
