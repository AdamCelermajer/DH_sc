# Owned gear/power stat lifecycle in Lua

Diagnostic property objects now retain an owned item-power dataset alongside
their exact property, class and loot generations. Imports copy caller bytes,
validate the complete file and replace only the current generation after success.
Existing objects retain old data across replacement and collection. Objects
created before an import retain its absence and must be recreated to use it.

The added methods are authored controls on source objects, not reconstructed
original Lua argument callbacks or original Character objects:

| Method | Input and behavior |
| --- | --- |
| `EquipGear(set,slot,item)` | Set 0/1, slot 0..15, item row or -1; assigns the snapshot and clears that slot's powers. |
| `EquipItem(set,slot,item)` | Earlier slot 0..2 control; now also assigns the gear snapshot and clears powers. |
| `SetItemPowers(set,slot,...)` | Up to 32 valid power IDs, retained in order; no IDs clears powers. |
| `UpdateBaseProperties(row)` | Checked signed integer row; resets/loads base, applies its class and recalculates. |
| `UpdateGearsProperties()` | Resets gear sheet, adds selected item/power stats, applies the base class and recalculates. |
| `RecalculateProperties([applyClass])` | Optional strict boolean (default true); class application then all 224 fields. |

Inputs must be actual numbers with exact integer values or the specified boolean;
strings, NaN, infinity, unsafe casts, invalid indices/counts reject. Rejection
preserves owned state. Dataset imports are bounded to 4 MiB and charged to the
runtime memory budget. Lua instruction hooks do not interrupt work inside these
bounded C calls.

Slots 1/2 use the selected equipment set; all other slots use set zero. Slot 2
is the off hand. Snapshot assignment does not automatically update gear stats;
call `UpdateGearsProperties` explicitly. The bonus snapshot's owner hand rule
refreshes after lifecycle changes. Buffs are empty; rendered armor/models do not
change. There are no equip requirements, original ItemInstance allocations,
random powers, inventory persistence, entity or combat lifecycle.

Host and strict sanitizer selftests cover owned caller release, dataset absence,
replacement/retention, invalid method atomic rejection, main/off-hand switching,
armor/shield/power contributions, base reload and memory-exhaustion recovery.
The corpus checks all 1,322 items and 937 powers in both hands: **4,518 cases /
1,012,032 final-field queries**. Expectations use the standalone C component
separately matched to original ARM32 instructions in 7,147 comparisons.
Lua query values retain the runtime's float32 representation; the corpus does
not execute original Lua/gameplay callbacks. See [component source](../gear-properties/README.md)
and the `gears-*-execution-validation.json` reports under `port/lua-runtime`.

Use `port/lua-runtime/tests/gears_corpus.py` with `--runner`, `--cache`,
`--reference` and `--report`; optionally `--adb` and `--serial` for the Android 17
component runner. The Android preview imports the cache through the document
picker. Full source gameplay remains unfinished.
