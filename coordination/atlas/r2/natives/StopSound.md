# StopSound

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `StopSound` | bindFunction | `LuaScript::_StopSound` | `_ZN9LuaScript12BindFunctionEv` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:91916 |

Address-identified callback: `LuaScript::_StopSound` at `0x0037e730`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c`, unified lines 94325-94370.
Recovered declaration: `int __fastcall LuaScript::_StopSound( LuaScript *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4, int a5, int a6, char a7)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: getString required; unit/scale Sounds row name. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:32`.
- Lua argument 2: getNumber required for known row; unit/scale fade milliseconds. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:41`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Converts required sound name to exact Sounds row. Miss-1 no-op and second arg not read. Known row requires arg2 number truncated fade milliseconds, invokes manager Stop. Java stops row ID; Vox requires existing ready data handle then fetches up to10 emitter handles and stops each with fade/1000 seconds. Does not unload asset or stop all sounds globally. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:23`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:28`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:36`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:41`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:42`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:44`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0036/00369fec.c:22`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:23`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:28`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:36`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:41`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:42`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:44`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0036/00369fec.c:22`.
**State, timers and events:**
- PROVEN: No Lua result; conditional row emitters stopped, no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:23`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:28`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:36`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:41`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:42`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:44`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0036/00369fec.c:22`.
**Return:**
- PROVEN: No Lua result; conditional row emitters stopped, no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:23`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:28`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:31`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:36`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:41`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:42`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c:44`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0036/00369fec.c:22`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** generic_lua_script_owner_v13.cpp:25 lists native but delegates source backend (:103-110); selected sound APIs do not prove per-row up-to10 emitter stopping/fade handling.
- Source evidence: `port/windows-foundation/features/bosses/runtime_swampking_core_ai_v1.cpp`:286 — `if (!music_call(body.at(vf.at("OnDied")), "StopSound", next.died_stop_music, next.died_stop_fade_ms) ||`.
- Source evidence: `port/windows-foundation/features/campaign_host/campaign_host.cpp`:739 — `std::cout << "[campaign] StopSound dispatch music=" << sound.music << " ordinal=" << sound.ordinal << " fade_ms=" << sound.fade_ms`.
- Source evidence: `port/windows-foundation/features/campaign_host/campaign_host.cpp`:742 — `unsupported_.note("StopSound not applied: " + detail);`.
- Source evidence: `port/level-world/generic_lua_script_owner_v13.cpp`:23 — `{"OnTargetDied",0x37ddc8},{"PlayMusic",0x37e420},{"PlaySound",0x37e578},{"StopSound",0x37e730}};`.

**Concrete behavioral difference:** generic_lua_script_owner_v13.cpp:25 lists native but delegates source backend (:103-110); selected sound APIs do not prove per-row up-to10 emitter stopping/fade handling.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: StopSound("missing") No Stop manager call and fade arg not accessed. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c`:23, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c`:28, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c`:31, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c`:36, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c`:41, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c`:42, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c`:44, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0036/00369fec.c`:22.
2. **PROVEN:** Given: known row S,existing ready handle2 emitters; StopSound(S,250) Stops both emitters with0.25-second fade. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c`:23, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c`:28, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c`:31, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c`:36, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c`:41, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c`:42, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c`:44, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0036/00369fec.c`:22.
3. **PROVEN:** Given: known S but no loaded handle; StopSound(S,0) No Vox emitter stop/load. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c`:23, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c`:28, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c`:31, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c`:36, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c`:41, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c`:42, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037e730.c`:44, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0036/00369fec.c`:22.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **4** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:88` | act1_level_boss_script_closure | `StopSound("SwampKingMusic", 2000)` | `[{"index":1,"expression":"\"SwampKingMusic\"","literal_tokens":["\"SwampKingMusic\""]},{"index":2,"expression":"2000","literal_tokens":["2000"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:652` | act1_level_boss_script_closure | `StopSound("SwampKingMusic", 2000)` | `[{"index":1,"expression":"\"SwampKingMusic\"","literal_tokens":["\"SwampKingMusic\""]},{"index":2,"expression":"2000","literal_tokens":["2000"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:682` | act1_level_boss_script_closure | `StopSound("SwampKingMusic", 2000)` | `[{"index":1,"expression":"\"SwampKingMusic\"","literal_tokens":["\"SwampKingMusic\""]},{"index":2,"expression":"2000","literal_tokens":["2000"]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:721` | act1_level_boss_script_closure | `StopSound("SwampKingMusic", 2000)` | `[{"index":1,"expression":"\"SwampKingMusic\"","literal_tokens":["\"SwampKingMusic\""]},{"index":2,"expression":"2000","literal_tokens":["2000"]}]` |

## Related natives

`LuaScript::_StopSound`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
