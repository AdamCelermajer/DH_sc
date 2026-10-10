# Character-menu Map page source adapter

`PresenterV1` is a fail-closed adapter around the exact `MenuCharMenu_Map`
source owner. It borrows one live campaign's Level, RoomZone registry, camera,
local player/Character, save, quest and event owners on each refresh. Its render
callback must execute the source `RenderMap` projection against those same
owners. Legend, reset-zoom and camera Show/Hide are delegated to their source
owners. No map geometry, scale, room rectangles or marker records are
manufactured here.

Readiness verifies the original `dqcharmenu_droid.swf` SHA-256 and MapSheet
sprite655, then requires all live providers. The tree has the native Map C1/Init
and icon-storage owner (`engine-ui/menu_postmovie_v62.*`). The new
`source_map_room_zone_v1.*` adapter accepts only a completed canonical factory
record, calls that receiver's actual `source_has_been_visited_v104` (which
borrows its same Module visited byte) and `source_has_inside_v104` fields, and
keeps the factory record alive through the Map.Show pass. It does not enumerate
or substitute the Level collection. `source_map_current_level_room_zones_v1`
composes a fresh caller-provided walk of the exact current Level+36 list through
those canonical records, preserving source order and publishing transactionally.
The integration caller still has to bind that list walk and record lookup; no
public caller exposes the original Level+36 RoomZone list or ordered Level+244
object walk yet. The map feature
now adapts the same `GameplayCameraRuntimeV11` `CameraViewV11` matrices and
requires a borrowed frame lease plus the actual active SceneManager and
CameraBase identities before and after view acquisition. The caller still has
to publish that frame/identity binding. Therefore Show/RenderMap and marker
production remain disconnected from the live Level. The original-resource regression
checks the actual SWF tag and authored `RenderMap` placement matrix; it proves
the source canvas anchor, not live world projection or pixel acceptance.

`source_map_kernel_v1.*` carries recovered native math from the original IDA
bodies. `source_map_show_bounds_v1` partitions actual visited/unvisited
RoomZone leases, tests object-bound centers through `RoomZone::HasInside`,
applies the original visibility result, unions admitted object bounds and
expands X/Y by one complete span on each side. `source_map_camera_offset_v1`
clamps CameraLevel translation around the current player's same X/Y position
while preserving Z. `source_map_project_marker_v1` accepts a position from a
live source marker owner, invokes its actual `CameraBase::GetScreenCoord`
service, then applies the exact gameswf rectangle order
`[left,right,top,bottom]`. The recovered
`source_camera_base_screen_coord_v1` implements CameraBase::GetScreenCoord from
its actual function body: it asks the same SceneManager for vtable+112 modes 0
and 2, forms projection*view, transforms the world point and divides by W. It
still needs a real same-SceneManager matrix borrower; a GameplayCameraRuntime
eye/target-derived view is not a substitute. `MapCameraSourceConfigV1` records the source camera
asset, animation set/clip and `SetData` constants; creating and targeting that
CameraLevel still requires the live camera factory and Level/player providers.
The source map-camera adapter stores byte133=1, word136=0 and float140=1,
disables damping, applies `SetData` through the exact +316/+312/+304/+308/+276
sequence, then applies the direct +276 up-vector override. The additive
runtime setters keep those mutations on the same retained camera. `SetData`
sets FOV, aspect, near/far and `(0,0,1)` up; Map then overrides up to
`(-1,1,0)`.

The recovered producer calls are recorded in the integration JSON and checked
against the original IDA pseudocode: objective families use the same local
Character's current Quest objectives; player families use actual PlayerManager
Character positions; NPC families use source visibility/classification flags;
exit families use the source Level object walk and PFWorld RoomExit list. Five
family slots have no producer in these recovered Show methods and stay
reserved. No marker coordinates are synthesized.

The projection adapter borrows both matrices from the same runtime's
`CameraViewV11`. Its frame validator must prove that the active SceneManager
and CameraBase identities still match the held frame both before and after
that view borrow; the current root has not wired this provider yet.

Recovered native fields: map-camera pointer `this+488`; expanded display
bounds `this+212/+216/+224/+228`; local-player XYZ `this+236/+240/+244`;
icon vectors at `this+260` with 18 families; legend byte `this+484`. These are
source-layout findings, not host ABI declarations.

Run `py -3 test_map_ui_source_resources.py` and
`py -3 test_map_ui_source_providers.py` from the repository root. Compile
`source_map_kernel_v1.cpp` with `source_map_kernel_v1_tests.cpp` as a C++17
standalone test. The integrated acceptance still needs a captured original map
page on a known level with visited/unvisited rooms, live player/NPC/objective
markers and legend/reset-zoom interactions, plus a same-profile runtime
capture.

`source_map_decoded_room_zones_v1` is a separate decoded-geometry producer for
`FixedMapV1::Borrow`. It retains the decoded map, calculates each placed
module's source transformed-root AABB from its actual BRES mesh bounds and
scene-instance matrices, and implements the same inclusive-XY `HasInside`
predicate. This remains a RoomZone candidate subset: FixedMap has no actual
RoomZone factory handle/type result, current Level+36 membership, live Module
backlink or `visited3fc` cell. Its visitation is therefore `nullopt`, and
`source_map_decoded_room_zone_inputs_v1` requires an explicit same-live-Module
visitation reader before it can feed the existing Map.Show collector. It fails
without publishing partial bounds/visitation results when required source data
is absent. The actual SWAMP decoded-source test and evidence are recorded in
`../map/runtime_source_map_page_v1-report.json`.
