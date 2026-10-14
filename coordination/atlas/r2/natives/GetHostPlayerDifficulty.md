# GetHostPlayerDifficulty

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#23**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `GetHostPlayerDifficulty` | bindFunction | `LuaScript::_GetHostPlayerDifficulty` | `_ZN9LuaScript12BindFunctionEv` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:91876 |

Address-identified callback: `LuaScript::_GetHostPlayerDifficulty` at `0x0037cb8c`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c`, unified lines 92725-92739.
Recovered declaration: `void __fastcall LuaScript::_GetHostPlayerDifficulty( LuaScript *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument none: Receiver-only callback; extra Lua arguments ignored; unit/scale none. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c:9`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c:11`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c:13`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Reads current Application Level dword +280, or returns0 if no current Level. Despite name, no Player difficulty field is read. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c:9`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c:11`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c:13`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c:9`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c:11`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c:13`.
**State, timers and events:**
- PROVEN: Read-only callback; starts no timers and raises no events. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c:9`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c:11`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c:13`.
**Return:**
- PROVEN: One Lua value as described; no state mutation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c:9`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c:11`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c:13`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** generic_lua_script_owner_v13.cpp:22 registers this callback but :103-110 requires supplied native backend or returns explicit required-service failure. Selected gameplay reads elsewhere do not establish complete global Lua callback behavior.
- Source evidence: `port/level-world/character_host_context.cpp`:73 — `result=dh2_script_vm_bind_source_values(vm,"GetHostPlayerDifficulty",difficulty,const_cast<HostContextBindings16*>(b));if(result)return result;`.
- Source evidence: `port/level-world/generic_lua_script_owner_v13.cpp`:21 — `{"GetNumPlayers",0x37cbd8},{"GetHostPlayer",0x37ca60},{"GetHostPlayerLevel",0x37cc00},{"GetHostPlayerDifficulty",0x37cb8c},`.

**Concrete behavioral difference:** generic_lua_script_owner_v13.cpp:22 registers this callback but :103-110 requires supplied native backend or returns explicit required-service failure. Selected gameplay reads elsewhere do not establish complete global Lua callback behavior.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: no current Level; GetHostPlayerDifficulty() Returns0. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c`:9, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c`:10, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c`:11, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c`:13.
2. **PROVEN:** Given: current Level difficulty field2; GetHostPlayerDifficulty() Returns2. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c`:9, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c`:10, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c`:11, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037cb8c.c`:13.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **15** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:340` | outside scoped roots | `GetHostPlayerDifficulty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:180` | outside scoped roots | `GetHostPlayerDifficulty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:303` | outside scoped roots | `GetHostPlayerDifficulty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:336` | outside scoped roots | `GetHostPlayerDifficulty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:29` | outside scoped roots | `GetHostPlayerDifficulty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:249` | outside scoped roots | `GetHostPlayerDifficulty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:23` | act1_level_boss_script_closure | `GetHostPlayerDifficulty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/mothcocoon_core.luac:15` | outside scoped roots | `GetHostPlayerDifficulty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:184` | outside scoped roots | `GetHostPlayerDifficulty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:296` | outside scoped roots | `GetHostPlayerDifficulty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:323` | outside scoped roots | `GetHostPlayerDifficulty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:230` | outside scoped roots | `GetHostPlayerDifficulty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:514` | act1_level_boss_script_closure | `GetHostPlayerDifficulty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:265` | outside scoped roots | `GetHostPlayerDifficulty()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:98` | act1_level_boss_script_closure | `GetHostPlayerDifficulty()` | `[]` |

## Related natives

`LuaScript::_GetHostPlayerDifficulty`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
