# Original octree construction and box selection

`octree.cpp` reconstructs `COctTreeTriangleSelector::constructOctTree` and
`getTrianglesFromOctTreeBox`, including their bounds/corner arithmetic,
triangle containment, stable partition and `TestWithBox`/`AddResult` order.
The captured function manifest and disassembly are bound to the original
engine SHA-256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

Each node recomputes bounds from its input geometry. Triangles entirely inside
one child cell move there; triangles spanning cells remain in the parent.
An inclusive split-plane triangle goes to the first containing child.
Children visit (min/max) XYZ combinations in this order: `000, 010, 001, 011,
100, 110, 101, 111`. Queries test parent triangles before visiting children.
Triangle box tests reject only when all vertices lie strictly outside one
axis; they are not exact triangle/AABB intersection tests.

Cell corners follow original `getEdges`: center is `(min+max)*0.5f`, delta is
`center-max`, and corners use center plus/minus delta. Replacing these with
stored bounds can change float rounding. The point-box early return uses
inclusive epsilon `0x358637bd`; other termination uses the supplied leaf limit.
The original PFFloor loader supplies leaf limit 15. Tree node bounds remain
the bounds computed before partitioning.

## Verified scope

The differential executes the original construction, vector insertion/resize,
corner/containment helpers, recursive query, triangle filters and identity
`AddResult` instructions. Allocation/deallocation are caller storage services;
imported float operations are modeled. The corpus includes all eight exported
Crypt room groups at leaf limits 0, 1, 15 and 1000, 40 synthetic clouds, and
two degenerate/epsilon cases. It compares 74 builds, 875 nodes, 3,235 retained
triangle records and 2,294 queries selecting 10,341 triangles in order.
Standalone, APK and Studio ARM64 libraries each pass with zero mismatches.

`octree_audit path/to/octree-original.bin` replays those original-verified
trees and query sequences under ASan/UBSan. It additionally checks 6,882 bounded
queries, empty trees and storage exhaustion. The original has a small-output
sibling overrun: it checks capacity after testing a parent, but does not stop
later siblings after a child fills the buffer. This native API truncates
safely. Full original output parity is claimed only for capacity at least the
input triangle count. Finite geometry/range limits, bounded caller-owned
storage, index output and invalid-input rejection are modern API policy.

## Still pending

The constructor's mesh extraction, node transform/inverse setup, nonidentity
`AddResult`, selector lifecycle and floor identity/flag/bounds producers are
not covered. Room grouping remains a fixture, not recovered floor ownership.
The new [selector adapter](../selector/NOTES.md) now wires this octree into
collision and graph-support callbacks, including query/output transforms.
It is not wired into gameplay yet.
Cross-floor sewing, route search, smoothing, obstacles and movement controller
integration remain necessary. No original machine code or translation runtime
is bundled; these are source-built native routines.
