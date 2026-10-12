# All-map material image dependencies V44

The audit covers all 51 design identities from the latest chapter-coverage-v36 source inventory. It preserves fixed authored declarations and procedural rule candidates as different scopes. It evaluates exact Module BRES node selectors using the selected native module_selected_scene_v2 producer. AnimatedDecor/Character inspection assets use the authored asset scene; their actual actor selector, activation, helper suppression and render-pass registration remain external.

The native ASAN/UBSAN audit read the complete 6,833-entry original archive, examined 84 distinct BRES resources through 305 asset/Module-selector requests, and compared 2,779 material/sampler identities against an independent bounded read of the original BRES bytes. No BRES open rejection occurred. Seven resources contain nongeometry/effect/invisible source data outside the current mesh Scene acceptance domain; their source material catalog is recorded explicitly without invented geometry instances. One exact authored Module selector is absent from the current selected source search. These are source findings, not selected-seed/runtime map failures.

The normalized full matrix retains every source object/module occurrence, exact asset+selector identity, authored BRES instance material-slot binding, material ID/catalog and ordered sampler/image record/full authored URI. The joins reconstruct 92,005 source instance/material/sampler rows for the level declarations: 57,232 resolved under the explicit supplied-cache compatibility rule, 34,385 source UINT32_MAX no-image entries and 388 references to five absent exact compatibility URIs. Those counts include repeated authored occurrences and procedural potential modules. They are not unique texture allocations or live objects.

All five absent exact URIs have existing same-directory pvr2_ variants. Their byte identities are preserved as diagnostic candidates only. This audit does not replace the original format/mount resolver with a guessed prefix, basename search, first-image fallback or global filename match. Genuinely absent exact archive entries are distinguished from absent textures: none of the five supports a claim that the required texture is unavailable to the original engine.

SWAMP has 57 static visual dependency declarations, 3,690 joined sampler rows, 2,855 compatibility resolutions and 835 source no-image entries; it has no absent exact texture URI in this scope. Three declared swamp effect occurrences use the nongeometry source domain. The five chest/50 Character V40 real-factory inspection model references are kept in a separate inspection-only section. The 68 SWAMP and 4,750 all-map Character/container/Door declarations requiring actual model services remain explicit; no actual production actor Visual is inferred from those inspection paths.

## Exact archive entry findings

| Absent exact compatibility URI | Existing diagnostic variant | Source level IDs |
|---|---|---|
| data/3d/textures/env_crypt_alpha.tga | data/3d/textures/pvr2_env_crypt_alpha.tga | ABBEY_ENTRANCE_01, GOTHICUS_CRYPT_01, GOTHICUS_CRYPT_02 |
| data/3d/textures/env_darktemple_alpha.tga | data/3d/textures/pvr2_env_darktemple_alpha.tga | DARK_DUNGEON_TEMPLE_01 |
| data/3d/textures/env_earth_temple_alpha.tga | data/3d/textures/pvr2_env_earth_temple_alpha.tga | EARTH_TEMPLE_01, EARTH_TEMPLE_02 |
| data/3d/textures/env_castle_alpha.tga | data/3d/textures/pvr2_env_castle_alpha.tga | ELEMENTAL_AIR, ELEMENTAL_EARTH, ELEMENTAL_FIRE, ELEMENTAL_LIGHT, ELEMENTAL_WATER, GOTHICUS_CASTLE_01, GOTHICUS_CASTLE_02, RANDOMFORALEX |
| data/3d/textures/env_windtemple_alpha.tga | data/3d/textures/pvr2_env_windtemple_alpha.tga | WIND_TEMPLE_01 |

## Exact Module selector finding

`GOTHICUS_CRYPT_01`: data/3d/modules/crypt/crypt.bdae selector `_module_entrance_s`, from data/3d/modules/crypt/mvx/entrance_s.mvx (procedural_source_candidate). The original Module selection path appends `-node` and searches the first scene library. No alias/default root is substituted.

## Artifact use

- reports/all-map-image-dependencies-v44.csv provides the 51-row status table.
- reports/all-map-image-dependencies-v44-summary.json contains compact counts, exact entry findings and source-domain cases.
- reports/all-map-image-dependencies-v44.json is the normalized 13.5 MB detailed matrix. The native asset table owns the full paths once, avoiding duplication of 92,005 joined records.
- reference/all-map-image-dependencies-v44/query_matrix.py --level SWAMP --problem-only displays scoped source findings. --verify replays every persisted exact asset/instance/material/sampler join.
- native-receipt.json records exact sources/compiler/run command and output hashes. all-map-image-dependencies-v44-verified.json records normalized-artifact readback and counts.

Main retains ownership of original filesystem/format routing, technique-dependent sampler binding, embedded CResFile image fallback, texture admission/accounting, GL context generation, GPU uploads and actual pass registration. This static potential matrix is not a texture residency list and must not be used to preallocate all candidate resources. No selected/shared source, CMake, manifest or device was changed.

| Subtask | Status |
|---|---|
| All 51 level source identity/dependency joins | Finished; candidate scope preserved |
| Exact native Module subtree/material/image records | Finished; 305 queries and 2,779 native/raw comparisons |
| Exact archive presence/nongeometry/malformed distinctions | Finished; source findings recorded |
| Normalized matrix readback and query tool | Finished; 92,005 rows verified |
| Original format/mount routing for five alpha paths | Open; existing variants are diagnostic only |
| Actual actor model/activation and GPU resource admission | Main services; remains open |
