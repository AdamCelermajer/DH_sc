# PFRoom loading from SAME retained scenery visual

Source-only; no configure/compiler/build/tests/hash/fixture/native/device jobs.
Root is read-only; adopt source into its existing owner only after the matching
RetainedGameObjectVisualV1 V76 APIs are present. No new CPP or implementation TU.

The coherent pair module_pf_room_v3.hpp/.cpp adds
ModulePFRoomsV3::load(RetainedGameObjectVisualV1&, unsigned sourceRoom, name, out,error).
Existing ModuleStaticScene overload and new retained-visual overload invoke ONE
source algorithm, preserving actual room publication, hidden attached mesh-search
snapshot, floor clone/Debug/load/hide/remove/nav append/map order and exit removal.
Caller supplies the already-existing visual; no new ModuleStaticScene/Scene/data
graph, visual C1, PF world, record authority or copied triangle world is created.

The new overload consumes actual V76 mesh local quaternion/scale and actual stable
attached/removal membership. All geometry/properties/name/world matrices come
from THAT visual's same retained Scene/BRES. It pins resource_owner_v76(): one
independent actual immutable byte block. PF clones preserve those bytes after
Visual D0 drops its own resource owner; no Visual/base/actor alias is held as a
substitute for the native shared resource lifetime.

V69 Decor/AnimatedDecor floor service can resolve its actual visual and call this
overload on candidate.module_rooms_v69.lock(). Actual empty-floor search returns
native nullable room only AFTER the real search, rather than inferring absence
from model names, visibility-filtered inspection data or missing providers. Same
room/flags24/ExtendBounds semantics stay on the original retained rooms owner.

Existing explicit source gaps remain: minimap AddNodeToMap, unsupported raw mesh/
floor domains and eventual real teardown/runtime agreement. This overload does
not turn those into success. Source snapshot scheduling/modern safety limits
stay declared adaptations, not ARM allocation/vtable parity claims.

Required coherent V76 visual methods: resource_owner_v76(),
mesh_attached_v76(unsigned), source_mesh_pose_v76(unsigned,array<float,4>&,
array<float,3>&,error), remove_floor_mesh_v76(unsigned,error),
remove_node_v76(unsigned,error). Their current actual owner is the static/hidden
visual source worker; merge the batches together, retaining Main's latest scene
clock/generic callbacks/Character/skinning/material/render changes.

The paired renderer_decor_floor_v77.hpp composes missing Decor/AnimatedDecor
load_room and extend_bounds services on the current V69 bundle. The visual root
is resolved against SAME graph-owned actual visuals; the existing PF owner loads
THAT graph and returns its actual room/flags24. A missing floor is accepted only
after actual source search. No alternative scene/world/room registry is created.
Original Decor already stores flags24, so new extend_source_bounds_v77 performs
only actual room then PFWorld bounds extension. Historical combined
extend_owner_bounds preserves its flag-write API and delegates to that SAME
kernel. Include the header and invoke bind_retained_decor_floor_v77(weak,output)
after the existing V49 physical service binding. Existing real supplied services
remain intact. Candidate/source adoption only; integrated validation pending.

Current paired CPP now also preserves original selected-minimap reached order:
no early failure merely upon finding minimap. Each actual floor load/clone/first
map add completes before the positive minimap transfer boundary; absence uses
the actual last clone for the second map add. No-floor/minimap-only search can
still return nativeNULL room. Independent authored minimap receiver ownership
remains unsupported. candidate/minimap-map-source/SWAMP-MINIMAP-SOURCE.md
records scoped original nine Modules (421 mesh children/16 floors/zero minimap)
and ten Decor (11 children/zero floors/minimap). This is source-data branch
inspection, not runtime/native parser or complete-level acceptance.
