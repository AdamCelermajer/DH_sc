> Imported static research from engine branch `e6da25b`. Current implementation and native wiring are tracked in the [branch audit](../../docs/BRANCH-AUDIT-2026-10-05.md). The reader-context C++ candidate described below is not merged or behaviorally validated in this branch.

# BRES `FileReader` context reconstruction

## Binary provenance

This trace is from the supplied `libDungeonHunter2.so` (SHA-256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`). The reader initializer is `glitch::res::File::Init(glitch::res::FileReader*)` at ELF VA `0x0069a880`, size 1,036 bytes, SHA-256 `2c8604d4442f9c5f8caace3a10a8b12c4f250614c125eca8c841a923e49aa495`. Its callee `glitch::res::File::Init()` is at `0x0069a40c`, size 1,108 bytes, SHA-256 `e36600c917439651bfec9dedadfdf85537210401a6c00e0a5f96617e7f50a05a`. Both original ARM listings are in [`reference/engine-branch-original-functions.asm`](reference/engine-branch-original-functions.asm).

The trace follows the actual vtable calls in those ranges: reader `getSize` at vtable `+0x14`, `seek` at `+0x10`, and `read` at `+0x0c`. Allocation and deallocation calls are preserved as observed; they do not establish a general destructor policy.

## Sequential read contract

Let `L` be the reader's reported size, `Nf` the header fixup count at `+0x10`, `B` the bulk byte count at `+0x2c`, `Nb` the block count at `+0x30`, and `T` the trailer byte count at `+0x38`. The initializer computes the main-image allocation size:

```text
Nmain = L - 4*Nf - B - T
```

For the normal reference selector at header `+0x14 == 0`, it performs these reads in order:

1. Read the 60-byte header into a scratch allocation.
2. Seek to source position 60.
3. Read `4*Nf` bytes into a separate `uint32_t[Nf]` fixup allocation.
4. Copy the scratch header into the main-image allocation, then read `Nmain-60` bytes into image offset 60.
5. When `B > 0`, read `8*Nb` row bytes into a separate row allocation.
6. Read the block payload bytes according to the allocation selector at header `+0x34`.
7. Leave the final `T` source bytes unread.

The initializer never loads header `+0x28` (the serialized bulk offset); it consumes the reader sequentially. Therefore the source position after step 4 is `Nmain+4*Nf`. The corpus scanner now records this independently derived position and compares it with the serialized bulk offset. It also checks the resulting bulk and trailer extent. See the [block-table census](block-table-audit/census.json) and its [analysis](block-table-audit/ANALYSIS.md).

For all 2,901 recovered BDAEs, the inferred source layout aligns with the declared fields: the main image is at least 60 bytes; `Nmain+4*Nf` equals header `+0x28`; row-table bytes plus row payload lengths equal `B`; and bulk plus trailer ends at `L`. This evidence ties the observed sequential reader operations to the stored block-table offsets for this corpus. It does not establish behavior for short reads: the original ignores returned read counts.

## Temporary `File` context

At `0x0069aaa8`, the reader initializer calls the no-argument `Init()` with a 44-byte temporary state record at `sp+0x14`. The field offsets follow the stores before and after that call:

| Temporary offset | Value and evidence |
| ---: | --- |
| `+0x00` | Main-image pointer (`fp`); passed as the record's first word and later copied to the persistent `File` object. |
| `+0x04` | Success byte, cleared before `Init()` and set from its zero/nonzero return afterward. |
| `+0x08` | Separate fixup-array pointer (`sl`); the no-argument initializer uses it to enter its non-null-context branch. The array is deleted and this persistent field is cleared after the call. |
| `+0x0c` | Declared image length, copied from image/header `+0x0c` by the no-argument initializer. |
| `+0x10` | Bulk byte count, copied from header `+0x2c`. |
| `+0x14` | Block count, copied from header `+0x30`. |
| `+0x18` | `L-B-T`, computed by the no-argument initializer; this is the source-space boundary before bulk and trailer. |
| `+0x1c` | Pointer to the copied 8-byte row table. |
| `+0x20` | Pointer to the array of block payload pointers. |
| `+0x24` | Boolean `header[+0x34] != 0`. |
| `+0x28` | Trailer byte count, copied from header `+0x38`. |

The record's `+0x1c` and `+0x20` tables are used by the alternate `File::Init()` relocation path. The same offsets are copied into the persistent object after the call. This maps the temporary state and producer-consumer relationship; it does not model the original C++ object ABI.

## Non-null relocation path: coordinate rules

The no-argument initializer's non-null branch uses the fixup pointer at context `+0x08`. Let `Nf` be the fixup count, `fixupBytes=4*Nf`, `S` the header reference selector at `+0x14`, and `mainEnd=file_size-bulk_size-trailer_size`. For the recovered selector-zero files, the ARM arithmetic and guards establish these mappings:

Before relocation, the function rejects a header already carrying flag `0x8000`, then sets that bit in the copied 16-bit flags word at image `+0x06`. The value-based helper mirrors this success-path marker in byte 7 of its copied little-endian header.

- Header fixup fields, whose source offsets are below 60, map to `imageBase + fieldOffset - S`.
- Main-image fields after the omitted fixup array map to `imageBase + fieldOffset - fixupBytes - S`. The ARM field comparison includes `mainEnd`; a field exactly there maps to one-past the compact image.
- A field inside payload row `j` maps to `blockBuffer[j] + fieldOffset - rowStart[j]`.
- A pointer target in copied main data maps to `imageBase + targetOffset - fixupBytes - S`. The `target > mainEnd` comparison is strict: a target exactly at `mainEnd` follows this main-image formula and becomes one-past the compact image.
- Targets past `mainEnd` use the block-route logic, which searches every row independently of the row that owns the fixup field. Main-image fields observed to point into payloads target exact row starts. The 5,171 payload-to-payload targets in the corpus happen to stay within their source row; cross-row target behavior is now represented by the independent row search.

The header contains five fixup locations at `+0x18`, `+0x1c`, `+0x20`, `+0x24`, and `+0x28` in every recovered file. The first (`+0x18`) initially contains `0x3c`; the reader branch replaces it with the separately allocated fixup-table pointer. The `+0x28` field targets `mainEnd`; for block-bearing files this numerically equals the block-table start, but the strict comparison maps it to one-past the compact image, not to the separately copied row table.

That same `+0x28` boundary occurs across the full cache: 1,505 targets equal the block-table start, 1,326 equal trailer start, and 70 equal EOF. All equal `mainEnd`. The reader body consumes no trailer bytes; all three cases follow the same one-past-image formula and bypass the global-slot path. What later consumes that boundary pointer is still unknown.

There is one important exception to ordinary field remapping: the ARM guard skips a fixup when its field offset is above `mainEnd` and `Nb >= ((fieldOffset-mainEnd-4) >> 3)`. This includes the row-table fields except its first word and can extend through three aligned words after the table. In the recovered corpus, the 23,648 skipped entries are exactly the rows' second words; their values remain source-file offsets for block-pointer construction and row search. A serialized fixup record is therefore not always a runtime pointer write. The [fixup-target audit](block-table-audit/fixup-target-audit/ANALYSIS.md) classifies raw offsets and separates them from applied remaps.

The ARM GOT references resolve to the exported globals `File::ExternalFilePtr` (`0x009f75d4`), `File::ExternalFileOffsetTableSize` (`0x009f75dc`), and `File::SizeOfHeader` (`0x009f75e4`). `Init()` writes the image pointer to the selected external slot, the boundary `60+4*Nf` to the slot's offset-size entry, and 60 to `SizeOfHeader`. The slot is selected by the high bit of header `+0x14`; all 2,901 recovered files use slot zero. Alternate-slot behavior and ownership are not corpus-verified.

## Block allocation selector

When `B > 0`, the initializer allocates the `Nb`-entry pointer table and `8*Nb`-byte row table and reads the rows. It stores `header[+0x34] != 0` in the context flag. At `0x0069aa08..0x0069aa14`, a zero flag branches to the aggregate path at `0x0069abf4`; nonzero falls through to the per-row loop.

- **Selector zero:** allocate `B-8*Nb` bytes once, read the payload region once, set pointer 0 to that base, and derive later pointers as `base + (row[i].offset-row[0].offset)`. All 2,901 recovered files use this branch; 1,505 have block rows.
- **Selector nonzero:** allocate and read `row[i].size` bytes separately for each row. This branch is present in the binary but absent from the recovered corpus.

The `B > 0` check gates block-table setup. Corpus files pair zero/nonzero `B` and `Nb`, but the initializer itself does not validate that relationship, so those observations are not a general input rule.

The separate reference selector at header `+0x14` controls a different path. When it is zero, the code reads a length-prefixed auxiliary name at header `+0x24`; it calls `CResFileManager::get(name, true)` only when that length exceeds one. Recovered files have length one (a NUL byte), so the manager lookup is skipped. Nonzero reference-selector behavior is not represented by this corpus.

## Ownership and remaining boundary

The main image is allocated through the observed `GlitchAlloc` thunk with hint `0x400`. The fixup array uses the array allocator and is released with array delete after `Init()`. The scratch header uses scalar delete. The row table, payload-pointer table, and block payload allocation(s) are retained in the `File` fields copied from the temporary record. For selector zero, later block pointers are interior pointers into one aggregate allocation; which pointer owns that allocation during destruction is not established by these two functions.

The no-argument initializer's null-context whole-buffer path is already ported. The value-based `dh2_bres_reader_context` helper sets the copied image's relocated flag, copies the fixup array and row table, and emits native-address pointer edges in a sidecar. For allocation selector zero it requires contiguous, nonempty aggregate payload data when rows exist; for a nonzero selector it copies each nonempty row into a separate caller-owned payload buffer and requires the row offsets to agree with the sequential read layout. It revalidates the view and capacities, models the ARM skip predicate, searches all rows for payload targets, and maps representable compact-image targets; it rejects destinations that cannot be represented safely in its copied storage. The sidecar reports skipped fixups with `applied == 0` and may contain the original one-past-image field address at `mainEnd`; it never writes host-width addresses into the four-byte serialized fields. The three observed global writes are metadata values. The helper accepts only reference selector zero and a one-byte NUL auxiliary name and does not mutate process globals. A skipped field outside the retained image/table/payload spans, unrepresentable fixup-table targets, and other unsupported field classes are rejected. No byte-backed or original-function differential has been run for this helper. Short-read behavior, allocation failures, downstream use of the one-past boundary, external/global targets beyond this corpus, zero-size row allocation semantics, and final buffer destruction remain unresolved. No build or test suite was run for this analysis.
