# Original two-slot animation blender

Original ELF SHA256: `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
This is bounded recovery of slot/fade metadata and a source trace of contribution
ordering. It does not change Playback, implement generic blended scene targets,
or establish blended pose/GPU parity.

## Slot construction and clip handoff

BlendedAnimSetController C2 `0x476b4c` calls AnimSetManager::GetAnimator
`0x4762c4` twice at`0x476ba4/0x476bb4` for the same animation-set identifier.
GetAnimator allocates a distinct0xa4-byte AnimatorSet and constructs it with
the shared animation-set intrusive reference at`0x47631c..0x476334`; these are
independent animator/timeline instances, not two pointers to one active timeline.
Its source animation-set compilation/manager lookup remains an ownership service.
The synchronized/raw animation-set source branch at`0x476384` is explicit and
is not reconstructed by this module.

The successful controller constructor allocates0xd0-byte AnimatorBlender at
`0x476c24..0x476c30`, inserts first/second animator in vector order, invokes
compile through vslot0x88 at`0x476ce0`, writes weights[0]=1 and weights[1]=0
at`0x476cf0/0x476cfc`, then attaches the Blender to the root through vslot0x6c
at`0x476d10`. Constructor-local references are released afterward. C1
`0x476ddc` repeats the corresponding sequence at`0x476eb4..0x476fa0`.
AnimatorBlender ctor initializes indices, requested duration, remaining duration,
reciprocal and last timestamp tozero (`0x367060..0x367084`).

Each slot retains its own selected animation index at AnimatorSet+0x50
(getCurrentAnimation `0x65f114`), timeline pointer at+8 (getTimelineCtrl
`0x599870/0x599868`), AnimApplicator object at+0x58, shared AnimationSet pointer
at+0x94 and selected Animation resource pointer at+0x98. SetCurrentAnimation
`0x3674ac` updates that slot's selected resource/reference counters and calls
the actual engine setCurrentAnimation `0x65f8c8`; it does not replace both slots.
Timeline current_ms+4, range+0x10/+0x14, frame_seconds+0x1c,
last_seconds+0x28, current_seconds+0x2c, scale+0x30, clip_index+0x38 and
ended/initialized+0x3c/+0x3d are independently retained per timeline, matching
the already recovered visual_timeline.hpp logical state. Slot applicator
previous timestamp/position and delta at+0x14..+0x2c are also independent;
the blender's own extra/pending callbacks are a further distinct state.

BlendedPlayClip `0x47680c` resolves the blender, calls Blend with signed
controller+0x14 at`0x47682c/0x476830`, then checks clip ID, reads the newly
selected slot, resolves/sets that slot's animation, configures loop/scale, calls
RootNewAnim and finally BlendPost. Thus slot rotation happens even when a later
clip lookup fails; a native implementation must preserve that source side effect
if it exposes the original failure behavior. BlendPost `0x366740` is exactly
bx lr. No animator cloning occurs inside Blend itself.

The previous-clip comparison and getLoop check in BlendedPlayClip refer to the
newly rotated slot's retained prior animation/timeline, rather than an aggregate
single-current-clip identity. Likewise its extra-time argument comes from the
blender applicator. Replacing this with one shared slot changes repeated-clip
restart behavior even before generic blended target output is considered.

CharAnimator::_SetAnimStep reads runtime step+0xc BlendOut and writes
controller+0x14 at`0x3caacc/0x3ca974`. Deferred/forced selection gates instead
write zero (`0x3ca970`). This is distinct from animator+0x44, loaded as the
unused third PlayClip argument at`0x3ca990`. Authored Idle BlendOut100 and
Walk/Run BlendOut0 are established in live-animation-selection/NOTES.md.

## Exact fade metadata

Original Blender fields:

| Original offset | Meaning |
| --- | --- |
| +0x28/+0x2c | animator pointer vector begin/end |
| +0x34/+0x38 | float weight vector begin/end |
| +0x70/+0x74 | current/outgoing slot index |
| +0x78 | duration stored for a future transition |
| +0x7c | signed remaining duration of current transition |
| +0x80 | reciprocal of previous duration, retained if nonpositive |
| +0x84 | last completed animate/update timestamp |
| +0x88 | BlenderApplicator object |
| +0x98 | applicator extra time (+0x10 within applicator) |

Blend `0x36679c` asserts the normal vector has two slots, copies current index
to outgoing, sets current=(current+1)%slot_count, copies **previously stored**
duration+0x78 to remaining+0x7c, updates reciprocal only if that previous duration
is strictly positive, and stores max(new requested duration,0) into+0x78.
It leaves weights and last timestamp unchanged. The requested duration therefore
belongs to the selected clip's future blend-out; it does not immediately become
the denominator of this transition. Repeated/failed selections still rotate slots.

animateNode `0x366cb4` and updateTime `0x366d90` share the same weight prefix:

1. If remaining<0 on entry, retain remaining/weights.
2. Otherwise subtract timestamp-last_timestamp with wrapping32-bit arithmetic,
   store the resulting signed word, and compare it as signed.
3. Positive remaining: outgoing=float(signed remaining)*reciprocal;
   current=1.0f-outgoing. Negative/zero remaining: outgoing=+0,current=1.

There is no clamp of positive outgoing values to[0,1], no timestamp monotonicity
guard, and no clamp of remaining tozero. Backwards/wrapping clocks can create
extrapolating weights. The source updates last_timestamp at its **tail**,
`0x366d64` or`0x366e8c`, after contribution/applicator/callback work.

normalizeWeights `0x366594` sums in vector order starting at+0. If sum compares
numerically equal tozero, it sets only weights[0]=1 and retains all other slots.
Otherwise it divides each weight by that sum in order. In particular[1,-1]
becomes[1,-1], not[1,0]. Sampling decisions occur before this normalization.

## Sampling, target contributions and displacement order

Normal animateNode:

1. Weight prefix above.
2. Virtual blender slot0x50 at`0x366d2c`, resolving to
   CSceneNodeAnimatorBlender::applyAnimationValues `0x65e350`.
3. That method visits slots in vector order, calling animator vslot0x4c only
   when weight is numerically nonzero (negative weights also sample), then
   normalizeWeights at`0x65e3cc`.
4. For each target in vector order: check track and bound target, obtain the
   first animator's extended track through vslot0x58, then call its type-specific
   value/applicator vslot0x18 at`0x65e468`, passing contiguous per-slot values,
   normalized weights/count, bound target and cloned applicator info.
5. BlenderApplicator::AnimateNode `0x366888` runs after scene-target application.
6. Current slot's timeline is fetched through vslot0x44; CheckCallback
   `0x36440c` executes, then last_timestamp is stored.

updateTime uses slot v0x14 for each nonzero weight at`0x366e50`, normalizes at
`0x366e60`, checks the current timeline callback and commits last timestamp.
It does not call BlenderApplicator::AnimateNode. This distinguishes the culled
time-only path from ordinary rendered scene animation.

computeAnimationValues `0x65e48c` samples the same nonzero-weight slots and
normalizes, but calls typed track vslot0x10 with output storage at`0x65e5a4`.
getAnimationValue on the blender itself `0x65e34c` is a no-op; its actual target
contribution functions and per-slot storage are the value producer.

compile `0x65ed98` allocates/zeros weights and per-target slot storage, binds
the first slot's output buffer through slot0x68, then uses the first slot's
track metadata to bind each subsequent slot through0x60 to its own contiguous
buffer. +0x4c holds per-target buffer bases; +0x58 bound targets; +0x64 cloned
applicator info. It calls forceBind `0x667f18` after marking compilation done.
The verified AnimatorBlender vtable primary address point is symbol0x9626e8+0xc:
slot0x50 resolves applyAnimationValues,0x4c computeAnimationValues,0x88 compile
`0x366764`, which tail-branches to`0x65ed98`. AnimatorSet slot0x4c resolves
`0x367368`, which restores selected animation+0x50 from selected resource+0x98
then tail-branches to engine computeAnimationValues. The slot0x50 apply variant
`0x36737c` does the equivalent restore before engine applyAnimationValues.
setTarget `0x65e244` releases prior applicator info through its vslot4 and clones
new info through vslot8; it does not simply borrow the supplied pointer.

BlenderApplicator::AnimateNode zeros its output delta (+0x24/+0x28/+0x2c), then
visits **all** animator slots, including zero-weight ones. Each slot fetches its
timeline through0x44, evaluates the displacement target at timeline current_ms
(+4) through0x7c, resolves the actual slot applicator via GetApplicator
`0x369160`, and executes CalculateDelta `0x364444` using scene timestamp.
It accumulates each slot's delta multiplied by that slot's normalized weight,
in slot order. ResetDelta `0x366a68`, when its reference node exists, resets only
the current slot; evaluation uses that timeline's clip-start+0x10 before actual
ResetDelta `0x3644cc`. Older slot delta/timeline state remains independently
retained. SetRefNode `0x366b80` first binds the blender applicator then visits
both slot applicators in order. CheckCallback uses only the current slot and
clears the blender applicator's pending byte after invoking the callback.

The quaternion target handler `0x6130d4` finds the first nonzero contribution,
copies that quaternion, and incrementally combines later nonzero contributions
using ratio=next_weight/(accumulated_weight+next_weight) through the recovered
quaternion interpolation routine `0x612d00`. Weight exactly1 can return a direct
copy. This is typed target interpolation; it is not interpolation of every
rendered matrix. Other typed handlers, target binding and animation track
producers remain a separate scene-pose recovery task.

## Native API and audit scope

animation_blender.hpp provides32-byte BlenderState and separate begin,
update_weights, normalize C kernels. They retain the source phase boundaries;
the caller owns both animator/timeline states, per-target values/applicator info,
reference node and callback policy, and commits last_time after completed work.
The module accepts exactly two valid slot indices and finite incoming float
metadata; malformed state/null pointers return1 atomically. Original routines
do not impose this native caller contract. Finite arithmetic may generate IEEE
exceptional output; comparison preserves NaN classification when generated.

The ARM64 differential executes actual complete Blend and normalize routines.
For both animateNode/updateTime fade prefixes it stops exactly before the first
virtual sampling service at`0x366d18/0x366df4`. It does not intercept a fade or
normalization algorithm.2,504 original cases and18 malformed-state checks pass;
host original-derived replay with ASan/UBSan also passes2,504/18 with no mismatches.
No production build, integration, renderer change or APK mutation was performed.

Remaining integration requires two independently advancing source timelines and
clip/root delta histories, source slot target buffers, typed target blending and
the clone/release/binding lifecycle above. This module alone does not implement
blended pose output or verify all original callbacks during a blended frame.
