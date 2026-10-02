# Android 17 and 16 KiB page support

This is a source audit for the standalone ARM64 compatibility app, not a device acceptance result. The published DH2 wrapper checks `Os.sysconf(_SC_PAGESIZE)` in `compatibility/work/fold7-build/java/com/zettabridge/launcher/Dh2Activity.java` and refuses to launch the guest unless it is 4096. Keep that check until the translator's guest-memory implementation is changed and tested on a 16384-byte host.

Android version and kernel page size are separate test dimensions. An Android 17 image with `adb shell getconf PAGE_SIZE` reporting 4096 can exercise Android 17 behavior with the current runtime. It cannot establish 16 KiB support. Conversely, a 16 KiB Android image is necessary to validate the memory redesign. The x86_64 emulator image is useful for platform checks, but it does not by itself validate the production ARM64 translator library and its translated ARM32 execution path.

## Evidence from the pinned translator

The source inspected was public ZettaBridge commit `7c647a4f1ea150eab7978ab0da28fdf49f3a79de`, the base recorded in `compatibility/BUILDING.md`. The repository carries a DH2 patch and selected modified files rather than the complete pinned translator tree. The observations below refer to that pinned upstream source, with the DH2 patch checked for page-size changes.

| Boundary | Current implementation | 16 KiB host consequence |
| --- | --- | --- |
| Guest ABI | `core/include/zb/guest_memory.h` defines `kPageSize = 4096`; `core/src/process.cpp` supplies `AT_PAGESZ = kPageSize`; `core/src/syscalls.cpp` interprets `mmap2` offsets in 4096-byte units. | The ARM32 guest must retain its 4 KiB ABI and page table. Replacing this constant with 16384 would change guest-visible behavior and file offsets. |
| Backing mappings | `core/src/guest_memory.cpp` reserves 4 GiB, then uses `mmap(MAP_FIXED)` for each guest mapping, including a direct file `mmap` with the guest offset. | A 4 KiB-aligned guest address or offset is not necessarily aligned to the host's 16 KiB page. A coarse remap can also replace a neighboring live 4 KiB guest page. |
| Protection and unmap | The same file calls `mprotect` and replaces unmapped ranges with `MAP_FIXED` anonymous `PROT_NONE` mappings at guest granularity. `core/src/syscalls.cpp` also forwards `madvise` and `msync` against the direct guest base. | Host protection and unmapping cannot independently represent four guest pages inside one host page. Rounded calls could alter live neighbors; unrounded calls can fail. |
| Executed memory | `core/src/guest_thread.cpp` sets Dynarmic's `fastmem_pointer` to the direct guest base. Its slower memory callbacks check the guest page flags. | Merely keeping 4 KiB flag bytes while broadening host mappings would let direct translated loads/stores bypass per-guest-page permissions. |
| Host bridges | `host_ptr` checks guest flags for many JNI, GL, syscall, and asset accesses, while several paths access `memory().base()` directly after their own checks. | Any new backing strategy must audit direct pointers, mapped buffer lifetimes, and host calls that assume a contiguous guest byte range. |

The DH2 patch (`compatibility/work/fold7-build/zettabridge-dh2.patch`) has no guest-memory page-size redesign. The 16 KiB `PT_LOAD` alignment already reported for host ELF files solves a different requirement: loading the ARM64 library. It does not change guest mapping behavior. Android's [16 KiB page guidance](https://developer.android.com/guide/practices/page-sizes) separately calls out code using 4096 and page-aligned `mmap` arguments; its package backcompat mode is not evidence that this translator's explicit mappings work.

An isolated mapper prototype and its 16 KiB emulator component test are in [`compatibility/16k-port`](../compatibility/16k-port/README.md). That test does not validate the ARM64 JIT, host bridges, complete file-mapping semantics, or gameplay.

## Required design and proof

1. Preserve the ARM32 4 KiB guest ABI (`AT_PAGESZ`, `mmap2` units, address checks, and the per-4 KiB metadata). Detect the host page size separately at runtime.
2. Introduce a host-granule backing layer that owns complete 16 KiB chunks without replacing a chunk when one of its four guest subpages is mapped, protected, or unmapped. Zero newly mapped anonymous guest subpages and preserve live neighbors' bytes.
3. Represent 4 KiB-aligned file mappings whose address or offset does not meet host alignment. Preserve private copy-on-write behavior and either implement shared mapping/writeback accurately or reject unsupported shared cases explicitly. Audit `mremap`, `madvise`, and `msync` against that representation.
4. Make translated loads, stores, instruction fetches, and exclusive accesses honor guest flags at 4 KiB granularity. A conservative starting point is to disable direct Dynarmic fastmem on 16 KiB hosts and use the checked callbacks, then measure performance. Re-enable fastmem only with equivalent fine-grained checks or another proven fault scheme.
5. Audit `memory().base()` and `host_ptr` users, especially JNI array and direct-buffer exposure, GL client arrays, and host syscalls. Ensure a host pointer never outlives a mapping change and cannot silently access an unmapped guest subpage.
6. Add tests on an actual 16 KiB kernel: two different mappings sharing one host page; protect or unmap only one guest subpage; a file map at a 4 KiB but not 16 KiB offset; private writes and shared-write handling; cross-subpage read/write/fetch; JIT fault address and signal delivery; multi-threaded mapping changes; ARM32 linker/libc startup; full game startup and save/load. Run the existing 4 KiB suite too.

These are linked correctness changes across the mapper, syscall layer, and JIT memory path. There is no safe wrapper-only or one-line runtime change that unlocks 16 KiB execution. The launch guard should be removed only after the new tests pass on a 16 KiB Android system and the standalone ARM64 build passes native library and APK alignment checks.
