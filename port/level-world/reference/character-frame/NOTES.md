# Character frame and focus/blur discovery

Read-only reconstruction discovery from original ELF SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
This establishes actual instruction call sites and field writes. It does not
execute a complete original frame, reconstruct all FSM states, or verify the
current Android movement checkpoint. No production code, tests, build files or
README files were changed for this discovery.

## Evidence

`.local-inputs/character-frame-discovery/manifest.json` combines the source
routine addresses, sizes and byte hashes. Each capture directory contains
`original-functions.json`, `reference/original-functions.asm`, and annotated
assembly with direct-call symbol names. `predicate-got.json` proves the three
event predicate pointers below from actual ELF GOT words. The source frame
dispatch preceding Character is also described in
`../frame-order/NOTES.md`; playback/reentry evidence is in
`../actor-playback/NOTES.md` and `../visual-timeline/NOTES.md`.

Focused entry points:

| Original routine | Address | Role |
|---|---|---|
| Level::Update | `0x3f82d8` | Physics before eligible objects |
| ObjectManager::Update | `0x34a620` | Active object virtual update |
| Character::Update | `0x3abe98` | Timers, AI, FSM, animator, GameObject |
| CharStateMachine::Update | `0x3c628c` | State elapsed time and OnUpdate |
| CharStateMachine::_SetState | `0x3c1938` | Ordered outgoing blur/incoming focus |
| CharStateMachine::RaiseStateEvent | `0x3c5684` | State event and transition predicate |
| CharAnimator::Update | `0x3caf3c` | Post-Step completion/step scheduling |
| CharAnimator::__CallbackEvent | `0x3c9984` | Synchronous authored trigger handoff |
| Character::RaiseEvent | `0x3a4d5c` | AI event routing, special buff event |
| CharAI::RaiseAIEvent | `0x3cbb34` | Event-specific handlers then FSM routing |
| GameObject::Update | `0x38cbe8` | Path, rotation, subobjects |
| GameObject::UpdatePath | `0x3940c0` | Uses virtual path policy |

## Normal eligible Character frame

The early scene-manager/root animation phase precedes GSLevel/Level gameplay.
Level calls PhysicalWorld::update at `0x3f84c8`, then spawn/group services,
ObjectManager::Update(float1) at `0x3f84e8`, and later camera/FX services.
ObjectManager invokes eligible ObjectBase virtual slot+0x2c at `0x34a84c`;
the Character override is `0x3abe98`. Online/active/deletion and CanUpdate gates
remain meaningful; this is the order of calls that execute, not an assertion
that every actor executes every frame.

Character calls virtual CanUpdate at `0x3abf60`. Its normal main chain is:

| Call site | Receiver / operation |
|---|---|
| `0x3ac024` | Optional TimerUtil pointer at Character+0x14e0 |
| `0x3ac02c` | CharTimers at Character+0x3b4 |
| `0x3ac034` | CharAI at Character+0x3c8 |
| `0x3ac03c` | CharStateMachine at Character+0x4fc |
| `0x3ac048` | CharAnimator at Character+0x49c |
| `0x3ac054` | GameObject::Update on the same Character |

CharStateMachine::Update adds Application::GetDt milliseconds to its word+0x60
at `0x3c62b8..0x3c62c4`. State-machine flags+0x2c masks 0x2/0x4 can first request
stun/scare states. It then invokes current StateInfo's state-object vslot+0x14
(OnUpdate) at `0x3c62fc`, with state ID, Character and machine. These machine
flags are separate from Character policy flags+0x520.

GameObject snapshots previous position/Euler, then UpdatePath `0x38ccc4`,
UpdateRotation `0x38cccc`, UpdateSubObjects `0x38ccd4`, target-node cache
`0x38ccdc`, and RequireOnlineUpdate `0x38cce4`. A state/animation event can
therefore change policy, body state or animation before this frame's path and
subobject synchronization. Already sampled root displacement belongs to this
frame; actor-issued body velocity affects the following physics integration.

## Scene events and animator completion are different phases

`CharAnimator::__CallbackEvent` stores triggered-event word0 at animator+0x58,
loads the event's word+4 as payload, and tailcalls Character::RaiseEvent with
event 0x28 at `0x3c999c`. RaiseEvent tailcalls CharAI::RaiseAIEvent for this event.
Its event 0x28 branch invokes `_OnAnimEvent` `0x3d4434` at `0x3cbe84`.
The handler can consume an event; only its accepted result continues through
the shared `0x3cbe58 -> 0x3cbc5c` FSM route. This is synchronous callback work,
not a queue deferred until after path. Original scene/applicator traversal
calls these callbacks during the early scene phase. Root NewAnim can also
reenter onAnimate synchronously while changing/replaying a clip in actor phase.

The ordinary completion callback `0x3c90ec` instead sets animator+0x49=1.
CharAnimator::Update checks Character+0x520 bit0x200, manages animator-set users,
and consumes completion/pending selection after Step. It raises sequence/step
events0x22..0x27, selects/replays authored steps, and can recurse while unwinding
nested selections. For example, final base sequence completion raises event 0x22
at `0x3cb1dc`; Attack OnInit registers this event to Idle state3 at `0x3c82b8`.
Completion.extra_ms must retain the applicator callback's captured value, as
already established by the playback oracle; finalized currentMS is not an
equivalent producer. Do not advance a second independent combat modulo/cursor
clock after GameObject::Update to approximate these phases.

## Ordered transitions and policies

`_SetState(new_id,event,payload)` invokes outgoing state's vslot+0x10 (OnBlur)
at `0x3c1988`, passing the new state ID. It resolves the new StateInfo, assigns
machine+0x20, clears elapsed+0x60 only if the ID changed, and invokes incoming
vslot+0xc (OnFocus) at `0x3c19f8`, passing previous ID/event/payload. It then
raises Character event 0x1d with previous ID at `0x3c19bc`. Same-ID transitions
still call blur/focus. Missing new states clear the current state and still
raise the notification. These calls are synchronous and may raise nested events.

| State | Original focus writes and ordered services |
|---|---|
| Idle 3 | If Character+0x538 is nonzero, `0x3c3080..0x3c3088` returns before policy/animation changes. Otherwise writes 0x2380 at `0x3c30ac`, resolves table Idle(+0x28)/stance mask 2, then ANIM_Set at `0x3c3108`. Idle blur clears +0x538 at `0x3c2da0`. |
| Move 4 | Writes 0x23c1 and move type+0x53c=0 at `0x3c3c64/70`, calls UpdateType at `0x3c3c7c`, then unpins body at `0x3c3c8c`. Blur calls complete GameObject::Stop at `0x3c3b08`, then pin at `0x3c3b18`. |
| Attack 5 | Writes 0x2341 at `0x3c40b0`; nonzero GetAttackDelay sets Character+0x528 mask 0x1. Player previous ID 4 selects table AttackMoving(+4) and unpins at `0x3c42b0`; ordinary other player predecessor selects AttackStatic(+8) and pins at `0x3c4184`. Stance masks are 0x40/0x80. ANIM_Set precedes pin/unpin, then PROPS_GetAttackSpeed, cache+0x52c, ANIM_SetSpeed, CancelSneaking. Nonplayers use static if its ID is not -1, otherwise moving. |
| Dead 12 | Writes 0x241 at `0x3c4dec`, OR 0x2000 for virtual IsPlayer at `0x3c4e10/14`, yielding 0x2241 for Prince. Requests controller LookAt(payload), sets controller+8=1, chooses/stops death animation, applies physical filter(0,0x51c,3,false), disables FX/removes buffs and raises events 0x2a/2c/2b. |

Attack blur checks original AI-table attack delay (`GetAttackDelay` reads runtime
AiProps+4). If nonzero it starts timer with event 0x2a at `0x3c4030`, then pins
at `0x3c3ff0`. Timer/script consequences remain services. Dead blur clears
controller+8 at `0x3c4a04` and resets the filter at `0x3c4a14`. Dead event 0x22
removes its physical object at `0x3c4cd0`; nonplayers then start despawn timing
and write flags 0x40. Full death cleanup/revive services are not reconstructed here.

Character::IsUpdatingPath `0x3a2e2c` returns flags+0x520 bit 7. Move and Idle
prefixes enable it; Attack and Dead prefixes disable it. Position-from-visual
is bit 0, physics bit 1, rotation source bits 2/3, visual-with-game-rotation is
inverted bit 4, and floor-validation is inverted bit 6. Attack/Dead retain bit 0:
disabling path is not equivalent to discarding authored root displacement.
Actual displacement also depends on the selected authored step MoveGO and
RootNewAnim enablement. State focus does not prove every attack/death clip moves.
UpdatePath still copies position to PFObject before its policy early return.

## Small event subset and explicit inputs

Idle and Move OnInit register event 0xc354 -> Attack 5 with predicate
Character::CSM_Attack `0x3ad22c`. ELF GOT slot 0x99556c stores that address.
The predicate accepts iff Character+0x528 mask 0x1 is clear. Idle also registers
event 0xc351 -> Move 4 unconditionally. Attack OnInit registers event 0xc351 ->
Move 4 with CSM_StoppedAttacking `0x3ad280` (GOT 0x997e1c), which accepts only
current ID 5 and nonzero Character+0x442. Attack's event 0x22 -> Idle 3 and
event 0xc358 -> Dead 12 have no predicate. Move event 0x3f -> Idle 3 is unconditional.
Move OnUpdate raises 0x3f when heading-active byte+0x1b5 is zero.

Character::RaiseEvent passes most events through AI, not directly into the FSM.
CharAI maps event 0 to 0xc351 and event 1 to 0xc352; event 0x28 uses the synchronous
animation handler. StateEvent first invokes current OnEvent, then registered
predicate, then `_SetState` only when accepted. The ordinary attack request
`SM_SetAttackState(payload,false)` routes 0xc354; forced=true directly enters 5.
`SM_SetDeadState` similarly separates event 0xc358 from forced state 12 and supplies
authored death animation IDs. Do not use the force path to bypass source gates.

Attack OnUpdate `0x3c14f8` faces Character's target+0x408 and refreshes attack
speed only if absolute change is not below float bits 0x38d1b717 (~0.0001).
Attack OnEvent 0x1c can swap static/moving animation and pin/unpin according to
player heading-active byte, even without leaving state 5. Attack combo step-end
handling `0x3d3e44` can raise 0x1b/1c, set/skip next steps, and clear nonsticky
targets. Those original player-input/AI bytes, target ownership and attack delay
producers are explicit inputs, not replacements with a generic attacking boolean.

## Minimal next integration

1. Let the active attack/death selection use the existing source Playback
   timeline/root bridge, alongside Idle/Walk/Run. Dispatch authored event crossings
   during scene_phase before genuine Step, and during synchronous NewAnim reentry.
   Preserve original callback/event identities and retained completion extra.
   Transitions raised by these callbacks execute synchronously at that point;
   do not defer early scene transitions until the post-Step actor phase.
2. After Step, apply a bounded source-backed Idle/Move/Attack/Dead transition
   coordinator, with outgoing blur before incoming focus, then state OnUpdate and
   animator completion/replay before the existing path/rotation/subobject runtime.
   Preserve AttackMoving root motion. Feed source policy flags into runtime;
   remove movement eligibility inferred solely from attacking/dead booleans.
3. Keep input requests, target services, timers, +0x442/attack-delay facts and
   unimplemented AI/status/death callbacks explicit. Differentially verify this
   small transition/service projection against these actual routines before
   claiming original FSM integration. Full FSM reconstruction is not a prerequisite
   for this bounded phase/policy correction, and is not claimed by it.

The current renderer inspected here invokes independent advance_player_attack
and combat pose sampling after advance_native_actor has already run path and
subobjects. Moving that entire call only to just before path still leaves ordinary
scene event sampling after Step. Pose/event sampling and post-Step completion
scheduling must be split. The checkpoint's final validation is parent-owned.

## Blending boundary

BlendedPlayClip `0x47680c` calls AnimatorBlender::Blend `0x36679c` at `0x476830`
with signed duration from controller+0x14 before animation selection. Blend moves
old/current animator indices into+0x74/+0x70, rotates to the next slot, copies
previous duration+0x78 to+0x7c, stores its reciprocal at+0x80 when positive, and
stores max(requested_duration,0) at+0x78. It expects two animator slots in its
normal backend. BlendPost `0x366740`, called after RootNewAnim at `0x4768f4`,
is exactly `bx lr` in this ELF.

Existing Playback verifies selected-clip timeline/replay/root behavior but has
one active sampled clip and no two-slot blend weights/fading/applicator ownership.
Attack/death can join that verified timing bridge with this explicit pose
boundary; do not claim original blended transition pixels or infer a fade
duration from the caller's unrelated CharAnimator+0x44 overshoot argument.
The complete weight/update producer remains separate recovery work. Source
callback timing and focus/body policy can progress without inventing those weights.
