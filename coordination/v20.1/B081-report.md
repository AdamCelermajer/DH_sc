# B081 - red X next to "5 Potion" in the inventory list

## Cause (observed in code, shot-3)
The red X is not a font glyph. It is the Status clip frame "No" that the inventory list draws on rows whose item requirements are unmet
(B057, `features/inventory/inventory_details.cpp`, `!row.requirements_met`). The row's quantity "5" is the separate Number field.
Our `equipment_menu::Presenter::view` computed `requirements_met = equipment_equippable_by(...)` for every category, including category 9
(valuables/potions), so the potion row got the "unmet" X.

## Original logic (IDA, libDungeonHunter2.so pseudocode-all.c)
- `NativeInvGetItemsListForSlot` 0x44bb94: `v9 = (slot == EquipmentSlots.Count)` (category 9 -> `GetValuableItems`);
  `ItemEquippable = v9 || ItemInstance::IsEquippableBy(item, player)`; `ItemEquipped = !v9 && ...`.
  So category 9 rows are always "equippable".
- SWF `GenerateInventoryListItems` (authored-actions.txt 0001d2ee-0001d318): `if(!ItemEquippable) Status.gotoAndStop("No") else "Empty"`.
  Hence the original never shows the red X on a valuables/potion row.

## Fix
`features/equipment/equipment_menu.cpp`: `row.requirements_met = slot_==9 || equipment_equippable_by(...)`. Generic (category rule, no item hack).
Test: `equipment_adapter_tests.cpp` gives Potion0 an unmet level requirement and checks the category-9 row stays `requirements_met`
(fails before the fix: "B081: category9 potion row flagged unmet (red X)"; passes after).

## Verification
- Unit test before (fail) / after (pass). Build exit 0 (-Jobs 2); ctest 165/166, only `session_skill_binding` fails (allowed in worktrees).
## Not verified
- Reference video: no potion-category inventory frame located (only equipment categories at P1 t=336/372 were previously studied), so
  "original shows no X on potions" rests on the IDA/SWF logic, not on footage.
- Real EXE before/after frames not captured; why the potion row failed `equipment_equippable_by` in the user's run (level/stat requirement
  of Potion0 vs. class gate) was not determined - the fix makes it irrelevant for category 9.
- The EQUIP button on a potion row was also gated by the same flag; it now follows the original (enabled for category 9).
