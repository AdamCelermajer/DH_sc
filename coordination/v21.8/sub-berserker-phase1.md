# v21.8 sub-berserker: Phase 1 investigation (Berserker subclass of Knight)

Agent `berserker`. Branch `v21.8-sub-berserker`, worktree `.claude/worktrees/sub-berserker`, base `windows-foundation` @ 7b9028cb.
Phase 1 only: investigation, no engine code, no test changes. Date 2026-10-14.

Labels: PROVEN = read directly from original source, IDA or data. LIKELY / GUESS = weaker. INFERENCE = my deduction, not observed.
L below means the skill level.

## 0. Sources and method

- Original skill scripts: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_*.luac`. These are plain-text Lua source (CRLF, with a header comment), not Lua bytecode, so they were read directly. The brief's reader `lua_read_v13.py` reads `port/level-world/reference/lua-script-v13/original.asm`, which has no Berserker content. `inspect_skill_lua_v46.py` is a bytecode decoder and does not apply. (PROVEN)
- Atlas (read-only, `.claude/worktrees/atlas`): `atlas show` JSON for skill, table, class-row and animation-sequence nodes, including ClassTable `Formulas`; `coordination/atlas/general/native-map.md` for engine-function status. Atlas status is used only as a data aid.
- IDA: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`. Lua engine names are bound in native registration blocks (lines ~105333, 132719-132913). The file `libraries/classes.dex/pseudocode-all.c` in the same export is empty.
- Port: `git grep` on base 7b9028cb for the use-profile table, cast coordinator and timed-buff owner. The Knight profile header `port/windows-foundation/features/generic_skills/runtime_skill_use_profile_knight_v1.hpp` is already on base.
- Class sheets: formulas from the atlas ClassTable rows; formula semantics from `port/game-data/class_tables.cpp` (`dh2_class_formula`). Property indices from `port/android-native/app/src/main/cpp/source_process_compiled_members_v121.hpp`. SnS_Level = 172 and Special_Equip_2H_In_One = 203 match the port notes. Fixed point is value/256 (PROVEN: the Lua traces `FromFixed(mana_cost)` as mana and uses `FromFixed(SnS_Duration)` as milliseconds).
- Ownership: `coordination/v21.8/SKILL-VERIFICATION-MATRIX.json`, tree Berserker = 16 skills (6 actives, 10 passives).

## 1. Coverage

Owned by Berserker (8 skills, all investigated below):

| Skill | Type | Lv | Script | Original source | Class rows |
|---|---|---|---|---|---|
| Berserk | active | 18 | prince_berseeker_berserk | prince_berseeker_berserk.luac | 151, 152, 153 |
| Intimidation (Warcry) | active | 15 | prince_berseeker_warcry | prince_berseeker_warcry.luac | 155 |
| WhirlingDrift | active | 12 | prince_berseeker_whirling_drift | prince_berseeker_whirling_drift.luac | 163 |
| KillingFrenzy | passive | 21 | prince_berseeker_killing_frenzy | prince_berseeker_killing_frenzy.luac | 156 (+ buff class id) |
| KillingStreak (Bloodlust) | passive | 15 | prince_berseeker_killing_streak | prince_berseeker_killing_streak.luac | 157, 158 |
| Ravager | passive | 21 | prince_berseeker_ravager | prince_berseeker_ravager.luac | 159, 160 |
| Survival | passive | 12 | prince_berseeker_survival | prince_berseeker_survival.luac | 161 |
| Toughness | passive | 18 | prince_berseeker_toughness | prince_berseeker_toughness.luac | 162 |

Class row numbers are the ClassTable indices in the atlas (`character_classes`). Row 154 (`Skill_Berserk_Frenzy`) exists in the table but is not referenced by any of the 8 scripts (INFERENCE: unused leftover, GUESS).

Shared with the Knight base (not owned here; listed for completeness, status as recorded in the matrix, not investigated): BashDown, Charge, GroundSlam (actives); CombatMaster, FastMetabolism, Hardiness, Leadership, PotionAddict (passives, matrix: "no gameplay effect in code").

## 2. Summary

| Skill | Original logic (one line) | Port CAST | ANIM | FX | SOUND | DAMAGE | EFFECT |
|---|---|---|---|---|---|---|---|
| Berserk | OnPre: 10% HP cost (while HP% > 10) and mana. Use: three volleys: stun all enemies in 360, damage top, knockback top | MISSING (rejected by design, coordinator:719-728) | clip 1235 present, not run | FX set 165 present, not run | cue 557 present, not run | MISSING | MISSING (stun, knockback, HP cost) |
| Intimidation | OnPre: closest enemies in 600, mana. Use: every non-boss enemy in the list gets scare (3/4) or stun (1/4) for SnS_Duration; fear or stun cue | PARTIAL (row admitted, rolls_combat=false, not run) | sequence 340, not run | FX set 168, not run | Lua fear and stun cues MISSING; generic cue not run | n/a (no roll) | MISSING (no scare/stun state) |
| WhirlingDrift | OnPre: 360 full circle, mana. Use: SkillCombatRoll on each pre-list target (no re-search). Push and forward move are not in the Lua | PARTIAL (row admitted, not run) | clip 341 (moving flag 1), not run | FX set 171, not run | cue 553 present, not run | PARTIAL (generic roll) | PARTIAL (push tail missing) |
| KillingFrenzy | Self melee hit: counter 1..5 builds a speed buff (SnS_Level = counter x L), 1.5 s reset, 5 s at max | MISSING (passive) | none | active_buff_fx (generic) | activate_4 commented out, not played | n/a | MISSING |
| KillingStreak | Self kill: PlayFX on self, two buffs for SnS_Cooldown ms (4000+500L), activate_2 sound | MISSING | none | FX set 'killing_streak' on self | activate_2 (wav present) | n/a | MISSING |
| Ravager | Level update: permanent buff +2L two-hand damage; from L7 sets Special_Equip_2H_In_One | MISSING | none | none | none | n/a | MISSING |
| Survival | Self hit: HP below 35% gives an 8 s buff (class formula 6), activate_3 sound; removed above 35% | MISSING | none | survival FX (BUFF_FX) | activate_3 (wav present) | n/a | MISSING (formula 6 unresolved) |
| Toughness | Level update: permanent buff Max_HP via formula 6 | MISSING | none | none | none | n/a | MISSING (formula 6 unresolved) |

## 3. Per-skill investigation

### 3.1 Berserk (active, Lv 18)

Original Lua (PROVEN, prince_berseeker_berserk.luac):
- Constants: RANGE 200; class ids Skill_Berserk_Berserk_Stun (153), _Damage (151), _KnockBack (152) (lines 14-18).
- OnSkillCheck: `HasMana(mana_cost), false`. OnSkillUpdate: `mana_cost = CalcManaCost(DAM_CLASS_ID, slvl)`.
- OnPreSkill_ (lines 56-87): Enemy, AttackableOnly, FrontalFirst; `TargetListSearch(200)`; `TargetListBackup("MaxAngleSearch")`; if the list is not empty, `target = top; LookAt(target)`. HP cost (lines 72-82): `health, HPMax, pct = GetPropHP()`; if `pct > 10`, `HP = health - health/10`. Then `UseMana(mana_cost)`.
- OnSkill_ (lines 90-140): `skill_inc` wraps to 0 when > 2. Each call re-runs `TargetListSearch(200, 360, "MaxAngleSearch")` with FrontalFirst. Volley by `skill_inc`:
  - 0: for every character in the list, `SetProp(SnS_Level, L)`, `ApplyPropClass(STUN)`, `SkillCombatRoll(target)`; `ClearProps()` at the end.
  - 1: top target only: `ApplyPropClass(DAM)`, `SkillCombatRoll`, `ClearProps()`.
  - 2: top target only: `ApplyPropClass(KB)`, `SkillCombatRoll`, `ClearProps()`.
  - Then `skill_inc = skill_inc + 1`.
- OnPostSkill_: `ClearTarget()`.
- Observations (PROVEN from source): the HP cost can take HP below 10% (it subtracts 10% of current HP when HP% is just above 10). The Lua comment says the push hits enemies "in the weapon path"; the code applies knockback to the top target only.
- Caveat (INFERENCE, LIKELY): `skill_inc` is a file-level variable reset only by the wrap, so a cast interrupted after volley 0 or 1 would start the next cast at volley 1 or 2.

Class sheets (atlas Formulas; L = skill level):
- Stun, row 153: SnS_Stun_Target_Chance (184) = 20480 + 1280L raw, i.e. 80 + 5L in value/256 units. SnS_Stun_Duration (185) = 512000 + 51200L raw, i.e. 2000 + 200L ms. Formula 0 also applies group row 225, not decoded.
- Damage, row 151: SnS_ManaCost (173) = 1024 + 512L raw, i.e. 4 + 2L. SnS_MinDamage (174) is set to 1280 raw, then scaled by TempProp1 (190) = 256 + 51L raw (about 1.0 + 0.2L), then Damage_Min_Main (79) weapon added at 1x. SnS_MaxDamage (175) is the same with Damage_Max_Main (80). 191 (TempProp2) is computed but not used by the Berserk script.
- Knockback, row 152: SnS_Push_Target_Chance (183) = 47360 + 1024L raw, i.e. 185 + 4L. No damage formulas.
- Chance units are unresolved (see OPEN-1): the literal decode gives values above 100.

Animation, FX and sound resolution (matrix + atlas, PROVEN for the names):
- AnimTable sequence 339 (`KnightBerserk_Berserk`, atlas table row): one step: Anim 1235 (`skill_dh2_prince_warrior_berseker_bersek.bdae`), FX 165 (`skill_dh2_prince_warrior_berseker_berserk`), Sound 557 (`sfx_skill_warrior_bsk_crippling_strike`, wav present in `windows-source-clock-v21.8-staged/audio-assets/data/sounds/`), Speed 1.3, MoveGO true.
- The Lua itself calls no PlaySound or PlayFX (PROVEN).
- Number of `do_skill` markers in clip 1235 not decoded (OPEN-2). The `skill_inc` 0..2 cycle implies three volleys (LIKELY).
- Atlas display name for this skill is "Crippling Strike" (icon crippling_strike); the SkillTable name is Berserk.

IDA / native evidence:
- TargetListSearch: bound at IDA lines 105333-105338 (GameObject::_TargetListSearch, atlas address 0x0038fcf0, PARTIAL).
- GetPropHP: Character::_GetPropHP, atlas 0x003b6cd8, PARTIAL.
- ApplyPropClass: Character::_ApplyPropClass, atlas 0x003babdc, PARTIAL.
- SkillCombatRoll: IDA mentions `_SkillCombatRoll` (6 hits), atlas 0x003b9fbc marked MISSING. The port has a roll path (see below), so the atlas status is stale or conservative. Not re-read in this phase.

Port state:
- CAST: not admitted. `runtime_skill_use_profile_knight_v1.hpp` (header lines 9-11) documents the rejection. The coordinator rejects with "No complete generic same-session Use arm" at `runtime_skill_cast_coordinator_v1.cpp:719-728`.
- Multi-volley is NOT missing on base: each distinct `do_skill` marker is a Use volley (`runtime_skill_cast_coordinator_v1.cpp:1313-1381`, `use_volleys`, from the skills-a branch, merged). So the Knight report's "needs multi-volley Use" is out of date. The real gaps are per-volley class token and target policy, and the HP write.
- HP self-cost: MISSING (no profile-level property write).
- DAMAGE: MISSING for Berserk. EFFECT: MISSING (stun and knockback states, push outcome).

Engine needs: E2 (per-volley Use), E3 (OnPre HP write), E4 (knockback outcome), E5 (chance units).

Open for Berserk: OPEN-2 (marker count), OPEN-8 (damage source for volleys 0 and 2: the stun and knockback class rows set no damage properties, so SnS_Min/Max damage depends on what is left in the sheet; LIKELY the stun and knockback volleys deal little or no damage, to be confirmed).

### 3.2 Intimidation (Warcry, active, Lv 15, script prince_berseeker_warcry)

Original Lua (PROVEN, prince_berseeker_warcry.luac):
- RANGE 600; CLASS_ID Skill_Berserk_Intimidation.
- OnSkillUpdate: `ClearProps(true)`, `SetProp(SnS_Level, L)`, `ApplyPropClass(CLASS, true)`, `mana_cost = GetProp(SnS_ManaCost, true)`, `bufftime = FromFixed(GetProp(SnS_Duration, true))`; if 0, fallback 2000 ms.
- OnPreSkill_ (lines 65-92): clears a global `buff` (never set, no-op); Enemy, AttackableOnly, ClosestFirst; `TargetListSearch(600)`; if not empty, `target = top; LookAt(target)`; `UseMana`. The howl FX is in the AnimTable (comment line 90-91).
- OnSkill_ (lines 95-140): only if `target` is set. Loop over the list: skip bosses and minibosses; `GetRand(1,4)`: 1, 2 and 4 call `SetScareState(bufftime, true)`; 3 calls `SetStunState(bufftime, true)`. Pop; next top. After the loop: PlaySound `sfx_skill_warrior_bsk_bloodmoon_howl_hit_fear` if any scare, PlaySound `..._hit_stun` if any stun (lines 129-136). Then `StartSkillTimerCB(bufftime, false, BuffExpired)`.
- PROVEN: OnSkill_ performs no TargetListSearch, so the list from OnPre persists. The scope is therefore ALL non-boss enemies in 600 (ClosestFirst order), not only the OnPre target.
- PROVEN: no SkillCombatRoll, no damage.
- PROVEN: GetRand(1,4) gives scare 3/4 and stun 1/4 per target. Uniform distribution LIKELY.

Class sheet, row 155 (atlas):
- SnS_ManaCost (173) = 2048 + 1024L raw, i.e. 8 + 4L.
- SnS_Duration (176) = 512000 + 64000L raw, i.e. 2000 + 250L ms (PROVEN given the /256 convention that the Lua uses).

Animation, FX and sound resolution:
- AnimTable sequence 340; clip `skill_dh2_prince_warrior_berseker_intimidate.bdae`; FX set 168; sound cue `sfx_skill_warrior_bsk_bloodmoon_howl` (ordinal 554, wav present). FX is authored in the sequence (PROVEN from the Lua comment).
- Lua-played cues `sfx_skill_warrior_bsk_bloodmoon_howl_hit_fear` and `..._hit_stun`: both resolve in the atlas (table rows), both wavs present in the staged package. The matrix lists only the single cue; the two Lua cues are missing from the matrix.

IDA / native evidence:
- SetScareState: `Character::_SetScareState` at IDA line 135172 (address 0x003b96b0, atlas agrees). It reads (duration, flag) and calls `CharStateMachine::SM_SetScareState(sm, duration, flag)`; the flag defaults to 1 when absent (PROVEN from the decompile). The meaning of the flag is unverified (OPEN-7).
- SetStunState: `Character::_SetStunState` at IDA line 135110, same pattern, `SM_SetStunState`.
- IsBoss / IsMiniBoss: bound (IDA 12 hits). Not re-read.

Port state:
- CAST: row admitted (`runtime_skill_use_profile_knight_v1.hpp`, Intimidation row; registry at `runtime_skill_use_profile_registry_v1.hpp`; admission at coordinator 146 and 719). Not run.
- ANIM/FX: generic sequence path (coordinator 705-713, 860-868). Not run.
- SOUND: the two Lua cues are not played (no PlaySound path in profile rows). MISSING.
- EFFECT: MISSING (no scare or stun state for characters on the skill path). The matrix atlas also says SetScareState and SetStunState are MISSING.
- Correction to the Knight header (`runtime_skill_use_profile_knight_v1.hpp`, Intimidation row and comment, lines 63 and 95): it says the scare/stun applies "on the OnPre target only". The Lua applies it to all non-boss enemies in the list. The row's `pre_selected_single` is a transcription error (PROVEN). The row should use `pre_list_all`. I did not change it (Phase 1; the knight row belongs to another wave).

Engine needs: E1 (scare and stun states), E6 (Lua-named cues), E13 (timers, cosmetic here).

### 3.3 WhirlingDrift (active, Lv 12)

Original Lua (PROVEN, prince_berseeker_whirling_drift.luac):
- RANGE 360; CLASS_ID Skill_Berserk_WhirlingDrift.
- OnSkillUpdate: `SetProp(SnS_Level, L)`; `mana_cost = CalcManaCost(CLASS_ID, slvl)`. OnSkillCheck: `HasMana(mana_cost), false`.
- OnPreSkill_ (lines 49-57): Enemy, AttackableOnly, NoSort; `TargetListSearch(360)`; `UseMana`. No LookAt, no SetSkillCooldown.
- OnSkill_ (lines 60-77): if the list is not empty: `SetProp(SnS_Level, L)`; `ApplyPropClass(CLASS_ID)`; repeat `target = GetTargetListTop(); PopTargetList(); dmg = SkillCombatRoll(target)` until empty.
- OnPostSkill_: `ClearTarget()`.
- PROVEN: the Whirling OnSkill_ never calls ClearProps (Berserk and Warcry do). Possible carry-over of the class sheet into the next cast (OPEN-10).
- PROVEN: the header comment says "targets receive half the warrior melee weapons damage and are pushed backwards a little. There is a forward movement in addition to the whirl." The Lua has no push or movement call. The push must come from the class (chance row 183) and the forward movement from the animation (matrix moving_flag = 1, clip 341).

Class sheet, row 163 (atlas):
- SnS_ManaCost (173) = 2304 + 256L raw, i.e. 9 + L.
- SnS_MinDamage (174): set to 512 + 128 x Damage_Min_Main (79), where the 128 coefficient is 0.5 in value/256 units. So 2.0 + 0.5 x W_min, then scaled by TempProp1 (190) = 230 + 25L raw (about 0.90 + 0.10L). SnS_MaxDamage (175) = 1280 + 128 x Damage_Max_Main (80), same scale. The 0.5 coefficient matches the comment "half the warrior melee weapons damage" (PROVEN from data, consistent with comment).
- SnS_Push_Target_Chance (183) = 39680 + 1280L raw, i.e. 155 + 5L. Units unresolved (OPEN-1).

Animation, FX and sound resolution:
- AnimTable sequence 341; clip `skill_dh2_prince_warrior_berseker_whirlwind.bdae`; FX set 171; sound cue `sfx_skill_warrior_bsk_annihilating_spin` (ordinal 553, wav present). The Lua plays no sound or FX itself (PROVEN).
- Animation moving flag = 1 (matrix). Whether the animation system moves the actor forward is not verified in this phase.

IDA / native evidence: TargetListSearch and SkillCombatRoll as in 3.1. No Whirling-specific native.

Port state:
- CAST: row admitted (Knight header row WhirlingDrift: `pre_list_all`, `rolls_objects = true`, `post_clear_target = true`). Unit test only per the Knight report. Not run.
- DAMAGE: generic roll path. The half-weapon factor is in class data, not in the port's row. PARTIAL.
- EFFECT: push tail MISSING (documented gap in the Knight header lines 13-14). Forward movement MISSING per the Knight header.
- ANIM/FX/SOUND: generic sequence path, not run.

Engine needs: E4 (push outcome from roll), E5 (chance units), E14 (forward move in attack).

### 3.4 KillingFrenzy (passive, Lv 21)

Original Lua (PROVEN, prince_berseeker_killing_frenzy.luac):
- BUFF_ID Skill_Berserk_KillingFrenzy; BUFF_FX active_buff_fx; BUFF_DELAY 1500 ms; BUFF_MAX_DELAY 5000 ms; MAX_COUNTER 5.
- `StealAIEvent("OnTargetHit", OnTargetHit_Skill)` (line 86). Handler (lines 38-85) acts only if `skill_level > 0`, `attacker:GetID() == GetID()` and `CanAttackInMelee()`.
  - If `CURR_COUNTER == 0 or not buff`: CURR = 1; create buff `CreateBuff(BUFF_ID, 0, true, L, BUFF_FX)`; `SetBuffProp(SnS_Level, ToFixed(CURR x L))`; `ApplyBuff`; timer 1500 ms to `resetBuff`.
  - Else if `CURR < 5 and buff`: CURR + 1; remove; create with SnS_Level = CURR x L; apply; restart 1500 ms timer.
  - Else if `CURR == 5 and buff`: remove and recreate with SnS_Level = 5L; restart 5000 ms timer.
- `resetBuff` (lines 27-35): removes the buff and sets `buff = nil`. CURR_COUNTER is NOT reset, but the first branch's `not buff` test restarts it at 1 (PROVEN).
- OnSkillCheck: `return false, true` (second value = active flag; see E9).
- Trace text (trace-only, traces = true at line 24): "Buff level 1, 60% speed increase", "...(50 + CURR x 10)% speed increase". Trace only.
- Sound: `PlaySound("sfx_passive_skill_activate_4")` is commented out (line 50): not played (PROVEN).

Class sheet, row 156 (atlas): Speed_Modifier_Walk (46), Speed_Modifier_Rotation (47), Speed_Modifier_Attack (48) each += 256 x (buff SnS_Level) raw, i.e. += counter x L in value/256 units. SnS_Cooldown (177) = 1280 raw, unused by a passive.
- CONFLICT (OPEN-3): the trace text says 60% to 100% speed increase; the class sheet gives counter x L (1 to 25 at max). The unit of Speed_Modifier is not verified. Do not take either as the rule.

Animation and FX: no sequence. BUFF_FX active_buff_fx (generic EffectDict row, resolves in atlas).

IDA / native evidence:
- StealAIEvent is not a native name in the export (0 hits). The event goes through `AISDefault::InitVCB` (IDA ~line 159394): if the AI's vtable has "OnTargetHit", flag 0x800 is set. `AISDefault::OnCombatResults(this, attacker, defender, result)` (IDA line 159458): if `(result[24] & 3) == 0` and flag 0x800 is set, it calls Lua `OnTargetHit(attacker, defender)`.
- Which AI object gets `OnCombatResults` (player AI, attacker AI or defender AI) is not identified (OPEN-4).
- CanAttackInMelee: `Character::_CanAttackInMelee` at 0x003b6f64, atlas MISSING.

Port state: MISSING all. No player-side OnTargetHit dispatch (the only `OnTargetHit` text in the tracked port is a comment in `runtime_skill_use_profile_rogue_v1.hpp`). No passive OnSkillUpdate hook. The timed-buff owner has a `permanent` flag but no capacity field (`runtime_timed_buff_v1.hpp`).

Engine needs: E7, E8, E9, E10, E11, E13.

### 3.5 KillingStreak / Bloodlust (passive, Lv 15)

Original Lua (PROVEN, prince_berseeker_killing_streak.luac):
- CLASS_ID Skill_Berserk_KillingStreak; DUMMY_ID Skill_Berserk_KillingStreak_dummy; BUFF_FX active_buff_fx; FX `skill_dh2_prince_warrior_berseker_killing_streak`. BUFFTIME_BASE 4000; BUFFTIME_LEVEL 500.
- `StealAIEvent("OnTargetHit")` (line 34). Handler (lines 23-33): if `skill_level > 0`, `attacker:GetID() == GetID()` and `defender:IsDead()`: `PlayFX(FX)` (on self); `buff = CreateBuff(CLASS_ID, bufftime, true, L)`; `buff2 = CreateBuff(DUMMY_ID, bufftime, false, L, BUFF_FX)`; `ApplyBuff` both; `PlaySound("sfx_passive_skill_activate_2")`.
- `bufftime = FromFixed(GetProp(SnS_Cooldown, true))` (line 49). If 0, fallback 4000 + 500L (lines 51-53). PROVEN: the class Cooldown value is exactly that formula, so the buff lasts the SnS_Cooldown projection.
- OnSkillUpdate (lines 41-79): on a level change while buff is set, removes and re-creates. Defect in the original script (PROVEN from source): lines 60 and 69 call `RemoveBuff(DUMMY_ID, buff2)`, but `buff2` is a block-local declared only at line 74 and is nil at lines 60 and 69. The dummy buff is therefore not removed on those paths. Effect INFERENCE (the dummy outlives a level change to 0 or a re-apply).

Class sheet (atlas): rows 157 and 158.
- Damage_Min/Max_Main_Hand (79, 80) and Off_Hand (81, 82) are SET to 1280 + 512L raw, i.e. 5 + 2L. TempProp1 (190) = 1280 + 512L.
- SnS_Cooldown (177) = 1024000 + 128000L raw, i.e. 4000 + 500L ms (PROVEN, matches the Lua fallback).
- Row 158 (dummy) has no formulas.
- SET versus accumulate for properties 79-82 in a buff sheet is not verified (OPEN-5).

Sound and FX: `sfx_passive_skill_activate_2` (wav present in staged package). FX `skill_dh2_prince_warrior_berseker_killing_streak` (atlas AnimatedEffect row, resolves).

Port state: MISSING (no player OnTargetHit, no passive dispatch, no PlayFX/PlaySound from a profile).

Engine needs: E7, E9, E10, E11, E6, E13.

### 3.6 Ravager (passive, Lv 21)

Original Lua (PROVEN, prince_berseeker_ravager.luac):
- CLASS_ID Skill_Berserk_Ravager; BUFF_ID Skill_Berserk_Ravager_buff.
- OnSkillUpdate: remove previous buffs; if L ~= 0: `buff = CreateBuff(CLASS_ID, 0, false, L)`, `ApplyBuff`. If L > 6: `SetProp(Special_Equip_2H_In_One, ToFixed(L))` and `buff2 = CreateBuff(BUFF_ID, 0, false, L)`, `ApplyBuff`. OnSkillCheck: `false, true`.
- The Lua does not clear the 2H flag when L drops to 6 or below (PROVEN). Likely irrelevant in normal play (INFERENCE).

Class sheet:
- Ravager, row 159: Damage_Bonus_TwoHand (91) += 512L raw, i.e. +2L.
- Ravager_buff, row 160: Special_Equip_2H_In_One (203) = 256L raw, i.e. L (any nonzero enables two-hand weapon in one hand).

Port state: MISSING (passive; no level-update hook). The source inventory notes already read property 203 (`port/game-data/reference/player-inventory-owned-v4/NOTES.md:9`), so the flag is read when set. The two-hand bonus gate (2H weapon equipped) belongs to the engine (LIKELY).

Engine needs: E9, E10, E11.

### 3.7 Survival (passive, Lv 12)

Original Lua (PROVEN, prince_berseeker_survival.luac):
- BUFF_ID Skill_Berserk_Survival; BUFF_FX `skill_dh2_prince_warrior_berseker_survival`; BUFF_TIMER 8000 ms; PCT_VALUE 35; TIMER_DELAY 3000 (unused).
- `StealAIEvent("OnTargetHit", OnTargetHit_Skill)` (line 53). Handler (lines 29-52) takes no attacker guard. If `skill_level > 0`:
  - if `HP% < 35` and no `buff_survival`: `SetProp(SnS_Level, L)`; `buff_survival = CreateBuff(BUFF_ID, 8000, false, L, BUFF_FX)`; `ApplyBuff`; `StartTimerCB(8000, StopBuffTimer)` (StopBuffTimer only sets `buff_survival = nil`, it does not remove the buff); `PlaySound("sfx_passive_skill_activate_3")`.
  - if `HP% >= 35` and buff: `RemoveBuff`, `buff_survival = nil`.
- OnSkillCheck: `false, skill_level > 0`.
- The Lua does not inspect damage; the description says "damage dealt is greatly increased", which must come from the class (formula 6, below).

Class sheet, row 161: TempProp1 (190) = 38 + 7L raw. Damage_Min/Max_Main/Off (79-82) each use formula 6 with parameter 190.

Sound and FX: `sfx_passive_skill_activate_3` (wav present). BUFF_FX `skill_dh2_prince_warrior_berseker_survival` (atlas AnimatedEffect row, resolves).

Port state: MISSING. The OnTargetHit trigger context is OPEN-4; formula 6 is OPEN-6.

Engine needs: E7, E9, E10, E11, E12.

### 3.8 Toughness (passive, Lv 18)

Original Lua (PROVEN, prince_berseeker_toughness.luac):
- CLASS_ID Skill_Berserk_Toughness. OnSkillUpdate: remove previous buff; if slvl ~= 0: `buff = CreateBuff(CLASS_ID, 0, false, slvl)`, `ApplyBuff`. OnSkillCheck: `false, true`. Description: "increase max HP".

Class sheet, row 162: TempProp1 (190) = 25 + 5L raw. Max_HP (38) uses formula 6 with parameter 190.

Port state: MISSING. Formula 6 is OPEN-6.

Engine needs: E9, E10, E11, E12.

## 4. ENGINE NEEDS (general systems; for the lead's dedup)

| ID | General system | Engine functions / concept | Original handler (address) | Needed by | Port status now |
|---|---|---|---|---|---|
| E1 | Skill-applied scare and stun states on characters | SetScareState(dur, flag), SetStunState(dur, flag) | Character::_SetScareState 0x3b96b0 (IDA 135172) -> CharStateMachine::SM_SetScareState; Character::_SetStunState (IDA 135110) -> SM_SetStunState. State IDs CSScared 0x3c861c, CSStunned 0x3c8730 (from `features/combat/b035_session_push_effect_v1.hpp` header) | Intimidation | MISSING on the skill path (atlas MISSING). Swamp King AI already rejects skills while Scared/Stunned (`features/bosses/runtime_swampking_core_ai_v1.hpp:21`) |
| E2 | Per-volley Use profile (volley index selects class token and target policy) | Multi-volley Use | Marker volleys exist (coordinator 1313-1381) | Berserk | PARTIAL: one class token per row; no per-volley class or target policy |
| E3 | OnPre property write for the caster (HP) | SetProp(CharacterProperties.HP) from Lua with GetPropHP | Character::_GetPropHP 0x3b6cd8; SetProp | Berserk | MISSING |
| E4 | Knockback / push outcome from SkillCombatRoll for characters | Push-chance properties 183 (SnS_Push_Target_Chance) and KB row; push state | SkillCombatRoll native (0x3b9fbc per atlas, not re-read); push state in b035 module | WhirlingDrift, Berserk | PARTIAL: session push effect module exists (`features/combat/b035_session_push_effect_v1.hpp`); not verified for these skills |
| E5 | Chance-unit semantics for SnS_*_Target_Chance values | Roll compare of chance properties | Roll code in SkillCombatRoll (not re-read) | Berserk (184, 183), WhirlingDrift (183) | UNRESOLVED (OPEN-1) |
| E6 | Lua-named PlaySound / PlayFX from profile rows (named cues; self-play) | PlaySound(name, false, 0, false); PlayFX(AnimatedEffectTable row) on self | GameObject::_PlayFX 0x3923cc (atlas, DONE) | Intimidation (fear, stun), KillingStreak, Survival | MISSING in profile rows (generic path plays sequence cues only) |
| E7 | Player-side OnTargetHit event (attacker, defender) | StealAIEvent("OnTargetHit") | AISDefault::OnCombatResults (IDA 159458, flag 0x800 set in InitVCB at 159394) | KillingFrenzy, KillingStreak, Survival | MISSING (no player dispatch) |
| E8 | CanAttackInMelee gate | CanAttackInMelee() | Character::_CanAttackInMelee 0x3b6f64 (atlas) | KillingFrenzy | MISSING |
| E9 | Passive activation: level-update callback (OnSkillUpdate_) with permanent buffs; active flag from OnSkillCheck second value | RegisterSkill OnSkillUpdate / OnSkillCheck active flag | CharAISkillScript::OnSkillCheck_Active 0x3db16c (reference notes: `port/level-world/reference/character-cancel-sneaking/NOTES.md`); permanent buff: CreateBuff duration 0 | Ravager, Toughness; update paths of KillingFrenzy, KillingStreak; Survival, KillingFrenzy checks | MISSING (no passive hook in coordinator). Permanent flag exists in `runtime_timed_buff_v1.hpp` (used by Celest faery passive) |
| E10 | Buff instance args: capacity (3rd arg of CreateBuff) and replacement | CreateBuff(class, dur, capacity bool, strength, fx) | Character::_CreateBuff 0x3b86a8 (IDA 134439) | All buff passives | PARTIAL: TimedBuffSpecV1 has no capacity field; apply replaces same class |
| E11 | Buff sheet combine rule per property (set vs accumulate) | Buff sheets overlay the actor sheet | Port doc says type&4 accumulates buff sheets (`runtime_timed_buff_v1.hpp` header) | Frenzy (46-48), Streak (79-82), Ravager (91, 203), Survival (79-82), Toughness (38) | UNVERIFIED (OPEN-5) |
| E12 | Class formula evaluator types 5 and 6 | dh2_class_formula case 5 and case 6 | Native formula handler (not yet IDA-checked) | Survival, Toughness (6); WhirlingDrift and Berserk damage (5) | Type 5 consistent with data; type 6 as coded looks wrong (OPEN-6) |
| E13 | Lua timers as skill callbacks: StartSkillTimerCB / StartTimerCB / StopTimerCB | Timer with callback | Not verified in this phase (IDA name not found by grep) | Frenzy (1.5 s, 5 s), Survival (8 s), Intimidation (cosmetic) | NOT VERIFIED |
| E14 | Forward move during a skill animation | Animation moving flag / MoveGO | Animation sequence fields (Speed, MoveGO) | WhirlingDrift (moving_flag 1), Berserk (MoveGO true) | NOT VERIFIED |

## 5. Corrections to existing reports and data

1. Knight report (`coordination/v21.8/skills-knight-report.md`) and `runtime_skill_use_profile_knight_v1.hpp`: Intimidation applies to only the OnPre target. The Lua applies it to all non-boss enemies in the 600 list (PROVEN, lines 95-127; no re-search). Row needs `pre_list_all`.
2. Knight report: "Berserk needs multi-volley Use" is out of date. Per-marker volleys are on base (coordinator 1313-1381). The gaps are E2 and E3.
3. Matrix (`SKILL-VERIFICATION-MATRIX.json`): Berserk, Intimidation and WhirlingDrift say "unknown (player Lua not in package)". The scripts are in the package (plain-text Lua). The matrix cast_mode fields should be updated.
4. Matrix: Intimidation lists one sound cue. The Lua plays two more (fear and stun, both present).
5. Atlas native-map: SkillCombatRoll is MISSING, but the port has a roll path (skills-a per-hand masks). The atlas status is stale or conservative.
6. Berserk Lua comment (push "in the weapon path") differs from code (knockback on top target only). Code is what is reproduced.

Atlas and IDA agreement: ApplyBuff binds to Character::_ApplyPropClass (atlas 0x3babdc; the IDA binding is confirmed, the function address was not re-read); SetBuffProp binds to Character::_SetProp (atlas 0x3b7eac, IDA binding confirmed); CreateBuff 0x3b86a8 and SetScareState 0x3b96b0 match the IDA function headers. No disagreement found in the addresses I checked.

## 6. Open questions

- OPEN-1: Chance units. Literal decode of SnS_Stun_Target_Chance (80+5L) and SnS_Push_Target_Chance (185+4L, 155+5L) exceeds 100. Need the roll compare in SkillCombatRoll (IDA) before any chance is implemented. GUESS: not a percent.
- OPEN-2: `do_skill` marker count in clips 1235 (Berserk), 340 (Intimidation) and 341 (WhirlingDrift). Needs the animation event track decoded or a real-run marker log. The Berserk `skill_inc` cycle suggests 3 (LIKELY).
- OPEN-3: Frenzy speed units versus trace text (60% to 100%).
- OPEN-4: Which AI object receives `CharAI::OnCombatResults` (player, attacker or defender). Caller not identified in the IDA export (it is a virtual call through CharAI slot 180). Decides whether Survival and KillingFrenzy fire on hits the player takes.
- OPEN-5: Buff-sheet combine rule for the properties used by passives (set vs accumulate for 79-82, 46-48, 91, 38, 203).
- OPEN-6: Formula type 6. As coded in `class_tables.cpp` `dh2_class_formula` case 6, the destination becomes `(dest_or_buff x src >> 8) + a`, where `a` is Param1 = 190 added as a raw constant. Read literally, Toughness sets Max_HP to about 12% of its value plus 0.7 HP, and Survival shrinks damage, which contradicts both descriptions. A plausible reading is `dest + (dest x src >> 8)` (a percentage bonus), but that is INFERENCE. Needs the native formula-6 handler in IDA before either passive is implemented.
- OPEN-7: Meaning of the second argument to SetScareState and SetStunState (the native default is 1).
- OPEN-8: Source of damage for Berserk volleys 0 and 2. The stun and knockback rows set no damage properties; whether SkillCombatRoll uses leftover SnS_Min/Max is unknown.
- OPEN-9: Whether Berserk `skill_inc` persists across an interrupted cast (see 3.1).
- OPEN-10: WhirlingDrift never calls ClearProps; whether the class sheet carries into the next cast.
- OPEN-11: Ravager 2H flag not cleared when level drops (Lua); whether level can drop in play.
- OPEN-12: Row 154 (`Skill_Berserk_Frenzy`) is not referenced by any Berserker script (GUESS: unused).

## 7. Next steps for the lead (not done here)

- Phase 2 needs IDA checks for OPEN-1, OPEN-4, OPEN-6 and OPEN-7 before any engine work on E1, E4, E5, E7 or E12.
- Real-run evidence per skill is not possible yet: no Berserker save or cast harness exists for these rows (Knight report). Intimidation and WhirlingDrift are the first candidates once E1 and E4 are settled.
- The Intimidation row scope fix (C1) should be made on the knight branch owner's side.
