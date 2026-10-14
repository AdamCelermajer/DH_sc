# CallPyScript

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **MISSING**.

Engine functions to implement next ranking: **#24**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `CallPyScript` | bindFunction | `LuaScript::_CallPyScript` | `LuaScript::BindFunction` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037b5a0.c:112 |

Address-identified callback: `LuaScript::_CallPyScript` at `0x0037ef3c`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ef3c.c`, unified lines 94731-94761.
Recovered declaration: `int __fastcall LuaScript::_CallPyScript( int this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: Exact Lua string tag4; unit/scale Script bank name; case-insensitive ScriptManager lookup. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ef3c.c:15`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Gets first case-insensitive script-bank name match scanning all names from index0 (include global=1). Only ID != -1 and IsScriptRunning=false call StartScript(id,-1,0); this is a script-bank request, not an arbitrary .lua file include. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ef3c.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0045/004591f0.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0045/004591f0.c:13`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ef3c.c:25`.
- PROVEN: IsScriptRunning is state-word !=2. StartScript gates ID bounds and AllPlayersDead, allowing online with player count>1. With admitted offline startup it sets current module=-1, script cursor=0, state=0, increments active count. Online mode may send CMsgScriptCmd before local state setup; SkipAllScripts/debug tracing are queried. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0045/00455bec.c:3`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0046/004605c0.c:14`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0046/004605c0.c:32`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0046/004605c0.c:35`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0046/004605c0.c:52`.
**Gates:**
- PROVEN: Missing required argument or non-string required value emits zero Lua values and skips the manager lookup. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ef3c.c:15`.
- PROVEN: A found already-running script is not restarted. Unknown name does not call StartScript. StartScript admission is additional to wrapper checks. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ef3c.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ef3c.c:24`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0046/004605c0.c:12`.
**State, timers and events:**
- PROVEN: No Lua result push or timer start. Admitted script manager startup changes script execution state and may send an online command as listed above. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ef3c.c:25`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0046/004605c0.c:32`.
**Return:**
- PROVEN: Zero Lua outputs; decompiled C integer is never pushed through ReturnValues.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**MISSING:** GenericLuaScriptOwnerV13 contains descriptor0x37ef3c but no built-in switch implementation; it asks resolve_native or installs explicit required-backend failure. Exact address/name search in level-world/windows-foundation/level-loader found no production callback backend. An arbitrary Include substitute would not preserve script-bank execution gates.
- Source evidence: `port/level-world/generic_lua_script_owner_v13.cpp`:20.
- Source evidence: `port/level-world/generic_lua_script_owner_v13.cpp`:111.
- Source evidence: `port/level-world/generic_lua_script_owner_v13.cpp`:115.

**Concrete behavioral difference:** GenericLuaScriptOwnerV13 contains descriptor0x37ef3c but no built-in switch implementation; it asks resolve_native or installs explicit required-backend failure. Exact address/name search in level-world/windows-foundation/level-loader found no production callback backend. An arbitrary Include substitute would not preserve script-bank execution gates.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: Offline, live players, SkipAllScripts off; script Foo index3 state2; CallPyScript("fOo") Case-insensitive match3; cursor0/state0/module-1; active count increments once. No Lua outputs. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0045/004591f0.c`:16, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ef3c.c`:25, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0046/004605c0.c`:35, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0046/004605c0.c`:52.
2. **PROVEN:** Given: Foo already has state0; CallPyScript("Foo") No restart and no new active-count increment. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0045/00455bec.c`:3, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0037/0037ef3c.c`:24.
3. **PROVEN:** Given: Offline AllPlayersDead=true; known stopped Foo; CallPyScript("Foo") StartScript admission fails; script state and active count unchanged. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0046/004605c0.c`:14.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **17** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:401` | outside scoped roots | `CallPyScript("ResumeLevelMusic")` | `[{"index":1,"expression":"\"ResumeLevelMusic\"","literal_tokens":["\"ResumeLevelMusic\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:228` | outside scoped roots | `CallPyScript("ResumeLevelMusic")` | `[{"index":1,"expression":"\"ResumeLevelMusic\"","literal_tokens":["\"ResumeLevelMusic\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:356` | outside scoped roots | `CallPyScript("ResumeLevelMusic")` | `[{"index":1,"expression":"\"ResumeLevelMusic\"","literal_tokens":["\"ResumeLevelMusic\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:407` | outside scoped roots | `CallPyScript("ResumeLevelMusic")` | `[{"index":1,"expression":"\"ResumeLevelMusic\"","literal_tokens":["\"ResumeLevelMusic\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:236` | outside scoped roots | `CallPyScript("Boss_intro")` | `[{"index":1,"expression":"\"Boss_intro\"","literal_tokens":["\"Boss_intro\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:414` | outside scoped roots | `CallPyScript(name)` | `[{"index":1,"expression":"name","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:382` | outside scoped roots | `CallPyScript("end_MadrukPhase2")` | `[{"index":1,"expression":"\"end_MadrukPhase2\"","literal_tokens":["\"end_MadrukPhase2\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:429` | outside scoped roots | `CallPyScript("end_MadrukPhase2")` | `[{"index":1,"expression":"\"end_MadrukPhase2\"","literal_tokens":["\"end_MadrukPhase2\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:45` | outside scoped roots | `CallPyScript("start_phase_01_event_01")` | `[{"index":1,"expression":"\"start_phase_01_event_01\"","literal_tokens":["\"start_phase_01_event_01\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:47` | outside scoped roots | `CallPyScript("start_phase_01_event_02")` | `[{"index":1,"expression":"\"start_phase_01_event_02\"","literal_tokens":["\"start_phase_01_event_02\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:49` | outside scoped roots | `CallPyScript("start_phase_01_event_03")` | `[{"index":1,"expression":"\"start_phase_01_event_03\"","literal_tokens":["\"start_phase_01_event_03\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:124` | outside scoped roots | `CallPyScript("end_MadrukPhase1")` | `[{"index":1,"expression":"\"end_MadrukPhase1\"","literal_tokens":["\"end_MadrukPhase1\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:237` | outside scoped roots | `CallPyScript("ResumeLevelMusic")` | `[{"index":1,"expression":"\"ResumeLevelMusic\"","literal_tokens":["\"ResumeLevelMusic\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:336` | outside scoped roots | `CallPyScript("ResumeLevelMusic")` | `[{"index":1,"expression":"\"ResumeLevelMusic\"","literal_tokens":["\"ResumeLevelMusic\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruk_dyingattack.luac:42` | outside scoped roots | `CallPyScript("end_MadrukPhase2")` | `[{"index":1,"expression":"\"end_MadrukPhase2\"","literal_tokens":["\"end_MadrukPhase2\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_dyingattack.luac:43` | outside scoped roots | `CallPyScript("end_MadrukPhase3ToPhase2")` | `[{"index":1,"expression":"\"end_MadrukPhase3ToPhase2\"","literal_tokens":["\"end_MadrukPhase3ToPhase2\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/resgod_dying_attack.luac:65` | outside scoped roots | `CallPyScript("end_MadrukPhase1")` | `[{"index":1,"expression":"\"end_MadrukPhase1\"","literal_tokens":["\"end_MadrukPhase1\""]}]` |

## Related natives

`LuaScript::_CallPyScript`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
