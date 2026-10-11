# B083 - skill (slot 2) not usable (after death/revive)

## Evidence
- Video (P1 mp4, t~478-494 s, 2 fps tile sheet): original Android HUD shows one red skill orb bottom-left, move orb, attack, potion; no 1/2/3 buttons (our 1-3 buttons/keys are the PC adaptation). No clear skill cast was identified in the footage, so skill-trigger timing was NOT verified against the video.
- IDA: NativeHUDSkill 0x43e734: CTRLIsAllowed (0x3ad430: not scared/stunned) -> SG_GetSkillInSlot != -1 -> v2Controller::Cmd_BeginSkill 0x405a20 (controller not blocked) -> CharAI::AI_IsSkillUsable 0x3d8358: rejects if state==6 (using skill, unless +1312&0x8000), state==7 (casting), script not loaded, else CharAISkillScript::OnSkillCheck_Usable 0x3da9dc (Lua OnSkillCheck: mana, cooldown, target). AI_BeginSkill 0x3d86bc then SM_SetSkillState -> event 50005 -> state 6.
- CSInjured::OnInit 0x3c8850 registers 50005->state 6 with no guard, so the original does NOT reject a skill while Injured.

## Reproduction (real EXE, quiet_run, `--test-player-hp 10` death fixture, key 2 via --skill-key-frame)
- Normal play: key 2 casts BashDown (frames 40/120; frame 80 correctly "already casting").
- After death + Continue + revive: every key-2 press logged `diagnostic=CharacterState/Session vitals do not match the same live source MP property sheet`, no cast (frames 500,540,600,700). Hero idle (before: scratch Dbefore/f520.png).

## Root cause
Skill cast prepare (runtime_skill_cast_prepare_v1.cpp character_vitals_match) cross-checks the ActorState vitals against the source property sheet (cells 36/41/38/43). Revive (deathWire step 4, `revive_actor_state`) refilled only the ActorState mirror (MP 27.25), while the sheet kept the pre-death MP (21.25 after the first cast): permanent mismatch, so skills (all slots) stayed unusable until a restart/transition.

## Fix
- `PlayableActorWorld::revive_actor` (playable_actor_world.cpp/.hpp): revive ActorState AND refill sheet cells 36/41 to 38/43 (additive saved/resolved bookkeeping as in game_save normalize; -1 sentinel cells untouched). Used by the death-flow revive and the spawn-pool respawn in main.cpp.
- Test: tests/playable_actor_world_tests.cpp (sheet+mirror refilled, maximum/saved bookkeeping, unbound actor rejected).

## Verification
- After: same fixture, key 2 at frames 500/600/700 casts BashDown (MP 21.25 -> 15.25 -> 9.25), frame 540 "already casting" (cooldown/cast lock), f520.png shows the cast pose (D/ in .local-inputs/claude-v20.1/fix083).
- Build exit 0 (-Jobs 2); ctest: only session_skill_binding fails (known worktree issue).

## Not changed / not verified
- coordinator still rejects skills while action==hurt (runtime_skill_cast_coordinator_v1.cpp:408, "unverified" comment); IDA CSInjured suggests the original allows it. Left alone (retained-pose coupling); candidate follow-up.
- Slots 1/3 empty is the fresh character (only one skill learned); keys 1/3 correctly do nothing.
- HUD mouse click on the skill button not exercised in this run (same uiInput.skills path as the key).
- Other desync paths of actor vs sheet (F9/save restore) not exercised.
