#include "player_save_named_writer_v1.hpp"
#include <cstring>
#include <limits>
#include <stdexcept>
namespace dh2::data {
PlayerSaveNamedWriterV1::PlayerSaveNamedWriterV1(std::shared_ptr<PlayerSaveLoadOwnerV1> authority,SkillTables::Borrow skills):authority_(std::move(authority)),skills_(std::move(skills)){
 if(!authority_)throw std::invalid_argument("Required same named-section Save/profile authority unavailable");
}
bool PlayerSaveNamedWriterV1::write(const char* section,const PlayerSaveByteWriterV1& supplied,std::string& error){
 if(running_){error="Unsupported recursive named Save writer callback";return false;}
 running_=true;struct Guard{bool& value;~Guard(){value=false;}}guard{running_};
 const auto authority=authority_;const auto sink=supplied;
 if(!section||!sink.owner||!sink.write){error="Required genuine named-section stream unavailable";return false;}
 auto integer=[&](std::int32_t value){const auto word=std::uint32_t(value);std::array<std::uint8_t,4> bytes{{std::uint8_t(word),std::uint8_t(word>>8),std::uint8_t(word>>16),std::uint8_t(word>>24)}};return sink.write({bytes.data(),bytes.size()},error);};
 auto& save=authority->save();error.clear();
 if(std::strcmp(section,"SKIL")==0){
  if(!save.skills_initialized()||!skills_){error="Original skill writer requires actual initialized Save skills and immutable skill-name table";return false;}
  const auto count=save.skills().size();
  if(count>std::size_t(std::numeric_limits<std::int32_t>::max())){error="Original saved-skill count assertion domain exceeded";return false;}
  if(!integer(std::int32_t(count)))return false;
  for(std::size_t row=0;row<count;++row){
   if(row>=save.skills().size()){error="Saved skill rows shrank during source stream callback";return false;}
   const auto id=save.skills()[row].id;
   if(id<0||std::size_t(id)>=skills_.skill_names().size()){error="Original saved skill-name lookup assertion domain exceeded";return false;}
   const auto name=skills_.skill_names()[std::size_t(id)];const auto length=name.size()+1;
   if(length>std::size_t(std::numeric_limits<std::int32_t>::max())){error="Original skill-name length assertion domain exceeded";return false;}
   if(!integer(std::int32_t(length))||!sink.write({reinterpret_cast<const std::uint8_t*>(name.c_str()),length},error))return false;
   if(row>=save.skills().size()){error="Saved skill rows shrank before source level write";return false;}
   const auto level=save.skills()[row].level;const std::array<std::uint8_t,2> bytes{{std::uint8_t(level),std::uint8_t(level>>8)}};
   if(!sink.write({bytes.data(),bytes.size()},error))return false;
  }
  for(unsigned set=0;set<2;++set){
   const auto size=save.skill_slots()[set].size();if(size>UINT32_MAX){error="Original skill-slot map count exceeds stream word";return false;}
   if(!integer(std::int32_t(std::uint32_t(size))))return false;
   auto current=save.skill_slots()[set].begin();
   while(current!=save.skill_slots()[set].end()){
    const auto key=current->first;
    if(!integer(key))return false;
    auto reached=save.skill_slots()[set].find(key);
    if(reached==save.skill_slots()[set].end()){error="Source skill-slot iterator erased during stream callback";return false;}
    if(!integer(std::int32_t(reached->second)))return false;
    if(save.skill_slots()[set].find(key)==save.skill_slots()[set].end()){error="Source skill-slot iterator erased during value callback";return false;}
    current=save.skill_slots()[set].upper_bound(key);
   }
  }
  return true;
 }
 if(std::strcmp(section,"PLVL")==0)return integer(save.level());
 if(std::strcmp(section,"PNAM")==0){
  const auto length=save.name().size()+1;
  if(length>std::size_t(std::numeric_limits<std::int32_t>::max())){error="Original Save string length assertion domain exceeded";return false;}
  if(!integer(std::int32_t(length)))return false;
  // Source writeAs(string) stores length before its first stream callback and
  // rereads the current begin pointer for the second delivery.
  const auto& current=save.name();
  if(current.size()+1<length){error="Save name shrank during length callback; original raw read unavailable";return false;}
  std::vector<std::uint8_t> bytes(current.c_str(),current.c_str()+length);
  return sink.write({bytes.data(),bytes.size()},error);
 }
 if(std::strcmp(section,"CFEE")==0){
  for(unsigned tier=0;tier<3;++tier){
   if(!save.faeries_initialized()[tier]){error="Original current-faery writer requires initialized real faeries";return false;}
   if(!integer(save.current_faery(tier)))return false;
  }
  return true;
 }
 if(std::strcmp(section,"FAES")==0){
  for(unsigned tier=0;tier<3;++tier){
   if(!save.faeries_initialized()[tier]){error="Original faery writer requires initialized real faeries";return false;}
   if(!integer(save.current_faery(tier))||!integer(5))return false;
   for(unsigned row=0;row<5;++row){
    const auto level=save.faeries()[tier][row].level;
    const std::array<std::uint8_t,2> level_bytes{{std::uint8_t(level),std::uint8_t(level>>8)}};
    if(!sink.write({level_bytes.data(),level_bytes.size()},error))return false;
    const auto state=save.faeries()[tier][row].state;
    if(!sink.write({&state,1},error))return false;
   }
  }
  return true;
 }
 error="Required original named-section writer not reconstructed: "+std::string(section);return false;
}
}
