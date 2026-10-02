# Dungeon Hunter 2 — source reconstruction

**New local input, 2026-10-02:** a separately supplied 433,189,197-byte cache ZIP passed full member CRC/length checks and contains 6,833 files. See [the complete-cache audit](docs/COMPLETE-CACHE.md). Earlier ten-part recovery figures below describe a different, truncated input.

**Current Android 17 check:** The [source-built Android renderer](port/android-app/README.md) installed on the official API 37.2, 16 KiB page emulator and drew a real candle mesh with its decoded PVRTC texture; it is an asset preview, not gameplay. A separate, self-contained ARM64 compatibility APK installed the verified guest and complete cache on Android 17. On the API 37.0 4 KiB emulator, a clean install of the final Test 10 build displayed a full-width menu, entered a saved 3D level, responded to attack-button taps with a nearby enemy disappearing, and moved the player and camera after a joystick swipe. Pause/return updated the visible save time; a force-stop/cold relaunch kept that timestamp, reentered the saved level, and accepted another movement input. The standard compatibility build retains its page-size guard on the 16 KiB image, but a separate strict experimental translator build on the API 37.2 16 KiB emulator reached a saved 3D level, moved the player/camera, and returned through pause to an updated save menu. Both game APKs still run the original ARM32 engine through a translator; extended play, all levels, physical devices and complete save/reload semantics remain unverified. See [the Android 17 results](docs/ANDROID-17-EMULATOR-RESULTS.md). No Fold7 device testing was done for this update.

## Engine source reconstruction — texture, material and render preview checkpoint

[Texture views and PVRTC decoding](port/texture-assets/README.md) classify all 363 external cache textures and decode the 234 BTEX/PVRTC images to RGBA8. Every decoded pixel in those 234 images matched the official PowerVR software decoder; the revised component also builds for Android ARM64. [Checked image/material views](port/material-bindings/README.md) cover nested BRES effect/material parameters and image-index links across all 2,904 BRES files. They resolve 3,854 local effect IDs and also build for Android ARM64. These are isolated source components; the ARM64 builds have not been loaded on an Android device.

[Checked scene hierarchy views](port/scene-payloads/README.md) now follow visual-scene references, traverse nodes, compose static world matrices and classify geometry instances across the complete 2,904-file BRES cache. The audit traversed 21,472 nodes and 11,648 instances; 10,403 of 10,444 local geometry links resolved. Host and ARM64 builds plus 16 focused safety and ordering checks pass. Animation, rendering and gameplay are still separate work.

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

Thirteen path cases, five earlier file cases, native library/hook checks and APK/signature checks pass. The historical Test 5 device instructions are preserved in `TEST5.md`; this Android 17 effort uses the official emulator and did not test a Fold7. Russian menus, display behavior and the earlier GL error remain unresolved or unverified.

Read [TEST5.md](compatibility/work/fold7-build/TEST5.md) for the diagnosis, assembly, test limits and device procedure. Test 5 modifies the engine binary; earlier byte-identical-engine statements refer to Tests 1–4.

The complete Test 5 compatibility snapshot is preserved in `compatibility-work-test5.zip`. Prepare a new work directory with current Git source overlaid on that verified snapshot:

```sh
python3 tools/prepare_local_agent.py ../DH2-local-work
```

The helper verifies the archive and runtime inputs, restores the original-library layout expected by the patchers, and leaves the checkout unchanged. The destination must not exist. `unpack_compatibility.py` remains the exact historical restorer; see the handoff before using it with newer files.

Authored source and reports are browsable under `compatibility/`; the standard full-directory ZIP is attached to the release. Previous releases and independent engine reconstruction are retained. External toolchains and duplicate intermediates are represented by versions, hashes and instructions. Original rights and third-party notices apply.

**Browse the verified recovery-source import:** The repaired [Android Java](port/android-java/README.md), reconstructed [JNI component](port/nativeinterface/README.md), authored recovery scripts and reports are ordinary Git files. [The first import ledger](docs/RECOVERY-SOURCE-IMPORT.md) maps 333 selected files to the checksum-verified older source ZIP. The same verified ZIP now also supplies 44 browsable [native pseudocode/index files](recovered/native/decompiled/README.md) and 721 [raw Java/smali code exports](recovered/android/README.md), each with per-file hashes. These generated exports are archival evidence, not a buildable game. The separate [Google Drive handoff](https://drive.google.com/drive/folders/1njTxLAJHt08SinEskn7gC6IWylXemwDW) still holds DWARF/XML/shader exports and the full native assembly/symbol bundles, plus original inputs and validation artifacts. Start with `START-HERE.txt` there. The complete cache ZIP used for newer audits was supplied separately and is not in this checkout.

This repository contains independently reconstructed port components, selected original-instruction evidence, archived code exports, compatibility work and documentation for the supplied **Dungeon Hunter 2 HD v1.0.2 Android APK**. The original game inputs and some recovery evidence remain separate materials. The repository is a reviewable starting point for restoring the game on modern Android.

**This is not the original studio source repository or a completed, playable source-built ARM64 port.** Native decompiler output is pseudocode that still requires reconstruction and behavioral validation. A successful export or compilation is not evidence that the game runs.

## What is available

The paths below are in this Git checkout unless marked **external recovery package**. The historical coverage totals describe the supplied APK and external package; they do not mean the complete recovered source tree is committed here. See the [source-availability inventory](docs/SOURCE-AVAILABILITY.md) for the Git-versus-archive accounting.

| Component | Evidence and scope | Location |
| --- | --- | --- |
| Repaired Android Java and reconstructed JNI | 288 hash-checked Java sources, buildable JNI component, recovery/build scripts and provenance ledger | [`port/android-java`](port/android-java/README.md), [`port/nativeinterface`](port/nativeinterface/README.md), [import ledger](docs/RECOVERY-SOURCE-IMPORT.md) |
| Archived code exports | 44 native pseudocode/index files and 721 raw Java/smali files copied byte-for-byte from the verified recovery ZIP; generated, not buildable studio source | [`recovered/native/decompiled`](recovered/native/decompiled/README.md), [`recovered/android`](recovered/android/README.md) |
| External recovery evidence | DWARF, XML/shaders, full native assembly/symbol bundles and original inputs | [Drive handoff](https://drive.google.com/drive/folders/1njTxLAJHt08SinEskn7gC6IWylXemwDW) |
| Cache audits | Complete local cache: 6,833 checked files. Earlier ten-part input: 5,839 verified files and 241 directories | [`docs/COMPLETE-CACHE.md`](docs/COMPLETE-CACHE.md); original cache archives are separate inputs |
| Engine math reconstruction | Buildable C++ for 20 original vector/quaternion/matrix routines; 21,477 ARM32/ARM64 comparisons passed | [`port/engine-math`](port/engine-math), [`reports/engine-math-validation.json`](reports/engine-math-validation.json) |
| Resource reconstruction | Buildable C++ for 35 complete reader/accessor bodies plus whole-buffer BRES relocation; all 2,901 recovered BRES files validated | [`port/engine-resources`](port/engine-resources), [`reports/engine-resources-validation.json`](reports/engine-resources-validation.json) |
| Mesh/animation reconstruction | ARM64 C++ decoder, OBJ/raw-key export, 26 animation bodies and 271,970 original-instruction comparisons | [`port/asset-payloads`](port/asset-payloads), [`reports/asset-payloads-arm-validation.json`](reports/asset-payloads-arm-validation.json) |
| External texture decoding | Checked BTEX/PVR, TGA and PNG views; PVRTC RGBA8 pixels matched a PowerVR reference for all 234 cache images; ARM64 build | [`port/texture-assets`](port/texture-assets), [`port/texture-assets/reference-validation.json`](port/texture-assets/reference-validation.json) |
| Image/material bindings | Checked nested BRES effect/material parameters and image indices across all 2,904 BRES files; ARM64 build | [`port/material-bindings`](port/material-bindings), [`port/material-bindings/validation.json`](port/material-bindings/validation.json) |
| Scene hierarchy | Checked node traversal, static world matrices and geometry-instance references across all 2,904 BRES files; host and ARM64 builds | [`port/scene-payloads`](port/scene-payloads/README.md), [`validation`](port/scene-payloads/validation.json) |
| Host graphics preview | One candle mesh primitive drawn flat and with its decoded diffuse texture using original unlit GLSL in WebGL1; pixel readback passed | [`port/renderer-preview`](port/renderer-preview/README.md) |
| Android 17 source renderer | Signed target-SDK-37 APK built from reconstructed asset readers; rendered a real textured candle on the 16 KiB emulator; no gameplay | [`port/android-app`](port/android-app/README.md), [`runtime validation`](port/android-app/runtime-validation.json) |
| Compatibility preparation | Snapshot unpacking and local-agent preparation scripts | [`unpack_compatibility.py`](unpack_compatibility.py), [`tools/prepare_local_agent.py`](tools/prepare_local_agent.py) |

Selected original assembly for the reconstructed functions is included in the module reference directories. The full assembly and ELF inventories, raw decompiler output, original Java/smali and recovered configuration/shader exports must be obtained from the separate recovery package. The checked-in `recovered/native/bundles/manifest.json` contains hashes only; the bundle archives are external. See [reproduction steps](docs/REPRODUCING.md) before using `tools/unpack_native.py`.

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
- Inspect [`port/scene-payloads/README.md`](port/scene-payloads/README.md) for scene/node traversal, geometry-instance links, malformed-input checks and remaining opaque instance types.
- Review the [reconstructed JNI](port/nativeinterface/README.md), [repaired Android Java](port/android-java/README.md) and [import ledger](docs/RECOVERY-SOURCE-IMPORT.md) directly in Git. Obtain the original bytecode, pseudocode and full instruction evidence from the separate recovery package.
- Give the rights holder this repository, the separate recovery package, the exact input APK and the complete original cache. Ask for original engine/build metadata and asset tooling; source-file names and subsystem inventories help focus that search.

The independent reconstruction modules use owner-supplied inputs. The compatibility snapshot is an explicit exception to their earlier packaging policy: it includes original `.so` inputs, the tested runtime bundle, and a deliberately public development signing-key fixture. Complete art/audio cache archives and external SDK/NDK/JDK/emulator toolchains are not included. The [source inventory](docs/SOURCE-AVAILABILITY.md) identifies which earlier recovery-package paths remain external. Recovered materials carry their original provenance; this repository does not assert an open-source license for the game. See [`RIGHTS.md`](RIGHTS.md).

