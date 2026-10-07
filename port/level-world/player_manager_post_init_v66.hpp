#pragma once
#include "application_player_manager_bootstrap_v59.hpp"
#include "application_services_owner_v5.hpp"
namespace dh2::player {
struct PlayerManagerPostInitServicesV66 {
 std::shared_ptr<void> provider;
 // Actual selected PlayerInfo virtual5c (CNet80f124), preserving its own
 // NetStruct fields178/1a0/1c8/1f0. Never equate this with Character Init.
 std::function<bool(PlayerInfoFieldsV1&,bool&,std::string&)> active5c;
 std::function<bool(std::uintptr_t,std::string&)> verify_specialization;
};
// Whole PostInitCharacters36f2bc ordered PM walk. No records/count/Save writes.
inline bool source_post_init_characters_v66(ApplicationPlayerManagerBootstrapV59& pm,
 application::ApplicationServicesOwnerV5& app,const PlayerManagerPostInitServicesV66& services,
 std::string& e){
 auto* manager=pm.manager();auto* network=pm.network();
 if(!manager||!network||!services.provider){e="Required SAME constructed PM/network/PostInit provider";return false;}
 std::int32_t count{};if(!manager->num_players(count,e))return false;
 for(std::int32_t i=0;i<count;++i){
  PlayerInfoFieldsV1* player{};if(!manager->get_player(i,false,player,e))return false;
  if(!player){e="Required original PM.GetPlayer nonnull virtual receiver";return false;}
  bool local{};if(!network->is_local(*player,local,e))return false;
  if(!local)continue;
  bool active{};
  if(!services.active5c){e="Required actual selected PlayerInfo.virtual5c";return false;}
  if(!services.active5c(*player,active,e))return false;
  if(!active)continue;
  const auto character=player->character660;
  // Source reads GetOnline BEFORE null assertion/VerifySpecialization.
  const auto online=app.get_online_loading_v55();
  if(!online){e="Required actual GetOnline source owner";return false;}
  const auto enabled=online->byte5();(void)enabled;
  if(!character){e="Source PostInitCharacters reached NULL Character; unsafe assertion/call rejected";return false;}
  if(!services.verify_specialization){e="Required Character.VerifySpecialization3bd000";return false;}
  if(!services.verify_specialization(character,e))return false;
 }
 e.clear();return true;
}
}
