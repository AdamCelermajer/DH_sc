# Reconstructed random streams

Portable C implements the original `Random::GetRandom(int,bool)` and the
supported numeric/tagged `GameObject::_Rand` projection. State is owned by the
caller; this library has no original engine, translator or Android dependency.

The update wraps 32-bit multiplication/addition before reduction:

```
seed = ((seed * 0xe6ab + 0x2b3fd) mod 2^32) mod 0xdaf26b
result = seed mod unsigned_bound
```

Zero bounds return zero and leave the seed unchanged while incrementing the
selected debug counter. Counters wrap modulo 2^32. Any nonzero sync selector
chooses the second stream. `dh2_random_next` requires a valid state pointer.

`dh2_random_callback` checks nulls, input/output aliases and its authored
32-argument limit before mutation. One numeric argument specifies the exclusive
upper bound. Two numeric arguments specify a lower bound and the wrapped
unsigned difference to the upper bound. Other counts use `[0,100)` and ignore
argument types. Invalid types at counts one/two return no values. Finite
negative operands clamp to zero; nonfinite/out-of-range conversions reject
atomically. Online fixtures replace the ordinary seed from the object and write
it back after the draw, matching the original supported path.

## Evidence

- 768 sequential original ARM32 / compiled source ARM64 / host core comparisons.
- 351 original callback comparisons, including offline/online fixtures, missing
  and extra arguments, type rejection, reversed/equal bounds and counter wrap.
- 35 original state/hit-count/name fixture cases; these record projections only.
- 147 host Lua checks compare original offline callback results and representable
  state/hit/name outputs after the original float32 return conversion.
- 12,000 ASan/UBSan/float-cast safety iterations; ARM64 load alignment is 16 KiB.

The test executes actual random, argument-indexing and numeric getter bodies.
ELF globals are relocated. Imported unsigned quotient/remainder uses a stated
mathematical model. GetOnline and ReturnValues pushes are fixture boundaries.
Original online networking, the original Lua VM and full Character ownership
are not executed. Source ARM64 comparisons run in Unicorn; hardware is untested.

Build with `python3 build.py --host --report host-build-validation.json` under
Linux, or `python build.py --ndk <Windows-NDK> --report android-build-validation.json`.
The differential tool requires the owner original library, the existing oracle,
both compiled libraries, and optionally the owned host Lua runtime. Its exact
hashes, original instruction ranges and dependency scope are in
`differential-validation.json`.

The Lua binding supplies per-runtime offline `Rand`. `DH2SeedRandom` is an
authored test/setup control; it resets both counters. Original game startup seed
selection and online actor ownership are unfinished. This is a component of the
source reconstruction, not a complete game.
