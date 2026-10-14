# GetTargetListTop

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#5**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `GetTargetListTop` | bindFunction | `GameObject::_GetTargetListTop` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105385 |
| `GetTargetListTop` | bindMethod | `GameObject::_GetTargetListTop` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105390 |

Address-identified callback: `GameObject::_GetTargetListTop` at `0x0038eb68`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038eb68.c`, unified lines 106003-106025.
Recovered declaration: `void __fastcall GameObject::_GetTargetListTop( GameObject *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
No explicit operator[] index was extracted. This does not prove a zero-argument callback.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: When empty, pushes a null pointer as one return value. When nonempty, pushes five values in order: the top object's userdata, stored distance, stored angle multiplied by 57.296 (radians to degrees), low bit of the entry byte at offset 12 as boolean, and the float at offset 16. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038eb68.c:9`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038eb68.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038eb68.c:12`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038eb68.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038eb68.c:17`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038eb68.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038eb68.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038eb68.c:20`.
**Return:**
- PROVEN: Return arity is one for an empty list and five for a nonempty list; the empty value is emitted by pushPointer(nullptr), not by the nonempty userdata route. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038eb68.c:12`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038eb68.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038eb68.c:20`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** The port's top entry returns the same identity, distance, angle conversion and flag bit, but hardcodes return value five to 0 instead of the source float at entry offset 16. On an empty list the port emits a default zero-initialized script value while source calls pushPointer(nullptr); their exact type parity is not established by the inspected handlers.
- Source evidence: `port/level-world/character_skill_native_v5.cpp`:43.
- Source evidence: `port/level-world/character_skill_native_v5.cpp`:52.
- Source evidence: `port/level-world/character_skill_native_v5.cpp`:54.
- Source evidence: `port/level-world/character_skill_native_v5.cpp`:105.
- Source evidence: `port/level-world/character_skill_native_v5.cpp`:106.
- Source evidence: `port/level-world/character_skill_native_v5.cpp`:107.
- Source evidence: `port/level-world/character_skill_native_v5.cpp`:108.

**Concrete behavioral difference:** The port's top entry returns the same identity, distance, angle conversion and flag bit, but hardcodes return value five to 0 instead of the source float at entry offset 16. On an empty list the port emits a default zero-initialized script value while source calls pushPointer(nullptr); their exact type parity is not established by the inspected handlers.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: GetTargetListTop() with begin == end. One return value is pushed: a null pointer. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038eb68.c`:9, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038eb68.c`:10, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038eb68.c`:12.
2. **PROVEN:** Given: GetTargetListTop() with top entry distance 12.5, angle 0.5 radians, bit0 set, and float at entry offset 16 equal to 7.25. Returns userdata, 12.5, approximately 28.648 degrees, true, 7.25, in that order. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038eb68.c`:16, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038eb68.c`:17, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038eb68.c`:18, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038eb68.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038eb68.c`:20.
3. **PROVEN:** Given: GetTargetListTop() with a nonempty top entry whose byte at offset 12 is 2 (bit0 clear). The fourth returned value is Lua false, even though the byte is nonzero. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038eb68.c`:19.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **157** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:179` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:188` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:268` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:293` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:299` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:147` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:135` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:144` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:224` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:249` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:255` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:233` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:262` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:287` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:293` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:277` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:147` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:53` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:90` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:15` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:188` | act1_level_boss_script_closure | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:233` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:32` | act1_level_boss_script_closure | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/shrine.luac:36` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/shrine_buff.luac:46` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/shrine_resto.luac:25` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/shrine_xp_major.luac:36` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/objects/shrine_xp_minor.luac:38` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:35` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:80` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_melee_00.luac:45` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_ranged_00.luac:43` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_slave_ranged_00.luac:43` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/edward_melee_00.luac:45` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/edward_melee_01.luac:46` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/edward_ranged_00.luac:43` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:263` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_hotty.luac:97` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_rocky.luac:92` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_wetty.luac:98` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_windy.luac:91` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_slam.luac:46` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:39` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:75` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_melee_00.luac:46` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_melee_00.luac:59` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:34` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:77` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruk_aoeknockback.luac:49` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruk_fastenergybolt.luac:43` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slam.luac:46` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slowenergybolt.luac:43` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slowenergybolt.luac:51` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_bufferspell.luac:42` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_healerspell.luac:41` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_beamray.luac:43` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_beamray.luac:55` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_bite.luac:45` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:56` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:113` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:150` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_arrow_default.luac:43` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_dummy.luac:37` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_dummy.luac:56` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_energybolt.luac:40` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_energybolt.luac:52` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_fireball.luac:42` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_poisonball.luac:43` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:50` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_exploding_arrows.luac:98` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_multihit.luac:69` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:93` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:109` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:117` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_pin_shot.luac:125` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_poison_arrows.luac:57` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_volley.luac:59` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_volley.luac:122` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_volley.luac:138` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:68` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:82` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:62` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:65` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:108` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:124` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:133` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_warcry.luac:82` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_warcry.luac:126` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_whirling_drift.luac:66` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_eclipse.luac:65` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_eclipse.luac:81` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_figments.luac:89` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_mesmerize.luac:24` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_terror.luac:232` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_timecontrol.luac:70` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_timecontrol.luac:91` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:81` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:163` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_chain_lightning.luac:195` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_coldray.luac:62` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_coldray.luac:79` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:118` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_cyclone.luac:228` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:68` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_deathclaw.luac:68` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_deathclaw.luac:85` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:73` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:93` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:72` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:144` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_pestilence.luac:164` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_divine_judgement.luac:69` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_divine_judgement.luac:89` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_guarding_aura.luac:126` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_offensive_aura.luac:120` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_purifying_aura.luac:73` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_jump_kick.luac:62` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_jump_kick.luac:79` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_roundhouse.luac:61` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_roundhouse.luac:76` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_vicious_strike.luac:70` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_vicious_strike.luac:92` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_bashdown.luac:66` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_charge.luac:85` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_charge.luac:106` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_ground_slam.luac:88` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_warrior_leadership.luac:30` | player_skill_lua | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/resgod_dying_attack.luac:54` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/root_troll_attack.luac:46` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/root_troll_attack.luac:66` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/root_troll_attack_noskill.luac:45` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/root_troll_charge.luac:45` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_emerge.luac:43` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_emerge.luac:57` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:60` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:104` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_1.luac:49` | act1_level_boss_script_closure | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_2.luac:49` | act1_level_boss_script_closure | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_3.luac:41` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_3.luac:57` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_4.luac:43` | act1_level_boss_script_closure | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_4.luac:57` | act1_level_boss_script_closure | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swampking_atk_spit.luac:60` | act1_level_boss_script_closure | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swampking_atk_spit.luac:104` | act1_level_boss_script_closure | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:136` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:151` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:211` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_bite.luac:45` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:51` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:74` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:56` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:77` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:53` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:74` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/witch_ranged_00.luac:44` | act1_level_boss_script_closure | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/worm_attack_1.luac:49` | outside scoped roots | `GetTargetListTop()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/worm_attack_2.luac:49` | outside scoped roots | `GetTargetListTop()` | `[]` |

## Related natives

`GameObject::_GetTargetListTop`

## Unresolved reasons

- **DONE:** The semantic meaning of the source float at entry offset 16 is not identified in the supplied evidence; preserve and compare its value without assigning a guessed label.

## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
