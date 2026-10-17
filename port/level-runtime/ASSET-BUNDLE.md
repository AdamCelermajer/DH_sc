# Static level asset closure bundle

`asset_bundle.py` stages a reproducible cache subset from paths established by
the bounded static loader and checked BDAE readers. For
`INFECTED_VILLAGE_01`, it contains LevelList inputs; the level MLX; imported
module MGP/MVP files; module and animated-decor BDAE scenes; direct LevelConfig
lightsets; common and level script tables; and BDAE material sampler textures.
Source-record and scene-node provenance is retained in the manifest.

The scene/material closure uses `dh2_static_scene_draws` to map module subtree
draw primitives to material indexes, then the checked material sampler APIs
to obtain BRES image source paths. The seven exact textures are:

- `env_infectedvillage.tga`
- `env_infectedvillage_spec.tga`
- `pvr2_env_infectedvillage_alpha.tga`
- `fx_smoke_03.tga`
- `atlas_fx_particles_001.tga`
- `fx_magic_lenz_flares_008.tga`
- `fx_spark_01.tga`

They all resolve to exact files beneath `data/3d/textures/`. Fog's BDAE has a
verified sampler path but zero static draw commands, so its texture reference
has material provenance without a node/primitive link. Three module sampler
references use an unbound `-1` image sentinel and remain explicitly unresolved.
External effect files, effect-group selection, and animated shader passes are
not resolved by these APIs.

Run from the repository root:

```powershell
python port/level-runtime/asset_bundle.py `
  --cache-root ../cache/files `
  --level INFECTED_VILLAGE_01 `
  --scene-library port/scene-draw/build/libdh2_scene_draw_host.dll `
  --stage-dir ../emulator-test/infected-village-source-bundle
```

The staging path must not already exist. Publication uses a temporary sibling
directory and a rename, so a failed build does not leave a partially published
bundle. The deterministic `asset-manifest.json` contains normalized relative
paths, byte sizes, SHA-256 hashes, roles, and source references. Every source
file is copied beneath the staging root only.

Bounds: at most 512 files, 128 MiB per file, and 512 MiB total. Cache paths use
the static loader's ASCII `data/` normalization, reject traversal and symlink
components, and are resolved beneath the supplied cache root. BDAE reader
traversal is bounded to 200,000 nodes/draws and 32,768 materials/samplers.
Existing staging directories are never overwritten. Limits can be reduced per
invocation for tests, but cannot be raised above the compiled-in ceilings.

## Known closure gaps

The native script loader's common and per-level packed tables are included and
format-validated. `ExecScript` calls resolve through the checked ordered name
tables, and `SpawnCharacter` names are checked against imported source records.
Numeric dialog/audio/music references are not mapped to data files, and no
sound or dialog resource tables are guessed into the bundle. For this level
the tables contain 15 common scripts / 81 commands and 6 level scripts / 36
commands. The `.pyscript` path is a logical source reference, not a standalone
file in the cache.

The supplied lightset XML files are parsed with a bounded XML audit and contain
no path-like values. This observation is not a schema-wide proof about other
lightset formats, and no nested lightset files were inferred. The level's
`camera_file` is empty. Source `xrefmax`/`editormax` fields point to authoring
`.max` files and are excluded because this loader does not establish them as
runtime dependencies. The bundle is an input package for further integration
work, not a complete or runnable level by itself.

Run closure and safety tests with:

```powershell
python port/level-runtime/tests/test_asset_bundle.py `
  --library port/level-runtime/build/libdh2_level_runtime_host.dll `
  --scene-library port/scene-draw/build/libdh2_scene_draw_host.dll `
  --cache-root ../cache/files
```
