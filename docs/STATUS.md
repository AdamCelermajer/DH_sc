# Recovery status — 2026-10-01

**Later input note (2026-10-02):** a separately supplied local cache ZIP is valid and contains 6,833 files, including the previously truncated WAV. See [COMPLETE-CACHE.md](COMPLETE-CACHE.md). Counts below document the earlier ten-part partial recovery and are not a full-cache validation result.

This package is a source-recovery workspace and studio handoff. It is not a completed game source restoration or a playable ARM64 APK.

## Final export accounting

| Library | Original distinct function starts | Starts attempted | Ghidra output functions | Pseudocode emitted | Failed exports |
| --- | ---: | ---: | ---: | ---: | ---: |
| `libDungeonHunter2.so` | 31,018 | 31,018 | 31,795 | 31,794 | 1 |
| `libStormGLOFT.so` | 1,498 | 1,498 | 3,333 | 3,332 | 1 |
| `libnativeinterface.so` | 10 | 10 | 31 | 31 | 0 |

Ghidra totals include heuristic functions and PLT thunks, so they exceed original named-start totals. Storm has 1,500 declared ranges but 1,498 distinct starts because two zero-sized division aliases share other bodies. Export completion is not semantic correctness. Emitted warnings and inferred types require review.

The engine's `ft_gzip_file_fill_output` caused the decompiler process to die. Storm's inferred PLT-area `__gnu_thumb1_case_uqi` timed out. Assembly evidence remains for both. All 6,488,258 executable bytes across the supplied libraries were preserved and round-tripped exactly. Every final UTF-8 byte index, shard reference and normalized ELF address passed verification.

## Source and test results

- Original Android recovery: all 357 classes, 2,432 defined methods and 46 native declarations; Java and exact smali preserved.
- Repaired Java: 288 source files compile to 365 class files, with zero errors and 15 legacy warnings under Java 17 / Android SDK 35. D8 produces a 571,752-byte DEX. All 46 declared native contracts match the original exactly.
- Reconstructed JNI source: all ten original function exports, including four JNI entry points. Host semantic and real JVM JNI tests pass. Original ARM32 instruction differential tests pass for the recorded SHA-1, passphrase and license-file cases. NDK r29 ARM64 compilation passes; load segments align to 16 KiB. Intentional safe differences from original undefined behavior are documented.
- Cache recovery: 5,839 complete files and 241 directories validated; 2,164 original text/shader files exported with SHA-256 provenance. Twelve recovery regression tests pass.
- Focused Java tests: vendor Base64 16,384 cases; Gameloft Base64 3,072 plus malformed vectors; CRC 1,296 checks; 30 installer/cache-integrity branches. These address identified decompiler damage, not whole-game behavior.
- All 2,164 text/shader hashes and all 288 repaired Java source hashes were independently rechecked.
- Reconstructed engine math: C++ for 20 original function starts (vector/quaternion plus matrix rotation), host and NDK r29 ARM64 builds, 16 KiB load alignment. 21,477 instruction differential comparisons pass; all 1,704 original instruction addresses in these ranges execute during tests. Original `__aeabi`/libm imports use a controlled host dependency model; historical Android libm and device integration remain unverified. See `port/engine-math/README.md` and `reports/engine-math-validation.json`.
- Reconstructed resources: C++ for 35 complete memory/subfile-reader and Collada-accessor bodies, plus the whole-buffer BRES relocation branch. Host and NDK r29 ARM64 builds pass. 10,449 stream comparisons and all 2,901 recovered BRES files pass original-ARM32/compiled-ARM64 checks, including 2,577,206 fixups, 5,154,412 native pointer values and 27,812 library/scene pointers. All 331 addresses in the complete bodies execute. Host ASan/UBSan input probes pass with leak detection disabled. See `port/engine-resources/README.md` and `reports/engine-resources-validation.json`.
- Mesh and animation reconstruction: immutable C++ views decode all 10,924 type-0 meshes, 1,641,664 vertices, 1,088,422 triangles and 890,301 animation time keys across the 2,901 recovered BRES files. The Prince's 173 meshes use a separately traced deferred-buffer form. Twenty-six complete animation accessor/typed-search bodies pass 271,970 original-ARM32/compiled-ARM64 comparisons with zero mismatches and all 456 instruction addresses executed. A separate host/ARM64 mesh check passes 25,682 comparisons on 1,149 meshes; it does not execute original GPU constructors. Host/ARM64 builds and 5,000 host ASan/UBSan probes pass. Small real OBJ/animation JSON exports are included. See `port/asset-payloads/README.md` and the four `reports/asset-payloads-*.json` files.

## Still unresolved

- Native pseudocode does not compile into a replacement engine. Original gameplay class fields, ownership, ABI and indirect-call behavior still require reconstruction.
- Sixteen of nineteen vector/quaternion pseudocode bodies incorrectly stop at floating-point helpers marked as non-returning. The new math module uses complete assembly; the archived pseudocode has not been regenerated with corrected helper metadata.
- JNI/reflection calls into non-native Java members remain unverified, including JADX-renamed obfuscated fields/helper methods.
- The cache ends partway through `m_world_map.wav`; at least 368,651 compressed bytes plus an unknown archive tail are absent.
- BRES container access, interleaved type-0 meshes, deferred bytes inside complete files and raw animation keys/search are reconstructed. Nine type-1 geometries, images/materials, scenes/controllers/skinning, track-specific animation application, split/external loading, ownership, rendering and a complete native engine build remain unfinished.
- No Android device execution of the reconstructed engine components, integrated menu/load/combat/quest/save validation, or extended gameplay test was performed. Separate compatibility work and its device reports do not establish a rebuilt ARM64 engine.
- The supplied APK contains later save-restoration/support additions; its untouched studio-release provenance is not established.

The isolated compiled artifacts included with a Drive handoff are validation outputs, not a playable game APK. The exact input APK and cache parts are kept separately from recovered source.
