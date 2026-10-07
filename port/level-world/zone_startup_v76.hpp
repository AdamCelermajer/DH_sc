#pragma once
#include "game_object_initialization_owner_v1.hpp"
namespace dh2::world {
// Reached engine resources only. This bag supplies no whole Zone InitPost,
// GameObject body, RNG, actor, registry, collision/save/quest or destructor.
struct ZoneStartupServicesV76 {
 std::shared_ptr<void> owner; // independent engine authority; weak World scope
 // Source virtual9c receives EXACT base.relative_aabb144(), false. The
 // implementation must operate on that same runtime (e.g. relative_box_v3).
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,const float*,bool,std::string&)> set_bounding_box;
 // Genuine40-byte Zone physical resource C1/dispatch/SetPhysicalObject(false).
 // Original46f2f0: App44 physicalWorld, THIS base, 1, stack0/1/0/-5/0x800,
 // trigger?4:0x51e,0; then actual source394bf8 assignment. Provider retains
 // every allocated prefix in its native journal on failure, never drops it
 // as a successful passive shared_ptr D0 or fabricates physical2dc.
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,bool,std::string&)> create_zone_physical;
 // Genuine470a18(_colzone) publishes SAME384 BEFORE intrusive grab/query/
 // mesh-box/InitWithBoundingBox/hide/triangle-selector operations. Both refs
 // below belong to this actual authored class. A failed continuation leaves
 // that published node+lease live for source destruction, without rollback.
 // Genuine lookup NULL is successful with384==0 and no retained node lease.
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,const char*,std::uintptr_t&,std::shared_ptr<void>&,std::string&)> bind_visual_collision_zone;
};
// Whole original Zone.InitPost39771c common startup over existing storage.
bool zone_init_post_v76(CanonicalGameObjectBaseOwnerV1&,GameObjectInitializationOwnerV1&,
 const GameObjectInitializationServicesV1&,std::array<float,3>& dimensions374,
 const bool& physical380,const bool& trigger381,std::uintptr_t& colzone384,
 std::shared_ptr<void>& colzone_lease,const ZoneStartupServicesV76&,std::string&);
}
