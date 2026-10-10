# SCHEMA report (Preview 14, stream schema, branch p14/schema)

Status: codec, stamping, panel projection and profile difficulty are implemented and built. ctest is green except `session_skill_binding` (known failure in worktrees because of the `.local-inputs` junction). The slot panel was verified in the EXE for a legacy slot and for slots after a save point (Knight and Mage). Not verified in the EXE: non-combat F5, pause-menu return, menu-return commit, and new-profile creation through the Start-game UI (see Open risks).

## Commits (branch p14/schema)
- `46653b05` CharacterState schema v4: codec, menu metadata module, CQPG v2 counters, tests.
- `ef036be1` SCHEMA: stamping at save points, menu panel projection, profile difficulty.
- `6bcbbe30` WIP snapshot (frontend edits, found applied and consistent).
- This round: main.cpp non-combat F5 branch rewritten to a plain `if/else` with the stamp call (behaviour unchanged, comma-expression removed).

## What changed (files)
- `port/windows-foundation/character_state.hpp/.cpp`: `character_schema_version` 3 -> 4; `CharacterMenuMetadata {known, save_time, level_row[3], current_act[3]}`; `CharacterVisitedModule {level_uri, module_id, visited}`; `current_difficulty`, `unlocked_difficulty`; accessors `character_current_difficulty()` / `character_unlocked_difficulty()`; validation (difficulty 0..2, row -1..4096, act 0..4096, unknown metadata must be blank, visited sorted/unique, visited <= 16384).
- `port/windows-foundation/save_store.cpp`: v4 tail encode/decode (below). Schema > 4 rejected; v1-v3 load with defaults (known=false).
- `port/windows-foundation/game_save.cpp`: same tail in the GameSave archive (schema >= 4).
- `port/windows-foundation/character_quest_blob.hpp` and `features/quests/character_quest_progress_v1.cpp`: CQPG v2 objective counter section (`QuestObjectiveCounterV2`, read/write/decode API); v1 blobs stay byte-identical when no counters are written.
- `port/windows-foundation/features/menu_metadata/menu_metadata_v1.hpp/.cpp` (new): `load_level_tables`, `find_level_row`, `initialize_fresh_menu_metadata` (row 41, act 1, Normal, date now), `stamp_menu_metadata[_for_level]`, `set_visited_module`, `project_slot_presentation` (uses engine-ui `menu_save_slot_projection_v1`, `save_slot_date_v1`, MenuLocalization for GLOBAL_LEVEL, MENU_ACT, MENU_LAST_SAVE, MENU_DIFFICULTY, GAMEPLAYMENUS_DIFFICULTY_*).
- `features/character_menu/menu_text.cpp/.hpp`: `constant()` and `parsed_symbol()` (NativeGetParsedString with ^d).
- `features/frontend/creation/dynamic_text_bindings.cpp/.hpp`: optional localized strings (level, act, last-save label, difficulty title and value); empty falls back to the old literals.
- `features/frontend/creation/runtime_creation_persistence_v1.cpp`: new profile stamped at creation (FS_StartGame).
- `features/frontend/frontend_presentation_v1.cpp`: menu starts on the profile's CurrentDifficulty; `interaction.selected_difficulty(...)` is passed to the frontend.
- `main.cpp` (shared hunks, small and commented): `stampSaveMetadata` lambda; `start_same_state` accepts 0..unlocked and stores `current_difficulty`; `project_selected_profile` uses the metadata projection; skill caps and reward context read the profile difficulty; stamps at pause profile save (~L2464-2487), combat F5 (~L2551-2555: also rewrites `options.save`), non-combat F5 (~L2556-2560).
- `CMakeLists.txt`: sources for `menu_metadata_v1` plus `level_tables`, `menu_save_slot_projection_v1`, `save_slot_date_v1` in `foundation_frontend`; tests `schema_v4` and `menu_metadata_v1`.
- Tests: `tests/schema_v4_tests.cpp` (new, 278 lines), `tests/save_tests.cpp`, `tests/game_save_tests.cpp`, `features/frontend/creation/runtime_creation_persistence_v1_tests.cpp`, `features/menu_metadata/menu_metadata_v1_tests.cpp` (new).

## Exact v4 byte layout (all integers little endian)
Header and body are unchanged from v3:
```
0   8 bytes  magic "DHSAVE\0\x01"
8   u32      file format version = 1
12  u32      character schema version = 4 (1..3 still load)
16  ...      body fields of schema 1..3 (unchanged; quest blob = u32 size + CQPG bytes)
```
Schema-4 tail, appended after the `source_quest_progress_cqpg` field:
```
+0   i32  current_difficulty      0..2 (PDFL CurrentDifficulty)
+4   i32  unlocked_difficulty     0..2 (UnlockedDiff)
+8   u8   menu_known              0 or 1 (other values rejected)
+9   u32  save_time               unix time (SG_SetSaveDate); 0 only when unknown
+13  i32  level_row[0]            LevelList row for Normal (-1 unset, <= 4096)
+17  i32  level_row[1]            Hard
+21  i32  level_row[2]            Heroic
+25  i32  current_act[0]          1..4096 (0 allowed only when unknown)
+29  i32  current_act[1]
+33  i32  current_act[2]
+37  u32  visited_count           <= 16384
+41  visited_count x { u32 len; len bytes level_uri; u32 module_id; u8 visited (0/1) }
```
Fixed tail = 41 bytes. Visited entries are unique and sorted by (level_uri, module_id). Reserved for the map stream, normally 0 entries.
Check: the in-EXE file `knight-after/character.save` is 1008 bytes = 967 (v3 Knight fixture) + 41 (tail with 0 visited entries). Header bytes after the save: `44 48 53 41 56 45 00 01 | 01 00 00 00 | 04 00 00 00`.

CQPG v2 (inside the quest blob, not a file-level change): `"CQPG" | u32 version=2 | u32 rows | u32 name_size | name | 6 buckets | u32 counter_count | counter_count x { u8 collection | u8 difficulty | u32 row | u32 objective | i32 quantity | u8 completed }`, strictly sorted and unique, objective < 64, counters <= 4096.

## Behaviour
- Legacy v1-v3 slots: `menu_metadata.known=false`; the panel shows name, class and LEVEL only; Act, location, difficulty and last-save rows stay blank until the next save point.
- Save points stamp `known=true`, `save_time=now` (wall clock, approved), and the LevelList row of the current level for `current_difficulty`. Act is stamped from the state (1 unless the quest stream sets it).
- New profile: row 41 (001_swamp, LOCATIONS_LOC_BOGLANDS "The Boglands") for all difficulties, act 1, difficulty 0, unlocked 0, date now.
- Difficulty: `start_same_state` accepts 0..unlocked_difficulty and writes `current_difficulty`. Unlocked is 0 for every profile now, so only Normal is startable.

## Tests run (real output)
`powershell -NoProfile -File DH_wt/p14_build.ps1 -Name schema -Test` (build exit 0), then `ctest --test-dir DH_wt/build-schema -j 6`:
```
99% tests passed, 1 tests failed out of 104
The following tests FAILED:
	 29 - session_skill_binding (Failed)     <- known: .local-inputs junction in worktrees
```
Including `schema_v4` (Passed), `menu_metadata_v1` (Passed), `save`, `game_save`, `private_save_transport_win32`, `character_menu`, `combat_session*` (Passed).
Coverage named in `schema_v4_tests.cpp`: v4 round trip and re-save byte exact; tail byte layout matches the documented one; v3 fixture loads with defaults and the read does not rewrite the file; explicit save upgrades v3 to v4; schema 2 loads; truncated tail; trailing byte; invalid known flag; difficulty 3; unlocked -1; level row beyond limit; visited count beyond limit; visited byte 2; unsorted/duplicate/empty visited; unsupported schema save does not overwrite the slot; CQPG v1 envelope valid; counters switch to v2; duplicate/unsorted counter rejected; failed write leaves blob unchanged; row beyond quest table rejected.

## Evidence (EXE, quiet runner, schema build `DH_wt/build-schema/dh-foundation.exe`)
Scratch: `.local-inputs/claude-preview14/schema/`. Frames converted with `claude-preview12/verify-combat/ppm2png.py` and inspected.

1. Legacy slot, menu route (`--start-mode menu --menu-frames 120`, fixture = v3 Knight, `knight-before/frames/frame-000120-3904ms.png`): `QA / Warrior / LEVEL 1`, then Act, location, difficulty and last-save rows blank, red X. Matches the MENUMETA survey B5 layout for legacy saves.
2. Save point, gameplay route (`--menu-actions menu_MainMenu.btn_MENU_SINGLE_PLAYER|menu_StartGame.StartMenuButtons.btn_MENU_SINGLE_PLAYER --save-frame 40 --frames 80`): log `Saved live checkpoint frame=40 HP=165.098 RNG=8504954/20` (Knight) and `frame=40 HP=141` (Mage). Slot file rewritten to schema 4.
3. Menu after the save point, Knight (`knight-after/frames/frame-000120-4192ms.png`, seen): `QA / Warrior / LEVEL 1 / Act 1 / The Boglands / Difficulty: / Normal / Last Save / 10/10 17:49` with the red X. Layout matches `ref_full65.png` (CRISR82 / Warrior / LEVEL 1 / Act 1 / The Boglands / Difficulty: Normal / Last Save 08/10 16:23). Only the name and the date differ.
4. Menu after the save point, Mage (`mage-after/frames/frame-000120-4778ms.png`, seen): `QA / Mage / LEVEL 1 / Act 1 / The Boglands / Difficulty: Normal / Last Save 10/10 17:49`. The Mage model and class label are correct.
5. `mage-before/frames/frame-000120-3998ms.png` was captured but not inspected in this round (its log shows the normal menu start).
The EXE shows "The Boglands" for the row that `find_level_row` returns for 001_swamp (41, asserted by `menu_metadata_v1_tests` lines 60-61 and the words[9] string check at line 74). This closes survey open question Q5 for the stamping path.

## Needs from schema v4 (for other streams)
- Read difficulty with `character_current_difficulty(state)`, unlocked with `character_unlocked_difficulty(state)`. Skills stream: set `unlocked_difficulty` (0..2) on the profile when a difficulty is unlocked; the start path and skill caps read it.
- Quest stream: call `menu_metadata::stamp_menu_metadata(state, now, -1, act)` (or set `menu_metadata.current_act[difficulty]` directly) when Quest::SetState case 9 equivalent runs; the panel shows it.
- Quest stream: objective counters through the CQPG v2 API in `character_quest_blob.hpp` (`write_quest_counters` / `read_quest_counters`); not yet called by gameplay.
- Map stream: `menu_metadata::set_visited_module(state, level_uri, module_id, visited)`; `find_level_row` maps a level file to its LevelList row.
- Other streams must not change layouts; any new persisted field needs a schema bump owned by this stream.

## Package files required
None new. Runtime needs the existing package files `data/levels_pyarray.bin`, `data/levels_pyarraynames.bin`, `data/levels_pystructnames.bin` (LevelList) and the menu localization in the assets root, as in Preview 13.

## Verifier script
Jobs (quiet runner, `-Parallel 4`, all exit 0, about 5 s each):
- `knight-before`, `mage-before`: `--start-mode menu --menu-frames 120` on legacy v3 copies. Expect blank Act/location/difficulty/date rows.
- `knight-play`, `mage-play` (run as `k2` and `m2` folders; the original names gave an unexplained early exit): `--start-mode menu --menu-actions menu_MainMenu.btn_MENU_SINGLE_PLAYER|menu_StartGame.StartMenuButtons.btn_MENU_SINGLE_PLAYER --save-frame 40 --frames 80` on the legacy copy. Expect `Saved live checkpoint frame=40` in the log and a schema-4 slot file.
- `knight-after`, `mage-after`: menu route on the stamped copies. Expect the rows listed in Evidence 3 and 4.

## Open risks and gaps
- Not exercised in the EXE: non-combat F5 (code path compiled), pause profile save, menu-return commit (`commit_v1`), and checkpoint saves. Their stamp calls are the same `stampSaveMetadata` helper as the verified combat path.
- New-profile creation: stamped in `runtime_creation_persistence_v1` and unit-tested (row 41, act 1, date). Not driven through the Start-game creation UI in the EXE this round.
- `--save-now` (dev option, main.cpp ~L1306) writes without stamping. It is not a save point.
- Difficulty arrows (btnLeftArrowDiff / btnRightArrowDiff) are not drawn yet; `unlocked_difficulty` is 0 for all profiles, so only Normal starts.
- Act is always 1 until the quest stream calls the setter. Visited modules have no producer yet.
- Level-transition stamp (`Level::_LoadProcess` case 0x22) is not wired: Windows has no level transition yet. The row comes from the save-point level only.
- Schema 4 is not readable by the preview-13 EXE (`decode()` rejects schema > 4). Accepted by the user's decision 3.
- Wall-clock date (approved Q4): the reference shows 08/10 16:23 from the original clock, which cannot match.
- session_skill_binding fails in worktrees only (junction); ignored per the brief.
