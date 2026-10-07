#include "character_npc_animation_set_v6.hpp"
#include <algorithm>
namespace dh2::character {
bool character_npc_register_animation_set_v6(const data::AnimationTables& tables,std::int32_t index,std::int32_t set,const std::vector<std::int32_t>& skills,NpcAnimationRegistrationServicesV6 services,std::string& error){
 error.clear();if(index<0||std::size_t(index)>=tables.characters.size()||!services.invoke){error="Required actual non-player CharAnim/registration services";return false;}
 auto call=[&](NpcAnimationRegistrationOperationV6 op,std::int32_t value){if(services.invoke({op,set,value},error))return true;if(error.empty())error="Required source NPC animation registration operation "+std::to_string(unsigned(op));return false;};
 const auto& character=tables.characters[index];std::vector<int> active;unsigned budget=0;
 std::function<bool(int)> add=[&](int sequence){
  if(sequence<0||std::size_t(sequence)>=tables.sequences.size())return true;
  if(++budget>10000||std::find(active.begin(),active.end(),sequence)!=active.end()){error="Cyclic/bounded source animation registration";return false;}
  active.push_back(sequence);
  if(!call(NpcAnimationRegistrationOperationV6::debug_load,sequence)||!call(NpcAnimationRegistrationOperationV6::debug_trace,sequence))return false;
  for(const auto& step:tables.sequences[sequence].steps){
   if(step.redir){if(!add(step.anim))return false;}
   else {
    if(!call(NpcAnimationRegistrationOperationV6::add_animation,step.anim))return false;
    if(services.sound_manager_present&&!call(NpcAnimationRegistrationOperationV6::preload_sound,step.sound))return false;
    if(step.fx>=0&&!call(NpcAnimationRegistrationOperationV6::preload_fx,step.fx))return false;
   }
  }
  active.pop_back();return true;
 };
 const auto scalar=[&](unsigned field){const auto& values=character.fields[field];return values.empty()?-1:values[0];};
 const int templ=scalar(33);
 if(templ>=0&&std::size_t(templ)<tables.sequences.size()){
  const auto& sequence=tables.sequences[templ];
  // Original asserted Type1 but continued when assertions were not fatal.
  // Unsupported/empty template cannot become an invented default library.
  if(sequence.steps.empty()){error="Required source template first step";return false;}
  if(!sequence.steps[0].redir&&!call(NpcAnimationRegistrationOperationV6::add_template,sequence.steps[0].anim))return false;
 }
 for(unsigned field:{20u,23u,30u,21u})if(!add(scalar(field)))return false;
 // Exact one-stance source call order, including array Interact/Spells.
 for(unsigned field:{9u,34u,26u,0u,1u,29u,32u,16u,8u,14u,2u,7u,6u,4u,3u,5u,24u,25u,35u,27u,11u,13u,10u,12u,36u,28u,18u,19u,15u,31u}){
  if(field==15||field==31){for(auto id:character.fields[field])if(!add(id))return false;}
  else if(!add(scalar(field)))return false;
 }
 // Shipping 3ca484 loads r1=0 on each iteration: preserve that actual lookup,
 // rather than silently correcting it to the loop index.
 for(std::size_t i=0;i<skills.size();++i)if(!add(skills.front()))return false;
 return true;
}
}
