# MENUMETA survey: main-menu character-slot metadata (read-only phase)

Scope: the PlayerInfos panel shown for the selected save slot on the main menu (name, class, level, act, location, difficulty, last save). No file under port/, docs/ or tools/ was edited. Scratch: `.local-inputs/claude-preview14/MENUMETA/` (jobs.json, summary.json, cap-knight/, cap-mage/, ref_*.png).

Summary: the panel exists in the Windows port and fills name, class and level. Act, location, difficulty and last-save date are blank in every case, because the portable slot file (`character.save`, CharacterState) stores none of them and the Windows projection clears them. Full fidelity needs (1) per-slot menu metadata written at the original save points, (2) a difficulty value that is not hardwired to Normal, and (3) wiring of the existing source projection code (`engine-ui/menu_save_slot_projection_v1`), which is currently unused by the Windows main.

---

## A. EXISTING

| Item | File(s) | Status |
|---|---|---|
| Slot panel text receivers (`menu_MainMenu/SlotText`, `menu_MainMenu/PlayerInfos/{player_class,Hud_Level,Hud_Act,Hud_Location,Last_Save,Last_Save_Infos,DifficultyTitle,Difficulty}/text`) | `port/windows-foundation/features/frontend/creation/dynamic_text_bindings.cpp` (`saved_profile_text_bindings`), `.hpp` (`SavedProfilePresentation`) | Component-tested. Integrated in main.cpp. Verified in EXE (see B5). |
| Windows projection lambda `project_selected_profile` | `port/windows-foundation/main.cpp` ~L651-668 | Integrated. Fills only character_id, class_token, class_label. Comment at L663-664 says act/location/difficulty/date are "not retained" by portable profiles. `current_act_known`, `difficulty_known` stay false, `localized_location` and `formatted_save_date` stay empty. |
| Portable character save (CharacterState, schema v3, magic DHSAVE, format version 1) | `port/windows-foundation/character_state.hpp/.cpp`, `save_store.cpp/.hpp` (`save_character`, `load_character`) | Integrated. Fields: id, name, class_id, stats (level, HP/MP, str/dex/end/energy...), points, inventory, equipment, skills, unlocks, skill slots, faery_by_difficulty (per-difficulty faery state, not CurrentDifficulty), source_quest_progress_cqpg blob. **No location, act, difficulty, unlocked difficulty or save date.** |
| Level-scoped save (GameSave, gameplay.save) | `port/windows-foundation/game_save.hpp/.cpp` | Integrated. Has `level_uri` and a full CharacterState copy. Single global file (`--game-save`, default gameplay.save), NOT per slot. |
| Source metadata reader (PlayerSavegame sections PNAM/PLVL/PCLS/PDFL/LNAM/LEPT/LUSP/QEST) | `port/game-data/menu_profile_metadata_v1.cpp/.hpp`, `player_savegame_v1.hpp` (SavedLocationV1: save_date, levels[3], seeds[3], current_acts[3], volatile_acts[3], entry_points[3], use_spawn_point[3]), `player_profile_index_v1` | Source-format only (original campaign cache layout). Component-tested. **Not used by Windows main** (no reference from main.cpp). The portable game does not read original campaign files for slots. |
| Source slot presentation builder | `port/engine-ui/menu_save_slot_projection_v1.cpp/.hpp` (`project_menu_save_slot_v1`, `menu_save_slot_local_date_v1`, `format_save_slot_local_date_v1` in save_slot_date_v1.hpp) | Source-only / component-tested (engine-ui/tests/menu_save_slot_projection_v1.cpp). Builds SwfFrontSaveSlotDetailsV1 incl. class+LEVEL label (GAMEPLAYMENUS_LEVEL), LevelList location (words[9]), date, act per volatile flag, difficulty override. **Not called by the Windows main.** |
| Original-AS slot setter writer | `port/engine-ui/swf_menu_save_slots.cpp` (`swf_front_write_save_slot_details_v1`, `swf_front_save_slot_details`) | Component-tested. Its occupied-slot branch `front_absent_profile` fails ("Existing campaign save requires PlayerSavegame loading"). Not called by Windows main. |
| Slot selection / occupancy / arrows | main.cpp L627-690 (`inspectProfileSlot`, `select_slot`, `remove_selected_slot`, `slotPath`) | Integrated, verified by slot tests. Slot 0 = `--save` path, slots 1-3 = `<stem>-slot-N<ext>`. |
| Source-indexed creation/assignment service | `port/windows-foundation/features/frontend/creation/source_indexed_slot_service_v1.*` | Component-tested. Creation/assignment only, not the metadata panel. Not in main.cpp. |
| Difficulty | main.cpp L123 `GameplayRewardContext::difficulty`, L517 `start_same_state` (accepts difficulty==0 only), L1912 `unlocked_difficulty=0` ("current gameplay start accepts Normal only") | Hardwired to Normal. No stored CurrentDifficulty in the portable save. |

Verified in the EXE (this survey): see B5 captures. The current panel is fed from the menu flow that already runs in preview-13-rc2 (`dh-foundation.exe` 2026-10 rc2 package).

---

## B. ORIGINAL BEHAVIOUR

### B1. What the original slot panel shows (IDA + SWF evidence)

IDA `NativeGetSaveSlotDetails` (0x44aa28, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0044/0044aa28.c`). Called by the SWF `getSlot` (droid SWF root/sprite510, offset 292352, per `port/windows-foundation/features/frontend/flow/source-evidence.md` L131/175). Behaviour:
1. Scans the 4 slots with `PlayerSavegame::SG_Exists`; occupied slots are listed first, then empty ones (index→slot mapping).
2. Optional 3rd AS argument = difficulty override; `-1` (none) → `v45 = PlayerSavegame::m_difficultyLevel` (CurrentDifficulty).
3. `PlayerSavegame(slot, 17, false)` loads the profile (PNAM, PLVL, PCLS, PDFL, LNAM, LEPT, LUSP, QEST).
4. Setters in order, with `s` = the SWF object:
   - `SlotID` (number) = slot index
   - `InUse` (bool)
   - `PlayerName` (string) = saved name
   - `PlayerClass` (string) = `StringManager::getString(CharacterTable row + 0x18 (StrID))`
   - `PlayerLVL` (number) = PLVL level (or -1 for empty)
   - `StringClassLVL` (string) = class label + " " + `StrID GAMEPLAYMENUS_LEVEL` + " " + level
   - `PlayerLocation` (string) = `StringManager::getString(Arrays::LevelList row[ location.levels[difficulty] ] + 36)` (i.e. the LevelList StrID at offset 0x24, the location name)
   - `PlayerLastSave` (string) = `strftime` of LNAM save date, by language: default `%m/%d     %H:%M` (cases 1,3,7 `%d/%m`; 2 `%d.%m.`; 4-6 `%m.%d.`)
   - `PlayerCurrentAct` (number) = QEST act for that difficulty (`SG_GetQuestSG()+4*diff+68`)
   - `Difficulty` (number) = difficulty index (0 Normal, 1 Hard, 2 Heroic)
   - `DifficultyUnlocked` (number) = unlocked difficulty (v75)
5. Empty slot: InUse=false, name/class/location/date empty, PlayerLVL -1, Act 1.

The SWF (authored actions in `.local-inputs/publication/checkpoint/session-contributions/menu-launch/evidence/current/main-menu-authored-actions.json`) maps these onto text receivers: `player_class` ← PlayerClass, `Hud_Location` ← PlayerLocation, `Hud_Act` ← `NativeGetParsedString("MENU_ACT")` with PlayerCurrentAct (menu.english `Act ^d`), `Last_Save` ← `MENU_LAST_SAVE` label, `Last_Save_Infos` ← PlayerLastSave, `Hud_Level` ← `GLOBAL_LEVEL` + " " + PlayerLVL, `DifficultyTitle` / `Difficulty` ← GAMEPLAYMENUS_DIFFICULTY_NORMAL / _HARD / _VERYHARD (gameplaymenus.english: Normal, Hard, Heroic). Also `btnDelete` (the red X), `btnLeftArrow` / `btnRightArrow` (slot switch), `btnLeftArrowDiff` / `btnRightArrowDiff` (difficulty switch, gated by UnlockedDiff).

Localization text, English: "Act ^d" (menu.symbols MENU_ACT), "Last Save" (MENU_LAST_SAVE), "LEVEL 1" style, "Difficulty:" / "Normal" / "Hard" / "Heroic" (gameplaymenus.english L862-866). Port uses literal "Act N", literal "Last Save", and hardcoded "Normal/Hard/Heroic": equal in English, not template-driven.

### B2. Where the values are written in the original (save points)

Save-date / profile writers (IDA):
- `Level::SG_SavePlayer` (0x3efa54): `SG_SetPlayerLevel` (PLVL) → `SG_SetSaveDate` → `SG_SetLevelEntryPoint(this+272, -1)` → `Character::SG_Save`.
  Callers: `Application::Pause` (0x322924, only when current level type ==38 and flag +324 set; meaning of type 38 unknown), `Level::SG_SaveLocalPlayer` (0x3f07b0), `Level::CheckpointSave` (0x3f04b4), `Level::_LoadProcess` (0x3f6990, around L179611 Checkpoint save during loading).
- `CheckpointZone::OnCollisionBegins` (0x395f04, L111122) → `Level::CheckpointSave` → `Character::SG_SaveCheckpoint` + `LevelSavegame::SaveCheckPoint`. That is the in-level checkpoint trigger, the closest original equivalent of "F5 checkpoint".
- Level location: `Level::_LoadProcess` case 0x22 (L179518): `Character::SG_SetLevelId(player, this->level_id, -1)` writes LevelList index into `levels[CurrentDifficulty]` (word index 20+diff). So the location is set when a level loads, per difficulty.
- New game `MenuBase::FS_StartGame` (0x4220a0, L208854-208893): `SG_SetSaveDate`, writes location word 20+diff = **41** (initial LevelList row for the first map), sets use-spawn word, `PlayerSavegame::SG_Save`. Inference: LevelList 41 = data/scene/001_swamp.mlx (The Boglands). Not yet decoded (see G9/open items).
- Act: `Quest::SetState` case 9 (0x480c78, L273222): `Character::SG_SetCurrentAct(player, this->act, -1)` → QEST word per difficulty (+64 setter; +68 read by NativeGetSaveSlotDetails). So Act is set by act-quest state transitions, not by level load.
- Difficulty: `PlayerSavegame::m_difficultyLevel` set by `NativeStartGame` (0x43e0d0, L227870) and `FS_StartGame`. Stored as PDFL.
- Menu save: `NativeStartGame` (0x43e0d0, L227870-227906) calls `SG_Save` after setting difficulty.

### B3. Reference video (Dungeon Hunter 2 v1.0.3 Part 1, `.local-inputs/reference-video/dh2-act1/...mp4`; contact sheet `C:/Users/adamc/Desktop/dh2_video_research/sheets1/s003.jpg`)

- 0:48-1:01: Knight/Mage class select ("Choose a class", Warrior/Mage, Confirm). Observed.
- 1:04 (contact sheet) and 1:05 (full-res frame, `ref_full65.png`, observed): main menu with existing profile. Panel, top to bottom:
  - `CRISR82` (name)
  - `Warrior` (class, small)
  - `LEVEL 1`
  - `Act 1`
  - `The Boglands` (location)
  - `Difficulty:` / `Normal` (two lines)
  - red X (btnDelete) + `Last Save` / `08/10   16:23` (date, `%m/%d     %H:%M`)
  - `>` arrow beside the name (slot switch).
  - Buttons: Start game, Options, MORE GAMES!, info icon; power button bottom-left.
- 1:08: LOADING screen (after Start game). Observed.
- Not seen in this video window: difficulty arrows (none drawn, consistent with UnlockedDiff=0 at this point). Whether arrows appear for Hard/Heroic is not verified (video Part 1 only, act 1).

Inference (not observed): slot list for a second occupied profile, and the panel while the difficulty arrows are shown.

### B4. Localized location name chain (LevelList)

- LevelList table: `levels_pyarray.bin` (schema `bSibiSiiiiiiiii`, 72-byte rows, `port/game-data/level_tables.hpp`, LevelRecord{scalar.words[18], description, file}). 51 rows. Source names for swamp: `001_swamp.mlx`.
- Location StrID = `words[9]` (offset 0x24). Localized string: `LOCATIONS_LOC_BOGLANDS` → "The Boglands" (`assets/original-cache/data/text/locations.english`, `locations.symbols`).
- Same text file also has "Gothicus Darkwood - To Boglands" (a Darkwood exit name); this is a different string key, so it is not the location name to use.
- Windows main already has the table: `sourceOwner.source().properties->levels` (populated by `level-world/character_game_design.cpp` L106 `load_levels`), and `profileLocalization` (MenuLocalization) for StrID lookup (main.cpp L624-625, `string_id`).
- Not verified: the exact row index for 001_swamp (expected 41 from FS_StartGame) and that its words[9] equals the LOCATIONS_LOC_BOGLANDS StrID. A quick decode via `load_levels` (see `level-world/tests/character_host_context.cpp` L49 for the file-loading pattern) would confirm it.

### B5. What the Windows port shows now (observed in EXE)

Quiet batch (`port/windows-foundation/tools/quiet_run.ps1`, `-Parallel 2`, both exit 0, 5 s each), EXE `.local-inputs/windows-source-clock-v19-preview-13-rc2/dh-foundation.exe`, `--start-mode menu` via a copy of its `startup.args` with absolute asset/save paths, `--menu-capture-directory`, `--menu-frames 120`:
- `cap-knight/menu-120.png` (save = b016-knight-bag-longsword02, name QA, KnightPlayerBase): panel shows `QA` / `WARRIOR` / `LEVEL 1`, then **four empty rows** (Act, location, difficulty title + value, last save), red X at bottom. Observed.
- `cap-mage/menu-120.png` (save = class-mage-copy, name QA, MagePlayerBase): `QA` / `Mage` / `LEVEL 1`, same empty rows. Observed. Class label and model switch correctly.
- Log line: `{"frames":120,"menu":"menu_MainMenu",...,"saved_user_profiles":false,...}` and `"menu":"menu_MainMenu","slot":0,...`.

Side differences visible in the same captures (outside this stream, for the owner of main-menu buttons): Windows buttons are Start game / Options / Info (no MORE GAMES, no power button); the original shows MORE GAMES! and the info icon plus power. The panel frame is at the same place.

### B6. StartGame page (reported by the user, preview10 evidence)

The StartGame page was not captured in this survey. Supplied screenshot `C:/Users/adamc/AppData/Local/Temp/codex-clipboard-f5dde03f-...png` (viewed): shows `Warrior` / `Warrior` (name row shows class label twice). `codex-clipboard-e11e3cae-...png` (viewed): `BOBGRATT` / `Warrior` on the class-select preview, which is the authored StartGame title layout. These are the authored Warrior placeholders the notes describe (`flow/source-evidence.md` L135, L175: "must be explicitly cleared rather than inherit authored Warrior placeholders").

---

## C. GAPS (each with evidence)

G1. **No act/location/difficulty/date in the slot file.** `character_state.hpp` (CharacterState fields) and `save_store.cpp` encode/decode (`encode()` L105-~190) have no such fields. Evidence: main.cpp L663-664 comment; `dynamic_text_bindings.hpp` `current_act_known=false`, `difficulty_known=false`. Observed: B5 empty rows.

G2. **Windows projection clears the rows.** main.cpp L651-668 sets only character_id/class_token/class_label. `dynamic_text_bindings.cpp` L~101-105 pushes `Hud_Act` "" , `Hud_Location` "", `Last_Save` "" and `Last_Save_Infos` "", `DifficultyTitle` "", `Difficulty` "". Observed in B5.

G3. **Difficulty hardwired to Normal.** main.cpp L517 (`difficulty!=0` rejected), L1912 (`unlocked_difficulty=0`), L123 (GameplayRewardContext difficulty), L117 comment. No CurrentDifficulty in CharacterState. Original: CurrentDifficulty is global, stored in PDFL, selected in the menu (DiffSlot arrows), and drives the location/act index (B1 step 2, B2).

G4. **Location is not per slot.** Only the global `gameplay.save` (GameSave.level_uri, `game_save.cpp`) knows the map, and it is shared by all slots. Slot 1-3 cannot show their map. Original: per slot and per difficulty (`levels[3]` in LNAM).

G5. **No save date at any save point.** Grep of save_character call sites in main.cpp: L1283 (`--save-now`), L1947 (skill-point menu), L2458 (pause profile-only save), L2526 (F5 non-combat); menu_return `commit_v1` (`features/frontend/menu_return/menu_return_v1.cpp` L91) also calls save_character. None record a time. Original: `SG_SetSaveDate` (time_t) at SG_SavePlayer, FS_StartGame and checkpoint saves (B2).

G6. **F5 in gameplay does not refresh the slot file.** main.cpp L2521-2524: when `combatSession` is active, F5 writes only `options.liveSave` (gameplay.save, with a character copy), not `options.save` (character.save). The slot panel is only refreshed by the menu-return commit (L2443) or the pause profile-only save (L2458). Original: checkpoint saves call `SG_SavePlayer` (B2), which writes the profile (player's slot file). So a panel after F5 + quit would show stale stats/date. (Inference on the exact original behaviour for F5: the port's F5 maps to the checkpoint.)

G7. **Source projection exists but is not used.** `engine-ui/menu_save_slot_projection_v1.cpp` `project_menu_save_slot_v1` (L22-58) implements class label+LEVEL, location via words[9], date via `format_save_slot_local_date_v1`, act via `volatile_acts ? ... : current_acts`, and difficulty override. Its input type `MenuProfileMetadataV1` is produced only by the source-format reader (`game-data/menu_profile_metadata_v1`), which reads original campaign files, not the portable DHSAVE slot. Two parallel presentation paths exist and only the portable one is wired.

G8. **Literal strings instead of localization templates.** `Hud_Act` = literal "Act N" (MENU_ACT is "Act ^d"), `Last Save` literal, difficulty names hardcoded. Correct in English only (`dynamic_text_bindings.cpp` L~110-125). Low priority, but do it with the fix (use MenuLocalization for MENU_ACT, MENU_LAST_SAVE, GAMEPLAYMENUS_DIFFICULTY_*).

G9. **New profile does not record initial location.** `select_new_profile` (main.cpp ~L499-507) creates CharacterState with no location. Original FS_StartGame: location word = 41 for the difficulty, act 1, save date = now. Needs the LevelList row for 001_swamp confirmed (B4, not verified).

G10. **Act is not tracked.** Act is set by quest state transitions (Quest::SetState case 9, B2). The Windows port has `source_quest_progress_cqpg` (opaque blob) and no act value. The quest stream (coded quest system) owns this. Current Windows code has no SG_SetCurrentAct equivalent.

G11. **Difficulty arrows / unlock not shown.** The original shows DiffSlot arrows gated by UnlockedDiff (btnLeftArrowDiff/btnRightArrowDiff). The Windows panel has no difficulty switch; `DifficultyUnlocked` is never set. Depends on the skills/stats stream (max skill level per unlocked difficulty, main.cpp L1912-1966).

G12. **Legacy saves cannot be fixed by the save-point change alone.** Existing `character.save` files (schema v1-v3) carry no metadata. Until a save point rewrites them, the panel cannot know the map/act/date. (Design choice, see G-open questions.)

---

## D. DESIGN (minimal, reusable)

### D1. Data model
Add to `CharacterState` (character_state.hpp), one block, append-only save schema bump to 4:
```
struct CharacterMenuMetadata {          // source PlayerSavegame menu fields, portable form
    bool known = false;                  // false for legacy v1-v3 saves: panel omits fields
    std::uint64_t save_unix_time = 0;    // source SG_SetSaveDate (time_t); 0 = unknown
    std::int32_t current_difficulty = 0; // source CurrentDifficulty / PDFL, 0..2
    std::int32_t unlocked_difficulty = 0;// source UnlockedDiff, 0..2 (skills stream owns the value)
    std::array<std::int32_t,3> level_row{-1,-1,-1};  // LevelList row per difficulty (SG_SetLevelId)
    std::array<std::int32_t,3> current_act{1,1,1};   // QEST act per difficulty (quest stream)
};
```
Keep `CharacterState::source_menu_metadata` (new field) and make `make_default_character` set known=false. A fresh profile sets known=true with level_row[0]=41, act 1, date=now (matches FS_StartGame).

Level identity: store the LevelList row index (source-faithful). The Windows GameSave uses `level_uri` (file path). Map via `LevelTables.levels[row].file` (e.g. "data/scene/001_swamp.mlx") at save time; both fields are kept consistent by the single writer.

### D2. Owners read/write
- Writers (one helper, e.g. `stamp_menu_metadata(CharacterState&, level_uri, difficulty, act, now)` in a new `features/frontend/menu_metadata/` file, or in `character_state.cpp`): called by the save points below before `save_character`.
- Readers: `project_selected_profile` (main.cpp L651-668) via `saved_profile_text_bindings`; uses `sourceOwner.source().properties->levels`, `profileLocalization.string_id(levels.levels[row].scalar.words[9])`, and `format_save_slot_local_date_v1` (engine-ui/save_slot_date_v1.hpp) with the language.
- Difficulty owner: the DiffSlot selection (start_same_state, main.cpp L516-520) writes `current_difficulty` into the profile before starting. Start path must accept 0..unlocked.
- Act owner: quest stream calls `set_current_act(state, difficulty, act)` on Quest::SetState equivalent.

### D3. Save points to stamp (mirror of B2)
1. Menu return `commit_v1` (`features/frontend/menu_return/menu_return_v1.cpp` ~L82-91): stamp date, row from the live level, difficulty, act. Writes character.save.
2. Pause profile-only save (main.cpp ~L2452-2458).
3. F5 in combat (main.cpp ~L2521-2524): ALSO write `options.save` (character.save) with the metadata (closes G6).
4. F5 non-combat (main.cpp ~L2526) already writes; add stamp.
5. New profile creation (main.cpp `select_new_profile` ~L497-507 and creation persistence): row 41, act 1, date now, difficulty 0, known=true.
6. Level transitions (no transition in Windows yet): stamp row when a level loads (the source `_LoadProcess` case 0x22 point). Keep as an API hook, do not invent a transition.

### D4. Persistence / versioning
- `save_store.cpp` encode: `character_schema_version` 3→4; write the block after `source_quest_progress_cqpg` (current last field). Decode: `if(serialized_schema>=4)` read it; else leave defaults (known=false). Keep the file magic/format version 1.
- Existing v1-v3 saves load unchanged (known=false). They become known after their next save point.
- Forward incompatibility: `decode()` throws for schema > character_schema_version, so the preview-13 EXE cannot read a schema-4 file. Consider keeping a schema-3 writer until the user accepts (open question Q2).
- Tests to add: round trip v4; decode of v3 fixture (known=false, panel omits); decode of v4 with difficulty=2 and act=3; reject row=-1 when known=true (only if invalid); date=0 when known=true → error.

### D5. Integration hunks (main.cpp / CMake)
- main.cpp ~L651-668 `project_selected_profile`: replace the comment and set `value.current_act/current_act_known`, `localized_location` (from row+StrID), `difficulty/difficulty_known`, `formatted_save_date` when `known`. Keep `classLabel` path.
- main.cpp ~L516-520 `start_same_state`: accept `difficulty` 0..unlocked, store to profile.
- main.cpp ~L1912 `skillCaps.unlocked_difficulty`: read from profile (skills stream).
- main.cpp ~L497-507 `select_new_profile`: call the stamp helper.
- main.cpp ~L2458 and ~L2521-2526 (F5 combat branch: add save_character(options.save,...)).
- main.cpp ~L2443 (commit path) already calls `commit_v1`; the stamp goes into `menu_return_v1.cpp` ~L80-91.
- `dynamic_text_bindings.cpp` ~L101-127: no change needed if fields are set (it already emits empty when unknown).
- CMakeLists: only if a new file is added (e.g. `features/frontend/menu_metadata/menu_metadata_v1.cpp`); tests in `features/frontend/creation/run_*` style.

---

## E. WORK BREAKDOWN (2-5 tasks, dependency order)

T1 (S, <2h). Data model + codec. Add `CharacterMenuMetadata` and schema v4 encode/decode in `character_state.*`, `save_store.cpp`. Tests: round trip, v3 fixture, validation. Verifier: `ctest`-style runner, plus a byte check that a v3 `b016` fixture still loads.

T2 (M, <half day). Save-point stamping. Implement the stamp helper; wire menu return (`menu_return_v1.cpp`), pause save, F5 combat (writes character.save), new-profile creation (row 41, act 1, date now). Tests: isolated save folders; after each action read back the slot file. Verifier (quiet batch): `--start-mode menu` for a fresh profile, expect `The Boglands` / `Act 1` / `Normal` / date in the panel.

T3 (S, <2h). Projection wiring. `project_selected_profile` fills act/location/difficulty/date from the metadata with MenuLocalization (MENU_ACT, MENU_LAST_SAVE, GAMEPLAYMENUS_DIFFICULTY_*). Prerequisite: confirm row 41 and words[9] (B4). Tests: `dynamic_text_bindings` unit test for known/unknown rows. Verifier: quiet batch with 3 fixtures (fresh=Normal/Act1/Boglands; hand-edited Hard/row X; legacy v3 = blank), crops compared with `ref_full65.png` layout.

T4 (M, depends on quests and skills streams). Act and difficulty source. Quest-state hook for current act (Quest::SetState case 9 equivalent), DiffSlot selection and unlock gating (btnLeftArrowDiff/Right, UnlockedDiff). Tests: act change after quest transition shows in the panel; Hard profile starts Hard; unlock 0 hides arrows.

T5 (S, optional). Source-format alignment: call `project_menu_save_slot_v1` from the source path or delete the duplicate once T3 is in; keep MenuProfileMetadataV1 only for original-campaign import.

---

## F. DEPENDENCIES / CONFLICTS

- **Skills/stats**: owns `unlocked_difficulty` (main.cpp L1912-1966 `skillCaps`) and the CharacterState points. T4 needs their value. Shared file: `character_state.hpp` (both add fields). Coordinate the schema bump so only one stream bumps the version.
- **Faery**: `faery_by_difficulty` already per difficulty in CharacterState (schema v2). Shares the `current_difficulty` concept; the faery page per-difficulty reads should use the same value. No change to the panel.
- **Quests**: owns act (T4 hook, G10) and the QEST/quest blob (`source_quest_progress_cqpg`). Act is set by Quest::SetState, so the quest stream must expose a setter.
- **Map**: owns the LevelList/level row to map name and the HUD minimap. Uses the same LevelTables and words[9] StrID (T3). Coordinate the row→file map helper.
- **Equipment, drops**: no direct overlap, but any save-point change in main.cpp (F5 branch L2521-2524, menu return) is also touched by the drops/pickup save path. Serialize edits to main.cpp.
- **Main-menu buttons (MORE GAMES, info, power)**: not this stream, noted in B5.
- **Shared files**: `character_state.hpp/.cpp`, `save_store.cpp`, `main.cpp` (L497-520, L651-668, L1912, L2443-2458, L2521-2526), `menu_return_v1.cpp`, `dynamic_text_bindings.cpp`.

---

## G. OPEN QUESTIONS (cannot decide from evidence)

Q1. Legacy saves (v1-v3, no metadata): show blank until the next save point (recommended, no fabrication), or migrate to the fresh-profile defaults (Boglands / Act 1 / Normal, no date)? The user must decide: the second shows a location that may be wrong for an older profile.
Q2. Forward compatibility: schema-4 saves are unreadable by the preview-13 EXE (`decode()` throws). Accept (ship with preview 14 only), or keep writing schema 3 and store the block in a sidecar file?
Q3. F5 in combat: the original checkpoint saves the profile (SG_SavePlayer). Should the port's F5 also rewrite character.save with the metadata (my recommendation, G6), or keep F5 as gameplay.save only?
Q4. Date: store the real local wall-clock time at each save (matches original `time(NULL)`), so the panel shows e.g. `10/10     16:23` in the port's clock. Confirm that wall-clock is acceptable (no test fixture can match 08/10 16:23 in the video).
Q5. Unverified: 001_swamp LevelList row (41?) and its words[9] StrID = LOCATIONS_LOC_BOGLANDS. Needs a decode of `levels_pyarray.bin` (one quick test). Not done in this survey (no file edits allowed).
Q6. Video version difference: the reference video is v1.0.3, the recovered code is v1.0.2 (COMMON-BRIEF). Act/location text is the same in both, to be confirmed on a second save.
