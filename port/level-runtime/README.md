# Bounded static-level runtime slice

This host-side coordinator joins the recovered LevelList catalogue with the
owned MLX/MGP/MVP importer and the checked BRES/Collada scene readers. It accepts
a caller-supplied cache root and any catalogue entry whose `LevelFile` is a
static `.mlx`. The first cache-backed closure is `INFECTED_VILLAGE_01`.

For that level it imports `005_infectedvillage.mlx`, then both MGP/MVP pairs in
the authored module order. It opens the referenced BDAE, resolves each
`xrefobject + "-node"` root, binds the root to its level placement, and returns
the module origins/correction and checked BDAE subtree records. It preserves
all level, gameplay and visual source records in per-file source order and
returns the entrypoint-zero `SpawnPoint` local/world coordinates. The observed
entrypoint is `_prim_EntryPoint`: local `(900.85,-4148.52,1348.88)`, world
`(-2547.65,-1148.52,1348.88)`.

## Run

From the repository root:

```powershell
python port/level-runtime/build.py --report port/level-runtime/build/build-validation.json
python port/level-runtime/runtime.py --cache-root ../cache/files --level INFECTED_VILLAGE_01 --report port/level-runtime/build/infected-village.json
python port/level-runtime/tests/test_runtime.py --library port/level-runtime/build/libdh2_level_runtime_host.dll --cache-root ../cache/files
```

The host build links the existing `world-data`, `scene-payloads`,
`engine-resources`, and `engine-math` readers. Catalogue tables are decoded by
the existing `port/level-catalogue/catalogue.py`. No new parser or runtime
dependency is required.

The source loader caps catalogue and XML inputs at 8 MiB, BDAE files at 128
MiB, modules at 256, imported objects at the existing 32,768 limit, one BDAE
module subtree at 65,536 nodes, and serialized output at 16 MiB. A failed load
throws an error and returns no partially assembled level report. Cache paths
are normalized with the existing explicit policy and resolved beneath the
caller-supplied cache root.

## Current boundary

This is source-backed static data loading and module-root placement, not an
active game world. Object records are parsed but not activated; factories,
property defaults, scripts, conditions, enemy spawning, AI, camera behavior,
quest state, collision, navigation, level transitions, rendering and gameplay
are not implemented by this slice. It does not load procedural `.rule.xml`
levels or claim full native transform/runtime parity. Build evidence is host
only; it is not an Android build or emulator test.

## Related source audits

- [`INFECTED-VILLAGE-GAMEPLAY.md`](INFECTED-VILLAGE-GAMEPLAY.md) maps the
  source-backed 53 records, six scripts, two Ambush triggers, exits and quest
  zone without claiming that the host audit activates gameplay.
- [`ASSET-BUNDLE.md`](ASSET-BUNDLE.md) documents deterministic, bounded asset
  closure for the same level, including checked BDAE sampler paths and explicit
  unresolved references.
