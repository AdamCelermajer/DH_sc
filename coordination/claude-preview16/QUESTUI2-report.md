# Preview 16 QUESTUI2 report (Quest Log: Completed rows, detail pane, Make Active; description text; tab hit zone)

Branch `p16/questui2` (worktree `DH_wt/p16questui2`), from `p16/integrate` (all Preview 16 streams). Never pushed.
Status: IN PROGRESS (skeleton). Sections below are filled as each part is verified.

## Investigation evidence (AGENTS.md items 1-4)
- Visual: part2 t=523-527 (Quest Journal). Frames: `.local-inputs/claude-preview16/questui2/frames/p2_native/qj523.png`, `qj524.png`, `qj525.png`, `qj526.png`; contact sheet `dh2_video_research/sheets2/q2-p2-515-541-2fps.png`. Observed: Assigned list (Buried Alive, Law and Order, The Hidden Path, Trial of Heroes 1), a "completed" header with a check icon, Completed list (Bogwalker's Delight 1/2, Destination - Gothicus Castle, Prison Break), selected row in orange, right pane with "Side Quest" tag, title, description, and a MAKE ACTIVE button at the bottom right. No objective counters or rewards are visible in the pane in these frames.
- Logic (IDA `pseudocode-all.c`): `Quest::GetTitle` 0x47f9dc (definition +4), `Quest::GetPreDescription` 0x47f984 (+8), `Quest::GetPostDescription` 0x47f94c (+12, "not specified" only for the none sentinel), `Quest::GetObjectiveDescription` 0x480a84 (+16, falls back to `ObjectiveList::GetDesc` 0x47e914 when +16 is the none sentinel 1835016), `Objective::GetDesc` 0x47e8e4 -> `GetDescStr` 0x47b9b8 -> `StringManager::getString(GetDescStrId 0x47a3a0)`.
  `Quest::SetState` 0x480c78 case 6 (Active): `DialogMsg(GLOBAL_QUEST_NEW, pre-description id, QuestMsgDialog)`. Case 12 (Closed): `DialogMsg(string of pre-description, reward string, QuestCompletedMsgDialog, 1)`.
  Quest menu AS variables (`ObjectiveDescription`, `PreDescription`, `Title`, `PostDescription`, `IsPrimary`) are set in the function near pseudocode line 235880 (`Quest` menu details).
- Table (decoded with `.local-inputs/claude-preview16/questui2/tools/dump_quest_text.py` from `original-cache/data/pydata/v2quests_pyarray.bin`): `text_fields` = [title, pre-description, location, none]. Every row has text_fields[3] = 1835016 (none), so `GetObjectiveDescription` uses `ObjectiveList::GetDesc`. Every objective `description` id is -1, so `Objective::GetDesc` returns the empty string for all Act 1 rows. The sentence the player reads is therefore the PRE-DESCRIPTION (text_fields[1]), e.g. Swamp_Moths pre = 0x1e0029 = "Kill 8 Bog Moths.", Swamp_KillWitch pre = 0x1e000e = "Defeat the Bogwitch in the Northeast Cavern.", DW_Demon_Child pre = 0x1e0064 = "Search for the demon child in the Wormroot Hollow.", Darkwoods_Bandit_Hunt pre = 0x1e007a = "Defeat 12 Guilty Bandits within the Darkwoods.".
- Text: StringIDs are sheet<<16 | index into `original-cache/data/text/sidequests.english` (sheet 0x1e). Verified by parsing the sheet (`tools/sheet_strings.py`): index 99 "Buried Alive", index 100 the description above. Labels: `menu.english` 316 "Side Quest", 317 "Main Quest"; `gameplaymenus.english` 435 "Make Active", 410/411 "not completed"/"completed"; constants `MENU_SIDE_QUEST`, `MENU_MAIN_QUEST`, `GAMEPLAYMENUS_QUEST_MAKE_ACTIVE` exist in `common_text_pycst.bin`.
- Priority: `v2QuestPriority` Primary 0, Secondary 1, Dialog 3, Debug 2 (`quest_runtime_v1.hpp` comment; Quest::IsPrimary reads the same field).
- Tab hit zones: each tab quad is 63.4 px wide on a 51.9 px pitch (btnQuestLogTab 369.05..432.45, btnMapTab 420.95..484.35), so the quest and map zones overlap by 11.5 px. All adjacent tab pairs overlap the same way.

## Changes
(in progress)

## Tests
(in progress)

## Integrated runtime verification
(in progress)

## Gaps
(in progress)

## Placeholders
(in progress)

## Package files required
(in progress)

## Verifier script
(in progress)
