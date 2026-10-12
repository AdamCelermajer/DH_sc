# Independent actual visual child / colzone source batch

SOURCE ONLY, UNCOMPILED/UNRUN. Shared Root read-only. No tests/build/syntax/hash/
proof/freeze/device/fixture jobs were performed. Parent owns active V69 merge.

New production files: retained_visual_child_v91.hpp/.cpp.
One new CPP links into the SAME existing level-world production owner; it uses
existing scene/asset-payload/math owners. No new target/cache/Scene/World exists.
New renderer helper: renderer_retained_colzone_v91.hpp (include globally BEFORE
V69); it is header-only. Existing Core Node/Instance and Visual layout changes
require coherent rebuild of ALL scene/animation/skinning/visual users at Main's
single integrated milestone, not interposition over stale binaries.

child-storage-integration-v91.patch changes CURRENT scene.hpp and actual Visual
HPP/CPP. Node/Instance vectors wrap independently owned native storage. Existing
parser, animation, matrix, visibility and detach writes address those SAME cells.
Copy creates independent scene/subtree storage, preserving existing copy domain;
move retains the actual backing. Existing Instance parser constructor retained.
Node flags/visibility/detach and mesh pose/flags/visibility views own NO shadow
arrays. Grabbed native child owns its actual node storage, descendants and mesh
resources, with weak parent; no strong Visual/base/World/BRES-byte alias exists.

Mesh resource owns decoded source XYZ/indices/bounds and live positions. Existing
skinned render path writes and reads those SAME positions/source-position cells;
selector pins this actual concrete source SVertexStream. The separate CPU
skinned draw output remains the existing render projection on the same owner;
selector never silently substitutes world-space CPU draw output for IMesh data. Child/root release can retire
Visual/BRES bytes while the grabbed node, mesh and selector remain valid.
Named identity is the actual handle object allocated at construction; node384
lease.get equals that object's address. Genuine searchNULL has no retained node.

Original positive startup:397818 lookup;397820 publishes384 BEFORE grab;
39786c SearchByType('daem') over complete subtree, hidden meshes included;
unique mesh assertion uses actual process mode/file/line0x60/expression.
A skipped invalid count retains grabbed384 with no manufactured selector.
3979a4 captures position before box copy/add. InitWithBoundingBox397594 writes
actual dims Y/Z/X then relative144, SetPosition(true), then recreates POZone if
actual380. Mesh local visibility is hidden BEFORE native selector C1/attachment.
Selector owns SAME mesh, receives original NULL scene-node transform and true
construction domain; no duplicate Node/Visual/actor is constructed.

renderer_retained_colzone_v91.hpp supplies original positive startup and per-peer
selector ray projection. Source Vec3f_K=(0,0,1) is written at312e3c/54/78.
397110 uses peer-relative line +/-float100; diagnostic
DisplayColZoneWhenColliding is queried AFTER collision and performs actual
recursive visibility and unique mesh visibility in reached order.
Plane projection uses SAME original dh2_vec3_normalize and literal358637bd
threshold from585e80; same-side triangle control follows585de8. Ray query is
native source text; exact device numerical agreement remains milestone pending.
Current admitted visual triangles domain is retained: unsupported render
primitive streams fail explicitly; no fabricated geometry/empty success exists.

Additional narrow dependencies:
- trigger-child-field-borrows-v91.patch adds actual mutable dims and physical/
  trigger flags +same384 lease getter to CURRENT TriggerZone, inherited by Exit.
- graph-child-query-v91.patch adds actual weak source resource query to existing
  GraphV68 (preserves Parent V90 native facet); no competing registry.
- colzone-native-lender-v91.patch delivers that graph query through existing V83
  mesh_inside transport. Shared copy timing is preserved through weak live graph.
- colzone-composition-v91.patch replaces the former positive-owner rejection
  and installs weak actual query on graph creation. Apply additions narrowly
  into Parent's CURRENT consolidated V69, not whole provider replacement.

All patches/new files reside in this ONE candidate. No active Core/Visual/scene
source adoption was performed while Parent consolidates current producer.
