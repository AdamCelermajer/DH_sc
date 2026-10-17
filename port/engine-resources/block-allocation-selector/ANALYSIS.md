> Imported static research from engine branch `e6da25b`. Current implementation and native wiring are tracked in the [branch audit](../../../docs/BRANCH-AUDIT-2026-10-05.md). The reader-context C++ candidate described below is not merged or behaviorally validated in this branch.

# BRES block allocation selector

## Finding

Header word `+0x34` selects how `File::Init(FileReader*)` materializes the bulk payload rows after it reads their 8-byte table. The reader stores only whether the word is zero in its temporary context at `+0x24`.

- When `+0x34 == 0`, it allocates `bulk_size - 8*block_count` bytes once, reads the payload area in one operation, and derives each later row pointer from its file-offset delta from row 0.
- When `+0x34 != 0`, it iterates rows in index order, allocates `row[i].size` bytes for each row, stores that allocation in the pointer table, and reads that row into its own buffer.

The second mode is present in the original binary but absent from all recovered BDAEs. The value-based helper now represents its row payloads through distinct caller-owned buffers while retaining separate row pointers in the emitted context. This models the address layout used by pointer edges, but does not perform the original allocations or establish ownership/destruction.

## APK provenance

Input APK SHA-256: `32c2d027b585a42547311cd95da6a3975fdb3174e663e513d42a7f49d1a4c200`.

Input library SHA-256: `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

| Function | ELF VA | Size | Function SHA-256 |
| --- | ---: | ---: | --- |
| `glitch::res::File::Init(glitch::res::FileReader*)` | `0x0069a880` | 1,036 | `2c8604d4442f9c5f8caace3a10a8b12c4f250614c125eca8c841a923e49aa495` |
| `glitch::res::File::Init()` | `0x0069a40c` | 1,108 | `e36600c917439651bfec9dedadfdf85537210401a6c00e0a5f96617e7f50a05a` |

Both complete instruction ranges are in [`reference/engine-branch-original-functions.asm`](../reference/engine-branch-original-functions.asm). Their address, size, and hashes also appear in [`external-split-functions.json`](../external-split-functions.json). The file-backed ELF load segment maps file offset to VA directly for these functions.

## Selector control flow

The `FileReader*` overload reads header `+0x34` at `0x0069a944`. At `0x0069a950–0x0069a958` it converts the full word to a byte flag (`0` stays zero; any nonzero value becomes `1`) in the temporary context at `+0x24`. This proves that the branch distinguishes zero from nonzero, not particular nonzero selector values.

After the fixup array and compact main image are read, a positive bulk byte count gates allocation of the block-pointer table (`4*block_count` bytes), the row table (`8*block_count` bytes), and one read of that row table. At `0x0069aa08–0x0069aa10`, the saved boolean selects the payload path:

### Nonzero selector: one allocation and read per row

The nonzero path starts at `0x0069aa14`. If the row count is positive, the loop starts with index zero at `0x0069aa20` and repeats through the row count. For each row it:

1. Loads the row's first word (the requested byte count) from the 8-byte table.
2. Calls the observed allocation routine with that count.
3. Stores the returned pointer at `block_buffers[i]`.
4. Calls `FileReader::read(block_buffers[i], row_size)` through vtable offset `+0x0c`.

The routine does not compare the reader's returned byte count before continuing. This is an observed unchecked-read behavior, not a guarantee of complete reads.

### Zero selector: one aggregate allocation and read

The zero branch begins at `0x0069abf4`. It calculates `bulk_size - 8*block_count`, allocates that many payload bytes, stores the base at `block_buffers[0]`, and reads the aggregate payload region once. When there is more than one row, the loop beginning at `0x0069ac3c` derives each later pointer as:

```text
block_buffers[i] = block_buffers[0]
                 + row[i].file_offset - row[0].file_offset
```

The recovered corpus confirms this mode's data is contiguous in file order; the original branch itself does not validate row contiguity before applying the offset difference.

## Context consumer and reconstruction boundary

The reader passes the block-pointer table and row table to the non-null `File::Init()` context at `0x0069aaa8`. Its fixup path uses each row's recorded file offset to choose a payload buffer and translate field/target addresses. Thus the per-row mode gives the consumer independently allocated row bases; the aggregate mode gives bases into one allocation. The `dh2_bres_reader_context` helper in `resources.cpp` represents the aggregate mode with one caller-owned arena and the nonzero mode with one caller-owned byte buffer per row. The latter copies each row from its recorded source offset; target lookup independently searches every row, as in the ARM path. Because the original reader consumes those row bytes sequentially but later fixup mapping uses recorded file offsets, the bounded helper requires each recorded row offset to match the sequential read position. It does not allocate or free these buffers like the original `FileReader` producer.

The recovered corpus census records `+0x34 == 0` in all 2,901 BDAEs, including all 1,505 files with block rows and 23,648 rows. See the [bulk-block census](../block-table-audit/census.json) and [reader-context trace](../READER-CONTEXT-RECONSTRUCTION.md).

## Limits

- No recovered file exercises the nonzero selector, so row-size zero behavior, malformed row handling, and allocator failure behavior are not corpus-verified.
- The bounded helper rejects nonzero-mode rows whose stored offsets do not match the sequential read layout; the original function does not explicitly validate that equality.
- The function ignores `read()` return values for payload reads; short-read outcomes and subsequent state are not validated here.
- These functions show allocation and pointer setup, but do not establish which allocation owns aggregate data during destruction or how independently allocated rows are released.
- The helper's per-row mode has not been built or differentially compared with original ARM execution. Its rejection of zero-size rows is a bounded-port choice because allocator behavior for size zero is not established here.
