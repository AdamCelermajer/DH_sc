# P14 FAERY report (continuation worker)

Branch `p14/faery`, worktree `DH_wt/faery`. Commits: `34e40c6c` (CharacterState page provider bound), `6ee3976a` (WIP snapshot from the killed worker), `c53926be` (this continuation: legacy-save handling, table load, log lines).

## Status by task

| Task | State | Evidence |
|---|---|---|
| T1 CMake wiring of `character_state_page_v1`, `original_art`, `source_text_v1`, `character_state_faery_v1`, tests | Done | `CMakeLists.txt` faery block (143-153) and `character_state_page` ctest (lines ~556-562). Legacy `faery_menu.cpp` / `session_faery_page_v1` NOT built. |
| T2 provider registered, stub removed, render from CharacterState, click gate, persist, key-4 refresh | Done and verified in EXE | `main.cpp` registration block (~2012-2030), stub removed (`Action::faery` now goes through `release`), `refreshPcHudForFaery`. |
| T3 SetFaeryState / IncFaeryLevel routed to live CharacterState | Done and verified in EXE for Swamp_Intro command 156 | `original_campaign_world_adapter.cpp` cases 27/28, `main.cpp` providers `set_faery_state` / `inc_faery_level`. |
| T4 one named difficulty accessor | Done | `active_faery_difficulty_v1()` in `features/faery_menu/character_state_faery_v1.hpp` (returns 0). Used by page, HUD key-4, script effects, cast arm. |

## What changed in this continuation (c53926be)

1. `main.cpp`: Faery tables are loaded for the page whenever not yet loaded (previously gated on `source_faery_state_known`, so legacy saves got "host requires ... FaeryTables" and no page).
2. `main.cpp`: T3 hooks log `Source SetFaeryState slot=N state=M committed to CharacterState` / `Source IncFaeryLevel ...`. For legacy saves (`source_faery_state_known == false`) they log `skipped: legacy save has no source Faery rows (limitation)` and return success so the script continues (no data is invented).
3. `main.cpp`: a commented hunk at the key-4 HUD reads the active difficulty via the accessor.

Earlier (WIP, 34e40c6c/6ee3976a, reviewed and kept): page provider rejects locked Faery in `select_character_state_faery_v1` (provider, not just UI); `character_state_faery_v1.{hpp,cpp}` host (commit with rollback on persist failure, HUD refresh, script effects); `original_campaign_world_adapter` kinds 27/28; CMake sources.

## Tests run (real output)

- `p14_build.ps1 -Name faery -Test`: build exit 0 (main.cpp rebuilt after each edit).
- `character_state_page` ctest: `Test #78: character_state_page ... Passed`. It covers tri-state gate, difficulty rows, unknown legacy state, provider rejection of locked slots without mutation, same-owner dispatch, invalid current, SetFaeryState/IncFaeryLevel row effects and overflow/invalid-slot rejection, and host persist rollback.
- Full ctest: `103 tests, 1 failed`: `29 - session_skill_binding (Failed)`. Output: `Asset path escapes asset root`. Cause: the worktree `.local-inputs` is a junction into `DH_sc/.local-inputs`, and `asset_catalog.cpp` `within()` rejects the canonical path. Running the same binary with the canonical root `C:/Users/adamc/Desktop/workspace/DH_sc` prints `PASS ...` and exit 0. Not touched by this stream (no diff in that test or in asset_catalog). Root should fix the test argument or the junction check for all worktrees.

## Evidence (quiet batch, EXE `build-faery/dh-foundation.exe`)

Jobs: `.local-inputs/claude-preview14/faery/run14/batch1.json` (6 jobs) and `batch2.json` (1 job). Logs and captures in `.local-inputs/claude-preview14/faery/run14/out/`. Save copies in `.../run14/saves/`.

- `run14-view-celest` (Rogue save, Celest unlocked and selected): `Character menu Faery selected frame=60 via CharacterState provider`. Capture shows Celeste detail text, slot 0 focused. Save byte 954 = 1 (Celest state).
- `run14-view-zero` (all rows zero): same log line, Celest focused but locked (focus shows the selected slot), save byte 954 = 0.
- `run14-script-zero` (zero save + `Swamp_Intro:156:40`): `Source command frame=40 script=Swamp_Intro index=156 kind=27`, `Source SetFaeryState slot=0 state=1 committed to CharacterState`. Save byte 954 changed 0 -> 1 on disk. T3 verified end to end.
- `run14-legacy-script` (legacy save known=0 + same script): `Source SetFaeryState slot=0 skipped: legacy save has no source Faery rows (limitation)`; process continues; save unchanged.
- `run14-locked-click` (click slot 2 at authored 242,76, locked): `Character menu action diagnostic: Faery slot 2 is locked; selection rejected`. Save unchanged.
- `run14-select-hotty` (save with Primula/slot 1 unlocked, current 0; click slot 1 at 162,76): `Faery selection committed current=1 HUD key-4 refresh`. Save current byte (950) = 1. HUD: `hud-hotty0` (same save, no menu) shows the gold Celest icon at key 4; `run14-select-hotty` shows the green icon. Refresh verified by comparing the two captures (`out/hud-hotty0-crop.png`, `out/select-hotty-crop.png`).
- Earlier scratch runs (previous worker, `.local-inputs/claude-preview14/faery/out`): `knight-empty` / `mage-empty` showed the missing-tables diagnostic (fixed in c53926be).

Captures looked at: `run14-view-celest.png` (rogue-celest), `unlock-visible.png`, `rogue-zero.png`, `crop-zero-vs-script.png`, `crop-unlockvis-vs-zero.png`, `hud-hotty0-crop.png`, `select-hotty-crop.png`.

What the captures show: a selected/unlocked slot is bright, the selected slot is drawn enlarged with gold/green glow and the detail panel shows the authored name ("Celeste - Normal Form / Lightning Faerie", slot 1 "Primula - Normal Form / Earth Faerie"). The non-selected locked/unlocked difference is weak in the captures: unlocked-but-idle slot 0 is smaller than the focused slot 0; the locked slots 2-4 look the same in both zero and unlock captures. I could not confirm a distinct locked visual from these images, so treat the locked art as unverified.

## Reference (Part 2) check (G5)

Scanned every unchecked sheet in `dh2_video_research/sheets2` (s001, s003, s004, s006-s009, s011-s015, s017-s019, s021, s023, s024, s026, s027, s029; 21 sheets, not 13). No frame shows the Faery page open. The butterfly tab (4th icon) is visible in the tab bar in the map/quest/skills frames but is never selected. Part 1 was not scanned in this pass. The Faery page layout therefore has no reference image in Part 2; the SWF geometry in `original_art.cpp` remains the authority.

## Needs from schema v4

- A real current-difficulty field on CharacterState (or a session field). Until then `active_faery_difficulty_v1()` returns 0 and is the only place to change.
- Nothing else. The page reads and writes only existing `faery_by_difficulty` fields. Legacy saves (`source_faery_state_known == false`) remain unknown and are not written by script effects.

## Package files required

- None new. Runtime uses existing content: `data/pydata/faeries_pyarray.bin`, `faeries_pyarraynames.bin`, `faeries_pystructnames.bin`, and the localization assets already loaded by the menu.
- Tests use `.local-inputs/character-skill-session-v2/cache/data/pydata/faeries_*.bin` and `.local-inputs/windows-main-frontend-v1/assets` (already in CMake).

## Verifier script

From `port/windows-foundation/tools`:
`quiet_run.ps1 -JobsFile <...>/run14/batch1.json -Parallel 6 -Summary <...>/run14/summary1.json` (6 jobs), then `batch2.json` (hud-hotty0). Expected:
- view-celest / view-zero: `Character menu Faery selected frame=60 via CharacterState provider`.
- script-zero: `Source SetFaeryState slot=0 state=1 committed`; save byte 954 = 1.
- legacy-script: `... skipped: legacy save ... (limitation)`.
- locked-click: `Faery slot 2 is locked; selection rejected`; no save change.
- select-hotty: `Faery selection committed current=1 HUD key-4 refresh`; byte 950 = 1; key-4 icon green (hud-hotty0 gold).

## Open risks and limitations (not resolved here)

1. Legacy saves never get Celest from Swamp_Intro (logged skip). Decision needed: keep the logged limitation or initialise rows for legacy saves (creation-equivalent zeros).
2. Scope: Rocky, Wetty, Windy are shown and selectable but have no spell code, so key-4 does nothing for them. Brief said "Scope Celest+Hotty spells"; I kept the page faithful (all five slots) and did not add a selection gate. Needs a user decision.
3. Slot 1 is "Primula - Earth Faerie" on the page (authored text). Mapping to the `faerie_hotty` row is inferred, not verified (survey G7).
4. Hotty cast (key 4 with slot 1) was not exercised in this pass; Celest cast path is unchanged.
5. First-unlock tutorial cinematic (`cinematic_Tuto_faery`, slot != 0) and the `faery_charged` trophy are not routed. The campaign export has no IncFaeryLevel commands, so the level path is inert in content today.
6. New character through the creation UI was not run. The proxy is a save with all rows zero, which is the same state creation writes (`runtime_creation_persistence_v1.cpp` 363-366).
7. Difficulty is fixed at 0 (T4 accessor).
8. Each script effect and each accepted click persists the save through `options.save` (rollback on failure for clicks only).
9. Locked-slot visual distinction is not confirmed (see Evidence).
10. ctest `session_skill_binding` fails in worktrees (junction path check), see Tests.
