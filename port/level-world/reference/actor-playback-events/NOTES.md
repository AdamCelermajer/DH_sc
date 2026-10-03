# Authored events, finite closure and animation swap

Original desktop oracle ELF SHA256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
`original-functions.json` hashes 30 recovered routines; the adjacent assembly
is captured from this ELF. The Android game uses native C++ implementations.

## Actual source ordering

`ISceneNodeAnimator::updateTime` (`0x667c48`) obtains its timeline, saves old
currentMS, calls the timeline's Update virtual, then loads the event manager
from animator+0x18. It dispatches `(old,current,start,end)` after Update has
finalized currentMS. `applyAnimationValues` (`0x65f418`) calls this update and
then re-fetches the timeline/selected animation before computing pose values.
`AnimatorSet::animateNode` (`0x3673a4`) computes the animation values, calculates
applicator delta, and calls CheckCallback. Root `onAnimate` (`0x35d168`) runs
animators, consumes displacement, animates children, then stores root timestamp.
Thus authored callbacks precede this traversal's pose/root displacement and
see the previous root timestamp. Accepted callbacks can change selection
synchronously; the outer pose must use the selected clip after callback return.

`CharAnimator::__CallbackEvent` (`0x3c9984`) reads triggered-record **word0 as
signed lag milliseconds**, stores it at CharAnimator+0x58, and passes **word4,
the authored string pointer**, to Character::RaiseEvent with ID **0x28**.
These words are neither event ID nor an opaque four-byte integer payload.
The native handoff widens the immutable string pointer. Its lifetime is the
borrowed clip bank's lifetime; callback clients must not erase/mutate the bank.
The inspected 13 static/moving/pre/post/Died tracks contain `attack_mainhand`
in six tracks; the remaining seven have no event keys. Array/group order and
float-rounded lag are interpreted by the existing engine-animation event kernel.
The observer does not interpret combat damage, AI acceptance, FX or sound names.
An observer adapter that handles only melee routing does not implement the
general source AI acceptance followed by FSM event28 route. Caller facts must
be refreshed after synchronous services that change heading/path/state. Global
speed should come from the source animator/state cache: Move/Attack Update
keeps the old factor when absolute property difference is below0.0001. Feeding
an uncached property value to animator_phase bypasses this source threshold.

The actual original animator/timeline/manager/Char callback probe used a labeled
immutable synthetic track on clip955's real interval `[266,600]`. It dispatched
`first,second,third`, with lags `234,234,134`, after finalized currentMS600.
Every callback saw the old manager reference count2. Replacing animator+0x18
inside the first callback did not cancel the remaining batch; the old manager
committed its own cursor and returned to reference count1. The native wrapper
retains the old observer identity and uses a local old cursor. A generation
change prevents old cursor completion from overwriting the new manager cursor.
This is source synchronous dispatch; it does not introduce a trigger queue.

## Replay and post-Step completion

NewAnim (`0x35d624`) resets applicator history at clip start with root timestamp+1
without updating the timeline or applying a live pose, then synchronously calls
root onAnimate at the previous root timestamp. The optional-observer path resets
history with a detached scene/binding sample before callbacks, preserving live
root/scene until the actual animation phase. Events use the same timeline update
and event kernel in `replay_phase`; there is no independent event clock.

The timeline's ending notification captures applicator extra from transitional
old currentMS. Animator phase must use this captured `completion.extra_ms`.
Recomputing from finalized currentMS changes replay stride. A large captured
extra can immediately cross the replay clip end and dispatch from its fresh
manager; a reset manager is not necessarily still at cursor -1 after NewAnim.
Ordinary small-extra replay need not dispatch any event. The host audit includes
both large-extra replay and a separately labeled source loop=1 caller fixture
with older root timestamp. The latter validates synchronous replay event routing,
not an ordinary Character loop producer.

`CharAnimator::Update` (`0x3caf3c`) runs after physics Step. For finite base
completion it marks byte+0x48 **before** RaiseEvent **0x22** (null payload),
then clears pending byte+0x49. Its source trace on two repeated pending updates
is `27,25,22,27,25`; closure22 appears once and sees closed1/pending1.
`ANIM_Set` (`0x3cacb0`) while pending49 is true stores the requested sequence at
word+0x50. Update clears49 before executing that requested selection. Native
finite-closure observer calls to start therefore replace one pending-selection
slot, then apply it after callback return. Authored0x28 observer calls to start
select synchronously. This bridge exposes closure22; it does not expose all
intermediate source events23..27 or replace original AI/FSM transition acceptance.
Finite scheduler completion retains its selected timeline/clip and terminal
scene pose. Owner removal remains the caller's lifecycle boundary.

## ANIM_Swap

Recovered `0x3caccc` has these exact branches:

* desired==-1: no-op.
* old!=-1, current root differs from old and equals desired: no-op.
* current root==old or old==-1: rewrite sequence IDs from root through current
  depth. Each new sequence must have the old type and step count; the child ID
  is the new sequence's step at the **existing frame index**. Preserve frame
  indices, remaining loops, current clip, speed, timeline, event manager and
  root history. The original updates Char+0x4c to the final sequence ID and
  performs **no PlayClip/_SetAnimStep call** in this branch.
* root differs from both old and desired: ANIM_Set(desired), then ANIM_SetSpeed
  with the saved old global speed.

`AnimationScheduler::swap_sequences` returns no_op/metadata/restart/rejected.
Metadata leaves its current direct step intact until source completion activates
the next step. `Playback::swap` delegates a restart to start with the supplied
current global factor. Invalid native tables reject transactionally; source
debug shape assertions are not emulated as a null write. The tests execute 144
original instruction fixtures across three roots, three depths and all branch
combinations, plus real authored static-to-moving Attack swaps.

## Reproduction and limits

The optional observer defaults null, preserving the prior 10,584 phase and
5,628 root-sample corpus. `tests/actor_playback_events.cpp` takes assets directory
and `event-fixtures.bin`. Its host run uses the real 35-node Prince graph and
authored clip bank at factors1.0/1.3, separate scene/animator phases, terminal
pose retention, callback state switches, replay and metadata swap.

`generate_original.py` reproduces the original/ARM64 event and handoff corpus
using the existing Unicorn loader. First compile its standalone oracle:

```text
clang++ --target=aarch64-linux-android24 -shared -fPIC -O0 -fno-fast-math -ffp-contract=off -std=c++17 -DDH2_ACTOR_PLAYBACK_KERNEL_ONLY port/level-world/actor_playback.cpp port/engine-animation/events.cpp -o .local-inputs/actor-playback-events-discovery/oracle.so
python port/level-world/reference/actor-playback-events/generate_original.py
actor_playback_events_audit ASSETS port/level-world/reference/actor-playback-events/event-fixtures.bin
```

Imported float/libc primitives and logging/Character RaiseEvent observation are
explicit desktop oracle services. Full original scene traversal is instruction
audited, not executed as a complete engine frame. Native tests validate single
selected authored pose/root composition; weighted blended poses, all intermediate
CharAnimator events, all selection producers and full AI/FSM/App parity remain
separate integration work. No emulator or APK claim is made by this host report.
