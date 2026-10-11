# AI batch fidelity audit (batch `ai`, 436 functions)

## Progress
- last_address_done: 0x52db48 (PFWorld::FindPath); all gameplay groups sampled
- groups_done: CharAI::_UpdateAggro/_UpdateTarget/_UpdateMaster, AI_IsEnemy/InSight/Range/CanAttack/MeleeRadius, AISMonster::OnUpdate (leash), AISDefault range handlers, AISFaery, AISPlayer, AttackDelay, PFWorld path kernels (FindPath/MovePath/SmoothPath/IsPastWaypoint/DropPath); port side: runtime_enemy_controller_v1, runtime_enemy_navigation_v1, character_update_aggro_v108, game-data ai.cpp/aggro.cpp, companions source_companions
- next: audit complete at sampling depth. NOT deep-read: CharAI groups (AddToGroup/GroupInfo), SetSkillsAndSpells, AI_DoRangeAttack, PFWorld::_SearchGraph/_CalcObstaclesForce/AvoidObstacles/ValidateDirection numeric compare, CharAISkillScript cooldown/GetInfo, monster.luac probabilities (port has Lua differential tests)

## Summary
Context: the port has two layers. (A) Kernel layer (`port/level-world/character_*`, `port/game-data/ai.cpp|aggro.cpp`, `level-world/navigation_*`, `features/companions/source_companions.cpp`, `features/enemy_ai/monster_decisions.cpp`) is a near line-by-line translation of the original and, where compared, is faithful. (B) The LIVE enemy driver that runs in the Windows build (`features/enemy_ai/runtime_enemy_controller_v1.cpp::update`, instantiated from `main.cpp` ~2582) is a much simpler per-tick controller that does NOT call the kernel layer (the enemy_ai README says native AIS lifecycle enrollment is still unfinished). Most player-visible divergences are layer B vs original.

Status counts (grouped, approximate, 436 functions): faithful ~41 (kernel, sampled), divergent 1 group, partial 6, missing 1 (leash), unclear ~12 groups, n/a ~330 (ctors/dtors, RTTI, getters, DBG, empty AIS stubs, Lua registration), intentional-adaptation 0.

Top 5 risks:
1. No leash/return-home in live driver (AISMonster::OnUpdate, AI row LeashDistance never consumed).
2. No threat/aggro table in live driver: single target_id, no AddAggro on damage, no switch factor, no highest-threat retargeting.
3. Live driver only drives AI rows with script=="monster", melee only: ranged/caster monsters walk into melee; other scripts get no AI.
4. Aggro scan every tick instead of 100-299 ms random delay + MyTurn queue; acquisition order/visibility differ from ObjectSearcher.
5. AISFaery/companion AI exist as faithful but dormant kernels (no live caller) - Faery never follows/warps.

## Findings

### F1 (high) Leash / return-to-spawn missing in live monster driver
- IDA: `AISMonster::OnUpdate` 0x3dd588: after `AISDefault::OnUpdate`, if `SM_IsIdle`, no current target (+1032 == 0) and not `GameObject::IsNearby(pos, spawnPos(+5200), 1.0)`, computes dist^2 between spawnPos and `GetTargetPosition`; if `AITable[id]+28 (LeashDistance)^2 < dist^2` then `v2Controller::Cmd_MoveTo(controller, spawnPos)`.
- Port: `features/enemy_ai/runtime_enemy_controller_v1.cpp` update() (~361-508): no-target branch only runs acquisition; no home position stored, no leash compare. `leash_distance` is parsed in `game-data/ai.cpp` but has no consumer (grep).
- Consequence: monsters that lose the player stay where they lost him; kiting pulls enemies permanently out of position.
- Confidence: high. Fix: store spawn position per actor; in idle/no-target branch replicate the three-condition test and MoveTo(spawn) through RuntimeEnemyNavigationV1.

### F2 (high) No threat table / retarget logic in live driver
- IDA: `CharAI::_UpdateAggro` 0x3cf3f0 monster branch: if highest-threat and current target both non-null and differ, `AI_GetAggro(highest) > AI_GetAggro(cur) * DesignSettings[+8]` then `AI_SetTarget(highest)`. Else requires target is enemy; Search at ViewRadiusNoAggro (row+64); empty -> `ClearAggro`, `SetTarget(null)`; local player in list with no outgoing relation and is enemy -> `AI_AddAggro(local, DesignSettings[+48])`. AddAggro/SetAggro 0x3d7c68/0x3d79ec with reciprocal incoming table.
- Port kernel: `level-world/character_update_aggro_v108.cpp` (faithful translation) and `game-data/aggro.cpp:dh2_aggro_apply/query` (faithful). `features/enemy_ai/enemy_aggro.cpp` wraps it, but `runtime_enemy_controller_v1.cpp::update` never calls it; it uses `owner->target_id` as sole authority ("No parallel aggro list", ~line 428).
- Consequence: enemies never switch to a higher-threat attacker (faery, companion, summon); target retention uses ViewRadius, not the original NoAggro-radius enemy search.
- Confidence: high that live path bypasses kernel; medium on exact in-game impact. Fix: route update() through update_aggro + target-update kernels with a real aggro owner fed by damage events.

### F3 (high) Ranged/caster monsters and non-"monster" scripts not driven
- IDA: `_UpdateTarget` 0x3cb908 raises 14/15/16/17 by `vtbl+292` (CanRange): range -> IsInCloseRange->16 / IsInRange->15 / else 14; melee -> IsInMeleeRange->17. `AISDefault::OnTargetInRangedRange` 0x3dc560: Stop then Attack. `AI_DoRangeAttack` 0x3d076c.
- Port: `runtime_enemy_controller_v1.cpp` ~358 `if (!ai || ai->script != "monster") continue;`; only `original_melee_in_range` separates attack vs approach (~450). No ranged band, no events 15/16.
- Consequence: archers/mages close to melee before attacking; other-script AI rows idle. (Bosses may be handled in features/bosses - not verified: unclear.)
- Confidence: high.

### F4 (medium-high) Aggro scan cadence/RNG
- IDA 0x3cf3f0 (non-player, non-faerie): if `!IsMyTurn && deferCounter(+12) < 500` only decrement timer(+8)/add dt and return; else counter=0; unless debug `DisableAIDelayTimerOptim`, timer counts down by dt and re-arms with `abs(seed%200)+100` (LCG 59051*s+177149 mod 0xDAF26B) = 100..299 ms.
- Port kernel reproduces exactly (`character_update_aggro_v108.cpp` lines 12-27). Live driver scans every Session tick and consumes no RNG.
- Consequence: instant aggro reaction (original up to ~0.3 s staggered latency); RNG stream shifts for later GetRand users.
- Confidence: high.

### F5 (medium) Sight/melee use different position and radius sources
- IDA: `AI_IsInSight(obj)` 0x3d4ed8 uses `GetTargetPosition` for both; `ViewRadius^2 > d2` (0x3d4ea0). Melee radius 0x3d4c34 = ItemInventory::CanMeleeAttack range + row MeleeRadius(+32); `AI_IsInMeleeRange` 0x3d6188 `(rA+rB)^2 > d2` only when target is enemy (else interaction range, padding AI+24, 80 for non-characters).
- Port: `inside_view` (`runtime_enemy_controller_v1.cpp:~22`) uses `transform.position`; `dh2_ai_range` (`game-data/ai.cpp`) sums only row melee radius. `PlayableActorWorld::make_binding` (`playable_actor_world.cpp:71`) adds weapon reach once at bind (original dynamic).
- Consequence: small sight-radius shifts; weapon swaps not reflected. Confidence: medium (GetTargetPosition offset not measured). Fix: use `original_actor_target_position.cpp` in inside_view.

### F6 (medium) Acquisition ordering and visibility
- IDA: spotted loop iterates ObjectSearcher::TargetList (sorted queue, mask 0x80000001, radius ViewRadius / NoAggro / `spawn_view_radius` field when awaiting spawn). Each candidate raises Enemy 9 / Friend 7 / Neutral 8; ALL enemies raise 9; no enemy and still holding a target raises 12.
- Port live: stable ActorId order, `break` at first target (~507), own comment: not list60 ordering. LOS is a custom Moller-Trumbore over `_colbox_` triangles (`runtime_enemy_navigation_v1.cpp:line_of_sight`). Awaiting-spawn custom radius not implemented.
- Consequence: wrong victim with multiple candidates; ambush spawns trigger at wrong distance. Confidence: medium; visibility equivalence unclear (ObjectSearcher::TargetList::Search not in this batch).

### F7 (medium) Approach/path refresh and stuck handling
- IDA: FindPath 0x52db48 = DropPath, store target, _SearchGraph, _SmoothPath, _CalcWaypointVec; MovePath 0x52d838 pops past-waypoint edge (dot >= 0), re-smooths. AISDefault range handlers re-issue MoveTo on events.
- Port kernels (`level-world/navigation_path.cpp`) faithful (soft-float order, line-intersection kinds 3/4/5). Live `runtime_enemy_navigation_v1.cpp::approach`: repath only when target id/exact position changed; no route -> idle; additionally cancels to idle when any non-floor triangle crosses the next waypoint (rule not in MovePath). Stuck timer exists only in monster.luac translation, not wired.
- Consequence: monsters may freeze idle when a leg grazes a _colbox_ or target unreachable. Confidence: medium.

### F8 (medium) Attack cooldown and in-combo target re-pick
- IDA: `CSAttack::OnBlur` 0x3c3f74 starts timer 42 for `AITable[id]+4` ms; `CSAttack::OnFocus` sets flag `+1320 |= 1` blocking `AI_DoMeleeAttack` until `OnAttackDelayExpired`. `AI_DoMeleeAttack` 0x3d01ac re-picks target via cone Search (ConstTable angle/2 * 0.017453) and requires `AI_CanAttack`.
- Port: `combat_session.cpp:1382` `cooldown_seconds = attack_delay*0.001`, timing `attack_departure` (units match; start timing belongs to combat batch). Cone re-pick absent in live controller.
- Consequence: monsters never switch to another in-cone victim mid-combo. Confidence: medium.

### F9 (medium) Faery / follower AI dormant
- IDA: AISFaery 0x3de318 (self+master idle -> HeadTo(master.pos - look*200)), 0x3de450 (HeadTowards delta), 0x3de4c8 (out of sight -> WarpTo), 0x3de4ec (friend spotted, no master -> SetMaster, link, ChangeFaery), 0x3de544 skin refresh; `_UpdateMaster` 0x3cc5a4 events 18-25.
- Port: `features/companions/source_companions.cpp:dispatch_faery` mirrors them exactly (200.0f, negated look vector); `source_master.cpp` master-sight faithful. No reference from `main.cpp`; BUGS-AND-IMPLEMENTATION I006 lists companions as unwired.
- Consequence: no Faery follow/warp in the live build. Confidence: high (kernel), medium (not wired; grep only).

### F10 (low) Combat-music trigger in AISPlayer::OnAggro
- IDA 0x3ddb70 accumulates `CharAI row +20` into a counter, at >= DesignSettings[+20] sets music state "combat". Port: not found in enemy_ai; audio batch may own it. unclear.

### F11 (low) AI_IsEnemy edge rules
- IDA 0x3d574c: both players -> not enemy; faction row lookup `value>>31`; non-Character targets via target vtbl 136/144 (==8). Port `game-data/ai.cpp:ai_enemy` matches for characters (clamps invalid faction to 10 where original asserts). Non-Character branch handled elsewhere (not verified).

## Mapping table
| IDA | IDA function (or group) | port location(s) | status | note |
|---|---|---|---|---|
| 0x3cf3f0 | CharAI::_UpdateAggro | level-world/character_update_aggro_v108.cpp; features/enemy_ai/enemy_aggro.cpp; live: runtime_enemy_controller_v1.cpp:update | partial | kernel faithful, live bypasses it (F2,F4,F6) |
| 0x3cb908 | CharAI::_UpdateTarget | game-data/ai.cpp:dh2_ai_target_update; level-world/character_target_update.cpp | partial | event order matches; live does not call (F3) |
| 0x3cc5a4 | CharAI::_UpdateMaster | companions/source_master.cpp, source_companion_events.cpp | partial | dormant (F9) |
| 0x3d574c | AI_IsEnemy | game-data/ai.cpp:ai_enemy; playable_actor_world.cpp:eligible_target | faithful | F11 |
| 0x3d5a98/0x3d511c | AI_IsNeutral / AI_IsFriend | level-world/character_world_ai_neutral_v1.cpp, character_world_ai_relationship_v1.cpp | unclear | not compared |
| 0x3d4ed8/0x3d4ea0 | AI_IsInSight | game-data/ai.cpp:dh2_ai_range; runtime_enemy_controller_v1.cpp:inside_view | divergent | F5 |
| 0x3d6188/0x3d63d8/0x3d6604 | AI_IsInMeleeRange/CloseRange/Range | playable_actor_world.cpp:original_melee_in_range | partial | melee ok; close/range unused live (F3) |
| 0x3d4c34 | AI_GetMeleeRadius | level-world/character_attack_geometry.cpp | faithful | weapon + row+32 |
| 0x3d4f98 | AI_IsInInteractionRange | features/interactions/original_interactions.cpp:43 | unclear | |
| 0x3d67f4 | AI_CanAttack | level-world/character_world_ai_can_attack_v1.cpp; combat_session.cpp:1024 | faithful | by structure |
| 0x3d6890 | AI_SetTarget | runtime_enemy_controller_v1.cpp:set_contact_target | partial | |
| 0x3d79ec/0x3d7c68/0x3d6d68/0x3d72d0/0x3d4a18/0x3d4ac8 | Set/Add/Clear/Get Aggro, GetHighest | game-data/aggro.cpp | faithful | unused live (F2) |
| 0x3d5fa8/0x3d6abc/0x3cd384 | ClearAllAggro variants | level-world/character_aggro_clear_all_v2.cpp | unclear | |
| 0x3dd588 | AISMonster::OnUpdate (leash) | none | missing | F1 |
| 0x3dd4cc-0x3dd550 | AISMonster event overrides | features/enemy_ai/native_monster_endpoint.cpp | faithful | per README; not live-wired |
| 0x3dc300/0x3dc560/0x3dc698/0x3dc284 | AISDefault range handlers | native_monster_endpoint.cpp, monster_decisions.cpp | partial | differential-tested vs monster.luac; not live |
| 0x3d01ac/0x3d076c | AI_DoMeleeAttack/DoRangeAttack | level-world/character_ai_attack.cpp | unclear | F8 |
| 0x3d3e44/0x3d4044 etc | _OnAnimStep* attack combo | level-world/character_animation_ai.cpp, character_ai_events.cpp | unclear | |
| 0x3ce044 | SetSkillsAndSpells | external_monster_natives.cpp, character_skill_ai_v3.cpp | unclear | |
| 0x3d7e88-0x3d8cfc | AI_Skill*/Spell* | level-world/character_skill_ai_v3.cpp | unclear | |
| 0x3da3d0-0x3db16c | CharAISkillScript::* | character_skill_ai_v3.cpp, scoped_skill_callback.cpp | unclear | |
| 0x3d2628-0x3d37d0 | GroupInfo / Add/RemoveFromGroup | level-world/character_ai_groups_v87.cpp | unclear | |
| 0x3de318-0x3de544 | AISFaery::* | companions/source_companions.cpp | faithful | dormant (F9) |
| 0x3dda34-0x3dde48 | AISPlayer::* | character_player_aggro_owner_v1.cpp, character_world_player_aggro_v2.cpp | unclear | F10 |
| 0x3dccd0-0x3dd440 | AISExternal::* | authored_monster_session.hpp + script runtime | n/a | Lua dispatch plumbing |
| 0x3d8e78-0x3da144 | CharAIScript state registration | script-runtime | n/a | boilerplate |
| 0x3cb34c-0x3cf3a4 misc | CharAI ctor/dtor/script load/pointers/queue | level-world/character_ai_*.cpp | n/a | lifecycle |
| 0x3d0b80-0x3d2410 | CharAI::On* forwarders | level-world/character_ai_events.cpp | n/a | thin |
| 0x3dbe78-0x3dbee8 | AISDefault empty stubs | - | n/a | 4-byte stubs |
| 0x4c74dc-0x5030b8 | Structs::AISetHP/AISetInt | struct readers | n/a | serialization |
| 0x52db48 | PFWorld::FindPath | level-world/navigation_path.cpp:find_path_source_v1 | faithful | |
| 0x52d838/0x528868/0x52899c/0x52d538/0x52aae4 | MovePath/_IsPastWaypoint/_CalcWaypointVec/_SmoothPath/DropPath | navigation_path.cpp | faithful | |
| 0x52b560/0x52cb90 | PFWorld::_SearchGraph | navigation_search.cpp, navigation_world.cpp | unclear | |
| 0x525944/0x525d60/0x525d84 | ValidateDirection/ValidatePosition | navigation_world.cpp, navigation_heading.cpp | unclear | |
| 0x527660/0x527cc4 | _CalcObstaclesForce/AvoidObstacles | navigation_avoidance.cpp | unclear | |
| 0x51b874-0x523c14 | PFFloor/PFRoom/PFWorld load/link/query | source_module_floors.cpp, navigation_* | unclear | |
| 0x5222xx DBG_* | PFWorld/PFFloor debug | - | n/a | debug |
| 0x341bcc / 0x388218 | SortModuleByDistance / PFRoom::ExtendBoundingBox | - | unclear | |
