# Two-slot native node-transform composition

The new `actor::BlendedPlayback` implements the recovered two-slot composition
and CharAnimator scheduling services. `compile` preserves the static
CAnimationSet node1/5/10 fixture domain. The separate `compile_dynamic` follows
the CDynamicAnimationSet registration/pruning/default policy independently
proved by the raw sampler worker. Exact AddTemplateAnim dictionary identity
and actual library registration order remain application producer boundaries.

Original desktop oracle ELF SHA-256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
No original instructions or ELF are used by the native implementation.

## Public ownership and setup

Compile once with the immutable ClipBank, authored scene and bound
SceneBinding. An overload accepts the exact caller-supplied registration
order. The convenience overload uses the native map's numeric clip-ID order;
that order is a port producer, not a recovered original library-order fact.
Authored template registration filters unsupported/unregistered URI targets;
it does not invent a channel for every graph property.

Dynamic setup accepts an explicit default-library Player and mismatch policy
retain1 (game producer) or prune0. The default comes from that resource's exact
model clipDB, not a scene graph reconstruction. Both fresh dynamic slots select
library0 before PlayClip, as actual base constructor init660af4 invokes
setCurrentAnimation(0); timeline constructor666e40 supplies loop1. Key cursors
are zero and no scene clock advances during setup. In the dynamic host fixture
the exact Prince_modular resource is registered first with native fixture key
20000. That key/order is deliberately not claimed as the original global
dictionary identity. Dynamic clip bounds come from BRES root+1c/+20.

Each slot owns an independent timeline, event cursor, root-history Delta,
resource identity, generation and one key-search cursor per target. Every
ordered target has two separate persistent value spans. Compiled resources
and event backing are immutable and shared. Selection does not reset raw
accessor key cursors: the original `setCurrentAnimation` (`0x65f8c8`) does not
reset them. Cursor initial values are zero from animator construction/init.

`current_timeline()` and `current_clip()` select the live current slot.
`scene_phase(timestamp)` is the rendered scene path. `time_phase(timestamp)`
is the culled path, with no pose/root sampling or actor root timestamp commit.
`animator_phase(..., extra_ms)` belongs after the physics Step; supply the
completion extra captured by the timeline callback, never recompute it from
the finalized/clamped current_ms. `start` queues every request while actor
pending is set. `swap` retains metadata/step/repeat/time for the source
ANIM_Swap branch, or invokes the source restart branch.

The observer receives source event IDs22..28. Authored28 has the original
string payload and unscaled lag. Scheduler22..27 have null payload:
22 uses sequence phase3, 27/25/23 animator phase4, 24/26 selection phase5.
Source scheduler callers ignore RaiseEvent returns. Original AI gating,
consumers, equipment/audio and FSM forwarding are explicit caller services;
the observer does not automatically accept those services or synthesize them.
The added parent `character_animation_events` router is a separate boundary.

Consumer adapters expose live depth, step index/count, metadata SetStep and
SkipNextStep, and StopLoop(bool). Public ANIM_SetStep (`0x3c9484` ->
`0x3c946c`) only writes the index; it does **not** call private _SetAnimStep,
raise26 or replay a clip. SetStep and Skip are suppressed by closed48; Skip
uses unsigned wrapping. Closed getters return indexUINT_MAX/count0.
StopLoop (`0x3c948c`) writes current loops0 if not closed; true additionally
sets4a and false preserves an already set4a. On an eligible animator update,
4a is cleared and pending49 raised before the ordinary pending gate
(`0x3cafc4..0x3cafdc`). `animator_phase` models that eligible-update branch;
application/FSM update eligibility remains a caller boundary.

## Source operation order

Rendered `AnimatorBlender::animateNode` (`0x366cb4`) executes:

1. Fade prefix with the original duration/remaining/reciprocal state.
2. Slot0 then slot1: re-read each live nonzero weight, advance its independent
   timeline/event manager and sample enabled, bound target spans. Negative
   weights remain nonzero; event dispatch is not multiplied by weight.
3. Normalize weights, then apply ordered full-position, quaternion and scale
   targets through the recovered typed kernels.
4. `BlenderApplicator::AnimateNode` (`0x366888`): initialize one shared XYZ
   scratch, visit **all** slots including zero weights, sample current root
   position into that scratch, calculate each independent history and sum
   weighted deltas. Missing records retain the previous slot's scratch bytes.
   An absent reference channel skips aggregate history work entirely.
5. Apply the aggregate root displacement once, then current-slot completion
   CheckCallback, then commit blender last_time. Scene-phase owner timestamp
   is committed after the scene operation returns.

`updateTime` (`0x366d90`) advances nonzero timelines/events, normalizes, checks
current completion and commits last_time without pose/root integration.
SetScale (`0x3666d8`) visits both timelines independent of weights.
ResetDelta (`0x366a68`) resets only the current history at timestamp+1. A bound
reference node with channel index-1 still resets it to XYZzero; outgoing
history is retained. NewAnim can synchronously execute the rendered path at
the existing owner timestamp. Retained old-manager event batches finish after
nested selection; generation prevents them overwriting the new cursor.

## Source scheduling and synchronous selection

`CharAnimator::Update` (`0x3caf3c`) retains its immutable sequence row before
27, then reads live frame state after callback return. Type1 intermediate
steps emit27, increment step, emit23, select the next leaf while pending=1,
then run the common pending tail. They skip25. Type0 repeat emits27,25,
decrements positive loops, emits23; a queued request skips old selection/RNG/
replay. Otherwise selection/replay runs, then the captured remaining loop
word is restored. A synchronous StopLoop inside replay is overwritten by that
source restoration. Finite root exhaustion marks closed before22; an explicit
second pending completion still emits27/25 while closed suppresses only22.
Child exhaustion recursively updates the parent and then executes the child's
common tail after the parent's tail.

The common tail clears actor pending, calls ANIM_Set with the still-visible
queued sequence, then clears that queue **after the full synchronous selection
returns** (`0x3cb100..0x3cb108`). A completion raised during consumed selection
can remain pending while a newly queued request is cleared by this tail.

`AnimationScheduler::start_with_services` and the selection services accepted
by `complete_with_services` follow `_SetAnim` (`0x3cab38`) and `_SetAnimStep`
(`0x3ca79c`): sequence/depth/loop metadata is written before24; RNG selection
uses the retained sequence row after24. The next step lookup re-reads the live
frame sequence. Its step pointer is captured and step index stored before26;
the retained pointer supplies redirect/clip/speed/MoveGO after26. Redirects
recursively expose24/26. Leaf preparation is a caller service; the coordinator
uses it for actual PlayClip/NewAnim/SetScale. Immutable table resources are
required across callbacks. Backend failure propagation is a native contract,
not a source event veto. Old `start` and `complete` APIs/corpora are unchanged.

## Corrected historical replay binding

Fresh actor closed48 is1 from actual CharAnimator constructor3c906c at3c90a0;
pending49/stop4a are0 and queued50 is-1. First selection24/26 can observe that
closed flag before the valid visual/clip leaf clears it. The native owner
initializes sequence_closed1 accordingly, instead of treating an unselected
actor as already playing.

The old standalone `dh2_timeline_replay` fixture treated PlayClip virtual44 as
a loop getter. Actual source tests **IsEnded** after unconditional SetClip,
which clears ended/initialized while retaining loop. Therefore same incoming
clip ID with old loop1 still jumps to start+aggregate extra. The new coordinator
executes that branch explicitly before delegating the remaining replay facts.
Historical helper/gold/reports remain byte-for-byte preserved; their old
virtual44 fixture is not evidence for complete caller binding. The independent
13 selection probes in `animation-blend-composition-reentry` cover this finding.

## Saved evidence

| Evidence | Compared cases and observations | Explicit services |
| --- | --- | --- |
| Original full composition versus native host capture | 420 records:400 rendered,20 culled;431 ordered slot calls;800 root calls;11,600 scene setters;420 completion callbacks | Compiled raw pose/root buffers and timeline/event advancement supplied; completion pending forced for ordering |
| Original composition versus dynamic host capture | Separate420 records, same ordered-call counts; actual dynamic model DB defaults and constructor library0 setup | Same explicit sampler/timeline services; dynamic raw sampler separately117,030 original/native records |
| Original Update/ANIM_Set versus native services | 48 cases,18 nested unwinds,6 retained-closed repeats;180 callbacks,51 preparation boundaries | Immutable resolved table rows, logger and leaf selection services |
| Original _SetAnim/_SetAnimStep versus native selection | 72 cases,36 nested starts;216 callbacks,48 compatible metadata swaps | Logger and leaf equipment/audio/visual service; actual callback and redirect instructions execute |
| Original public animator controls versus native adapters | 2,160 cases;432 SetStep,432 Skip,432 StopLoop,864 getters;depth0..2, closed/flag state, unsigned wrapping | Resolved sequence rows for GetStepCount |
| ASan/UBSan real-asset host replay | 17 clips,35 Prince nodes,29 static targets;3,280 scene frames;2,500 root/speed checks;2,442 zero-weight histories;796 retained terminal frames;20 time-only frames | Source tables/cache assets; caller speed factors1.0/1.3; explicit observer services |
| Focused native source-service regressions | 2 counterexamples: queued selection reassertion/clear-after-return and StopLoop overwritten after repeat replay | Controlled callback reentry, independently observed actual original traces in the reentry review |

All compared records pass with zero mismatches and zero sanitizer findings.
These are host x86_64 comparisons to original ARM32 instructions; this bundle
does not claim a packaged ARM64 coordinator replay. Existing independently
audited typed kernels, raw static sampler, timeline and root kernels supply the
component behavior. The host binder verifies regenerated capture hashes,
executable/shared-library/source hashes and every selected asset's exact bytes
against cache SHA-256
`3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`.

Current limitations: actual application AddTemplateAnim identity/library
registration order; scalar/material and component/compressed channel producers; complete original
AI/audio/equipment/full application behavior; rendered GPU parity. The two-slot
source composition is proved for the supplied service domain, not inferred
from the typed-kernel tests or host asset replay alone.

Run the four parent CMake sanitizer targets through
`tests/actor_blended_playback_host.py --rebuild` to snapshot compiler inputs
before/after the host build. Later runs reject source/executable/DSO changes
against that snapshot. Reference binary formats are BPF1
native pre/post composition capture, BPG1 original post-state gold, BPS1
scheduler callback/service traces, BSS1 nested selection traces and BSC1 public
control/getter records. There are now four parent CMake audit targets.
