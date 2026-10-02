# Source availability and restoration inventory

This inventory distinguishes source that can be browsed in Git from source-like evidence stored in archives or supplied separately. The baseline counts below describe commit `15dafc0` (2026-10-02). The current branch also includes a selected recovery-source import described below. See [RIGHTS.md](../RIGHTS.md) for provenance.

## In the Git checkout

At the audited baseline, Git tracks **309 paths**: 225 under `compatibility/`, 55 under `port/`, 13 under `reports/`, and the remaining documentation, tools and four root-level snapshot ZIPs. The directly browsable independent C++ components now include `port/engine-math/`, `port/engine-resources/`, `port/asset-payloads/`, `port/texture-assets/`, `port/material-bindings/` and `port/scene-payloads/`. Their READMEs and test reports specify which isolated behavior was reconstructed and checked. The compatibility tree contains authored wrapper, patch, test and selected Java/C/C++ sources. It is a development wrapper around the original ARM32 engine, not a source replacement for that engine.

At that baseline, `recovered/`, `port/nativeinterface/`, `port/android-java/`, `tools/unpack_native.py` and `docs/REPRODUCING.md` were absent. A later source import added the browsable Java/JNI source, recovery scripts, reproduction guide and a checksum manifest at `recovered/native/bundles/manifest.json`. It did **not** add original smali, raw native pseudocode, full assembly/symbol records, XML/shader exports or original APK/cache files.

## Verified recovery-source import on this branch

The linked historical Drive source ZIP was retrieved and passed SHA-256 and full ZIP CRC checks: 17,036,838 compressed bytes, 3,388 members, 79,687,588 uncompressed bytes, SHA-256 `b3ff974e2b74f50387465d5665f60d56ac79c29a449c6299745461998045c4d8`. Its two separate bundle hashes also matched the handoff manifest. `tools/unpack_native.py` restored 3,625 assembly and 71 symbol evidence files **outside Git**. `tools/verify_recovery.py` then rechecked every named function start and export shard, 2,164 recovered text/shader files and 288 repaired Java source files.

Selected files were imported as ordinary Git paths: 288 repaired Java sources in [`port/android-java`](../port/android-java/README.md), the 14-file reconstructed [`port/nativeinterface`](../port/nativeinterface/README.md) component, authored recovery/build tools and the required source hashes and reports. The [import ledger](RECOVERY-SOURCE-IMPORT.md) records each source and local SHA-256. Run `python tools/verify_recovery_import.py` to check the files in a clone; pass `--archive` with the downloaded ZIP to check provenance against the exact handoff. These are reconstructed/decompiled materials, not original studio source or a completed game.

## Tracked compatibility snapshots

The repository tracks `compatibility-work-test2.zip` through `compatibility-work-test5.zip`. They are content-addressed snapshots rather than ordinary source directories. Test 5's 16,454,234-byte ZIP has a manifest for **2,195 logical files** stored as **1,194 deduplicated blobs** plus the manifest. Its logical paths include 357 decompiled Java files (about 1.72 MB of text), original and patched smali trees, original ARM32 libraries, compatibility build inputs and test evidence. The Java is decompiler output; the snapshot does not contain the full native engine pseudocode and assembly recovery described below.

To restore Test 5 into a new sibling directory, from the repository root run:

```sh
python tools/prepare_local_agent.py ../DH2-local-work
```

The destination must not exist and must be outside the checkout. The helper verifies the pinned snapshot hash and each restored file, overlays current tracked compatibility sources, restores the original-library layout expected by the patch tools and checks the bundled runtime. It does not build or run the game. See [LOCAL_AGENT_HANDOFF.md](../LOCAL_AGENT_HANDOFF.md) for the build and test context.

## Separate owner-supplied inputs and older recovery package

The locally supplied original APK (`Dungeon-Hunter-2-HD-v1-0-2.apk`, SHA-256 `32c2d027b585a42547311cd95da6a3975fdb3174e663e513d42a7f49d1a4c200`) and complete cache ZIP (`Dungeon-Hunter-2-HD-v1-0-2-cache (1).zip`, SHA-256 `3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`) are separate local inputs, not tracked files. The cache contains 6,833 CRC-checked files; see [the cache audit](COMPLETE-CACHE.md).

[The earlier recovery handoff](../RECONSTRUCTION-HANDOFF.md) describes a separate [Drive folder](https://drive.google.com/drive/folders/1njTxLAJHt08SinEskn7gC6IWylXemwDW) containing `Dungeon-Hunter-2-Source-Recovery.zip`, `assembly.tar.gz`, `symbols.tar.gz`, `Component-Validation-Artifacts.zip` and checksums. It was published through the engine-math checkpoint; later resource, payload, texture and material modules are in Git. The ZIP and bundles were retrieved and verified locally for the selective import, but remain outside Git. Their raw Java/smali, native pseudocode, original assembly/symbol evidence, DWARF and XML/shaders can be examined from the external archive.

The available pseudocode and decompiled Java are reverse-engineered evidence, not original studio source. The isolated reconstructed modules, including the checked scene reader, and compatibility wrapper do not yet form a buildable, playable full game from source. [RIGHTS.md](../RIGHTS.md) records provenance and does **not** grant a game-wide open-source license.
