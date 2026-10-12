V99 SAME Level cleanup first-failure correction — source only

Adopt level_unload_source_v1.hpp and level_destroy_source_v1.hpp into the existing port/level-loader headers used by Main. These are surgical successors of current shared Root source; no engine provider or loading pipeline is replaced. Shared Root was read only.

Both journals now latch a recursive execute failure at the active native phase before nested receiver admission can return. The existing attempted no-replay policy is unchanged. A private continuing(error) check restores the first failure regardless of later callback error clearing. call() checks it before assigning a new phase and immediately after callback delivery, before accepting the callback result.

The same check covers direct QuickSave, camera borrow/D0, item flush, actual event194 borrower/destructors/native destruction, batch borrow/D1, inherited EventManager.flush, Save runtime/release and embedded Lua D1. Subsequent native bodies, raw128/12c/158/15c/194 clears, Save/Lua ownership retirement, later phase mutations and completion cannot pass a latched failure. Existing owner/control-block replacement checks remain intact. No nonempty mutable error is treated as failure unless the original bool body failed or execute recursion explicitly failed.

Original call order, source stores, actual methods, quiescence providers, owner pins and no-replay semantics remain unchanged. Completed partial native cleanup stays a failed prefix; this change invents no retry, recovery, barrier, resource or manager. Existing GSLevel callbacks bind these same journal execute methods. No CMake/TU change is needed.

No tests, compilation, builds, hashes, device or runtime jobs ran. Main schedules integrated milestone validation.
Parent generated level-release-first-failure-v99.patch as the exact two-header delta against current Main. Source/peer review confirms direct native paths stop before later calls/stores and synchronous Level/constructor pins remain. Apply delta surgically rather than overwrite evolving shared headers. No validation jobs.
