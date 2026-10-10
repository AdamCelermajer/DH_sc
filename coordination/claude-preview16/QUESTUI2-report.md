# Preview 16 QUESTUI2 report (Quest Log: Completed rows, SIDE/MAIN tag, detail pane, Make Active; description text in banners; tab hit zone)

Branch `p16/questui2` (worktree `DH_wt/p16questui2`), from `p16/integrate` (HEAD 2d703432, all Preview 16 streams). Never pushed.
Commits: `e91be20a` (text, tag, Completed list, selection), `1e1ba0d7` (Completed shift, wrap clip, banner text), `8256eea6` (deferred banner text, tab zone test).

## Status (short)
- DONE and verified in the EXE (quiet runs, frames looked at): Completed rows list under the COMPLETED header; SIDE QUEST / MAIN QUEST tag from `Quest::IsPrimary`; detail pane (title, two-line description, MAKE ACTIVE button); row select (Assigned and Completed); MAKE ACTIVE click moves the current-quest marker; Completed row shows details without MAKE ACTIVE; NEW QUEST and QUEST COMPLETED banners carry the authored sentence; tab zone no longer overlaps the Map tab.
- NOT in the reference and NOT added: objective counters and rewards in the Quest Log pane (see Investigation and Gaps).
- ctest: 129/130 pass; only `session_skill_binding` fails (known junction issue).

## Investigation evidence (AGENTS.md items 1-4)
- Visual (part2, Quest Journal). Frames: `.local-inputs/claude-preview16/questui2/frames/p2_native/qj523.png`, `qj524.png`, `qj525.png`, `qj526.png`; contact sheets `dh2_video_research/sheets2/q2-p2-500-560-2fps.png` and `q2-p2-515-541-2fps.png`. Journal visible about t=523-527. Observed: Assigned list (Buried Alive, Law and Order, The Hidden Path, Trial of Heroes 1), a COMPLETED header with a check icon, Completed list (Bogwalker's Delight 1/2, Destination - Gothicus Castle, Prison Break), orange selected row, right pane with a "Side Quest" tag, the title, the description, and a MAKE ACTIVE button at the bottom right. Pane shows nothing else in these frames. The COMPLETED header sits below the four Assigned rows (about y 195 of 320 authored).
- Banner reference: `claude-preview14/QUESTS/p1/f761.png` (QUEST COMPLETED, t~761): "Quest Completed", "Kill 8 Bog Moths.", "Reward", "20 EXP", "150 GOLD". Order matches the implementation.
- Logic (IDA `pseudocode-all.c`):
  - `Quest::GetTitle` 0x47f9dc (definition +4 = text_fields[0]), `GetPreDescription` 0x47f984 (+8 = text_fields[1]), `GetPostDescription` 0x47f94c (+12 = text_fields[2], "not specified" only for the none sentinel), `GetObjectiveDescription` 0x480a84 (+16 = text_fields[3]; none sentinel -> `ObjectiveList::GetDesc` 0x47e914).
  - `Objective::GetDesc` 0x47e8e4 -> `GetDescStr` 0x47b9b8 -> `StringManager::getString(GetDescStrId 0x47a3a0)`.
  - `Quest::SetState` 0x480c78: case 6 (Active) `DialogMsg(GLOBAL_QUEST_NEW, pre-description, QuestMsgDialog)`; case 12 (Closed) `DialogMsg(pre-description string, reward string, QuestCompletedMsgDialog, 1)`.
  - Menu AS variables set near pseudocode line 235880: `Title`, `PreDescription`, `ObjectiveDescription`, `PostDescription`, `IsPrimary`. No reward or counter variable exists in the pane's data.
- Table (decoded by `.local-inputs/claude-preview16/questui2/tools/dump_quest_text.py`): every row has text_fields[3] = 1835016 (none), and every objective `description` id is -1. So `ObjectiveList::GetDesc` is EMPTY for all Act 1 rows. The sentence the player reads is the PRE-DESCRIPTION (text_fields[1]). Examples: Swamp_Moths pre 0x1e0029 "Kill 8 Bog Moths."; Swamp_KillWitch pre 0x1e000e "Defeat the Bogwitch in the Northeast Cavern."; DW_Demon_Child pre 0x1e0064 "Search for the demon child in the Wormroot Hollow."; Darkwoods_Bandit_Hunt pre 0x1e007a "Defeat 12 Guilty Bandits within the Darkwoods."; Swamp_Escape pre 0x1e0002 "Escape the swamp with Rene and Celeste.".
- Titles: text_fields[0] (Swamp_Moths title 0x1e0028 = "Bogwalker's Delight 2"; Swamp_KillFiveLizman title 0x1e001e = "Bogwalker's Delight 1"). Consistent with the Completed list in the video.
- Text: StringID = sheet<<16 | index. The 0x1e sheet is `sidequests.english` (index 99 "Buried Alive", 100 the description; verified with `tools/sheet_strings.py`). Labels: `menu.english` 316 "Side Quest", 317 "Main Quest"; `gameplaymenus.english` 435 "Make Active", 411 "completed". Constants `MENU_SIDE_QUEST`, `MENU_MAIN_QUEST`, `GAMEPLAYMENUS_QUEST_MAKE_ACTIVE` exist in `common_text_pycst.bin`.
- Priority: v2QuestPriority Primary 0, Secondary 1, Debug 2, Dialog 3. `Quest::IsPrimary` = Primary. Prison Break (priority 0) shows MAIN QUEST; Moths and DW_Demon_Child (priority 1) show SIDE QUEST, which matches the video's "Side Quest" tag on Buried Alive.
- Tab hit zones (`original_art.cpp`): each tab quad is 63.4 px wide on a 51.9 px pitch. btnQuestLogTab was 369.05..432.45 and btnMapTab 420.95..484.35, an 11.5 px overlap. Adjacent tabs overlap the same way (see Gaps).
- Crash diagnosis (tool-side): the banner text service re-bound the item-name text owner (`bind_profile` + `borrow_text`) from inside the quest event handler. Those runs crashed (exit 0xC0000374 / 0xC0000005) once banners carried text. Fix: use the one Quest Log resolver that the page already uses every frame. Lesson for others: do not call `bind_profile` / `borrow_text` from gameplay event paths.

## Changes
1. Banner and pane text (`quest_runtime_v1.cpp` `row_objective_text_v1`): the banner sentence is text_fields[1] (pre-description). Previously it read objective descriptions, which are always -1, so banners never had text.
2. Resolver (`quest_text_resolver_v1.cpp/.hpp`, test `source_quest_log_services_v1_tests.cpp`): objective description = text_fields[3] and post check = text_fields[2], per IDA offsets (were swapped). New `primary` flag (priority == Primary).
3. Quest Log page (`runtime_quest_menu_v1.cpp/.hpp`):
   - `refresh()` builds BOTH lists (Assigned and Completed). `completed_rows` is new in the frame. Selection is kept across the per-frame refresh while the row is still listed (before, every frame cleared it).
   - `route_hit()`: row release selects from whichever list holds the row; MAKE ACTIVE uses the Assigned list.
   - Drawing: Completed rows use the unselected art and the Completed parent matrix; no current marker on Completed rows.
   - Layout: `completed_list_shift_v1(assigned)` moves the COMPLETED header and the Completed rows down by one authored row step (`row_step * 0.5615`) for each Assigned row beyond the first. The reference supports this (four Assigned rows put COMPLETED near y 195 authored; the fixed position overlapped a second Assigned row).
   - Tag: `QuestMain/text` shows `MENU_MAIN_QUEST` / `MENU_SIDE_QUEST`.
4. Hit routing (`source_quest_menu_page_provider_v1.cpp`): Assigned rows use the Assigned parent; Completed rows use the Completed parent plus the same shift; the MAKE ACTIVE hit depends on the selection's `activation_visible` only.
5. Multiline clip (`main.cpp`, shared character-menu text path): the clip is the union of each wrapped line's own `clip_rect` (through `clip_matrix`), and it grows by the extra wrapped lines. Before, the clip was the one-line authored field box, which cut the second line of the description. The reference description wraps to two lines ("Escape the swamp with Rene / and Celeste."). Skills multiline text uses the same path; its per-line rects are no wider than before.
6. Banner text service (`main.cpp`): `questServices.text` answers through the Quest Log resolver (`questMenuText`, bound once in the composition block). Bind-time banners (fresh game NEW QUEST) get their text in `takeQuestBanners()`, after the resolver exists. Diagnostics print only real resolver failures.
7. Tab hit zone (`original_art.cpp`): btnQuestLogTab right edge 432.45 -> 420.95 (the btnMapTab left edge). The Map zone is unchanged (MAPFIX's zone; no Map page file touched).
8. Test `character_menu_tests.cpp`: Quest Log and Map zone x-ranges do not overlap.
9. Report and tools: `coordination/claude-preview16/QUESTUI2-report.md`; `.local-inputs/claude-preview16/questui2/tools/` (`dump_quest_text.py`, `sheet_strings.py`, `make_args.py`).

## Tests
- ctest (`p14_build.ps1 -Name p16questui2 -Jobs 6 -Test`, then ctest directly with the llvm-mingw bin on PATH): 129/130 pass, only `session_skill_binding` fails. Includes `source_quest_log_services_v1_tests` (updated index check and primary flag), `character_menu_tests` (new overlap check) and `quest_runtime_v1`.

## Integrated runtime verification (real EXE, `quiet_run.ps1`, hidden desktop, silent)
Build: `DH_wt/build-p16questui2/dh-foundation.exe`. Args from the MERGEFIX quest run (`make_args.py`). Jobs: `run/jobs5.json` (list, detail, make, c-completed), `run/jobs6.json` + `jobs7.json` (make2, banner-new, banner-done), `run/jobs8.json` (map regression). Logs and frames in `.local-inputs/claude-preview16/questui2/run/out/`. All exit 0 in the final runs.
- Assigned + Completed (`list.png`, `list.log`): Assigned "Prison Break" (current), COMPLETED header, Completed row "Bogwalker's Delight 2" (= Swamp_Moths after 8 kills).
- Detail (`detail.png`, click Prison Break at authored 154,81 on the Quest tab): MAIN QUEST tag, title "Prison Break", description wrapped to two lines, orange selection, MAKE ACTIVE at bottom right.
- Completed row (`c-completed.png`, click completed row): SIDE QUEST tag, "Bogwalker's Delight 2", "Kill 8 Bog Moths.", no MAKE ACTIVE.
- MAKE ACTIVE (`make.png` before, `make2.png` after; click Prison Break row then MAKE ACTIVE at 380,285): the current-quest diamond moves from "Bogwalker's Delight 2" to "Prison Break". The page is cleared after the click (existing behaviour; see Gaps).
- Banners: `banner-new.png` (frame 90): NEW QUEST with "Escape the swamp with Rene and Celeste." (bind-time banner). `banner-done.png` (frame 420): QUEST COMPLETED, "Kill 8 Bog Moths.", Reward, 20 EXP, 150 GOLD. Logs show `text='Kill 8 Bog Moths.'` on the NEW QUEST / QUEST UPDATED / QUEST COMPLETED lines of the Moths run.
- Map regression (`map-regress.log`, `map-regress.ppm`): exit 0 with the new tab zone.
- Frame-level verification of the Map tab itself and a real tab-row click on the Quest tab were NOT done (the Quest page is opened by the test aid `--quest-page-frame`; the zone is checked by the unit test).

## Gaps (explicit)
1. Objective counters and rewards in the pane: NOT added. The reference pane (t=523-527) shows only title and description, and the menu's data (IDA AS variables) has no reward or counter field. The objective sentence (`ObjectiveList::GetDesc`) is empty for every Act 1 row, so nothing is shown in its place.
2. Initial selection on opening the page: the reference shows "Buried Alive" selected when the journal opens (t=523). Our page opens with nothing selected. The rule (current quest, or first row) is not yet evidenced in IDA. Left open.
3. After MAKE ACTIVE the page clears the selection (existing wave-1 behaviour). The original post-click state is not captured in the reference frames.
4. QUEST UPDATED (objective counter) banner: no original banner observed for counters (`New Objective` / `Quest Updated` strings exist in `global.english` 52/53, but no IDA call site was found). It keeps the Preview 16 tracker form: sentence + "n / m". Placeholder, see below.
5. Banner dialog frame: the ornate QuestMsgDialog / QuestCompletedMsgDialog art is not exported. The panel is the placeholder (see Placeholders).
6. Tab-row overlap: quest/map overlap is fixed. Other adjacent tab pairs (faery/quest, stats/equipment, skills/faery, and so on) still overlap by 11.5 px (tab row geometry, `original_art.cpp`). Not changed here; the tab-row owner should decide one partition rule (for example zones of exactly 51.9 px centred on the icons).
7. `export_art.py` regeneration is guarded (wave-1 exporter issue). The quest zone change was made by hand in `original_art.cpp`; a regeneration would restore the old edge.
8. Initial NEW QUEST banner (bind-time, Prison Break at fresh start) is shown on load as before. Its trigger/timing was not re-investigated.
9. Scripts still do not run (wave-1 gap); the NPC talk is an interact-press approximation (wave-1 gap).

## Placeholders
- Banner panel (`drawQuestBanner`, `quest_banner_presenter_v1.cpp`): dark translucent panel with a bronze border, drawn in place of the original dqhud QuestMsgDialog / QuestCompletedMsgDialog frame. Replicate from `claude-preview14/QUESTS/p1/quad-p1-newquest.png` (NEW QUEST, t~659) and `p1/f761.png` (QUEST COMPLETED, t~761).
- QUEST UPDATED counter line: not an original banner (see Gaps 4). Verify against the Moths kill sequence before keeping it.
- The Quest Log text itself (titles, sentences, tags, MAKE ACTIVE label) is authored text from the original tables (not placeholders).

## Package files required
- `original-cache/data/pydata/v2quests_pyarray.bin` and `v2quests_pyarraynames.bin` (absent from `windows-source-clock-v19-preview-15-rc4`; present in `.local-inputs/claude-preview16/levels-root/original-cache/data/pydata/`). Without them the runtime reports `Quest table diagnostic` and stays inactive.
- Text (present in rc4): `original-cache/data/text/sidequests.english`, `gameplaymenus.english`, `global.english`, `menu.english`, `common_text_pycst.bin` (constants).

## Verifier script
1. Build and test: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p16questui2 -Jobs 6 -Test` (expect only `session_skill_binding`).
2. Jobs (already written, use the same JSON): `.local-inputs/claude-preview16/questui2/run/jobs5.json` (list / detail / make / c-completed page frames), `jobs6.json` (make2, banner-new, banner-done), `jobs7.json` (banner-done at 420). Run each with `port/windows-foundation/tools/quiet_run.ps1 -JobsFile <job> -Parallel 4`.
3. Expected log lines: `Quest zones built=1 _prim_WitchQuestStart ... level=41`; `Quest banner kind=NEW QUEST row=53 ... text='Kill 8 Bog Moths.'`; `Quest banner kind=QUEST COMPLETED row=53 xp=20 gold=150 text='Kill 8 Bog Moths.'`; `Character menu Quest Log selected frame=150`.
4. Frames to LOOK at: `run/out/list.png`, `detail.png`, `c-completed.png`, `make.png`, `make2.png`, `banner-new.png`, `banner-done.png`.
5. Test aids (not gameplay): `--quest-page-frame`, `--quest-debug-accept F:ROW`, `--quest-debug-kill F:TEMPLATE:COUNT`, `--menu-release F:X:Y` (authored 480x320 coordinates: Assigned row 0 at 154,81; row 1 at 154,109; Completed row 0 at 154,136; MAKE ACTIVE at 380,285; Quest tab at about 395,12).
