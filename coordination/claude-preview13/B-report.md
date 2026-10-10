# Preview 13 Group B report: Equipment page visuals (B042, B016, B019)

Status: PARTIAL. One composition fix is implemented and its focused tests pass. It is NOT visually verified in a
normal-build window (the integrated EXE is the root's job), so B042 must stay OPEN until the root captures it.
B016 and B019 were observed but not changed by this group.

## 1. Evidence

### Candidate reproduction (Preview 12 candidate, dh-foundation.exe copy)
Run folder: `.local-inputs/claude-preview13/group-b/run/` (exe copy plus junctions to the package `assets`,
`audio-assets`, `ui-assets`). During this session packaging renamed `windows-source-clock-v19-preview-12-candidate/`
to `windows-source-clock-v19-preview-12/` (same exe, timestamp 13:51). My junctions were repointed to the new folder.
- Equipment tab, normal window (`run/equip-normal.args`, `--equipment-page-frame 60`): `run/equip-normal.png` (1201x720).
- Equipment tab, 480x320 window (`run/equip-480.args`, `--window-size 480,320`): `run/equip-480.png`.
- Torso details open (`run/detail-normal.args`, `--menu-release 80:112:84`): `run/detail-normal.png`. This is the BEFORE image.
- Skills tab, same frame (`run/skills-normal.args`): `run/skills-normal.png`.

### Reference (Part 2 video `video/part2_cIAW38IfLxY.mp4`, 640x360, 916 s)
- Scanned the whole video with 8 s contact montages (`ref/mont-01.png`, `ref/mont-02.png`) and a 1 fps montage
  t=376-415 (`ref/win-376.png`). Stats at t~391-393 (`ref/f-392.png`); Skills at t~394-396 (`ref/f-395.png`).
- Observed directly: the slot pages at t~399-405 are the InventorySheetMain screen in MERCHANT mode. It has a header
  with BUY and SELL, a back arrow, Total Gold, a slot title ("Torso", "Ring 1"), a category rail on the left, a grey
  top-right item panel with UNEQUIP, an orange-brown lower-right panel with EQUIP/Value/SELL, and the avatar standing
  over the right side. Frames: `ref/f-400.png` (Torso, Soldier Armor), `ref/f-403.png` (Ring 1).
- Not found: a plain 9-slot Equipment grid frame (the candidate's no-details state) in the Part 2 montages. The
  reference equipment-type frames are the Details state of the same sheet, so only the Details state is compared.
- Side-by-side, candidate Torso details vs reference Torso: `cmp/side-torso.png`.

### Logic (SWF and authored evidence; IDA not consulted for this layer)
- `port/engine-ui/reference/character-menu-flow-v1/authored-actions.txt` pushes `menu_InventorySheetDetails` with
  `NativePushMenu` (around lines 6783-6907). So Details is a separate pushed menu over the still-open
  `menu_InventorySheetMain`; the sheet is not replaced. `features/inventory/README.md` says the same ("overlays the
  main sheet ... no source hide rule exists").
- Read-only probe of the exported InvMain sprite 439, frame 0 display list (`dqcharmenu_droid.swf`): avatarpane (218),
  sprite 388 at depth 3 (bar shape 387, bottom left), player_gold (390), AUTOEQUIP_ALL (248), btn_Swap (395),
  shape 396 at depth 34 (full-stage plate, bbox about x -5..488, y 43..323 in stage px), inv_anim (435), sprite 185 at
  depth 35 (small bar, bottom left), shape 436 at depth 177 (full-stage plate, bbox about x -36..520, y 57..353), Title.
- `features/inventory/inventory_details.cpp` `DetailsPresenter::frame` erased EVERY batch, text and solid whose role
  starts with `menu_InventorySheetMain/` before appending the Details art. That removed both full-stage plates (34 and
  177) and the bottom bars. The original keeps the plates under the Details menu.

### Observed difference (candidate Details state vs reference)
- Candidate Details (`run/detail-normal.png`): the left list, detail and avatar area sit on one plain brown fabric
  and the dark plates are missing. Mean colours of the 1201x720 captures: top-right region no-detail (52,46,33) vs
  detail (72,52,20); bottom-right no-detail (47,41,32) vs detail (44,29,12). In the reference the top-right equipped
  panel is grey and the bottom-right candidate panel is orange-brown (`cmp/side-torso.png`). The panel tints still
  differ from the reference and are NOT explained by this change (see Uncertainties).
- Equipment no-details state (`run/equip-normal.png`): the 9 slots, brass avatar frame and fabric look close to the
  Preview 12 art. No reference frame of that exact state was found, so it is not claimed as matching.

## 2. Expected behaviour and minimal implementation
Expected: when a slot's Details panel is open over the Equipment sheet, the main sheet's full-stage background plates
stay visible under the Details art. The Details art replaces only what the Details menu itself draws (its slot list,
avatar pane, item text, buttons, category rail). The main slot grid, main avatar pane, Title, gold and main action
buttons stay replaced as before, so the 3D avatar is not drawn twice.

Implementation (`features/inventory/inventory_details.cpp`): helper `details_replaces_main(path)` returns false only for
`menu_InventorySheetMain/34` and `menu_InventorySheetMain/177`, and true for every other `menu_InventorySheetMain/` role.
It is used in the three erase calls (art batches, text, solids) in place of the old prefix test. The depth 3/35 bars
are still erased, because the main 3D pane's insertion point (`before_role` `menu_InventorySheetMain/3/`) must not draw
a second avatar under Details.

## 3. Changes (my feature files only; no shared file touched)
1. `port/windows-foundation/features/inventory/inventory_details.cpp`
   - Anchor: after `bool prefix(...)` in the anonymous namespace: added `details_replaces_main`.
   - Anchor: `DetailsPresenter::frame`, the three `erase(std::remove_if(...))` lines for `next.art.batches`,
     `next.text`, `next.solids`: `prefix(...,"menu_InventorySheetMain/")` replaced with `details_replaces_main(...)`.
2. `port/windows-foundation/features/inventory/inventory_source_tests.cpp`
   - Anchor: the `none_of ... "Original details did not clear stale main/details bitm..."` check: plates 34 and 177
     exempted from the main-sheet clear (the stale fixture `menu_InventorySheetMain/stale` is still removed).
   - Anchor: after the `stale source fields` check: new check that plates 34 and 177 survive Details.
- No generated file changed (the exporter was only run in read-only `--inspect` mode, plus a private probe script in /tmp).
- `main.cpp` and `CMakeLists.txt` untouched.

## 4. Tests (commands and real output)
Build folder: `.local-inputs/claude-preview13/group-b/build/`. Linked against the existing archives in
`.local-inputs/windows-foundation-build/*.a` (read only; no ninja or cmake run there). Compiler:
`.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe`, `-std=c++17 -Wall -Wextra -Werror`.
- `clang++ -fsyntax-only ... inventory_details.cpp` -> `SYNTAX OK`.
- `inventory_source_tests.exe <run/assets>` -> `original inventory source tests PASS name=Useless Blade stats=Damage 12 - 15 potions=Potions: 3` (exit 0; `build/src-test.log`).
- `python -I inventory_character_pane_source_tests.py` -> `original inventory character pane source tests PASS` (exit 0).
- `pane_tests.exe` (`inventory_character_pane_tests.cpp` + `original_inventory_art.cpp`) -> `inventory character pane API tests PASS` (exit 0).
- `equipment_main_page_tests.exe <run/assets>` (feature sources plus archives) -> `equipment_main_page_tests PASS: shared source slot/instance, Details, native Gear routing and compatibility equip/unequip` (exit 0).
- Not run: `features/equipment/run_source_equipment_renderer_smoke.ps1` (writes into `windows-foundation-build`, which is forbidden to me). `features/inventory/run_inventory_character_pane_tests.ps1` writes into the shared `.local-inputs/inventory-character-pane-tests`, so I compiled the same sources privately instead. `inventory_menu_tests` and `inventory_feature_tests` were not built (they do not call DetailsPresenter::frame). The `run_runtime_equipment_*` runners are not touched by this change and were not run.
- Edge cases covered by the existing test: stale `menu_InventorySheetMain/stale` batch and stale details text/solids are still removed; equipped-only marker removal, transmute variants and solid order are asserted by the same file.
- No rendered-pixel test was added: none of these binaries renders the menu frame offscreen. The new check is a geometry check on the composed Frame.

## 5. Package files required
- No new asset. The plates (shapes 396, 436) and bars come from the generated `character_menu/original_art.cpp` (art1) and use the existing hud atlas.
- Source movie: `port/android-native/app/src/main/assets/original-cache/data/menus/dqcharmenu_droid.swf`, SHA-256 `43227075f407626b52eca4c345ac6f568023fcb39c269588201026c0fc6760e0` (matches `character_menu/source_layout.json`).
- Present in the Preview 12 candidate package: `windows-source-clock-v19-preview-12/assets` (the source pydata tests read it) and `ui-assets` (contains `source_bitmap_*.tga`). I did not hash the atlas files.

## 6. Uncertainties / not verified
- B042 is NOT closed. The fixed build was not captured. The root must check the Details state with the next integrated EXE.
- The fill tints differ from the reference: candidate top-right brown (72,52,20) vs reference grey; bottom-right dark brown vs orange-brown. I did not find which layer carries the tint. Candidates: Details art `menu_InventorySheetDetails/8`, `186/1`, `237/1` (hud UVs), and the solids' RGBA. An IDA/SWF layer dump is needed.
- The reference Part 2 equipment-type frames are Merchant mode (BUY/SELL header, SELL button). The candidate has no merchant state, so its header and buttons will differ. Only the Details panel is comparable.
- The 9-slot no-details state could not be checked against the reference (no such frame in the video montages).
- B016 (text spacing, duplicate Transmute/Value labels): not changed here. In the candidate Details capture I saw no Transmute or Value label. The tracker says the Value provider and triple-Transmute fix are terminal; the normal-build long-name/value capture is still open.
- B019 (avatar reflects selection): the candidate shows the Knight with the Longsword in hand in both states. Pose and gear were not compared with the reference (no matching reference avatar frame).

## Verifier script (integrated EXE)
Use a copy of the integrated package with fresh saves; never the live saves. Each args file is `swamp.args` plus the lines below.
- Equipment grid: `dh-foundation.exe --startup-config equip-normal.args`, where the added lines are `--equipment-page-frame 60 --frames 75 --capture equip-normal.ppm`. Expected: 9 slots and brass avatar frame; log line `Character menu Equipment selected frame=60`.
- Details: `--equipment-page-frame 60 --menu-release 80:112:84 --frames 95 --capture detail-normal.ppm`. Expected: Torso details open; the grey top-right and orange lower-right panels and the dark plates visible; compare with `ref/f-400.png` (Part 2, t=400 s).
- 480x320 window: add `--window-size 480,320` (the value is `W,H`, not a single number).
- Path note: the candidate folder was renamed mid-session; check the package path before using junctions.
