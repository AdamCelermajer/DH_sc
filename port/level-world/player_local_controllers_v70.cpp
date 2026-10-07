#include "player_local_controllers_v70.hpp"
#include <algorithm>
namespace dh2::player {
bool PlayerLocalControllersOwnerV70::update(std::string& error){
 if(delivering_||!manager_.source_initialized_v59()||!services_.provider||!services_.gamepad_count||!services_.gamepad||!services_.online){error="Required constructed SAME PM/Input _CheckLocalControllers receiver";return false;}
 delivering_=true;struct Exit{bool& value;~Exit(){value=false;}} exit{delivering_};
 std::int32_t count{};if(!services_.gamepad_count(count,error))return false;count=std::max(count,1);
 // Native admission bound prevents malformed external InputManager storage
 // from driving an unbounded loop; it does not change the captured count.
 if(count>65536){error="Source InputManager gamepad count exceeds native admission";return false;}
 for(std::int32_t index=0;index<count;++index){
  PlayerLocalControllerBorrowV70 pad;
  if(!services_.gamepad(index,pad,error)||!pad.receiver||!pad.identity||!pad.connected758){if(error.empty())error="Required actual InputManager virtual8 controller receiver";return false;}
  const bool connected=index==0||*pad.connected758!=0;
  bool online{};if(!services_.online(online,error))return false;
  if(online){error="Required online _CheckLocalControllers Matching/NetPlayer selection body";return false;}
  const bool missing=!manager_.is_player_in_local_map_v59(index);
  if(connected){
   if(missing){
    // Original source tuple: internal=index, controller=-1,
    // local_index=index, local=true. This is not a bootstrap replay.
    if(!manager_.add_player(index,-1,index,true,error))return false;
    continue;
   }
   PlayerInfoFieldsV1* info{};
   if(!manager_.get_by_internal(index,false,info,error)||!info){if(error.empty())error="Required existing local source PlayerInfo";return false;}
   if(info->save_slot664==-1){
    if(!services_.unselected_profile){error="Required source local controller profile/joining input tail";return false;}
    if(!services_.unselected_profile(index,*info,pad,error))return false;
   }
  }else if(!missing){
   // Original disconnected branch still calls GetPlayerByInternalID(false)
   // but does not remove the record or dispatch the profile-input tail.
   PlayerInfoFieldsV1* info{};if(!manager_.get_by_internal(index,false,info,error))return false;
  }
 }
 return true;
}
}
