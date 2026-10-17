# Native level catalogue

`level_tables.cpp` turns both original level arrays into owned native strings
and vectors. It reuses the format recovered in `port/level-catalogue/catalogue.py`
and verifies the two exact schema sections before reading records.

The supplied files contain **33 FastTravelList rows followed by 51 LevelList
rows**. Both names sections use that order too. Looking only at the first names
section misses the Crypt declaration in the second section.

## Serialized records and source offsets

FastTravelDestination writes DescriptionId, EntryPointId, a length-prefixed
LevelName, LocationType and StringId. LevelDeclaration writes a one-byte
Dbg_IsStable, a length-prefixed DynamicBusRouting, Hub, a one-byte IsRandom,
LevelDescription, a length-prefixed LevelFile, and nine signed words.

The original LevelDeclaration object has a vptr and string length/pointer
pairs. Its **runtime stride is 72 bytes**. Serialized rows have variable size;
indexing the raw file as `OID * 72` is incorrect. The native adapter maps the
Lua caller's six range-word offsets to current owned rows, resolving vector
storage afresh on each read.

| Source word offset | Native field |
| --- | --- |
| `0x30` / `0x3c` | normal maximum / minimum |
| `0x34` / `0x40` | hard maximum / minimum |
| `0x38` / `0x44` | nightmare maximum / minimum |

GOTHICUS_CRYPT_01 is ordinal 23. Its LevelFile is
`007_crypt_01.rule.xml`; ranges are 8–10, 45–47 and 74–76. SWAMP is ordinal 41,
with ranges 1–4, 40–41 and 70–71. Values are ordinary signed integers.

The reader supports changed row counts for mods, with bounded counts and
strings. It rejects malformed UTF-8, invalid booleans, changed schemas,
unresolved fast-travel references, unsafe filenames and trailing bytes.
Failed loads preserve the previous owned catalogue.

## Verification and limits

`tests/run_level_tables_host.py` compares every field of all 84 original-cache
records with the existing Python catalogue, then executes 153 range-query
compositions and 12 malformed-input/offset guards.

With `--original-elf`, it also executes the original 1388-byte LevelDeclaration
reader and 556-byte FastTravelDestination reader on every actual record. All
84 record comparisons match. Stream primitives, allocation/free and
readStringEx are named modeled dependencies. This does not reconstruct their
bodies, the ARM32 object ABI, allocation policy, array constructors or complete
current-Level lifecycle.

The Android viewport now loads this catalogue from three unchanged bundled
assets and retains it in native ownership across scene operation. Its log
reports the Crypt ranges. Current GSLevel identity, difficulty selection and
hosting PlayerInfo ownership remain separate integration work. The catalogue
probe does not assign a current level or initialize an active AIS.
