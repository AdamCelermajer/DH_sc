#pragma once
#include "character_world_npc_object_v1.hpp"
#include "character_script_session.hpp"
#include "character_world_runtime_v1.hpp"
#include <cstdint>
#include <string>
namespace dh2::character {
// Original common Random::s_seed, s_syncedSeed and debug counters. References
// deliberately cannot create another random authority beside existing combat.
struct WorldNpcSpawnGlobalsV1 {std::uint32_t& local_seed;std::uint32_t& network_seed;std::uint32_t& local_count;std::uint32_t& network_count;};
// Recovered GetThis7fd794 -> instance7fd744 -> derived825000 -> base7fd838
// constructor writes byte5=0. This is only
// that mode field, not a successful networking subsystem implementation.
struct WorldNpcNetworkModeV1 {std::uint8_t byte5{};};
struct WorldNpcInitFinalFieldsV1 {
 std::uint8_t once1395{}; // Character ctor3aa1b4
 std::int32_t cached270{-1},threshold274{100}; // GameObject ctor38c130
 std::uint8_t deleted82{}; // no ctor producer claimed; only reached stores
 std::string pf_debug_name254;
};
bool character_npc_spawn_handle_player_v1(skills::CharacterWorldRuntimeV1&,std::uintptr_t,bool&,std::string&);
bool character_npc_actor_player_v1(skills::CharacterWorldRuntimeV1&,std::uintptr_t,bool&,std::string&);
std::int32_t character_generate_spawn_probability_v1(WorldNpcSpawnGlobalsV1&,bool network) noexcept;
// Whole original constructor catalog and GetLightSetIdFromName 40c3cc.
std::uint32_t character_light_set_id_v1(const std::string&) noexcept;
bool character_npc_external_init_final_v1(CharacterScriptSession&,std::uintptr_t actual_active,std::string&);
struct WorldNpcSpawnServicesV1 {
 void* context{};
 // Includes source GetHandle / live lookup before the IsPlayer virtual.
 bool (*handle_is_player)(void*,bool&,std::string&){};
 bool (*network_enabled)(void*,bool&,std::string&){};
 bool (*network_index)(void*,std::int32_t&,std::string&){};
 bool (*network_generator)(void*,bool&,std::string&){};
 bool (*object_delete)(void*,std::string&){};
 bool (*mark_for_deletion)(void*,std::string&){};
};
// Whole CheckSpawnProbability38bd64. cached270 / deleted82 are borrowed source
// fields, threshold274 is read after callbacks. No alternate handle authority.
bool character_check_spawn_probability_v1(WorldNpcSpawnGlobalsV1&,std::int32_t& cached270,
 const std::int32_t& threshold274,std::uint8_t& deleted82,CharacterWorldNpcObjectV1&,
 const WorldNpcSpawnServicesV1&,std::int32_t& result,std::string&);
struct WorldNpcInitFinalServicesV1 {
 void* context{};
 bool (*check_spawn)(void*,std::int32_t&,std::string&){};
 bool (*has_visual)(void*){};
 bool (*sync_visual)(void*,std::string&){};
 bool (*assign_light)(void*,std::uint32_t,std::string&){};
 bool (*has_scene)(void*){};
 // Actual scene node lookup. Null is a valid result; unavailable is failure.
 bool (*find_target_node)(void*,const char*,std::uintptr_t&,std::string&){};
 bool (*char_type)(void*,std::int32_t&,std::string&){};
 bool (*is_player)(void*,bool&,std::string&){};
 bool (*follower_placement)(void*,std::string&){};
 bool (*ai_init_final)(void*,std::string&){};
 bool (*local_player_tail)(void*,std::string&){};
};
// Borrows source once-only1395, threshold274, PF debug-name254, target node180
// and same GameObject lifecycle owner. No property/life/FSM/AI copies.
class CharacterWorldNpcInitFinalV1 {
 WorldNpcObjectFieldsV1& fields_;CharacterWorldNpcObjectV1& object_;
 std::uint8_t& initialized_;std::int32_t& probability_;
 std::string& pf_name_;std::uintptr_t& target_node_;
 WorldNpcInitFinalServicesV1 services_;std::string error_;
 bool game_final(const char*,const std::string&,const float*,const float*);
 bool call(bool (*)(void*,std::string&),const char*);
 bool player(bool&);
public:
 CharacterWorldNpcInitFinalV1(WorldNpcObjectFieldsV1&,CharacterWorldNpcObjectV1&,
  std::uint8_t&,std::int32_t&,std::string&,std::uintptr_t&,WorldNpcInitFinalServicesV1);
 bool initialize(const char* name,const std::string& light_set,const float* position3,const float* absolute_aabb6);
 const std::string& error()const noexcept{return error_;}
};
}
