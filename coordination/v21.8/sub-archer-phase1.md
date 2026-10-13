# v21.8 sub-archer: Phase 1 investigation (Archer, Rogue tree, script prefix prince_archer_*)

Branch `v21.8-sub-archer`, worktree `.claude/worktrees/sub-archer`, base `windows-foundation` @ 7b9028cb. No engine code, no build, no subst letter used. Phase 1 only.

Evidence key: **Lua** = original skill script (plain-text Lua under `.local-inputs/assets-extra/android/data/scripts/skills/`; the `.luac` extension holds source, verified by header bytes). **Atlas** = Original Data Atlas (`.claude/worktrees/atlas/coordination/atlas/data/tables.json`, read-only; a data aid, not proof). **IDA** = offline export `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c` (the rea MCP was not used; the .i64 is not openable here). **Port** = our code on this base. Confidence: PROVEN / LIKELY / GUESS. Anything marked INFERENCE is my reading, not directly observed.

Scope: the Archer tree has 16 skills (SkillTree row 28, character row 326 `RoguePlayerBase_Archer`, AnimTable 50 `PlayerRogue`, `ClassID 120`). Archer-only: 5 actives and 3 passives. Shared with the Rogue base: 4 actives (JumpKick, Quickness, Roundhouse, ViciousStrike) and 4 passives (Acrobat, Ambidextrous, ComboMaster, FindWeakness). The shared ones are listed in section 3 for completeness and are not audited in depth.

## 0. Corrections to the inherited claims

1. **The five Archer actives are NOT blocked on this base by the projectile executor.** `coordination/v21.8/skills-rogue-report.md` (branch base aa2e8624) says the player skill path does not spawn through `RuntimeSessionProjectileV1`. That is stale on 7b9028cb:
   - `features/effects/runtime_session_projectile_v1.hpp` exposes `launch_skill(...)` (Lua SpawnSkillProjectile) with per-projectile `check` / `delayed` callbacks, an explicit target, and `RuntimeSessionSkillProjectileEventV1::expired`.
   - `features/generic_skills/runtime_mage_skill_owner_v1.cpp` (ChainLightning, Cyclone, StoneEscape) already calls `executor->launch_skill(...)` for player casts, and `main.cpp` (around lines 8520-8564) wires the same executor into `RuntimeMageSkillOwnerV1`.
   - What is missing for the Archer is the Archer's own Pre/Use owner (or data arm) and its registration. That is an engine-routing gap, not a projectile-system gap. The table in section 1 is updated accordingly.
2. **`external_owner` is a single slot.** `main.cpp` assigns `request.external_owner = mageSkillOwner.get()` for every cast, and `RuntimeSkillCastCoordinatorV1` consults `profile->external_owner` and `handles()`. Adding a second owner needs a registry (engine need N1), not a second pointer.
3. **Admission:** the Archer actives have no row in any `kRuntimeSkillUseProfiles*` table on this base (`runtime_skill_use_profile_v1.hpp` base table, `_knight_`, `_rogue_`, `_magesub_`). The coordinator therefore rejects them with its generic "no Use arm" detail. PROVEN by grep of the tables.

## 1. Archer-only actives (5)

Lua: all five are `RegisterSkill`/`RegisterSkillEx` scripts. Callback names below are the Lua function names. "Atlas ID" = ProjectileTable row index (`projectiles#group0`). Anim/Sound from the static checker (`coordination/v21.8/SKILL-VERIFICATION-MATRIX.json`) and atlas.

| Skill (SkillTable id) | Script | Lua logic (plain words) | Native / engine calls | Status on 7b9028cb (CAST / ANIM / FX / SOUND / DAMAGE / EFFECT) |
|---|---|---|---|---|
| ExplodingArrows (41, level 21) | prince_archer_exploding_arrows | Pre: target list Enemy, FrontalFirst, range 1200 angle 220; LookAt top; UseMana; SetSkillCooldown(SnS_Cooldown). Use (OnSkill_): pop target, SpawnSkillProjectile(ArrowExplode) (also when no target). Delayed(target): SetProp(SnS_Level), ApplyPropClass(CLASS_ID), SkillCombatRoll on the hit target; then StartSkillTimerCB(500) DoExplosion(target): TargetListSearch(200, 360, target) sorted ClosestFirst, plays FX spell_dh2_prince_rogue_burst_shot on the main target, PlaySound sfx_impact_burst_shot, rolls up to MAX_COUNT=3 victims. Post ClearTarget. OnSkillUpdate reads TempProp1 into RANGE_EXPL. | SpawnSkillProjectile (IDA `Character::_SpawnSkillProjectile` 0x3b99d0, Lua name bound as `SpawnSkillProjectile__`), StartSkillTimerCB, TargetListSearch with origin, SkillCombatRoll | CAST MISSING (no row/owner). ANIM: clip `skill_dh2_prince_rogue_archer_exploding_arrows.bdae` present (static check). FX: AnimatedEffectTable `spell_dh2_prince_rogue_burst_shot` resolves in atlas (table bd109770) but no `spell_dh2_prince_rogue_burst_shot.bdae` by filename in `assets-extra` (NONE). SOUND: `sfx_skill_rogue_arc_burst_shot` (clip marker) and `sfx_impact_burst_shot` (PlaySound) resolve to wav in staged package. DAMAGE/EFFECT MISSING. ProjectileTable row 4 ArrowExplode: model 10 `projectile_arrow_explode.bdae` present in assets-extra. |
| MultiHitArrow (75, level 18) | prince_archer_multihit | Pre: Enemy, FrontalFirst, RANGE 600, ANGLE 220; pot_targets = list size; LookAt top; UseMana; hit=0. Use (OnSkill_, called by THREE animation events): pop top target, SpawnSkillProjectile(ArrowDefault). Delayed(target): hit++; if hit==3 and pot_targets==1: SetKnockBackState(true) (big KB); if hit<3 and pot_targets==2: SetKnockBackState(false); SkillCombatRoll. Gate: HasBow(); HasMana(cost). | SpawnSkillProjectile, SetKnockBackState (IDA `Character::_SetKnockBackState` 0x3b7680), HasBow | CAST MISSING. ANIM `skill_dh2_prince_rogue_archer_multi_hit.bdae` present. FX: none in AnimTable. SOUND `sfx_skill_rogue_arc_rain_of_arrows` (marker) present. ProjectileTable row 3 ArrowDefault (model 9). KB: port has a SetKnockBackState path in `combat_session.cpp` for other sources (not wired to this skill; PARTIAL). |
| PinShot (83, level 15) | prince_archer_pin_shot | Pre: Enemy, FrontalFirst, RANGE 600 (no angle); LookAt top; UseMana; SetSkillCooldown. Gate: HasBow(); not HasSkillCooldown; HasMana. Use (OnSkill_): spawn ArrowPinShot at the first target; if count>1 and level>4: after 750 ms a second `PinShot` timer plays anim `skill_dh2_prince_rogue_archer_pinning_shot` and spawns another projectile at the second target; if count>2 and level>9: third at 1500 ms. Delayed(target): dedupe by name (first/second target), SkillCombatRoll; on hit target:SetStunState(duration, true) where duration = SnS_Duration (ms). Pinned count from level: 1 (<=4), 2 (5..9), 3 (>=10) written to TempProp1. | SpawnSkillProjectile(ArrowPinShot row 6), StartSkillTimerCB(750 and 1500), PlayAnim (RegisterAnim), SetStunState (IDA `Character::_SetStunState` 0x3b9580 -> `CharStateMachine::SM_SetStunState` 0x3c5ffc) | CAST MISSING. ANIM `skill_dh2_prince_rogue_archer_pinning_shot.bdae` present (RegisterAnim + the timer PlayAnim). SOUND `sfx_skill_rogue_arc_maim_shot` present. FX none in AnimTable. DAMAGE/EFFECT MISSING, STUN MISSING (no stun state in port, see N4). Note: the Lua uses the global `target` at line 117/125 (no local), harmless but noted. |
| PoisonArrows (84, level 15) | prince_archer_poison_arrows | Pre: Enemy, FrontalFirst, RANGE 600 ANGLE 220; LookAt top; UseMana (no cooldown). Gate: HasBow(); HasMana. Use: pop, SpawnSkillProjectile(ArrowPoison). Delayed: SetProp(SnS_Level), ApplyPropClass(CLASS_ID), SkillCombatRoll. The header states "Normal arrow damage + DOT"; the author note says the DOT "does not seem to work". The DOT is only what the class sheet does (Dot_* cells) and is not applied by the Lua. | SpawnSkillProjectile(ArrowPoison row 7), SkillCombatRoll | CAST MISSING. ANIM `skill_dh2_prince_rogue_archer_poison_arrows.bdae` present. SOUND `sfx_skill_rogue_arc_venom_shot` present. ProjectileTable row 7 ArrowPoison: ImpactFX 244 (`spell_poison_hurt`, effect_sets 244 -> file 216); the atlas's `spell_poison_hurt.bdae` is NOT in assets-extra by filename (NONE). DOT: UNKNOWN (Lua author note says not working; see open question Q4). |
| Volley (115, level 12) | prince_archer_volley | Pre: Enemy, FrontalFirst, RANGE 900 ANGLE 270; LookAt top; UseMana; resets oldTarget/newTarget. Use: pop top, first `ArrowDefault` projectile (remembered as `projectile`). Delayed(target, proj): dmg via SkillCombatRoll on the target (for the rebound projectile the Lua swaps the target back to newTarget because "SetProjectileTarget doesn't seem to work"); records oldTarget on first hit; then do_rebound(target): rebound chance = GetRand(0,100) < TempProp1 (from class, "chances"); if yes, TargetListSearch(RANGE 900, 360, origin=victim), NoSort, pops until a target whose name differs from oldTarget, spawns a second ArrowDefault at origin and marks it as rebound. | SpawnSkillProjectile(ArrowDefault row 3) x2, SetProjectileTarget (IDA `Character::_SetProjectileTarget` 0x3ba298), GetRand, TargetListSearch with origin | CAST MISSING. ANIM `skill_dh2_prince_rogue_archer_volley.bdae` present (staged: `assets/data/3d/characters/prince/animations/`). SOUND `sfx_skill_rogue_arc_pierce_shot` present. ProjectileTable row 3 (model 9, vel 45). Rebound chance = TempProp1 (prop 190) per the Lua; whether the Volley class row writes prop 190 is not confirmed (its decoded formula list was truncated, Q15). Lua `traces = true` (debug) left on. |

Bow-gated: ExplodingArrows, MultiHitArrow, PinShot, PoisonArrows and Volley all call `HasBow()` either in OnSkillCheck_ or Pre (ExplodingArrows, PinShot, MultiHitArrow, PoisonArrows, Volley). Port has no `HasBow` skill predicate (N7).

### Cross-skill facts
- Single `SpawnSkillProjectile` per Use in ExplodingArrows/PoisonArrows/Volley; multi per Use in MultiHitArrow (3 do_skill events) and PinShot (up to 3 projectiles through timers).
- Each Use reads one of three SkillCombatRoll consumers (`SkillCombatRoll(target)`) and `ApplyPropClass(CLASS_ID)`; the existing mage owner already implements this roll (`RuntimeMageSkillOwnerV1::roll`), so it is reusable.
- Cooldowns: ExplodingArrows and PinShot call `SetSkillCooldown(SnS_Cooldown)` in Pre (the coordinator already supports `sets_cooldown`, used by Charge/GroundSlam).

## 2. Archer passives (3 Archer-only + 4 shared)

All seven passives are `RegisterSkill(..., nil, ...)` with empty Pre/Use/Post. Behaviour is: `OnSkillUpdate_` (on skill level change) removes the previous buff and calls `CreateBuff(CLASS_ID, 0, false, slvl)` + `ApplyBuff`. Duration 0 means permanent (IDA `Character::_CreateBuff` 0x3b86a8; port `runtime_timed_buff_v1.hpp` says "Permanent buff: no expiry timer ... Ends only by RemoveBuff", so the port supports it). The class-row Formula values were decoded from the atlas (`character_classes#group0`) and the DestProp index names from `source_process_compiled_members_v121.hpp` (CharacterProperties member table). The formula evaluator is `port/game-data/class_tables.cpp` `dh2_class_formula`: type 1 gives `dest = (p1 == -666 ? dest : p1) + p3 * (src[p2] >> 8)` (PROVEN by reading the code; the raw x256 scaling is the same as the port's).

| Passive (skill id) | Owner | Lua | Class effect (raw x256 units) | Port status on this base |
|---|---|---|---|---|
| BowMastery (10, lvl 12) Archer | Archer | "Increases damage and attack rating when using ranged weapons". Buff `Skill_Archer_BowMastery`, strength = slvl. | DestProp 55 `Rating_Attack_Bonus_Range` += 1280 per skill level; DestProp 87 `Damage_Bonus_Range` += 512 per skill level. | NO EFFECT. Text only (`runtime_skills_text_v1.cpp`). Needs the permanent buff owner wiring and readers of props 55/87 in the player ranged hit formula (not verified, Q2). |
| Precision (87, lvl 21) Archer | Archer | Header: "Increase chance of a successful hit and critical hit". Dated note: "now the skill have a chance to stun with the weapons". Code: only when HasBow() and when the player's own OnTargetHit fires (StealAIEvent), creates buff; a 1000 ms timer `checkbuff` removes the buff when the player has no bow. | DestProp 145 `Slow_Target_Chance` += 512 + 64 per level; DestProp 146 `Slow_Duration` = 512000 + 64000 per level. | NO EFFECT. Needs OnTargetHit steal (N6), timer (N2), HasBow (N7), permanent-or-timed buff. The class writes SLOW props, not crit or stun. See Q3 (source conflict). The existing debuff-slow path reads property 146 on hits (`runtime_timed_buff_v1.hpp` lines 57-64), so this may work once the buff is applied (LIKELY, not verified). |
| SideStep (101, lvl 18) Archer | Archer | "Deflection": when a melee enemy attacks the player (OnTargetHit, defender == self and attacker:TargetInMeleeRange()), roll GetRand(0,100) < KNOCKBACK (= FromFixed(TempProp1)); if so start a 500 ms repeating timer `check` that calls attacker:SetKnockBackState(true), and stops the timer when the attacker's AI state is KnockedBack, or stops and calls attacker:Stop() when it is not Attack. | DestProp 190 `TempProp1` = 2560 + 256 × SnS_Level (Formula 1 with p2=172). KNOCKBACK = FromFixed(TempProp1) (about 10 + level percent; INFERENCE, Q5). | NO EFFECT. Needs OnTargetHit defender hook (N6), monster knockback (N5), repeating timer (N2), AI state query (`object:GetState`, `AIStates` constants). |
| Acrobat (1, lvl 3) Rogue | Rogue (shared) | "+2% dodge to melee" | DestProp 60 `Rating_Dodge` += 256 per level. | NO EFFECT (text only). Permanent buff wiring (N8). |
| Ambidextrous (2, lvl 0) Rogue | Rogue (shared) | "+2 AR"; at level > 6 the second-hand capability is granted (buff `Skill_Rogue_Ambidextrous_buff`). The Lua comment says "there is currently no commands in LUA to have that capability". | DestProp 81/82 `Damage_Min/Max_Off_Hand` += 256 per level; buff 204 sets DestProp 202 `Special_Equip_Main_In_Offhand` = 256 at level > 6. | NO EFFECT. The off-hand-main-hand equip capability is an equipment-layer change (N9). |
| ComboMaster (16, lvl 6) Rogue | Rogue (shared) | "+2 damage per combined attack" | DestProp 94 `Damage_Bonus_Combo` += 128 per level. | NO EFFECT. Needs the combo damage reader (not verified). |
| FindWeakness (46, lvl 9) Rogue | Rogue (shared) | "+2% critical hit chance" with daggers and ranged weapons | DestProp 63 `Rating_Critical` += 256 per level. | NO EFFECT. Needs the crit reader for ranged hits (not verified). |

Summary: 7/7 passives are text only on this base; the generic mechanism (permanent `RuntimeTimedBuffOwnerV1` + a class projection at level change) exists and is the right general route (N8). Only BowMastery, Precision and SideStep are Archer-specific; the other four are the shared Rogue passives (owned by the base-rogue agent).

## 3. Shared Rogue actives inside the Archer tree (not audited, for completeness)

JumpKick (56), Quickness (90), Roundhouse (97), ViciousStrike (114) are present in the base use-profile table (`runtime_skill_use_profile_v1.hpp` rows at lines ~127-145). Per the matrix they are CAST/ANIM/FX/SOUND present. Their runtime verification is outside this brief.

## 4. Bow basic attack (the Archer's weapon attack) and comparison with the Mage staff bolt

### 4.1 Mechanism (native path, shared by bow and staff)
- **Event dispatch:** `CharAI::_OnAnimEvent` (IDA 0x3d4434). In state 5 (attack), the named events `attack_ranged`, `attack_mainhand` and `do_skill` call `ProjectileManager::Spawn(projectile id from the owner's ranged lookup, owner, CharAI::_HandleProjectile)`. The ranged lookup is a virtual call at vtable slot +296 on the owner object (IDA lines ~152680-152710; the function behind slot +296 is not yet identified: INFERENCE that it returns the ProjectileTable row of the main-hand ranged item). Other events go to state handlers (state 6 `do_skill` -> `CharAI::_SkillEvent`, state 7 `do_spell`).
- **Port equivalent:** `combat_session.cpp:556-566` classifies the event with `classify_original_named_animation_event(name, state, ranged)` (`original_actor_animation_events.cpp` lines 9-13: in state 5 and `ranged`, `attack_ranged`/`attack_mainhand`/`do_skill` are `projectile`). `ranged` = `properties.sheets.resolved[32] != -1 || main_item.words[22] in {4,5}`. The projectile marker goes to `take_projectile_markers` (combat_session.cpp:2067) and then `main.cpp` `launchPlayerRangedProjectile` (line ~3747), which calls `runtime_source_range_selection_v1` and `RuntimeSessionProjectileV1::launch_state5`.
- **Projectile row selection:** `runtime_source_range_selection_v1` -> `dh2_attack_range_parameters` (`level-world/character_attack_geometry.cpp`): for a player with no fixed RangeProjectileID (`words[32] == -1`), the main-hand ItemTable row supplies `[words[38], words[39], words[40]]` = [min range, max range, ProjectileTable row], and only rows with `words[22]` (Type) = 4 (bow) or 5 (staff) are ranged (PROVEN by code). The atlas `Param4/Param5/Param6` columns of the item row are these same words (PROVEN by value match: Staff01 Param4=1, Param5=1000, Param6=20 -> FireWandProjectile row 20 whose timer is 1000 ms, matching the magefix2 report).
- **Ranged mode is not a separate code path:** the bow and the staff use the same `launchPlayerRangedProjectile`, the same executor and the same contact sensor. No bow-specific branch exists in the port or (as far as the event dispatch shows) in the original (LIKELY). This means the bow basic attack is probably already wired, but it has not been runtime-verified with a bow (see Q1).

### 4.2 Item rows (atlas `loot_table#group3`, decoded fields)
| Field | Bow01 (Archer) | Bow02 | Staff01 (Mage) |
|---|---|---|---|
| Type (words[22]) | 4 | 4 | 5 |
| Param4 = min range (words[38]) | 1 | 1 | 1 |
| Param5 = max range (words[39]) | 1000 | 1000 | 1000 |
| Param6 = ProjectileTable row (words[40]) | 10 `BowProjectile` | 10 `BowProjectile` | 20 `FireWandProjectile` |
| EnergyRequirement / DexterityRequirement | Dex 10 | Dex 13 | Energy 8 |
| AudioVisualID / SwooshSoundFX | 17 / -1 | 17 / -1 | 17 / 479 |
| ModularModule | MC_LWeapon_Bow_01 | MC_LWeapon_Bow_02 | MC_RWeapon_Quarterstaff_01 |

### 4.3 Projectile rows (atlas `projectiles#group0`; floats decoded from int bits)
| Row | Name | Model (ProjectileDict) | Velocity | Timer | Damping | TimeBetweenHit | ImpactFX (effect_sets) | ImpactSFX | ObjectImpactSFX | IsMagic | FriendlyFire |
|---|---|---|---|---|---|---|---|---|---|---|---|
| 3 | ArrowDefault | 9 projectile_arrow_default | 45 | 2000 | 0.25 | 1000 | 221 spell_dh2_hurt_regular_arrow | 291 env_weapon_arrow_hit | 292 env_weapon_arrow_miss | 0 | 0 |
| 4 | ArrowExplode | 10 projectile_arrow_explode | 45 | 2000 | 0.25 | 1000 | 241 spell_fire_hurt | 291 | 292 | 0 | 0 |
| 6 | ArrowPinShot | 12 projectile_arrow_pin_shot | 45 | 2000 | 0.25 | 1000 | 221 | 291 | 292 | 0 | 0 |
| 7 | ArrowPoison | 13 projectile_arrow_poison | 45 | 2000 | 0.25 | 1000 | 244 spell_poison_hurt | 291 | 292 | 0 | 0 |
| 10 | BowProjectile | 9 projectile_arrow_default | 20 | 2000 | 0.25 | 1000 | 221 | 291 | 292 | 0 | 0 |
| 20 | FireWandProjectile | 1 elemental_bolt_fire | 20 | 1000 | 0 | 1000 | 241 spell_fire_hurt | -1 | -1 | 1 | 0 |

The ImpactSFX/ObjectImpactSFX ids are rows of the sound-event table (`sdd_dungeon_hunter_2_iphone`, atlas audio.json context.row): 291 = `env_weapon_arrow_hit` -> `sfx_impact_arrow_flesh_1_22.wav`, 292 = `env_weapon_arrow_miss` -> `sfx_impact_arrow_object_1_22.wav` (both wav present in the staged package `audio-assets/data/sounds/`). PROVEN by atlas and filesystem.

### 4.4 Animation (what plays when the bow is drawn and fired)
- Stance: `Character::GetAnimStance` (IDA 0x3a53e0): HasStaff -> 3, HasBow -> `byte_4` (the value is 4, see INFERENCE below), dual -> 2, two-hander -> 1, none -> 5. The port reproduces it in `level-world/character_stance.cpp` (`stance_has_bow` -> 4). INFERENCE for the bow value: IDA prints the operand as `byte_4`, which is the data address 0x4 holding the constant 4; this matches the stance table (`Human_Idle_04_Bow`, `Human_Walk_04_Bow`, `Human_Run_04_Bow`, `Human_Attack_05_Bow`); confidence LIKELY.
- The stanced attack sequence = base AnimTable cell + stance when the AnimStancedAnim mask bit is set (port `runtime_player_combo_chain_v1.cpp` lines 172-177: bit 0x40 for Attack, 0x80 for AttackStatic). PlayerRogue Attack = 248 (`Human_Attack_00_1H`), AttackStatic = 243 (`Human_AttackStatic_00_1H`). So bow stance 4 gives 252 `Human_Attack_05_Bow` and 247 `Human_AttackStatic_05_Bow`; staff stance 3 gives 251 `Human_Attack_03_Staff` and 246 `Human_AttackStatic_03_Staff` (PROVEN for the arithmetic and names; the mask bits are read from the source table at runtime).
- Bow-specific clips in the original: `BowAttackStatic01/02/03` (animation_sequences 94-96) -> `prince_ranged_attack.bdae`, `_02`, `_03` (animations_dictionary 1077-1079, staged at `assets/animations/prince_ranged_attack.bdae`, present). Their state-5/6 events: `do_skill` at 1133 ms (`prince_ranged_attack`) and 1067 ms (`_02`, `_03`) per the atlas reference list (PROVEN for the event names, timings from the atlas).
- **Conflict (open, Q6):** the atlas resolves `Human_Attack_05_Bow` (252) and `Human_AttackStatic_05_Bow` (247) steps to animation-dictionary entries 94-96, whose names are `cs_darkqueen_scene03a_camera/faery/prince`, i.e. cutscene clips, which is implausible for a normal attack. The same atlas rows resolve `Human_Attack_00_1H` to `cs_swampking_intro_*`. So either the atlas index for these rows is wrong, or the stanced attack graph uses a different index space. The port computes the actual clip ids at runtime; a log line of the loaded attack clip names would settle it.
- Sound: the bow sequences carry `Sound = -1`. The shot itself has no sound cue in the sequence; sounds come from the projectile impact/miss rows (291/292) and any `sfx_` marker in the clip (not checked for the Prince bow clips: Q6).
- Swoosh: `SwooshSoundFX = -1` for the bow (Bow01), `479` for the staff (Staff01). The staff plays a swoosh, the bow does not (PROVEN by item row).

### 4.5 Side by side
| Aspect | Mage staff bolt (Staff01) | Archer bow basic attack (Bow01) | Same or different |
|---|---|---|---|
| Ranged predicate | words[22] = 5 | words[22] = 4 | Different constant, same predicate (PROVEN) |
| Range | 1..1000 | 1..1000 | Same |
| Projectile row | 20 FireWandProjectile (model elemental_bolt_fire, timer 1000, IsMagic 1) | 10 BowProjectile (model projectile_arrow_default, timer 2000, damping 0.25, not magic) | Different rows, same launch path |
| Impact FX / SFX | FX 241 spell_fire_hurt, no impact SFX | FX 221 spell_dh2_hurt_regular_arrow, SFX 291 arrow hit, object SFX 292 arrow miss | Different data |
| Attack anim | Human_Attack_03_Staff (251) / AttackStatic 246 | Human_Attack_05_Bow (252) / AttackStatic 247 (clip names disputed, Q6) | Different stance offset |
| Event that spawns | do_skill / attack_ranged / attack_mainhand in state 5 | same | Same |
| Swoosh | 479 | none | Different |
| Melee radius | staff is ranged (no melee radius) | ranged | Same |

## 5. ENGINE NEEDS (general systems, named by engine function or concept)

N1. **External owner registry (multiple skill owners).** Today `RuntimeSkillCastCoordinatorV1` takes one `external_owner`; `main.cpp` sets the mage owner on every request. Need: a list/registry of owners consulted with `handles(profile)`, so Archer (and later Assassin/Illusionist/Necromancer) owners coexist. Archer: ExplodingArrows, MultiHitArrow, PinShot, PoisonArrows, Volley all need it (or a data arm that the coordinator can run).

N2. **Generic skill timers (`StartSkillTimerCB` / `StopTimerCB`).** Only the mage owner has a clock (`RuntimeMageSkillOwnerV1::advance`, statue/tornado). Need a session-level timer service keyed by owner, with one-shot and repeating timers and cancellation. Archer: ExplodingArrows DoExplosion (500 ms one-shot), PinShot second/third target (750 and 1500 ms one-shot), Precision `checkbuff` (1000 ms repeating), SideStep `check` (500 ms repeating).

N3. **Player SpawnSkillProjectile routing.** Exists: `RuntimeSessionProjectileV1::launch_skill` (with `check`, `delayed`, `expired`, and `explicit_target`). Archer needs it fed with ProjectileTable rows 3, 4, 6, 7 (skills) and row 10 (basic attack, already through `launch_state5`). Status: the executor exists; the Archer owner is the missing piece (see N1).

N4. **Monster stun state (`SetStunState(duration, true)`).** Missing. Original: `Character::_SetStunState` (IDA 0x3b9580) -> `CharStateMachine::SM_SetStunState` (0x3c5ffc): returns if the actor IsBoss; otherwise starts timer id 43 for `duration`, sets state flag bit 2 (once), and plays an AnimTable clip read from `CharAnimTable` member +140 (which cell that is was not confirmed; INFERENCE that it is the Stunned clip). Port note: `runtime_skill_use_profile_magesub_v1.hpp` says "no stun state in CombatSession". Archer: PinShot (duration = SnS_Duration on the hit monster). Shared: ViciousStrike stun side effect (Rogue base), Knight/Mage rows.

N5. **Monster knockback state (`SetKnockBackState(bool)`).** Partially present: `combat_session.cpp` and `features/combat/b035_session_push_effect_v1` reference SetKnockBackState for other sources; no path for the Archer's Lua call. Original: `Character::_SetKnockBackState` (IDA 0x3b7680) -> `CharStateMachine::SM_SetKnockBackState` (0x3c5ea0), not decoded in this phase. Archer: MultiHitArrow (standard KB on 2-target volley, big KB on 1-target third arrow), SideStep (attacker KB from the deflection timer).

N6. **OnTargetHit AI-event steal (`StealAIEvent("OnTargetHit", fn)`).** Missing (grep for StealAIEvent on port: no hits). Needed for Precision (player as attacker, creates buff when HasBow) and SideStep (player as defender, melee range check, chance roll). The IDA handler address for StealAIEvent was not located in this phase (TODO). Shared with other subclasses (any passive that reacts to hits).

N7. **Weapon-type predicates for skill admission and passives: HasBow / HasStaff.** The equipment layer already has the facts (`dh2::player::query_bow` in `features/equipment/runtime_player_locomotion_v1.cpp` line 164, `stance_has_bow`), but there is no Lua-facing `HasBow()` for skills. Original: `Character::_HasBow` (IDA 0x3b6f84) -> `ItemInventory::HasBow` (main-hand item Type == 4 at offset +148 of the item object; INFERENCE that +148 is the Type word, consistent with the atlas Type field). Archer: OnSkillCheck_ of 5 actives, PinShot/MultiHit/Precision gates.

N8. **Permanent passive buffs with a level-driven class projection.** The mechanism exists (`RuntimeTimedBuffOwnerV1`, permanent = duration 0; CreateBuff/ApplyBuff at level change, RemoveBuff of the previous). Missing: a passive owner that runs `OnSkillUpdate_` for passive rows (no Pre/Use) and calls the timed-buff owner with permanent=true, plus `SetProp(SnS_Level)` before projection where the Lua does it. Archer: BowMastery, Precision, SideStep (plus the four shared passives). Readers of the affected properties (55, 87, 94, 60, 63, 81/82, 145/146, 190) are needed for the effect to show (Q2).

N9. **Equipment-layer side effect: second-hand main weapon (Ambidextrous > level 6).** Original Lua itself says there is no command for it. The class buff sets DestProp 202 `Special_Equip_Main_In_Offhand`. Equipment layer must read that property (shared Rogue passive; not Archer-specific). Not in scope for Archer phase 2 unless the Rogue agent asks.

N10. **Target-list search with an explicit origin (`TargetListSearch(range, angle, origin)`).** Exists: `features/generic_skills/runtime_skill_target_query_v1.hpp` lines ~81-87 ("Only the search origin changes"). Archer: ExplodingArrows (200, 360 around the impact), Volley rebound (900, 360 around the victim). Reuse, no new system.

N11. **Repeated `do_skill` markers per Use.** MultiHitArrow fires OnSkill_ on three animation events of one Use, each popping the target list. Need: the state-6 do_skill marker to re-enter the external owner's `use()` per event (the coordinator currently calls `use()` once per Use, LIKELY, verify in `runtime_skill_cast_coordinator_v1.cpp` ~1265-1300). Archer: MultiHitArrow only.

N12. **SetProjectileTarget steering.** The `ArrowDefault` row has `TargetLock = 0`; per the port's own header, steering needs the row's target-lock flag. The Lua for Volley says SetProjectileTarget "doesn't seem to work" and fakes the target; so the Archer does not need steering (the owner should do what the Lua does, not invent steering). No new system required (PROVEN for the flag, INFERENCE for the Lua intent).

Already reusable (no new system): SkillCombatRoll with the ApplyPropClass formula sheet (mage owner `roll`), SetSkillCooldown/HasSkillCooldown, LookAt, UseMana/HasMana, timed buff owner (`RuntimeTimedBuffOwnerV1`), projectile executor contact/update, ProjectileTable/ProjectileDict loading (`load_runtime_session_projectile_tables_v1`).

### Mapping of needs to Archer skills
| Skill | N1 | N2 | N3 | N4 | N5 | N6 | N7 | N8 | N10 | N11 |
|---|---|---|---|---|---|---|---|---|---|---|
| ExplodingArrows | x | x | x | | | | x | | x | |
| MultiHitArrow | x | | x | | x | | x | | | x |
| PinShot | x | x | x | x | | | x | | | |
| PoisonArrows | x | | x | | | | x | | | |
| Volley | x | | x | | | | x | | x | |
| BowMastery | | | | | | | | x | | |
| Precision | | x | | | | x | x | x | | |
| SideStep | | x | | | x | x | | x | | |
| Bow basic attack | | | x (already) | | | | | | | |

## 6. Assets and resolution (per name the Archer uses)

| Name | Kind | Resolution | Status |
|---|---|---|---|
| skill_dh2_prince_rogue_archer_exploding_arrows.bdae | clip | original `animations_dictionary` 1219 | present (static check) |
| skill_dh2_prince_rogue_archer_multi_hit.bdae | clip | 1220 | present |
| skill_dh2_prince_rogue_archer_pinning_shot.bdae | clip | 1221 | present |
| skill_dh2_prince_rogue_archer_poison_arrows.bdae | clip | 1222 | present |
| skill_dh2_prince_rogue_archer_volley.bdae | clip | 1223 | present (staged) |
| prince_ranged_attack(.bdae, _02, _03) | clip (bow basic) | 1077-1079 | present (staged, `assets/animations/`) |
| sfx_skill_rogue_arc_burst_shot / rain_of_arrows / maim_shot / venom_shot / pierce_shot | sound (clip markers) | sdd sound tables | wav present in staged package (matrix) |
| sfx_impact_burst_shot | sound (PlaySound) | atlas sound object | resolves; wav not verified by filename |
| env_weapon_arrow_hit (291), env_weapon_arrow_miss (292) | sound (projectile) | sdd rows 291/292 -> sfx_impact_arrow_*_1_22.wav | present (staged) |
| spell_dh2_prince_rogue_burst_shot | AnimatedEffectTable | atlas table bd109770 | NO `.bdae` by filename in assets-extra; port name resolution not verified (Q7) |
| spell_dh2_hurt_regular_arrow (221) | impact FX | effect_sets 221 -> file 218 | `spell_dh2_hurt_regular_arrow.bdae` in assets-extra/android/data/3d/interface/ |
| spell_fire_hurt (241) | impact FX | effect_sets 241 -> file 208 | `spell_fire_hurt.bdae` in assets-extra |
| spell_poison_hurt (244) | impact FX | effect_sets 244 -> file 216 | NOT found by filename in assets-extra (Q7) |
| projectile_arrow_default / explode / pin_shot / poison (.bdae) | ProjectileDict 9, 10, 12, 13 | `assets-extra/android/data/3d/projectiles/` | present in original; NOT found by filename in the staged package (Q8) |
| elemental_bolt_fire.bdae | ProjectileDict 1 (mage bolt) | `assets-extra` | also NOT found by filename in the staged package, yet magefix2 reports a real fire-bolt run, so packaged names differ from original names (Q8) |

Caveat for the staged check: names were searched by filename only (`find` in `windows-source-clock-v21.8-staged` and `windows-shared-assets`). The staged package uses its own folder layout (`assets/`, `audio-assets/`), so "NOT found" means "not under that filename", not "absent from the package". The port's AssetCatalog is the authority; confirm with a load in phase 2.

## 7. Open questions and evidence gaps

- **Q1. Bow basic attack runtime evidence.** Not run with a bow in this phase. The magefix2 real run used the staff. Phase 2 should run the bow basic attack in a hidden window and record: the attack clip name played, the `Mage projectile launch ... row=10` line (or the Archer equivalent log), the sensor radius, and impact frames. Also confirm which attack clip (247/252 vs 94-96 bow clips) actually plays (Q6).
- **Q2. Readers of the passive properties.** Need to confirm the port's ranged hit formula reads DestProp 55 (Rating_Attack_Bonus_Range) and 87 (Damage_Bonus_Range), and the rogue crit path reads 63, before passives can be verified. Not checked.
- **Q3. Precision: source conflict.** Lua header: hit and crit chance. Lua dated note: chance to stun. Class formula: Slow_Target_Chance and Slow_Duration (props 145/146). INFERENCE: the data is authoritative for what the class does, and the Lua comment is stale. Needs IDA confirmation of how the original's hit path uses property 145/146 for a player with a bow (not done).
- **Q4. PoisonArrows DOT.** The Lua author note says the DOT "does not seem to work". The Lua itself applies only the class sheet (ApplyPropClass(CLASS_ID)) and SkillCombatRoll; any DOT would come from the class sheet's Dot_* cells and the hit path. The decoded PoisonArrows class formulas were truncated in the atlas dump, so I did not confirm whether Dot_* cells are written. Not verified.
- **Q15. Volley and PinShot class formulas were only partly decoded.** The atlas dump for `Skill_Archer_Volley` and `Skill_Archer_PinShot` was truncated at 420 characters. Full rows (including whether prop 190 TempProp1 is written for Volley) need a complete decode before phase 2.
- **Q5. SideStep KNOCKBACK value.** TempProp1 = 2560 + 256 × SnS_Level per the class formula (p2 = 172 = SnS_Level). The Lua SideStep does not call SetProp(SnS_Level) before projection, so the level used depends on the sheet state. Value is INFERENCE (about 10 + level percent).
- **Q6. Bow attack clip conflict.** Atlas: 247/252 Human bow sequences resolve to cutscene-named clips (`cs_darkqueen_scene03a_*`, indices 94-96). Atlas: BowAttackStatic01-03 (94-96) resolve to `prince_ranged_attack*.bdae` (consistent). Either the atlas index mapping for the Human sequences is wrong or the stanced graph is not what I think. Resolution: log the clip names the port loads for the Archer's attack bank in a real run.
- **Q7. Effect and sound filenames.** `spell_dh2_prince_rogue_burst_shot.bdae` and `spell_poison_hurt.bdae` are not in assets-extra by filename. Check whether the AnimatedEffectTable row resolves to another file (`effect_sets` 244 -> file 216; check file 216 and the burst-shot effect's file list).
- **Q8. Projectile model files in the staged package.** Not found by filename (also the mage bolt). Confirm through the AssetCatalog load path before phase 2.
- **Q9. HasBow offset.** IDA `ItemInventory::HasBow` reads `+148` of the item object; the port reads `words[22]`. The atlas Type=4 for bows agrees with words[22], so the two are consistent (INFERENCE). Confirm the item object layout if IDA is re-checked.
- **Q10. ExplodingArrows knockback.** The Lua header says "chance to knockback", but DoExplosion has no SetKnockBackState/SetStunState call. Treat as no knockback unless the original's `SkillCombatRoll` applies a KB outcome (not verified; the port's SkillCombatRoll behaviour for KB outcomes is unknown).
- **Q11. Lua SpawnSkillProjectile binding name.** IDA binds `SpawnSkillProjectile__` (line 132606). The Lua calls `SpawnSkillProjectile`. Probably a binder suffix; confirm the registered name before porting.
- **Q12. IDA vtable slot +296.** The ranged-projectile lookup used by `CharAI::_OnAnimEvent` is not identified (INFERENCE that it returns the main-hand ranged ProjectileTable row).
- **Q13. StealAIEvent native address.** Not located in phase 1.
- **Q14. Lua details to keep in mind for phase 2:** Volley uses `traces = true` and an undeclared global `target` in `do_rebound`; PinShot uses an undeclared global `target` in OnSkill_ (lines 117, 125). They are harmless in the single-threaded Lua VM but should be ported as locals.

## 8. Phase 2 hand-off (what I recommend the lead does)

1. Assign N1 (owner registry) + N2 (timers) + N7 (HasBow) to one worktree first, since five actives and three passives depend on them.
2. Assign N4 (stun) and N5/N6 (knockback and OnTargetHit steal) to a second, separate worktree; they are general combat-session features shared with other subclasses.
3. The Archer agent then builds the Archer owner (or data arms) over those, verifies the five actives in hidden-window runs, and runs the bow basic attack check (Q1).
4. Passives (N8) can go in parallel with step 1 once the permanent-buff owner is exposed to the Rogue/Archer rows; verify the property readers first (Q2).

No engine code was written in this phase. No data was changed. No junctions were created. The Lua decoder helper written during the investigation lives outside the repo (`C:/dh218/archer-scratch/dump_lua.py`), but it was not needed because the `.luac` files are plain text.
