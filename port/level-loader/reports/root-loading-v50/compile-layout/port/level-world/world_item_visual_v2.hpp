#pragma once
#include "world_item_object_owner_v1.hpp"
#include "retained_gameobject_visual_asset_connection_v2.hpp"
#include "game_object_initialization_owner_v1.hpp"
#include "item_color_lookup_v2.hpp"
namespace dh2::character {
// The Item's qualified GameObject InitPost owns the same resource-backed graph.
// Remaining initialization callbacks are actual world borrows supplied by caller.
class WorldItemVisualV2 {
 std::shared_ptr<RetainedWorldItemObjectV1> item_;
 world::RetainedGameObjectVisualAssetConnectionV2 registry_;
 world::GameObjectVisualAssetOwnerV1 assets_;
 world::GameObjectInitializationOwnerV1 initialization_;
 world::GameObjectInitializationServicesV1 bind(world::GameObjectInitializationServicesV1);
public:
 WorldItemVisualV2(std::shared_ptr<RetainedWorldItemObjectV1>,world::RetainedGameObjectVisualServicesV1,world::GameObjectInitializationServicesV1);
 WorldItemVisualV2(const WorldItemVisualV2&)=delete;
 bool init_post(std::string&);
 bool init_final_v23(bool& source_eligible,std::string&);
 bool apply_mesh_box(std::string&);
 // InitAgain discards GetColor return, then invokes controller PlayClip(0,false,0,0).
 bool init_again_clip(std::string&);
 bool init_again_source(const ItemColorLookupServicesV2&,std::string&);
 bool present(bool&,std::string&)const;
 bool update(std::uint32_t absolute_ms,std::string&);
 bool update_v3(std::uint32_t absolute_ms,const std::function<bool(std::string&)>& notify_visibility_changed,std::string&);
 bool release(std::string&);
 std::shared_ptr<world::RetainedGameObjectVisualV1> visual()const{return registry_.attached();}
 std::shared_ptr<world::RetainedGameObjectVisualV1> constructing_root_v3(std::uintptr_t root)const{return registry_.lookup_root(root);}
};
}
