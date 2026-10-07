#include "character_menu_potions_v4.hpp"
#include <cassert>
#include <iostream>
using namespace dh2::ui;
int main(){
 CharacterMenuPotionServicesV4 services;services.owner=std::make_shared<int>(1);std::vector<std::string> trace;std::string error;bool handled=false;std::int8_t cap=7;
 services.player=[&](int index,bool remote,std::uintptr_t& character,std::string&){assert(index==0&&!remote);trace.push_back("player");character=101;return true;};
 services.local_player=[&](int index,bool remote,std::uintptr_t& character,std::string&){assert(index==0&&remote);trace.push_back("local");character=101;return true;};
 services.count=[&](auto character,int& count,std::string&){assert(character==101);trace.push_back("count");count=5;return true;};
 services.capacity=[&](auto character,std::int8_t& value,std::string&){assert(character==101);trace.push_back("capacity");value=cap;return true;};
 services.localized_format=[&](std::string& value,bool& null,std::string&){trace.push_back("localized");value="actual declared format ^d";null=false;return true;};
 services.parse_integer=[&](const char* format,int count,std::string& result,std::string&){trace.push_back("parse");assert(std::string(format)=="actual declared format ^d"&&count==5);result="declared formatted result";return true;};
 CharacterMenuCallV1 call;call.arguments.resize(2);call.arguments[0].kind=5;call.arguments[0].object=123;call.arguments[1].kind=2;call.arguments[1].number=0;
 call.result.kind=4;call.result.text="unchanged";call.number=[](auto& value,double& out,std::string&){out=value.number;return true;};
 call.member=[&](auto receiver,const char* name,const auto& value,std::string&){assert(receiver==123);trace.push_back(name);
  if(std::string(name)=="StrNumPotions")assert(value.kind==4&&value.text=="declared formatted result");
  if(std::string(name)=="NumPotions"){assert(value.kind==2&&value.number==5);cap=-7;}
  if(std::string(name)=="MaxNumPotions")assert(value.kind==2&&value.number==-7);return true;};
 assert(character_menu_potions_call_v4("NativeGetStringNumPotions",call,services,handled,error)&&handled);
 assert((trace==std::vector<std::string>{"player","localized","count","parse","StrNumPotions"}));assert(call.result.kind==4&&call.result.text=="unchanged");
 trace.clear();assert(character_menu_potions_call_v4("NativeGetNumPotions",call,services,handled,error));assert((trace==std::vector<std::string>{"local","count","NumPotions","capacity","MaxNumPotions"}));assert(call.result.text=="unchanged");
 trace.clear();auto failed=services;failed.capacity={};assert(!character_menu_potions_call_v4("NativeGetNumPotions",call,failed,handled,error));assert((trace==std::vector<std::string>{"local","count","NumPotions"}));
 trace.clear();auto null=services;null.player=[](int,bool,std::uintptr_t& character,std::string&){character=0;return true;};null.count={};null.localized_format={};null.parse_integer={};
 assert(character_menu_potions_call_v4("NativeGetStringNumPotions",call,null,handled,error)&&trace.empty()&&call.result.text=="unchanged");
 trace.clear();failed=services;failed.localized_format={};assert(!character_menu_potions_call_v4("NativeGetStringNumPotions",call,failed,handled,error));assert((trace==std::vector<std::string>{"player"}));
 std::cout<<"Potion whole typed wrappers: source order, SAME identity, fresh signed capacity, null player and failure prefixes PASS; declared backend fixtures\n";
}
