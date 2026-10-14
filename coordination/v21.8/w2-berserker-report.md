# v21.8 w2-berserker: Berserker subclass skills on the general engine (phase 2b, real runs)

Branch `v21.8-w2-berserker`, worktree `.claude/worktrees/w2-berserker` (subst B:), build `C:/dh218/w2-berserker-build`.
Merged `windows-foundation` into the branch at 30f6485e (Necromancer merge, positional-row fixups, subclass menu preview fix).
Package: `.local-inputs/windows-source-clock-v21.8-staged3` (228,404-byte original-melee-bindings.xml; no `Source actor property row unavailable` line in any run).
Save: `C:/dh218/w2-berserker-saves/bers30.save` (KnightPlayerBase_Berserker, level 30, Berserk in slot 0 = key 2; `intim30.save`, `whirl30.save` the same with Intimidation / WhirlingDrift in slot 0).
Runs: `C:/dh218/cases/w2r3-*` (exe built from 30f6485e: `C:/dh218/w2-berserker-build/dh-foundation.exe`).

## Summary

- The subclass menu preview now works for the Berserker save. Frame `C:/dh218/cases/w2r3-menucap2/menucap/frame-000276-4721ms.png` (menu_StartGame) shows "ADA / BERSERKER / LEVEL 30 / Act 1 / Bogwitch Cavern" with the Berserker model. Log: `profile_actor PASS` on menu_MainMenu and menu_StartGame; `Frontend launched ... class=KnightPlayerBase_Berserker`.
- Real hidden-window runs with real keys (`--skill-key-frame` = key 2, mouse clicks for the menu) reached gameplay for every Berserker skill that is in scope. Berserk, Intimidation, WhirlingDrift, KillingStreak and Survival were cast or triggered in-game. KillingFrenzy and Toughness/Ravager were observed through their buffs on real hits.
- No skill is WORKED in full. Berserk and Intimidation are the strongest (cast, animation/FX frames viewed, status states and push observed). Damage numbers are real but the original's values are not compared (no original footage numbers in the repo).
- No gameplay engine change this round. The only code change is the `[skill] OnPre self HP cost` log line (commit 198cdc18). CTest: see the final section.
- Sounds: resolved wav files exist in staged3 `audio-assets/data/sounds/`. Nobody has listened to them.

## Real-run table

Evidence key: F = frame viewed (PNG from the case folder), L = log line, N = number from log. Frame numbers are game frames at `--fixed-step .016`.

| Skill | Cast accepted | Animation | FX | Sound (wav resolves) | Numbers | Verdict |
|---|---|---|---|---|---|---|
| Berserk (Berserk) | L: `Source skill key=2 slot=0 ... skill=Berserk` at 200, 300, 400, 500, 600 | L: `Timed buff probe action=3` at cast, `action=0` at lifecycle end (276). F: w2r3-bers/f228.png, w2r3-knock/f458.png (sword swing) | L: `Source player step FX ... sequence=339 dispatched=1 sets=165` at 200 and 300. F: fire arc in f228, f458 | none in Lua (no PlaySound in `prince_berseeker_berserk.luac`) | HP cost (N): 283.93 -> 255.537 -> 229.982 -> 206.982 -> 186.286 -> 167.657 (10 % of current each, gate 10 %). Volleys per cast: stun, damage, knockback. Stun hit once (cast 1, frame 221, `outcomes=0x40`, lizard Idle -> Stunned for 2200 ms, back to Idle at 358). Damage landed 2 of 5 casts (12.83 at frame 439 with outcome 0x4, 26.64 at frame 639). Knockback (0x80) landed in casts 3, 4, 5 (frames 458, 558, 658); push shown in F: w2r3-knock/f474.png (lizard thrown up and away). Stun on the lizard ended at 358. | PARTIAL |
| Intimidation (Warcry) | L: `skill=Intimidation` key 2 at 200 and 300 | L: authored step FX dispatched at 200 and 300 (`sequence=340 sets=168`). F: w2r3-intim/f205.png shows the pose and impact, howl ring not clearly visible | L: step FX dispatched (above); no separate howl FX line | `sfx_skill_warrior_bsk_bloodmoon_howl_hit_fear.wav` submitted at 228 and 328; `..._hit_stun.wav` submitted at 228 (both in `audio-assets/data/sounds/`). Not listened. | L: lizard A `Idle -> Scared (SetScareState 50013)`, lizard B `Idle -> Stunned (SetStunState 50012)` at 200 (GetRand draw). Scare and stun ended at frames 364-368 (`scare timer end`, `stun timer end`), about 2.7 s at 16 ms; exact SnS_Duration not logged. Boss refusal not tested. | PARTIAL (states and sounds verified in log; scared visual not confirmed in frames) |
| WhirlingDrift | L: `skill=WhirlingDrift` key 2 at 200 and 260 | F: w2r3-whirl/f228.png (sword arc, lizard in front). L: step FX per the same sequence path | none in Lua (no PlaySound) | none | N: cast 1 hit at 228, `outcomes=0x80` push, damage 21.26 (lizard 62.5 -> 41.24), `[push] KnockBack monster ... outcomes=0x80`. Cast 2 hit at 288 was a miss (`0x1`). `[status] enemy decisions blocked ticks=60` after the push. | PARTIAL (1 push, 1 miss, 2 casts) |
| KillingStreak | L: passive; no cast | n/a | not dispatched (`PlayFX(FX)` in Lua not dispatched) | `sfx_passive_skill_activate_2.wav` exists; NOT dispatched (no log line) | N: on the moth kill at 399 (`died=1`, `Source death reward xp=152`): `Skill_Berserk_KillingStreak strength=3 durationMs=5500` (4000 + 500 x 3, matches the Lua fallback) with cells 79/80 (Damage_Min/Max_Main_Hand) +2816 raw = +11, cells 81/82 (Damage_Min/Max_Off_Hand) +2816 raw = +11; dummy buff applied too. Damage after the kill not measured (no target left). | PARTIAL (trigger and buff numbers verified; damage bonus and presentation not) |
| KillingFrenzy | L: passive rank 3 | n/a | n/a (`activate_4` commented out in Lua) | none | N: buff rebuilt on each player hit: counter 1 at 202 (strength 3), 2 at 239 (6), 3 at 258 (9), 4 at 321 (12), 5 at 358 (15), stays at max at 419 (15). Cells 46/47/48 (Speed_Modifier_Walk/Rotation/Attack) = counter x rank x 256. Reset timers exercised by the rebuild path. | PARTIAL (event chain works; CanAttackInMelee gate missing; speed effect not measured) |
| Survival | L: `Timed buff applied ... Skill_Berserk_Survival strength=3 durationMs=8000` at 128, when the lizard hit the player at 28 % HP (80/283.93) | n/a | not dispatched (buff FX recorded only) | `sfx_passive_skill_activate_3.wav` exists; NOT dispatched | N: Damage_Min_Main_Hand 4096 -> 5040 raw (16.0 -> 19.7), Damage_Max_Main_Hand 8960 -> 11025 (35.0 -> 43.1), i.e. +23 %. Damage before/after NOT measured: the Berserk damage volley missed in both the Survival run and the control (w2r3-surv-ctl), and the one hit in both was the stun volley (removed=1). Return above 35 % not tested. | PARTIAL (trigger and buff numbers verified; damage effect and sound not) |
| Toughness | L: passive rank 3 at frame 60 | n/a | n/a | none | N: Max_HP cell 38 72686 -> 84043 raw = +44.36 HP: sheet 283.93 -> 328.29 (the Berserk OnPre line then reads `max=328.293`). Not compared to an in-game character sheet view. | PARTIAL (numbers from log only) |
| Ravager | L: passive rank 7 at frame 60 | n/a | n/a | none | N: Damage_Bonus_TwoHand (cell 91) 612 -> 4196 raw; threshold buff `Skill_Berserk_Ravager_buff` cell 203 (Special_Equip_2H_In_One) 0 -> 1792 (= 7). Damage effect NOT isolated: the pass-on and pass-off runs diverge in RNG after the first passive event, so the volley damage cannot be compared (37.35 off vs 38.75 on at different RNG states). | PARTIAL |
| Knight tree skills (inherited) | not run | not run | not run | not run | not run | NOT RUN (out of budget; say so) |

Also run: Knight control save (`w2-knight-ctl`) from the earlier batch reached the frontend (`profile_actor PASS`) but timed out in gameplay; not repeated here.

## Engine changes this round

1. Merge `windows-foundation` (30f6485e). No conflicts in the Berserker files.
2. `runtime_skill_cast_coordinator_v1.cpp`: `[skill] OnPre self HP cost <skill> before= after= max= gate=` and a `skipped` variant (commit 198cdc18). Log only.
3. No change for KillingFrenzy's CanAttackInMelee gate and no change for the defender-side passives (see gaps).

## Gaps and open items (labelled)

1. KillingFrenzy `CanAttackInMelee` gate is NOT implemented. The IDA native `Character::_CanAttackInMelee` (0x3b6f64) calls `ItemInventory::CanMeleeAttack`, which is not decoded in the export. Any gate written now would be an invented rule (INFERENCE: passes for a melee weapon, which is the only case for the Berserker). Every player hit counts until the original rule is decoded.
2. Defender-side passives (Mirror, GuardianShield, SideStep, Precision, Petrify) are NOT wired: they need an attacker-in-melee-range predicate for the passive listener, a HasBow predicate (only `GetAnimStance` consumes `Character::_HasBow`), and per-passive chance/duration. The monster stun/knockback state now exists (ES-STATUS), so the stun part is possible. None of these passives belongs to the Berserker tree; not implemented here.
3. Block outcome: in the port, `outcomes=0x4` (block) still removed full damage (12.83 at frame 439). Whether the original reduces or nullifies damage on block is not verified.
4. Stale log text: each Use hit prints "stun/push have no host state, only hurt drives a reaction". This is wrong now (`[status] ... Idle -> Stunned` and `[push] KnockBack` are real). Not changed in this round; the wording should be fixed in the coordinator's Use-path log.
5. Passive sounds and FX are not dispatched: KillingStreak (`sfx_passive_skill_activate_2`, PlayFX), Survival (`sfx_passive_skill_activate_3`, buff FX), KillingFrenzy (`activate_4` is commented out in the Lua).
6. Intimidation duration: SnS_Duration is not logged; the end is observed at frames 364-368 for a cast at 200.
7. Berserk damage numbers: no original numbers in the repo. The HP cost matches the Lua formula exactly (`HP -= HP/10` while HP% > 10).
8. Survival removal above 35 % HP and Survival damage before/after are not measured (volleys missed in the comparison).
9. Visual harness note: every run starts with a "Quest Completed" banner in the centre of the frame (saved profile state). It partly covers combat text and FX; it is not a Berserker defect.
10. Knight inherited skills were not re-run.
11. Original footage: no original Berserker gameplay video is in the repo; visual comparison to the original is NOT done (AGENTS.md note recorded here).

## Original evidence (logic)

- `prince_berseeker_berserk.luac`: RANGE 200; OnPre HP cost (`if HP% > 10: SetProp(HP, health - health/10)`), UseMana; OnSkill cycles `skill_inc` 0 (stun, `ApplyPropClass(STUN)` then `SkillCombatRoll` on every target 360 deg), 1 (damage, top target), 2 (knockback, top target). No PlaySound.
- `prince_berseeker_warcry.luac` (Intimidation): RANGE 600; per non-boss target `GetRand(1,4)`: 1,2,4 -> SetScareState(bufftime, true), 3 -> SetStunState(bufftime, true); PlaySound fear then stun after the loop.
- `prince_berseeker_killing_streak.luac`: on player kill: PlayFX, two CreateBuff (Skill_Berserk_KillingStreak capacity true, dummy with BUFF_FX), `PlaySound("sfx_passive_skill_activate_2")`; bufftime from SnS_Cooldown or 4000 + 500 x L.
- `prince_berseeker_survival.luac`: OnTargetHit with no attacker guard; HP% < 35 arms an 8000 ms buff and a flag, `sfx_passive_skill_activate_3`; HP% >= 35 removes the buff.
- `prince_berseeker_killing_frenzy.luac`: OnTargetHit with attacker == self and CanAttackInMelee; counter 1..5, rebuilt buff with SnS_Level = counter x L, 1500 ms reset, 5000 ms at max.
- `prince_berseeker_whirling_drift.luac`: RANGE via TargetListSearch, ApplyPropClass(CLASS), SkillCombatRoll per target; no PlaySound.

## Tests

- Unit: `runtime_skill_use_profile_knight_v1_tests`, `event_passive_owner_v1_tests` (sections E, F, G), as in the earlier report.
- CTest (B: build, 30f6485e): see the line below.

## Real-run cases (this round)

- `w2r3-menucap2`: menu_StartGame frames (menu capture option `--menu-capture-directory`, real clicks), PASS.
- `w2r3-menu`: Continue route with `--menu-actions`, gameplay frames only (the menu is not in the capture stream on that path).
- `w2r3-bers`, `w2r3-knock` (identical rerun: same RNG and numbers, frames 440-474 viewed).
- `w2r3-intim`, `w2r3-intim-long`, `w2r3-whirl`, `w2r3-streak`, `w2r3-surv-cast`, `w2r3-surv-ctl`, `w2r3-pass-off`, `w2r3-pass-on`.
- Batch scripts: `C:/dh218/tools/batch-w2-bers-r3.ps1`, `batch-w2-bers-r3b.ps1`, `batch-w2-bers-r3c.ps1`.
- Earlier failures (menu preview error `Selected profile class has no authored source preview actor: KnightPlayerBase_Berserker`, cases `w2-bers-a`, `w2-paladin-*`) are fixed by 721be991 and are not reproduced.

## CTest

CTest (B: build, after the merge, 30f6485e + report commit): 214/214 passed, 0 failed (`C:/dh218/w2-berserker-build/ctest.log`, total 847.9 s).

## Next steps

1. Decode `Character::_CanAttackInMelee` / `ItemInventory::CanMeleeAttack` (IDA export) before the KillingFrenzy gate.
2. Defender-side passives: melee-range and HasBow predicates for the listener (other subclasses).
3. Dispatch the passive PlayFX/PlaySound (KillingStreak, Survival) through the presentation seam.
4. Fix the stale "no host state" log wording.
5. Survival: measure damage before/after with a player hit and remove the buff above 35 % HP.
6. Listen to the wav files (not done; the resolved names are listed above).
