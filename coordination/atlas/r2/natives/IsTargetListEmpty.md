# IsTargetListEmpty

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#4**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `IsTargetListEmpty` | bindFunction | `GameObject::_IsTargetListEmpty` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105367 |
| `IsTargetListEmpty` | bindMethod | `GameObject::_IsTargetListEmpty` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105372 |

Address-identified callback: `GameObject::_IsTargetListEmpty` at `0x0038e970`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e970.c`, unified lines 105824-105832.
Recovered declaration: `void __fastcall GameObject::_IsTargetListEmpty( GameObject *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
No explicit operator[] index was extracted. This does not prove a zero-argument callback.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Returns one Lua boolean computed by comparing target-list end and begin pointers; it does not inspect or filter entries. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e970.c:7`.
**Return:**
- PROVEN: Pushes true exactly when target-list end equals begin and false otherwise. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e970.c:7`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** The port computes the same empty predicate from list_.count in CharacterSkillNativeBindingsV5, but its evidence is a skill-native owner route with one retained list, not proof of the source GameObject-wide function/method binding domain.
- Source evidence: `port/level-world/character_skill_native_v5.cpp`:12.
- Source evidence: `port/level-world/character_skill_native_v5.cpp`:23.
- Source evidence: `port/level-world/character_skill_native_v5.cpp`:102.
- Source evidence: `port/level-world/character_skill_native_v5.cpp`:103.

**Concrete behavioral difference:** The port computes the same empty predicate from list_.count in CharacterSkillNativeBindingsV5, but its evidence is a skill-native owner route with one retained list, not proof of the source GameObject-wide function/method binding domain.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: IsTargetListEmpty() with target-list begin == end. Returns Lua true. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e970.c`:7.
2. **PROVEN:** Given: IsTargetListEmpty() with target-list begin != end. Returns Lua false. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e970.c`:7.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **233** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:177` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:200` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:267` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:291` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:313` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:145` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:161` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:133` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:156` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:223` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:247` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:269` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:230` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:247` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:261` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:285` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:307` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:275` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:296` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:145` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:161` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:51` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:67` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:89` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:14` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:186` | act1_level_boss_script_closure | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:202` | act1_level_boss_script_closure | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:230` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:247` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:30` | act1_level_boss_script_closure | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:46` | act1_level_boss_script_closure | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/shrine.luac:34` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/shrine.luac:46` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/shrine_buff.luac:44` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/shrine_buff.luac:56` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/shrine_resto.luac:23` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/shrine_resto.luac:34` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/shrine_xp_major.luac:34` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/shrine_xp_major.luac:46` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/shrine_xp_minor.luac:35` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/shrine_xp_minor.luac:48` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:34` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:51` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:79` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_melee_00.luac:44` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_ranged_00.luac:42` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_slave_ranged_00.luac:42` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/edward_melee_00.luac:44` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/edward_melee_01.luac:45` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/edward_ranged_00.luac:42` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:246` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:258` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:282` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_hotty.luac:76` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_hotty.luac:92` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_hotty.luac:115` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_rocky.luac:76` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_rocky.luac:86` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_rocky.luac:107` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_wetty.luac:76` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_wetty.luac:93` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_wetty.luac:113` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_windy.luac:76` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_windy.luac:86` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_windy.luac:105` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_slam.luac:45` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:38` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:43` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:74` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_melee_00.luac:45` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_melee_00.luac:57` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_melee_00.luac:63` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:33` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:48` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:76` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruk_aoeknockback.luac:45` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruk_aoeknockback.luac:53` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruk_fastenergybolt.luac:42` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slam.luac:45` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slowenergybolt.luac:42` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slowenergybolt.luac:50` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_bufferspell.luac:41` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_healerspell.luac:40` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_beamray.luac:42` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_beamray.luac:53` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_beamray.luac:67` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_bite.luac:44` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:54` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:112` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:122` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:149` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_arrow_default.luac:42` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_dummy.luac:34` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_dummy.luac:42` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_dummy.luac:53` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_dummy.luac:61` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_energybolt.luac:39` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_energybolt.luac:51` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_fireball.luac:41` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_poisonball.luac:42` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:47` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:57` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:97` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:107` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_multihit.luac:68` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_multihit.luac:84` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:92` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:108` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_poison_arrows.luac:56` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_poison_arrows.luac:66` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_volley.luac:58` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_volley.luac:68` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_volley.luac:121` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_volley.luac:137` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:67` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:61` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:64` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:105` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:116` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:121` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:130` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_warcry.luac:81` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_warcry.luac:126` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_warcry.luac:127` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_whirling_drift.luac:61` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_whirling_drift.luac:75` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_eclipse.luac:64` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_eclipse.luac:80` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_eclipse.luac:91` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_figments.luac:85` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_figments.luac:108` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_mesmerize.luac:22` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_mesmerize.luac:30` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:231` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:241` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_timecontrol.luac:69` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_timecontrol.luac:90` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_timecontrol.luac:99` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:79` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:89` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:162` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:193` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:209` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_coldray.luac:61` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_coldray.luac:77` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_coldray.luac:89` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:113` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:126` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:227` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:58` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:80` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_deathclaw.luac:67` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_deathclaw.luac:84` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_deathclaw.luac:95` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:71` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:81` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:88` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:96` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:71` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:78` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:143` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:158` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:163` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:199` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_divine_judgement.luac:66` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_divine_judgement.luac:72` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_divine_judgement.luac:94` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_guarding_aura.luac:124` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_guarding_aura.luac:139` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_offensive_aura.luac:117` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_offensive_aura.luac:134` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_purifying_aura.luac:71` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_purifying_aura.luac:88` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_jump_kick.luac:61` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_jump_kick.luac:74` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_jump_kick.luac:88` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_roundhouse.luac:60` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_roundhouse.luac:72` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_roundhouse.luac:87` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_vicious_strike.luac:69` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_vicious_strike.luac:89` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_vicious_strike.luac:98` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_bashdown.luac:65` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_charge.luac:84` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_charge.luac:104` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_charge.luac:109` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_ground_slam.luac:86` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_ground_slam.luac:95` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_leadership.luac:28` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_leadership.luac:37` | player_skill_lua | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/resgod_dying_attack.luac:46` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/resgod_dying_attack.luac:58` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/root_troll_attack.luac:45` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/root_troll_attack.luac:65` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/root_troll_attack.luac:74` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/root_troll_attack_noskill.luac:44` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/root_troll_charge.luac:44` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_emerge.luac:42` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_emerge.luac:60` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:59` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:100` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:110` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_1.luac:48` | act1_level_boss_script_closure | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_2.luac:48` | act1_level_boss_script_closure | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_3.luac:40` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_3.luac:61` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_4.luac:42` | act1_level_boss_script_closure | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_4.luac:60` | act1_level_boss_script_closure | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swampking_atk_spit.luac:59` | act1_level_boss_script_closure | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swampking_atk_spit.luac:100` | act1_level_boss_script_closure | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swampking_atk_spit.luac:110` | act1_level_boss_script_closure | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:134` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:139` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:149` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:154` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:209` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:217` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_bite.luac:44` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:47` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:54` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:71` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:92` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:52` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:59` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:75` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:96` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:49` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:56` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:72` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:93` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/witch_ranged_00.luac:43` | act1_level_boss_script_closure | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/worm_attack_1.luac:48` | outside scoped roots | `IsTargetListEmpty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/worm_attack_2.luac:48` | outside scoped roots | `IsTargetListEmpty()` | `[]` |

## Related natives

`GameObject::_IsTargetListEmpty`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
