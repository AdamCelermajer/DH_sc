# The northbound combat setup pulse did not establish an obstruction

Read-only diagnosis of APK `77f1e1cd67c8fe656c95c89f2c9535e7ae37d432c86bd9f9773d96e12e31961e`.
The failing original smoke/log are preserved in
`.local-inputs/player-scene-live-combat-77f1e1cd`. No emulator operations,
production edits, shared tests, package rebuild or position adapter were used.

The waypoint is reachable in the inspected authored floor/decor geometry.
The evidence supports a touch pulse that ended before a motion-bearing source
animation frame; it does not support a wall collision or a heading defect.
Exact hidden live slot clocks were not logged, so the timeline explanation is
a source-backed diagnosis rather than a captured slot-state replay.

## The actual failing observations

The last successful Run pulse moved from
`(-1403.1411,-869.6138,41.7079)` to
`(-1407.8186,-496.9652,128.6967)`. It executed source Move/Run,
sequence271/clip1114, beginning Step798. On release the character selected Idle.

The next pulse requested northward Walk for200ms (wall command875ms). Log
lines868–891 show sequence280/clip1126, flags23c1, timeline speed1.29999995,
entry Step856, then Idle release Step858. Only Steps856/857 ran in Move.
The measured position is unchanged and `blocked` rises30→32. The same log
contains a HWUI784ms stall with DequeueBufferDuration774053600ns.
The earlier Step840 native-body observation reports contacts0/0. There is no
contact observation at the exact failing frame; zero earlier counters alone
would not rule out a later collision.

`model_renderer.cpp::advance_native_actor` increments `blocked_steps` whenever
a Move frame has XY displacement²<=0.000001. This counter is not a source
collision result, floor reject or contact counter. The smoke immediately calls
any settled pulse with displacement<=0.1 “blocked”, without checking whether
the pulse obtained any advancing incoming timeline frames.

## Geometry executed independently

`.local-inputs/player-route-77f1e1cd/geometry.cpp` loads actual `crypt.bdae`,
`crypt01.dwld` and `crypt01.dact`; it runs the genuine reconstructed source
floor selector, PFWorld height/position/direction kernels and exact decor
marker/body producers. The isolated O2 ASan/UBSan executable links the preserved
private central53 dependency snapshot; no sanitizer diagnostics occurred.
This is a native host geometry check, not a new instruction replay of the APK.

All27 center queries, in5unit increments from Y=-496.96521 to-366.96521 at
X=-1407.8186, hit floor0/type0. Source ValidatePosition accepts every query,
and ValidateDirection leaves north `(0,1,0)` unchanged/valid. Floor height
rises128.696655→194.332031; successive5unit steps rise about2.5244, well below
the genuine default100unit height-delta limit. The target Y=-370 lies between
the final accepted samples. Source direction validation's10unit lookahead
also accepts all samples. This checks the same point-based rules as the live
source controller; it does not substitute the old eight-point supported-floor
movement adapter.

The actual82 decor definitions produce four-vertex polygons. The nearest
polygon to the entire remaining segment ending Y=-370 is room0
`_anim_candle2_001`/`crypt_candle2.bdae`, with centerline distance1190.151261.
Subtracting the actual logged Prince radius113.699707 leaves1076.451554units
of clearance. Thus neither grazing a decor collider nor radius growth explains
this failure. The northbound heading of about-pi is correct for the recovered
`look_towards` convention `(x=0,y>0)`; the preceding two Run pulses already
travelled in that direction. Enemy automatic AI is disabled by this smoke.

Asset and dependency hashes, polygons, raw floor outputs, executable hashes
and producing-script hashes are bound by `diagnosis.json`. The available
`native-player-scene-build-capture.zip` now contains a later d57c6e01 APK,
not77f1e1cd; its three world assets match these inputs, but it must not be cited
as an exact77f1e1cd compiler snapshot. Original-source manifests bind the
original ELF36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80.

## Why two scheduled frames can produce no movement

The live ordering is Scene animation→world Step→timers→input/state selection
→animator→UpdatePath/rotation/subobjects. Thus Walk selection at Step856
occurs after that frame's ordinary scene sample.

Original PlayClip0x47680c selects its incoming timeline through SetClip
0x666f7c, clearing initialized/ended. Blender Begin0x36679c rotates slots;
incoming fade weight can be0 at replay time. `BlendedPlayback::animate` skips
timeline updates for a zero-weight slot, while ResetDelta0x366a68/0x3644cc
stores the incoming start sample as baseline. On the next positive-weight
update, source CTimelineController::update0x667104 initializes its clock
without advancing clip time. CalculateDelta0x364444 then sees that same start
sample and returns0. A further scene frame is required before that incoming
sample can advance. Source Move flags23c1 use visual root displacement rather
than a constant physical walking velocity. Idle release invokes Stop/pin and
changes the visual-position policy.

`startup.cpp`/`startup.json` independently replay real fade/timeline/root
kernels with explicitly synthetic40ms timestamps,150ms fade and a linear root.
The incoming samples are `(weight0,uninitialized,clip0,delta0)`, then
`(weight0.266667,initialized,clip0,delta0)`, then
`(weight0.8,clip52,delta52)`. This proves that a Move entry plus one subsequent
frame does not imply displacement. It is not an assertion of the live fade
duration, live absolute timestamps, exact1126 root keys or outgoing slot state.

## Smallest faithful correction

Keep the current route and all measured-position acceptance criteria. No
renderer/controller change or obstruction detour is justified by this evidence.
The harness must distinguish an unscheduled/initializing pulse from actual
no-progress movement. A bounded retry of the same real joystick pulse after
an entry/release interval of only two source Steps is reasonable; retain the
50-attempt limit, signed waypoint progress, finite position checks, displacement
>0.1 and final35unit tolerance. Record the insufficient-frame attempt rather
than accepting it as movement. A pulse that obtains motion-bearing source
frames and still fails displacement must remain a failure.

Alternatively use genuine held DOWN/MOVE and guaranteed UP in cleanup, bounded
by a deadline, until existing authored-event/native observations establish
actual movement; then release and measure settled Idle. This avoids depending
on wall time under GL stalls, but transport latency must still constrain the
Walk approach to avoid overshoot. Do not reintroduce an unbounded held input,
synthetic position, setup teleport, lower displacement threshold or invented
collision bypass. This audit implements neither harness alternative.
