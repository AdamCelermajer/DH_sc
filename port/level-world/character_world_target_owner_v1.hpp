#pragma once
#include "character_skill_readonly_v6.hpp"
#include "character_skill_target_queries_v6.hpp"
#include "character_controller_commands.hpp"
#include "character_path_commands.hpp"
#include "../game-data/ai.hpp"
#include "../game-data/combat_application.hpp"
#include "../scene-materials/scene.hpp"
namespace dh2::character::skills {
// Complete source GetTargetPosition pointer selection (0x3935dc). Null
// selected backing means a required native borrow is unavailable.
extern "C" const float* dh2_world_target_position_v1(const float* position,const float* cached,std::uintptr_t target_node,std::uint8_t enabled);
struct WorldTargetActorBorrowV1 {
 std::uintptr_t identity{};
 target_search::Object48* search{};
 SkillTargetCharacterV6* character{}; // null means genuine base GameObject
 const scene::Scene* scene{};
 data::CombatActorState* life{};
 // Exact GetTargetPosition source fields: +180 target node, +80 enable,
 // +184 cached absolute position, +160 actual world position.
 const std::uintptr_t* target_node{};
 const std::uint8_t* target_enabled{};
 const float* cached_target_position{};
 const float* position{};
 float* heading_angle{};
 float* controller_heading_angle{};
 const std::uint8_t* base_byte2ed{};
 const float* aabb6{};
};
struct WorldTargetServicesV1 {
 void* context{};
 // Fresh lookup of SAME live actor/Scene/properties. Pointer lifetimes remain
 // valid through source Search and synchronous callbacks; no GL detach/rebind.
 int(*actor)(void*,std::uintptr_t,WorldTargetActorBorrowV1*){};
 // Fresh genuine controller gates. Must retain exact actual controller owner.
 int(*controller)(void*,std::uintptr_t,ControllerCommandState32*){};
 // Original AI_IsFriend is unreconstructed; required only when reached.
 int(*friend_query)(void*,std::uintptr_t,std::uintptr_t,std::uintptr_t*){};
 // Whole original AI_IsEnemy includes shared GetHandle/GetObject, kind and
 // assertion boundaries before faction lookup; cached faction kernel alone
 // does not satisfy this continuation.
 int(*enemy_query)(void*,std::uintptr_t,std::uintptr_t,std::uintptr_t*){};
 // Optional authoritative flags producer. If supplied, reached interaction
 // reads require the SAME State56.flags; unavailable NPC FSM is a failure.
 int(*character_flags)(void*,std::uintptr_t,std::uint32_t*){};
};
class CharacterWorldTargetOwnerV1 {
 const target_search::Registry8& registry_;
 const data::AiTables& ai_;
 WorldTargetServicesV1 services_;
 std::vector<std::int32_t> types_;
 target_providers::Types16 type_view_{};
 std::string error_;
 bool actor(std::uintptr_t,WorldTargetActorBorrowV1&);
 int query(std::uint32_t,std::uintptr_t,std::uintptr_t,std::int32_t&);
 static int query_service(void*,const target_providers::Request24*,std::uintptr_t*);
 static int search_service(void*,const target_search::Request24*,target_search::Response16*);
 static int control_service(void*,const CharacterControlRequest32*,CharacterControlResponse16*);
public:
 // Borrows actual populated World intrusive registry and design AI table.
 // No encounter relation maps, inventory, Save, Session, timer or VM ownership.
 CharacterWorldTargetOwnerV1(const target_search::Registry8&,const data::AiTables&,WorldTargetServicesV1);
 CharacterWorldTargetOwnerV1(const CharacterWorldTargetOwnerV1&)=delete;
 target_search::Services16 search_services()noexcept{return {this,search_service};}
 target_providers::Services16 query_services()noexcept{return {this,query_service};}
 SkillNativeWorldV5 native_world(std::uintptr_t player,const dh2_script_object_services*);
 int look_at(std::uintptr_t character,std::uintptr_t target);
 const std::string& error()const noexcept{return error_;}
};
}
