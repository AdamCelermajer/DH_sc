# Original animation mixing calculations

This bounded source component implements original weight normalization,
position/scale weighted sums, scalar weighted sums and ordered quaternion
addition. These calculations are needed for animation blending, but this
module does not implement the animator state machine, transitions, events,
scene targets or gameplay. Its weight normalization and vector mixing are now used by the source Android
[absolute-layer preview](../animation-layers/README.md). The additive quaternion
API remains a standalone tested component; Android runtime checks do not cover
that operation.

## Preserved behavior

| API | Original calculation |
| --- | --- |
| `dh2_animation_weights_normalize` | Sum weights in order, then divide each by the sum. For zero sum, set only the first weight to one and preserve the others. Empty lists do nothing. |
| `dh2_animation_vector_mix` | Position/scale blend and addition share a weighted sum. A single input is copied with its weight ignored; an empty list returns a zero vector. |
| `dh2_animation_scalar_mix` | Blend and addition share an ordered weighted sum, including the single-input case. Empty lists return zero. |
| `dh2_animation_quaternion_add` | Starting at identity, multiply each identity-to-input slerp in order. Negative weights conjugate the input and use positive magnitude; zero weights are skipped. |

Arrays contain at most 256 values. The caller owns all storage. Mixing outputs
must be disjoint from both input arrays. Inputs and results must be finite;
invalid counts, aliases, null inputs or arithmetic overflow are rejected with
unchanged output. Normalization is explicitly in place but also leaves the
weights unchanged on rejection. These are defensive port constraints, rather
than reproductions of unsafe original input behavior.

The existing [animation-values module](../animation-values/README.md) separately
implements ordered quaternion blending. Quaternion addition is a different
original operation; it does not use the accumulated-weight rule of blending.

## Checks and provenance

[The differential report](arm-differential-validation.json) records **842
three-way cases** across eight original bodies: weight normalization, position
blend/addition, scale blend/addition, scalar blend/addition and quaternion
addition. Actual original ARM32, compiled ARM64 and host outputs matched float
bits, allowing only signed zero. Tests include empty/single/multiple input
lists, zero weights, cancelling sums and positive/negative weights. Vector,
scalar and normalization cases reach the 256-value bound; quaternion instruction
comparisons reach eight values. Input preservation, output guards and stack
restoration passed. The report pins all original addresses, sizes and hashes.

The original engine SHA-256 is
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Original quaternion arithmetic also executes; imported arithmetic/libm use the
recorded host C model, which does not prove equivalence to every historical
Android math library.

[The host safety report](build-validation.json) records 6,000 bounded lists and
24,000 API calls under AddressSanitizer/UndefinedBehaviorSanitizer, including
nonfinite inputs, oversized counts, aliases and overflow. Error paths preserve
outputs and weights. [The ARM64 build report](arm64-build-validation.json)
checks ELF64/AArch64 and 16 KiB PT_LOAD alignment. No physical ARM64 execution
or Android runtime result is claimed by these standalone module reports.

## Reproduce

```sh
python3 port/animation-mixing/build.py
python port/animation-mixing/build.py --arm64-only --ndk PATH_TO_NDK --report port/animation-mixing/arm64-build-validation.json
python port/animation-mixing/tests/differential.py --original PATH_TO_ORIGINAL_ENGINE --host port/animation-mixing/build/mixing-host.so --arm64 port/animation-mixing/build/mixing-arm64.so --oracle PATH_TO_ORACLE_SO --report port/animation-mixing/arm-differential-validation.json
```

The original comparison requires Unicorn, pyelftools and the C arithmetic
oracle described by [engine-math](../engine-math/README.md). The private original
engine is used only by the instruction comparison; no original binary is built
into this component.
