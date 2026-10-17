> Imported static research from engine branch `e6da25b`. Current implementation and native wiring are tracked in the [branch audit](../../../docs/BRANCH-AUDIT-2026-10-05.md). The reader-context C++ candidate described below is not merged or behaviorally validated in this branch.

# BRES bulk-block table route

## Finding

The original resource initializer has a block-backed `FileReader` path in addition to its whole-buffer relocation path. In the recovered BDAE corpus, the 60-byte header's bulk region at `+0x28/+0x2c` (offset and byte length) contains an array of 8-byte rows followed by the row payloads. Each row is `{payload_byte_count, payload_file_offset}`. The table and payloads account for the complete declared bulk span in every block-bearing corpus file.

This identifies and exposes the bounded block-row view. The original `File::Init(FileReader*)` allocation branches are now traced in the [allocation-selector audit](../block-allocation-selector/ANALYSIS.md), but neither is recreated here. Context-dependent pointer relocation, external-library lookup, and original ownership are also outside this block-view helper.

## APK evidence

The source APK SHA-256 is `32c2d027b585a42547311cd95da6a3975fdb3174e663e513d42a7f49d1a4c200`; the `lib/armeabi-v7a/libDungeonHunter2.so` SHA-256 is `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

| Function | ELF VA | Bytes | SHA-256 |
| --- | ---: | ---: | --- |
| `glitch::res::File::Init()` | `0x0069a40c` | 1,108 | `e36600c917439651bfec9dedadfdf85537210401a6c00e0a5f96617e7f50a05a` |
| `glitch::res::File::Init(glitch::res::FileReader*)` | `0x0069a880` | 1,036 | `2c8604d4442f9c5f8caace3a10a8b12c4f250614c125eca8c841a923e49aa495` |

Both ranges are in `port/engine-resources/reference/engine-branch-original-functions.asm`. The first function branches on the supplied relocation context at object `+0x08`. The reader overload creates that context and calls it at `0x0069aaa8`.

`File::Init(FileReader*)` obtains the source length through the reader vtable, reads the 60-byte header, and stores the header's bulk byte count (`+0x2c`), block count (`+0x30`), and allocation selector (`+0x34`) in its runtime object. Its main-image size arithmetic subtracts fixup-table bytes, bulk bytes, and trailing bytes from the source length. It then reads the block table and uses each row's first word to allocate a buffer and request that many bytes from the reader. The subsequent non-null-context `File::Init()` path consults the same row's second word while remapping fixup targets into the corresponding block buffer. These are direct field-use observations; the names above describe only the behavior evidenced by the code and corpus.

The reader code does **not** load header `+0x28` (bulk file offset); it consumes the stream sequentially. It seeks to byte 60, reads the `4*fixup_count` table into a separate allocation, then reads `Nmain-60` bytes into the image after its copied header, where `Nmain = file_size - 4*fixup_count - bulk_size - trailer_size`. The resulting source position is `Nmain + 4*fixup_count`. The updated [census](census.json) verifies that this equals header `+0x28` in all 2,901 files, and that the following bulk span plus trailer ends at the declared file size. This ties the code's sequential reader path to the stored offsets for the recovered corpus without asserting that the initializer checks those equalities.

For selector zero, which is the only value in the corpus, the code allocates one payload buffer of `bulk_size - 8*block_count` bytes, reads it once, and derives later block pointers from the row-offset deltas. A nonzero selector instead allocates and reads each row separately; that path is present in the binary but unexercised by these assets. Block setup is gated by `bulk_size > 0`, not by a check that the row count agrees with it. The exact temporary context and full sequential read layout are recorded in [reader-context reconstruction](../READER-CONTEXT-RECONSTRUCTION.md).

## Corpus census

The reproducible [scanner](../tools/census_bres_block_tables.py) reads the extracted cache at `work/cache-recovery/extracted/com.gameloft.android.GAND.GloftD2SS/files`; it does not modify those assets. The complete file records and raw rows are in [census.json](census.json).

| Check | Result |
| --- | ---: |
| BDAE files / bytes | 2,901 / 143,565,008 |
| Header, fixup-end, table-span checks | 2,901 valid; zero errors |
| Reader image is at least the copied 60-byte header | 2,901 / 2,901 |
| Reader-derived bulk start equals serialized `+0x28` | 2,901 / 2,901 |
| Reader-derived bulk and trailer extent ends at file size | 2,901 / 2,901 |
| Files with block rows / rows | 1,505 / 23,648 |
| Files without block rows | 1,396 |
| Header allocation selector `+0x34` | zero in all 2,901 |
| Block rows whose size sum plus 8-byte row headers equals `bulk_size` | 1,505 of 1,505 |
| Rows that tile contiguously from the end of the table and fit within the bulk span | 1,505 of 1,505 |
| `bulk_offset + bulk_size + trailer_size == file_size` | 2,901 of 2,901 |

For example, `data/3d/animateddecors/candle_flame.bdae` (16,856 bytes; SHA-256 `7ece047e6cb253036d50260018fd7299ad9835e743ca0fcd4e2d3004ea0269a6`) has bulk offset `0x4090`, bulk size `0x148`, and four rows:

```text
{0x90, 0x40b0}, {0x0c, 0x4140}, {0x80, 0x414c}, {0x0c, 0x41cc}
```

The row table occupies `4 * 8 = 0x20` bytes. Its payload lengths sum to `0x128`; table plus payload bytes equal the declared `0x148`. The recorded payload offsets are contiguous and end at the file end. Across the corpus, each second word is the absolute file position used for the row's payload bytes. All extracted files were checked by header extent and individually recorded size, CRC-32, and SHA-256; the deterministic source-file index hash is `b641aa2c9911c1fda66a1e7c02125e9779eb957d89397fe7b082436f21f3f4b7`.

## Bounded source view

`dh2_bres_block` in `resources.hpp`/`resources.cpp` returns a borrowed `BresBlock` for one row. It checks the row index, selector mode, header/count agreement, bulk table size, row offset, payload range, and trailer bound before exposing pointers. It neither allocates nor decompresses/copies the payload and does not apply pointer fixups. The original object ABI remains unmodeled.

The corpus exercises allocation selector zero only. Header reference/base selector `+0x14` is also zero throughout this recovered set; its auxiliary name has length one in every file, so the optional manager lookup is skipped. The nonzero allocation-selector branch is mapped in the [selector audit](../block-allocation-selector/ANALYSIS.md) and represented by the caller-owned per-row path in `dh2_bres_reader_context`, but its behavior has no asset-backed or differential confirmation. The [reference-selector audit](../REFERENCE-SELECTOR-ANALYSIS.md) maps the nonempty-name manager call and cache-hit global writes; neither path is corpus-exercised. Behavior for malformed block tables and whole-buffer-versus-reader caller selection remain open. No builds or tests were run for this addition; evidence is the APK-matched ARM listing plus the read-only corpus census.
