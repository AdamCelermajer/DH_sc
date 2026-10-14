# GetHostPlayer

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#117**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `GetHostPlayer` | bindFunction | `LuaScript::_GetHostPlayer` | `_ZN9LuaScript12BindFunctionEv` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:91866 |

Address-identified callback: `LuaScript::_GetHostPlayer` at `0x0037ca60`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ca60.c`, unified lines 92681-92692.
Recovered declaration: `void __fastcall LuaScript::_GetHostPlayer( LuaScript *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument none: Receiver-only callback; extra Lua arguments ignored; unit/scale none. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ca60.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ca60.c:9`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ca60.c:10`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Fetches PlayerManager hosting Player and pushes its Character userdata at Player+1632; no missing-player null check in callback. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ca60.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ca60.c:9`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ca60.c:10`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ca60.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ca60.c:9`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ca60.c:10`.
**State, timers and events:**
- PROVEN: Read-only callback; starts no timers and raises no events. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ca60.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ca60.c:9`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ca60.c:10`.
**Return:**
- PROVEN: One Lua value as described; no state mutation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ca60.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ca60.c:9`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ca60.c:10`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** generic_lua_script_owner_v13.cpp:22 registers this callback but :103-110 requires supplied native backend or returns explicit required-service failure. Selected gameplay reads elsewhere do not establish complete global Lua callback behavior.
- Source evidence: `port/windows-foundation/features/companions/live_companions.cpp`:51 — `if(!b.host_player){error="Required source GetHostPlayer producer";return false;}`.
- Source evidence: `port/windows-foundation/features/companions/live_companion_scripts.cpp`:118 — `{"GetHostPlayer",Operation::host_player,false,true},{"IsMasterHostPlayer",Operation::is_master_host_player,true,false},`.
- Source evidence: `port/windows-foundation/features/companions/source_owner_scripts.cpp`:58 — `{"GetHostPlayer",0x37ca60},{"IsMasterHostPlayer",0x3b6fc4},{"HasPath",0x38e98c},{"GetState",0x3b6d78},`.
- Source evidence: `port/level-world/generic_lua_script_owner_v13.cpp`:21 — `{"GetNumPlayers",0x37cbd8},{"GetHostPlayer",0x37ca60},{"GetHostPlayerLevel",0x37cc00},{"GetHostPlayerDifficulty",0x37cb8c},`.

**Concrete behavioral difference:** generic_lua_script_owner_v13.cpp:22 registers this callback but :103-110 requires supplied native backend or returns explicit required-service failure. Selected gameplay reads elsewhere do not establish complete global Lua callback behavior.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: hosting Player owns Character H; GetHostPlayer() Pushes userdata H. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ca60.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ca60.c`:9, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ca60.c`:10.
2. **PROVEN:** Given: hosting Player exists but Character null; GetHostPlayer() Pushes null userdata; does not fall back to receiver. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ca60.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ca60.c`:9, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ca60.c`:10.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **3** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:60` | outside scoped roots | `GetHostPlayer()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:144` | outside scoped roots | `GetHostPlayer()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:138` | act1_level_boss_script_closure | `GetHostPlayer()` | `[]` |

## Related natives

`LuaScript::_GetHostPlayer`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
