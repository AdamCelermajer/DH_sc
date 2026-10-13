# SkillCombatRoll__

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#169**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `SkillCombatRoll__` | bindFunction | `Character::_SkillCombatRoll` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132645 |
| `SkillCombatRoll__` | bindMethod | `Character::_SkillCombatRoll` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132650 |

Address-identified callback: `Character::_SkillCombatRoll` at `0x003b9fbc`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9fbc.c`, unified lines 135553-135655.
Recovered declaration: `void __fastcall Character::_SkillCombatRoll( Character *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: getter not adjacent in decompilation; handler lines 135581, 135604.

## Recovered behavior, gates, state, timers, events and return

- **PROVEN:** The original _F_CalculateResult 0x3b2638 requests Random(100), then shifts the random integer left by 8 before the chance helper. Random uses seed modulo the positive bound, so the comparison domain is integers 0..99 multiplied by 256. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b2638.c:184`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003af6d8.c:6`, `port/level-world/reference/character-dot-attack/original-functions.asm:199`.
- **PROVEN:** Mode 2 stun and push use resolved SnS chance properties 184 and 183. If the raw chance is positive, the effective threshold is chance + 5*attacker_level_raw - defender_resistance_raw - 5*defender_level_raw; strict threshold > random_raw succeeds. Stun resistance is 138; push resistance is 136. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b0fa0.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b0d78.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b0fa0.c:35`.
- **PROVEN:** Values such as 80+5L and 185+4L are chance percentage points represented in fixed256, not an 8-bit probability divided by 256. A base above 100 can still fail after defender resistance and level adjustment. A successful roll is also separate from state admission, including the boss rejection in SM_SetStunState. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b2638.c:209`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b0fa0.c:27`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c:8`.
- **PROVEN:** SkillCombatRoll(object) is a Lua wrapper passing SKILL_ID to SkillCombatRoll__(SKILL_ID, object), whose registered handler is Character::_SkillCombatRoll 0x3b9fbc; it runs F_SkillAttack and then F_ApplyResult. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:178`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9fbc.c:70`.

## Lua wrapper/helper traces

Proven source chain: `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:40` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_melee_00.luac:57` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_ranged_00.luac:63` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_slave_ranged_00.luac:63` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/edward_melee_00.luac:57` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/edward_melee_01.luac:58` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/edward_ranged_00.luac:63` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_slam.luac:59` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:24` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_melee_00.luac:61` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:37` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/madruk_aoeknockback.luac:52` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/madruk_fastenergybolt.luac:63` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slam.luac:58` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slowenergybolt.luac:67` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_beamray.luac:58` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_bite.luac:56` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:67` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/npc_arrow_default.luac:63` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/npc_energybolt.luac:76` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/npc_fireball.luac:63` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/npc_poisonball.luac:63` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:52` `SkillCombatRoll(newtarget)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:126` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_multihit.luac:110` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:168` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_poison_arrows.luac:82` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_volley.luac:170` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:99` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:106` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:111` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:86` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:105` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:114` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:125` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:134` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_whirling_drift.luac:69` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_figments.luac:92` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:118` `SkillCombatRoll(item)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:247` `SkillCombatRoll(attackTarget)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_coldray.luac:81` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:67` `SkillCombatRoll(obj.ptr)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:89` `SkillCombatRoll(victim_data.ptr)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:124` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:72` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:74` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_deathclaw.luac:87` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:95` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:76` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:186` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:195` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_divine_judgement.luac:91` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_jump_kick.luac:81` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_roundhouse.luac:80` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_roundhouse.luac:85` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_vicious_strike.luac:96` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_bashdown.luac:85` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_charge.luac:107` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_ground_slam.luac:94` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/resgod_dying_attack.luac:57` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/root_troll_attack.luac:69` `SkillCombatRoll(newTarget)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/root_troll_attack_noskill.luac:58` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/root_troll_charge.luac:58` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_emerge.luac:58` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:82` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:108` `SkillCombatRoll(aoe_target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_1.luac:62` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_2.luac:62` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_3.luac:60` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_4.luac:58` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/swampking_atk_spit.luac:82` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/swampking_atk_spit.luac:108` `SkillCombatRoll(aoe_target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:137` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:152` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_bite.luac:57` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:133` `SkillCombatRoll(object)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:140` `SkillCombatRoll(object)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:169` `SkillCombatRoll(object)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:177` `SkillCombatRoll(object)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:146` `SkillCombatRoll(target_list[curr_index])`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:155` `SkillCombatRoll(target_list[1])`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:143` `SkillCombatRoll(target_list[curr_index])`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:150` `SkillCombatRoll(target_list[1])`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/witch_ranged_00.luac:65` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/worm_attack_1.luac:62` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.
Wrapper call: `.local-inputs/assets-extra/android/data/scripts/skills/worm_attack_2.luac:62` `SkillCombatRoll(target)`; chain `SkillCombatRoll` -> `SkillCombatRoll__`.

## Source-backed behavior cases

### SnS target-chance units and comparisons

- **PROVEN:** The original _F_CalculateResult 0x3b2638 requests Random(100), then shifts the random integer left by 8 before the chance helper. Random uses seed modulo the positive bound, so the comparison domain is integers 0..99 multiplied by 256. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b2638.c:184`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003af6d8.c:6`, `port/level-world/reference/character-dot-attack/original-functions.asm:199`.
- **PROVEN:** Mode 2 stun and push use resolved SnS chance properties 184 and 183. If the raw chance is positive, the effective threshold is chance + 5*attacker_level_raw - defender_resistance_raw - 5*defender_level_raw; strict threshold > random_raw succeeds. Stun resistance is 138; push resistance is 136. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b0fa0.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b0d78.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b0fa0.c:35`.
- **PROVEN:** Values such as 80+5L and 185+4L are chance percentage points represented in fixed256, not an 8-bit probability divided by 256. A base above 100 can still fail after defender resistance and level adjustment. A successful roll is also separate from state admission, including the boss rejection in SM_SetStunState. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b2638.c:209`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b0fa0.c:27`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c:8`.
- **PROVEN:** SkillCombatRoll(object) is a Lua wrapper passing SKILL_ID to SkillCombatRoll__(SKILL_ID, object), whose registered handler is Character::_SkillCombatRoll 0x3b9fbc; it runs F_SkillAttack and then F_ApplyResult. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:178`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9fbc.c:70`.


## Current frozen port source

**PARTIAL:** Identifier is present in production source but a complete reusable Lua callback/binding path is not proven.
- Source evidence: `port/windows-foundation/features/bosses/runtime_swampking_core_ai_v1.cpp`:330 — `if (!contains(delayed, "SkillCombatRoll(target)") || !contains(delayed, "SummonTimerTrap(TIMERTRAP_ID") ||`.
- Source evidence: `port/windows-foundation/features/bosses/runtime_swampking_core_ai_v1.cpp`:333 — `} else if (contains(skill, "repeat") && contains(skill, "SkillCombatRoll(target)") && contains(skill, "PlayFX(")) {`.
- Source evidence: `port/windows-foundation/features/bosses/runtime_swampking_core_ai_v1.cpp`:342 — `} else if (contains(skill, "if(target)") && contains(skill, "SkillCombatRoll(target)") && !contains(skill, "repeat") &&`.
- Source evidence: `port/windows-foundation/features/bosses/runtime_swampking_core_ai_v1.cpp`:828 — `return fail(error, "Boss SkillCombatRoll requires the CharacterTable, ClassTables and property rules");`.
- Source evidence: `port/windows-foundation/features/enemy_ai/runtime_ranged_skill_monster_v1.cpp`:273 — `next.delayed_combat_roll = contains(delayed, "ApplyPropClass(CLASS_ID)") && contains(delayed, "SkillCombatRoll(target)") &&`.
- Source evidence: `port/windows-foundation/features/enemy_ai/runtime_ranged_skill_monster_v1.cpp`:278 — `return fail(error, "Skill script is not a FrontalFirst pre-search / projectile / delayed SkillCombatRoll skill");`.
- Source evidence: `port/windows-foundation/features/generic_skills/runtime_skill_cast_coordinator_v1.cpp`:1333 — `active.receipt.detail = "Source object SkillCombatRoll Interact failed after committed OnPre";`.
- Source evidence: `port/windows-foundation/features/generic_skills/runtime_skill_cast_coordinator_v1.cpp`:1366 — `active.receipt.detail = "Source SkillCombatRoll loop failed after its applied ordered hit prefix";`.
- Source evidence: `port/windows-foundation/features/generic_skills/runtime_skill_cast_coordinator_v1.cpp`:1388 — `: "Authored source skill Use applied the ordered Character SkillCombatRoll result loop";`.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: Mode2 stun; equal levels, zero resistance, chance85*256; supplied random84*256. Chance succeeds; random85*256 fails because comparison is strict. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b2638.c`:184, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003af6d8.c`:6, `port/level-world/reference/character-dot-attack/original-functions.asm`:199, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b0fa0.c`:21, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b0d78.c`:21, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b0fa0.c`:35, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b2638.c`:209, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b0fa0.c`:27, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c`:8, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac`:178, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9fbc.c`:70.
2. **PROVEN:** Given: Mode2 push; equal levels, chance185*256, resistance100*256. Effective threshold is85*256; random85*256 fails despite the base exceeding100. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b2638.c`:184, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003af6d8.c`:6, `port/level-world/reference/character-dot-attack/original-functions.asm`:199, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b0fa0.c`:21, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b0d78.c`:21, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b0fa0.c`:35, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b2638.c`:209, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b0fa0.c`:27, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c`:8, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac`:178, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9fbc.c`:70.
3. **PROVEN:** Given: Mode2 stun; chance0, supplied random0. No positive base bonus is applied and 0>0 is false. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b2638.c`:184, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003af6d8.c`:6, `port/level-world/reference/character-dot-attack/original-functions.asm`:199, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b0fa0.c`:21, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b0d78.c`:21, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b0fa0.c`:35, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b2638.c`:209, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b0fa0.c`:27, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003c5ffc.c`:8, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac`:178, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9fbc.c`:70.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **1** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:178` | autoloaded_lua_common_helpers | `SkillCombatRoll__(SKILL_ID, object)` | `[{"index":1,"expression":"SKILL_ID","literal_tokens":[]},{"index":2,"expression":"object","literal_tokens":[]}]` |

## Related natives

`Character::_SkillCombatRoll`, `SkillCombatRoll__`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
