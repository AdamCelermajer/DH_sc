# Original animation completion dispatch

This source component reconstructs `AnimApplicator::SetCallback` and
`CheckCallback`, the field registration and pending-notification boundary.
It does not determine when an animation finishes or implement the original
character callbacks, attack effects, triggered events or gameplay.

Set saves context and callback without changing pending status. Check calls
the callback with the supplied timeline handle and stored context only when
pending is nonzero and a callback exists. It clears pending **after** that
callback returns. With no callback, pending remains set so later registration
can receive it. A callback that changes registration or context keeps those
changes; a callback that changes pending still gets the final clear.

The source uses native function/context pointers and a `uintptr_t` timeline
handle. Portable pending is an integer constrained to the original byte range
0–255. Out-of-range check rejects without mutation. All callback/context/state
storage is borrowed: the caller must keep it alive, serialize access, and use a
callback that returns normally. Field changes during dispatch are supported;
this does not establish arbitrary recursive dispatch or callback destruction.
The API does not own objects or validate arbitrary caller-provided pointers.

## Validation and reproduction

The differential test executes both original ARM32 bodies against compiled
ARM64 and host source. Callback bodies are controlled test effects, not the
game's callbacks. It verifies argument passing, conditional dispatch, deferred
registration, clearing after callback, field changes during callback and no
second notification after clear. Pointer presence, context and pending fields
are projected across the different object/ABI layouts. Wide timeline cases
give original ARM32 the low 32 bits and ARM64/host the full handle, checking
that the source does not truncate it.

The recorded run passed 384 setter calls, 768 checks and 144 controlled
dispatches, with zero mismatches. See [the comparison record](arm-differential-validation.json).

The builds check ARM64 16 KiB load alignment and 10,000 safety checks under
AddressSanitizer and UndefinedBehaviorSanitizer. Adjacent reports pin exact
source, callback test, function bytes and binaries. No Android integration or
physical-device run is included. No original binary is linked into this source
library. The test runner uses the existing `unicorn`/`pyelftools` environment.

```sh
python3 port/animation-completion/build.py
python port/animation-completion/build.py --arm64-only --ndk PATH_TO_NDK --report port/animation-completion/arm64-build-validation.json
python3 port/animation-completion/tests/differential.py --original ORIGINAL_ENGINE --host port/animation-completion/build/completion-host.so --arm64 port/animation-completion/build/completion-arm64.so --oracle port/skin-payloads/build/oracle.so --report port/animation-completion/arm-differential-validation.json
```
