# Original blended playback completion and reentry

This is an original ARM32 instruction audit, not a native playback parity
report. The original ELF SHA-256 is
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The focused manifest captures 20 functions with individual byte hashes;
`reference/original-functions.asm` contains their decoded instructions.
Three scripts produced **63 passing original-instruction fixtures**:

| Script | Saved trace | Cases |
| --- | --- | ---: |
| `probe_original.py` | `source-probes.json` | 25 |
| `probe_selection.py` | `selection-probes.json` | 13 |
| `probe_actor_actions.py` | `actor-action-probes.json` | 25 |

Each saved trace binds the original ELF and the producing script SHA-256.
`audit-bindings.json` additionally binds the focused capture and all traces.
All fixtures use synthetic resolved fields and explicitly observed services.
No Android builds, packaged tests, native production writes or asset-factory
parity are part of this audit.

## Completion is retained across selection

`AnimatorBlender::Blend` (`0x36679c`) changes previous/current slot and fade
fields. It does not clear aggregate applicator pending at blender `+0xb8` or
extra milliseconds at `+0x98`. It rotates the slot even for the `PlayClip(-1)`
failure at `0x476838` after `Blend` has already run.

`BlendedAnimSetController::PlayClip` (`0x47680c`) remembers the incoming slot's
old clip ID before selecting it. Actual `SetCurrentAnimation` (`0x3674ac`)
calls actual `setCurrentAnimation` (`0x65f8c8`). In the event-library branch,
selection unconditionally calls timeline `SetClip(0)` (`0x666f7c`), including
same clip ID. `SetClip` clears both ended `+0x3c` and initialized `+0x3d`,
refreshes bounds and moves current milliseconds `+4` to the start bound.

Then the same-ID check in `PlayClip` calls `IsEnded` **after this reset**.
The observed result is false even when the old timeline was ended. It calls
actual `SetTime` (`0x666f10`) to incoming start plus aggregate extra
milliseconds (`0x476938..0x476954`). The fixtures use start=100, extra=17:
same-ID current becomes 117; new-ID current remains 100. Loop is set to1
and scale to1 before the root replay service. Aggregate pending remains1
and extra remains17 unless that replay actually checks its callback.

`AnimApplicator::CheckCallback` (`0x36440c`) requires both pending and a
nonnull callback. A missing callback leaves pending set. A present callback
receives `(timeline, context)` while pending is still set. Only **after** it
returns does `0x36443c` clear pending. A callback that reasserts pending is
therefore cleared too. The replay fixture optionally redirects its explicit
root service to these actual instructions; root hierarchy/replay itself is
not supplied by this probe.

## Root reset and scale

Aggregate `ResetDelta` (`0x366a68`) returns only when reference node `+8` is
null. Otherwise it selects the CURRENT slot, fetches its timeline and
applicator, initializes XYZ to zero, and optionally samples its root target
at the timeline start. Root target `-1` skips sampling but still calls
`AnimApplicator::ResetDelta` (`0x3644cc`) with the zero point.

The incoming history receives the requested timestamp, sampled/zero XYZ and
zero delta. Outgoing history is untouched. Aggregate completion and extra
time are untouched. For a target that exists but produces no default/sample,
the root sample service leaves the initial zero bytes intact. The actual
no-default sampler is a separate source boundary; this probe observes its
write/no-write contract rather than claiming to reconstruct that sampler.

This differs from aggregate **Animate**'s `rootTarget==-1` branch, which
skips the all-slot history/delta loop (existing composition capture).
`SetScale` (`0x3666d8`) visits every slot timeline, regardless of either
weight, including weights `[0,0]`. The fixture executes actual per-timeline
`SetScale` (`0x666c20`) and confirms both scale words.

`_HandleAnimEnding` (`0x366628`) compares callback timeline identity against
the LIVE current slot's timeline. Outgoing completion is ignored even when
that outgoing slot has nonzero weight. Matched completion computes source
extra time, then sets aggregate pending. Fixtures exercise both current
slots and both ending identities.

## Actor pending and callbacks

`CharAnimator::ANIM_Set` (`0x3cacb0`) checks actor pending byte `+0x49`.
If true, it stores the requested sequence at `+0x50` and returns. This gate
applies to **every** caller while pending, including calls between scene and
animator phases. Last request wins; speed and completion remain untouched.
With pending false, it writes global speed `+0x40` to1 and calls `_SetAnim`.
Queuing only during the finite-close callback is insufficient.

`CharAnimator::Update` (`0x3caf3c`) retains the sequence record before
event27, but reads live frame fields after synchronous callback return.
It calls `Character::RaiseEvent` (`0x3a4d5c`) with owner `animator+4`, event
number and a null payload for events27,25,23,22. Caller event return values
are ignored. Fixture observers return0/1 and can tail-enter actual
`ANIM_Set` during each of these events. These requests occupy `+0x50` and
do not veto normal completion progression.

| Source action | Ordered callbacks/services before pending consumption |
| --- | --- |
| type1 ordinary intermediate step | 27, increment step, 23, `_SetAnimStep` |
| type0 repeat | 27,25, decrement positive repeat,23, optional old `_SetAnim` |
| finite base exhausted | 27,25, closed48=1,22 |
| same retained finite root with a second explicit pending notification | 27,25; no second22 |

The source sequence **type** is row `+0x10`; it must not be confused with
loop count at row `+4`. `_SetAnimStep` is the exact symbol for `0x3ca79c`.
Older service labels `PrepareAnim` in these probes mean this boundary;
its unsigned `r1` is the step index, not the sequence ID.

Repeat has a distinct pending branch at `0x3cb0b8..0x3cb0cc`. If `+0x50`
already holds a request, it skips the old repeat `_SetAnim` (`0x3cb2ac`),
including old RNG/selection/replay side effects. Ordinary next-step advance
always calls `_SetAnimStep` first while actor pending remains1
(`0x3cb244`). A queued request does not suppress finite event22.

The common tail clears actor pending at `0x3cb0ec`, then invokes
`ANIM_Set(+0x50)` at `0x3cb100`, and finally clears `+0x50` to-1 at
`0x3cb108`. Nested finite event22 selection therefore queues while
closed48=1,pending49=1 and takes effect after the callback returns.
The source repeat counter is normalized/restored before this tail.

## Deeper selection services: captured, not yet fixture-composed

The focused capture also resolves these source facts:

* `_SetAnim` (`0x3cab38`) validates sequence/depth; writes last sequence
  `+0x4c`, depth `+0x2c`, frame sequence and authored repeat before event24
  (null payload, `0x3cac14`). Event24's return is ignored. It then chooses
  step0 or a source random step and calls `_SetAnimStep`.
* `_SetAnimStep` (`0x3ca79c`) writes live frame step at `0x3ca7f0` before
  event26 (null, `0x3ca804`). Event26's return is ignored. A redirect child
  (`step+0x28==1`) enters `_SetAnim(child=step+8, depth+1)` at `0x3caae8`.
* Leaf selection copies displacement byte `step+0x34` into actor `+0x30`,
  performs equipment/audio services, then writes authored step speed
  `step+0x30` into actor `+0x34` at `0x3ca924`. The valid visual/clip branch
  clears closed48 at `0x3ca944`, calls replay producer `0x3c9b7c`, then
  `PlayClip`, then `SetScale(globalSpeed * stepSpeed)`.

These last selection side effects and nested redirected stack callbacks
require their own caller-service composition audit. Full AIS event
forwarding, sounds/equipment and callbacks that directly alter tables or
arbitrarily replace stack storage are not claimed here. Tables/resources
are immutable fixture inputs; ordinary ANIM_Set requests and retained
manager batch semantics are the synchronous reentry contract under audit.

## Narrow integration consequences

Preserve separate slot/aggregate/actor completion, queue all pending49
requests, clear callback pending after synchronous return, reset only the
incoming history even for absent root target, and scale both timelines.
Expose source27/25/23 as synchronous services with null payload and ignored
return values. Let repeat requests bypass old repeat selection while
ordinary next-step preparation still occurs. Retain finite root metadata;
closed48 guards repeated22 rather than suppressing all later pending
Update callbacks. Extend live scheduler services for deeper24/26/redirect
behavior from the captured instructions, not a generic restart.
