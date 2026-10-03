# Deferred Walk selection in candidate d17eec5e

Read-only diagnosis of saved logs and compiler-captured inputs. The candidate
APK SHA-256 is `d17eec5ecd1cf9fc2d334767b91254b2a29b8cbcceba0e69a99087b68259b656`.
No emulator interaction, builds, production edits or test changes were performed.
`../../reports/controller-facing-selection-diagnosis.json` binds the evidence,
frozen production sources and authored animation bytes.

The bank smoke failure is an immediate-log contract race. Both runs used the
same d17 APK. The requested Walk sequence was correct in both runs, and the
failed run subsequently applied the correct Walk clip and moved the body.

| Saved log | Evidence |
| --- | --- |
| bank `prince-bank.log:385` | state4, flags23c1, requested sequence280; retained current clip1040 |
| bank `:386` | immediate set-speed service reports retained clip1040, timeline scale1 |
| bank `:390..394` | old Idle completion events27,25,23 followed by selection24,26 |
| bank `:399..400` | applied clip1126 authored `ev_step_left`, scene before Step115 |
| bank `:401` | Step120 actual clip1126, ms773, body(-22.2777,10.5979), state4 |
| bank `:315` | earlier body(-22.2777,12.2093); displacement is real |
| movement `live-actor.log:401..402` | state4/root280 selected immediately: clip1126, scale1.29999995 |
| movement `:407` | Step120 actual clip1126 and displaced body |

The failing assertion in `prince_bank_smoke.py:130` first waits successfully
for clip1126 and measured body displacement, then requires a **different**
earlier log line to contain clip1126 and scale1.3. That earlier line is emitted
in the caller's state `set_speed` service, not when a queued selection is
actually applied. `live_actor_smoke.py:163..164` has the same dependency;
its successful run happened to request Walk outside the pending window.

## Original source explains both outcomes

Actual `CharAnimator::ANIM_Set` at `0x3cacb0` loads pending byte+49. When nonzero,
`0x3cacb8` stores only the requested sequence at+50 and `0x3cacbc` returns.
It does not replace the current clip or reset global speed. Pending=false
instead writes global speed1 at `0x3cacc4` and enters `_SetAnim`.
The saved original-instruction fixtures in
`../animation-blend-composition-reentry/source-probes.json` exercise both cases.

The common completion tail clears pending49 at `0x3cb0ec`, reads seq50 and
executes the actual queued `ANIM_Set` at `0x3cb100`, then clears seq50 **after**
that synchronous call at `0x3cb108`. Old repeat initialization is skipped when
seq50 is present (`0x3cb0b8..0x3cb0cc`). Events27/25/23 still occur before queue
consumption. This matches the failed run's chronology.

`ANIM_SetSpeed` at `0x3c93fc` stores requested global multiplier+40 and multiplies
it by the **currently selected** authored step speed+34 (`0x3c9418..0x3c9424`).
The authored asset table directly decodes to Idle262: stepSpeed1, Walk280:
dictionary1126/stepSpeed1.2999999523162842, Run271:
dictionary1114/stepSpeed1.2999999523162842. Walk/Run have one direct step,
Type0 and Loop=-1. Consequently the immediate retained Idle scale1 is expected
for this property's multiplier1; it does not demonstrate a wrong Walk speed.

Frozen d17 and prior verified 4f capture contain byte-identical
`actor_blended_playback.cpp`, `animation_scheduler.cpp`, `visual_timeline.cpp`
and `character_state.cpp`. Their renderer differs in the controller-facing
integration, but the relevant start, set-speed and animator-phase calls were
not changed. In d17, scene completion raises actor pending before the state
request; `BlendedPlayback::start` queues under that pending gate. The subsequent
animator phase consumes it after state update. The immediate logging currently
labels a request as “selected”/“authored restart” even when selection is deferred.

## Proposed bounded correction

Keep source pending and callback ordering. Validate the requested state/root
(Walk4/280 or Run4/271) separately from the later applied clip. Retain the
existing requirement for an actual corresponding clip1126/1114 frame and
measured body displacement. Do not require the earlier request-time log to
already show that clip or its scale.

For genuine live speed verification, add owner-approved telemetry that reads
the actual current timeline scale after queued application, preferably alongside
the periodic actor frame's clip/state. Require that applied observation to
match the authored step speed times current property multiplier. Moving the
request-time log or accepting a stale scale as proof would not establish this.
This report proposes no production scheduler fix.

Pending49 itself and the later current timeline scale are not present in these
logs. The deferred branch is inferred from the frozen control flow and observed
callback sequence; eventual scale1.3 was not directly measured in the failed
bank run. Subsequent bank freeze/recreation assertions were not reached. This
diagnosis does not convert that failed bank report into PASS or claim complete
original live frame parity.
