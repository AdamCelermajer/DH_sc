#include "player_manager_loading_clear_v70.hpp"
namespace dh2::player {
bool player_manager_clear_loading_v70(PlayerManagerOwnerV1& manager,PlayerNetworkLocalOwnerV4& network,const PlayerOnlineQueryV70& online,std::string& e){
 auto* fields=manager.source_frame_fields_v68();auto* loading=manager.source_loading6cc_v70();bool enabled{};
 if(!fields||!loading||!online){e="Required constructed SAME PM ClearLoadingInfo";return false;}
 // Even though the returned online receiver is unused, its source call is
 // executed before any of the following observable field stores.
 if(!online(enabled,e))return false;
 *loading=-1;fields->byte6d0=0;fields->byte6cb=0;fields->byte710=0;fields->byte71a=0;
 PlayerInfoFieldsV1* local{};if(!manager.get_local_player(0,false,local,e)||!local){if(e.empty())e="Required source GetLocalPlayer for ClearLoadingInfo";return false;}
 bool is_local{};if(!network.is_local(*local,is_local,e))return false;
 if(is_local){
  auto actual=network.borrow(*local);if(!actual||!actual->managed_fields_produced_v70){e="Required SAME derived PlayerInfo ClearLoadingInfo storage";return false;}
  // SetIngame(false), inline NetStruct4e8 assignment, SetDoneLoading(false),
  // SetReadyToRoll(false), SetCharacterDeathTimer(-1), in source call order.
  actual->character_visible4e5_v70=0;
  if(!network.source_set_in_cutscene_v99(*local,false,e))return false;
  actual->character_loading525_v70=0;
  actual->ready_to_roll545_v70=0;
  if(!network.source_reset_dead_local_v99(*local,e))return false;
  actual->loading_clear_produced_v70=true;
 }
 fields->field714=0;fields->byte711=0;return true;
}
bool player_manager_clear_quest_sync_v70(PlayerManagerOwnerV1& manager,std::string& e){
 auto* fields=manager.source_frame_fields_v68();auto* stream=manager.source_quest_sync6e0_v70();
 if(!fields||!stream){e="Required SAME embedded PM StreamBuffer6e0";return false;}
 stream->source_clear_v70();fields->byte710=0;return true;
}
bool player_manager_offline_transition_v70(PlayerManagerOwnerV1& manager,PlayerNetworkLocalOwnerV4& network,const PlayerOnlineQueryV70& online,std::string& e){
 auto* fields=manager.source_frame_fields_v68();if(!fields||!online){e="Required SAME PM online-transition receiver";return false;}
 bool enabled{};
 if(!fields->byte6ca){if(!online(enabled,e))return false;if(enabled){e="Required positive _CheckOnlineTransition online-entry body";return false;}}
 if(!online(enabled,e))return false;if(enabled){e="Required positive _CheckOnlineTransition online-state body";return false;}
 if(!online(enabled,e))return false;if(enabled){e="Required positive _CheckOnlineTransition matching-body";return false;}
 fields->byte719=0;fields->byte6ca=0;
 return player_manager_clear_loading_v70(manager,network,online,e)&&player_manager_clear_quest_sync_v70(manager,e);
}
}
