> Imported static research from engine branch `e6da25b`. Current implementation and native wiring are tracked in the [branch audit](../../../../docs/BRANCH-AUDIT-2026-10-05.md). The reader-context C++ candidate described below is not merged or behaviorally validated in this branch.

# BRES fixup targets in block-bearing assets

## Finding

Serialized fixup targets in the 1,505 block-bearing BDAEs reach the main-image interval, each block-table start, and every row payload. Within this block-bearing subset, none point into a trailer or beyond the file. Extending the same scan to all 2,901 BDAEs shows a boundary case: every header `+0x28` target equals `mainEnd`, which can be the block-table start, trailer start, or exact EOF depending on the resource.

The 2,245,356 ordinary fixups divide into:

| Serialized target region | Fixup entries | Share |
| --- | ---: | ---: |
| Main image, after the fixup table and before the bulk region | 2,191,384 | 97.5963% |
| Block-table rows | 1,505 | 0.0670% |
| Row payload bytes | 52,467 | 2.3367% |
| Trailer, exact end-of-file, or out of file | 0 | 0% |

The 1,505 raw targets classified in the block-table interval each come from header field `+0x28` and equal that file's `bulk_offset`, the first byte of its block table. Under the non-null initializer, however, `mainEnd=bulk_offset` and the target comparison is strict `>`. Equality therefore follows the main-image coordinate formula and maps to one-past the compact image; it does **not** become a pointer to the copied row table. The other 52,467 payload targets break down by the location of the fixup field:

| Fixup field is stored in | Targets into row payloads | Interpretation supported by bytes and original code |
| --- | ---: | --- |
| Block table | 23,648 | Serialized row-offset fixup entries target their own payload starts; context `Init()` skips them and leaves the offsets raw. |
| Main image | 23,648 | Exactly one main-image pointer targets each row payload. |
| Row payload | 5,171 | Every one targets a location in the same row as its source field; no cross-row payload references were found. |

Every one of the 23,648 payload rows has a serialized fixup entry for its row-table offset word and exactly one target from the main image. A further 5,171 serialized fixup fields inside payload bytes target within their own row. The distinction matters: the non-null `File::Init()` path has a block-table guard that skips the row-offset words, preserving those values as source-file offsets for the reader's block-pointer setup. This census counts serialized fixup records and target values; it does not imply that every entry is rewritten to a native pointer.

Every block-bearing file also has one special first fixup: field `+0x18` targets `0x3c` (decimal 60), the start of the fixup table. It is excluded from the ordinary-fixup table above. Including it gives 2,246,861 total fixup entries and adds 1,505 fixup-table targets. Among these 1,505 files, there are no targets to the header itself, exact EOF, or the trailer. This remains true even though 119 have nonzero trailer sizes.

## Boundary fixups across the full corpus

The scanner also covers all 2,901 files. Its 2,577,206 raw target values classify as follows:

| Serialized target interval | Entries | Source field / runtime interpretation |
| --- | ---: | --- |
| Fixup table | 2,901 | One per file: header `+0x18` → `0x3c`; the reader replaces this header field with its allocated fixup-array pointer. |
| Post-fixup main image | 2,518,937 | Includes ordinary main-image targets and the three other header pointers in each file. |
| Block-table interval | 1,505 | Header `+0x28` equals `mainEnd=bulk_offset`; strict `>` keeps equality on the main-image path, producing a one-past-image pointer. |
| Row payloads | 52,467 | 47,296 exact row starts and 5,171 same-row interior targets. |
| Trailer interval | 1,326 | All are header `+0x28`; each equals `mainEnd`, the trailer start, and takes the same one-past-image route. The reader does not copy trailer bytes. |
| Exact EOF | 70 | All are header `+0x28`; each equals `mainEnd=file_size` and takes the same one-past-image route. |

Every file has five header fixup locations: `+0x18`, `+0x1c`, `+0x20`, `+0x24`, and `+0x28`. The block-bearing subset has 23,648 fixup records whose field is a row's second word. For any field above `mainEnd`, the context branch's arithmetic guard computes `(field-mainEnd-4)>>3`, compares it with block count, and skips when block count is greater or equal. This guard can cover aligned fields after the row table too, but the corpus exercises it only on the row-offset words, which remain raw file offsets for block-pointer setup. The scanner counts serialized entries; the table above distinguishes their input values from the pointer mapping performed by the loader.

## Address and function evidence

The resource initializer evidence is the APK-matched ARM listing in `../../reference/engine-branch-original-functions.asm`:

| Function | ELF VA | Bytes | SHA-256 |
| --- | ---: | ---: | --- |
| `glitch::res::File::Init()` | `0x0069a40c` | 1,108 | `e36600c917439651bfec9dedadfdf85537210401a6c00e0a5f96617e7f50a05a` |
| `glitch::res::File::Init(glitch::res::FileReader*)` | `0x0069a880` | 1,036 | `2c8604d4442f9c5f8caace3a10a8b12c4f250614c125eca8c841a923e49aa495` |

The no-context initializer adds the image base to each listed pointer field after its first, special entry. Its context branch uses an 8-byte-stride block-row table, reads the row's second word as a file-offset boundary, selects the matching row, and combines that row's backing buffer with the file-relative offset. A separate exact trace establishes that row-offset fields are skipped by the table guard and that header `+0x28`'s target at `mainEnd` follows the main-image coordinate formula, becoming one-past the compact image rather than the separately copied row-table pointer. The reader overload at `0x0069a880` builds and passes this non-null context. These instructions establish the remap mechanism; the census below establishes serialized target intervals. Exact reader allocation ownership and runtime success remain separate questions.

The source APK SHA-256 is `32c2d027b585a42547311cd95da6a3975fdb3174e663e513d42a7f49d1a4c200`; its ARM ELF SHA-256 is `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

## Representative files

### Small file with all four payload rows referenced

`data/3d/animateddecors/candle_flame.bdae` is 16,856 bytes, SHA-256 `7ece047e6cb253036d50260018fd7299ad9835e743ca0fcd4e2d3004ea0269a6`, with 647 fixups and four rows. Its block table occupies `[0x4090, 0x40b0)` and the payload interval is `[0x40b0, 0x41d8)`. The four rows start at `0x40b0`, `0x4140`, `0x414c`, and `0x41cc`; each receives two targets, one from its row's offset word and one from the main image. The header field at `0x28` targets the table start `0x4090`; the special first field at `0x18` targets `0x3c`.

### File with self-contained payload pointers

`data/3d/animateddecors/castle/biblio_throneroom.bdae` is 57,900 bytes, SHA-256 `7f5edc1be8aa32da7bd110a1fb50ac7dbe56161eb2b495080af9ada59b333f3b`, with 563 fixups and nine rows. It has 20 targets into row payloads: nine table offset words, nine main-image fields, and two fields inside payloads. Those last two target their own source rows.

### Large file across many rows

`data/3d/modules/darkwood/darkwood2.bdae` is 5,714,348 bytes, SHA-256 `8b51825a87a8c7543ec3c6a62881aa09add3b424a7c77d6a458e008a43d3d134`, with 171,431 fixups and 1,955 rows. Its 3,910 payload targets are exactly one from each row's block-table offset field and one from the main image for each payload. It has no same-row internal payload pointers.

## Method and limits

The reproducible [scanner](scan_fixup_targets.py) reads the extracted cache and the existing [block-table census](../census.json). It hash-matches all 2,901 files and verifies every raw block row in the 1,505 block-bearing files before reading fixup tables. The source-file index hash from that census is `b641aa2c9911c1fda66a1e7c02125e9779eb957d89397fe7b082436f21f3f4b7`.

Each fixup-table entry is treated as the offset of a four-byte field; the value at that field is classified against these serialized file ranges: header `[0, 60)`, fixup table `[fixup_offset, after_fixups)`, main image `[after_fixups, bulk_offset)`, block table `[bulk_offset, bulk_offset + 8 * block_count)`, each row payload `[row_offset, row_offset + row_size)`, trailer `[bulk_offset + bulk_size, file_size)`, and exact EOF. This naming describes positions in the recovered file. It does not assert that every entry is dereferenced, that offsets have already been translated to native pointers, or that original initialization succeeded.

The block-bearing corpus totals 2,246,861 fixups. Its non-first denominator is 2,245,356 after excluding exactly one `field=0x18 -> target=0x3c` entry in each file. The full-corpus denominator is 2,574,305 after excluding one such entry per file. Extracted file bytes and block rows were hash-matched to the existing census; the APK and ELF identities above preserve binary provenance. No source assets were edited, and no build or test was run.

The distribution is corpus-specific: the existing census reports allocation selector `+0x34` as zero, so this does not cover alternate selector modes or external-library resolution. Static file-offset mapping also does not replace a differential execution of `File::Init(FileReader*)`.
