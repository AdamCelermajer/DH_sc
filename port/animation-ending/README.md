# Original end-of-animation timing

This standalone component reconstructs the extra-time field calculation in
`CharAnimator::CalculateExtraTime` and the extra/pending fields in
`AnimatorBlender::_HandleAnimEnding` and `Animator::_HandleAnimEnding`.
It connects timeline snapshots to an
ending notice; it does not implement the subsequent game action or animator
object lookup, own timelines, dispatch callbacks, or provide gameplay.

The original converts `delta_seconds * 1000` and `current_seconds * 1000`
to signed integers by truncation. It subtracts the stored current milliseconds
using 32-bit wrap. When that difference is nonnegative and strictly below the
converted frame delta, extra time is `delta - difference`; otherwise it is zero.
The two original bodies share this calculation. A missing timeline retains
the character's prior extra-time field.

The ordinary `Animator` ending handler has no active-child filter. Its checked
projection passes the sender as both active and sender to the same source API.

The ending notice compares the sender with the active timeline. A different
sender changes nothing. A matching nonzero handle computes extra time and marks
pending. Matching null handles mark pending while retaining previous extra time.
Active timeline lookup remains caller work; the source accepts its handle and
a minimal snapshot. The caller can derive a snapshot from `timeline::State`'s
`current_ms`, `delta_magnitude` and `current_seconds` fields.

Snapshots and output must be disjoint. Nonfinite products or undefined float
to integer conversions are rejected without changing output. Original integer
wrap uses explicit unsigned arithmetic and bit conversion to avoid C++ signed
overflow. These checks are defensive source behavior, without executing original
undefined casts as a claimed reference case.

## Evidence and reproduction

The differential runner executes all three exact original ARM32 bodies and compares
integer state with compiled ARM64 and host source. It covers signed/zero frame
deltas, truncation and strict boundaries, null timelines, matching/different
senders, large timestamps and input preservation. The original active timeline
getter is a controlled pointer-return stub. It does not validate child ownership,
lookup implementation or actual character callback effects. Arithmetic/casts
use the existing bounded host C dependency model.

[The recorded run](arm-differential-validation.json) passed 495 extra-time
calculations, 1,980 filtered blender notices and 495 ordinary animator notices,
with zero mismatches.

Safety checks exercise 10,000 arbitrary sample pairs and 20,000 calculation/
notification calls under AddressSanitizer and UndefinedBehaviorSanitizer,
including guarded casts, aliases and unchanged rejection state. ARM64 libraries
have checked 16 KiB load alignment. Adjacent reports identify exact source,
test, binary and original function bytes. This module is not integrated into an
Android APK and has no phone test. No original binary is linked into its library.

```sh
python3 port/animation-ending/build.py
python port/animation-ending/build.py --arm64-only --ndk PATH_TO_NDK --report port/animation-ending/arm64-build-validation.json
python3 port/animation-ending/tests/differential.py --original ORIGINAL_ENGINE --host port/animation-ending/build/ending-host.so --arm64 port/animation-ending/build/ending-arm64.so --oracle port/skin-payloads/build/oracle.so --report port/animation-ending/arm-differential-validation.json
```

The runner uses the existing `unicorn` and `pyelftools` dependencies.
