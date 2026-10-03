# Raw compiled transform targets

`TransformSet` in `animation.hpp/.cpp` supplies raw per-target sampling for two
independent animator slots. It owns immutable clip bytes, key views and independent
event-track backing. It never advances a timeline, emits an event, writes a node,
resets a pose or computes a blended scene. Existing `Player.load/sample` code and
behavior remain unchanged.

The supported domain is full node position1, quaternion5 and scale10 with one
channel/sampler and uncompressed float keys. Component, material, multiple-sampler
and compression factories fail explicitly; existing Player finite-key/norm/time
validation is retained as a bounded caller contract. Clip DB/default bytes and
retained caller bytes are copied exactly, including signed zero, NaNs and infinity.

## Ordered compile and template policy

The caller supplies clip order explicitly. Within each resource, channels retain
serialized library order. Actual `CAnimationSet::addAnimation`6601fc and
`constructCompatibilityTable`670a60 prove that the restricted1/5/10 submatrix is
identity: exact URI/type deduplication preserves first insertion order. This does
not recover the original complete game bank-loading order or generic compatibility
rules for unsupported properties.

Default `TransformTemplatePolicy::authored` registers the supplied immutable
authored transformation graph and filters unregistered URI channels before union
insertion, following template `isAnimationExist`6e22cc called at6607ac/6607b4.
`setUnAdded`667320 and `addChannels`667384 prepare unused template records; the
captured latter routine never reads its incoming vector pointers or appends targets
to the union. This implementation therefore does not append every graph property.
Explicit `none` preserves the no-template path: unbound targets have nodeUINT32_MAX
and absent DB defaults have no authored fallback. No generic removal policy is
claimed; unsupported channels fail rather than being silently removed.

Each clip-major/target-minor binding is mode2 when its blendable animation exists,
mode1 otherwise. Original lookup61c1e0 and default61c6bc execute in the differential.
Node DB lookup61c290 calls getVisualScene(0)60e54c, then searches root nodes and
children in source order through61c214. Defaults copy SNode+0x0c position,
+0x18 quaternion, +0x28 scale via61c2f8; later visual scenes are not searched.

**Template fallback is mode1-only.** At660980 a present track branches directly to
mode2 handling660920. Only an absent track with null DB default reaches66099c,
checks the template6609a0 and calls its default virtual6609cc. Mode2/null-default
therefore does not copy authored template values. Animation record.optional_default
is a distinct accessor/factory field, not the compiled node DB default. Current
Prince transform tracks have optional defaults and no compression; many clips
have no serialized visual-scene DB, so their mode2 compiled DB defaults are null.

## Sampling and the recovered cursor boundary

`sample(clip,target,ms,out,capacity,cursor,error,interpolate=true)` serves both pose
slot storage and the shared root scratch. Original getAnimationValue65f7b4 copies
nonnull default bytes, then evaluates only mode2 through actual66a1a8. Mode1/null
retains both output and cursor. Sampling rejects invalid indices, insufficient or
misaligned output, and output/cursor overlap before any write; failed compilation
preserves the previous complete set. Output must be valid caller-owned storage and
must not alias immutable set backing.

Source accessor66b814/66b65c dispatches typed hint searches66a368(byte variant
66a894, ushort66b130). The previous key is clamped and the source probes one
backward/two forward neighbors before binary fallback. Neighbor intervals have an
inclusive upper boundary. Consequently a prior cursor can retain the preceding
key at an exact next-key time; this changes the pose when interpolation is disabled
and changes key state even when interpolated pose bytes agree. It is not merely a
performance hint. Retain the per-slot/per-target cursor across pose/root calls and
clip selections; a null cursor starts at zero for that call. Existing Player uses
its existing uncached selector and is intentionally unchanged.

The original AnimatorSet+0xc field controls the boolean passed to66a1a8 as
`field != 1` at65f890..65f89c. The raw API exposes that policy explicitly rather
than inferring it from target count. Defaulttrue is the port call default; the
coordinator must recover and supply the actual source field policy.

## Evidence and limits

`compiled_transforms_differential.py` executes original ordered union, template
filter, clip lookup/default, getAnimationValue, accessor/key interpreters and
quaternion math against O2 compiled ARM64 TransformSet. It uses genuine Prince
35-node authored model and five exact cache clips (Idle, Move, stationary/moving
Attack, Death), plus four explicit original-format fixtures for unregistered URI,
empty library, two contiguous segments with IEEE DB defaults and ushort times.

The extended-track factory identity, node ID virtual getter, selected immutable
clip and selected segment are synchronous oracle fixtures. They do not replace
key/default/pose arithmetic. Allocator/libc/soft-float/libm dependencies are modeled;
unknown executed imports fail. ARM64 runs actual native STL/resource/compiler code
with pointers above4GiB. It matches11,656 raw samples,29 authored/33 no-template
ordered targets,558 bindings,309 original union calls,157 filters,667 default
queries,3,000 retained samples and prior-cursor regression cases with zero mismatch.

The direct-source host ASan/UBSan replay verifies the same golden samples, eight
atomic rejections, compiled resource independence after all input Players die,
retained event-name leases after a set move, and zero sanitizer findings. Test-only
libm wrappers verify every original input/order and supply the audited UCRT output;
production math is unchanged. This avoids host glibc/UCRT rounding noise and makes
no historical Bionic claim. The existing unchanged Player audit also passes2,033
milliseconds and5,000 mutated images against the original candle_flame fixture.

Host CMake target `compiled_transforms_audit` directly compiles this test,
animation/events/event_track, scene, resources, payloads and math. It links no game
DSO and uses `--wrap=sinf/acosf/sqrtf`, `-fno-builtin`, `-fno-fast-math` and
`-ffp-contract=off`. Parent owns that CMake target. `compiled_transforms_host.py`
binds the prebuilt executable/source/CMake hashes and exact cache inputs to its
new host report. No prior reports, checkpoint APK, Android build or emulator were
changed for this task. This is raw target sampling; BlenderPlayback composition,
event reentry, GPU pose output and complete game parity require their own proof.
