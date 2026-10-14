# GetGameObjectsByType

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#74**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `GetGameObjectsByType` | bindFunction | `LuaScript::_GetGameObjectsByType` | `_ZN9LuaScript12BindFunctionEv` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:91886 |

Address-identified callback: `LuaScript::_GetGameObjectsByType` at `0x0037f03c`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`, unified lines 94787-94892.
Recovered declaration: `void __fastcall LuaScript::_GetGameObjectsByType( LuaScript *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: string tag4; unit/scale exact class name. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:46`.
- Lua argument 2: optional number tag3 -> unsigned; unit/scale skip matching entries. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:31`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Requires string arg1 exact PropertyMap class name. Optional numeric arg2 unsigned skip count, default0. Iterates global ObjectManager ordered registry; skips missing/non-GameObject handles and nonmatching classes; skips requested number of matching objects, then pushes at most15 userdata (v13<=14 loop permits fifteenth). Returns multiple Lua objects, not an array/table. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:29`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:41`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:43`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:45`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:48`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:55`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:57`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:60`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:29`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:41`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:43`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:45`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:48`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:55`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:57`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:60`.
**State, timers and events:**
- PROVEN: Zero to15 userdata values; read-only registry traversal, no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:29`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:41`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:43`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:45`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:48`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:55`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:57`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:60`.
**Return:**
- PROVEN: Zero to15 userdata values; read-only registry traversal, no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:29`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:41`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:43`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:45`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:48`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:55`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:57`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c:60`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** generic_lua_script_owner_v13.cpp:23 delegates callback backend, not a built-in enumeration; selected scene registry lookups do not establish class-name matching, skip semantics or15-value cap.
- Source evidence: `port/level-world/generic_lua_script_owner_v13.cpp`:22 — `{"GetCurrentLevelRange",0x37f1f0},{"GetGameObjectsByType",0x37f03c},{"SetGameType",0x37f878},{"GetGameScript",0x37c934},`.

**Concrete behavioral difference:** generic_lua_script_owner_v13.cpp:23 delegates callback backend, not a built-in enumeration; selected scene registry lookups do not establish class-name matching, skip semantics or15-value cap.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: registry has2 matching Character objects A,B; GetGameObjectsByType("Character",1) Returns only B. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:29, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:31, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:41, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:43, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:45, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:48, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:55, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:57, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:60.
2. **PROVEN:** Given: registry has20 matching objects; GetGameObjectsByType("Character") Returns first15 in source registry order. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:29, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:31, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:41, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:43, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:45, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:48, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:55, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:57, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:60.
3. **PROVEN:** Given: GetGameObjectsByType(5) No Lua values. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:29, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:31, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:41, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:43, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:45, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:48, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:55, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:57, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037f03c.c`:60.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **3** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/vip_core.luac:58` | outside scoped roots | `GetGameObjectsByType("Dummy", num_found)` | `[{"index":1,"expression":"\"Dummy\"","literal_tokens":["\"Dummy\""]},{"index":2,"expression":"num_found","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:130` | outside scoped roots | `GetGameObjectsByType("Dummy", num_found)` | `[{"index":1,"expression":"\"Dummy\"","literal_tokens":["\"Dummy\""]},{"index":2,"expression":"num_found","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:52` | outside scoped roots | `GetGameObjectsByType("Dummy", num_found)` | `[{"index":1,"expression":"\"Dummy\"","literal_tokens":["\"Dummy\""]},{"index":2,"expression":"num_found","literal_tokens":[]}]` |

## Related natives

`LuaScript::_GetGameObjectsByType`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
