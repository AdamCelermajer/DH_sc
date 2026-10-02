# Archived native decompiler evidence

This directory makes the historical Ghidra 11.0.3 export browsable as ordinary
Git files. The 44 files under the three library directories are byte-for-byte
copies from the verified `Dungeon-Hunter-2-Source-Recovery.zip` handoff (SHA-256
`b3ff974e2b74f50387465d5665f60d56ac79c29a449c6299745461998045c4d8`).
Their combined uncompressed size is 50,247,939 bytes. See [the per-file
manifest](manifest.json) and [the recovery import record](../../../docs/RECOVERY-SOURCE-IMPORT.md).

| Library label | Pseudocode shards | Index and summary | Exported function results |
| --- | ---: | ---: | ---: |
| `libDungeonHunter2.so` | 33 | 2 | 31,794 succeeded; 1 failed |
| `libStormGLOFT.so` | 4 | 2 | 3,332 succeeded; 1 failed |
| `libnativeinterface.so` | 1 | 2 | 31 succeeded; 0 failed |

Each `functions-*.pseudo.c` shard contains generated C-like text with original
address and symbol comments. `function-index.jsonl` maps function records to a
shard, byte offset and byte length. `summary.json` gives export counts. The
shards include inferred types, warnings and possible decompiler errors; even
the many reported successes mean only that Ghidra produced text. The export is
**not compilable original C/C++**, a validated native game implementation, or
a substitute for the original ARM instructions. The independently reconstructed
and tested components live under [`port/`](../../../port/).

Verify the checked-out files with:

```sh
python tools/verify_native_decomp_import.py
```

If the original recovery ZIP is available separately, also verify each exact
archive member and the archive's SHA-256:

```sh
python tools/verify_native_decomp_import.py --archive /path/to/Dungeon-Hunter-2-Source-Recovery.zip
```

The original ELF libraries, APK, cache, assembly and symbol bundles, and raw
asset XML/shader exports are not part of this import. These generated files
retain the provenance and rights of the supplied game material. This repository
does not assert a game-wide open-source license; see [`RIGHTS.md`](../../../RIGHTS.md).
