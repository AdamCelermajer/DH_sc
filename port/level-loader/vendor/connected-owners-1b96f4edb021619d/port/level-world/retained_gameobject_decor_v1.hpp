#pragma once
#include "retained_gameobject_visual_v1.hpp"
#include "physical_world.hpp"
#include <functional>
namespace dh2::world {
struct RetainedGameObjectDecorServicesV1 {
 std::shared_ptr<void> owner;
 std::function<bool(const char*,bool&,std::string&)> debug_switch;
 std::function<bool(std::string&)> update_pf;
 std::function<bool(std::uintptr_t,std::string&)> destroy_previous;
 std::function<bool(void*,std::uintptr_t&,std::string&)> peer_owner;
 std::function<bool(std::uintptr_t,std::uint8_t&,std::string&)> peer_visible80;
};
// Source PODecor39fc2c/PhysicalObject46f2f0 selected polygon path, over SAME
// world and parent bounds. Original PF/collision/Debug delivery stays explicit.
class RetainedGameObjectDecorV1 {
 CanonicalGameObjectBaseOwnerV1& base_;RetainedGameObjectVisualV1& visual_;
 physical::NativeWorld& world_;RetainedGameObjectDecorServicesV1 services_;
 physical::NativeBody native_{};physical::WorldObject world_object_{};
 physical::DecorBodyConfig config_{};
 bool assigned_{};
 static unsigned test(void*,void*,const physical::Filter*,const physical::Filter*);
 static void contact(void*,physical::ContactEvent,void*,const float*,unsigned);
 static void velocity(void*,float*);
 bool missing(const char*,std::string&)const;
 bool debug(const char*,bool&,std::string&);
public:
 RetainedGameObjectDecorV1(CanonicalGameObjectBaseOwnerV1&,RetainedGameObjectVisualV1&,
                         physical::NativeWorld&,RetainedGameObjectDecorServicesV1);
 ~RetainedGameObjectDecorV1();
 RetainedGameObjectDecorV1(const RetainedGameObjectDecorV1&)=delete;
 bool initialize(std::string&);
 bool detach(std::string&); // exact SetPhysicalObject(NULL,false), including Debug gate
 bool release(std::string&); // explicit owned-body destruction before World teardown
 physical::NativeBody& native()noexcept{return native_;}
 physical::WorldObject& world_object()noexcept{return world_object_;}
 const physical::DecorBodyConfig& config()const noexcept{return config_;}
 bool assigned()const noexcept{return assigned_;}
};
}
