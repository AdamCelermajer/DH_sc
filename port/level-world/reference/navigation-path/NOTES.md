# Original path smoothing and waypoint lifecycle

`navigation_path.cpp` reconstructs PFWorld::FindPath (`0x52db48`) and its
DropPath, SmoothPath, CalcWaypointVec, IsPastWaypoint and MovePath sequence,
PFObject::GetPathLengthSQ and Point2D::lineIntersection. The capture contains
55 hash-identified routines, including the prior world search. The original
ARM32 ELF is a local comparison input and is never bundled or loaded by Android.

## Recovered behavior

FindPath drops the existing list, sets the requested target, calls the recovered
world-coordinate search with the object's current position and supplied search
budget, then smooths and calculates a waypoint only on success. Profiling and
optional debug timing/deque accumulation are omitted; timing is disabled in the
original comparison fixture. Failed routes preserve the new requested target.

SmoothPath refines exactly one portal. A path whose owned-edge count is nonzero
is unchanged. Otherwise it copies the first edge into an owned direct edge,
removes the old list element and prepends the copy. The copied edge has null
graph endpoints, weight 1 and distance/clearance 0. Its source/destination
coordinates are copied exactly. Embedded direct edges also receive an owned
copy, but have no destination node and skip portal intersection.

For a graph edge, the destination node supplies a portal centered on its
position, along its XY direction, with half width `(node.width-radius)*0.5`.
There is no clamp when the radius exceeds the width. The portal intersects the
line from the copied edge's source to the object's requested target. Original
classification 3 or 5 uses the intersection; classification 4 chooses the
first portal endpoint if the portal parameter is negative and the second
otherwise. Other classifications retain the original destination. Z is retained
in every branch. This is incremental portal refinement, not whole-route pruning.

Point2D lineIntersection rounds each subtraction, product, reciprocal and sum
individually. The parallel epsilon is the original float `0x38d1b717`, with
strict comparisons. Parallel branches preserve the supplied point/parameters;
classifications 0/1 denote the original parallel cases, 2 neither segment,
3 first only, 4 second only and 5 both, using inclusive [0,1] parameters.

CalcWaypointVec writes destination minus source in X/Y and positive zero Z.
IsPastWaypoint checks `((position.x-source.x)*waypoint.x +
(position.y-source.y)*waypoint.y) + waypoint.z*0 >= 0`. It uses the first
edge's **source** plane; it is not a distance-to-destination check. MovePath
removes at most one edge per call. After a removal it smooths/calculates the
new head and returns its destination, or returns the requested target with
false when the path was exhausted. An already empty path returns current
position and false. It does not move or validate the object's position.

DropPath deletes owned head edges and clears the list. It copies current
position to the requested target only when the list was nonempty; an empty
Drop leaves target/direction untouched. GetPathLengthSQ sums the squared 3D
length of **each** edge, rather than squaring the total length.

## Native ownership and evidence

PathSegment uses stable graph IDs, zero for the embedded direct edge and
UINT_MAX for the owned copied edge. Immutable graph coordinates are materialized
in caller-owned bounded storage. The reconstructed SmoothPath producer owns at
most one head edge. Allocator/container growth, deletion and immutable graph
lifetime are modern adapters. Full PFObject initialization and other path
producers are outside this ownership contract. Finite position/target/radius,
well-formed graphs and adequate segment/route scratch are modern caller
contracts; FindPath rejects malformed/storage requests before dropping a path.

Three ARM64 binaries (standalone and both APK builds) compare 661 independent
intersection cases and 4,509 path operations across 323 sessions. Comparisons
include complete segment coordinates/metadata, embedded direct coordinates,
owned counts, waypoint vectors, path lengths, outputs and original destructor
execution. The primitive corpus deliberately includes arbitrary graph-edge
lists to test individual branches; it is not a movement replay.

The complete original FindPath corpus uses actual authored Crypt routes: 366
requests in 46 sessions, 9 direct and 127 graph successes, 230 failures,
131 existing-path replacements and 45 cached failures. All 669 original floor
query IDs/coordinates are compared through independent original floor/selector/
octree/collision execution. Host ASan/UBSan repeats both corpora; FindPath loads
the real BRES/DWLD and first checks graph and geometry against original gold.
The host query count refers to gold records; query order is observed by the
instruction differential, not by the host audit.

Android startup invokes FindPath for all 64 ordered floor pairs, including
replacement of the previous route. It checks 2,086 segments and owned count 64,
with route/path-state checksum `9356b419cae2bc57`. These are zero-radius,
capability-zero centroid probes. They do not drive characters. Original
ValidatePosition/ValidateDirection, obstacle/collision response, actor radius/
flags producers, cache invalidation, controller speed/root-motion integration
and moving enemy pursuit remain pending. Physical ARM64 and original GPU parity
remain unverified; this checkpoint is not the full playable game.
