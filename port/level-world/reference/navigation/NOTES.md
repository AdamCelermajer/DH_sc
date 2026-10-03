# Native PFFloor graph construction

The source rebuilds `_CreateNodes` (`0x520588`), `_CreateNode`
(`0x51fa64`) and `_CreateEdge` (`0x51e98c`). The captured original engine
SHA-256 is `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The manifest binds each captured routine to its original body hash.

Each triangle produces its original reverse normal, then nodes for AB, AC
and BC, followed by directed AB–AC, AB–BC and AC–BC links in both directions.
Nodes sit at edge midpoints. CompPos compares X/Y with the original
`0x38d1b717` epsilon, then compares Z exactly. Approximate equivalence is
not transitive, so an explicit red-black tree retains original insertion
and lookup behavior rather than assuming a transitive key equivalence.
Node directions normalize B−A; their width and length equal that edge's
3D length. Travel weight is 3D distance between midpoints. Clearance is
the smaller endpoint width. Repeated directed pairs reuse their edge.

New nodes query floor support at midpoint ± normalize(cross(B−A,normal)).
Either rejection produces an InvalidNode with its original endpoints,
normal and the other two node identities for later sewing. Reverse links
also retain their original validation order, including null identities.
Flags `0x01000000` disable construction; `0x02000000` disable links.
Each distinct floor has its own midpoint map, while node identities belong
to the shared graph. The original caller's force-node variant is not
exposed by this triangle builder: `_CreateNodes` always passes false.

## What the differential executes

Original triangle traversal, geometry arithmetic, node lookup/insertion,
RB rotations, InvalidNode and validation-vector writes, normalization and
edge weight writes run as original ARM32 instructions. GraphSparse node
and edge allocation/lookup are supplied storage fixtures; the original
GraphSparse containers are not reconstructed by this test. Floor support
answers are also supplied fixtures. Every support query coordinate and
its short-circuit order is compared with ARM64 execution.

The corpus covers all 314 exported Crypt triangles with all-pass,
all-reject and mixed support answers; shared edges, duplicates, multiple
floors, both flags, zero-length edges and chains around the CompPos
epsilon. Nodes, links, rejected records, reverse-link references and RB
trees are compared after every triangle. Imported IEEE single-precision
operations and sqrt are modeled; arithmetic NaN payload/sign is portable
only as a NaN class. Logical snapshots in the replay fixture canonicalize
NaN words before hashing. Finite float bits compare exactly.

## Integration boundary

This historical builder checkpoint used supplied floor support answers.
Later [floor records](../floor-records/NOTES.md) supply actual Crypt geometry,
flags, identities, selectors and collision, and build the graph during gameplay
load. [Floor sewing](../navigation-link/NOTES.md) now links those graphs using
original boundary and room-order rules. Graph search, endpoint selection,
smoothing, obstacles and the original movement controller remain pending.
The existing Prince movement policy is separate. Graph construction/sewing
does not establish an original route or moving-enemy behavior.

The caller supplies bounded storage. Invalid requests and insufficient
storage reject before writes; storage checks conservatively reserve the
maximum outputs for one triangle. This allocation boundary is modern
adapter policy, not original allocator behavior.
