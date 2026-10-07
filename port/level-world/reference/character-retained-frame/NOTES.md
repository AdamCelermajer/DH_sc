# Retained Character eligible Idle frame wiring

There is a genuine bounded startup branch for the eleven current Crypt monsters:
published active AI, Idle3, constructor `zoned=1/in_zone=0`, genuine AI Type4,
no heading, and an eligible visual. The original AI dispatcher returns before
target/master/aggro/selected-AIS Update in this branch. This is sufficient to
avoid inventing a successful zoning service. It does **not** remove Character
eligibility, controller, timers, FSM, animator, GameObject, or Character's tail.
Those mandatory prefixes need delivery before a live frame can be called complete.

This is new read-only source discovery and a concrete integration plan. No
frozen production, CMake, renderer, shared runtime, APK or device was changed.
No new native adapter is claimed: complete Character eligibility/tail and
several source visual/global ownership inputs still require real backends.
The existing frozen kernels are sufficient for the middle FSM/Idle/event chain.

## Evidence and scope

Original ELF SHA256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The root manifest and `dependencies`, `eligible-helpers`, `constructors`,
`fx-queries`, `animator-users` each preserve complete function bytes and assembly.
Together they contain 36 captured routines. `literals-and-vtables.json` binds
the actual Character/default-controller/AISExternal dispatch keys and strings.
`direct-calls.json` is a static BL-site inventory, not an execution trace.

`probe_original.py` executes 41 original-only prerequisite cases:
5 complete outer Character constructors with different prefill patterns;
9 actual eligible CanUpdate visual branches; 1 actual empty default controller
Update; 2 actual outer AI zoned skips; 20 actual FSM Update to actual Idle
OnUpdate/CommonUpdate bodies; 4 actual unchanged Idle UpdateStateFX branches.
All pass. Allocation/base/container/property constructors, online/GetPlayer and
current AI row projection are explicitly named service fixtures. These are
neither a complete Character frame nor original/native differential tests.
`original-prerequisites.json` preserves their exact inputs, outputs and providers.
The original source-frame manifests and existing independent kernel proofs
remain separate from this new observation bundle.

Reproduce from repository root with the pinned Python and existing PYTHONPATH:

```
python port/level-world/reference/character-retained-frame/probe_original.py
```

## One shared frame, distinct clocks

Use the actual current application dt word, the application frame stamp and the
shared accumulated scene timestamp as distinct inputs. Each original GetDt
reads Application+0x8c. Advancing timers and the FSM by the same dt is correct:
they are separate domains. Advancing the **same FSM elapsed word twice** is not.
Do not subtract timer time from FSM dt, feed elapsed into an animation cursor,
or integrate physics once per actor.

The ordinary source order is:

1. All eligible source scene/animation roots receive the shared absolute scene
   timestamp. Retained animation instances may emit synchronous authored events
   during this phase; their events must route through Character/AI/FSM before Step.
2. Level executes one shared `PhysicalWorld::update` (`0x34bd08`), using
   unsigned GetDt converted to float, multiplied by float0.001, iterations10.
   Real contact callbacks execute within this Step, with the current frame stamp.
3. Level spawn/group services precede ObjectManager's eligible actor traversal.
   The object traversal argument is float1, not dt. Object/room/online eligibility
   remains an outer owner boundary.
4. Each eligible Character executes its ordered prefixes, then
   optional TimerUtil → CharTimers → CharAI → FSM → CharAnimator → GameObject.
5. Character's inline posttail executes. Level's camera/VisualFXManager update
   is shared later work, not a per-monster substitute for Character's FX tail.

Reference call sites for step4 are respectively `0x3ac024/2c/34/3c/48/54`.
Application/scene/Level caller evidence remains in `../frame-order/NOTES.md`.
Scene animation visibility/culling policy and complete ObjectManager traversal
are not supplied by iterating arbitrary rendered actors.

The current `CharacterIdleEvents.scene_phase(timestamp)` and
`.animator_phase()` fit steps1 and4. The latter consumes the callback-captured
completion extra; it must not recompute extra from finalized timeline time.
Same-clip NewAnim can synchronously animate inside actor phase. Do not move that
callback into a second scene loop or manually advance an independent Idle cursor.

## Mandatory Character prefixes

`Character::Update 0x3abe98` first calls genuine empty release profiler
`0x3136b4`, then Debug.load/string/GetSwitch/destructor for `KillPlayerOne`
(`0x3abeec` string construction) and `Give50Potions` (`0x3abf30`). Both query
results affect control flow: they are not the ignored tracing diagnostics from
the StateOwner adapter. Use the retained world DebugSwitches/file owner. Failed
existing-file decoding fails delivery; no hardcoded false debug provider.

At `0x3abf60`, source Character vtable+0x148 resolves to **CanUpdate0x3a52a4**.
False skips the main chain. It is not an unconditional IsDead negation:

- If the captured visual exists, clear its node byte+0x200 first.
- Query online receiver and raw byte5. Its remote path uses owner v54.
- Offline with a visual calls genuine GetPlayer(0,true) and reads Player+0x660.
  When owner+0x418 differs, visual-node word+0x118 or owner byte+0x2fc can demand
  complete `TestCullingBeforeUpdate0x33de90` with owner AABB+0x12c.
- Node+0x118==0 **and** byte+0x2fc==0 bypass that culling helper. This is a
  source field branch, not permission to fabricate those values every frame.
- Query actual IsDead v34. A living actor reaches the accepted branch. Captured
  visual plus nonzero owner byte+0x80 writes node+0x200=1. Other accepted cases
  retain the cleared node byte. Dead/nonenabled paths can call CanRespawn0x3a5248.

The nine new CanUpdate observations execute those loads/stores and both online/
Player service deliveries. Their visual fields are explicit fixture inputs.
Production needs the genuine visual-node culling/update field producers; existing
factory Scene/BlendedPlayback ownership by itself is not a full RootSceneNode
visibility producer. An unsupported culling/respawn/online branch is failure.

Character reads current StateInfo ID. Published active AI at owner+0x3e4 being
nonnull provides the short Idle3 route from `0x3ac378..80` back to the main
chain, avoiding still-loading script/spawn/player-specific branches. Do not
pretend that uninitialized or pending-only script ownership is active.
Application game-stats word+0x5c is incremented before controller Update.

Controller+0x378 vslot8 is called at `0x3abfd0`, before the third debug query.
Complete source Character C1 allocates the default v2Controller, whose actual
vtable+8 is **v2Controller::Update0x3a2f44**, one `bx lr`. Its lock/force bytes8/9
are constructor0. This is a genuine empty provider only for that exact class/key;
gamepad/network/other subclass Updates are nonempty and must not be suppressed.
Then Debug.load/string/GetSwitch/destructor queries **isABot** (`0x3abff0`),
whose true player/online branches are not the ordinary monster route.

## Timers, AI and the one authoritative FSM

Character C1 sets optional TimerUtil+0x14e0=null. If a real producer later
installs it, original TimerUtil.Update0x317ae4 must run before CharTimers.
It reads its paused byte4; otherwise subtracts current GetDt from its word0
with wrapping arithmetic, then clears a negative signed result to0. It is
not an animation duration, and cannot be replaced by ignoring a nonnull pointer.

Session-owned `update_timers(dt,source_script_blocked)` is the genuine TimerStore
stage. The blocked source byte is ScriptManager+0x30. Timer expiry passes the
actual stable Timer object, not its user_ref. Source Init timers0x33/0x34 are
real repeat-1 timers, with design AI_Tick/DoT_Tick producers. Even zoned actors
still advance these timers before the AI skip. Route expiries through complete
Character→RaiseAIEvent, including genuine regen/DoT/OnTimer/pause/buff services
as appropriate. Missing positive-DoT/buff/world/controller providers are failure,
not a reason to stop all timers or ignore expired callbacks. Void expiry callbacks
require a persistent error latch checked before later stages.

Use `dh2_character_ai_frame` with the same live owner/controller/flags projections.
Its pause/force/block/lock/bit0x100 gates precede IsZonable. Source constructor
`zoned1/in_zone0`, actual Crypt Type4 IsZonable=true, and Idle flags0x2380 reach
the genuine outside-zone skip. The two new actual original calls verify no
Target/Master/Aggro/OnUpdate entry and no updated-byte+0x88 store. Do not call
this AI skip unconditionally after a real zone notification changes in_zone.

Once in_zone becomes true, full Target→Master→Aggro→OnUpdate service delivery is
required. The exact selected External+0x18 key is **0x3dce64**, not Default
alone: Default.OnUpdate3dc798; reload callback flags+b8; optional private Lua
OnUpdate; always CallStateUpdate3d8eb4; then CallStateConditions3d8ea0. The existing
ScriptStates update kernel reconstructs that order, but the current session does
not own/expose a ScriptStates registry. The supplied monster/commons have no
RegisterAIState/ChangeAIState and constructor current+b4=null; that is a genuine
empty state-call branch only while no producer changes it. Alias flag0 absence
skips common OnUpdate; it is not permission to suppress a real rene alias.
Full OnUpdate also performs source zoning/visibility/restoration tail work.

Build one `StateOwnerFrameContext56` around the SAME owner/FSM installed before
private VM Init. Its native outer services handle actual GetDt, genuine empty
3136b4/b8 profiler bodies, and required stun/scare setters. The frozen adapter
calls `dh2_character_native_fsm_update` once and its bounded tail calls
`dh2_character_state_update(...,dt0)`. Current NativeState.elapsed_ms is the
same field as Character+0x55c/FSM+0x60, not a second observer clock. Twenty new
actual original FSM→Idle executions verify wrapping old+dt and exactly one
GetDt read. IdleCommonUpdate adds no dt.

The frame body's `idle_common_update` callback must call existing
`dh2_character_idle_update`, projecting current state_time from that same elapsed
word. Supply genuine IsPlayer from current decoded property/AI ownership. A
Type4 actor with heading0 returns after IsPlayer; heading byte+0x1b5 nonzero
raises real Character event0. Route it through `CharacterIdleEvents.raise`,
never assign Move directly. An unexpected player branch requires its actual
online/PlayerManager/constants/point-controller providers. Since body callbacks
are void, latch Idle/provider failure and check it after the enclosing frame;
a successful bounded-tail integer alone is insufficient evidence of delivery.

`other_updates` must be a nonnull failure provider even while current Idle is
bounded: synchronous events can change the current state before dispatch. Do
not fabricate support for other nonempty update families. Refresh Facts/source
projections at real call/reload points, not only once at the start of a frame.

## Animator and GameObject stages

CharAnimator Update first reads current owner policy bit0x200. It manages its
users byte+0x5c before consuming pending completion. First enabled update calls
IncAnimSetUsers3c9168: current visual+0x38 controller v8 obtains retained set;
nonnull set+0x24 increments; temporary reference is dropped. Disable calls
DecAnimSetUsers3c91c8. The new complete captures preserve both. BlendedPlayback
implements scheduling/completion, but it does not by itself prove this resource
user bookkeeping or policy producer. Preserve that required prefix, even though
Idle0x2380 enables animation. Use the one retained instance and caller-shared RNG.
Its source events22..28 use the existing IdleEvents observer, with real end/FSM
self-transition choreography. Leaf selection equipment/audio/FX services remain
separate from observer routing; inspect exact authored leaf fields and fail
nonempty unbound requests rather than accepting them.

GameObject.Update0x38cbe8 performs Debug.load/string/GetSwitch/destructor for
**TraceUpdateGameObjectOnce** (return ignored), then increments app game-stats
word+0x58. Nonzero pending-interaction owner+0x2e4 calls current Character v98
Interact3a4d78, and clears that word AFTER the callback. Preserve its source
capture/reentry; missing Interact is failure when reached.

Then snapshot current game XYZ to previous+0x190 and current EulerXYZ to
previous+0x19c. Call ordered UpdatePath3940c0 → Rotation393710 →
SubObjects3943cc → TargetPosition393d74 → RequireOnlineUpdate38b8b8.
TargetPosition reads a nonnull target scene node's cached absolute XYZ; it does
not recompute its transform or copy actor destination. RequireOnlineUpdate
queries genuine online byte5, then source network fields/services if enabled.
Finally signed idle sound index+0x370>=0 calls UpdateIdleSound38ae2c.
Constructor index is-1; a later sound producer requires its real backend.

Bind navigation controller/path/PF object, body, scene binding and source game
position as coherent views of one Character. Idle flags0x2380 enable path,
floor validation and ordinary rotation, even with no heading. No-heading is
not an early return before all path/PF/subobject work. Native floor/registry/
Stop kernels can be reused; requested parent-list mutation, visual vUpdate,
physical transform, camera or auxiliary services must be delivered or fail.
SubObjects has an unsigned/void observer ABI, so non-query provider failure
also requires an enclosing persistent latch. Root application occurs at this
source subobject point; do not additionally displace the physics body during
the early scene sample or replay the sampled delta after this stage.

## Character tail and minimum FX branch

After GameObject, Character reads float+0x14fc. If greater than0, it subtracts
float(unsigned GetDt) and stores it (`0x3ac894..ac`); constructor is float-1.
This source field is distinct from TimerUtil, FSM elapsed and animation time.
It must not be conflated with them or decremented twice.

The nonplayer tail queries actual IsMerchant (GetCharType==7), online state
and source remote fields. Current Crypt Type4 makes IsMerchant false. Ordinary
constructor byte+0x14e4=0 follows the shorter online path, but online queries
are still mandatory. Every application frame whose raw stamp&7==0 calls
**Character.UpdateStateFX3a4470** before interaction/target FX work.

UpdateStateFX captures signed prior state+0x1490, calls IsDead, checks real
FSM stun/scare masks with policy bits0x800/0x400, then current
**PROPS_GetWalkSpeed3de6c4**, NOT HP percentage. WalkSpeed reads resolved
property46 (`Character+0x10b0`), signed→float, *1/256, *0.01, +1, clamp below0.
A resolved property46 value >=0 produces WalkSpeed >=1. With zero stun/scare
masks, a living actor and prior stateFX0, source selects stateFX0 again and
returns before Drop/Grab. Four actual original
observations prove that branch. Negative modifier or changed FX state can
require genuine effects set IDs, Drop/Grab/Enable/visual operations. Full effects
metadata alone is not an animated FX factory or a successful Play provider.

Subsequent interaction/target FX references+0x1494/149c/14a0 are constructor
null, prior interaction index+0x1498=-1 and raw byte+0x1480=0. This eliminates
those pointer setter calls only while real live fields remain unchanged.
The tail still calls **IsMultiplayerGame3a42f4**, then
**ScriptManager.IsCutSceneRunning455c54**; the latter traverses current manager
contexts via455bec, not an invented pause boolean. When no cutscene, pending
scrolling-combat words+0x1500/+0x1504 are checked. Constructor-1 skips their
gold/XP player/text delivery paths. Actual constructor outputs are in the new
probe. Do not bypass this tail merely because the initial FX pointers are null.

## Concrete integration contract and stop boundaries

Retain per MonsterHandle one StateOwner, session, IdleEvents adapter, animation
instance, body/PF/navigation/controller projections, source raw Character-tail
fields and all provider contexts. Keep the immutable bank/design/skills/effects,
shared RNG, world debug/files and actual object identities pinned through VM
close and synchronous callbacks; GL resources may be recreated separately.
Bind GetState to this owner before Init, publish selected External after genuine
Init, then source LevelLoadStates preset3 and real Idle focus. Reuse these exact
owners in every subsequent frame; no prototype controller/scheduler/health state.

The smallest next native additive frame owner should restore the ordered
Character/CanUpdate/controller/animator-user/GameObject/posttail coordinators
above over those borrowed records, invoking existing TimerStore, AIFrame,
StateOwnerFrame, IdleUpdate, IdleEvents/Playback, navigation/rotation/subobject
kernels. Its unknown branches must return required-prefix failure. Global/frame
producer callbacks, real culling fields, resource users, timer effects, physical
contacts, online/script-manager, sound and FX are mandatory when reached.
Existing source hooks prove their call order; they do not make them complete
providers. A generic wrapper that marks each callback accepted would hide exactly
these missing inputs, so this discovery deliberately stops before such an adapter.

Stop driving an owner after provider failure. Preserve delivered effects and
raw logs; no rollback of elapsed/timer/scene/state prefixes, no fake successful
retry, and no suppression of mandatory events. Shared Step cannot be undone:
contact failures must latch and stop subsequent actor work. A live startup
milestone can accurately report source Idle selection/CPU pose and bounded
zoned AI skip while the remaining eligible-frame coordinator is being proved;
it cannot yet report a complete original per-character frame.
