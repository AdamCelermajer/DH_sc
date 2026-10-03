# Original visual timeline and replay tail

Source: desktop oracle `libDungeonHunter2.so`, SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
`original-functions.json` records individual original addresses, sizes and hashes;
`original-functions.asm` preserves the captured ARM32 instructions. The native
module uses logical fields and explicit services, not an original engine overlay.

## Timeline

`CTimelineController::update` at 0x667104 receives signed absolute milliseconds.
It converts them to single-precision seconds. On its first update it preserves
the current position, initializes the clock and reports zero elapsed time. Later
updates advance by `(now-last)*scale`, retaining single-precision intermediate
rounding. Negative elapsed time selects the start boundary; all other elapsed
values select the end boundary. Crossing is strict: equality does not complete.

A looping crossing wraps with `fmodf(current-boundary,length)` and emits a
callback on every crossing. A zero length uses a zero remainder. A nonlooping
crossing clamps to the boundary, sets the ended byte, and emits the callback only
when that byte was zero. `frame_seconds` is the elapsed magnitude (negative
elapsed is negated by its sign bit). Inverted ranges, backward clocks, negative
scales and IEEE NaNs/infinities are retained rather than sanitized into a new
clock policy. Signed integer arithmetic wraps as original ARM instructions do.

Completion callbacks run after the seconds, clock and ended fields are written,
but BEFORE `current_ms` is recomputed. Reentrant jump/range/scale calls therefore
affect the final millisecond conversion. The native conversion matches original
AEABI saturation and NaN behavior without undefined C++ float-to-int conversion.

`init` at 0x666f04 changes the integer range only. `jumpTo` at 0x666f10 writes
position and clears initialized/ended. `setRange` at 0x666c38 ignores supplied
bounds while a clip library is bound; its optional jump still uses the current
clip start. `setClip` at 0x666f7c resets initialized/ended and adopts the resolved
clip bounds. Clip bound virtual lookups are explicit fixture facts in this
module; the existing authored clip loader owns that resolution.

## Completion and replay

`Animator::_HandleAnimEnding` at 0x366224 computes overshoot from the NEW seconds
position and OLD integer position, then sets the applicator pending byte.
`CharAnimator::CalculateExtraTime` at 0x3c90f8 performs the same calculation and
stores the result separately. Their integer formula is:

```
frame = trunc(frame_seconds * 1000)
remaining = wrap(trunc(current_seconds * 1000) - current_ms)
extra = remaining >= 0 && remaining < frame ? wrap(frame-remaining) : 0
```

A missing timeline leaves the previous extra value unchanged; notification still
sets pending. `CharAnimator::__Callback` at 0x3c90ec sets a pending byte;
actual authored step advancement occurs later in CharAnimator::Update.

`BlendedAnimSetController::PlayClip` at 0x47680c resolves/selects an animation,
rejects mapped index -1, obtains its applicator, and checks the CURRENT loop flag.
Timeline virtual slot +0x44 is **getLoop** at 0x666c30, not an ended query. If the
selected index equals the previous index and the current loop flag is false,
the timeline jumps to clip start plus applicator overshoot. It then sets the
requested loop byte and scale 1, calls root NewAnim, and calls BlendPost.
Native `dh2_timeline_replay` reconstructs this post-selection tail. Existing
nested authored selection, controller binding, refcounts and blend weights
remain owned by the existing animation scheduler/controller bridge.

The ordered replay services are NewAnim then BlendPost. Original root NewAnim
at 0x35d624 enables/disables displacement and, when displacement is enabled, a
root exists and its last timestamp is nonzero, resets delta at timestamp+1 and
synchronously calls root onAnimate at the prior timestamp. A native NewAnim
service can run `dh2_timeline_update` synchronously and then sample SceneBinding
with the resulting position and restart flag. Replay returns the position AFTER
those services. Timestamp zero does not request the original root restart.

The blended controller does not set scene flag 0x200 in PlayClip. That write
belongs to the separate base `AnimController::PlayClip` at 0x474b50; scene
ownership policy must keep these paths separate. Character animation step
setup passes loop=false. Step +0x1c is displacement, not looping. Step +0x30
is the authored speed factor, multiplied by CharAnimator +0x40 for controller
scale. Replay's scale=1 occurs before this later caller scale assignment.

The caller's CharAnimator +0x44 argument and applicator overshoot are distinct.
_SetAnimStep reads +0x44 into PlayClip r3 at 0x3ca990; the blended PlayClip does
not consume that r3 value. Its same-clip jump uses applicator +0x10 (blender
+0x98), captured during completion. _SetAnimStep does not recalculate extra;
no branch/pointer/relocation caller of CharAnimator::CalculateExtraTime was
found in the supplied ELF. Do not recompute applicator extra after timeline
current_ms has finalized. AnimatorSet/Blender end handlers first set applicator
pending; CheckCallback later calls Character::__Callback, which sets the
character pending byte for its actor phase. The live adapter can observe both
phases without conflating their stores.

## Verification and integration boundary

`kernel-fixtures.bin` contains 5,970 original-derived records with 455 callback
state snapshots, including stateful forward/backward traces, exact endpoints,
zero/inverted/wrapped ranges and unusual IEEE values. Reentrant callback helper
methods execute original/native instructions. `replay-fixtures.bin` contains
640 original full BlendedPlayClip control-flow cases, 1,212 ordered callbacks
and 312 actual root restart branches. Selection/binding and root reset backend
are observed fixtures; root onAnimate optionally executes the actual timeline
update. This does not claim reconstructed full scene animation or blend setup.

ARM32 oracle versus ARM64 producer comparisons have zero mismatches. Separate
ASan/UBSan host executables pass both corpora and 12 malformed-input atomicity
checks. Reports record corpus hashes and scope. Build the standalone ARM64
oracle with `tools/build_visual_timeline_oracle.ps1`. The two differential
scripts accept `--engine`, `--library`, `--report`, `--reference-output`.
The host sources are `tests/visual_timeline.cpp` and
`tests/visual_timeline_replay.cpp`; each accepts its corresponding fixture path.

The live frame clock must feed original scene animation before physics Step,
then actor animator/path/subobjects. It must not derive clip position from a
wall-clock modulo or defer the synchronous replay sample into a later frame.
