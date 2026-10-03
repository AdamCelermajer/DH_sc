# Bounded source Character state coordinator

The original ELF SHA256 is
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
`original-functions.json` identifies 32 captured routines by address, size and
byte hash; `reference/original-functions.asm` contains their ARM instructions.
This module projects Character/CharStateMachine fields into flat native structs.
It does not overlay ARM objects or substitute a general AI FSM.

## State and service ordering

`_SetState 0x3c1938` captures the previous ID, calls outgoing OnBlur at
`0x3c1988`, performs the state lookup, assigns the incoming state, resets elapsed
milliseconds only when IDs differ, then calls incoming OnFocus at `0x3c19f8`.
Same-state transitions still run blur and focus. Character event `0x1d` follows
focus and carries the previous ID, including -1. Services are synchronous and
can mutate projected fields or reenter the coordinator.
Facts are borrowed current producer storage. A callback that changes a producer
must refresh its facts before returning: original Stop copies default heading
at 0x39394c/0x393958/0x393960, and a subsequent same-state Move focus reads the
new heading for UpdateType. The audit includes this callback/fact mutation.
Stop's default is `Vec3f_Origin` at0x99f854, raw words[0,0,0], referenced
through GOT0x9989c4. Stop preserves owner rotation; the service must not turn a
zero heading reset into SetHeadingDirection or SetOrientation.

| State | Exact focus | Exact blur |
|---|---|---|
| Idle 3 | `0x3c3020`: nonzero Character+0x538 suppresses focus; otherwise flags+0x520=0x2380, Idle table+0x28, stance mask2, ANIM_Set. | `0x3c2d3c`: clear byte+0x538. |
| Move 4 | `0x3c3bf8`: flags0x23c1, move_type+0x53c=0, UpdateType, then unpin an existing body. | `0x3c3aa4`: complete GameObject.Stop, then pin an existing body. |
| Attack 5 | `0x3c404c`: flags0x2341; nonzero GetAttackDelay sets Character+0x528 bit1. Player predecessor4 selects moving Attack table+4, otherwise AttackStatic+8. Nonplayer selects static unless its raw ID=-1. ANIM_Set precedes pin/static or unpin/moving, then cached attack speed+0x52c, ANIM_SetSpeed, CancelSneaking. | `0x3c3f74`: nonzero source delay starts timer(delay,0,event0x2a,null), then pin existing body. |
| Dead 12 | `0x3c4d50`: flags0x241 plus0x2000 for player; controller.Cmd_LookAt(payload), controller byte8=1, remove highlight. Byte+0x53a alternate skips SM_SetAnim; otherwise SM_SetAnim(-1) consumes machine+0x28 override if present. StopLoop(false); if final current animation=-1 and nonplayer, start despawn timer. Primary filter change, CancelSneaking, disable state/self FX, remove buffs, raise0x2a,0x2c,0x2b. | `0x3c499c`: controller byte8=0; reset body filters if body exists. |

The source death caller selects the Died sequence and writes machine+0x28
before transitioning. The native caller must populate State.animation_override;
Facts.death alone is not a fallback used by Dead focus. The default Knight
moving table field is named `Attack`, not `AttackMoving`. Source runtime labels
can describe moving attack, while authored lookup must use `Attack`.

Move UpdateType `0x3c0f18` computes `((x*x)+(y*y))+(z*z)` with individual
single-precision operations. For a player already running(type2), strict
`walkThreshold² > heading²` changes to walk1. Otherwise strict
`runThreshold² < heading²` selects run2; initial type0 falls back to walk1;
existing walk1 remains. Nonplayer only changes type0 to walk1. On a change,
WalkSpeed is cached, Walk table+0x94(mask0x10) or Run+0x70(mask0x20) is set,
then ANIM_SetSpeed follows. Thresholds are original DesignSettings row fields
+0x5c/+0x60 supplied as explicit facts.

Move update `0x3c1184` raises Character event0x3f immediately when heading byte
+0x1b5 is zero. Without a target, a nonplayer at destination and not following
a path also raises0x3f. Otherwise UpdateType precedes checking WalkSpeed again.
Attack update `0x3c14f8` requests LookAt(target) before the speed check. Both
updates use strict `abs(new-cached) < floatbits0x38d1b717` to avoid a speed set;
unordered NaN comparison enters the set branch.

## Events and predicates

The differential executes original Idle/Move/Attack/Dead OnInit registrations
at 0x3c7e60/0x3c80ac/0x3c8284/0x3c8920. The native bounded map supports:

| Current | Event | Incoming / predicate |
|---|---|---|
| Idle3 | C351 | Move4 |
| Idle3 | C352,22,23 | Idle3, including blur/focus |
| Idle3 or Move4 | C354 | Attack5 iff Character+0x528 bit1 clear (`CSM_Attack 0x3ad22c`) |
| Idle3, Move4, Attack5 | C358 | Dead12 |
| Move4 | 3f | Idle3 |
| Attack5 | C351 | Move4 iff Character+0x442 is nonzero (`CSM_StoppedAttacking 0x3ad280`, current ID5) |
| Attack5 | 22 | Idle3 |

Attack OnEvent `0x3c1288` event1c applies only to players. It chooses moving or
static from heading byte at entry, emits ANIM_Swap(desired,old), then reloads
body presence and unpins/moving or pins/static. The chosen branch remains even
if the synchronous swap callback changes heading. Event1a looks at target
position; without target it calls SetHeadingDirection(currentHeading,true)
only for ranged inventory plus active heading. These are explicit target and
inventory service facts, not inferred combat rules.
Request look_at argument0 distinguishes GameObject.LookAt(object)=0,
controller.Cmd_LookAt(object)=1, and LookAt(target.GetTargetPosition())=2.
GameObject.LookAt(object)0x393d48 ignores null and otherwise obtains
GetTargetPosition0x3935dc. That provider chooses cached+0x184 only if owner
+0x180 is nonnull and byte+0x80 nonzero, otherwise owner position+0x160.
Point LookAt0x393cec subtracts owner XYZ then calls SetOrientation0x393b1c;
it does not enable heading or replace heading words. Controller.Cmd_LookAt
0x4052bc checks forced byte9: when zero, it rejects global s_blocked or local
locked byte8; forced skips those guards. Allowed calls owner virtual slot0x14.
Global s_blocked0x9a318b (GOT0x9980e8) begins zero. Dead issues this command
before writing controller locked=1. These receiver services remain explicit.

Dead OnEvent22 `0x3c4c3c` calls SetPhysicalObject(null,false); the service must
perform the actual body detach/destruction. Nonplayers then start the original
despawn timer and write flags0x40. Player keeps0x2241. Dead OnUpdate is empty.
Unsupported stun/scare, dialogue, knockback and other states are outside the
module; direct unsupported transition is rejected before mutation. Events
must already have passed Character->AI routing. The module does not invent
AI policy, automatically route RaiseEvent, or clear the attack gate on Idle.

## Scalar and filter source facts

`PROPS_GetAttackSpeed 0x3de74c` loads signed int32 at CharProperties+0xb58,
which is native resolved property48 at header+0xa98. Exact operations are int
to float, multiply3b800000(1/256), multiply3c23d70a(.01), add3f800000(1),
then strict positive comparison; otherwise returns positive zero. The helper
accepts the resolved224 table and reproduces those operations.

`SM_IsIdle 0x3c0260` returns true for IDs3/13, `!mode` for ID18, and false
for every other ID, including Move4. This does not establish a name for ID18.

`PhysicalObject.setFilter 0x46ece8` signature is
`(int16 groupIndex,uint16 categoryBits,uint16 maskBits,bool applySecondary)`.
Primary shape+0x18 receives group atshape+0x26, category+0x22, mask+0x24,
then world.Refilter(shape)0x7e7afc. Secondary+0x1c changes only when the final
bool is true. Dead emits `(0,0x51c,3,false)`. The owner cached byte+0x26 is
cleared. `resetFilter 0x46ec6c` restores saved owner category+0x20,
mask+0x22,group+0x24 to both existing shapes and Refilters each. The native
service must preserve the original secondary policy and contact-owner facts.

## Validation and remaining boundaries

The optimized ARM64 audit has 3,910 original-derived comparisons: 2,818 state
cases, 1,010 attack-speed scalars, and 82 idle predicates. It compares every projected word
and every ordered Request; only arithmetic NaN cached-speed/scalar sign/payload
are compared by NaN class. Fixtures cover all outgoing/incoming pairs including
same-state, player/nonplayer, body-present gates, disabled Idle, attack delay,
stance flags, invalid static animation, override consumption, strict movement
and epsilon boundaries, signed zero, finite overflow, nonplayer destination,
target/ranged event1a and synchronous animation callback mutations. Native
callback context and target identities exceed4GiB.

The same corpus replays on the x86_64 host under ASan/UBSan and warning errors,
plus 21 malformed no-mutation checks and a synchronous completion reentry.
Source tables, body/controller/animation/AI/timer/FX/buff services are controlled
caller fixtures. Actual full animation blending, AI/event consumers, timer
expiration and death cleanup backends are not claimed by this module.

Source ANIM_Swap0x3caccc(desiredRoot,oldRoot) is additionally captured under
`.local-inputs/character-state-discovery/swap`. desired=-1 is no-op. If current
root equals oldRoot, or oldRoot=-1, it replaces corresponding sequence IDs
through the existing recursive stack while preserving step/time fields, using
replacement steps[currentStep].childId; counts/types must match. If current
differs both old and desired, ANIM_Set(desired) then restores prior speed. If
current already equals desired and differs from old, it does nothing. An
ordinary restart is not the matching-stack service implementation. This module
emits the service request and keeps that scheduler boundary explicit.
