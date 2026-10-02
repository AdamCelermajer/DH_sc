# Ordered array names and source GetPyOID

This module reads one count/length/name table and supplies its first matching
zero-based ID. Unknown names return -1. Names remain ordered; duplicates select
the first entry. The owned Lua importer installs `GetPyOID` for imported classes.
It does not implement array records, structured fields or game objects.

## Original evidence

[Original ARM32 trace](../../reports/pydata-array-name-trace.json) executes the
original `PyDataArrays` constructor caller. It captures 142 class registrations
(Arrays and Structs) and 142 file-reader registrations, including ordered readers
for files holding multiple tables. Registration installation is stubbed.

Actual original name readers decode **8,863 names in 71 array tables from 35
files**. Actual `GetMemberIDByString` bodies pass **266** first/middle/last/unknown
lookup cases. Primitive readers execute. Virtual stream reads, allocation,
finalization and `strcmp` use controlled bounded models. Class record sizes are
set to the name-file counts: array-record decoding/count agreement, original
ownership and Structs field-name loading are not established. Scripts do not run.

[Source corpus](corpus-validation.json) matches every decoded name/position on
ARM64 and host. ARM64 repeats the 266 original lookup cases; host additionally
checks every unique name (8,863). Inputs, output guards and restored stacks pass.
Source libc byte search/comparison uses an explicit bounded dependency model.

## Source ownership

C views borrow immutable bytes. One table must be fully consumed, with at most
16 MiB and names up to 255 bytes without NUL. Bad counts/lengths, trailing bytes,
corrupt views and output aliasing preserve output on rejection. Batch copying
validates before writing. These bounds are authored safety rules.

The Lua importer owns all class/member strings and IDs within its memory cap.
It stages a new class map and atomically replaces that class after allocations
succeed. Other class maps are immutable and retained. Malformed input and OOM
preserve prior IDs. The callback needs two strings, ignores trailing arguments,
uses their first NUL as the C-string boundary, and returns no values for wrong
count/type. Unknown classes/members return -1. Caller buffers can be released.

The source importer uses the captured class names and original reader boundaries
to import all 71 array tables. **8,863 authored Lua queries** pass on host, strict
host sanitizers and both Android 17 x86_64 page sizes. Selftests also verify
duplicates, wrong arguments, borrowed-buffer release, unknown names, malformed
rollback and allocation-failure rollback. `GetPyStruct` remains absent.

- [10,000 C safety iterations](host-build-validation.json)
- [ARM64 build / 16 KiB alignment](arm64-build-validation.json)
- [Host Lua queries](../lua-runtime/names-host-data-validation.json)
- [Host sanitizer corpus](../lua-runtime/names-host-sanitizer-corpus-validation.json)
- [Android 17 / 4 KiB](../lua-runtime/names-android-4k-data-validation.json)
- [Android 17 / 16 KiB](../lua-runtime/names-android-16k-data-validation.json)

Each Android run verifies all 74 staged segments/list/assertion/runner hashes;
the original 35 files are also hash-checked before slicing. The updated runtime
passes all 5,608 constant queries, 504 arithmetic vectors and the 220-input script
parse corpus on both page sizes. Original game scripts are unexecuted. ARM64
is cross-built, without a hardware run. No Fold7/physical device is tested.
Source APK integration and full source-built gameplay remain unfinished.

## Reproduce

```sh
python3 port/pydata-names/build.py --host --report /path/to/host.json
python3 port/pydata-names/build.py --ndk /path/to/windows-ndk --report /path/to/arm64.json
python3 tools/trace_pydata_arrays.py --help
python3 port/pydata-names/tests/corpus.py --help
python3 port/lua-runtime/tests/names_corpus.py --help
```

Tracing needs the owner's original library, Unicorn/pyelftools and the existing
host oracle. Source builds need none of those proprietary inputs. Corpus tests
use owner cache files. The [rights statement](../../RIGHTS.md) applies to recovered
names and registration metadata.
