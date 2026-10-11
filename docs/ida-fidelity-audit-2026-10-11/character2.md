# character2 audit (Character:: 0x3acd58-0x3b7acc, 146 functions)

## Progress

- last_address_done: 0x3b7acc (whole batch triaged; deep comparison done for combat-result group, F_ApplyResult, potion, Ctrl_Click, faery, F_GOAttack, Lua binding coverage; InitPost at overview level)
- groups_done: all groups have a mapping-table row
- next (if resumed): not deep-compared -> F_ApplyResult(GameObject*) 0x3b01b8, scrolling-combat-text 0x3af77c / combat-sound 0x3afee0 numeric tables, CreatePlayer/CreateNPC, InitPost field details (0.009/0.01 scale fields, props 194/210/211), _InitSkillsSlots, SafeGetCharPropsId, Ctrl_HeadTowards(point) 90-degree target clear wiring.

## Summary

The combat kernels (result calc, melee/spell/skill/dot masks, apply logic, potion) are close transcriptions of the IDA code. The weakness is wiring: the Windows runtime (combat_system.cpp / combat_session.cpp) consumes only the damage amount and a few outcome bits.

Approximate per-function status (146 fns): faithful ~30, partial ~22, missing ~22 (mostly Lua natives + GameObject attack), divergent ~3, unclear ~12, n/a ~57 (thunks, stubs, camera/FX/sound init, UI), intentional-adaptation 0 (F7 might qualify if documented).

Top risks:
1. F1 (high): stun/fear/slow/DoT/leech/threat/dodge-block-knockdown counters from F_ApplyResult are not applied in the live Windows melee path.
2. F3 (high): trap / hazard damage (F_GOAttack + GameObjectDamager table, _DealDamages, TriggerTrap, ProjectileTrap, moth urn fire) has no port implementation.
3. F4 (medium): after 100 lifetime potions, a potion is consumed but the heal is skipped and an error returned.
4. F5 (medium): ~25 Character Lua natives are catalog-only in the Session binder (HasMana/UseMana partly bound; RegenHP/RegenMP/SetKnockBackState/AllowSkillBreak/AllowRotation/EnableSpotTargeting/CanAttackFromRange/HasBow/GetPropBonusDamage/Pause/ResumeTimer... not).
5. F2/F6/F7 (medium): gold-steal flag rejected; SnS_CleanUp class not applied after alternate melee; Ctrl_Click and the "walking away >90 deg drops target" rule not wired (PC uses own targeting).

## Findings

### F1 [HIGH] Live melee hit applies only HP damage (+ injury/push/text/sound); F_ApplyResult status tail not wired
- IDA 0x3b10b4 Character::F_ApplyResult(AttackResult const&, Character*, Character*, bool): after HitFor: leech RegenHP(attacker, res+16)/RegenMP(res+20) (runs even when defender died); if defender alive: PROPS_AddDot(defender, dur>>8, amt, element) when dur>0 && amt>0; dodge -> SM_SetDodgingState + prop215+1; block -> SM_SetBlockingState + prop214+1; hurt -> SM_SetInjureState(special=mask&0x18000000); push -> SM_SetKnockBackState(mask>>20&1) + prop222+1; stun -> SM_SetStunState(attacker prop 140, or 185 if mask&0x1000, >>8, 1); fear -> SM_SetScareState(prop 143 / 187 if mask&0x4000, >>8) if nonzero; slow -> PROPS_DebuffSlow(prop 146 / 189 if mask&0x10000, >>8). Also AI_AddAggro(defender AI, attacker, ThreatPerDamage*amount/256), and both AIs notified (AI vtable+180) unless mask&0x20000000.
- Port kernel (faithful): port/game-data/combat_application.cpp:5-40 and combat_result.cpp:43-66. Live path: port/windows-foundation/combat_system.cpp:151-176 (consume_marker -> apply_actor_damage) and combat_session.cpp:1434 (log text: "only direct health application is active"). No windows-foundation code consumes CombatResult hp_leech/mp_leech/dot_*; stun/fear/slow exist only in the unwired kernel; enemy_ai/enemy_aggro.cpp has no damage->threat path; features/status_effects/status_effects.hpp:39 states the F_ApplyResult continuation is a required, not-yet-supplied service. Consumers that do exist: actor_combat_runtime.cpp:244 (hurt bit -> injury pose with 3000 ms gate), features/combat/b035_session_push_effect_v1.cpp (push), combat_text, audio.
- Consequence: gear/skill stun, fear, slow, life/mana leech, DoT riders and damage-driven aggro do not occur on Windows; dodge/block/knock-down stat counters (props 215/214/222) and their trophies never increment; the player-idle "hit becomes hurt/dodge" rule is not applied in the live path.
- Confidence: medium-high (negative greps; an unseen wiring in main.cpp via indirect call is possible).
- Fix: route PlayableCombatResolution.melee.original through dh2_combat_apply_player_to_monster / apply_monster_to_player and consume status_requests (stun/fear/slow/dot/leech/threat/counters).

### F3 [HIGH] Trap/hazard damage (F_GOAttack / GameObjectDamager) absent
- IDA 0x3b061c F_GOAttack(result, GameObject attacker, Character defender, damagerIdx, a5): row = Arrays::GameObjectDamager (24-byte rows): rnd = Random(row[3]+1-row[4]); damage=(rnd+row[4])<<8; if row[2] (immunity ms) !=0 and row[1]>=0: defender ImmunityTimer(row[1]) must be expired else damage 0, then timer started with row[2]; hurt flag (0x10) set if defender prop134 < a5<<8; result word 9 = row[5]. Callers: GameObject::_DealDamages 0x3906fc, ProjectileTrap::_HandleProjectile 0x39cca8, TriggerTrap::Update 0x39eedc; applied via F_ApplyResult(GameObject*) 0x3b01b8.
- Port: no implementation. grep for GameObjectDamager/F_GOAttack/ImmunityTimer in windows-foundation, level-world, game-data finds nothing; level-world/canonical_trigger_trap_v37.* only stores the damager index (damager3fc_) and is not in the Windows CMake. features/interactions/README.md:119 acknowledges the moth urn GameObjectDamager call is not integrated.
- Consequence: floor traps, projectile traps, trigger traps and scripted DealDamages (e.g. breakable urn fire burst) deal no damage; per-hazard immunity windows missing.
- Confidence: high that no port code exists; medium on how many Act 1 objects use it.
- Fix: implement F_GOAttack from the GameObjectDamager table (damage range, immunity timer slot/duration, resist roll vs prop134) and a GameObject-attacker F_ApplyResult path.

### F4 [MEDIUM] 100th potion consumed without healing, then error
- IDA 0x3ad6f0 Ctrl_UsePotion: RemoveOnePotion; if player: prop219 += 1, trophy "use_100_potions" if count > 99; then always RegenHP(-1), RegenMP(-1).
- Port: features/inventory/runtime_session_potion_use_v1.cpp (~lines 238-252) returns consumed_prefix_failure ("original trophy producer is unavailable after source count99") when (prop219>>8) > 99, before RegenHP/RegenMP, publishing the consumed potion with no heal and returning false.
- Consequence: after 100 lifetime potions each potion is consumed with no heal (and an error). The vitals-full gate (HP or MP ratio < 1.0) mirrors the HUD native NativeUsePotion 0x43d2c8 (not Ctrl_UsePotion itself) - correct.
- Confidence: high. Fix: skip/stub the trophy and continue to RegenHP/RegenMP.

### F5 [MEDIUM] Character Lua binding coverage
- IDA Character::createBindings 0x3b56bc registers ~45 natives (0x3b5678-0x3b7acc). Port binds by original address in level-world/character_script_session.cpp:296-368 (+ session_v2/v3, source_owner_scripts.cpp, native_skill_services, character_skill_native_v5, character_player_skills_v3). Implemented: ClearTarget 3b5690, Stop 3b56b4, GetTarget 3b6c7c, GetPropHP 3b6cd8, GetState/GetStateTime, HasTarget 3b6f50, StopTimer/StartTimer, RegisterAnim, SetLevel 3b73a4, UseMana/HasMana (skill_native_v5), faery element/level/spellinfo, HasMaster/IsMasterHostPlayer, HasShield.
- Address appears only in .inc catalogs (no implementation found): _ClearMaster 3b5678, _LookAtTarget 3b5684, _GetMaster 3b6c70, _GetSpotTarget 3b6c88, _GetHitCount 3b6cc8, _GetCharID 3b6d5c, _GetCurrentEquippedFaeryId 3b6da0, _GetCharAIFlags 3b6e80, _IsConnected 3b6ec4, _CanAttackFromRange 3b6f10, _CanAttackInMelee 3b6f64, _HasBow 3b6f84, _TargetInMeleeRange 3b7020, _HasAggro 3b7044, _EnableSpotTargeting 3b7144, _ResumeTimer 3b74a8, _PauseTimer 3b751c, _SetKnockBackState 3b7680, _RotateBy 3b7704, _RegenMP 3b7774, _RegenHP 3b77ec, _AllowSkillBreak 3b7974, _AllowRotation 3b79ec, _SetIsTargetable 3b7a64 (character_targetability_owner_v1 exists; binding unverified), _GetPropBonusDamage 3b7acc.
- Consequence: authored Lua (monster/boss/NPC/skill scripts) calling these natives hits the "unsupported" stub. monster.luac AI runs through a native translation (features/enemy_ai/README.md), so impact depends on which natives shipped scripts call; I could not enumerate them without decompiling the .luac set. Boss/skill scripts using SetKnockBackState/AllowSkillBreak/AllowRotation/RegenHP/HasMana are the most likely to be affected.
- Confidence: medium (catalog vs implementation inferred from grep). Fix: enumerate natives referenced by shipped .luac and bind them.

### F2 [MEDIUM] Gold-steal damage (mask 0x400000) rejected
- IDA 0x3b10b4: if mask&0x400000: amount = min(gold(a2[231])<<8, amount); ItemInventory::AddGold(-(amount>>8)) on a2+892 (a2 = attacker argument, verify direction before implementing), then normal damage continues.
- Port: combat_application.cpp:7 returns error 1 for mask&0x400000; level-world/character_skill_application_v6.cpp:60, character_dot_application_v7.cpp:52, combat_ctrl_kill_owner_v1.cpp:46 also abort. Consequence: any gold-stealing attack aborts instead of stealing. Unverified whether shipped data sets bit 22. Confidence: medium.

### F6 [MEDIUM] F_MeleeAttack alternate path: SnS_CleanUp class not applied
- IDA 0x3b3368: when alternate (a5) true, ClassTable entry "SnS_CleanUp" is applied to the attacker (PROPS_ApplyClass) before result calc; mask 0x5554A vs 0x22AAB5 (asm 0x3b33f4-0x3b3408 confirms).
- Port: mask constants identical (combat_result.cpp:64) but SnS_CleanUp appears nowhere (grep); original_combat_properties.hpp:52 calls it a separate lifecycle service. Consequence: the buff/state that class clears remains after the alternate strike. Confidence: medium (depends on which skills set alternate).

### F7 [MEDIUM / unclear] Player click targeting and target-break not wired
- IDA 0x3addc8 Ctrl_Click: picks nearest non-neutral interactive Character within DesignSettings radius (+44 enemy / +80 other; DPad mode enemies only), else on release (non-DPad) nearest non-Character object within radius +84, else sets target null, SyncLastTarget and MoveTo/HeadTo; while casting/using a skill it only buffers the click. IDA 0x3adb60 Ctrl_HeadTowards(dir): if move direction is more than 1.5708 rad from the vector to the current target, the target is cleared.
- Port: kernel level-world/world_click_target_v1.cpp:20-55 matches the pseudocode line by line but is not referenced from windows-foundation. PC uses targeting.cpp / target_selection_runtime.cpp (cone/camera selection) and a sticky target in combat_session.cpp. No document found calling this an intentional adaptation. The 90-degree "walking away drops target" rule has no equivalent found (level-world/character_heading_owner_v1.cpp not referenced by windows-foundation). Confidence: medium.

### F8 [LOW-MEDIUM] Faery acquisition side effects (documented gap)
- IDA 0x3ae7d4 SetFaeryState: first non-zero faery unlock on difficulty 0 (offline, settings flag +43) starts script "cinematic_Tuto_faery" once. 0x3ae61c IncFaeryLevel: CharAI::UpdateAllSkills, trophy "faery_charged" when all levels > 0. 0x3ae99c ChangeFaery: UpdateAllSkills + reload model.
- Port: features/faery_menu/character_state_faery_v1.cpp:35-62 writes only state/level bytes; character_state_faery_v1.hpp:15-30 documents that cinematic, trophy and UpdateAllSkills are NOT routed and difficulty is fixed to 0. Consequence: tutorial cinematic absent; skill refresh after faery level-up depends on other code (unverified).

### F9 [LOW / unclear] InitPost details
- IDA 0x3b4d60: spawn-probability gate (CheckSpawnProbability < +0x274, else Delete), SG_Load, props load/recalc, fields +288..296 = values*0.009/0.009/0.01, player: _InitEquipment (AddLoot of prop9 default gear if empty), ResetGearsProperties, _InitSkillsSlots (skill 0 level >= 1), +932=999999999, +936=max(prop194,0), Revive, _InitHpMp (full regen), AI script init, AddToGroup.
- Port: spawn probability in features/enemy_ai/runtime_source_population_v1.cpp and level-world/application_spawn_random_owner_v4.cpp; init vitals in level-world/character_script_init_vitals.cpp; features/enemy_ai/README.md (older note) says whole-actor InitPost remained incomplete. Not compared field-by-field (evidence missing for consumers of props 194/210/211 and the scale fields).

## Verified faithful (no finding)
- 0x3b2638 _F_CalculateResult: roll order (miss/dodge, block, crit shared with hurt roll, push/stun/fear/slow pairs), damage types 0/2/3 and which result words are written match port/game-data/combat_result.cpp:43-61; miss type rule ((mask&5)!=0 physical else magical) matches line 48.
- 0x3b3368 F_MeleeAttack masks and weapon category (equipped item +148); F_SpellAttack mask|0x1005554A (hotty_character_cast_v1.cpp:128, celest_source_use_v1.cpp:159); F_DotAttack 0x20080000 + direct amount (level-world/character_dot_attack.cpp:34-48); F_SkillAttack |0x8000000 (character_skill_attack_v6.cpp:28).
- 0x3b10b4 F_ApplyResult kernel (combat_application.cpp:5-40): dead clears 0x160, idle-player hit->hurt/dodge, push-death bit, stun/fear/slow property selection and >>8 durations, leech before dead check, special=mask&0x18000000, dot args. Not verified: IDA gate `a3[68]==-1` before HitFor (unknown field), debug GOD/NoDamages and defender flag byte +5360 (likely invulnerable) skip; multiplayer difficulty scaling (n/a single player).
- 0x3ad6f0 Ctrl_UsePotion decrement/prop219/RegenHP(-1)/RegenMP(-1) except F4.
- CSM_Interrupted/StopSkill AllowSkillBreak bit16: combat_session.cpp:373-375.

## Mapping table

| IDA address | IDA function (or group) | Port location(s) | Status | Note |
|---|---|---|---|---|
| 0x3acd58,0x3acea4,0x3ad064,0x3ad1f8 | CreatePlayer/CreateNPC | features/actor_frame/*, level-world factories, runtime_source_population_v1 | unclear | construction not compared |
| 0x3ad22c-0x3ad2c8 | CSM_Attack/Injured/Interrupted/StoppedAttacking/StopSkill/CanStopInteraction/AfterInteraction | combat_session.cpp:373 | partial | interaction callbacks not checked |
| 0x3ad2e4 | CSM_Spawn | enemy_ai/runtime_source_population_v1.cpp | unclear | respawn/group CanSpawn gate not traced |
| 0x3ad430 | CTRLIsAllowed (scared/stunned blocks input) | none (no stun/scare state) | missing | follows from F1 |
| 0x3ad410,0x3ad418 | Ctrl_RotateBy | actor_movement.cpp | n/a | heading add |
| 0x3ad470,0x3ad478 | Ctrl_OpenIGM | pause_ui | n/a | UI toggle |
| 0x3ad520,0x3ad528 | Ctrl_Kill | level-world/combat_ctrl_kill_owner_v1.cpp | faithful | |
| 0x3ad574,0x3ad57c | Ctrl_DropObject (RaiseEvent 6) | features/interactions | unclear | |
| 0x3ad5ac,0x3ad614,0x3ad690 | ForceUseOOI/UseOOI/Ctrl_UseOOI | features/combat/object_of_interest_owner_v1.cpp | unclear | idle/moving gate not compared |
| 0x3ad6f0 | Ctrl_UsePotion | features/inventory/runtime_session_potion_use_v1.cpp | partial | F4 |
| 0x3ad834-0x3ad87c | Ctrl_EndCast/BeginCast/EndSkill/BeginSkill/Attack stubs+thunks | n/a | n/a | |
| 0x3ad890-0x3adb60 | Ctrl_Stop/HeadTowards/LookAt/MoveTo/HeadTo | level-world/character_heading_owner_v1.cpp, controller_policy.cpp | partial | F7 |
| 0x3adcf4 | Ctrl_WarpTo | level-world/character_path_commands.cpp | unclear | |
| 0x3addc8 | Ctrl_Click | level-world/world_click_target_v1.cpp (unwired) | unclear | F7 intentional-adaptation only if documented |
| 0x3ae5a0-0x3ae614,0x3aeac0 | GetCharFaeryListId/List/GetFaeryCharacter/GetCharFaery | game-data/faery_tables.cpp | unclear | asserts/table lookups |
| 0x3ae61c,0x3ae7d4,0x3ae94c,0x3ae99c | IncFaeryLevel/SetFaeryState/IncAllFaeriesLevel/ChangeFaery | features/faery_menu/character_state_faery_v1.cpp | partial | F8 |
| 0x3aea9c | IsCastingPossible | generic_skills cast coordinator | unclear | |
| 0x3aef68,0x3af0c8,0x3af1fc,0x3af77c | F_ApplyScrollingCombatText Gold/XP/GO/Char | features/combat_text/*, level-world/character_combat_text_v1.cpp | unclear | styles tested; numbers not compared |
| 0x3af3b0 | _F_ResetResult | combat_result.hpp defaults | faithful | |
| 0x3afd38,0x3afee0 | F_ApplyCombatSound | level-world/character_combat_sound_v1.cpp, features/audio/runtime_combat_audio_v1.cpp | unclear | |
| 0x3b01b8 | F_ApplyResult (GameObject attacker) | none | missing | F3 |
| 0x3b061c | F_GOAttack | none | missing | F3 |
| 0x3b10b4 | F_ApplyResult (Character,Character) | game-data/combat_application.cpp (kernel); live path health only | divergent | F1, F2 |
| 0x3b2638 | _F_CalculateResult | game-data/combat_result.cpp:43-61 | faithful | |
| 0x3b2e68 | F_DotAttack | level-world/character_dot_attack.cpp | faithful | |
| 0x3b3004 | F_SpellAttack | faery_menu/hotty_character_cast_v1.cpp:128 | faithful | |
| 0x3b31a4 | F_SkillAttack | level-world/character_skill_attack_v6.cpp:28 | faithful | |
| 0x3b3368 | F_MeleeAttack | game-data/combat_result.cpp:62-66 | partial | F6 |
| 0x3b35f0,0x3b36b0,0x3b36e4,0x3b36ec,0x3b3d38 | InitAll/InitializePlayerSavegame/UpdateInventoryLocalization/SafeGetCharProps* | player_profile_properties.cpp, character_state.cpp | unclear | |
| 0x3b379c | InitSpawned | enemy_ai/runtime_source_population_v1.cpp | unclear | |
| 0x3b3928,0x3b4bc4 | InitCoopCam/InitCam | gameplay_camera.cpp | n/a | camera |
| 0x3b395c,0x3b3a90 | _InitEquipment/_InitSkillsSlots | level-world/player_initial_grants_v2.cpp | unclear | skill0 level>=1 default |
| 0x3b3a70 | _InitHpMp | level-world/character_script_init_vitals.cpp | faithful | full regen |
| 0x3b3b00,0x3b4088,0x3b4738,0x3b4834,0x3b4978 | _InitSounds/InitPhysicalObject/RegisterCharacterFXTable/InitLightSetAndMaterial/InitFinal | audio/physics/effects features | n/a | init plumbing |
| 0x3b4d60 | InitPost | see F9 | unclear | F9 |
| 0x3b56bc | createBindings | level-world/character_script_session.cpp:296-368 | partial | F5 |
| 0x3b5678-0x3b7acc | Lua natives (45 fns) | see F5 | partial | implemented subset only |
