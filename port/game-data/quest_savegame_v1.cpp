#include "quest_savegame_v1.hpp"
#include <cstring>
#include <limits>
namespace dh2::data {
namespace {
std::int32_t signed_word(std::uint32_t value){std::int32_t result;std::memcpy(&result,&value,4);return result;}
bool read_word(Bytes bytes,std::size_t& used,std::uint32_t& value,std::string& error){
 if(used>bytes.size||bytes.size-used<4){error="Truncated source quest section at byte "+std::to_string(used);return false;}
 const auto* p=bytes.data+used;value=std::uint32_t(p[0])|std::uint32_t(p[1])<<8|std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24;used+=4;return true;
}
}
bool QuestSavegameV1::attach_initialized_quests(const std::array<std::vector<std::uintptr_t>,3>& quests,std::string& error){
 for(const auto& list:quests)if(list.size()>std::numeric_limits<std::uint32_t>::max()){error="Quest owner exceeds source count";return false;}
 try{auto copy=quests;quests_.swap(copy);initialized_=true;error.clear();return true;}
 catch(...){error="Quest identity storage allocation failed";return false;}
}
bool QuestSavegameV1::unpack(std::uint32_t difficulty,Bytes bytes,bool volatile_state,
 const QuestLoadServicesV1& services,std::size_t& used,bool& mismatch,std::string& error){
 used=0;mismatch=false;
 if(!initialized_||difficulty>=3||(!bytes.data&&bytes.size)||bytes.size>UINT32_MAX){error="Required initialized quest owner/span unavailable";return false;}
 std::uint32_t count;if(!read_word(bytes,used,count,error))return false;
 const auto& quests=quests_[difficulty];
 if(count!=quests.size()){mismatch=true;error.clear();return true;}
 for(std::uint32_t i=0;i<count;++i){
  std::uint32_t id;if(!read_word(bytes,used,id,error))return false;
  if(id>=quests.size()){error="Unsafe source saved quest index";return false;}
  const auto quest=quests[id];
  // Source null entry returns without consuming Quest::_loadQuestData.
  // Debug's bounds-unsafe continuation is not accepted as a native memory read.
  if(!quest)continue;
  if(!services.load_quest){error="Required Quest::_loadQuestData owner unavailable";return false;}
  std::size_t delivered=0;const Bytes remaining{bytes.data+used,bytes.size-used};
  const bool success=services.load_quest(services.context,quest,remaining,volatile_state,delivered,error);
  if(delivered>remaining.size){error="Quest data owner returned unsafe consumption";return false;}
  used+=delivered;if(!success)return false;
 }
 std::uint32_t value;
 if(!read_word(bytes,used,value,error))return false;progress_.current_quest[difficulty]=signed_word(value);
 if(!read_word(bytes,used,value,error))return false;progress_.primary_quest[difficulty]=signed_word(value);
 if(!read_word(bytes,used,value,error))return false;
 progress_.current_act[difficulty]=progress_.compatible_act[difficulty]=signed_word(value);
 error.clear();return true;
}
bool QuestSavegameV1::load(Bytes bytes,const QuestLoadServicesV1& services,std::size_t& used,
 std::array<bool,3>& mismatch,std::string& error){
 used=0;mismatch={};
 if((!bytes.data&&bytes.size)||bytes.size>UINT32_MAX){error="Invalid source quest payload";return false;}
 for(std::uint32_t difficulty=0;difficulty<3;++difficulty){
  std::size_t local=0;const Bytes remaining{bytes.data?bytes.data+used:nullptr,bytes.size-used};
  const bool ok=unpack(difficulty,remaining,false,services,local,mismatch[difficulty],error);
  used+=local;if(!ok)return false;
 }
 error.clear();return true;
}
bool load_player_quests_v1(Bytes bytes,QuestSavegameV1& regular,QuestSavegameV1& volatile_quests,
 const QuestLoadServicesV1& services,std::size_t& used,std::string& error){
 std::array<bool,3> mismatch{};
 if(!regular.load(bytes,services,used,mismatch,error))return false;
 return volatile_quests.load(bytes,services,used,mismatch,error);
}
}
