# GetTarget

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PORTED**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `GetTarget` | bindFunction | `Character::_GetTarget` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132442 |
| `GetTarget` | bindMethod | `Character::_GetTarget` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132447 |

Address-identified callback: `Character::_GetTarget` at `0x003b6c7c`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6c7c.c`, unified lines 133120-133128.
Recovered declaration: `void __fastcall Character::_GetTarget( Character *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::UserData **a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument none: Receiver-only callback; extra Lua arguments ignored; unit/scale none. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6c7c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6c7c.c:7`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Pushes receiver current target userdata at +1032, without performing a target search or admission. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6c7c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6c7c.c:7`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6c7c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6c7c.c:7`.
**State, timers and events:**
- PROVEN: Read-only callback; starts no timers and raises no events. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6c7c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6c7c.c:7`.
**Return:**
- PROVEN: One Lua value as described; no state mutation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6c7c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6c7c.c:7`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PORTED:** gameobject_lua_representation.cpp:24-29 and character_script_objects.cpp:76 expose same retained target identity as source object. Callback path/source parity proven for supported receivers, no runtime claim.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:237 — `if(target&&b.original_callback==0x3b6c7c&&objects){`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:159 — `auto& t=*static_cast<Impl*>(opaque);if(!returned||!out||!capacity||!t.target)return fail(error,size,"Malformed GetTarget output");*returned=0;`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:160 — `return dh2::gameobject_lua::dh2_gameobject_lua_get_target(out,returned,t.target->state)?fail(error,size,"Native GetTarget delivery failed"):0;`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:239 — `if(dh2_script_vm_bind_source_objects(c.vm,b.name,&get_target,this))throw std::runtime_error("Source GetTarget registration failed");`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:248 — `else if(target&&b.original_callback==0x3b6f50){function=&dh2_character_target_has_lua;context=target;}`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:265 — `else if(b.original_callback==0x3b9d8c){function=&dh2_character_get_prop;context=&property_bindings;}`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:266 — `else if(b.original_callback==0x3b73a4){function=&dh2_character_set_level_lua;context=&level_bindings;}`.

**Port comparison:** gameobject_lua_representation.cpp:24-29 and character_script_objects.cpp:76 expose same retained target identity as source object. Callback path/source parity proven for supported receivers, no runtime claim.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: current target T; GetTarget() Pushes userdata T. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6c7c.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6c7c.c`:7.
2. **PROVEN:** Given: target null; GetTarget() Pushes null userdata via source ReturnValues contract. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6c7c.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6c7c.c`:7.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **99** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/coward_core.luac:20` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/coward_core.luac:37` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/coward_core.luac:38` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/coward_core.luac:120` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:164` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:467` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:485` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:488` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:500` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:513` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:529` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:132` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:290` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:308` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:311` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:323` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:336` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:352` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:120` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:414` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:432` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:435` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:450` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:452` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:462` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:478` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:218` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:458` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:478` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:481` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:498` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:500` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:512` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:530` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:67` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:105` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:126` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:134` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:152` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:155` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:164` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:325` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:348` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:401` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:233` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:244` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:53` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:70` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:88` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:92` | act1_level_boss_script_closure | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:111` | act1_level_boss_script_closure | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:134` | act1_level_boss_script_closure | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:164` | act1_level_boss_script_closure | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:170` | act1_level_boss_script_closure | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:189` | act1_level_boss_script_closure | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:192` | act1_level_boss_script_closure | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:149` | act1_level_boss_script_closure | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:132` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:290` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:308` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:311` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:326` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:328` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:340` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:356` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:38` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:228` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:259` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:414` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:440` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:441` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:239` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:275` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:149` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:185` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:367` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:376` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:174` | act1_level_boss_script_closure | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:417` | act1_level_boss_script_closure | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:446` | act1_level_boss_script_closure | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:650` | act1_level_boss_script_closure | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:678` | act1_level_boss_script_closure | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:690` | act1_level_boss_script_closure | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:218` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:387` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:407` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:410` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:427` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:429` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:441` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:459` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:171` | act1_level_boss_script_closure | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:183` | act1_level_boss_script_closure | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:46` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_1.luac:35` | act1_level_boss_script_closure | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_2.luac:35` | act1_level_boss_script_closure | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swampking_atk_spit.luac:46` | act1_level_boss_script_closure | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/worm_attack_1.luac:35` | outside scoped roots | `GetTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/worm_attack_2.luac:35` | outside scoped roots | `GetTarget()` | `[]` |

## Related natives

`Character::_GetTarget`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
