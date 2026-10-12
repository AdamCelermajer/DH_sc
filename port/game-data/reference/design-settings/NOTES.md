# Native DesignSettings ownership

This stage reconstructs the complete first DesignSettings table, its row names,
and its 43-field schema. It supplies owned authored `EnemySpottedAggro` storage
to the existing EnemySpotted coordinator. It does not reconstruct the remaining
tables in the design streams or whole Application registration.

## Original evidence

Original ARM32 ELF SHA256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The ten complete captured functions and individual instruction hashes are in
`original-functions.json`; instructions are in `reference/original-functions.asm`.

| Original function | Address | Behavior used |
| --- | --- | --- |
| Arrays::DesignSettingsTable::read | 0x4b3cd0 | Count, row allocation, ordered row reads |
| Structs::DesignSettings::read | 0x4ee0d0 | All 43 scalar fields |
| StreamReader::readAs<float> | 0x4db94c | Exact four serialized bytes |
| StreamReader::readAs<int> | 0x459090 | Exact four serialized bytes |
| StreamReader::readAs<unsigned> | 0x313a90 | Table count |
| Arrays::DesignSettingsTable::readNames | 0x4b7638 | Ordered name allocation/reading after count initialization |
| Arrays::GetMemberIDByString<DesignSettingsTable> | 0x4aed88 | Ordered, case-sensitive first-match lookup |
| Structs::GetMemberIDByString<DesignSettings> | 0x4aedfc | Ordered, case-sensitive field lookup |
| Arrays::DesignSettingsTable::finalize | 0x4a90b8 | Source row storage cleanup |
| Arrays::DesignSettingsTable::finalizeNames | 0x4a901c | Source name storage cleanup |

The serialized row is 172 bytes. Its source runtime projection is 176 bytes:
the first word is a vtable pointer, followed by 43 four-byte scalar fields at
offsets 4 through 172. The native projection sets the header word to zero and
preserves all payload words. The float reader preserves IEEE payload bits,
including NaN, infinity, and signed zero; it does not evaluate or normalize them.
The captured reader has a fixed little-endian path. No defaults are synthesized.

Zero-based integer fields are 4, 8, 9, and 24 through 33; all other fields are
float words. The kind accessor exposes this distinction without converting the
stored words. All 43 fields are decoded; this is not a threat-only row fixture.

Table count/row/name globals are respectively 0x9a6494, 0x9a6498, and 0x9a649c.
The original name lookup compares CString text with `strcmp`, so case matters,
the first embedded NUL ends comparison, duplicate names return their first
matching index, and misses return -1. Actual row names are loaded by the source
`readNames` routine after the table count has been initialized.

The field-name getter proof supplies genuine schema names through the original
CString layout (24-byte records with the text pointer at +20). It does not run
whole Application startup. In particular, the static `m_dataOffsets` storage
at 0x99a0dc was uninitialized in the isolated ELF. Field offsets used by the
proof are derived from the complete row-reader instructions, not from that
uninitialized global. The native owner validates the exact authored 43 names.

## Actual cache inputs and threat

Inputs are from the authorized cache ZIP SHA256
`3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`.

| File | Bytes | SHA256 |
| --- | ---: | --- |
| design_pyarray.bin | 692 | fd70c8be93cd3d5b7947309e22f00a7c5230a4a7541ec6f9ebfdac67335e6296 |
| design_pyarraynames.bin | 272 | d9d5ec686e24d2ee0dcf9f7b627b6679ee522b2df311c6861b7c1bb39ab96c7b |
| design_pystructnames.bin | 1443 | d955c29a9ddeb091bc5382eff44002a93e6390228bbe0974769648cc58044dc6 |

The first table has one row named `Default`. Its table prefix consumes 176
bytes and its names prefix consumes 15 bytes. `EnemySpottedAggro` is field 11,
runtime offset 0x30, raw word 0x41200000, representing float 10. The unrelated
stream suffixes contain difficulty/debug tables and remain outside this owner.
The owner reports consumed offsets so callers can distinguish the decoded
prefix from the complete input file.

## Native API and lifetime

`port/game-data/design_settings.hpp/.cpp` expose:

- `dh2_design_settings_decode_record`: complete raw row decoding, return 0 on
  success and 1 on malformed input; output and consumed count are atomic.
- `dh2_design_settings_decode_table`: complete table-prefix decoding; return
  0 on success, 1 on malformed input, or 2 on insufficient caller capacity.
- `DesignSettingsOwner::load(records, names, schema, error)`: validates and owns
  all first-table rows/names/schema. Failure preserves the previous snapshot.
- `DesignSettingsOwner::borrow()`: pins the immutable snapshot independently
  of the owner and input byte buffers. Reload is denied while a borrow pins it.
- Borrowed row/field lookup, raw `word(row, field)`, field-kind lookup, and
  `enemy_spotted_aggro_bits(row=0)`. Missing rows/fields return null, not defaults.

Keep a `Borrow` alive for as long as its threat pointer is attached to
`EnemySpottedServices24::initial_threat`. Obtain the intended row by its actual
name when constructing the binding. The verified cache row is `Default` at 0.
The returned pointer refers to owned `uint32_t` bits, matching the prefix API;
there is no float-pointer alias or dependency on the original gold corpus.

Native safety bounds are explicit port contracts: streams at most 8 MiB,
at most 65,536 rows/names, names at most 1 MiB, exact first-schema dimensions and
names, aligned/disjoint output spans, and complete serialized inputs. Input
bytes themselves need not be aligned. Source short-read partial writes are
not advertised as native success; malformed native decoding is atomic.
These guards are separately tested rather than asserted as ARM32 semantics.

## Proof and reproduction

`design-settings-arm64-differential.json` records PASS with 385 complete records,
97 complete tables, 33,484 raw-word comparisons, and 52 actual original name
queries, with zero mismatches. The corpus includes the actual cache row/table,
IEEE edge words, random complete rows, multi-row/zero-row tables, first-NUL and
case-sensitive names, and an original duplicate-name first-match check. Actual
source row/table/name/getter instructions execute; stream/allocation/string
imports are explicit services. The run observed 32,961 reads and 105 allocations.

`design-settings-host-audit.json` records PASS with 3,874 checks, the same
385/97/33,484/52 comparisons, 1,433 native guards, and zero AddressSanitizer,
UndefinedBehaviorSanitizer, or LeakSanitizer findings. It destroys both owner
and input buffers while a borrow remains, then consumes the still-valid actual
threat through the frozen native EnemySpotted prefix (15 ordered service calls).
Debug, group, state-machine, combat, and aggro callbacks in that attachment test
are explicit fixtures; it is not a live AI or complete frame proof.

From the repository root in PowerShell:

```powershell
& port/game-data/tools/build_design_settings_oracle.ps1
& 'C:/Users/adamc/AppData/Roaming/uv/python/cpython-3.12.13-windows-x86_64-none/python.exe' port/game-data/tests/design_settings_differential.py
& 'C:/Users/adamc/AppData/Roaming/uv/python/cpython-3.12.13-windows-x86_64-none/python.exe' port/game-data/tests/design_settings_host.py
```

Set the existing oracle environment `PYTHONPATH` to
`C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages`
and `PYTHONDONTWRITEBYTECODE=1` for the differential runner. The host runner
builds only the new owner/test and frozen EnemySpotted source under WSL; it
does not rebuild central DSOs. Its report binds compiler/run commands,
executable bytes, source/input/proof hashes, and verifies them before/after.

For a central host target, add `design_settings.cpp` to game-data and link
`tests/design_settings.cpp` with game-data plus the existing EnemySpotted world
implementation. CLI arguments are the gold corpus followed by the three actual
cache files above. No CMake, GameDesign, ScriptSession, renderer, or APK edits
are part of this handoff.

Frozen CPP SHA256:
`68b641c45a23b7d4ba55686a39506de470f0b2605b8c6f7293479d548ffb4dc0`.
Frozen HPP SHA256:
`4f7826db209feaf66822ca6156f7d503c3e7864165cd4c786bc2ab19f3c6b9f4`.
Gold SHA256:
`cbae8b3e059aca0f1c7cfc2ac998722172a94be97e35814f190ad1dc8f456682`.

