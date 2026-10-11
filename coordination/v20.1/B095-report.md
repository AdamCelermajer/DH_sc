# B095 report (HANDOFF, unfinished)

Branch fix/b095 (from v20.1/integrate). Not verified fixed.

## Evidence
- IDA/SWF actions: port/engine-ui/reference/character-menu-flow-v1/authored-actions.txt 0000e94c-0000ea74: btnInventoryTab.onRelease sets LastSelectedTab, calls switchTab, then NativePopAllAbove(parent name) and NativePushMenu("menu_InventorySheetMain"): clicking the bag tab (even when active) pops the per-slot Details page and pushes the global Equipment sheet. All tabs share this pattern.
- Video P1 ~355-380 s (sheet in .local-inputs/claude-v20.1/fix095/sheet1.png): global Equipment page, then Details (Torso/Feet) pages; return via tab/back.
- Our code: Presenter::release/SourceCompositionV1::release re-select the tab; MainPage keeps details_ open (equipment_main_page.cpp, leave_page() only closes it on session retire).

## Done (commits)
- 28378efe: main.cpp hunk after characterMenuComposition->release(...): if the hit action is any tab, runtimeEquipmentPage->leave_page() (closes Details).
- cherry-pick of B103 017e9f79 (WM_MOUSEMOVE client position in platform_win32.cpp + tools/quiet_click.ps1).
- UNCOMMITTED/TEMP: DH_CLICK_DEBUG std::cout debug lines in main.cpp (2 lines, "B095dbg"); remove before final.

## Real-EXE finding (observed)
Fixture: scratch .local-inputs/claude-v20.1/fix095/run.ps1 (swamp.args minus --fresh-player, --window-size 960,640, --equipment-page-frame 60, --menu-release 300:112:84 opens Torso Details, quiet_click.ps1 injects real Win32 clicks; capture-at ppm; sheet.sh makes contact sheets). Build: p14_build.ps1 -Name fix095 -Jobs 2.
- Injected clicks reach the game (B095dbg down/held frames logged, focused=1).
- Click on bag tab (490,36 px at 960x640) while Details is open: characterMenu.hit_test returned Action::none (log "B095dbg click action=0"), so my leave_page hunk never fires and Details stays. => Root cause is likely hit_test: the ACTIVE tab (or tab zones while a Details page is shown) returns none (original_menu_hit_zones / nearestTab logic in features/character_menu/character_menu.cpp ~line 60-80), or the tab click is consumed elsewhere before. Next step: log hit_test for each tab x with Details open vs global page; check if tab zone excludes the active (red highlighted) tab art. Possibly shared with B103/B102.
- Slot clicks via injection at (150,163),(270,165),(224,168) did not open Details either (diagnostic --menu-release does), suggesting page-level click routing (hit zones / equipment_.hit_test) also differs for injected clicks: verify unit coordinate scaling (authored = px/(w/480)).

## Remaining
1. Find why hit_test gives none for the bag tab; fix (smallest general). 2. Keep leave_page hunk (all tabs pop the page). 3. Test + before/after frames, ctest, remove debug lines.
