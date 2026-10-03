# Selector transforms and coupled native collision

`selector.cpp` reconstructs `COctTreeTriangleSelector::getTriangles(box)`,
`CTriangleSelector::Setup(matrix/box)` and transformed `AddResult`, together
with original matrix multiplication, `getInverse`/`makeInverse` and
`transformBox` arithmetic. Captures are bound to original engine SHA-256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

Setup begins with the supplied extra matrix or identity. For geometry not
already baked, it postmultiplies the node world transform, inversely maps the
query box and applies the composed matrix to selected triangles. The explicit
identity hint controls shortcuts, even when numeric entries appear identical.
Inversion uses twelve float minors, the recovered cofactor association and
inclusive determinant epsilon `0x358637bd`, followed by reciprocal multiply.
A singular inverse leaves the copied matrix unchanged; Setup still uses it.

`transformBox` transforms only the supplied minimum and maximum corners, then
swaps reversed axes. It does not transform eight corners. Query box selection
therefore follows this original behavior even for rotation/shear. Already-baked
geometry ignores the node transform; an extra output transform still applies.

`dh2_selector_raycast` supplies the ray's bounds to the reconstructed selector,
then feeds its ordered transformed triangles to the reconstructed collision
manager. `dh2_selector_floor` checks supplied world bounds and casts Z+1000 to
Z-1000. `dh2_selector_floor_query` is the graph builder's callback. No original
instructions, service pointers or ARM translation runtime execute in the APK.

## Evidence

The differential executes original octree construction, getTriangleCount,
getTriangles, Setup, matrix inversion/multiplication, transformBox, AddResult,
collision-manager, floor and triangle bodies. Raw mesh triangles, allocation,
node-world-matrix getter and scene/node/selector ownership are caller services.
The selector's triangle list is not a service fixture in this experiment.

Standalone, packaged and Studio ARM64 libraries match 1,007 inverse cases
(57 singular), 32 constructed trees, 3,072 box selections, 3,072 complete ray
queries and 3,072 complete floor queries. Node-absent, dynamic and baked modes
each have 1,024 cases; 768 selections additionally supply an extra matrix.
Finite float bits, identity state, output triangle order, hit point/triangle,
selected-list index and retained miss bytes match. Arithmetic NaNs compare as
an unordered class rather than requiring payload/sign parity.

The graph/selector differential sends all 314 exported Crypt triangles through
graph construction and the complete floor/manager/selector/octree chain.
All 1,239 support queries and logical graph snapshots match. Host executables
`selector_audit path/to/selector-original.bin` and
`navigation_audit path/to/navigation-selector-original.bin` replay the original-
verified corpora under ASan/UBSan. NAV3 references select this callback; NAV1/2
retain their earlier fixture/collision-only contracts.

## Boundaries

The original constructor's mesh extraction/baking and floor flag/bound
arithmetic are now reconstructed and verified separately in
[floor-source](../floor-source/NOTES.md). Selector lifecycle, actual floor
ownership/identities/default flags and mesh-node clone transforms are supplied
inputs in this historical selector corpus. The later
[floor-records checkpoint](../floor-records/NOTES.md) now builds them from actual
Crypt assets, uses object flags one, constructs the graph and controls gameplay
height through the selector. Full metadata parsing, cross-floor sewing, route
search, smoothing, obstacles and the original movement controller remain pending.

Native layouts and workspace ownership are modern API adapters. Query workspace
must fit all raw triangles, avoiding the legacy small-buffer sibling overrun.
Input flags and insufficient storage reject; ray result remains unchanged on
rejection. The output result index describes the ordered selected list, not a
stable raw mesh triangle ID. Both ARM64 and x86_64 compile; no physical ARM64
Android phone or original GPU equivalence has been tested.
