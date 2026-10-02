# 16 KiB host-page mapper proof of concept

This directory holds experimental patches for public ZettaBridge commit `7c647a4f1ea150eab7978ab0da28fdf49f3a79de`. They remain separate from the supported DH2 build. An isolated local test APK was built with these patches and a temporarily changed page guard; that APK is not a validated game release. The repository builder's 4096-byte host-page launch guard remains in force.

## What the patch does

- Keeps the ARM32 guest ABI at 4096-byte pages and detects the host kernel page size independently.
- On a 16384-byte host, allocates complete host pages while tracking the four guest subpages separately. Mapping, protection and unmapping one guest subpage preserve the bytes and guest flags of its neighbors.
- Materializes `MAP_PRIVATE` file mappings so a 4096-byte file offset can be used on a 16384-byte host. `MAP_SHARED` file mappings fail explicitly with `ENOTSUP` in this prototype.
- Disables Dynarmic's direct fastmem pointer for 16 KiB hosts so translated accesses should use the checked guest-memory callbacks.
- Leaves the original 4 KiB host path intact.

The separate `zettabridge-madvise-wipeonfork-16k.patch` handles one verified guest syscall issue. It returns success for `MADV_WIPEONFORK` on a 16 KiB host after checking that the guest range is mapped. ZettaBridge refuses guest `fork` (`clone` without `CLONE_VM|CLONE_THREAD`), so this fork-only advice has no effect on supported guest execution. Passing the 4 KiB-aligned range through to the host kernel returned `EINVAL` and made Android bionic abort. Other `madvise` operations are unchanged and need guest-aware handling.

## Reproduce the focused test

Use the pinned ZettaBridge source and apply `zettabridge-guest-memory-16k-poc.patch`. The source test is `tests/guest_memory_coarse_test.cpp` in this directory. The build needs Android NDK r29 and an Android x86_64 emulator that reports `16384` from `adb shell getconf PAGE_SIZE`.

```powershell
git clone https://github.com/ZailoxTT/ZettaBridge.git ZettaBridge-16k-test
cd ZettaBridge-16k-test
git checkout 7c647a4f1ea150eab7978ab0da28fdf49f3a79de
git apply path/to/DH_sc/compatibility/16k-port/zettabridge-guest-memory-16k-poc.patch

$ndkBin = 'path/to/android-sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin'
$test = 'path/to/DH_sc/compatibility/16k-port/tests/guest_memory_coarse_test.cpp'
& (Join-Path $ndkBin 'x86_64-linux-android35-clang++.cmd') -std=c++20 -O2 -fPIE -pie -static-libstdc++ -I core/include $test core/src/guest_memory.cpp core/src/log.cpp -llog -o guest_memory_coarse_test
adb -s emulator-5558 push guest_memory_coarse_test /data/local/tmp/guest_memory_coarse_test
adb -s emulator-5558 shell 'chmod 755 /data/local/tmp/guest_memory_coarse_test; /data/local/tmp/guest_memory_coarse_test'
```

Use the serial reported by `adb devices` for the 16 KiB emulator; `emulator-5558` was the local test serial. The test creates and removes a temporary file under `/data/local/tmp`.

## Observed result, 2026-10-02

The patch applied cleanly to a fresh worktree of the pinned commit. The patched `guest_memory.cpp` also compiled to an ARM64 object with NDK r29, API 35. The NDK r29 x86_64 executable ran on the Android API 37.2 16 KiB emulator (`sdk_gphone16k_x86_64`). `getconf PAGE_SIZE` returned `16384`. All 23 checks passed with zero failures:

- two adjacent 4 KiB guest mappings preserve each other's contents inside one 16 KiB host page;
- read-only protection and unmap apply to only one guest subpage according to guest metadata;
- remapping zeros the selected guest subpage without erasing its neighbor;
- two private file mappings at 4 KiB host-unaligned offsets contain the expected bytes and preserve adjacent mappings;
- unsupported shared file mapping returns `ENOTSUP`;
- unmapping and remapping a full host page yields zeroed memory.

This is a memory-component result, not a game launch or an ARM64 runtime result. The test is an x86_64 Android executable, and the patched `guest_thread.cpp` was not part of that executable.

## Isolated Android 17 16 KiB runtime trial, 2026-10-02

An isolated ARM64 `libzbridge.so` was built with NDK r29 from the pinned ZettaBridge commit, the existing DH2 translator patch, the mapper patch above, the separate `MADV_WIPEONFORK` patch, and ZettaBridge's required `third_party/patches/dynarmic-0001-thumb32-armv8.patch`. The binary's SHA-256 was `e18028f24efd9447036b6f0abdb5c49abefa2840026c91dfede12597e2a5efca`; all ELF `LOAD` segments had 16 KiB alignment. A private Test 9 wrapper APK bundled this library and the owner's verified game/cache inputs. Its page guard was changed **only in the isolated build copy** to admit 16 KiB hosts. That signed test APK had SHA-256 `3c138043b6b12300ae4d92b49748a9d3e14fc935a52bc63c78be03e2bc101bfd`.

On the official Android 17/API 37.2 `sdk_gphone16k_x86_64` emulator, `getconf PAGE_SIZE` returned `16384`. The APK installed, the wrapper opened, and **Launch Game** loaded the ARM64 translator through Android's x86 emulator native bridge. The guest linker started, `libnativeinterface.so` loaded, the original game engine bound 32 JNI natives, `libStormGLOFT.so` loaded, and the GL thread reached shader processing. This is real runtime progress beyond the page guard, but no menu or gameplay was reached.

The GL thread then aborted: guest bionic Scudo reported `corrupted chunk header ... memory corruption or a double free`. Nearby kernel logs contain repeated `do_madvise: addr ... not page aligned` for other 4 KiB guest ranges. Those calls are distinct from the handled `MADV_WIPEONFORK`; the log alone does not prove they caused the Scudo failure. The guest process ended with signal 6. The isolated test therefore **does not establish 16 KiB compatibility**. Its APK and private cache were not added to Git. No Fold7 or physical device was tested.

## Code-level blockers before app integration

1. The private-file materialization is a snapshot and does not reproduce all Linux `mmap` behavior, especially `SIGBUS` for accesses wholly beyond EOF or observation of later file changes. Shared file mappings and writeback are unimplemented. The guest linker and game must be traced for these cases before integration.
2. `sys_madvise` and `sys_msync` in pinned `core/src/syscalls.cpp` forward 4 KiB guest ranges directly to host calls. The isolated trial confirms that other `madvise` calls still reach the 16 KiB kernel with invalid alignment. These calls need guest-aware handling; `mremap` and mapping replacement require a full regression suite.
3. Dynarmic was rebuilt with fastmem disabled and exercised far enough to start the guest engine. Tests still must prove translated loads, stores, instruction fetch, exclusive operations, guest fault addresses, and signal delivery at boundaries between differently protected 4 KiB subpages.
4. JNI, GL and syscall host bridges include direct guest-base pointers and long-lived mapped buffers. They need an audit for permission checks, mapping lifetime, and concurrent map/unmap. The prototype's per-page flag vector is not synchronized for concurrent modifications.
5. The materialization currently stages each private file mapping in memory before commit. Large mappings need bounded staging and failure-atomic behavior before production use.
6. Run the complete translator's guest and host suites on both 4 KiB and 16 KiB Android systems, then build and test the ARM64 standalone app on a real 16 KiB ARM64 runtime environment. Check every native ELF and APK ZIP alignment separately.

Do not remove the wrapper's page-size guard or package this patch as a supported game runtime until those blockers are resolved and the gameplay path is tested.
