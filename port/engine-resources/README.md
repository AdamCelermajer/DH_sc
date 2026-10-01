# Reconstructed resource-loading checkpoint

This module adds buildable C++ for **35 complete original function bodies**: 11 memory-reader methods, eight subfile-reader methods, and 16 Collada database accessors. It also reconstructs the **whole-buffer branch** of `glitch::res::File::Init()`, exposing immutable BRES offset views for ARM64. The other branches of that function, original object ownership/constructors, nested resource schemas, mesh construction and rendering remain unfinished. This component is not a complete engine or game.

`original-functions.json` maps every translated body to its original mangled symbol, address, size and machine-code SHA-256. `reference/original-functions.asm` contains the complete original ranges, including the still-unimplemented branches of `File::Init()`. The interfaces in `resources.hpp` are independently reconstructed port interfaces; they are not the original studio headers or a replacement ABI for the existing ARM32 library.

## Reader behavior retained

| Method | Observed original behavior |
| --- | --- |
| Memory `getBuffer(long*)` | Returns the buffer and writes the current **position**, not file length |
| Memory seek | Signed 32-bit arithmetic, including wrapping addition; positions below zero are accepted |
| Memory read | Clamps at end of file, returns actual copied count, then advances the cursor |
| Memory async read | Runs synchronously, calls completion immediately, returns true even at EOF; error is one when zero bytes were read |
| Memory async read at offset | Ignores seek failure and then reads from the surviving cursor |
| Subfile read | Resynchronizes the shared underlying cursor when it differs; advances by actual bytes read |
| Subfile async read | Assigns its cursor before checking the end; no callback on that rejection; advances by the clamped **requested** count after forwarding, even after a short read |
| Subfile completion | Receives the underlying reader, not the subfile reader |
| Subfile seek | Computes the requested argument using both cursors; drift changes the result. Cursor updates can survive an underlying seek failure |

The port deliberately rejects copies outside the borrowed memory buffer. It retains negative seek state without reproducing the original's out-of-bounds reads. Both readers borrow their buffer/underlying file and names: callers own those lifetimes. The original `glitch::core::string`, reference counting, boost ownership, cloning and constructor/destructor ABI have not been reconstructed. Completion pointers must be valid and nonnull. Callback reentrancy, deferred/custom underlying readers and multithreaded use need further integration testing.

## BRES findings

Every recovered `.bdae` is a little-endian BRES image with a 60-byte header and file-offset pointer fields. The complete cache corpus contains 2,901 files, 143,565,008 bytes and 2,577,206 fixup entries. All identify version `0,0,0,777` through the Collada root's version-string pointer.

| Header byte offset | Recovered meaning/evidence |
| --- | --- |
| 0 | Four-byte `BRES` magic, tested by the original initializer |
| 4 | Byte-order marker `fe ff` in all recovered files |
| 6 | Flags; the original initializer sets bit `0x8000` after relocation |
| 8 | Header size: 60 |
| 12 | Complete serialized image length |
| 16 | Number of four-byte fixup-table entries |
| 20 | Reference/base selector used by split/external-resource loading; zero in every recovered file |
| 24 | Fixup-table offset: 60 before relocation |
| 28 | Start after the fixup table; string data follows |
| 32 | Offset of the 192-byte Collada root |
| 36 | Auxiliary section offset; the reader-loading branch reads a length-prefixed external-resource name here. Every recovered file's first word at this offset is one |
| 40 | Start of the bulk-data area, or the file end when no bulk data exists |
| 44, 48 | Bulk-data byte size and block count, used by the original split-loader allocation path |
| 52 | Separate-allocation selector in the original split-loader path; zero in this corpus |
| 56 | Trailing-byte count retained by the original initializer |

The fixup table contains **offsets of pointer fields**, not a list of resource payloads. Its first entry is 24, the header's own table pointer. The original whole-buffer initializer first adds the image base to that pointer, then turns every table entry into a field address. For each entry after the first, it adds the image base to the four-byte pointer stored at that field. The first field is skipped because it was already relocated. Some targets are exactly the file end; the checked view allows such pointers without dereferencing them.

Writing eight-byte pointers back into these four-byte serialized fields would corrupt the file. `dh2_bres_open` therefore validates and retains the unmodified image. `dh2_bres_fixups` writes field/target pairs to a separate native-pointer array supplied by the caller. Collada accessors compute native pointers from serialized offsets. A view must come from a successful open; its bytes must stay alive and immutable. The component allocates no resource storage itself.

The checker supports the exact whole-buffer format in this recovered corpus. It rejects truncated headers/images, unexpected endian/header markers, already-relocated images, nonzero external bases, invalid fixup ranges/targets and invalid root spans. These checks intentionally exceed the original's unchecked behavior. Split allocations and cross-file/high-bit relocation references are not supported.

## Recovered root libraries

The pointer locations and record strides come from the original accessors' load/multiply instructions. Count locations immediately before their pointers are supported by the cache and bounds checks. These are serialized record sizes, not native ARM64 C++ object sizes. Empty tables return null through the checked port API; the old unchecked accessor could return an invalid address for an out-of-range index.

| Library | Count / pointer offsets within root | Serialized record bytes | Records across recovered files |
| --- | --- | ---: | ---: |
| Animation | `0x24 / 0x28` | 32 | 49,060 |
| Animation clip | `0x34 / 0x38` | 12 | 2,417 |
| Camera | `0x3c / 0x40` | 28 | 76 |
| Light | `0x44 / 0x48` | 24 | 45 |
| Image | `0x4c / 0x50` | 20 | 3,662 |
| Effect | `0x54 / 0x58` | 116 | 3,855 |
| Material | `0x5c / 0x60` | 36 | 4,329 |
| Geometry | `0x68 / 0x6c` | 16 | 10,933 |
| Controller | `0x70 / 0x74` | 12 | 727 |
| Emitter | `0x78 / 0x7c` | 144 | 415 |
| GNPS emitter | `0x80 / 0x84` | 232 | 0 |
| Force | `0x88 / 0x8c` | 16 | 149 |
| Coronas | `0x90 / 0x94` | 36 | 0 |

The clip-library descriptor is at root `+0x34`; the scene descriptor is at root `+0xb8`. Nested records, GPU buffer layouts, animation curves, textures and material construction still need decoding. The counts above include repeated records in different resource files, not unique game objects or distinct animations.

## Build, inspect and validate

From the reconstruction workspace root, using a Linux host compiler and Android NDK r29:

```sh
python -m pip install -r port/engine-resources/requirements.txt
python port/engine-resources/build.py --ndk /absolute/path/to/android-ndk-r29
python port/engine-resources/tools/inspect_bres.py /absolute/path/to/candle_flame.bdae
python port/engine-resources/tests/differential.py \
  --original /absolute/path/to/libDungeonHunter2.so \
  --assets /absolute/path/to/recovered/cache/files
```

The inspector accepts files or directories and can write a JSON-lines catalog with `--output`. It reads through the compiled host component, not an independent Python parser. `--max-files` on the differential harness is a smoke-test option; omit it to validate the whole corpus. The harness refuses an original engine with any SHA-256 other than `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`, and verifies every selected function hash before execution.

To exercise checked inputs under host AddressSanitizer/UndefinedBehaviorSanitizer:

```sh
ASAN_OPTIONS=detect_leaks=0 python port/engine-resources/build.py \
  --sanitize /absolute/path/to/candle_flame.bdae
```

This performs 10,000 deterministic corruption/truncation probes plus bounded-reader cases. Leak detection is disabled because this execution environment does not expose the process information LeakSanitizer requires; it is not a leak test.

## Recorded validation and limits

`reports/engine-resources-validation.json` records **10,449 original-ARM32 versus compiled-ARM64 reader comparisons**, with zero mismatches. All **331 original instruction addresses** in the 35 complete function bodies execute during the full run. Another 65 addresses execute in the partially reconstructed BRES initializer. Full address coverage is not exhaustive path or input coverage.

For all **2,901 recovered BRES files**, the harness executes the original whole-buffer initializer and checks every resulting byte against the expected in-place relocation. It compares the compiled ARM64 fixup output's **5,154,412 native field/target pointer values**, checks that the port leaves the input untouched, and checks surrounding guards. It also compares **27,812** root-library/scene pointers with the original accessors. A separate miniature BRES graph exercises every library type, including the two absent from the cache. Version pointers, all table counts, selected first/middle/last records, invalid indices, malformed headers and insufficient output capacity are checked.

ARM64 code, data, stack, callback and resource pointers all run above 4 GiB in Unicorn. Original readers call the actual original virtual memory-reader methods. Only imported libc copying/zeroing and the ARM32 caller completion fixture are modeled; the ARM64 completion fixture executes compiled C++. No selected engine algorithm is mocked. The ARM64 library builds with 16 KiB load-segment alignment and imports libc/libm/libdl without `libc++_shared.so`.

Host ASan/UBSan corruption probes pass with leak detection disabled. No Android device load, game integration, GPU upload, scene rendering or playback has been performed for this component. The full engine and modern Android game remain unfinished.
