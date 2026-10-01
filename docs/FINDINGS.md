# Recovery findings and interpretation

## Inputs and identities

The source of truth is the supplied APK, not an assumed online version or an earlier compatibility patch. It contains `classes.dex`, the launcher/resources, `libDungeonHunter2.so` (15,938,284 bytes), `libStormGLOFT.so` (907,744 bytes), and `libnativeinterface.so` (11,320 bytes) under `armeabi-v7a`; another copy of the small JNI library exists under `armeabi`. All three examined native libraries are ELF32 ARM. SHA-256 hashes are recorded in the reports.

The package is `com.gameloft.android.GAND.GloftD2SS`. The manifest declares minSdk 8, no explicit targetSdk, and launcher `Zirconia_DRM`; it also includes newer compile-SDK metadata. Additional save-restoration classes are present in this supplied DEX. A filename containing v1.0.2 does not prove pristine release provenance. No account of the original studio build environment is asserted.

## Native recovery

| Library | Unique named functions | Physical function ranges | Interpretation |
| --- | ---: | ---: | --- |
| `libDungeonHunter2.so` | 31,021 | 31,018 | Rich static/dynamic symbols and mapping metadata; all named instruction ranges decoded without Capstone failures |
| `libStormGLOFT.so` | 1,596 | 1,500 | Many aliases and unsymbolized ranges; ARM/Thumb inference is less reliable without mapping symbols |
| `libnativeinterface.so` | 10 | 10 | Small component suitable for manual source reconstruction; distinct from the main engine |

The engine has 54,463 relocations, 1,628 vtables, 30,992 unwind-index entries and 867 unique source filenames in `STT_FILE` records. These records preserve names and ABI evidence; they do not recover class data layouts or source-file contents. Function aliases share the same physical machine-code range and are preserved in the indexes instead of inflating recovery counts by copying the same code.

All executable bytes, including PLT and unattributed gaps, are retained in the assembly bundle and independently round-tripped against the ELF inputs. Decoder coverage and byte preservation are different claims. Undecodable support-library ranges remain explicit raw bytes; their instruction semantics are unresolved.

Ghidra outputs are indexed by normalized ELF address and original analysis address/image base. `success=true` means the decompiler emitted pseudocode, **not** that it understood the function correctly. Warning comments, failed functions, unmatched symbol starts and heuristic extra functions are reported. Indirect calls, register conventions, aliased memory, inlined code, templates and old compiler constructs require human review. ARM/Thumb mode is recovered from `st_value` and corrected explicitly where generic analysis introduced wrong flow. Reproduction depends on analyzer/tool versions and can differ in heuristic function counts.

## Debugging metadata

The main library contains 150,327 DIE records, 136,458 validated DIE references, 11,621 type records and 19,692 subprogram records. Only 1,308 subprogram DIEs have nonzero addressed ranges, representing 982 distinct addressed names. These counts include declarations, duplicates, inlining and runtime/library material; they are not counts of recovered gameplay implementations.

All 32 compilation units concern licensing/online glue, STLport, or libgcc. There are 24,821 line rows across 212 source paths. The exporter retains DIE attributes, raw operands, references, location/range information and line-program commands, with zero parsing errors. The `.hpp` sketches contain readable declarations and confidence limits, without invented bodies.

## Android layer

Fresh APKTool and JADX recovery accounts for all 357 original DEX classes and 2,432 defined methods. JADX writes 364 Java files because it synthesizes 7 resource classes. Smali remains the bytecode-level reference. The initial JADX output has warning annotations and fails Java compilation in vendor billing code; original exports are kept unchanged.

The separate Java repair tree restores synthetic accessors and repairs decompiler damage against smali. Its local README and reports distinguish compilation, DEX/native-name checks, and remaining runtime work. JADX-renamed obfuscated fields and methods can break native `GetFieldID`/`GetMethodID` or reflection even when Java compiles. All such call sites require an explicit name audit before claiming engine compatibility.

The original storage strings include:

```text
/sdcard/Android/data/com.gameloft.android.GAND.GloftD2SS/files
/data/data/com.gameloft.android.GAND.GloftD2SS/libs/
<external-storage>/Android/obb/com.gameloft.android.GAND.GloftD2SS
```

Legacy strings also appear elsewhere. These describe original behavior, not a tested installation procedure for modern Android. A new port should import cache through Android's storage picker and place it in its own accessible directory, then update every native and Java path consistently.

## Cache and assets

The cache was reconstructed in numerical part order and parsed through local ZIP headers, stopping at the first incomplete entry. Recovery does not guess file boundaries by searching compressed payloads for ZIP magic and does not fill missing bytes. Complete entries must pass CRC-32 and decompressed-length checks.

The available prefix yields 5,839 files and 241 directories, totaling 509,330,036 uncompressed file bytes. The last incomplete entry is `m_world_map.wav`; its known compressed shortfall is 368,651 bytes. The original total file count and archive tail length are unknown.

The original text exports include game-object/module configuration and 32 GLSL sources plus shader configuration. See cache format reports for BRES/BTEX/PVR signatures, XML counts and duplicate-attribute filenames.

The new `port/engine-resources` implementation confirms the BRES whole-buffer relocation algorithm from original instructions. A fixup entry identifies the location of a four-byte pointer field. The first entry names the header's already-relocated table pointer; later entries relocate both the table entry and its pointed-to field. All 2,901 recovered images match the original in-place mutation exactly. Native ARM64 pointers must be kept separately from these serialized fields. The port exposes 13 top-level Collada libraries and their record strides, including 10,933 geometry and 49,060 animation records across files. Counts include repetition and do not imply distinct assets. Nested payloads, split/external references and runtime GPU construction are not yet decoded.

Memory/subfile readers retain original seek and callback quirks, including negative seek states, ignored seek failure, callback file identity, and subfile asynchronous advancement by requested rather than actual bytes. The port intentionally prevents unsafe memory copies and uses borrowed lifetimes pending ownership reconstruction. Exact evidence, ABI boundaries and test limits are in the resource module's README and validation report.

## Validated reconstructed component

`port/nativeinterface` supplies source for all ten original exported functions, including the four JNI entry points. It builds as ARM64 with 16 KiB-aligned load segments. Differential tests execute original ARM32 code in Unicorn for SHA-1, passphrase and license-file behavior; host-JVM smoke tests call the JNI methods. Exact test cases and known safety differences are documented in its report.

The original `storeLicenseKey` mixes UTF-8 bytes with `wcslen`, which can overread allocation slack. The reconstructed implementation documents a bounded, deterministic approximation rather than reproducing unsafe reads. Matching zero-padded test inputs does not prove equivalence for every real-world string or allocator layout.

This support-library result does not establish a working license service, working Android DRM flow, graphics compatibility or gameplay on a Fold7. No license check has been removed by the project.
