# Source availability and restoration inventory

This inventory distinguishes source that can be browsed in Git from source-like evidence stored in archives or supplied separately. The counts below describe the audited Git baseline `15dafc0` (2026-10-02); newer pull-request commits may add files. See [RIGHTS.md](../RIGHTS.md) for provenance.

## In the Git checkout

At the audited baseline, Git tracks **309 paths**: 225 under `compatibility/`, 55 under `port/`, 13 under `reports/`, and the remaining documentation, tools and four root-level snapshot ZIPs. The directly browsable independent C++ components are `port/engine-math/`, `port/engine-resources/`, `port/asset-payloads/`, `port/texture-assets/` and `port/material-bindings/`. Their READMEs and test reports specify which isolated behavior was reconstructed and checked. The compatibility tree contains authored wrapper, patch, test and selected Java/C/C++ sources. It is a development wrapper around the original ARM32 engine, not a source replacement for that engine.

The following recovery-package paths are **absent** from the audited Git tree: `recovered/`, `port/nativeinterface/`, `port/android-java/`, `tools/unpack_native.py` and `docs/REPRODUCING.md`. No `.smali`, `.pseudo.c` or `.glsl` files are directly tracked at that baseline. Older references to those paths describe the separate recovery package or material within the tracked compatibility snapshot; they are not links to browsable files in this checkout.

## Tracked compatibility snapshots

The repository tracks `compatibility-work-test2.zip` through `compatibility-work-test5.zip`. They are content-addressed snapshots rather than ordinary source directories. Test 5's 16,454,234-byte ZIP has a manifest for **2,195 logical files** stored as **1,194 deduplicated blobs** plus the manifest. Its logical paths include 357 decompiled Java files (about 1.72 MB of text), original and patched smali trees, original ARM32 libraries, compatibility build inputs and test evidence. The Java is decompiler output; the snapshot does not contain the full native engine pseudocode and assembly recovery described below.

To restore Test 5 into a new sibling directory, from the repository root run:

```sh
python tools/prepare_local_agent.py ../DH2-local-work
```

The destination must not exist and must be outside the checkout. The helper verifies the pinned snapshot hash and each restored file, overlays current tracked compatibility sources, restores the original-library layout expected by the patch tools and checks the bundled runtime. It does not build or run the game. See [LOCAL_AGENT_HANDOFF.md](../LOCAL_AGENT_HANDOFF.md) for the build and test context.

## Separate owner-supplied inputs and older recovery package

The locally supplied original APK (`Dungeon-Hunter-2-HD-v1-0-2.apk`, SHA-256 `32c2d027b585a42547311cd95da6a3975fdb3174e663e513d42a7f49d1a4c200`) and complete cache ZIP (`Dungeon-Hunter-2-HD-v1-0-2-cache (1).zip`, SHA-256 `3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`) are separate local inputs, not tracked files. The cache contains 6,833 CRC-checked files; see [the cache audit](COMPLETE-CACHE.md).

[The earlier recovery handoff](../RECONSTRUCTION-HANDOFF.md) describes a separate [Drive folder](https://drive.google.com/drive/folders/1njTxLAJHt08SinEskn7gC6IWylXemwDW) containing `Dungeon-Hunter-2-Source-Recovery.zip`, `assembly.tar.gz`, `symbols.tar.gz`, `Component-Validation-Artifacts.zip` and checksums. That package documents the full Java/smali export, Ghidra native pseudocode, original assembly/symbol evidence, DWARF, recovered XML/shaders, JNI source and Android Java repairs. It was published through the engine-math checkpoint; the later resource, payload, texture and material modules are in Git. This audit did not download or reverify the Drive package, and those package files were not present alongside the two local original inputs.

The available pseudocode and decompiled Java are reverse-engineered evidence, not original studio source. The isolated reconstructed modules and compatibility wrapper do not yet form a buildable, playable full game. [RIGHTS.md](../RIGHTS.md) records provenance and does **not** grant a game-wide open-source license.
