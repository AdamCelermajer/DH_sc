# Checked image and material record bindings

This C++17 module adds immutable views over BRES image (20-byte), effect (116-byte) and material (36-byte) records using the checked BRES container reader. It exposes image ID/name/source-path strings, material ID/name/effect references, nested parameters and serialized image-index links. An effect URL beginning with `#` is resolved against effect IDs in the same BRES file when the material has no external-effect filename.

The views borrow the original BRES bytes and never rewrite four-byte serialized offsets into native pointers. They are **new port interfaces**, not recovered studio classes or a playable renderer. External effect filenames are reported by basename only; runtime search order and shader behavior remain unknown.

## Observed nested layout

| Record | Offsets and checked content |
| --- | --- |
| Effect, group 0 | `+8/+12`: count/offset of 12-byte named records; `+16/+20`: 24-byte parameters; `+24/+28`: four-byte image indices |
| Effect, group 1 | The same three pairs at `+32/+36`, `+40/+44`, and `+48/+52` |
| Effect parameter | `+0`: ID string; `+4`: numeric type code; `+8`: unresolved numeric word; `+12`: value count; `+16/+20`: offsets to type-specific metadata and data |
| Material | `+16/+20`: count/offset of 24-byte parameters |
| Material parameter | `+0/+4`: ID and semantic strings; `+8`: type code; `+12`: value count; `+16/+20`: offsets to count header and type-specific data |

For material type code 11, the value data holds an offset to a four-byte image index. `0xffffffff` means unbound. `dh2_material_sampler_image` resolves a valid index to the corresponding checked BRES image record. `dh2_effect_group_image` does the same for an effect group's index array. The two effect groups are numbered 0 and 1 because their runtime selection has not been established. The 12-byte named records and all other type-specific payloads stay opaque. Nine Prince effect parameter records use metadata at `+16` that differs from the usual count word, so the effect view does not impose the material count-header rule.

The exact APK's `libDungeonHunter2.so` was checked against the local copy (SHA-256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`). Its symbol table names `CColladaDatabase::constructEffect(..., SEffect*, ...)` at `0x0060e570` and `constructMaterial(..., SMaterial*, ...)` at `0x0061cca8`. These identify original code paths still needing an instruction-level field trace. The nested layout above is established by serialized fixups, record spans and full-cache consistency checks, not by those symbol names alone.

## Evidence from the complete local cache

The expanded [validation report](validation.json) covers all 2,904 BRES files in the separately supplied valid ZIP. All 3,662 image records, 3,873 effect records and 4,329 material records passed checked access. The new views checked 60,906 effect parameters, 49,902 material parameters and 9,625 effect image references. They resolved 5,979 material sampler indices and found 710 explicit unbound sentinels. For every one of the 3,854 materials linked to an effect in the same file, the material sampler-index set matches **both** effect-group image-index sets (7,708/7,708 group comparisons). This is strong structural evidence for the index relationship; it does not establish when or how the renderer uses either group. The other 475 material URLs name an external effect file; every basename has a case-insensitive match somewhere in the cache, but runtime search order remains unknown.

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

For Windows, use a `.dll` output filename. The report is an asset-structure check, not a pixel, GPU, Android or gameplay test. The next work is to trace field use and runtime aliases against original ARM instructions and actual rendering.

For an Android ARM64 component build with NDK r29, add `--ndk /path/to/android-ndk-r29 --arm64-output /path/to/libdh2_material_bindings_arm64.so` to the build command. This revision builds with the official Windows NDK r29, exports all five new entry points, and has two `PT_LOAD` segments aligned to 16 KiB ([build evidence](arm64-build.json)). The host boundary checks and full-cache audit were repeated after the cross-build. It compiles an isolated view library, not an integrated APK.
