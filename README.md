# Dungeon Hunter 2 — source reconstruction

**New local input, 2026-10-02:** a separately supplied 433,189,197-byte cache ZIP passed full member CRC/length checks and contains 6,833 files. See [the complete-cache audit](docs/COMPLETE-CACHE.md). Earlier ten-part recovery figures below describe a different, truncated input.

**Current Android emulator check:** Official Android 17/API 37 x86_64 phone images with 16 KiB and 4 KiB pages both ran the wrapper launcher. The 16 KiB image hit the translation runtime's explicit page-size guard; the 4 KiB image could not load its AArch64 proxy into an x86_64 plugin process. A standalone build of the reconstructed asset readers did run on the 16 KiB image. The game is not playable on Android 17 yet. See [the Android 17 results](docs/ANDROID-17-EMULATOR-RESULTS.md). An older Android 9 diagnostic emulator ran the locally signed Test 6 ARM32 guest through its cinematic, title and animated main menu with the complete cache and one cache copy, but level loading failed with native-translation memory exhaustion; see [the diagnostic report](docs/EMULATOR-TEST-2026-10-02.md).

## Engine source reconstruction — texture, material and render preview checkpoint

[Texture views and PVRTC decoding](port/texture-assets/README.md) classify all 363 external cache textures and decode the 234 BTEX/PVRTC images to RGBA8. Every decoded pixel in those 234 images matched the official PowerVR software decoder; the revised component also builds for Android ARM64. [Checked image/material views](port/material-bindings/README.md) cover nested BRES effect/material parameters and image-index links across all 2,904 BRES files. They resolve 3,854 local effect IDs and also build for Android ARM64. These are isolated source components; the ARM64 builds have not been loaded on an Android device.

The [host WebGL renderer preview](port/renderer-preview/README.md) draws one real candle mesh primitive in flat and textured diagnostic modes. The textured mode follows its BRES diffuse image link, decodes the cached PVRTC texture, uploads RGBA pixels to WebGL and uses an original unlit textured GLSL pair. Both draws passed 53,185-pixel readback; the textured image has 6,460 distinct RGB colors. The shader choice and state are diagnostic, so original game material behavior, scenes, Android engine texture upload and a playable rebuilt engine remain unfinished.

The [Android 17 source smoke check](port/android-smoke/README.md) ran reconstructed PVRTC and BRES/material readers in a native x86_64 executable on the official API 37 emulator with 16 KiB pages. Synthetic checks and two owner-supplied cache fixtures passed with matching on-device hashes. This validates those isolated source functions on current Android; it is not a game build.

**Local agent: start with [the Android 17 results](docs/ANDROID-17-EMULATOR-RESULTS.md), [the older emulator diagnostic](docs/EMULATOR-TEST-2026-10-02.md) and [LOCAL_AGENT_HANDOFF.md](LOCAL_AGENT_HANDOFF.md).** They document the tested Android version, build layout, prior device history and verified workspace-preparation helper. For the independent native-source route, read [RECONSTRUCTION-HANDOFF.md](RECONSTRUCTION-HANDOFF.md).

## Engine source reconstruction — mesh and animation checkpoint

[Browse the mesh/animation C++ module](port/asset-payloads/README.md) and [verified binary layouts](port/asset-payloads/FORMATS.md). The loader decodes all 10,924 type-0 meshes and 890,301 animation time keys in the recovered BRES cache, including the Prince model's deferred buffers. Twenty-six complete animation accessor/search bodies pass [271,970 original-ARM32/compiled-ARM64 comparisons](reports/asset-payloads-arm-validation.json) with zero mismatches; all 456 original instruction addresses were exercised. Separate target checks pass 25,682 comparisons on 1,149 meshes. Host/ARM64 builds and 5,000 sanitizer probes pass. [OBJ and animation JSON examples](port/asset-payloads/examples/README.md) are included. Nine type-1 geometries, scene/controller and full material/effect semantics, GPU ownership, game rendering and gameplay remain unfinished. This checkpoint is published directly on GitHub.

## Engine source reconstruction — resource checkpoint

[Browse the resource-loading C++ module](port/engine-resources/README.md). This adds 35 complete reader/Collada-accessor bodies and the whole-buffer BRES relocation branch, reconstructed from original ARM instructions and compiled for ARM64. [Validation](reports/engine-resources-validation.json) passed 10,449 reader comparisons and every fixup in all 2,901 earlier recovered BRES images: 2,577,206 fixups, with zero mismatches. The port exposes top-level animation/image/material/geometry tables using native pointers while preserving the serialized 32-bit offsets. Later components add mesh/raw-animation decoding, bounded image/material bindings and PVRTC pixels. Game rendering and gameplay remain unfinished. This checkpoint is published directly on GitHub; the existing Drive ZIP remains the earlier math checkpoint.

## Engine source reconstruction — math checkpoint

[Browse the reconstructed C++ module](port/engine-math/README.md). Twenty original vector/quaternion/matrix function starts have been rewritten from ARM assembly and compile for ARM64. The [recorded test report](reports/engine-math-validation.json) contains 21,477 original-ARM32 versus compiled-ARM64 comparisons with zero mismatches, under the documented external arithmetic/libm model. All 1,704 original instruction addresses in these routines were exercised. This is a tested engine component; asset loading, rendering and gameplay reconstruction remain unfinished. The full source ZIP and validation-artifact ZIP in the Drive folder below now include this checkpoint.

## Fold7 compatibility build — test 5

[Download the Test 5 APK and complete work archive](https://github.com/Noamcelermajer/DH_sc/releases/tag/v1.0.2-fold7-test5). The Test 4 phone report confirms its directory guard ran, followed by a new failure reopening the prince character model with a duplicated cache root. An original-engine probe reproduces the bad absolute-path classification. Test 5 changes five ARM instructions to recognize Android absolute paths and records model-open results.

Thirteen path cases, five earlier file cases, native library/hook checks and APK/signature checks pass. **Loading and gameplay still need a Fold7 retest.** Install over Test 4, keep the same options and cache, repeat the load, then export DH2-test5-diagnostics.zip. Russian menus, display behavior and the earlier GL error remain unresolved or unverified.

Read [TEST5.md](compatibility/work/fold7-build/TEST5.md) for the diagnosis, assembly, test limits and device procedure. Test 5 modifies the engine binary; earlier byte-identical-engine statements refer to Tests 1–4.

The complete Test 5 compatibility snapshot is preserved in `compatibility-work-test5.zip`. Prepare a new work directory with current Git source overlaid on that verified snapshot:

```sh
python3 tools/prepare_local_agent.py ../DH2-local-work
```

The helper verifies the archive and runtime inputs, restores the original-library layout expected by the patchers, and leaves the checkout unchanged. The destination must not exist. `unpack_compatibility.py` remains the exact historical restorer; see the handoff before using it with newer files.

Authored source and reports are browsable under `compatibility/`; the standard full-directory ZIP is attached to the release. Previous releases and independent engine reconstruction are retained. External toolchains and duplicate intermediates are represented by versions, hashes and instructions. Original rights and third-party notices apply.

**Download the earlier recovery handoff:** [Google Drive folder](https://drive.google.com/drive/folders/1njTxLAJHt08SinEskn7gC6IWylXemwDW). That separate package contains the Java/smali/decompiler/DWARF/XML recovery, full native assembly/symbol bundles, the JNI source, validation artifacts, the supplied APK and the earlier ten cache parts. Start with START-HERE.txt. This Git checkout contains the engine math/resource/payload/texture/material checkpoints, a host renderer preview and compatibility work. The complete cache ZIP used for the newer audits was supplied separately and is not in this checkout.

This repository contains independently reconstructed port components, selected original-instruction evidence, compatibility work and documentation for the supplied **Dungeon Hunter 2 HD v1.0.2 Android APK**. The full earlier recovery tree and original game inputs are separate materials. The repository is a reviewable starting point for restoring the game on modern Android.

**This is not the original studio source repository or a completed, playable ARM64 port.** Native decompiler output is pseudocode that still requires reconstruction and behavioral validation. A successful export or compilation is not evidence that the game runs.

## What is available

The paths below are in this Git checkout unless marked **external recovery package**. The historical coverage totals describe the supplied APK and external package; they do not mean the complete recovered source tree is committed here. See the [source-availability inventory](docs/SOURCE-AVAILABILITY.md) for the Git-versus-archive accounting.

| Component | Evidence and scope | Location |
| --- | --- | --- |
| Earlier APK recovery | Java/smali, native symbols and pseudocode, DWARF, XML/shaders, JNI source, Android Java repairs and complete native bundles | **External recovery package** in the [Drive handoff](https://drive.google.com/drive/folders/1njTxLAJHt08SinEskn7gC6IWylXemwDW); `recovered/`, `port/nativeinterface/` and `port/android-java/` are absent from this checkout |
| Cache audits | Complete local cache: 6,833 checked files. Earlier ten-part input: 5,839 verified files and 241 directories | [`docs/COMPLETE-CACHE.md`](docs/COMPLETE-CACHE.md); original cache archives are separate inputs |
| Engine math reconstruction | Buildable C++ for 20 original vector/quaternion/matrix routines; 21,477 ARM32/ARM64 comparisons passed | [`port/engine-math`](port/engine-math), [`reports/engine-math-validation.json`](reports/engine-math-validation.json) |
| Resource reconstruction | Buildable C++ for 35 complete reader/accessor bodies plus whole-buffer BRES relocation; all 2,901 recovered BRES files validated | [`port/engine-resources`](port/engine-resources), [`reports/engine-resources-validation.json`](reports/engine-resources-validation.json) |
| Mesh/animation reconstruction | ARM64 C++ decoder, OBJ/raw-key export, 26 animation bodies and 271,970 original-instruction comparisons | [`port/asset-payloads`](port/asset-payloads), [`reports/asset-payloads-arm-validation.json`](reports/asset-payloads-arm-validation.json) |
| External texture decoding | Checked BTEX/PVR, TGA and PNG views; PVRTC RGBA8 pixels matched a PowerVR reference for all 234 cache images; ARM64 build | [`port/texture-assets`](port/texture-assets), [`port/texture-assets/reference-validation.json`](port/texture-assets/reference-validation.json) |
| Image/material bindings | Checked nested BRES effect/material parameters and image indices across all 2,904 BRES files; ARM64 build | [`port/material-bindings`](port/material-bindings), [`port/material-bindings/validation.json`](port/material-bindings/validation.json) |
| Host graphics preview | One candle mesh primitive drawn flat and with its decoded diffuse texture using original unlit GLSL in WebGL1; pixel readback passed | [`port/renderer-preview`](port/renderer-preview/README.md) |
| Compatibility preparation | Snapshot unpacking and local-agent preparation scripts | [`unpack_compatibility.py`](unpack_compatibility.py), [`tools/prepare_local_agent.py`](tools/prepare_local_agent.py) |

Selected original assembly for the reconstructed functions is included in the module reference directories. The full assembly and ELF inventories, decompiler output, Java/smali and recovered configuration/shader exports must be obtained from the separate recovery package; there is no `tools/unpack_native.py` or `recovered/` tree in this checkout.

## Findings that change the restoration plan

1. **The main engine is not stripped.** Its symbol tables preserve gameplay and engine names, virtual tables, relocations, 867 build-source filenames and ARM/Thumb mapping symbols. Assembly recovery round-trips all executable bytes across the three libraries: **6,488,258 bytes**. This is strong binary evidence; it is not C++ source recovery or a runtime test.
2. **DWARF does not contain the gameplay implementation.** All 32 identified compilation units cover licensing/online glue (7), STLport (22) or libgcc (3). The debugging metadata is useful for these components, but cannot substitute for reconstructing the engine's gameplay classes.
3. **The earlier ten-part cache upload was incomplete.** Those parts total exactly 314,572,800 bytes and stop inside `data/sounds/m_world_map.wav`; their ZIP central directory is absent. Files reported from that input passed decompressed-length and CRC-32 checks. A separately supplied complete cache ZIP is now available locally and passed all 6,833 member checks; see [the complete-cache audit](docs/COMPLETE-CACHE.md).
4. **A considerable amount of game data is already readable.** Most module/game-object formats are XML, and the shader package contains original GLSL text. Binary resources include BRES `.bdae`, BTEX/PVR texture wrappers, WAV and VoxN audio. Three original XML files contain duplicate attributes; they are preserved unchanged and documented for parser compatibility.
5. **The supplied APK's provenance is not established as an untouched studio release.** Its manifest has compile-SDK 33 metadata and its DEX contains save-restoration/additional support classes. The repository faithfully documents this supplied binary. Those observations do not establish who repackaged it or what changes were made.
6. **Rebuilding for ARM64 is a separate engineering project.** The three supplied native libraries are ELF32 ARM. Renaming ABI folders, changing target SDK, or compiling the small JNI library cannot turn the engine into a native ARM64 game. Class layouts, pointers, JNI/reflection names, graphics, storage, audio, networking and gameplay must be validated together.

See [`docs/FINDINGS.md`](docs/FINDINGS.md) for evidence interpretation and [`docs/PORTING.md`](docs/PORTING.md) for the remaining work.

## Start reviewing

- The exact input APK SHA-256 is `32c2d027b585a42547311cd95da6a3975fdb3174e663e513d42a7f49d1a4c200`.
- Read [`docs/STATUS.md`](docs/STATUS.md) for generated coverage and test results.
- For behavior, prefer exact smali/assembly over a decompiler's inferred types or control flow. A `.pseudo.c` file deliberately does not claim to compile.
- Inspect [`port/engine-math/README.md`](port/engine-math/README.md) for the math module, original-address mapping and differential-test limits. Floating-point helper metadata incorrectly truncates 16 of 19 vector/quaternion pseudocode bodies; their assembly remains complete.
- Inspect [`port/engine-resources/README.md`](port/engine-resources/README.md) for memory/subfile readers, checked BRES offset loading, Collada table layouts and original-instruction checks. The later payload, texture and material components extend these checked views; game rendering remains unfinished.
- Inspect [`port/asset-payloads/README.md`](port/asset-payloads/README.md) for checked mesh/deferred-buffer decoding, raw animation keys, original keyframe timing/search and small real exports. Its format tables and reports distinguish complete animation functions from partial GPU/ownership evidence.
- Inspect [`port/texture-assets/README.md`](port/texture-assets/README.md) and [`port/material-bindings/README.md`](port/material-bindings/README.md) for the pixel decoder, nested image links, cache audits and ARM64 build limits; see the [renderer preview](port/renderer-preview/README.md) for one host WebGL draw.
- Consult the separate recovery package for the buildable `libnativeinterface.so` component, its differential tests and the Android Java repairs.
- Give the rights holder this repository, the separate recovery package, the exact input APK and the complete original cache. Ask for original engine/build metadata and asset tooling; source-file names and subsystem inventories help focus that search.

The independent reconstruction modules use owner-supplied inputs. The compatibility snapshot is an explicit exception to their earlier packaging policy: it includes original `.so` inputs, the tested runtime bundle, and a deliberately public development signing-key fixture. Complete art/audio cache archives and external SDK/NDK/JDK/emulator toolchains are not included. The local-agent handoff identifies which older recovery-package paths are absent from this checkout. Recovered materials carry their original provenance; this repository does not assert an open-source license for the game. See [`RIGHTS.md`](RIGHTS.md).

