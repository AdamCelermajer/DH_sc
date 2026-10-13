# v21.8 sub-assassin: Phase 1 investigation (Assassin tree, Rogue family)

Agent: assassin. Branch `v21.8-sub-assassin`, worktree `.claude/worktrees/sub-assassin`, base `windows-foundation` @ 7b9028cb.
Scope: every Assassin skill (8 owned: 4 active, 4 passive; plus the 8 Rogue-base skills the Assassin tree shares).
Phase 1 rules: investigation only. No engine code, no build, no CTest run. Only this file is changed.

Evidence key: PROVEN (read directly in source/IDA/data), LIKELY (strong indirect evidence), GUESS (unverified). Inferences are marked INFERENCE.
Paths: repo-relative to `port/windows-foundation/` unless stated. `.local-inputs/` is in the primary checkout `C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\`.

Status legend (per the brief): CAST / ANIM / FX / SOUND / DAMAGE / EFFECT = WORKS / PARTIAL / MISSING / UNVERIFIED (asset or code present, no run evidence).

---

## 0. Summary

- The 16-skill Assassin tree (`SkillTree 29`, RogueAssassin list `[56,114,90,97,2,1,16,46,50,9,85,5,4,113,17,126]`, `class_tables`/`skills_pyarray` via atlas `mobs/327.md`):
  - Owned (this agent): Assassinate (P, 12), Assassination (A, 21), Blindness (A, 15), Contaminate (P, 18), GrapplingHook (A, 12), PoisonSting (A, 18), TreasureHunter (P, 15), blade_defect (P, 21).
  - Shared from Rogue base (not ours unless broken): JumpKick, Quickness, Roundhouse, ViciousStrike (A); Acrobat, Ambidextrous, ComboMaster, FindWeakness (P). Listed in section 5.
- Verdicts: Blindness and PoisonSting are PARTIAL (castable through profile rows, never run). Assassination, GrapplingHook, Contaminate, TreasureHunter, Assassinate and blade_defect are not admitted.
- The most important engine gaps are shared with other classes: a passive skill owner, a split of "can be targeted" from "can be attacked", an AI hit-event subscription, the monster stun/HeadTo state, the boss/mini-boss flags and DOT ticks.

### Corrections to `coordination/v21.8/skills-rogue-report.md` (verified here)

1. **Assassination "no kill primitive" is wrong.** A kill primitive exists: `CombatSession::script_kill_actor` (`combat_session.cpp:2291-2294`) -> `ActorCombatRuntime::kill` (`actor_combat_runtime.cpp:310-319`), used by the campaign `Script_KillActor` (`features/campaign_host/campaign_host.cpp:593-595`). What is missing is the killer attribution, the boss/mini-boss gate, and the skill wiring.
2. **GrapplingHook "BLOCKED-PROJECTILE" is stale.** The shared skill-projectile launch API exists (`features/effects/runtime_session_projectile_v1.hpp:126-137`, impl `runtime_session_projectile_v1.cpp:359`) with the `check`/`delayed` callbacks that mirror RegisterSkillEx, and the mage external owner already launches player skill projectiles (`features/generic_skills/runtime_mage_skill_owner_v1.cpp:220, 382`). The grapple is blocked by stun, pull and boss flags (E6, E8, E9), not by the projectile.
3. **GrapplingHook sound "not linked" is resolved.** The Lua calls `PlaySound("sfx_skill_rogue_ass_throwing_knife")`. `sounds.xml` has that exact label at uid 139, file `sfx_skill_rogue_ass_throw_knife.wav`, and the file is present in `.local-inputs/windows-source-clock-v21.8-staged/audio-assets/data/sounds/` (PROVEN).
4. **Blindness and PoisonSting "written, never run"** is confirmed. Their rows are on base (`runtime_skill_use_profile_rogue_v1.hpp:44-55`) and no ctest or build log under `C:\dh218` contains a `PASS Rogue Blindness` / `PASS Rogue PoisonSting` line. Not re-run in Phase 1.
5. **The skills-rogue report omits Special_Sneak.** Blindness's class row also writes `Special_Sneak` (198), and the target search gates on it (`features/generic_skills/runtime_skill_target_query_v1.cpp:172-176`). Part of the blinding may already run through that gate. See E15 and open question Q6.
6. **Assassinate's header comment disagrees with its code** (level gate, section 1.1). Contaminate's header is a copy of Assassinate's and describes the wrong skill (section 1.4).

### Corrections to the matrices

- `SKILL-VERIFICATION-MATRIX.json`: GrapplingHook `sound_cues` should link uid 139 (label `sfx_skill_rogue_ass_throwing_knife`, file `sfx_skill_rogue_ass_throw_knife.wav`).
- The matrix's "sound_marker []" for Assassination, Blindness and PoisonSting is wrong for the sound: the AnimationSequence rows carry the cue (section 2). It also says GrapplingHook has "no FX set"; that is right (FX -1), but the sequence's sound is also -1, so the Lua `PlaySound` is the only sound for the grapple.
- `CHARACTER-COVERAGE-MATRIX.md` rows 67-70 say "no cast" for the four Assassin actives. Blindness and PoisonSting now have profile rows (CAST PARTIAL).

---

## 1. Original logic (decoded Lua, plain text)

Method: the `.luac` files are plain Lua text (CRLF), not bytecode. Read them directly from `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_*.luac`. `.local-inputs/lua_read_v13.py` is NOT a decoder: it prints blocks from `port/level-world/reference/lua-script-v13/original.asm` whose header matches an argument (PROVEN). Tell the team, so nobody else loses time on it.

Class-table reading (ClassTable rows, atlas `coordination/atlas/general/mobs/327.md`, `character_classes#group0[142..150, 147, 148, 145]`). Formula semantics are the IDA-verified dispatch (IDA `CharProperties::_LoadClass` 0x3e2e20, cases 0-9; `_LinearWithPropPlusBaseValue` 0x3e2d4c for type 1). A type-1 row is `dest = (P1 or dest if P1 == -666) + P3 * (src >> 8)` where `src = P2` (172 = SnS_Level, 19 = Level). Values are x256 fixed point: `Param1 2048000 / 256 = 8000`. Property indices are from `character_properties_pystructnames.bin` (224 names, parsed in Phase 1; e.g. 19 Level, 172 SnS_Level, 173 SnS_ManaCost, 176 SnS_Duration, 177 SnS_Cooldown, 190 TempProp1, 191 TempProp2, 198 Special_Sneak, 199 Special_Sneak_Detection, 125 Dot_Duration, 145 Slow_Target_Chance, 146 Slow_Duration, 150 Stat_Dexterity, 195 Special_Loot_Gold_Multiplier, 92 Damage_Bonus_Stunned, 93 Damage_Bonus_Sneaking, 94 Damage_Bonus_Combo, 59 Rating_Defense, 63 Rating_Critical, 60 Rating_Dodge, 46-48 Speed_Modifier_Walk/Rotation/Attack).

`L` below = skill rank (SnS_Level). Values are shown in display units after /256.

### 1.1 Assassinate (passive, lvl 12, SkillTable id 4, script `prince_assassin_assassinate`)
- Lua: `OnSkillUpdate_`: on rank change, `RemoveBuff(CLASS)`, then `CreateBuff(CLASS, 0, false, slvl)` + `ApplyBuff` when slvl != 0. Check returns `(false, true)`. Pre/Skill/Post empty. No OnDelayed. Header: "Increase damage when in sneak mode or attacking a stunned opponent".
- Class `Skill_Assassin_Assassinate` (row 142): `92 Damage_Bonus_Stunned += 3*L`, `93 Damage_Bonus_Sneaking += 3*L` (both type 1 with P1 = -666).
- Passive: no anim, FX or sound.

### 1.2 Assassination (active, lvl 21, id 5, script `prince_assassin_assassination`, Anim 517)
- Lua constants: `RANGE 100`, `ANGLE 180`, `CLASS = Skill_Assassin_Assassination`, `BOSS_CLASS = Skill_Assassin_Assassination_boss`.
- `OnSkillInfo_` returns `cooldown`. `OnSkillUpdate_`: mana = CalcManaCost(CLASS, slvl); `cooldown = FromFixed(GetProp(SnS_Cooldown, true))`, clamped at 0.
- `OnSkillCheck_`: `not HasSkillCooldown() and HasMana(mana_cost)`.
- `OnPreSkill_`: filter Enemy; object filter AttackableOnly; ClosestFirst; `TargetListSearch(100)`; `TargetListBackup("MaxAngleSearch")`; if non-empty `target = top; LookAt(target)`; `UseMana`; `SetSkillCooldown(cooldown)`.
- `OnSkill_` (only if a target was chosen): `TargetListSearch(100, 180, "MaxAngleSearch")`, `local target,_,_, character = GetTargetListTop()`. Only characters are handled (objects do nothing).
  - `level` = GetProp(player Level), `en_level` = target Level (both raw x256).
  - if `en_level < level + 2` (raw comparison):
    - not `IsBoss(target)`: `target:Kill(GetSelf())` and `PlaySound("sfx_passive_skill_activate_4")`.
    - boss: `SetProp(SnS_Level)`, `ApplyPropClass(BOSS_CLASS)`, `SkillCombatRoll(target)`.
  - else:
    - not boss: `SetProp(SnS_Level)`, `ApplyPropClass(CLASS)`, `SkillCombatRoll(target)`.
    - boss: BOSS_CLASS + `SkillCombatRoll`.
- `OnPostSkill_`: `ClearTarget()`.
- The kill branch never calls `SkillCombatRoll`, so no damage and no hit receipt.

Findings:
- **Gate units (E7).** Both sides are raw x256 values (Level 256 = level 1 in `mobs/327.md`; formulas use asr8 on prop 19). So `en_level < level + 2` compares against `level + 2/256`, which is effectively `en_level <= level`. The header ("level = player's level +2", "BaseMonsters or lower") does not match the code. The code is authoritative. INFERENCE (LIKELY): the port must implement the raw comparison.
- `IsMiniBoss` is not checked, so a mini-boss at or below the player's level is killed outright. INFERENCE (PROVEN from the code, the design intent is unknown).
- Class `Skill_Assassin_Assassination` (row 143): `173 mana = 14 + 4L`; `174 min = (10+L) * T1`; `175 max = (20+L) * T1` (both Formula 5 scaled by `190`); `190 TempProp1 raw = 230 + 25L` (`T1 = (230+25L)/256`); `177 cooldown = 8350 - 300L ms` (clamped at 0 by Lua).
- Boss class `Skill_Assassin_Assassination_boss` (row 144): same mana/min/max/TempProp1/cooldown, plus `63 Rating_Critical += 20 + L`.

### 1.3 Blindness (active, lvl 15, id 9, script `prince_assassin_blindness`, Anim 518)
- Lua: `BUFF_ID = Skill_Assassin_Blindness`, `BUFF_FX = spell_dh2_assassin_blindness_persist`, `BUFF_TIME = FromFixed(GetProp(SnS_Duration, true))` (temp sheet).
- `OnPreSkill_`: if `buff`: `SetIsTargetable(true)`, `RemoveBuff`. Then `UseMana`. No Pre query, no cooldown, no Post.
- `OnSkill_`: `SetIsTargetable(false)`; `buff = CreateBuff(BUFF_ID, BUFF_TIME, false, slvl, BUFF_FX)`; `ApplyBuff`; `buff_timer = StartSkillTimerCB(BUFF_TIME, false, BuffExpired)`. `BuffExpired`: `SetIsTargetable(true)`, `buff = nil`.
- `OnTargetHit_Skill(attacker, defender)`: if slvl > 0 and `attacker:GetID() == GetID()`: `RemoveBuff`, `SetIsTargetable(true)`, `buff = nil`. So any hit the caster lands ends the buff.
- `RANGE` is read from TempProp1 and never used by the Lua. Dead value. PROVEN.
- Class `Skill_Assassin_Blindness` (row 146): `173 mana = 16 + 4L`; `176 SnS_Duration = 8000 + 2000L ms`; `198 Special_Sneak += 100*L` and `198 Special_Sneak += 100*Level` (two rows, P1 = -666, so both accumulate); `190 TempProp1 = 300 + 20L`.
- Anim/FX/Sound (AnimationSequence 518): Anim 1225, FX 143, Sound 548 (uid 140 `sfx_skill_rogue_ass_concealing_cloud`, file `sfx_skill_rogue_ass_concealing_cloud.wav`, staged).
- BUFF_FX `spell_dh2_assassin_blindness_persist` = effect_sets[212] (atlas, present).

### 1.4 Contaminate (passive, lvl 18, id 17, script `prince_assassin_contaminate`)
- Lua header: "Contaminate -> OutBreak (new name: numbing stud). Increase damage when in sneak mode..." The header text is copy-pasted from Assassinate. The code is a slow-on-hit proc (PROVEN).
- Constants: `BUFF_ID = Debuff_Slow` (class row 60: `Speed_Modifier_Walk/Rotation/Attack = -50` each), `BUFF_PERSIST = AUTO_DEBUFF_SLOW` (effect_sets[1]).
- `OnTargetHit_Skill(attacker, defender)`: if slvl > 0 and `defender:GetID() == GetID()` and `attacker:TargetInMeleeRange()`:
  - `proc_chance = GetRand(0,100)`; `enemy_proc = GetRand(0, attacker Slow_Resist_Chance)` is computed and traced but never used in the decision (PROVEN).
  - if `proc_chance < SLOW`: `attacker:CreateBuff(Debuff_Slow, duration, false, slvl, AUTO_DEBUFF_SLOW)` + `attacker:ApplyBuff`.
- `OnSkillUpdate_`: `SLOW = FromFixed(TempProp1)`, `duration = FromFixed(TempProp2)`.
- Class `Skill_Assassin_Contaminate` (row 147): `190 TempProp1 = 8 + 2L` (percent chance); `191 TempProp2 = 1000 + 500L ms`. Its `174/175` rows add 0.
- Note for fidelity (not a bug to fix): the Slow_Resist_Chance roll has no effect in the authored script.

### 1.5 TreasureHunter (passive, lvl 15, id 113, script `prince_assassin_treasure_hunter`)
- Lua: same passive pattern as Assassinate (`CreateBuff(CLASS, 0, false, slvl)` + `ApplyBuff` on rank change). Header is copy-pasted from Assassinate.
- Class `Skill_Assassin_TreasureHunter` (row 150): `195 Special_Loot_Gold_Multiplier = 5*L` (DisplayProps [195]).

### 1.6 blade_defect (passive, lvl 21, id 126; SkillTable name is the literal `blade_defect`, script `prince_assassin_blade_deflect`)
- Lua: same passive pattern (CLASS = Skill_Assassin_BladeDeflect). Header: "use his blades to deflect incoming attacks... defensive edge without a shield".
- Class (row 145): `190 TempProp1 raw = 12 + 3L`; `59 Rating_Defense` = Formula 6 with P1 = 190 (recovered semantics: `dest = (sheet[190] * dest) >> 8`, IDA `CharProperties::_ScaleWithPercentageForBuff` 0x3e2c10).
- Port type-6 discrepancy (E13): `port/game-data/class_tables.cpp:24` computes `(...) + a` (adds the raw P1 = 190). IDA `_ScaleWithPercentageForBuff` has no added term. Under the IDA semantics, Rating_Defense is multiplied by roughly `(12+3L)/256`, which reads as a large reduction and is implausible for a defensive passive. Under the port semantics it gets +190/256. Neither reading is verified. GUESS until measured on the character panel (Q3).

### 1.7 GrapplingHook (active, lvl 12, id 50, script `prince_assassin_grapling_hook`, Anim 519)
- Lua constants: `RANGE 1000`, `ANGLE 220`, `CLASS = Skill_Assassin_GrapplingHook`, `PROJECTILE_ID = ProjectileTable GrapplingHook`. `traces = true` (debug only).
- `OnSkillUpdate_`: mana; `duration = FromFixed(GetProp(SnS_Duration, true))`.
- `OnPreSkill_`: filter Enemy; object filter NONE; FrontalFirst; `TargetListSearch(1000, 220)`; `LookAt(top)` if any; `UseMana`.
- `OnSkill_`: `PlaySound("sfx_skill_rogue_ass_throwing_knife")`; `SpawnSkillProjectile(PROJECTILE_ID)` (no explicit target: launches along the caster's forward).
- `OnPostSkill_`: `ClearTarget()`.
- `OnDelayedSkill_(target)` (RegisterSkillEx):
  - if target is a Character: `SetProp(SnS_Level)`, `ApplyPropClass(CLASS)`, `SkillCombatRoll(target)`.
    - if not boss and not mini-boss: `target:SetStunState(duration, true)`.
    - if `target:GetState() == Stunned` and not boss and not mini-boss: `target:HeadTo(GetPosition())` (pull toward the caster). Lua TODO: "need to speed up the pulling".
  - else (object): `SkillCombatRoll(target)` only.
  - target nil (expiry/miss): nothing.
- Class `Skill_Assassin_GrapplingHook` (row 148): `173 mana = 12 + 4L`; `174 min = (1 + 0.098L) * T1`; `175 max = (5 + 0.098L) * T1`; `190 TempProp1 = 230 + 25L`; `176 SnS_Duration = 2000 + 250L ms` (stun duration).
- ProjectileTable `GrapplingHook` (projectiles#group0[21], atlas): Model 27 (`projectile_returning_knives.bdae`), ImpactFX 80, DisappearOnHit 1, TargetLock 0, Dedicated 0, FriendlyFire 0, Timer 2000 ms, TimeBetweenHit 1000 ms. Float bit patterns (PROVEN decode): Velocity 0x41F00000 = 30.0, VelocityDamping 0x3E800000 = 0.25, MaxDistance 0xBF800000 = -1.0 (sentinel, meaning not proven). ImpactFX 80 indexes effect_sets[80] = `bloodsplat_hero` by index only (GUESS; atlas does not link it).
- Anim (AnimationSequence 519): one step, Anim 908 = `npc_assassin_throw_knife.bdae` (animations_dictionary 908, PROVEN), FX -1, Sound -1, MoveGO true, Speed 1.3.
- Prince hook clips `skill_dh2_prince_rogue_assassin_hook`, `_hook_pre`, `_hook_post` are in the dictionary (1226-1228) and staged, but no sequence references them (PROVEN). Orphans or used by a path not decoded (open, Q4).

### 1.8 PoisonSting (active, lvl 18, id 85, script `prince_assassin_poison_sting`, Anim 520)
- Lua: `BUFF_ID = Skill_Assassin_PoisonSting`, `BUFF_FX = skill_dh2_assassin_poison_sting_persist` (effect_sets[145]). `BUFF_TIME = FromFixed(GetProp(Dot_Duration, true))`, fallback 4000 when 0.
- `OnSkillUpdate_`: if `buff` is set, re-runs `OnSkill_` (refresh). `OnSkill_`: `CreateBuff(BUFF_ID, BUFF_TIME, false, slvl, BUFF_FX)` + `ApplyBuff` + `StartSkillTimerCB(BUFF_TIME, 0, BuffExpired)` (expiry only clears `buff`).
- `OnPreSkill_`: `RemoveBuff` if active; `UseMana`. `OnPostSkill_`: `ClearTarget()`. No Pre query, no cooldown.
- Class `Skill_Assassin_PoisonSting` (row 149): `173 mana = 15 + 5L`; `123 Dot_Min += 20 + 4L` and `+= 0.5 * Stat_Dexterity(150)`; `124 Dot_Max += 25 + 5L` and `+= 0.5 * Dex`; `125 Dot_Duration = 3000 + 750L ms`; `145 Slow_Target_Chance = 47 + 3L`; `146 Slow_Duration = 5000 ms`.
- Anim/FX/Sound (AnimationSequence 520): Anim 1229, FX 144, Sound 549 (uid 141 `sfx_skill_rogue_ass_plague_edge`, file staged).

---

## 2. Per-skill summary table (port state)

| Skill (lvl, type) | Anim seq -> anim / FX / sound | Port CAST | ANIM | FX | SOUND | DAMAGE | EFFECT | Phase 1 verdict |
|---|---|---|---|---|---|---|---|---|
| Assassinate (12, P) | none | n/a | n/a | n/a | n/a | MISSING (92/93 consumers not verified, Q5) | MISSING (no passive owner, E3) | Not admitted. Needs E3, E7 |
| Assassination (21, A) | 517 -> 1224 / 142 / 547 (uid142 assassinate.wav) + Lua 146 (`sfx_passive_skill_activate_4`) | MISSING (rejected "no complete generic same-session Use arm", coordinator:728) | UNVERIFIED (assets staged) | UNVERIFIED (set 142 staged) | UNVERIFIED (both cues resolve by label/uid) | MISSING (BOSS/normal class path needs IsBoss E6) | MISSING (kill: E5, E6, E7) | Not admitted. Feasible after E5, E6, E7 |
| Blindness (15, A) | 518 -> 1225 / 143 / 548 (uid140 concealing_cloud.wav) | PARTIAL (profile row `runtime_skill_use_profile_rogue_v1.hpp:44-49`, unit transcription only) | UNVERIFIED | UNVERIFIED (BUFF_FX effect_sets[212]) | UNVERIFIED | n/a | PARTIAL: buff applied (`coordinator:1243,1255`); SetIsTargetable not split (E1); timer restore missing; break-on-own-hit missing (E2) | Written, never run. Needs run and E1, E2 |
| PoisonSting (18, A) | 520 -> 1229 / 144 / 549 (uid141 plague_edge.wav) | PARTIAL (profile row `...rogue_v1.hpp:50-55`) | UNVERIFIED | UNVERIFIED (BUFF_FX effect_sets[145]) | UNVERIFIED | MISSING (DOT ticks E11) | PARTIAL: buff applied; DOT not applied (`combat_session.cpp:1607`); slow tail unverified (E12) | Written, never run. Needs run; E11 and E12 |
| GrapplingHook (12, A) | 519 -> 908 (npc clip) / none / none; Lua PlaySound uid139 | MISSING (no profile row) | UNVERIFIED (NPC clip staged) | none (projectile ImpactFX 80 unlinked) | RESOLVES (label match, file staged) | MISSING (delayed roll path exists generically) | MISSING (stun E8, pull E9, boss flags E6). Projectile is NOT the blocker (E10) | Not admitted. Needs E6, E8, E9, E10 (shared executor exists) |
| Contaminate (18, P) | n/a (passive) | n/a | n/a | n/a (BUFF_PERSIST effect_sets[1]) | n/a | n/a | MISSING (OnTargetHit listener E2; Debuff_Slow apply path exists E4) | Not admitted. Needs E2, E4 |
| TreasureHunter (15, P) | n/a | n/a | n/a | n/a | n/a | n/a | MISSING (E3); consumer of 195 unverified (Q5) | Not admitted. Needs E3 |
| blade_defect (21, P) | n/a | n/a | n/a | n/a | n/a | n/a | MISSING (E3); type-6 formula disputed (E13) | Not admitted. Needs E3, E13 |

Port-state references: profile header `features/generic_skills/runtime_skill_use_profile_rogue_v1.hpp`; registry `runtime_skill_use_profile_registry_v1.hpp:15-19`; coordinator buff arm `runtime_skill_cast_coordinator_v1.cpp:1242-1255`; timed buff owner `runtime_timed_buff_v1.hpp` (permanent: `:39-41`, impl `runtime_timed_buff_v1.cpp:181, 200`).

---

## 3. ENGINE NEEDS (general systems, deduplicated for the lead)

Each entry names the engine function or concept, the original handler (IDA address, `libDungeonHunter2.so/pseudocode-all.c`), who needs it, the port state, and the suggested owner area. No per-skill hacks.

**E1. Acquisition-only targetability (SetIsTargetable) with buff-coupled restore.**
- Original: `Character::_SetIsTargetable` (0x3b7a64) writes the flag read by `IsInteractive` (target selection), per the port's own notes (`features/bosses/runtime_swampking_core_ai_v1.hpp:16`, `playable_actor_world.hpp:89`). The IDA decompile types the object as ReturnValues, so the exact offset is LIKELY Character+1045 (INFERENCE from the port's note).
- Users: Blindness (A). Submerged Swamp King uses the same thing (`runtime_swampking_core_ai_v1.cpp:1322`).
- Port: `PlayableActorWorld::eligible_target` (`playable_actor_world.cpp:171-173`) = `!selection_suppressed && damage_eligible`, and `damage_eligible` requires `targetable` (`:175-183`). The same `eligible_target` gates continued attacks and damage (`combat_session.cpp:793, 828, 939, 948, 1669`; `runtime_ranged_skill_monster_v1.cpp:865`). Setting the player's suppression would also stop monsters from attacking. The Blindness header says blinded enemies "attack at close range but can't move or target MC". That is a design statement, not a verified behaviour.
- Need: separate predicates, acquire (selection) vs attack (continue/damage). Timer expiry must restore the flag (the buff owner does not call back).

**E2. Actor AI event subscription: `StealAIEvent("OnTargetHit", fn(attacker, defender))` for a player-owned script.**
- Original: the registration native is NOT in `libDungeonHunter2.so` binding tables (grep for StealAIEvent, StartSkillTimerCB, SetTempProps, RegisterSkill: no hits). The event dispatch is `CharAI::RaiseAIEvent` (0x3cbb34). GUESS for the exact registration path.
- Users: Blindness (break when the caster hits), Contaminate (incoming melee-range proc). Callbacks receive every hit, so both need the defender and attacker.
- Port: monster `target_hit` events exist (`features/enemy_ai/monster_decisions.cpp:66-67`), but there is no player-side subscription and no hook at the hit application (`apply_source_result`).

**E3. Generic passive-skill owner (rank-keyed permanent class buff).**
- Original pattern (all 8 passives and the shared Rogue passives): `OnSkillUpdate_`: on rank change, `RemoveBuff(CLASS)`, then `CreateBuff(CLASS, 0, false, rank)` + `ApplyBuff`.
- Port: `TimedBuffSpecV1.permanent` exists (`runtime_timed_buff_v1.hpp:39-41`, impl `runtime_timed_buff_v1.cpp:181, 200`). The only user is the faery passive, hard-coded in `main.cpp:9241-9261` (`faery_passive_buff_class_v1`). There is no generic passive owner that reads SkillTable ranks.
- Users: Assassinate, TreasureHunter, blade_defect (A); Acrobat, Ambidextrous, ComboMaster, FindWeakness (shared, section 5).

**E4. Apply a timed buff to another actor (enemy-side Debuff_Slow).**
- Port: `apply_timed_buff(session, ActorId, ...)` takes any actor (`runtime_timed_buff_v1.hpp:53, 112`). Weapon slow already applies to `event.target` (`main.cpp:9203-9207`).
- Status: LIKELY PORTED. Needs a unit test for a non-player target and FX (AUTO_DEBUFF_SLOW).
- Users: Contaminate (A-passive proc).

**E5. Kill with killer attribution (Character::Kill).**
- Original: `Character::_Kill` (0x3b7e40) gets `killer = args[0]` (GameObject userdata) and calls `v2Controller::Cmd_Kill(controller, killer, 0)`. `Character::Kill(GameObject*, bool)` is at 0x3a5b18; it runs trophy and event-manager code (`TrophyManager`, `EventManager`), so the side effects are wider than death. Only partly read in Phase 1 (Q2).
- Port: `CombatSession::script_kill_actor(id)` has no killer argument and no trophy/event side effects; `ActorCombatRuntime::kill` only calls `kill_actor_state` and `synchronize`.
- Users: Assassination (A).

**E6. Boss / mini-boss flags.**
- Original: `Character::IsMiniBoss` (0x3a3144) = `(CharAI+20 >> 1) & 1`; `Character::IsBoss` (0x3a3158) = `(CharAI+20 >> 2) & 1`.
- Port: no accessor. The only `IsBoss` mention is a comment (`features/combat/b035_session_push_effect_v1.cpp:223`). The per-actor CharAI flag word is not in the actor state.
- Users: Assassination (IsBoss gate, both branches), GrapplingHook (IsBoss and IsMiniBoss gates on stun and pull).

**E7. Raw fixed-point comparisons and units.**
- Original: Level and SnS_Level are x256 (Level 256 = level 1, `mobs/327.md`). Lua compares GetProp values raw. The Assassinate gate `en_level < level + 2` is effectively `en_level <= level`.
- Need: any port reimplementation of these Lua comparisons keeps raw units. Do not "fix" the +2 into two levels without evidence.
- Users: Assassination (kill gate, boss split).

**E8. Monster/actor stun state.**
- Original: `Character::_SetStunState` (0x3b9580) calls `CharStateMachine::SM_SetStunState(sm, duration_ms, flag, nullptr, 0)`; state `Stunned` is read via `GetState` (0x3b6d78). The Lua passes `(duration, true)`.
- Port: MISSING ("no stun state in CombatSession", `runtime_skill_use_profile_magesub_v1.hpp:16`; `runtime_skill_use_profile_knight_v1.hpp:17`; `docs/BUGS-AND-IMPLEMENTATION.md` B165).
- Users: GrapplingHook (A). Also ViciousStrike (`_Stun` outcome, base Rogue), Knight Warcry, Mage magesub (2000-4000 ms). This is the most-shared gap.

**E9. HeadTo(point): forced locomotion toward a point.**
- Original: `Character::_HeadTo` (0x3ba71c).
- Port: MISSING for characters. Only the companion `HeadTowards` (`features/companions/source_companions.cpp:211`) and the commented source command (`original_controller_commands.hpp:26`).
- Users: GrapplingHook (pull). Mage tornado carries are an inference already noted (`runtime_mage_skill_owner_v1.cpp:428, 553`). Pull speed is not authored (Lua TODO).

**E10. Player skill projectiles (correction to the skills-rogue report).**
- Port: the shared executor and skill launch API exist (`runtime_session_projectile_v1.hpp:126-137`, `runtime_session_projectile_v1.cpp:359`), used by the mage owner (`runtime_mage_skill_owner_v1.cpp:220, 382`).
- Need for GrapplingHook: a rogue external owner (same pattern as mage, `runtime_skill_external_owner_v1.hpp`) and the ProjectileTable row 21 loaded by the launch path. Not a blocker in the sense the report claimed.
- Users: GrapplingHook (A). Archer x5 (not this agent).

**E11. DOT ticks.**
- Original: `CharProperties::HandleDots` (0x3df3f0): for 5 DOT slots, if the DOT property is > 0, `F_DotAttack` + `F_ApplyResult` per tick.
- Port: "Full original DOT/status/leech/FX/quest outcomes retained for host; only direct health application is active" (`combat_session.cpp:1607`). MISSING.
- Users: PoisonSting (`Dot_Min/Max/Duration`, 123-125). Affects other classes with DOT too.

**E12. Slow tail on skill hits (F_ApplyResult slow outcome).**
- Port: `debuff_slow_spec_v1` (`runtime_timed_buff_v1.hpp:44-57`) implements the 0x100 outcome with duration from 189 or 146. It is wired to weapon/source hits (`main.cpp:9203-9207`). Whether a PoisonSting skill hit raises the same outcome and mask is not verified.
- Users: PoisonSting (Slow_Target_Chance 145, Slow_Duration 146). PARTIAL.

**E13. Class formula type 6 (shared game-data).**
- Port: `port/game-data/class_tables.cpp:24`: `sheet[d] = add(asr8(mul(buff?buff[d]:sheet[d], sheet[a])), a)`.
- IDA: `CharProperties::_ScaleWithPercentageForBuff` (0x3e2c10): `SetProperty(dest, (GetProperty(P1) * GetProperty(dest or base)) >> 8)`, no added term.
- Users: blade_defect (59 <- 190). Other classes may use type 6 too (not checked here). Not fixed in Phase 1: it is shared code and the IDA reading of the caller is needed first (Q3).

**E14. Skill-level class projection (SetTempProps / GetProp(...,true)).**
- Port already projects the class sheet (`formula_sheet`, `runtime_skill_cast_coordinator_v1.cpp:1243`; `runtime_skill_external_owner_v1.hpp:31-33`). Status: WORKS for the Quickness pattern. Reuse for Blindness/PoisonSting/Assassination.

**E15. Sneak state (Special_Sneak > 0).**
- Port: CancelSneaking branch on `resolved[198] > 0` (`features/enemy_ai/runtime_enemy_controller_v1.cpp:244-253`), and the target search gate `caster.199 < candidate.198` (`runtime_skill_target_query_v1.cpp:172-176`). Blindness adds Special_Sneak, so part of the blinding and the sneak bonus may already run through these gates. PARTIAL, needs a run (Q6).

---

## 4. Passive items: what the damage/loot consumers need

Assassinate writes 92/93, TreasureHunter writes 195, Contaminate and blade_defect write 190/191/59. The port has no name-based consumer for 92, 93, 195 or 59 outside the character menu (grep of `port/`). The damage and loot code is ported by sheet index, not by name, so consumption cannot be ruled in or out from the names. OPEN (Q5).

---

## 5. Shared Rogue skills (base batch owns these, listed for completeness)

| Skill | Type/lvl | Status seen in Phase 1 |
|---|---|---|
| JumpKick | A/0 | Profile row present (`runtime_skill_use_profile_v1.hpp:127`); matrix/coverage say working-verified in EXE (P13). Not audited here |
| ViciousStrike | A/3 | Profile row present; stun outcome MISSING (E8) (B165) |
| Roundhouse | A/9 | Profile row present. Not audited |
| Quickness | A/6 | Buff row (`runtime_skill_use_profile_v1.hpp:145`). Not audited |
| Acrobat, Ambidextrous, ComboMaster, FindWeakness | P/3,0,6,9 | Matrix: "no gameplay effect in code". Same passive gap as E3 |

---

## 6. Visual evidence (AGENTS.md rule) - NOT gathered

- The only original gameplay footage in `.local-inputs` is `.local-inputs/reference-video/dh2-act1/` (Part 1 of an Act 1 walkthrough). No Assassin footage was found. The earliest Assassin skill is level 12 and the subclass unlock level is not recorded here, so Part 1 is unlikely to show it (INFERENCE).
- Needed for the verify phase: original footage of Assassination (kill and damage), Blindness (who stops targeting the caster, does the attack continue, does the caster's hit break it), PoisonSting (DOT and slow visible), GrapplingHook (stun, pull speed, projectile path), Contaminate (proc frames). Frame sequences, not stills.
- Until then every visual claim above is UNVERIFIED.

---

## 7. Open questions (for the lead)

- **Q1.** AnimationSequence Anim 1224/1225/1229 and 908: confirm the dictionary names (matrix says prince clips for 517/518/520, NPC clip for 519). Not re-verified for 1224/1225/1229 in Phase 1.
- **Q2.** `Character::Kill` (0x3a5b18): what does it do beyond death (XP, trophies, events, loot)? Needed before E5 is designed.
- **Q3.** Type 6 (E13): confirm the caller's operand order with the IDA decompile of the class apply loop, then decide Rating_Defense semantics for blade_defect. Also check which other class rows use type 6.
- **Q4.** Prince hook clips (`skill_dh2_prince_rogue_assassin_hook{,_pre,_post}`, dictionary 1226-1228): used by any original path? No sequence references them.
- **Q5.** Name-independent consumers of 92, 93, 195, 59, 198: confirm which port code reads them by index (damage formula, loot, defense, sneak).
- **Q6.** Is Blindness's blinding carried by Special_Sneak (198) through the existing sneak/target gates, by SetIsTargetable, or by both? Needs a run with the Assassin row and an enemy-side trace.
- **Q7.** StealAIEvent / StartSkillTimerCB / SetTempProps / RegisterSkill: which native or script layer implements them? Not in the libDungeonHunter2 binding tables (grep). Needed for E2 and for the timer semantics in Blindness and PoisonSting.
- **Q8.** Assassination gate: confirm the raw comparison with a runtime check (a level-N player against a level-N and a level-N+1 monster in a test save).
- **Q9.** `Special_Loot_Gold_Multiplier` (195) and the unit of `Damage_Bonus_*` (92/93): percent or flat? Needed to state the expected visible value.
- **Q10.** The atlas `show skill:Assassination` returned the `Assassinate` record (name lookup collision). Phase 1 used the table rows instead. Report to the atlas owner (read-only use here).
- **Q11.** Text for the localized names (`SkillName` 1245475/78/81/84/87/90/93/... for the Assassin skills) was not resolved in Phase 1. Needed to show the right name in UI tests.

---

## 8. Recommended order for Phase 2 (for the lead; not decided here)

1. E7 and E13 (shared formula semantics; small, testable).
2. E1 and E2 (player targetability split and hit events; shared with Swamp King and the Rogue base).
3. E3 (generic passive owner; unlocks 4 Assassin passives + 4 Rogue passives).
4. E5 + E6 (kill with killer, IsBoss/IsMiniBoss; unlocks Assassination).
5. E8 + E9 (stun and HeadTo; unlocks GrapplingHook; shared with ViciousStrike, Warcry, Mage).
6. E11 and E12 (DOT and skill slow tail; unlocks PoisonSting fully).

Then: run Blindness and PoisonSting on the activation harness (the first step is the existing `runtime_skill_activation_session_v1_tests` fixture), and capture original footage for the visual gate.

---

## 9. What this phase changed

- Created branch `v21.8-sub-assassin` and worktree `.claude/worktrees/sub-assassin` (from `windows-foundation` 7b9028cb).
- Added this file only. No engine code, no build, no ctest, no junctions, no subst letter used.
- Scratch file `C:\dh218\assassin-lua-dump.txt` (empty result of the wrong reader) was deleted. `C:\dh218\assassin-charprops-names.json` (CharacterProperties name order) was kept as a reference.
