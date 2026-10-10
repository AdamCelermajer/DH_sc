# Quests checkpoint

Exclusive editable feature ownership: `port/windows-foundation/features/quests/` and `reports/feature-quests.json`. Root main/CMake/playable files remain exclusive to integration lead/root. No further child spawning. Existing working files are uncommitted shared workspace changes; do not reset or recreate them.

Implemented code:

- `original_quest_adapter.hpp/.cpp`: stable identity `(regular0/volatile1,difficulty0..2,source table index)`, live original projections, source current/primary/monotonic-act cells, SAME CharacterMenuQuestsV51 / PlayerSavegameV1 authority.
- `original_quest_runtime_bridge.cpp`: original native compile/update/prerequisite kernels, original campaign script bridge. Quest script references require a dot and use suffix lookup in level-only script domain. Original prefix CompileQuests cache28 failures remain latched.
- `source_quest_service_binding.hpp/.cpp`: source frame sync14/online-host/script admission gates, real online/difficulty collection selection, original progress fields2c/38/44, actual sync-owner callback before SG_Update, source SG_GetQuestByID state0 alias. Bounds/assertion/recheck precedes CompileQuests; mode2 fails, mode0 invalid bound returns genuine NULL. Ordered allquests live loop invokes actual `quest_allcomplete` trophy leaf only if source states exceed12.
- `construct_source_quest_owner`: initializes both original arrays on an EXISTING Character-bound fresh Save; rejects duplicate arrays. Use this helper only where source lifecycle genuinely owns fresh InitQuests; canonical selected-profile factory below intentionally does NOT pre-initialize arrays before Load2.
- `construct_source_condition_runtime`: constructs one native Condition arena from retained decoded actual tables plus genuine PM/local/current-Level/quest/event/assertion services; refuses replacing an existing owner slot.
- `construct_source_objective_runtime`: constructs one native GameEventRuntimeV75 around the actual loaded GameEventManagerV50 Level194 and actual Level/constants/script/project-event callbacks; refuses replacing a slot or incomplete native manager load. Mechanic/inventory/network leaves remain actual providers and fail on reach if absent.
- `canonical_quest_graph_factory.hpp/.cpp`: validates real CanonicalCharacterCandidateRecordV60 actor identity, source Save14e8 cell, SAME SaveLoad and quest-sync owner. `prepare_canonical_quest_profile(record,tables,CharacterProfileBootstrapInputsV59,out,error)` requires PM InitializePlayerSavegame already done. It constructs Quest facade only; injects it into actual CharacterProfileBootstrap read inputs and runs original bootstrap prepare masks. Deferred mask2 stays native InitPost; no duplicate Initialize replay. Actual record.profile_bootstrap stores the sole bootstrap, including interrupted source prefixes. `bind_canonical_quest_character_services` derives native profileQuest/difficultyGlobal/try_sync callbacks from actual record resolver. `publish_canonical_quest_runtime` requires initialized same profile quests, actual complete Level194 pointer, matching inherited Level EventManager identity, shared original Condition/Objective owners and marker transport, calls ObjectiveRuntime.attach_manager_v75 for SAME-manager validation, then publishes native quest runtime once.

Tests currently passing:

`port/windows-foundation/features/quests/run_component_tests.ps1`

Native llvm-mingw Windows C++17 static full linked adapter/service-binding test. Output: `PASS 64 original rows across 6 collections/difficulties; same-owner objective/progress and native compile prefixes`. Tests use actual source native quest table fixture `port/level-world/reference/character-menu-profile-v51/cache/v2quests_pyarray.bin` and names. Checks source rows/IDs/text/objectives/rewards, SAME saved objective/progress fields, monotonic Act, native ordered compile and interrupted cache28 prefix, native actual RewardList stop-on-false, missing runtime/reward/frame/unlock rejects, script suffix/no-dot semantics, same Save factory duplicate rejection, sync14 gate, native progress leaf, missing sync/registration provider failures, and assertion mode0/mode2 bounds behavior. Full canonical factory/profile dependency closure is NOT linked or live-tested by this test.

Factory compiler check passed with no warnings in owned code:

Compiler `.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe`; `-std=c++17 -Wall -Wextra -Dfinite=_finite`; includes game-data, level-world, engine-ui, scene-materials, engine-resources, engine-animation, engine-skinning, engine-textures, script-runtime; Box2D header directory `port/physics-backend/box2d-2.0.1/Include` supplied as system include. Output ignored at `features/quests/test-output/canonical_quest_graph_factory.o`. Factory requires linked native `CharacterProfileBootstrapV59::prepare` and `PlayerSaveQuestSyncOwnerV3::try_sync` closure when root adopts it. Don't misreport syntax/object compilation as full linked acceptance.

Source authorities / aliases:

- `CanonicalCharacterCandidateRecordV60::initialize_player_save`: original PM endpoint creates Save, source Save14e8 store then SG_SetCharacter, SaveLoad and sync owner.
- `CharacterProfileBootstrapV59::prepare` + `CharacterMenuProfileLoadV51::init_quests`: correct profile/mask initialization order and SAME regular/volatile owners.
- `QuestTablesPersistenceV51`: actual v2Quest read504a94; no invented maps/rewards/triggers.
- `NativeQuestRuntimeV76`: source compile480178/update state/script/objective/reward pipeline.
- `NativeConditionRuntimeV69`: same actual quest state0/currentLevel194 predicates.
- `source_campaign_conditions_v70.cpp`: SG_GetQuestByID native bounds/assertion/recheck then compile; never infer completion for missing provider.
- `source_campaign_quest_frame_v108.inc`: actual frame gate, progress fields2c/38/44, suffix scripts, reward/trophy source leaves.
- `EventManagerOwnerV12::raise_async`: literal339090 tails synchronously to Raise338ebc, no clone/queue required. Interactions worker now owns exact QE_TalkToNPC fields + mutable scoped GameEvent projection in `features/interactions/npc_source_services.hpp`. My early queue/lifetime advice was explicitly corrected to the worker.

Root graph requests / known blockers:

Encounter worker (`/root/act1_inventory`) and menu worker (`/root/character_state`) confirmed no live canonical PlayerInfo660 / CharacterSave14e8 / PlayerSavegameV1 / CharacterMenuQuests graph in current root GameSave/CharacterState/actor-roster projection. They expose only provider seams/projections. Integration lead's `/root/integration_lead/source_character_owner_factory` is constructing the actual canonical Character owner; it has received this API. Root must publish actual PM/canonical Character source graph, actual immutable tables, Level inherited EventManager and actual loaded Level194 GameEventManager. Condition and Objective arenas are ONE world/level owners reused across quest/encounters/interactions. Character reward/dialog/transition-save/IsLocalPlayer/online/TrophyManager leaves remain required actual producers. No fake sync14, gold or reward defaults.

Acceptance audit (`/root/acceptance_audit`) owns whole campaign codec/staging. It has been told to stage actual CharacterProfileBootstrap/quests/native registrations against STAGED Level dispatcher, then nofail publish whole graph. Raw Quest load retains reached writes on truncation; counts and exact consumed bytes must be validated. Never publish detached second QuestPersistenceOwner alongside live menu owner.

Next bounded task for successor:

1. Read the actual source-character factory agent's current public API and wire `prepare_canonical_quest_profile` in its source PM/profile order, with real profile/read inputs; integrate only within assigned quests module or communicate root seams.
2. Link the canonical factory through the real native profile/sync dependency closure and run a meaningful constructed-source-owner test. Do not add test mocks that claim live acceptance.
3. Supply source ConditionRuntime quest_state leaf via binding and consume interactions' actual QE_TalkToNPC scoped payload through SAME Level EventManager/native Objective runtime; prove one actual authored objective changes SAME saved fields and source native state transitions/save/reward calls proceed in captured order.
4. Record live done criterion and strict remaining gaps in report. Current status is `component_verified_live_campaign_integration_incomplete`.

`ROOT_INTEGRATION.md` and `CONTRACT.md` provide additional API/serialization details. Feature report `reports/feature-quests.json` needs a final append describing condition/objective construction helpers added at this checkpoint; current report already includes canonical factory and its object-only validation limitation. No root files have been edited.
