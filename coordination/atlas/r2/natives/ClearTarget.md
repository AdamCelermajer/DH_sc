# ClearTarget

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PORTED**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `ClearTarget` | bindFunction | `Character::_ClearTarget` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132460 |
| `ClearTarget` | bindMethod | `Character::_ClearTarget` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132465 |

Address-identified callback: `Character::_ClearTarget` at `0x003b5690`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5690.c`, unified lines 132248-132260.
Recovered declaration: `int __fastcall Character::_ClearTarget( Character *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument none: Receiver-only callback; extra Lua arguments ignored; unit/scale none. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5690.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5690.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5690.c:11`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d6890.c:25`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Calls AI_SetTarget(receiver AI,null,0) then AI_SyncLastTarget unconditionally. Unlike SetTarget(null), also writes last-target null. Clears hit count if old target differed; null setter preserves alive/sight cached bytes. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5690.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5690.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5690.c:11`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d6890.c:25`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5690.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5690.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5690.c:11`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d6890.c:25`.
**State, timers and events:**
- PROVEN: No Lua result; target/candidate/last-target/hit count changes, no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5690.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5690.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5690.c:11`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d6890.c:25`.
**Return:**
- PROVEN: No Lua result; target/candidate/last-target/hit count changes, no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5690.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5690.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5690.c:11`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d6890.c:25`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PORTED:** character_target_bindings.cpp:58-60 implements null set then sync last-target; generic source-scoped binding at :86. Callback source parity, no runtime claim.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:242 — `if(target&&(b.original_callback==0x3b8f38||b.original_callback==0x3b5690)){`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:248 — `else if(target&&b.original_callback==0x3b6f50){function=&dh2_character_target_has_lua;context=target;}`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:265 — `else if(b.original_callback==0x3b9d8c){function=&dh2_character_get_prop;context=&property_bindings;}`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:266 — `else if(b.original_callback==0x3b73a4){function=&dh2_character_set_level_lua;context=&level_bindings;}`.

**Port comparison:** character_target_bindings.cpp:58-60 implements null set then sync last-target; generic source-scoped binding at :86. Callback source parity, no runtime claim.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: target T,last T,hit5; ClearTarget() target/candidate/last null, hit0; no Lua value. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5690.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5690.c`:10, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5690.c`:11, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d6890.c`:25.
2. **PROVEN:** Given: target null,last T,hit5; ClearTarget() last null, hit5 preserved because current target unchanged. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5690.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5690.c`:10, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b5690.c`:11, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d6890.c`:25.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **73** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/attacking_follower.luac:43` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/attacking_follower.luac:59` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/attacking_follower.luac:104` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/attacking_follower.luac:114` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:165` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:259` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:280` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:468` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:133` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:291` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/energyspawner.luac:40` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:121` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:215` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:236` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:415` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/follower.luac:36` | act1_level_boss_script_closure | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:219` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:253` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:274` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:459` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:68` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:86` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:165` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:402` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:83` | act1_level_boss_script_closure | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:94` | act1_level_boss_script_closure | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:78` | act1_level_boss_script_closure | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:133` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:291` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:39` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:415` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:442` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:81` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:6` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:368` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:377` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:175` | act1_level_boss_script_closure | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:679` | act1_level_boss_script_closure | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:691` | act1_level_boss_script_closure | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:219` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:388` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:159` | act1_level_boss_script_closure | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:172` | act1_level_boss_script_closure | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/edward_rise.luac:313` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/generic_teleport_skill.luac:113` | outside scoped roots | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:118` | player_skill_lua | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_multihit.luac:92` | player_skill_lua | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:134` | player_skill_lua | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_poison_arrows.luac:74` | player_skill_lua | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_volley.luac:151` | player_skill_lua | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:120` | player_skill_lua | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:76` | player_skill_lua | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:101` | player_skill_lua | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:144` | player_skill_lua | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_whirling_drift.luac:81` | player_skill_lua | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:255` | player_skill_lua | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_coldray.luac:95` | player_skill_lua | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:241` | player_skill_lua | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_deathclaw.luac:100` | player_skill_lua | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:156` | player_skill_lua | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:205` | player_skill_lua | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_rise.luac:317` | player_skill_lua | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_divine_judgement.luac:100` | player_skill_lua | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_guarding_aura.luac:150` | player_skill_lua | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_offensive_aura.luac:145` | player_skill_lua | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_purifying_aura.luac:152` | player_skill_lua | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_jump_kick.luac:94` | player_skill_lua | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_quickness.luac:101` | player_skill_lua | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_roundhouse.luac:93` | player_skill_lua | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_vicious_strike.luac:104` | player_skill_lua | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_bashdown.luac:91` | player_skill_lua | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_charge.luac:116` | player_skill_lua | `ClearTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_ground_slam.luac:101` | player_skill_lua | `ClearTarget()` | `[]` |

## Related natives

`Character::_ClearTarget`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
