# Mesh and animation payload reconstruction

This component reads real Dungeon Hunter 2 BRES geometry and animation data using immutable, checked C++ views. It builds for the host and Android ARM64, exports local-space triangle meshes to OBJ and raw animation keys to JSON, and reproduces the original key-time and keyframe-search behavior. The input image must remain alive and unchanged while its views are used.

It adds **18 complete animation accessor bodies and 8 complete typed search bodies** reconstructed from ARM instructions. Mesh layouts, deferred buffer selection and inner animation relocation are traced through four additional original routines; their GPU construction, allocation and reference counting are **not** reconstructed. The new interfaces deliberately differ from the original ARM32 class ABI. See [original-functions.json](original-functions.json), [complete assembly evidence](reference/original-functions.asm) and [binary layouts](FORMATS.md).

## Validated results

| Check | Result | Evidence |
| --- | --- | --- |
| Original ARM32 versus compiled ARM64 animation access/search | 271,970 comparisons, zero mismatches; all 456 instruction addresses in the 26 complete ranges executed | [ARM report](../../reports/asset-payloads-arm-validation.json) |
| Full recovered BRES cache | 2,901 files; 10,924 meshes; 1,641,664 vertices; 3,265,266 indices / 1,088,422 triangles | [Cache report](../../reports/asset-payloads-cache-validation.json) |
| Animation payloads | 49,060 animation records, 3,732 segments, 82,880 sampler/segment vector pairs and 890,301 time keys decoded and checked | Same cache report |
| Host / Android ARM64 compilation | Both pass with strict floating-point evaluation and 16 KiB target load alignment | [Build report](../../reports/asset-payloads-build.json) |
| Corrupted / truncated BRES inputs | 5,000 host ASan/UBSan probes pass; leak detection disabled because of the execution environment | Same build report |
| Host versus executed ARM64 mesh decoder | Real mesh metadata, pointers, vertex samples and index samples agree, including the Prince deferred buffers | [ARM64 mesh report](../../reports/asset-payloads-arm64-mesh-validation.json) |

The full-cache audit checks every mesh/index range and every animation time key. It checks finiteness of all 19,856,116 vertex components and 3,244,961 animation components, with 90,964 vertex, 23,924 index and 140,511 animation scalar-reader sample comparisons. Counts include repeated assets across files.

The original-instruction animation test samples the first/middle/last animation and first/last segment of each animation-bearing cache file. It covers 6,984 real animation/segment views plus nine synthetic fixtures, including both data-storage modes, integer/compressed times, step interpolation, duplicate keys, optional scale records, mixed sampler types and large signed times. Imported compiler arithmetic/conversion/comparison helpers and libc use an explicit host dependency model. Engine algorithms execute actual instructions. This does not establish historical compiler-helper behavior outside the tested domains or device gameplay.

## Behaviors preserved

- Compressed key-time getters multiply frames by the exact double constant `33.333332` and truncate. Search uses the original float constant and evaluation order, which can produce a different millisecond boundary.
- Generic time access and search select sampler **zero's** time type, even for another sampler. The mixed-type fixture checks this quirk.
- Search chooses the last key at or before its float query. Its equality test returns false on an exact key and on the last key. A query before the first key can return true; the fraction overload clamps its negative fraction to zero.
- A zero interpolation enum disables the fraction path. When that path is inactive the original leaves the caller's fraction unchanged; the port does too.
- Frame selection and fraction arithmetic retain 32-bit signed wrap where the assembly performs integer subtraction. Invalid indices and unrepresentable float-to-int conversions are rejected safely.
- Animation entry pointers are relative to their **pointer word**, not to the BRES base. The decoder resolves them without modifying or widening the input.
- `prince_modular.bdae` alone uses deferred mesh buffers in this corpus. Its 173 meshes reference 16-byte on-demand records rather than immediate vertex/index bytes. Complete-file offsets are resolved without recreating allocator or GPU ownership.

## Build and reproduce

From this directory, with Python 3.12, a host C++17 compiler and the owner's exact cache and original ELF:

```sh
python -m pip install -r requirements.txt
python build.py --ndk /path/to/android-ndk-r29

python tools/export_assets.py /path/to/candle_flame.bdae \
  --obj candle-flame.obj --animation-json candle-flame.animations.json

python tests/audit_cache.py --cache /path/to/cache/files \
  --report cache-validation.json
python tests/differential.py --original /path/to/libDungeonHunter2.so \
  --cache /path/to/cache/files --report arm-validation.json
python tests/arm64_mesh.py --cache /path/to/cache/files \
  --report arm64-mesh-validation.json
ASAN_OPTIONS=detect_leaks=0 python build.py --ndk /path/to/android-ndk-r29 \
  --sanitize /path/to/candle_flame.bdae --report build-validation.json
```

`ANDROID_NDK_HOME` can replace `--ndk`. The build reuses the sibling `engine-resources` source; the ARM test reuses its ELF/Unicorn harness. Evidence regeneration additionally needs the restored original symbol/assembly export tree and `tools/capture_provenance.py --original ... --workspace /path/to/recovery`.

[Small real exports](examples/README.md) demonstrate two candle-flame meshes (four triangles) and both animation tracks. Exporting the main-menu swamp resource produced three meshes and 2,146 triangles. OBJ preserves local coordinates, material names, normals, first UV set and triangle winding; it does not apply scene transforms or create textures/material files. JSON preserves raw component values, including byte outputs, without guessing track-specific normalization.

## Remaining engine work

Nine type-1 geometry records are explicitly rejected and listed in the cache report. All decoded meshes use interleaved streams; separate-stream meshes and already-relocated/runtime-mutated images are unsupported. OBJ currently requires triangle lists. Other primitive map values are retained as engine enums, not mistaken for GL enums.

Default/scale getters expose checked borrowed addresses; their variant payloads are not interpreted. Scene hierarchy, controllers, skin weights, skeletons, track-specific value interpolation/application, cached generic search dispatch, clips, material/image decoding, GPU buffers and rendering still need reconstruction. Deferred bytes are supported only when contained in the complete image; external/split-file loading and the original string/shared ownership ABI remain unfinished. No Android device execution or integrated game build was performed for this module. It is a usable asset and animation foundation, not a complete engine or playable rebuilt APK.
