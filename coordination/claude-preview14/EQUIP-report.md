# Preview 14 report EQUIP (branch p14/equip)

Status: CONTINUATION ROUND 3. Steps 1-4 of the brief are committed and verified (Drop, Transmute, B045, Auto-Equip, ALL).
Step 5 (B042 visual leftovers) is NOT done; its status and next probes are in section 5. Build: 105 ctests, 104 pass;
`session_skill_binding` is the known worktree environment failure (asset root junction), ignored as instructed.

Commits on p14/equip (this round): `271a80cb` drop seam, `6f53f579` --bag-item diagnostic, `2008ce88` B045 fix + test.
Earlier: `f35f4047` Drop/Transmute owners (previous worker), `22508fcd` WIP (auto-equip, ALL, class gate, 202/203 flags).

## Evidence note (AGENTS.md investigation rule)
- Visual evidence: reference Part 1 t=372 (`.local-inputs/claude-preview14/equip/ref/ref-372.png`): Feet Details for an unequipped
  candidate (Vagrant Boots, Armor 6, Req 4 dex) with VALUE 11, TRANSMUTE and DROP buttons, "Total Gold 13" bottom-left. Our
  captures of the same screen (ClothBoots01 candidate, Value 7) show the same layout: `r3/out/f-transmute-feet.png`,
  `r3/out/sel-y232.png`. Direct observation, not inference.
- Logic evidence: IDA `pseudocode-all.c`: `Character::INV_TransmuteItem(item,false)` 0x3a4a3c (value = max(1, (TransmuteMultiplier *
  ((ItemInstance value<<8) * (prop197+256) >> 8)) >> 16); one unit leaves the bag (RemoveItem if qty<=1 else AddQty(-1)); AddGold(value);
  PROPS_AddInt(213,1); trophy "gear_transmute" when counter >= 300); `NativeInvTransmuteItem` 0x43f23c (calls INV_TransmuteItem(idx,0),
  INV_UpdateSkin); `NativeInvDropItem` 0x43cfb4 (offline: ItemInventory::TransferItemTo(inv,idx,temp,1) then ItemObject::DropInventory:
  ONE unit becomes a world item at the player; online: CMsgDropLoot message). Authored script: both buttons call menu_confirm2 first
  (GAMEPLAYMENUS_TRANSMUTE_QUESTION / GAMEPLAYMENUS_DROP_QUESTION, sound MenuConfirm); buttons are disabled/hidden for equipped rows.
- Expected behaviour: Transmute pays Value gold and removes one unit; Drop removes one unit and places a world item; neither acts on an
  equipped row; the Details list reselects after the change.
- Uncertainties: confirmation popup not ported (see Open risks); world item system is the DROPS stream; the 213 counter and trophy
  are not reproduced (no persisted slot in schema v3).

## 0. Starting state
Reviewed WIP `22508fcd` and `f35f4047` (Drop/Transmute owners from the previous continuation worker, which had no report entry).
Checked `f35f4047` against IDA: transmute formula in `runtime_equipment_text_v1.cpp` `source_transmute_value` matches
INV_TransmuteItem exactly (wrapped multiply, >>8, >>16, min 1). The Details ValueBox and the Transmute action share one packet.

## 1. Auto-Equip (per slot and ALL) — done in WIP, verified in the EXE
- Per-slot: `Equipment AutoEquip slot=3 -> slot0=StartingSuit/... slot3=StartingBoots/... slot4=StartingGloves/...` (run `f-autoequip-feet`,
  Feet slot, `r3/out/f-autoequip-feet.log`). The candidate chosen was the equipped Ceremonial Boots (SortByValueAndClass favours it over
  ClothBoots01); the equipped row is unequipped then re-equipped, as the original `_EquipSlotAuto` does.
- ALL: `Equipment AutoEquip ALL -> slot0=... slot1=... slot3=... slot4=...` (run `f-all-main`, release at 305:187:52 on the main sheet
  "Auto-equip All" banner). Empty slots stay empty (no candidate).
- Stat recalculation: NOT verified in logs. The EXE prints no damage/armor line; `EquipmentAdapter::change()` recalculates gear stats
  (equipment_adapter.cpp:93-110) but no run compares Armor/Damage before and after. The Details "Armor: 1" text is unchanged in captures.
  Unit tests cover the adapter path (equipment_adapter_tests).
## 2. Class restriction / rules — done in WIP (equipment_equippable_by + equipment_class_allows; SortByValueAndClass table; 202/203 flags)
Unit tests: equipment_adapter_tests (ctest PASS). Not re-verified in the EXE this round.

## 3. Drop and Transmute
Implemented (`features/equipment/equipment_inventory_actions_v1.{hpp,cpp}`, test `equipment_inventory_actions_v1_tests.cpp`):
- `transmute_inventory_item`: refuses equipped/unknown/amount<1/gold overflow; removes one unit; adds gold. Atomic.
- `drop_item_to_world(one_unit, error)`: the narrow DROPS seam. It returns false with "no world store bound" until the DROPS branch is
  merged; the bag is not changed on failure. `drop_inventory_item(character, id, error, publish = &drop_item_to_world)` publishes ONE unit
  first and removes it from the bag only after success.
- main.cpp dispatch (request_transmute / request_drop block, ~2436-2460): transmute amount from the shared packet; Details reselection.
Runtime (EXE, quiet batch `r3/jobs-final.json`):
- `f-transmute-feet`: `Equipment Transmute instance=bag-0-ClothBoots01 item=ClothBoots01 gold 0 -> 7 amount=7`; the capture shows
  Total Gold 7 and only Ceremonial Boots left in the list (`r3/out/f-transmute-feet.png`).
- `f-transmute-sword03`: `Equipment Transmute instance=bag-1-Longsword03 item=Longsword03 gold 0 -> 55 amount=55`.
- `f-drop-feet` / `f-drop-sword03`: `Equipment Drop diagnostic: no world store bound instance=bag-0-ClothBoots01` (bag unchanged).
- Equipped rows: Drop hidden / Transmute disabled (inventory_details.cpp gating; the click returns with no command).
Tests: `equipment_inventory_actions` PASS (transmute single/stack/failure/overflow; drop publish-one-unit, stack keeps row, failure keeps
item, unbound store fails loudly).
Not done (documented limitations):
- Confirmation popup: the original pushes `menu_confirm2` (dqcharmenu_droid.swf contains it; text GAMEPLAYMENUS_TRANSMUTE_QUESTION /
  GAMEPLAYMENUS_DROP_QUESTION, values in common_text_pycst.bin). Not ported: no art or menu for it in the port, and this step does not
  invent one. Transmute and Drop act on the first press. This is a known deviation (see Open risks).
- CharProperties 213 counter and trophy "gear_transmute" at 300: not reproduced.

## 4. B045 second-row hit regions — FIXED
Cause (measured): `DetailRowArt::hit` for each list row is only the bottom border sliver (3.3 units high, x 66.6-193.5), e.g. rel +1
hit y=[230.9,234.2], rel 0 y=[204.9,208.2]. The visible row body (selected highlight `list/btn_0/6` etc., x 33.2-224.2, 21 units tall,
pitch 26) was not hittable, so a click on the second row missed and fell through: `MainPage::release` returned to the main-sheet slot
hit test, and a click at (112,220) selected "Ring 1" (captured as `r3/out/feet-probe-r194` and `row-220` before the fix).
Fix:
- `inventory_details.cpp`: `row_contains` uses the union of the row's visible art (unselected + selected batches) as the hit box;
  `details_row_hit()` exported in `inventory_details.hpp`.
- `equipment_main_page.cpp`: while Details is open, a miss inside Details returns consumed (no fall-through to main-sheet slots/ALL).
Test: `features/inventory/details_row_geometry_tests.cpp` (CMake target `inventory_details_row_geometry`): centre of rows -1/0/+1 selects
that row; rows abut without gap or overlap; boundary belongs to the next row; no leak outside width or below the row. PASS.
EXE evidence: the second-row band scan before the fix selected only y 231-234 (`r3/out/band-y*.log`: transmute worked at 231/233/234
only). After the fix, a click at y=220 selects ClothBoots01 (`f-transmute-feet.log`).

## 5. B042 leftovers — NOT DONE this round (investigation notes only)
Status per item (no change made; nothing invented):
1. Details plate UVs / damask extent (ours ends x~173, reference x~219): not started. Next: decode `original-cache/data/menus/MenuGraphics02.tga`
   with port/engine-textures and map the plate UVs (the survey confirmed the decode works).
2. Grey top-right / orange-brown bottom-right panels: not started. Survey probe list still applies (colour transform on sprite456 depth 5,
   shape 440 217,64,0 alpha 6/255; dependence on equipped state). In the current captures the right panels are dark (see section 4 images:
   the reference shows grey UNEQUIP and orange EQUIP panels).
3. Flat white sword on the avatar: not started. The weapon atlas `atlas_weapons_dh2.tga` decodes correctly (survey); the EXE material path
   is not compared yet.
4. Slot icon column brightness (ours 69-75, reference 24-33): not started.
5. Selected-row glyph vs quantity: our rows show "1" on every row (also unselected ones, e.g. "1 Ceremonial Boots" in
   `r3/out/sel-y232.png`), while the reference t=372 shows no quantity glyph on Feet rows. Next: check the quantity rule in the
   original Details list (not investigated).
Also observed: black separator bars and a black gap in the Details area in our captures (the list frame lines), not in the reference.

## Needs from schema v4
- Transmute counter CharProperties 213 (PROPS_AddInt) and the "gear_transmute" trophy trigger at 300 (needs a persisted counter).
- Nothing else. The Drop world item needs the DROPS stream's world store, not a save field here.

## Package files required
- None new. The EQUIP changes use the existing Preview 13 assets (common_text_pycst.bin and the Details art already in the port).
  The confirmation popup, when ported, needs dqcharmenu_droid.swf `menu_confirm2` art (already in the package).

## Verifier script
Quiet batch (`port/windows-foundation/tools/quiet_run.ps1 -JobsFile ... -Parallel 8`), args = Preview 13 b016 pkg base args with the
equipped items `StartingSuit StartingGloves Longsword01 StartingBoots`, `--combat-main-item Longsword01`, `--bag-item ClothBoots01 --bag-item
Longsword03`, `--equipment-page-frame 300`, then `--menu-release` steps:
- Transmute: `305:400:228 315:112:220 330:272:300` -> expect `Equipment Transmute instance=bag-0-ClothBoots01 item=ClothBoots01 gold 0 -> 7 amount=7`.
- Drop: `305:400:228 315:112:220 330:429:307` -> expect `Equipment Drop diagnostic: no world store bound instance=bag-0-ClothBoots01`.
- Second row (B045): `305:400:228 315:112:220` -> no log; the next release on 330 selects the row (Transmute works).
- Auto slot: `305:400:228 315:126:51` -> expect `Equipment AutoEquip slot=3 -> ...`. ALL: `305:187:52` -> `Equipment AutoEquip ALL -> ...`.
- Right hand: `305:112:132 315:112:220 330:272:300` -> `Equipment Transmute instance=bag-1-Longsword03 ... gold 0 -> 55 amount=55`.
Harness note (C7): with `--start-mode swamp` the save's equipment and bag are NOT loaded (direct bootstrap); equipment comes from
`--equipped-item` and bag rows from `--bag-item` (diagnostic). Real profile route not exercised.

## Open risks
1. Transmute and Drop run on the first press, without the original menu_confirm2 confirmation (GAMEPLAYMENUS_TRANSMUTE_QUESTION /
   DROP_QUESTION). Transmute is irreversible. This must be ported (art from dqcharmenu_droid.swf) before a preview ships.
2. Drop has no world owner until the DROPS branch is merged: it fails loudly by design.
3. Stat recalculation after equip/transmute is not verified in the EXE (no stat log line).
4. B042 visual leftovers (section 5) are open.
5. `--bag-item` is a diagnostic arg in main.cpp (direct bootstrap only); it is not persisted and does not change the save path.
6. Dual-wield / two-hander policy flags (202/203) are unit-tested only; not verified in the EXE this round.
