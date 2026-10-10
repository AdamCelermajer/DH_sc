# Preview 14 survey: QUESTS (quest system logic + Quest Log menu)

Survey only. No file under port/, docs/ or tools/ was edited. No game EXE was started (quiet rules not needed). Scratch frames: `.local-inputs/claude-preview14/QUESTS/`.
Short answer: the quest system is NOT coded in the Windows EXE. Quest code exists as component/fixture code (level-world, game-data, windows features/quests) and as the Android runtime, but `dh-foundation` does not compile any of it, `main.cpp` never references it, and the Character menu has no Quest tab (`Tab { stats, equipment, skills, faery }` in `port/windows-foundation/features/character_menu/character_menu.hpp`).

---

## A. EXISTING

### A1. Windows-foundation quest feature (`port/windows-foundation/features/quests/`)
Not in `port/windows-foundation/CMakeLists.txt` (grep for quest/features/quests returns nothing). Only `run_component_tests.ps1` compiles it. Per `coordination/integration-lead/handoffs/quests.md`, the status is `component_verified_live_campaign_integration_incomplete`.

| File | Purpose | Status |
|---|---|---|
| original_quest_adapter.hpp/.cpp | Stable identity (collection regular0/volatile1, difficulty0..2, row), projections over `CharacterMenuQuestsV51`/`PlayerSavegameV1` | component-tested, not integrated |
| original_quest_runtime_bridge.cpp | Bridge to native compile/update kernels | component-tested |
| source_quest_service_binding.hpp/.cpp | Frame gates, sync/online/difficulty selection, state0 alias, quest_allcomplete trophy | component-tested |
| canonical_quest_graph_factory.hpp/.cpp | Builds quest facade inside canonical Character profile bootstrap | object-only validation; needs native closure |
| source_quest_log_services_v1.hpp/.cpp | GetQuestFunctor / list sorting / details / currentquest | fixture-tested only |
| source_quest_page_v1, source_quest_menu_page_provider_v1 | Sprite599 `menu_QuestLogSheetNEW` page + route_hit for row/Activate | fixture-tested; NOT registered in menu stack |
| runtime_quest_menu_v1.hpp/.cpp, runtime_quest_menu_art_v1 | Page binding over CharacterState CQPG, row select, Make Active | fixture-tested; not in main |
| character_quest_progress_v1.hpp/.cpp (CQPG codec) | Per-row source state + currentquest per bucket; default `unknown` | component-tested |
| character_quest_page_v1, quest_text_resolver_v1 | Filter Active 6..12 / Closed >12, title/objective text | component-tested |
| npc_talk_objective_integration_test.cpp | QE_TalkToNPC -> objective on fixture Save | fixture only |

### A2. Level/game-data quest owners (not in the Windows build)
- `port/game-data/quest_persistence_v51.*`, `quest_savegame_v1.*`, `player_save_quest_sync_v3.*`: original QEST save owner, sync owner.
- `port/level-world/native_quest_runtime_v76.*`, `quest_condition_compile_v70.*`, `quest_talk_marker_v76.*`, `canonical_quest_move_zone_v31.*`, `loot_pickup_quest_tail_v10.*`, `character_menu_quests_v51.*`.
- `port/level-world/tests/quest_persistence_v51.cpp`, `quest_reward_sequence_v108.cpp`; `port/level-loader/tests/swamp_quest_event_route_v1.cpp` (best Swamp evidence, see B).
- Android only: `port/android-native/app/src/main/cpp/source_campaign_quests_v76.*`, `source_campaign_quest_frame_v108.inc`. Status per `.local-inputs/live-objective/quests/act1-quest-event-audit.md`: the live Act 1 log stops at Stage 10 (`RoomZone.InitObjectList`, missing receiver) before any quest activates, so no live quest behaviour is established.

### A3. Windows integration that does exist
- `port/windows-foundation/character_quest_blob.hpp` + `character_state.hpp:93` `source_quest_progress_cqpg` (bytes, empty = unknown). Schema 3 (`character_schema_version = 3`). `game_save.cpp:98-105` round-trips it when schema>=3 (`GameSave` version 1..3).
- `character_state.hpp:74-75` `experience` and `gold` (u64). Reward receivers can write these directly.
- `features/loot/original_loot.*`: has a "quest tail" receiver name but reports "Missing actual pickup inventory/quest/gold receiver" when absent. Not wired to quests.
- `features/dialogue/runtime_source_npc_dialogue_v1.*` and `features/interactions/npc_*`: exist, but neither is in the dh-foundation CMake. `main.cpp` contains no dialogue/NPC-talk/event-manager calls.
- `port/level-world/character_kill.cpp` (kill producer that raises quest events, `kill_raise_event`) is level-world only, not in the Windows build.

### A4. Status per item (quest stream)
- Source-only / component-tested: quest tables decode, identity, objective and reward kernels, CQPG codec, page projection, text resolver, Make Active fixture.
- Integrated in main.cpp: NONE of the quest modules. CQPG field is in CharacterState/GameSave (data only).
- Verified in the EXE: NO. No quest log lines appear in the preview-13 quiet-test logs (grep "quest" returns nothing from `.local-inputs/claude-preview13/quiet-test/*.log`; the `verify-final` hits are file-path matches, not runtime quest events).

---

## B. ORIGINAL BEHAVIOUR

### B1. Authored data (v2quests, 64 rows; decoded and checked by `port/level-loader/tests/swamp_quest_event_route_v1.cpp` and the audit)
Row names: `port/level-world/reference/character-menu-profile-v51/cache/v2quests_pyarraynames.bin.json`. Binary: `.../cache/v2quests_pyarray.bin` (hash in the audit).

| Row | Accept | Objective | Rewards (per difficulty row) | Notes |
|---|---|---|---|---|
| Swamp_Escape | (main) | type 0, event id 358, count 1 | n/a in survey | prerequisite of Moths (Escape state param 3) |
| Swamp_Moths | prereq type 2 (Swamp_Escape, state 3) | type 10 (KillEnemyTemplate), template 94, count 8 | XP 20 (type 1), gold 150 (type 0) | event `KillMoths_Post`, completion script `SwampScripts.Swamp_Camp_PrisonerC_b` |
| Swamp_KillFiveLizman | TalkToNPC type 5 (oid 366/41) | type 10, template 93, count 5 | XP 20, gold 200 | event `Kill5Lizman_Post`, script `SwampScripts.Swamp_Camp_PrisonerB_b`; scripts[6] `SwampScripts.LizBlood_Activate` |
| Swamp_KillWitch | type 4 accept zone `_prim_WitchQuestStart` | type 0, event 435, count 1 | XP 50, gold 50 | scripts: `SwampCaveWitchScripts.Open_portal_witch`, `SwampScripts.Swamp_Witch_Activate`, `SwampScripts.Return_Cinematic` |

Naming note: the row is "KillFiveLizman" but the journal text says "Kill 5 Bogwomps" (Bogwalker's Delight 1). The video names the monster "Bogwomp". Text source: `port/android-native/app/src/main/assets/original-cache/data/text/sidequests.english` (binary, read with bundled python). Journal strings found there:
- "Bogwalker's Delight 1" / "Kill 5 Bogwomps" / "Find and kill 5 Bogwomps?"
- "Bogwalker's Delight 2" / "Kill 8 Bog Moths." / "Find and kill 8 Bog Moths?"
- "Vengeance - Witch Hunt" / "Defeat the Bogwitch in the Northeast Cavern."

Not verified by me: the exact accept condition/NPC for Swamp_Moths (Mothgiver `_prim_NPC_Mothgiver` appears in the pyscripts; dialogue "Here, take this. We won't need it anymore."). The v2eventmanager event rows (`KillMoths_Post`, `Kill5Lizman_Post`) were checked only through the level-loader fixture, not re-decoded by me. The pyscript bytecode (001_swamp_pyscripts.bin) was only string-scanned: script names are visible (FirstQuest_Start, LizBlood_PostAccept, Swamp_Witch_Activate, Swamp_Camp_PrisonerB_b/C_b), but I did not decode the quest start/advance commands.

### B2. Logic (IDA, from the audit and symbol list; addresses from `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/`)
- `Quest::ReInit` 0x480060; `Quest::SetState` 0x480c78; `Quest::Update` 0x481818 (evaluates 14 states in order, rechecks current after each transition); `Quest::UpdateCompleted` 0x4814d8; `Quest::GiveRewards` 0x48015c; `RewardList::Give` 0x482920 (stops at first false).
- Other symbols: `Quest::UpdateAvailable/PreAvailable/PostAvailable/UpdateActive/UpdateClosed/UpdateLocked`, `Quest::Compile`, `Quest::Synchronize`, `Quest::HandleSaveInStateTransitonIfNeeded`.
- Objectives: `ObjectiveTemplate_KillEnemies<v2QuestKillEnemyTemplate, QE_KillEnemyTemplate, TestCharTemplate, Objective_SavedQty>` (Moths/Lizman), `ObjectiveTemplate_KillCharacter<v2QuestClearEnemies...>`, `ObjectiveTemplate_InteractWith<v2QuestTriggerPlate / TriggerOn / OpenGameObject / PickedUpLiftable / DestroyGameObject>`. Addresses not extracted in this pass.
- Conditions: `Condition_IsQuestInState`, `Condition_IsQuestStateLower/Higher`.
- Save: `Quest::_saveQuestData` / `_loadQuestData`; `QuestSavegame.cpp`. Objective progress lives in `completed14` / `quantity20` per the audit.
- Kill producer: `AISMonster::OnTargetDied`, `AISDefault::OnKill(Character*)`, `Script_KillActor::Execute`. Level-world `character_kill.cpp` mirrors this and raises the event with type 4 / template id.
- Gating observed in code (fixture-level): Available->Active requires prerequisite; Active->Completed requires objective completed; Completed runs rewards (stop at first false), then PreClosed/Closed. Reward type 4 (ConsumeLoot) is not used by Swamp rows.

### B3. Reference video, Part 1 (`.../dh2_video_research/video/part1_z_Zky7qQdYs.mp4`, 22:11, 640x360)
Method: contact sheets (1 frame per 20 s over the whole video, plus 1/2 and 1/3 fps for windows 320-400, 560-680, 680-860) and full-res frames. Observation vs inference marked.
- ~t=656-665 (OBSERVED, frames 656/659/662/665): dialogue "Sounds like a lead. Let's check it out." (CRISR82) -> banner "NEW QUEST: Defeat the Bogwitch in the Northeast Cavern." (659) -> location title "The Boglands" (662) -> enemy label "BOGWOMP" in combat (665). This is the Witch quest accept, then Bogwomp kills for Lizman.
- ~t=761-764 (OBSERVED, frames 761/764): "QUEST COMPLETED" panel over the cave with "Kill 8 Bog Moths", "Reward: 20 EXP, 150 GOLD". Matches the authored Moths row. Floating combat text "+GOLD"/"MISSED" overlaps (incidental).
- ~t=770-800 (INFERENCE): Stats page with allocation "+" buttons, then Skills page; plausibly a level-up from the 20 XP. Not verified.
- Moths accept: NOT located. Sampled 320-400 (0.5 fps), 560-680 and 680-860 (1/3 fps), 0-320 and 860+ only at 20 s steps. Treat the accept timestamp as unknown.
- Tutorial stat/skill pages are visible around t=320-400 (no quest).

### B4. Reference video, Part 2 (`.../video/part2_cIAW38IfLxY.mp4`, 15:16, Darkwoods/Gothicus section)
- ~t=74-79 (OBSERVED at 1 fps thumbnails, medium confidence on text): Quest Journal page opened; list top entry "Destination - Gothicus Castle"; hint box "To change your quest information, activate a different quest in the quest list."
- t=76 frame (OBSERVED full-res): Celeste dialogue "Gothicus is just at the end of this road, crisr82. Go ahead and lead the way!" (no menu).
- t=523 (OBSERVED full-res, `.local-inputs/claude-preview14/QUESTS/p2/quad-p2.png` bottom-left):
  - Header "Quest Journal" next to the back arrow; tab strip of six icons with the scroll/quest icon highlighted (5th of 6).
  - Left list, section "Quest Journal" (Assigned): Buried Alive (white dot = current/primary marker, orange bar = selected row), Law and Order, The Hidden Path, Trial of Heroes 1.
  - Section "COMPLETED" (check icon): Bogwalker's Delight 1, Bogwalker's Delight 2, Destination - Gothicus Castle, Prison Break (list may scroll).
  - Right detail: "SIDE QUEST" tag, title "Buried Alive", description "Search for the demon child in the Wormroot Hollow.", bottom-right "MAKE ACTIVE" button.
- t=524 (OBSERVED full-res, bottom-right): same page, Law and Order selected, "Defeat 12 Guilty Bandits within the Darkwoods."; the dot stays on Buried Alive (current marker unchanged by selection).
- Map page at t~520-540 (OBSERVED thumbnails): green cross markers = objectives; legend at ~t=63 lists Objective (green cross), Destination, Character, Merchant, Dungeon, Entrance.
- Implication (INFERENCE): the Bogwalker's Delight rows appear as COMPLETED in Part 2, consistent with Part 1 completion.

### B5. Logic-to-visual mapping
- Accept: NEW QUEST banner appears when state moves Available->Active (Witch: zone accept at t~656).
- Progress: kills of the authored template/event ids increment the objective; no HUD counter was seen in the frames sampled (not verified).
- Complete: QUEST COMPLETED panel with the authored reward lines; rewards applied to XP/gold.
- Journal: four tabs of rows plus COMPLETED; selection changes the detail pane; Make Active sets current (dot).

---

## C. GAPS (windows EXE, Act 1)

Minimal complete loop = accept -> progress -> complete -> reward -> journal display -> persistence. Each item below is missing in the Windows build unless stated.

1. Quest data loaded in dh-foundation: MISSING. Decoders exist (`QuestTablesPersistenceV51`) but no asset load of v2quests in main.cpp and no CMake sources. Evidence: `grep -i quest port/windows-foundation/main.cpp` returns nothing; CMakeLists has no quest sources.
2. Quest runtime (state machine Available/Active/Completed, prerequisites, objective counters): MISSING in Windows. Only the Android/level-world runtime (`native_quest_runtime_v76`) and fixtures exist, and they need EventManager, ConditionRuntime, ScriptRuntime services not present in dh-foundation.
3. Accept triggers: MISSING. Zone trigger (`_prim_WitchQuestStart`, accept type 4), NPC TalkToNPC (type 5), prerequisite gate (type 2). No NPC talk path (dialogue/interactions not in build).
4. Progress feed: MISSING. Kill events (template 93/94, event 358/435) are not raised from Windows combat death (`combat_session.cpp`/`combat_system.cpp` have no event producer). `character_kill.cpp` is level-world only.
5. Completion + rewards: MISSING. XP/gold fields exist (`character_state.hpp:74-75`) but nothing applies quest rewards; no "Quest Completed" panel; no XP/level path verified from quest.
6. HUD: MISSING. No quest marker/objective tracker/map objective icons in dh-foundation (map icon work belongs to the map stream). "NEW QUEST" banner and "QUEST COMPLETED" panel art not implemented; source art location not confirmed in this pass.
7. Quest Log menu route: MISSING. `CharacterMenu::Tab` has no quest tab (`character_menu.hpp`). `SourceCompositionV1` has a `source_menu_pages` map for `menu_QuestLogSheetNEW`, but nothing registers it (`source_composition.hpp:36`). The original tab-strip icon (5th of 6) has no Windows counterpart.
8. Quest Log content: component-only. `RuntimeQuestMenuV1` works on CQPG with `unknown` default, so a new character shows no quests until `initialize_fresh_progress` is called by a creation path (not wired). Current state shows nothing real: the row source (Assigned/Completed) comes from CQPG, which is never written by gameplay.
9. Persistence of objective progress: MISSING. CQPG stores per-row state and currentquest only (`character_quest_progress_v1.hpp` header). Kill counts (Moths 0..8, Lizman 0..5, Witch 0/1) and per-objective completed flags have no field. Original stores them as `completed14`/`quantity20` in the QEST owner.
10. Make Active: component-tested only; not reachable from the real menu route.
11. Verification: none in the EXE. No quiet run has exercised a quest.

Wrong or inconsistent items found:
- Row name `Swamp_KillFiveLizman` vs journal text "Kill 5 Bogwomps" (event `Kill5Lizman_Post`): the three names are all valid original strings; keep row/event ids authoritative, show the text as authored.
- `character_quest_progress_v1.hpp` says `record_source_state` "does not simulate objective/prerequisite transitions", so the CQPG path cannot advance quests by itself.

---

## D. DESIGN (minimal reusable)

### D1. Data model
- `QuestRowKey {collection(0 regular/1 volatile), difficulty(0..2), row}` (existing identity, keep).
- `QuestObjectiveProgress {int quantity; bool completed;}` per authored objective (matches `quantity20`/`completed14`).
- `QuestRuntimeState {state 0..13 per row (source enum), objectives[], current_quest}`.
- Event inputs (typed, reusable): `KillEvent{template_id or event_id}`, `TalkToNpc{oid1,oid2}`, `ZoneEnter{zone_name}`, `Pickup{item}`. Map each objective type 0/4/5/10 to these; no map or class names.

### D2. Ownership/state
- Read: `v2quests` decoded table (immutable, from asset cache), `CharacterState` (level/gold/xp).
- Write: `CharacterState::source_quest_progress_cqpg` (persist), `CharacterState::experience/gold` (rewards). Reuse `RuntimeQuestMenuV1` for the page.
- Keep quest runtime owned by the character session (not the menu). SaveStore/GameSave: no new top-level section.

### D3. New files (suggested)
- `port/windows-foundation/features/quests/quest_runtime_v1.hpp/.cpp`: thin Windows runtime: load table, evaluate prerequisites/accept, apply events, complete, grant rewards, serialize.
- `port/windows-foundation/features/quests/quest_events_v1.hpp/.cpp`: typed event adapter (kill/zone/talk) to runtime.
- Tests: `quest_runtime_v1_tests.cpp` plus `run_quest_runtime_v1_tests.ps1` (standalone clang++ build like the existing runners).

### D4. Integration hooks (main.cpp / CMake; locate by anchor)
- CMakeLists.txt: after the `target_sources(dh-foundation ... features/generic_skills/...)` lines (~line 69-72), add `features/quests/quest_runtime_v1.cpp features/quests/quest_events_v1.cpp` and link `../game-data` quest decoders (or `foundation_data`).
- main.cpp: (a) after character load in the session setup (near `request.shared_state=sharedCharacter` ~line 503 area / character init), call `quest_runtime.load_progress(state)`; (b) in the combat death path (near the `combatSession` kill/target handling ~line 2623 area) call `quest_events.kill(...)`; (c) in the zone enter handler call `quest_events.zone(...)`; (d) in the Character menu tab setup add the quest page provider on `menu_QuestLogSheetNEW` via `bind_source_quest_menu_page_provider_v1`; (e) save path: CQPG write when the character is saved (already in GameSave).
- These anchors must be re-read before editing; main.cpp is shared.

### D5. Persistence
- Keep `source_quest_progress_cqpg` as the only field. Extend the CQPG envelope to version 2 (add per-objective quantity/completed), keeping v1 readable. Empty bytes stay `unknown` (legacy). No character schema bump is needed if the blob is versioned inside the envelope; if the envelope can't be extended, bump character schema to 4 and load 3 with empty quest blob.

---

## E. WORK BREAKDOWN

1. (M) Quest runtime core + data load: decode v2quests in dh-foundation, implement state transitions (Available/Active/Completed), prerequisite gate, objective counters, rewards to CharacterState. Test: 64-row load, Moths prerequisite, kill 8 -> completed, kill 7 -> not, duplicate kill after complete ignored, reward grants 20 XP/150 gold exactly once, Witch zone enter.
2. (M) Event producers: kill from combat death, zone enter, TalkToNPC from interaction (only if interactions is in build; else defer Lizman/Witch accept). Test: scripted kill events in a headless harness; then a quiet EXE run with a fresh save where the Witch zone enter and 8 moth kills are triggered by debug input.
3. (M) Journal page routing: register `menu_QuestLogSheetNEW` on the Character menu quest tab, with the 5th-of-6 tab icon. Test: visual capture of Assigned/Completed rows, detail, Make Active; fidelity against Part 2 t=523/524 frames.
4. (S) Persistence: CQPG v2 with objective counters; round-trip, legacy-empty, truncated, mismatched-character tests.
5. (S) Banners/HUD: NEW QUEST and QUEST COMPLETED visuals (needs source art located first; blocked until art is identified).

Verifier script idea: `port/windows-foundation/features/quests/run_quest_runtime_v1_tests.ps1` (headless) plus a quiet `quest_act1_swamp` run using `.local-inputs/claude-preview13/p/b016/` pattern with a fresh save folder and `quiet_run.ps1`.

---

## F. DEPENDENCIES / CONFLICTS
- Skills/stats: XP (rewards) and level-up path share `CharacterState.experience`; coordinate with the skills stream (level-up screen observed after Moths at t~770-800, inference).
- Faery: a quest-faery route handoff exists (`port/windows-foundation/features/faery_menu/B009_quest_faery_route_handoff.md`, not read here).
- Map: quest objective markers on the map (green cross) and the Map page quest legend (B4). Map stream owns the icon; quest stream supplies the objective list.
- Equipment: none directly.
- Main menu metadata: current act/quest for the slot can read `current_quest`/act from the quest owner; agree on source of truth.
- Drops: quest-item pickup receiver in `features/loot/original_loot.*` (quest tail) is a dependency for pickup objectives (not Act 1 Swamp kill loop).
- Shared files: `main.cpp` (hooks in D4), `CMakeLists.txt`, `character_state.hpp` (CQPG envelope, if changed), `game_save.cpp` (no change if CQPG is versioned internally).

---

## G. OPEN QUESTIONS (not decidable from evidence)
1. Reuse the existing Android/level-world `NativeQuestRuntimeV76` closure in dh-foundation (large dependency: EventManager, ConditionRuntime, ScriptRuntime) or write the thin Windows runtime in D3? I recommend the thin runtime with the same state semantics, but it is a design decision.
2. Swamp_Moths accept: which NPC/condition starts it (Mothgiver dialogue?) and at what video timestamp? Not found in sampled windows.
3. Source art for "NEW QUEST" banner and "QUEST COMPLETED" panel: is it in the original SWF/cache (name needed) or from the menu art exporter?
4. Does the original show a visible kill counter in the HUD or journal for objectives? Not seen in frames sampled (Moths kill phase not sampled).
5. Objective counters persisted in the save: confirm the QEST layout (completed14/quantity20 per objective) so the CQPG v2 extension matches the original.
6. Tab strip: which of the original six icons map to Windows Stats/Equipment/Skills/Faery plus Quest? Needs a frame of the full tab strip with labels or the menu SWF tab order.
