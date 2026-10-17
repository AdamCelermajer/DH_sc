# ObjectManager per-object slice

Original ELF SHA256: `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Caller: `ObjectManager::Update(float)` 0x34a620/1528. Pinned spans: 0x34a7f4/92 and 0x34aa10/124. This is one bounded slice, zero complete bodies.

The nonnull object is captured once. Fresh virtual +0x24, optional UpdateAIPointers, then live +0x85/+0x8a decide whether GetOnline/byte+5 and fresh virtual +0x54 run. Offline writes +0x86=0 before a second +0x24 and optional UnLoadScriptProcess (fresh supplied r1/r3, r2=0). Update reloads +0x81; nonzero stops before deletion. Zero writes +0x88=0 before fresh virtual +0x2c; its return is discarded.

The oracle executes 52 pinned instructions: three nonnull selection/precondition instructions plus 49 slice instructions. 0x34aa5c/60 list advancement is excluded; deletion stops at 0x34a9e0. Virtual callees, online/AIPointers/unload bodies, lists, outer Level gate and post-update bookkeeping remain providers. Vtable entries are pinned at their original table+8 address points.

30 ARM comparisons include callback mutations, fresh deletion/unload values, replacement virtual tables and mutation of the selected list node while retaining its captured object. Host guards cover alignment/aliases, full-width arguments, active output reuse, identity/pointer reentry, table capture and effects retained after provider failure/exception. The runner pins inputs before compile and rejects changes afterward.

Replay (absolute original ELF/compiler required):
`python port/level-world/tests/run_object_update_dispatch_host.py --original-elf ORIGINAL.so --compiler g++.exe`
No native/Android/gameplay claim.
