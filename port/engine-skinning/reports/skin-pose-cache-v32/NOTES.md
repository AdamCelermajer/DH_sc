# Actor pose and skinning reuse V32

This subsystem replaces temporary per-draw deformation storage with retained
CPU buffers and adds an explicit numeric-only animation sampler. It keeps the
existing source interpolation, palette, skin-point and transform arithmetic.
It never changes timeline, event, AI, movement, physics, targeting or bone-FX
cadence. There is no offscreen animation skip.

## Interfaces and lifetimes

`SkinPoseCacheV32` binds one immutable Skin and rest-position stream. Their
resource owner must outlive the cache. Rebind after resource relocation;
reset and rebind after any in-place stream mutation. Sampling compares the
actual world matrices of every referenced bone bitwise. Changed poses run the
same original kernels into reusable scratch, then commit. Failed sampling
preserves the previous valid positions. Cached output is read-only.

Each successful deformation receives a process-wide unique native cache
revision. This is render bookkeeping, not an original gameplay counter.
Consumers compare the uploaded revision, including when an intermediate
consumer sampled the same pose before upload. `changed_last_call` alone is
insufficient for multiple consumers. A shared GPU buffer must also track which
actor last uploaded it.

`VisualSkinOwnerV6::draw_views` and its equipment forwarding method return a
synchronous borrow. Positions remain valid until the next view call, selection
mutation or owner destruction. The separate retention lease pins geometry and
materials for topology tracking; it does not authorize deferred cached-position
access. Rigid weapon views reference immutable geometry positions directly;
their fresh bone-anchor world placement still updates each call. The existing
owned `draw_parts` API remains available and unchanged as a parity reference.

`Player::sample_reuse` uses a caller-owned PoseSampleWorkspaceV32, commits only
numeric node/instance fields after complete validation, and preserves failure
atomicity. It does not copy materials, strings, topology or bindings. Existing
`Player::sample` remains unchanged as an independent reference. Object
resources own this workspace and per-primitive skin caches. The original
explicit rigid-vertex fallback is applied to a local output copy, so it cannot
corrupt the software skin cache's zero-weight output.

## Verification

Strict O2 Android component compile: five translation units, both ARM64 and
x86_64, ten successful compilations, Werror, no fast math, FP contraction off.
See `android-compile.json`.

The standalone current-source sanitizer test compared all 172 actual modular
resources bit-for-bit with the existing source kernels; compared 128 authored
pose samples including seeks/endpoints; compared 32 whole object samples with
an explicitly declared unweighted-vertex fixture; checked direct bone mutation,
invalid graph/joint, failure recovery, and legacy owned-snapshot lifetime.
The O1 and O2 pre-weapon run passed 28,101 checks with ASan/UBSan and no findings.
Three hundred frozen draws produced 1,200 cache hits with no extra storage
growth. Changing poses had no growth after both scratch buffers were warmed.

The extended actual anchored-weapon borrow/lifetime run is recorded separately
in the latest host receipt. Receipts bind their exact source hashes; earlier
receipts are retained and are not relabeled as the extended test.

## Performance limits

The CPU benchmark uses twelve independent actual four-part player instances
over 240 continuously changing authored poses. This is representative source
work, not a claim about twelve different NPC models or device frame rate.
The initial non-sanitized O2 run measured legacy/cached skin time ratio 1.097,
and legacy/reused pose time ratio 1.032. Sanitizer ratios were higher and must
not be presented as production speedups. Avoiding unchanged deformation and
upload work is the stronger optimization. Root owns integrated GPU/UI timings
and emulator validation; no APK, install or emulator operation was performed
by this subagent.

## Renderer review

Root's equipment integration compares retained pose revisions. NPC caches are
per actor, cleared when restored into new render resources, and shared group
VBO uploads compare both actor owner and uploaded revision. Fallback uploads
clear the owner marker. Dynamic clip bounds use the current complete deformed
positions; culling defers GPU work while source animation/target bounds still
advance. Invalid bounds conservatively submit. Read-only review found no
actionable lifetime or stale-upload regression in these hunks.

Further SceneBinding dirty-hierarchy optimization was deliberately held after
measurement; current GPU/UI stage measurements determine the next bottleneck.
