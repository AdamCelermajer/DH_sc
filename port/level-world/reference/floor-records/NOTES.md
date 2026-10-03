# Authored floor records and gameplay collision

`floors.cpp` builds eight owned floor records from the real Crypt BRES streams
and authored room layout. It supplies the reconstructed mesh constructor,
octree, selector, collision and graph routines with actual asset geometry.
`world::height` now calls that native selector/collision chain. Cross-floor
surface selection, movement radius, subdivision and step limits remain new
adapter policies; the original movement controller is not reconstructed.

## Original behavior and ownership

Collada `constructNode` creates a CSceneNode with the authored transform, then
an identity-local-transform BaseMeshSceneNode child. `_LoadNavMesh` reads the
floor container's name and properties, moves its mesh child to its cached
absolute position, and copies that child's local rotation and scale into a
parentless clone. Consequently the floor clone uses absolute translation and
the mesh child's identity rotation/unit scale for this Crypt layout. The
container's rotation and scale are not copied into the clone.

PFFloor construction and PFRoom's caller initialize type flags to zero and
object flags to one. `_CreateNodes` reads **object flags at +0x20**, rather than
type flags at +0x24. The record retains both words separately. Crypt's floor
nodes have no `floortypes` overrides. The native loader rejects such overrides
until the original CStrProps decoding/map lookup is reconstructed; it does not
silently apply default flags to them.

The original selector constructor reverses triangle corner order and bakes
the clone transform. Bounds use original interval transform arithmetic and
expand Z by 1000 in each direction. The graph sees unraised triangles through
the real selector. Only after graph construction does the floor's separately
retained triangle array receive +1 Z. Collision geometry stays unraised.

Native records own triangle arrays, octree storage and selector workspaces.
Stable record addresses and a stable world owner preserve graph callbacks.
This ownership API is a modern adapter. Original deep resource copying,
clone construction/name ownership, scene detachment and the full loader are
not executed or reconstructed as a complete lifecycle.

## Evidence

The engine-hash-bound captures include the loader, CopyMeshSceneNode, position/
rotation/scale/name access, relative and absolute matrix updates, Collada node
construction and floor construction. Capturing a body does not establish its
complete reconstruction.

`floor_records_differential.py` executes the original loader's position-move
range, complete CopyMeshSceneNode choreography, actual getters and dirty matrix
updates, selector constructor, bounds routines and retained-triangle +1 loop.
Deep resource copy, clone object construction and name ownership are supplied
services. Eight authored clones plus 200 nontrivial transform cases match each
packaged ARM64 build. All 314 authored triangles, eight bounds and 314 raised
triangles match. The actual shared asset decoder and node-matrix code execute.

The exported floor records then drive the original/native graph, manager,
selector, octree and collision instructions: 314 triangle insertions and 1,239
real support queries match all intermediate logical snapshots. Final state is
321 nodes, 778 edges, 300 invalid records and 942 validation references, with
FNV-1a64 `542a55a83f199cd9`. Object flags and bounds come from actual records.
The host asset loader independently builds the same final graph state.

`inspect_floors crypt.bdae crypt01.dwld output.bin` exports the FLI1 asset
streams, clone inputs, flags, bounds and both triangle arrays. `navigation_audit`
replays the original-verified NAV3 snapshots under ASan/UBSan. The Android
[checkpoint validator](../../../android-native/tools/validate_floor_records_checkpoint.py)
binds these checks to both built APKs, their assets and ten emulator world cases.

## Remaining work

The later [floor-sewing checkpoint](../navigation-link/NOTES.md) now links
the actual graphs and follows original post-load room order. Original property
parsing, complete scene/clone lifecycle, rotated/generated room producers,
route search, smoothing, obstacles and the original current-floor/movement
controller remain pending. Building and linking the graph
does not yet make monsters pursue the player. Physical ARM64 testing and
original GPU parity remain unverified; the full game and cache are incomplete.
