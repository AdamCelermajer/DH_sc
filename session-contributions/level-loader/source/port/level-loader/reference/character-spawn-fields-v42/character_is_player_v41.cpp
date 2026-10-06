#include "character_is_player_v41.hpp"
#include <cstring>
namespace dh2::character {
bool character_is_player_v41(const CharacterIsPlayerFieldsV41& fields,
 const CharacterIsPlayerServicesV41& services,bool& out,std::string& error){
 if(!fields.receiver_lease||!fields.identity||!services.actual_get_char_type){error="Required SAME Character receiver and actual GetCharType3a3054 provider";return false;}
 std::int32_t type{};if(!services.actual_get_char_type(fields.identity,type,error))return false;
 bool player;
 if(type){player=type==1;}
 else{if(!fields.live_archetype){error="Required SAME live type0 Character archetype CString";return false;}const auto* text=fields.live_archetype->c_str();player=std::strstr(text,"PlayerCharacter")==text;}
 out=player;error.clear();return true;
}
}
