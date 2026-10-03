# Two-slot animation composition discovery

Source ELF SHA256: `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
`original-functions.json` identifies every captured routine by address, byte size
and SHA256; `original-functions.asm` contains those original ARM instructions.
This is source discovery and original-only fixture execution. It does not change
production, build an APK, operate an emulator, or establish native blended pose
parity. The existing 2,504-case fade/slot kernel in
`../animation-blender/NOTES.md` remains the metadata implementation and audit.

## Ownership and the two slots

BlendedAnimSetController construction `0x476b4c` obtains two distinct AnimatorSet
instances from `0x4762c4`, both retaining the same immutable animation-set resource.
It compiles the blender, initializes weights to `[1,0]`, and attaches one blender
to the actor root. Each slot has a separate timeline, selected animation, event
manager, pose-output buffers, accessor key cursors and AnimApplicator root history.
Only resources are shared. Starting a new clip does not clone the outgoing slot.

Important original fields (ARM32 layout only):

| Owner | Field | Meaning |
|---|---|---|
| AnimatorSet | +8 | timeline |
| AnimatorSet | +0x18 | event-manager intrusive reference |
| AnimatorSet | +0x1c/+0x20 | event callback/context |
| AnimatorSet | +0x24 | compiled engine animation set |
| AnimatorSet | +0x28 | bound per-target output pointers |
| AnimatorSet | +0x34 | owned cloned applicator-info pointers |
| AnimatorSet | +0x40 | accessor key cursors |
| AnimatorSet | +0x4c | selected clip's binding-record base index |
| AnimatorSet | +0x50 | selected engine animation index |
| game AnimatorSet | +0x58 | its AnimApplicator |
| game AnimatorSet | +0x94/+0x98 | shared set/selected game resource references |
| Blender | +0x28/+0x2c | slot vector begin/end |
| Blender | +0x34/+0x38 | weight vector begin/end |
| Blender | +0x4c | per-target contiguous slot-value buffer bases |
| Blender | +0x58 | bound scene/material targets |
| Blender | +0x64 | owned cloned applicator-info pointers |
| Blender | +0x70/+0x74 | current/previous slot indices |
| Blender | +0x88 | aggregate BlenderApplicator |

Compile `0x65ed98` lays out each target's values contiguously in slot order,
using that target's extended track value size. It binds the first slot through
vslot0x68; subsequent slots use first-slot channel metadata and
setCompatibleTarget `0x669574` to find a compatible URI/type/component and bind
their own buffer. It zero-initializes the buffer allocation. It allocates target
and applicator-info arrays and calls forceBind `0x667f18`. Both blender setTarget
`0x65e244` and AnimatorSet setTarget `0x65f208` release the previous info through
vslot4 and clone a new supplied info through vslot8. A material parameter index
inside applicator info is therefore owned metadata, not a dangling borrowed view.

## Compiled channel union and missing tracks

CAnimationSet::addAnimation `0x6601fc` deduplicates channels in insertion order
using URI and the original compatibility bit table. It additionally compares
material component indices for type0x0e and the relevant string for type0x56.
A native compiler must preserve this ordered channel union and compatibility
rules; node/type sorting is not a demonstrated substitute.

CAnimationSet::compile `0x660710` gathers library channels, applies optional
template filtering/augmentation, and allocates 12-byte records for every
`clip_index * union_target_count + target_index`. The records are initially zero.
For each record it queries the clip database's default value `0x61c6bc` and
blendable animation `0x61c1e0`. A found animation yields mode2 and its accessor;
an absent animation yields mode1. If the clip lacks a default and an animation-set
transformation template exists, its getDefaultValue `0x6e23d0` supplies a fallback.
Template defaults are obtained from registered original scene-node databases,
not synthesized arbitrary neutral values. Unsupported channels can be removed
in the earlier pass when neither clip nor template supplies a value and the set
policy permits removal (`0x660a18..0x660adc`). That policy must be recovered before
claiming a complete generic compiler.

`0x6e24e8` registers position(type1), quaternion(type5) and scale(type10) targets
for an original scene node, then recursively its children. The database default
lookup `0x61c2f8` resolves authored node properties: position/component types1..4
borrow node+0x0c, quaternion-related types5..9 borrow node+0x18, scale types10..11
borrow node+0x28; other channels follow distinct database/material routes.
Do not infer these defaults from the animated, compensated current graph.

AnimatorSet computeAnimationValues `0x367368 -> 0x65f5dc` first advances its timeline
and events via `0x667c48`, then re-fetches its selected timeline/resource/clip.
For every enabled, bound target:

1. Copy a nonnull record default into the slot's target buffer using the extended
   track's getValueSize, even when mode2 is about to evaluate the track.
2. For mode2, evaluate SAnimationAccessor::getValue `0x66a1a8` into that buffer
   using source key/segment selection and the per-target cursor.
3. For another mode with null default, leave the buffer's previous bytes intact.

Thus missing tracks may have authored defaults, template defaults or retained
slot values. Replacing this with two independent Player::sample calls would
reset all scene nodes to each player's rest graph and erase that distinction.
Current Player also rejects duplicate property tracks and hides raw track data;
a compiled target-buffer sampler is the required next boundary.

## Typed composition and actual target setters

`blendTargets` is not a separate discovered symbol. The actual dispatch loop is
applyAnimationValues `0x65e350`; computeAnimationValues `0x65e48c` uses the same
slot sampling and normalization but calls the value-producing vslot0x10 instead
of the scene-applying vslot0x18.

| Value | Original getter/apply | Source calculation |
|---|---|---|
| material scalar | 0x620134 / 0x62008c | slot-order weighted sum |
| position vector3 | 0x626b4c / 0x6284a4 | three separate slot-order weighted sums |
| scale vector3 | 0x6275fc / 0x62d634 | same |
| position-X / scale-X extended tracks | 0x626df8 / 0x6278a8 | full three-float values, same sums |
| quaternion | 0x6130d4 / 0x620968 | first-nonzero plus incremental interpolation |

Scalar/vector counts of one copy the input ignoring its weight; zero counts
produce +0. Other counts multiply and add in original slot order from +0 with
separate single-precision rounding, including zero-weight products. This is not
the algebraically equivalent `a+(b-a)*weight`, which can differ in finite bits
and exceptional-value behavior. No independent division by total weight occurs
inside these handlers; blender normalization occurs before typed dispatch.

Quaternion `0x6130d4` starts with identity, skips numeric-zero weights (including
-0), copies the first nonzero quaternion and accumulated weight, and immediately
returns that exact copy when its weight equals1. It ignores all later slots in
that branch. Otherwise each later nonzero slot uses
`next_weight / (accumulated_weight + next_weight)` with `0x612d00`. All-zero or
nonpositive-count input produces identity. There is no positive-weight filter or
extra final quaternion normalization; first-weight1 can preserve a nonunit input.
The existing `dh2_quat_slerp` reconstructs `0x612d00` and should be reused.
Negative/extrapolated weights and source normalization's `[1,-1]` zero-sum case
must retain these rules. Arithmetic NaNs require class comparisons, with no
portable payload/sign claim. The generic CVector3dEx `0x6e3fb0` uses a distinct
incremental ratio/lerp algorithm and is not the scene-position/scale handler.
The additive quaternion/vector methods are likewise separate from this path.

The typed wrappers are position `0x62859c`, scale `0x62d72c`, quaternion
`0x6209b4`. They dispatch to actual scene-node position vslot0xa4, scale0x94 and
rotation0x9c. Setters `0x59712c/0x5970c4/0x5970f4` copy values into local
position/quaternion/scale and OR dirty bits8/2/4. They do not blend matrices.
Material scalar application instead calls `0x5c6b8c` with the unsigned16-bit
parameter index at applicator-info+8. Component channels remain ordered full
vector writes; combining them into one unordered node transform loses source
write ordering.

## Rendered phase, root ownership and synchronous events

Normal AnimatorBlender animateNode `0x366cb4` does:

1. Existing source fade prefix using scene timestamp and retained last timestamp.
2. In vector order, compute pose for each slot whose weight is numerically
   nonzero, including negative weights (`0x65e388..0x65e3c4`). Each gate re-reads
   the live weight: a synchronous earlier event can change whether the next slot
   runs. The slot vector pointer is also re-read. Do not snapshot a complete
   frame's clip selections or defer all event-driven clip changes.
3. Normalize weights through `0x366594` after sampling. Sum0 sets only weight0=1;
   it retains later weights. Sampling can be skipped before that fallback.
4. Apply typed targets in compiled target order after enabled/bound checks.
5. Run aggregate BlenderApplicator::AnimateNode `0x366888`.
6. Resolve the CURRENT slot's timeline and CheckCallback `0x36440c`.
7. Commit blender last timestamp at the end (`0x366d64`).

Root aggregation initializes its XYZ delta tozero and a single scratch XYZ tozero,
then visits ALL slots, including zero-weight slots. Per slot it reads that slot's
timeline current_ms, evaluates the reference position channel via
`0x367390 -> 0x65f7b4`, calls that slot's actual CalculateDelta `0x364444`, and
adds normalized-weight times delta in slot order. The Y and Z multiplications
precede X, but independent axis accumulation order is unchanged. Evaluating the
root channel uses the same compiled mode/default/accessor records as pose; it
does not advance a timeline or trigger events. The shared scratch is NOT cleared
between slots: absent default/non-mode2 leaves the prior slot's sample in it.
The aggregate delta is the one producer consumed by RootSceneNode displacement;
it must not be applied once per slot to the actor.

Zero-weight slot pose timelines do not advance in step2, but root evaluation and
history calculation still run in step5 using their retained current_ms. They must
remain owned even when invisible. Re-enabling a previously initialized timeline
uses its source clock history rather than an invented independent modulo clock.

SetRefNode `0x366b80` binds the aggregate and both slot applicators. Original
AnimApplicator::SetRefNode `0x36473c` retains the scene node, searches channel URI
and type1, and stores the LAST matching union index, continuing after a match.
The source missing index is -1. ResetDelta `0x366a68` resets ONLY the current
slot, evaluates its root at clip start, and calls `0x3644cc`; outgoing history
remains. RootNewAnim `0x35d624` then performs the source immediate root animation
pass, including events. Aggregate scratch/history ownership must match both the
normal scene call and this synchronous replay path.

SetCallbacks `0x366eb8` installs authored event callback/context on BOTH slots
and their currently owned managers. Their timeline end callbacks instead route
to blender `_HandleAnimEnding` `0x366628`, which compares the callback timeline
against the CURRENT slot's timeline. An outgoing end is ignored. A current end
stores captured extra_ms at aggregate applicator+0x10 (blender+0x98) and pending
at applicator+0x30. Aggregate callback checking follows target/root work. Preserve
the source pending-clear-after-callback and already recovered CharAnimator
finite-closure/deferred-start rules; do not recompute extra after timeline clamp.

Both nonzero slots may dispatch authored events in slot order, without scaling
payload or lag by weight. Each manager independently retains its old batch across
synchronous clip changes, as in `../actor-playback-events/NOTES.md`. No dominant
slot event filter exists here. Event28 keeps original signed lag/string payload;
accepted finite closure22 remains actor/AI/FSM policy after the scene/physics
boundary. The current slot may change during either manager's callback.

Time-only update `0x366d90` calls slot vslot0x14 for nonzero weights, normalizes,
checks only current completion and commits the timestamp (`0x366e8c`); it does
not produce pose or root displacement. It must be a separate coordinator path.

## Minimum next implementation API and integration order

These are proposed native types and seams, not recovered studio headers:

```cpp
struct CompiledTarget {
    ChannelIdentity identity;   // URI, source type, component/material metadata
    ValueKind value_kind;       // scalar, vector3, quaternion; source byte size
    TargetBinding destination; // scene property or explicit external applicator
};
struct ClipTarget {
    uint32_t mode;
    DefaultValue default_value; // optional immutable byte view with owned backing
    AccessorBinding accessor;  // optional original-format accessor/segment view
};
struct CompiledAnimSet {
    vector<CompiledTarget> targets; // source union order
    vector<ClipTarget> bindings;    // clip-major, target-minor
    vector<ClipResource> clips;     // retained backing, bounds and event tracks
};
struct AnimatorSlot {
    TimelineState timeline;
    EventManagerState events;      // cursor plus retained synchronous batch lease
    SelectedResource selection;
    vector<KeyCursor> key_cursors;
    AnimApplicatorHistory root;
    // Pose output is the slot's span in each shared per-target value allocation.
};
struct BlendedPlayback {
    BlenderState weights;          // existing fade kernel, no replacement
    AnimatorSlot slots[2];
    SharedTargetBuffers pose;
    AggregateApplicator aggregate; // reference target, delta, extra, pending
    SchedulerState actor_sequence; // one actor scheduler, not one per slot
};
```

Expose read-only compiled target/default/accessor views from engine-animation and
an evaluation operation writing one typed target buffer at explicit current_ms.
It must preserve default-copy/retained-output behavior and optional key cursors;
it should not reset or replace an entire scene. Compile/bind once from actual
animation banks plus authored template graph, retain backing and cloned external
applicator info, and maintain source union order. Provide a root-target evaluation
variant with caller-owned scratch and identical default/accessor behavior.

Implement the two-slot coordinator after the typed kernels: begin/play rotates
the existing metadata slot, configures only that slot, invokes current-only reset
and source replay; scene_phase follows the seven steps above. Typed setters write
local properties and graph dirtiness. Aggregate root compensation and world
projection run once through the existing source root/VisualMotion bridge. Actor
FSM/animator closure work still follows physics Step. Keep live slot/event reads
at actual source boundaries and only preserve the old event-manager batch lease;
do not apply a generic generation abort to the outgoing batch.

First validate binding/default records and per-target samples against the original
on real Prince35-node graph and two different authored clips. Then validate entire
ordered scene/root/event phases including zero weights, old-duration transitions,
same-clip replay, synchronous event selection changes, current/outgoing completion,
missing channels and partial/component writes. Only then attach the single blended
output to renderer skinning and actor motion. Quaternion kernels alone or weighted
TRS snapshots do not establish this composition parity.

Generic material tracks, full compatibility table, raw/synchronized set-resource
branches, all template filtering policies and opaque external applicator binding
still require producer/lifecycle recovery. Restrict an initial supported bank
explicitly rather than silently assigning neutral defaults. No full blended
GPU/pose or original application/FSM parity is claimed by this discovery.

## Original-only executable evidence

`probe_original.py` executes `0x65e350`, actual normalization, typed wrappers and
scene-node setters with synthetic synchronous sampling/binding observers. It
checks32 cases,43 sampling calls,80 target writes and exact dirty words, including
source live-weight mutation from the first slot, disabled/null targets and
normalization zero-sum behavior. `source-probes.json` preserves all raw node words,
inputs and ordered calls. Sampling/key evaluation, full asset binding, root and
event manager composition are explicitly outside that fixture. Native comparisons
are zero; this evidence does not replace future composition differentials.

Reproduce with the project's configured Python/Unicorn dependencies:

```powershell
python port/level-world/reference/animation-blend-composition/probe_original.py `
  --engine .local-inputs/libDungeonHunter2.so `
  --output .local-inputs/animation-blend-composition/composition-probes.json
```
