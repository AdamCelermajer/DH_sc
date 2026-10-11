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

## B095 resumed investigation / test plan (2026-10-11, before new behavior changes)
- VIDEO FIRST: inspected `video350-381.png`, sampled at one frame/second from original v1.0.3 P1. At 366-368 s global Inventory is visible; at 369-374 s Feet category/details replaces its body under the same tabs, selected item changes and EQUIP updates. The category-to-global active-bag click itself is not shown in this sequence; it is source-confirmed below, not visually asserted. Historical unlabelled sheet1 is contextual, not proof of that exact click.
- Recovered authored logic: `character-menu-flow-v1/authored-actions.txt` lines 3060-3098 / byte offsets e94c-e9de: Inventory onRelease always sets LastSelectedTab, switches tab art, pops all above its parent menu then pushes menu_InventorySheetMain and MenuTab sound. `switchTab` e1a0-e244 suppresses redundant art changes only; it does not gate the caller's pop/push. NativePopAllAbove 0x43ac28 admits exactly one string argument and calls MultiMenuManager::PopMenu(name,true), 0x439270; that loop stops at the named parent, invokes hide callbacks for pages above it and pops them. NativePushMenu 0x43b1b4 -> MenuManager::PushMenu(name) 0x431924 resolves an existing menu before pushing. Back onRelease e642-e835 instead handles points confirmation or NativePopAllMenus + NativeBackToHud; do not make Back equal bag.
- Prior handoff inference corrected: action=0 was the scheduled slot/body click at frame300. Later real bag down/up had no corresponding semantic click. The fixture leaves Swamp_Intro active with --campaign-triggers, so the main cinematic gate consumes releases; this cannot establish a bag-hit contour bug. Authored bag contour includes stage245,18 and Presenter has no active-tab exclusion. B102's reviewed latch prerequisite cherry-picked as b645c2dc; B103 client-position equivalent was already present.
- Expected: an accepted character-menu tab activation discards the current subpage and enters that tab's global page, even when already selected. Body misses preserve Details; rejected/unready tab transitions preserve the current page; duplicate bag clicks are harmless and do not mutate inventory/equipment. Reuse source-provider lifecycle/real contours, no invented pixel rectangles. PC mouse scaling is independent width/480 and height/320, matching menu draw.
- Focused verification defined before coding: reproduce with fresh absolute profile/world saves and no active cinematic; real10ms Win32 clicks global->Torso->item Details->bag->global, repeated bag and slot reopen; repeat empty category at 960x640 and1280x720. Unit checks active-tab dispatch, failure/readiness, body miss and duplicate reset with inventory unchanged. Build/ctest and silent private-desktop frames on the frozen test EXE.
- Uncertainties: exact active-bag transition not directly visible in inspected video; Android binary/menu source and iOS v1.0.3 footage may differ. Audio cannot be heard in required silent runs. Root's final merged/package EXE must be reverified independently.
