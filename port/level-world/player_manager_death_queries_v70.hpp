#pragma once
#include "player_manager_owner_v1.hpp"
#include <functional>
namespace dh2::player {
struct PlayerManagerDeathServicesV70 {
 std::shared_ptr<void> provider;
 std::function<bool(std::uintptr_t,bool&,std::string&)> character_is_dead34;
 std::function<bool(PlayerInfoFieldsV1&,const std::int32_t*&,std::shared_ptr<void>&,std::string&)> death_timer3a8;
};
bool player_manager_all_players_dead_v70(PlayerManagerOwnerV1&,const PlayerManagerDeathServicesV70&,bool&,std::string&);
bool player_manager_integer714_v70(PlayerManagerOwnerV1&,std::int32_t&,std::string&);
}
