# `CharAI::IsMyTurn` bounded decision

## Recovered source decision

`character_ai_turn.cpp` reconstructs only the nonempty-queue decision in
`CharAI::IsMyTurn(CharAI const*)`. The checked original is the local
ARM32 `libDungeonHunter2.so` with SHA-256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The exact ELF symbols, sizes, byte hashes, global identities, and Character
vtable-slot evidence are recorded in
[`original-functions.json`](original-functions.json).

The actual routine at `0x3cc484` (`288` bytes) first subtracts the source
deque's end and begin iterators by calling the actual
`std::priv::_Deque_iterator_base<CharAI*>::_M_subtract` at `0x3cb49c` (`68`
bytes). The ELF symbols identify `CharAI::s_updateQueue` at `0x9a2b48` and
`CharAI::s_updateTimer` at `0x999760`; their contents are replaced by test
fixtures, not claimed as original startup values. The source deque subtract
uses a 32-pointer block stride (`lsl #5` after dividing the map-node pointer
difference by four), which the test builds as actual begin/end iterators and
checks against the routine's returned queue length.
[`original-functions.asm`](original-functions.asm) preserves the relevant
original instruction ranges for both routines.

For a nonempty queue, the routine reads the front `CharAI*` only when the
signed update timer is less than or equal to zero. A matching front returns
one immediately. Every other case evaluates the owner in this order:

1. Direct `Character::IsFollower()` at `0x3a307c`.
2. Direct `Character::IsFaerie()` at `0x3a3094` if the first query is false.
3. Virtual `Character::IsPlayer()` if both direct queries are false.

The final source branch returns the virtual call's raw `r0` value. The vtable
is not inferred from a presumed class layout: ELF symbol `_ZTV9Character` is
at `0x965f30`; its Itanium ABI address point is `0x965f38`; the vptr slot at
`+0x28` is file address `0x965f60`, whose relocation is `R_ARM_RELATIVE` and
whose raw target is the exact `_ZNK9Character8IsPlayerEv` symbol at
`0x3a49f0`. The oracle sets Character objects to this source address point
and intercepts each exact function entry to supply the fixture's concrete
query result. It does not execute the Character query bodies.

## Differential and API checks

[`run_character_ai_turn_host.py`](../../tests/run_character_ai_turn_host.py)
executes the original `IsMyTurn` and real deque subtract under Unicorn in
ARM32 mode. Twelve fixtures cover the due front fast path, positive and
negative signed timers, due non-front fallback, 32-entry deque block
boundaries, follower/faerie short-circuits, raw player return values, and
owner replacement between callbacks. For each case, the host result must
match the original ARM result, ordered query/owner trace, owner mutations,
front read, and the queue length returned by the original subtract.

The C++ API guard suite also checks null and misaligned inputs, aliased views,
zero AI/owner identities, missing services, callback errors with retained
synchronous mutations, a due front match without an owner service, captured
service-table behavior, and explicit empty-queue rejection. Empty queue is
not run through the original function: its assert-level logging and deliberate
null write at assert level two are outside this adapter's contract. Its
bounded result is `Status::empty_queue_unsupported`; no source queue/timer
producer or empty-queue behavior is fabricated.

## Limits

This is a local decision kernel, not a complete AI scheduler. It does not
construct or populate the update queue, produce/update its timer, execute
`Character::IsFollower` / `IsFaerie` / `IsPlayer`, or model source object
lifetime. The caller supplies a logical queue length, front identity, signed
timer, and synchronous callbacks. It does not overlay the source 32-bit deque
on the host's `std::deque`, and it does not claim full `CharAI::Update` or
frame-order parity.

## Reproduction

From the repository root, with the original ELF and Python packages
`pyelftools` and `unicorn` available:

```powershell
python port/level-world/tests/run_character_ai_turn_host.py `
  --original-elf ..\standalone-build\compatibility\work\original\lib\armeabi-v7a\libDungeonHunter2.so `
  --output port\level-world\build\character-ai-turn-host\character_ai_turn.exe `
  --report port\level-world\build\character-ai-turn-host\validation.json
```

The report keeps the exact original ELF symbol/slot checks, ARM-executed
results, host executable hash, compiler command, and current source hashes.
This is a host differential; it does not build or test an Android APK.
