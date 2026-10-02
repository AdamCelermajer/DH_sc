# Original animator transition state

This source component projects the weight/timestamp state in the game's
`AnimatorBlender::Blend` and `updateTime`. It is a standalone building block
for character actions. It does not implement the full animator, its children,
target compilation, pose application, event callbacks or gameplay.

## Preserved behavior

- `Blend` rotates current/previous child indices, starts with the **previously
  requested duration**, and then saves the new nonnegative duration for next time.
  It leaves weights and the last timestamp unchanged. A nonpositive previous
  duration leaves the stored reciprocal unchanged.
- `updateTime` subtracts the timestamp difference using original 32-bit wrap
  semantics. Positive remaining time gives `float(remaining) * reciprocal`
  to the previous child and `1 - previous_weight` to the current child.
  Zero/negative completion assigns zero/one, in that order. A negative remainder
  on subsequent frames skips fade recalculation.
- Nonzero children are selected in ascending order **before** normalization.
  The original normalization can make the first weight one after no child
  was selected in an all-zero frame. Selection and final weights are distinct.
- The source API returns those active bits and updates the timestamp. Calling
  child timelines and `AnimApplicator::CheckCallback` remains caller work.

The bounded projection contains eight weights and 28 bytes of state. Update
supports one through eight children. Begin accepts exactly two, which follows
the original `Blend` path without its debug/assertion branches. Initialization
is authored scaffolding: zero state with the first weight one. It is not a
reconstruction of object construction or allocation.

Finite weights/reciprocal and valid indices are required. Rejection preserves
state and active-bit output; that output cannot overlap state. Bit conversion
avoids C++ signed overflow when the original timestamp arithmetic wraps.

## Validation

The differential runner executes original ARM32 `Blend`, `updateTime` and
normalization against compiled ARM64 and host source state. Child update/getter
and `CheckCallback` are **explicit non-mutating stubs** that record call order.
This validates state and which children would be called, without testing their
timeline bodies, callback implementation or effects. Arithmetic/imports use the
existing host C dependency model. This does not claim full animator equivalence.

[The differential run](arm-differential-validation.json) passed 400 sequences,
300 transition requests and 4,800 updates/dispatch-order checks, with zero
mismatches. The build also checks ARM64 16 KiB load alignment and 5,000 safety sequences
with 100,000 valid/corrupted operations under AddressSanitizer and
UndefinedBehaviorSanitizer. See the adjacent build and differential reports
for exact binary/source identities and observed comparison totals.

```sh
python3 port/animation-transition/build.py
python port/animation-transition/build.py --arm64-only --ndk PATH_TO_NDK --report port/animation-transition/arm64-build-validation.json
python3 port/animation-transition/tests/differential.py --original ORIGINAL_ENGINE --host port/animation-transition/build/transition-host.so --arm64 port/animation-transition/build/transition-arm64.so --oracle port/skin-payloads/build/oracle.so --report port/animation-transition/arm-differential-validation.json
```

The runner requires the existing `unicorn` and `pyelftools` test dependencies.
No original engine binary is linked into the reconstructed libraries. This
module has not yet been integrated into an Android app or tested on a phone.
