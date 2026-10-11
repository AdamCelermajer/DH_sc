# Fidelity audit: quests batch (Quest, Objective_*, Condition_*, ScriptManager/Script_*, GameEvent/EventManager, LuaScript, QuestSavegame, dialogs)

## Progress

- last_address_done: 0x4b8acc (plus added scope 0x47a898-0x47f634, 0x482d80-0x482ef0, 0x440080, 0x4594a8, 0x43b4dc-0x43b53c: done)
- groups_done: Quest state machine; Objective_* (EventReceiver, GatherLoot, MoveInZone, TalkToNPC, Automatic); ConditionData/ConditionList/Condition_*; ScriptManager (Start/Execute/Skip/Stop); Script_Wait/ExecScript/Lock/Unlock/SpawnCharacter/PutCharacterInIdle/MarkScripted/Open+CloseDoor/MoveActor/LookActor/SetActorPosition; EventManager; GameEvent/GameEventManager; LuaScript scalar natives; QuestSavegame (partial); Quest text getters
- next: complete. Optional deeper pass: Script_* bodies with no Windows implementation (see F3), DialogMsg ctor details, Arrays::*::read table parsing for non-condition tables
- port map: level-world/native_quest_runtime_v76.cpp (Quest), level-loader/game_event_runtime_v75.cpp (Objective + GameEvent), level-world/native_conditions_runtime_v69.cpp + native_conditions_table_v69.cpp (conditions), level-world/event_manager_owner_v12.cpp (EventManager), windows-foundation/original_campaign_runtime.cpp + original_campaign_world_adapter.cpp (ScriptManager + commands), windows-foundation/features/{cinematics,progression_barriers,audio,quests,interactions}, script-runtime/script_scalar_bindings.c (Lua natives)

## Summary

Method note: the port keeps a very literal "native replica" of the original quest/condition/event/objective kernels in `port/level-world` and `port/level-loader` (written from IDA, with source addresses in comments). Those kernels are mostly gameplay-faithful. The real problems are (a) none of this is wired into the Windows executable, and (b) the Windows-side script command layer covers about 40% of the original command kinds and contains at least one inverted blocking rule.

Counts (group-level, ~45 groups covering the 634 functions): faithful 22, divergent 3, partial 10, missing 5, unclear 7, n/a 4 (boilerplate), intentional-adaptation 1.

Top 5 risks:
1. F1 [high] Quest, GameEvent, Condition, ScriptManager runtimes are not live in `windows-foundation/main.cpp` (only a diagnostic `--source-commands` replay exists). No quest changes state, no objective completes, no Script_* runs, no reward is granted in normal play. Documented as open (BUGS-AND-IMPLEMENTATION.md I004/I007, quests/ROOT_INTEGRATION.md) but it is the dominant gameplay gap.
2. F2 [high] `Quest::SetState` registers the quest "end" objective on PostCompleted(10) instead of Completed(9) and never removes the end marker on PostCompleted. The hand-in objective would not be listening while the quest waits in Completed.
3. F3 [high] Windows script command adapter rejects (hard error, latches the whole campaign runtime) about 45 of 80 command kinds, including DropLoot, KillActor, SetLevelState, ChangeLevel, EndGame, SpawnContainer, traps/trigger plates, ShowActor/HideActor, PlayEffect, equipment commands, StartDialogID, trophies, tutorial queue.
4. F4 [medium] `Script_CloseDoor::IsBlocking` is implemented with the OpenDoor rule (inverted).
5. F5/F6 [medium] ScriptManager skip semantics (SkipScript/StopSkipping, "IsBlocking ignored while skipping except ExecScript") and QuestMoveInZone collision events are not implemented on the Windows side.

## Findings

### F1. Quest/Script/Condition/GameEvent runtime not wired into the Windows executable
- severity: high (player-visible; documented open item); confidence: high
- IDA: whole batch (Quest::Update 0x481818, ScriptManager::ExecuteAllScripts 0x45c37c, GameEventManager::Update 0x4798c8, QuestSavegame::UpdateQuests 0x46bb58). These are driven every frame by the Level/Character update.
- Port: `port/windows-foundation/main.cpp:1504` creates `OriginalCampaignRuntime sourceCampaign` only to `load()` (main.cpp:1830) and to replay serialized single commands from `--source-commands` through `OriginalCampaignWorldAdapter::command` (main.cpp:2962-2965). There is no `sourceCampaign.tick/start/trigger_contact` call anywhere in non-test code (grep). `NativeQuestRuntimeV76`, `GameEventRuntimeV75`, `SourceCampaignDispatchV1`, `features/quests/*` and `features/cinematics/*` are not referenced by `windows-foundation/CMakeLists.txt`; only `native_conditions_*` and `condition_data_init_v3.cpp` are compiled in (CMakeLists.txt:581-582) and nothing in main calls them. Quest Log (CQPG) is a CharacterState-side model that only displays/stores states ("does not simulate objective/prerequisite transitions", features/quests/ROOT_INTEGRATION.md).
- Consequence: quests never leave their initial state; quest start/complete dialogs, objective completion, quest rewards (RewardList::Give), current quest/primary/act updates, GameEvent-driven scripts and every authored Script_* sequence never happen in a normal run.
- Suggested fix: bind a frame owner that calls QuestSavegame::UpdateQuests -> Quest::Update per frame, GameEventManager::Update, ScriptManager::ExecuteAllScripts(dt); publish NativeQuestRuntimeV76/GameEventRuntimeV75/ConditionRuntime over the same Save/Level graph as described in ROOT_INTEGRATION.md.

### F2. Quest::SetState: end-objective Register / RemoveMarker on the wrong states
- severity: high (once the runtime is wired); confidence: high (asm verified: vtable slot +0x18 Register at state 9, +0x14 RemoveObjectiveMarker at state 10)
- IDA: `Quest::SetState` 0x480c78. `case 9` (Completed): `end->vtbl[+24]` = Register, then RemoveObjectiveMarkers(list), ExecScript(3), SG_SetCurrentAct. `case 0xA` (PostCompleted): `end->vtbl[+20]` = RemoveObjectiveMarker, ExecScript(8). Cross-check: Quest::Compile 0x480178 for state 9 does Register(+24) then InstallObjectiveMarker(+16); SetState case 3/4 use +24 Register / +20 RemoveMarker for the accept objective.
- Port: `level-world/native_quest_runtime_v76.cpp:204-205`: `case 9` removes list markers, exec(3), current_act, but never calls `registration(q.end,true)`; `case 10` does `registration(q.end,true)&&exec(8)` instead of `remove(q.end)&&exec(8)`.
- Consequence: after Active -> Completed the end (hand-in, e.g. TalkToNPC) objective is not registered on the EventManager, so the hand-in event is not received and `UpdateCompleted` (needs end.completed14) cannot proceed, except after a save/reload where Compile (which is correct, native_quest_runtime_v76.cpp:~129) re-registers it. At PostCompleted the end objective is registered late and the TalkToNPC marker on the end NPC is not removed.
- Suggested fix: in case 9 add `registration(q.end,true)` before exec(3); in case 10 replace the registration with `remove(q.end)` (type-5 marker removal), keep exec(8).

### F3. Windows script command layer lacks ~45 command kinds (hard failure)
- severity: high (once the runtime is wired; currently masked by F1); confidence: high
- IDA: ScriptManager command table (kinds 0-79; descriptors in `level-loader/command_c1_descriptors_v59.inc`), Execute bodies 0x45c438-0x4613d4.
- Port: `windows-foundation/original_campaign_world_adapter.cpp:18` accepts kinds {1,2,24,25,30,31,32,39,69,70,77,78,79,27,28} plus camera 4/8, dialog 10/12, flash 22/23; cinematics adds 5,6,7,40,41,45,46; doors 54,55; audio 13,14; the runtime itself handles 0 and 26. Every other kind returns "Unsupported original campaign command kind" and `fail()` latches the whole OriginalCampaignRuntime (original_campaign_runtime.cpp:11,86).
- Missing kinds (no implementation anywhere in port except descriptors/IsBlocking constants in `level-loader/script_command_execution_v96.cpp`): 3 CONSOLE, 9 EmptyImpl (must be a no-op), 11 StartDialogID, 15 PlayLevelMusic, 16/17 Enter/LeaveSafeZone, 18/19 PlayAnimById/Name, 20/21 PlayEffect/StopEffect, 29 PutCharacterInLimbus, 33-38 AI*, 42/43 ShowActor/HideActor, 44 KillActor, 47 SetActorMaster, 48 DropLoot, 49-52 AutoEquip/UnEquip*/ReEquipHands, 53 SetStatic, 56-59 projectile traps / trigger plates, 60 SpawnContainer, 61 SetLevelState, 62 SetWorldMapLocationState, 63 RestartLevel, 64 ChangeLevel, 65 EndGame, 66-68 trophies, 71-76 tutorial message queue.
- Consequence: any authored script using these aborts the entire campaign script runtime (including unrelated scripts), so level transitions, enemy kill/drop scripts, trap triggers and cutscene effects cannot work.
- Suggested fix: implement each Execute/IsBlocking/Update body from the IDA addresses in the mapping table; at minimum make EmptyImpl/Console no-ops and make unsupported kinds non-fatal during bring-up.

### F4. Script_CloseDoor::IsBlocking inverted
- severity: medium; confidence: high (asm 0x455848 vs 0x455884)
- IDA: OpenDoor::IsBlocking = !(state3a8 in {1,3}) (EOR #1 at 0x455874); CloseDoor::IsBlocking = (state3a8 in {1,3}) (no EOR). Both return 0 if the cached door is null or the wait byte (this+0x14) is 0.
- Port: `features/progression_barriers/source_door_commands.cpp:42` uses `blocking=state!=1&&state!=3` for both kinds 54 and 55.
- Consequence: a CloseDoor with wait=1 returns "not blocking" while the door is still open/closing and then blocks forever once the door reaches a non-{1,3} state (script hang).
- Suggested fix: for kind 55 use `blocking = (state==1||state==3)`.

### F5. ScriptManager skip/stop semantics not modeled
- severity: medium; confidence: medium-high
- IDA: `ScriptManager::ExecuteScript` 0x45c16c checks `IsBlocking` only when `skipTarget==-1` or the command kind is ExecScript; while a skip is active (SkipScript 0x4604c0 flushes the dialog queue; StopSkipping when idle) all non-ExecScript commands run through with Execute(skip=true) and blocking is ignored. `StopScript` 0x455bb8 force-ends a script (state 2, skip=-1). StartScript 0x4605c0 also gates on AllPlayersDead and the SkipAllScripts debug switch.
- Port: `original_campaign_runtime.cpp:68-76` always evaluates blocking, has no skip target, no StopScript, no running-script count (+4) or StopSkipping. Skip is only a per-command provider callback (`SourceCampaignDispatchServicesV1::skip`).
- Consequence: cutscene skip cannot fast-forward waits/camera/anim blocks; scripts cannot be force-stopped.
- Suggested fix: add skipTarget/StopScript/running counter to OriginalCampaignRuntime and implement the `skipTarget==-1 || kind==ExecScript` gate.

### F6. QuestMoveInZone collision event and non-TriggerZone zones not implemented on Windows
- severity: medium; confidence: medium-high
- IDA: `QuestMoveInZone::OnCollisionBegins` 0x396120: if the colliding object IsCharacter (vtbl+36) and a current Level exists, `RaiseAsync(QuestEvent{type=v2QuestObjectiveType.MoveInZone, character=go, zone=this+100})`; consumed by Objective_MoveInZone::handleEvent 0x47bcd4.
- Port: only a service slot `whole_collision_begin` in `level-world/canonical_quest_move_zone_v31.cpp:29` (implemented by the android-native / level-loader catalogs, not by windows-foundation). `OriginalCampaignRuntime::trigger_contact` (original_campaign_runtime.cpp:87) rejects any source object whose gametype is not `TriggerZone`.
- Consequence: MoveInZone objectives and checkpoint zones never fire on Windows.
- Suggested fix: route zone contacts to a QuestMoveInZone handler that raises the MoveInZone event on the shared EventManager.

### F7. QuestSavegame current-quest verification / main-quest-updated / act-compat missing
- severity: medium (low until F1 is fixed); confidence: medium
- IDA: `QuestSavegame::SG_GetCurrentQuest` 0x46b5c0 calls `SG_VerifyCurrentQuest` 0x46b510: if the current quest is not state 6 (Active), reselect the first Primary quest in state Active (else leave unchanged). `HasMainQuestUpdated` 0x46b5dc and `IsActCompatible` 0x46b144 gate multiplayer `Synchronize` 0x46b644.
- Port: no matches for these functions in game-data/level-world/windows-foundation. Quest Log current marker is `source.current_quest==row` (`features/quests/character_quest_progress_v1.cpp:215`) with no Active re-verification.
- Consequence: after a current quest completes, the Windows log keeps showing it as current; the original falls back to another active primary quest. Multiplayer sync is n/a.
- Suggested fix: apply the VerifyCurrentQuest rule when reading the current quest.

### F8. Verified faithful kernels (no finding, recorded for reuse)
- Compared line by line against IDA: Objective::SetIsCompleted (completed flag set before script lookup; script id + common-script count offset; `game_event_runtime_v75.cpp:complete`), onEvent wrapper (byte8 gate; completed early-out without network send), GatherLoot handleEvent/Compile/InitWithCurrentQty, MoveInZone handleEvent character/props filter and Unregister-after-complete, Automatic Compile (compiled + complete), ObjectiveList::Eval/IsValid, GameEvent::Update/SetState/Compile/ReInit, EventManager Attach (dup reject), Detach, DelayedDetach/DropDelayedDetach, Raise (snapshot, stop on result==1), Update (queue drained then DropDelayedDetach), RaiseAsync==Raise.
- Condition factory order (`s_conditionImplDataMap` 0:InState 1:Lower 2:Higher 3:PlayerInLevel 4:LevelInState 5:PlayerAtLevel 6:EventInState), QuestInState compare semantics (==, param>state, param<state), IsLevelInState/IsPlayerAtLevel literally return true in the original (`native_conditions_runtime_v69.cpp` case 4/5), IsPlayerInLevel uses Level+60 id, ConditionList short-circuit, ConditionData::IsTrue (tested flag / empty list -> true).
- Quest::Update ladder (scripts waited per state 9/11/6/10/8/13/8/12/2; Available returns to Locked when prerequisites fail; Completed gives rewards then online act; PreClosed sets +93; Closed sets +93 then local-player trophy), SetState volatile-state save gating (IsVolatileState table, HandleSaveInStateTransition script-id pairs), Compile: faithful except F2.
- Quest text getters (Title/Pre/Post/ObjectiveDescription incl. sentinel 1835016, "not specified", ObjectiveList::GetDesc newline join, DescStr id<=0 -> empty): faithful in `quests/quest_text_resolver_v1.cpp`.
- Script_Wait (Execute resets, Update adds dt, blocking elapsed<duration), Script_ExecScript (child id + common offset when !global flag, LCG 59051/177149 mod 0xDAF26B, IsBlocking = wait && running(child)), Lock/Unlock "All" rules (Lock case-insensitive + skip gate, Unlock case-sensitive), SetActorPosition party offsets (-200x, +200x, -200y for players 1/2/3), LookActor HighestThreatPlayer filter, OpenDoor/CloseDoor Execute (type==2 check, Door::Open/Close(skip)), Lua ToFixed/FromFixed (0x3B800000)/MulFixed/DivFixed (a/(b>>8))/Bit* (`script-runtime/script_scalar_bindings.c`).

### F9. Kill/Clear-enemy objectives cannot complete on Windows (no event emitter), though the objective kernel is faithful
- severity: high (once F1 is fixed; today moot); confidence: high
- IDA: `ObjectiveTemplate_KillCharacter<...>` (ClearEnemies / ClearEnemyTemplate: Compile 0x47f070 captures loaded count into +44 and completes when loaded<=killed; handleEvent 0x47f100: match event id vs data id, ++qty, mirror to event+20 / network max, complete when qty >= required) and `ObjectiveTemplate_KillEnemies<KillEnemies/KillEnemyTemplate>` (Compile 0x47ed70/0x47ee18: required = data+40 authored count, id = data+32, level gate = data+36, byte8=0 when level mismatch or no loaded enemy). `Objective_EventReceiver::HasEnemyOfTypeLoaded<TestCharPropId>` 0x47c7f4: counts ObjectManager characters whose props id matches, else falls back to `HasCharOIDInCache`; the `<TestCharTemplate>` variant 0x47cc20 returns 0 when the count is 0 (no cache fallback, because Compare functions differ). Producers are the character kill path raising QuestEvents KillXEnemies, ClearEnemies, KillEnemyTemplate, ClearEnemyTemplate in that order (port transcription: `level-world/character_kill.cpp:91-101`).
- Port: the consumer side matches (`level-loader/game_event_runtime_v75.cpp:compile_objective` types 0/1/10/11 and `on_event`; required field and level-gate offsets agree with IDA; template variant skips cache fallback via `enemies_loaded(templ,...)` flag). The emitter exists only in `level-world/character_kill*.cpp` (level-world library); `windows-foundation` has no reference to KillXEnemies/ClearEnemies/RaiseAsync outside NPC talk, container, loot pickup and zone paths, and `features/loot/runtime_death_rewards_v1.cpp` (the Windows death path) raises no quest events (confirmed: grep).
- Consequence: kill and clear objectives can never complete on Windows, even after the quest runtime is wired, until the death path calls the character_kill emitter with the shared Level EventManager.
- Suggested fix: from the Windows death handler call the Character kill quest tail (four events in order, skipped when actor->suppress_quest, template events skipped when template_id==-1).

### F10. Minor items from the added scope
- `Script_CharacterState_Callback` 0x4594a8: "All" (case-insensitive) iterates the whole ObjectManager character list (+44), else a name lookup with module (callback receives possibly NULL). Port delegates to `providers_.character_selector` (original_campaign_world_adapter.cpp:31-39); selector semantics (including "All" and NULL tolerance) belong to the host provider and are not implemented in windows-foundation. status: partial, low.
- `GetQuestFunctor` 0x440080 + `IsNonDebugQuest*` 0x43b4dc-0x43b53c: faithful in `features/quests/source_quest_log_services_v1.cpp` and `character_quest_progress_v1.cpp:45-46` (Active = state 6..12 inclusive, Closed = state > 12, Debug-priority excluded, DisplayAllQuestInLog / DisplayAllDebugOnlyQuestInLog switch order).
- `GetRewardImplInstance<Reward_*>` 0x482d80-0x482ef0: factory ctors only; port `native_quest_runtime_v76.cpp:prepare` bounds type to the five captured entries and enables types <=3 or type 4 (ConsumeLoot) with parameter2>=0. Reward value semantics live in RewardList (outside this batch). status: faithful (structure), unclear (Give semantics).
- `ObjectiveTemplate_InteractWith<...>` (TriggerPlate, TriggerOn, OpenGameObject, DestroyGameObject, PickedUpLiftable, TalkToNPC): handleEvent 0x47f194 pattern (event id match, ++qty or network max, complete when qty >= data+40) matches the generic branch in `game_event_runtime_v75.cpp:on_event` (types 2,3,5,7,8,9 use field20 as required id and field28 as required count). Their producers (openable container, loot pickup, zone collision, NPC talk) exist in level-world / features/interactions and are individually wired outside the quest runtime; GetPositions bodies (minimap markers) not found in port.

## Mapping table

| IDA address | IDA function (or group) | port location(s) | status | note |
|---|---|---|---|---|
| 0x480c78 | Quest::SetState | level-world/native_quest_runtime_v76.cpp:set_state_v108 (~170-215) | divergent | F2: end-objective Register/RemoveMarker swapped (cases 9/10); rest faithful |
| 0x481818 | Quest::Update | native_quest_runtime_v76.cpp:update_quest_v108 | faithful | not wired (F1) |
| 0x4817b4,0x4813e4,0x481380,0x481700,0x48131c,0x4815d4,0x481688,0x4812b8 | Quest::Update{Locked,PostLocked,PreAvailable,Available,PostAvailable,PreActive,Active,PostActive} | update_quest_v108 branches 0-7 | faithful | |
| 0x481254,0x4814d8,0x4811f0,0x481184,0x481448,0x47f700 | Quest::Update{PreCompleted,Completed,PostCompleted,PreClosed,Closed,PostClosed} | update_quest_v108 branches 8-13 | faithful | online act sync via callback |
| 0x47fb74,0x47f70c | HandleSaveInStateTransitonIfNeeded / IsVolatileState | native_quest_runtime_v76.cpp:~177-186 | faithful | Level QuickSave/SG_Save delegated to transition_save callback |
| 0x480178 | Quest::Compile | native_quest_runtime_v76.cpp:compile | faithful | |
| 0x47ff20,0x47fde4,0x47fa80 | Quest::ExecScript / TestIsScriptRunning / GetScriptIDFromID | native_quest_runtime_v76.cpp:script_v108 | faithful | selector 0..13 order matches |
| 0x48015c | Quest::GiveRewards | native_quest_runtime_v76.cpp:quest_reward_sequence_v108 | faithful | RewardList::Give outside this batch (callback) |
| 0x480060,0x4802b4,0x481150,0x481158,0x47f734,0x47f78c | ReInit, Synchronize, ReinitStateAfterLoading, _save/_loadQuestData | native_quest_runtime_v76.cpp, features/campaign_save | partial | Synchronize (multiplayer) absent; ReinitStateAfterLoading via volatile64 |
| 0x47f7fc,0x47f850,0x47f8a4,0x47f8f8,0x47f94c,0x47f984,0x47f9dc,0x480a84 | Quest::Is{Dialog,Debug,Secondary,Primary}, Get{Title,Pre,Post}Description, GetObjectiveDescription | features/quests/quest_text_resolver_v1.cpp, source_quest_log_services_v1.cpp | faithful | |
| 0x4808e4,0x48081c,0x4807d8 | Quest ctor / AssignPyData / SetOwnerToChildren | native_quest_runtime_v76.cpp:prepare | faithful | |
| 0x47a444,0x47a498,0x47a3e8,0x47a540-0x47a690 | ObjectiveList Eval/IsValid/Register/Compile/LoopOnAll | game_event_runtime_v75.cpp, native_quest_runtime_v76.cpp | faithful | |
| 0x47ba10 | Objective::SetIsCompleted | game_event_runtime_v75.cpp:complete | faithful | |
| 0x47acf0,0x47ac94,0x47adbc,0x47ae70 | Objective_EventReceiver onEvent/CheckMustSendThroughNetwork/Unregister/Register | game_event_runtime_v75.cpp:on_event, register_objective | faithful | online send via service |
| 0x47ba5c,0x47bae4,0x47aba4,0x47eaec,0x47d6fc | Objective_GatherLoot | game_event_runtime_v75.cpp (type 12) | faithful | |
| 0x47bcd4,0x47c868,0x47b850 | Objective_MoveInZone handleEvent/Compile/GetPositions | game_event_runtime_v75.cpp (type 4) | partial | event source missing on Windows (F6); GetPositions (minimap) not found |
| 0x47db34,0x47dbc4,0x47d8e8,0x47da6c | Objective_TalkToNPC Register/Unregister/Install/RemoveMarker | game_event_runtime_v75.cpp (talk_flag), native_quest_runtime_v76.cpp:marker, features/interactions | faithful | marker install/remove via services |
| 0x47bd80,0x47a1d8-0x47a1e0 | Objective_Automatic | game_event_runtime_v75.cpp (type 6) | faithful | |
| 0x47a388,0x47ab3c-0x47ab84 | Objective(_SavedQty) _reset/_load/_saveData | features/campaign_save, QuestObjectivePersistenceV51 | unclear | serialization not byte-compared |
| 0x33dd24,0x33e5f8,0x33e7c8,0x33eb28 | ConditionData::{SetAsTested,IsTrue,Clear,Init} | level-world/condition_data_init_v3.cpp, native_conditions_runtime_v69.cpp:condition_data_is_true | faithful | |
| 0x478704,0x478914,0x478d78,0x478db4,0x478fe8,0x478638,0x478640 | ConditionList::Eval/AssignPyData, Condition_Is{PlayerInLevel,EventInState,QuestInState*,LevelInState,PlayerAtLevel} | native_conditions_runtime_v69.cpp | faithful | LevelInState/PlayerAtLevel are literal true in the original |
| 0x478634,0x478674,0x4786dc,0x4786f0,0x47899c-0x478b5c,0x478ba4-0x478fcc | Condition/ConditionList ctors, factories, DBG_Trace | native_conditions_runtime_v69.cpp | n/a | boilerplate / debug |
| 0x33811c,0x3382a4,0x3383f4,0x3384ac,0x338da0,0x338ebc,0x33900c,0x339090 | EventManager Detach/DelayedDetach/DropDelayed/Flush/Attach/Raise/Update/RaiseAsync | level-world/event_manager_owner_v12.cpp | faithful | |
| 0x479408,0x4793d0,0x4793a4,0x4794fc,0x479538,0x4798c8,0x47992c,0x4796f0,0x479848,0x479e5c | GameEvent / GameEventManager Update/SetState/ExecScript/Compile/ReInit/GetEventByID/GetEventByName/Load | level-loader/game_event_runtime_v75.cpp, game_event_manager_v50.cpp | faithful | |
| 0x4605c0 | ScriptManager::StartScript | windows-foundation/original_campaign_runtime.cpp:start | partial | admission callback (AllPlayersDead/online/debug) delegated; unconditional offline restart matches |
| 0x45c16c,0x45c37c | ScriptManager::ExecuteScript / ExecuteAllScripts | original_campaign_runtime.cpp:execute/tick | partial | F5: no skip gate / running counter |
| 0x4604c0,0x455be0,0x455bb8,0x455c54 | SkipScript / StopSkipping / StopScript / IsCutSceneRunning | - | missing | F5 |
| 0x455bec,0x455b8c,0x4591f0,0x455c40 | IsScriptRunning / GetNameFromID / GetIDFromName / GetScriptModule | original_campaign_runtime.cpp:running/script_id | faithful | |
| 0x45ad40,0x45b264,0x45a1c8,0x45a2ac,0x455ad8,0x45632c | LoadScriptFileNames / LoadScriptFile / UnLoadAllScripts / Flush / InitCommands / ScriptCmds::Free | original_campaign_runtime.cpp:load (pre-exported XML) | intentional-adaptation | scripts pre-exported to originalCampaign XML; descriptors in level-loader/command_c1_descriptors_v59.inc |
| 0x45c438,0x4597b4,0x455754 | Script_Wait Execute/Update/IsBlocking | original_campaign_runtime.cpp:execute (kind 26) | faithful | |
| 0x4607f4,0x455c08 | Script_ExecScript | original_campaign_runtime.cpp:execute (kind 0) | faithful | RNG supplied by host callback (same LCG elsewhere in port) |
| 0x45dda0,0x45dc80 | Script_Lock/UnlockCharacter | original_campaign_world_adapter.cpp case 24/25; cinematics/source_cinematics.cpp | faithful | |
| 0x45f400,0x45d590,0x45d4c8,0x45eec0 | SpawnCharacter, PutCharacterInIdle, MarkCharacterAsScripted, StopActor | original_campaign_world_adapter.cpp:30-39 | partial | provider-bound; Idle gate (Script_CharacterState_Callback) supplied by host, not verified |
| 0x45d75c,0x45d650,0x455848,0x455884 | Script_OpenDoor/CloseDoor Execute/IsBlocking | features/progression_barriers/source_door_commands.cpp | divergent | F4 (CloseDoor blocking inverted); Execute faithful |
| 0x45f0a8,0x459698,0x45ec50,0x45e5c8,0x45e890 | MoveActor, LookActor, SetActorPosition, PlayActorAnim | features/cinematics/source_cinematics.cpp:actor_command | partial | Look/SetPosition faithful; MoveActor/PlayActorAnim bodies delegate to unbound providers |
| 0x455678,0x4592cc,0x45932c,0x459370,0x45c670,0x460004,0x460110,0x45c5e8 | Set/Play/Wait Camera, CameraClip, CameraTarget | features/cinematics/source_camera_*.cpp, campaign_camera_adapter.cpp | unclear | camera numbers not compared in depth |
| 0x45a384,0x4599ac | Enter/ExitCutSceneMode | adapter kinds 1/2 -> providers_.cutscene_mode | unclear | HUD StartCutscene/StopCutscene, AnimController scaling, player block done by unbound provider |
| 0x460fe8,0x45c4d8,0x4598d8,0x461114 | StartDialog / WaitDialog / StartDialogID | adapter kinds 10/12 -> providers_.dialog; kind 11 missing | partial | EnterLocationDialog queue rule and StartDialogID missing (F3) |
| 0x45f8c8,0x45f78c,0x4556c4,0x455708 | ShowFlash/HideFlash | adapter kinds 22/23 -> providers_.flash | partial | HUD-substring callbacks provider-bound |
| 0x45f5cc,0x45f6c8 | IncFaeryLevel / SetFaeryState | adapter kinds 27/28 | partial | writes CharacterState (unlock/level only) per comment in adapter |
| 0x45fd20,0x45fe2c,0x45fefc | PlayLevelMusic / StopSound / PlaySound | audio kinds 13/14 only | partial | PlayLevelMusic (kind 15) missing |
| 0x46033c-0x460b1c,0x4613d4,0x45925c,0x460950,0x460a58 | Tutorial/CharMenu tutorial commands, LockTutorial, DoTutorial, FlushMessages | adapter kinds 77,78,79 | partial | 71-76 missing (F3) |
| 0x45ca80,0x45cba0,0x45cdd0,0x45ce78,0x45cf40,0x45d008,0x45d0e0,0x45d1d8,0x45d2d4,0x45d3cc,0x45d868,0x45d95c,0x45da8c,0x45db88,0x45ded0,0x45e028,0x45e124,0x45e24c,0x45e374,0x45e4b8,0x45ea04,0x45eb14,0x45f4e8,0x45fa9c,0x459890,0x45fbe0,0x45fc80,0x459b04,0x4593c4,0x45f2fc | EndGame, ChangeLevel, RestartLevel, SetWorldMapLocationState, SetLevelState, SpawnContainer, Trap/Plate (de)activate, SetStatic, Equip commands, PlayAnimByName/ById, DropLoot, SetActorMaster, KillActor, ShowActor, PutCharacterInLimbus, PlayEffect/StopEffect, SafeZone, AwardEndGameTrophies, AwardTrophy, AISetInt | - | missing | F3 |
| 0x455630-0x455978 | trivial IsBlocking stubs / AI* Execute BX LR bodies / ShowTrophies Execute | level-loader/script_command_execution_v96.cpp (kind constants) | faithful | structural check only |
| 0x37df30,0x37e2d8 | LuaScript::_Rand/_RandF | level-world/generic_lua_script_owner_v13.cpp (provider slot) | unclear | no concrete implementation in Windows build; original: a + GetRandom(b-a); RandF uses integer random of truncated range |
| 0x37ebc4,0x37ee84,0x37e1a4,0x37e068,0x37e814,0x37e9ec,0x37f750,0x37f814 | Lua To/FromFixed, Mul/DivFixed, Bit* | script-runtime/script_scalar_bindings.c | faithful | |
| 0x37cc00,0x37cb8c,0x37cbd8,0x37f1f0,0x37ddc8,0x37e420,0x37e578,0x37e730 | GetHostPlayerLevel/Difficulty, GetNumPlayers, GetCurrentLevelRange, IsPlayerCharacter, PlayMusic/PlaySound/StopSound | generic_lua_script_owner_v13.cpp (provider slots) | unclear | bound only when host supplies native values; not wired in Windows |
| 0x37b23c,0x379fe8,0x37b574,0x37b5a0,0x37c390-0x37c514,0x37de5c,0x37d990,0x37da30,0x37ec14,0x37ec70,0x37dc44,0x37be00 | LuaManager AddFile/Flush, LuaScript Load/BindFunction/Call, SetInt/GetInt, VFTable ops | level-world/generic_lua_script_owner_v13.cpp, script-runtime/script_int_bindings.cpp | partial | structure present, not byte-compared |
| 0x396120 | QuestMoveInZone::OnCollisionBegins | canonical_quest_move_zone_v31.cpp (service slot) | missing | F6 |
| 0x396330,0x396378 | QuestMoveInZone ctors | canonical_quest_move_zone_v31.cpp | n/a | |
| 0x46b170,0x46b1d0,0x46b258,0x46b2d4,0x46b940,0x46ba4c | QuestSavegame SG_GetNumQuests/GetQuestByID/GetNext/GetPrev/GetQuestByName | game-data/quest_savegame_v1.cpp, features/quests/source_quest_service_binding.cpp | faithful | bounds/compile-on-demand matches (not line-compared for GetPrev/ByName) |
| 0x46b510,0x46b5c0,0x46b5dc,0x46b144,0x46b644 | SG_VerifyCurrentQuest / GetCurrentQuest / HasMainQuestUpdated / IsActCompatible / Synchronize | - | missing | F7 |
| 0x46b824,0x46bb58,0x46c1a8 | CompileQuests / UpdateQuests / InitQuests | level-world/character_menu_quests_v51.cpp, source_quest_service_binding.cpp | partial | UpdateQuests loop only exists in binding->update, not driven (F1) |
| 0x46c344,0x46c48c,0x46c550,0x46c584,0x46c658,0x46c6fc | Quest Pack/Unpack/Load/SaveQuests | features/campaign_save, QuestTablesPersistenceV51 | unclear | serialization not byte-compared |
| 0x4a4588-0x4a8c44,0x4b1460-0x4b3608,0x4b8acc-0x4bc798,0x4da94c,0x4dcbd4 | Arrays::v2Quests/v2Events/v2Conditions/Dialogs/DialogActors read/readNames/finalize, ConditionalDialog | game-data quest tables, level-world/native_conditions_table_v69.cpp | unclear | only v2Conditions table decode compared (schema and row layout match) |
| 0x433b9c,0x433dd8,0x433ea8,0x4343d8,0x434434,0x43452c,0x43f828 | DialogMsg ctors/SetActorName/operator= | engine-ui dialog, features/dialogue | unclear | not compared |
| ctors/dtors/thunks | non-virtual thunks (Objective_*, QuestMoveInZone), EventManager/ConditionData/DialogMsg dtors, DBG_Trace* | - | n/a | boilerplate / debug |
| 0x47a898-0x47f634 | ObjectiveTemplate_KillCharacter / KillEnemies / InteractWith<...> (Compile, handleEvent, GetPositions, DBG, dtors, GetObjectiveImplInstance factories) | level-loader/game_event_runtime_v75.cpp (compile_objective, on_event) | faithful (consumer) / missing (emitter on Windows) | F9; GetPositions not found; ctors/dtors/DBG n/a |
| 0x47c7f4,0x47cc20 | Objective_EventReceiver::HasEnemyOfTypeLoaded<TestCharPropId/TestCharTemplate> | game_event_runtime_v75.cpp services_.enemies_loaded (host provider) | partial | provider not implemented in windows-foundation |
| 0x482d80-0x482ef0 | GetRewardImplInstance<Reward_*> | native_quest_runtime_v76.cpp:prepare | faithful | factory only |
| 0x440080,0x43b4dc,0x43b4f0,0x43b514,0x43b53c | GetQuestFunctor, IsNonDebugQuest*, IsDebugQuest | features/quests/source_quest_log_services_v1.cpp, character_quest_progress_v1.cpp:45 | faithful | |
| 0x4594a8 | Script_CharacterState_Callback | adapter kinds 31/32/39 via providers_.character_selector | partial | F10 |
