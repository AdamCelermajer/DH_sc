#include "character_menu_reload_action_v1.hpp"
#include <cstring>
#include <stdexcept>
namespace dh2::ui {
CharacterMenuReloadActionV1::CharacterMenuReloadActionV1(CharacterMenuReloadActionGraphV1 graph):graph_(std::move(graph)){
 if(!graph_.owner)throw std::invalid_argument("Reload action requires retained player graph");
}
bool CharacterMenuReloadActionV1::dispatch(const char* name,CharacterMenuCallV1& call,std::string& error)const{
 if(!name||std::strcmp(name,"NativeReloadSkills")){error="Unowned character-menu reload callback";return false;}
 // Copy the owning lease before any conversion callback may synchronously
 // reenter or replace its surrounding facade. Every borrow lasts this call.
 const auto retained=graph_.owner;
 std::int32_t index=0;
 if(call.arguments.size()==1){
  double number;
  const auto& argument=call.arguments.front();
  if(argument.kind==2)number=argument.number;
  else if(!call.number||!call.number(argument,number,error)){
   if(error.empty())error="Actual AS numeric conversion required for reload";
   return false;
  }
  if(!graph_.player_index||!graph_.player_index(number,index,error)){
   if(error.empty())error="Source player-index conversion required for reload";
   return false;
  }
 }
 std::uintptr_t character=0;
 if(!graph_.player||!graph_.player(index,false,character,error)){
  if(error.empty())error="Actual NativeGetPlayerChar required for reload";
  return false;
 }
 if(!character)return true;
 MenuReloadResult16V1 result;
 const auto status=dh2_character_menu_reload_v1(&result,character,&graph_.reload);
 if(status!=1){
  error=status==-1?"Malformed retained Character reload graph":
   "Required Character reload service failed at phase "+std::to_string(result.phase);
  return false;
 }
 return true;
}
}
