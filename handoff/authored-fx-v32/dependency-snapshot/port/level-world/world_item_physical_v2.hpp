#pragma once
#include "world_item_object_owner_v1.hpp"
#include "item_body_config_v2.hpp"
#include "physical_world.hpp"
#include "native_body.hpp"
#include <functional>
namespace dh2::character {
struct WorldItemPhysicalServicesV2 {
 std::shared_ptr<void> owner;
 std::function<bool(const char*,bool&,std::string&)> debug;
 std::function<bool(std::uintptr_t,std::string&)> destroy_previous;
 std::function<bool(std::string&)> update_pf;
 std::function<bool(void*,std::uintptr_t&,std::string&)> peer_owner;
 std::function<bool(std::uintptr_t,std::uint8_t&,std::string&)> peer_visible;
 // SAME ObjectHandle GetHandle->GetObject(false)->type_f4 guard; the actual
 // source GetAsCharacter cast is used separately for the peer.
 std::function<bool(std::uintptr_t,std::uintptr_t&,std::uint32_t&,std::string&)> resolve;
 std::function<bool(std::uintptr_t,std::uintptr_t&,std::string&)> as_character;
 std::function<bool(std::uintptr_t,std::uintptr_t&,std::string&)> character_ooi;
};
// Original POItem constructor/assignment and virtual collision routes over
// SAME canonical Item fields and actual shared Box2D PhysicalWorld.
class WorldItemPhysicalV2 {
 RetainedWorldItemObjectV1& item_;physical::NativeWorld& world_;
 WorldItemPhysicalServicesV2 services_;physical::NativeBody native_{};
 physical::WorldObject transport_{};physical::CharacterBodyConfig config_{};
 bool assigned_{};std::string error_;
 b2Shape* secondary_shape_{}; // SAME source PhysicalObject+1c sensor shape.
 static unsigned test(void*,void*,const physical::Filter*,const physical::Filter*);
 static void contact(void*,physical::ContactEvent,void*,const float*,unsigned);
 static void velocity(void*,float*);
 bool missing(const char*,std::string&);
 bool debug(const char*,bool&,std::string&);
public:
 WorldItemPhysicalV2(RetainedWorldItemObjectV1&,physical::NativeWorld&,WorldItemPhysicalServicesV2);
 ~WorldItemPhysicalV2();
 WorldItemPhysicalV2(const WorldItemPhysicalV2&)=delete;
 // These correspond to TWO original calls: PhysicalObjectC2 then
 // SetPhysicalObject(new,false). Never merge/skip their Debug failure prefixes.
 bool construct(std::string&);
 bool assign(std::string&);
 bool detach(std::string&);
 bool collision(physical::ContactEvent,void* actual_peer,std::string&);
 physical::NativeBody& native()noexcept{return native_;}
 b2Shape*& secondary_shape()noexcept{return secondary_shape_;}
 const physical::CharacterBodyConfig& config()const noexcept{return config_;}
 const std::string& error()const noexcept{return error_;}
};
}
