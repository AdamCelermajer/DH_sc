# Locally supplied complete cache ZIP — 2026-10-02

The owner supplied a separate ZIP named `Dungeon-Hunter-2-HD-v1-0-2-cache (1).zip` on the local machine. It is **not** the ten-part, 314,572,800-byte prefix described in the earlier recovery reports. The ZIP is an additional input; it is not committed to this repository.

`tools/verify_cache_zip.py` read every member to EOF, checked ZIP CRCs and lengths, rejected duplicate/case-colliding or unsafe paths, and recorded the archive SHA-256. The reproducible result is in [`reports/complete-cache-archive.json`](../reports/complete-cache-archive.json).

| Check | Result |
| --- | ---: |
| ZIP bytes | 433,189,197 |
| ZIP SHA-256 | `3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679` |
| CRC-valid files | 6,833 |
| Directories below `files/` | 253 |
| Uncompressed file bytes | 648,357,710 |
| BRES files with expected magic | 2,904 |
| `BTEXpvr` texture wrappers | 234 |
| Other `.tga` files | 8 |

The archive contains the previously cut-off `data/sounds/m_world_map.wav` (1,037,460 bytes) and `data/3d/characters/prince/prince_modular.bdae` (3,207,072 bytes). Its entries live under `com.gameloft.android.GAND.GloftD2SS/files/`, which matches the compatibility importer's expected layout. A valid ZIP and these assets do not by themselves prove that every reference needed for full gameplay is present or that the Test 5 APK loads them.

The existing C++ mesh/animation cache auditor also passed on all 2,904 BRES files from this ZIP. Its [new report](../reports/asset-payloads-complete-cache-validation.json) records 10,924 decoded type-0 meshes, 890,301 animation time keys and the same nine unsupported type-1 geometries. This is a host component check; it does not exercise a GPU or gameplay.

The [resource differential run](../reports/engine-resources-complete-cache-validation.json) compared the compiled ARM64 reader against the exact original ARM32 engine for all 2,904 BRES files: 2,579,645 fixups, 5,159,290 native pointer values and 27,823 library/scene pointer checks, with zero mismatches. This controlled instruction-level test does not cover Android framework integration or gameplay.

The [animation accessor/search differential run](../reports/asset-payloads-complete-cache-arm-validation.json) scanned all 2,904 BRES files and checked 1,919 animation-bearing files: 271,970 original ARM32 versus compiled ARM64 comparisons, zero mismatches. The texture and material view modules also compile as Android ARM64 ELF shared libraries with 16 KiB PT_LOAD alignment; their host cache checks are documented in their own READMEs. None of these results is a rendered frame or a working game.

To verify another copy without extracting it:

```sh
python tools/verify_cache_zip.py /path/to/cache.zip --report /path/to/audit.json
```

The archive includes saves and game assets. Keep the input ZIP private unless the rights holder authorizes distribution; this report contains counts and checksums only. The earlier partial-cache test reports remain valid for their exact input and are not silently reinterpreted as full-cache results.
