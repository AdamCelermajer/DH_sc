# ClearProps

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#105**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `ClearProps` | bindFunction | `Character::_ClearProps` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132861 |
| `ClearProps` | bindMethod | `Character::_ClearProps` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132866 |

Address-identified callback: `Character::_ClearProps` at `0x003b82e4`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`, unified lines 134261-134318.
Recovered declaration: `int __fastcall Character::_ClearProps( int this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: pointer tag2 or Booleantrue; unit/scale sheet identity or shared temp selector. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:23`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Requires arg1; nonnull pointer tag2 resets provided property sheet then RecalcProperties(receiver,1). Booleantrue resets shared temporary sheet via PROPS_ResetSheet(null) without recalc. Pointer null, Booleanfalse, other type/missing no-op. ResetSheet null selects CharProperties::s_temp. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:24`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:28`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:30`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:33`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:34`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:37`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:41`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003def84.c:3`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:24`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:28`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:30`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:33`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:34`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:37`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:41`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003def84.c:3`.
**State, timers and events:**
- PROVEN: No Lua result; sheet reset/recalc conditional, no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:24`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:28`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:30`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:33`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:34`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:37`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:41`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003def84.c:3`.
**Return:**
- PROVEN: No Lua result; sheet reset/recalc conditional, no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:24`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:28`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:30`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:33`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:34`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:37`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c:41`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003def84.c:3`.

## Lua wrapper/helper traces

Proven source chain: `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Proven source chain: `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:253` `SetTempProps(class_id, skill_level)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/edward_rise.luac:266` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/faerie_hotty.luac:37` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/faerie_wetty.luac:38` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/faerie_windy.luac:40` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_bowmastery.luac:18` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:65` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:75` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_multihit.luac:38` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_multihit.luac:50` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:50` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_poison_arrows.luac:29` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_poison_arrows.luac:39` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_precision.luac:49` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_sidestep.luac:140` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_sidestep.luac:153` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_volley.luac:86` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_volley.luac:97` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassinate.luac:14` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:33` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:44` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blade_deflect.luac:16` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:56` `SetTempProps(BUFF_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:68` `CalcManaCost(BUFF_ID, skill_level)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:44` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:55` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:33` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:43` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:45` `SetTempProps(BUFF_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:68` `CalcManaCost(BUFF_ID, skill_level)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_treasure_hunter.luac:14` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:36` `SetTempProps(DAM_CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:47` `CalcManaCost(DAM_CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_frenzy.luac:90` `SetTempProps(BUFF_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_streak.luac:37` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_streak.luac:48` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_ravager.luac:17` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_survival.luac:57` `SetTempProps(BUFF_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_toughness.luac:14` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_warcry.luac:37` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_whirling_drift.luac:29` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_whirling_drift.luac:40` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_disciples.luac:65` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_disciples.luac:75` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_eclipse.luac:33` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_eclipse.luac:44` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_figments.luac:48` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_figments.luac:61` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_mesmerize.luac:35` `SetTempProps(BUFF_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_mirror.luac:48` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_mirror.luac:64` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_petrify.luac:58` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:194` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:205` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_timecontrol.luac:32` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_timecontrol.luac:47` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:116` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:128` `CalcManaCost(CLASS_ID, skill_level)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_coldray.luac:29` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_coldray.luac:40` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:200` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:210` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_elemental_mastery.luac:15` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_faeryblood.luac:17` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_faeryblood.luac:31` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_magi_shielding.luac:17` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_staffmaster.luac:14` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:92` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:103` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_carrioneater.luac:41` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_carrioneater.luac:52` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_deathclaw.luac:34` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_deathclaw.luac:44` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:115` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:127` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_leecher.luac:15` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:99` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:111` `CalcManaCost(CLASS_ID, skill_level)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_putrid.luac:46` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_putrid.luac:57` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_rise.luac:261` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_rise.luac:271` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_wrath.luac:37` `SetTempProps(BUFF_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_conversion.luac:156` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_divine_judgement.luac:34` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_divine_judgement.luac:45` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_energy_balance.luac:67` `SetTempProps(data.BUFF_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_guarding_aura.luac:59` `SetTempProps(BUFF_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_guarding_aura.luac:77` `CalcManaCost(BUFF_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_holy_weapon.luac:50` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_holy_weapon.luac:61` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_offensive_aura.luac:54` `SetTempProps(BUFF_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_offensive_aura.luac:72` `CalcManaCost(BUFF_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_purifying_aura.luac:99` `SetTempProps(BUFF_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_purifying_aura.luac:112` `CalcManaCost(BUFF_ID, skill_level)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_shield.luac:154` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_shield.luac:166` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_acrobat.luac:15` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_ambidextrous.luac:25` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_ambidextrous.luac:38` `CalcManaCost(CLASS_ID, skill_level)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_combomaster.luac:15` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_findweakness.luac:17` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_jump_kick.luac:34` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_jump_kick.luac:43` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_quickness.luac:52` `SetTempProps(SKILL_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_quickness.luac:62` `CalcManaCost(SKILL_ID, skill_level)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_roundhouse.luac:32` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_roundhouse.luac:44` `CalcManaCost(CLASS_ID, skill_level)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_vicious_strike.luac:41` `SetTempProps(DISP_CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_vicious_strike.luac:49` `CalcManaCost(STUN_CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_vicious_strike.luac:56` `SetTempProps(DISP_CLASS_ID, skill_level)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_bashdown.luac:36` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_bashdown.luac:46` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_charge.luac:52` `SetTempProps(CLASS_ID_SHIELD, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_charge.luac:54` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_charge.luac:66` `CalcManaCost(CLASS_ID, slvl)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_combat_master.luac:16` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_fast_metabolism.luac:15` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_ground_slam.luac:40` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_ground_slam.luac:52` `CalcManaCost(CLASS_ID, skill_level)`; chain `CalcManaCost` -> `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_hardiness.luac:25` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_leadership.luac:42` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_potion_addict.luac:15` `SetTempProps(CLASS_ID, slvl)`; chain `SetTempProps` -> `ClearProps`.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** Selected skill/buff services own property sheets but general ClearProps Lua callback0x3b82e4 absent in CharacterScriptObjects (:144-146). Boolean temporary reset and supplied-sheet reset/recalc must remain distinct.
- Source evidence: `port/level-world/character_skill_properties_v1.cpp`:53 — `return dh2_script_vm_bind_source_values(vm,"ClearProps",skill_clear_properties_v1,b)||dh2_script_vm_bind_source_values(vm,"SetProp",skill_set_property_v1,b)||dh2_script_vm_bind_source_values(vm,"ApplyPropClass",skill_apply_class_v1,b)?-1:0;`.

**Concrete behavioral difference:** Selected skill/buff services own property sheets but general ClearProps Lua callback0x3b82e4 absent in CharacterScriptObjects (:144-146). Boolean temporary reset and supplied-sheet reset/recalc must remain distinct.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: ClearProps(true) Resets s_temp; no RecalcProperties call. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:21, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:24, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:28, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:30, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:33, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:34, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:37, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:41, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003def84.c`:3.
2. **PROVEN:** Given: ClearProps(false) No reset/recalc. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:21, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:24, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:28, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:30, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:33, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:34, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:37, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:41, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003def84.c`:3.
3. **PROVEN:** Given: ClearProps(nonnull sheet P) Resets sheet P then recalculates bound receiver. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:21, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:24, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:28, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:30, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:33, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:34, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:37, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b82e4.c`:41, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003def84.c`:3.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **6** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:246` | autoloaded_lua_common_helpers | `ClearProps(true)` | `[{"index":1,"expression":"true","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:61` | player_skill_lua | `ClearProps(true)` | `[{"index":1,"expression":"true","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:117` | player_skill_lua | `ClearProps()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:126` | player_skill_lua | `ClearProps()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:135` | player_skill_lua | `ClearProps()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_warcry.luac:48` | player_skill_lua | `ClearProps(true)` | `[{"index":1,"expression":"true","literal_tokens":[]}]` |

## Related natives

`Character::_ClearProps`, `ClearProps`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
