# Absolute character animation preview

A bounded source evaluator joins checked animation keys, node transforms and
software skin palettes. This is a diagnostic preview of stored absolute keys.
It does not reconstruct the original default-relative animator, blending,
transitions, root motion, gameplay state or streaming.

## Supported input and API

One borrowed BRES animation segment contains at most 128 tracks. Supported
tracks have one channel and sampler, no value scales/offsets, and float
translation (type 1, three components), quaternion (type 5, four components)
or scale (type 10, three components). Float scalar position components
(types 2/3/4) and angle rotation (type 9) use checked serialized defaults.
One position and one rotation track per node are supported; conflicting
component/full-vector tracks are rejected. Compressed values and other types return
an error. Strictly increasing signed times and finite values are checked
before a clip is exposed. Negative clip starts are supported; total duration
must fit a signed 32-bit millisecond value. A clip and its backing bytes must stay alive together.

`dh2_pose_sample` uses the reconstructed key search and
[original-instruction-checked absolute value calculations](../animation-values/README.md).
These preserve the original weighted interpolation and quaternion blender
rules; full animator equivalence remains unverified. `dh2_pose_node` copies a
node and replaces the fields addressed by matching track target IDs.
`dh2_pose_skin_palette` traverses one visual, applies posed world transforms,
and resolves skin joints by their scope IDs. Every animation target must match
exactly one node. Duplicate joints, cycles, absent targets, excessive depth,
insufficient output capacity and nonfinite transforms return errors. Input
bytes remain unchanged. The bounded traversal permits 64 levels and 20,000
nodes; a skin permits at most 256 joints.

The field/type mapping is supported by original
`CColladaDatabase::getDefaultValue`, ELF `0x0061c2f8` (Ghidra `0x0062c2f8`),
and the scene-node interpreter listings in
[`recovered/native/assembly`](../../recovered/native/assembly/libDungeonHunter2.so).
Original quaternion interpreters also calculate default-relative values;
this preview intentionally does not claim their complete semantics.

## Reproduction

From the repository root, with a C++17 compiler and Python 3:

```sh
python3 port/animation-pose/build.py
python3 port/animation-pose/tests/check.py \
  --library port/animation-pose/build/pose-host.so \
  --model /private/cache/data/3d/characters/prince/prince_low_poly_warrior.bdae \
  --animation /private/cache/data/3d/characters/prince/animations/prince_walk_1hand.bdae \
  --report port/animation-pose/validation.json
```

The [build record](build-validation.json) pins the component sources. The
[sampling check](validation.json) covers the warrior's 27 walk tracks,
27 selected exact keys, 249 interpolated values and five animated palettes
of 18 joints. Input hashes and unchanged input buffers are recorded. The
reconstructed time accessor can round a frame time to 99 ms while the search
uses a 100 ms boundary; the checks follow the search result instead of assuming
that every rounded accessor time is an exact key boundary.

[Safety checks](safety-validation.json) passed 6,000 corruption/truncation
cases with AddressSanitizer and UndefinedBehaviorSanitizer, including sampling
and skeleton traversal. Compile `tests/safety.cpp`, `pose.cpp`, `../animation-values/values.cpp`, and the same
supporting sources listed in `build.py` with
`-fsanitize=address,undefined -fno-omit-frame-pointer`, then run the executable
with animation and model paths in that order.

## Android preview

The [source Android app](../android-app/README.md) can import the warrior,
its texture, and `prince_walk_1hand.bdae`. Play, Pause and a time slider show
changing poses. [Runtime evidence](../android-app/animation-runtime-validation.json)
records the exact installed APK on Android 17 x86_64 with both 4 KiB and
16 KiB pages. ARM64 libraries are built and checked for alignment, but ARM64
hardware execution remains unverified. The preview normalizes its bounds per
frame and uses one imported texture; it is not original game rendering.

## Player animation corpus

The [character audit](character-corpus-validation.json) checks 342 private
player animation files against the warrior model. It produces finite palettes
and sampled skinned positions for 308 clips, including 302 with positive
duration: 987 palette checks and 2,961 position checks. Inputs remain unchanged.
Two remaining clips contain unsupported property tracks, and seven need
model bindings that this warrior does not provide.
This is selected-time source validation, not original animator equivalence.

```sh
python3 port/animation-pose/tests/audit_character.py \
  --library port/animation-pose/build/pose-host.so \
  --model /private/cache/data/3d/characters/prince/prince_low_poly_warrior.bdae \
  --animations /private/cache/data/3d/characters/prince/animations \
  --report port/animation-pose/character-corpus-validation.json
```

The guarding-aura clip runs from -333 to 466 ms. The source APK imports its
25 tracks, seeks and plays on Android 17 with 16 KiB pages, with distinct
textured poses and no fatal runtime error. [Evidence](../android-app/negative-time-runtime-validation.json)
pins the APK, fixture hashes and screenshots. This adds a pose preview;
spell particles and effects are unfinished.

The latest scalar-track APK was checked with `prince_walk_dual.bdae` (25 tracks,
799 ms) on Android 17 with 4 KiB pages and `cs_darkqueen_scene03a_prince.bdae`
(29 tracks, 1,099 ms) with 16 KiB pages. Both show changing textured poses and
pass import, midpoint seek and Play/Pause with exact installed APK hashes.
[Runtime evidence](../android-app/scalar-track-runtime-validation.json) belongs
to this newer build; the walk and negative-time runtime records above preserve
their earlier build identities.
