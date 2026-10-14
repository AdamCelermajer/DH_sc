# ChangeAIState

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#85**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `ChangeAIState` | bindFunction | `CharAIScript::_ChangeState` | `_ZN12CharAIScript24CharAIScriptBindFunctionEv` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:156352 |

Address-identified callback: `CharAIScript::_ChangeState` at `0x003d9710`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c`, unified lines 156756-156781.
Recovered declaration: `void __fastcall CharAIScript::_ChangeState( CharAIScript *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: exactly1 value converted getString; unit/scale registered AI state name. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:14`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Requires exactly1 arg, converts getString (no tag guard), looks up named registered AI state. Unknown state no-op. Found state calls current state post callback, updates state pointer, calls new state init; even same-name change performs post/init. Init reads state+68 callback string, post state+92; calls LuaScript::Call, not numeric FSM event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:12`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:14`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8e78.c:5`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8e8c.c:5`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:12`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:14`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8e78.c:5`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8e8c.c:5`.
**State, timers and events:**
- PROVEN: No Lua result; state pointer and synchronous post/init script calls, no native wrapper timer. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:12`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:14`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8e78.c:5`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8e8c.c:5`.
**Return:**
- PROVEN: No Lua result; state pointer and synchronous post/init script calls, no native wrapper timer. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:12`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:14`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:18`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:21`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c:22`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8e78.c:5`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8e8c.c:5`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** Selected enemy scripts use custom state providers; generic CharAIScript state registry callback path is distinct from Character FSM events. Catalog/operation name does not implement source string map post/init ordering.
- Source evidence: `port/level-world/character_script_session.cpp`:317 — `if((!std::strcmp(b.name,"RegisterAIState")&&b.original_callback==0x3da144u)||(!std::strcmp(b.name,"ChangeAIState")&&b.original_callback==0x3d9710u))return true; //SAME private Session installed this exact source entry before delivery.`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:236 — `if((!std::strcmp(b.name,"RegisterAIState")&&b.original_callback==0x3da144u)||(!std::strcmp(b.name,"ChangeAIState")&&b.original_callback==0x3d9710u))return true; //SAME private Session installed this exact source entry before delivery.`.
- Source evidence: `port/level-world/character_script_states.cpp`:54 — `if(!name){if(capacity)std::snprintf(error,capacity,"native ChangeAIState numeric/identity getString coercion unsupported");return 1;}`.
- Source evidence: `port/level-world/character_script_states.cpp`:56 — `try{const ScriptStatesCalls24 calls{&t,&scoped_call,nullptr};const auto result=t.self->change_name(name,calls);if(result<0&&capacity)std::snprintf(error,capacity,"native ChangeAIState failure %d",result);return result<0;}catch(const std::ba`.
- Source evidence: `port/level-world/character_script_states.cpp`:56 — `try{const ScriptStatesCalls24 calls{&t,&scoped_call,nullptr};const auto result=t.self->change_name(name,calls);if(result<0&&capacity)std::snprintf(error,capacity,"native ChangeAIState failure %d",result);return result<0;}catch(const std::ba`.
- Source evidence: `port/level-world/character_script_states.cpp`:81 — `int ScriptStates::bind_source_entry_v108(dh2_script_vm* vm,const dh2_script_aliases* aliases,bool change){if(!vm||!aliases||(impl_->vm&&impl_->vm!=vm)||(impl_->aliases&&impl_->aliases!=aliases))return -1;impl_->vm=vm;impl_->aliases=aliases;`.

**Concrete behavioral difference:** Selected enemy scripts use custom state providers; generic CharAIScript state registry callback path is distinct from Character FSM events. Catalog/operation name does not implement source string map post/init ordering.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: ChangeAIState("missing") with current stateA State remainsA; no post/init. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c`:12, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c`:14, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c`:18, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c`:20, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c`:21, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8e78.c`:5, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8e8c.c`:5.
2. **PROVEN:** Given: registered B,currentA; ChangeAIState("B") Calls A post then publishes B then B init. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c`:12, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c`:14, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c`:18, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c`:20, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c`:21, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8e78.c`:5, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8e8c.c`:5.
3. **PROVEN:** Given: registered A,currentA; ChangeAIState("A") Calls A post then A init; same state is not suppressed. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c`:12, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c`:14, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c`:18, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c`:20, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c`:21, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d9710.c`:22, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8e78.c`:5, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003d8e8c.c`:5.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **1** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/miniboss.luac:18` | outside scoped roots | `ChangeAIState("StateBasic")` | `[{"index":1,"expression":"\"StateBasic\"","literal_tokens":["\"StateBasic\""]}]` |

## Related natives

`CharAIScript::_ChangeState`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
