#pragma once
#include <level_faery_placement_v8.hpp>
#include <player_manager_owner_v1.hpp>
namespace model_renderer {
struct SourceTerminalNativeV97 {
 std::shared_ptr<void> owner; //independent Main transport, callbacks weakWorld
 std::function<bool(dh2::player::PlayerInfoFieldsV1&,bool&,std::string&)> is_host;
 std::function<bool(bool&,std::string&)> local_hosting,all_loading_done,all_clients_ready;
 std::function<bool(dh2::world::CanonicalObjectManagerV1&,std::string&)> network_init;
 //Actual Main resource leaves for reached faery/follower methods. Loader
 //retains source3f0898 sequencing; no replacement Level/actor is supplied.
 dh2::world::LevelFaeryPlacementServicesV8 faery;
 std::function<bool(dh2::player::PlayerInfoFieldsV1*&,std::shared_ptr<void>&,std::string&)> hosting_player;
};
}
