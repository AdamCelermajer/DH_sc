# B060 Belt/Helm icons swapped

## Evidence
- Video Part 1 ~t=366 s (frame extracted, main equipment page): right column top to bottom = head (helm icon, "Empty"), hands, belt (belt icon, "Vagrant Belt"), boots, potions. Left: torso, main hand, off hand, ring, ring.
- User screenshot b060: "Conscript Belt" at the top-right row (helm icon), "Conscript Helm" at the third row (belt icon).
- Reproduced in the Preview 15 EXE (`--equipped-item PlateBelt01/PlateHelm01`, `--equipment-page-frame 60`): same swap.
- Logic: `engine-ui/reference/character-menu-flow-v1/authored-actions.txt` Init439: btn_head sets InvSlotId=8 (0001a2a7), btn_waist sets InvSlotId=7 (0001a3c5). Loot constants: 7=Waist, 8=Head; `source_equipment_appearance.cpp` index 8 = MC_Head.

## Root cause
`export_equipment_art.py` mapped btn_head -> slot 7 and btn_waist -> slot 8, so generated `original_equipment_art.cpp` / `source_equipment_layout.json` drew slot 7 (Waist, the belt item) on the head button and slot 8 (Head) on the belt button. The inventory-details art (`source_inventory_layout.json`) already had the right order. Slot names/items and icons are bound by `SlotArt.source_slot`, so the whole row (icon+name+hit area) was swapped, not just the icon.

## Change
- exporter mapping fixed; generated data and layout json updated by hand (no Python on this machine) with the same swap.
- test: `equipment_main_page_tests` asserts each slot art category/button equals the original InvSlotId table (torso,main_hand,off_hand,feet,hands,ring,ring,waist,head).

## Verification
- ctest: all pass except `session_skill_binding` (known) and `winmm_pump_priority_v1` (0xc0000135, missing DLL in this environment; unrelated).
- Real EXE before/after (quiet run): before = Belt next to helm icon; after = "Conscript Helm" top-right with helm icon, "Conscript Belt" third row with belt icon, matching the video order. Images: `.local-inputs/claude-preview15/fix060/runs/{before,after}-bh.png`.
- Contact sheet of all slot categories: the same after capture shows torso, main hand, off hand, ring, ring, head, hands, waist, feet, potions icons, each on its own named slot; consistent with the video. Other slots were not swapped.

## Remaining gaps
- Details-page rail icons not re-captured (they are indexed by InvSlotId and were correct in source). Dark "normal" rail art for icons 0,1,2,5,6 still missing (B056 area).
- Package files required: new dh-foundation.exe only.
