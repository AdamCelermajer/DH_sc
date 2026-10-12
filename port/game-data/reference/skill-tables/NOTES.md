# Owned Skill tables

The new `skill_tables.hpp/.cpp` owns the complete first SkillList table and the following Skill table from the supplied Android cache. All scalar words, raw boolean bytes, ordered vector elements, string bytes, row names and field names are retained. It supplies backing for the separately recovered CancelSneaking coordinator; it does not infer Sneak flags or register a whole Application.

## Original evidence and actual boundaries

`original-functions.json` and `reference/original-functions.asm` capture 21 complete routines from `libDungeonHunter2.so`, SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. The input cache ZIP is SHA256 `3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`.

| Stream | First block | Following block | Total consumed |
|---|---|---|---:|
| skills_pyarray.bin | SkillListTable, 36 lists, bytes 0..999 | SkillTable, 127 rows, bytes 1000..11861 | 11862 |
| skills_pyarraynames.bin | 36 names, bytes 0..558 | 127 names, bytes 559..2756 | 2757 |
| skills_pystructnames.bin | List field, bytes 0..11 | 15 Skill fields, bytes 12..232 | 233 |

The actual three streams have no unparsed suffix. The owner API deliberately exposes consumed offsets and leaves any future suffix to another owner.

The actual table readers are `Arrays::SkillListTable::read` 0x4b9964 and `Arrays::SkillTable::read` 0x4b9810. Name readers 0x4b0464/0x4b0938, name getters 0x4ad1a8/0x4ad0dc, and field getters 0x4ad21c/0x4ad150 execute in the original oracle. Schema text pointers are projected into the genuine getters' metadata; Application registration/startup is outside this proof.

| Input | SHA256 |
|---|---|
| skills_pyarray.bin | e336986d5aee78fd1aed7fe7ec43cc8d58fa03d4fd27216779c0e96e47acc2d6 |
| skills_pyarraynames.bin | 31642dc0d8bf11f1ebd13698bbefda4fda0aefa2c727fb4ae9449f2df073fb9f |
| skills_pystructnames.bin | fd882501b0b7af0191272e6802558d1e1ac4031f6237cf5e2c767ebae946f4b9 |

## Record layout

The source SkillList runtime record is 12 bytes: header at +0, count at +4, signed-word array pointer at +8. Its serialized payload is count followed by ordered signed 32-bit elements.

The Skill runtime record is 76 bytes. Offsets below are hexadecimal. Variable records serialize in this exact field order.

| Field | Runtime offset | Serialized value |
|---|---:|---|
| Anim | 04 | 32-bit word |
| AnimIsMoving | 08 | raw byte |
| DisplayProps | 0c/10 | count and ordered signed-word vector |
| ElementalType | 14 | 32-bit word |
| FairieDependantText | 18 | raw byte |
| Flags | 1c | 32-bit word |
| Level | 20 | 32-bit word |
| Script | 24/28 | byte count and exact string bytes |
| SkillAssignable | 2c | raw byte |
| SkillCurrLevel | 30 | 32-bit word |
| SkillDescription | 34 | 32-bit word |
| SkillIcon | 38/3c | byte count and exact string bytes |
| SkillName | 40 | 32-bit word |
| SkillNextLevel | 44 | 32-bit word |
| Type | 48 | 32-bit word |

The fixed serialized minimum is 51 bytes. The source boolean reader 0x4db89c copies one byte without normalization. `StreamReader::readStringEx` 0x317454 copies the complete counted payload and appends a runtime NUL; embedded NUL and subsequent bytes are retained. Native `std::string` owns that same payload and supplies its terminator.

`SkillProjection76` zeroes the source header, three pointer words and boolean padding. Counts, string lengths, raw boolean bytes and all scalar words remain exact. Native vectors/strings own the source pointer payloads separately. These projections are not an ARM32 pointer layout that can be passed to the original binary.

Name lookup uses ordered, case-sensitive, first-match C-string comparison. Duplicate names are retained. Embedded NUL truncates comparison, while stored suffix bytes remain available. Null or missing keys return -1. The owner validates the exact one-field/15-field source schema. Invalid or negative list references are preserved, not repaired.

## Ownership and integration

`SkillTables::load(records,names,schema,error)` produces an immutable owned snapshot. `borrow()` pins it through input-buffer and owner destruction. Reload is rejected while a Borrow is pinned; malformed input leaves the prior snapshot intact. Access to vectors through a missing Borrow throws; lookup and consumed-offset methods return their documented empty values.

Three C entry points expose atomic bounded decoding: `dh2_skill_decode_record`, `dh2_skill_decode_list`, and `dh2_skill_tables_measure`, returning 0 for success and 1 for malformed input. Their output objects must be aligned and disjoint; input may be unaligned. Their returned spans borrow the input. The C++ owner immediately copies those spans. Port bounds are 16 MiB per stream, 65536 rows/vector elements/names, and 1 MiB per text. These bounds and malformed-input rejection are native contracts, not original allocator parity.

To connect CancelSneaking, retain a Borrow, construct `List16` views over its ordered list vectors and `Skill76` scalar projections over its owned records, then retain those adapter arrays for every call. Source CancelSneaking only reads scalar Skill fields in the recovered branch. Other consumers needing Script/Icon/DisplayProps must use the owned payloads, not zeroed source pointer words.

Every actual cache Skill Flags word lacks bit 0x02000000. The owned CancelSneaking host check therefore proves the genuine complete scan without invoking a guessed skill backend. Its nonplayer classification service is an explicit fixture. Buff destruction, active skill/Pre callbacks, full skill VM behavior, Character ownership and whole Application registration remain separate dependencies.

## Proof and reproduction

`skill-tables-arm64-differential.json` records zero mismatches for 383 records (127 actual plus 256 synthetic), 164 lists, 65 tables and 195 original name/field queries. The actual original table/row/name/getter instructions execute; stream/allocation/string imports are explicit oracle services. Dynamic payload comparison covers 142294 bytes. Duplicate and first-NUL lookup cases execute in the original.

The O2 ARM64 library builds with `DH2_SKILL_DECODER_ONLY`; its proof covers the full bounded record/list/table parsers. The owned C++ snapshot is independently audited on the sanitized host, not claimed as an ARM64 ownership instruction comparison.

`skill-tables-host-audit.json` records 40212 checks, 14868 malformed/ownership guards and zero AddressSanitizer, UndefinedBehaviorSanitizer or LeakSanitizer findings. It replays the complete gold, owns every actual row, tests synthetic raw booleans/embedded NUL/vector words, rejects truncated streams and malformed empty name blocks, verifies atomic failure and pinned use after destruction, and invokes frozen CancelSneaking over the owned cache data. The runner hashes compiler inputs and executable before/after; no central DSO is rebuilt.

From the repository root, with the configured Python/Unicorn environment:

```powershell
& port/game-data/tools/build_skill_tables_oracle.ps1
python port/game-data/tests/skill_tables_differential.py
python port/game-data/tests/skill_tables_host.py
```

A future main-linked `skill_tables_audit` target uses `tests/skill_tables.cpp`, links game-data plus the world CancelSneaking implementation, and accepts these four positional arguments:

```text
port/game-data/reference/skill-tables/skill-tables-fixtures.bin
.local-inputs/skill-tables/skills_pyarray.bin
.local-inputs/skill-tables/skills_pyarraynames.bin
.local-inputs/skill-tables/skills_pystructnames.bin
```

Stable production SHA256: CPP `85695e3a274dae96bb92abcfdd12bdfb24b4d9f910f0026a777a39ce6c5534dd`; header `fe01ceadd86c43a2a6f92bcf8edd76466643c521db01b7176bab25fad311abab`. Gold SHA256 `e7b5e11a874f3b7994f95e4fe0a36e72100bf572de4d1f31b0ab4345bbc498d4`. Reports bind the original manifest, assembly, cache inputs, compiler sources and actual proof binaries.
