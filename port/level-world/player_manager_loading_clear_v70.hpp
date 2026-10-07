#pragma once
#include "player_network_local_owner_v4.hpp"
#include <functional>
namespace dh2::player {
using PlayerOnlineQueryV70=std::function<bool(bool&,std::string&)>;
bool player_manager_clear_loading_v70(PlayerManagerOwnerV1&,PlayerNetworkLocalOwnerV4&,const PlayerOnlineQueryV70&,std::string&);
bool player_manager_clear_quest_sync_v70(PlayerManagerOwnerV1&,std::string&);
// Ordinary offline _CheckOnlineTransition37193c; positive online matching,
// room/status migration remains a separate source implementation.
bool player_manager_offline_transition_v70(PlayerManagerOwnerV1&,PlayerNetworkLocalOwnerV4&,const PlayerOnlineQueryV70&,std::string&);
}
