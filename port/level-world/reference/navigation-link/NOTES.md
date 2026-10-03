# Native floor sewing

`dh2_nav_link` reconstructs the complete original PFFloor::_Link body at
`0x51fd78`. Each floor retains its midpoint tree and boundary-record range.
Node/edge identities are shared. `floors::post_load` now invokes sewing during
the authored Crypt's native level load; this does not yet provide route search.

## Recovered sequence

The OR of the two floors' object flags gates sewing with `0x04000000`.
Boundary midpoints must lie within the other floor's bounds with one unit of
inclusive tolerance on every axis. Eligible boundary edges create forced
nodes in their own floor's midpoint tree, preserving lookup and insertion
order. Forced creation bypasses support queries. Node-disabled floors cannot
produce boundary records through the original triangle builder.

First-floor boundary nodes get directed links to both original neighbours,
with each reverse link appended to that floor's validation list. Second-floor
internal links are emitted during the first selected first-floor boundary.
Pairs get cross-floor links only when squared 3D separation is **strictly less**
than squared summed widths, and absolute Z separation is **strictly less than
100**. Each edge call uses its caller floor's object flags, which matters when
only one floor disables links. Existing directed pairs are reused and updated.
The two neighbour-floor sets gain membership for every selected pair even
when its distance test fails. Native storage preserves logical membership;
it uses IDs rather than the original set's pointer keys.

Original PFWorld::PostLoad visits room pairs in increasing layout order using
50 units of inclusive bounding-box tolerance. PFRoom::_Link repeats the floor
overlap gate and preserves floor order. After that room's links to later rooms,
PFRoom::_PostLoad links its internal floor pairs with zero overlap margin and
clears each eligible floor's temporary boundary vectors. Native code closes
the active invalid range but retains raw records as a diagnostic archive.

## Evidence

The engine-hash-bound captures contain the floor, room and world bodies,
forced CreateNode/CreateEdge, original midpoint map lookup/insertion/RB
balancing, floor-set insertion and final floor cleanup.

`navigation_link_differential.py` compares original instructions with native
ARM64 in 259 cases: the actual asset-verified Crypt plus synthetic floors,
duplicates, reverse/repeated sewing, disabled flags, degenerate edges,
inclusive +/-1 bounds, strict width/distance equality and the Z=100 cutoff.
All 2,147 initial triangle operations, 869 sewing calls and 1,643 logical
snapshots match. The original room/world overlap gates and floor call order
also match in all 259 cases. Original execution observes 516 distance-pass
branches and 404 vertical-pass branches.

Original graph node/edge allocation and deduplication are storage services,
as in the prior graph oracle. Link-local pair-vector growth is supplied
storage. Original midpoint and neighbour-floor tree operations execute;
debug property/string services are fixtures when checking room/world caller
order. Initial synthetic floor support is supplied, with coordinates and
short-circuit order checked. Crypt initial support executes the original
selector/octree/collision chain. The packaged low-level sewing routine does
not call collision during forced creation.

Both packaged ARM64 builds run this corpus. `navigation_link_audit` replays
its LNK1 logical snapshots under ASan/UBSan and checks atomic capacity and
malformed-range rejection. Its 877 snapshots cover links and floor cleanup;
the additional initial-floor snapshots are differential checks.

The independent host asset loader builds **335 nodes, 838 edges, 998 validation
references and 14 directed neighbour-floor relations**. Its complete logical
state matches FNV-1a64 `57999e27060699df` from the original-verified differential.
The prior unlinked graph had 321 nodes and 778 edges. Floor asset inputs and
collision triangles are unchanged; collision still controls gameplay height.
Connectivity inspection confirms all 335 nodes belong to one component spanning
the eight floors, with actual cross-floor edges for all seven room links. This
diagnostic traversal does not reconstruct the original route-search algorithm.

## Remaining scope

Caller allocation, workspace and native pointer/ID ownership are modern
adapters. Capacity checks reserve worst-case outputs before mutation. Archived
invalid geometry is distinct from original active-vector lifetime.
Metadata parsing, complete floor/scene lifecycle, route search, endpoint
selection, smoothing, obstacles and the original movement controller are
still pending. No moving-enemy pursuit, full-game or original GPU equivalence
is claimed. Physical ARM64 execution remains untested.
