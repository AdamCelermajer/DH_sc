# Original game data corpus scope audit

**Date:** 2026-10-13
**Scope:** Read-only audit of bounded, already-known original-data and staged-original-asset roots for the Original Data Atlas. This is corpus scope, not a claim that every indexed row has been decoded, semantically resolved, or integrated into the reconstruction.

## Finding

`tools/atlas/common.py::source_roots()` currently walks five roots: `assets-extra/android/data`, `assets-extra/ios/data`, `assets-extra/android/original-cache/data`, `assets-extra/android/3d`, and `audio-v34/cache`. `source_files()` additionally includes only `audio-v34/cache/sounds.xml` and the two `items-discovery/sdd_dungeon_hunter_2_iphone_pyarray{,names}.bin` files. These roots cover substantial Android-authored world, actor, effect, animation, script, scene, and table data, plus partial audio caches. They do **not** cover the full supplied game corpus: an extracted iPad app includes 6,145 mapped numbered data packs with logical paths for menus/SWF, font, localized text, scripts, pydata tables, scenes, and 3D assets. Separately, the tracked Android runtime payload contains a provenance-indexed subset of original UI/localization/cache assets.

## Current explicit inputs and observed footprint

Counts below were obtained only by walking the explicit roots named in `common.py` and a few specific candidate roots. Paths are relative to `.local-inputs/` unless labeled `port/`.

| Input | Files / bytes | File types and observed coverage | Provenance / use |
| --- | ---: | --- | --- |
| `assets-extra/android/data` | 6,040 / 249,514,165 B | `.bdae` 2,892; `.mgp` 629; `.mvp` 598; `.mlx` 400; `.mvx` 253; `.mgx` 250; `.tga` 242; `.bin` 219; `.luac` 219; `.lightset_xml` 40; `.xml` 39; `.txt` 22; `.state` 1; `.nomedia` 236. Top-level: `3d/` 5,484; `pydata/` 217; `scene/` 107; `scripts/` 226; plus six root metadata/table files. | `assets-extra/android/MANIFEST.json` says copied from Android device data, game data version `1.0.0B`. Covers characters/mobs, animation, effects, module geometry, textures, camera, scenes, scripts, and game tables including characters, items, skills, effects, sounds, fonts, and quests. |
| `assets-extra/android/original-cache/data` | 152 / 15,717,023 B | `.mgp` 47; `.mvp` 47; `.mgx` 12; `.mvx` 12; `.mlx` 6; `.bdae` 5; plus `.nomedia` and small metadata/text files. | Recovered partial Android cache. Keep as distinct source authority; do not assume it is a complete Android install. |
| `assets-extra/android/3d` | 18 / 100,684 B | 18 `.bdae`, all under `camera/animations/`, including swamp intro camera scenes. | Android extras; source label already `android-extra`. |
| `assets-extra/ios/data` | 256 / 168,025,320 B | `.wav` 239; `.vxn` 14; `.bdae` 3. | `assets-extra/ios/MANIFEST.json` says extracted from `Dungeon_Hunter_2_HD_1.0.0_ios_3.2.ipa`, explicitly an iPad build, bundle version 1.0.0. The three BDAE files are `data/3d/camera/minimapcameras.bdae`, `data/3d/camera/minimapcamerashud.bdae`, and `data/3d/interface/level_up.bdae`; the other 253 are sound banks/cues. This is a partial staged extraction, not the full iPad app corpus. |
| `audio-v34/cache` | 265 / 260,563,243 B | `.wav` 244; `.vxn` 17; `.xml` 2; plus `assets.txt` and `.nomedia`. | Partial audio cache, including `sounds.xml` and `sounds_he.xml`; `common.py` also explicitly admits the English `sounds.xml`. |
| `items-discovery` | 34 / 2,435,963 B | 9 `.bin`, 14 `.json`, 6 `.asm`, 4 `.py`, 1 `.so`. | Mixed reverse-engineering outputs/fixtures and table copies. Do not recursively treat this work directory as a new original-data root. Its copied tables should be provenance/hash-compared with `assets-extra/android/data/pydata`; its SDD pair is explicitly admitted because it is an original table authority. |

## Missing source roots with direct evidence

### Full numbered iPad data packs and logical-name index

The bounded original package is already present at:

- `.local-inputs/claude-preview15/ipa/extracted/Payload/dungeonhunter2iPad.app/data/`
- `.local-inputs/claude-preview15/ipa/extracted/Payload/dungeonhunter2iPad.app/GLIVE_RES/`
- `.local-inputs/claude-preview15/ipa/index_map.tsv`
- `.local-inputs/claude-preview15/ipa/ziplist.tsv`
- `.local-inputs/claude-preview15/ipa/sounds_zip_list.tsv`

The app `data/` directory has 6,157 files / 507,118,559 B: 6,146 numbered `.dat` blobs, one separate `file00000-plantext.dat`, seven `.ttf` fonts, two `.xml` files, and one `.key`. The sibling `index_map.tsv` contains 6,145 rows with two tab-separated columns: packed path (for example `data/file000784.dat`) and logical path (for example `data/3d/animateddecors/candle_flame.bdae`). Its mapped logical categories are 5,257 `3d/`, 333 `text/`, 219 `scripts/`, 215 `pydata/`, 106 `scene/`, eight `menus/`, two `tweaker/`, and one each `gfx/`, `shaders.pak`, `shaders.state`, `sounds/`, and `version.txt`. The mapped menu resources explicitly include `data/menus/dqcharmenu.swf`, `dqhud.swf`, `dqmenus.swf`, `dqmenus2.swf`, `dqmenusapple.swf`, `dqshared.swf`, `loadanims.swf`, and `sct_font_3.fnt`. Localized paths include `text/ingame.english` and the `.english`, `.german`, `.french`, `.italian`, `.spanish`, `.japanese`, and `.korean` text families. `pydata/` includes names such as `character_templates`, `loot_table`, `skills`, `effects`, `fonts`, and `v2quests`.

`ziplist.tsv` is a three-column tab-separated listing (uncompressed size, compressed size, IPA-relative member path). `sounds_zip_list.tsv` has four fields (uncompressed size, compressed size, CRC, internal `_build_iphone/data/sounds/...` path). `index_map.tsv` has 6,145 rows while app `data/` has 6,146 numbered `.dat` files; `file000000.dat` is the sole unmapped numbered blob. Preserve it as `UNRESOLVED` rather than dropping or guessing its logical path. `data/file00000-plantext.dat` is a separate 471,682-byte file and may be useful mapping/source metadata; it has not been interpreted in this audit.

The supplied extraction scripts under `.local-inputs/claude-preview15/ipa/scripts/` are useful provenance, not source content. `export.py` identifies `data/file000787.dat` as `data/sounds/sounds.zip`; `patchzip.py` documents replacing that container's first four bytes with a ZIP local-header signature for reading. `export.py` also documents a header-only first-four-byte BRES restoration for the three staged iOS BDAE files. These are format-specific observations: do not apply either transformation to every `.dat` pack. No full decode was attempted here.

`GLIVE_RES/` has 50 files / 1,178,646 B (49 `.png` plus one `.bar`) of app UI/resource imagery. At the app root there are `response_EN.txt` and other language `response_*.txt` files, and language `.lproj` folders. The `.lproj` folders observed contain `InfoPlist.strings`; their presence alone does not establish that they contain game dialogue/localization. Treat the response files by observed content/provenance before assigning a semantic category.

### Tracked Android original-cache runtime assets

`port/android-native/app/src/main/assets/original-cache/data/` contains 387 files / 88,577,491 B, including 30 `.swf`, 37 files each for the seven language extensions `.english`, `.german`, `.french`, `.italian`, `.japanese`, `.korean`, `.spanish`, plus `.symbols`, `.sc`, fonts, textures, and small table/resource blobs. The adjacent `index.json`, `text-index.json`, and `texture-index.json` include source `cache_member` paths, byte sizes, and SHA-256 values; e.g. entries name the original Android cache member `com.gameloft.android.GAND.GloftD2SS/files/data/...`. This is a useful **provenance-verified runtime subset** and contains original UI/localization files absent from `assets-extra/android/data`. Include it only as an explicitly labeled supplemental source, using the adjacent indexes to preserve origin and deduplicate against other roots by canonical logical path and SHA-256. Do not classify the entire Android APK/build tree as original corpus.

## Repo references versus original files

`port/engine-ui/reference/` holds code/evidence snapshots and feature references; `port/engine-resources/reports/swf-resource-v39/ui-asset-census.json` is a derived census. These are useful references for interpreting consumers and established UI work, but are not the full original-asset corpus. Original SWF/font/text payload evidence is in the tracked Android runtime `original-cache/data/menus`, and additional menu SWFs are listed in the iPad `index_map.tsv`. `port/windows-foundation/features/skill_ui/source_skill_art.json` is a hash/path catalog referencing a particular staged SWF; its asset list is a catalog, not a replacement for source files. Keep reference code/catalogs separate from source rows, while linking them as consumer/evidence references.

## Recommended scoped source inventory

1. Keep the five existing `common.py` roots, with their current provenance labels and partial-cache meaning.
2. Add an explicit `ipad-packed` source rooted only at `.local-inputs/claude-preview15/ipa/extracted/Payload/dungeonhunter2iPad.app/data/`; attach `index_map.tsv` as a logical-name map and `ziplist.tsv` as package-size metadata. Retain both packed container path and mapped original logical path on each row. Preserve unmapped files and map conflicts with explicit unresolved reasons.
3. Add the bounded iPad side roots `GLIVE_RES/`, app-root `response_*.txt`, and per-language `.lproj` files as separate labeled inputs if they belong in the requested inventory; do not include `dungeonhunter2iPad` executable, movies, generic IPA metadata, or the entire extracted app by recursive default.
4. Add `port/android-native/app/src/main/assets/original-cache/data/` only as `port-runtime-original-cache`, with its adjacent JSON indexes required for source-member provenance and hash deduplication. Exclude other `port/` build outputs and test fixtures.
5. Keep `items-discovery` out of recursive source-root discovery. Admit only explicit original tables there, such as the current SDD pair; index its decodes/decompiles/fixtures as evidence or generated intermediates, not as duplicate original game data.
6. Avoid recursive traversal of all `.local-inputs`: it contains numerous per-feature builds, emulator logs, screenshots, output bundles, and copied snapshots. Stay with named corpus roots and explicit maps/manifests. Exclude `.o`, `.so`, `.exe`, `.log`, captures, test/build folders, and source copies unless a narrow provenance manifest identifies them as actual original payload.

## Version and coverage boundaries

The Android staging manifest identifies version `1.0.0B`; the iPad manifest identifies an iPad `1.0.0` build. Same logical names do not establish byte or behavior parity. Preserve platform/version labels, compare hashes and format-specific headers per logical path, and mark differences rather than choosing a winner. The iPad native executable is not Android or iPhone semantic evidence; the Android APK/ELF IDA export remains the stated native authority. This audit does not prove that no other original corpus exists outside the bounded roots listed here, and it does not claim the packed iPad files have been semantically decoded.
