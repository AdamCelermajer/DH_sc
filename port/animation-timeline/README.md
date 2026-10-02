# Reconstructed animation timeline

This bounded C++ component reproduces the original range-only timeline's
clock arithmetic. It supports signed ranges, seeking, forward/reverse speed,
looping, end clamping, and repeated or backward input timestamps. It does not
implement clip-library lookup, callback dispatch, animation events, transitions
or gameplay.

## Original behavior

The original `glitch::collada::CTimelineController` object is 64 bytes on ARM32.
Its current integer time is at +4, range endpoints at +0x10/+0x14, loop flag
at +0x18, delta magnitude at +0x1c, start/length/last-input/current seconds at
+0x20/+0x24/+0x28/+0x2c, scale at +0x30 and finished/started flags at +0x3c/+0x3d.
The new caller-owned `State` uses fixed-width fields and no original pointers.

The first update establishes the input-clock origin and contributes zero time.
Further updates add `(input_seconds - last_input_seconds) * scale`. Direction
depends on that delta, including when the input clock moves backward. Forward
playback crosses the end only when strictly greater than it; reverse playback
crosses the start only when strictly less. Looping uses signed `fmodf` with the
original operand order. Clamping sets the finished flag, which remains set until
a jump. A jump clears started/finished but preserves the last input and delta.

Inputs are restricted to +/-1,000,000,000 milliseconds, a positive duration and
finite scale of magnitude at most 64. Invalid states/inputs return an error
without changing output. These bounds are new defensive constraints. Original
zero-duration and invalid-state behavior is not reproduced.

## Validation

[The differential report](arm-differential-validation.json) records **400
sequences, 4,000 updates and 400 jumps** against the actual original ARM32
instructions, compiled ARM64 source and host source. All compared state words
matched, allowing only the sign of floating zero. The five original bodies are
setScale, setLoop, setRange (range-only mode), jumpTo and update with null
callbacks. Their exact addresses, sizes and hashes are in the report.

The pinned original engine SHA-256 is
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Imported arithmetic and `fmodf` use the recorded host C/libm dependency model;
this does not prove equivalence to every historical math-library implementation.

[The host build report](build-validation.json) records 5,000 sanitizer sequences
and 50,000 valid/corrupted-state updates, with unchanged output on rejection.
[The ARM64 build report](arm64-build-validation.json) checks ELF64/AArch64 and
16 KiB PT_LOAD alignment. These reports concern standalone module testing.
The source Android preview separately integrates the range clock.

## Reproduce

On Linux with a C++ compiler:

```sh
python3 port/animation-timeline/build.py
```

With an installed Android NDK:

```sh
python port/animation-timeline/build.py --arm64-only --ndk PATH_TO_NDK --report port/animation-timeline/arm64-build-validation.json
```

The differential test requires Unicorn and pyelftools, the pinned private
original engine and the existing engine-math/skin C arithmetic oracle:

```sh
python port/animation-timeline/tests/differential.py --original PATH_TO_ORIGINAL_ENGINE --host port/animation-timeline/build/timeline-host.so --arm64 port/animation-timeline/build/timeline-arm64.so --oracle PATH_TO_ORACLE_SO --report port/animation-timeline/arm-differential-validation.json
```

The source preview's Play/resume starts a new Android monotonic-clock epoch
and jumps to the current pose. Seek pauses and jumps. Uptime is kept outside
the bounded original clock. The on-screen time follows native playback. These
are diagnostic UI choices; they do not establish original animator state-machine
or full-game equivalence. No original engine binary is used by the source APK.
