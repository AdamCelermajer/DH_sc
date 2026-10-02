# Original animation movement delta

This standalone source module reconstructs the explicit-position overloads
of `AnimApplicator::ResetDelta` and `CalculateDelta`. These calculations are
building blocks for extracting animation movement. They do not sample a root
track, move a scene node, resolve collisions or implement gameplay.

Reset saves the supplied position and timestamp and clears delta. Calculate
subtracts the previous position when the timestamp changes. At the same
timestamp it produces zero delta. **Both paths save the supplied position**;
the next evaluation measures from that latest value, including values supplied
during a repeated timestamp. This is a positional difference, without division
by elapsed time or a units conversion. Timestamps may wrap or move backwards;
only equality controls this calculation.

The portable state is 28 bytes: timestamp, previous vector and delta vector.
The corresponding original object fields begin at offsets `0x14`, `0x18`
and `0x24`. Input positions must be finite and disjoint from state. A changing
timestamp requires a finite previous position and finite subtraction results.
Rejection leaves state unchanged. Reset can replace corrupt prior state, and
same-timestamp calculation can replace a nonfinite previous position without
using it; these conditions follow which values the original actually reads.
The finite/alias rejection policy is defensive source behavior.

## Evidence and reproduction

The differential test executes the exact original ARM32 explicit-position
methods and compares the projected state with compiled ARM64 and host source.
The recorded run passed 800 resets and 4,000 calculations across 400 sequences,
including 1,600 repeated-timestamp checks, with zero mismatches.
It checks repeated/backwards/wrapping timestamps, changing positions,
input preservation and guarded output boundaries. Original float subtraction
imports use the existing host C dependency model. It does not use node/track
overloads or stub out either original calculation.

The build also checks ARM64 16 KiB load alignment and 5,000 safety sequences
containing 100,000 valid/corrupted operations under AddressSanitizer and
UndefinedBehaviorSanitizer. Safety probes include NaNs/infinities, subtraction
overflow and state/input overlap. Adjacent reports pin the exact artifacts,
source and test files. This module has no Android app integration or phone run.

```sh
python3 port/animation-motion/build.py
python port/animation-motion/build.py --arm64-only --ndk PATH_TO_NDK --report port/animation-motion/arm64-build-validation.json
python3 port/animation-motion/tests/differential.py --original ORIGINAL_ENGINE --host port/animation-motion/build/motion-host.so --arm64 port/animation-motion/build/motion-arm64.so --oracle port/skin-payloads/build/oracle.so --report port/animation-motion/arm-differential-validation.json
```

The runner uses the existing `unicorn` and `pyelftools` test dependencies.
No original game binary is linked into the reconstructed library.
