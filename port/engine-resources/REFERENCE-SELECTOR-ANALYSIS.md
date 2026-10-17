> Imported static research from engine branch `e6da25b`. Current implementation and native wiring are tracked in the [branch audit](../../docs/BRANCH-AUDIT-2026-10-05.md). The reader-context C++ candidate described below is not merged or behaviorally validated in this branch.

# BRES reference selector and auxiliary-name lookup

## Provenance

The trace is from the supplied `libDungeonHunter2.so`, SHA-256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Function hashes below cover the ARM instruction and literal bytes in each ELF
range. The original listings are in the recovery assembly files referenced by
`external-split-functions.json`.

| Symbol | ELF range | Size | ARM-byte SHA-256 |
| --- | --- | ---: | --- |
| `glitch::res::File::Init()` | `0x0069a40c..0x0069a85f` | 1,108 | `e36600c917439651bfec9dedadfdf85537210401a6c00e0a5f96617e7f50a05a` |
| `glitch::res::File::Init(glitch::res::FileReader*)` | `0x0069a880..0x0069ac8b` | 1,036 | `2c8604d4442f9c5f8caace3a10a8b12c4f250614c125eca8c841a923e49aa495` |
| `glitch::collada::CResFileManager::get(char const*, bool)` | `0x0065a9a8..0x0065ac5b` | 692 | `62a918d472da5807ba14390942d578cd99130a7929117f900cb0bbc0b5060854` |

The shared globals are `glitch::res::File::ExternalFilePtr` at
`0x009f75d4` (8 bytes), `ExternalFileOffsetTableSize` at `0x009f75dc` (8
bytes), and `SizeOfHeader` at `0x009f75e4` (4 bytes). Their addresses and
symbols are also recorded in the ELF symbol index and the existing external
split analysis.

## Header fields and selector branches

Let `S = header[0x14]` (decimal byte offset 20) and
`A = header[0x24]` (decimal byte offset 36). `S` is a full 32-bit value; it is
not only a boolean or a single selector bit.

In `File::Init(FileReader*)`, instructions `0x0069a8e8..0x0069a8f4` load `S`
and `A`, then compare the *whole* `S` against zero. Any nonzero `S` skips the
auxiliary-name path and continues at the ordinary seek to byte `0x3c`. Only
`S == 0` reaches `0x0069ab58`:

1. It calls the reader's virtual `seek(A, S)`; on this branch `S` is zero.
2. It reads four bytes into a local 32-bit length at `sp+0x40`.
3. `cmp length,#1; ble` uses the signed ARM condition. Lengths 0, 1, and
   values with the sign bit set skip the lookup.
4. For signed lengths above one, it seeks to `A+4`, reads
   `(length+3)&~3` bytes into `sp+0x44`, and calls
   `CResFileManager::get(name, true)` at `0x0069abec`.
5. The return value is not saved or checked; execution resumes at the normal
   header read at `0x0069a8f8`.

No terminator is appended. The one-byte corpus payload is NUL, which is
consistent with the length covering the terminator for that case. For longer
names, the code passes a `char const*` to the manager, so a NUL within the
readable payload is required for bounded interpretation, but the reader does
not validate it.

The name buffer begins at `sp+0x44`. The function reserves `0x14c` stack
bytes, leaving `0x108` (264) bytes from that start to the end of its frame.
The code does not compare the rounded read size with 264, validate
`A+4+round_up(length,4)` against the reader size with checked arithmetic, or
check any seek/read result. Thus a
large length can write past the local buffer, and a short read can leave
uninitialized bytes for the manager's C-string scan. Those are properties of
the recovered routine, not behaviors a safe port should reproduce.

In `File::Init()`, the full `S` is subtracted from serialized fixup fields and
targets, while `S >> 31` selects slot 0 or 1. At `0x0069a45c..0x0069a460`,
the current image base is written to `ExternalFilePtr[S >> 31]`. This is
separate from the reader's full-zero test: any nonzero low-bit value skips the
name path even when its high bit is clear.

## Manager behavior and side effects

The name path calls `CResFileManager::get(name, true)`, not
`IReadFile::getExternalBuffer`. The base `IReadFile::getExternalBuffer` at
`0x0056eb38` returns null, and the recovered symbol index contains no
override. The manager lookup uses its resource cache; on a miss, the true flag
allows it to obtain a reader and construct/cache a `CResFile`. The caller
discards the returned `CResFile*`, so the lookup's global writes are relevant
to subsequent fixups.

On a cache hit, `CResFileManager::get` reads the resolved image base from its
`CResFile` record and performs these stores (instructions
`0x0065aa7c..0x0065aac4`):

```text
R = resolved image base
T = resolved header[0x14]
ExternalFilePtr[T >> 31] = R
ExternalFileOffsetTableSize[T >> 31] = old SizeOfHeader + 4 * resolved header[0x10]
SizeOfHeader = resolved header[0x08]
```

The `old SizeOfHeader` operand is loaded before the manager stores the
resolved header's `+0x08` value. This ordering is explicit in the instructions;
the corpus does not exercise it. On a cache miss, `true` enables a separate
path that attempts to obtain a reader and, on success, constructs/registers a
`CResFile` and invokes additional manager helpers; it does not flow through
this direct cache-hit store block. Whether those helpers cause the same global
writes indirectly has not been established here. The current file's later `File::Init()` writes
its own image and metadata to the slot selected by its own `S`.

The manager routine's direct cache-hit path updates the globals. Its load-miss
path, failure behavior, string normalization, manager cache lifetime, and
eventual resource destruction are outside the `File::Init(FileReader*)`
caller and have not been differentially validated for BRES names.

## Corpus evidence and limits

The recovered set contains 2,901 `.bdae` files. All have `header[0x14] == 0`,
and all 2,901 auxiliary sections have length one with a single NUL byte. Thus
none invokes `CResFileManager::get`; the manager's name-load route and every
nonzero-`S` case are established from static ELF instructions only, not from a
corpus execution or differential comparison. The corpus does not prove the
global-slot ordering above is correct for a real named external resource.

The evidence supports a safe bounded value port if it takes an explicit
resource resolver and two caller-owned external slots. Such a port can reject
truncated sections, oversized names, missing in-range NUL terminators, and
unknown selector cases. A faithful port of the original manager/cache
ownership and its global mutation order is not yet established: there is no
nonempty-name fixture, no nonzero-selector fixture, and no differential run
through this path. The recovered implementation itself does not impose the
bounds needed for a safe value API.
