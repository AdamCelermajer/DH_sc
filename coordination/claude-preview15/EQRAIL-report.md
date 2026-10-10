# EQRAIL report: B046 Details slot rail (icon clicks and up/down arrows)

Status: implemented, unit-tested, verified in quiet EXE batches. One visual gap remains (dim normal art for 5 icons, see Open risks).
Commits on `p15/eqrail`: `c74365b0` (hit tests, arrows, selection), `cf1adb25` (Highlight rule).

## 1. Investigation (evidence)
Visual: reference video Part 1 t=372 (Feet Details, `.local-inputs/claude-preview15/eqrail/ref/g372full.png`, zoom `rail372zoom.png`).
Observed: the rail shows all 10 icons with up/down arrows; the selected icon (boots) is bright, the others are dark. The video cannot show an arrow press, so arrow behaviour comes from the SWF only (logic evidence).

Logic (`port/engine-ui/reference/character-menu-flow-v1/authored-actions.txt`, menu_InventorySheetDetails block ~0001c000-0001dd40):
- `SideList/btn_Type0..9` (array `SelectedItemType`, built ~0001dae0-0001dbae) = icon N = InvSlotId N (0 torso, 1 main hand, 2 off hand, 3 feet, 4 hands, 5 ring1, 6 ring2, 7 waist (band), 8 head, 9 potions; the category titles confirm 7 = Waist).
  `on_focus_in` (0001dc00-0001dc9a): `InvSlotId = ItemType`, then `DisplayEquipedItem`, `CheckIcons`, `GenerateInventoryListItems`.
- `CheckIcons` (0001dcaf): hides every `Highlight`, then shows `SelectedItemType[InvSlotId].Highlight`.
- `btn_left` (up arrow) -> `ClassChangeUp` (0001d9ea): if InvSlotId > 0 then -1, else 9. `btn_right` (down arrow) -> `ClassChangeDown` (0001da6b): if InvSlotId < 9 then +1, else 0. Both then refresh the Details (`GenerateInventoryListItems(0)`).
  So the arrows step ONE SLOT with wrap. They do not page and they do not move the list row.

Why our page ignored the clicks (before, Preview 14 rc1 EXE):
1. No hit regions for the rail icons. `DetailsPresenter::release` returned `none` for them and `MainPage::release` swallowed the click (the B045 guard for Details).
2. `DetailArt.actions` mapped btn_left/btn_right to `previous`/`next`, which stepped the LIST ROW (wrong target).
3. All rail `Highlight` batches were drawn unconditionally, so no selection state was shown.

Rail art boxes (authored 480x320, from `SideList/btn_TypeN/*` batches): T0 x6-24 y90.7-107.9, T1 y109.6-124.8, T2 y127.4-146.3, T3 y147.5-164.5, T4 y166.4-185.0, T5 y187.1-201.8, T6 y205.7-220.5, T7 y226.6-239.4, T8 y242.3-260.4, T9 y261.8-279.9. btn_left y63.8-87.8 (up), btn_right y282.6-305.9 (down).

Highlight art (atlas cells): normal-state icons use v about 0.04-0.08; Highlight (bright) cells use v about 0.50-0.54. Capture `rails-3-9.png` confirms: a non-selected icon with normal art renders dark (like the reference), the selected one bright.

## 2. Changes
- `features/inventory/inventory_details.hpp`: `DetailAction::slot`; `DetailRailBox`, `details_rail_box()`, `details_rail_slot_at()`. Comment: previous/next = arrow slot step.
- `features/inventory/inventory_details.cpp`:
  - `DetailsPresenter::release`: rail icon hit -> `open(N)` (same refresh as the main-sheet slot hit: equipped item or first candidate), returns `DetailAction::slot`.
  - previous/next: InvSlotId -1/+1 with wrap, via `open()`. Replaces the row stepping.
  - `frame()`: a rail Highlight batch is hidden unless its icon is selected, but only for icons that have normal art (`rail_normal_art`, `rail_highlight_hidden`). Icons 0,1,2,5,6 have only Highlight art, so they stay drawn.
- `features/equipment/equipment_main_page.cpp`: `DetailAction::slot` added to the selection-only case.
- Tests:
  - `features/inventory/details_row_geometry_tests.cpp`: every icon 0-9 (non-empty box, centre/corners map to its slot, no overlap, outside x=100 -> none), both arrows (centre maps to none, up above icon 0, down below icon 9, gap selects nothing).
  - `features/equipment/equipment_main_page_tests.cpp`: icon clicks (3, 9, 0), arrow wrap 0->9 and 9->0, one step each way, highlight follows the selection, equipment not mutated.
  - `features/equipment/runtime_equipment_binding_v1_tests.cpp`: the old "next = row +1" expectation is now "down arrow = slot 2, up arrow = slot 1", and row +1 is tested by a row click.

## 3. Tests run
- `p14_build.ps1 -Name eqrail -Test`: build exit 0. CTest 110/111 pass. The only failure is `session_skill_binding`, the known worktree junction issue.
- Individually: `equipment_main_page`, `inventory_details_row_geometry`, `equipment_inventory_actions`, `equipment_adapter`, `equipment_visual`, `combat_session_equipment_multiplicity` pass.
- `runtime_equipment_binding_v1_tests.cpp`: compiles (`clang++ -fsyntax-only`, same include list as its runner). NOT executed: its runner needs `source_character_owner_factory_native_build/native.a`, which does not exist in this worktree.

## 4. Visual verification (quiet batch, real EXE, `--menu-release` clicks on the Torso Details)
Captures: `.local-inputs/claude-preview15/eqrail/runs/*.png` (ppm -> png via ffmpeg). Strips: `titles.png` (after), `titles-before.png`, `rails-3-9.png`.
- before (rc1 EXE): icon 1, icon 9, up, down x3 -> title stays "Torso". Icon clicks and arrows do nothing.
- after (this build):
  - icon 1 -> "Right hand" (Useless Blade / Plain Blade / Flawed Moon list)
  - icon 3 -> "Feet"; icon 5 -> "Ring 1"; icon 7 -> "Waist"; icon 9 -> "Potions" (5 Potion)
  - up from Torso -> "Potions" (wrap 0 -> 9)
  - down x3 from Torso -> "Feet"
  - icon 9 then down -> "Torso" (wrap 9 -> 0)
  - list rows change with each slot; the selected icon is bright, non-selected icons with normal art are dark.

## Verifier script
`.local-inputs/claude-preview15/eqrail/gen.ps1 -Out jobs.json` writes 18 jobs (9 before, 9 after). Args files go in `.local-inputs/claude-preview15/eqrail/pkg-{before,after}/` (junctions to the asset dirs). Startup-config sets the working directory to the args file folder, so relative asset paths need that layout. Run: `quiet_run.ps1 -JobsFile jobs.json -Parallel 12 -Summary summary.json`. All 18 exit 0.
Expected: see section 4.

## Package files required
None new. The fix uses the existing Preview 14 asset set. The EXE must be built from this branch.

## Open risks / not verified
1. Icons 0 (torso), 1 (main hand), 2 (off hand), 5 (ring1), 6 (ring2) keep their bright Highlight art when not selected. The original shows them dark. Their normal-state art is missing from `original_inventory_art.cpp` (exporter gap). Fix: export the normal-state SideList batches for all 10 icons (art export owner). Not guessed or synthesised here.
2. The original's on_focus_in can also fire on hover/keyboard focus. The PC port uses a click (PC adaptation).
3. Clicking an already-selected icon re-runs `open()` (first candidate or equipped item). The original calls `GenerateInventoryListItems()` with no Index on that path. What it selects was not traced.
4. Hit boxes are the bounding boxes of the SideList batches (same approach as the B045 rows). Icons 0,1,2,5,6 have only Highlight batches for their box.
5. Runtime of `runtime_equipment_binding_v1_tests` not executed (see section 3). Real mouse/touch input not exercised; the quiet runner uses the same release path.
