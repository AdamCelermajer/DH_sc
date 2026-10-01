# Dungeon Hunter 2 — source reconstruction

## Engine source reconstruction — math checkpoint

[Browse the reconstructed C++ module](port/engine-math/README.md). Twenty original vector/quaternion/matrix function starts have been rewritten from ARM assembly and compile for ARM64. The [recorded test report](reports/engine-math-validation.json) contains 21,477 original-ARM32 versus compiled-ARM64 comparisons with zero mismatches, under the documented external arithmetic/libm model. All 1,704 original instruction addresses in these routines were exercised. This is a tested engine component; asset loading, rendering and gameplay reconstruction remain unfinished. The full source ZIP and validation-artifact ZIP in the Drive folder below now include this checkpoint.

## Fold7 compatibility build — test 3

[Download the test 3 APK and full work archive](https://github.com/Noamcelermajer/DH_sc/releases/tag/v1.0.2-fold7-test3). Test 2 completes the cinematic on SM-F966B / Android 16, then aborts at the fairy loading screen. Test 3 records the native abort state and logs, adds **Export diagnostic ZIP**, and provides optional English/display/context settings. The loading crash is not yet claimed fixed. Install over test 2 to retain cache and saves.

Read the [test 3 changes and phone procedure](compatibility/work/fold7-build/TEST3.md), [independent bridge assessment](compatibility/work/fold7-build/BRIDGE-ASSESSMENT.md), [known issues](compatibility/ISSUES.md), and [device checklist](compatibility/DEVICE-TESTING.md). Five translated abort/exit/fault probes, seven media-query cases, complete library loading/hook checks and package checks pass; Fold7 gameplay still needs a retest.

All **2,102 compatibility-work files** are preserved in `compatibility-work-test3.zip`, with exact paths and SHA-256 hashes. After cloning into a clean directory, restore the full trees with:

```sh
python unpack_compatibility.py
```

The script verifies the archive and every restored file. Authored code and reports are also browsable under `compatibility/`. The standard full-directory ZIP is attached to the test 3 release. Prior releases and the test 2 snapshot are retained. External toolchains and duplicate build intermediates are represented by versions, hashes and build instructions. The compatibility snapshot includes native/runtime inputs and a deliberately public development-only signing fixture; original rights and third-party notices apply.

**Download the complete recovery handoff:** [Google Drive folder](https://drive.google.com/drive/folders/1njTxLAJHt08SinEskn7gC6IWylXemwDW). The folder contains the source ZIP, full native assembly/symbol bundles, validation artifacts, the exact supplied APK and all ten supplied cache parts. Start with START-HERE.txt. This GitHub repository contains the engine math checkpoint and compatibility work. The remaining recovery paths below refer to the downloaded source package.

This repository contains code recovered from the supplied **Dungeon Hunter 2 HD v1.0.2 Android APK**, binary evidence for its native engine, recovered level configurations and shaders, and independently reconstructed port components. Its purpose is a reviewable studio handoff and a starting point for restoring the game on modern Android.

**This is not the original studio source repository or a completed, playable ARM64 port.** Native decompiler output is pseudocode that still requires reconstruction and behavioral validation. A successful export or compilation is not evidence that the game runs.

## What is here

| Component | Recovered evidence | Location |
| --- | --- | --- |
| Android layer | All 357 original DEX classes, Java exports, exact smali, decoded manifest, 46 native declarations | [`recovered/android`](recovered/android), [`reports/java-recovery.json`](reports/java-recovery.json) |
| Main engine | 31,021 unique named functions; 31,018 physical ranges; 867 original build filenames; symbols, relocations, vtables and full executable bytes | Native bundles; [`reports/native-inventory.json`](reports/native-inventory.json) |
| Native high-level analysis | Ghidra pseudocode with per-function address, warning and failure indexes | [`recovered/native/decompiled`](recovered/native/decompiled), [`reports/decompiler-coverage.json`](reports/decompiler-coverage.json) |
| Debugging information | Complete DWARF export; 11,621 type records and 32 declaration sketches | [`recovered/native/debug`](recovered/native/debug) |
| Game configuration/shaders | 2,164 exact XML/text/shader resources, with provenance hashes | [`recovered/assets/source-data`](recovered/assets/source-data) |
| Cache audit | 5,839 verified files and 241 directories from the uploaded prefix; file formats, hashes, truncation evidence | [`reports/cache-recovery.md`](reports/cache-recovery.md), [`reports/cache-formats.md`](reports/cache-formats.md) |
| JNI support source | Reconstructed source for the small `libnativeinterface.so`; ARM64 compilation and original-ARM differential tests | [`port/nativeinterface`](port/nativeinterface) |
| Android Java repair | Separately maintained compilation repairs; exact validation and remaining JNI/reflection risks in its own README | [`port/android-java`](port/android-java) |
| Engine math reconstruction | Buildable C++ for 20 original vector/quaternion/matrix routines; 21,477 ARM32/ARM64 comparisons passed | [`port/engine-math`](port/engine-math), [`reports/engine-math-validation.json`](reports/engine-math-validation.json) |
| Recovery tooling | Scripts to reproduce the recovery from the owner's APK and cache parts | [`tools`](tools), [`docs/REPRODUCING.md`](docs/REPRODUCING.md) |

Large assembly and ELF inventories are included as compressed bundles to keep the repository manageable. No evidence is dropped by this packaging. After cloning, restore their individual files with:

```sh
python tools/unpack_native.py
```

This recreates `recovered/native/assembly/` and `recovered/native/symbols/`, including all original function aliases and raw bytes outside named functions. Decompiler output, Java, smali, recovered configuration and shader files are available directly in the repository.

## Findings that change the restoration plan

1. **The main engine is not stripped.** Its symbol tables preserve gameplay and engine names, virtual tables, relocations, 867 build-source filenames and ARM/Thumb mapping symbols. Assembly recovery round-trips all executable bytes across the three libraries: **6,488,258 bytes**. This is strong binary evidence; it is not C++ source recovery or a runtime test.
2. **DWARF does not contain the gameplay implementation.** All 32 identified compilation units cover licensing/online glue (7), STLport (22) or libgcc (3). The debugging metadata is useful for these components, but cannot substitute for reconstructing the engine's gameplay classes.
3. **The cache upload is incomplete.** The ten parts total exactly 314,572,800 bytes and stop inside `data/sounds/m_world_map.wav`. At least 368,651 compressed bytes are missing to complete that entry, followed by an unknown amount of remaining archive data. The ZIP central directory is absent. Files reported as recovered passed decompressed-length and CRC-32 checks; the incomplete file is excluded.
4. **A considerable amount of game data is already readable.** Most module/game-object formats are XML, and the shader package contains original GLSL text. Binary resources include BRES `.bdae`, BTEX/PVR texture wrappers, WAV and VoxN audio. Three original XML files contain duplicate attributes; they are preserved unchanged and documented for parser compatibility.
5. **The supplied APK's provenance is not established as an untouched studio release.** Its manifest has compile-SDK 33 metadata and its DEX contains save-restoration/additional support classes. The repository faithfully documents this supplied binary. Those observations do not establish who repackaged it or what changes were made.
6. **Rebuilding for ARM64 is a separate engineering project.** The three supplied native libraries are ELF32 ARM. Renaming ABI folders, changing target SDK, or compiling the small JNI library cannot turn the engine into a native ARM64 game. Class layouts, pointers, JNI/reflection names, graphics, storage, audio, networking and gameplay must be validated together.

See [`docs/FINDINGS.md`](docs/FINDINGS.md) for evidence interpretation and [`docs/PORTING.md`](docs/PORTING.md) for the remaining work.

## Start reviewing

- The exact input APK SHA-256 is `32c2d027b585a42547311cd95da6a3975fdb3174e663e513d42a7f49d1a4c200`.
- Read [`docs/STATUS.md`](docs/STATUS.md) for generated coverage and test results.
- For behavior, prefer exact smali/assembly over a decompiler's inferred types or control flow. A `.pseudo.c` file deliberately does not claim to compile.
- Inspect [`port/nativeinterface/README.md`](port/nativeinterface/README.md) for the genuinely buildable native component, its differential tests, and documented safe differences from undefined behavior in the original.
- Give the rights holder this repository together with the exact input APK and complete original cache when available. Ask for original engine/build metadata and asset tooling; source-file names and subsystem inventories help focus that search.

Art/audio binaries, input APKs, original `.so` files, toolchain binaries and signing keys are not committed. The recovery tooling operates on copies supplied separately by the owner. Recovered materials carry their original provenance; this repository does not assert an open-source license for the game. See [`RIGHTS.md`](RIGHTS.md).
