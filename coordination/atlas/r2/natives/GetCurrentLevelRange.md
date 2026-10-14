# GetCurrentLevelRange

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#30**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `GetCurrentLevelRange` | bindFunction | `LuaScript::_GetCurrentLevelRange` | `_ZN9LuaScript12BindFunctionEv` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:91881 |

Address-identified callback: `LuaScript::_GetCurrentLevelRange` at `0x0037f1f0`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`, unified lines 94894-94942.
Recovered declaration: `void __fastcall LuaScript::_GetCurrentLevelRange( LuaScript *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: optional number tag3 truncated int; unit/scale difficulty selector0/1/2. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:29`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Reads current Level table ID +60 (current Level must exist). ID -1 pushes(-1,-1). Missing/non-numeric selector defaults0; selector0 reads LevelList row(+60,+48), selector1(+64,+52), selector2(+68,+56), in that order. Other numeric selector pushes nothing. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:27`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:32`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:33`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:34`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:27`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:32`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:33`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:34`.
**State, timers and events:**
- PROVEN: Two integers for known selector or invalid table ID; other selector no values. Read-only, no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:27`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:32`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:33`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:34`.
**Return:**
- PROVEN: Two integers for known selector or invalid table ID; other selector no values. Read-only, no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:27`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:32`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:33`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c:34`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** generic_lua_script_owner_v13.cpp:23 lists getter but requires native backend (:103-110). runtime_monster_level_policy_v1 consumes delivered range; does not establish generic Lua table selector result or max/min ordering.
- Source evidence: `port/level-world/character_host_context.cpp`:74 — `return dh2_script_vm_bind_source_values(vm,"GetCurrentLevelRange",range,const_cast<HostContextBindings16*>(b));`.
- Source evidence: `port/level-world/generic_lua_script_owner_v13.cpp`:22 — `{"GetCurrentLevelRange",0x37f1f0},{"GetGameObjectsByType",0x37f03c},{"SetGameType",0x37f878},{"GetGameScript",0x37c934},`.

**Concrete behavioral difference:** generic_lua_script_owner_v13.cpp:23 lists getter but requires native backend (:103-110). runtime_monster_level_policy_v1 consumes delivered range; does not establish generic Lua table selector result or max/min ordering.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: current Level table ID-1; GetCurrentLevelRange(2) Returns(-1,-1). Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:16, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:18, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:25, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:27, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:32, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:33, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:34.
2. **PROVEN:** Given: valid row fields+64=20,+52=7; GetCurrentLevelRange(1) Returns(20,7), preserving max/min source order. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:16, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:18, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:25, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:27, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:32, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:33, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:34.
3. **PROVEN:** Given: valid row; GetCurrentLevelRange(8) No Lua result. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:16, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:18, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:25, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:27, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:32, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:33, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f1f0.c`:34.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **14** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:182` | outside scoped roots | `GetCurrentLevelRange(pl_diff)` | `[{"index":1,"expression":"pl_diff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:305` | outside scoped roots | `GetCurrentLevelRange(pl_diff)` | `[{"index":1,"expression":"pl_diff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:338` | outside scoped roots | `GetCurrentLevelRange(pl_diff)` | `[{"index":1,"expression":"pl_diff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:31` | outside scoped roots | `GetCurrentLevelRange(pl_diff)` | `[{"index":1,"expression":"pl_diff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:251` | outside scoped roots | `GetCurrentLevelRange(pl_diff)` | `[{"index":1,"expression":"pl_diff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:25` | act1_level_boss_script_closure | `GetCurrentLevelRange(pl_diff)` | `[{"index":1,"expression":"pl_diff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/mothcocoon_core.luac:17` | outside scoped roots | `GetCurrentLevelRange(pl_diff)` | `[{"index":1,"expression":"pl_diff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:186` | outside scoped roots | `GetCurrentLevelRange(pl_diff)` | `[{"index":1,"expression":"pl_diff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:298` | outside scoped roots | `GetCurrentLevelRange(pl_diff)` | `[{"index":1,"expression":"pl_diff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:325` | outside scoped roots | `GetCurrentLevelRange(pl_diff)` | `[{"index":1,"expression":"pl_diff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:232` | outside scoped roots | `GetCurrentLevelRange(pl_diff)` | `[{"index":1,"expression":"pl_diff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:516` | act1_level_boss_script_closure | `GetCurrentLevelRange(pl_diff)` | `[{"index":1,"expression":"pl_diff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:267` | outside scoped roots | `GetCurrentLevelRange(pl_diff)` | `[{"index":1,"expression":"pl_diff","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:100` | act1_level_boss_script_closure | `GetCurrentLevelRange(pl_diff)` | `[{"index":1,"expression":"pl_diff","literal_tokens":[]}]` |

## Related natives

`LuaScript::_GetCurrentLevelRange`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
