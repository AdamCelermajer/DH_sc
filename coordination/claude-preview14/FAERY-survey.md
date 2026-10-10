# FAERY survey (Preview 14, read-only)

Scope: the Faery character-menu page (unlock, selection, levels, abilities, details), tied to CharacterState. Companion AI is out of scope and listed only as a dependency. No files under port/, docs/ or tools/ were edited. The EXE was not run in this survey; runtime facts below come from existing Preview 13 reports.

## A. EXISTING

Status key: SRC = source only, CT = component-tested (own runner), INT = integrated in main.cpp, EXE = verified in EXE (with source).

| Item | File(s) | Purpose | Status |
|---|---|---|---|
| CharacterState faery fields | `port/windows-foundation/character_state.hpp` 53-60, 87-90 | `CharacterFaeryProgress{state u8, level u16}`, `CharacterFaeryDifficulty{current_faery, faeries[5]}`, `faery_by_difficulty[3]`, `source_faery_state_known`, `source_faery_list_id` | SRC; validated in `character_state.cpp` 141-146 (current_faery in 0..4) |
| Save persistence | `save_store.cpp` 139-145, 199-206; `game_save.cpp` 85-95 (schema>=2) | Writes/reads current_faery and 5x(state,level) per difficulty | SRC; round-trip used by Preview 12/13 saves |
| Creation init | `features/frontend/creation/runtime_creation_persistence_v1.cpp` 322-323, 363-366 | Fresh character: `current_faery=0`, all `faeries[f]={}` (state 0, level 0) for all 3 difficulties | SRC |
| Faery tables | `port/game-data/faery_tables.{hpp,cpp}` | Immutable owner of original FaeryList and Faery rows (36-byte projection + script name) | CT (game-data tests) |
| Faery page: CharacterState provider | `features/faery_menu/character_state_page_v1.{hpp,cpp}` (292 lines) | Presents 5 slots from CharacterState + tables: unlock byte==1 via `source_faery_knowledge_v1`, level, names/desc symbols, `select_character_state_faery_v1`, `bind_character_state_faery_provider_v1` (SourcePageProviderV1) | SRC. Tests exist (`features/faery_menu/tests/character_state_page_tests.cpp`) but NO runner in tree references them; not in CMake. Not built, not run |
| Faery page: legacy Save provider | `features/faery_menu/faery_menu.{hpp,cpp}`, `session_faery_page_v1.*`, `native_binding_v1.*` | Same page over `dh2::data::PlayerSavegameV1` and a Session host (`bind_session_faery_page_provider_v1`) | SRC; not in CMake; not run. Reads the legacy save, not CharacterState, so it is NOT the path to bind |
| Faery art | `features/faery_menu/original_art.cpp`, `source_layout.json` | SWF geometry for menu_FaerySheet (480x320), five slot contours, img_Faery frames | SRC; not in CMake. Character-menu art for the Faery tab (`character_menu/original_art.cpp`, art3) deliberately excludes menu_FaerySheet (test `character_menu_tests.cpp` 33-35) |
| Character menu routing | `features/character_menu/character_menu.{hpp,cpp}`, `source_composition.hpp` 49, 61-64, 298-299 | Tab::faery and Action::faery; `SourceCompositionV1::select(Tab::faery)` fails when no provider is registered | CT (`character_menu_tests.cpp` 69-75 tests the release route; `source_composition_tests.cpp` 43-45 tests unbound Faery) |
| Faery stub in main.cpp | `port/windows-foundation/main.cpp` ~2401-2406 | On Action::faery prints `Character menu Faery page requires live content provider; current page retained` and does nothing | INT (stub) |
| No Faery register_page in main.cpp | `main.cpp` ~1804-1994 (Equipment and Skills register; Faery does not) | Faery provider never registered | INT missing |
| Celest cast (key 4) | `features/faery_menu/celest_cast_v1.*`, `celest_source_use_v1.*`; main.cpp ~2647-2685 (Source Faery key=4 arm) | Spell path for faery_celest (slot 0), MP debit via source cast | EXE (Preview 13 H/I/verify-ui reports: MP 30.25 -> 20.25, slot 0) |
| Hotty cast | `features/faery_menu/hotty_cast_v1.*`, `hotty_character_cast_v1.*`, `hotty_session_cast_v1.cpp`, `hotty_effects_v1.*`, `hotty_source_use_v1.*` | faerie_hotty spell (see section B for which slot) | CT (`run_hotty_cast_v1_tests.ps1`); the session runner had a link error in P13 (I-report line 91); not EXE-verified as a key-4 cast |
| Rocky / Wetty / Windy spells | none in port code | Script rows exist in the tables (`faerie_rocky`, `faerie_wetty`, `faerie_windy`, and `*_mage` variants) | NOT IMPLEMENTED |
| HUD key-4 icon | main.cpp 2058-2066; `features/generic_skills/pc_gameplay_hud_v1.cpp` 232-245, 280-288 | `layout.active_faery_id = state.faery_by_difficulty[0].current_faery` (B002/B024) | INT + EXE (verify-ui-report: gold Faery icon at key 4 in WIP; "4 Faery" label) |
| Source Faery tables loaded | main.cpp 766-767, 840-843 | Loads faeries_pyarray/names/structnames.bin into `sourceFaeryTables` | INT |
| Cast cooldown clock | main.cpp 990-992, 2509, 2559 | `faeryCooldownClock` for Faery casts | INT |
| Companion faery ops (dependency only) | `features/companions/source_companions.{hpp,cpp}` 21, 197-231 | Operation current_faery_id / change_faery / faery_association used by companion master logic | SRC; companion AI not in scope |

Bottom line for A: all data and page logic for the CharacterState-tied page exists in source, and the only missing link is the registration in main.cpp plus CMake entries. The page has never been compiled or run. The on-screen Faery tab still shows the stub. P13 H-report (`coordination/claude-preview13/H-report.md` lines 39, 94) and the verify-ui report confirm the stub line at frame 80 in the EXE.

## B. ORIGINAL BEHAVIOUR

### B1. Storage and functions (IDA, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`)

- `NativeHUDGetActiveFaery` 0x44a820 (function header at pseudocode-all.c:234786 per the P13 H-report; registered at 217743) returns `Character::SG_GetCurrentFaerieId(v, -1)`.
- `Character::SG_GetCurrentFaerieId` 0x3bb98c (line 136571): returns `save->current_faery[PlayerSavegame::m_difficultyLevel]`. The `-1` argument means the CURRENT difficulty, not difficulty 0. The port reads `faery_by_difficulty[0]` always (see C).
- `PlayerSavegame::SG_SetFaerieState` 0x466588 (line 252626): writes one byte at the faery row (state).
- `PlayerSavegame::SG_SetFaerieLevel` 0x46663c (line 252651): writes a WORD at row+2 (level). Row stride is 4 bytes, so the disk layout matches the port's `state u8, level u16` pair.
- `PlayerSavegame::SG_IsFaerieUnlocked` (no address captured; pseudocode-all.c:253618): if the DebugSwitch `UnlockAllFaeries` is on, returns true; otherwise `state == 1` (exact compare).
- `NativeHUDGetIsFaeryUnlocked` (native, body at pseudocode-all.c ~228350-228380; registered at 217739): calls `SG_IsFaerieUnlocked(player, slot, -1)` and returns a bool to AS. The SWF calls it with slot 0..4 (authored actions, see B4).
- `NativeHUDSetActiveFaery` 0x43ee40 (body at pseudocode-all.c ~228385-228420; registered 217735): calls `Character::ChangeFaery(player, slot)`, then reloads the level/HUD of the current level.
- `Character::ChangeFaery` (line 128104): asserts `slot < SG_GetFaerieCount(diff)`, calls `SG_SetCurrentFaerie`, `CharAI::UpdateAllSkills`, and places the visual of the new Faery (CharModel name lookup). It does NOT test unlock state (observation from IDA; no SG_IsFaerieUnlocked call in this body). The UI button is the gate (inference, see G).
- `Character::SetFaeryState` 0x3ae7d4 (line 128044): writes state. For slot != 0 and state==1 when the save's tutorial flag is set and difficulty==0, it starts script `cinematic_Tuto_faery` once, then clears the flag. This is the first-unlock tutorial.
- `Character::IncFaeryLevel` 0x3ae61c (line 127983): level += 1, `UpdateAllSkills`, and when every faery's level is nonzero it unlocks trophy `faery_charged`.
- `Character::IncAllFaeriesLevel` 0x3ae94c (line 128082): for each faery sets state=1 then IncFaeryLevel. Callers: debug menu cases (lines 33552, 33602) only.

### B2. Who unlocks and levels faeries (script commands)

- `Script_SetFaeryState::Execute` (0x45f6c8, line 248090) calls `SetFaeryState(local player, arg8=faery slot, arg12=state)`.
- `Script_IncFaeryLevel::Execute` (0x45f5cc, line 248060) calls `IncFaeryLevel(local player, arg8=slot)`.
- Campaign data (`.local-inputs/claude-preview13/h/pkg/assets/original-campaign.xml`, one export): the ONLY SetFaeryState command is in `script id=30 name="Swamp_Intro"` (command index 156, line 3005): slot 0, state 1. So the Celest slot (0) is unlocked by the Swamp intro script. Observation from the file; inference that this is Act 1 Celest unlock.
- The same export has ZERO `Script_IncFaeryLevel` commands (grep count 0). Either Act 1 level scripts that level up faeries are not in this export, or they are absent. Open question G1.
- Because slot 0 is never set by an IncFaeryLevel in this export, the level rule is: level starts at 0 for each faery, and Celest's level 1 is only reached through a later script (unknown).
- Fresh creation (port `runtime_creation_persistence_v1.cpp` 363-366) zeroes all rows, so a new character has NO unlocked faery until Swamp_Intro runs. Inference from IDA plus the script export; not yet verified in an EXE run.

### B3. Selection and cast effects

- Selection: `NativeHUDSetActiveFaery` -> `ChangeFaery` -> writes `current_faery` only. No level change, no cost.
- Casting uses the current faery's table row and its saved level: Celest `min(level,1)` (celest_cast_v1.cpp 71; celest_source_use_v1.cpp 110), Hotty `saved_level` at hotty_cast_v1.cpp 139 and 381. The port's key-4 arm reads `faeries[selected].level` (main.cpp 2672) and picks 600 vs 800 range from it.
- `UpdateAllSkills` after both ChangeFaery and IncFaeryLevel: faery changes affect skill ownership/animation. Dependency on the skills stream.

### B4. SWF menu (authored actions, `port/engine-ui/reference/character-menu-flow-v1/authored-actions.txt`)

- Page root: `menu_FaerySheet` (pushed by `NativePushMenu` at line 3261). Title `GAMEPLAYMENUS_FAERIES_TITLE`, and `btnFaeriesTab` tab icon (character menu).
- Five buttons: `btn_GAMEPLAYMENUS_FAERY_1..5` (lines 10592-10705). Each button gets `GetFaeryIsUnlocked` (lines 10626-10684: `NativeHUDGetIsFaeryUnlocked(slot)` with slot 0..4) and sets Focused/locked visual.
- Slot identity from the authored img names: `img_Faery` frames in `source_layout.json` 184-216: Celest=frame 0 (slot 0), Hotty=frame 1, Rocky=frame 2, Wetty=frame 3, Windy=frame 4. Slot order is inferred from frame order and the Celest slot-0 fact (observed). Treat Hotty=slot 1 as inference.
- Selected display: `GetActiveFaery` -> `NativeHUDGetActiveFaery` into `objFaery.FaeryIdNum` (lines 10711-10731). `FaeryUpgraded` is an authored flag set on the object; it selects the upgraded text variant (inference: the `_B` symbols).
- Detail text: `FaeryNameText`, `faery_desc` (htmlText) from `NativeGetStringFromSymbol` with symbols `GAMEPLAYMENUS_FAERY_1..4`, `_DESC`, `_SPELL`, and `_1B/_2B/_4B` (the B variants are the upgraded text; inference). Names in the SWF: Celest (1), Hotty (2), Rocky, Wetty, Windy (lines 10737-10890).
- Select: a button click calls `NativeHUDSetActiveFaery(slot)` (ChangeFaery). I did not find a separate "Upgrade" button in the authored actions. Level-ups are script-only (B2). FaeryUpgraded is a display flag.

### B5. Faery table (`.local-inputs/claude-preview13/h/pkg/assets/data/pydata/faeries_pyarray*.bin`, `faeries_pystructnames.bin`)

- Lists: `DEFAULT`, `Knight`, `Mage`, `Rogue` (plus `AAA_DONT_DELETE`). Names include `Celest`, `Hotty`, `Rocky`, `Wetty`, `Windy`, `Fake_*`, and `Mage_*`.
- Row scripts: `faerie_celest`, `faerie_hotty`, `faerie_celest_mage`, `faerie_hotty_mage`, `faerie_rocky`, `faerie_wetty`, `faerie_windy`, `faerie_rocky_mage`, `faerie_wetty_mage`, `faerie_windy_mage`.
- Struct fields: Description, Elemental, ModelFile, Name, SpellScript, SpellType, Type, List. The port's `FaeryRecord` holds the 9-word projection and script string.
- Which row each class's list uses is NOT verified (inference: DEFAULT = the 5 menu slots; Knight/Mage/Rogue lists choose Fake_/Mage_ variants). Open question G4.

### B6. Reference video (Part 2, `dh2_video_research/video/part2_cIAW38IfLxY.mp4`, 15:16 long, 640x360 vp9 30fps; sheets2 = 6 frames every 4 s)

Observed directly (frames looked at):
- Fully scanned: s030-s039 (11:36 to 15:16, contiguous). Sampled (one sheet per area): s002, s005, s010, s016, s020, s022, s025, s028. NOT looked at: s001, s003-s004, s006-s009, s011-s015, s017-s019, s021, s023-s024, s026-s027, s029.
- 1:36-1:56 (s005): first tutorial (melee/walking attack). 3:36-3:56 (s010): Celeste on the bridge with Rene and Crisr82 (story; Celeste is named as a character). 7:40 (s020): "New Quest". 8:24-8:44 (s022): Map tab (Gothicus Darkwood). 10:56 (s028): Map tab. 12:00-12:20 (s031): Quest Journal tab (Buried Alive / The Hidden Path / Trial of Heroes I). 12:40-13:20 (s032-s033): Skills tab (Class/Skill Mapping, Upgrade Skill, Skill points left). 13:20 (s034): Stats tab. 13:24-13:32 (s034): Equipment tab. 13:36 (s035): Equipment (Auto-equip All). 14:00-14:20 (s036): Map / Quest Journal. 14:56 (s038): Map tab. 15:08 (s038): world map popup. 15:12 (s039): loading screen, end of video.
- Tab bar order (observed in 12:00-14:20 frames): heart (Stats), bag/gear (Equipment), crossed swords (Skills), BUTTERFLY (the 4th icon = Faery), scroll (Quest Journal), map (World/Gothicus Darkwood). The butterfly tab was NOT seen selected in any sampled frame.
- Conclusion: no Faery page frame was observed in Part 2 (sampled as above). The Faery page is not in the 11:36-15:16 section, which is contiguous. A complete check of the unlooked sheets (s001, s003-s004, s006-s009, s011-s015, s017-s019, s021, s023-s024, s026-s027, s029) is still needed. Open question G5.
- The butterfly icon at the 4th tab position matches the brief's click coordinates (Faery at authored x=349 in 480-wide space) and the character-menu hit test. This is an observation of the icon, not of the page content.

### B7. Visual facts still needed

- Layout of the five slots, unlocked vs locked (SWF `Focused`/`locked` visuals), selected slot, text (name, desc, spell) and the upgraded text: not captured in Part 2 (see G5). The port's `source_layout.json` and `original_art.cpp` already hold the SWF geometry, which is authoritative.

## C. GAPS (with evidence)

1. The Faery tab is a stub. `main.cpp` ~2401-2406 prints the "requires live content provider" line and keeps the current page. No `characterMenuComposition->register_page(Tab::faery, ...)` exists (main.cpp 1804-1994 registers Equipment and Skills only). Evidence: P13 H-report and verify-ui report.
2. The page modules are not built. `port/windows-foundation/CMakeLists.txt` lines 143-148 list only six faery_menu sources: `hotty_cast_v1.cpp`, `hotty_cast_session_v1.cpp`, `hotty_character_cast_v1.cpp`, `hotty_source_use_v1.cpp`, `celest_cast_v1.cpp`, `celest_source_use_v1.cpp`. Not listed (grep of CMakeLists.txt for each name): `character_state_page_v1.cpp`, `faery_menu.cpp`, `session_faery_page_v1.cpp`, `original_art.cpp` (faery_menu), `native_binding_v1.cpp`, `source_text_v1.cpp`, `source_faery_ability_binding_v1.cpp`, `hotty_effects_v1.cpp`, `hotty_effects_tables_v1.cpp`. Not checked: whether another CMake file or glob pulls them in.
3. The page tests have no runner. `features/faery_menu/tests/{character_state_page_tests,source_art_tests,native_binding_contract_tests,source_faery_ability_tests}.cpp` are not referenced by any `.ps1` or CMake target (grep of windows-foundation returned nothing). Status: never executed.
4. Difficulty is hard-coded to 0. `main.cpp` 2061 uses `faery_by_difficulty[0]` for the HUD and 2672 for the rank; the cast arm uses `arm.difficulty=0` (main.cpp ~2675). IDA uses `PlayerSavegame::m_difficultyLevel`. CharacterState has no current-difficulty field (`character_state.hpp` has none; `game_save.cpp` and `save_store.cpp` do not store it). The per-difficulty unlock table therefore cannot be selected correctly for difficulty 1 or 2 yet.
5. Unlock is not set in any live flow. `faeries[].state` is written only by creation (zeroed), save load and tests. No code calls the equivalent of `SetFaeryState` (Swamp_Intro command 156). Consequence: a fresh Rogue/Mage/Knight has all faeries locked, and Celest is never unlocked by the script path. The Rogue test profile (preview13 `h/pkg/h-rogue.save`) has Celest pre-set, so the EXE tests never exercised the unlock path. Evidence: grep of `faeries[` and `.faeries` in windows-foundation finds writers only in creation (runtime_creation_persistence_v1.cpp 365), save I/O, and tests.
6. Levels are never changed in live code. No port code calls the IncFaeryLevel equivalent. The export has no IncFaeryLevel script command (B2). The `level` field only comes from saves.
7. Rocky, Wetty, Windy have no spell code (grep for `faerie_rocky|faerie_wetty|faerie_windy` in port finds only table strings). Selecting them on the page would show a faery whose key-4 cast does nothing. Evidence: table rows exist (B5), no `features/faery_menu` handler.
8. Mage/Rogue/Knight list selection is not modelled. `Knight/Mage/Rogue` lists exist in the table (B5) but the port reads only the first list (`source_faery_list_id`, `tables.lists().front()` in faery_menu.cpp 22-26; `character_state_page_v1.cpp` 9-20 chooses by `source_faery_list_id`, which is fine if the provider uses it). Verify per class.
9. HUD key-4 icon does not refresh after an in-session change. The HUD is composed once (`pcHudReady`); Preview 13 H-report line 95 lists this as unchecked. Selecting a faery on the page must re-compose the key-4 circle.
10. `ChangeFaery` path lacks the unlock gate in source (IDA shows none). The port must gate selection on `state==1` (UI-level, matching the authored button) or else an unlocked-check bug can select a locked faery. Decide in design (G2).
11. Stub message bypass: while the page is missing, clicks on the Faery tab do not change the page at all (main.cpp 2401-2406). A minimal "locked/unavailable" placeholder is needed, like the Equipment placeholder (main.cpp 1826-1832).
12. The Part 2 reference video did not show a Faery page in any sampled frame (B6). Fidelity against footage is not yet possible for the page layout.

## D. DESIGN (minimal, reusable)

Data model: no new persisted fields are needed. Reuse `CharacterState::faery_by_difficulty[d].{current_faery, faeries[slot].{state, level}}` and `source_faery_list_id`. Save schema stays at its current version (no bump). Add nothing to save_store/game_save for the page.

Difficulty: add a read-only difficulty source, not a persisted CharacterState field. Recommended: the active game difficulty held by the session/profile owner (the same value the port uses for creation's `fresh_difficulty`). If none exists, expose a parameter on `CharacterStateFaeryBindingsV1::difficulty` (already in the header, line 58) and have main.cpp pass the active difficulty. Coordinate with the main-menu metadata stream (G3).

Owners and reads/writes:
- Reads: CharacterState (faery rows, source_faery_list_id), FaeryTables (lists, faery rows, names), localization (`NativeGetStringFromSymbol` equivalent, existing `localize` callback), debug UnlockAllFaeries (existing `debug_unlock_all_faeries`, default off).
- Writes: only `current_faery` on the CharacterState of the same player, via `activate_slot` (the ChangeFaery equivalent). The page never writes state or level (matches `session_faery_page_v1.hpp` 22-25 rule).
- Skills side effect: after ChangeFaery, call the existing skill refresh (`CharAI::UpdateAllSkills` equivalent) owned by the skills stream. Dependency, not this stream's code.

New or changed files:
- Use `features/faery_menu/character_state_page_v1.{cpp,hpp}` as the page provider (it already takes CharacterState*). Do NOT bind the legacy `faery_menu.cpp`/`session_faery_page_v1.cpp` (they need PlayerSavegameV1 and a Session host).
- Add to `port/windows-foundation/CMakeLists.txt` near lines 143-149 (the faery_menu block): `features/faery_menu/character_state_page_v1.cpp`, `features/faery_menu/original_art.cpp`, `features/faery_menu/source_text_v1.cpp` (plus whatever `original_art.cpp` and `character_state_page_v1.cpp` pull in: `source_layout.json` geometry is compiled as data). Check each link dependency; faery_menu.cpp's `Bindings` types are needed only if `original_art` references them.
- New small test runner `features/faery_menu/run_character_state_page_v1_tests.ps1` (pattern: `run_hotty_cast_v1_tests.ps1`) for `tests/character_state_page_tests.cpp`.
- Optional: `features/faery_menu/faery_page_fixture_v1` is NOT needed (existing tests cover fixtures).

Integration hooks in main.cpp (by anchor):
1. Near the Skills registration (~line 1994, `register_page(f::character_menu::Tab::skills, runtimeSkillsMenu->source_page_provider(), error)`): add `bind_character_state_faery_provider_v1(bindings, provider, error)` and `register_page(Tab::faery, provider)`. Bindings: `character` = `&state` (the live CharacterState), `tables` = `sourceFaeryTables` (already loaded ~840-843; move the load before this point if needed), `difficulty` from the active difficulty, `validate_same_character` (identity check against the live character), `localize` = existing localization owner, `debug_unlock_all_faeries` = existing debug switch read, `activate_slot` = a lambda that sets `state.faery_by_difficulty[d].current_faery=slot`, refreshes the key-4 HUD (see 3) and the skill refresh.
2. Replace the stub at ~2401-2406: keep the same `characterMenuComposition->select` flow (remove the "requires live content provider" branch) so Action::faery goes through `release` like Equipment/Skills.
3. HUD refresh: after `activate_slot`, re-run the composition that sets `layout.active_faery_id` (main.cpp 2058-2066) and recompose the key-4 circle (`pcHudReady` path). Gap C9.
4. Unlock source (dependency, not this stream): the Swamp_Intro SetFaeryState(0,1) needs the script executor to call a `set_faery_state(slot,state)` on the live CharacterState. The quest/script stream owns that hook. This page only reads state.
5. CMake: add the three faery_menu sources listed above to the target near lines 143-149.

Save format: no change. Existing saves already contain the faery rows (schema 2+). Old saves without `source_faery_state_known` show the page as unknown (`SourceFaeryKnowledgeV1::unknown`). Never default a missing row to unlocked.

Rules to keep in the page: unlocked iff `state==1` (or debug all). Locked button shows the locked visual and does not call activate. Level is displayed, not changed. Selection of a locked faery is rejected in the provider (gap C10).

## E. WORK BREAKDOWN

T1 (S, under 2h): Build and run the existing page tests. Wire CMake for the three files and add `run_character_state_page_v1_tests.ps1`. Tests: present() gives 5 slots from a CharacterState fixture; locked/unlocked/unknown from raw state 0/1/2; level text; selected slot; activate of a locked slot rejected; activate of an unlocked slot sets `current_faery` only. Verifier idea: the runner prints PASS lines; no EXE.

T2 (M, under half a day): Register the provider in main.cpp (hooks 1-3 in D), replace the stub, and add the difficulty input. Tests: CT for provider + activate; then EXE verifier with quiet_run: copy the Preview 12 Rogue save (`.local-inputs/claude-preview13/h/pkg/h-rogue.save`, Celest current), run `--profile-click-frame` and `--menu-release` at the Faery tab (x=349,y=12 in 480-wide space per H-report line 85), capture the page, click slot 2 (Hotty) if unlocked. Expected log: `Character menu opened ...`, no "requires live content provider". Capture shows 5 slots with Celest selected and locked slots dimmed. Fresh Knight profile: all slots locked, key-4 empty. Use fresh save copies per QUIET-RULES.md.

T3 (M, under half a day; shared with quests stream): Unlock/level path. Route Swamp_Intro's SetFaeryState(0,1) and any IncFaeryLevel script commands to the live CharacterState (only the state/level write; no UI). Tests: CT that the script command sets the row and leaves other rows; EXE: a fresh Rogue playing to the Swamp intro shows Celest unlocked on the page. Blocks on G1 (where the level scripts are).

T4 (S, dependency): Per-difficulty current faery (`faery_by_difficulty[d]`) read by HUD and casts with the active difficulty instead of 0 (gap C4). Tests: a difficulty-1 fixture gives the page and HUD the difficulty-1 current faery.

Out of scope for this stream (listed for the root): Rocky, Wetty, Windy spell scripts (gap C7) and Hotty session runner link error (I-report line 91). Each needs its own cast survey.

## F. DEPENDENCIES / CONFLICTS

- Stats/Skills stream: ChangeFaery calls `UpdateAllSkills`; the key-4 cast uses the skill slot logic. The page must not write skill rows. Shared: `main.cpp` near the skills provider registration (~1994) and skills HUD.
- Quests stream: unlock/level come from scripts (Swamp_Intro command 156, IncFaeryLevel). The script hook for SetFaeryState and IncFaeryLevel is the shared point (T3). Also the trophy `faery_charged` (IncFaeryLevel) is a quest/trophy hook, not needed for the page.
- Map stream: none directly, but the Character menu tab bar is shared (character_menu source_composition).
- Equipment stream: shares `main.cpp` character menu composition; Equipment registration is at ~1826-1880. Faery registration goes after it. No shared data.
- Main-menu metadata stream: the active difficulty input (D). The main menu slot metadata may already hold difficulty; use the same source.
- Drops stream: none.
- Companions: `source_companions` faery ops (`current_faery_id`, `change_faery`, `faery_association`) read the same current_faery. Do not change companion AI here; any companion-driven ChangeFaery must go through the same `activate_slot` owner.
- Files touched by the implementation: `main.cpp` (hooks 1-3, stub removal), `CMakeLists.txt` (faery block 143-149), `features/faery_menu/character_state_page_v1.*` (fixes only if tests fail), new runner `.ps1`. Not `character_state.hpp` or save format.

## G. OPEN QUESTIONS

G1. Which scripts raise faery levels (`Script_IncFaeryLevel`) in Act 1? The exported campaign has zero such commands (B2). Is the export complete for level scope, or do Act 1 quests use another path? Needed for T3.
G2. Should selecting a locked faery be impossible (UI-only gate, as the authored button suggests) or should the provider also reject it? IDA shows ChangeFaery has no unlock check (C10). Recommendation: reject in the provider too; confirm with the user.
G3. Where does the active difficulty live in the port today (main menu / profile / game save)? CharacterState has none (C4). Needed for D and T4.
G4. Which FaeryList row each class uses (DEFAULT vs Knight/Mage/Rogue lists, and Fake_/Mage_ variants)? Needed to show the correct five faeries per class.
G5. The Faery page visual was not found in Part 2 sampled sheets (B6). Scan the remaining Part 2 sheets (s001, s003-s004, s006-s009, s011-s015, s017-s019, s021, s023-s024, s026-s027, s029) and Part 1 for the butterfly tab to get the locked/unlocked, selected and detail visuals. Cost: about 20 image reads.
G6. Is the Swamp_Intro unlock of Celest the only Act 1 unlock? Confirm with a fresh-save run once T3 exists.
G7. Hotty slot number (1 by SWF frame order, inference) and the `_B` upgraded text trigger (FaeryUpgraded, inference). Confirm from the SWF action code or a video frame with a leveled faery.
G8. Rocky/Wetty/Windy spells: are they in scope for Preview 14 or deferred? The page can list them, but casting does nothing until implemented.
