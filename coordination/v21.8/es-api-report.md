# v21.8 ES-API report (branch v21.8-es-api, worktree .claude/worktrees/es-api, subst M:)

Base: windows-foundation @ bb604d97. Build: C:/dh218/es-api-build (Release, llvm-mingw, CMake/Ninja 3.22.1).
Scope per ENGINE-SYSTEMS-PLAN.md section ES-API and the phase-1 items N8, N9, N10, N11, N12, N14 (paladin),
N9, N10, N12 (illusionist), E14, E15, Q19 (necromancer), E5, E6, E7 (assassin), E3, E6 (berserker).

## CTest

206/206 passed (baseline 205 + the new `runtime_skill_actor_api_v1` test). Log: C:/dh218/es-api/ctest.out.

## Items

Evidence key: UT = synthetic-world unit test (runtime_skill_actor_api_v1_tests.cpp); SESS = asset-backed session test
(tests/combat_session_actor_transition_tests.cpp, run_scripted_kill_retirement); RR = real hidden-window run (not done).

| Item | Result | Evidence |
|---|---|---|
| Profile PlaySound arm at Use (`use_sound_name`) | PARTIAL | Lua: prince_illusionist_figments.luac:84, timecontrol.luac:83. Coordinator records a sound cue at Use; main.cpp dispatches via runtimeAudio->play_named_plain. No row uses it yet. Build + CTest only; RR not done. |
| Named PlayFX on the caster at Use (`self_fx_name`) | PARTIAL | Lua: offensive_aura.luac:136 (PlayFX(BUFF_FX) on self). Cue recorded, dispatched via sourceEffectsFactory->play_set at the caster position. Build + CTest only. |
| Per-target PlayFX, unconditional (`target_fx_each_name`, `target_fx_each_non_player_only`) | PARTIAL | Lua: illusionist_eclipse.luac:80-86 (FX under `not IsPlayer`, no damage gate). Separate pass over ordered_character_targets, so rolls_combat=false rows still get it. Build + CTest only. |
| PlayFX at OnPre per target (`pre_target_fx_name`) | PARTIAL | Lua: paladin_divine_judgement.luac:66-72 (PopTargetList + PlayFX per target). Cue appended in begin_skill_cast_v1 after the pre query. Dispatched when the receipt is seen, which is at Use time or earlier, not at the exact OnPre call. |
| TargetListBackup(name) + TargetListSearch(R, A, name) | WORKED (UT) | IDA: `_TargetListBackup` 0x390bd4 stores the list, string branch of `_TargetListSearch` 0x38fcf0 searches the backup. `SkillTargetListBackupsV1` + `query_backup_character_targets_v1`. UT: backup overwrite/default/absent, non-member and out-of-range exclusion. Not wired into a Lua-equivalent arm. |
| SetTargetListCharacterFilter(Player) with self exclusion | WORKED (UT) | IDA: `_SetTargetListCharacterFilter` 0x390690 stores the raw number; search uses PlayerList when bit 0x80 is set. Atlas: AITargetList_CharacterFilter.Player = 128 (ai_pycst, proven). `query_player_character_targets_v1`. UT: enemy, dead, far, and self excluded; cone pi/2 drops the rear ally; origin override. The Enemy numeric value is not in the atlas. |
| Relative search origin `TargetListSearch(R,A,0,-40,0,true)` | PARTIAL | IDA five-arg branch: origin = caster + look*forward (+ lateral/vertical terms built from constants I did not decode). `relative_forward_search_origin_v1` covers forward only. UT: (1,2,3) + (-40 along -y) = (1,42,3). |
| IsTargetListEmpty / PopTargetList / GetSelf / GetTargetListSize | NOT IMPLEMENTED (as Lua bindings) | The coordinator's ordered vectors already carry these semantics; no Lua binding layer exists in the port. Atlas still lists them MISSING. |
| SetProp(HP, v) / GetPropHP (N9) | WORKED (UT) | `PlayableActorWorld::set_current_health` / `current_health`: writes sheet cell 36 (x256) with revive's saved-cell bookkeeping and mirrors ActorState::health. UT: 50 written and read back, saved cell moves, NaN and unbound refused, no kill on write. Native: PROPS_Set 0x3b7eac, _GetPropHP 0x3b6cd8. |
| Kill(killer) with attribution (E5, Assassination) | PARTIAL | `CombatSession::script_kill_actor(victim, killer, error)` records {victim, killer} for a kill that took effect; a duplicate records nothing. SESS: killer 1 recorded once, duplicate ignored. Trophy/event side effects of Character::Kill 0x3a5b18 are NOT modelled. |
| Stop() (N8) | DID NOT WORK | Native `_Stop` = `v2Controller::Cmd_Stop`. Port has only enemy-navigation / follower stop services bound in main.cpp (~6044, ~6102); no generic actor-level Stop seam was added. |
| GetEquippedFaeryElement (N10) | PARTIAL | `equipped_faery_element_v1` = list[current_faery] row, word 2 of scalar block (native +8; Android presentation reads the same word as element). Not unit-tested: needs a loaded FaeryTables asset, and no test loads it in this round. No EnergyBalance wiring (phase 2b). |
| Paladin/Illusionist/Necromancer/Assassin/Berserker engine needs (N11, N12, E14, E15, E3, E6, E7) | PARTIAL | N11/N12/E14 covered by the target arms above. E3/E6 covered by HP write and presentation arms. E7 (raw x256 level compare) and E15 (presentation dispatch) are documented, not separately implemented. |
| Q19 DeathClaw pre-cone default | CONFIRMED, no change | IDA: `TargetListSearch(RANGE)` with no angle passes pi as the cone argument (`1078530011`), which is a full circle. kSkillPiV1 in the profile table is therefore correct for the no-angle default. The "135 deg" row is not in this tree (DeathClaw is phase 2b), so nothing was edited. |
| Integration test (one skill through the new arms) | NOT DONE | Coordinator-level cue generation is not unit-testable here (it needs an asset-backed CombatSession). No row uses the new arms, so no real-run evidence exists. |

## Open findings for the lead

1. Cone units. IDA passes `(deg * 0.017453) * 0.5` (half angle) to `ObjectSearcher::TargetList::Search` for the two-argument form. The port passes the full cone in radians (`runtime_skill_target_query_v1.cpp`). Either Search doubles the value internally or the existing Charge/ColdRay/JumpKick cones are off by a factor of two. Not verified here (TargetList::Search body not read); flagged, not changed.
2. Atlas status to update: TargetListBackup, SetTargetListCharacterFilter (native-side arm exists in the port's query layer, not as a binding), GetPropHP/SetProp (HP cell path now exists in PlayableActorWorld).
3. Presentation cue dispatch uses a per-generation counter in main.cpp. The counter is not reset when the coordinator is recreated (lines near 6885, 8149); generations are assumed monotonic.
4. Atlas: the README "Engine functions to implement next" was read from the es-api worktree copy before the atlas moved to the primary checkout (coordination/atlas). Later atlas reads use the primary path, read-only.

## Files

- port/windows-foundation/features/generic_skills/runtime_skill_target_query_v1.{hpp,cpp}: backups, Player filter query, backup query, relative origin, SkillPresentationCueV1.
- port/windows-foundation/features/generic_skills/runtime_skill_use_profile_v1.hpp: five presentation fields appended after fixed_cooldown_ms (defaults; existing rows unchanged).
- port/windows-foundation/features/generic_skills/runtime_skill_cast_coordinator_v1.{hpp,cpp}: receipt.presentation_cues; OnPre and Use cue recording.
- port/windows-foundation/main.cpp: one dispatch block next to the existing target-hit FX block; one counter declaration.
- port/windows-foundation/playable_actor_world.{hpp,cpp}: set_current_health, current_health.
- port/windows-foundation/combat_session.{hpp,cpp}: script_kill_actor(victim, killer, error), script_kill_records().
- port/windows-foundation/features/faery_menu/faery_ability_registry_v1.{hpp,cpp}: equipped_faery_element_v1.
- port/windows-foundation/features/generic_skills/runtime_skill_actor_api_v1_tests.cpp (new) + CMakeLists.txt target.
- port/windows-foundation/tests/combat_session_actor_transition_tests.cpp: killer attribution assertions.
