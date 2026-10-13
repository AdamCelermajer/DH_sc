# HasTarget

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PORTED**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `HasTarget` | bindFunction | `Character::_HasTarget` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132433 |
| `HasTarget` | bindMethod | `Character::_HasTarget` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132438 |

Address-identified callback: `Character::_HasTarget` at `0x003b6f50`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f50.c`, unified lines 133332-133345.
Recovered declaration: `void __fastcall Character::_HasTarget( Character *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument none: Receiver-only callback; extra Lua arguments ignored; unit/scale none. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f50.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f50.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f50.c:12`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Returns receiver target pointer at +1032 is nonzero. A dead target remains true; no range/sight/HP gate is read. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f50.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f50.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f50.c:12`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f50.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f50.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f50.c:12`.
**State, timers and events:**
- PROVEN: Read-only callback; starts no timers and raises no events. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f50.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f50.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f50.c:12`.
**Return:**
- PROVEN: One Lua value as described; no state mutation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f50.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f50.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f50.c:12`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PORTED:** character_target_bindings.cpp:59 and :67-75 implement pointer nonnull with Lua Boolean and bind at :84. Same target owner callback proven; no runtime verification.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:248 — `else if(target&&b.original_callback==0x3b6f50){function=&dh2_character_target_has_lua;context=target;}`.
- Source evidence: `port/level-world/character_target_bindings.cpp`:60 — `extern "C" int dh2_character_target_has_lua(void* p,const dh2_script_value* a,std::uint32_t count,dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,char* e,std::size_t ec){`.
- Source evidence: `port/level-world/character_target_bindings.cpp`:64 — `if((count&&!a)||!bindings_valid(b)||!aligned(out)||!capacity)return fail(e,ec,"Malformed HasTarget receiver/output");`.
- Source evidence: `port/level-world/character_target_bindings.cpp`:65 — `std::uint32_t result=0;if(dh2_character_has_target(&result,b->state))return fail(e,ec,"Malformed HasTarget state");`.
- Source evidence: `port/level-world/character_target_bindings.cpp`:76 — `int r=dh2_script_vm_bind_source_values(vm,"HasTarget",dh2_character_target_has_lua,b);if(r)return r;`.

**Port comparison:** character_target_bindings.cpp:59 and :67-75 implement pointer nonnull with Lua Boolean and bind at :84. Same target owner callback proven; no runtime verification.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: target null; HasTarget() False. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f50.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f50.c`:10, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f50.c`:12.
2. **PROVEN:** Given: target nonnull but dead; HasTarget() True. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f50.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f50.c`:10, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b6f50.c`:12.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **57** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/attacking_follower.luac:25` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/coward_core.luac:19` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:163` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:456` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:484` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:131` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:279` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:307` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/energyspawner.luac:28` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:119` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:403` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:431` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:217` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:445` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:477` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:75` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:163` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:189` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:299` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:400` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:220` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:66` | act1_level_boss_script_closure | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:229` | act1_level_boss_script_closure | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:131` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:280` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:307` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:37` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:78` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:258` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:343` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:413` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:121` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:274` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:356` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:31` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:184` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:283` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:366` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:375` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:173` | act1_level_boss_script_closure | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:213` | act1_level_boss_script_closure | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:445` | act1_level_boss_script_closure | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:573` | act1_level_boss_script_closure | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:650` | act1_level_boss_script_closure | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:677` | act1_level_boss_script_closure | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:689` | act1_level_boss_script_closure | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:217` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:374` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:406` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:146` | act1_level_boss_script_closure | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:209` | act1_level_boss_script_closure | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:45` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_1.luac:34` | act1_level_boss_script_closure | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_2.luac:34` | act1_level_boss_script_closure | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swampking_atk_spit.luac:45` | act1_level_boss_script_closure | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/worm_attack_1.luac:34` | outside scoped roots | `HasTarget()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/worm_attack_2.luac:34` | outside scoped roots | `HasTarget()` | `[]` |

## Related natives

`Character::_HasTarget`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
