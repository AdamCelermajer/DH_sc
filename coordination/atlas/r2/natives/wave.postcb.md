# wave.postcb

Contract status: **NOT-NEEDED**. Static IDA/Lua/port source investigation only. Frozen port status: **MISSING**.

Engine functions to implement next ranking: **#240**.

## Binding and handler

**NOT A NATIVE:** source-identified Lua callback/table field; no Binder registration is expected.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
**PROVEN Lua signature/call shape:** `wave.postcb(waveId,extraTime,data)`. This is a Lua-defined helper/table-field contract unless explicitly marked NOT-FOUND.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Table-field callback called only typefunction; receives current wave ID,g_wave_time,wave.postcbdata before advancing wave. Authored waves_sdd PostCB placeholder has empty body; no registration required. Evidence: `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:350`, `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/waves_sdd.luac:6`.
**Gates:**
- PROVEN: Lua definition/field type admission as described; not an independent engine registration. Evidence: `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:350`, `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/waves_sdd.luac:6`.
**State, timers and events:**
- PROVEN: Lua table/context mutation and native calls only as described. Evidence: `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:350`, `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/waves_sdd.luac:6`.
**Return:**
- PROVEN: Lua return as described; no native C signature because this row is a Lua helper/table callback. Evidence: `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:350`, `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/waves_sdd.luac:6`.

## Lua wrapper/helper traces

- PROVEN: Exact Binder public-name scan and unified original registration strings checked; Lua definition/table field found at supplied source; real engine calls are dependencies below, not a hidden native of this spelling. Evidence: `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:350`, `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/waves_sdd.luac:6`.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**MISSING:** This Lua helper needs its authored script loaded plus its reusable native dependencies; a same-name C++ identifier or generic native registration is not its implementation. Runtime execution not verified.

**Concrete behavioral difference:** This Lua helper needs its authored script loaded plus its reusable native dependencies; a same-name C++ identifier or generic native registration is not its implementation. Runtime execution not verified.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: wave.postcb=7 No callback invoked. Evidence: `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac`:350, `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/waves_sdd.luac`:6.
2. **PROVEN:** Given: wave.postcb=function(...) capture end Receives(current_wave,g_wave_time,postcbdata) in that order. Evidence: `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac`:350, `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/waves_sdd.luac`:6.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **1** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:351` | outside scoped roots | `wave.postcb(g_current_wave, g_wave_time, wave.postcbdata)` | `[{"index":1,"expression":"g_current_wave","literal_tokens":[]},{"index":2,"expression":"g_wave_time","literal_tokens":[]},{"index":3,"expression":"wave.postcbdata","literal_tokens":[]}]` |

## Related natives

No exact related name established.

## Unresolved reasons

- **NOT-NEEDED:** Exact original Lua definition/table field proves this ranked parser candidate is not an independent native; preserved in sealed inventory.

## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
