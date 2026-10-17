# Actor-owned Ghost AI session bridge

This renderer-independent host bridge composes existing recovered source
kernels and the unchanged external `monster` Lua script for one stable actor.
It does not claim a complete native AI or a full source `Character::Update`.

## What the bridge runs

`ActorSession::bind` owns one `monster_external_script::Session` per actor and binds
typed borrowed projections for the current CharAI, Character, selected AIS,
event dispatcher, controller, and path services. `search_and_dispatch` runs
the already initialized filter-2/closest aggro search, classifies candidates
in source order (Enemy, Friend, Neutral), raises the corresponding AI events,
and uses `CharAI::OnEnemySpotted` plus the original Lua
`monster_OnEnemySpotted` callback. Lua `SetTarget` goes through the recovered
`CharAI::AI_SetTarget` kernel. Lua `HeadTo`/`MoveTo` runs the recovered
Character controller-command and `GameObject::PathTo` kernels.

The owner must call only after the source `_UpdateAggro` prefix selects the
normal candidate-search branch. It supplies the selected radius/cone and a
source initialized TargetList. The newly reconstructed acquisition-prefix
module is an available caller-side producer; this bridge does not invoke it
yet. It does not implement the delay/tick prefix, the local-Monster existing
target branch, `IsMyTurn` queue ownership, `_UpdateMaster`, `_UpdateTarget`,
the full AIS lifecycle, or the full `Character::Update` loop.

The event and script seams remain explicit. The bridge intercepts event 9's
CharAI virtual +0x34 and executes the bounded `OnEnemySpotted` gate. Other
AI-event operations—including event 0x0c—go to the caller's existing event
service. The source `RaiseEvent` wrapper is a 28-byte relay into
`RaiseAIEvent`; the bridge uses the recovered dispatcher for it rather than
inventing an event acceptance rule.

## Event 0x0c detail

When the candidate loop finds no notified enemy and the current CharAI+0x40 is
non-null, `_UpdateAggro` raises `RaiseEvent(0x0c, current_target)`. The
`RaiseAIEvent` source dispatcher maps event 0x0c to CharAI virtual slot +0x48.
That source virtual is no-argument `CharAI::OnTargetOutOfSight()`; the
dispatcher does not forward the event's pointer argument to the vfunc. In the
original image, `_ZTV6CharAI` begins at 0x966790, its address point is 0x966798,
and address-point slot +0x48 at 0x9667e0 points to 0x3d2410. The dispatch
branch at 0x3cc170..0x3cc180 calls that slot without moving the incoming
payload into the virtual-call argument. The host fixture verifies the no-arg
slot receives payload zero while the current target projection contains the
fresh value read after the search provider. It does not implement
`OnTargetOutOfSight`'s body.

## Ownership and failure boundary

The source lifecycle also has a staged path. `prepare_staged` creates the
stable callback adapter before an AIS VM exists; `staged_services` returns the
exact copied service table plus a shared lifetime lease for that adapter.
After the AIS-owned `Session` reaches `external_loaded`, `adopt_staged` checks
that its complete service table matches and points the actor bridge at that
same VM. This avoids creating a second VM for the pending-to-active AIS
transition. The external Session must outlive this ActorSession binding. The
shared lease keeps the callback context alive through VM teardown even if the
ActorSession wrapper is reset first. This stage-sharing API is host-tested; the
native AIS lifecycle has not yet wired it.

`prepare_pending` adds the actual pending-before-active contract. Its selected
AIS identity names the borrowed lifecycle's pending field while script getters
execute; the enemy active projection continues to name the existing active
field, including null. Acquisition is unavailable during this phase. Adoption
requires the source lifecycle to publish that same pending identity to active
and the caller to refresh the enemy active identity/callee. It then retains the
same VM and checks the lifecycle active field on every callback. Pending
replacement, owner replacement, or later active replacement rejects stale
callbacks. The lifecycle and all its backing storage must outlive both the
borrowed VM and the callback lease, including after wrapper reset. Scan output
may not overlap the borrowed lifecycle projection.

Each actor owns a separate Lua VM and alias table in the standalone `bind`
convenience path. A rebind creates a candidate
VM before replacing the old binding; failed setup preserves the previous
session. Synchronous rebind/reset while a source callback is active is
rejected. Before committing `ScanResult`, the bridge rejects overlap with
known state, owner, controller, path, room-registry, target-list, and target
heap projections. The caller must also keep dynamic room nodes, object/AI
tables, and service backing storage disjoint from the output and alive for the
complete synchronous call. A provider/service error stops at that point and
preserves earlier game-state effects; there is no rollback.

Active-AIS identity is captured at bind and must remain stable for the
synchronous source callback. A changed selected AIS/owner/CharAI is reported
as a stale binding. This is an adapter lifetime rule, not a claim that original
game re-entry behaves identically.

`GetProp` transports the raw integer property result unchanged as the VM's
numeric input. Existing Lua `FromFixed` applies the recovered conversion; the
native provider must not pre-shift or multiply the SkillTree property ID/value.
Neutral Lua identity tables are only object-ID transport. Real methods,
relations, target list objects, Character state, and PathTo remain supplied by
typed source-owned providers.

## Validation

Run:

```powershell
python port/level-world/tests/run_ghost_ai_session_host.py
```

The runner compiles the source modules with warning-as-error flags and executes
the unchanged `_commons` and `monster` script bytes. Thirteen host case groups
pass: source search through FindPath, two independent actor VMs/target IDs,
stale-owner rejection/rebind, reentrant rebind rejection, partial effects on
late PathTo failure, fresh post-search and post-relation target state for
event 0x0c, and output
alias rejection, staged VM sharing, actual lifecycle publication, pending and
active replacement checks, and lifecycle output alias rejection. It pins both
script inputs and emits a generated validation
JSON under `port/level-world/build/`.

This is host validation with explicit fixture providers. `native_wired` is
false; it does not test Android or an emulator. The native actor currently
returns early from `update_enemy` for gated Ghosts.

## Next Android adapter boundary

The smallest useful live step is **normal acquisition through the first real
enemy event**, using stable, actor-owned GameObject/Character projections:

1. Keep `ObjectActor`, `SpawnOwner`, their `PropertyState`, scheduler, and
   coordinator at stable addresses for each loaded Ghost. Store the session
   and all adapter projections in `SpawnOwner` (or an owned object whose
   address does not change when `object_groups` vectors move).
2. Add a per-world registry of stable `aggro_search::GameObject` and
   `aggro_search::Character` views, including the player. Populate source
   position/target-position/facing/visibility and map GameObject identities
   to live Character/property/state owners. Rebuild intrusive room links only
   at a documented world-update boundary, and retain them through synchronous
   search/dispatch.
3. Bind relation services to existing faction/AI data (`AiTables`, current
   Character property projections, `GetCharAIFactionId` semantics and source
   interaction virtuals), not Euclidean player shortcuts. Bind the actual
   source selected AIS/session and its owner lifetime.
4. Consume the frozen acquisition-prefix decision and its captured owner and
   radius only for `ready_normal_acquisition`; explicitly reject unsupported
   local-Monster retarget branches. Run the actor-owned `TargetList`, then
   dispatch event 9 into this bridge. First prove the live search can select
   Prince and pass the real `OnEnemySpotted`/Lua target assignment.

The first integration still needs real services for the OnEnemySpotted gates
(group, awaiting/Limbus, combat, player, aggro reads/writes, selected AIS),
DebugSwitches and AI sight used by `AI_SetTarget`, and the source movement
controller. `PathTo` is reconstructed, but the native Ghost has no genuine
controller target-position/facing, navigation route replacement, and
kinematic/physical movement path bound yet. Do not accept a successful Lua
`HeadTo` call as movement until those services update the live actor pose and
body. Do not route Ghosts through the older proximity/legacy combat code.

The current `SpawnOwner::service` deliberately leaves `idle_common_update`
without AI dispatch, while `update_enemy` returns immediately for
`gated_spawn`. That is the immediate integration gap this composition is
intended to close once stable native projections and real providers exist.
