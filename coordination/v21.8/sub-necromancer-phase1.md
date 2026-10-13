# Necromancer subclass: Phase 1 investigation (v21.8)

Date: 2026-10-14. Agent: necromancer. Branch: `v21.8-sub-necromancer`. Worktree: `.claude/worktrees/sub-necromancer`. Base: `windows-foundation` @ 7b9028cb.
Scope: investigation only. No engine code, no build, no test run in this phase. Only this report is committed.

Confidence labels: PROVEN (read directly from the source named), LIKELY (strong reading, not directly confirmed), GUESS (unverified). Anything marked INFERENCE is not original behaviour.

## 0. Method and sources

- Original Lua: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_*.luac` and `prince_mage_*.luac`. These are plain-text Lua, not bytecode, so they were read directly (the `.luac` extension is misleading; `lua_read_v13.py` is an unrelated original.asm reader and was not used). Dumps are in `C:/dh218/necro-scratch/`.
- Shared helpers (defines `RegisterSkill`, `RegisterSkillEx`, `StartSkillTimerCB`, `SetSkillCooldown`, `SkillCombatRoll`, `StealAIEvent`, `SetTempProps`, `CalcManaCost`, `IsBoss`): `skills/_commons.luac` and `ai/_commons.luac`.
- ClassTable sheets (raw): the existing probe `C:/dh218/probe/classdump.exe` on `pydata`, output in `C:/dh218/necro-scratch/necro_classdump.txt`. Property names come from that probe. The atlas `tables.json` rows agree on every value (checked for all 8 Necromancer rows).
- Fixed point: IDA `LuaScript::_FromFixed` 0x37ee84 is `(int)x >> 8` times 0.0039062 (1/256). Value rule used below: actual = (p1 + p3 * SnS_Level) / 256 for Formula type 1. Cross-checked: the Skill_Necromancer_Pestilence cooldown raw 768000 gives 3000 ms, which matches the script header "Cool down 3 sec", and the Pestilence Lua fallback RANGE = 400 + 20 * L matches the class row TempProp1 (102400 + 5120 L)/256. (PROVEN for the scale; the L-linear reading is LIKELY.)
- Atlas (read only, `.claude/worktrees/atlas`): `coordination/atlas/general/native-map.md`, `README.md` "Engine functions to implement next", `data/tables.json`. Not run: `regen`, `check`, any edit.
- IDA: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`. Addresses below are from this export.
- Port: `port/windows-foundation/features/generic_skills/*`, `combat_session.cpp`, `features/combat/b035_*`, `features/spawn/spawn_character_v1.hpp`, `features/status_effects/*`, `main.cpp`.
- Assets: `.local-inputs/windows-source-clock-v21.8-staged/audio-assets/data/sounds/` (checked by file name); `coordination/v21.8/SKILL-VERIFICATION-MATRIX.json` (static columns).

## 1. Tree ownership (from SKILL-VERIFICATION-MATRIX.json, tree "Necromancer", 16 entries = 8 active + 8 passive)

Owned by the Necromancer script prefix (8, the subject of this phase):

| id | Skill | Type | Level | Script |
|---|---|---|---|---|
| 26 | DeathClaw (Harrowing Grasp) | active | 21 | prince_necromancer_deathclaw |
| 47 | FireSkullRain (Reaper Storm) | active | 15 | prince_necromancer_fireskullrain |
| 81 | Pestilence (Pestilence Breath) | active | 18 | prince_necromancer_pestilence |
| 93 | Rise (Raise) | active | 12 | prince_necromancer_rise |
| 11 | CarrionEater (Soul Eater) | passive | 18 | prince_necromancer_carrioneater |
| 60 | Leecher (Drain Sphere) | passive | 12 | prince_necromancer_leecher |
| 89 | Putrid (Putrid Touch) | passive | 21 | prince_necromancer_putrid |
| 125 | Wrath (Spell Overdrive) | passive | 15 | prince_necromancer_wrath |

Shared from the Mage base tree (listed for completeness, owned by the v21.8-mage wave; the brief says these are not ours unless broken):

| id | Skill | Type | Script |
|---|---|---|---|
| 12 | ChainLightning | active | prince_mage_chain_lightning |
| 14 | ColdRay | active | prince_mage_coldray |
| 19 | Cyclone | active | prince_mage_cyclone |
| 104 | StoneEscape | active | prince_mage_stone_escape |
| 36 | ElementalMastery | passive | prince_mage_elemental_mastery |
| 42 | FaeryBlood | passive | prince_mage_faeryblood |
| 72 | MagisShielding | passive | prince_mage_magi_shielding |
| 103 | StaffMaster | passive | prince_mage_staffmaster |

Note on the matrix: the Necromancer `skills` count of 8 active includes the 4 shared Mage actives, and the 8 passives include the 4 shared Mage passives. The Necromancer ClassTable row is 292 (`MagePlayerBase_Necromancer`, subclass of Mage).

## 2. Corrections to the brief and to the mage-sub report

These were checked against the source. Each one changes the engine need.

- C1 (PROVEN). Pestilence has no OnCheckDead hook. `OnCheckDead_` is never defined in any script (the only textual reference is `prince_necromancer_pestilence.luac:209`). It is passed as the 6th argument of `RegisterSkill`, which `skills/_commons.luac:41-47` maps to the `delayed` callback. An undefined global is nil, so the callback is the default no-op. Pestilence death detection is a script poll: `StartSkillTimerCB(500, true, UpdateAttackedTargets)` runs `IsDead()` over the infected list every 500 ms. The engine need is therefore a repeating timer plus an IsDead query, not a new callback.
- C2 (PROVEN for the Lua, LIKELY for the native side). Pestilence has no DoT tick loop in Lua. `ApplyPropClass(CLASS_ID)` writes SnS_Dot_Min_Damage (179), SnS_Dot_Max_Damage (180) and SnS_Dot_Duration (181). The damage over time must come from the native F_ApplyResult DOT tail. The port does not apply it: `combat_session.cpp:1607` reads "Full original DOT/status/leech/FX/quest outcomes retained for host; only direct health application is active".
- C3 (PROVEN). The knockback gap is narrower than the report says. Native knockback exists: F_ApplyResult calls SM_SetKnockBackState when outcome bit 0x80 is set and the defender is a Player in Idle (`main.cpp:5945` comment; `features/combat/b035_session_push_effect_v1.*`; monster defenders admitted at `combat_session.cpp:2427`). What is missing is the Lua call `target:SetKnockBackState(true)` that DeathClaw makes unconditionally on each damaged Character. See E5 for the IDA detail (it selects the "great" bank).
- C4 (PROVEN). The mage-sub report says the shared projectile executor is "on v21.8-mage, not merged". On base 7b9028cb it is present: `features/generic_skills/runtime_mage_skill_owner_v1.*` (ChainLightning, Cyclone, StoneEscape via `handles()` at `runtime_mage_skill_owner_v1.cpp:137-140`) over `features/effects/runtime_session_projectile_v1.*`. Rise's projectile is therefore no longer a blocker by itself. What remains for Rise is the summon and the path probes (E7, E8).
- C5 (PROVEN). DeathClaw's OnPre search is `TargetListSearch(RANGE)` with no angle (`prince_necromancer_deathclaw.luac`, OnPreSkill_). The profile row `runtime_skill_use_profile_magesub_v1.hpp:32-36` uses `pre_cone_radians = 2.356 (135 degrees)`, and its header comment claims "OnPre TargetListSearch(RANGE 680, ANGLE 135)". That is a transcription error. The same header documents the no-angle default as pi (`runtime_skill_use_profile_v1.hpp`, `pre_cone_radians` comment). The OnSkill_ search (680, 135 degrees, origin shift -40) is correct as written. Phase 2 must fix the pre cone to the no-angle default.
- C6 (PROVEN). Pestilence's Lua never starts a cooldown. Its OnPreSkill_ has no SetSkillCooldown (the only one is commented in the header), and OnSkillCheck_ is `HasMana` only. The class row still carries SnS_Cooldown = 3000 ms and the header says "Cool down: 3 sec". Whether the engine reads SnS_Cooldown for this skill is an open question (Q4).
- C7 (PROVEN). FX set numbering disagrees between sources. The matrix gives FX sets 157 (FireSkullRain), 158 (Pestilence), 159 (Rise). The atlas `effect_sets` index for the same AnimatedEffectTable names is 230, 231, 232 (`data/tables.json`, names `spell_dh2_prince_necromancer_*`, confidence PROVEN). The two numbering spaces are not reconciled (Q2).
- C8 (PROVEN). Sound sources disagree for Pestilence. The matrix's ledger cue is `sfx_skill_mage_nec_pestilence_breath` (ordinal 534, uid 129). The Lua PlaySound is `sfx_skill_mage_doppelganger_statue_explode` (`prince_necromancer_pestilence.luac`, DamageNearbyTargets). Both wav files are present in the staged package.
- C9 (PROVEN). The mage-sub report's DoT and knockback claims were written before the Lua was read in full. Its Pestilence entry is corrected by C1 and C2, and its FireSkullRain entry (repeating 1000 ms timer, max 3 ticks, stops when empty) matches the Lua.

## 3. Per-skill summary

Port status key: CAST (admission and cost/cooldown), ANIM (clip resolves), FX (presentation dispatch), SOUND (cue dispatch), DAMAGE (SkillCombatRoll), EFFECT (buff, DoT, knockback, summon, etc.). Values: WORKS / PARTIAL / MISSING.

| # | Skill | CAST | ANIM | FX | SOUND | DAMAGE | EFFECT | Verdict |
|---|---|---|---|---|---|---|---|---|
| 26 | DeathClaw | PARTIAL (profile row admitted; pre-cone error C5; unit test registered in CTest but never compiled or run per mage-sub report) | WORKS (static) | MISSING (Lua FX line is commented out, so none is dispatched by the Lua) | PARTIAL (cue resolves, dispatch path unresolved, Q5) | PARTIAL (generic coordinator loop, unrun) | MISSING (explicit great knockback, E5) | PARTIAL |
| 47 | FireSkullRain | MISSING | WORKS (static) | MISSING (no dispatch path) | PARTIAL (Q5) | MISSING (needs repeating timer, E1) | MISSING | MISSING |
| 81 | Pestilence | MISSING | WORKS (static) | MISSING | PARTIAL (two different cues, C8) | MISSING (initial hit, explosion roll, E1, E2) | MISSING (DoT tail E4, cooldown Q4) | MISSING |
| 93 | Rise | MISSING | WORKS (static) | MISSING | PARTIAL (cue resolves, not played by Lua, Q5) | MISSING | MISSING (summon E7, projectile landing E8, expiry timers E1) | MISSING |
| 11 | CarrionEater | MISSING (passive; no listener) | n/a | n/a | n/a | n/a | MISSING (E6, heal primitive E13 available) | MISSING |
| 60 | Leecher | MISSING (passive) | n/a | n/a | n/a | n/a | MISSING (E10, E11) | MISSING |
| 89 | Putrid | MISSING (passive) | n/a | n/a | n/a | n/a | MISSING (E3, E4, E6) | MISSING |
| 125 | Wrath | MISSING (passive) | n/a | n/a | n/a | n/a | MISSING (E1, E10) | MISSING |
| 12 | ChainLightning | owned by v21.8-mage (mage owner handles it) | | | | | | not reviewed here |
| 14 | ColdRay | owned by v21.8-mage (base profile row exists) | | | | | | not reviewed here |
| 19 | Cyclone | owned by v21.8-mage (mage owner) | | | | | | not reviewed here |
| 104 | StoneEscape | owned by v21.8-mage (mage owner) | | | | | | not reviewed here |
| 36, 42, 72, 103 | Mage passives | MISSING in port (only the skill-text module `runtime_skills_text_v1.cpp` names them; no effect code found) | n/a | n/a | n/a | n/a | MISSING (owned by mage wave) | not reviewed here |

Port hooks that are PARTIAL or present (by file): sound and FX dispatch services (`runtime_mage_skill_owner_v1.hpp`, `RuntimeMageSkillServicesV1::play_effect / play_sound`); timed buffs with an ActorId and a permanent flag (`features/generic_skills/runtime_timed_buff_v1.hpp:51-53`, `runtime_timed_buff_v1.cpp:179-181`); HP regen primitive (`main.cpp`, `playable_actor_world.hpp`); a ClassTable-driven SkillCombatRoll (`runtime_skill_cast_coordinator_v1.cpp`).

## 4. Per-skill detail

### 4.1 DeathClaw (Harrowing Grasp), id 26, active, class 183

Lua (`prince_necromancer_deathclaw.luac`), plain words:
- OnSkillUpdate: mana = CalcManaCost(class). Cooldown = 1500 + 0 * FromFixed(plvl) + 0 * slvl. `plvl` is an undefined global (C10 below).
- OnSkillCheck: not on cooldown and enough mana.
- OnPre: enemy-only, attackable-only, FrontalFirst sort. `TargetListSearch(680)` with no angle. SetSkillCooldown(1500). LookAt top target if any. UseMana.
- OnSkill: SetProp(SnS_Level) and ApplyPropClass(class). `TargetListSearch(680, 135 degrees, origin (0,-40,0), true)`. Loop: pop top; SkillCombatRoll(target); if the target is a Character and dmg > 0, `target:SetKnockBackState(true)`. Objects are rolled too but never knocked back. The PlayFX on the target is commented out.
- OnPost: ClearTarget.

Class sheet (raw, /256): mana 21760 + 2560 L = 85 + 10 L (C-note: the header of the Pestilence and Rise scripts says mana 15; the class row is authoritative for the port, see Q6). SnS_MinDamage 6400 + 1024 L = 25 + 4 L. SnS_MaxDamage 10240 + 1280 L = 40 + 5 L. TempProp1 (230 + 25 L)/256 (about 1.0 at L=1). Formula type 5 writes Min and Max from TempProp1 (exact meaning not decoded, GUESS: a scaled copy). No SnS_Cooldown row (the 1500 comes from Lua).

C10 (LIKELY). `plvl` in DeathClaw OnSkillUpdate is a global that is never assigned. `FromFixed(nil)` reads the value as 0 in the native (`LuaScript::_FromFixed`, getNumber on nil assumed to be 0), so the cooldown evaluates to 1500. This is a latent original script bug, with no visible effect if the native behaves as read.

Anim/FX/Sound:
- ANIM: AnimTable root 415 resolves to `data/3D/characters/prince/animations/skill_dh2_prince_mage_necromancer_death_claw.bdae` (matrix, present).
- FX: the Lua FX `spell_dh2_prince_necromancer_deathclaw` is commented out, so no FX is dispatched by the Lua. The matrix's FX set 156 (`skill_dh2_prince_mage_necromancer_death_claw.bdae`) is the interface FX on the skill row, dispatch path unresolved (Q5).
- SOUND: cue `sfx_skill_mage_nec_harrowing_grasp` (ledger source_id 533, uid 126, wav present). The Lua has no PlaySound, so the cue is played by a data path (Q5).

Port: profile row `runtime_skill_use_profile_magesub_v1.hpp:32-36`, fields `external_owner=false`, `rolls_combat=true`, `fixed_cooldown_ms=1500`, `use_sort=frontal_first`, `use_look_at=false` match the Lua. The unit test (`runtime_skill_use_profile_magesub_v1_tests.cpp`, CTest name `runtime_skill_use_profile_magesub_v1`) asserts admission only. It has not been compiled or run in a real hidden-window session (no Necromancer save exists, mage-sub report line 39).

IDA for the calls: `Character::_SetKnockBackState` 0x3b7680 (see E5). `Character::_SkillCombatRoll` 0x3b9fbc (not read further in this phase).

Verdict: PARTIAL. Needs C5 fixed, an explicit knockback path (E5), and a real run.

### 4.2 FireSkullRain (Reaper Storm), id 47, active, class 184

Lua:
- OnSkillUpdate: mana = CalcManaCost. Cooldown = FromFixed(SnS_Cooldown); if <= 0 use 4000 ms.
- OnPre: UseMana(mana_cost); SetSkillCooldown(cooldown). No target query.
- OnSkill: if no timer is running, `StartSkillTimerCB(1000, true, DamageTick)`.
- DamageTick (every 1000 ms): enemy characters only, object filter NONE, NoSort. `TargetListSearch(280, 360)` around the caster (no origin argument). If tick_count < 1: play the meteor FX on the first target (`PlayFX` on the target), or if the list is empty, `PlayFX(METEOR_FX, x+160, y+160, z)` at a fixed offset from the caster. If the list is empty: stop the timer, reset tick_count, return. Otherwise `ApplyPropClass(class)`, SkillCombatRoll on every target. If tick_count >= MAX_TICKS (2): stop the timer and return. Otherwise tick_count + 1.
- Result: up to 3 damage ticks (ticks with tick_count 0, 1, 2), stopping early if the area empties. The first tick happens 1000 ms after cast if StartTimer fires at +interval (INFERENCE, LIKELY). FX only on the first tick.
- SPOT_FX (`target_circle_05_attack`) is declared and never used. EXTRA_RANGE is declared and never used.

Class sheet: mana 16640 + 2304 L = 65 + 9 L. Min 6400 + 256 L = 25 + L. Max 10240 + 256 L = 40 + L. TempProp1 (230 + 25 L)/256. SnS_Duration 256000 + 128000 L = 1000 + 500 L ms (NOT read by the Lua; Q7). SnS_Cooldown 832000 = 3250 ms flat.

Anim/FX/Sound: ANIM 418 resolves to `skill_dh2_prince_mage_necromancer_fire_skull_rain.bdae` (present). FX `spell_dh2_prince_necromancer_fireskullrain` = effect_sets 230 (atlas PROVEN); matrix FX set 157 (C7). SOUND `sfx_skill_mage_nec_reaperstorm` (ledger 536, uid 127, wav present); the Lua has no PlaySound (Q5).

Port: no profile row, no owner (MISSING). The matrix's "projectile: heuristic" flag is wrong for this Lua: the skill is not a projectile, it is a timed AoE on the caster area, with FX on the target.

Verdict: MISSING. Needs E1 (repeating skill timer) and the DAMAGE path via a radius search around the caster.

### 4.3 Pestilence (Pestilence Breath), id 81, active, class 186

Lua:
- Constants: RANGE (overwritten in OnSkillUpdate), radius 200 (explosion), attackedTargets list, timer.
- OnSkillUpdate: mana = CalcManaCost(class, skill_level). timeofdot = FromFixed(SnS_Dot_Duration) (4000 + 1000 L ms). RANGE = FromFixed(TempProp1) (400 + 20 L, or 400 + 20 L fallback if 0). If skill_level != 0: restart `StartSkillTimerCB(500, true, UpdateAttackedTargets)`.
- OnPre: enemy-only, attackable-only, FrontalFirst; `TargetListSearch(RANGE, 240)`; LookAt top target; UseMana. No cooldown (C6).
- OnSkill: ApplyPropClass(class). For each popped target that is not the player: if it is a Character and not already in attackedTargets (matched by name), `SkillCombatRoll` (initial hit), add to attackedTargets, and `StartSkillTimerCB(timeofdot + 1000, false, RemoveTarget, target)` (the infection window is timeofdot + 1000 ms). Objects get SkillCombatRoll only.
- UpdateAttackedTargets (every 500 ms): for each infected victim that IsDead(): DamageNearbyTargets(victim).
- DamageNearbyTargets(victim): ApplyPropClass(class). `TargetListSearch(200, 360, victim)` ClosestFirst, enemy, attackable-only. `victim:PlayFX(spell_dh2_prince_necromancer_pestilence)`. `PlaySound("sfx_skill_mage_doppelganger_statue_explode")`. SkillCombatRoll on every popped target. The victim itself may be included in that search (INFERENCE, Q8).
- Post: ClearTarget.
- No DoT ticks are written in Lua. The DoT is data (SnS_Dot_Min/Max_Damage and SnS_Dot_Duration) applied by the native hit path (C2, E4).

Class sheet: mana 19200 + 2304 L = 75 + 9 L. Dot min 6400 + 256 L = 25 + L. Dot max 8960 + 256 L = 35 + L. SnS_Dot_Duration 1024000 + 256000 L = 4000 + 1000 L ms (5000 ms at L=1, matches "DoT Duration 5 sec"). SnS_Cooldown 768000 = 3000 ms (not started by Lua, C6). TempProp1 written twice: (230 + 25 L)/256 and then (102400 + 5120 L)/256 = 400 + 20 L. LIKELY: the later row wins, which matches the Lua RANGE fallback formula exactly.

Anim/FX/Sound: ANIM 421 resolves to `skill_dh2_prince_mage_necromancer_pestilence.bdae` (present). FX `spell_dh2_prince_necromancer_pestilence` = effect_sets 231 (PROVEN in atlas). SOUND: two candidates (C8): the Lua `sfx_skill_mage_doppelganger_statue_explode` (wav present) and the ledger cue `sfx_skill_mage_nec_pestilence_breath` (ordinal 534, uid 129, wav present). The Lua is the only caller-level evidence, so the Lua cue is the one played by this script.

Port: nothing (MISSING). Needs E1 (500 ms loop, per-victim one-shot), E2 (IsDead on a tracked list), E4 (DoT tail), and an explosion radius search from a victim position.

Verdict: MISSING.

### 4.4 Rise (Raise), id 93, active, class 188

Lua (large; the important logic):
- Constants: MOVE_RANGE 650; risen table RISEN[1..3]; PROJECTILE_ID = ProjectileTable `TeleportTool` (projectile index 35); RISE_FX = `spell_dh2_prince_necromancer_rise`; CLASS_ID; `plvl = GetProp(CharacterProperties.Level)` captured at file load (not updated; INFERENCE, Q9).
- OnSkillUpdate: mana = CalcManaCost(class). bufftime = FromFixed(SnS_Duration) (9000 + 1000 L ms; if 0 use 10000).
- OnPre: UseMana; DoRise(count). DoRise tries four probe directions with `MoveTo(dir, 650, true)` and `HasPath()`: front (0), back (180), right (90), left (270), else front again. The probe spawns `SpawnSkillProjectile(TeleportTool, angle)`.
- OnSkill: count = checkslot() (first empty slot; else the first slot whose risen is dead, freed; else -1 when all three are alive). If count != -1: if x is nil, take the landing position from the projectile (`GetPosition` through a `_this` hack). spawn(count): Summon(CharacterTable `RisenZombie`, false, x, y, z, false), LOCK, PlayFX(RISE_FX), SetLevel(plvl). Start a one-shot timer `StartSkillTimerCB(bufftime, false, killed, count)` that kills and unlocks the risen. If count == -1: ForceKillAndSpawnNew (kills the slot with the oldest spawn counter, stops its timer, spawns a new one at the last landing position).
- OnDelayedSkill (projectile landed on a non-character): record x, y, z from the projectile position. OnDelayedSkillCheck: true when the target is a non-character.
- OnSkillCleanUp: KillAllSummoned, reset counters. Registered through RegisterSkillEx `cleanup`.
- OnPost: ClearTarget. No cooldown (header "Cool down: na"). OnSkillCheck: HasMana only.
- Header comment says "Time to respawn after death: 2 sec". The code has no 2 s respawn: a dead risen frees its slot immediately in checkslot. Header and code disagree (Q10).

Class sheet: mana 12800 + 1280 L = 50 + 5 L. SnS_Duration 2304000 + 256000 L = 9000 + 1000 L ms (10000 ms at L=1, matches header "Duration before redeath 10 sec"). No cooldown row.

Anim/FX/Sound: ANIM 422 resolves to `skill_dh2_prince_mage_necromancer_rise.bdae` (present). FX `spell_dh2_prince_necromancer_rise` = effect_sets 232 (PROVEN in atlas). SOUND `sfx_skill_mage_nec_raise` (ledger 535, uid 128, wav present), not played by this Lua (Q5). The summoned monster `RisenZombie` is CharacterTable name `RisenZombie` (atlas has rows 503 and 163945; which row is the one used by the Lua is not decoded, Q11).

Port: nothing for the summon (see E7). The projectile executor exists (C4), so the TeleportTool launch and its landing callback (E8) are the reusable parts.

Verdict: MISSING.

### 4.5 CarrionEater (Soul Eater), id 11, passive, class 182

Lua: `StealAIEvent("OnTargetHit", OnTargetHit_Skill)`. When skill_level > 0, attacker is self, and defender is dead: SetProp(SnS_Level); ApplyPropClass(class); heal `regenHP_amount` (TempProp1), clamped to max HP, with `RegenHP` as fallback if the first SetProp did not change HP. Comment: "RegenHP did not work ... used another method and tested".

Class sheet: TempProp1 (1280 + 1280 L)/256 = 5 + 5 L HP per kill. Header says 5 per kill (matches only at L=0; LIKELY the formula is the authoritative value, Q12).

Port: no OnTargetHit delivery (E6). HP primitive: `RegenHP` and SetProp HP exist in the port (main.cpp / playable_actor_world; E13, available).

IDA: `AISDefault::OnCombatResults` 0x3dc9a8 calls `OnTargetHit(attacker, defender)` when the combat result has no miss bits (`a4[24] & 3 == 0`) and the AI's flag 0x800 is set. The flag is set in `AISDefault::InitVCB` 0x3dc7d8 from `LuaScript::IsInVFTable(this, "OnTargetHit")`. The AI on which the listener lives is the AI that received `AddToVFTable`. For the player's own hits this path is clear (PROVEN in the reading). For the kill event, the attacker AI is the player's AI (PROVEN for the path; the ordering of kill and event is LIKELY).

Verdict: MISSING.

### 4.6 Leecher (Drain Sphere), id 60, passive, class 185

Lua: OnSkillUpdate: remove old buff; if slvl != 0: `CreateBuff(class, 0, false, slvl)` and `ApplyBuff(class, buff)`. Nothing else in the script. Header: "Drain mana from elemental enemies, reduce the elemental damage they deal. MP Drained: 2/hit." The actual drain and damage reduction must live in the buff's class sheet and in native leech handling. Neither is in the script.

Class sheet: Leech_MP (133) = (256 + 128 L)/256 = 1 + 0.5 L per hit. The header's "2/hit" equals this only at L = 2 (Q13). The elemental damage reduction has no property in this row (only Leech_MP), so the reduction is either native or in another row (Q13).

Port: `combat_session.cpp:1607` says leech outcomes are not applied (E11). The passive buff itself can use the permanent path (E10).

Verdict: MISSING.

### 4.7 Putrid (Putrid Touch), id 89, passive, class 187

Lua: `StealAIEvent("OnTargetHit", OnTargetHit_Skill)`. When skill_level > 0 and defender is self and attacker is in melee range: roll `GetRand(0,100)`; if < DOT (TempProp1 as percent): `attacker:CreateBuff(class, duration, false, skill_level, AUTO_DOT_00_NORMAL)` and `attacker:ApplyBuff(class, buff)`. duration = TempProp2 (ms). The PROC_PCT and PROC_PCT_SCALE constants are unused (the class row carries the same values).

Class sheet: proc chance TempProp1 (1280 + 512 L)/256 = 5 + 2 L %. Header: "Chance of DoT 5%" and PROC_PCT_SCALE 2 per level, which agree with the formula (PROVEN for the numbers, LIKELY for the L-linear reading). DoT duration TempProp2 (768000 + 128000 L)/256 = 3000 + 500 L ms. Dot_Tick_Damage_Normal (126) = (7680 + 512 L)/256 = 30 + 2 L per tick (the tick value; its consumer is native, E4).

IDA: `Character::_CreateBuff` 0x3b86a8 returns early unless the class id is valid and the first state-stack entry is state 3 (Idle). So a CreateBuff on a monster that is mid-attack (the usual state at the moment of a hit, INFERENCE) silently creates no buff (PROVEN gate; LIKELY consequence). `ApplyBuff` gate not read.

Defender-side delivery (Q14, OPEN): the OnTargetHit listener is registered in the player's AI VFTable. `AISDefault::OnCombatResults` only calls it on the attacker's AI. The export shows only one forwarder call to `CharAI::OnCombatResults` (`pseudocode-all.c:150154-150165`, vtable slot +180). The defender-side caller was not located in the export, so it is not known whether a monster hit reaches the player's Putrid listener at all.

Verdict: MISSING. Needs E3, E4, E6 (and the Q14 answer).

### 4.8 Wrath (Spell Overdrive), id 125, passive, class 189

Lua: StartSkillTimerCB(3000, true, canApplyBuff) when skill_level != 0. canApplyBuff: if HP% < 40 and no buff: `CreateBuff(class, 0, false, skill_level, active_buff_fx)`, `ApplyBuff`, `PlaySound("sfx_passive_skill_activate_3")`. If HP% >= 40 and buff exists: RemoveBuff. The buff is the only source of the bonus.

Class sheet: Spell_Damage_Bonus_Fire (166), Water (167), Lightning (168), Earth (169), Air (170): each (768 + 768 L)/256 = 3 + 3 L. Header says "+20% elemental damage" (the unit and scale of 3 + 3 L do not give 20 at any small level; Q15). No consumer of props 166-170 was found by name in the damage path (the only hits are in menu/art files, `features/character_menu/*`), so the bonus is not wired (Q15).

Port: nothing (MISSING). Needs E1 (3000 ms loop), E10 (permanent buff with class sheet), and a consumer of 166-170 in the elemental damage path.

Verdict: MISSING.

## 5. Shared Mage passives (owner: mage wave; listed for the Necromancer tree)

- ElementalMastery (36): CreateBuff(class, 0, false, slvl) and ApplyBuff on self (buff: elemental damage up, resistances up). No effect code for the class in the port (MISSING in base; the skill-text module names it only).
- FaeryBlood (42): CreateBuff/ApplyBuff on self and `SetInt("Faery_Cooldown", SnS_Cooldown)`. Depends on the faery cooldown system (not read in this phase).
- MagisShielding (72): CreateBuff/ApplyBuff on self (avoidance, max HP). MISSING in base.
- StaffMaster (103): CreateBuff/ApplyBuff on self (damage up, mana cost down). MISSING in base.

All four are the same self-buff pattern as Leecher, so they use E10.

## 6. Shared Mage actives (owner: mage wave; facts only)

- ChainLightning (12): PlaySound `sfx_skill_mage_thunder_braid` and `_add_1/_add_2` (all present). Projectile chain via SimplifiedChainLightning (ProjectileTable) with penalty; port handles it in the mage owner.
- ColdRay (14): TargetListSearch(650, 90 degrees), ApplyPropClass, SkillCombatRoll for each popped target (objects too), PlayFX `elem_dmg_02_water` on Character targets with dmg > 0. Port: base profile row exists (`runtime_skill_use_profile_v1.hpp`, ColdRay row).
- Cyclone (19): tornado projectile with 500 ms tick, HeadTo for stunned victims (not for bosses or minibosses), a DoT once and collateral damage. Port: mage owner.
- StoneEscape (104): PlayFX on the old position, TeleportTool projectile, WarpTo to the landing point, Summon(CharacterTable `Dummy`) at the old position, explosion after 2200 ms. Port: mage owner (statue explode via the owner's clock).

## 7. Sound, animation, FX resolution (every name used by the Necromancer scripts)

| Name | Used by | Resolves to | Status |
|---|---|---|---|
| skill_dh2_prince_mage_necromancer_death_claw.bdae | DeathClaw (AnimTable 415) | staged clip | present |
| skill_dh2_prince_mage_necromancer_fire_skull_rain.bdae | FireSkullRain (AnimTable 418) | staged clip | present |
| skill_dh2_prince_mage_necromancer_pestilence.bdae | Pestilence (AnimTable 421) | staged clip | present |
| skill_dh2_prince_mage_necromancer_rise.bdae | Rise (AnimTable 422) | staged clip | present |
| spell_dh2_prince_necromancer_deathclaw (AnimatedEffectTable) | DeathClaw (commented out) | not dispatched | n/a |
| spell_dh2_prince_necromancer_fireskullrain | FireSkullRain METEOR_FX | effect_sets 230 (atlas) | resolved; numbering C7 |
| spell_dh2_prince_necromancer_pestilence | Pestilence victim FX | effect_sets 231 (atlas) | resolved; numbering C7 |
| spell_dh2_prince_necromancer_rise | Rise RISE_FX | effect_sets 232 (atlas) | resolved; numbering C7 |
| target_circle_05_attack | FireSkullRain SPOT_FX | declared, never used | n/a |
| AUTO_DOT_00_NORMAL | Putrid buff FX | AnimatedEffectTable row (atlas rows exist) | resolved by name |
| active_buff_fx | Wrath buff FX | AnimatedEffectTable row (atlas rows exist) | resolved by name |
| TeleportTool | Rise projectile | ProjectileTable index 35 (atlas) | resolved |
| RisenZombie | Rise summon | CharacterTable (atlas rows 503, 163945) | which row is not decoded (Q11) |
| sfx_skill_mage_nec_harrowing_grasp | DeathClaw cue | ledger 533 / uid 126 / wav present | resolved; dispatch Q5 |
| sfx_skill_mage_nec_reaperstorm | FireSkullRain cue | ledger 536 / uid 127 / wav present | resolved; dispatch Q5 |
| sfx_skill_mage_nec_pestilence_breath | Pestilence cue (matrix) | ledger 534 / uid 129 / wav present | resolved; conflicts with Lua cue (C8) |
| sfx_skill_mage_doppelganger_statue_explode | Pestilence PlaySound (Lua) | wav present | resolved |
| sfx_skill_mage_nec_raise | Rise cue | ledger 535 / uid 128 / wav present | resolved; dispatch Q5 |
| sfx_passive_skill_activate_3 | Wrath PlaySound | wav present | resolved |

Ledger rule used: the sound ledger (`port/engine-audio/reference/source-bindings-v38/ledger.json`, 638 rows) maps source_id to a uid and to a wav, with `exists: true` for each. The ledger does not say which animation or FX row triggers a source_id, so the dispatch path for the four data-bound cues is unresolved (Q5). No clip BDAE contains an `sfx_` marker for the four Necromancer actives (matrix `sound_marker` empty).

## 8. IDA evidence for the engine calls used by the Necromancer scripts

| Engine function (Lua) | Native handler (IDA) | What the decompiled code does | Atlas status | Agree? |
|---|---|---|---|---|
| target:SetKnockBackState(b) | Character::_SetKnockBackState 0x3b7680 -> CharStateMachine::SM_SetKnockBackState 0x3c5ea0 | No-op for bosses (IsBoss). Otherwise reads CharAnimTable row; for b = true uses offset +36 with stance mask 0x800; for false uses +72 with 0x400. Port names +36 "GreatKnockedBack", +72 "KnockedBack". With a true argument the great bank is selected. | MISSING (port) | Yes (address only) |
| SpawnSkillProjectile(id, angle) | Character::_SpawnSkillProjectile 0x3b99d0 | Gate: the state-stack count is > 1 and the first state is 3 (Idle). Otherwise returns. | MISSING (port) | Yes |
| CreateBuff(class, dur, cap, str, fx) | Character::_CreateBuff 0x3b86a8 | Gate: count != 0, first state is 3 (Idle), and class id < ClassTable size. Otherwise returns with no buff. | PARTIAL | Yes; the Idle gate is new information |
| RemoveBuff | Character::_RemoveBuff 0x3b842c | not read in this phase | PARTIAL | n/a |
| Kill | Character::_Kill 0x3b7e40 | If the first state is not 7 (Cast), or the stack is empty, calls Cmd_Kill with no user data; otherwise kill with user data. | PARTIAL | Yes |
| IsDead | Character::IsDead 0x3a2ed4, GameObject::_IsDead 0x38ea48 | Query. | PARTIAL | Yes |
| StartTimer / StopTimer | Character::_StartTimer 0x3b7590, _StopTimer 0x3b7064 | Native timers under the Lua wrappers `StartTimerCB` / `StopTimerCB` (`ai/_commons.luac`). | not in atlas (MISSING) | Atlas gap, not a disagreement |
| Summon | GameObject::_Summon 0x39193c | Creates an NPC from a CharacterTable row, places it, adds it to the caller's room, runs the spawn state. | PARTIAL | Yes (port spawn notes agree) |
| SetMaster / SetLevel | Character::_SetMaster 0x3b9084 / _SetLevel (PORTED) | Master link; level. | PARTIAL / PORTED | Yes |
| LOCK / UNLOCK | GameObject::_Lock 0x38d5f8, _Unlock 0x38d7a8 | Lock / unlock the object. | PARTIAL | n/a |
| TargetInMeleeRange | Character::_TargetInMeleeRange 0x3b7020 | Query. | atlas MISSING | Port gap, not a semantic disagreement |
| PopTargetList | GameObject::_PopTargetList 0x38fbb8 | Pop top. | atlas MISSING | Port gap |
| FromFixed / ToFixed | LuaScript::_FromFixed 0x37ee84 (value >> 8 times 1/256) | /256 fixed point. | PORTED | Yes |
| PlaySound | LuaScript::_PlaySound 0x37e578 | Play sound by name. | PARTIAL | Yes |
| PlayFX | GameObject::_PlayFX 0x3923cc | FX on object or at a position. | PARTIAL | Yes |
| StealAIEvent("OnTargetHit") | LuaScript::_AddToVFTable 0x37ec70 + AISDefault::InitVCB 0x3dc7d8 (flag 0x800) + AISDefault::OnCombatResults 0x3dc9a8 | Event delivered on the attacker AI when the result has no miss bits. | not in atlas | n/a |
| CharAI::OnCombatResults | 0x3d0da4 forwards to AI script slot +180 | Forwarder only. Defender-side caller not found in the export. | n/a | Open (Q14) |

## 9. ENGINE NEEDS (general systems, deduplicated; none is a per-skill hack)

- E1. General skill timers (StartSkillTimerCB / StopTimerCB semantics): repeating and one-shot timers with a callback and a reference, bound to a skill id, with stop. Original: `Character::_StartTimer` 0x3b7590, `_StopTimer` 0x3b7064, Lua `StartSkillTimerCB` (`skills/_commons.luac:146`). Port: only ad hoc clocks inside the mage owner (`runtime_mage_skill_owner_v1.hpp`, `advance()`). Needed by FireSkullRain (1000 ms loop, 3 ticks), Pestilence (500 ms loop, one-shot per victim), Wrath (3000 ms loop), Rise (one-shot expiry per risen). Note: the cooldown path already uses timers in the cast coordinator; a shared timer owner would serve both.
- E2. IsDead / death-watch queries on a tracked list of ActorIds (Pestilence poll; Rise checkslot). Replaces the nonexistent OnCheckDead. Available pieces: CombatSession alive state. Needed by Pestilence and Rise.
- E3. Enemy-targeted CreateBuff/ApplyBuff (Putrid on the attacker, Wrath/Leecher on self). The timed buff owner already takes any ActorId (`runtime_timed_buff_v1.hpp:51-53`). Missing: the native Idle gate (IDA `_CreateBuff` state==3), and the monster DoT tick. Needed by Putrid.
- E4. DOT status tail on hit: SnS_Dot_Min_Damage (179), SnS_Dot_Max_Damage (180), SnS_Dot_Duration (181) applied by F_ApplyResult, and the per-tick consumer of Dot_Tick_Damage_Normal (126). Port: `combat_session.cpp:1607` says only direct health is applied. A `DotTickBorrow` service exists in `features/status_effects/status_live_binding.hpp` but its wiring to CombatSession was not verified (Q16). Needed by Pestilence and Putrid.
- E5. Explicit great-knockback call from a skill: `target:SetKnockBackState(true)` on a damaged Character with the great bank (IDA 0x3b7680 / 0x3c5ea0, +36 bank, stance 0x800). Port has the native push (outcome bit 0x80 and the Player-in-Idle gate) but no skill-level call. Needed by DeathClaw. Whether the native push already fires on DeathClaw hits (outcome bit) is unknown (Q1).
- E6. Passive OnTargetHit listener delivered to a skill (attacker and defender sides). IDA `AISDefault::OnCombatResults` 0x3dc9a8, flag from `InitVCB` 0x3dc7d8. Port: none. Needed by CarrionEater (kill heal) and Putrid (melee defend). Defender-side delivery is open (Q14).
- E7. Runtime summon of a CharacterTable row with master, level, LOCK/UNLOCK and kill, from a skill, with expiry and slot management. Original: `GameObject::_Summon` 0x39193c, `_SetMaster` 0x3b9084, `_Kill` 0x3b7e40. Port: `features/spawn/spawn_character_v1.hpp` admits only slots from a pool built before the session ("CombatSession has no runtime add API"). Needed by Rise (RisenZombie x3). The Dummy statue of StoneEscape needs the same (mage wave).
- E8. Delayed-landing projectile callbacks (RegisterSkillEx `delayed` / `delayed_check`) and a four-direction free-spot probe (`MoveTo(dir, 650, true)` + `HasPath()`). Projectile executor is present (C4). Port: landing and teleport services exist for StoneEscape (`teleport_path` in `RuntimeMageSkillServicesV1`), but not a general probe. Needed by Rise.
- E9. SpawnSkillProjectile gate (state == 3 Idle, count > 1). Rise calls it from OnPre (before the skill state). Whether the caster is Idle at that moment is unknown (Q3).
- E10. Passive self-buffs with a class sheet and no duration (permanent until RemoveBuff). Port: exists (`runtime_timed_buff_v1.cpp:179-181`, permanent flag, used by the Celest faery passive). Needed by Leecher, Wrath, and the shared Mage passives. The consumer of the buff properties is the real gap (E11, Wrath below).
- E11. Consumers of buff properties in the damage / elemental path: Leech_MP (133), Spell_Damage_Bonus_* (166-170). No consumer found by name in the damage path (only menu/art). Needed by Leecher and Wrath.
- E12. Skill cleanup hook (RegisterSkillEx `cleanup`), used by Rise to kill all summoned on reset. Port: no generic hook (only the mage owner's own cleanup). Original caller of OnSkillCleanUp not located (Q17). Needed by Rise.
- E13. HP regen / set-HP primitive for the CarrionEater heal. AVAILABLE in the port (`RegenHP` in `main.cpp` and `playable_actor_world.hpp`, potion path). Not a missing engine need, listed so phase 2 does not duplicate it.
- E14. Target-list search origin offset and object flag: `TargetListSearch(RANGE, ANGLE, 0, -40, 0, true)` (DeathClaw). The port ignores the origin shift (`runtime_skill_use_profile_magesub_v1.hpp` header). Needed by DeathClaw (low impact; the -40 shift is in the original, its effect is LIKELY small).
- E15. Presentation dispatch by name (PlaySound, PlayFX on target and at position). AVAILABLE through the mage owner services (`play_effect`, `play_sound`). Listed so phase 2 reuses it.

## 10. Open questions

- Q1. Does the native F_ApplyResult push (outcome bit 0x80) already fire for DeathClaw hits? If yes, the explicit Lua call only adds the great variant on non-player defenders. Needs a trace of the DeathClaw hit's outcome bits.
- Q2. Reconcile FX set numbers: matrix 157 / 158 / 159 against atlas effect_sets 230 / 231 / 232 for the same names.
- Q3. Is the caster in Idle (state 3) during DeathClaw / Rise OnPre, given the SpawnSkillProjectile gate (E9)?
- Q4. Does the engine read SnS_Cooldown (177) for Pestilence and Rise, which never start it in Lua?
- Q5. Dispatch path for the four data-bound Necromancer cues (ledger 533 / 534 / 535 / 536) and the AnimTable / FX rows that trigger them.
- Q6. Mana values: the class sheets give DeathClaw 85 + 10 L, Pestilence 75 + 9 L, Rise 50 + 5 L, FireSkullRain 65 + 9 L. The 2010 script headers say 15 for Pestilence and Rise. Which is correct for the port (headers may be stale)? Check in game.
- Q7. FireSkullRain SnS_Duration (1000 + 500 L ms) is never read by its Lua. Is it consumed by the engine (e.g., the FX or the timer length)?
- Q8. Pestilence explosion search: does TargetListSearch(200, 360, victim) include the dead victim (it is rolled again)?
- Q9. Rise `plvl` is captured once at script load. Does the original refresh it, or is the risen level fixed at load? Also the DeathClaw `plvl` global is undefined (C10).
- Q10. Rise header says a 2 s respawn after death. The code has no such delay. Which one is the original behaviour?
- Q11. Which CharacterTable row is `RisenZombie` (atlas has rows 503 and 163945 by name)?
- Q12. CarrionEater: header 5 per kill, class 5 + 5 L. Which applies at the level the player reaches?
- Q13. Leecher: header 2 per hit, class 1 + 0.5 L. Where is the elemental damage reduction (no property in the row)?
- Q14. Defender-side OnTargetHit / OnCombatResults delivery: which call site invokes CharAI::OnCombatResults (vtable +180) for the defender? Needed for Putrid and for CarrionEater's kill event.
- Q15. Wrath: header +20 percent elemental damage vs class 3 + 3 L on five bonus cells. Unit and consumer unknown. Also check whether the damage path reads the properties by index (166-170) without a name.
- Q16. DotTickBorrow (`features/status_effects/status_live_binding.hpp`): is it wired to the CombatSession hit path? If so, E4 may be partly present.
- Q17. Who calls the original OnSkillCleanUp (skill reset, unset, death, level change)?
- Q18. Subclass save: no Mage-subclass save exists (mage-sub report line 39), so no real hidden-window run of any Necromancer skill is possible until one is created. Phase 2 needs a `MagePlayerBase_Necromancer` (CharacterTable 292) save.
- Q19. DeathClaw profile pre-cone (C5). Phase 2 must confirm the no-angle default value against the TargetListSearch implementation before changing the row.

## 11. What phase 2 should consider (not decided here)

- E1 (skill timer owner) and E2 (death-watch query) are needed by four of the eight skills. They are the first candidates for the lead's shared-engine assignment.
- E4 (DOT tail) and E3 (monster buff with Idle gate) are needed by Pestilence and Putrid, and they share the monster-side buff path.
- E6 (OnTargetHit listener) needs the Q14 answer before design.
- E7 (runtime summon) is also needed by the shared Mage StoneEscape Dummy.
- DeathClaw is the cheapest real verification: fix C5, decide E5 (Q1 first), then run it once a Necromancer save exists (Q18).

## 12. Evidence artifacts

- Lua text dumps: `C:/dh218/necro-scratch/prince_necromancer_*.txt`
- ClassTable dump with names: `C:/dh218/necro-scratch/necro_classdump.txt`
- Dump helper scripts: `C:/dh218/necro-scratch/dump_luac.py`, `class_rows.py`
- No files were created in the repository other than this report.
