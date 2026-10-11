# B073 Quest Journal not as intended (fix/b073, build `fix073`)

## Evidence
Visual (video Part 2 `part2_cIAW38IfLxY.mp4`, frames at 1/2 fps t=66-84, 1 fps t=82-93, and t=518-532):
- t=84: the Quest tab opens with the current quest already highlighted (orange bar, dot) and its details (MAIN QUEST crown, title, text); MAKE ACTIVE visible. Direct observation; the mechanism is not visible.
- Section headers read "Quest Journal" (orange scroll icon + underline) and "completed" (green check icon + underline). Side quest tag has a yellow bullet, main quest tag a crown (t=84 vs t=523/524).
- Completed rows sit below COMPLETED; the current-quest dot stays on its row while another row is selected (t=524).
Ours (user shot b073, Preview 16 EXE frames): header "Quest Log", no header/check icons, no underlines, crown shown for side quests, nothing selected on open in the test harness.

IDA/SWF (`port/engine-ui/reference/character-menu-flow-v1/authored-actions.txt`, menu_QuestLogSheetNEW):
- 0x268e1: onShow sets Assigned/Header.text = GAMEPLAYMENUS_QUEST_JOURNAL_TITLE ("Quest Journal"); 0x26911 CompletedTitle = GAMEPLAYMENUS_COMPLETED. GAMEPLAYMENUS_QUEST_LOG (0x266b0) only seeds QuestDetailsText.
- 0x2647a-0x26512: row onRelease: QuestPrio.gotoAndStop(1) + MENU_MAIN_QUEST if IsPrimary, else gotoAndStop(2) + MENU_SIDE_QUEST; SelectedQuest is set only for Assigned rows (MAKE ACTIVE only for Assigned).
- 0x26a87/0x26a9c: QuestPrio hidden while the Assigned list is empty.
- 0x26c94-0x26d48: MAKE ACTIVE skips when SelectedQuest == CurrentActiveQuest, else NativeSetCurrentQuest (0x43d3b4 -> Character::SG_SetCurrentQuest) and moves LightOn; selection is not cleared.

## Root causes
1. Exporter bug (`export_runtime_quest_menu_art_v1.py`): the list mask (shape 556, solid fill) produced an empty clip region (bitmap triangles only), so every bitmap under `content` was clipped: underlines, scroll icon, check icon.
2. Solid fills (orange scroll fill) had no z-anchor and were drawn before the page background.
3. Header text mapped to GAMEPLAYMENUS_QUEST_LOG instead of QUEST_JOURNAL_TITLE.
4. QuestPrio exported only as frame 0 (crown) for every quest.
5. Selection cleared after MAKE ACTIVE and nothing selected on open.

## Changes (port/windows-foundation/features/quests, plus one CMake test hunk)
- Exporter: mask includes solid triangles; solids get an `after` bitmap anchor; QuestPrio exported per frame (`prio_art[0]` main, `[1]` side); regenerated runtime_quest_menu_art_v1.cpp/.hpp.
- runtime_quest_menu_v1.cpp: Header text key; Completed header art shifts with the Assigned list like its text; QuestPrio frame by IsPrimary and hidden when Assigned is empty; refresh selects the current Assigned row when nothing is selected; MAKE ACTIVE keeps the selection.
- Test quest_journal_art_v1_tests.cpp (+ CMakeLists hunk after quest_banner_presenter_v1).

## Verification
- ctest in build-fix073: 140/141 pass; only the expected session_skill_binding fails; quest_journal_art_v1 passes.
- Real EXE frames (quiet_run, --quest-page-frame 150), scratch `.local-inputs/claude-preview161/fix073/out/`: before-rows.png (Preview 16) vs after-rows.png, after-side.png (icons, underline, side bullet, details), after-auto2.png (auto-selected current row, dot, MAKE ACTIVE).

## Not verified / gaps
- The original's initial-selection mechanism is unknown (the ActionScript has no auto-select); implemented from the video at t=84. With no current Assigned row nothing is selected.
- A real mouse click on MAKE ACTIVE and on the tab row was not exercised in the EXE (test aid only); keeping the selection follows the ActionScript, not a frame.
- Tutorial hint popup over the journal and list scrolling with many rows (adjustScrollArea) not examined.

## Package files required
None new (geometry compiled in; strings already present).
