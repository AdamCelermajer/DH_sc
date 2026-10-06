# Generic load orchestration V36

Implemented in `lifecycle_v36.hpp/.cpp`, with an independent `tests/cmake-lifecycle-v36` probe. No selected core or shared checkout changes.

## Verified

The original `Level::_LoadProcess()` at **0x3f6990** dispatches actual field130 through38 state entries. The native stage manifest matches all38 original ARM dispatcher targets. Original ARM scalar tail0x3f6e9c..0x3f6ef4 matches195 native cases: signed minimum of words134/138; progress division by38; state36 progress100 while still waiting; state38 progress100 while skipping counter updates.

The adapter executes at most one stage-body call per tick, retains the actual Level and resource pins, latches missing/failed/throwing providers, rejects reentry and unexpected state rewinds, accepts the real menu36->37 transition, retains completed source prefixes after callback failure, and retains resources until cancellation cleanup completes. Eleven meaningful adapter safety cases pass ASAN/UBSAN. Counters are uint32_t physical words; signed original comparisons use a memcpy bit interpretation. No pointer reinterpretation of64-bit constructor fields is allowed.

`source_finished` is a statement about the supplied actual source loading state only. `gameplay_ready` and `whole_level_init_verified` remain false. Host fixture bodies do not verify whole Level initialization.

## Source facts and adapter decisions

- Original map-loading state7 and ObjectManager.InitPost state10 spin internally until successful. The native adapter yields a pending body after one service call; the provider must retain the authentic cursor/prefix, so constructors and side effects cannot replay. This is an explicit responsiveness adaptation.
- Source states1/27/28/35 have no stage body and increment through the original common/default branch. States3/16/19 have only debug-switch queries then increment. Those debug queries are deliberately excluded from the production-effect service table; no claim of complete debug-path execution.
- Source state36 executes online/network/quest synchronization as applicable, then leaves field130 unchanged. Only the actual menu `NativeEndLoading` may produce37. The adapter never fabricates that transition.
- Source state37 finishes menu/input handling before incrementing38. Progress100 at36 alone does not complete loading.
- Cancellation and diagnostics are native safety additions; no cancellation branch exists in the original LoadProcess. Providers must be bounded, because this wrapper cannot preempt a synchronous callback. Elapsed time and process CPU ticks measure actual callback execution; they never drive progress.
- Original `Level::Unload()`0x3f0690 begins `QuickSave(fieldf0)`, pushes the loading menu, deletes actual camera fields128/12c, saves players, stops sounds, resets field130, uninitializes network, updates players and timers. It does **not** by itself prove destruction of all Level/object/visual resources. Production cancellation must distinguish an incomplete preparation abort from normal saved-game Unload and complete actual owner destruction/lease release before returning complete. Main owns save side effects.

## Required main-session bindings

1. Genuine32-bit **actual** constructor storage for field30/130/134/138 and a retained actual-Level borrow. The received `LoadingFieldsV26` exposes30/130 only; active snapshot constructor130/134/138 are uintptr_t. Do not cast those to uint32_t pointers. This adapter is presently a standalone contract/probe until the genuine constructor dependency and all four borrows are supplied.
2. Real source bodies for states0/2/4..15/17..18/20..26/29..34/36..37 from existing authoritative managers. The stage table includes entry addresses and precise dependency names. The loader owns sequencing; main owns AI, state machines, save, script/condition execution, sound, real event manager compilation and gameplay behavior.
3. Actual post-tail progress publication/menu/license behavior, consumed by the menu/main owners. NativeEndLoading must remain the same real global/receiver service.
4. Actual resource counter snapshot and correct incomplete-abort/Unload/destruction provider. Missing counters remain explicitly unavailable rather than guessed.
5. Real owner-thread execution. Shared pins prevent lifetime loss but do not confer cross-thread safety.

## Reproduce

Run the Python oracle `reference/level-init-v36/prove_lifecycle_v36.py`; it requires the existing Unicorn test runtime and original `.local-inputs/libDungeonHunter2.so`. It stops before external stage bodies/menu/license callbacks and writes immutable-value gold plus provenance.

```text
wsl.exe -d Ubuntu -- cmake -S /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/tests/cmake-lifecycle-v36 -B /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/lifecycle-v36-host -G Ninja -DDH2_LIFECYCLE_SANITIZE=ON
wsl.exe -d Ubuntu -- cmake --build /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/lifecycle-v36-host -j 2
wsl.exe -d Ubuntu -- /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/lifecycle-v36-host/dh2_loader_lifecycle_v36 /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/level-init-v36/lifecycle_v36_original_gold.bin
```

The receipt is `reports/lifecycle_v36_verified.json`. No emulator launch/install/process mutation occurred.
