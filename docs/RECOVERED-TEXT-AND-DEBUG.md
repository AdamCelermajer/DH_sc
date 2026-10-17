# Verified remaining recovery evidence import

The same `Dungeon-Hunter-2-Source-Recovery.zip`, SHA-256
`b3ff974e2b74f50387465d5665f60d56ac79c29a449c6299745461998045c4d8`,
now supplies **2,214 exact files**, totaling **21,618,099 bytes**:

- 2,164 shader/configuration text resource files (4,397,508 bytes).
- Their unchanged provenance and the historical incomplete cache manifest.
- 48 native DWARF/debug export files, including readable declaration sketches,
  summaries and compressed JSONL records.

Every selected archive member passed the recovery ZIP CRC check and its
recorded SHA-256. The imported text resource hashes also matched their original
provenance. [The per-file ledger](../reports/remaining-recovery-evidence-import.json)
records exact member names and hashes; [the verifier](../tools/verify_remaining_recovery_import.py)
checks a clone, optionally against the original ZIP. `--git-index` additionally
verifies that Git records every member with the exact same content. Git attributes preserve
archival bytes without line-ending conversion.

## Where the evidence is

| Evidence | Git path |
| --- | --- |
| Original shader and configuration text | [`recovered/assets/source-data`](../recovered/assets/README.md) |
| Historical incomplete cache accounting | [`cache-manifest.json`](../recovered/assets/cache-manifest.json) |
| Native debug summaries | [`debug/summary.json`](../recovered/native/debug/summary.json) |
| DWARF indexes and declaration sketches | [`debug/libDungeonHunter2.so`](../recovered/native/debug/libDungeonHunter2.so/README.md) |

The native debug export records 32 compilation units, 150,327 DIE records,
11,621 type records, 19,692 subprogram records and 212 source paths. The units
cover license/online glue, STLport and compiler runtime. The export identifies
no gameplay compilation unit and recovers **zero original function bodies**.
`libStormGLOFT.so` and `libnativeinterface.so` contain no identified DWARF info.
Compressed `.jsonl.gz` files can be read with Python's `gzip` module; readable
declaration sketches are informational and require review before compilation.

The archived text provenance describes an earlier incomplete cache. It is
distinct from the later complete 6,833-member owner cache validated in
[COMPLETE-CACHE.md](COMPLETE-CACHE.md). The source-built Android preview still
uses its own diagnostic shader; this import does not establish original shader
runtime behavior. [RIGHTS.md](../RIGHTS.md) records provenance and licensing limits.
