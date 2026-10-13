# Paladin (Knight subclass) - Phase 1 investigation

Branch `v21.8-sub-paladin`, worktree `.claude/worktrees/sub-paladin`, base `windows-foundation` @ 7b9028cb.
Phase 1 only: no engine code, no hidden-window run, no CTest change, no subst letter used. Not pushed, not merged.

Scope: the Paladin tree (CharacterTable 265 `KnightPlayerBase_Paladin`, SkillTree `KnightPaladin`, 16 entries).
Paladin-owned skills (per `SKILL-VERIFICATION-MATRIX.json` tree ownership): 4 actives and 4 passives (section 1).
Knight-base skills the Paladin also lists (BashDown, Charge, GroundSlam; CombatMaster, FastMetabolism, Hardiness,
Leadership, PotionAddict) are not covered here. The three actives belong to skills-a.

Evidence key: PROVEN = read from the original Lua text or from the IDA handler code. LIKELY = strong, not shown end to end.
GUESS = plausible only. "Port" = `port/windows-foundation/features/generic_skills/` unless a path is given.

## 0. Sources, method and corrections to earlier notes

Sources used:
- Original Lua: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_*.luac` (8 files).
- Class sheets: ClassTable rows 190-201 and 225 (`SnS_CleanUp`), read from the atlas mob file
  `coordination/atlas/general/mobs/265.md` and the `character_classes#group0[N]` rows of the atlas (read-only).
  Formula semantics from the port evaluator `port/game-data/class_tables.cpp:19-26`. Evaluated for SnS_Level 1 and 5 and
  Level 1 with no weapon (helper scripts kept outside the repo in `C:/dh218/paladin/`).
- Property names: `port/android-native/app/src/main/cpp/source_process_compiled_members_v121.hpp` (index 172 = SnS_Level,
  173 = SnS_ManaCost, 174/175 = SnS_MinDamage/MaxDamage, 176 = SnS_Duration, 177 = SnS_Cooldown, 183 = SnS_Push_Target_Chance,
  184 = SnS_Stun_Target_Chance, 185 = SnS_Stun_Duration, 190 = TempProp1, 191 = TempProp2, 39/40 = Regen_HP/CombatRegen_HP,
  0 = AIFaction, 19 = Level, 71 = Physical_Armor, 83-89 = Damage_Bonus_* (Sword, Axe, Mace, Dagger, Range, Staff, Wand)).
- IDA: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c` (the brief omitted the
  `libraries/` segment). Functions are found by their `/* FUNCTION <addr> <name> */` header.
- Native binding table: `port/level-world/character_script_owner_bindings.inc` and `gameobject_lua_catalog.inc`.
- Atlas engine status: `coordination/atlas/README.md` "Engine functions to implement next". These statuses describe the
  Lua VM bindings used by monster scripts. The player skill path is C++ profile rows, so MISSING there does not mean a
  skill cannot run.
- Staged package: `.local-inputs/windows-source-clock-v21.8-staged`; selection `coordination/v21.8/asset-selection-v21.8.json`.

Corrections to the brief and to earlier notes:
1. The `.luac` files are plain-text Lua, not bytecode. `.local-inputs/lua_read_v13.py` is not a decoder. It greps a
   disassembly reference for block names and returned nothing for these scripts. The scripts were read directly.
2. The brief's IDA path is missing the `libraries/` segment (corrected above).
3. `SKILL-VERIFICATION-MATRIX.json` gives `cast_mode: "unknown (player Lua not in package)"` for DivineJudgement,
   OffensiveAura, GuardingAura and PurifyingAura. The four scripts are in the original data and in the staged package
   (`assets/data/scripts/skills/`). That matrix cell is stale.
4. `skills-knight-report.md` says PurifyingAura's heal is a gap. Confirmed. But the heal is not a buff-sheet write. IDA
   shows that the two-argument `SetBuffProp` is bound to the native `SetProp` (0x3b7eac), and that form is a live HP
   write (section 1.3, PROVEN).
5. The Knight header (`runtime_skill_use_profile_knight_v1.hpp`, comment block) says the Purifying class row "only carries
   Regen_HP / CombatRegen_HP". The row also sets TempProp2 (191) = 15 x level, which equals the Lua `HEAL_AMOUNT`.
6. The Knight header lists a "DivineJudgement push row" as a gap. The class data gives push chance 210 + 5 x level, which
   is above 100 at every level (section 1.4, LIKELY that every hit then pushes).

## 1. Paladin-owned skills

| Skill (SkillTable, req. char level) | Script | Kind | Lua shape | Port: CAST | Port: EFFECT / DAMAGE | Overall (phase 1) |
|---|---|---|---|---|---|---|
| OffensiveAura (80, L12) | prince_paladin_offensive_aura | active, buff | self buff + ally loop | admitted (unit only) | self buff arm PARTIAL; ally buffs MISSING | PARTIAL |
| GuardingAura (52, L18) | prince_paladin_guarding_aura | active, buff | self buff + ally loop | admitted (unit only) | self buff arm PARTIAL; ally buffs MISSING | PARTIAL |
| PurifyingAura (88, L15) | prince_paladin_purifying_aura | active, buff + heal | heal + self buff + ally loop | admitted (unit only) | heal MISSING; self buff arm PARTIAL | PARTIAL (heal gap) |
| DivineJudgement (28, L21) | prince_paladin_divine_judgement | active, hit | OnPre query + FX, Use rolls | admitted (unit only) | damage loop present (unverified); push tail MISSING; OnPre FX MISSING | PARTIAL |
| Shield / GuardianShield (100, L12) | prince_paladin_shield | passive | OnTargetHit: push/stun procs | none (text only) | MISSING (no passive hook, no stun/knockback state) | MISSING |
| EnergyBalance (39, L15) | prince_paladin_energy_balance | passive | faery-element elemental buff, refreshed every 3 s | none | MISSING (depends on faery element query) | MISSING |
| Conversion (18, L18) | prince_paladin_conversion | passive | OnTargetHit: chance to convert a monster | none (text only) | MISSING (no passive hook, no faction buff use) | MISSING |
| HolyWeapon (54, L21) | prince_paladin_holy_weapon | passive | OnTargetHit: attacker loses HP per hit | none (text only) | MISSING (no passive hook, no kill path) | MISSING |

No skill has a runtime-verified status yet. The Knight rows exist on the base branch (merged from skills-knight), but no
real hidden-window run has been done for any Paladin skill.

Common class-sheet facts (PROVEN from the rows): every Paladin row includes row 225 `SnS_CleanUp` first, which zeroes
SnS props 173-189 (178 = -1.0). Mana is `base + per_level x L` from the sheet, so the Lua constants `MANA_BASE` /
`MANA_SLVL` in guarding_aura are unused (the Lua calls `CalcManaCost`). Level-1 and level-5 values below are from the
evaluator: L1 = SnS_Level 1, L5 = SnS_Level 5, player Level 1, no weapon.

### 1.1 OffensiveAura (SkillTable id 80, script prince_paladin_offensive_aura, class Skill_Paladin_OffensiveAura)

Logic (Lua, PROVEN):
- OnSkillCheck: `HasMana(mana_cost)`.
- OnPre: if a buff is active, `RemoveBuff` it and stop its timer. Then `UseMana(mana_cost)` unconditionally.
- OnSkill (OnPre has cleared `buff`, so this always runs): `CreateBuff(BUFF_ID, 12000, false, rank, BUFF_PERSIST)` on the
  caster; `TargetListSearch(6000, 360)` with filters Player / NONE / NoSort; for each popped target whose name is not the
  caster's: `PlayFX(BUFF_FX2)`, `CreateBuff` + `ApplyBuff` on that target at rank; then `ApplyBuff(self)`, `PlayFX(BUFF_FX)`
  on the caster, and `StartSkillTimerCB(12000)` whose callback clears `buff`.
- OnSkillUpdate: when the rank changes while active, `SetBuffProp(SnS_Level)` updates the live buff strength.
- No `SetSkillCooldown`. No `OnSkillInfo_` return value.

Consequences (PROVEN from the Lua):
- Re-casting while active removes the buff, spends mana, and creates a new one. It refreshes, it does not toggle off.
- No code path in these scripts turns the aura off, and the class row has no MP drain formula. The header's "until the
  player stops it or the caster is too low on MP" is therefore not implemented in this script. LIKELY: it is not in this
  version (open question Q3).

Class sheet (row 200):
- SnS_ManaCost = 15 + 5 x L (L1 20, L5 40).
- Buff sheet on caster and allies: Damage_Bonus_Sword / Axe / Mace / Dagger / Staff / Wand (83, 84, 85, 86, 88, 89) =
  5 x L x TP1 + 0.742, where TP1 (190) = (230 + 25 x L) / 256. Values: L1 5.72, L5 35.41. Damage_Bonus_Range (87) and
  Dual are not touched. The "+0.742" term depends on formula type 6 (open question Q1).
- Buff duration is the Lua constant 12000 ms. SnS_Duration (176) is 0 for this row, so the constant is what applies.

Animation, sound and FX resolution:
- Anim: `3d/characters/prince/animations/skill_dh2_prince_warrior_paladin_offensive_aura.bdae`. Present, staged.
- Sound: step cue ordinal 560 (uid 105) -> `sfx_skill_warrior_pld_crusade.wav`, present, staged. The Lua has no
  PlaySound for this skill, so the sound comes from the animation's clip marker or step cue, not from Lua.
- FX (all in the original `3d/interface/`, all MISSING from the staged package):
  - BUFF_FX `skill_dh2_prince_warrior_paladin_offensive_aura.bdae` (played at Use on the caster).
  - BUFF_FX2 `spell_dh2_prince_warrior_paladin_offensive_aura.bdae` (played on each ally).
  - BUFF_PERSIST `skill_dh2_prince_warrior_paladin_offensive_aura_persist.bdae` (buff FX).
  - Texture `fx_skill_dh2_prince_warrior_paladin_offensive_aura_weapon.tga` is staged.
- Name-to-file rule: AnimatedEffectTable name -> `3d/interface/<name>.bdae` (LIKELY, from the atlas `assets.json` candidate
  paths). The rows are present in `effects_dictionary_pyarraynames.bin`.

IDA cross-check (PROVEN handler behaviour):
- `ApplyBuff` (and `ApplyPropClass`) share native 0x3babdc (`Character::_ApplyPropClass`). Its pointer-argument form calls
  `PROPS_ApplyClassToSheet` then `RecalcProperties`. This matches the port comment in `runtime_timed_buff_v1.hpp:9`.
- `CreateBuff` = 0x3b86a8 (`Character::_CreateBuff`). Arguments in order: class id, duration (0 means no expiry timer),
  a boolean (passed `false`, mapped to 1 by the handler), strength, FX id (default -1).
- `RemoveBuff` = 0x3b842c (`PROPS_DelBuff`). `UseMana` = 0x3b7864, `HasMana` = 0x3b78ec.
- `TargetListSearch` = 0x38fcf0 (`GameObject::_TargetListSearch`). Angle argument: degrees x 0.017453 x 0.5 (half-angle).

Port state (file:line):
- CAST: admitted. `runtime_skill_use_profile_knight_v1.hpp:77-82` (row), registry `runtime_skill_use_profile_registry_v1.hpp:11-20`,
  admission `runtime_skill_cast_coordinator_v1.cpp:146`. Unit test only. Not run in a window.
- ANIM: unverified. The coordinator requires the SkillTable animation sequence in the retained bank (coordinator ~866).
- FX: MISSING. No Use-time PlayFX in the profile (knight header gap), and the buff FX files are not staged.
- SOUND: unverified (depends on clip-marker dispatch).
- EFFECT (self): PARTIAL. Buff arm `runtime_skill_cast_coordinator_v1.cpp:1232-1264` sets strength = rank and duration
  12000 (`kOffensiveAuraBuffV1`, knight header line 33). Owner `runtime_timed_buff_v1.hpp:107`. Unit test only.
- EFFECT (allies): MISSING. No ally or Player-filter target query exists. Single-player inference: no other Player
  character exists, so only the self buff applies (INFERENCE, LIKELY).
- Rank change while active: MISSING. Strength is set at Use only.
- Re-cast: PARTIAL. The coordinator removes the previous buff of the same class (coordinator ~933-941) before
  `UseMana`. The port's order matches the Lua (remove, then UseMana) and the Lua calls remove before mana.
- DAMAGE: not applicable (`rolls_combat = false`).

### 1.2 GuardingAura (SkillTable id 52, script prince_paladin_guarding_aura, class Skill_Paladin_GuardingAura)

Logic (Lua, PROVEN): same shape as OffensiveAura. OnPre sets filters (no search), removes any active buff, and calls
`UseMana`. OnSkill creates the 12000 ms buff, loops over allies in 6000 / 360, applies the buff to each, applies it to
the caster, plays `BUFF_FX`, and starts the timer. The header says "Increases defense for 5 seconds" but the constant is
12000 ms. The code wins (LIKELY; the header is stale).

Class sheet (row 198):
- SnS_ManaCost = 15 + 5 x L (L1 20, L5 40).
- Buff on Physical_Armor (71) = L x TP1 + 0.742, with TP1 (190) = (51 + 6 x L) / 256. Values: L1 0.965, L5 2.324.

Resolution: anim present and staged; sound `sfx_skill_warrior_pld_spiritual_sight.wav` staged; FX
`skill_` / `spell_` / `_persist` `guarding_aura` are original-only (MISSING from staged); texture
`fx_skill_dh2_prince_warrior_paladin_guarding_aura_shield.tga` staged.

Port state: as OffensiveAura. Buff arm `kGuardingAuraBuffV1` (knight header line 39). Whether the buff owner resolves
Physical_Armor (71) on the actor is unverified.

### 1.3 PurifyingAura (SkillTable id 88, script prince_paladin_purifying_aura, class Skill_Paladin_PurifyingAura)

Logic (Lua, PROVEN):
- OnSkillCheck: `HasMana`. OnPre: `UseMana` only. No target query, no buff removal.
- OnSkill, `SearchForTargets`: if a buff is active, remove it. `TargetListSearch(6000)` with Player filter, no angle.
  For each Player target that is not the caster: `PlayFX(BUFF_FX2)`, `Heal(target, 15)`, `CreateBuff` + `ApplyBuff` at rank.
  Then `Heal(self, 15)`, `CreateBuff` + `ApplyBuff` on self, and `PlayFX(BUFF_FX)` on self. No timer and no cooldown.
- `Heal(t, amount)`: `HP_Curr, HP_Max = GetPropHP()`; `target = HP_Curr + rank x amount`; if `HP_Max > target` write
  `target`, else write `HP_Max`. Net effect: HP := min(HP + 15 x rank, MaxHP), in integer HP units.

Heal mechanism (IDA, PROVEN):
- `GetPropHP` (0x3b6cd8) pushes (HP >> 8, MaxHP >> 8, percent). So `HP_Curr, HP_Max` are HP and MaxHP.
- The Lua name `SetBuffProp` is bound to native 0x3b7eac, which is `Character::_SetProp`. With two arguments it calls
  `PROPS_Set`, which is a live property write. With a third argument that is a sheet pointer it calls `PROPS_SetToSheet`.
  So the heal is an immediate HP write, not a buff-sheet write. It happens before the self buff exists, so it does not
  depend on buff lifetime.

Class sheet (row 201):
- SnS_ManaCost = 10 + 5 x L (L1 15, L5 35).
- TempProp2 (191) = 15 x L (L1 15, L5 75). This equals the Lua `HEAL_AMOUNT` (15) times level, so the heal amount is
  authored in the class sheet too. The Lua hard-codes 15.
- Buff sheet: Regen_HP (39) = 6 x L (L1 6, L5 30); CombatRegen_HP (40) = 3 x L (L1 3, L5 15). The Lua does not read
  them. They act only through the regen consumer (unverified).
- TempProp1 (190) = 2 x L, unused by the Lua.
- SnS_Cooldown (177) = 10000 ms constant. The Lua never calls `SetSkillCooldown`, so this script does not set a cooldown
  (LIKELY; the value may be read elsewhere, open question Q6).
- Buff duration: Lua constant 12000 ms.

Resolution: anim present and staged; sound `sfx_skill_warrior_pld_alastor_blessing.wav` staged; FX
`skill_` / `spell_` / `_persist` `purifying_aura` original-only (MISSING from staged).

Port state:
- CAST: admitted (knight row `runtime_skill_use_profile_knight_v1.hpp:89-94`, `pre_range 0` matches the Lua). Unit only.
- HEAL: MISSING. The profile has no heal arm and no live HP write. Knight header gap confirmed.
- EFFECT (self buff): PARTIAL (buff arm `kPurifyingAuraBuffV1`, knight header line 45).
- EFFECT (allies): MISSING (same as OffensiveAura).
- Cooldown: `sets_cooldown = false`, which matches the Lua (no `SetSkillCooldown`).

### 1.4 DivineJudgement (SkillTable id 28, script prince_paladin_divine_judgement, class Skill_Paladin_DivineJudgementAura)

Logic (Lua, PROVEN):
- OnSkillCheck: `not HasSkillCooldown() and HasMana(mana)`.
- OnPre: filter Enemy / AttackableOnly, sort ClosestFirst, `TargetListSearch(600)`, `TargetListBackup("MaxAngleSearch")`.
  For every popped target: `PlayFX(spell_dh2_prince_warrior_paladin_divine_judgement)` on that target. Then
  `SetSkillCooldown(cooldown)` and `UseMana(mana)`. Both calls are unconditional, so they happen even when no target was
  found. `target` is set to the last popped target, or nil.
- OnSkill: `if (target)` only. Then `SetProp(SnS_Level)`, `ApplyPropClass(CLASS_ID)`,
  `TargetListSearch(600, 360, "MaxAngleSearch")`, and for each popped target `SkillCombatRoll(target)`. The damage return
  is unused.
- OnPost: `ClearTarget`. OnSkillInfo returns `cooldown` (for the UI).

Consequences (PROVEN from the Lua):
- No enemy in 600: MP and the 2 s cooldown are spent, no roll happens.

IDA (PROVEN):
- `TargetListSearch` with a string third argument (0x38fcf0, string branch) calls `GetResultsBackup(name)` and passes that
  backup as the search's `ResultsBackup` source (PROVEN). LIKELY: the OnSkill step therefore searches the OnPre
  "MaxAngleSearch" backup with range 600 and angle 360 rather than the live world. The port re-queries the world
  (`runtime_skill_cast_coordinator_v1.cpp:1159-1170`). In single-player the result is LIKELY the same, but the two differ
  when the backup and the live set diverge (open question Q9).
- `TargetListBackup` = 0x390bd4 (`BackupResults(name or "default")`).

Class sheet (row 191):
- SnS_ManaCost = 25 + 5 x L (L1 30, L5 50).
- SnS_Cooldown = 2000 ms (any level). The Lua comment says "2 sec", which agrees.
- SnS_MinDamage (174) = (25 + Damage_Min_Main_Hand) x TP1. SnS_MaxDamage (175) = (40 + Damage_Max_Main_Hand) x TP1.
  TP1 (190) = (230 + 25 x L) / 256: L1 0.996, L5 1.387. The weapon terms are from the caster's main hand. With no weapon,
  L1 gives min 24.9 and max 39.8.
- SnS_Push_Target_Chance (183) = 210 + 5 x L (L1 215, L5 235). SnS_Stun_Target_Chance (184) = 0. No stun duration.
- LIKELY: a chance above 100 means every hit pushes (open question Q12). Shield's stored chance is used as a percent
  (`GetRand(0,100) < chance`, section 1.5), so the same scale is assumed.

Animation, sound and FX resolution:
- Anim: `3d/characters/prince/animations/skill_dh2_prince_warrior_paladin_divine_judgement.bdae`. Present, staged.
- Sound: step cue 559 (uid 107) -> `sfx_skill_warrior_pld_condemnation.wav`. Present, staged.
- Interface FX `skill_dh2_prince_warrior_paladin_divine_judgement.bdae` (AnimTable FX set 174): present, staged.
- OnPre per-target FX `spell_dh2_prince_warrior_paladin_divine_judgement.bdae`: original-only, MISSING from staged.

Port state:
- CAST: admitted (knight row `runtime_skill_use_profile_knight_v1.hpp:71-76`: pre_range 600, `sets_cooldown = true`,
  `rolls_combat` default true). Cooldown from the projected SnS_Cooldown (coordinator 765-767, 841, 929).
- Empty OnPre: the port does not return early (coordinator ~886-929), so MP and cooldown are spent. This matches the Lua.
  Test branch in phase 2.
- OnPre per-target FX: MISSING.
- DAMAGE: the Use loop calls the rolls (coordinator 1323). PARTIAL, unverified.
- EFFECT (push tail): MISSING (knight header gap). The push chance is in the sheet but the port's combat tail is not
  confirmed.
- Use targeting: re-query, not the backup (see IDA note above). PARTIAL.

### 1.5 Shield / GuardianShield (passive, SkillTable id 100, script prince_paladin_shield, class Skill_Paladin_GuardianShield)

Logic (Lua, PROVEN). Event: the attacker's hit on the caster (`StealAIEvent("OnTargetHit")`), when `skill_level > 0`,
the defender is the caster, and `attacker:TargetInMeleeRange()`.
- Two independent rolls: `GetRand(0,100)` against KNOCKBACK (SnS_Push_Target_Chance, 183) and against STUN
  (SnS_Stun_Target_Chance, 184).
- Push roll passes: start a 500 ms repeating timer `check(attacker)`. Each tick calls `attacker:SetKnockBackState(false)`;
  if the attacker's state is not Attack it stops the timer and calls `attacker:Stop()`; if the state is KnockedBack it
  stops the timer.
- Stun roll passes: start a 500 ms repeating timer `check2(attacker)`. Each tick calls `attacker:SetStunState(3000, true)`;
  if the state is not Attack it stops the timer and calls `Stop()`; if the state is Stunned it stops the timer.

Consequences (PROVEN from the Lua and the native handler):
- `SetKnockBackState(false)` clears the flag (IDA 0x3b7680: a false or missing argument clears, `true` sets). The push
  branch never sets knockback. As shipped it does not push the attacker. Whether the engine pushes on its own is open
  question Q4.
- The stun is 3000 ms, applied on the first tick (about 500 ms after the proc). The timer stops once the attacker is
  Stunned (LIKELY: first tick applies it).

Class sheet (row 197): SnS_Push_Target_Chance = 15 + L (L1 16, L5 20). SnS_Stun_Target_Chance = 10 + L (L1 11, L5 15).
SnS_Stun_Duration (185) = 3000, matching the Lua constant. ManaCost is 9 + L, unused (passive).

Resolution: sound `sfx_passive_skill_activate_1` is not used by Shield. No anim, no FX.

Port state: text only (`runtime_skills_text_v1.cpp:98`). MISSING: player OnTargetHit passive hook (the only OnTargetHit
dispatch is the monster-side one, `character_combat_results_ai_v1.cpp:11`), stun state (no stun state found in the port
outside bindings and comments), knockback state.

### 1.6 EnergyBalance (passive, SkillTable id 39, script prince_paladin_energy_balance, classes per element 192-196)

Logic (Lua, PROVEN):
- `GetData()` picks the buff row from `GetEquippedFaeryElement()` (native 0x3b6dc4, returns the equipped faery's element
  index). Element not in the table, including no faery, uses Lightning.
- OnSkillUpdate (on rank change): remove the old buff and timer; if rank != 0, `CreateBuff(data.BUFF_ID, 3000, false, rank)`,
  `ApplyBuff`, and `StartSkillTimerCB(3000, repeat, updateSelfBuff)`. The repeating timer re-creates the buff every 3 s,
  choosing the element at that moment.
- OnSkillInfo depends on the live faery element. The text projection explicitly leaves it out (comment,
  `runtime_skills_text_v1.cpp:36-43`).

Class sheet (rows 192-196, per element):
- Damage_Elemental_Min/Max_Main_Hand (95, 96) and Off_Hand (98, 99) each += 2 x L (L1 +2, L5 +10).
- Damage_Elemental_Type (97, 100) = element id: Fire 0, Water 1, Lightning 2, Earth 3, Air 4 (LIKELY, from the stored
  values; the enum is the Elemental CST and was not checked directly).

Resolution: no anim, no sound, no FX in the Lua. The faery element FX are a separate system.

Port state: MISSING. No passive path and no element query binding in the skill layer (the faery system exists, faery2
report). Element-change latency: a switch takes effect at the next 3 s refresh, unless OnSkillUpdate fires on faery
change (unknown; open question Q10).

### 1.7 Conversion (passive, SkillTable id 18, script prince_paladin_conversion, class Skill_Paladin_Conversion)

Logic (Lua, PROVEN). Event: the caster's hit on a monster (`StealAIEvent("OnTargetHit")`), `skill_level > 0`, attacker is
the caster, defender is neither boss nor miniboss.
1. If the defender's name is in the 6-slot converted memory: `defender:RemoveBuff(BUFF_ID, its buff)`, clear the slot.
   `skip = true`. Hitting a converted monster breaks the conversion and does not re-convert it.
2. `rnd = GetRand(0,100)`. If `rnd < BUFF_CHANCE` (= TempProp1 at 190), not dead, and not skipped:
   `defender:CreateBuff(FactionBuff3, BUFF_DELAY, false, rank, spell_dh2_prince_warrior_paladin_conversion_persist)`,
   `defender:ApplyBuff`, `PlaySound("sfx_passive_skill_activate_1")`, `Stop()` on the caster and `defender:Stop()`.
   If no 2-second targetability timer is running, `defender:SetIsTargetable(false)` and a 2000 ms timer restores it.
   The slot stores the defender's name.

Class sheet (row 190):
- SnS_ManaCost = 9 + L (L1 10, L5 14). Unused (passive).
- TempProp1 (190) = 2 + 0.25 x L: conversion chance in percent, L1 2.25, L5 3.25.
- SnS_Duration (176) = 5000 + 5000 x L: L1 10000 ms, L5 30000 ms. This is the buff duration passed to CreateBuff.
- Stun cells (184, 185) are set but not used by this script.

Faction buff (ClassTable row 72 `FactionBuff3`, atlas): `DestProp 0 (AIFaction) := 0` via formula type 9 (PROVEN from the
row). The conversion is a buff that sets AIFaction to 0. The Illusionist `FactionBuff` (row 70) sets AIFaction to 8. This
is a shared need (section 2, N2).

Contradictions and quirks (PROVEN from the code):
- The Lua header says "The conversion is permanent". The code gives a timed buff of 10-30 s (LIKELY: the code wins;
  open question Q2).
- `table.insert(memory_buff, buff)` inserts a nil (the file-level `buff` is never assigned). The slot is kept by the
  indexed assignment, so the effect is harmless.
- Slots are keyed by monster name. Two monsters with the same name collide.
- A single shared `buff_timer` means only one 2-second targetability window runs at a time.
- Boss and miniboss are excluded, as the header says.

Resolution: sound `sfx_passive_skill_activate_1.wav` present, staged. FX `spell_dh2_prince_warrior_paladin_conversion_persist.bdae`
present in original, MISSING from staged.

Port state: MISSING. No faction buff use anywhere in the port (`FactionBuff` has no hits outside the bindings). No player
OnTargetHit path. `SetIsTargetable` is bound (`character_script_owner_bindings.inc:171`), and a targetability owner exists
(`character_targetability_owner_v1.cpp`), but it sets an interactive flag, not the script's targetable field. The relationship
code reads the resolved AIFaction word (`character_world_ai_relationship_v1.cpp:20`), so a buff-driven faction flip may
already be expressible. Unverified.

### 1.8 HolyWeapon (passive, SkillTable id 54, script prince_paladin_holy_weapon, class Skill_Paladin_HolyWeapon)

Logic (Lua, PROVEN). Event: any hit on the caster (`defender:GetID() == GetID()`), with `skill_level > 0`. No melee
check in the code.
- `attacker:SetProp(SnS_Level, rank)`, `attacker:ApplyPropClass(Skill_Paladin_HolyWeapon)`. This applies the row to the
  attacker's sheet (it sets only TempProp1 there).
- `hp = attacker:GetHP()` (HP, MaxHP, percent); `hp = max(hp - regenHP_amount, 0)`, where `regenHP_amount` is the
  caster's TempProp1 cached in OnSkillUpdate. `attacker:SetProp(HP, hp)`. If `hp == 0`, `attacker:Kill()`.

The Lua comment says "Damage dealt is received in a certain proportion by the attacker", but the code subtracts a flat
amount. The header's "2/hit, next: +2/hit" matches 2 x rank.

Class sheet (row 199): TempProp1 (190) = 2 x L (L1 2, L5 10). No other cells. No ManaCost.

Resolution: no anim, no FX, no sound.

Port state: MISSING. No passive hook, no attacker HP write, no kill path (`Kill` is PARTIAL in the atlas, native
0x3b7e40).

## 2. Engine needs (general systems, deduplicated for the lead)

Each item names the native handler (IDA, PROVEN) and the skills that need it. The owner's rule: general API, not
per-skill code.

- N1. Buff owner on any Character (monsters and allies), with FX and removal by handle.
  Native: CreateBuff 0x3b86a8, ApplyBuff (ApplyPropClass pointer form) 0x3babdc, RemoveBuff 0x3b842c.
  Skills: Conversion (monsters); auras (allies, none in single-player). Port: `RuntimeTimedBuffOwnerV1` takes an ActorId,
  so monsters may work. Unverified.
- N2. Faction through the buff sheet: AIFaction (prop 0) set by a buff, read by monster relationship code.
  Skills: Conversion (row 72); shared with Illusionist Terror (row 70). Port: relationship reads resolved word 0
  (`character_world_ai_relationship_v1.cpp:20`). Retargeting after a faction change: unverified.
- N3. Monster stun state with duration: `SetStunState(ms, bool)`, native 0x3b9580 (CharStateMachine).
  Skills: Shield (3000 ms). Shared with Warcry, Berserk and the Mage stuns. Port: no stun state found outside bindings.
- N4. Knockback state and the hit push tail: `SetKnockBackState(bool)` native 0x3b7680 (SM_SetKnockBackState); the push
  chance comes from the hit outcome in `F_ApplyResult` (0x3b10b4, per port comment). Skills: Shield (clear only), DivineJudgement
  (push 215-235). Port: the push tail is missing (knight header). Shared with Berserker WhirlingDrift.
- N5. Player passive event delivery: OnTargetHit with (attacker, defender), with the TargetInMeleeRange predicate (native
  0x3b7020), for hits the caster makes (Conversion) and hits the caster receives (Shield, HolyWeapon). Port: only the
  monster-side dispatch (`character_combat_results_ai_v1.cpp:11`). The original's registration name is StealAIEvent,
  which is not in the IDA export or the catalog. CharAI::RaiseAIEvent (0x3cbb34) is a candidate only (GUESS).
- N6. Skill timers with callbacks: one-shot and repeating, with cancel. Native candidates StartTimer 0x3b7590 and StopTimer
  0x3b7064 (catalog names; the Lua-side names StartSkillTimerCB / StopTimerCB are not in the catalog, so the mapping is
  unverified). Skills: Shield (500 ms repeat), EnergyBalance (3000 ms repeat), Conversion (2000 ms), auras (12000 ms).
- N7. Monster targetability: `SetIsTargetable(bool)` native 0x3b7a64 (writes a byte at sheet offset +1045). Skills:
  Conversion (2 s). Port: a targetability owner exists for the interactive flag only. Partial.
- N8. Stop: `Character::_Stop` native 0x3b56b4. Skills: Shield, Conversion. Atlas: PARTIAL.
- N9. Live HP write and kill: `SetProp(HP, value)` (two arguments, PROPS_Set, 0x3b7eac), `GetPropHP` 0x3b6cd8,
  `Kill` 0x3b7e40. Skills: PurifyingAura heal (write), HolyWeapon (write and kill). Atlas: Kill PARTIAL.
- N10. Equipped faery element query: `GetEquippedFaeryElement` native 0x3b6dc4 (returns the current faery's element at
  sheet offset +8). Skills: EnergyBalance. Port: faery system exists (`faery2-report.md`); no skill binding.
- N11. Named target backup and string-argument search: `TargetListBackup(name)` 0x390bd4 and `TargetListSearch(range, angle, name)`
  (string branch in 0x38fcf0, filters the backup). Skills: DivineJudgement Use. Shared with Charge and ColdRay.
  Port: re-query instead; atlas status MISSING for TargetListBackup.
- N12. Ally query with the Player character filter and self exclusion. Skills: OffensiveAura, GuardingAura, PurifyingAura.
  Native: `SetTargetListCharacterFilter` 0x390690 (atlas MISSING in the port). Port: no Player filter in
  `runtime_skill_target_query_v1`.
- N13. Live buff strength update on rank change (Lua `SetBuffProp(SnS_Level)` in OnSkillUpdate). Skills: the three auras.
- N14. Presentation hooks: PlayFX on the caster at Use, PlayFX per target at OnPre (DivineJudgement) and per ally
  (auras). Native `GameObject::_PlayFX` 0x3923cc. Port: profile has no hook; `target_hit_fx_name` is for damage hits only.
- N15. Passive buff refresh: EnergyBalance rebuilds its buff every 3 s from the current faery element (N6 + N10).
- N16. Attacker-sheet apply: HolyWeapon applies its class row to the attacker's sheet (`ApplyPropClass` on a monster).
  Uses N1 with a monster target.

## 3. Asset and staging gaps (for the package owner)

Staged selection `asset-selection-v21.8.json` includes the four Paladin active scripts (group `player_skill_lua_active35`)
and one Paladin FX (`skill_dh2_prince_warrior_paladin_divine_judgement.bdae`, group `skill_fx_missing23`). Missing from the
staged package (original present, PROVEN by file search):
- `3d/interface/spell_dh2_prince_warrior_paladin_divine_judgement.bdae`
- `3d/interface/skill_dh2_prince_warrior_paladin_{offensive,guarding,purifying}_aura.bdae` (BUFF_FX)
- `3d/interface/skill_dh2_prince_warrior_paladin_{offensive,guarding,purifying}_aura_persist.bdae` (BUFF_PERSIST)
- `3d/interface/spell_dh2_prince_warrior_paladin_{offensive,guarding,purifying}_aura.bdae` (BUFF_FX2 ally FX)
- `3d/interface/spell_dh2_prince_warrior_paladin_conversion_persist.bdae` (Conversion FX)
Passive scripts not staged (`assets/data/scripts/skills/` has only the four active Paladin scripts):
`prince_paladin_{conversion,energy_balance,holy_weapon,shield}.luac`. Needed only if the passives are scripted.
Recommended addition to the selection: the 11 interface files above and the 4 passive scripts. Owner: lead / package owner.

## 4. Open questions

- Q1. Formula type 6. The port evaluator computes `dst = ((dst x sheet[p1]) >> 8) + p1`, adding the raw parameter (190 here)
  (`port/game-data/class_tables.cpp:24`). This affects OffensiveAura (+0.74 on each damage bonus, a constant) and
  GuardingAura (+0.74 armor). Needed: IDA of the class formula handler (not located by name in the export) and a check of
  the type-6 branch.
- Q2. Conversion duration: the header says permanent, the code uses 10-30 s (SnS_Duration). Needed: `PROPS_AddBuff`
  duration semantics in IDA (the port comment says duration 0 = permanent) and an original-gameplay video.
- Q3. Aura gameplay: header says "until stopped or MP too low"; the Lua has no off path and no MP drain; the guarding header
  says 5 s, the code 12 s. Needed: original gameplay video of a cast, showing duration and whether re-cast toggles.
  AGENTS.md requires visual evidence before implementation. No Paladin gameplay notes exist in `coordination/` or `docs/`
  (searched).
- Q4. Shield push: does the engine set KnockBack from a hit's push result? If yes, Shield's clear is the only Lua part.
  Needed: IDA callers of `SM_SetKnockBackState(true)`.
- Q5. HolyWeapon: "proportion" (header) versus flat `TempProp1` (code). Needed: original video of a hit on a Paladin.
- Q6. PurifyingAura cooldown 10000 ms in the class sheet with no `SetSkillCooldown` in the Lua. Needed: who reads
  SnS_Cooldown (177) besides `SetSkillCooldown`. The one-argument `SetSkillCooldown` has no catalog entry.
  `SetSkillCooldownTimerId__` (0x3b97e0) needs two arguments (slot, value), so it is not the one-argument call (PROVEN).
- Q7. `SkillCombatRoll(target)` (one argument, used by about 70 scripts) has no catalog entry. The two-argument
  `SkillCombatRoll__` (0x3b9fbc, slot + target) does F_SkillAttack (0x3b31a4) and F_ApplyResult (0x3b10b4 per port comment),
  and checks a weapon flag (`& 0x800000`). The one-argument registration is not found (needed for phase 2).
- Q8. Catalog aliases (IDA-checked): `ApplyBuff` = `ApplyPropClass` (0x3babdc, pointer form, PROVEN); `SetBuffProp` =
  `SetProp` (0x3b7eac, two-argument form is live, PROVEN); `GetHP` = `GetPropHP` (PROVEN). The names are misleading.
- Q9. DivineJudgement Use: the string-argument branch uses the OnPre backup (PROVEN), the port re-queries the world.
  Equivalence in single-player is LIKELY, not shown.
- Q10. EnergyBalance: element enum values (0-4) and the default when no faery is equipped. Also: does `OnSkillUpdate` fire
  on a faery change. Needed: `GetEquippedFaeryElement` return values for "none" and the caller of OnSkillUpdate.
- Q11. EnergyBalance is absent from the text projection by design (comment, `runtime_skills_text_v1.cpp:36-43`). Tooltip is
  therefore unavailable. Confirm this is wanted.
- Q12. DivineJudgement push chance 215-235 (raw/256 scale). Whether a value above 100 means always pushes. Needed: IDA
  compare in F_ApplyResult.
- Q13. Lua `SetSkillCooldown` / `StartSkillTimerCB` / `StopTimerCB` / `StealAIEvent` / `CalcManaCost` / `SetTempProps` /
  `IsBoss` (global) have no catalog entry. IDA has `IsBoss` and `IsMiniBoss` as C++ members (0x3a3158, 0x3a3144). The rest
  are not located by name. Needed for phase 2 (the mapping is what makes the binding general).

## 5. Atlas versus IDA

Agreements (PROVEN):
- ApplyBuff / ApplyPropClass: same native 0x3babdc, pointer-argument form is the buff path.
- GetHP / GetPropHP: same native 0x3b6cd8; returns (HP, MaxHP, percent).
- SetBuffProp / SetProp: same native 0x3b7eac. Two-argument form is a live write.

Disagreements or gaps:
- Atlas says `TargetListBackup` is MISSING in the port. IDA has a native (0x390bd4) and the Lua uses it in DivineJudgement.
- Atlas lists `SetTargetListCharacterFilter` and `SetTargetListObjectFilter` as MISSING in the port. The Paladin
  auras need the Player filter (N12).
- The catalog has no entry for `SetSkillCooldown`, `CalcManaCost`, `SetTempProps`, `StealAIEvent`, `GetCurrentSkillInfo`
  (catalog name `GetCurrentSkillInfo__`), or the one-argument `SkillCombatRoll` (Q6, Q7, Q13).
- The catalog's `SetSkillCooldownTimerId__` and `SkillCombatRoll__` need more arguments than the Lua calls (Q6, Q7).
- Atlas engine statuses describe the Lua VM bindings. The player skill path uses C++ profile rows, so MISSING there does not
  predict whether a skill runs.

## 6. Phase 2 verification plan (outcome branches, for the Paladin skills once the engine exists)

- DivineJudgement: (a) one enemy in 600: per-target FX, roll, push; (b) no enemy: MP and 2 s cooldown spent, no roll;
  (c) cooldown active: no cast; (d) two enemies: both rolled; (e) enemy outside 600: not rolled.
- Auras (each): (a) cast: self buff, 12 s, FX and sound; (b) re-cast while active: buff refreshed, MP spent; (c) rank change
  while active: strength follows (Lua behaviour); (d) expiry at 12 s; (e) no ally: self buff only (single-player).
- PurifyingAura: (a) HP low: HP + 15 x rank; (b) HP at max: capped; (c) no cooldown (Lua).
- Passives (after engine): Shield proc (melee only), stun 3000 ms after about 500 ms; HolyWeapon 2 x rank per hit and kill at 0;
  Conversion gating (boss excluded, 2-3% chance), re-hit breaks conversion, 2 s untargetable window; EnergyBalance element
  swap within 3 s.

## 7. Summary of phase 1 status

- Investigation complete for all 8 Paladin-owned skills. No engine code written. No real run.
- The Paladin actives are admitted in the port but unverified. PurifyingAura's heal is missing and is a live HP write
  (not a buff write). DivineJudgement's OnPre FX and push tail are missing. Auras have no ally path and no FX in the staged
  package.
- All four passives have no gameplay implementation (text only; EnergyBalance deliberately left out of text).
- The largest general gaps are the player passive event path (N5), buff-driven faction (N2), the stun / knockback state
  (N3, N4), and the ally and named-backup target queries (N11, N12).
