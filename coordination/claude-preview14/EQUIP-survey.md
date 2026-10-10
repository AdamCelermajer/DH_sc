# Preview 14 survey: EQUIP (equipment / inventory pages)

Read-only survey. No file under port/, docs/ or tools/ was edited. Scratch and captures are under
`.local-inputs/claude-preview14/equip/` (decode/, run/, ref/). EXE used: `.local-inputs/windows-source-clock-v19-preview-13-rc2/dh-foundation.exe`
through `port/windows-foundation/tools/quiet_run.ps1` (hidden, silent). Two quiet batches were run (5 + 2 jobs), all exit 0.

Summary: equip/unequip/drop-target/transmute-target plumbing exists and runs in the EXE. Auto-Equip works as a
native "equip this selected item into its own slot" call, which is NOT the original per-slot best-item behaviour.
The ALL auto-equip button is not wired. Drop and Transmute are not handled by main.cpp. Several original rules
(class restriction, two-hander policy flags) are not enforced or are unverified. The B042 grey/orange panels are
still unexplained, but the PVRTC decoder claim in the old notes is wrong: both candidate atlases decode.

## A. EXISTING

| Item | Files | Purpose | Status |
|---|---|---|---|
| Main equipment page | features/equipment/equipment_main_page.{cpp,hpp} (+_tests) | Slot/Details hit handling; DetailAction -> MainPageCommand (equipped, unequipped, request_drop, request_auto_equip, request_transmute) | component-tested; integrated (EXE capture shows page + Details) |
| Equipment presenter | features/equipment/equipment_menu.{cpp,hpp} | Row view per owned item: slotting, applicable_to_selected_slot, requirements_met, equipped, required/actual stats | component-tested; integrated |
| Adapter (mutations) | features/equipment/equipment_adapter.{cpp,hpp} (+_tests) | change(): rebuild native EquipmentState72V3, call dh2_equipment_to_slot_v3 / dh2_equipment_auto_v3 / dh2_equipment_from_slot_v3, reload gear stats/powers, recalc, write vitals | component-tested (rogue auto_equip test); integrated via runtime binding |
| Native equipment rules | port/game-data/player_equipment_v3.cpp (dh2_equipment_*_v3) | equip(), unequip(), automatic() slotting codes, two() two-hander check | source/component-tested |
| Runtime binding | features/equipment/runtime_equipment_binding_v1.{cpp,hpp} | auto_equip / unequip / equip_to_slot / menu_actions (equip+unequip callbacks), refresh of render+appearance | integrated (main.cpp bindEquipmentPage, ~1817-1870) |
| Runtime page | features/equipment/runtime_equipment_page_v1.{cpp,hpp} | Character-menu equipment provider; take_source_pending_command; uses runtime.menu_actions (runtime_equipment_page_v1.cpp:59) | integrated |
| Equipment text / ValueBox | features/equipment/runtime_equipment_text_v1.*, transmute_value in main.cpp (~1870) | Item text, transmute value from property 197 x design multiplier | component-tested (B016 provider); integrated |
| Avatar | features/equipment/runtime_equipment_preview_v1, runtime_equipment_avatar_matrix_v1_tests, runtime_player_locomotion_* | Preview pane, attachments | B019 matrix passed in isolation; normal-renderer capture still open per docs |
| Native gear callbacks | features/equipment/native_gear_actions.{cpp,hpp} | Alternate NativeGearActionBinding::callbacks(); not on the runtime path traced here | source only (not traced further) |
| Inventory main tabs | features/inventory/inventory_menu.{cpp,hpp}, original_inventory_art.cpp | Category icons/hit contours, ItemColor fill (main rows only) | component-tested; integrated |
| Inventory Details | features/inventory/inventory_details.{cpp,hpp} | Details overlay: art, rows, action hits (equip/unequip/drop/transmute/auto_equip), carved-frame drop, Drop hidden for equipped | component-tested; integrated (EXE capture) |
| Inventory projection | source_inventory_projection_v1.cpp, source_item_descriptors.cpp, source_character_inventory_binding.cpp, source_instance_resolver.cpp | ItemTable-backed names/stats/requirements; stable projection IDs | component-tested |
| Potions | features/inventory/runtime_session_potion_use_v1.* (main.cpp ~2064-2072, 2688) | Potion count on main page ("Potions: N" from Potion0 count); potion use | integrated; potion tab UI not checked in this survey |
| Character-menu composition | features/character_menu/* (character_menu.cpp, original_art.cpp, menu_text*.cpp) | Tab selection, page registration, original art table (includes btn_GAMEPLAYMENUS_AUTOEQUIP_ALL art) | integrated |
| Texture decoder | port/engine-textures (textures.cpp, pvrtc.cpp), used by port/windows-foundation/texture_loader.cpp | BTEX/PVR v2 open + PVRTC 2/4bpp decode, validated against original ARM (per its README) | integrated in the Windows texture loader |

main.cpp dispatch of the Details result (lines ~2405-2421): only `request_auto_equip` is handled
(`runtimeEquipment->auto_equip(pending.selected_instance_id)`). Every other command prints
"Equipment action requires a gameplay owner: command=N". equip/unequip go through menu_actions inside the page.

## B. ORIGINAL BEHAVIOUR

Sources: IDA pseudocode `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`;
authored actions `port/engine-ui/reference/character-menu-flow-v1/authored-actions.txt`; reference video Part 1.

B1. Details Auto-equip button (observed in the authored script, inference for the call chain).
- authored-actions.txt ~8996-9005 (0001de14-0001de55): `btn_AutoEquip` onRelease -> `NativeInvAutoEquipSlot(InvSlotId)`,
  then `GenerateInventoryListItems`. Observed: the button passes the selected SLOT id, not an item id.
- IDA 0x43da2c NativeInvAutoEquipSlot (pseudocode ~227547): NativeGetPlayerChar, checks slot < GetNumEquipmentSlots,
  then Character vtable+324(slot) and vtable+320(slot). Inference: +324 = unequip slot, +320 = auto-equip slot. Names
  of those vtable entries were NOT resolved.
- IDA ItemInventory::_EquipSlotAuto (pseudocode ~186199): GetItemListForSlot(slot); sort with SortByValueAndClass; take
  the first item where IsEquippableBy(character) and not already equipped; _EquipItemToSlot(slot, item); return 1.
  So the original picks the BEST unequipped candidate for that slot.
- SortByValueAndClass (pseudocode ~182992): switch on a per-class PyDataConstant (Character+5064), scaling item value
  (field +84 word 21) by item multipliers at +32 / +48 (cases 263/264, others not read). Full table NOT decoded.
- ItemInstance::IsEquippableBy (pseudocode ~181033): online-bypass branch; then Item[34] class code maps to a class
  table row (1 KnightPlayerBase, 2 Berserker, 3 Paladin, 4 RoguePlayerBase, 5 Assassin, ...) compared to the player class.

B2. Main-sheet ALL button.
- authored-actions.txt ~7044-7052: `btn_GAMEPLAYMENUS_AUTOEQUIP_ALL` onRelease -> `NativeInvAutoEquipSlot(-1)`.
- NativeInvAutoEquipSlot with -1 calls vtable+312 (inference: all slots).
- MenuBase::FS_AutoEquipSlot (0x42143c, pseudocode ~208312) for the string "ALL": unequip every slot (+324) in order,
  then auto-equip from the LAST slot down to slot 0 (+320). Observed in code; the same ordering is implied for the
  native ALL path.
- The ALL button art exists in the port table: character_menu/original_art.cpp, `menu_InventorySheetMain/btn_GAMEPLAYMENUS_AUTOEQUIP_ALL/3/1`
  (x 187.55, y 51.7) and its text. It is not in DetailAction and not in the main-page hit list.

B3. Equip/unequip/drop/transmute authored (observed in SWF script): btn_EquipItem, btn_Unequip, btn_Drop, btn_GAMEPLAYMENUS_TRANSMUTE2.
Hit contours (authored 480x320, from original_inventory_art.cpp, bounding boxes computed by me):
- auto_equip x 46-206 y 40-62; unequip x 322-482 y 40-62; equip x 321-481 y 181-203;
- drop x 379-479 y 297-318; transmute x 223-322 y 261-318; previous x 0-30 y 64-88; next x 0-30 y 283-306.

B4. Reference video, Part 1 t=372 (v1.0.3, one still, observed directly, timing otherwise unknown): Details for the
Feet slot. Top-left list panel dark damask with an "Auto-equip" button across the top. Left list: Ceremonial Boots
(equipped) and Vagrant Boots (selected, orange row). Top-right grey panel: UNEQUIP, Ceremonial Boots, Armor: 1.
Bottom-right orange-brown panel: EQUIP, Vagrant Boots, Armor: 6, Req: 4 dex, Value 11, TRANSMUTE and DROP buttons.
Bottom-left "Total Gold 13". Avatar with the sword in the right hand. Frame: `.local-inputs/claude-preview14/equip/ref/ref-372.png`.
Part 1 t=330 is gameplay (not equipment): `ref/ref-330.png`. The Q-report's frames at t=336/342 were not re-viewed in this survey.
No main-sheet (non-Details) equipment frame was found in the frames checked; the ALL button's on-screen appearance is UNVERIFIED.

## C. GAPS

Each has evidence. "Run" = quiet batch in `.local-inputs/claude-preview14/equip/run/`.

C1. Details Auto-equip does not do the original action. Port: equipment_main_page.cpp:97 -> request_auto_equip ->
runtime_equipment_binding_v1.cpp:402 -> equipment_adapter.cpp:35 `auto_equip(id)` = `change(&id,9)` -> native
automatic() (player_equipment_v3.cpp:30) = equip THE SELECTED item into its own slot. Original = unequip the slot, then
equip the best unequipped candidate for the slot (B1). Run `autoequip-bag03`: log `Equipment AutoEquip instance=equipped/Longsword01 slot=1`
(the click landed on the equipped row, so the test did not exercise a bag item).

C2. ALL auto-equip button missing (B2). No DetailAction/hit for it, no command in main.cpp. Not visible in the base capture
(`run/base-knight-bag03.png`, expected position authored (187,52) = screen ~(467,130), empty there).

C3. Drop and Transmute are not executed. main.cpp prints "Equipment action requires a gameplay owner" for request_drop
(world drop: depends on the drops stream) and request_transmute (no owner). Expected: the Details Drop/Transmute buttons
should show disabled, or call the owner. Currently the press does nothing visible.

C4. Class restriction not enforced by the port. IDA IsEquippableBy switches on Item[34] (class). grep of
features/equipment and port/game-data for `words[34]` / IsEquippableBy found nothing. equipment_meets_requirements
(equipment_adapter.cpp:19-27) checks only words[29..33] stats against properties 19,149,150,151,152. Whether the native
service callbacks apply the class check is UNVERIFIED (not traced into the EquipmentServices16V3 callbacks).

C5. Two-hander / shield flags are defaults. equipment_adapter.hpp:16 `dual_wield=false, one_hand_two_hander=false`;
main.cpp never sets them (only copied at runtime_equipment_binding_v1.cpp:284-286). The native automatic() uses the
state flags flag1320/flag1324 to convert slotting (-3 -> off-hand allowed, -4 -> two-hander), and two() checks a
two-hander in main hand before a shield goes on (slotting 2 unequips main). The off-hand/shield unequip path is
therefore present, but the original source of the flags is unknown (open question G3).

C6. Bag item not offered for Right hand. In run `base-knight-bag03` / `autoequip-bag03` the Right-hand Details list shows
only "1 Useless Blade" (the equipped Longsword01). The save `b016-knight-bag-longsword03.save` holds `bag-longsword03 Longsword03`
(DHSAVE listing). Cause unknown: applicability (slotting/type), requirements_met, or the item is not in the projection.
Needs one check (the Presenter view row for bag-longsword03).

C7. Harness confound: equipment page binds only when the --equipped-item args are present. Run `v2-*` (args without
`--equipped-item`/`--combat-main-item`) logs `Foundation error: Equipment page diagnostic selection: Equipment provider is
awaiting the current Session binding`. The runs with the args override the save's equipment bindings
(`Equipment state count=4 ... equipped/StartingSuit ... equipped/Longsword01`). The real user path (main menu -> load
profile) was NOT exercised by this survey; treat all these runs as diagnostic only.

C8. Stat effects on equip not measured. adapter change() recomputes gear stats/powers/vitals (equipment_adapter.cpp:93-110),
but no run compared Damage/Armor/Stats before and after equip, and the character-menu Stats page was not checked against it.
Main page shows "Damage 12 - 15" for Useless Blade (observed).

C9. Gold/potions. Main page: coin icon with "0" (observed). Details: "Total Gold 0" bottom-left (observed); reference
t=372 shows "Total Gold 13". Gold source not checked. Potions: "Potions: 0" on the main page (observed); the potion
count is Potion0 quantity (main.cpp:2064). Potion tab/use not verified in this survey.

C10. Equip/unequip from the list: menu_actions -> RuntimeEquipmentBindingV1::equip_to_slot/unequip (runtime_equipment_page_v1.cpp:59).
Code path exists; NOT exercised by a click in this survey (no captured after-equip frame).

C11. B042 visuals (see section E4 for the full task list): grey top-right and orange-brown bottom-right panels still absent
(ours dark brown in run `autoequip-bag03.png`, reference t=372 grey/orange); damask extent differs; left slot-icon column
brightness differs; selected row shows the glyph where the reference shows quantity "1" (Q-report, not re-checked here).

## D. DESIGN (minimal, reusable)

D1. Per-slot auto-equip (replaces the item-based call for the Details button):
- New `EquipmentAdapter::auto_equip_slot(slot, error)` in equipment_adapter.cpp next to auto_equip (line 35):
  1. `dh2_equipment_from_slot_v3(&state, slot, -1, &services)` (unequip, as the original does first).
  2. Candidates = owned rows with `applicable_to_selected_slot` for this slot (equipment_menu.cpp:40-50 rule), not equipped,
     `equipment_meets_requirements`, and the class check from IsEquippableBy (new helper: Item[34] -> class row ID).
  3. Sort by SortByValueAndClass (needs the full per-class multiplier table; see G2). Pick the first.
  4. Equip with `dh2_equipment_to_slot_v3(&state, slot, index, 0, &services)`, then the existing refresh path in change().
- ALL: `auto_equip_all()` = unequip slots 0..8 in order, then `auto_equip_slot` from slot 8 down to 0 (FS_AutoEquipSlot ordering).
- Command plumbing: equipment_main_page.hpp enum add `request_auto_equip_slot` (keep `request_auto_equip` name or rename it),
  equipment_main_page.cpp:97-99 pass `source_slot`; add an ALL hit in the main page (art already in original_art.cpp) that
  returns `request_auto_equip_all`.
- main.cpp (~2412-2415): branch on command: slot form calls `runtimeEquipment->auto_equip_slot(pending.source_slot)`; ALL calls
  `auto_equip_all()`. Keep the refresh/render-change block after it unchanged.
- Drop/Transmute: keep the diagnostic branch; make the Details buttons inert (no command) until the owners exist, rather than a
  visible no-op. Drop belongs to the drops stream.

D2. Persistence: change() already rewrites CharacterState.equipment and inventory; no new save fields. Old saves unaffected.

D3. Texture (B042 layer): no decoder work needed. port/engine-textures reads both candidate atlases (see E4).

## E. WORK BREAKDOWN

E1 (S, under 2h): per-slot Auto-equip + ALL button hit/command in the adapter/page/main.cpp (D1).
Test: equipment_adapter_tests (knight and rogue rows: Feet slot with two boots, one better: expect the better one, the old
one unequipped; ALL: every slot filled with the best candidate; a slot with no candidate stays empty; equipped items not
re-chosen). Verifier (quiet batch): save with a bag holding 2+ candidates for one empty slot; click the slot (authored
coords from the page layout), click Auto-equip at (126,51) authored; capture and log expect `Equipment AutoEquip slot=N -> item=X`.
Do not reuse the --equipped-item args (C7); use a save that already holds the equipment, or fix C7 first.

E2 (M, under half day): class restriction + full SortByValueAndClass decode (IDA ~182992 and IsEquippableBy ~181033).
Test: strict unit tests with real ItemTable rows; a class-restricted item must not be offered to the other class.

E3 (S, under 2h): bag-item visibility (C6) and the harness fix (C7): quiet batch without the forced args on a real loaded
profile, prove the bag item appears in the candidate list and Drop/Transmute show their state.

E4 (M to L): B042 visual tasks, as a list:
 1. Plate atlas: decode is possible (see below). Map the SWF bitmap used by menu_InventorySheetDetails/plates to its cache file
    (candidate `original-cache/data/menus/MenuGraphics02.tga`, 1024x1024 PVRTC4; contains damask base, carved corner/pillar art
    and button bars). Verify the plate UVs against this atlas, then compare the damask extent (ref damask to stage x ~219; ours 173).
 2. Grey top-right / orange-brown bottom-right panels: not in the Details display list of dqcharmenu_droid.swf (Q-report 1.3).
    Next probes, in order: (a) colour transforms (PlaceObject cxform/alpha) on the Details children and on sprite456 depth 5
    (shape 440 is a full-stage solid 217,64,0 at alpha 6/255: the orange hue matches, the alpha does not); (b) whether the
    panel tint depends on equipped state (ref: grey = equipped/UNEQUIP panel, orange = candidate/EQUIP panel); (c) if neither,
    the v1.0.3 video art differs from the v1.0.2 SWF (open question G4). Do not invent a fill.
 3. Left slot-icon column brightness: compare the icon batch colour/alpha with ref t=372 (dimming on equipped vs candidate).
 4. Selected row: glyph vs quantity "1" (ref shows the quantity on the selected row).
 5. Re-capture the Feet Details state (equipped and candidate) and compare panel-by-panel against ref-372.png.

## F. DEPENDENCIES / CONFLICTS

- Drops stream: Drop (C3) needs the world-drop owner; the equipment page must not own it.
- Skills/stats: stat recalc after equip (C8) shares the CharacterState/property path with the Stats page.
- Main-menu metadata: the equipment page needs a loaded profile with real equipment (C7); coordinate with the profile loader.
- Shared files: main.cpp equipment dispatch block (~2405-2421) and bindEquipmentPage (~1817-1870); equipment_main_page.{cpp,hpp};
  equipment_adapter.cpp; inventory_details.cpp (B042 visual owner); original_art.cpp (character_menu) for the ALL art hit.
- B042 visual work touches inventory_details.cpp and the character_menu art tables; the equipment logic (E1/E2) touches the adapter
  and main page. Two owners, disjoint files, except main.cpp.

## G. OPEN QUESTIONS

G1. Vtable names of Character +312 (all), +320 (equip slot auto), +324 (unequip slot): confirm before implementing E1.
G2. SortByValueAndClass per-class multiplier table (which PyDataConstant and which item fields per class): needed for E2.
G3. Source of the two-hander / dual-wield policy: are flags1320/1324 driven by a character property, a Debug switch, or a constant?
G4. Is the grey/orange panel art in the v1.0.3 video absent from the v1.0.2 SWF (version difference), or drawn at runtime?
G5. Main-sheet ALL button: is it visible on the main page in the reference video? No main-page frame was checked in this survey;
    need a Part 1 frame of the equipment main sheet.
G6. Bag item visibility (C6): is Longsword03 excluded by slotting, by requirements, or by the projection?

## Texture check (brief item): decode result

- `dec.exe` (scratch, compiled against port/engine-textures textures.cpp + pvrtc.cpp with the llvm-mingw toolchain) decodes:
  - `atlas_weapons_dh2.tga` (BTEX PVR v2, 1024x1024, PVRTC 4bpp, payload 524,288): decoded fully; the PNG shows the sword,
    shield and armour art correctly (`decode/weapons_512.png`). This is the weapon texture.
  - `envmap_swamp.tga` (64x64, PVRTC 4bpp): decodes.
  - `MenuGraphics02.tga` (1024x1024, PVRTC 4bpp): decodes; damask and carved frame visible (`decode/menugraphics02_512.png`).
- The Windows texture loader (port/windows-foundation/texture_loader.cpp:54-68) already calls dh2_texture_open/dh2_texture_decode.
  So the sword texture is NOT a decode failure. The old note "no CPU decoder here" (Q-report 1.5) is wrong.
- Not checked: the rendered sword in the EXE versus the reference (material/envmap path and colour). Next step: a quiet
  capture of the Details avatar sword and a side-by-side with ref-372.png.
- Caveat: the decoder is the legacy PVR path; the 2bpp path differs from the SDK decoder (engine-textures README). All three
  decoded files here are 4bpp.

## Verification status

- Source read: equipment/*, inventory/*, character_menu/*, main.cpp dispatch, engine-textures, IDA excerpts above.
- Runtime: 2 quiet batches (7 EXE runs, exit 0). Auto-equip log line observed in 3 runs (all on the equipped Longsword01, see C1);
  the 2 `v2-*` runs failed to bind the equipment page (C7).
- Not run: equip/unequip click from a bag, drop, transmute, potion tab, stat before/after, main-sheet ALL, real profile load.
