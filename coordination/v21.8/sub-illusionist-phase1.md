# v21.8 subclass wave, Phase 1: Illusionist (Mage tree, prince_illusionist_*)

Agent: illusionist. Branch `v21.8-sub-illusionist`, worktree `.claude/worktrees/sub-illusionist`, base `windows-foundation` @ 7b9028cb.
Scope: investigation only. No engine code, no data rows changed. The one trivial-data question (Figments profile row) was checked and needs no change.

## 0. Method and evidence sources

- **Source A, original Lua:** `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_*.luac`. These files are plain Lua source text, not bytecode (they start with a `--[[ ... ]]` header). `lua_read_v13.py` reads an asm dump and `inspect_skill_lua_v46.py` decodes binary bytecode, so neither was used for these files. Shared helpers come from `skills/_commons.luac` (StealAIEvent, SetTempProps, CalcManaCost, IsBoss, IsMiniBoss, StartSkillTimerCB, SetSkillCooldown).
- **Source B, class rows:** the ClassTable and CharacterProperties rows, decoded with a scratch probe built from the port's own `game-data/class_tables.cpp` (the `dh2_class_formula` recovered formula). Formula: `dest = (p1 or dest if p1==-666) + p3 * (SourceProp>>8)` in 24.8 fixed point (/256). Source prop 172 = SnS_Level, 19 = Level, 152 = Stat_Energy (names from the probe). Projected values below use integer skill levels L.
- **Source C, atlas (read-only):** `atlas show skill:<Name>` for all 8 skills; `atlas find`. The atlas resolves the animation, effect, sound, script and table names. The atlas `native-map.md` gives port status per Lua-VM function. The atlas is a data aid, not proof of behaviour.
- **Source D, IDA:** `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/` (`names.jsonl`, `strings.jsonl`, `pseudocode-all.c`). The rea MCP was not used because the export is offline.
- **Source E, port:** `port/windows-foundation/` (features/generic_skills, combat_session.cpp, playable_actor_world.cpp, original_actor_properties.cpp, runtime_timed_buff_v1.*).
- Confidence tags: PROVEN (read directly in source, Lua, class row or IDA), LIKELY, GUESS. Inferences are marked INFERENCE.

Note on the matrix: `SKILL-VERIFICATION-MATRIX.json` lists the 8 owned skills with the flags used below. Several of its sound flags are wrong (see section 6).

## 1. Ownership

Owned by Illusionist (matrix "tree ownership", `owner_tree == Illusionist`): 4 actives and 4 passives.

| id | Skill (table name) | Display name (atlas) | Type | Level | Script |
|---|---|---|---|---|---|
| 30 | Eclipse | Eclipse | active | 12 | prince_illusionist_eclipse |
| 45 | Figments | Nightmare | active | 18 | prince_illusionist_figments |
| 110 | Terror | Confusion | active | 15 | prince_illusionist_terror |
| 111 | TimeControl | Hold Time | active | 21 | prince_illusionist_timecontrol |
| 27 | Disciples | Hypnotic Gaze | passive | 18 | prince_illusionist_disciples |
| 73 | Mesmerize | Charisma | passive | 12 | prince_illusionist_mesmerize |
| 74 | Mirror | Magnetic Shield (matrix icon "retribution") | passive | 15 | prince_illusionist_mirror |
| 82 | Petrify | Withering Touch | passive | 21 | prince_illusionist_petrify |

Shared with Mage and Necromancer (not owned here; port status noted only where it matters): ChainLightning, ColdRay, Cyclone, StoneEscape, ElementalMastery, FaeryBlood, MagisShielding, StaffMaster. The matrix lists the Illusionist tree as these 8 shared skills plus the 8 owned above (the matrix's count columns were not decoded).

Binding prerequisite (MASTER-HANDOFF KEY FINDING): `coordination/v21.8/subclass-bindings-manifest.json` now records the export of subclass bindings with `checks_passed: true`. `MagePlayerBase_Illusionist` differs from the base `MagePlayerBase` only in ClassDescString, ClassString, Loot and SkillTree. The regenerated XML lives in the primary `.local-inputs/windows-melee-bindings/`, not in this worktree, so a real run needs the junction described in COMMON-RULES.

## 2. Active skills

### 2.1 Eclipse (id 30)

**Original Lua (PROVEN, source A):**
- OnSkillCheck: `HasMana(mana_cost)` only. No HasSkillCooldown, no SetSkillCooldown anywhere in the file, so Eclipse has no cooldown in the Lua. The class has no SnS_Cooldown row either (PROVEN).
- OnPreSkill: filters Enemy / AttackableOnly / NoSort, `TargetListSearch(600)`, `LookAt(top)`, `UseMana(mana_cost)`.
- OnSkill: `TargetListSearch(600)`, then for each popped target that is not a player and has a character component: `target:PlayFX(FX)` with FX = `spell_dh2_prince_mage_illusionist_eclipse`, then `target:CreateBuff(Skill_Illusionist_Eclipse, bufftime, false, L, BUFF_FX)` and `target:ApplyBuff`. BUFF_FX = `skill_dh2_prince_mage_illusionist_eclipse_persist`.
- bufftime = SnS_Duration (3000 + 1000·L ms by the class row). The Lua fallback `3000 + 1000·L` is identical.
- No damage. No stun. No boss/miniboss exclusion (Eclipse hits bosses; TimeControl does not). No sound call.
- Re-cast: the single upvalue `buff` is overwritten per target and removed with a global `RemoveBuff(CLASS_ID, buff)`. See INFERENCE D13/D14.

**Header claim vs data:** the header says "they attack at close range but can't move or target MC". Neither the Lua nor the class rows encode a move or target lock. The buff class rows (Skill_Illusionist_Eclipse, p2 = SnS_Level):
- Speed_Modifier_Walk (46), Speed_Modifier_Rotation (47), Speed_Modifier_Attack (48): p1 = -25 (raw -6400), p3 = -1 per L. Values: L=1 -26, L=5 -30 (units).
- Speed factor (port `original_speed_modifier`, PROVEN for walk: `1 + raw·0.01`): -26 gives 0.74; -30 gives 0.70. This is a percent slow, not a stop.
- TempProp1 (190) and Rating_Attack (50): p1 = 0, p3 = 1698038016 (raw). Interpreted as fixed point this gives about 6.6e6 per level (wrapping), which cannot be a real value. Read as a float32 bit pattern it is 5.37e22. **Unresolved anomaly (D6). Do not use these two rows.**

**Projected:** duration 4000 ms (L=1), 8000 ms (L=5). Mana 58 / 90 (50 + 8·L). Speed -26 / -30.

**Atlas:** resolves Mage_Eclipse animation, do_skill event (handler 0x3d4434, same for all do_skill events), spell and persist FX tables, sound table sfx_skill_mage_ilu_eclipse (+ .wav present), Skill_Illusionist_Eclipse table, FactionBuff not used here. Consistent with the Lua.

**IDA handlers:** TargetListSearch 0x38fcf0; SetTargetListCharacterFilter 0x390690; SetTargetListObjectFilter 0x390624; SetTargetListSorting 0x390548; PopTargetList 0x38fbb8; LookAt 0x3b8ed8; PlayFX 0x3923cc; CreateBuff 0x3b86a8; RemoveBuff 0x3b842c; ApplyPropClass 0x3babdc; IsPlayer 0x38e9f0; IsCharacter 0x38ea1c.

**Port state:**
- CAST: MISSING. No profile row in any registry (`find_skill_use_profile_registered_v1`, runtime_skill_use_profile_registry_v1.hpp). Rejected at runtime_skill_cast_coordinator_v1.cpp:719.
- ANIM: clip `skill_dh2_prince_mage_illusionist_eclipse.bdae` present. Do_skill routes to the generic native event bridge (combat_session.cpp:551). Not run for this skill: UNVERIFIED.
- FX: MISSING. Per-target PlayFX is only a conditional field (`target_hit_fx_name`, only for dmg > 0; runtime_skill_use_profile_v1.hpp).
- SOUND: no Lua PlaySound. Cue `sfx_skill_mage_ilu_eclipse` exists (matrix link by ordinal 528), but the caller is not in the Lua. Source of the cue is unproven (see section 6).
- DAMAGE: none authored.
- EFFECT: MISSING. Enemy-target CreateBuff/ApplyBuff call site does not exist. The buff owner is actor-generic (runtime_timed_buff_v1.cpp:174), so the data path is close, but the rotation and attack speed rows have no consumer (only walk, runtime_enemy_navigation_v1.cpp:144).

### 2.2 Figments (id 45, display "Nightmare")

**Original Lua (PROVEN, source A):**
- OnSkillCheck: `not HasSkillCooldown() and HasMana(mana_cost)`.
- OnPreSkill: `UseMana(mana_cost)`, `SetSkillCooldown(cooldown)` where cooldown = SnS_Cooldown (class: 640000 raw = 2500 ms, no level scaling).
- OnSkill: `TargetListSearch(1000, ANGLE 360)`, filters Enemy / NONE objects / ClosestFirst. `PlaySound("sfx_skill_mage_ilu_nightmare")`. Then per popped target:
  1. `ApplyPropClass(Skill_Illusionist_Figments)` (caster-side; writes the SnS_Dot_* values the engine reads for the DoT).
  2. `SkillCombatRoll(target)`.
  3. If not boss and not miniboss: `target:SetStunState(GetRand(1500, 4000), true)`.
  4. `CreateBuff(Skill_Illusionist_Figments_buff, BUFF_DURATION, false, L, spell_dh2_prince_illusionist_figments)` and `ApplyBuff`. The buff class has **no property rows**, so it is FX-only over the DoT duration.
- No Post.

**Class rows (projected, in units = raw/256):** mana 75 + 15·L → 90 / 150 (L=1/5); DoT min 10 + 2·L (+0.5·Stat_Energy, p2 = 152); DoT max 20 + 3·L (+0.5·Energy); DoT duration 4000 + 250·L ms → 4250 / 5250; cooldown 2500 ms.

**Discrepancies (D1, D2):** the header says Mana 15 and Cool down 3 s; the class rows give 90 / 150 mana (L=1/5) and 2500 ms. The Lua code reads the class rows, so the code is the reference (PROVEN for cooldown: the code reads SnS_Cooldown). The mana header is stale (INFERENCE, LIKELY). Confirm in game before treating either as truth.

**Header claim:** "DoT 5/sec, DoT duration 5 sec". The class gives DoT min 12 / max 23 per tick at L=1 (not 5/sec). Treat header as stale (INFERENCE).

**IDA handlers:** as Eclipse, plus Random::GetRandom(int,bool) 0x33ff90; SetStunState 0x3b9580 (native SM_SetStunState 0x3c5ffc, IsBoss gate natively, state 9 + event 50012, TMR type 43, see section 4 N4); SkillCombatRoll 0x3b9fbc; ApplyPropClass 0x3babdc.

**Port state (profile row `runtime_skill_use_profile_magesub_v1.hpp:38-43`, admitted by `find_skill_use_profile_registered_v1`):**
- CAST: PARTIAL. Row admitted, coordinator path wired (runtime_skill_cast_coordinator_v1.cpp:136-146). Cooldown uses the class projection (fixed_cooldown_ms = 0; cast coordinator line 765). Unit test file exists (`runtime_skill_use_profile_magesub_v1_tests.cpp`). Per skills-mage-sub-report.md the branch was never compiled or run, and MASTER-HANDOFF says the subclass activation fixture prints SKIP. Status: admitted, NOT verified.
- ANIM: clip `skill_dh2_prince_mage_illusionist_figments.bdae` present; generic path (UNVERIFIED).
- FX: MISSING. buff = nullptr. The FX-only buff (spell_dh2_prince_illusionist_figments) is not played.
- SOUND: MISSING. The profile has no sound field and no PlaySound arm. **The matrix claim "no sound linked" is wrong: the Lua calls PlaySound on line 84.**
- DAMAGE: PARTIAL. SkillCombatRoll per target (rolls_combat = true) is admitted. The DoT is MISSING (magesub header lines 14-17: no DoT tail in CombatSession).
- EFFECT: MISSING. SetStunState (random 1500-4000, non-boss) MISSING (no stun state in port; see N4). FX-buff MISSING (see N1).

### 2.3 Terror (id 110, display "Confusion")

**Original Lua (PROVEN, source A):** a conversion ("confusion") skill with a 3-slot memory.
- OnSkillCheck: `HasMana(mana_cost)`. No cooldown check. OnPreSkill: empty. No SetSkillCooldown.
- OnSkill: filters Enemy / NONE objects / FrontalFirst; `TargetListSearch(650, ANGLE 240)`. Pops targets until one is found that is not already in a slot (`not_there` by name), `LookAt(target)`, then `check_slots(target)`.
- `check_slots`: a free slot (1, 2, 3) takes the target. If all three are full, the oldest is freed (`delete_slot`) and replaced.
- `add_slot(slot, item)`:
  1. `UseMana(mana_cost)`
  2. `ApplyPropClass(Skill_Illusionist_Terror)`
  3. `SkillCombatRoll(item)` (initial roll on the converted target)
  4. `item:CreateBuff(FactionBuff, BUFF_DURATION, false, L, spell_dh2_prince_illusionist_terror)` and `ApplyBuff`
  5. `StartSkillTimerCB(BUFF_DURATION, false, delete_slot, slot)`
- `delete_slot` calls a **global** `RemoveBuff(BUFF_ID, SLOTn_BUFF)` (see INFERENCE D13).
- BUFF_DURATION = SnS_Fear_Duration (class: 4250 + 750·L ms; projected 5000 / 8000 at L=1/5). Failsafe 2000 + 1000·L only when zero.
- AIStates.Scared is declared (`CST_SCARED`) and never used.
- FactionBuff class row (PROVEN, class 70): `dest 0 (AIFaction) type 9 p1 = 8`. The class sets AIFaction to 8. Two related rows exist (FactionBuff2 = 13, FactionBuff3 = 0) but are not used by these skills.

**Class rows:** mana 40 + 5·L (45 / 65 at L=1/5). No damage rows (MinDamage / MaxDamage = 0).

**Header claim:** "Cool down: 3 sec". The Lua has no cooldown code and the class has no SnS_Cooldown row. Treat as unresolved (D3).

**IDA handlers:** TargetListSearch 0x38fcf0; SkillCombatRoll 0x3b9fbc; ApplyPropClass 0x3babdc; CreateBuff 0x3b86a8 → PROPS_AddBuff 0x3e232c (capacity 1 = replace same class, see section 5 N2); RemoveBuff 0x3b842c; LookAt 0x3b8ed8. StartSkillTimerCB native handler: not located in this pass.

**Port state:**
- CAST: MISSING. No profile row.
- ANIM: clip `skill_dh2_prince_mage_illusionist_terror.bdae` present (generic path, UNVERIFIED).
- FX: MISSING (spell_dh2_prince_illusionist_terror is the buff FX).
- SOUND: no Lua PlaySound. Cue `sfx_skill_mage_ilu_terror` exists (matrix link), caller not in Lua (section 6).
- DAMAGE: initial SkillCombatRoll is MISSING with the rest of the cast.
- EFFECT: MISSING. Faction flip MISSING (N3). Slot memory with timed release MISSING (N7). Enemy FactionBuff MISSING (N1).

### 2.4 TimeControl (id 111, display "Hold Time")

**Original Lua (PROVEN, source A):**
- OnSkillCheck: `not HasSkillCooldown() and HasMana(mana_cost)`.
- OnPreSkill: filters Enemy / AttackableOnly / ClosestFirst; `TargetListSearch(1000)`; `LookAt(target)`; `SetSkillCooldown(cooldown)`; `UseMana(mana_cost)`.
- cooldown = SnS_Cooldown raw 1536000 = 6000 ms (no level scaling; fallback 6000 if ≤ 0).
- OnSkill: `bufftime = 4000 + 1000·L` (hard-coded in Lua; equals the SnS_Slow_Duration row, which the Lua does not read). `PlaySound("sfx_skill_mage_ilu_hold_time_hit")`. `ApplyPropClass(Skill_Illusionist_TimeControl)`. `TargetListSearch(1000)` (filters inherited from OnPre). For each popped character target that is not the player, not boss, not miniboss: `CreateBuff(Buff_Illusionist_TimeControl, bufftime, false, L, spell_dh2_prince_illusionist_timecontrol)` and `ApplyBuff`.

**Buff rows (PROVEN, class 46 Buff_Illusionist_TimeControl):** Speed_Modifier_Walk / Rotation / Attack, p1 = -30, p3 = -3 per L. Speed factor 0.70 - 0.03·L (L=1 0.67, L=5 0.55). The projected SnS_Slow_Duration and SnS_Slow_Target_Chance and TempProp1 rows on the skill class are NOT read by the Lua (D8).

**Projected:** mana 58 / 90; slow duration 5000 / 9000 ms; cooldown 6000 ms.

**IDA handlers:** as above; plus PlaySound (LuaScript::_PlaySound 0x0037e578, atlas).

**Port state:**
- CAST: MISSING.
- ANIM: clip `skill_dh2_prince_mage_illusionist_time_control.bdae` present (generic, UNVERIFIED).
- FX: MISSING for the buff FX.
- SOUND: MISSING. Lua plays the `_hit` variant at Use. The matrix lists the non-hit cue `sfx_skill_mage_ilu_hold_time` (wrong cue). Atlas resolves both files.
- DAMAGE: none authored.
- EFFECT: PARTIAL infrastructure. The monster-side slow path exists (faery2 `Debuff_Slow`: `debuff_slow_spec_from_duration_v1`, runtime_timed_buff_v1.cpp:99-105, with boss gate). The walk rate follows the live Speed_Modifier_Walk (runtime_enemy_navigation_v1.cpp:144). Rotation and attack speed have no consumer (N11). The skill-driven enemy CreateBuff call site is MISSING (N1).

## 3. Passive skills

Passives are event hooks or permanent self/party buffs. The matrix marks all four "no gameplay effect in code". The port has no passive stat system and no AI-event subscription (N5, N6). Passive status is therefore MISSING for all four.

### 3.1 Disciples (id 27, "Hypnotic Gaze")

**Original Lua (PROVEN, source A):** `StealAIEvent("OnTargetHit", OnTargetHit_Skill)`. If skill_level > 0, the attacker is the MC, the defender has HP > 0 and is a character: `SetTarget(defender)`; roll `GetRand(1, 100)`; if roll < attack_chance and defender is not boss/miniboss: `buff[counter] = defender:CreateBuff(Skill_Illusionist_Disciples, 10000, false, L, spell_dh2_prince_mage_illusionist_hypnotic_gaze_persist)`, `ApplyBuff`, `StartSkillTimerCB(10000, BuffExpired)`, counter++.
- attack_chance = TempProp2 = 2 + 0.5·L percent (L=1 2.5%, L=5 4.5%).
- RANGE = TempProp1 = 300 + 25·L, used only in trace text; the code never searches for nearby monsters.
- Class row (PROVEN): `AIFaction (dest 0) type 9 = 8`. The buff flips the struck monster's faction to 8 for 10 s. This is the same faction value as Terror's FactionBuff.

**Header vs code (D4):** the header says 10% chance, AoE 200, "nearby monsters target the enemy". The code uses 2 + 0.5·L %, has no area search, and affects only the struck defender. The header's "WILL affect the bosses faction" is contradicted by the boss check in code.

**IDA:** OnTargetHit is raised from `AISDefault::OnCombatResults(attacker, defender, results)` at 0x3dc9a8: it pushes both characters, and if `results & 3` is set it calls OnTargetMissed when flag 0x1000 is set, otherwise `Call("OnTargetHit", attacker, defender)` (PROVEN, pseudocode line 159472). Whether the **player's** AI raises the same event is not established: no `AISPlayer::OnCombatResults` symbol is in the export (INFERENCE, LIKELY that the player inherits the default; see open question Q3). The `StealAIEvent` wrapper is `_commons.luac:294-321`, backed by `LuaScript::_AddToVFTable` 0x37ec70. The commons comment says some events "are not propagated to Lua for the players" (PROVEN text).

**Port state:** MISSING. No OnTargetHit or StealAIEvent path in the port (grep: only comments, rogue profile line 13). The only AI-event bridge (combat_session.cpp:551) handles animation events. Faction flip MISSING (N3). Timed buff MISSING for monsters (N1).

### 3.2 Mesmerize (id 73, "Charisma")

**Original Lua (PROVEN, source A):**
- OnSkillUpdate: removes the prior self buff (global `RemoveBuff`), then if L ≠ 0 `CreateBuff(Skill_Illusionist_Mesmerize, 0, false, L)` and `ApplyBuff` on self, then `startGroupBuff()`.
- startGroupBuff: `TargetListSearch(1200)` with CharacterFilter = Player, NoSort. For each target whose name is not self and that `IsPlayer()`: `target:CreateBuff(BUFF_ID, 0, false, L)` and `ApplyBuff`. Duration 0 = permanent (no timer), capacity 1.
- Class row (PROVEN, class 171): Damage_Min_Main_Hand / Max / Off_Hand (+1·L), Rating_Attack (+5·L), Spell_Rating_Attack (+0.5·L).

**Header claim (D16):** "better prices with merchants". No Lua or class row encodes a price. Native `CharProperties::PROPS_GetModifiedPrice(int)` at 0x3de814 computes `((X + 25600) / 100 * price) >> 8` where X is the dword at CharProperties index 879 (byte offset 0xDBC), which this pass did not identify. Not in the port (grep: no PROPS_GetModifiedPrice / price modifier). **Open question Q5.**

**Port state:** MISSING. Passive stat application MISSING (N6). Group buff needs a Player-filter target list plus permanent party buffs (N6). Permanent buffs themselves exist (runtime_timed_buff_v1.hpp, Celest precedent).

### 3.3 Mirror (id 74, "Magnetic Shield")

**Original Lua (PROVEN, source A):**
- OnSkillUpdate: removes prior self buff; if L ≠ 0 `CreateBuff(Skill_Illusionist_Mirror, 0, false, L)` and `ApplyBuff` on self (permanent). Class row (PROVEN, class 172): Physical_Armor (dest 71) = base + 10·L (p1 = -666 keep, p3 = 2560).
- STUN = TempProp1 = 4 + 1·L percent (L=1 5%, L=5 9%). DURATION = TempProp2 = 3000 ms (flat, Level-based but p3 = 0).
- `StealAIEvent("OnTargetHit")`: if L > 0 and defender is the MC and `attacker:TargetInMeleeRange()`: roll `GetRand(0,100)`; if roll < STUN: `attacker:SetStunState(DURATION, true)`.

**Header vs code (D7):** header says "the attacker gets stunned or knocked back". Code stuns only. No knockback.

**IDA:** TargetInMeleeRange 0x3b7020; SetStunState 0x3b9580 → SM_SetStunState 0x3c5ffc (see N4).

**Port state:** MISSING for the passive (N5, N4). Self armor buff (permanent) has no class wiring yet (N6).

### 3.4 Petrify (id 82, "Withering Touch")

**Original Lua (PROVEN, source A):** `StealAIEvent("OnTargetHit")`. If L > 0 and attacker is the MC: `currentDamageRating = FromFixed(target Damage_Min_Main_Hand)`. Threshold: boss or miniboss needs > 30, others > 10. If the threshold passes: `SetProp(SnS_Level, L, true)` (global, on MC), then `buff = target:CreateBuff(Skill_Illusionist_Petrify, 0, 5, L)` and `target:ApplyBuff`. Duration 0 = permanent. Third argument 5 = capacity (see N2).
- Class row (PROVEN, class 173): Damage_Min_Main_Hand and Damage_Max_Main_Hand, p1 = -1, p3 = -2 per L. Per instance: L=1 -3, L=5 -11 damage points.
- Stacking: up to 5 instances, FIFO eviction (native PROPS_AddBuff capacity semantic, section 5 N2).

**IDA:** PROPS_AddBuff 0x3e232c (the `a4 <= 0 → 128` default and the eviction loop at lines ~85-142 of its pseudocode, read this pass; LIKELY).

**Port state:** MISSING. Passive hook MISSING (N5). Capacity arg not in `TimedBuffSpecV1` (runtime_timed_buff_v1.hpp:24-40), and `apply_timed_buff` replaces by class name (runtime_timed_buff_v1.cpp:214-215), so 5 stacks cannot be modelled yet (N2).

## 4. Native (IDA) handler index used by this document

| Function | Handler | Note |
|---|---|---|
| TargetListSearch | GameObject::_TargetListSearch 0x38fcf0 | |
| TargetListSearchRect | GameObject::_TargetListSearchRect 0x392b80 | not used |
| PopTargetList | GameObject::_PopTargetList 0x38fbb8 | |
| SetTargetList{CharacterFilter,ObjectFilter,Sorting} | 0x390690 / 0x390624 / 0x390548 | |
| LookAt / LookAtTarget | Character::_LookAt 0x3b8ed8 / _LookAtTarget 0x3b5684 | |
| SetTarget / ClearTarget | Character::_SetTarget 0x3b8f38 / _ClearTarget 0x3b5690 | Disciples calls SetTarget |
| PlayFX | GameObject::_PlayFX 0x3923cc | |
| CreateBuff | Character::_CreateBuff 0x3b86a8 → PROPS_AddBuff 0x3e232c | arguments: class, duration, capacity (bool false → 1, bool true → 0, number → that number), strength, fx, source |
| RemoveBuff | Character::_RemoveBuff 0x3b842c | operates on the **calling** character's buff map (see D13) |
| ApplyBuff | not named in IDA export; atlas maps to _ApplyPropClass 0x3babdc | atlas-only, UNVERIFIED here (D17) |
| ApplyPropClass | Character::_ApplyPropClass 0x3babdc | |
| SkillCombatRoll | Character::_SkillCombatRoll 0x3b9fbc | |
| SetStunState | Character::_SetStunState 0x3b9580 → CharStateMachine::SM_SetStunState 0x3c5ffc | IsBoss gate natively (line ~142854 onward); state 9, event 50012, TMR type 43 |
| SetScareState | Character::_SetScareState 0x3b96b0 → SM_SetScareState 0x3c6144 | not used by Illusionist |
| SetSkillCooldown | Lua commons (_commons.luac:205); IDA name Character::_SetSkillCooldownTimerId 0x3b97e0 | role of the native handler not read this pass |
| TargetInMeleeRange | Character::_TargetInMeleeRange 0x3b7020 | |
| IsBoss / IsMiniBoss | Character::IsBoss 0x3a3158 / IsMiniBoss 0x3a3144 | |
| IsPlayer / IsCharacter / GetID / GetName | GameObject 0x38e9f0 / 0x38ea1c / 0x38ebe4 / 0x38ebf0 | |
| GetRand | Random::GetRandom(int,bool) 0x33ff90 | |
| StealAIEvent | Lua commons (_commons.luac:294) → LuaScript::_AddToVFTable 0x37ec70 | hashes event name, registers proxy |
| AI event raise | AISDefault::OnCombatResults 0x3dc9a8 → Call("OnTargetHit") | PROVEN for AISDefault; player AI see Q3 |
| StartSkillTimerCB | Lua commons (_commons.luac:146) | native handler not located |
| PROPS_DebuffSlow | CharProperties::PROPS_DebuffSlow 0x3e2a5c | port: runtime_timed_buff_v1.cpp:99-105 |

Atlas vs IDA: atlas and IDA agree on every handler address listed. Where the atlas and the port disagree in scope, see section 7.

## 5. Port-state summary

| Skill | CAST | ANIM | FX | SOUND | DAMAGE | EFFECT |
|---|---|---|---|---|---|---|
| Eclipse | MISSING | UNVERIFIED (clip present) | MISSING | cue, no caller | none | MISSING (enemy buff, speed rows) |
| Figments | PARTIAL (admitted, not run) | UNVERIFIED (clip present) | MISSING | MISSING | PARTIAL (roll only; DoT MISSING) | MISSING (stun, FX buff) |
| Terror | MISSING | UNVERIFIED (clip present) | MISSING | cue, no caller | MISSING | MISSING (faction, slots) |
| TimeControl | MISSING | UNVERIFIED (clip present) | MISSING | MISSING (_hit cue) | none | PARTIAL infra (walk slow exists) |
| Disciples | MISSING (passive) | n/a | MISSING | n/a | n/a | MISSING (hook, faction) |
| Mesmerize | MISSING (passive) | n/a | MISSING | n/a | n/a | MISSING (stat rows, group buff, price) |
| Mirror | MISSING (passive) | n/a | n/a | n/a | n/a | MISSING (hook, stun, armor row) |
| Petrify | MISSING (passive) | n/a | n/a | n/a | n/a | MISSING (hook, capacity 5) |

## 6. Sound / animation / FX resolution

Animation (clips from the matrix, existence from atlas): Mage_Eclipse / Mage_Figments / Mage_Terror / Mage_TimeControl animation tables exist; their `do_skill` events route to the skill Use (atlas: handler 0x3d4434; port combat_session.cpp:551). Clips: `skill_dh2_prince_mage_illusionist_{eclipse,figments,terror,time_control}.bdae` present in the staged assets (matrix `anim_present: true`).

Sound (name → atlas file; Lua call site):

| Cue | Lua call | Atlas file | Status |
|---|---|---|---|
| sfx_skill_mage_ilu_nightmare | Figments, PlaySound line 84 | sfx_skill_mage_ilu_nightmare.wav present | RESOLVED; matrix wrongly says unlinked |
| sfx_skill_mage_ilu_hold_time_hit | TimeControl, PlaySound line 83 | sfx_skill_mage_ilu_hold_time_hit.wav present | RESOLVED; matrix lists the non-hit cue instead |
| sfx_skill_mage_ilu_hold_time | none in Lua | .wav present | matrix cue, no caller in Lua |
| sfx_skill_mage_ilu_eclipse | none in Lua | .wav present | matrix cue (ordinal 528), no caller in Lua |
| sfx_skill_mage_ilu_terror | none in Lua | .wav present | matrix cue (ordinal 532), no caller in Lua |

Port sound path: PlaySound is generic (features/audio/runtime_session_audio_v1.*; mage owner plays `sfx_skill_mage_thunder_braid` through it, runtime_mage_skill_owner_v1.cpp:61). No Illusionist skill calls it yet.

FX (AnimatedEffectTable name → atlas effect table; Lua use):

| FX | Atlas | Lua use |
|---|---|---|
| spell_dh2_prince_mage_illusionist_eclipse | table d325673e… | Eclipse PlayFX per target |
| skill_dh2_prince_mage_illusionist_eclipse_persist | table e344a056… | Eclipse buff FX |
| spell_dh2_prince_illusionist_figments | table 10bcd38d… | Figments buff FX |
| spell_dh2_prince_illusionist_terror | table dd1d67b0… | Terror FactionBuff FX |
| spell_dh2_prince_illusionist_timecontrol | table 6fd1bd60… | TimeControl buff FX |
| spell_dh2_prince_mage_illusionist_hypnotic_gaze_persist | table bd71e104… | Disciples buff FX |
| (none) | — | Mirror, Mesmerize, Petrify: no FX |

## 7. ENGINE NEEDS (general systems; Illusionist needs listed)

Each is named by engine function or concept. None is a per-skill hack.

- **N1. Skill-driven CreateBuff/ApplyBuff on enemy targets (per-target loop, FX per buff, duration per target, capacity, boss gating).** Needed by Eclipse, Figments (FX buff), Terror (FactionBuff), TimeControl, Disciples, Petrify. Native: Character::_CreateBuff 0x3b86a8 → PROPS_AddBuff 0x3e232c. Port: the owner `RuntimeTimedBuffOwnerV1` is actor-generic (runtime_timed_buff_v1.cpp:174) but no monster call site exists. Note that the FX-only buff (Figments) needs a class with no rows.
- **N2. Buff capacity (PROPS_AddBuff a4).** Semantics: 1 = replace the same class (current port behaviour); N > 1 = keep up to N instances, FIFO eviction; ≤ 0 = 128 (IDA, LIKELY). Needed by Petrify (5), Mesmerize / Eclipse / TimeControl / Disciples / Terror (1, already equal to replace). Port: `TimedBuffSpecV1` has no capacity field.
- **N3. Buff-driven AIFaction write (type-9 row on dest 0) with re-sync.** Needed by Terror (FactionBuff → 8), Disciples (class → 8). Port: `actor.faction_id` is set once at spawn (combat_session.cpp:1504). Consistency checks treat a later mismatch with the property sheet as an error (playable_actor_world.cpp:88; target_selection_runtime.cpp:13-14). Hostility lookup `ai_enemy(tables, factionA, factionB, ...)` already exists (playable_actor_world.cpp:180; runtime_ranged_skill_monster_v1.cpp:729), so only the live-faction re-sync is missing. Faction 8's relations to the monster factions are not yet read from the table (open question Q4).
- **N4. Monster stun state (SetStunState) and scare state.** Needed by Figments (random 1500-4000 ms, non-boss) and Mirror (3000 ms on the attacker). Native: Character::_SetStunState 0x3b9580 → CharStateMachine::SM_SetStunState 0x3c5ffc (TMR_Start type 43 with the duration; state 9; event 50012; IsBoss gate; immediate `_SetState` when the bool is true). Port: no stun state (grep). Knight Warcry needs the same (scare + stun).
- **N5. Passive AI-event subscription (StealAIEvent / OnTargetHit).** Needed by Disciples, Mirror, Petrify, and the other subclasses' passives (archer_precision, archer_sidestep, assassin_blade_deflect, assassin_blindness, assassin_contaminate, berserker killing_frenzy / killing_streak / survival, necromancer carrioneater / putrid, paladin conversion / holy_weapon / shield). Native: LuaScript::_AddToVFTable 0x37ec70 (registration) + AISDefault::OnCombatResults 0x3dc9a8 (raise OnTargetHit / OnTargetMissed). Port: none; the only AI-event bridge handles animation events (combat_session.cpp:551). Needs: an event on combat result with (attacker, defender, result bits), a per-listener predicate (attacker is MC / defender is MC), and an optional OnTargetMissed branch.
- **N6. Always-on passive class rows (permanent buff) and party application.** Needed by Mesmerize (self + allies with Player filter; stat rows) and Mirror (self armor row). Port has a permanent path (runtime_timed_buff_v1.hpp, Celest passive). Missing: the level-change trigger, the Player-filter target loop, and the price modifier (PROPS_GetModifiedPrice 0x3de814, field X unidentified).
- **N7. Timer callback on expiry (StartSkillTimerCB with a Lua callback).** Needed by Terror (slot release at BUFF_DURATION) and Disciples (BuffExpired at 10 s). The buff owner already expires buffs; the missing piece is a per-skill callback. Port: a timer owner exists only inside runtime_mage_skill_owner_v1.cpp (mage-specific by its header).
- **N8. Skill DoT tail (SkillCombatRoll reads SnS_Dot_* from the caster's sheet and applies poison ticks).** Needed by Figments. Port: documented gap (magesub header lines 14-17). The slow outcome branch exists (outcome bit 0x100 → Debuff_Slow, runtime_timed_buff_v1.cpp:99-105); a DoT branch is the sibling and is NOT verified in the native F_ApplyResult (0x3b10b4) for this pass.
- **N9. Per-target PlayFX arm (unconditional).** Needed by Eclipse (PlayFX per target, every target). Port: `target_hit_fx_name` plays only for dmg > 0 (runtime_skill_use_profile_v1.hpp, ColdRay). Small and general.
- **N10. Profile sound arm (PlaySound name at Use).** Needed by Figments and TimeControl. Port: no field. Small and general (runtime_session_audio_v1 resolves names).
- **N11. Monster rotation and attack speed modifiers.** Needed by Eclipse (-25 - L) and TimeControl (-30 - 3L). Port: only walk (Speed_Modifier_Walk, runtime_enemy_navigation_v1.cpp:144) is consumed. Rotation and attack consumers not located in this pass.
- **N12. Filters and cone (checked, no new system).** Needed by all actives. Port: `runtime_skill_target_query_v1.cpp` implements the cone rule (line 306: π or more = unlimited, which matches Figments' ANGLE 360 mapped to kSkillPiV1; PROVEN from code). Player and NONE filters for Mesmerize / Figments are to be confirmed in phase 2.
- **Not needed:** SpawnSkillProjectile, summon, knockback. None of the 8 Lua files calls them.

## 8. Discrepancies (header vs code vs matrix vs atlas)

- D1 Figments cooldown: header 3 s; class 2500 ms; code reads class. Code is the reference (PROVEN).
- D2 Figments mana: header 15; class 75 + 15·L raw (90 at L=1). Unresolved. Check against in-game display.
- D3 Terror cooldown: header 3 s; Lua has no cooldown code; no class row. Unresolved; the Lua is the only executable reference.
- D4 Disciples chance and radius: header 10% and AoE 200 with area search; code uses 2 + 0.5·L % and no area search.
- D5 Eclipse "can't move or target MC": absent from Lua and class rows; class rows give -25 - L % speed (walk, rotation, attack).
- D6 Eclipse Rating_Attack and TempProp1 rows: p3 = 1698038016, unresolvable as fixed point (see section 2.1).
- D7 Mirror "stunned or knocked back": code stuns only.
- D8 TimeControl: SnS_Slow_Target_Chance (90 + L %) and TempProp1 (30 + 3L) and SnS_Slow_Duration rows are not read by the Lua (duration is hard-coded 4000 + 1000·L, equal to the row).
- D9 Figments mana/DoT header vs class (see D2 and section 2.2).
- D10 Matrix sound flags: Figments "not linked" (wrong, Lua line 84); TimeControl non-hit cue (wrong cue); Eclipse and Terror cues have no Lua caller (source unknown).
- D11 Matrix passives: "no gameplay effect in code" is correct.
- D12 Petrify header matches the code (decreases damage per hit). Disciples header does not (D4).
- D13 INFERENCE (LIKELY): native `_RemoveBuff` (0x3b842c) operates on the calling character's buff map. The Lua calls it globally (on the MC) with a buff handle that belongs to another actor (Terror delete_slot, Disciples BuffExpired, Eclipse re-cast). Those calls probably do not remove the monster's instance, so monsters keep the buff until its own timer ends. Confirm by tracing PROPS_DelBuff with a foreign pointer.
- D14 INFERENCE (LIKELY): Eclipse re-cast removes at most the last target's buff (single upvalue, same mechanism as D13).
- D15 Terror slots: because of D13, the converted-monster count may exceed 3 while buffs last (INFERENCE, LIKELY).
- D16 Mesmerize "better prices": no Lua/class encoding; native PROPS_GetModifiedPrice field unidentified.
- D17 ApplyBuff handler: not in the IDA export; the atlas maps it to _ApplyPropClass 0x3babdc (unverified here).
- D18 StartSkillTimerCB native handler: not located.

## 9. Open questions (for phase 2 or the lead)

- Q1. Does Eclipse have any "cannot move or target" effect in the native state machine, outside the Lua and class rows? (Look at `CharStateMachine` for speed or target locks keyed by a buff.)
- Q2. What do the 1698038016 rows (D6) mean in the native property schema? A decoder check of the CharacterProperties type for dest 50 and 190 is needed.
- Q3. Does the player's AI raise OnTargetHit (AISPlayer or AISPlayerIPhone inherits AISDefault::OnCombatResults?). The commons comment says some events are not propagated to Lua for players. Passives Disciples, Mirror and Petrify depend on it.
- Q4. Faction 8 relations: read the faction table row for faction 8 against the monster factions (`dh2::data::ai_enemy`). Decides whether Terror and Disciples make monsters attack each other or the MC.
- Q5. The Mesmerize price field: identify CharProperties dword 879 (byte offset 0xDBC) and its writers.
- Q6. Verify D13 by IDA: PROPS_DelBuff (called from _RemoveBuff) with a pointer that is not in this character's list.
- Q7. Figments DoT: does the native F_ApplyResult tail (0x3b10b4) read SnS_Dot_* from the attacker sheet? Needed to size N8.
- Q8. Mana display: is the Figments/Terror mana shown to the player the class projection (75 + 15·L raw / 256) or a different number?

## 10. Phase 2 suggestions (not done here)

- Build order for the shared engine pieces (for the lead): N2 + N1 (enemy-target buff with capacity), then N3 (faction re-sync), then N4 (stun), then N5 (passive event bus). N9 and N10 are small and can go first.
- Verification per skill (phase 2): Eclipse and TimeControl first (buff arm + speed), then Figments (stun + FX buff + sound; DoT blocked on N8), then Terror (faction + timers). Passives after N5.
- Real-run prerequisite: regenerate the subclass melee bindings in the run's `.local-inputs` (subclass-bindings-manifest.json says the export and checker passed for the primary checkout). Activation fixtures print SKIP without this (MASTER-HANDOFF).

## 11. Files touched by Phase 1

- `coordination/v21.8/sub-illusionist-phase1.md` (this file). No other repo files changed. Scratch files were written only under `C:\dh218\illusionist\` (outside the repo).
