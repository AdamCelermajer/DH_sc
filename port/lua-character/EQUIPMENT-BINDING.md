# Owned item data on diagnostic script objects

`dh2_lua_import_loot_tables` atomically copies a validated whole eight-table item
file into Lua-accounted userdata. Newly created `DH2CreatePropertyState` objects
retain its generation along with their property and class datasets. Replacement
affects new objects; old objects retain their exact input data. Objects created
before item import must be recreated to use equipment controls.

```lua
local state = DH2CreatePropertyState(2)
state:EquipItem(0, 1, 0) -- set, slot, item row; -1 removes an item
state:SelectEquipmentSet(0)
local attackBonus = state:GetAttackRatingBonus(false)
local shield = state:HasShield()
```

`EquipItem` and `SelectEquipmentSet` are **authored diagnostic snapshot controls**.
They accept exact numeric integers: sets 0/1, slots 0..2, and item row -1 or a
valid row. A failed call preserves the whole snapshot. Assigning any item to a
slot is allowed here; original equip requirements and inventory mutation remain
unimplemented. These controls do not contribute item properties to the gear
sheet or alter the preview model.

The original names `GetCritRatingBonus`, `GetAttackRatingBonus`, `GetDamageBonus`
and `HasShield` use the checked source projection. Bonus methods return no values
when their first argument is missing. Boolean false, numeric zero and nil select
main hand; boolean true and nonzero numbers select offhand. NaN/infinity count
as nonzero. Extra arguments are ignored. String/pointer arguments are rejected
through a controlled error. `HasShield` returns a boolean and ignores arguments.

Every cache item passes through the Lua binding on host, strict sanitizers and
Android 17 x86_64 with 4 KiB/16 KiB pages: **2,644 set configurations and 18,508
bonus/shield queries**. Expected values come from the standalone projection
separately checked against original ARM instructions. Selftests cover caller
release, typed arguments, generations, malformed/OOM import retention and
recovery. The 312-case original callback trace separately checks the supported
argument paths against host source Lua.

- [Host corpus](../lua-runtime/equipment-host-execution-validation.json)
- [Strict host corpus](../lua-runtime/equipment-strict-host-execution-validation.json)
- [Android 4 KiB corpus](../lua-runtime/equipment-android-4k-execution-validation.json)
- [Android 16 KiB corpus](../lua-runtime/equipment-android-16k-execution-validation.json)
- [Original callback trace](../../reports/equipment-callback-trace.json)

These are diagnostic property/equipment objects. The original Character, buffs,
inventory lifecycle, AI, combat dispatch and full gameplay remain unfinished.
