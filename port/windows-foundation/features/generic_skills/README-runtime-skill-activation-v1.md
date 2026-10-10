# Generic source-skill activation visuals

`runtime_skill_activation_v1.hpp` resolves a selected class-list position from
the caller's same `CharacterState`, actual `CharacterTable`, saved SkillList
order, and pinned `SkillTables::Borrow`. It requires the exact saved rank to be
known, positive, and currently available, then reads the source `SkillTable.Anim`
root. No class name or animation mapping is hardcoded.

`play_skill_visual_request_v1` requires a preloaded plan and sequence policy for
that exact source root, preserves the caller's selection and marker callbacks,
and plays on the supplied actor in the existing `CombatSession` through
`play_actor_source_sequence`. It changes no CharacterState, mana, rank, slots,
health, or damage. The root must already be in the actor's source clip bank;
this helper does not reload or replace session visuals.

Native cast admission is separate. `CharAI::AI_IsSkillUsable` delegates to the
real `CharAISkillScript::OnSkillCheck_Usable`; cached BashDown Lua computes
`CalcManaCost(CLASS_ID, rank)` in `OnSkillUpdate_` and returns
`HasMana(mana_cost)` from `OnSkillCheck_`. Generic CharacterState/SkillTables do
not own that Lua/property graph or the current FSM casting state. Therefore
`request_skill_cast_animation_v1` does not guess a mana formula or accept an
unknown gate: it returns an explicit unknown/rejected outcome without playing.
Only a caller-supplied named source admission fact can start the visual
sequence. Even then, playback is not a claim of completed native cast, mana
spend, target resolution, damage, or status effects.

The authored Warrior Headsplitter is Knight SkillList position 0 → SkillTable
row 7 `BashDown` → SkillTable Anim root 347 (`Knight_BashDown`). The original
source sequence is type 0, non-looping, with the authored `do_skill` marker.
The exact source row and animation mapping are checked against the actual
shared asset tables. Source admission remains the responsibility of a genuine
provider over the original skill script/property/FSM owners.

`run_runtime_skill_activation_v1_tests.ps1` is an isolated actual-table test.
`runtime_skill_activation_session_v1_tests.cpp` additionally exercises
unknown/rejected admission and successful source sequence playback on one
asset-backed CombatSession, verifying the retained pose owner, marker, and
completion. The session test requires the coherent Foundation link target and
is kept separate from the table-only runner.

## Same-session BashDown target prefix

The connected Session fixture now also exercises the source target-query prefix on the existing player and two actual eligible enemy actors. `runtime_skill_target_query_v1.*` replays the authored character portion of `Enemy + AttackableOnly, FrontalFirst, TargetListSearch(160)` using the same Session actor positions, source cached target centers, heading, property sheets and melee reach. It takes the original population's actor order and same-actor visibility/zone/interactivity facts from its caller; it does not manufacture these facts or substitute the nearest cursor target. Character interaction radius is read from the same live property sheet's AI row (`resolved[1]`) and original `AiTables`, with the exact source fallback row8. The recovered comparator orders character candidates ahead of non-character objects and then by the source absolute facing angle. The fixture verifies that the farther frontal actor wins, that the Lua-local target is not written to `ActorState::target_id`, and that the original `LookAt(Point)` kernel changes only the same actor's heading.

This is still only a target-selection/turning prefix. A reached visibility, zone, or interaction fact that is missing returns unavailable. A missing Character AI row plus fallback8 returns unavailable; valid `PlayableActorWorld` AI tables always retain fallback8. `ActorState` currently has no corresponding visibility/zone/interactivity facts. When no character qualifies, the query returns `no_actor_target` only if the caller proves that no non-character `AttackableOnly` object is present; otherwise it reports `non_character_target_unknown`, because destructible objects are not actors in this world. The generic Session API also does not expose the source `LookAt` controller admission test. This helper does not claim `UseMana`, `SkillCombatRoll`, full damage/effects, or successful source-native cast completion.

`run_runtime_skill_activation_session_v1_tests.ps1` includes the target fixture and is pending the lead's shared Foundation build clearance. The changed target helper and connected test translation units pass strict standalone `clang++ -fsyntax-only`.

The isolated linked runner now passes against the coherent Foundation archives. It compiles the source LookAt leaf (`character_path_commands.cpp` plus its `navigation_heading.cpp` dependency) into the temporary feature test executable; it does not rebuild shared archives. Its output confirms the existing same-Session mana prefix, target-query checks, separate visual sequence, and one row7 source result applied through `CombatSession::apply_source_result` at `do_skill`. It still does not execute the full native F_ApplyResult/effect pipeline or prove a complete native cast.

The connected test additionally routes the actual BashDown row7 request at the authored `do_skill` marker through `CombatSession::apply_source_result` with the exact `SkillAttack` source mask bit and same-actor formula scratch. It verifies positive target HP removal and duplicate occurrence suppression without an extra RNG draw. Admission and GameObject predicate values remain test inputs, so this is not full cast admission or production query acceptance. The source result API's basic HP/injury/death application is not the complete `F_ApplyResult` tail.

