#include "spawn_group_manager_v108.hpp"
#include <algorithm>
#include <cstdio>
#include <cstring>
namespace dh2::world {namespace {
bool missing(std::string& e,const char* leaf){if(e.empty())e=std::string("Required original SpawnGroupManager ")+leaf;return false;}
std::int32_t signed_word(std::uint32_t bits){std::int32_t value;std::memcpy(&value,&bits,4);return value;}
}
std::uint32_t SpawnGroupManagerV108::next_spawn_name_v108(){
 //Source function-static BSS9a3050 and guarded empty initializer; wraps32.
 static std::uint32_t sequence{};return ++sequence;
}
std::shared_ptr<SpawnGroupManagerV108> process_spawn_group_manager_v108(){
 static auto manager=std::make_shared<SpawnGroupManagerV108>();return manager;
}
bool SpawnGroupManagerV108::insert(const SpawnSpotBorrowV108& spot,const SpawnGroupServicesV108& s,std::string& e){
 if(!spot.owner||!spot.identity||!spot.group374||!spot.position160||!s.owner||!s.group_id)return missing(e,"SAME InsSpawn receiver/Arrays");
 std::int32_t id;if(!s.group_id(spot.group374->c_str(),id,e))return false;
 if(id==-1){e.clear();return true;} //actual unknown name return, no tree insert
 auto [i,created]=groups_.try_emplace(id);if(created)i->second.timer8=0;
 //Source list allows duplicate pointer registrations; DelSpawn removes all.
 i->second.spots.push_back({spot.owner,spot.identity,spot.group374,spot.position160});e.clear();return true;
}
bool SpawnGroupManagerV108::erase(const SpawnSpotBorrowV108& spot,const SpawnGroupServicesV108& s,std::string& e){
 if(!spot.owner||!spot.identity||!spot.group374||!s.owner||!s.group_id)return missing(e,"SAME DelSpawn receiver/Arrays");
 std::int32_t id;if(!s.group_id(spot.group374->c_str(),id,e))return false;
 if(id==-1){e.clear();return true;}auto found=groups_.find(id);if(found==groups_.end()){e.clear();return true;}
 found->second.spots.remove_if([&](const Member& m){return m.identity==spot.identity;});
 if(found->second.spots.empty())groups_.erase(found);e.clear();return true;
}
bool SpawnGroupManagerV108::update(double,const SpawnGroupServicesV108& s,std::string& e){
 if(busy_||!s.owner||!s.application_dt)return missing(e,"actual nonreentrant Update/GetDt");
 busy_=true;struct Guard{bool& value;~Guard(){value=false;}} guard{busy_};
 std::uint32_t dt;if(!s.application_dt(dt,e))return false;
 std::vector<SpawnSpotBorrowV108> eligible;
 for(auto i=groups_.begin();i!=groups_.end();++i){
  const auto key=i->first;auto& group=i->second;
  const auto* captured=&group;
  auto still_present=[&](){auto current=groups_.find(key);if(current==groups_.end()||&current->second!=captured)return missing(e,"current native spawn tree node erased during source callback");return true;};
  group.timer8=signed_word(std::uint32_t(group.timer8)-dt);
  if(group.timer8>0)continue;
  if(!s.log||!s.log("spawn group update",e))return missing(e,"actual source log");
  SpawnGroupDefinitionV108 definition;
  if(!s.definition||!s.definition(i->first,definition,e)||!definition.owner||!definition.choice)return missing(e,"actual Arrays.SpawnGroups row");
  group.timer8=definition.delay8; //BEFORE sum/random/eligibility as source
  std::uint32_t total_bits{};
  for(std::uint32_t k=0;k<definition.count_c;++k){SpawnGroupChoiceV108 row;if(!definition.choice(k,row,e))return false;total_bits+=std::uint32_t(row.weight8);}
  const auto total=signed_word(total_bits);if(total<=0)continue;
  std::int32_t roll;if(!s.random||!s.random(total,roll,e))return missing(e,"SAME process Random.GetRandom");
  SpawnGroupChoiceV108 selected;bool selected_row{};
  for(std::uint32_t k=0;k<definition.count_c;++k){SpawnGroupChoiceV108 row;if(!definition.choice(k,row,e))return false;
   roll=signed_word(std::uint32_t(roll)-std::uint32_t(row.weight8));if(roll<0){selected=row;selected_row=true;break;}}
  if(!selected_row)continue;
  if(!selected.quantity_c){if(!s.log("spawn group zero quantity",e))return false;continue;}
  eligible.clear();
  for(auto m=group.spots.begin();m!=group.spots.end();++m){auto pin=m->owner.lock();if(!pin)return missing(e,"live registered SpawnSpot lifetime");
   if(definition.local_only4){bool zonable;if(!s.is_zonable_c4||!s.is_zonable_c4(m->identity,zonable,e))return missing(e,"selected SpawnSpot virtualc4");
    if(zonable){std::uint8_t in_zone,enabled;if(!s.zone_flags||!s.zone_flags(m->identity,in_zone,enabled,e))return missing(e,"SAME source2ee/2f0");
     if(in_zone&&!enabled)continue;}}
   eligible.push_back({std::move(pin),m->identity,m->group,m->position});
  }
  if(eligible.empty()){if(!s.log("spawn group no eligible spots",e))return false;group.timer8=1000;continue;}
  if(!s.log("spawn group eligible spots",e))return false;
  for(std::int32_t n=0;n<selected.quantity_c&&!eligible.empty();++n){
   std::int32_t index;if(!s.random(static_cast<std::int32_t>(eligible.size()),index,e))return false;
   if(index<0||std::size_t(index)>=eligible.size())return missing(e,"original bounded random spot index");
   auto chosen=eligible[std::size_t(index)];eligible.erase(std::remove_if(eligible.begin(),eligible.end(),[&](const auto& v){return v.identity==chosen.identity;}),eligible.end());
   char name[64];std::snprintf(name,sizeof(name),"Spawn_%05u",next_spawn_name_v108());
   std::uintptr_t character;if(!s.spawn||!s.spawn("Character",name,true,true,character,e))return missing(e,"source Spawn34b724");
   if(character){if(!s.init_spawned||!s.init_spawned(character,selected.character4,chosen.position160,e)||!still_present())return missing(e,"SAME Character.InitSpawned3b379c");
    if(!s.place||!s.place(chosen.identity,character,e)||!still_present())return missing(e,"SAME SpawnSpot.PlaceObject3ea704");
    if(!s.log("spawn group spawned Character",e))return false;}
  }
  if(!s.log("spawn group complete",e)||!still_present())return false;
 }
 e.clear();return true;
}
}
