#pragma once
#include "world_item_visual_v2.hpp"
#include "world_item_physical_v2.hpp"
#include "world_item_drop_sound_v2.hpp"
#include "object_enable_condition_v2.hpp"
#include "game_object_set_position_v2.hpp"
namespace dh2::world {class ConditionDataBindingV72;}
namespace dh2::character {
struct WorldItemGraphServicesV3 {
 world::RetainedGameObjectVisualServicesV1 visual;
 world::GameObjectInitializationServicesV1 initialization;
 WorldItemPhysicalServicesV2 physical;
 world::GameObjectSetPositionServicesV2 position;
 world::ObjectEnableConditionServicesV2 enable;
 ItemColorLookupServicesV2 color;
 sound::VoxPlay3DOwnerV2* vox{};std::uintptr_t vox_identity{};
 void* destruction_context{};
 bool(*before_item_destroy)(void*,data::ItemInstanceV1&,std::string&){};
 std::shared_ptr<world::ConditionDataBindingV72> condition_binding_v75;
};
// Same canonical receiver graph used by the real145 pool. No registry/IDs or
// inventory are duplicated; operations not owned here remain required outer
// world services (actor Update, movement/tooltip/local player/interaction).
class WorldItemGraphV3 {
 std::shared_ptr<RetainedWorldItemObjectV1> item_;
 WorldItemGraphServicesV3 services_;
 WorldItemVisualV2 visual_;WorldItemPhysicalV2 physical_;
 bool pending_physical_assignment_{};
 std::uint8_t filter_disabled26_{}; // sole POItem filter toggle field, source ctor0
 world::GameObjectInitializationServicesV1 initialization();
 bool position(const float*,bool,std::string&);
 bool visibility(bool,std::string&);
 bool enable_event(bool,std::string&);
 bool filter(bool,std::string&);
 physical::NativeWorld& world_;
public:
 WorldItemGraphV3(std::shared_ptr<RetainedWorldItemObjectV1>,physical::NativeWorld&,WorldItemGraphServicesV3);
 // handled=false means caller must deliver its actual other source receiver.
 bool clear_conditions_v75(std::string&);
 bool route(const WorldItemRequestV1&,std::int32_t& result,bool& handled,std::string&);
 WorldItemVisualV2& visual()noexcept{return visual_;}
 WorldItemPhysicalV2& physical()noexcept{return physical_;}
 const std::uint8_t& source_filter_disabled26_v4()const noexcept{return filter_disabled26_;}
 RetainedWorldItemObjectV1& receiver_v4()noexcept{return *item_;}
 bool source_visibility_v23(bool value,std::string& e){return visibility(value,e);}
 // TestEnableCondition mutates enabled8a before virtual+44/+48 delivery.
 // Dispatch that event directly; SetEnable would suppress the equal value.
 bool source_enabled_event_v44(bool value,std::string& e){return enable_event(value,e);}
};
}
