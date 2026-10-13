# armdiff scoping: original ARM32 versus Track B reconstructions

Date: 2026-10-13. Research only. Track A was read, not changed. Nothing was installed, and no Track A harness was run in a way that writes into Track A (the one loader run used `-B`, and no `__pycache__` was created).

## Summary

- Track A's "original ARM" checks are CPU emulation with Unicorn 2.1.4 of the original `libDungeonHunter2.so`. They are not qemu, a phone, or an emulator image. The shared loader and call machinery is `port/engine-resources/tests/cpu.py`. Track A's reconstructed side is ARM64 code built with the Android NDK, which is not installed here.
- Installed and working: Python 3.14 (`py -3`), Unicorn 2.1.4, pyelftools 0.33, capstone. Missing: any ARM or ARM64 cross compiler, clang, zig, the Android NDK, and adb. Present: MinGW-w64 GCC 16.2 (x86_64), which is Track B's host toolchain.
- The original library is present and matches the pinned SHA-256, but not at the path given in the task (see section 3).
- The original-side machinery can be reused in concept. Track B's rules say it uses Track A code as a behaviour reference only and never copies it in (`port/decomp-reconstruction/TRACK-B.md`), so armdiff should be a fresh implementation.
- Track A's loader has gaps that matter for Track B. It ignores 4,969 ABS32 and 11 GLOB_DAT relocations, including the stack guard, so stack-protected functions would fault.
- Roughly 4,800 of the 10,485 reconstructed functions (46%) make no call to other original code. That is the v1 scope. Calls to other original functions (about 5,650, 54%) need a callee policy and belong in phase 2.
- Recommendation: build `tools/trackb/armdiff` now on the MinGW x86_64 host build, with Unicorn ARM32 for the original side and generated layout bridges that copy fields by original offset. Estimated effort is 3 to 4 weeks for v1. The main risk is the layout tables, not the emulator.

## 1. What exists in Track A

Paths are under `C:/Users/nacworkstation/Documents/Projects/Dungeon Hunter 2 Recomp/DH_sc/`.

| Path | Role |
|---|---|
| `port/engine-resources/tests/cpu.py` | Shared loader. Class `Cpu(path, arm64, provenance)` maps PT_LOAD segments (ARM32 at base 0, ARM64 at 0x1_0000_0000), reads `.dynsym`/`.symtab`, stubs JUMP_SLOT imports, and provides `invoke(symbol_or_addr, args, stack, budget)`. Used by `engine-resources`, `asset-payloads`, `engine-textures`, `engine-skinning`, and the `level-world` host tests. |
| `port/engine-resources/tests/differential.py` | Example full run. README reports 10,449 original ARM32 versus compiled ARM64 comparisons over 35 functions, zero mismatches (`reports/engine-resources-validation.json`). |
| `port/engine-math/tests/differential.py` | Self-contained `Cpu` with its own ELF loader. Imported libm and soft-float calls go to a host oracle DLL built with `cc` and called through ctypes. |
| `port/nativeinterface/tests/differential_arm32.py` | The closest pattern to Track B. Original ARM32 under Unicorn, compared with a host build loaded by `ctypes.CDLL`. Libc is modelled by hand. It patches the stack guard by hand (relocation type 21). |
| `port/level-world/tests/run_character_apply_result_host.py` | Compiles a host executable with g++, runs it, and compares its JSON output with a Unicorn oracle built on `cpu.py`. |
| `compatibility/work/native-port/verify_translations.py` | Four functions, 45,330 comparisons. Pins the original SHA-256 `36498eb8...`. Its ARM64 side is hand-written assembly built with Zig 0.13 (`requirements.txt`). |
| `port/*/build.py` (e.g. `port/character-state/build.py`) | Build ARM64 `.so` files with the Android NDK clang, and `*-host.so` with `cc`. |

The quoted figures trace to:

- "412 original-ARM cases match": `docs/PROJECT-CHECKLIST.md` (TargetList filters).
- "58 ARM comparisons / zero mismatches": `docs/COMBINED-RECONSTRUCTION-STATUS.md` (Ordinary Player Kill continuation).
- Harness scale: 111 files named `*differential*.py` under `port/*/tests`, and 70 files under `port/*/tests` that call `emu_start`.

### How the cpu.py harness works

- Load: mapping, symbols and entry addresses come from the ELF. A Thumb symbol keeps bit 0 in `st_value`, and Unicorn honours it when the entry address is odd (checked in section 2).
- Imports: each JUMP_SLOT relocation (type 22) gets a 16-byte stub in an "extern" region. A `UC_HOOK_CODE` on that region runs a Python model (memcpy, memmove, memset, a caller-completion fixture). Any other import raises "Unmodeled dependency".
- Call: `invoke()` puts the first four arguments in r0-r3, spills the rest to the stack at SP, sets LR to a stop address, runs `emu_start(address, stop, count=budget)`, checks that PC returned to the stop address, and returns r0.
- Memory: the caller writes fixtures into a 32 MiB data arena (`base + 0x2000000`) with `uc.mem_write`, and reads results back with `mem_read`.
- Compare: tests run the same inputs through two `Cpu` instances (original ARM32 and reconstructed ARM64, or host ctypes) and compare r0 and output memory.

### Gaps in cpu.py that affect armdiff (verified against the library)

- Only JUMP_SLOT relocations are applied. The library's `.rel.dyn` holds 49,132 RELATIVE (harmless at base 0), 4,969 ABS32, and 11 GLOB_DAT entries. All 4,969 ABS32 entries point at undefined (imported) symbols, and each holds 0 in place. Those slots stay null, so any vtable or function-pointer slot that refers to an import is null. The 11 GLOB_DAT entries are `__stack_chk_guard`, `_ctype_`, `__sF`, `_toupper_tab_`, `_tolower_tab_`, `__dso_handle`, `_ZSt7nothrow`, `malloc`, `free`, `calloc` and `realloc`. The stack guard is the important one: 354 reconstructed functions call `__stack_chk_fail`, so they read the guard through a GOT slot that cpu.py leaves at 0. I did not run one of them to confirm the fault.
- Thumb works by accident. Track A's code does not handle it explicitly.
- There is no write log, no fault classification, and no separation between "unmodelled" and "mismatch". An unmodelled call raises, which is acceptable, but a memory fault is not labelled.

No qemu-arm usage was found in Track A. A separate Android 17 emulator effort exists (`docs/ANDROID-17-EMULATOR-RESULTS.md`). It is whole-app testing, not function-level, and I did not inspect its tooling.

## 2. What is installed on this machine

Checked with `py -3` (Python 3.14.7, `AppData\Local\Programs\Python\Python314`) and PATH lookups. Nothing was installed.

| Item | State |
|---|---|
| unicorn | 2.1.4 imports and runs. Matches the Track A pin. |
| pyelftools | 0.33. Matches the pin. |
| capstone | Imports. `__version__` reports 5.0.7, but pip metadata reports 5.0.9 (pin). Disassembly works. |
| ziglang | Not installed (pinned 0.13.0 for the ARM64 prototype). |
| keystone, lief | Not installed (not needed). |
| clang, clang++, zig, cl, `cc` | Not on PATH. |
| aarch64 or ARM32 cross compiler | None. |
| Android NDK | Not installed (`ANDROID_NDK_HOME` empty, no SDK). |
| adb, Android emulator, qemu-arm | Not on PATH. |
| MinGW-w64 GCC 16.2, x86_64-w64-mingw32 g++-posix, cmake, ninja | Installed (WinLibs). This is the Track B host toolchain. No i686 MinGW. |

Behaviour checks I ran, all read-only:

- Track A's `cpu.py` loaded the original library. `luaO_log2` returned 0, 1, 10 and 31 for inputs 1, 2, 1024 and 0x80000000. 352 imports were resolved.
- Unicorn Thumb probe: an even entry address ran ARM mode and returned 0. An odd entry address (Thumb bit set) ran `movs r0,#42; bx lr` and returned 42. Thumb entry works when the bit is set.

Track B host build: `build/tb-host` exists with static libraries, including `libdh2_game.a`, `libdh2_core_platform.a`, `libdh2_glitch_core_io.a` and `libdh2_glitch_scene_collada_gui.a`. `ctest_final.log` reports 413 tests with 9 failing (98% pass). `tools/trackb/check_tu.sh` provides the compile flags and include sets that armdiff should reuse.

## 3. The original library

- Path: the task named `lib/armeabi-v7a/libDungeonHunter2.so`. That file does not exist. The file is at `C:/Users/nacworkstation/Documents/Projects/Dungeon Hunter 2 Recomp/lib/armeabi-v7a/ida/libDungeonHunter2.so`. Its SHA-256 is `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`, which matches the pin in Track A's harnesses. Track B's `recovered/native/*/libDungeonHunter2.so` copies also exist, but I did not hash them.
- Symbols: 31,021 defined function symbols. 29,915 have even `st_value` (ARM) and 1,106 have odd `st_value` (Thumb). All 1,106 Thumb symbols are in Track B's `runtime-stl` bucket, which is out of scope. Game and engine code is ARM only.
- Track B's `inventory.csv` (35,159 rows) strips bit 0 from addresses, so Thumb status must come from the ELF symbol table, not the inventory.
- Imports: 351 JUMP_SLOT names. Of these, 131 belong to classes that are hard to model: C++ runtime (`__cxa_*`, `_Unwind`), the printf family (`fprintf`, `sprintf`, and others), file I/O (`fopen`, `fread`, `fwrite`), time, sockets, dlopen, and about 100 GL calls (`gl*`).
- Most-used imports across reconstructed functions (count of functions that reference each): `__stack_chk_fail` 354, `__aeabi_fmul` 277, `__aeabi_fadd` 232, `memcpy` 213, `strcmp` 192, `__aeabi_i2f` 149, `_ZdlPv` 137, `fprintf` 119, `__aeabi_f2iz` 117, `__aeabi_fsub` 114, `memset` 112, `__aeabi_fdiv` 99, `strlen` 99, `sprintf` 85, `_Znwj` 68, `__aeabi_fcmpgt` 65, `__aeabi_fcmplt` 55, `_Znaj` 55, `_ZdaPv` 53, `__aeabi_fcmpeq` 47.

## 4. Track B reconstructed functions by call shape

Source: category ledgers `port/decomp-reconstruction/*/ledger.csv`, rows with status `reconstructed`. There are 10,485 distinct addresses when the CSV is parsed properly. A naive line count gives 10,211 because notes contain line breaks.

Method: capstone ARM decode over each symbol's `[addr, addr+st_size)`. A BL or BLX to a PLT entry is an import call. A BL to another `.text` function is an internal call. A B to another function is a tail call. BLX register is an indirect call. Classification is approximate: tail jumps into the PLT are not classified in the first pass. A second pass that counts them gives 1,031 import-only functions, not 934.

| Shape | Functions | Share |
|---|---:|---:|
| No BL, BLX or tail jump to other code (includes a few tail jumps into imports) | 3,868 | 36.9% |
| Import calls only, no internal call | 934 | 8.9% |
| Calls other original code (BL or tail branch) | 5,655 | 53.9% |
| Indirect calls and no internal call | 27 | 0.3% |
| No symbol size | 1 | |

Functions with any indirect call: about 90. v1 scope (no internal call, no indirect call) is about 4,800 functions, or 46%.

## 5. Design: `tools/trackb/armdiff`

### 5.1 Files

- `armdiff.py`: driver. Selects functions from the manifest, generates cases, runs each batch in a child process, and writes `reports/decomp-reconstruction/armdiff/<batch>.json`.
- `arm_original.py`: fresh Unicorn ARM32 loader and stubs (section 5.2).
- `layout.py`: original-layout tables per class, plus the constructor self-check (section 5.4).
- `gen_bridge.py`: generates one C++ bridge per batch (section 5.3).
- `cases.py`: deterministic input generation (section 5.5).
- `compare.py`: comparison and outcome taxonomy (section 5.6).
- Negative controls: a deliberately wrong host result per batch, which must be reported as a mismatch.

### 5.2 Original side (Unicorn ARM32)

- Map PT_LOAD segments at base 0, as cpu.py does. Entry address is `st_value`, so a Thumb entry keeps bit 0. Only game code is in scope, so no Thumb handling beyond that is needed.
- Apply all relocation types, not just JUMP_SLOT:
  - JUMP_SLOT and ABS32 or GLOB_DAT to an undefined function symbol write the address of an extern stub.
  - GLOB_DAT to a data symbol gets a fixture cell. For `__stack_chk_guard` this is a constant cell. For the ctype and `__sF` tables it needs real content, so functions using them stay unmodelled in v1.
  - RELATIVE at base 0 needs no change.
- Stub models (Python, one per import name):
  - Memory and string: `memcpy`, `memmove`, `memset`, `strlen`, `strcmp`, `strcpy`.
  - Heap: `_Znwj`, `_Znaj`, `_ZdlPv`, `_ZdaPv` and `malloc` and `free` use a fixture heap with sequence numbers. Pointers are reported as tokens, not addresses.
  - Soft-float helpers `__aeabi_fadd`, `fsub`, `fmul`, `fdiv`, `i2f`, `f2iz`, `fcmp*`, implemented with Android's semantics. For example, `f2iz` saturates and NaN gives 0.
  - `__stack_chk_fail`: fault.
- Write log: a `UC_HOOK_MEM_WRITE` on the fixture arena records every write, so untabled writes can be detected.
- Fault taxonomy: unmapped access, budget exhaustion, or an unmodelled import gives `unmodeled`, never `match`.

### 5.3 Reconstructed side (host)

- Build one DLL per batch with MinGW x86_64 g++, using the flags and include sets of `tools/trackb/check_tu.sh` (`-std=c++17`, the category include dirs, the vendor include dirs). Link the batch's translation units, the generated bridge, and a fixture translation unit that implements the same helper set in host code. Do not use C casts for float-to-int conversion. Use the same saturating helper as the ARM side.
- Entry point per function: `extern "C" int armdiff_<addr>(ArmdiffCall*)`. It unpacks the object images into host objects, calls the real member or free function, packs the objects back, and writes the return value.
- Run each batch in a child process. A `std::abort()` from a partial body, or a crash, must not take down the driver. A process exit status is recorded as `host_unsupported`.

### 5.4 Layout: 32-bit original, 64-bit host

The 32-bit original layout is the source of truth. Host structs are never cast onto original memory, and raw host pointers never cross the boundary.

- Layout table per class, as JSON: `{class, original_size, vtable_symbol, fields: [{name, orig_off, width, kind}], embedded: [...]}`.
- Sources:
  - `// +0x..` annotations on members in `port/decomp-reconstruction/game/include/dh2/*.hpp`. There are 3,105 annotations in 305 headers. Warning: many of these annotate virtual-method vtable slots (for example `// +0x08 0x474678`), not fields. The generator must separate member declarations from vtable-slot comments.
  - `// Original size N` comments (for example `AnimController.hpp`: "Original size 8").
  - `reports/decomp-reconstruction/class-facts.json` for bases, `alloc_sizes` and `vtable_bytes`.
  - `_ZTV<class>` symbols give each class's original vtable address.
- Host offsets come from `offsetof(T, member)`. The bridge copies each declared member into a 32-bit image at `orig_off`, using the member's type and width. Pack and unpack are generated per class.
- Pointers become 32-bit tokens through a per-case registry. A host pointer maps to a token, and an ARM fixture address maps to the same token. Null stays 0.
- Vtable pointer: the image holds the class's original vtable address (from `_ZTV`). The host bridge gives the host object a valid vptr by constructing the class in fixture storage, or by copying the vptr slot from a host-constructed instance. On pack, the host vptr maps back to the class token, and then to the original address.
- Untabled bytes: any byte outside the table must be unchanged. An ARM write to an untabled byte is reported as a `layout_gap`. This is a useful output, because it finds members missing from the header.
- Constructor self-check: for classes with an original constructor in scope, run the ARM constructor on fresh memory and check its writes against the table. Mismatches are layout errors to fix before any comparison counts.
- Type widths: `int`, `unsigned`, `float`, `bool`, `char` and `short` match on both sides. `long` is 32 bits on both (LLP64 MinGW, ILP32 ARM). Pointers, `size_t`, `wchar_t` (2 bytes on MinGW, 4 on Android) and `long double` differ. Those fields are excluded, or mapped explicitly.

### 5.5 Inputs

- Scalars: a boundary set (0, ±1, INT_MIN, INT_MAX, 0x7fffffff, 0x80000000, 0xffffffff, powers of two and their neighbours), plus seeded random 32-bit values. Float arguments are bit patterns: ±0, ±1, denormals, ±inf, NaN (0x7fc00000), and large values. Enums and bools are generated from their declared domain.
- Object images: a buffer of `original_size` bytes. Declared fields take values from their domain, pointer fields take fixture tokens (null, a valid fixture object, and an invalid one where the code path requires it), and the vptr is the class vtable. Globals the function reads or writes are declared explicitly, by ELF symbol.
- Both sides receive the same image bytes and the same scalars. Seeds are fixed so runs repeat.

### 5.6 Comparison

- Return value: r0 (and r1 for 64-bit returns). Float returns use r0 under the soft-float ABI, so the bit patterns are compared with NaN canonicalised. A struct returned through a hidden pointer is compared through its image.
- Objects: packed image compared over tabled bytes. Untabled bytes must equal their input bytes, otherwise `layout_gap`.
- Fixtures: each fixture object compared the same way.
- Trace: the sequence of stub calls (import name, with pointer arguments normalised to tokens) must be identical on both sides.
- Outcomes: `match`, `mismatch` (with the first failing case), `unmodeled`, `host_unsupported`, `layout_gap`. A batch is green only if it has zero `mismatch` and zero `layout_gap`, and its negative control fails.

### 5.7 Known semantic differences to handle

- NaN sign and payload differ between Android's soft-float helpers and x86 SSE. Compare canonical NaNs.
- Float-to-int out of range: ARM `__aeabi_f2iz` saturates. x86 `cvttss2si` returns 0x80000000. The host fixture must use the ARM semantics, or the out-of-range inputs must be excluded.
- libm (`sinf`, `cosf`, `sqrtf` and others): run both sides against the same host oracle, as `engine-math` does. This tests the reconstruction logic, not libm.
- Widths: `wchar_t`, `long double`, `size_t` and pointers, as in 5.4.

## 6. What it can and cannot handle

| Class | Phase | Notes |
|---|---|---|
| Scalar math with no calls | v1 | About 3,870 functions have no BL, BLX or tail call. Soft-float helpers are modelled (5.2). |
| Struct mutators on one owner object, scalar and pointer-free fields | v1 | Needs the layout table for the owner class. |
| Functions calling `memcpy`, `memset`, `strcmp`, `strlen`, `strcpy`, `operator new` and `delete` | v1 | Fixture heap, pointers as tokens. |
| Stack-protected functions (about 354) | v1 | Only after the GLOB_DAT fix in 5.2. |
| Functions calling other original functions (about 5,650) | phase 2 | Per-callee policy: record and script the callee, or run the reconstructed callee on the host while the original callee runs on ARM. |
| Indirect and virtual calls (about 90 direct; virtual calls through `_ZTV` images) | phase 2 | Slot target is original code or a fixture. |
| C++ exceptions and unwinding, printf family, GL, sockets, file I/O, time, dlopen (131 imports) | not modelled | Reported as `unmodeled`. |
| Partial (4,550) and skipped (2,925) rows | excluded | They have no complete reconstruction to compare. |
| Declared globals only, no other global state; TLS; threads; nondeterministic output | excluded | |
| Thumb code in game code | not an issue | None found outside `runtime-stl`. |

## 7. Effort estimate

One engineer, rough estimates, in working days:

| Phase | Work | Days |
|---|---|---:|
| P0 | Function manifest from ELF symbols, ledgers and inventory (address, size, Thumb flag, status, owner, call shape) | 1 to 2 |
| P1 | Original-side runner: fresh loader with the relocation and stub fixes, write log, fault taxonomy | 3 to 4 |
| P2 | Layout tables for the v1 owner classes, and the constructor self-check | 4 to 6 |
| P3 | Host bridge generator, MinGW build, ctypes driver, child-process isolation | 4 to 5 |
| P4 | Case generators, comparator, reports, negative controls | 3 to 4 |
| v1 total | About 4,800 functions | 15 to 21 (3 to 4 weeks) |
| Phase 2 | Callee policy, fixtures for indirect and virtual calls | 3 to 6 weeks, depending on how many callees are reconstructed |

The largest uncertainty is P2. The number of v1 owner classes has not been counted yet, and the `// +0x` comments mix field offsets with vtable-slot offsets.

## 8. Alternatives considered

- Track A style: build Track B for ARM64 (NDK clang, or Zig 0.13) and run it in Unicorn ARM64 beside the original. This reuses Track A's machinery more fully, but it still has 64-bit pointers, so the layout bridge is still needed. It also needs a toolchain that is not installed. Not recommended for v1.
- ARM32 build of the Track B source (NDK `armeabi-v7a`). This gives true 32-bit layouts and identical memory images on both sides, so no bridge is needed and the memory comparison is exact. The costs are: installing the NDK (needs approval), a second build configuration that goes against Track B's 64-bit rule, and possible compile issues. Worth revisiting if the layout bridge proves too expensive.
- Host MinGW with a layout bridge: available now, closest to Track B's existing build, and consistent with its rules. Recommended.

## 9. Recommendation

1. Build `tools/trackb/armdiff` on the MinGW host build now. Write the loader fresh inside Track B. Do not import or copy Track A code, as `TRACK-B.md` requires.
2. Set v1 scope to the about 4,800 reconstructed functions with no internal call and no indirect call. Pilot on the no-call functions first (about 3,870). Choose a category that already has tests, such as `game-anim`.
3. Build the layout-table generator first and validate it with the constructor self-check. Layout is the largest risk.
4. Fix the loader gaps (ABS32, GLOB_DAT, write log, fault classes) before trusting any result. Add a negative control to every batch: a deliberately wrong host result must be reported as a mismatch.
5. Decisions needed from the user or integrator: whether to approve an Android NDK install (only if the bridge proves too costly), and where armdiff's generated layout tables should live.

Not verified: the hash of Track B's own `recovered/native` copy of the library; whether any of the 9 failing host tests affects functions in the v1 scope; and the internals of Track A's Android emulator effort.
