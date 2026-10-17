# Infected Village source floor query check

This host-only check imports the recovered `INFECTED_VILLAGE_01` MLX and its
two source module roots, selects the two original floor-named BDAE mesh nodes,
and checks floor height at the authored entrypoint. It reuses the checked
scene, mesh and `dh2_nav_query_height` code; it does not run on Android or add
cache/APK assets to the repository.

Run it from the repository root with the supplied extracted cache:

```powershell
python port/infected-village-navigation/tests/run_host.py --cache ..\cache\files
```

On 2026-10-03 the check passed with two floor surfaces and 439 triangles. The
module-zero `_prim_EntryPoint` at world position
`(-2547.650,-1148.520,1348.880)` resolves to source floor node
`_floor_infectedvillage_01-node_PIVOT` at Z `1342.612`, a vertical difference
of `6.268` source units. Both source floor nodes omit a `floortypes` property;
the recovered name-token fallback produces mask zero for each. The transformed
floor meshes share 15 quantized triangle edges across the module boundary at
0.01-unit coordinate precision; their longest shared edge is 400 units. This
establishes a geometric seam in the imported source meshes, not player
traversability.

## Scope boundary

This is enough evidence for a bounded source-derived floor-height query at the
actual level entry and a geometric cross-module seam check. It is not a
controllable-player movement slice. The query selects the closest height to a
caller reference Z, while native `PFRoom` height queries use stored floor order;
native tie and vertical-band parity remains unresolved. The sample does not
establish `SpawnPoint::PlaceObject`'s actor pivot offset. The source does not
provide a recovered player collision radius or movement response here, and the
existing endpoint-only movement experiment is deliberately restricted to
SWAMP module zero. Consequently this check makes no walkability, collision,
movement-speed, camera, path-graph, or complete-gameplay claim.

Native floor selection and height-validation evidence is recorded in
[`../navigation/README.md`](../navigation/README.md), including
`PFWorld::LoadRoom`, `PFRoom::_LoadFloor`, `PFFloor::_LoadNavMesh`,
`PFWorld::ValidatePosition`, and `PFObject::CanPathOn`. Source level object
import remains described in [`../world-data/README.md`](../world-data/README.md).
