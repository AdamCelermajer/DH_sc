#include "application_pause_save_v1.hpp"

namespace dh2::application {
bool application_pause_save_player_v1(
 const std::shared_ptr<void>& current_level,std::uint32_t phase130,std::uint8_t active144,
 const std::function<bool(const std::shared_ptr<void>&,std::int32_t,bool,std::string&)>& save_player,
 std::string& error){
 error.clear();
 // IDA Application::Pause first obtains the current Level; no Level means no
 // Level::SG_SavePlayer call. Its saved-character path is guarded by both
 // Level+0x130 == 38 and the raw byte at Level+0x144 being nonzero.
 if(!current_level||phase130!=38||!active144)return true;
 if(!save_player){error="Required exact current-Level SG_SavePlayer receiver";return false;}
 return save_player(current_level,0,false,error);
}
}
