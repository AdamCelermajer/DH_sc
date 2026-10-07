#pragma once
#include "character_ai_pointer_fields_v105.hpp"
#include "character_target_bindings.hpp"
#include <functional>
#include <string>
#include <vector>
namespace dh2::character {
enum class AggroFrameQueryV108 {Player,Npc,Monster,Faerie,Remote,MyTurn,HasAggro,AwaitingSpawn};
enum class AggroRelationV108 {Enemy,Friend,Neutral};
struct AggroFrameTargetV108 {std::uintptr_t identity{};std::uint32_t flags{};};
struct AggroFrameServicesV108 {
 std::function<bool(AggroFrameQueryV108,bool&,std::string&)> query;
 std::function<bool(const char*,bool&,std::string&)> debug;
 std::function<bool(std::uint32_t&,std::string&)> dt,random200;
 std::function<bool(std::uintptr_t&,std::string&)> highest,current_character,local_player;
 std::function<bool(std::uintptr_t,float&,std::string&)> threat;
 std::function<bool(float&,std::string&)> switch_factor,spotted_amount,view_radius,no_aggro_radius,spawn_radius;
 std::function<bool(std::uintptr_t,bool&,std::string&)> has_relation;
 std::function<bool(AggroRelationV108,std::uintptr_t,bool&,std::string&)> relationship;
 //Select actual source list60 or70, never a nearest/map/world substitute.
 std::function<bool(bool tracked,float,std::uint32_t flags,std::vector<AggroFrameTargetV108>&,std::string&)> search;
 std::function<bool(std::uintptr_t,std::string&)> clear;
 std::function<bool(std::uintptr_t,float,std::string&)> add;
 std::function<bool(std::uintptr_t,std::string&)> set_target;
 std::function<bool(std::uint32_t,std::uintptr_t,std::string&)> raise;
 std::function<bool(std::string&)> noncharacter_assert;
};
bool character_update_aggro_v108(CharacterAiPointerFieldsV105&,TargetState48&,
 const AggroFrameServicesV108&,std::string&);
}
