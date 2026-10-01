# Checked image and material record bindings

This module adds immutable C++17 views over the BRES image (20-byte), effect (116-byte) and material (36-byte) records. It uses the existing checked BRES container reader. The views expose the observed image ID/name/source-path strings and material ID/name/external-effect-file/effect-URL strings. An effect URL beginning with `#` is resolved against effect IDs in the same BRES file when the material has no external-effect filename. Other material and effect words remain raw because their shader/state meaning has not been established.

The views borrow the original BRES bytes and never rewrite four-byte serialized offsets into native pointers. They are **new port interfaces**, not recovered studio classes or a playable renderer. The local-effect lookup is a corpus-grounded string relationship; it has not yet been compared against original ARM instructions. External effect filenames are reported by basename only, without asserting how the original loader selects among matching paths.

## Evidence from the complete local cache

The report at [`reports/material-bindings-complete-cache.json`](../../reports/material-bindings-complete-cache.json) was generated from all 2,904 BRES files in the separately supplied valid ZIP. All 3,662 image records, 3,873 effect records and 4,329 material records passed checked string access. Of the material URLs, 3,854 resolve to local effect IDs and 475 name an external effect file; all 475 external basenames have a case-insensitive match somewhere in the cache. This does not establish the runtime search path or effect behavior.

Among 3,662 image source paths, 3,444 filenames match a cached texture directly. Another 42 unmatched references have a `pvr2_` prefixed filename candidate. The remaining references need path/alias investigation; a filename match alone does not verify texture loading, and the audit does not silently substitute an alias. The report lists representative missing names. Both trailing 32-bit words of every image record are zero in this corpus, so this module retains them as raw values instead of assigning speculative meanings.

## Build and repeat

Use a C++17 host compiler, Python 3.10 or later and a private extracted cache directory containing `data/3d/textures` and the BRES files:

```sh
python port/material-bindings/build.py --output /tmp/libdh2_material_bindings.so
python port/material-bindings/tests/check.py \
  --library /tmp/libdh2_material_bindings.so \
  --fixture /path/to/cache/files/data/3d/animateddecors/candle_flame.bdae
python port/material-bindings/tools/audit_cache.py \
  --library /tmp/libdh2_material_bindings.so \
  --cache /path/to/cache/files \
  --report /tmp/material-bindings-report.json
```

For Windows, use a `.dll` output filename. The report is an asset-structure check, not a pixel, GPU, Android or gameplay test. The next work is to trace the remaining material/effect fields, image bindings and runtime aliases against original instructions and actual rendering.

For an Android ARM64 component build with NDK r29, add `--ndk /path/to/android-ndk-r29 --arm64-output /path/to/libdh2_material_bindings_arm64.so` to the build command. This command passed with NDK r29 and produced 16 KiB-aligned load segments ([build evidence](../../reports/new-components-arm64-build.json)). It compiles the isolated view library; it does not integrate it into an APK.
