# CF__CalcDodge

Contract status: **UNRESOLVED**. Static IDA/Lua/port source investigation only. Frozen port status: **MISSING**.

Engine functions to implement next ranking: **#158**.

## Binding and handler

**NOT-FOUND:** no exact Binder function/method registration was located.
No scoped Lua site; this row remains because it is in the ranked inventory.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
**PROVEN Lua signature/call shape:** `NOT-FOUND native ABI; Lua caller passes(attacker,defender,roll,attackType)`. This is a Lua-defined helper/table-field contract unless explicitly marked NOT-FOUND.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: CF_CalcDodge wrapper requires stored attacker/defender; otherwise returnsfalse. With both calls CF__CalcDodge. Exact Binder/function symbol/string search finds no CF__CalcDodge in Android pseudocode/registration exports. Nearby CF__CalcMissOrDodge0x3b1df0 has six args and combined semantics; it is not this four-arg binding. No supplied iPhone executable to establish a version-specific legacy API. Evidence: `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:773`.
**Gates:**
- PROVEN: The visible CF_CalcDodge Lua caller requires stored attacker and defender. Native admission gates are NOT-FOUND. Evidence: `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:773`.
**State, timers and events:**
- PROVEN: No native binding/body found; native state writes, timers and events remain unknown. Caller merely forwards four arguments.
**Return:**
- PROVEN: Lua wrapper returns false without combatants; with combatants it forwards unknown native results. Native return type/count is NOT-FOUND.

## Lua wrapper/helper traces

- PROVEN: Exact Binder public-name scan and unified original registration strings checked; no exact native name found. The retained original Lua call is a distinct undefined engine-looking symbol; no alias to similarly named CF__CalcMissOrDodge is proven. Evidence: `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:773`.

## Source-backed behavior cases

No source-backed native result case is asserted: the source binding/body is genuinely NOT-FOUND after the specific attempts below.

## Current frozen port source

**MISSING:** No exact CF__CalcDodge Lua callback is proven in the frozen port; CF__CalcMissOrDodge is a distinct six-argument combined calculation, so neither that service nor root push/stun probability cases establishes this missing four-argument API.

**Concrete behavioral difference:** No exact CF__CalcDodge Lua callback is proven in the frozen port; CF__CalcMissOrDodge is a distinct six-argument combined calculation, so neither that service nor root push/stun probability cases establishes this missing four-argument API.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

No answer is promoted from a generic name or guessed semantics.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **1** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:775` | outside scoped roots | `CF__CalcDodge(g_cf_data["attacker"], g_cf_data["defender"], roll, attackType)` | `[{"index":1,"expression":"g_cf_data[\"attacker\"]","literal_tokens":["\"attacker\""]},{"index":2,"expression":"g_cf_data[\"defender\"]","literal_tokens":["\"defender\""]},{"index":3,"expression":"roll","literal_tokens":[]},{"index":4,"expression":"attackType","literal_tokens":[]}]` |

## Related natives

No exact related name established.

## Unresolved reasons

- **UNRESOLVED:** NOT-FOUND after exact Binder/string/symbol search; only a four-argument Lua reference retained. Nearby six-argument CF__CalcMissOrDodge has no proven alias/adapter; dodge result semantics/address remain unavailable, not inferred from push/stun chances.

## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
