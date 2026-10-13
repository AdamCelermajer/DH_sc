# StealAIEvent

Contract status: **NOT-NEEDED**. Static IDA/Lua/port source investigation only. Frozen port status: **MISSING**.

## Binding and handler

**NOT A NATIVE:** source-identified Lua callback/table field; no Binder registration is expected.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
**PROVEN Lua signature/call shape:** `StealAIEvent(eventName,func)`. This is a Lua-defined helper/table-field contract unless explicitly marked NOT-FOUND.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Lua helper requires function. First listener creates global AIEventProxy_eventName and AddToVFTable(eventName,proxyname); appends(func,WORKING_ID). Proxy loops insertion order, temporarily sets SKILL_ID perlistener, passes originalargs, restores priorID after normal completion. Repeated registration appends duplicate listener; no dedupe. Does not independently intercept native events not propagated to Lua. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:294`.
**Gates:**
- PROVEN: Lua definition/field type admission as described; not an independent engine registration. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:294`.
**State, timers and events:**
- PROVEN: Lua table/context mutation and native calls only as described. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:294`.
**Return:**
- PROVEN: Lua return as described; no native C signature because this row is a Lua helper/table callback. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:294`.

## Lua wrapper/helper traces

- PROVEN: Exact Binder public-name scan and unified original registration strings checked; Lua definition/table field found at supplied source; real engine calls are dependencies below, not a hidden native of this spelling. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:294`.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**MISSING:** This Lua helper needs its authored script loaded plus its reusable native dependencies; a same-name C++ identifier or generic native registration is not its implementation. Runtime execution not verified.

**Concrete behavioral difference:** This Lua helper needs its authored script loaded plus its reusable native dependencies; a same-name C++ identifier or generic native registration is not its implementation. Runtime execution not verified.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: StealAIEvent("OnHit",7) No listener/alias installed. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac`:294.
2. **PROVEN:** Given: two valid registrations sameevent One alias/proxy installed, two listeners executed in insertionorder. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac`:294.
3. **PROVEN:** Given: dispatch currentSKILL_ID9,listener captured3 Listener sees3; normal return restores9. Evidence: `.local-inputs/assets-extra/android/data/scripts/skills/_commons.luac`:294.

## Original Lua call sites

Scoped exact original call ledger (supplement excludes outside-scope calls): **15** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_precision.luac:46` | outside scoped roots | `StealAIEvent("OnTargetHit", OnTargetHit_Skill)` | `[{"index":1,"expression":"\"OnTargetHit\"","literal_tokens":["\"OnTargetHit\""]},{"index":2,"expression":"OnTargetHit_Skill","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_sidestep.luac:132` | outside scoped roots | `StealAIEvent("OnTargetHit", OnTargetHit_Skill)` | `[{"index":1,"expression":"\"OnTargetHit\"","literal_tokens":["\"OnTargetHit\""]},{"index":2,"expression":"OnTargetHit_Skill","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:41` | outside scoped roots | `StealAIEvent("OnTargetHit", OnTargetHit_Skill)` | `[{"index":1,"expression":"\"OnTargetHit\"","literal_tokens":["\"OnTargetHit\""]},{"index":2,"expression":"OnTargetHit_Skill","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:41` | outside scoped roots | `StealAIEvent("OnTargetHit", OnTargetHit_Skill)` | `[{"index":1,"expression":"\"OnTargetHit\"","literal_tokens":["\"OnTargetHit\""]},{"index":2,"expression":"OnTargetHit_Skill","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_frenzy.luac:86` | outside scoped roots | `StealAIEvent("OnTargetHit", OnTargetHit_Skill)` | `[{"index":1,"expression":"\"OnTargetHit\"","literal_tokens":["\"OnTargetHit\""]},{"index":2,"expression":"OnTargetHit_Skill","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_killing_streak.luac:34` | outside scoped roots | `StealAIEvent("OnTargetHit", OnTargetHit_Skill)` | `[{"index":1,"expression":"\"OnTargetHit\"","literal_tokens":["\"OnTargetHit\""]},{"index":2,"expression":"OnTargetHit_Skill","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_survival.luac:53` | outside scoped roots | `StealAIEvent("OnTargetHit", OnTargetHit_Skill)` | `[{"index":1,"expression":"\"OnTargetHit\"","literal_tokens":["\"OnTargetHit\""]},{"index":2,"expression":"OnTargetHit_Skill","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_disciples.luac:61` | outside scoped roots | `StealAIEvent("OnTargetHit", OnTargetHit_Skill)` | `[{"index":1,"expression":"\"OnTargetHit\"","literal_tokens":["\"OnTargetHit\""]},{"index":2,"expression":"OnTargetHit_Skill","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_mirror.luac:40` | outside scoped roots | `StealAIEvent("OnTargetHit", OnTargetHit_Skill)` | `[{"index":1,"expression":"\"OnTargetHit\"","literal_tokens":["\"OnTargetHit\""]},{"index":2,"expression":"OnTargetHit_Skill","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_petrify.luac:54` | outside scoped roots | `StealAIEvent("OnTargetHit", OnTargetHit_Skill)` | `[{"index":1,"expression":"\"OnTargetHit\"","literal_tokens":["\"OnTargetHit\""]},{"index":2,"expression":"OnTargetHit_Skill","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_carrioneater.luac:38` | outside scoped roots | `StealAIEvent("OnTargetHit", OnTargetHit_Skill)` | `[{"index":1,"expression":"\"OnTargetHit\"","literal_tokens":["\"OnTargetHit\""]},{"index":2,"expression":"OnTargetHit_Skill","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_putrid.luac:43` | outside scoped roots | `StealAIEvent("OnTargetHit", OnTargetHit_Skill)` | `[{"index":1,"expression":"\"OnTargetHit\"","literal_tokens":["\"OnTargetHit\""]},{"index":2,"expression":"OnTargetHit_Skill","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_conversion.luac:149` | outside scoped roots | `StealAIEvent("OnTargetHit", OnTargetHit_Skill)` | `[{"index":1,"expression":"\"OnTargetHit\"","literal_tokens":["\"OnTargetHit\""]},{"index":2,"expression":"OnTargetHit_Skill","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_holy_weapon.luac:45` | outside scoped roots | `StealAIEvent("OnTargetHit", OnTargetHit_Skill)` | `[{"index":1,"expression":"\"OnTargetHit\"","literal_tokens":["\"OnTargetHit\""]},{"index":2,"expression":"OnTargetHit_Skill","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_shield.luac:146` | outside scoped roots | `StealAIEvent("OnTargetHit", OnTargetHit_Skill)` | `[{"index":1,"expression":"\"OnTargetHit\"","literal_tokens":["\"OnTargetHit\""]},{"index":2,"expression":"OnTargetHit_Skill","literal_tokens":[]}]` |

## Related natives

No exact related name established.

## Unresolved reasons

- **NOT-NEEDED:** Exact original Lua definition/table field proves this ranked parser candidate is not an independent native; preserved in sealed inventory.

## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
