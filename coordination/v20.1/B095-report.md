# B095 — bag tab returns to global Equipment

**READY WITH CAVEATS for independent review.** Implementation commit `220bd9e9` on `fix/b095`; source and executable checks below passed. Root still owns combined integration, release verification, packaging and tracker closure.

## Investigation and expected behavior

The original is a two-page flow under a persistent CharacterMenu tab row: a category slot replaces global Inventory with Details, and activating the bag discards the subpage and enters global Inventory even when the bag is already selected. Back has its own HUD/points-confirmation route and is not changed here.

**Visual evidence, inspected before implementation:** original v1.0.3 Part 1 at `.local-inputs/reference-video/dh2-act1/Dungeon Hunter 2 (v1.0.3) Part 1 [720p] [z_Zky7qQdYs].mp4`, sequence 350–381 s at 1 fps, preserved as `.local-inputs/claude-v20.1/fix095/video350-381.png`. Global Inventory is visible at 366–368 s; Feet Details replaces its body at 369–374 s while the tab row stays visible. A different item is selected and EQUIP updates. This sequence does **not** directly show an active-bag return click; that exact rule is established from the authored logic, not asserted from a still or inferred from the title. Historical `sheet1.png` is contextual but unlabelled.

**Logic evidence:** recovered `port/engine-ui/reference/character-menu-flow-v1/authored-actions.txt`:
- Inventory tab onRelease, offsets `0xe94c–0xe9de` (lines 3060 onward), always sets LastSelectedTab, calls switchTab, `NativePopAllAbove(parent name)`, `NativePushMenu("menu_InventorySheetMain")`, then MenuTab sound. The caller has no active-tab exclusion. `switchTab`, `0xe1a0–0xe244`, skips redundant art changes, then returns to that unconditional caller. Other character tabs use the same pop/push pattern.
- Torso category `0x19fae–0x1a040`: sets InvSlotId=0, pops the main page, pushes `menu_InventorySheetDetails` and sets its category caption. Head `0x1a27d–0x1a30b` does the same with InvSlotId=8. These navigation callers do not require an owned item; empty Head is a valid navigation case.
- Back `0xe642–0xe835`: points/skill confirmation branches or `NativePopAllMenus` + `NativeBackToHud`. It is distinct from bag reselection.

IDA `pseudocode-all.c`: `NativePopAllAbove` **0x43ac28** requires one string argument and invokes `MultiMenuManager::PopMenu(name,true)` **0x439270**. Its above-menu loop stops at the named parent and invokes hide callbacks/changes state for the removed pages. `NativePushMenu` **0x43b1b4** calls `MenuManager::PushMenu(name)` **0x431924**, resolving an existing source menu before pushing. `NativePopMenu` **0x43b158** supplies the category caller's pop behavior. These are menu operations, not inventory/equip mutations.

Minimal reusable implementation: a retained page provider gets an optional, nonthrowing/idempotent `leave` callback. After the existing readiness and open-menu gates accept a tab selection, `SourceCompositionV1::select` calls the departing provider's callback, including when that provider is selected again. Equipment binds the callback to its existing `leave_page()` / `DetailsPresenter::close()`. No authored hit region or coordinate transform is altered. Screen-to-stage remains independent width/480 and height/320, the existing menu draw transform. Mouse input is the established PC transport; no new input adaptation is introduced.

## Historical diagnosis corrected

The earlier B095 worker added a root `leave_page` hunk in `28378efe`, but its fixture retained `--campaign-triggers` and started Swamp_Intro. The later real bag down/up messages were consumed by the existing cinematic admission gate. The logged `action=0` belonged to the initial scheduled slot/body click at frame300, **not** to the later bag click. Thus its proposed active-tab hit-test defect was unsupported and is retracted.

On the correctly isolated no-cinematic fixture, the authored bag contour includes stage245,18; real bag clicks return Action::equipment (2). The prior root hunk works in that fixture, but it used a raw hit result and could clear Details even after a rejected provider selection. The final provider lifecycle replaces that hunk and checks accepted selection first. Temporary committed `DH_CLICK_DEBUG` / `B095dbg` lines are removed.

Shared input prerequisites: equivalent B103 WM_MOUSEMOVE/client-position and injector changes were already in this branch (`8d85adea`); reviewed B102 latch was cherry-picked as `b645c2dc` from `8b5bfcee`. Root has those changes already: keep each once on merge.

## Focused test plan (recorded before behavior changes)

Use fresh absolute profile/world saves and a private silent desktop with no active cinematic. Verify global -> Torso -> item row/Details -> active bag -> global; repeat bag, reopen category, empty Head -> bag, and Stats -> bag. Use real 10 ms Win32 down/up clicks at 960x640 and 1280x720. Verify original art, unchanged inventory/equipment, failure/readiness, body misses and duplicate resets. Build and run focused and full CTest; identify a frozen tested executable. Exact active-bag video timing remains unverified because the inspected original sequence does not contain that click.

## Implementation

- `SourcePageProviderV1::leave` / `SourceCompositionV1::select`: reusable accepted-selection subpage retirement; unready, unbound, closed or invalid destinations do not call it.
- `RuntimeEquipmentPageV1::source_page_provider`: retains the same Equipment owner and binds its existing close operation.
- Main no longer performs a second raw tab hit test or the root-specific B095 cleanup; no duplicated reset or debug output.
- `source_composition_tests.cpp`: active tab dispatch, duplicates, unready/unbound refusal and closed menu.
- `equipment_main_page_tests.cpp`: actual source slot and Details state through the composition callback; rejected active tab and body miss preserve Details; populated/empty categories reset; repeated clicks preserve inventory/equipment at both aspect ratios.

No package assets required.

## Verification and frozen executable

Scratch evidence is under `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/claude-v20.1/fix095/` (same junction visible in fix095). Scripts: `navigation-run.ps1`, `make-sequences.py`. All Python commands used `py -3 -I`; Win32 messages were injected by repository `quiet_click.ps1` on private desktops with `DH_AUDIO_SILENT=1`. Absolute separate `*.profile.json` / `*.world.save` paths were supplied. Accepted package assets were read; accepted files/user saves/user windows were not modified.

Frozen production-path test EXE: **`frozen-220bd9e9.exe`**, 19,487,744 bytes, SHA256 **`42F072452B9ABAEEE027F4A3ADC11DE4F53C8059663FF62E9B20EADA824C0B00`**. This is the full normal executable built at implementation commit220bd9e9, using the v20 staged original art/assets. Root's merged EXE is not this frozen build and needs integration verification.

Controlled failure build: `noreset-regression.exe`, SHA256 **`6402CB22D5173D89B101CC59D29502856756ECD36F162D2687DCD6DAF228B9B4`**. Same source/input prerequisites, with only the Equipment `provider.leave` binding disabled. It was built separately, frozen, and the source line was immediately restored. No regression toggle remains in committed source. `noreset960` real-click run exits0; frames570/680 stay Torso after bag clicks9/11 s, and frame1300 returns to stale Torso after Stats -> bag. This reproduces the reported failure on reliable input independently of the invalid cinematic fixture.

Fixed real-click runs: `fixed960.log` and `fixed1280.log`, both exit0, no timeout. Clicks at5/7/9/11/13/15/17/19/21/23 s with10 ms hold: Torso, item row, bag, duplicate bag, empty Head, bag, Torso reopen, Stats, bag, duplicate bag. Original contour points scale with each axis; no per-tab hit hack. Inspect `fixed960-sequence.png` and `fixed1280-sequence.png` alongside full-size `*-FRAME.ppm` captures. Ordered frames170/360/570/680/800/920/1040/1160/1300/1460 show:

| Frame | Visible result at both aspect ratios |
|---|---|
|170|global Inventory, all source slot rows|
|360|Torso Details, Ceremonial Garb selected|
|570|global Inventory after active bag|
|680|global Inventory after duplicate bag|
|800|empty Head Details|
|920|global Inventory after empty category bag|
|1040|Torso Details reopened|
|1160|Stats after leaving Equipment|
|1300|global Inventory after returning from Stats|
|1460|global Inventory after another duplicate bag|

Both logs report only the initial Equipment state (StartingSuit/Boots/Gloves/Longsword); there is no equip/unequip/auto-equip action. Actual source art and avatar remain visible, with the same equipped rows at the end. The unit test additionally checks inventory instance/quantity and empty equipment remain unchanged through the navigation operations.

Build: `p14_build.ps1 -Name fix095 -Jobs 2`, exit0. Focused CTest `character_menu`, `semantic_input`, `equipment_main_page`:3/3 pass. Header-only `source_composition_tests.cpp` compiled directly with the same foundation/static libraries and executed as `source_composition_b095.exe`: pass. Full CTest: **167/168 pass**, only known worktree `session_skill_binding` fails with `Asset path escapes asset root`; see `ctest-final.txt` (16.26 s). A final restored-source rebuild also passed; its freshly relinked build-dir EXE hash is `6F9E105F67D7A8073D31C03C139C1A2FDE6211C4F8D81334E19F6738C1E3414E`. Visual checks refer to the preserved frozen42F07245 build, not that relink.

## Match statements and remaining gaps

- **Matches video:** global Inventory/category Details layout, persistent tab row and selected-item presentation in the inspected sequence. **Not verified against video:** exact active-bag transition/click timing; it is absent from that sequence.
- **Matches IDA/recovered authored logic:** accepted active-bag pop-above-parent then global-page entry; other accepted tabs discard Equipment subpages; empty category callers are valid; navigation is separate from equip mutations. The portable provider callback preserves the visible result without recreating Flash's internal object stack.
- **Not verified:** audible MenuTab sound or original animation duration (silent runs), retail/version differences, physical mouse on the user's desktop, root's merged/package build. No sound change was made.

## Handoff

Branch `fix/b095`; merge the final branch, preserving root's already-reviewed input prerequisites once. Read-only reviewer should inspect report, provider callback diff, readiness/failure tests, controlled no-reset failure and both frozen real-click sequences. Root owns combined build/visual checks and removal from the active bug table only after those pass. Shared trackers were read but not edited by this worker.
