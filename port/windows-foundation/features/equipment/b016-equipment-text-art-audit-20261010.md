# B016 equipment text and art audit — 2026-10-10

## Finding

The current frozen production path renders the Details row title and quantity without overlap and composes one Transmute label with a separate Value label. The supplied earlier still shows the reported failure, but its current-build captures do not reproduce it. I found no equipment-owned source correction to make. B016 remains open for a representative long item name and the real transmute amount projection.

## Visual evidence

- The supplied still `C:/Users/adamc/AppData/Local/Temp/codex-clipboard-7529b917-27f9-48a7-b95b-dc2796298344.png` has no timestamp or sequence metadata. It visibly shows a list quantity/title collision and three visible “Transmute” labels (two above the button and one on the button).
- Frozen Details capture `.local-inputs/v19-frontend-hotfix/source-panes/rogue-details.png`, captured 2026-10-10 01:08, from `source-panes/dh-foundation.exe` SHA-256 `1FBA9F36032226F5EFA7B391DD0486E848A22FF4A8A87CC3A587967FD3581214`: list title and quantity are separated; the equipped selection has its disabled Transmute art omitted while the Value text remains. This is one still, not animation evidence.
- Frozen Details capture `.local-inputs/v19-frontend-hotfix/current-abi/rogue-transmute-enabled.png`, captured 2026-10-10 01:22, from `current-abi/dh-foundation.exe` SHA-256 `E11BBF4F79B3FAA9D526D32F076E40F7CA55D67FFD5862BA3897D41C5FEE33CE`: list title and quantity are separated; the enabled state shows one Transmute label and one Value label. The amount field is blank. The item label is “Useless Spike”, so this capture does not cover a long label.

## Original art and logic evidence

The exact source asset is `port/android-native/app/src/main/assets/original-cache/data/menus/dqcharmenu_droid.swf`, SHA-256 `43227075f407626b52eca4c345ac6f568023fcb39c269588201026c0fc6760e0`. In `menu_InventorySheetDetails`, sprite456 depth 212 contains child sprite449. Its authored `Idle` frame 0 has two button-art batches; `disabled` frame 23 has no button batches or solids. Both frames retain three independent font-103 fields: `ButtonName/text`, `ValueText/value`, and `ValueBox/value`. The extracted layout records their frame-specific baselines at y=299.35/269.2/280.3 for idle and y=325.35/331.4/333.5 for disabled.

The source ActionScript trace identifies `displaySelectedItemInfos` at offset 116837 and `DisplayEquipedItem` at 118392. The former refreshes selected-item details and the latter only toggles the fixed `EquipedSwordIcon`; the recovered slice does not establish every ActionScript caller gate. The native consumer `NativeInvGetItemDetails` is IDA address 0x44ca5c (capture hash `ab3e94cc…6963`). It resolves the current player and actual inventory index, then formats `ItemTransmuteValueString` from the actual item value, `CharacterDesign.TransmuteMultiplier`, resolved property 197, and the original StringManager formatter. This is the source for `ValueBox`; it cannot be guessed from the label.

The current normal menu renderer already maps each menu glyph quad through all four corners of the field matrix in `main.cpp::drawMenuGlyph` (around line 2992), followed by the authored viewport scale/translation. The earlier inventory note pointing at an `OverlaySprite` width/height shortcut refers to a different target-HUD loop, not the current menu-row renderer. The current captures confirm that the production menu text path is rendering these fields with the corrected affine geometry.

The current feature composition gates Details through `equipment_menu::MainPage::release`: an equipment-slot or inventory-slot hit opens Details, and `DetailsPresenter::frame` emits it only while open. Its selected-row `equipped` state chooses the authored idle or disabled sprite449 state. For transmute text, `DetailsPresenter::frame` removes those three paths from generic projection and writes each role once: `GAMEPLAYMENUS_TRANSMUTE2` to ButtonName, `MENU_VALUE_TITLE` to ValueText, and the actual ItemTransmuteValueString callback to ValueBox when supplied. `DetailsPresenter::release` only returns a typed transmute request; the real transmute owner is still external to this page.

## Remaining gap and verification

`RuntimeEquipmentTextProviderV1::bind` fills names, generic item details, symbols, and potion text, but does not supply `DetailBindings::transmute_value`. Consequently the current enabled capture correctly shows the separate Value label but cannot yet show the amount. The actual native owner/query path must supply that string from the same live ItemInstance before the art/text page can claim full value fidelity. This work did not edit shared `main.cpp`, inventory-owned files, actions, or CMake.

The existing source composition/runtime test was run against the current executable and passed:

`runtime_equipment_binding_v1_tests.exe <workspace-root>` → `PASS same CombatSession player/world/CharacterState, original MAINPAGE/Details composition, typed drop/auto-equip/transmute source requests, existing AutoEquip kernel and render receipt, source modular preview transitions/prefix failure/recovery, same-Scene actual source packet preparation and provenance, same-Scene draw_views/gear borrow, equipped rebind and unchanged pose clock, slot-driven equip/unequip receipts, source socket identity, rollback and detach lease gate`.

The existing `inventory_source_tests.cpp` assertions cover exactly three distinct transmute roles, a missing value provider yielding a blank ValueBox, the disabled no-button-art branch, and the source row text matrices. The prior feature report records that strict source run as passing. The runtime composition test is not a pixel test, and no fresh capture of a representative long item name was made here. Keep B016 open until the amount-provider owner and long-label capture are verified on the current frozen main build.
