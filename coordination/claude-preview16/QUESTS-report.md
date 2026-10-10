# Preview 16 QUESTS report (thin quest runtime; Quest Log tab NOT done)

Branch `p16/quests` (worktree `DH_wt/p16quests`, from `p15/integrate`). Commits: report skeleton `ed088e1b`, core `14b452d7`, host wiring `2446561b`, exact XP + test aid `873d969d`. Never pushed.

Short status: the runtime (load, state machine, events, CQPG v2 counters, rewards, accept/make-active) is implemented and unit-tested, and it runs in the real EXE on Swamp. Kill events from the live combat producer, NPC talk, zone triggers and the Quest Log tab are NOT verified/wired. Banners are logged, not drawn. Details under "Open risks".

## Investigation evidence (AGENTS.md items 1-4)

Logic (IDA pseudocode `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`):
- `Quest::Update` 0x481818: calls UpdateLocked, PostLocked, PreAvailable, Available, PostAvailable, PreActive, Active, PostActive, PreCompleted, Completed, PostCompleted, PreClosed, Closed, PostClosed in order; each guarded by `v2QuestState` constant equality.
- Transitions: UpdateLocked (prereqs true -> PostLocked); UpdateAvailable (prereqs false -> Locked; accept completed14 -> PostAvailable); UpdateActive (ObjectiveList::Eval -> PostActive); UpdateCompleted 0x4814d8 (end objective completed14 -> `GiveRewards` 0x48015c -> PostCompleted); UpdatePreClosed/UpdateClosed (Closed -> PostClosed).
- `Quest::SetState` 0x480c78 side effects: case 6 (Active) GLOBAL_QUEST_NEW dialog + SG_SetCurrentQuest(row) (NEW QUEST banner); case 7 (PostActive) SG_SetCurrentQuest(-1); case 9 (Completed) SG_SetCurrentAct(act); case 12 (Closed) QuestCompletedMsgDialog with the reward string (QUEST COMPLETED panel).
- Condition operators (`Condition_IsQuestInStateGeneric::Eval` 0x478fe8): op 0 state==p2, op 1 state<p2, op 2 state>p2. `Condition_IsPlayerInLevel::Eval` 0x478d78: p1 == current level row.
- Kill fan-out (`level-world/character_kill.cpp`, IDA-derived): one kill raises four v2QuestObjectiveType constants (KillXEnemies 0, ClearEnemies 1, KillEnemyTemplate 10, ClearEnemyTemplate 11) with the property id (types 0/1) or the template id (types 10/11) as subject.
- Kill objective handleEvent (`ObjectiveTemplate_KillCharacter<v2QuestKillEnemyTemplate,...>::handleEvent`): subject must equal the authored id, count++ then complete when count >= authored value; `TestCharTemplate::Compare` 0x47b05c = template id compare.
- Clear* objectives use HasEnemyOfTypeLoaded (level state), so they are deliberately NOT counted (reported as unsupported).

Table facts (decoded from the staged `v2quests_pyarray.bin`, 64 rows; dump in `.local-inputs/claude-preview16/quests-dump.txt`):
- Act coverage: act 1 = 32 rows, act 2 = 2, acts 3..14 present (e.g. Abbey rows act 8). Other-act rows are decoded and run through the same runtime.
- Prerequisite operators used: 0, 1, 2, 3 (IsPlayerInLevel). Objective types used: 0, 4 (25 rows, 25 MoveInZone), 5, 7, 10. Accept types 4/5/6; end objectives are all type 6 (automatic). Reward types: only 0 (gold) and 1 (XP).
- Swamp_Escape (row 50): prereq IsPlayerInLevel(41); accept automatic; objective type 0 property 358 x1; XP 100.
- Swamp_Moths (row 53): prereq op 2 (Escape state > 3); accept TalkToNPC oid1 365 / oid2 41; objective type 10 template 94 x8; reward XP 20, gold 150 (matches the QUEST COMPLETED frame text "Kill 8 Bog Moths", 20 EXP, 150 GOLD from the survey).
- Swamp_KillFiveLizman (row 51): accept TalkToNPC 366/41; objective type 10 template 93 x5; XP 20, gold 200.
- Swamp_KillWitch (row 52): accept MoveInZone with trigger name in str2 (`_prim_WitchQuestStart`) and level row in oid1 (41); objective type 0 property 435 x1; XP 50, gold 50.
- Finding: MoveInZone zone name is `str2`, not `str1`. 25 zone objectives have no name in str2 and have no authored match rule here; they are reported unsupported.

Video (survey B3/B4, NOT re-extracted in this session): NEW QUEST banner at part1 t~659 ("Defeat the Bogwitch in the Northeast Cavern"); QUEST COMPLETED panel at t~761 ("Kill 8 Bog Moths", "Reward 20 EXP, 150 GOLD"); Quest Log at part2 t=523-524. The banner/panel art itself is not located.

## Changes (all under `port/windows-foundation`)
- NEW `features/quest_runtime/quest_events_v1.hpp/.cpp`: `QuestEvent` (kill with property/template ids; zone_enter with zone name + level row; talk_to_npc with oid/oid2; object_interact; item_pickup) and the bus `raise_quest_event(const QuestEvent&)` / `bind_quest_event_sink`. Returns false when no sink is bound.
- NEW `features/quest_runtime/quest_table_v1.hpp/.cpp`: decode/load of the original v2quests PyArray + names (wraps `game-data/quest_persistence_v51`), `quest_rows_in_act_v1`.
- NEW `features/quest_runtime/quest_runtime_v1.hpp/.cpp`: `QuestRuntimeV1` over the CharacterState: `load` (fresh init when CQPG empty, else decode + counters), `save` (CQPG + counters into `source_quest_progress_cqpg`), `accept_quest(row)`, `make_active(row)`, `handle(event)`, `update()`, `take_banners()`, `state_of`, `current_quest`, `diagnostics()`. States and transitions follow the original (14 states, scripts not run). Rewards once (Completed -> PostCompleted only). Services: `give_experience`, `text`, `current_level_row`.
- `features/quests/character_quest_progress_v1.hpp/.cpp`: added `set_current_quest` (SG_SetCurrentQuest semantics; row -1 clears).
- `features/loot/runtime_death_rewards_v1.*`, `runtime_session_death_rewards_v1.*`: added `award_experience` (quest XP through the existing `award_player_xp` owner, same level-up path). Exact XP (`amount << 8`); `progression_award_raw_v1` adds kill-share rounding (+1) and is not used for quests.
- `main.cpp` (anchors "P16 QUESTS"): include; `bindQuestRuntime` lambda (decodes table from `original-cache/data/pydata`, creates runtime per session, binds sink, population ids map, prints bind line); `raiseQuestKills` per frame from combat death events whose attacker is the player; quest XP via deathRewards; test aid options `--quest-debug-kill FRAME:TEMPLATE:COUNT` and `--quest-debug-accept FRAME:ROW`; bind after the three `bindDeathRewards()` sites (initial, reload, restore).
- `CMakeLists.txt` ("Preview 16 thin quest runtime" block): `foundation_runtime_quests` library (+ game-data quest decoder and savegame sources), `quest_runtime_v1_tests` and ctest `quest_runtime_v1`.

Assumptions in code (flagged, verify in-game): kill event `property_id` = population `source_character_cache` (CharacterTable row) and `template_id` = `source_template_cache` (Charater_Templates row). Not verified against an in-game moth kill (see below).

## Tests run
- `p14_build.ps1 -Name p16quests -Test`: build exit 0. ctest: 114/115 pass; only `session_skill_binding` fails (known worktree junction issue, per brief). `quest_runtime_v1` passes.
- `quest_runtime_v1_tests <pydata>` (real table) checks: truncated/missing table rejects; Escape starts Active from IsPlayerInLevel(41); Moths waits for accept (Escape > 3); `accept_quest` rejects non-Available; Moths accept -> NEW QUEST banner; 7 kills keep Active, 8th completes (gold 150, XP 20 once, QUEST COMPLETED banner with rewards); duplicate kill grants nothing; reload keeps closed state and does not repay; Lizman counter survives save/reload (3 -> 5), gold 200, XP 20 once; Witch: wrong level and wrong zone do not accept, `_prim_WitchQuestStart` at level 41 accepts, property 435 completes (gold 50, XP 50); `make_active` on Active row sets currentquest, Available and Completed rows refused, PostActive clears current; other-act rows (act 2, act 8+) load and have valid states; the event bus delivers only with a bound sink.

## Verification (EXE, quiet batches, hidden desktop, silent)
Assets: the Preview 15 rc4 package lacks the quest tables, so a scratch copy `.local-inputs/claude-preview16/quests-run/assets` (full copy of rc4 assets + the two `v2quests_pyarray*.bin` from `.local-inputs/claude-preview16/levels-root/original-cache/data/pydata`) was used. Nothing in the package was written.
- smoke (`out/quest-smoke.log`): `Quest runtime bound rows=64 actors=33 current=50 cqpg=1612` (Swamp_Escape current, as the unit test predicts). No quest diagnostics.
- scripted (`out/quest-scripted.log`, args `quests-run/scripted.args`): accept Moths at frame 60 -> `NEW QUEST row=53`; 7 kills (frame 100) -> `current=53`, gold 0 xp 0; 1 kill (frame 140) -> `QUEST COMPLETED row=53 xp=20 gold=150`, state `gold=150 xp=20 cqpg=1676`; 3 duplicate kills (frame 150) -> applied=0, gold 150, xp 20; save frame 200 (`quest-scripted.save`, 2669 bytes); in-process reload at frame 220 -> `Quest runtime bound ... current=-1 cqpg=1676`; 3 kills after reload (frame 240) -> applied=0, gold 150, xp 20 (no repay across reload).
- Fight attempt (`out/quest-fight.log`): `--combat-auto --attack-start-frame 120 --attack-frames 2000`: the player never engaged an enemy (`target=0`), so no live kill occurred. No live kill path was exercised.

## Package files required
- `original-cache/data/pydata/v2quests_pyarray.bin` and `v2quests_pyarraynames.bin` (absent from `windows-source-clock-v19-preview-15-rc4`; present in `.local-inputs/claude-preview16/levels-root/original-cache/data/pydata/`). Without them the runtime reports `Quest table diagnostic: Quest table file is missing` and stays inactive.
- Text for later: `original-cache/data/text/sidequests.english` is present in rc4 (not read yet; see gaps).

## Verifier script
1. Create the scratch assets copy (described above) and `quests-run/scripted.args` (rc4 `swamp.args` with `--assets` pointed at the copy, plus `--frames 300 --save <out>/quest-scripted.save --save-frame 200 --load-frame 220` and the four `--quest-debug-*` options listed in the log).
2. `quiet_run.ps1 -JobsFile quests-run/jobs-scripted.json -Parallel 1 -Summary ...` (job exe `DH_wt/build-p16quests/dh-foundation.exe`).
3. Expect in the log, in order: `Quest runtime bound rows=64 actors=33 current=50`, `Quest debug accept frame=60 row=53 accepted`, `Quest banner kind=NEW QUEST row=53`, `Quest banner kind=QUEST COMPLETED row=53 xp=20 gold=150`, `Quest state frame=140 gold=150 xp=20`, duplicates `applied=0`, `Quest runtime bound ... current=-1 cqpg=1676` after the reload, and `Quest state frame=240 gold=150 xp=20`.

## Interface for p16/host and p16/containers
- Zone: `raise_quest_event(QuestEvent{Kind::zone_enter, zone="<trigger prim name>", level_row=<current level row>})`. Accept zones match `str2`; `level_row` must be the Level's row (the same value `menu_metadata::find_level_row` returns).
- Talk: `QuestEvent{Kind::talk_to_npc, object_id=<NPC oid>, secondary_id=<oid2>}`.
- Object interaction (TriggerPlate/TriggerOn/OpenGameObject/DestroyGameObject/PickedUpLiftable): `QuestEvent{Kind::object_interact, object_id=<oid1>}`.
- Kill (done in main via `raiseQuestKills`): `QuestEvent{Kind::kill, property_id, template_id}`.
- Accept without dialogue: `QuestRuntimeV1::accept_quest(row)`; Make Active: `make_active(row)`.

## Open risks / not verified / not done
1. Quest Log tab (item 5): NOT done. `CharacterMenu::Tab` still has four tabs; no 5th/6th tab-strip art or hit zones exist in the package, so no faithful route to the page is possible yet. The fixture-tested `RuntimeQuestCharacterMenuBindingV1` / `source_quest_menu_page_provider_v1` are not registered. Needs the original six-icon strip art (survey G6).
2. Banners (item 4): NEW QUEST / QUEST COMPLETED are queued and logged with StringID, authored XP/gold, but NOT drawn. Art not located; text not resolved (`text` service unbound: text='' in logs). Objective tracker: not done.
3. Live kill producer: mapping of the population caches to property/template ids is an inference (not verified in-game). Live kills were not exercised (player did not engage in the fight run). A scripted walk-and-attack to moths is still needed to verify `template=94` for Moths.
4. Text: no StringID resolver bound (quest title/objective text, sidequests.english not read). Quest Log text would be empty.
5. Unsupported by design (reported once in `diagnostics()`, never silently): Clear* objectives (1/11, need level enemy state), GatherLoot (12, inventory), MoveInZone without a trigger name (25 rows), reward types 2/3/4 (props, loot, consume), condition ops other than 0-3, quest scripts (SwampScripts.*, not executed; e.g. the Mothgiver/LizBlood side effects are absent), IsPrimary/SG_SetCurrentPrimaryQuest.
6. Objective `oid2` on OpenGameObject (type 7) is not matched (unknown semantics).
7. `CharacterQuestLogPolicyV1::debug_priority` defaults to 0 in the existing feature code, which hides every Primary quest from the policy filter; this runtime sets 2 (v2QuestPriority.Debug). The shared default should be fixed by the owner of `features/quests`.
8. Quest XP exactness: awarded as `amount << 8` (no kill-share +1). Consistent with the QUEST COMPLETED text (20 EXP) but the original call site is not decoded.
9. The debug options `--quest-debug-kill/--quest-debug-accept` are a test aid only, not gameplay.
10. Save: CQPG is written into `CharacterState` on every event; it reaches disk at the next existing save point (verified in-process at frame 200/220 only).
11. Not run: the full ctest-quiet-batch verifier of the Preview 16 release (only this branch's scripted job).
