# Preview 16 QUESTUI report (Quest Log tab, banners, live quest events)

Branch `p16/questui` (worktree `DH_wt/p16questui`), from `p16/quests` (wave-1 runtime). Never pushed.

Status: IN PROGRESS (skeleton). Sections are filled as each step is verified.

## Investigation (AGENTS.md items 1-4)
- Banners: `Quest::SetState` case 6 (Active) `DialogMsg(GLOBAL_QUEST_NEW, DialogStyles.QuestMsgDialog)`; case 12 (Closed) `DialogMsg(reward string, DialogStyles.QuestCompletedMsgDialog)` (IDA `pseudocode-all.c` ~273190-273260). The style art is in `original-cache/data/menus/dqhud_droid.swf` (style names found in the SWF).
- Quest Log tab: `dqcharmenu_droid.swf` has `btnQuestLogTab` (label `LogMap` in `export_art.py`) and `btnMapTab`; the Windows character menu has four tabs only.

## Investigation evidence
(to be completed)

## Implementation
(to be completed)

## Tests
(to be completed)

## Integrated runtime verification (EXE, quiet runs)
(to be completed)

## Gaps
(to be completed)

## Placeholders
(to be completed)

## Package files required
- `original-cache/data/pydata/v2quests_pyarray.bin` and `original-cache/data/pydata/v2quests_pyarraynames.bin`. Not in `windows-source-clock-v19-preview-15-rc4`. Source copies: `.local-inputs/claude-preview16/levels-root/original-cache/data/pydata/` and `.local-inputs/assets-extra/android/data/pydata/`. The runtime reads them through the normal asset root (`assets.read("original-cache/data/pydata/...")`); a missing file reports `Quest table diagnostic` and the quest runtime stays inactive.

## Verifier script
(to be completed)
