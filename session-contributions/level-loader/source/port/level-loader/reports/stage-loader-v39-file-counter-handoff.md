# Genuine source file counter13c V39

Source13c is a32-bit scalar; field140 remains the actual LoadFileData/stream-state pointer. All retained Level direct13c accesses are: C1 zero3f3258; constructor alias zero3f35f0; state0 zero3f7524; state7 LDR3f72c0 / ADD1 at3f72c4 / STR3f72c8. Original `Level::LoadFile` does not directly read13c; its continuation uses140. Source13c is not a pointer or the140 owner.

Original ARM instructions verify9 zero/reset poison cases and7 ADD32 boundaries, including UINT32_MAX->0.140 remains unchanged throughout these isolated counter slices. The complete original state7 oracle already verifies130 increment before13c increment, then common progress tail.

`stage-loader-v39-file-counter-proposed.diff` changes only semantic13c uintptr_t->uint32_t against the private V5 constructor header after reviewed134/138 adoption. It preserves140 as uintptr_t and adds comments identifying the scalar/pointer distinction. Base saved bytes SHAd673aca40592f36af03ebf6c216bf79130223325ae3a377c39dd99289bad46eb; proposed saved bytes SHAdfe08c0300629ca7c01289891e02bbd266a29efc966d26a1c715f4782d2781b1. Selected constructor graph is not changed by this packet.

`stage_loader_v39_file_counter.hpp` borrows actual completed C1 fields through existing validated same-owner lifecycle borrow. A static_assert rejects legacy uintptr_t13c; no cast/reinterpret,64-bit pointer arithmetic or shadow counter. Its `actual_file_counter_increment_v39` service requires actual130=8, performs defined uint32 increment/wrap exactly once, latches wrong-transition failure and preserves field140. Bind this service to Stage7BodyV38.increment_actual_file13c; bind Stage7BodyV38.after_source_increment to the source lifecycle state7 post-increment slot.

Standalone ASAN/UBSAN probe passes9 shape/safety cases, including7 native ADD32 boundaries, incomplete C1 rejection and wrong source-state failure/no retry. Actual production C1 is not executed by that shape probe; parent can instantiate the typed borrow after validating/adopting the genuine scalar header against full C1.

Reproduce original oracle with `python reference/level-init-v36/stage-loader-v39-file-counter-proof.py`; existing Unicorn/test helpers and original ELF are read-only dependencies. Native commands:

```text
wsl.exe -d Ubuntu -- cmake -S /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/tests/cmake-stage-file-counter-v39 -B /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/stage-file-counter-v39 -G Ninja
wsl.exe -d Ubuntu -- cmake --build /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/stage-file-counter-v39 -j 1
wsl.exe -d Ubuntu -- /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/stage-file-counter-v39/dh2_file_counter_v39
```

Whole Init/runtime/rendering remain pending. No shared graph, constructor graph, common CMake/manifest, save or emulator mutation occurred.
