# Independent current coordinator review

Read-only review of `actor_blended_playback.cpp` and
`AnimationScheduler::complete_with_services`, against the 63 original
instruction fixtures and focused capture in this directory. Only this new
note was written; production and other workers' tests were not edited.

The review found three concrete ordering mismatches. The same-clip mismatch
was corrected by the owning worker during this review; the later snapshot
below contains that correction. Two callback-order findings were handed
directly to the worker and parent. This note is not a post-fix native audit.

Reviewed later snapshot SHA-256:

| Source | SHA-256 |
| --- | --- |
| `port/level-world/actor_blended_playback.cpp` | `30d783455de61d24105dc7578403f3925ddb40e0a389d3b30852268eb6ca1489` |
| `port/game-data/animation_scheduler.cpp` | `0f0659bd5b5a3b46d51d983e6dfd651fe84355aaa0b8d7bff36361a312eaa29f` |
| `port/level-world/visual_timeline.cpp` | `9a6ffdec646fc927ce82199a193df7d1d14ba2f738464e312efead143b4899ca` |

## Same incoming clip: loop is not IsEnded

Initial `apply_selection` correctly invoked unconditional timeline SetClip,
but then delegated the same-ID replay decision to the historical helper's
`!s->loop` condition. SetClip preserves loop and clears ended/initialized.
Original `PlayClip` tests incoming IsEnded after SetClip, not the loop flag.

Counterexample from `selection-probes.json`: incoming ID7 already selected,
old loop1, start100, extra17, old ended either0 or1. Original SetClip clears
ended/init and source SetTime changes current to117. The initial coordinator
left current100 because loop1 skipped the historical replay helper's jump.

The later reviewed coordinator explicitly jumps on `previous==mapped &&
!slot.timeline.ended`, then supplies previous=-1 to the old helper. This
matches the saved source selection branch without changing the historical
helper's prior proof. No remaining same-clip counterexample was found in
this reviewed branch.

## Queued request is cleared after selection returns

In the reviewed `animator_phase` finish service, `pending_sequence=-1` ran
before `start(...)`; `start` also cleared the queue before `apply_selection`.
Original Update instead clears actor pending49, invokes
`ANIM_Set(seq50)` at `0x3cb100`, then clears seq50 at `0x3cb108` **after the
complete synchronous selection/replay returns**. Ordinary ANIM_Set with
pending49=false does not independently clear seq50.

Concrete service counterexample: an old finite frame consumes queued243.
During the `_SetAnim` selection/replay service, a nested source completion
sets pending49=1 and a synchronous event tail-enters actual ANIM_Set248.
The original common tail then discards that newly written seq50 while
retaining the newly raised pending49. The reviewed coordinator cleared
the old queue before replay, so the new queued248 survives. A recursive
child's second common tail can then incorrectly consume it.

An additional original-instruction probe ran inline without writing fixtures
or production files. Actual Update, ANIM_Set and its common tail executed;
resolved tables/loggers and `_SetAnim` were explicit caller services:

```text
event27: pending1, queued243
event25: pending1, queued243
event22: pending1, queued243
actual ANIM_Set immediate243
_SetAnim service: pending0, queued243
service raises completion and invokes actual ANIM_Set248
before 0x3cb108: pending1, queued248
final: pending1, queued-1
```

This is a callback-service counterexample, not a claim that a particular
authored clip currently triggers it. Fixing only the finish service's
pre-clear is insufficient while ordinary `start` still pre-clears the queue.
Native regression coverage should supply completion/reentry during the
consumed request's actual replay and inspect both pending fields afterward.

## Repeat count is restored after replay callbacks

The reviewed scheduler saved decremented remaining loops and restored
`stack[depth].loops` immediately after `enter`, before `services.prepare`.
Original repeat Update saves the old remaining counter, calls old `_SetAnim`
and all synchronous selection/replay services, then normalizes/restores
that saved counter at `0x3cb0dc..0x3cb0e4`, before its common pending tail.

Counterexample: type0 repeating frame, loops2, no queued selection. Update
decrements to1; during repeat `_SetAnim` replay a synchronous caller invokes
actual `ANIM_StopLoop(false)` (`0x3c948c`), which writes live loops0.
Original afterward restores saved loops1. The reviewed scheduler restored1
before its preparation callback, so that StopLoop remains0 instead.

A second inline original-instruction probe executed actual StopLoop from
the explicit `_SetAnim` boundary and confirmed:

```text
event27: pending1, loops2
event25: pending1, loops2
event23: pending1, loops1
_SetAnim repeat service: pending1, loops1
actual ANIM_StopLoop(false): live loops0
final: pending0, queued-1, loops1
```

Restoration must follow successful preparation/replay, addressing the
retained original depth even if entering a redirect child changes the live
top frame. Error propagation and arbitrary destructive stack replacement
remain explicit native service constraints.

## Matching reviewed behavior and remaining limits

* `start` now queues any valid request while actor completion.pending is
  true, including between phases. Finite callbacks retain pending until
  completion's common tail. Last queued request wins.
* `complete_with_services` now processes retained closed root frames even
  when playing=false, so source27/25 still occur on another explicit pending
  notification. The actor close guard suppresses only repeated22.
* Repeat with a queued request skips old selection/preparation; an ordinary
  intermediate advance prepares before consuming the pending request.
* Recursive parent completion followed by a child's common finish tail
  structurally matches original recursive Update, provided request clearing
  and replay-side effects observe the ordering corrections above.
* No concrete dangling-reference use was found for the documented immutable
  bank/table contract. Slot references designate fixed array elements;
  target/clip references designate immutable compiled resources. Scheduler
  completion retains immutable table type/count across event callbacks and
  re-reads live stack words by index afterward. Metadata swap may replace
  stack storage, but no retained mutable frame reference crosses those event
  callbacks in `complete_with_services`.
* Raw reentry that recompiles/erases the bank, mutates authored tables or
  directly replaces exposed scheduler storage outside the documented
  ANIM_Set/metadata-swap services is not proven. Full24/26/redirected leaf
  side effects and AIS forwarding remain the source boundaries recorded in
  NOTES.md; this review does not expand the native parity claim.

The 63 saved probes remain unchanged. The two extra inline original cases
are recorded here only; their `_SetAnim` caller services were deliberately
controlled and no full scene/asset end-to-end comparison was performed.
