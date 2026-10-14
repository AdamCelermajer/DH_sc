# UpdatePlayerPosition

Contract status: **NOT-NEEDED**. Static IDA/Lua/port source investigation only. Frozen port status: **MISSING**.

Engine functions to implement next ranking: **#173**.

## Binding and handler

**NOT A NATIVE:** source-identified Lua callback/table field; no Binder registration is expected.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
**PROVEN Lua signature/call shape:** `UpdatePlayerPosition()`. This is a Lua-defined helper/table-field contract unless explicitly marked NOT-FOUND.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Two authored Lua global definitions: vip_core iterates player objects and uses FromFixed(squared-distance-name-query), safe radius950 removes debuffs, >30000 alive kills, >950 adds unique debuff and starts repeating1000ms timer once; monsters >7000 kills. Escort-big stores player records(obj), radius1250/global g_canApplyDebuff alternates applying/removing, >30000 kills regardless HP gate. These version-specific Lua rules are not a native distance-radius inference. Evidence: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/vip_core.luac:160`, `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee_vip_escort_big.luac:77`.
**Gates:**
- PROVEN: Lua definition/field type admission as described; not an independent engine registration. Evidence: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/vip_core.luac:160`, `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee_vip_escort_big.luac:77`.
**State, timers and events:**
- PROVEN: Lua table/context mutation and native calls only as described. Evidence: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/vip_core.luac:160`, `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee_vip_escort_big.luac:77`.
**Return:**
- PROVEN: Lua return as described; no native C signature because this row is a Lua helper/table callback. Evidence: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/vip_core.luac:160`, `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee_vip_escort_big.luac:77`.

## Lua wrapper/helper traces

- PROVEN: Exact Binder public-name scan and unified original registration strings checked; Lua definition/table field found at supplied source; real engine calls are dependencies below, not a hidden native of this spelling. Evidence: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/vip_core.luac:160`, `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee_vip_escort_big.luac:77`.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**MISSING:** This Lua helper needs its authored script loaded plus its reusable native dependencies; a same-name C++ identifier or generic native registration is not its implementation. Runtime execution not verified.

**Concrete behavioral difference:** This Lua helper needs its authored script loaded plus its reusable native dependencies; a same-name C++ identifier or generic native registration is not its implementation. Runtime execution not verified.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: vip_core player distance value30001 after FromFixed,HPpositive; UpdatePlayerPosition() Calls player Kill. Evidence: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/vip_core.luac`:160, `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee_vip_escort_big.luac`:77.
2. **PROVEN:** Given: escort-big distance exactly1250; UpdatePlayerPosition() Neither strict>1250 nor<1250 debuff branch runs. Evidence: `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/vip_core.luac`:160, `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee_vip_escort_big.luac`:77.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **1** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:200` | outside scoped roots | `UpdatePlayerPosition()` | `[]` |

## Related natives

No exact related name established.

## Unresolved reasons

- **NOT-NEEDED:** Exact original Lua definition/table field proves this ranked parser candidate is not an independent native; preserved in sealed inventory.

## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
