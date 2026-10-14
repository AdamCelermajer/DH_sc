# PopTargetList

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PORTED**.

Engine functions to implement next ranking: **#13**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `PopTargetList` | bindFunction | `GameObject::_PopTargetList` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105394 |
| `PopTargetList` | bindMethod | `GameObject::_PopTargetList` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105399 |

Address-identified callback: `GameObject::_PopTargetList` at `0x0038fbb8`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038fbb8.c`, unified lines 107165-107186.
Recovered declaration: `void __fastcall GameObject::_PopTargetList(int a1, int a2, _DWORD *a3)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument none: Receiver-only callback; extra Lua arguments ignored; unit/scale none. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038fbb8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038fbb8.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038fbb8.c:20`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Computes receiver target queue size; pops exactly one priority-queue top when nonempty and does nothing when empty. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038fbb8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038fbb8.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038fbb8.c:20`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038fbb8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038fbb8.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038fbb8.c:20`.
**State, timers and events:**
- PROVEN: No Lua result; updates target priority queue only, no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038fbb8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038fbb8.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038fbb8.c:20`.
**Return:**
- PROVEN: No Lua result; updates target priority queue only, no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038fbb8.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038fbb8.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038fbb8.c:20`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PORTED:** character_skill_native_v5.cpp:110 implements empty no-op and heap pop; callback supported. Static callback/source path proven; no runtime verification.

**Port comparison:** character_skill_native_v5.cpp:110 implements empty no-op and heap pop; callback supported. Static callback/source path proven; no runtime verification.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: queue empty; PopTargetList() Queue stays empty; no Lua return. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038fbb8.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038fbb8.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038fbb8.c`:20.
2. **PROVEN:** Given: queue has three targets; PopTargetList() Removes current top, leaves two targets. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038fbb8.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038fbb8.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038fbb8.c`:20.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **83** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:199` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:312` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:159` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:155` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:268` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:245` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:306` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:278` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:159` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:65` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:200` | act1_level_boss_script_closure | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:245` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:44` | act1_level_boss_script_closure | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/shrine.luac:37` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/shrine_buff.luac:47` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/shrine_resto.luac:26` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/shrine_xp_major.luac:37` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/shrine_xp_minor.luac:39` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:264` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_hotty.luac:98` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_rocky.luac:93` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_wetty.luac:99` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_windy.luac:92` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:40` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_melee_00.luac:62` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruk_aoeknockback.luac:50` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slowenergybolt.luac:53` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_beamray.luac:66` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:120` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_dummy.luac:38` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_dummy.luac:57` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:51` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:108` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_multihit.luac:85` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:112` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:121` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_poison_arrows.luac:67` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_volley.luac:66` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_volley.luac:140` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:109` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_warcry.luac:125` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_whirling_drift.luac:67` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_eclipse.luac:82` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_figments.luac:90` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_mesmerize.luac:29` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:233` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_timecontrol.luac:97` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:83` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:196` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_coldray.luac:80` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:119` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:69` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_deathclaw.luac:86` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:94` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:73` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:165` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_divine_judgement.luac:70` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_divine_judgement.luac:90` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_guarding_aura.luac:134` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_offensive_aura.luac:129` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_purifying_aura.luac:83` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_jump_kick.luac:80` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_roundhouse.luac:77` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_vicious_strike.luac:93` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_charge.luac:108` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_ground_slam.luac:93` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_leadership.luac:36` | player_skill_lua | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/resgod_dying_attack.luac:55` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/root_troll_attack.luac:67` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_emerge.luac:59` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:105` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_3.luac:58` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_4.luac:59` | act1_level_boss_script_closure | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swampking_atk_spit.luac:105` | act1_level_boss_script_closure | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:138` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:153` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:216` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:53` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:91` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:58` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:95` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:55` | outside scoped roots | `PopTargetList()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:92` | outside scoped roots | `PopTargetList()` | `[]` |

## Related natives

`GameObject::_PopTargetList`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
