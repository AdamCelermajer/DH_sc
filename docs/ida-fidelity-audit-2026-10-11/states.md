# states batch fidelity audit (CharStateMachine / CS* / CharAnimator / AnimController)

## Progress
- last_address_done: 0x3caf3c (all gameplay groups reviewed; remaining tail is boilerplate)
- groups_done: StateMachine generic stack (n/a); CharStateMachine SM_Set*/SM_Is*/Update/RaiseStateEvent; CSStunned/CSScared/CSKnockedBack/CSInjured/CSDead/CSDespawn/CSIdle/CSMove focus-blur-update-event; all CS*::OnInit registration tables compared tuple by tuple; CSM_* predicates (0x3ad22c..0x3ad2e4); IdleCommonUpdate; CSMove::UpdateType/OnUpdate; CharAnimator::Update/CalculateExtraTime/SetSpeed; Windows-exe link-set check; 20 GetNewState<CS*> factories (added by coordinator).
- next: nothing required. Optional deeper passes: CSSpawn/CSPreSpawn/CSReviving/CSRevived/CSInteract/CSLifting* bodies (not linked into Windows exe), AnimController/BlendedAnimSetController blend constants, CharAnimator::_SetAnimStep/_AddAnimTable.

Not-gameplay groups (n/a): CSemaphore*, CSignIn*/CSignInGLLive*/CSignInDummy*, CSocket* (network); SlideEventCharacter (UI); Arrays::*AnimTable read/finalize/readNames (data loading); generic StateMachine stack; all `~CS*()` dtors; trivial `bx lr` OnUpdate/OnEvent stubs; std::map/__copy instances; AnimController/AnimSetController/BlendedAnimSetController ctors/dtors/getters.

## Summary
Key architectural fact: the Windows exe (`port/windows-foundation/CMakeLists.txt`) does NOT compile the faithful CharStateMachine reconstruction. `level-world/character_state.cpp`, `character_state_owner*.cpp`, `character_native_fsm.cpp`, `character_native_effects.cpp`, `character_knockback_reaction_v1.cpp`, `player_injure_state_v7.cpp`, `character_idle_update.cpp`, `features/status_effects/*` and `android-native/app/src/main/cpp/source_campaign_character_fsm_v101.cpp` appear in no windows-foundation target (only e.g. `level-world/move_state.cpp`, `character_timers.cpp`, `character_buffs.cpp`, `character_skill_state_v4.cpp` are linked). The live Windows state machine is `combat_session.cpp` + `actor_combat_runtime.cpp` + `features/physics/session_actor_transition_v1.cpp`, which supports only original states 3/4/5/6/7/10/11/12 (the "verified boundary") and hard-fails on others.

Where the reconstruction library exists it was checked and is gameplay-faithful (constants, flags, ordering, registration tables, predicates all matched; see Finding 8).

Status counts (mapping rows): faithful 14, partial 9, divergent 1, missing 5, unclear 4, n/a 4, intentional-adaptation 0 (one covered inside a faithful row; Finding 7).

Top risks:
1. Stunned/Scared states and timers are not reachable in the Windows exe.
2. Knockback admission is narrower than IDA (player-only, Idle-only) so monsters never get knocked back and a moving/attacking hero is never pushed.
3. Despawn (corpse removal after Despawn_Delay) and Spawn/PreSpawn/Reviving/Revived/Interact/Lifting/Anim states have no Windows consumer.
4. Idle-hero hit-to-Injury/Dodge conversion (F_ApplyResult) not ported (documented note in RESOLVED-BUGS B013).
5. Injury 3000 ms gate only for source-profile actors.

## Findings

### 1. Stunned (9) / Scared (8) not live in the Windows exe -- severity high
- IDA `SM_SetStunState` 0x3c5ffc / `SM_SetScareState` 0x3c6144: boss (`CharAI+20` bit 2) immune; if machine gate bit 2 (stun) / 4 (scare) clear then `CharTimers::TMR_Start(duration,0,event 43/44)` and set the bit; anim = CharAnimTable row +140 (Stunned idx32) / +124 (Scared idx29) + stance; `_SetState(9|8, 50012|50013)` if force else `RaiseStateEvent`; mode sets flags 0x800/0x400. `CharStateMachine::Update` 0x3c628c re-asserts stun/scare (duration 0xFFFFFFFF) when gate bit set but state != 9/8. CSStunned::OnUpdate 0x3c549c: gate bit 2 clear -> `SM_SetIdleState(false)`. Focus flags 0x2202 / 0x2240. Caller `Character::F_ApplyResult` 0x3b01b8 takes duration from result property 140/185 (stun) and 143/187 (scare), `>>8`.
- Port library (faithful, unlinked): `level-world/character_native_effects.cpp:21-70`, `character_native_fsm.cpp:33-42`, `features/status_effects/status_effects.cpp:22-25`; matches IDA (index fallback 17 = GetCharAnimTableId 0x3a3228, boss flag `ai.flags&4`, timer events 0x2b/0x2c, flags, mode bits).
- Windows exe: no status_effects/native_effects target; `session_actor_transition_v1.cpp:199,264,315` reject states outside 3/4/5/6/7/10/11/12; no other stun/scare references in non-test windows sources.
- Consequence: stun/scare effects (monster or hero) never apply.
- Confidence: medium-high (build list + grep, not run). Fix: bind the existing effects owner into the Session transition consumer and add states 8/9.

### 2. Knockback admission narrower than the original -- severity high (partly known: B035)
- IDA F_ApplyResult 0x3b01b8 (pseudocode-all.c ~129900-130030): `if(defender IsPlayer && SM_IsIdle(false)) { if((flags&0x16)==0) flags |= dmg>0 ? 0x10 : 0x02 }` only auto-fills Injure/Dodge bits. The cascade after it (Dodge 0x2, Block 0x4, Injure 0x10, KnockBack flag 0x80 with great = mask 0x100000, Stun 0x40, Scare 0x20, Slow) runs for every defender. `SM_SetKnockBackState` 0x3c5ea0 refuses only bosses; anim row +36 (GreatKnockedBack idx8) with gate=0x18, else +72 (KnockedBack idx16) with gate &= ~0x18; `_SetState(10,50011)`.
- Port: `features/combat/b035_session_push_effect_v1.cpp` (~75-80) `source_push_gate_admitted = target_was_player && alive && source_state==3` (comment equates the whole status block with IsPlayer+Idle); `combat_session.cpp:2103-2108` requires `traits->is_player`, attacker, state 3 or 11.
- Consequence: monsters never knocked back; hero not knocked back while Moving/Attacking/Casting although the original pushes whenever the result has flag 0x80.
- Known open: BUGS-AND-IMPLEMENTATION B035. KnockedBack bodies (0x3c4a48 flags 0x2341, filter 0/0x51c/3 only if gate&8 and body, gate 0x10->0x20, controller lock, unpin; blur resetFilter+pin; event 0x23 dead check; 0x27 resetFilter) match `character_knockback_reaction_v1.cpp:21-41` and `session_actor_transition_v1.cpp:178-186,240-303`.
- Confidence: medium (origin of result flag 0x80 for monster-vs-hero hits not traced; combat batch should confirm). Fix: admit push for any defender carrying flag 0x80 regardless of current state, honoring registered predicate CSM_Interrupted (state 10: ((gate^0x20)>>5)&1).

### 3. Idle-hero hit conversion to Injure/Dodge not ported -- severity medium (documented)
- IDA as above. RESOLVED-BUGS.md B013 row lists it as an open divergence note. Consequence: an idle hero does not flinch from ordinary hits unless the result already carries 0x10.

### 4. Injury 3000 ms gate scope -- severity low/medium
- IDA `SM_SetInjureState` 0x3c5d84: if `Character+0x14fc > 0` no-op; else store 3000.0f (0x453B8000), anim row +60 (Injured idx14) + stance (mask 0x1000), `_SetState(11,50010)`; gate decremented by dt in Character::Update. CSInjured focus flags 0x2b41, blur `Cmd_LookAt(+408)`.
- Port kernel `level-world/player_injure_state_v7.cpp:12-53` faithful. Windows: `actor_combat_runtime.cpp:245-249,399-401` applies the gate only when `poses.source_injury_gate_enabled` (set in `combat_session.cpp:1251` for staged source actors); legacy bindings flinch on every flagged hit with no cooldown. Confidence medium.

### 5. Despawn / Despawn_Delay missing -- severity medium
- IDA CSDead::OnFocus 0x3c4d50 and OnEvent(34) 0x3c4c3c start timer event 46 with `CharacterDesign/Despawn_Delay` for non-players; CSDespawn focus 0x3c32fc (flags 0x200), blur 0x3c3794 deletes non-respawnable summoned/flagged characters, unloads script, purges animset.
- Port library has the timer (`character_state.cpp:57-60,87-89`), but no `Despawn_Delay` use in windows-foundation; corpses persist (MILESTONES.md:125). Confidence medium.

### 6. States without a Windows consumer; library gaps -- severity medium
- Spawn(1), PreSpawn(17), Limbus(0), Reviving(15), Revived(16), Interact(13), Anim(14), LiftingIdle/Move(18,19) are only in the unlinked library. Revive tracked as I024.
- Inside the library itself, `state_method_v101` / `state_update_v101` / `state_frame_v101` (`canonical_character_candidate_v60.hpp:59-61`) are never assigned, so Despawn/Reviving/Revived/Lifting bodies return -1, and CSInteract Liftable pick_up/release/drop paths deliberately fail (`source_campaign_character_fsm_v101.cpp:660-690`). Carrying objects and reviving are unimplemented even there.

### 7. Walk/run selection -- severity low
- IDA CSMove::UpdateType 0x3c0f18: hero walk<->run by squared heading magnitude vs DesignSettings +92 (PlayerRunToWalkPercent) / +96 (PlayerWalkToRunPercent); run anim row +112, walk +148; speed `PROPS_GetWalkSpeed` (raw/256*0.01+1, floor 0); NPCs always walk. Library `character_state.cpp:19-30` identical (`move_state.cpp` multiplier too). Windows: Shift-held run (documented intentional adaptation, MILESTONES.md:292; `actor_movement.cpp:118`). Library facts hard-code thresholds 0.45/0.85 (`model_renderer.cpp:1880`), not verified against DesignSettings (value not located in readable data): unclear.

### 8. Factories (GetNewState<CS*>) -- severity low
- 20 factories exist in IDA (CSLimbus, CSSpawn, CSDespawn, CSIdle, CSMove, CSAttack, CSSkill, CSCast, CSScared, CSStunned, CSKnockedBack, CSInjured, CSDead, CSInteract, CSAnim, CSReviving, CSRevived, CSPreSpawn, CSLiftingIdle, CSLiftingMove), each a 4-byte singleton getter. Port `character_state_owner_data.inc` `source_infos[20]` registers all 20 ids 0-19 with matching focus/blur/update/event addresses (checked 0-13 explicitly). So the library registers every state; the Windows exe supports only 8 of them (Findings 1, 2, 5, 6).

### 9. Verified faithful (no action)
- OnInit registration tuples for all 17 CS states with OnInit vs `character_state_owner_data.inc:3-109` identical.
- CSM predicates 0x3ad22c/23c/244/280/290/29c/2c8 vs `character_state_owner.cpp:67-78` (CSWTCH.5399 = {0,0,0,0} confirmed in full-listing.asm). CSM_Spawn 0x3ad2e4 not compared.
- RaiseStateEvent 0x3c5684, Update 0x3c628c, IsIdle/IsMoving, bit clearing for events 0x2a-0x2c, event 0x30 re-pin, elapsed reset: `character_state_owner.cpp:40-60`, `character_native_fsm.cpp:25-42`.
- SM_SetDeadState 0x3c58c8 (rows +16/+28, +24/+20, masks 0x20000/0x8000/0x40000/0x10000, awaiting-revive = state 0/16/17): `npc_dead_state_v1.cpp:7-33`; CSDead focus flags 0x241 (+0x2000 player), timer 46, filter 0/0x51c/3, events 0x2a,0x2c,0x2b: `character_state.cpp:50-67`.
- IdleCommonUpdate 0x3c0b78: `character_idle_update.cpp:16-37`.
- CharAnimator::Update 0x3caf3c event order 39,37 (skipped for non-final sequential step),35,34-once, loop counters: `game-data/animation_scheduler.cpp:85-135`. CalculateExtraTime / ANIM_SetSpeed not individually checked.
- CharAnimTable field index -> row offset mapping (idx i -> 4+4i; Interact/Spells are 8-byte vectors) consistent with `game-data/animation_tables.cpp:12,50`.

## Mapping table
IDA address | IDA function (or group) | port location(s) | status | note
--- | --- | --- | --- | ---
0x339e70-0x33a924 | StateMachine generic stack | n/a | n/a | UI/game-state stack
0x3bffe0-0x3c0084 | trivial CS* dtors, bx-lr OnUpdate/OnEvent | `level-world/character_state_empty.cpp`, `reference/character-state-methods/native-methods.inc` | faithful | empty-body catalog
0x3c0084-0x3c0420 | _HasState/_HasEvent/SM_Is* | `character_state.cpp:138-145`, `character_state_owner.cpp` | partial | IsIdle faithful; IsIncapacitated(8-11)/IsLifting(18,19)/IsAwaitingToRevive(0,16,17) no direct port
0x3c0454.. (20 fns) | GetNewState<CS*> factories | `character_state_owner_data.inc` source_infos | faithful | Finding 8
0x3c0b04 | CSSpawn::OnEvent | `character_spawn_owner_extensions.cpp` | partial | unlinked in Windows
0x3c0b78 | CSIdle::IdleCommonUpdate | `character_idle_update.cpp` | faithful | unlinked; single-player moot
0x3c0f18, 0x3c1184 | CSMove UpdateType/OnUpdate | `character_state.cpp:19-30,128-135` | faithful | Windows uses Shift run (Finding 7)
0x3c0ea0 | CSLifting OnUpdate | none | missing | Finding 6
0x3c1558,0x3c5548,0x3c4f8c,0x3c8984 | CSInteract | `source_campaign_character_fsm_v101.cpp:650-690` | partial | Liftable hard-fails; not in Windows exe
0x3c1600-0x3c1b58 | CharStateMachine ctor/_GetEvent/_GetState/_SetState | `character_state_owner.cpp` | faithful | blur -> focus -> event 0x1d order
0x3c1a14,0x3c1a3c,0x3c324c,0x3c2dd0,0x3c8b28 | CSAnim | `fsm_v101.cpp:615-619,832` | partial | not in Windows exe
0x3c1a74,0x3c2be4,0x3c2e58,0x3c7cc0 | CSLimbus | `source_campaign_limbus_body_v118` | partial | not in Windows exe
0x3c2734,0x3c35ec,0x3c2f7c,0x3c7d0c | SM_SetSpawnState / CSSpawn | `character_spawn_body.cpp` | missing | Windows; Finding 6
0x3c2810,0x3c67d4,0x3c688c,0x3c8d24 | CSPreSpawn | `character_pre_spawn.cpp` | missing | Windows; Finding 6
0x3c2b28,0x3c45c4,0x3c4788,0x3c4834,0x3c6144,0x3c861c | CSScared / SM_SetScareState | `character_native_effects.cpp` | partial | faithful lib, unlinked (Finding 1)
0x3c3b4c,0x3c3cc0,0x3c549c,0x3c5ffc,0x3c8730 | CSStunned / SM_SetStunState | `character_native_effects.cpp` | partial | Finding 1
0x3c48e0,0x3c4a48,0x3c5ab0,0x3c5ea0,0x3c87d8 | CSKnockedBack / SM_SetKnockBackState | `character_knockback_reaction_v1.cpp`; Windows `session_actor_transition_v1.cpp`, `b035_session_push_effect_v1.cpp` | divergent | bodies faithful, admission narrower (Finding 2)
0x3c33e8,0x3c4ba4,0x3c5d84,0x3c8850 | CSInjured / SM_SetInjureState | `player_injure_state_v7.cpp`; `actor_combat_runtime.cpp:245` | partial | Findings 3, 4
0x3c4c3c,0x3c499c,0x3c4d50,0x3c58c8,0x3c8920 | CSDead / SM_SetDeadState | `character_state.cpp`, `npc_dead_state_v1.cpp`, `canonical_character_death_v84.cpp` | faithful | combat_session maps dead to 12
0x3c32fc,0x3c3794,0x3c7dfc | CSDespawn | library only | missing | Finding 5
0x3c5294,0x3c6724,0x3c34cc,0x3c53ec,0x3c5880,0x3c58a4,0x3c8b8c,0x3c8cec | Reviving / Revived | none functional | missing | I024
0x3c3140,0x3c50b4,0x3c3e40,0x3c5198,0x3c8d5c,0x3c8ee0 | CSLiftingIdle/Move | none | missing | Finding 6
0x3c3020,0x3c2d3c,0x3c7e60 | CSIdle focus/blur/init | `character_state.cpp:41-49` | faithful | flags 0x2380
0x3c3bf8,0x3c3aa4,0x3c80ac | CSMove focus/blur/init | `character_state.cpp` | faithful | flags 0x23c1
0x3c5684,0x3c628c | RaiseStateEvent / Update | `character_state_owner.cpp`, `character_native_fsm.cpp` | faithful | unlinked; Windows has own dispatch
0x3c5b3c,0x3c5c60 | SM_SetDodging/Blocking | `character_defensive_state_v1.cpp`, `npc_injury_runtime_v1.cpp` | unclear | not read in detail
0x3c6394,0x3c6488,0x3c6670,0x3c64ac | SM_SetCast/Attack/Skill/Interact | `character_skill_state_v4.cpp`, combat_session | unclear | belong to skill/attack batches
0x3c71e0-0x3c7b18 | std::map / RegisterState / SM_RegisterEvent | `character_state_owner.cpp` ctor | faithful | table identical
0x3c8ff4-0x3caf3c | CharAnimator | `game-data/animation_scheduler.cpp`, `level-world/actor_playback.cpp`, `actor_blended_playback.cpp` | faithful (Update); unclear (_SetAnimStep/_AddAnimTable/extra time) | Windows retained animation owners
0x474678-0x476ddc | AnimController / AnimSetController / BlendedAnimSetController | `engine-animation/*`, `level-world/animation_blender.cpp` | unclear | blend constants not checked
0x4a9a9c-0x4b8174 | Arrays::AnimTable/CharAnimTable read/finalize | `game-data/animation_tables.cpp:12,50` | faithful | field mapping verified
0x41f6c4,0x4236c0 | SlideEventCharacter | n/a | n/a | UI
0x80e2cc-0x828b30 | CSemaphore/CSignIn*/CSocket* | n/a | n/a | network/sign-in
