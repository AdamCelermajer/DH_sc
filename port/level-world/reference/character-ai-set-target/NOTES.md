# CharAI::AI_SetTarget source body

This module reconstructs the original 556-byte `CharAI::AI_SetTarget(GameObject*, bool)` body at ELF `0x3d6890`. The original library SHA and exact instruction/vtable ranges are pinned in `original-functions.json`. It has no live Android wiring yet.

## Source order

The method first writes the requested GameObject pointer to AI `+0x3c`. A true `force` byte then copies it to AI `+0x40` and returns; that branch skips every other operation.

For `force=false`, it compares the initial AI `+0x40` pointer with the request. If they differ, it clears the owner's 16-bit field at Character `+0x14d0` before querying the debug manager. It always loads `DebugSwitches` and asks for the exact key `IsTracingCharAITarget`. After that query, the source rereads AI `+0x40`. When tracing is enabled and the pointer changed, it queries the lowercase key `isTracingCharAITarget` if either old or new pointer is nonnull; that result does not gate target installation. The traced old-nonnull to null branch stores null and exits without refreshing other target facts.

The ordinary path stores the requested pointer at AI `+0x40`. A null target exits without updating AI `+0x44`, `+0x48`, `+0x49`, or `+0x4c`. For a nonnull target:

1. Source calls `Character::GetCharAIId()` on the current owner. Invalid row IDs resolve to Basic row 8; this particular caller ignores the result.
2. It freshly compares AI `+0x40` to AI `+0x44`. If different, it clears sticky byte `+0x4c` and copies the current target into `+0x44`.
3. It dispatches target virtual slot `+0x34` (`IsDead`), then stores the returned bool XOR 1 in AI `+0x48`.
4. It reloads AI `+0x40`, calls `AI_IsInSight(GameObject const*)`, and stores that return in AI `+0x49`.

There is no Character-only target gate in this body. A Character target reaches `Character::IsDead` through vtable `+0x34`; a base GameObject reaches `GameObject::IsDead`, which returns false. The method accepts any valid source GameObject pointer and lets virtual dispatch determine that result. The sight call is a separate borrowed service: it owns the original getter/design-data/range calculation and accepts a null argument, for which the original helper checks its live target.

## Port boundaries and error behavior

The public structs are 64-bit host-side projections, not overlays of the original ARM32 classes. Stable identities are borrowed; owners and target facts must remain alive for the synchronous call. The adapter exposes DebugSwitches load/key lookup, target `IsDead`, and full `AI_IsInSight` as ordered services. It preserves source writes before a service error and does not roll them back; service failures are adapter errors because the original method is void. Malformed/overlapping projection storage is rejected before source writes.

The default host test command is:

```powershell
python port/level-world/tests/run_character_ai_set_target_host.py
```

That runner verifies the original library SHA, eight pinned function ranges and two vtable ranges before compiling with `g++ -std=c++17 -Wall -Wextra -Werror -pedantic`. Its seven host suites cover force/no-call behavior, null clearing, duplicate refresh, both debug trace branches, reentrant fresh target/owner reads, base-object virtual dispatch, partial side effects on adapter failure, and pre-mutation alias rejection. DebugSwitches, `IsDead` dynamic dispatch and `AI_IsInSight` are fixtures, not rebuilt dependencies. Its report is `port/level-world/build/character-ai-set-target/validation.json`.

The independent differential command is:

```powershell
python port/level-world/tests/character_ai_set_target_differential.py
```

It executes the original ARM32 `AI_SetTarget` body from the SHA-pinned ELF under Unicorn and compares nine source branch/reentry scenarios with the compiled host kernel. It matched final state fields and ordered DebugSwitches, IsDead and AI_IsInSight service calls with zero mismatches. Those callee bodies remain fixtures. The differential report is `port/level-world/build/character-ai-set-target/arm-differential.json`.

Neither test report claims Android actor binding, long-lived target ownership, actual debug settings, or recovered view-radius geometry.
