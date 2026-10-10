#include "player_level_quicksave_v29.hpp"
#include <cstring>
namespace dh2::player {
bool PlayerLevelQuickSaveV29::execute(const PlayerQuickSaveLevelV29& level,bool clear,std::string& error){
 error.clear();receipt_={};
 if(busy_||!services_.owner||!services_.players||!level.owner||!level.identity||!level.save_ec||!level.phase130){error="Required same nonreentrant Level/PM QuickSave graph";return false;}
 busy_=true;struct Guard{bool& b;~Guard(){b=false;}}guard{busy_};
 PlayerInfoFieldsV1* info{};
 // Source query is before even the Save_ec/phase guard, and uses the local
 // roster with real Character660 filtering. It does not consume count6c4.
 if(!services_.players->get_local_player(0,true,info,error)||!info){if(error.empty())error="Required source QuickSave PlayerInfo";return false;}
 receipt_.player=info->character660;receipt_.phase=PlayerQuickSavePhaseV29::gate;
 auto finish=[&]{receipt_.phase=PlayerQuickSavePhaseV29::complete;receipt_.complete=true;return true;};
 if(!*level.save_ec||*level.phase130!=38||!info->character660)return finish();
 PlayerQuickSaveCharacterV29 player;receipt_.phase=PlayerQuickSavePhaseV29::alive;
 if(!services_.character||!services_.character(info->character660,player,error)||!player.owner||player.identity!=info->character660||!player.dead){if(error.empty())error="Required actual QuickSave Character IsDead";return false;}
 if(*player.dead)return finish();
 receipt_.phase=PlayerQuickSavePhaseV29::online;std::uint8_t online{};
 if(!services_.online_byte5||!services_.online_byte5(online,error)){if(error.empty())error="Required QuickSave GetOnline byte5";return false;}
 if(online){
  bool host{};if(!services_.local_hosting||!services_.local_hosting(host,error)){if(error.empty())error="Required QuickSave IsLocalPlayerHosting";return false;}
  if(!host)return finish();
  if(!services_.player_manager719){error="Required same QuickSave PM719 field";return false;}
  if(*services_.player_manager719)return finish();
 }
 receipt_.phase=PlayerQuickSavePhaseV29::checkpoint;
 if(!player.position160||!player.checkpoint1468){error="Required same QuickSave position/checkpoint fields";return false;}
 std::memcpy(player.checkpoint1468,player.position160,12);
 const auto actual_save=*level.save_ec;std::uint8_t* flag{};receipt_.phase=PlayerQuickSavePhaseV29::flag;
 if(!actual_save||!services_.save_flag39||!services_.save_flag39(actual_save,flag,error)||!flag){if(error.empty())error="Required actual QuickSave Save39 field";return false;}
 const auto previous=*flag;auto* original_flag=flag;if(clear)*flag=0;
 receipt_.phase=PlayerQuickSavePhaseV29::save;
 // Original rereads Save_ec when its flag was cleared; without that store it
 // calls the captured receiver. Do not replace this with a dummy file sink.
 const auto receiver=clear?*level.save_ec:actual_save;
 const bool saved=receiver&&services_.save_ec&&services_.save_ec(receiver,error);
 const auto save_error=error;
 receipt_.saved=saved;
 // Save is synchronous at this boundary. Restore the original byte even when
 // the native serializer reports failure; on success, honor the source's
 // reread of Level::Save_ec and restore that actual receiver as well.
 flag=nullptr;std::string restore_error;
 const bool reread=*level.save_ec&&services_.save_flag39&&services_.save_flag39(*level.save_ec,flag,restore_error)&&flag;
 if(reread)*flag=previous;
 if(!reread&&original_flag)*original_flag=previous;
 if(!saved){error=save_error.empty()?"Required source LevelSave::Save delivery":save_error;return false;}
 if(!reread){error=restore_error.empty()?"Required reread QuickSave Save39 receiver":restore_error;return false;}
 receipt_.phase=PlayerQuickSavePhaseV29::restore;return finish();
}
}
