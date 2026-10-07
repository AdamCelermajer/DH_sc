#pragma once
#include "navigation_objects.hpp"
#include "native_body.hpp"
#include <cstdint>
#include <string>
namespace dh2::character {
// Sole recovered ObjectBase/GameObject lifecycle fields. byte80 is deliberately
// unavailable until source PropertyMap defaults/overrides or SetVisible write
// it: neither original constructor stores it.
struct WorldNpcObjectFieldsV1 {
 std::uint8_t visible80{}, disabled81{}, static84{}, updating85{}, enabled8a{1};
 std::uint8_t non_zonable2ed{}, zoning2ee{1}, entered2f0{}, disabled373{};
 std::uint8_t assigned2ef{};
 std::uintptr_t room2f4{};
 bool visible_written{};
};
enum class WorldNpcObjectMethodV1 : std::uint32_t {
 SyncVisibility, EnablePhysicalFilter, DisablePhysicalFilter, CharacterStop
};
struct WorldNpcObjectServicesV1 {
 void* context{};
 // Actual retained VisualObject / PhysicalObject presence, refreshed per call.
 bool (*has_visual)(void*){};
 physical::NativeBody* (*physical)(void*){};
 int (*method)(void*,WorldNpcObjectMethodV1,std::string&){};
 // Source virtual slots b4/b8/bc. Each query occurs only when reached.
 bool (*is_pf_obstacle)(void*,bool&,std::string&){};
 bool (*obstacle_weight)(void*,float&,std::string&){};
 bool (*obstacle_extent)(void*,float&,std::string&){};
 bool (*absolute_aabb)(void*,float* six,std::string&){};
};
// Character vtable b4/b8/bc: whole leaf methods 3a2ee8/3a2ef0/3a2efc.
// Keeps every other provider and its borrowed context unchanged.
WorldNpcObjectServicesV1 character_pf_services_v1(WorldNpcObjectServicesV1) noexcept;
// Borrows the actor's ONE PFObject (also used by ActorRuntime), native physical
// body and lifecycle fields. It owns no FSM, AI, health, property or position.
class CharacterWorldNpcObjectV1 {
 WorldNpcObjectFieldsV1& fields_;
 navigation::NavigationObject& pf_;
 const navigation::CollisionWorld* geometry_;
 navigation::ObstacleRegistry* obstacles_;
 std::uint64_t identity_;
 WorldNpcObjectServicesV1 services_;
 std::string error_;
 bool method(WorldNpcObjectMethodV1);
 bool visual();
 bool set_visible_impl(bool);
 bool game_enabled(bool);
public:
 CharacterWorldNpcObjectV1(WorldNpcObjectFieldsV1&,navigation::NavigationObject&,
  const navigation::CollisionWorld*,navigation::ObstacleRegistry*,std::uint64_t,
  WorldNpcObjectServicesV1);
 bool set_visible(bool); // GameObject::SetVisible 38b0f0
 bool enabled();         // Character::Enabled 3a598c
 bool disabled();        // Character::Disabled 3a5974, including Stop tail
 bool set_enable(bool);  // ObjectBase::SetEnable 33ddc8
 bool zone_entered();    // GameObject::ZoneEntered 38c710
 bool zone_exited();     // GameObject::ZoneExited 38c69c
 bool read_visible(std::uint8_t&)const noexcept;
 bool update_pf();       // whole GameObject::UpdatePFObject 393ea0
 // Only the PFWorld::InitObject call in InitFinal; caller must have completed
 // spawn/disabled gates and must still run the source visual/light/node tail.
 bool init_pf_object(const float* position3,const float* absolute_aabb6);
 const std::string& error()const noexcept{return error_;}
};
}
