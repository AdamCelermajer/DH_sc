# ClearAggro

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#22**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `ClearAggro` | bindFunction | `Character::_ClearAggro` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132424 |
| `ClearAggro` | bindMethod | `Character::_ClearAggro` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132429 |

Address-identified callback: `Character::_ClearAggro` at `0x003b8648`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c`, unified lines 134411-134437.
Recovered declaration: `sfc::script::lua::Value *__fastcall Character::_ClearAggro( Character *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: pointer tag2/userdata tag7; unit/scale other Character identity. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c:21`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Requires pointer/userdata argument; AI_ClearAggro nonnull target removes pair from receiver threat map if found, removes receiver from target reciprocal map, notifies target AI virtual+60. If target current target==receiver, clears that target and issues target Cmd_Stop, even when threat entry absent. Null arg no-op; no zero-argument clear-all. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d6d68.c:37`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d6d68.c:37`.
**State, timers and events:**
- PROVEN: No Lua value; pair threat erasure/AI notification/target stop conditional, no wrapper timer. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d6d68.c:37`.
**Return:**
- PROVEN: No Lua value; pair threat erasure/AI notification/target stop conditional, no wrapper timer. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d6d68.c:37`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** Selected AI threat services exist, but actor Lua dispatch/general reciprocal threat callback missing in CharacterScriptObjects (:144-146); clearing only caller threat/target omits reciprocal stop/event side effects.
- Source evidence: `port/level-world/character_clear_aggro.cpp`:46 — `if(!dh2_script_callback_scope_valid(scope)){if(error&&size)std::snprintf(error,size,"Malformed ClearAggro callback capability");return 1;}`.
- Source evidence: `port/level-world/character_clear_aggro.cpp`:48 — `if(status&&error&&size)std::snprintf(error,size,"Native ClearAggro delivery failed (%d)",status);return status?1:0;`.
- Source evidence: `port/level-world/character_clear_aggro.cpp`:50 — `extern "C" int dh2_character_clear_aggro_bind(dh2_script_vm* vm,ClearAggroBindings16* b){if(!aligned(b)||!aligned(b->state)||!aligned(b->services))return -1;return dh2_script_vm_bind_source_scoped(vm,"ClearAggro",dh2_character_clear_aggro_s`.
- Source evidence: `port/level-world/character_update_aggro_v108.cpp`:45 — `auto clear=[&]{if(!required(bool(s.clear)&&bool(s.set_target),"Required source ClearAggro/SetTarget")||!s.clear(current,e)||!s.set_target(0,e))return false;return dh2_character_ai_sync_last_target(&target)==0;};`.
- Source evidence: `port/level-world/character_world_clear_aggro_v40.cpp`:5 — `if(scope&&!dh2_script_callback_scope_valid(scope)){error_="Required valid current ClearAggro callback scope";return -1;}`.
- Source evidence: `port/level-world/character_world_clear_aggro_v40.cpp`:7 — `if(!services_.actor||!services_.actor(services_.context,id,v,error_)){if(error_.empty())error_="Required SAME registered ClearAggro actor";return false;}`.
- Source evidence: `port/level-world/character_world_clear_aggro_v40.cpp`:8 — `if(v.identity!=id||!v.lifetime||!v.target||!v.target->state||!v.target->state->owner||v.target->state->owner->identity!=id){error_="Required canonical Character/Target owner and lifetime for ClearAggro";return false;}return true;`.
- Source evidence: `port/level-world/character_world_clear_aggro_v40.cpp`:15 — `if(dh2_aggro_apply(&change,&request,data::aggro_clear)){error_="Source ClearAggro reciprocal erase rejected";return -1;}`.
- Source evidence: `port/level-world/character_world_clear_aggro_v40.cpp`:20 — `if(!a.target->state->owner||!b.target->state){error_="ClearAggro live target lifetime changed";return -2;}`.
- Source evidence: `port/level-world/character_world_clear_aggro_v40.cpp`:24 — `if(dh2_character_ai_set_target(binding->state,0,0,&binding->services)){error_="Required source ClearAggro SetTarget(NULL)";return -2;}`.
- Source evidence: `port/level-world/character_world_clear_aggro_v40.cpp`:28 — `if(fresh.target!=binding){error_="ClearAggro target binding lifetime changed";return -2;}`.

**Concrete behavioral difference:** Selected AI threat services exist, but actor Lua dispatch/general reciprocal threat callback missing in CharacterScriptObjects (:144-146); clearing only caller threat/target omits reciprocal stop/event side effects.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: ClearAggro() No threat mutation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c`:18, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c`:21, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c`:25, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d6d68.c`:37.
2. **PROVEN:** Given: ClearAggro(null pointer) No threat mutation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c`:18, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c`:21, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c`:25, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d6d68.c`:37.
3. **PROVEN:** Given: ClearAggro(T), pair absent but T targets caller T target cleared and T controller Stop issued. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c`:18, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c`:21, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b8648.c`:25, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d6d68.c`:37.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **30** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/coward_core.luac:38` | outside scoped roots | `ClearAggro(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:164` | outside scoped roots | `ClearAggro(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:467` | outside scoped roots | `ClearAggro(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:132` | outside scoped roots | `ClearAggro(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:290` | outside scoped roots | `ClearAggro(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:120` | outside scoped roots | `ClearAggro(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:414` | outside scoped roots | `ClearAggro(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:218` | outside scoped roots | `ClearAggro(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:458` | outside scoped roots | `ClearAggro(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:67` | outside scoped roots | `ClearAggro(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:164` | outside scoped roots | `ClearAggro(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:401` | outside scoped roots | `ClearAggro(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:93` | act1_level_boss_script_closure | `ClearAggro(aggro)` | `[{"index":1,"expression":"aggro","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:132` | outside scoped roots | `ClearAggro(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:290` | outside scoped roots | `ClearAggro(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:38` | outside scoped roots | `ClearAggro(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:259` | outside scoped roots | `ClearAggro(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:414` | outside scoped roots | `ClearAggro(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:441` | outside scoped roots | `ClearAggro(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:277` | outside scoped roots | `ClearAggro(myTarget)` | `[{"index":1,"expression":"myTarget","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:187` | outside scoped roots | `ClearAggro(myTarget)` | `[{"index":1,"expression":"myTarget","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:367` | outside scoped roots | `ClearAggro(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:376` | outside scoped roots | `ClearAggro(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:174` | act1_level_boss_script_closure | `ClearAggro(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:448` | act1_level_boss_script_closure | `ClearAggro(myTarget)` | `[{"index":1,"expression":"myTarget","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:678` | act1_level_boss_script_closure | `ClearAggro(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:690` | act1_level_boss_script_closure | `ClearAggro(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:218` | outside scoped roots | `ClearAggro(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:387` | outside scoped roots | `ClearAggro(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:171` | act1_level_boss_script_closure | `ClearAggro(GetTarget())` | `[{"index":1,"expression":"GetTarget()","literal_tokens":[]}]` |

## Related natives

`Character::_ClearAggro`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
