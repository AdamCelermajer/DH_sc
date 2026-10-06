# Live current Crypt world-touch adapter

Scope is the current retained DACT development graph. The submitted renderer
camera inverse ray is an explicit modern input adapter; selection is whole
original Ctrl_Click, not screen boxes or a nearest-distance replacement.

Include these at global namespace scope, before the renderer anonymous namespace:

```cpp
#include "world_click_fields_owner_v1.hpp"
#include "world_click_application_v1.hpp"
#include "world_touch_projection_v1.hpp"
#include "character_world_runtime_v1.hpp"
#include "floors.hpp"
#include "design_settings.hpp"
#include "renderer_world_touch_live_v2.inc"
```

Required standard headers are algorithm, cmath, cstring. Retain exactly one
`std::unique_ptr<dh2::world::RendererWorldTouchLiveV2> world_touch;` in
PlayerSkillsRuntime (or the canonical player transport). This owns only the five
previously unprojected constructor fields; do not also create another
WorldClickFieldsOwner. Create after the existing `bind_attack(t)`/World
registration and source target binding. Destroy before the World/Floor graph;
recreate on reload, since all services synchronously borrow the current graph.

WorldActor::refresh must publish the canonical absolute AABB pointer, currently
omitted by the renderer. Add to its existing player/NPC branches:

```cpp
// Player branch, alongside existing same runtime heading borrows:
if(native_actor_ready)out->aabb6=prince_runtime.subobjects.absolute_bounds;
// NPC branch, alongside existing same runtime/animation scene borrow:
if(a.npc->source_bounds_ready)
 out->aabb6=a.npc->runtime.subobjects.absolute_bounds;
```

Do not reject the entire registration if bounds are not ready: leave this field
NULL and preserve the earlier initialization lifecycle. The reached nearby
query fails clearly until its real producer completes. These are source+12c
minXYZ/maxXYZ fields: CanonicalGameObjectBaseOwnerV1::absolute_aabb12c() aliases
the same array. Player native_actor initialization writes the actual
CharacterOwnerBounds.absolute_box there. NPC initialize_npc_owner_bounds_v1
writes actual CPU model/InitPost root/CollisionScale bounds and sets readiness.
Neither rendered screen silhouette nor constructor +/-100 boxes are accepted
as the live model bounds. Source position updates must continue refreshing the
same absolute array through the existing actor subobject coordinator.

The current CharacterWorldRuntimeV1 registry has exactly one projected room:
room sentinel -> room -> sentinel, and its object sentinel links the actors_ list
in registration order. Thus this adapter's borrowed cursor visits each currently
registered actor once in DACT insertion order. It does not claim canonical
multi-room ObjectManager adoption; that future producer needs its own iterator.

```cpp
t.world_touch=std::make_unique<dh2::world::RendererWorldTouchLiveV2>();
auto& b=t.world_touch->same;
b.world=t.targets.get(); b.floors=level.native_floor.get();
b.machine=&prince_state; b.target=&t.world->player_object->target;
b.target_services=t.world->player_object->binding.services;
b.submitted_camera=submitted_camera.data();
b.width=&submitted_width; b.height=&submitted_height;
b.blocked=&controller_global_blocked; b.forced=&prince_controller_forced;
b.settings=t.world->saved_options.get();
const auto row=t.world->settings.row_index("Default");
if(row<0)throw std::runtime_error("Actual Default DesignSettings row missing");
b.design=&t.world->settings.rows().at(row);
b.debug=[&t](std::uint32_t site,bool& value,std::string& error){
 const char* key=nullptr;
 switch(site){
 case 0x3ae074:case 0x3ae248:case 0x3ae318:key="isTracingChar_CTRL";break;
 case 0x3ae208:key="UseClickToMove";break;
 default:error="Unknown source Ctrl_Click Debug callsite";return false;
 }
 std::uint32_t word{};
 if(dh2_character_debug_load(t.world->debug.get(),&t.world->debug_files)!=1 ||
    dh2_character_debug_get(&word,t.world->debug.get(),key,&t.world->debug_files)!=1){
  error="Required same source Debug owner/files";return false;
 }
 value=word!=0;return true;
};
```

Source Debug CString creation/destruction is represented by the existing native
Debug public key API, matching the current real attack adapter; no VM callback
or fabricated Debug result is supplied. Actual key/provider failure propagates.

On a HUD-unconsumed world press/release, use **scene viewport pixels** after the
current UI uniform-viewport coordinate conversion. Do not pass SWF logical
coordinates. Call synchronously after actual submitted_camera exists:

```cpp
std::string error;bool hit=false;
if(!t.world_touch->dispatch(scene_x,scene_y,is_release,hit,error)){
 __android_log_print(ANDROID_LOG_ERROR,"DH2Native","World touch required: %s",error.c_str());
}
```

Call press with false, release with true; route each pointer event once, exclude
HUD-consumed joystick/skill/menu input, and do not derive release from frame
polling. The adapter source casting/using path retains the pending source point
fields; it does not claim the future deferred-command consumer is integrated.
The target mutation directly uses the same player target binding already read
by HUD and native skills. No attack is automatically issued by this adapter.

Add world_click_target_v1.cpp, world_touch_projection_v1.cpp,
character_world_ai_neutral_v1.cpp to the build. Runtime neutral shares its existing
handle registry/faction rows/current-target/assertion services. Existing selector,
collision and floor sources are already linked. Both ABI syntax is checked in
tests/world_touch_live_v2.cpp; no live touch or full positive runtime claim yet.

Reached gaps are honest: current DACT graph has no canonical generic ObjectManager
tree, so generic selection fails explicitly when reached; source no-target move
requires the genuine virtual Ctrl_Click movement continuation and fails if
reached. For an eligible enemy hit, neither gap is reached. Missing scene target
fields/AABB/State/Debug fail before inventing selection. Floor traversal uses the
actual source ordered room/floor vectors, masks source special flags, and raycasts
the SAME retained selector geometry through dh2_selector_raycast. No physics or
floor ownership is changed.
