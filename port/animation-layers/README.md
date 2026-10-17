# Checked absolute animation layers

This diagnostic source module combines up to eight absolute character poses
using the [original weight/vector mixing](../animation-mixing/README.md) and
[quaternion blending](../animation-values/README.md) calculations. It computes
node transforms and a software skin palette on a matching imported model.
It is integrated into the Android source preview, which exposes two layers.

This is authored scene integration around reconstructed calculations. It does
not reproduce the original animator's target compiler, default-relative pose
application, transition scheduler, synchronization controller, events or gameplay.

## Contract

Each layer borrows a validated `pose::Clip`, an independently supplied time,
and a finite nonnegative weight. All referenced BRES bytes must remain alive.
Weights are normalized in their original order; zero total selects the first
layer through the original fallback. Missing tracks use the imported model's
unchanged transform. Every track in every layer must resolve to exactly one
node, including layers with zero weight; missing/duplicate targets and joints
are rejected. This strict binding policy is a diagnostic choice.

Traversal is bounded to 64 levels, 20,000 nodes and 256 joints. Outputs remain
unchanged on rejection; palette outputs cannot overlap model/animation bytes,
clip storage or the layer descriptor. No original binary is used by this module.

## Evidence

[The host fixture check](validation.json) combines the warrior dual walk
(25 tracks, 799 ms) with Dark Queen scene 03a prince (29 tracks, 1,099 ms).
Five independent time phases and five weights produced 25 finite 18-joint
palettes. Ten endpoint comparisons matched the individual source-pose evaluator
within the stated numerical tolerance. Intermediate weights produced different
palettes; the zero-sum fallback and five unchanged-output rejections passed.
Model and animation bytes remained unchanged. This is source integration
validation, rather than an instruction comparison of the full original animator.

[The safety build](build-validation.json) records 3,000 second-clip
corruption/truncation probes under AddressSanitizer/UndefinedBehaviorSanitizer,
with layer count, weight and capacity failures. Rejections left the palette
unchanged, and writes remained within the requested joint count.

The exact source APK, 254,496 bytes, SHA-256
`2e9b30c1560aee54083dfe7ba77273bc9a610ad78a828bba16a0885027c9b71d`,
passed both fixture imports, midpoint seek, 0/50/100% mixing, advancing Play and
stable Pause on Android 17 x86_64 with 4 KiB and 16 KiB pages. Installed package
hashes matched. Visually inspected screenshots show three distinct textured
poses, with no in-run fatal error or rejected frame.
[Runtime evidence](../android-app/layers-runtime-validation.json).
Play was tested after selecting 100%; the other weights were checked while paused.
ARM64 is built with checked 16 KiB alignment but remains untested on hardware.

The preview maps the primary clip's normalized phase to the second clip's
duration, using bounded integer arithmetic, and exposes a manual mix slider.
These UI choices are not proof of the game's synchronized animator behavior.
Rendering still uses diagnostic normalization, one texture and one skin.

## Reproduce

```sh
python3 port/animation-layers/build.py --first PATH_TO_DUAL_WALK --second PATH_TO_CUTSCENE --model PATH_TO_WARRIOR_MODEL
python port/animation-layers/tests/check.py --library port/animation-layers/build/layers-host.so --first PATH_TO_DUAL_WALK --second PATH_TO_CUTSCENE --model PATH_TO_WARRIOR_MODEL --report port/animation-layers/validation.json
```

The host check needs the repository's existing ctypes readers. The Android
build compiles this module into both source libraries; see
[the app instructions](../android-app/README.md) for the repeated emulator test.
