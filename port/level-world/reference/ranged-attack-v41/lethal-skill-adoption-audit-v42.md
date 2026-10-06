# Lethal skill production adoption audit V42

## Applied retained receiver correction

RetainedCharacterActorV1 now embeds one CharacterKillFieldsV21. Fresh canonical C1 constructs it. Existing-actor adoption deliberately leaves it unproduced. For the current development Crypt allocation, root must call construct_kill_fields_source_c1_v42 immediately after allocating a NEW MonsterScriptHandle at its genuine Character C1 prefix; that method initializes only kill fields and never touches runtime/PF/position. Restoration must not call it. Truly already-initialized actor adoption uses adopt_kill_fields_observed_v42 with exact observed values. Both paths reject replay. This is an additive native class layout change requiring coherent consumer rebuild.

New retained_character_kill_borrow_v42.hpp/.cpp composes the exact typed NPC borrow from the actual receiver lease, session property view, life, controller, FSM, canonical handle, kill metadata pointers and supplied SAME outgoing AggroTable. Failed preflight leaves output unchanged. Root passes its result to CharacterKillProductionV23::add alongside the actual contributor owner. Link additions: retained_character_kill_borrow_v42.cpp and character_kill_fields_v21.cpp if not already linked; existing retained_character_actor_v1.cpp consumes the latter. No root model/CMake edits were made here.

Strict six compilation checks passed (three current TUs, both ABIs). O1/O2 ASAN+UBSAN field fresh/observed/replay tests passed10 each. Full new retained graph/runtime borrow acceptance remains pending; these constructor checks alone are not live kill proof. Player metadata still requires root's corresponding sole fresh-only field publication.

Current live evidence: actor100000004 _prim_tmp_cultist05 HP1177 ->0, status-2; frame reports original Hit source service8. The current renderer_player_manager_v1.inc hit backend has no hit_controller_kill case. Its unconditional unknown-service failure explains this specific black frame before a complete source CtrlKill transition. No native crash or stale FX pointer is established by that log.

## Genuine ordered continuation

The Hit8 request supplies actual controller in subject, defender identity from HitActor, attacker in target and actual force. Pass those unchanged to CharacterKillProductionV23::command. CmdKill dispatches the actual controllable receiver; CtrlKill checks dead, then Kill checks dead and publishes dead+HP0 before external callbacks. CombatCtrlKillOwnerV1 commits that dead write to SAME CombatActorState before each callback. It supports source dead-gated reentry and retains failure mutations. CharacterKillLiveWorldV21 prohibits retry after a failed prefix. Do not reset dead, restore HP or retry rewards to escape an incomplete callback.

For NPC force0, exact order is current Level -> mandatory DropLoot when150=0 -> each actual threat contributor event4, counters and threshold trophies -> credited-killer XP -> actual remotely-updated/suppress gate -> immediate typed quest events -> victim Character RaiseEvent2. Event2 executes selected AIS/reciprocal threat cleanup/FSM12/death animation through existing RendererNpcDeathV2. It is not a broad world.notify_death broadcast and cannot be delivered before reward/quest completion.

## Same receiver contracts

CharacterKillLiveBorrowV21 requires retained receiver lease, SAME PropertyView/CombatActorState, actual controller with controllable character equality, shared canonical Handle, OID64, property13c8, tracked14a4, actual outgoing AggroTable and produced CharacterKillFieldsV21. RetainedCharacterActorV1 already provides kill_metadata_borrow_v23, shared_handle, session, object, controller and machine. The missing field owner is killer144c/master14d4/template13ca/suppress14e4: embed exactly once, with fresh C1 construct or explicit observed adoption. Source C1 stores are recorded in character_kill_fields_v21.cpp. Do not derive template from actor names, replace OID with Handle key or create independent life/OOI.

## Runtime providers root must compose before Hit8 publication

1. Real current canonical Level borrow and EventManager: KillLevelProviderV23 reads the sole currentLevel synchronously and pins receiver only through command. Preserve source150=0; no forced loot gate. RaiseAsync is immediate source Raise, so quest payload lifetime is the actual callback stack.
2. ONE RendererCharacterLootGameplayV23: bind World/PM/Gear/cache tables/powers/Application RNG/canonical manager/Scene/PF; initialize and genuine pending InitFinal145. Drop route delegates WorldLootGameplayV23::route. Its actual world items subsequently update on same scene clock, submit existing draw_scenes and interact through same Gear. Isolated pool1627 proof is not proof these live providers have been published.
3. Actual CharacterProgressionWorldV23 route at kill_distribute_xp. Requires genuine PM6c4, same saved/resolved actor sheets, Save/class/position/local/remote/currentLevel and level-up tails. Source count0 is not corrected by stamping1.
4. Contributor source callbacks: CharacterKillContributorEventV23 per registered Character, fresh selected AI fields and actual scoped event4 capability. Victim event2 uses renderer_kill_npc_raise_v21, not a copied Lua session or extra OnDied hook.
5. Actual trophy manager/constants/network locality/remotely-updated providers. Required threshold and positive branches remain explicit.

The existing renderer_character_kill_production_v23.inc performs exact Hit graph validation and command routing, but no current backend calls it and no runtime production owner is published. Hook only after these real owners exist; a callback-shaped function alone does not make the complete path available.

## Lifetime constraints

Keep World/actor receiver leases through all synchronous callbacks. Source death does not immediately erase NPC object, animation, registered Handle or anchored FX. RendererNpcDeathV2 retains actor/session/FSM and drops actual state/highlight/self FX through the same manager; timer2e later owns despawn. Do not remove source actor at HP0 or notify targets independently. Reciprocal target cleanup is source AIS delivery; keep current/last/OOI distinct. FX floor/anchor consumers must retain registered actor until their actual drop/despawn lifetime completes.

This audit supplies an exact blocked production composition contract; it does not claim lethal skills, drops, XP, quests or death animation accepted in the installed app. Root is collecting current runtime readiness and owns global integration.
