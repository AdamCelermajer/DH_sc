# Native floor collision

The source reconstructs original triangle `isOnSameSide`, `isPointInside`,
`getIntersectionOfPlaneWithLine` and `getIntersectionWithLine`, the scene
collision manager's `getCollisionPoint`, and PFFloor's vertical
`GetCollisionAt`. The manifest binds each captured body to engine SHA-256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

The triangle normal is the original reverse cross product. Glitch vector
normalization multiplies by the reciprocal length and leaves a zero-length
vector unchanged; this differs from Point3D normalization used by the graph
builder. Plane-parallel rejection uses inclusive epsilon `0x358637bd`.
Each of the three same-side tests compares its cross-product dot with zero.
A plane intersection outside the triangle still writes the output point.

The manager normalizes end-start, rejects candidates outside the ray bounds,
then retains the original nearest-vertex shortcut: if all three vertices
are at least as far away as the current hit, it skips the candidate. It
accepts a triangle intersection only when distances to both ray endpoints
are strictly below the ray length squared, and distance to the start is
strictly below the current hit. Triangle order is significant. Misses
preserve output point and triangle bytes.

PFFloor first checks inclusive XYZ bounds, then casts from point Z+1000
to Z-1000. The native FloorSet callback now supplies graph support by
executing this native floor/manager/triangle chain. It uses no original
machine code, service pointers or translation runtime.

## Evidence and boundaries

The differential executes original triangle, normalization, collision-manager
and floor bodies, with ordered transformed triangle lists supplied through
selector vtable fixtures. Scene services and selector storage are caller
fixtures. Imported IEEE float operations/sqrt are modeled; finite bits must
match, and arithmetic NaNs match as a class without requiring payload/sign.

The corpus has 2,409 line, 1,400 ordered ray and 1,262 floor cases. It includes
stacked triangles, nearest-hit ordering, endpoint exclusion, degenerate
geometry, epsilon boundaries, signed zero, nonfinite line inputs and all
314 exported Crypt triangles. The native host replay compares all output
words and retained miss bytes under ASan/UBSan.

An additional graph differential passes the 314 Crypt triangles through
original graph construction and original floor collision, and compares them
with the compiled native graph calling compiled native floor collision.
All 1,239 support coordinates/decisions match. For this experiment, room
grouping defines eight supplied floors, their bounds are supplied snapshots,
and floor flags are zero. This is not evidence that those are the original
runtime floor identities or flags. Host replay calls the native floor
callback and checks the original-verified graph snapshots.

The original PFFloor loader creates **COctTreeTriangleSelector**, not a simple
CTriangleSelector. Its identity-space BVH construction and box selection/order
are now reconstructed separately in [octree.cpp](../octree/NOTES.md). A separate
[selector adapter](../selector/NOTES.md) couples them to collision and graph
support, including query/output transforms. Mesh extraction/baking, real
floor bounds/identity/flag producers, cross-floor sewing, graph search,
smoothing, obstacles and controller/animation integration remain pending.
Gameplay still uses its earlier floor movement policy and does not call this
new graph/collision backend yet. The original GetFloorHeightAt normal-output
variant is captured for context but is not implemented by this API.

Null/oversized caller storage rejects without writes as modern adapter policy.
The Result hit/index header and typed Floor/FloorSet layouts are native API
representations, not copies of the original object's binary layout.
