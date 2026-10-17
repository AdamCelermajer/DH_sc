> Imported static research from engine branch `e6da25b`. Current implementation and native wiring are tracked in the [branch audit](../../docs/BRANCH-AUDIT-2026-10-05.md). The reader-context C++ candidate described below is not merged or behaviorally validated in this branch.

# BRES external and split initialization analysis

## Decision

The reader producer, temporary object fields, both payload-allocation paths, and the alternate initializer's coordinate rules are now traced. The recovered corpus exercises the non-null path statically, and a byte-backed census shows which raw field/target offsets it reaches. A bounded value-based helper represents aggregate payloads and separate per-row buffers and emits pointer edges, but it has no original-ARM differential comparison and does not reproduce the `FileReader` virtual-call producer or original ownership. The [reference-selector analysis](REFERENCE-SELECTOR-ANALYSIS.md) maps the optional auxiliary-name lookup and cache-hit global updates; no recovered asset exercises those branches.

## Exact binary evidence

The reference library is `libDungeonHunter2.so`, SHA-256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80` (recorded in `original-functions.json` and the validation reports).

| Function/range | ELF virtual address | Size | SHA-256 |
| --- | ---: | ---: | --- |
| `glitch::res::File::Init()` | `0x0069a40c` | 1,108 bytes | `e36600c917439651bfec9dedadfdf85537210401a6c00e0a5f96617e7f50a05a` |
| `glitch::res::File::Init(glitch::res::FileReader*)` | `0x0069a880` | 1,036 bytes | `2c8604d4442f9c5f8caace3a10a8b12c4f250614c125eca8c841a923e49aa495` |
| `glitch::collada::CResFileManager::get(char const*, bool)` | `0x0065a9a8` | 692 bytes | `62a918d472da5807ba14390942d578cd99130a7929117f900cb0bbc0b5060854` |
| Assembly listing containing both ranges | — | — | `53677ce0dbce044fede72b2165857f9bb145217d06532f52d07075f71b47fac5` |

Function hashes are over the instruction/literal bytes represented by the address ranges in the original recovery listings. The two `File::Init` listings are copied into `reference/engine-branch-original-functions.asm`. The manager lookup is an external dependency: its exact range/hash is indexed in `external-split-functions.json`, and its listing remains in the sibling recovery workspace at `../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_collada_CResFileManager-ac8fc0a26e72-001.asm` (path relative to the `DH_sc` repository root).

## Branch and caller contract

`Init(FileReader*)` builds a temporary 44-byte `File` state record at `sp+0x14`, then calls `Init()` at `0x0069aaa8`. The temporary object's `+0x08` points to the separately copied fixup table, which selects the non-null path. The null-context branch at `0x0069a804` is already ported; the non-null ARM branch begins at `0x0069a4c8`. The port-side `dh2_bres_reader_context` supports the observed selector-zero reference layout with either allocation mode and emits native pointer edges separately, without writing host-width addresses into four-byte serialized fields.

```asm
0069a4b8  b6 c0 c3 e1  strh ip, [r3, #6]
0069a4bc  08 c0 90 e5  ldr ip, [r0, #8]
0069a4c0  00 00 5c e3  cmp ip, #0
0069a4c4  ce 00 00 0a  beq #0x69a804
0069a4c8  88 43 9f e5  ldr r4, [pc, #0x388]
0069a4cc  88 53 9f e5  ldr r5, [pc, #0x388]
```

The context fields, stream partition, table allocations, aggregate-versus-per-row payload bases, and ARM global indirections are documented in [reader-context reconstruction](READER-CONTEXT-RECONSTRUCTION.md). Key findings: the copied image omits the fixup array; main-data addresses subtract `4*fixup_count`; block rows use `{byte_count,file_offset}`; header `+0x28` maps to one-past the compact image even though it numerically equals the block-table start; and row offset words are skipped by a table guard and stay as offsets. The [reference-selector analysis](REFERENCE-SELECTOR-ANALYSIS.md) distinguishes the reader's full-word zero test from `S >> 31` slot selection, and records the external manager's cache-hit writes. The high bit chooses one of two global slots (`ExternalFilePtr`, `ExternalFileOffsetTableSize`), while `SizeOfHeader` is shared.

`Init(FileReader*)` is the producer for this context. It reads a 60-byte header, allocates and reads the fixup table, uses `+44` and `+48` to size its block-related allocations, reads the 8-byte block records, and uses `+52` to select another setup branch. It then builds a stack context containing the image, fixup table, and block tables and calls `File::Init()` at `0x0069aaa8`. Every recovered file has fixups (2,577,206 total), so successful reader initialization requests a nonzero fixup-table allocation and passes its pointer in the context.

When header `+20` is zero, the reader seeks to the section referenced by `+36` and reads its length word. The manager lookup at `0x0065a9a8` happens only when that length exceeds one. Every recovered file has length one there, so this optional name-lookup route is skipped by the corpus. The manager function is included as an external dependency for longer names; it is not called by the recorded corpus cases.

The binary exports these shared fields:

- `glitch::res::File::ExternalFilePtr` at `0x009f75d4`, size 8 bytes.
- `glitch::res::File::ExternalFileOffsetTableSize` at `0x009f75dc`, size 8 bytes.
- `glitch::res::File::SizeOfHeader` at `0x009f75e4`, size 4 bytes.

The base `glitch::io::IReadFile::getExternalBuffer(char const*, long*)` at `0x0056eb38` returns null. The current symbol index identifies no override. That optional virtual does not resolve the distinct manager lookup or explain the lifetime and ownership of the external-file slots.

## Recovered-cache coverage

A read-only header census of all 2,901 recovered `.bdae` files found:

- Header `+20` is zero in all 2,901 files.
- Header `+52` is zero in all 2,901 files.
- Header `+44` (bulk byte size) and `+48` (block count) are both zero in 1,396 files and both nonzero in 1,505 files.
- The word at each file's auxiliary offset (`header[+36]`) is 1 in all 2,901 files. Sample records contain a one-byte NUL payload, and the exact reader code skips manager lookup for lengths of one or less.

The cached whole-buffer differential runs verify the null-context path only. The reader initializer and a [fixup-target census](block-table-audit/fixup-target-audit/ANALYSIS.md) establish the static alternate-path inputs for the recovered files; no differential report compares the new helper's copied context and relocation edges against the original. Nonzero reference selectors and nonempty external-name lookup remain unrepresented. The helper models nonzero allocation-selector rows only when their stored offsets match the sequential read positions; that mode is absent from the corpus. Across all 2,901 files, 1,326 header `+0x28` targets equal trailer start and 70 equal EOF; all equal `mainEnd` and follow the one-past compact-image formula because the branch tests strict `>`. The reader does not copy a tail buffer, and later use of this boundary pointer remains unknown.

## Evidence needed for parity and broader support

Compare the value-based helper's output against original ARM execution through `Init(FileReader*)` for block-bearing and no-block files, and reconstruct the exact reader producer including short reads and allocation ownership. Separately capture a nonempty auxiliary-name fixture and a high-bit reference-slot fixture, mapping each consumed word to its producer and backing span. The nonzero `+0x34` mode also needs an original-function differential despite the bounded port. Those cases are needed before claiming the full initializer or original ownership behavior.
