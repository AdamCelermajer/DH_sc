# Original position-to-position world search

`navigation_world.cpp` reconstructs PFWorld::_SearchGraph at `0x52b560`,
PFWorld::GetCollisionAt at `0x5256d4`, PFRoom::GetCollisionAt at `0x52113c`,
floor midpoint-map lookup and the PFInnerTest predicates. Captured routines
and hashes are in `original-functions.json`. The original ELF is used only
for local instruction comparisons, never inside an APK.

## Recovered rules

World and room bounds check all three axes inclusively. Rooms and their floors
are visited in caller order; the first successful floor wins. Ordinary queries
skip floors whose **type** has `0x03000000`, independently of object flags.
The existing native selector/octree/collision chain supplies the floor hit,
its triangle in original winding/order and its point. This differs from the
current player-height adapter's nearest-height policy.

The wrapper queries source first, then target, and stops on a missing hit.
Equal triangle coordinates bypass graph search. Otherwise each triangle's
AB, AC and BC midpoints are looked up in that floor's original CompPos tree.
The nearest available midpoint node by single-precision squared 3D distance
wins; ties retain the first. X/Y epsilon and exact Z ordering match the map.
Missing nodes fail; equal source/target nodes also bypass search.

Direct success with a PFObject and an output list copies the exact requested
source/target points into the object's owned direct edge and appends that edge
to the list. Without both an object and output, it returns success without
emitting an edge. Native edge ID zero represents this object-owned direct edge.
This wrapper does **not** add temporary nodes or edges to the shared graph.

Normal search uses the separately recovered graph search. Edge validity is
the original constant-valid edge getter, plus `clearance >= object.radius`
when an object is supplied. Node validity is constant-valid plus floor object
flag bit 1; an object also requires all floor type bits in its capability mask.
Goal nodes retain the graph search's validity bypass. Without an object the
path-validity predicates still check floor object bit 1.

Failed `(source node, target node, expansion limit)` triples are retained and
searched from the end; a matching triple returns failure without another graph
search. A newly failed search appends a triple and clears the supplied output
list. A successful output longer than one edge drops its last edge when that
edge's source is any of the target triangle's midpoint nodes. External output
prefixes, absent objects/output, direct coordinates and cache contents are
included in the differential comparison.

## Ownership and verification boundary

CollisionWorld supplies room/floor grouping and bounds. RouteWorld supplies
the existing graph, original midpoint-tree roots, floor type/object facts and
bounded cache storage. RouteWorkspace supplies graph-search scratch and an
internal output buffer. Graphs and trees must be well formed; modern storage,
finite input positions/radius and valid external edge IDs are caller contracts.
Scene ownership, array growth and allocation services are modern adapters.
`floors::post_load` now owns these structures for the actual authored Crypt;
`floors::route` invokes the source-coordinate API. Cache invalidation is exposed
explicitly; its original update/lifecycle producer remains to be recovered.

The ARM64 differential executes the full original world wrapper, room/world
traversal, midpoint lookup, predicates, graph search and list/cache behavior.
Its floor service delegates to a second original-instruction oracle that runs
the complete floor, selector, matrix, octree and collision chain. The native
side calls the compiled chain directly. Authored geometry and room ownership
are caller facts already separately checked against the assets.

525 cases cover every ordered Crypt floor pair, 320 seeded requests, repeated
cache requests, radius/capability/disabled-floor facts, missing midpoint trees,
direct paths, disabled pathfinding, supplied output prefixes, absent actors
and outputs, zero/short/full budgets and out-of-world endpoints. All 64 ordinary
floor pairs succeed through world-coordinate endpoints, emitting 2,086 segments
with result/statistics checksum `3a3ab2a724cc383b`. The separate ASan/UBSan audit
loads the actual BRES/DWLD, compares its graph and geometry with original gold,
then replays all requests/cache sessions through that native world.

The live Android probe uses zero radius and capability mask zero, development
centroid endpoints and ordinary collision mode. It does not move characters.
Special-inclusive collision mode is implemented from the original room branch
but is outside this wrapper's ordinary-mode corpus. Full PFObject initialization,
cache lifecycle, FindPath, smoothing, waypoint/controller movement, obstacle
avoidance and moving enemy pursuit remain pending. Player movement still uses
the supported-floor adapter. Full metadata, generated-room lifecycle, original
GPU rendering and physical ARM64 testing are also unproven.
