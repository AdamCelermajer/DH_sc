# Source-selected SWAMP floor geometry

This slice imports the supplied `SWAMP` MLX, binds its nine original module
roots in the supplied `swamp.bdae` catalogue, and copies triangles only from
the catalogue's floor-named mesh nodes. It applies each node's full scene
transform and the module catalogue-to-level translation correction before
storing world-space triangles. The result is a bounded geometric query data
set for height samples and zero-radius segment intersections. It is not a
complete pathfinder and a returned hit is not a movement verdict.

## Native evidence and selection

Recovered native routines in `recovered/native/decompiled/libDungeonHunter2.so/`
and `recovered/native/assembly/libDungeonHunter2.so/` establish the selection
path:

| Original routine | ELF address | Behavior used here |
| --- | ---: | --- |
| `PFWorld::LoadRoom` | `0x523c14` | Searches the selected room/module hierarchy for mesh scene nodes whose names contain the floor prefix, then calls `_LoadFloor`. |
| `PFRoom::_LoadFloor` | `0x521eb8` | Creates a `PFFloor` from the mesh scene node and loads its navigation mesh. |
| `PFFloor::_LoadNavMesh` | `0x520b40` | Copies the selected mesh for navigation, reads parent `UserProperties` key `floortypes`, and has a name-token fallback for `void`, `wall`, `hole`, and `water`. |
| `PFFloor::GetFloorHeightAt` | `0x51badc` | Obtains height through the floor collision/triangle query. |
| `PFFloor::GetCollisionAt(start,end,out)` | `0x51b874` | Passes the floor's triangle selector and query line to the scene collision manager. |
| `PFRoom::GetCollisionAt(start,end,out,includeAll)` | `0x5212e0` | Walks stored floors in order and returns the first floor hit; its filtered path skips either native void/wall category bit. |
| `PFRoom::GetFloorHeightAt` | `0x520f98` | Searches the room's floors after an AABB check; its filtered branch skips either `void`/`wall` category bit in `PFFloor::type_mask`. |
| `PFWorld::GetFloorHeightAt` | `0x525508` | Searches rooms after a world AABB check. |
| `PFWorld::GetCollisionAt(start,end,includeAll)` | `0x525800` | Visits rooms and floors in stored order; with `includeAll=false`, skips floor masks carrying native `void` or `wall` category bits and returns the first floor with a hit. |
| `CSceneCollisionManager::getCollisionPoint` | `0x6c62e8` | Tests the selected floor's triangles and retains the nearest intersection for that floor. |
| `triangle3d<float>::getIntersectionWithLine` | `0x58615c` | Intersects the infinite line defined by a point and direction with a triangle plane, then tests triangle containment. The caller must enforce finite-segment limits. |
| `triangle3d<float>::getIntersectionOfPlaneWithLine` | `0x585e80` | Rejects near-parallel line/plane pairs and computes the plane intersection point. |
| `triangle3d<float>::isPointInside` | `0x585de8` | Checks the point against the three triangle edges; exact edge points are accepted. |
| `PFWorld::ValidatePosition` | `0x525d84` | Queries a filtered room floor, then calls `PFObject::CanPathOn`. |
| `PFObject::CanPathOn` | `0x524230` | Returns true for zero floor requirements; otherwise checks `(floorMask & objectMask) == floorMask`. |

The API uses the native `floor` name selection within each checked module
subtree and keeps only type-3 geometry instances that the existing checked
mesh reader can open as triangle meshes. It does not use ordinary wall,
decoration, water-rendering, or other module geometry just because it is
visible. Separate floor-named geometry such as `_floor_water_...` remains a
separate surface with its exact source node and geometry IDs.

`dh2_world_bind_module` verifies the MLX module's translation-only placement
and finds the exact `xrefobject + "-node"` catalogue root. The scene walker
composes descendant transforms; `dh2_world_place_matrix` applies the
catalogue-to-level correction to every selected mesh. No cache files are
included in this source component.

## Query and limits

`dh2_nav_build_swamp` accepts only a level named `SWAMP` with nine module
instances that all reference `data/3d/modules/swamp/swamp.bdae`. It rejects
unsupported mesh layouts/transforms and uses explicit limits of 256 surfaces,
100,000 triangles and 65,536 scene nodes per visual walk. Names and source
record IDs are copied into the owned result. Initialize the output with `{}`;
call `dh2_nav_free` when finished.

## Link boundary with the live runtime

The importer and the live runtime have distinct C++ types and navigation entry
points, so they can share one native library without conflicting definitions:

| Imported data | Live runtime |
| --- | --- |
| `dh2::scene_payload::{Scene,Node,Instance}` | `dh2::scene::{Scene,Node,Instance}` |
| `dh2::world::SourceLevel` | `dh2::world::Level` |
| `dh2::navigation::SurfaceTriangle` (48 bytes, includes source IDs) | `dh2::navigation::Triangle` (36 bytes) |
| `dh2_nav_get_triangle` copies an imported record | `dh2_nav_triangle` creates PF graph nodes |

These names describe port interfaces; original function names and evidence
remain unchanged. Convert imported geometry explicitly at the adapter boundary.
No alias or cast makes an imported record into a live runtime object.

`tests/run_reader_runtime_boundary.py --cache <cache-root> --cxx <compiler>`
compiles both sets of headers in one translation unit and links both reader and
live scene/world/navigation implementations in one executable. It loads the
SWAMP catalogue through both scene paths, imports the source MLX, retrieves an
imported triangle, and independently builds a live PF graph triangle.

## Geometry queries

`dh2_nav_query_height` projects the query onto world X/Y, finds triangle
containment with barycentric coordinates, interpolates world Z, and selects
the hit nearest the caller's reference Z within a caller-supplied vertical
band. A small caller-supplied barycentric tolerance includes points on triangle
edges. Degenerate projected triangles are skipped because they cannot define a
single-valued height over X/Y. Ties keep the first triangle in source order.
Queries do not extrapolate beyond triangles.

`dh2_nav_query_actor_floor` is a separate source-backed eligibility query; the
geometric API above remains unrestricted. It first skips surfaces whose parsed
mask contains native `void` (`0x01000000`) or `wall` (`0x02000000`) category
bits. On the remaining low path requirements it applies the native
`CanPathOn` subset rule using the caller's explicit `object_path_mask`: zero
requirements pass, and each required bit must be present in the object mask.
The query returns the nearest remaining geometric hit, including its original
floor tag and parsed mask. It does not perform movement, collision response,
step handling, path graph updates, or module transitions.

`dh2_nav_query_segment` sweeps a zero-radius point across the imported world
space 3D triangles. It accepts finite `start` and `end` points and computes the
line direction internally. Hits at either endpoint are included; parallel and
coplanar segments have no unique plane intersection and report no hit. Points
exactly on a triangle edge are accepted. Projected-vertical triangles remain
eligible because this query uses the full 3D surface.

Segment queries visit imported surfaces in their original module/floor order.
When `include_all=false`, surfaces with unknown type masks or the native `void`
(`0x01000000`) or `wall` (`0x02000000`) category bits are skipped. For the first
eligible surface that intersects the segment, the query returns that surface's
nearest hit along the segment; it does not search later surfaces for a globally
closer result. `include_all=true` includes those otherwise-filtered surfaces.
The query bounds the imported dataset to 256 surfaces and 100,000 triangles,
limits segment length to 1,000,000 world units, and rejects endpoint or triangle
coordinates outside ±10,000,000.

This API models a zero-radius point and returns intersection metadata only. It
does not model actor radius, collision volumes, movement acceptance or rollback,
sliding, step handling, path capabilities, or the complete `PFWorld`/`PFRoom`
movement path. Do not wire it to actor movement until radius and response
behavior have a separate source-backed implementation and validation.

This host query deliberately retains the host sampler's nearest-reference-Z
selection within a caller-provided vertical band. Recovered `PFRoom` code
iterates its stored floor array and returns a successful floor hit in that
order; the native edge/tie behavior and its exact vertical thresholds have
not been established. Therefore actor eligibility follows the recovered mask
rules, while hit ordering and vertical selection are not claimed to reproduce
native `ValidatePosition` fully.

Each selected surface now copies the decoded `floortypes` property value into
`floor_type_tag` when the key is present, including a present empty value.
Absent keys leave that tag empty and use the native fallback to the source
node name for mask parsing. Surfaces expose `floor_type_tag_present`, the
parsed `floor_type_flags`, and a known bit. Every triangle retains its owning
surface index, so callers can retrieve the same tag and flags without
duplicating metadata on up to 100,000 triangles. `FloorHit` also copies the
selected surface's tag and flags.

The host reader maps case-sensitive substrings to the recovered bits:
`hole=0x00000001`, `water=0x00000002`, `void=0x01000000`, and
`wall=0x02000000`. The high void/wall category flags remain separate from the
low hole/water path requirements. `PFObject::CanPathOn` at `0x524230` reads
the floor mask from `PFFloor+0x24` and the actor path mask from `PFObject+0x14`;
the native subset test is implemented by the actor-floor query. `PFRoom`'s
filtered height-query branch at `0x5210e8` tests both high category bits and
skips either. `PFWorld::ValidatePosition` at `0x525d84` calls that filtered
query before `CanPathOn`. Height and actor-floor queries expose verified floor
selection and eligibility rules; the segment query covers point intersections
only. Together these APIs are not a complete movement or collision-response
implementation.

## Validation

From the repository root, with the supplied cache available at `../cache/files`:

```text
python port/navigation/build.py --report port/navigation/build-validation.json
python port/navigation/tests/check_navigation.py \
  --library port/navigation/build/libdh2_navigation_host.dll \
  --cache ../cache/files \
  --original ../standalone-build/compatibility/work/decoded-original/lib/armeabi-v7a/libDungeonHunter2.so \
  --report port/navigation/cache-validation.json
```

Host validation checks a synthetic sloped triangle and metadata carry-through,
exact edge inclusion, outside/no-extrapolation behavior, a projected-degenerate
triangle, vertical distance gating, invalid tolerance rejection, overlapping
actor-query surfaces for zero/hole/water/both-bit requirements and `void`/`wall`
rejection, segment endpoints, parallel/coplanar misses, 3D edge hits including
projected-vertical triangles, finite segment clipping, nearest hit within one
surface, source surface ordering, and the `include_all` category filter. It
checks all nine original module roots,
real `hole`, `water`, `wood`, and `door` tags, and the entry point at
`(1090.75,-212.202,258)`. With the supplied cache, the builder selects 16 floor
surfaces and 626 triangles. The entry XY resolves to source node
`_floor_obj_4of4_brdwalk_sw_-node` at Z=255, three units below the recorded
entry Z; the vertical segment query returns that source surface at fraction
0.375. This verifies imported geometry and the bounded point query, not actor
movement or complete native walkability.

The host check also runs `tests/check_triangle_arm.py` under Unicorn against the
original ARM32 ELF. The harness calls the native function at `0x58615c` and
executes its original plane-intersection and point-inside instructions, while
small IEEE-754 shims supply the ELF's unresolved arithmetic PLT helpers. It
checks interior hits, the intersection at the finite segment endpoint,
parallel lines, triangle-edge hits, and outside-triangle misses. The original
routine takes a point and direction and represents an infinite line; finite
segment clipping and surface-order selection are separately tested against the
host API. Install the test dependencies with `python -m pip install -r
requirements.txt`. The original library remains outside this source component;
follow [provenance and rights](../../RIGHTS.md). Build reports and host binaries
remain local under the ignored `build/` directory.

The nine-module cache selection contains `hole`, `water`, `wood`, and `door`
tags but no selected `void` or `wall` surface. The distinct high-bit category
values are therefore checked with synthetic metadata carry-through as well as
the pure floor-type mapper; their native graph/height effects are not applied
by this navigation API.
