# tcallback.mycallback

Contract status: **NOT-NEEDED**. Static IDA/Lua/port source investigation only. Frozen port status: **NOT-APPLICABLE**.

Engine functions to implement next ranking: **#233**.

## Binding and handler

**NOT A NATIVE:** source-identified Lua callback/table field; no Binder registration is expected.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
No explicit operator[] index was extracted. This does not prove a zero-argument callback.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: This expression calls the callback stored in the AI _commons AnimEvents entry; it is a Lua function field, not an engine Binder native. Evidence: `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:10`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:115`.

## Lua wrapper/helper traces

- AttachToAnimEvent stores callback as mycallback plus refData; OnAnimEvent looks up the event record and invokes mycallback(ev, refData). Evidence: `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:5`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:10`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:111`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:112`, `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:115`.

## Source-backed behavior cases

No source-backed native result case is asserted: the available source still needs semantic cases; this row remains TODO.

## Current frozen port source

**NOT-APPLICABLE:** No exact production-source identifier hit or curated callback/binding proof was found.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

No answer is promoted from a generic name or guessed semantics.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **1** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/_commons.luac:115` | autoloaded_lua_common_helpers | `tcallback.mycallback(ev, tcallback.refData)` | `[{"index":1,"expression":"ev","literal_tokens":[]},{"index":2,"expression":"tcallback.refData","literal_tokens":[]}]` |

## Related natives

No exact related name established.

## Unresolved reasons

- **NOT-NEEDED:** Source proves this scoped spelling is a Lua table callback field, so a native contract is not applicable.

## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
