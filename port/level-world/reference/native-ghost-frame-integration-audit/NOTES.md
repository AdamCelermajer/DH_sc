# Native Ghost frame integration audit

This is a read-only integration plan after the frozen `Character::CanUpdate`
review. It adds no reconstructed function, provider body, native activation,
or gameplay-parity credit. Original addresses refer to ELF SHA-256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The draft is separate from the current APK/source milestone.

## Current integration and the same-VM boundary

[`NativeMonsterInitialization`](../../../android-native/app/src/main/cpp/model_renderer.cpp)
owns the selected AIS, source initialization fields, real class/property and
Debug backends, skill owner, and its initialized Lua VM. The renderer currently
includes `ghost_ai_owner.hpp` but does not construct, bind, or tick that frame
owner. Its initialization `construct_service` calls `vm.create(script_services())`:
these are direct `NativeMonsterInitialization` callbacks, including explicit
unbound state/path/controller callbacks.

[`Owner::bind_staged`](../../../android-native/app/src/main/cpp/ghost_ai_owner.cpp)
requires a matching earlier `prepare_pending`, and
[`ActorSession::adopt_staged`](../../ghost_ai_session.cpp) verifies
`Session::uses_services` against the prepared callback table. The renderer
still creates its VM with direct `NativeMonsterInitialization` callbacks and
never installs the Ghost Owner table or constructs/ticks that frame owner.

The source AISExternal constructor creates its Lua resource before
`SetScript<AISExternal>` (`0x3ccaf4/240`) stores the returned pending pointer:
construction is at `0x3ccb68`, and the pending store is at `0x3ccb6c`. The
same-VM ordering constraint is now supported by
[`Session::install_created_services`](../monster-created-service-install/NOTES.md):
create the VM, publish the real pending identity, prepare and retain the
matching Ghost callback context, install it before binding/loading, then adopt
the exact ready VM. This bridge has focused host coverage; the Android renderer
has not been wired to it.

The fresh-owner integration target is:

1. Retain a heap-stable native Ghost frame `Owner`, its source projections,
   callback contexts, lifecycle, and real property/world owners.
2. Allocate the selected AIS identity and create the Lua VM at the original
   constructor point, retaining unbound callback storage. Publish pending only
   after the source constructor returns. Do not move the source pending store
   ahead of VM construction to make a port preparation predicate pass.
3. With the matching `ghost_ai_session::Bindings` and genuinely published
   pending identity, call `Owner::prepare_pending`. Install the returned
   service table **and returned callback lifetime** into the same still
   unbound VM through `Session::install_created_services`.
4. Advance original binding, SetCharacter, common/external loads, InitVCB,
   OnInit, active publication, HP/MP, skills, post, and final stages using this
   same VM. Retain the original source ordering rather than reloading chunks.
5. Refresh the active projection after source publication, then call
   `Owner::bind_staged` with the same bindings and that exact ready VM.
6. On graphics recreation, rebind only volatile actor/property/visual/body
   pointers after storage exists. Retain source target/aggro/timers/VM state;
   do not replay OnInit or SetLevel, create a replacement VM, or heal actors.

Already initialized VMs from the older direct-table path need an explicit
adapter migration/rebinding design or must remain in their bounded initialized
mode until a fresh world. Replaying initialization to force adoption is not a
valid migration. See the source constructor and association evidence in
[`ais-external-initialization`](../ais-external-initialization/original-functions.json)
and the [script lifecycle](../character-script-lifecycle/NOTES.md).

## Implemented pre-binding service bridge

`Session::install_created_services` replaces callbacks only on the same-owner,
created/unbound VM and retains their lifetime before releasing the prior
context. It preserves VM identity, aliases, cache and stage. The focused host
runner covers stage/owner guards, same-VM callback replacement, source Lua,
retirement/reentry and exact Owner prepare/install/adopt composition; see
[`monster-created-service-install`](../monster-created-service-install/NOTES.md).
This port ownership adapter adds no reconstructed original-function credit.

## Real provider closure required before a live frame

| Required boundary | Current evidence and missing native owner work |
|---|---|
| Eligibility | `Character::CanUpdate` `0x3a52a4/316` is source-tested. Bind the actual captured Visual/current root, visibility byte `+0x80`, raw GetOnline byte, local PlayerInfo Character, dead/respawn, and culling services. `ObjectBase::TestCullingBeforeUpdate` `0x33de90/432` is a separate missing complete provider; it owns byte `+0x86` and may query six live camera planes. A distance check or constant true is not that provider. |
| Room and zoning | Source frame `0x3cfbf4` checks `+0x2ee` at `0x3cfc84` and `+0x2f0` at `0x3cfd34` after IsZonable. The renderer's search synchronization currently writes proven constructor values `1/0`, so a zonable Ghost correctly skips its frame. Bind actual room/zone producers and preserve their live fields across synchronization. DACT/PFRoom indices and TriggerZone contact do not establish RoomZone ownership. |
| Shared turn queue and clock | Retain one application-owned `character_ai_queue::State`, source constructor registration order, live owner readiness, `GetDt`, and `character_ai_turn` globals. Native constructor `queue_order` is only a registration record today. Run source `IncUpdateQueue` after ObjectManager actors, as Level call site `0x3f84ec` shows; do not advance once per Ghost or before this frame's actors. |
| Frame fields and target/master | Read source flags `+0x520`, controller `+8/+9`, pause `+0x18`, real zoning and updated byte `+0x88`. Reuse complete target/master callers and exact sight/range predicates. Constructor-null master is valid only until a genuine setter changes it. Keep one canonical target/requested/last/alive/sight/sticky storage used by every Lua getter, setter, update, and acquisition projection. |
| Acquisition, relations and notifications | Compose the verified timed prefix, real flat ObjectManager Character list, target search/consumer, relations, event 9 relay and EnemySpotted gate. Bind actual raw property/classification/interactive/queue/Debug queries and real outgoing/incoming aggro mutation plus required OnAggro/OnDeAggro dispatch. The existing numerical map helper reports notification requests; that alone is not a completed callback. Retarget/retention must remain their source branches on later frames. |
| Lua and AIS updates | `AISExternal::OnUpdate` `0x3dce64/64` uses AISDefault `0x3dc798/64`, exact InitVCB `b8`, actual collision-produced `bc`, and independently fresh `b4` state calls. Reuse Session update/state interfaces. Null `b4` comes from the constructor and unchanged original script registration; keep a retained live projection rather than treating every unsupported state as null. Preserve zero explicit Lua arguments for OnUpdate. |
| Controller, path and body | Original monster EnemySpotted invokes SetTarget then HeadTo; out-of-range uses fresh Idle/HasPath/GetTarget before MoveTo. Bind source controller gates/events and existing PathTo to the owned live route/floor. Stop requires the new source `GameObject::Stop` caller `0x3938f8/248` and genuine DropPath, physics-position policy, velocity, transform and final body-reset owners. Its virtual `+0x64` is `IsUpdatingPositionFromPhysics`: GameObject address point `0x964750` resolves to `0x34006c` (returns 1); Character address point `0x965f38` resolves to `0x3a2e44` (live flags `+0x520`, bit 1). A generic movable constant is not this predicate. The separate frozen Stop caller replay passed 246 cases and all 59 active instructions; its DropPath/body providers still need native wiring. |
| Character OnUpdate effects | The caller may require `DisableZoning` `0x38c600/156`, `EnableZoning` `0x38c790/236`, `VisualObject::SyncVisibility` `0x4713d0/108`, real positions and zone/list notifications. None may be reported complete through a successful diagnostic-only callback. |

The renderer's `NativeMonsterInitialization::has_target/get_target` currently
read constructor projection `ai->state.target_40`. A future ActorSession setter
operates `character::set_target::State`. These must not become independent
live targets. Also refresh `AIUpdateState80` after source event/controller
callbacks change the actual FSM, flags, target, zoning or visual; an initial
per-frame snapshot is insufficient for later source reads.

The source slot-20B `ObjectBase::IsRemotelyUpdated` at `0x33dd10` tests word
`+0x110` against `-1`, otherwise returns byte `+0x118`. Offline mode may skip
that branch in CanUpdate, but controller MoveTo still requires its own genuine
remote query. A source predicate's unused path is not an invented false fact.

## Proposed next runtime milestone

Bind one retained Crypt Ghost through the prepared same-VM path, then run its
actual eligible Character phase in this order:

**scene/root animation â†’ one world Step â†’ eligibility â†’ timers â†’ CharAI
frame (target, master, aggro, OnUpdate) â†’ FSM â†’ animator â†’ GameObject path,
rotation, subobjects/target cache â†’ shared AI queue advance.**

Original Character call sites are `0x3ac02c` (timers), `0x3ac034` (AI),
`0x3ac03c` (FSM), `0x3ac048` (animator), and `0x3ac054` (GameObject). The
current native Ghost timers/FSM loop and later draw-time Ghost animator/path
loop must be composed per eligible actor; appending AI after all FSMs would
break this order. See the [complete read-only frame trace](../frame-order/NOTES.md).

A reviewable live gate should show source selection of the Prince from the
real list, original EnemySpotted â†’ unchanged Lua â†’ SetTarget/HeadTo, actual
owned route/body displacement, subsequent target/retention updates, and
normal Stop. The same two Ghost owners must retain their VM, target, HP/MP and
route lifetime safely through reload/rotation without duplicate initialization
or timers. Close the listed taken-path providers first; failures remain explicit
and no autonomous-native-AI claim is made by this note.

For a disjoint next source increment, the missing culling caller plus its
source remotely-updated leaf, or the zoning/visibility callers, directly close
these real frame prerequisites. They are not new implementations in this note.
