# Infected actor skeleton compatibility audit

This host-only check joins the six cache-derived model choices for
`InfectedVillage_CommonType1` with the exact `Idle` and `Walk` entries in the
infected `CharAnimTable`. It also checks the two `Attack` BDAEs and the
NPC-talk BDAE referenced by `Stunned`, so their cutscene target mismatches are
visible instead of silently accepted.

The checker reuses the reconstruction's BRES, scene, animation, skin, and
absolute-pose readers. It opens each model's named skin controller, confirms
that every skin joint scope resolves once, enumerates every BDAE segment,
matches each animation track target against model scene-node IDs, and samples
the pose/skin palette at clip start, midpoint, and end. `validation.json`
records source file hashes, per-model controller/joint counts, every segment's
target names and match counts, and palette sample results. Recovered cache
payloads are read in place and are not copied into this directory.

## Reproduce

Run from the repository root with Python 3 and a C++17 host compiler available
as `CXX` or `c++`:

```powershell
python port/animation-pose/build.py `
  --output port/infected-actor-compatibility/build `
  --report port/infected-actor-compatibility/build-validation.json

python port/infected-actor-compatibility/check_compatibility.py `
  --cache ..\cache\files `
  --library port/infected-actor-compatibility/build/pose-host.dll `
  --report port/infected-actor-compatibility/validation.json
```

On Linux, build and run with the `.so` output and cache-relative path:

```sh
python3 port/animation-pose/build.py \
  --output port/infected-actor-compatibility/build \
  --report port/infected-actor-compatibility/build-validation.json

python3 port/infected-actor-compatibility/check_compatibility.py \
  --cache ../cache/files \
  --library port/infected-actor-compatibility/build/pose-host.so \
  --report port/infected-actor-compatibility/validation.json
```

The cache root must contain the extracted `data/3d/...` paths.

## Source mapping

The checked actor choices are recorded in
[`../level-runtime/infected-village-actor-assets.json`](../level-runtime/infected-village-actor-assets.json), derived from the four `Character` records in
`infected01.mgp` that name `Charater_Templates` / `InfectedVillage_CommonType1`.
That file pins all six model paths, their skin controller IDs, the animation
table name/ID, animation-template IDs and their source animation dictionary
paths. Its SHA-256 and the actual cache-file hashes are recorded in
`validation.json`.

The exact selected mappings are:

| Infected state | Anim-template ID | Source name | BDAE mapping |
| --- | ---: | --- | --- |
| Idle | 321 | `Infected_Idle` | `infected_idle.bdae`, `infected_idle_02.bdae` |
| Walk | 328 | `Infected_Walk` | `infected_walk.bdae` |
| Attack | 315 | `Infected_Attack1H` | `cs_madruk_end_scene06_faery01.bdae`, `cs_madruk_end_scene06_prince.bdae` |
| Stunned talk | 326 | `Infected_Stunned` | `cs_madruk_intro_rene_scene07_talk.bdae` |

`Limbus` has `anim_tpl_id = -1` in the source table. This check does not make
up or substitute a Limbus animation.

## Results

The checked cache has all six model BDAEs and six selected animation BDAEs.
Each model has one expected named controller, 20 skin joints, and one unique
scene scope for each joint. Model scene-node counts are 27–33.
The variants are `infected`, `burned`, `inf_blacksmith`, `inf_maid`,
`inf_merchant`, and `inf_nun`. All 12 selected model and animation asset files
match the source manifest's recorded sizes and SHA-256 hashes.

| State / BDAE | Model variants with a full reader + skeleton binding | Segment results |
| --- | ---: | ---: |
| Idle `infected_idle.bdae` | 6/6 | 12/12 segments across variants |
| Idle `infected_idle_02.bdae` | 6/6 | 12/12 |
| Walk `infected_walk.bdae` | 6/6 | 6/6 |
| Attack Faery cutscene | 0/6 | 0/12; all 45 tracks per segment target Faery nodes absent from the infected models |
| Attack Prince cutscene | 1/6 (`inf_blacksmith`) | 1/6; the other five variants each miss `Bip01_L_Toe0-node` and `Bip01_R_Toe0-node` |
| Stunned NPC-talk cutscene | 0/6 | 0/6; five variants miss five node IDs, and blacksmith misses three |

Across the matrix, 31 of 54 model/segment combinations pass the bounded
reader, unique target binding, skin-joint resolution and sampled palette
checks, with 91 of 150 sampled palette evaluations succeeding. Those comprise
all Idle and Walk segments plus the Prince cutscene segment on the blacksmith
model. The latter cutscene segment has zero duration in its source segment, so
its successful skeleton binding does not show that it supplies a usable
gameplay attack. The exact start/end values are included per segment in the
JSON report.

The Faery cutscene's 26 distinct missing target names, the two Prince toe
targets, the five NPC-talk missing target names, and their repeated per-segment
occurrences are listed in `clip_file_summary` and `model_clip_results`. No
target resolves to multiple nodes. The audit tests serialized node targets and
skin scopes; it does not establish game-state selection, animation semantics,
default-relative blending, transitions, root motion, or native rendering.

## Scope and provenance

This is a compatibility investigation, not a gameplay feature and not a
complete engine. It uses only cache paths, hashes and metadata; original game
assets remain outside the repository. A successful sampled pose does not
establish that the original runtime would select that clip for that character.
No game-wide open-source license is implied by this reconstructed checker.
