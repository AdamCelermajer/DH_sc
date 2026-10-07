#include "character_generic_animation_registration_v62.hpp"
#include <algorithm>
namespace dh2::character {
bool character_generic_register_animation_set_v62(const data::AnimationTables& tables,
 std::int32_t table,std::int32_t set,std::int32_t count,std::uint32_t mask,
 const std::vector<std::int32_t>& skills,NpcAnimationRegistrationServicesV6 services,std::string& e){
 if(table<0||std::size_t(table)>=tables.characters.size()||count<0||count>256||!services.invoke){e="Required original generic animation registration inputs";return false;}
 auto call=[&](auto operation,int resource){if(services.invoke({operation,set,resource},e))return true;if(e.empty())e="Required actual animation registration service";return false;};
 using O=NpcAnimationRegistrationOperationV6;
 std::vector<int> active;unsigned budget{};
 std::function<bool(int,int,std::uint32_t,std::uint32_t)> add;
 add=[&](int base,int stance,std::uint32_t flags,std::uint32_t stanced){
  const auto index=static_cast<std::int64_t>(base)+stance;
  if(base<0||index<0||index>=static_cast<std::int64_t>(tables.sequences.size())||((stanced&flags)!=flags&&stance!=0))return true;
  if(++budget>10000||std::find(active.begin(),active.end(),int(index))!=active.end()){e="Cyclic/bounded original animation registration";return false;}
  active.push_back(int(index));
  if(!call(O::debug_load,int(index))||!call(O::debug_trace,int(index)))return false;
  for(const auto& step:tables.sequences[std::size_t(index)].steps){
   if(step.redir){if(!add(step.anim,0,0,0))return false;}
   else{
    if(!call(O::add_animation,step.anim))return false;
    bool sound_present=services.sound_manager_present;
    if(services.current_sound_manager&&!services.current_sound_manager(sound_present,e))return false;
    if((sound_present&&!call(O::preload_sound,step.sound))||
       (step.fx>=0&&!call(O::preload_fx,step.fx)))return false;
   }
  }active.pop_back();return true;
 };
 const auto& row=tables.characters[table];
 auto scalar=[&](unsigned field){return row.fields[field].empty()?-1:row.fields[field][0];};
 const auto templ=scalar(33);
 if(templ>=0&&std::size_t(templ)<tables.sequences.size()){
  const auto& sequence=tables.sequences[templ];if(sequence.steps.empty()){e="Required source animation template step";return false;}
  if(!sequence.steps[0].redir&&!call(O::add_template,sequence.steps[0].anim))return false;
 }
 for(unsigned field:{20u,23u,30u,21u})if(!add(scalar(field),0,0,0))return false;
 // Exact3ca08c..3ca3f0 order/flags; scalar sequence variants are consecutive
 // table rows. Zero flags deliberately register adjacent variants too.
 struct Entry{unsigned field;std::uint32_t flags;};
 static constexpr Entry fields[]={{9,2},{34,16},{26,32},{0,64},{1,128},{29,256},{32,512},{16,1024},{8,2048},{14,4096},{2,8192},{7,16384},{6,32768},{4,65536},{3,131072},{5,262144},{24,1048576},{25,524288},{35,0},{27,0},{11,4},{13,8},{10,8},{12,0},{36,0},{28,0},{18,16777216},{19,16777216}};
 for(int stance=0;stance<count;++stance){
  for(auto entry:fields)if(!add(scalar(entry.field),stance,entry.flags,mask))return false;
  for(auto id:row.fields[15])if(!add(id,stance,0x800000,mask))return false;
  for(auto id:row.fields[31])if(!add(id,stance,0x400000,mask))return false;
  // Original3ca484 moves the incremented r8 into r1, not literal zero.
  for(auto id:skills)if(!add(id,stance,0x200000,mask))return false;
 }e.clear();return true;
}
}
