# GetCurrentSkillInfo__

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#159**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `GetCurrentSkillInfo__` | bindFunction | `Character::_GetCurrentSkillInfo` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132559 |
| `GetCurrentSkillInfo__` | bindMethod | `Character::_GetCurrentSkillInfo` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132564 |

Address-identified callback: `Character::_GetCurrentSkillInfo` at `0x003b8f9c`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`, unified lines 134807-134843.
Recovered declaration: `void __fastcall Character::_GetCurrentSkillInfo( Character *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: number -> int index; inverted numeric bounds gate preserved; unit/scale receiver skill index. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:20`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Requires argument1; numeric tag3 bypasses wrapper range check; nonnumeric converted index is range checked. Calls GetCharSkill (discarded) then SG_GetSkillLevel(save,index), which returns-1 without save. Pushes level integer, not skill table/metadata. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:32`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:33`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bbed0.c:5`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:32`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:33`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bbed0.c:5`.
**State, timers and events:**
- PROVEN: Read-only callback; starts no timers and raises no events. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:32`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:33`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bbed0.c:5`.
**Return:**
- PROVEN: One Lua value as described; no state mutation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:32`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c:33`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bbed0.c:5`.

## Lua wrapper/helper traces

Proven source chain: `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/npc_dummy.luac:81` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_bowmastery.luac:23` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:70` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_multihit.luac:43` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:56` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_poison_arrows.luac:34` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_precision.luac:54` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_sidestep.luac:146` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_volley.luac:91` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassinate.luac:19` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:39` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blade_deflect.luac:21` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:62` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:49` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:38` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:62` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_treasure_hunter.luac:19` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:41` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_frenzy.luac:95` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_streak.luac:43` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_ravager.luac:22` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_survival.luac:62` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_toughness.luac:19` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_warcry.luac:43` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_whirling_drift.luac:34` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_disciples.luac:70` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_eclipse.luac:39` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_figments.luac:55` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_mesmerize.luac:40` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_mirror.luac:54` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_petrify.luac:63` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:199` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_timecontrol.luac:41` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:121` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_coldray.luac:34` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_coldray.luac:57` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:205` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_elemental_mastery.luac:20` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_faeryblood.luac:22` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_magi_shielding.luac:22` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_staffmaster.luac:19` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:97` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_carrioneater.luac:46` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_deathclaw.luac:39` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:121` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_leecher.luac:20` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:104` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_putrid.luac:51` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_rise.luac:266` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_wrath.luac:42` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_conversion.luac:161` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_divine_judgement.luac:40` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_energy_balance.luac:57` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_energy_balance.luac:73` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_guarding_aura.luac:64` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_holy_weapon.luac:55` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_offensive_aura.luac:59` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_offensive_aura.luac:112` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_purifying_aura.luac:105` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_shield.luac:160` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_acrobat.luac:20` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_ambidextrous.luac:30` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_combomaster.luac:20` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_findweakness.luac:22` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_jump_kick.luac:39` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_quickness.luac:57` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_quickness.luac:87` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_roundhouse.luac:37` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_vicious_strike.luac:46` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_bashdown.luac:41` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_charge.luac:60` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_combat_master.luac:21` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_fast_metabolism.luac:20` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_ground_slam.luac:45` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_hardiness.luac:30` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_leadership.luac:47` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_potion_addict.luac:20` `GetCurrentSkillInfo()`; chain `GetCurrentSkillInfo` -> `GetCurrentSkillInfo__`.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** character_script_session_v3.cpp:264 binds current_skill_info_v2; character_current_skill_v2.cpp:10-22 validates selected skill/list indices and returns required-service failure outside supported original assertion/index domain. Normal saved-level/no-save result is supported, while arbitrary scene object-method0x3b8f9c dispatch is absent (character_script_objects.cpp:144-146).

**Concrete behavioral difference:** character_script_session_v3.cpp:264 binds current_skill_info_v2; character_current_skill_v2.cpp:10-22 validates selected skill/list indices and returns required-service failure outside supported original assertion/index domain. Normal saved-level/no-save result is supported, while arbitrary scene object-method0x3b8f9c dispatch is absent (character_script_objects.cpp:144-146).

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: GetCurrentSkillInfo__() No Lua value. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:16, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:18, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:20, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:21, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:25, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:31, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:32, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:33, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bbed0.c`:5.
2. **PROVEN:** Given: valid skill index0 and no save; GetCurrentSkillInfo__(0) Returns-1 after discarded skill lookup. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:16, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:18, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:20, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:21, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:25, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:31, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:32, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:33, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bbed0.c`:5.
3. **PROVEN:** Given: valid skill index0 save level3; GetCurrentSkillInfo__(0) Returns3. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:16, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:18, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:20, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:21, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:25, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:31, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:32, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8f9c.c`:33, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003bbed0.c`:5.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **1** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:173` | autoloaded_lua_common_helpers | `GetCurrentSkillInfo__(SKILL_ID)` | `[{"index":1,"expression":"SKILL_ID","literal_tokens":[]}]` |

## Related natives

`Character::_GetCurrentSkillInfo`, `GetCurrentSkillInfo__`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
