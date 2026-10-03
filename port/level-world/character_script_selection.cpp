#include "character_script_selection.hpp"
#include <cstddef>
namespace {
bool valid_name(const char* name){
 if(!name)return false;
 for(std::size_t i=0;i<=4096;++i)if(!name[i])return true;
 return false;
}
bool equal(const char* a,const char* b){
 for(std::size_t i=0;;++i){if(a[i]!=b[i])return false;if(!a[i])return true;}
}
bool valid(const dh2::character::ScriptSelectionState16* state,
           const dh2::character::ScriptSelectionServices16* services){
 return state&&!state->reserved&&state->scripted<=1&&services&&services->construct;
}
void select(dh2::character::ScriptSelectionState16* state,const char* name,
            const dh2::character::ScriptSelectionServices16* services){
 using namespace dh2::character;
 if(name[0]=='_'&&name[1]=='_'){
  const auto* suffix=name+2;
  std::uint32_t kind;
  bool clear=false;
  if(equal(suffix,"monster__"))kind=script_monster;
  else if(equal(suffix,"player__"))kind=script_player_iphone;
  else if(equal(suffix,"faery__"))kind=script_faery;
  else {kind=script_default;clear=!equal(suffix,"npc__");}
  services->construct(services->context,state,kind);
  // The recognized builtin branches do not write the old filename field.
  if(clear)state->external_name=0;
 }else{
  services->construct(services->context,state,script_external);
  state->external_name=reinterpret_cast<std::uintptr_t>(name);
 }
}
}
extern "C" int dh2_character_script_select(
 dh2::character::ScriptSelectionState16* state,const char* name,
 const dh2::character::ScriptSelectionServices16* services){
 if(!valid(state,services)||!valid_name(name))return -1;
 select(state,name,services);return 1;
}
extern "C" int dh2_character_script_create_step(
 dh2::character::ScriptSelectionState16* state,const dh2::character::ScriptCreationFacts24* facts,
 const dh2::character::ScriptSelectionServices16* services){
 using namespace dh2::character;
 if(!valid(state,services)||!facts||facts->reserved||
    !valid_name(facts->script_length?facts->script_name:facts->owner_name))return -1;
 if(facts->script_length){
  select(state,facts->script_name,services);
  state->scripted=1;
 }else{
  services->construct(services->context,state,equal(facts->owner_name,"Player")?script_player:script_default);
  state->external_name=0;state->scripted=0;
 }
 return 1;
}
