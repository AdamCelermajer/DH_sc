#pragma once
#include "character_target_bindings.hpp"
#include <array>
#include <functional>
#include <string>
namespace dh2::world {
// Same live Character fields; no owned replacement target or FSM state.
struct WorldClickFieldsV1 {
 std::uintptr_t identity{};
 std::uint8_t* pending14c8{};std::int16_t* skill14ca{};
 float* destination14b0{};float* origin14bc{};
 std::uint8_t* click_target413{};
 character::TargetState48* target{};
 character::TargetServices16 target_services{};
};
struct WorldClickServicesV1 {
 void* context{};
 // Genuine source methods; missing reached providers fail after prefix.
 std::function<bool(bool&,std::string&)> ctrl_allowed;
 std::function<bool(bool&,std::string&)> is_casting,is_using_skill,application_mode320e74;
 // Cursor borrows original Character list / ObjectManager signed handle tree.
 // first=true starts; identity0 means end. Callback reloads next from SAME live
 // container, and must pin iterator/actor through synchronous source callbacks.
 std::function<bool(bool,std::uintptr_t&,std::uintptr_t&,std::string&)> characters,objects;
 std::function<bool(std::uintptr_t,std::array<float,3>&,std::string&)> target_position;
 std::function<bool(std::uintptr_t,std::uintptr_t,bool&,std::string&)> interactive,neutral,enemy;
 // Whole source IsNearby38ac0c; parameter is actual DesignSettings radius.
 std::function<bool(std::uintptr_t,const std::array<float,3>&,float,bool&,std::string&)> nearby;
 // Genuine DesignSettings+2c(enemy),+50(friend),+54(generic special).
 const float* enemy_radius2c{};const float* friend_radius50{};const float* object_radius54{};
 std::function<bool(std::uintptr_t,const char*&,std::string&)> object_type_name5c;
 const char* excluded_type_name{"Character"}; // exact Ctrl_Click type_name+5c source literal
 // Whole Debug Load→CString→query→destroy caller service; callsite selects
 // exact source key, including discarded queries before target mutation.
 std::function<bool(std::uint32_t,bool&,std::string&)> debug;
 std::function<bool(const std::array<float,3>&,bool,std::string&)> move;
};
// Full Character::Ctrl_Click3addc8 ordered control over borrowed world owners.
// stopped=true reproduces source skip/deferred-casting/selection completion.
bool world_click_target_v1(WorldClickFieldsV1&,const std::array<float,3>&,
                           bool released,const WorldClickServicesV1&,std::string&);
// Exact source IsNearby: expanded absolute AABB by radius*100 on every axis,
// inclusive comparisons. No visual bounds, cone, or inferred touch radius.
bool world_click_nearby_v1(const float* absolute_aabb6,const float* point3,float radius)noexcept;
}
