# Normal aggro candidate events

## Scope

Root's `character_aggro_candidate_events.{hpp,cpp}` reconstructs the bounded
normal `_UpdateAggro` candidate-consumption loop after source TargetList search.
It uses the new fixed all-relationship/Character-only/closest TargetList module;
it does not implement search geometry, turn/delay selection, type-specific
branches, relationship classifier bodies, aggro storage, or event acceptance.
This is one new bounded source branch, not the full 2,052-byte `_UpdateAggro`
body, and it is not native wired. Dependencies' original ranges are evidence,
not additional newly reconstructed bodies.

## Original operation map

| Original instructions | Bounded consumer |
| --- | --- |
| `0x3cf670..0x3cf67c` | Test whether source candidate queue is empty; empty proceeds to current AI `+0x40` check. |
| `0x3cf684`, constructor `0x3cf618` | Initialize local no-enemy flag to 1. |
| `0x3cf6e0..0x3cf704` | Caller requires Character bit 0 set in each TargetInfo; diagnostic assertion/crash modes for other flags are outside this kernel. |
| `0x3cf704` | Capture candidate from **TargetInfo `+0` GameObject identity**. |
| `0x3cf708..0x3cf714` | Fresh owner `AI+4`, embedded owner AI `+0x3c8`, `AI_IsEnemy(candidate)` at `0x3d574c`. |
| `0x3cf6b4..0x3cf6c4` | Enemy: fresh owner; `RaiseEvent(9, candidate)` before pop; set no-enemy flag to 0 after the normal event call returns. |
| `0x3cf720..0x3cf72c` | Otherwise fresh owner and `AI_IsFriend(candidate)` at `0x3d511c`. |
| `0x3cf738..0x3cf744` | Friend: fresh owner, `RaiseEvent(7, candidate)` before pop. |
| `0x3cf8ec..0x3cf8f8` | Otherwise fresh owner and `AI_IsNeutral(candidate)` at `0x3d5a98`. |
| `0x3cf904..0x3cf910` | Neutral: fresh owner, `RaiseEvent(8, candidate)` before pop. |
| `0x3cf6c8..0x3cf6dc` | Source priority-queue pop at `0x38fb18`; continue until every candidate is consumed, including later enemies. |
| `0x3cf924..0x3cf928` | If an enemy event was normally raised, return through list cleanup; otherwise check AI `+0x40`. |
| `0x3cf974..0x3cf988` | Read AI `+0x40` freshly; when nonzero, fresh owner and `RaiseEvent(0x0c, that identity)` with the actual identity as R2 payload. |

The port's `TargetInfo.character_identity` is resolution metadata for source
Character validation. It does not replace source `TargetInfo+0` for classifier
arguments or event payload. The tests deliberately supply different values to
make an accidental substitution observable.

All three relation predicates can be truthy in a synthetic adapter; the source
calls enemy first, friend only if not enemy, neutral only if neither enemy nor
friend. A candidate with no truthy relation is still removed. A notified enemy
does not end the loop. No-enemy/empty results do not fabricate event 12 when
current AI `+0x40` is zero, and do not substitute a null event payload.

Original `Character::RaiseEvent` (`0x3a4d5c`) relays these event IDs to the
embedded owner's `CharAI::RaiseAIEvent` at `0x3cbb34`, preserving the payload.
The existing `character_ai_events` kernel reconstructs that dispatcher, including
its own gates. This consumer emits events and does not bypass those gates or
directly invoke monster Lua.

## Ownership and errors

The list is the equivalent of the original stack-local TargetList. Its backing
heap, owner projection/object, exposed identities and service context must stay
live until return. Owner and `+0x40` may change through callbacks; their later
reads are fresh. Callbacks must not destroy, mutate or reenter this same list.
Independent lists/outputs may nest while using current live State, as the
source's nested `_UpdateAggro` would construct a separate local list.

Root corrected the consumer's preflight to match the TargetList pop contract:
alignment before typed reads, capacity 1..65,536, coherent owner/object/heap,
count bounds, sort/reserved fields, pointer-range overflow and overlap checks,
and nonzero flag-1 candidate identities. Empty count still needs an initialized
positive-capacity list. Known malformed input rejects before provider calls or
result writes; no zero-capacity special case is accepted.

The copied service table survives callback replacement. Missing providers are
required only on taken branches. Provider failure/exception returns failure and
retains prior effects; the current candidate remains unpopped. Earlier completed
candidates remain consumed. An event-12 failure after the loop leaves the list
empty. Counters record completed events and consumed candidates, and attempted
classification calls. No rollback, extra event, or aggro clear is introduced.

## Validation

The C++ fixture checks 33 cases: relation precedence and raw truthiness; multiple
enemies without early break; actual source search/heap/pop integration with
nearest order; source object identity despite different resolution metadata;
event-before-pop count/top observations; fresh owner and fresh `+0x40`; empty
and no-relation results; service-table replacement; independent-list nested
dispatch; errors/exceptions preserving partial effects; and malformed pointer,
capacity, owner/object, candidate flag/identity, alignment and alias guards.

The optional ARM branch oracle compares 16 original instruction traces to the
compiled consumer. It executes the normal loop from `0x3cf670` through its
enemy/friend/neutral/no-enemy branches until cleanup convergence `0x3cf92c`.
The original `Character::RaiseEvent` wrapper also executes, and its embedded-AI
relay arguments are checked. Relation classifiers, CharAI event delivery and
priority-queue pop are observed external services. The fixture presents a known
nearest-first queue and advances it on observed pop; it does not run original
heap/deque/search/assertion/destructor/timing code. Host search/heap mechanics
execute separately, and equal-distance ordering is not claimed stable.

The runner verifies the original ELF SHA, seven declared original symbols,
sizes and body hashes, and records every compiled source/header plus test,
runner and reference hash. No original APK/Android/live AI result is claimed.

```powershell
python port/level-world/tests/run_character_aggro_candidate_events_host.py `
  --compiler C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe `
  --original-elf C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/test_strategy/libDungeonHunter2.so `
  --output port/level-world/build/character-aggro-candidate-events-review/host.exe `
  --report port/level-world/build/character-aggro-candidate-events-review/validation.json
```

## Native work still required

Integrate real room/actor owner projections, fresh classifiers, source search and
timing/type decisions, and Character event services. Event 9 then reaches the
new `character_enemy_spotted` gate and selected AISExternal/session through the
existing source dispatcher. Keep current AIS/Lua session ownership coherent;
do not add a synthetic first-enemy selector or AISMonster substitute.
