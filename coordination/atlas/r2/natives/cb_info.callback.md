# cb_info.callback

Contract status: **NOT-NEEDED**. Static IDA/Lua/port source investigation only. Frozen port status: **NOT-APPLICABLE**.

Engine functions to implement next ranking: **#185**.

## Binding and handler

**NOT A NATIVE:** source-identified Lua callback/table field; no Binder registration is expected.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
No explicit operator[] index was extracted. This does not prove a zero-argument callback.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: This expression calls a callback field stored in the AI _commons TimerEvents record; it is a Lua function value, not a Binder native. Evidence: `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:32`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:99`.

## Lua wrapper/helper traces

- StartTimerCB stores callback in TimerEvents[timerId]; OnTimer(timerId) looks up the record and invokes cb_info.callback(timerId, cb_info.refData). Evidence: `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:26`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:30`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:32`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:95`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:96`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:99`.

## Source-backed behavior cases

No source-backed native result case is asserted: the available source still needs semantic cases; this row remains TODO.

## Current frozen port source

**NOT-APPLICABLE:** Identifier is present in production source but a complete reusable Lua callback/binding path is not proven.
- Source evidence: `port/windows-foundation/actor_population.cpp`:91 — `error = "Required external shared source RNG callback for CharacterTemplate selection";`.
- Source evidence: `port/windows-foundation/combat_session.cpp`:327 — `const auto callback=entry.stateServices.departed;`.
- Source evidence: `port/windows-foundation/combat_session.cpp`:337 — `if(callback&&!callback(id,from,next_state,error))return false;`.
- Source evidence: `port/windows-foundation/combat_session.cpp`:337 — `if(callback&&!callback(id,from,next_state,error))return false;`.
- Source evidence: `port/windows-foundation/combat_session.cpp`:344 — `const std::function<bool(ActorId,std::string&)>& callback,std::string& error){`.
- Source evidence: `port/windows-foundation/combat_session.cpp`:360 — `if(callback&&!callback(id,error))return false;`.
- Source evidence: `port/windows-foundation/combat_session.cpp`:360 — `if(callback&&!callback(id,error))return false;`.
- Source evidence: `port/windows-foundation/combat_session.cpp`:629 — `const auto callback=bound.stateServices.finished;`.
- Source evidence: `port/windows-foundation/combat_session.cpp`:634 — `if(!actor_transition(id,receipt,e,&callback))return false;`.
- Source evidence: `port/windows-foundation/combat_session.cpp`:638 — `const auto callback=bound.stateServices.finished;bound.stateSequence=false;`.
- Source evidence: `port/windows-foundation/combat_session.cpp`:649 — `if(callback&&!callback(id,e))return false;`.
- Source evidence: `port/windows-foundation/combat_session.cpp`:649 — `if(callback&&!callback(id,e))return false;`.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

No answer is promoted from a generic name or guessed semantics.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **1** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:99` | autoloaded_lua_common_helpers | `cb_info.callback(timerId, cb_info.refData)` | `[{"index":1,"expression":"timerId","literal_tokens":[]},{"index":2,"expression":"cb_info.refData","literal_tokens":[]}]` |

## Related natives

No exact related name established.

## Unresolved reasons

- **NOT-NEEDED:** Source proves this scoped spelling is a Lua table callback field, so a native contract is not applicable.

## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
