# ToFixed

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PORTED**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `ToFixed` | bindFunction and/or Method (see registration ledger) | `LuaScript::_ToFixed` | `LuaScript::createBindings/BindFunction` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037b5a0.c:47 |

Address-identified callback: `LuaScript::_ToFixed` at `0x0037ebc4`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ebc4.c`, unified lines 94552-94568.
Recovered declaration: `void __fastcall LuaScript::_ToFixed( LuaScript *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: Any provided Value read through getNumber; unit/scale Integer truncation BEFORE 32-bit <<8; not fractional multiplication. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ebc4.c:13`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Converts first getNumber to signed int, shifts left 8, pushes integer. There is no explicit numeric-tag gate. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ebc4.c:13`.
**Gates:**
- PROVEN: No arguments yields zero results. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ebc4.c:11`.
**State, timers and events:**
- PROVEN: Only the explicitly listed operations occur in this handler; no invented cooldown or animation transition. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ebc4.c:1`.
**Return:**
- PROVEN: One integer when an argument exists.

## Lua wrapper/helper traces

Proven source chain: `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Proven source chain: `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:253` `SetTempProps(class_id, skill_level)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/edward_rise.luac:266` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/faerie_hotty.luac:37` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/faerie_wetty.luac:38` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/faerie_windy.luac:40` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_bowmastery.luac:18` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:65` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:75` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_multihit.luac:38` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_multihit.luac:50` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:50` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_poison_arrows.luac:29` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_poison_arrows.luac:39` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_precision.luac:49` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_sidestep.luac:140` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_sidestep.luac:153` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_volley.luac:86` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_volley.luac:97` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassinate.luac:14` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:33` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:44` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blade_deflect.luac:16` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:56` `SetTempProps(BUFF_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:68` `CalcManaCost(BUFF_ID, skill_level)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:44` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:55` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:33` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:43` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:45` `SetTempProps(BUFF_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:68` `CalcManaCost(BUFF_ID, skill_level)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_treasure_hunter.luac:14` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:36` `SetTempProps(DAM_CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:47` `CalcManaCost(DAM_CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_frenzy.luac:90` `SetTempProps(BUFF_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_streak.luac:37` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_streak.luac:48` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_ravager.luac:17` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_survival.luac:57` `SetTempProps(BUFF_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_toughness.luac:14` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_warcry.luac:37` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_whirling_drift.luac:29` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_whirling_drift.luac:40` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_disciples.luac:65` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_disciples.luac:75` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_eclipse.luac:33` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_eclipse.luac:44` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_figments.luac:48` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_figments.luac:61` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_mesmerize.luac:35` `SetTempProps(BUFF_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_mirror.luac:48` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_mirror.luac:64` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_petrify.luac:58` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:194` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:205` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_timecontrol.luac:32` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_timecontrol.luac:47` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:116` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:128` `CalcManaCost(CLASS_ID, skill_level)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_coldray.luac:29` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_coldray.luac:40` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:200` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:210` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_elemental_mastery.luac:15` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_faeryblood.luac:17` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_faeryblood.luac:31` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_magi_shielding.luac:17` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_staffmaster.luac:14` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:92` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:103` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_carrioneater.luac:41` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_carrioneater.luac:52` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_deathclaw.luac:34` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_deathclaw.luac:44` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:115` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:127` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_leecher.luac:15` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:99` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:111` `CalcManaCost(CLASS_ID, skill_level)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_putrid.luac:46` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_putrid.luac:57` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_rise.luac:261` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_rise.luac:271` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_wrath.luac:37` `SetTempProps(BUFF_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_conversion.luac:156` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_divine_judgement.luac:34` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_divine_judgement.luac:45` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_energy_balance.luac:67` `SetTempProps(data.BUFF_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_guarding_aura.luac:59` `SetTempProps(BUFF_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_guarding_aura.luac:77` `CalcManaCost(BUFF_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_holy_weapon.luac:50` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_holy_weapon.luac:61` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_offensive_aura.luac:54` `SetTempProps(BUFF_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_offensive_aura.luac:72` `CalcManaCost(BUFF_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_purifying_aura.luac:99` `SetTempProps(BUFF_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_purifying_aura.luac:112` `CalcManaCost(BUFF_ID, skill_level)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_shield.luac:154` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_shield.luac:166` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_acrobat.luac:15` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_ambidextrous.luac:25` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_ambidextrous.luac:38` `CalcManaCost(CLASS_ID, skill_level)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_combomaster.luac:15` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_findweakness.luac:17` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_jump_kick.luac:34` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_jump_kick.luac:43` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_quickness.luac:52` `SetTempProps(SKILL_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_quickness.luac:62` `CalcManaCost(SKILL_ID, skill_level)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_roundhouse.luac:32` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_roundhouse.luac:44` `CalcManaCost(CLASS_ID, skill_level)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_vicious_strike.luac:41` `SetTempProps(DISP_CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_vicious_strike.luac:49` `CalcManaCost(STUN_CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_vicious_strike.luac:56` `SetTempProps(DISP_CLASS_ID, skill_level)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_bashdown.luac:36` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_bashdown.luac:46` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_charge.luac:52` `SetTempProps(CLASS_ID_SHIELD, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_charge.luac:54` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_charge.luac:66` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_combat_master.luac:16` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_fast_metabolism.luac:15` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_ground_slam.luac:40` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_ground_slam.luac:52` `CalcManaCost(CLASS_ID, skill_level)`; chain `CalcManaCost` -> `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_hardiness.luac:25` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_leadership.luac:42` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_potion_addict.luac:15` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ToFixed`.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PORTED:** Frozen reusable scalar callback implements signed conversion and wrapping shift, with identity/string services for nonnumeric values. No runtime equivalence is claimed here.
- Source evidence: `port/script-runtime/script_scalar_bindings.c`:79.

**Port comparison:** Frozen reusable scalar callback implements signed conversion and wrapping shift, with identity/string services for nonnumeric values. No runtime equivalence is claimed here.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: ToFixed(1.9) 256. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ebc4.c`:13.
2. **PROVEN:** Given: ToFixed(-1.9) -256. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ebc4.c`:13.
3. **PROVEN:** Given: ToFixed() No Lua outputs. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ebc4.c`:11.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **221** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:356` | outside scoped roots | `ToFixed(m_lvl)` | `[{"index":1,"expression":"m_lvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:204` | outside scoped roots | `ToFixed(m_lvl)` | `[{"index":1,"expression":"m_lvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:327` | outside scoped roots | `ToFixed(m_lvl)` | `[{"index":1,"expression":"m_lvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:360` | outside scoped roots | `ToFixed(m_lvl)` | `[{"index":1,"expression":"m_lvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:53` | outside scoped roots | `ToFixed(m_lvl)` | `[{"index":1,"expression":"m_lvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:273` | outside scoped roots | `ToFixed(m_lvl)` | `[{"index":1,"expression":"m_lvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:268` | outside scoped roots | `ToFixed(HPAmount)` | `[{"index":1,"expression":"HPAmount","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:279` | outside scoped roots | `ToFixed(HPAmount)` | `[{"index":1,"expression":"HPAmount","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:47` | act1_level_boss_script_closure | `ToFixed(m_lvl)` | `[{"index":1,"expression":"m_lvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/mothcocoon_core.luac:54` | outside scoped roots | `ToFixed(m_lvl)` | `[{"index":1,"expression":"m_lvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:60` | act1_level_boss_script_closure | `ToFixed(m_lvl)` | `[{"index":1,"expression":"m_lvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:104` | outside scoped roots | `ToFixed(HPMax)` | `[{"index":1,"expression":"HPMax","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:114` | outside scoped roots | `ToFixed(HPMax)` | `[{"index":1,"expression":"HPMax","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:208` | outside scoped roots | `ToFixed(m_lvl)` | `[{"index":1,"expression":"m_lvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:320` | outside scoped roots | `ToFixed(m_lvl)` | `[{"index":1,"expression":"m_lvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:347` | outside scoped roots | `ToFixed(m_lvl)` | `[{"index":1,"expression":"m_lvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:253` | outside scoped roots | `ToFixed(m_lvl)` | `[{"index":1,"expression":"m_lvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:343` | outside scoped roots | `ToFixed(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:439` | outside scoped roots | `ToFixed(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:539` | act1_level_boss_script_closure | `ToFixed(m_lvl)` | `[{"index":1,"expression":"m_lvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:289` | outside scoped roots | `ToFixed(m_lvl)` | `[{"index":1,"expression":"m_lvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:126` | act1_level_boss_script_closure | `ToFixed(m_lvl)` | `[{"index":1,"expression":"m_lvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:56` | outside scoped roots | `ToFixed(Rand(0, 100))` | `[{"index":1,"expression":"Rand(0, 100)","literal_tokens":["0","100"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:570` | outside scoped roots | `ToFixed(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:620` | outside scoped roots | `ToFixed(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:684` | outside scoped roots | `ToFixed(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:701` | outside scoped roots | `ToFixed(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:247` | autoloaded_lua_common_helpers | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:38` | outside scoped roots | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_ranged_00.luac:61` | outside scoped roots | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_slave_ranged_00.luac:61` | outside scoped roots | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/edward_ranged_00.luac:61` | outside scoped roots | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:12` | outside scoped roots | `ToFixed(10)` | `[{"index":1,"expression":"10","literal_tokens":["10"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:12` | outside scoped roots | `ToFixed(20)` | `[{"index":1,"expression":"20","literal_tokens":["20"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:13` | outside scoped roots | `ToFixed(0.2)` | `[{"index":1,"expression":"0.2","literal_tokens":["0.2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:13` | outside scoped roots | `ToFixed(0.2)` | `[{"index":1,"expression":"0.2","literal_tokens":["0.2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:14` | outside scoped roots | `ToFixed(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:14` | outside scoped roots | `ToFixed(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:259` | outside scoped roots | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_hotty.luac:12` | outside scoped roots | `ToFixed(20)` | `[{"index":1,"expression":"20","literal_tokens":["20"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_hotty.luac:12` | outside scoped roots | `ToFixed(10)` | `[{"index":1,"expression":"10","literal_tokens":["10"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_hotty.luac:13` | outside scoped roots | `ToFixed(0.2)` | `[{"index":1,"expression":"0.2","literal_tokens":["0.2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_hotty.luac:13` | outside scoped roots | `ToFixed(0.2)` | `[{"index":1,"expression":"0.2","literal_tokens":["0.2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_hotty.luac:14` | outside scoped roots | `ToFixed(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_hotty.luac:14` | outside scoped roots | `ToFixed(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_hotty.luac:93` | outside scoped roots | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_rocky.luac:12` | outside scoped roots | `ToFixed(10)` | `[{"index":1,"expression":"10","literal_tokens":["10"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_rocky.luac:12` | outside scoped roots | `ToFixed(20)` | `[{"index":1,"expression":"20","literal_tokens":["20"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_rocky.luac:13` | outside scoped roots | `ToFixed(0.2)` | `[{"index":1,"expression":"0.2","literal_tokens":["0.2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_rocky.luac:13` | outside scoped roots | `ToFixed(0.2)` | `[{"index":1,"expression":"0.2","literal_tokens":["0.2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_rocky.luac:14` | outside scoped roots | `ToFixed(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_rocky.luac:14` | outside scoped roots | `ToFixed(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_rocky.luac:87` | outside scoped roots | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_wetty.luac:12` | outside scoped roots | `ToFixed(10)` | `[{"index":1,"expression":"10","literal_tokens":["10"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_wetty.luac:12` | outside scoped roots | `ToFixed(20)` | `[{"index":1,"expression":"20","literal_tokens":["20"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_wetty.luac:13` | outside scoped roots | `ToFixed(0.2)` | `[{"index":1,"expression":"0.2","literal_tokens":["0.2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_wetty.luac:13` | outside scoped roots | `ToFixed(0.2)` | `[{"index":1,"expression":"0.2","literal_tokens":["0.2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_wetty.luac:14` | outside scoped roots | `ToFixed(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_wetty.luac:14` | outside scoped roots | `ToFixed(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_wetty.luac:94` | outside scoped roots | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_windy.luac:12` | outside scoped roots | `ToFixed(20)` | `[{"index":1,"expression":"20","literal_tokens":["20"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_windy.luac:12` | outside scoped roots | `ToFixed(10)` | `[{"index":1,"expression":"10","literal_tokens":["10"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_windy.luac:13` | outside scoped roots | `ToFixed(0.2)` | `[{"index":1,"expression":"0.2","literal_tokens":["0.2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_windy.luac:13` | outside scoped roots | `ToFixed(0.2)` | `[{"index":1,"expression":"0.2","literal_tokens":["0.2"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_windy.luac:14` | outside scoped roots | `ToFixed(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_windy.luac:14` | outside scoped roots | `ToFixed(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_windy.luac:87` | outside scoped roots | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruk_fastenergybolt.luac:61` | outside scoped roots | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slowenergybolt.luac:65` | outside scoped roots | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_healerspell.luac:75` | outside scoped roots | `ToFixed(targethealingAmount)` | `[{"index":1,"expression":"targethealingAmount","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:74` | outside scoped roots | `ToFixed(final_hp)` | `[{"index":1,"expression":"final_hp","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_arrow_default.luac:61` | outside scoped roots | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_fireball.luac:61` | outside scoped roots | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_poisonball.luac:61` | outside scoped roots | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_bowmastery.luac:27` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:40` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:74` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:124` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_multihit.luac:16` | player_skill_lua | `ToFixed(10)` | `[{"index":1,"expression":"10","literal_tokens":["10"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_multihit.luac:17` | player_skill_lua | `ToFixed(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_multihit.luac:18` | player_skill_lua | `ToFixed(3)` | `[{"index":1,"expression":"3","literal_tokens":["3"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_multihit.luac:49` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_multihit.luac:98` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:62` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:69` | player_skill_lua | `ToFixed(pinned_ennemies)` | `[{"index":1,"expression":"pinned_ennemies","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:166` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_poison_arrows.luac:38` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_poison_arrows.luac:80` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_precision.luac:59` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_volley.luac:96` | player_skill_lua | `ToFixed(slvl)` | `[{"index":1,"expression":"slvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_volley.luac:168` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassinate.luac:23` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:43` | player_skill_lua | `ToFixed(slvl)` | `[{"index":1,"expression":"slvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:97` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:104` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:109` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blade_deflect.luac:26` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:13` | player_skill_lua | `ToFixed(10)` | `[{"index":1,"expression":"10","literal_tokens":["10"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:14` | player_skill_lua | `ToFixed(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:15` | player_skill_lua | `ToFixed(3)` | `[{"index":1,"expression":"3","literal_tokens":["3"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:67` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:54` | player_skill_lua | `ToFixed(slvl)` | `[{"index":1,"expression":"slvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:42` | player_skill_lua | `ToFixed(slvl)` | `[{"index":1,"expression":"slvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:83` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:13` | player_skill_lua | `ToFixed(10)` | `[{"index":1,"expression":"10","literal_tokens":["10"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:14` | player_skill_lua | `ToFixed(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:15` | player_skill_lua | `ToFixed(3)` | `[{"index":1,"expression":"3","literal_tokens":["3"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:67` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_treasure_hunter.luac:24` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:46` | player_skill_lua | `ToFixed(slvl)` | `[{"index":1,"expression":"slvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:81` | player_skill_lua | `ToFixed(health-hp_leech)` | `[{"index":1,"expression":"health-hp_leech","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:112` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:122` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:131` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_frenzy.luac:48` | player_skill_lua | `ToFixed(CURR_COUNTER*skill_level)` | `[{"index":1,"expression":"CURR_COUNTER*skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_frenzy.luac:60` | player_skill_lua | `ToFixed(CURR_COUNTER*skill_level)` | `[{"index":1,"expression":"CURR_COUNTER*skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_frenzy.luac:74` | player_skill_lua | `ToFixed(CURR_COUNTER*skill_level)` | `[{"index":1,"expression":"CURR_COUNTER*skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_streak.luac:50` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_ravager.luac:29` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_ravager.luac:41` | player_skill_lua | `ToFixed(slvl)` | `[{"index":1,"expression":"slvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_survival.luac:35` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_survival.luac:68` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_toughness.luac:24` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_warcry.luac:49` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_whirling_drift.luac:39` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_whirling_drift.luac:62` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_disciples.luac:74` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_eclipse.luac:43` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_eclipse.luac:76` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_figments.luac:60` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_mesmerize.luac:46` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_mirror.luac:59` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_petrify.luac:42` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_petrify.luac:68` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:15` | player_skill_lua | `ToFixed(15)` | `[{"index":1,"expression":"15","literal_tokens":["15"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:16` | player_skill_lua | `ToFixed(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:17` | player_skill_lua | `ToFixed(3)` | `[{"index":1,"expression":"3","literal_tokens":["3"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:204` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_timecontrol.luac:46` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_timecontrol.luac:85` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:126` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:170` | player_skill_lua | `ToFixed(MIN_DAMAGE)` | `[{"index":1,"expression":"MIN_DAMAGE","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:171` | player_skill_lua | `ToFixed(MAX_DAMAGE)` | `[{"index":1,"expression":"MAX_DAMAGE","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:234` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:242` | player_skill_lua | `ToFixed(minDamage * (1 - ((victim_count - 1) * penalty)))` | `[{"index":1,"expression":"minDamage * (1 - ((victim_count - 1) * penalty))","literal_tokens":["1","1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:243` | player_skill_lua | `ToFixed(maxDamage * (1 - ((victim_count - 1) * penalty)))` | `[{"index":1,"expression":"maxDamage * (1 - ((victim_count - 1) * penalty))","literal_tokens":["1","1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_coldray.luac:39` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_coldray.luac:75` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:74` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:114` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_elemental_mastery.luac:25` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_elemental_mastery.luac:31` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_faeryblood.luac:27` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_magi_shielding.luac:27` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_staffmaster.luac:25` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:59` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:102` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:162` | player_skill_lua | `ToFixed(plvl)` | `[{"index":1,"expression":"plvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_carrioneater.luac:20` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_carrioneater.luac:27` | player_skill_lua | `ToFixed(max_hp)` | `[{"index":1,"expression":"max_hp","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_carrioneater.luac:51` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_deathclaw.luac:43` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_deathclaw.luac:80` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:12` | player_skill_lua | `ToFixed(6)` | `[{"index":1,"expression":"6","literal_tokens":["6"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:13` | player_skill_lua | `ToFixed(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:14` | player_skill_lua | `ToFixed(1)` | `[{"index":1,"expression":"1","literal_tokens":["1"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:90` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:126` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_leecher.luac:25` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:52` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:110` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:155` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_putrid.luac:56` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_rise.luac:270` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_wrath.luac:48` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_conversion.luac:166` | player_skill_lua | `ToFixed(slvl)` | `[{"index":1,"expression":"slvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_divine_judgement.luac:44` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_divine_judgement.luac:84` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_energy_balance.luac:86` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_guarding_aura.luac:14` | player_skill_lua | `ToFixed(10)` | `[{"index":1,"expression":"10","literal_tokens":["10"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_guarding_aura.luac:15` | player_skill_lua | `ToFixed(0)` | `[{"index":1,"expression":"0","literal_tokens":["0"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_guarding_aura.luac:16` | player_skill_lua | `ToFixed(3)` | `[{"index":1,"expression":"3","literal_tokens":["3"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_guarding_aura.luac:68` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_guarding_aura.luac:73` | player_skill_lua | `ToFixed(slvl)` | `[{"index":1,"expression":"slvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_guarding_aura.luac:76` | player_skill_lua | `ToFixed(slvl)` | `[{"index":1,"expression":"slvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_holy_weapon.luac:19` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_holy_weapon.luac:37` | player_skill_lua | `ToFixed(hp)` | `[{"index":1,"expression":"hp","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_holy_weapon.luac:60` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_offensive_aura.luac:63` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_offensive_aura.luac:68` | player_skill_lua | `ToFixed(slvl)` | `[{"index":1,"expression":"slvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_offensive_aura.luac:71` | player_skill_lua | `ToFixed(slvl)` | `[{"index":1,"expression":"slvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_purifying_aura.luac:46` | player_skill_lua | `ToFixed(maxHealth)` | `[{"index":1,"expression":"maxHealth","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_purifying_aura.luac:48` | player_skill_lua | `ToFixed(HP_Max)` | `[{"index":1,"expression":"HP_Max","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_purifying_aura.luac:111` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_purifying_aura.luac:119` | player_skill_lua | `ToFixed(slvl)` | `[{"index":1,"expression":"slvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_shield.luac:165` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_acrobat.luac:25` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_ambidextrous.luac:37` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_combomaster.luac:25` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_findweakness.luac:27` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_jump_kick.luac:42` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_jump_kick.luac:75` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_quickness.luac:61` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_quickness.luac:89` | player_skill_lua | `ToFixed(slvl)` | `[{"index":1,"expression":"slvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_quickness.luac:90` | player_skill_lua | `ToFixed(slvl)` | `[{"index":1,"expression":"slvl","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_roundhouse.luac:43` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_roundhouse.luac:73` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_vicious_strike.luac:51` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_vicious_strike.luac:87` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_bashdown.luac:42` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_bashdown.luac:82` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_charge.luac:65` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_combat_master.luac:26` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_fast_metabolism.luac:25` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_ground_slam.luac:50` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_ground_slam.luac:83` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_hardiness.luac:36` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_leadership.luac:61` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_potion_addict.luac:25` | player_skill_lua | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/resgod_dying_attack.luac:47` | outside scoped roots | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/witch_ranged_00.luac:63` | act1_level_boss_script_closure | `ToFixed(skill_level)` | `[{"index":1,"expression":"skill_level","literal_tokens":[]}]` |

## Related natives

`LuaScript::_ToFixed`, `ToFixed`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
