# refData.callback

Contract status: **NOT-NEEDED**. Static IDA/Lua/port source investigation only. Frozen port status: **NOT-APPLICABLE**.

Engine functions to implement next ranking: **#154**.

## Binding and handler

**NOT A NATIVE:** source-identified Lua callback/table field; no Binder registration is expected.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
No explicit operator[] index was extracted. This does not prove a zero-argument callback.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: This expression invokes the callback field passed through skills _commons timer or animation-event wrappers. The wrapper temporarily sets SKILL_ID to the captured skill ID and restores the prior value after callback delivery. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:136`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:140`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:141`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:155`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:158`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:159`.

## Lua wrapper/helper traces

- StartSkillTimerCB packages callback, skill_id and refData before forwarding through AI StartTimerCB; SkillTimerCB_ dispatches refData.callback(timerId, refData.refData). AttachToSkillAnimEvent uses the analogous animation callback wrapper. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:136`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:146`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:147`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:149`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:164`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:167`.

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

Global exact-name/function-ID ledger, including calls outside the selected roots: **2** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:140` | autoloaded_lua_common_helpers | `refData.callback(timerId, refData.refData)` | `[{"index":1,"expression":"timerId","literal_tokens":[]},{"index":2,"expression":"refData.refData","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:158` | autoloaded_lua_common_helpers | `refData.callback(event_name, refData.refData)` | `[{"index":1,"expression":"event_name","literal_tokens":[]},{"index":2,"expression":"refData.refData","literal_tokens":[]}]` |

## Related natives

No exact related name established.

## Unresolved reasons

- **NOT-NEEDED:** Source proves this scoped spelling is a Lua table callback field, so a native contract is not applicable.

## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
