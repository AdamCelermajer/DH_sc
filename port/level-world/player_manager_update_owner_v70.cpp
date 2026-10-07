#include "player_manager_update_owner_v70.hpp"
namespace dh2::player {
bool PlayerManagerUpdateOwnerV70::update(std::string& error){
 if(delivering_||manager_.source_running_v59()){error="PlayerManager.Update reentrant delivery";return false;}
 auto* fields=manager_.source_frame_fields_v68();
 if(!fields||!services_.provider){error="Required SAME constructed PlayerManager Update receiver";return false;}
 delivering_=true;struct Exit{bool& value;~Exit(){value=false;}} exit{delivering_};
 std::int32_t count{};if(!manager_.num_players(count,error))return false;
 // Original captures the bound once; getters and raw character cells remain
 // fresh on each iteration, including any preceding service mutation.
 for(std::int32_t i=0;i<count;++i){
  PlayerInfoFieldsV1* p{};if(!manager_.get_player(i,false,p,error))return false;
  if(!p){error="Required PlayerManager.GetPlayer Update receiver";return false;}
  if(p->character660){
   const std::uint8_t* disabled{};std::shared_ptr<void> pin;
   if(!services_.disabled81||!services_.disabled81(p->character660,disabled,pin,error)||!disabled||!pin){if(error.empty())error="Required Character.disabled81 Update borrow";return false;}
   if(*disabled)p->character660=0;
  }
 }
 if(fields->byte71b){
  // 379064 -> 36e478 is GetLocalPlayer(0,false). This selector must
  // remain local even while the transition byte71b is set.
  PlayerInfoFieldsV1* p{};if(!manager_.get_local_player(0,false,p,error))return false;
  const std::uint8_t* visible{};std::shared_ptr<void> pin;
  if(!p||!services_.visible4e5||!services_.visible4e5(*p,visible,pin,error)||!visible||!pin){if(error.empty())error="Required selected local PlayerInfo.visible4e5";return false;}
  if(*visible)fields->byte71b=0;
 }
 constexpr PlayerManagerUpdateStageV70 order[]={PlayerManagerUpdateStageV70::check_online_transition,PlayerManagerUpdateStageV70::check_local_controllers,PlayerManagerUpdateStageV70::check_remote_controllers,PlayerManagerUpdateStageV70::manage_characters,PlayerManagerUpdateStageV70::check_local_deaths,PlayerManagerUpdateStageV70::check_global_deaths,PlayerManagerUpdateStageV70::statistics_update};
 for(auto stage:order){if(!services_.stage){error="Required whole PlayerManager.Update stage receiver";return false;}if(!services_.stage(stage,manager_,error))return false;}
 return true;
}
}

