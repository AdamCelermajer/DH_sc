# Native Prince bank metadata

This is a new port metadata envelope, **not an original game serialization
format**. Its sole source is the existing, unmodified
`port/android-native/app/src/main/assets/data/prince-animation-bank.json`,
SHA-256 `76633bb4f0ab5f645f4516e407e1f926df61641ea66faa805f68da57f043eb81`.
That manifest binds original registration producer probe
`port/engine-animation/reference/prince-registration/probe.json`, SHA-256
`663429c6905a55f6ce013e4297ca0716e91146432e64369d2e291984415a5cd4`,
the original ELF and supplied cache hashes. Their reconstruction evidence stays
in the original registration reference; this reader does not re-execute those
instructions or claim an original loader ABI.

The resulting asset is
`port/android-native/app/src/main/assets/data/prince-animation-bank.bin`,
31,839 bytes, SHA-256
`c0cf8bfbb804256050e0d1ac326b8388b472c9b3034c8366c992f2056f477b0b`.
It retains KnightPlayerBase, animation table48, set12302, template1111, all116
resource records in first-unique order, and all158 ordered registrations including
duplicates. Every original authored path, Android asset path, cache entry path,
resource byte count and SHA-256 is preserved. Resource1138 remains a registered
resource; animation/timeline bounds are read from its actual asset elsewhere.

## PAB1 version1 layout

All words are little-endian uint32 unless marked signed. Digests are32 raw bytes.
There is no alignment padding or terminating NUL in strings. A string is word
byte-length followed by exactly that many printable ASCII bytes.

| Offset | Field |
| --- | --- |
| 0 | Four bytes `PAB1` |
| 4 | Version1 |
| 8 | Total binary byte count |
| 12 | Identity policy1 |
| 16 | Animation table ID |
| 20 | Animation set ID |
| 24 | Signed template clip ID |
| 28 | Unique resource count |
| 32 | Ordered registration request count |
| 36 | Source JSON SHA-256 |
| 68 | Cache SHA-256 |
| 100 | Original ELF SHA-256 |
| 132 | Original registration producer SHA-256 |
| 164 | Character string |

Then each resource has signed clip ID, byte count, SHA-256, authored path string,
asset path string, cache entry string, in that order. Finally come exactly the
ordered request-count signed clip IDs. Resource order also encodes
`first_unique_resource_order`; the reader checks this against first occurrence
in the request list. The first request must equal the designated template.
There is no trailing data.

Policy1 defines one **port resource-cache identity token** per exact unique asset
path: first-unique resource index+1. All occurrences of a resource ID resolve
to that same token. This is a declared owner/cache policy, not a reconstruction
of an original CCDB pointer or content-hash equality policy. The live engine
adapter must load each resource once and retain its actual canonical Player/cache
identity for original RegistrationSet operations. Ordered occurrences must still
be appended158 times; deduplicating the116 resource loads does not deduplicate
original animation libraries.

## Reader and producer boundaries

`animation_bank.hpp/.cpp` provide owned `data::AnimationBank`, resource records,
`load_animation_bank(Bytes,bank,error)`, checked resource/index lookup, and policy1
identity lookup. The native reader has no JSON or cryptographic library dependency.
It retains hashes as metadata; the owner may verify actual resource bytes before
loading. It rejects null/wrapped spans, inputs over2MiB, unsupported magic/version/
policy, size mismatches, zero or over65,536 counts, more unique resources than
requests, negative IDs, missing requests/resources, inconsistent first occurrence
order/default, zero/over64MiB resource lengths, all-zero digests, string lengths
outside1..4096, non-printable/non-ASCII/NUL strings, absolute/backslash/colon paths,
empty/`.`/`..` path components, duplicate IDs or any duplicate authored/asset/cache
path, and trailing data. On failure it returns false and retains the old Bank;
the error string records the failure. On success all strings/vectors are owned.

`tools/produce_animation_bank.py` preflights the exact source JSON hash, 116/158
counts, first-unique order, template1111/table48/set12302, safe distinct paths,
and byte size/SHA-256 of all116 actual Android asset files before writing.
It refuses to overwrite a mismatching existing binary. An identical existing
binary is accepted; `--verify-only` requires the binary to exist and match.
The generator does not alter JSON, source assets or original producer evidence.
Default report is `reports/animation-bank-producer.json`.

## Verification

`tests/animation_bank.cpp` decodes the real binary, exports owned decoded metadata
to a diagnostic JSON file, checks277 resource/index/identity lookups, performs
1,918 atomic rejection tests, and destroys source bytes before checking retained
output. Truncation fixtures repair the total-size header to exercise actual
payload reads rather than only header mismatch. Failures cover boundaries inside
all349 strings, header/digest/count/record/request mutations, duplicate ID/path,
missing IDs, default/order mismatches and trailing bytes.

`tests/animation_bank_host.py --rebuild` freezes compiler/source/test hashes,
builds the isolated native decoder under ASan/UBSan, compares every source JSON
metadata field and all116 resource records/158 requests to decoded output, and
binds the source registration producer hash. It also verifies producer refusal
for changed JSON, mismatching existing binary, missing assets and changed asset
bytes. Final result: zero mismatches and zero sanitizer findings. Report:
`reports/animation-bank-host-audit.json`.

The native test accepts binary asset path and writable diagnostic JSON path as
its two arguments. Parent integration can add the new source to game-data and
link this test to that library. This bounded task edits no CMake, renderer,
coordinator, existing JSON, original assets, APK or emulator state.
