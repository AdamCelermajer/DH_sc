#include "character_menu_potions_v4.hpp"
#include <cmath>
#include <cstring>
#include <climits>
namespace dh2::ui {
namespace {bool fail(std::string& e,const char* text){e=text;return false;}
std::int32_t integer(double n){return std::isnan(n)?0:n>=2147483647.?INT32_MAX:n<=-2147483648.?INT32_MIN:std::int32_t(n);}
bool write(CharacterMenuCallV1& call,std::uintptr_t receiver,const char* name,const CharacterMenuValueV1& value,std::string& error){
 if(!receiver||!call.member)return fail(error,"Required actual potion AS receiver/member service");return call.member(receiver,name,value,error);
}}
bool character_menu_potions_call_v4(const char* name,CharacterMenuCallV1& call,const CharacterMenuPotionServicesV4& services,bool& handled,std::string& error){
 handled=false;if(!name)return true;
 const bool text=!std::strcmp(name,"NativeGetStringNumPotions"),numbers=!std::strcmp(name,"NativeGetNumPotions");if(!text&&!numbers)return true;
 handled=true;if(!services.owner)return fail(error,"Required same native player/potion/text owner");
 if(call.arguments.size()<(text?2u:1u))return fail(error,"Source-invalid potion callback missing argument");
 const auto receiver=call.arguments[0].kind==5?call.arguments[0].object:0;
 std::uintptr_t character=0;
 if(text){double index=0;if(!call.number||!call.number(call.arguments[1],index,error)){if(error.empty())error="Required actual potion player-index numeric conversion";return false;}
  if(!services.player||!services.player(integer(index),false,character,error)){if(error.empty())error="Required actual GetPlayerChar source query";return false;}
 }else if(!services.local_player||!services.local_player(0,true,character,error)){if(error.empty())error="Required actual GetLocalPlayer source query";return false;}
 if(!character)return true; // source genuine no-character, result untouched
 if(text){std::string format;bool null=false;
  if(!services.localized_format||!services.localized_format(format,null,error)){if(error.empty())error="Required actual localized GAMEPLAYMENUS_POTIONS";return false;}
  std::int32_t count=0;if(!services.count||!services.count(character,count,error)){if(error.empty())error="Required same live ItemInventory potion quantity";return false;}
  std::string formatted;
  if(!services.parse_integer||!services.parse_integer(null?nullptr:format.c_str(),count,formatted,error)){if(error.empty())error="Required source StringManager integer varargs parse";return false;}
  CharacterMenuValueV1 value;value.kind=4;value.text=std::move(formatted);return write(call,receiver,"StrNumPotions",value,error);
 }
 std::int32_t count=0;if(!services.count||!services.count(character,count,error)){if(error.empty())error="Required same live ItemInventory potion quantity";return false;}
 CharacterMenuValueV1 value;value.kind=2;value.number=count;
 if(!write(call,receiver,"NumPotions",value,error))return false;
 // Reread actual capacity after NumPotions setter; synchronous watchers may
 // have changed the same inventory byte. Do not snapshot both values early.
 std::int8_t capacity=0;if(!services.capacity||!services.capacity(character,capacity,error)){if(error.empty())error="Required same inventory signed potion capacity byte2c";return false;}
 value.number=capacity;return write(call,receiver,"MaxNumPotions",value,error);
}
}
