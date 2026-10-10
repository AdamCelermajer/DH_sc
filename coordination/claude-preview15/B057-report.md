# B057 Imbued Armor "REQ: 7 ENG" cannot be equipped

## Verdict
Blocking is the ORIGINAL behaviour; the bug was that our Details panel gave no feedback and that the gate was slightly wrong.

## Evidence
- Video (Part 1) 8:32 (reference frame) and the 8:10-9:00 sheet: unmet items ("Useless Claw" REQ 8 DEX, "Imbued Cowl", "Thief Boots") show a red X at the row start and a dark EQUIP button; met items show neither. The Knight cannot equip "Imbued Cowl" in the original either. Green name = item colour (1 power), unrelated to requirements.
- Authored AS (character-menu-flow-v1/authored-actions.txt 0001c98b, 0001d2ee): per row `ItemEquippable` false -> Status.gotoAndStop("No") (sprite 101 frame 1 = red X) else "Empty"; selected item not equippable -> btn_EquipItem.gotoAndStop("disabled") (sprite 179 frame 19).
- IDA: `ItemInstance::IsEquippableBy` 003fa330: online bypass; level (Character dword 1041) >= Item[29]<<8; for STR/DEX/END/ENG: (Character[1175+j] + Character[1171+j]) >= Item[30+j]<<8 (our property cells Stat_* 149..152 + Prereq_* 153..156); class switch on Item[34]. Callers: NativeInvGetItemsListForSlot 0044bb94 / NativeMerchantGetItemsListForSlot 00444168 feed `ItemEquippable`.
- Mapping verified: REQ words 29..33 = LEVEL/STR/DEX/END/ENG (item_presentation_v5.cpp keys), ENG = Stat_Energy (152). Fresh Knight Energy < 7 (Imbued Armor row is blocked in the exe).
## Root cause
1. `equipment_meets_requirements` ignored Prereq_* (153..156); the UI query owner already added them (inconsistent).
2. The Windows Details panel (inventory_details.cpp) never rendered Status "No" / disabled EQUIP, and EQUIP release on an unmet item went to the adapter and failed with an error.
## Change
- equipment_adapter.cpp: raw 8.8 compare `Stat + Prereq >= req<<8` (level without prereq).
- inventory_details.cpp/hpp + generated `original_inventory_gate_art.inc` (exporter `export_inventory_gate_art.py`, included at the end of original_inventory_art.cpp so test scripts keep compiling one file): red X per unmet (not equipped) row, disabled EQUIP art/label for an unmet selection, EQUIP release no-op when disabled. Auto-equip already used the same gate (adapter).
- export_art.py: two compat fixes so the SWF exporter runs again (exporter had bit-rotted; regenerating original_inventory_art.cpp was NOT done because the current output differs from the committed hand-patched file).
## Tests
- equipment_adapter: per stat (STR/DEX/END/ENG) below/equal/above, Prereq bonus (whole and partial), level boundary.
- equipment_main_page: unmet item shows X + disabled EQUIP art + EQUIP release no-op; equal boundary equips; above passes; Prereq_Energy makes it pass; revert; art switches back.
- ctest 119: all pass except the known `session_skill_binding`.
## Before/after (real EXE, hidden desktop, silent)
B057-before-sel.png vs B057-after-sel.png (Torso, Imbued Armor selected, REQ 7 ENG: after = red X on Imbued/Mystic Armor rows, dark EQUIP). B057-before-rh/after-rh: right hand list, Plain Blade (unmet DEX) X, Flawed Moon (met) none. Scratch: .local-inputs/claude-preview15/fix057/.
## Remaining / not verified
- A real EXE click on the disabled EQUIP was verified by unit test only. Class-restricted items are handled by the same gate (existing test).
- VALUE text overlapping the avatar is B056 layout (not touched). Equipped-row glyph (Status "Yes"/"Yes2") not done.
- Which class the reporter used is unknown; a Mage with Energy >= 7 can equip it (equal boundary tested).
- Merchant/sell lists do not use this Details panel (not changed).
## Package files required
None new (art comes from the existing cached dqcharmenu data compiled into the exe).
