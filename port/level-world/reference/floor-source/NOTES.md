# Mesh extraction, constructor baking and floor arithmetic

`floor_source.cpp` reconstructs CTriangleSelector's mesh-buffer extraction and
optional baking, the `_LoadNavMesh` substring/flag operations and world/floor
bounds arithmetic. Original engine SHA-256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Sixteen captured functions retain their original instruction-body hashes.

The selector iterates mesh buffers in order, accepts engine primitive type 6
and position widths 2/3/4, and supports seven signed/unsigned integer/float
scalar types. Indexed input is unsigned 16-bit; absent indices use consecutive
vertices. Triangles use corner order 2,1,0. Two-component positions receive Z=0;
four-component positions ignore W. Vertical and degenerate faces are retained.
Baking uses the original product/add association, ends with translation and
ignores the matrix's identity hint. This differs from selector query shortcuts.

Tags use case-sensitive substring matches for `void`, `wall`, `hole` and
`water`, OR-ing floor bits 0x01000000/0x02000000/1/2 respectively. A void/wall
floor mask also ORs object bits 0x07000000. Existing bits are retained; the first
NUL terminates the string. These are arithmetic operations on caller-supplied
initial flags, not a reconstructed floor constructor or metadata lookup.

World bounds use the original `transformBoxEx` minimum/maximum interval
arithmetic. It always computes the matrix transform and retains the recovered
float association. Floor bounds then add 1000 to maximum Z and subtract 1000
from minimum Z. Selector query `transformBox` uses another algorithm.

## Evidence and scope

The differential runs the complete original CTriangleSelector constructor,
all seven format-dispatch/createTriangles bodies and constructor baking.
Allocation, mesh getters, ownership and buffer mapping are caller services.
Numeric conversion and triangle output execute original instructions. The
world-box getter executes its dirty and cached paths and `transformBoxEx`.
Tag and floor-bound checks enter `_LoadNavMesh` at 0x520bd4 and 0x520ce4 and
stop at 0x520c60 and 0x520d68 respectively. They do not execute the full loader.
The unfiltered getTriangles capture is retained for subsequent loader work;
this test reads the original constructor's vector directly.

Standalone, packaged and Studio ARM64 binaries each match 328 constructions,
850 triangles, 212 tag cases and 512 world/floor-bound cases. Thirteen cases
give nonidentity matrix values an identity hint to exercise unconditional
baking. Both packaged tests execute 5,100 calls into the actual packaged
vertex decoder library; no Python replacement supplies its decoded values.
Finite float bits match, with arithmetic NaNs compared as an unordered class.
The FMS2 corpus has SHA-256
`4a7f723bf4fa4fe4f2b8de4248d8d5f0400e0161843db56cbf30a633622cfbcd`.
`floor_source_audit path/to/floor-source-original.bin` replays it under
ASan/UBSan and checks 249 atomic capacity/index/topology rejections.

Streams, matrices, tag text and initial flags in this historical corpus are
experiment inputs. The later [floor-records checkpoint](../floor-records/NOTES.md)
adds actual BRES streams, floor identities/default flags, mesh-child clone
transforms, owned selectors, graph construction and +1 stored-triangle Z.
Its selector/collision chain now controls gameplay height. Full metadata
parsing, scene lifecycle and route search remain pending.
The APK contains no original ARM32 library or instruction execution runtime.
