# character3 fidelity audit (Character:: last third by address, 0x3b7b28-0x3bf828)

## Progress

- last_address_done: 0x3bf828 (batch complete at the depth described below)
- groups_done: XP/level-up; HP/MP/regen/UseMana/HasMana; stat points (IncStat*, ResetStats*); skills (IncSkill, ResetSkills, CanIncrementSkill, IsSkill*); Lua bindings (sampled deeply: CreateBuff, SkillCombatRoll, SpellCombatRoll, RemoveDots, WarpBehind, Flee, SetStunState; the rest checked only for registration); SG_* save getters/setters = n/a
- next: none. If resumed: Lua bindings _SetProp/_GetProp/_ClearProps/_ApplyPropClass/_PlayAnim/_HeadTo/_MoveTo/_WarpTo/_SpawnSkillProjectile/_SetSkillCooldownTimerId/_AddAggro/_Kill and the hosts in port/level-world/character_script_session*.cpp deserve a line-level compare.

## Summary

Status counts (by mapping-table rows; groups count once): faithful 9, partial 2, divergent 1, missing 1, unclear 4, n/a 3, intentional-adaptation 0.

The reconstruction transcribes these functions very literally (property ids, design-setting offsets, Q8 math, debug-switch call order). The level-world kernels (`player_progression_v1.cpp`, `character_skill_buff_bindings_v3.cpp`, `character_skill_combat_v6.cpp`, `character_skill_mana_v5.cpp`, `character_timer_effects.cpp`) match IDA. The risks are in the Windows integration layer that re-implements or skips them.

Top risks:
1. F1: live level-up path drops overflow XP carry (medium-high).
2. F2: Windows generic-skill Use path never applies the SkillCombatRoll dual-wield (mask 0x800000) branch nor the no-weapon skip (medium, unclear for the four implemented skills).
3. F3: Stat/skill respec (NativeStatsReset -> ResetStats + ResetSkills + gold cost StatsReset_GoldCostPerLevel) is not implemented in the Windows build (medium, missing).
4. F4: Companion WarpBehind destination producer is absent from the live build (low-medium, unclear).
5. F5: LevelUp side effects (SG_Save, HP/MP regen call semantics, status message, FX 135, trophies, tutorials) depend on callbacks in the live path (low).

## Findings

### F1 (medium-high) Level-up XP carry is lost in the live loot/XP runtime
- IDA 0x3bf498 `_GiveXP`: when xp >= threshold (prop33 >= prop34) it calls `LevelUp(this, (prop33 - prop34) >> 8)`; the carry is computed BEFORE LevelUp. IDA 0x3beb88 `LevelUp`: `AddInt(19,1)`, `SetInt(33,0)`, UpdateBaseProperties, RegenHP(-1), RegenMP(-1), ..., then if new level < cap: clamp carry to `(prop34>>8)-1` when carry > (prop34>>8), and `PROPS_AddInt(33, carry)`. `_GiveXP` finally clamps prop33 to prop34.
- Port reference implementation `port/level-world/player_progression_v1.cpp:70-96` is faithful (carry computed first and passed in). But the code actually reached by the Windows build, `port/windows-foundation/features/loot/runtime_death_rewards_v1.cpp:128-185` (`level_up`), recomputes the carry after `dh2_property_set_int(&view, 33, 0)` (line ~149): `carry = asr8(resolved[33] - resolved[34])` (lines 167-169). After the reset resolved[33] is ~0 (game-data/properties.cpp `resolve` type-32/4 path: base+saved), so carry <= 0 and the `bounded_carry > 0` guard (line 171) skips the add. It also adds carry even if the new level reached the cap (IDA only when level < cap).
- Consequence: every level-up resets XP to exactly 0; overflow XP (big kills, low-level thresholds, multi-level worth of XP) is discarded, so leveling is slower than the original. Player-visible.
- Confidence: medium-high (static reasoning, not run). Fix: capture `(xp - threshold) >> 8` before the prefix reset (as `player_progression_v1.cpp` does), apply the cap condition.

### F2 (medium, unclear scope) Windows skill Use ignores the dual-wield branch of `_SkillCombatRoll`
- IDA 0x3b9fbc `Character::_SkillCombatRoll`: loads `CharSkill` mask (+28). If mask & 0x800000: attack with main hand only if `HasMainHandWeapon`, and additionally attack with the off-hand (`mask | 0x4000000`) if `HasOffHandWeapon`; each pushes a damage int. Otherwise one `F_SkillAttack(mask, element=+20)`.
- Port kernel `port/level-world/character_skill_combat_v6.cpp:28-76` implements exactly this (faithful, level-world/Android path). The Windows runtime does not call it: `port/windows-foundation/features/generic_skills/runtime_skill_cast_coordinator_v1.cpp:1160-1180` builds one `CombatSessionSourceHit` per target with `mask = row_mask | 0x08000000` and `category = main_damage_class`, with no 0x800000/off-hand handling and no weapon-presence gate.
- Consequence: only matters for skills whose SkillTable mask has bit 0x800000. BashDown's mask 0x00335535 does not (runtime_skill_activation_session_v1_tests.cpp:1089-1091); the masks for GroundSlam/Charge/JumpKick were not checked. Also the 0x08000000 OR is port-added (presumably the SkillAttack flag set inside F_SkillAttack); cross-check with the combat batch.
- Confidence: medium. Fix: check the masks of the four implemented skills; if any has 0x800000, add the main/off-hand double hit and the weapon-presence gate.

### F3 (medium) Stat/skill respec missing in Windows build
- IDA 0x3bd26c `ResetStats`, 0x3bd1cc `ResetStatsChange`, 0x3bc860 `ResetSkills`; caller `NativeStatsReset` (pseudocode-all.c ~228020) charges `StatsReset_GoldCostPerLevel * level` via `ItemInventory::AddGold(-cost)` and runs ResetStats + ResetSkills. ResetSkills refunds every skill rank into prop157 (skill 0 stays at 1, so refund = sum - 1), clears all slot assignments (SetSkillInSlot -1) and reloads the AI skill script. ResetStats moves props 149..152 into 148 and revalidates equipment requirements (`INV_CheckItemsRequirements`) and HP/MP (`ValidateHPMP`: clamp 36<=38, 41<=43).
- Port: `features/character_menu/stat_training_v1.cpp` covers only single-point spend/refund (ResetStatsChange semantic). No ResetSkills/ResetStats/StatsReset_GoldCostPerLevel anywhere in `port/windows-foundation` or `port/level-world` (only the Android `native_process_menu_actions_v119.inc` mentions it).
- Consequence: no way to respec in Windows; also the ValidateHPMP clamp and equipment re-check after a stat refund (ResetStatsChange) are not confirmed in stat_training_v1 (not verified).
- Confidence: high that the full reset is missing; check docs that it is a planned item.

### F4 (low-medium, unclear) Companion WarpBehind destination has no live producer
- IDA 0x3b8d98 `_WarpBehind`: destination = target position + (-normalize(target lookAtVec)) * 100, then `Cmd_WarpTo`. 0x3b91a0 `_Flee`: MoveTo(selfPos + (selfPos - targetPos)) i.e. a point mirrored through self, using the stored target (`+258`).
- Port: `features/companions/runtime_companion_movement_v1.cpp:210-221` requires an externally supplied `RuntimeCompanionWarpResultV1` whose producer does not exist outside tests; the companions feature is not referenced from `main.cpp`. Faery/companion follow-warp therefore cannot occur on Windows; `Flee` for enemies not located in the Windows enemy_ai feature (script catalog entries only).
- Confidence: medium.

### F5 (low) LevelUp side effects in the live path
- IDA LevelUp also: RegenHP(-1)/RegenMP(-1) (full refill), `SG_SetPlayerLevel`, `SG_Save`, MENU_LEVEL_UP status message, script `PlayerLevelUp`, `PlayAnimFXSet(135)`, tutorial scripts at level 2 (difficulty 0, offline), spec prompt at level 12, trophies at 10,20,..,100 for the local player.
- Port `runtime_death_rewards_v1.cpp:152-157` fills vitals to max via property deltas (equivalent to Regen(-1)); presentation is delegated to `services.on_level_up` (216-225); no SG_Save inside this function (persistence presumably elsewhere, unverified). Not verified that main.cpp wires on_level_up to message/FX 135/trophies.

### F6 (low) UseMana/HasMana negative cost
- IDA 0x3bdef4/0x3bd40c: negative amount only trips a debug assert and then proceeds (HasMana: `amount <= mana`; UseMana adds `-amount`). Port `character_skill_mana_v5.cpp:19-20` rejects `cost<0` with status -2. Only reachable with malformed data. Rest (GOD_MANA option/debug switch, byte +5360 free-cast flag, online-mode shortcut `vfunc+84`) matches.

### Verified faithful (no finding)
- `GetLevelScaledXP`, `PROPS_GetModifiedXP`, `DistributeXP` (design offsets 0x8c/0x90/0x94/0x98/0x9c/0xa0/0xa4 = IDA +140..+164; range gate with killer exempt; (n-1)*overlap% deduction; `(int)(x+1)<<8`): `player_progression_v1.cpp:100-134`, `runtime_death_rewards_v1.cpp:238+`.
- `_GiveXP` gates (level cap per unlocked difficulty, IsPlayer, vfunc+84, current level +336, OneKillLevelUp, difficulty<level->256).
- `IncSkill`/`CanIncrementSkill`/`IsSkillAvailable`/`IsSkillEquippable`: `features/generic_skills/runtime_skill_progression_v1.cpp:112-139` (points>0, level>=req, rank<cap per unlocked difficulty, rank<=level-req, slot 18 != -1).
- `IncStatStr/Dex/End/Nrg` (148 -1; 149/150/151/152 +1): `stat_training_v1.cpp:23-96`.
- `RegenTick/RegenHP/RegenMP` (props 36/38, 41/43; rates 39/40 HP, 44/45 MP, combat variant on aggro or states 5/6/7; amount<0 means full; clamp to max): `level-world/character_timer_effects.cpp:23-46`.
- `_CreateBuff` argument parsing/PROPS_AddBuff: `character_skill_buff_bindings_v3.cpp:13-30`.
- `_RemoveDots`: IDA `PROPS_RemoveDot` is an empty stub; port `dh2_character_dot_remove` no-op (faithful).
- `_SkillCombatRoll` / `_SpellCombatRoll` kernels (level-world), see F2 for the Windows integration gap.

## Mapping table

| IDA address | IDA function (or group) | port location(s) | status | note |
|---|---|---|---|---|
| 0x3beb88 | LevelUp | level-world/player_progression_v1.cpp:36-66; windows-foundation/features/loot/runtime_death_rewards_v1.cpp:128-185 | divergent | F1 (live carry lost), F5 |
| 0x3bf498 | _GiveXP | player_progression_v1.cpp:70-97; runtime_death_rewards_v1.cpp:192-236 | faithful | gates/OneKillLevelUp/difficulty->256 match |
| 0x3bf828 | DistributeXP (+GetLevelScaledXP, PROPS_GetModifiedXP) | player_progression_v1.cpp:100-134; runtime_death_rewards_v1.cpp:238-290 | faithful | design offsets match |
| 0x3bdbb8,0x3bdca4,0x3bdd90 | RegenMP, RegenHP, RegenTick | level-world/character_timer_effects.cpp:23-46; level-world/character_skill_application_v6.cpp:32 | faithful | |
| 0x3bdef4,0x3bd40c | UseMana, HasMana | level-world/character_skill_mana_v5.cpp:19-30 | partial | F6 negative cost rejected instead of asserted |
| 0x3bd0c0-0x3bd120,0x3bd2dc,0x3bd338 | SetMP/SetHP/GetTotal*/GetMP/GetHP/GetLevel/GetHP%/MP% | prop 36/38/41/43/19 accessors | n/a | trivial getters/setters |
| 0x3bd140 | ValidateHPMP | not located as a standalone function | unclear | clamps 36<=38, 41<=43 after stat/skill changes; port stat_training_v1 not checked for it |
| 0x3bd394,0x3bd3bc,0x3bd3e4 | GetEffectiveThreatPer{Damage,Heal,DamageToFriendly} | prop 204 etc. * 1/256 in game-data/aggro.cpp | unclear | not compared against aggro.cpp (aggro batch) |
| 0x3bd4f8,0x3bd600,0x3bd708,0x3bd810 | IncStatNrg/End/Dex/Str | features/character_menu/stat_training_v1.cpp:23-96 | faithful | props 148, 149-152 |
| 0x3bda74 | ModifyCharProps | not located | unclear | player-only property add + UpdateBaseProperties |
| 0x3bd1cc,0x3bd26c | ResetStatsChange, ResetStats | stat_training_v1.cpp (single-point refund only) | partial | F3 |
| 0x3bc860 | ResetSkills | none | missing | F3 |
| 0x3bcc58,0x3bc9ec,0x3bca50,0x3bca84 | IncSkill, CanIncrementSkill, IsSkillAvailable, IsSkillEquippable | features/generic_skills/runtime_skill_progression_v1.cpp:112-161 | faithful | IncSkill's UpdateAllSkills/RecalcProperties(1) side effects not in this function |
| 0x3bcad8,0x3bc5c0-0x3bc784,0x3bd000 | IsSkillUsable, skill list/OID getters, VerifySpecialization | features/generic_skills, skill_ui | n/a | UI/getter plumbing; spec prompt (levels>11, classes 263/325/290) not compared |
| 0x3bc6b8,0x3bc690,0x3bc688 | CancelSneaking, IsSneaking, IsUsingFreezingRay | level-world/character_cancel_sneaking.cpp, character_sneaking_tables.cpp | unclear | not compared (DelBuff 146, skill mask 0x2000000 search) |
| 0x3bb728-0x3bc4d0 | SG_* save/quest/faery/fast-travel/level-state accessors (about 60 functions) | port/windows-foundation/game_save.*, save_store.* | n/a | save-data boilerplate |
| 0x3b86a8 | _CreateBuff | level-world/character_skill_buff_bindings_v3.cpp:13-30 | faithful | arg parsing incl. bool->!bool, FX table bound |
| 0x3b842c | _RemoveBuff | character_skill_buff_bindings_v3.cpp:31-36 | faithful | |
| 0x3b7be0 | _RemoveDots | character_timer_effects.cpp (dot_remove no-op) | faithful | original PROPS_RemoveDot is an empty stub |
| 0x3b9fbc | _SkillCombatRoll | level-world/character_skill_combat_v6.cpp:28-76; Windows: runtime_skill_cast_coordinator_v1.cpp:1160-1180 | partial | F2 |
| 0x3b92a4 | _SpellCombatRoll | level-world/character_current_spell_v1.cpp / skill_combat | unclear | not read in port |
| 0x3b9580,0x3b96b0 | _SetStunState, _SetScareState | level-world/character_native_fsm.cpp:29-32; features/status_effects/status_effects.cpp:18-22 | unclear | argument defaults (bool default 1, duration units) not compared |
| 0x3b8d98 | _WarpBehind | features/companions/runtime_companion_movement_v1.cpp:210-221 | unclear | F4 no producer |
| 0x3b91a0 | _Flee | script catalog only | unclear | F4 |
| 0x3b97e0,0x3b90e4 | _SetSkillCooldownTimerId, _SetSpellCooldownTimerId | level-world/character_skill_cooldown_v3.cpp, character_player_skills_v3.cpp | unclear | registered; cooldown store (+24, -1 when nil) not compared |
| 0x3b99d0,0x3ba298 | _SpawnSkillProjectile, _SetProjectileTarget | level-world/character_skill_projectile_callbacks_v112.* | unclear | registered; position/destination handling not compared |
| 0x3b8098,0x3b8648 | _AddAggro, _ClearAggro | level-world/character_clear_aggro.cpp, character_skill_aggro_v6.* | unclear | not compared |
| 0x3b7e40 | _Kill | level-world/character_kill.cpp | unclear | Cmd_Kill(self, killer) not compared |
| 0x3b7eac,0x3b9d8c,0x3b82e4,0x3babdc,0x3b7b28,0x3b7b84 | _SetProp, _GetProp, _ClearProps, _ApplyPropClass, _GetPropBonusCritRating/AttackRating | level-world/character_property_bindings.cpp, character_script_session_v2.cpp | unclear | registered in character_script_owner_bindings.inc; not compared line by line |
| 0x3b8bd8,0x3b8cb8,0x3b93cc,0x3b94ac,0x3b8f9c | _DoSkill, _BeginSkill, _EndSkill, _StopSkill, _GetCurrentSkillInfo | level-world/character_skill_native_v5.cpp, character_current_skill_v2.cpp | unclear | registered |
| 0x3ba388,0x3ba71c,0x3bada8,0x3bb268,0x3b8ed8,0x3b8f38,0x3b9084,0x3b9f44,0x3b856c | _PlayAnim, _HeadTo, _MoveTo, _WarpTo, _LookAt, _SetTarget, _SetMaster, _Attack, _GetSkillIDFromOID | level-world/character_script_session.cpp, character_path_commands.cpp | unclear | movement/animation script commands; registered, not compared |
| 0x3b7dbc | _DBG_DumpProps | none | n/a | debug |
