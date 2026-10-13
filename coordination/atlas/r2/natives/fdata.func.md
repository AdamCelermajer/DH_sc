# fdata.func

Contract status: **NOT-NEEDED**. Static IDA/Lua/port source investigation only. Frozen port status: **NOT-APPLICABLE**.

Engine functions to implement next ranking: **#192**.

## Binding and handler

**NOT A NATIVE:** source-identified Lua callback/table field; no Binder registration is expected.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
No explicit operator[] index was extracted. This does not prove a zero-argument callback.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: This expression iterates the skill _commons AI event listener records and invokes each record's Lua func field. It is not a Binder native. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:306`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:309`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:318`.

## Lua wrapper/helper traces

- StealAIEvent stores each listener as {func=func, skillId=WORKING_ID}; its generated AIEventProxy temporarily sets SKILL_ID from each record before invoking fdata.func(...). Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:294`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:301`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:306`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:307`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:309`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:311`, `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:318`.

## Source-backed behavior cases

No source-backed native result case is asserted: the available source still needs semantic cases; this row remains TODO.

## Current frozen port source

**NOT-APPLICABLE:** Identifier is present in production source but a complete reusable Lua callback/binding path is not proven.
- Source evidence: `port/script-runtime/lua/lapi.c`:63 — `Closure *func = curr_func(L);`.
- Source evidence: `port/script-runtime/lua/lapi.c`:64 — `sethvalue(L, &L->env, func->c.env);`.
- Source evidence: `port/script-runtime/lua/lapi.c`:69 — `Closure *func = curr_func(L);`.
- Source evidence: `port/script-runtime/lua/lapi.c`:71 — `return (idx <= func->c.nupvalues)`.
- Source evidence: `port/script-runtime/lua/lapi.c`:72 — `? &func->c.upvalue[idx-1]`.
- Source evidence: `port/script-runtime/lua/lapi.c`:83 — `Closure *func = curr_func(L);`.
- Source evidence: `port/script-runtime/lua/lapi.c`:84 — `return func->c.env;`.
- Source evidence: `port/script-runtime/lua/lapi.c`:213 — `Closure *func = curr_func(L);`.
- Source evidence: `port/script-runtime/lua/lapi.c`:215 — `func->c.env = hvalue(L->top - 1);`.
- Source evidence: `port/script-runtime/lua/lapi.c`:216 — `luaC_barrier(L, func, L->top - 1);`.
- Source evidence: `port/script-runtime/lua/lapi.c`:777 — `StkId func;`.
- Source evidence: `port/script-runtime/lua/lapi.c`:781 — `func = L->top - (nargs+1);`.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

No answer is promoted from a generic name or guessed semantics.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **1** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:309` | autoloaded_lua_common_helpers | `fdata.func(...)` | `[{"index":1,"expression":"...","literal_tokens":[]}]` |

## Related natives

No exact related name established.

## Unresolved reasons

- **NOT-NEEDED:** Source proves this scoped spelling is a Lua table callback field, so a native contract is not applicable.

## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
