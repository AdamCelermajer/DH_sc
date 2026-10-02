# Reconstructed float scene animation values

Checked source calculations for stored translation, quaternion rotation and
scale keys, plus values relative to a reference key and weighted quaternion
blending. These reproduce isolated original calculations. They do not implement
the animator state machine, scene application, transitions, streaming or gameplay.

## Original behavior

The float value APIs accept one-channel, one-sampler borrowed animation views
of type 1 (position), 5 (quaternion) or 10 (scale). Values must be float data
with three or four components. Offset/scaled and compressed tracks return an
unsupported error. Keep the original BRES bytes alive. Outputs contain four
floats (the fourth position/scale component is zero) and must be disjoint from
the input BRES.

| API | Calculation |
| --- | --- |
| `dh2_animation_float_key` | Copy a selected stored key |
| `dh2_animation_float_interpolate` | Original consecutive-key absolute interpolation |
| `dh2_animation_float_delta` | Change from a selected reference key, with optional interpolation |
| `dh2_animation_quaternion_blend` | Original ordered, weighted quaternion blending, at most 256 values |

Absolute position and scale use `(1 - t) * first + t * second`. Their relative
forms use `first + t * (second - first) - reference`, preserving binary32
arithmetic order. Quaternion absolute interpolation passes the two weights to
the original ordered blender. Relative rotation uses
`conjugate(reference) * slerp(first, second, t)`. This is a difference between
keys, not evidence of the complete game's application of default transforms.

The quaternion blender skips zero weights, copies the first nonzero value,
returns immediately if that first weight is exactly one, and otherwise blends
later nonzero values using `weight / accumulated_weight`. An empty or all-zero
list returns identity. The port preserves these rules. It checks finite inputs
and outputs, ranges, output aliases and count limits before exposing results.
Errors leave the output unchanged.

## Instruction evidence and checks

[The three-way comparison](arm-differential-validation.json) executes 13 original
ARM32 key/interpolation/delta/blend bodies, compiled ARM64 instructions and host
C++ on the same inputs. All 1,685 cases matched float bits except signed zero;
stack restoration, output guards and unchanged port input bytes passed. Inputs
include the supplied warrior walk, synthetic float position/rotation/scale
tracks, endpoint fractions, and quaternion lists of 0, 1, 2, 3 and 8 values
with zero and nonzero weights. The report pins ELF addresses, symbols and
instruction hashes. It records 592 distinct instruction addresses exercised
in the selected bodies; it does not claim exhaustive instruction coverage.

The original ELF SHA-256 is
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The existing recovered assembly directory contains the original body listings.
Ghidra addresses add `0x10000` to ELF addresses. Imported compiler arithmetic,
libm and libc helpers use the existing host C dependency model. This does not
prove historical Android libm behavior or execute the game.

The [host build and safety record](build-validation.json) includes 3,000 actual
buffer truncation/corruption cases with AddressSanitizer and
UndefinedBehaviorSanitizer, plus invalid-key, alias and nonfinite-weight checks.
The [ARM64 build record](arm64-build-validation.json) verifies 16 KiB alignment.

From the repository root:

```sh
python3 port/animation-values/build.py --sanitize /private/walk.bdae
python3 port/animation-values/build.py --ndk /path/to/android-ndk
python3 port/animation-values/tests/differential.py \
  --original /private/libDungeonHunter2.so \
  --arm64 port/animation-values/build/values-arm64.so \
  --host port/animation-values/build/values-host.so \
  --oracle /path/to/libfp_oracle.so --sample /private/walk.bdae \
  --report port/animation-values/arm-differential-validation.json
```

The comparison needs Python packages `unicorn` and `pyelftools`. Build its
dependency oracle from `port/engine-math/tests/fp_oracle.c` with a host C compiler,
`-shared -fPIC -O2 -fno-fast-math -ffp-contract=off -lm`.
Windows ARM64 compilation uses `--arm64-only --ndk PATH`, alongside a Linux host
build for the comparison. No original ELF or cache is packaged by this component.

## Android source integration

The [pose evaluator](../animation-pose/README.md) now calls these verified
absolute calculations after key search. The [source Android app](../android-app/README.md)
contains the component for both ARM64 and x86_64. The exact updated APK passed
walk import, midpoint time selection, Play and Pause on Android 17 x86_64
with both 4 KiB and 16 KiB pages. Native ARM64 execution on physical Android
hardware remains unverified. Complete source-built gameplay is unfinished.
