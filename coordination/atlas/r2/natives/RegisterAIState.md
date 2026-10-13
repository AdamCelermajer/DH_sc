# RegisterAIState

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#86**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `RegisterAIState` | bindFunction | `CharAIScript::_RegisterState` | `_ZN12CharAIScript24CharAIScriptBindFunctionEv` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:156347 |

Address-identified callback: `CharAIScript::_RegisterState` at `0x003da144`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`, unified lines 157434-157556.
Recovered declaration: `int __fastcall CharAIScript::_RegisterState( CharAIScript *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1..5: 2..5 all string tag4; unit/scale state name then callbacks(update/init/check/post source offsets). Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:43`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Accepts2..5 arguments all string tag4. First indexes named CharAIScript state map; args2..5 assign four callback strings in order. Omitted callback strings in existing state are retained, not reset; new map value default construction supplies absent fields. No state activation/callback call in registration. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:36`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:45`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:47`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:50`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:56`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:63`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:69`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:71`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:75`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:36`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:45`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:47`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:50`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:56`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:63`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:69`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:71`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:75`.
**State, timers and events:**
- PROVEN: No Lua value; named state callback strings only; no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:36`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:45`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:47`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:50`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:56`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:63`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:69`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:71`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:75`.
**Return:**
- PROVEN: No Lua value; named state callback strings only; no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:36`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:45`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:47`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:50`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:56`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:63`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:69`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:71`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c:75`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** Generic source AI script callback registry not installed by CharacterScriptObjects; selected state machine providers use own tables. Preserve partial re-registration and2..5 all-string gate rather than building numeric FSM transitions.
- Source evidence: `port/level-world/character_script_session.cpp`:317 — `if((!std::strcmp(b.name,"RegisterAIState")&&b.original_callback==0x3da144u)||(!std::strcmp(b.name,"ChangeAIState")&&b.original_callback==0x3d9710u))return true; //SAME private Session installed this exact source entry before delivery.`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:236 — `if((!std::strcmp(b.name,"RegisterAIState")&&b.original_callback==0x3da144u)||(!std::strcmp(b.name,"ChangeAIState")&&b.original_callback==0x3d9710u))return true; //SAME private Session installed this exact source entry before delivery.`.
- Source evidence: `port/level-world/character_script_states.cpp`:48 — `static int registration(void* pointer,const dh2_script_callback_scope* scope,const dh2_script_value* args,std::uint32_t count,char* error,std::size_t capacity){auto& t=*static_cast<Impl*>(pointer);if(scope->vm!=t.vm||!dh2_script_callback_sc`.
- Source evidence: `port/level-world/character_script_states.cpp`:81 — `int ScriptStates::bind_source_entry_v108(dh2_script_vm* vm,const dh2_script_aliases* aliases,bool change){if(!vm||!aliases||(impl_->vm&&impl_->vm!=vm)||(impl_->aliases&&impl_->aliases!=aliases))return -1;impl_->vm=vm;impl_->aliases=aliases;`.

**Concrete behavioral difference:** Generic source AI script callback registry not installed by CharacterScriptObjects; selected state machine providers use own tables. Preserve partial re-registration and2..5 all-string gate rather than building numeric FSM transitions.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: RegisterAIState("A","update") Creates/updates state A first callback only; does not activate. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:36, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:45, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:47, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:50, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:56, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:63, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:69, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:71, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:75.
2. **PROVEN:** Given: existing A four callbacks; RegisterAIState("A","new") Replaces first callback; other three retained. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:36, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:45, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:47, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:50, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:56, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:63, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:69, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:71, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:75.
3. **PROVEN:** Given: RegisterAIState("A",7) No map write; all args muststrings. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:36, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:45, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:47, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:50, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:56, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:63, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:69, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:71, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003d/003da144.c`:75.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **1** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/miniboss.luac:15` | outside scoped roots | `RegisterAIState("StateBasic",\t\t\t\t"StateBasicUpdate",\t\t\t\t"StateBasicConditions",\t\t\t\t\t"StateBasicInit",\t\t\t\t"StateInitPost")` | `[{"index":1,"expression":"\"StateBasic\"","literal_tokens":["\"StateBasic\""]},{"index":2,"expression":"\"StateBasicUpdate\"","literal_tokens":["\"StateBasicUpdate\""]},{"index":3,"expression":"\"StateBasicConditions\"","literal_tokens":["\"StateBasicConditions\""]},{"index":4,"expression":"\"StateBasicInit\"","literal_tokens":["\"StateBasicInit\""]},{"index":5,"expression":"\"StateInitPost\"","literal_tokens":["\"StateInitPost\""]}]` |

## Related natives

`CharAIScript::_RegisterState`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
