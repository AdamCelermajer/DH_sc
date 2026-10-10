# P15 DROPGATE report (verifier rc2 item 3j: Drop missing on an unequipped bag sword)

Worktree: `C:/Users/adamc/Desktop/workspace/DH_wt/dropgate`, branch `p15/dropgate` (from `p15/integrate`).
Build: `build-dropgate`. Scratch and captures: `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/claude-preview15/dropgate/`.

## Verdict

The gate is correct. The verifier's FAIL for 3j was a harness artifact. The job did not contain a bag sword at all. The "Useless Blade" row it captured was the equipped Longsword01 itself, so hiding Drop was the correct behaviour. With a real unequipped bag copy in the bag, Drop is shown, and Drop and Transmute act on that copy. No gate code change was needed. The change in this branch is one CTest-gated unit test that locks the per-instance contract.

## Root cause of the verifier observation (evidence)

1. `main.cpp:797-800`: with `--fresh-player` and an existing `--save`, the save is loaded and then `state.equipment.clear(); state.inventory.clear();`. The direct bootstrap regenerates only the starter gear from the live actor. So the save's bag item (`bag-longsword03`, Longsword03) is discarded. The verifier's `DROP/bag-sword-rc2` and `REG/b042-eq-bag` jobs use `--fresh-player` plus `--save b016-knight-bag-longsword03.save`, so they never had a bag item.
2. Confirmed with a temporary debug print in `DetailsPresenter::frame` (reverted, not committed) on the verifier's job args: `selected=equipped/Longsword01 equipped_id=equipped/Longsword01 drop=0`, with rows `[equipped/Longsword01 equipped=1]` only. The equipment bindings were `slot-1 -> equipped/Longsword01`, so the flag was correct.
3. The same job with `--bag-item <DEF>` (the P14 diagnostic that adds unequipped rows to the direct bootstrap) gives the real bag rows. `--bag-item Longsword03` shows "Plain Blade" x1 (`equipped=0`), and `--bag-item Longsword01` shows a same-definition copy (`equipped=0`).

## Runtime evidence (current HEAD, no code change)

Image paths are under `.local-inputs/claude-preview15/dropgate/`.

| Job | Setup | Observed | Image |
|---|---|---|---|
| base-bag | verifier args replica (no bag item), baseline EXE | "Useless Blade" = equipped one, EQUIP label, no Drop, no Transmute, no Value | `base-bag/bag-sword.png` |
| bag03 | `--bag-item Longsword03`, no click (equipped selected) | equipped row selected, no Drop; bag rows "Plain Blade" x3 below | `bag03/bag-sword.png` |
| sel-bag | `--bag-item Longsword03`, click row 1 (`100:120:220`) | Plain Blade selected: Value 55, Transmute, DROP shown; equipped row no bottom panel. Debug: `drop=1` | `sel-bag/sel-bag.png` |
| samedef | `--bag-item Longsword01`, click row 1 (`100:120:220`) | bag copy of the equipped definition selected: Value 18, Transmute, DROP shown; `drop=1` for `bag-0-Longsword01`, `equipped=0` | `samedef/samedef.png` |
| act-drop | samedef + click Drop (`110:428:308`) | log: `Equipment Drop instance=bag-0-Longsword01 item=Longsword01 -> world item`; `World item draws frame=110 count=1`; equipment still `slot=1:equipped/Longsword01` | log only |
| act-tm | samedef + click Transmute (`110:272:308`) | log: `Equipment Transmute instance=bag-0-Longsword01 item=Longsword01 ... gold 0 -> 18 amount=18` (matches Value 18) | log only |

The runs are quiet (`port/windows-foundation/tools/quiet_run.ps1`, `-Parallel 1` or `2`). Exit codes are 0 on all of them.

## Original-behaviour evidence (logic, IDA)

- `pseudocode-all.c` near line 235725 (`NativeInvGetItemsListForSlot`): each list row gets two flags.
  - `ItemEquipped = !valuable && item == ItemInventory::GetEquippedItem(player, listed slot)`.
  - `ItemEquippedOtherHand` = the item is equipped in the paired slot (other hand, or other ring finger). The code picks the paired slot from `LeftHand`/`RightHand` or `LeftHandRingFinger`/`RightHandRingFinger`, and sets it to 0 for other slots.
- `authored-actions.txt` `displaySelectedItemInfos` (0001cbbe-0001cc44): if `ItemEquipped` or `ItemEquippedOtherHand` is true, Transmute is set to the disabled state and `btn_Drop._visible=false`. Only when both are false does Transmute get the idle state and Drop stays visible.
- Consequence: Drop is hidden when the selected item is equipped in this slot or in its paired slot. The current gate `rows[current].equipped` (instance-level equipped state, any slot) gives the same result whenever an equipped item is listed in its own slot or its paired slot. That is an inference from the flag definitions and the slot pairing, not checked against every list. The earlier open risk "global `equipped` vs per-slot list" is narrowed to that inference (see open risk 4).

## Drop and Transmute act on the right instance

- `runtime_equipment_page_v1.cpp:118-132`: the pending command takes `equipment->selected_instance()`, the same instance the Details display uses (`focus()`).
- `main.cpp:2715-2733`: Drop and Transmute look up that instance id. `drop_inventory_item` / `transmute_inventory_item` (`equipment_inventory_actions_v1.cpp`) refuse equipped items. The runtime log above confirms that the bag copy is the one dropped or transmuted.

## Changes (this branch)

1. `port/windows-foundation/features/inventory/inventory_drop_gate_tests.cpp` (new). It drives the real `DetailsPresenter::frame` with the staged assets (same pattern as `equipment_main_page_tests`):
   - Case 1: the same definition equipped in slot 1 plus a bag copy. Selecting the bag copy gives a Drop batch. Selecting the equipped one gives no Drop batch and no Drop text.
   - Case 2: rings. Ring01 is equipped in slot 5 and Ring02 is a bag ring. Viewing slot 6 (both listed), the bag ring keeps Drop and the ring equipped in the paired slot does not.
2. `port/windows-foundation/CMakeLists.txt`: target `inventory_drop_gate_tests`, CTest `inventory_drop_gate` with the `windows-shared-assets` argument (same as `equipment_main_page`).

No change to `inventory_details.cpp`, `equipment_menu.cpp` or the drop/transmute actions. A temporary debug print was added and reverted.

## Tests

- Mutation check: `drop_available=true` (gate forced on). The new test fails with `Equipped selection still shows Drop (original hides it on the equipped path)`. The gate is restored afterwards.
- `p14_build.ps1 -Name dropgate -Test`: 114 of 115 pass. The one failure is `session_skill_binding`, which fails in worktrees because of the `.local-inputs` junction (known, ignored per the brief).
- Related targets pass: `inventory_drop_gate`, `inventory_details_row_geometry`, `equipment_main_page`, `equipment_inventory_actions`.

## Verifier script (corrected 3j)

The DROP/REG jobs need `--bag-item Longsword01` (or another DEF), inserted before the first `--equipped-item`, and the save copy is then only a profile. Use the same args as `verify-rc2/DROP/bag-sword-rc2/run.args` otherwise, with `--frames 130` and these clicks:

- A. Same-definition bag copy selected: `--menu-release 80:112:132` then `--menu-release 100:120:220`. Expect in the PNG: bottom-right panel with `Value 18`, `Transmute`, `DROP`; top panel "Useless Blade" UNEQUIP.
- B. Equipped selected (no second click): expect no Drop, no Transmute, no Value.
- C. Drop: A plus `--menu-release 110:428:308`. Expect the log line `Equipment Drop instance=bag-0-Longsword01 item=Longsword01 -> world item`.
- D. Transmute: A plus `--menu-release 110:272:308`. Expect `Equipment Transmute instance=bag-0-Longsword01 ... gold 0 -> 18`.

Do not use `--fresh-player` with a save that has bag items. Verifier jobs that need a bag item should use `--bag-item`.

## Package files required

None. The change is tests and CMake only.

## Open risks

1. Harness trap: `--fresh-player` plus `--save` drops the save's inventory (`main.cpp:797-800`). This is intentional for the direct bootstrap. Verifiers must use `--bag-item`. Not changed here.
2. The real frontend-loaded save path (main menu, or in-game load with the save's bag) was not run. The EXE's `--load-frame` path needs a live `gameplay.save`, and it is not set up for this save. The unit test uses a real `CharacterState` and the presenter, so the gate logic is covered, but a frontend load of a bag save is not verified end to end.
3. `DetailsPresenter::frame` uses `focus()` for display and gate (falls back to row 0 when the selected instance is not in the current slot list), while the Drop and Transmute commands use the selected instance. The actions still refuse equipped items, so the failure mode is a refused action, not a wrong drop. Not changed.
4. Equipped-in-paired-slot rule: derived from the IDA flags (`ItemEquipped` / `ItemEquippedOtherHand`) and the authored branch. The ring case in the new test assumes Ring02 is applicable to slot 6 (it passes, so it is). Equipped items that are listed in a non-paired slot did not occur in any list here.
5. The `--bag-item` diagnostic rows are named `bag-<n>-<DEF>` and the bootstrap puts them after the equipped items. Ordering in the list is therefore not the save order.
