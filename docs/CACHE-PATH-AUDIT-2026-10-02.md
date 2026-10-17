# Android 9 cache path audit — 2026-10-02

The Android 9 x86 emulator's directly installed Test 6 guest reached the game menus using the supplied 6,833-file cache. Its captured log contains seven `Could not open file` messages for four unique texture paths. Every requested path begins with `qata/3d/textures/`; the cache has no `qata/` directory. All four exact filename and suffix counterparts exist under `data/3d/textures/`, each 524,348 bytes long. The complete request-to-candidate mapping and source file hashes are in [`reports/emulator-test6-texture-path-audit.json`](../reports/emulator-test6-texture-path-audit.json).

The requested texture filenames are:

- `atlas_skinned_characters_animdecor_gameobjects_001.tga`
- `atlas_modular_character_01.tga`
- `atlas_weapons_dh2.tga`
- `atlas_fx_particles_002.tga`

This is evidence of a path spelling error during those opens, not evidence that the texture assets are absent. The original APK's native libraries do not contain the literal `qata` byte sequence; the emulator log alone does not establish where the spelling changes in memory. A failure-only loader retry may resolve these opens, but that requires a separate runtime test. The path audit does not establish why level loading remains at 31%, or whether these texture failures cause it.

The read-only audit can be repeated against a later log:

```text
python tools/audit_missing_cache_paths.py --cache PATH_TO_EXTRACTED/files --log PATH_TO_LOGCAT.txt --report reports/cache-path-audit.json
```

The tool reads local assets for existence, size and SHA-256. It neither copies cache files nor creates aliases in the repository. The log and supplied cache are local test inputs, not committed game assets.
