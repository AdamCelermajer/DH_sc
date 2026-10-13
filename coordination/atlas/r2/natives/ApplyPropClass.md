# ApplyPropClass

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#12**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `ApplyPropClass` | bindFunction | `Character::_ApplyPropClass` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132852 |
| `ApplyPropClass` | bindMethod | `Character::_ApplyPropClass` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132857 |

Address-identified callback: `Character::_ApplyPropClass` at `0x003babdc`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c`, unified lines 135973-136048.
Recovered declaration: `unsigned int __fastcall Character::_ApplyPropClass( unsigned int this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: getter not adjacent in decompilation; handler lines 136003, 136016, 136039.

## Recovered behavior, gates, state, timers, events and return

- **PROVEN:** Character::createBindings 0x3b56bc explicitly registers ApplyBuff both as global bound function and object method to Character::_ApplyPropClass 0x3babdc. The atlas alias is correct; this is not a guessed similarly named routine. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56bc.c:641`.
- **PROVEN:** A numeric class first argument and nonnull pointer second argument select PROPS_ApplyClassToSheet on the bound receiver. That routine invokes _LoadClass(sheet,class,true), then _ApplyPropClass recalculates receiver properties. A boolean second argument selects PROPS_ApplyClass instead; omitted second argument uses the nonbuff false branch. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c:71`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003df314.c:7`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c:72`.
- **PROVEN:** The source wrapper allows class index equal to size past its `>` gate, but _LoadClass subsequently admits only indices strictly less than size. The supplied pointer is a native sheet; the wrapper does not prove ownership or lifetime beyond its null check. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c:33`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2e20.c:9`.

## Lua wrapper/helper traces

Proven source chain: `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Proven source chain: `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:253` `SetTempProps(class_id, skill_level)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/edward_rise.luac:266` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/faerie_hotty.luac:37` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/faerie_wetty.luac:38` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/faerie_windy.luac:40` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_bowmastery.luac:18` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:65` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:75` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_multihit.luac:38` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_multihit.luac:50` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:50` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_poison_arrows.luac:29` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_poison_arrows.luac:39` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_precision.luac:49` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_sidestep.luac:140` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_sidestep.luac:153` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_volley.luac:86` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_volley.luac:97` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassinate.luac:14` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:33` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:44` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blade_deflect.luac:16` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:56` `SetTempProps(BUFF_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:68` `CalcManaCost(BUFF_ID, skill_level)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:44` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:55` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:33` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:43` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:45` `SetTempProps(BUFF_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:68` `CalcManaCost(BUFF_ID, skill_level)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_treasure_hunter.luac:14` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:36` `SetTempProps(DAM_CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:47` `CalcManaCost(DAM_CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_frenzy.luac:90` `SetTempProps(BUFF_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_streak.luac:37` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_streak.luac:48` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_ravager.luac:17` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_survival.luac:57` `SetTempProps(BUFF_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_toughness.luac:14` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_warcry.luac:37` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_whirling_drift.luac:29` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_whirling_drift.luac:40` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_disciples.luac:65` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_disciples.luac:75` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_eclipse.luac:33` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_eclipse.luac:44` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_figments.luac:48` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_figments.luac:61` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_mesmerize.luac:35` `SetTempProps(BUFF_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_mirror.luac:48` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_mirror.luac:64` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_petrify.luac:58` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:194` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:205` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_timecontrol.luac:32` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_timecontrol.luac:47` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:116` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:128` `CalcManaCost(CLASS_ID, skill_level)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_coldray.luac:29` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_coldray.luac:40` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:200` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:210` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_elemental_mastery.luac:15` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_faeryblood.luac:17` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_faeryblood.luac:31` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_magi_shielding.luac:17` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_staffmaster.luac:14` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:92` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:103` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_carrioneater.luac:41` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_carrioneater.luac:52` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_deathclaw.luac:34` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_deathclaw.luac:44` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:115` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:127` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_leecher.luac:15` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:99` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:111` `CalcManaCost(CLASS_ID, skill_level)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_putrid.luac:46` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_putrid.luac:57` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_rise.luac:261` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_rise.luac:271` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_wrath.luac:37` `SetTempProps(BUFF_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_conversion.luac:156` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_divine_judgement.luac:34` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_divine_judgement.luac:45` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_energy_balance.luac:67` `SetTempProps(data.BUFF_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_guarding_aura.luac:59` `SetTempProps(BUFF_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_guarding_aura.luac:77` `CalcManaCost(BUFF_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_holy_weapon.luac:50` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_holy_weapon.luac:61` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_offensive_aura.luac:54` `SetTempProps(BUFF_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_offensive_aura.luac:72` `CalcManaCost(BUFF_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_purifying_aura.luac:99` `SetTempProps(BUFF_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_purifying_aura.luac:112` `CalcManaCost(BUFF_ID, skill_level)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_shield.luac:154` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_shield.luac:166` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_acrobat.luac:15` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_ambidextrous.luac:25` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_ambidextrous.luac:38` `CalcManaCost(CLASS_ID, skill_level)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_combomaster.luac:15` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_findweakness.luac:17` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_jump_kick.luac:34` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_jump_kick.luac:43` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_quickness.luac:52` `SetTempProps(SKILL_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_quickness.luac:62` `CalcManaCost(SKILL_ID, skill_level)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_roundhouse.luac:32` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_roundhouse.luac:44` `CalcManaCost(CLASS_ID, skill_level)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_vicious_strike.luac:41` `SetTempProps(DISP_CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_vicious_strike.luac:49` `CalcManaCost(STUN_CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_vicious_strike.luac:56` `SetTempProps(DISP_CLASS_ID, skill_level)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_bashdown.luac:36` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_bashdown.luac:46` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_charge.luac:52` `SetTempProps(CLASS_ID_SHIELD, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_charge.luac:54` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_charge.luac:66` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_combat_master.luac:16` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_fast_metabolism.luac:15` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_ground_slam.luac:40` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_ground_slam.luac:52` `CalcManaCost(CLASS_ID, skill_level)`; chain `CalcManaCost` -> `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_hardiness.luac:25` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_leadership.luac:42` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_potion_addict.luac:15` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ApplyPropClass`.

## Source-backed behavior cases

### ApplyBuff binding and pointer branch

- **PROVEN:** Character::createBindings 0x3b56bc explicitly registers ApplyBuff both as global bound function and object method to Character::_ApplyPropClass 0x3babdc. The atlas alias is correct; this is not a guessed similarly named routine. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56bc.c:641`.
- **PROVEN:** A numeric class first argument and nonnull pointer second argument select PROPS_ApplyClassToSheet on the bound receiver. That routine invokes _LoadClass(sheet,class,true), then _ApplyPropClass recalculates receiver properties. A boolean second argument selects PROPS_ApplyClass instead; omitted second argument uses the nonbuff false branch. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c:71`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003df314.c:7`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c:72`.
- **PROVEN:** The source wrapper allows class index equal to size past its `>` gate, but _LoadClass subsequently admits only indices strictly less than size. The supplied pointer is a native sheet; the wrapper does not prove ownership or lifetime beyond its null check. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c:33`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2e20.c:9`.


## Current frozen port source

**PARTIAL:** Source-backed class evaluation is wired for skill owners; generic script-owner/class-table admission remains context-dependent.
- Source evidence: `port/windows-foundation/features/bosses/runtime_swampking_core_ai_v1.cpp`:331 — `!contains(delayed, "TargetListSearch(AOE_RANGE, 360") || !contains(delayed, "ApplyPropClass(AOE_CLASS_ID)"))`.
- Source evidence: `port/windows-foundation/features/enemy_ai/runtime_ranged_skill_monster_v1.cpp`:273 — `next.delayed_combat_roll = contains(delayed, "ApplyPropClass(CLASS_ID)") && contains(delayed, "SkillCombatRoll(target)") &&`.
- Source evidence: `port/windows-foundation/features/faery_menu/celest_cast_v1.cpp`:143 — `if (error.empty()) error = "Original Celest ApplyPropClass projection failed";`.
- Source evidence: `port/windows-foundation/features/faery_menu/celest_source_use_v1.cpp`:134 — `if (error.empty()) error = "Celest source ApplyPropClass failed before SpellCombatRoll";`.
- Source evidence: `port/windows-foundation/features/faery_menu/hotty_cast_v1.cpp`:178 — `if (error.empty()) error = "Source Hotty ApplyPropClass projection failed";`.
- Source evidence: `port/windows-foundation/features/faery_menu/hotty_character_cast_v1.cpp`:116 — `if (error.empty()) error = "Hotty source ApplyPropClass projection failed before SpellCombatRoll";`.
- Source evidence: `port/windows-foundation/features/generic_skills/runtime_mage_skill_owner_v1.cpp`:191 — `return fail(error, "Mage skill Use requires its profile and ApplyPropClass formula sheet");`.
- Source evidence: `port/windows-foundation/features/generic_skills/runtime_skill_cast_coordinator_v1.cpp`:1216 — `active.receipt.detail = "Source ApplyPropClass failed after committed OnPre prefix";`.
- Source evidence: `port/level-world/character_skill_properties_v1.cpp`:53 — `return dh2_script_vm_bind_source_values(vm,"ClearProps",skill_clear_properties_v1,b)||dh2_script_vm_bind_source_values(vm,"SetProp",skill_set_property_v1,b)||dh2_script_vm_bind_source_values(vm,"ApplyPropClass",skill_apply_class_v1,b)?-1:0;`.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: ApplyBuff(validClass, liveBuffSheetPointer). Class rows execute on that sheet in buff mode, then receiver properties recalculate. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56bc.c`:641, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c`:71, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003df314.c`:7, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c`:72, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c`:33, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2e20.c`:9.
2. **PROVEN:** Given: ApplyBuff(validClass, nullPointer). Pointer branch performs no class application or recalculation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56bc.c`:641, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c`:71, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003df314.c`:7, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c`:72, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c`:33, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2e20.c`:9.
3. **PROVEN:** Given: ApplyPropClass(validClass, true). Boolean branch applies the class through PROPS_ApplyClass, not the pointer-sheet buff branch. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56bc.c`:641, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c`:71, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003df314.c`:7, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c`:72, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003babdc.c`:33, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e2e20.c`:9.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **85** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:248` | autoloaded_lua_common_helpers | `ApplyPropClass(class_id, true)` | `[{"index":1,"expression":"class_id","literal_tokens":[]},{"index":2,"expression":"true","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:39` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_melee_00.luac:56` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_ranged_00.luac:62` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_slave_ranged_00.luac:62` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/edward_melee_00.luac:56` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/edward_melee_01.luac:57` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/edward_ranged_00.luac:62` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:260` | outside scoped roots | `ApplyPropClass(CLASS_ID + skill_level)` | `[{"index":1,"expression":"CLASS_ID + skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_hotty.luac:94` | outside scoped roots | `ApplyPropClass(CLASS_ID + skill_level)` | `[{"index":1,"expression":"CLASS_ID + skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_rocky.luac:88` | outside scoped roots | `ApplyPropClass(CLASS_ID + skill_level)` | `[{"index":1,"expression":"CLASS_ID + skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_wetty.luac:95` | outside scoped roots | `ApplyPropClass(CLASS_ID + skill_level)` | `[{"index":1,"expression":"CLASS_ID + skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_windy.luac:88` | outside scoped roots | `ApplyPropClass(CLASS_ID + skill_level)` | `[{"index":1,"expression":"CLASS_ID + skill_level","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_slam.luac:58` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:23` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_melee_00.luac:60` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:36` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruk_aoeknockback.luac:46` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruk_fastenergybolt.luac:62` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slam.luac:56` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slowenergybolt.luac:66` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_bite.luac:55` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:59` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_arrow_default.luac:62` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_energybolt.luac:75` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_fireball.luac:62` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_poisonball.luac:62` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:41` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:125` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_multihit.luac:99` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:63` | player_skill_lua | `ApplyPropClass(CLASS_ID, true)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"true","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:167` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_poison_arrows.luac:81` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_volley.luac:169` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:98` | player_skill_lua | `ApplyPropClass(BOSS_CLASS_ID)` | `[{"index":1,"expression":"BOSS_CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:105` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:110` | player_skill_lua | `ApplyPropClass(BOSS_CLASS_ID)` | `[{"index":1,"expression":"BOSS_CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:84` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:113` | player_skill_lua | `ApplyPropClass(STUN_CLASS_ID)` | `[{"index":1,"expression":"STUN_CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:123` | player_skill_lua | `ApplyPropClass(DAM_CLASS_ID)` | `[{"index":1,"expression":"DAM_CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:132` | player_skill_lua | `ApplyPropClass(KB_CLASS_ID)` | `[{"index":1,"expression":"KB_CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_warcry.luac:50` | player_skill_lua | `ApplyPropClass(CLASS_ID, true)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]},{"index":2,"expression":"true","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_whirling_drift.luac:63` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_figments.luac:91` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:117` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_timecontrol.luac:86` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:235` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_coldray.luac:76` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:75` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:115` | player_skill_lua | `ApplyPropClass(AREA_CLASS_ID)` | `[{"index":1,"expression":"AREA_CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:60` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_carrioneater.luac:21` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_deathclaw.luac:81` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:91` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:53` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:156` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_conversion.luac:167` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_divine_judgement.luac:85` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_jump_kick.luac:76` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_roundhouse.luac:74` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_vicious_strike.luac:90` | player_skill_lua | `ApplyPropClass(STUN_CLASS_ID)` | `[{"index":1,"expression":"STUN_CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_bashdown.luac:83` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_charge.luac:99` | player_skill_lua | `ApplyPropClass(CLASS_ID_SHIELD)` | `[{"index":1,"expression":"CLASS_ID_SHIELD","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_charge.luac:101` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_ground_slam.luac:84` | player_skill_lua | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/resgod_dying_attack.luac:48` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/root_troll_attack.luac:68` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/root_troll_attack_noskill.luac:57` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/root_troll_charge.luac:57` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_emerge.luac:53` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:81` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:101` | outside scoped roots | `ApplyPropClass(AOE_CLASS_ID)` | `[{"index":1,"expression":"AOE_CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_1.luac:60` | act1_level_boss_script_closure | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_2.luac:60` | act1_level_boss_script_closure | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_3.luac:51` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_4.luac:53` | act1_level_boss_script_closure | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swampking_atk_spit.luac:81` | act1_level_boss_script_closure | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swampking_atk_spit.luac:101` | act1_level_boss_script_closure | `ApplyPropClass(AOE_CLASS_ID)` | `[{"index":1,"expression":"AOE_CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_bite.luac:56` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:247` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:188` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:178` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/witch_ranged_00.luac:64` | act1_level_boss_script_closure | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/worm_attack_1.luac:60` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/worm_attack_2.luac:60` | outside scoped roots | `ApplyPropClass(CLASS_ID)` | `[{"index":1,"expression":"CLASS_ID","literal_tokens":[]}]` |

## Related natives

`ApplyPropClass`, `Character::_ApplyPropClass`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
