# Checked integer PyData constants and Lua bridge

This source component reads little-endian group count; length/name and entry
count per group; then length/name and int32 value per entry. Its owned Lua
importer installs `GetPyCst`. Array IDs, structured records and objects are open.

## Original reader evidence

[Original ARM32 trace](../../reports/pydata-constant-reader-trace.json) executes
the actual `PyDataConstants::reloadData`, primitive reads and string reads on
27 owner-cache files. Virtual stream reads use a bounded byte-source model.
Two map insertion boundaries capture keys and subsequently stored integer
values. Logging, diagnostic strings and stack guard setup are explicit stubs.
Original map allocation/lookup and scripts do not execute.

26 files are fully consumed, yielding **5,608** entries. The 1,283-byte
`sounds_pycst.bin` is consumed only through byte 994, emitting 27 partial entries.
The reader misreads string-valued `SoundBus/Amb_loop_omni` as integer 13, then
stops at an oversized following name. The source rejects this whole mixed file;
it does not reproduce the partial/misread import or claim complete sound data.

[Source corpus evidence](corpus-validation.json) compares all complete entries
with compiled ARM64/host decoding: counts, names and integers match the recorded
original writes. 420 source lookups, immutable inputs and output guards pass.
Lookups are checked against records, not execution of the original map lookup.
ARM64 libc byte search/comparison uses a bounded dependency model.

## Ownership and bounds

C views borrow immutable bytes. Files require exact consumption, at most 16 MiB
and names up to 255 bytes without NUL. Invalid counts/lengths, trailing bytes,
corrupted views and output aliasing preserve outputs on rejection. Batch
enumeration validates before writing. Views use one thread; buffers stay
unchanged during calls. These bounds and ownership rules are authored repairs.

The Lua importer validates, clones previous mappings into staging tables, merges
and commits after all allocations succeed. Lua owns names and values within its
memory cap. Caller buffers can be released. Malformed input and allocation
failure retain previous mappings. Duplicate keys use the last value; there are
no duplicate keys across the 26 checked files.

`GetPyCst` needs two strings, ignores trailing arguments, returns no values for
wrong count/type and numeric zero for unknown names. Input strings use the first
NUL as the C-string boundary. Integers use the float32 Lua number profile. Full
original binding/type equivalence and engine object ownership are unfinished.

## Runtime checks

All **5,608 authored Lua queries** pass after releasing caller buffers.
Selftests cover duplicate keys, unknown names, wrong types/count, malformed
imports, allocation-failure rollback and non-string Lua errors. Strict host
ASan/UBSan/float-cast checks pass with immediate failure on errors.
**No original game script is executed.**

- [Host queries](../lua-runtime/constants-host-data-validation.json)
- [Host sanitizer corpus](../lua-runtime/constants-host-sanitizer-corpus-validation.json)
- [Android 17 / 4 KiB](../lua-runtime/constants-android-4k-data-validation.json)
- [Android 17 / 16 KiB](../lua-runtime/constants-android-16k-data-validation.json)
- [12,000 C safety iterations](host-build-validation.json)
- [ARM64 build / 16 KiB alignment](arm64-build-validation.json)

Both Android runs use the exact source x86_64 standalone runtime and verify all
30 staged file/list/assertion/runner hashes. The 220-input script parse corpus
and 504 arithmetic vectors pass too. ARM64 is built, without runtime testing.
No Fold7 or physical device is used. This runtime is not packaged in the source
preview APK yet; full source-built gameplay remains unfinished.

## Reproduce

```sh
python3 port/pydata-constants/build.py --host --report /path/to/host.json
python3 port/pydata-constants/build.py --ndk /path/to/windows-ndk --report /path/to/arm64.json
python3 tools/trace_pydata_constants.py --help
python3 port/pydata-constants/tests/corpus.py --help
python3 port/lua-runtime/tests/constants_corpus.py --help
```

Original tracing needs the owner's library, Unicorn/pyelftools and the existing
host oracle. Ordinary source builds need none of those proprietary inputs.
Cache tests use owner files; the trace records exact hashes and decoded rows.
The repository [rights statement](../../RIGHTS.md) applies to recovered data.
