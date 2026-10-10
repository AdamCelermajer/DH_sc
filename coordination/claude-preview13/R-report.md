# Preview 13 wave 4 report R: DROP on the eq-bag scenario (B042 regression check)

Status: NO CODE CHANGE NEEDED. The rc2 gate already hides DROP only when the selected row is the equipped one. The
eq-bag run that the verifier flagged as "unequipped" is in fact an EQUIPPED selection (Useless Blade, Longsword01), so
rc2 hides DROP correctly under the reference rule. The verifier's FAIL 9b rests on a wrong premise. Nothing committed.
Scratch: `.local-inputs/claude-preview13/r/`.

## 1. Why rows[current].equipped is true in eq-bag (task 1)

Save `b016-knight-bag-longsword03.save` (read from the file):
- Inventory: `starter-item-3` Longsword01 (shown as "Useless Blade"), `bag-longsword03` Longsword03 (shown as "Plain Blade"),
  StartingSuit, StartingBoots, StartingGloves, Potion0.
- Equipment bindings: `slot-1` -> starter-item-3 (right hand). Longsword03 is in the bag and NOT equipped.

Mechanism (code read in `equipment/equipment_menu.cpp` and `features/inventory/inventory_details.cpp`):
1. `Presenter::select_slot(slot)` sets `selected_` to the instance bound to that slot. Right hand (slot 1) therefore selects
   the equipped Longsword01 automatically.
2. `DetailsPresenter::frame` computes `current = focus(rows, selected)`. The selection is in `rows`, so `current` is the
   real selection, not the fallback 0. `rows[current]` is Longsword01, so `equipped == true` and DROP is hidden.
3. The eq-bag job's only click (`--menu-release 80:112:132`) opens the Right hand Details (equipment page slot hit). It
   does not select a bag row. A Details probe at (112,132) and (112,84) returned action none.

Probe (private, `r/probe_tests.cpp`, rc2 assets, save inventory and bindings):
```
PROBE ok=1 selected=sword rows=2
PROBE row inst=sword def=Longsword01 equipped=1 applicable=1 slotting=1 type=0
PROBE row inst=sword2 def=Longsword03 equipped=0 applicable=1 slotting=1 type=0
PROBE drop_batches=0
LISTTEXT menu_InventorySheetDetails/list/btn_0/Host = Useless Blade
LISTTEXT menu_InventorySheetDetails/list/btn_post0/Host = Plain Blade
```
(ids were renamed to `sword`/`sword2` only so the test's lease provider accepts them; names and slots are the save's.)

Why the verifier thought "unequipped": EQUIP is shown in both states. The reference at t=336 also shows EQUIP for an
equipped selection (Q-report), so EQUIP does not discriminate. The highlighted row in the eq-bag capture reads
"1 Useless Blade", which is the equipped item, and the top block shows UNEQUIP for it.

So: for the eq-bag job, `current` is the equipped row and hiding DROP matches the rule (selected equipped -> no DROP;
reference Torso t=336 and Hands t=342). The P12 build shows DROP only because it has no gate (P12 is the baseline, not
the expected result).

## 2. Reproduction (task 1, quiet batch, rc2 EXE)

Job file `r/jobs-r.json`, run with `port/windows-foundation/tools/quiet_run.ps1 -JobsFile ... -Parallel 4 -Summary r/summary-r.json`.
All four exit 0 (about 6.5 s each). PNGs: `r/runs/<job>/`. Montage: `r/montage/r-montage.png`.

| Job | Args (copied from verify-final, capture path changed) | Observed | Expected by rule |
|---|---|---|---|
| eq-bag-R2 | `r/runs/eq-bag-R2/run.args` (= verify eq-bag-R, save b016) | Right hand, "Useless Blade" (equipped) selected, EQUIP shown, **no DROP** | no DROP (selected is equipped) |
| eq-torso-R2 | `r/runs/eq-torso-R2/run.args` (= verify eq-torso-R) | Torso, Ceremonial Garb equipped, **no DROP** | no DROP (Torso t=336) |
| eq-feet-R | `r/runs/eq-feet-R/run.args` (eq-bag args, `--menu-release 80:380:235` = Feet slot centroid) | Feet, Ceremonial Boots equipped, **no DROP** | no DROP (equipped) |
| eq-bag-sel-R | eq-bag args + `--menu-release 84:130:232` | Click hit the equipment rail: slot changed to **Ring 1** (empty list). DROP shown (no rows) | not a valid bag-selection test |

Notes:
- `eq-feet` did not exist in verify-final (only eq-torso, eq-weap, eq-bag). The eq-feet-R job above is new.
- An unequipped feet or bag selection could not be reproduced with the scripted clicks. The Plain Blade row (list
  `btn_post0`) is in the text list, but the click coordinates I derived from its hit geometry land on the rail. A
  next-arrow click (`84:10:290`, DetailAction::next centroid) did not change the selection (`eq-bag-next-R`).
  The bag-selected case is therefore covered by the unit test, not by a capture.

## 3. Changes

- `port/windows-foundation/features/inventory/inventory_source_tests.cpp` (test only, after the "Equipped selection still
  shows the Drop button" check, before `owner.equipment.clear()`):
  1. bag item (`sword2`) selected beside the equipped `sword`: Drop present.
  2. valuables category (slot 9, no rows, asserted empty): Drop present (previous behaviour kept).
  3. restore `select_slot(1)` so the later arrow test still starts from `sword`.
- `inventory_details.cpp`: NO change. The existing gate `drop_available = rows.empty() || !rows[current].equipped` already
  implements "hide only when the selected row is the equipped one". Changing it would not fix eq-bag, which is correct.
- Nothing else touched. No shared build folder, no tracker, no commit.

## 4. Tests (commands and real output)

Builds: `bash .local-inputs/claude-preview13/r/build_r.sh` -> `BUILD_INV_OK`, `BUILD_EQ_OK`, `BUILD_PANE_OK` (private folder
`r/build_final`, `-Wall -Wextra -Werror`, shared archives read-only). Toolchain bin on PATH for the runtime DLLs.

- `inventory_source_tests.exe <rc2 assets>` -> exit 0,
  `original inventory source tests PASS name=Useless Blade stats=Damage 12 - 15 potions=Potions: 3`
- `equipment_main_page_tests.exe <rc2 assets>` -> exit 0,
  `equipment_main_page_tests PASS: shared source slot/instance, Details, native Gear routing and compatibility equip/unequip`
- `pane_tests.exe` -> exit 0, `inventory character pane API tests PASS`
- Negative check: `r/neg/` copy of `inventory_details.cpp` with the gate changed to `rows.empty()||equipped_id.empty()`
  (hides Drop whenever the slot has any equipped item). Result: exit 1,
  `Unequipped bag item beside an equipped sibling lost its Drop button`. The new assertion catches that wrong gate.
- Existing unequipped (no equipment) and equipped-selected assertions still pass (they run before the new block).

## 5. Verifier re-check (rc2 EXE; no rebuild needed for this item)

Run `quiet_run.ps1 -JobsFile coordination...` with `.local-inputs/claude-preview13/r/jobs-r.json` (its four entries are the
jobs above; the `eq-bag-sel-R` and `eq-bag-next-R` entries are NOT valid checks and should be dropped from the list, the
`jobs-r2.json` file holds the next-arrow job). Expected: eq-bag-R2 no DROP with "Useless Blade" selected; eq-torso-R2 no
DROP; eq-feet-R no DROP. The DROP-shown case needs an unequipped selection, which only the unit test covers.

## 6. Uncertainties / open risks (not verified)

- The verifier's "eq-bag should show DROP" expectation was based on EQUIP visibility. It is wrong. Please update the
  verifier's expectation for eq-bag to "selected = equipped Useless Blade, no DROP".
- `equipped` in `equipment_menu::Presenter::view` is global (true if bound to ANY slot), while the Details list is per
  slot. An item equipped in another slot but listed for this slot would hide DROP. Not seen in the save used; not changed
  (the top block and UNEQUIP depend on the same flag).
- The Plain Blade row is in `frame.text` (`list/btn_post0/Host`) but not visible in the rc2 capture. Separate rendering
  gap in the list (the reference shows the second row). Not investigated in this wave.
- The row-click coordinates for the list are unclear: a Details click at the row's hit centroid hit the equipment rail.
  The release-space mapping for list rows was not checked.
- Unequipped feet, rings and potions were not captured (no matching save). Covered by the unit test only.
- B042 stays open for the items in the Q-report (grey/orange panels, damask extent, icons, sword). This report does not
  close any of them.
