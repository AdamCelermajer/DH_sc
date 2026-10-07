#pragma once
#include "application_services_owner_v5.hpp"
#include "trophy_manager_owner_v1.hpp"
#include "properties.hpp"
namespace dh2::application {
struct MainMenuProcessLeavesV114 {
 std::shared_ptr<void> provider;
 std::function<bool(std::string&)> current;
 // SAME PM Character660's CharProperties560, pinned through synchronous call.
 std::function<bool(std::uintptr_t,std::shared_ptr<void>&,data::PropertyView*&,std::string&)> properties;
 // Original nativeIsWifiAlive JNI CallStaticIntMethod; any nonzero is true.
 std::function<bool(std::int32_t&,std::string&)> internet_access;
 // SAME CXPlayerManager::s_inst byte5; never COnline.byte5 or a copied flag.
 std::function<bool(std::shared_ptr<void>&,std::uint8_t*&,std::string&)> cx_player_state5;
 std::function<bool(std::int32_t,bool&,std::string&)> upload_my_score;
 // Actual process manager + actual Arrays::TrophyTable.size cell. Main lends
 // existing owner; this helper creates/reloads no trophies or catalog.
 std::function<bool(std::shared_ptr<void>&,trophies::TrophyManagerOwnerV1*&,const std::uint32_t*&,std::string&)> trophies;
 std::function<bool(std::int32_t,std::string&)> notify_trophy;
};
bool source_send_score_v114(ApplicationServicesOwnerV5&,const MainMenuProcessLeavesV114&,std::string&);
bool source_unlock_trophies_v114(ApplicationServicesOwnerV5&,const MainMenuProcessLeavesV114&,std::string&);
// Uses existing lazy process COnline facet, actual ctor byte5 and actual setter.
// It does not alter multiplayer BSS, CXPlayerManager5, Appab or main-menu event.
bool source_menu_online_v114(ApplicationServicesOwnerV5&,std::shared_ptr<SourceOnlineLoadingOwnerV55>&,bool&,std::string&);
}
