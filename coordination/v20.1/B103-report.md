# B103 report: character menu buttons not clickable (partial; session resumed after a rate-limit cut)

## Root cause (what is established)
1. The five icons on the Statistics page (btnStatisticsTab/Defence/Offense/Magic/Recovery, authored in sprite279 of the character-menu SWF,
   authored-actions.txt 0xf6ba-0xf9aa) have NO hit zone and NO handler in our port: `Presenter::hit_test` only knows the six top tabs,
   close and map controls, and `SourceCompositionV1::release` on the stats tab only resolves the four `+` buttons. The original handlers
   call NativePopMenu / NativePushMenu(menu_CharacterSheetStats|Defense|Offense|Magic|Recovery) + sound MenuSelect. So clicking the icons does
   nothing: this is a missing feature path, not a hit-test bug. NOT FIXED in this session (needs the Defence/Offense/Magic/Recovery
   sub-page art+text, which our single combined Stats page does not have).
2. The `+` buttons are disabled in the original when Points left is 0 (deactivated frame, 0x1310a); the user's shot-21 shows Points left 0.
   Inferred: the `+` "not clickable" there is correct behaviour. With points they work (below).
3. Harness-only finding: `Window::cursor_position` used GetCursorPos, which is invalid for injected WM_* messages on a hidden desktop.
   Fixed (platform_win32.cpp): the window now records the client position from WM_MOUSEMOVE/WM_LBUTTONDOWN and uses it first. Real mouse
   behaviour is unchanged (same coordinates). This is a robustness/test change, not the user's bug.
Shared with B102/B095: the click path is main.cpp uiSurface.hit -> SemanticInput (menu_item) -> `uiInput.clicks` -> `characterMenuComposition->release`.
It works end to end for real injected clicks (below); B102/B095 should look at their page's release provider, not at hit-testing/input.

## Verification (real EXE, quiet hidden desktop, injected WM_MOUSEMOVE/LBUTTONDOWN/UP; tools/quiet_click.ps1, new)
- `C` key (WM_KEYDOWN) opens the menu (log "Character menu opened via profile input").
- With `--test-stat-points 5` (test-only flag), two injected clicks on the Strength and Dexterity `+`: Points left 5 -> 3, stats raised
  (frame .local-inputs/claude-v20.1/fix103/z.png). (Strength shows 1 because the test grant re-projects the sheet; test artifact.)
- Not verified: clicks on the stat sub-tab icons (no handler), real physical mouse (GetCursorPos path unchanged), Faery/Quest/Map pages.

## Video / IDA match
IDA/SWF: authored-actions.txt as cited. Video not consulted this session (rate-limit cut); the original sub-page screens were not compared.

## Commits
017e9f79 (injector + WM_MOUSEMOVE position), ab3507a2 (test-only flag, diagnostic detail). Build -Jobs 2 exit 0; ctest not re-run.
