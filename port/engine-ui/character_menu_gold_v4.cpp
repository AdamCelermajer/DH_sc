#include "character_menu_gold_v4.hpp"
#include <cmath>
#include <climits>
namespace dh2::ui {
namespace {bool required(bool v,const char* message,std::string& e){if(!v){e=message;return false;}return true;}
std::int32_t integer(double n){return std::isnan(n)?0:n>=2147483647.?INT32_MAX:n<=-2147483648.?INT32_MIN:std::int32_t(n);}}
bool character_menu_gold_call_v4(CharacterMenuCallV1& call,const CharacterMenuGoldServicesV4& s,std::string& e){
 if(!required(s.owner!=nullptr,"Required same player/gold/text owner",e)||!required(!call.arguments.empty(),"Source-invalid gold callback missing argument0",e))return false;
 double index=0;const auto& arg=call.arguments[0];if(arg.kind==2)index=arg.number;
 else if(!required(bool(call.number),"Required source gold numeric conversion",e)||!call.number(arg,index,e))return false;
 std::uintptr_t receiver=0;bool remote=false;
 if(call.arguments.size()==2&&call.arguments[1].kind==5)receiver=call.arguments[1].object;
 if(call.arguments.size()==3){const auto& flag=call.arguments[2];if(flag.kind==1)remote=flag.boolean;
  else if(!required(bool(call.boolean),"Required source gold remote conversion",e)||!call.boolean(flag,remote,e))return false;}
 std::uintptr_t actor=0;if(!required(bool(s.player),"Required actual NativeGetPlayerChar for gold",e)||!s.player(integer(index),remote,actor,e))return false;
 if(!actor)return true;
 std::int32_t amount=0;if(!required(bool(s.gold),"Required same ItemInventory gold20",e)||!s.gold(actor,amount,e))return false;
 std::string formatted;if(!required(bool(s.parse_integer),"Required original StringManager gold parse",e)||!s.parse_integer("^d",amount,formatted,e))return false;
 CharacterMenuValueV1 value;
 if(receiver){
  // Original rereads gold AFTER parsing, before the first AS setter.
  if(!s.gold(actor,amount,e))return false;
  if(!required(bool(call.member),"Required actual Gold/GoldString AS receiver",e))return false;
  value.kind=2;value.number=amount;if(!call.member(receiver,"Gold",value,e))return false;
  value={};value.kind=4;value.text=std::move(formatted);if(!call.member(receiver,"GoldString",value,e))return false;
  value={};value.kind=5;value.object=receiver;
 }else{value.kind=4;value.text=std::move(formatted);}
 call.result=std::move(value);return true;
}
}
