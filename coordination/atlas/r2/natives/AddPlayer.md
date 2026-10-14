# AddPlayer

Contract status: **NOT-NEEDED**. Static IDA/Lua/port source investigation only. Frozen port status: **MISSING**.

Engine functions to implement next ranking: **#157**.

## Binding and handler

**NOT A NATIVE:** source-identified Lua callback/table field; no Binder registration is expected.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
**PROVEN Lua signature/call shape:** `AddPlayer(player)`. This is a Lua-defined helper/table-field contract unless explicitly marked NOT-FOUND.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Lua function assignment fills script players table keyed GetID; nil/already present no-op. Requires name begins "Player" via string.find(...,1,true)==1; then stores obj and increments player count. Escort-big version additionally SetMaster(player). Referee declares local prototype; that source does not prove imported globals fill its lexical local, so actual loading/scoping must be verified separately. Evidence: `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:82`, `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee_vip_escort_big.luac:43`, `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:86`.
**Gates:**
- PROVEN: Lua definition/field type admission as described; not an independent engine registration. Evidence: `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:82`, `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee_vip_escort_big.luac:43`, `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:86`.
**State, timers and events:**
- PROVEN: Lua table/context mutation and native calls only as described. Evidence: `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:82`, `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee_vip_escort_big.luac:43`, `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:86`.
**Return:**
- PROVEN: Lua return as described; no native C signature because this row is a Lua helper/table callback. Evidence: `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:82`, `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee_vip_escort_big.luac:43`, `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:86`.

## Lua wrapper/helper traces

- PROVEN: Exact Binder public-name scan and unified original registration strings checked; Lua definition/table field found at supplied source; real engine calls are dependencies below, not a hidden native of this spelling. Evidence: `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:82`, `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee_vip_escort_big.luac:43`, `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:86`.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**MISSING:** This Lua helper needs its authored script loaded plus its reusable native dependencies; a same-name C++ identifier or generic native registration is not its implementation. Runtime execution not verified.
- Source evidence: `port/level-world/application_player_manager_bootstrap_v59.cpp`:119 — `e="First-local setup skipped AddPlayer because internal ID 0 is mapped to a non-local PlayerInfo"`.
- Source evidence: `port/level-world/player_manager_combat_runtime_v2.cpp`:44 — `if(!record||record->internal670!=id){error_="Required source AddPlayer map publication";return false;}`.

**Concrete behavioral difference:** This Lua helper needs its authored script loaded plus its reusable native dependencies; a same-name C++ identifier or generic native registration is not its implementation. Runtime execution not verified.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: nil player; AddPlayer(nil) No players/count mutation. Evidence: `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac`:82, `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee_vip_escort_big.luac`:43, `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac`:86.
2. **PROVEN:** Given: new player namePlayer1/idP; AddPlayer(player) P stored once and total incremented; duplicate same identity ignored. Evidence: `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac`:82, `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee_vip_escort_big.luac`:43, `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac`:86.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **3** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:292` | outside scoped roots | `AddPlayer(target)` | `[{"index":1,"expression":"target","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee_vip_escort_big.luac:151` | outside scoped roots | `AddPlayer(friend)` | `[{"index":1,"expression":"friend","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/soundtest/referee_sdd.luac:59` | outside scoped roots | `AddPlayer(friend)` | `[{"index":1,"expression":"friend","literal_tokens":[]}]` |

## Related natives

No exact related name established.

## Unresolved reasons

- **NOT-NEEDED:** Exact original Lua definition/table field proves this ranked parser candidate is not an independent native; preserved in sealed inventory.

## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
