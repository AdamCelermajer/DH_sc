# Player AI timer events and regeneration caller

## Original input and attribution

The pinned original ELF SHA256 is
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The manifest pins 16 function bodies and three vtables. The ARM capture runs
25 cases through the actual Character/CharAI route and reaches 220 distinct
pinned instruction words. This is bounded instruction coverage, not complete
execution of all manifested constructors, ordinary events, or frame handlers.
Debug/string, cached property, HP/MP regeneration, dead and combat application
calls in that capture are explicit service fixtures.

Adam's source snapshots c3ae797332a82a30a586b9156cddc25445e36a4c and
791e961b12233100b303038c961666834f4beb9d were audited. The latter V6 Player
implementation reuses its V3 source and private skill/buff/timer owner. This
adaptation instead composes the maintained CharAI event, remote getter,
HandleDots, regeneration, Debug, property and DoT attack implementations with
the current sole Coordinator/Session/PropertyView/buff owners. No Adam private
VM, timer, property, inventory or Save owner is imported. Aggro and state query
leaves use the existing actual typed fields; no additional query leaf body
credit is claimed. New caller coverage is Character::RegenTick and the narrow
_UpdateRegen/GetInCombat composition.

## Actual route and source order

CharTimers::Update (0x3db640, 612 bytes) calls Character::RaiseEvent
(0x3a4d5c, 28 bytes). Event 0x36 goes directly to properties BuffExpired
(0x3e123c). Other events reach CharAI::RaiseAIEvent (0x3cbb34, 1764 bytes).
Its 0x33 branch returns _UpdateRegen (0x3cb77c, 68 bytes), and 0x34 returns
CharProperties::HandleDots (0x3df3f0, 144 bytes), before ordinary AI gates.
Neither special branch forwards an additional state-machine event.

_UpdateRegen calls owner virtual 0x54, proven by the Character vtable to be
ObjectBase::IsRemotelyUpdated (0x33dd10, 20 bytes). It reads remote word 0x110:
anything except -1 returns 1; otherwise it returns the raw byte at 0x118.
Actual ObjectBase C1 stores byte 0 at 0x33f2c8 and word -1 at 0x33f2d8;
C2 mirrors them at 0x33f47c/0x33f48c. These constructor instructions are pinned
and statically inspected; the timer capture supplies the resulting inputs.

GetInCombat (0x3d4bc4, 112 bytes) checks actual aggro counts at AI+0x8c/+0xa4,
then owner SM states 5, 6 and 7 in short-circuit order. The composition borrows
CharAI tree counts and the same Coordinator.state. Regeneration does not add
dead, paused, active-AIS, initialization or readiness gates.

RegenTick (0x3bdd90, 356 bytes) always loads real Debug, constructs the actual
`isTracingChar_Stats` key, queries and discards it, then destroys the string.
It reads cached HP rate 39/40, delivers RegenHP, freshly reads MP rate 44/45,
then delivers RegenMP. No dt scaling or recalculation is inserted. The original
mutation cases change MP rates during HP delivery, proving the fresh read.

HandleDots reads current cached properties 126..131, acts only on positive
signed words, checks actual dead state, then delivers real self F_DotAttack
and F_ApplyResult. There is no separate empty DoT owner to fabricate. Positive
buff groups can populate these fields through canonical property resolution.

Player and PlayerIPhone vtable update slot 0x18 resolves to nonempty
AISDefault::OnUpdate (0x3dc798, 64 bytes). Its counter>199 branch resets the
counter, pauses AI update for 1000ms and commands the current owner to stop.
That is a frame handler and is not invoked by these timer special branches.

## API, ownership and failure

`character_regen_tick_v1::tick` borrows typed State/Globals/Services and performs
the eight source actions above. Nonzero/throw retains the completed prefix;
there is no rollback or synthesized cleanup.

`player_ai_timer_events_v1::Runtime(Bindings)` borrows actual associated CharAI,
the bound sole Coordinator, actual ObjectBase remote projection, controller
and property identities, the same grouped PropertyView, dead word and real
Debug globals/services. Optional DoT Runtime/Storage and full Player apply
provider are mandatory when a positive living DoT reaches them. Runtime owns
only temporary string backing. Failed Debug queries retain surviving strings.

`deliver(0x33/0x34, timer, result, error)` requires a timer from that same
Coordinator store. The event argument is captured by the source traversal;
an observer mutating Timer32.event cannot change the delivery. Invalid argument
or same Runtime reentry leaves outputs untouched. Backing must stay live on one
owning thread; providers cannot rebind actor or resolved sheet identities.

CoordinatorBindings appends an optional `route_timer_event` field, preserving
older aggregates. The existing before observer runs, then the typed route:
`machine` forwards the existing FSM; `delivered` skips it; `failed` or throw
preserves timer/provider effects and skips FSM and after observer. Successful
delivery runs the existing after observer. There is one timer traversal/store.

Player event `0x35` now enters `dh2_character_ai_event_script_timer` from the
existing `prince_timer_before` hook. Its active-AIS gate and slot `+0x90` call
are identity-bound to the same Character, Coordinator timer and retained
AISDefault/VM; the virtual adapter invokes that VM's `OnTimer(id)`. An inactive
AIS is a source no-op; a different active AIS fails closed. This is a bounded
callback adapter, not a new AIS vtable or complete `AISDefault::OnScriptTimer`
body reconstruction.

## Selected host proof and native limits

The runner builds the actual selected dh2_level_world DSO and single selected
dh2_script_runtime DSO; no direct standalone module object is staged. It
checks pre/post hashes of compiled inputs. Result: 11 exact original RegenTick
traces, 33 composition cases over actual Knight/Mage/Rogue base rows, 93 failure
prefixes and five guards, plus all existing Coordinator regressions.

Composition tests use actual CharAI construction/association, Coordinator,
retained same Session/VM, file-backed Debug and canonical property/HP/MP/DoT
attack kernels. Type-4 test values enter a registered sole BuffOwner instance
via dh2_property_set_to_sheet, resolving the real borrowed buff groups. VM
closes before buff retirement. The positive application test provider performs
an explicit property effect; it does not prove complete native Player
F_ApplyResult. Both absent attack and absent apply providers have reached-failure
regressions. Native positive DoT still needs its genuine combat dependencies.

The read-only native review confirms same owners, refresh/attach before tick,
typed 0x33/0x34/0x36 routes and owned-ID teardown. The source proof report is
independent of Android compiler or live gameplay gates, which the parent owns.

Actual CharAI::OnDied (0x3d1000, 80 bytes) calls AIS virtual 0x24 then tail
AI_SetDead (0x3d6cdc, 140 bytes). AI_SetDead stops AI+0x10 and AI+0x14 via
CharTimers::StopTimer at 0x3d6d20/0x3d6d30, then stores both IDs=-1 at
0x3d6d38/0x3d6d3c. Current native death state request does not deliver this
caller. Full source death cleanup is a proven remaining native dependency;
inserting a dead regeneration guard or an invented timer pause would alter
the source. `death-cleanup-dependency.json` pins the relevant bodies and stores.
