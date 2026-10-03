# Actor playback composition audit

The parent-owned `actor_playback.hpp/.cpp` coordinator was inspected without
production changes. The audit joins the existing source-derived authored
scheduler, timeline, completion notification, replay and root-motion kernels.
It passed 10,584 phase-state comparisons and 5,628 independently sampled root
positions on the bundled 35-node Prince graph, under ASan and UBSan.

## Original instruction evidence

Original desktop oracle: `.local-inputs/libDungeonHunter2.so`, SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The composition corpus executes original CTimelineController Update `0x667104`,
clip `0x666f7c`, jump `0x666f10`, scale `0x666c20`, notification `0x366224`,
BlendedPlayClip `0x47680c`, NewAnim `0x35d624`, and RNG `0x3ca708` instructions.
Actual GetLoop/SetLoop/Jump/Scale and synchronous timeline reentry execute during
replay. Float arithmetic/AEABI imports, resolved authored clip bounds and the
existing animator binding/selection services use the established desktop oracle
dependency model. Root EnableDisplacement and ResetDelta calls are observed;
root onAnimate invokes the original timeline update. A replay can itself notify
again if a very large captured extra still crosses the next clip's end.

`composition-fixtures.bin` SHA256:
`979481e5ed42302507f757aaae47db3f3627a3feabfd4291d5c2b8be4d24735c`.
There are 5,292 records in fourteen stateful traces. Normal scene updates produce
130 completion callbacks. There are 152 PlayClip calls including initial starts,
normal repeats and eight state switches. In 128 observations the captured extra
differs from CalculateExtraTime executed after currentMS finalization; these
distinctions make the completion producer timing observable.

Corpus layout is little endian: magic `APG1`, uint32 record count, then 108-byte
records. Input is trace/op/argument/sequence (four uint32), global factor float32,
mapped dictionary clip int32. Expected output is the 56-byte timeline State,
8-byte Completion and five uint32 values: root timestamp, restart count,
completion count, RNG seed and RNG call count. Operations 0/1/2/3 are start,
scene phase, animator phase and explicit boundary-fixture jump. The initial
record of each trace resets the scene, binding, RNG and playback state.

## Real authored fixture

The audit loads the current asset `animations_pyarray`, dictionary and schema
files, all nine reachable Android locomotion clips, and
`models/prince_modular.bdae`. It checks original Knight animation-table row48
maps Idle262, Walk280 and Run271, and selects root_camera node33 of35. The bank
is derived from the supplied Android ELF's stance mask `0xd2`: five authored
Idle and Walk variants, un-stanced Run. Default-stance random Idle selection
actually visits dictionary clips1040 and1041; Walk uses1126 and Run1114. All
nine loaded clips have bound, supported tracks; four default clips participate
in these stateful composition traces. Other stance clips already have separate
every-sample visual-motion asset evidence.

Each state is tested at global factors1.0 and1.3. Idle authored Speed is1.0;
Walk/Run authored Speed is float32 1.3. The latter yields float32 timeline scales
1.3 and1.6899998188018799. The fixture rounds both input factors to float32 before
forming the product. Raw decoded clip translation at each **original expected**
currentMS supplies the independent root oracle; the audit does not estimate a
constant walking speed or infer time from a modulo clock.

Every trace runs normal scene phase, then a separate animator phase. Repeated
scene timestamps suppress displacement. Completion callbacks clamp currentMS
before that phase's root sampling. Animator replay then consumes the extra
captured by the earlier notification, samples clip start as its reset baseline,
and synchronously samples the selected currentMS at the prior scene timestamp.
Delayed animator fixtures preserve the first captured notification while the
timeline remains clamped. Explicit jump fixtures check equality at end emits no
completion and a subsequent positive advance crosses strictly beyond end.

The two runs use heading0 and heading pi/2. The independent quarter-turn root
oracle maps `(dx,dy)` to `(-dy,dx)`, confirming authored -Y maps to game +X.
Root Z remains zero, helper compensation is negative raw animated XYZ, graph
matrices remain finite, and every instance uses its corresponding node matrix.
Root sums allow0.03 game units absolute tolerance for quaternion rounding over
long accumulated traces; timeline, callback, counters and RNG words are exact.

## Producer and integration boundaries

`animator_phase(..., extra_ms)` must receive `completion.extra_ms` captured by
the timeline's notification callback. Original AnimatorSet/AnimatorBlender
notification stores applicator extra before timeline Update finalizes currentMS.
CharAnimator's callback subsequently sets its pending flag. CharAnimator+44 is
a separate PlayClip argument producer: the recovered `_SetAnimStep` loads it,
but BlendedPlayClip does not consume that argument. No live caller of the
separate CharAnimator CalculateExtraTime routine was recovered. Recomputing
extra using the finalized timeline in this bridge would change source behavior.

The composition matches the verified components in these fixtures. Full
Character FSM, original input/stance selection, blend weights/refcounts, FX,
audio, animation event consumers and the actual applicator callback ownership
are still integration boundaries. Global dictionary IDs replace controller
local clip IDs in this single-bank adapter; comparisons preserve a consistent
identity but do not reproduce the original binding registry. The audit does
not step physics, validate live floor placement, invoke UpdateSubObjects, or
verify rendering/camera frame scheduling. Those parent-owned stages have their
own tests; this audit's phase separation permits their insertion between scene
and animator processing. Negative/NaN speed, inactive phase and missing bank
are checked as adapter rejection behavior, with no claim of failure atomicity.

## Reproduction

Host target: `actor_playback_audit`, linked to `dh2_level_world`, compiled with
`-fno-fast-math -ffp-contract=off` and ASan/UBSan. Arguments:

```text
actor_playback_audit <assets-root> <reference/actor-playback/composition-fixtures.bin>
```

The verified manual host executable is
`.local-inputs/actor-playback-discovery/host-audit`; its stdout is JSON. Original
corpus generation is the local scratch
`.local-inputs/actor-playback-discovery/generate.py`, importing the existing
timeline/replay desktop oracle classes. Generation executes original ARM32
instructions only; no native production method generates expected state.
The original selection/completion dispatch is an explicit table fixture, with
actual original RNG and the previously verified one-step/random-sequence
completion policy. The asset root used was the repository Android project's
current `app/src/main/assets` directory.
