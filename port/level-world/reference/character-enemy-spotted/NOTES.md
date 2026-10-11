# Source CharAI enemy-spotted gate

## Scope and source ownership

`character_enemy_spotted.{hpp,cpp}` implements one bounded source gate/dispatch
from `CharAI::OnEnemySpotted(Character*)` (480 bytes at `0x3d14b4`). It owns the
ordering of group notification, state gates, optional threat addition, and late
selected-AIS dispatch. The GroupInfo notification body is implemented separately
by `character_group_info_owner_v1.{hpp,cpp}` from source function
`0x3d27cc`; the native event-9 adapter wires both owners in `model_renderer.cpp`.
Candidate search, complete combat producers, universal aggro storage, native Lua
bindings, and the complete external-AI lifecycle remain outside these kernels.

The exact source body includes debug-switch/string services and stack-canary
code. A supplied `debug_switch` adapter runs the entry and positive-add diagnostic
sequences in source order. An absent adapter explicitly excludes that diagnostic
subsystem; its value never gates gameplay. No full 480-byte body claim is made.
The manifest's dependency ranges are evidence, not extra reconstructed bodies.

## Exact source sequence

| Original instruction / field | Rebuilt operation |
| --- | --- |
| `0x3d14e8..0x3d1514` | Optional entry diagnostic service. |
| `0x3d1518`, AI `+0x34`; `0x3d1524`, AI `+4` | Capture current group and owner; notify `GroupInfo::OnEnemySpotted(group, owner, enemy)` at `0x3d152c` before state gates. |
| `0x3d153c`, `0x3d1570` | Query enemy, then freshly loaded owner `SM_IsAwaitingToSpawn`, at `0x3c0230`; state ID **17**. |
| `0x3d1580`, `0x3d1598` | Query enemy, then separately freshly loaded owner `SM_IsInLimbus`, at `0x3c01c0`; state ID **0**. |
| `0x3d15a8` | Call `AI_IsInCombat` on the original fixed AI identity. |
| `0x3d15c0` | Only when in combat: call the enemy's current virtual `+0x28`, `IsPlayer`; a false result skips the aggro query/add, then continues to dispatch. |
| `0x3d15d4` | Otherwise call the fixed AI's `AI_GetAggro(enemy)`. |
| `0x3d15dc` | `__aeabi_fcmpeq(threat, +0.0f)` at PLT `0x30df8c`. Only equality takes the add branch; both signs of zero qualify, NaN and nonzero values do not. |
| `0x3d15ec..0x3d1604` | Capture owner afresh, then read design global `+0x30`, then call **that captured owner's** embedded CharAI (`owner+0x3c8`) `AI_AddAggro(enemy, amount)`. |
| `0x3d160c` | `__aeabi_fcmpgt(delta, +0.0f)` at PLT `0x30e2f8`. Positive delta requests the second diagnostic sequence; all normal outcomes continue to dispatch. |
| `0x3d1618..0x3d1634` | Read the current selected AIS from AI `+0x1c` after preceding calls; null returns normally. Dispatch that AIS's virtual `+0x34` with the fixed enemy argument. |

The first two predicates are **not dead predicates**. The verified symbol and
instructions at `0x3c0230` compare state 17. Actual `SM_IsDead` is `0x3c03f0`,
compares state 12, and is not called here. State 12 passes the two source gates
in the bounded oracle. This is only this handler's acceptance behavior; separate
event producers and GroupInfo can apply their own gates.

`AI_GetAggro` (`0x3d4ac8`) searches the outgoing red-black tree rooted in AI's
`+0x7c/+0x80` map and returns the stored binary32 threat, or positive zero when
missing. This call is read-only; it is not an inserting `getAt` operation.
`AI_AddAggro` (`0x3d7c68`) performs actual relationship mutation through
`AI_SetAggro` and returns a threat delta. It must run on the zero-threat branch
even when the returned value appears to be used only by diagnostics. The new
kernel delegates this mutation to an explicit service rather than replacing it
with a query or changing selected targets itself.

`AI_IsInCombat` (`0x3d4bc4`) queries `AI_HasAggro`, `AI_IsAggroed`, and owner
state attack/skill/casting conditions in source order. That service's body remains
external. State services must read current state-machine storage on each query;
`is_player` must resolve the current enemy virtual provider at its taken call.
No precomputed dead/combat snapshot is accepted as evidence for those producers.

## Lifetime, mutation, reentry and failure

The caller retains the AI, fixed enemy argument, context and every owner/group
projection exposed until the synchronous operation finishes. Owner/group/AIS
fields may change through callbacks. The source AI identity and enemy argument
are captured; owner and selected AIS are read at the exact later source points.
The adapter must update the selected AIS identity and callable as a coherent pair.

Only after the source selects a non-null AIS does the port acquire a hold for
that exact AIS/callee pair. Retain must hold the callable's backing controller or
Lua session, including a retired selection during replacement. Dispatch uses
the captured pair even if retain or dispatch changes the current selection.
The hold is released after normal dispatch, failed dispatch, or exception. This
is portable object ownership, not an added source gameplay cleanup. No hold of
an earlier selected AIS delays source selection or routes an event to stale AI.

`in_progress` is a port guard, not an original object field or recovered source
rule. Same-state nested calls reject before effects; different states may nest.
Providers must not modify that guard or the result. Source service tables are
copied once, and borrowed storage must remain coherent and live for the call.
This component is single-threaded and does not claim concurrent replacement.

Top-level nulls, zero enemy/AI identities and overlapping state/services/result
reject before effects. Null owner is rejected when a taken path consumes it;
null current AIS returns normally without demanding dispatch services. Missing
services reject only on taken paths. Callback errors and exceptions preserve all
earlier source and provider mutations; there is no rollback or extra aggro clear.
Partial counters describe attempted calls. The active-hold service is the only
port cleanup performed during unwinding.

## Tests and reproduction

The host fixture covers 38 cases: all four source state gates; a dead-state
counterexample; group-before-gates order; source bool truthiness; combat/player
short-circuiting; NaN, infinity, negative threat, both zero signs and returned
delta edges; owner mutation during group/query/design calls; selected AIS
replacement during AddAggro/diagnostics/retention/dispatch; held session lifetime;
copied service-table ownership; partial failure, exception, missing services,
reentry and top-level aliases.

The optional original ARM comparison executes the gate at `0x3d1518` to normal
convergence `0x3d1548`, including real original `SM_GetState`,
`SM_IsAwaitingToSpawn` and `SM_IsInLimbus` instructions. It compares 24 source
traces, decisions, captured AddAggro owners and selected AIS identities to the
compiled host kernel. Group/combat/aggro/virtual services are observed; imported
soft-float comparisons are modeled, diagnostic bodies and the initial prefix
are omitted. The range hashes and declared symbols/sizes are verified against
the original ELF. This does not run the original APK or prove full AI behavior.

```powershell
python port/level-world/tests/run_character_enemy_spotted_host.py `
  --compiler C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe `
  --original-elf C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/test_strategy/libDungeonHunter2.so `
  --output port/level-world/build/character-enemy-spotted-review/host.exe `
  --report port/level-world/build/character-enemy-spotted-review/validation.json
```

## Next native boundary

`character_ai_events` supplies event-9 routing but does not implement this body.
Its CharAI OnEnemySpotted service can call this gate after exposing live owner,
group, state, aggro and selected AIS storage. The final dispatch can call
`monster_external_script::Session::dispatch(Event::enemy_spotted, enemy)` for
the selected plain `monster` AISExternal, using this hold to retain that session.
The existing original Lua body then performs `HasTarget`, optional `SetTarget`
and `HeadTo`. The gate does not synthesize candidates or substitute AISMonster.

The native adapter currently binds GroupInfo member state-17 queries and spawn
transitions through each member's existing retained `SpawnOwner`/Coordinator.
That provider is available for an object with a live native ObjectActor; group
members without that retained owner fail explicitly after preserving earlier
source-order effects. The adapter also binds only its supported combat/aggro
providers and selected monster AIS session. Candidate production, coverage of
other Character providers such as Faery controllers, and a compatible production
link boundary for the multiple raw Lua cores remain unresolved. Static source and
host-fixture evidence do not establish gameplay behavior in the original engine
or a running native build.
