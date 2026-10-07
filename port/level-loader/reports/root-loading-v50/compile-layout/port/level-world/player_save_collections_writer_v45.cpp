#include "player_save_collections_writer_v45.hpp"
namespace dh2::level {
bool player_save_level_states_v45(const PlayerSaveLevelStatesBorrowV45& b,SavegameStreamV2& out,std::string& e){
 if(!b.receiver||!b.level_names||!b.map_names||!b.levels||!b.maps||b.level_names->size()>INT32_MAX||b.map_names->size()>INT32_MAX){e="Required SAME source Level/Map state arrays and immutable names";return false;}
 auto write=[&](const auto& names,const auto& tiers){for(unsigned tier=0;tier<3;++tier){const auto count=names.size();
  if(tiers[tier].size()<count){e="Required initialized SAME saved state array";return false;}
  if(!out.write_u32(std::uint32_t(count),e))return false;
  for(std::size_t i=0;i<count;++i)if(!out.write_string(names[i],e)||!out.write_u32(std::uint32_t(tiers[tier][i]),e))return false;
 }return true;};
 return write(*b.level_names,*b.levels)&&write(*b.map_names,*b.maps);
}
bool player_save_fast_travel_v45(const std::array<std::uint64_t,3>* bits,SavegameStreamV2& out,std::string& e){
 if(!bits){e="Required actual Save bitsets17c/184/18c";return false;}
 for(unsigned tier=0;tier<3;++tier){std::string text(64,'0');for(unsigned i=0;i<64;++i)if(((*bits)[tier]>>i)&1u)text[63-i]='1';if(!out.write_string(text,e))return false;}return true;
}
bool quest_save_collection_v45(const QuestSaveCollectionBorrowV45& b,SavegameStreamV2& out,std::string& e){
 if(!b.receiver||!b.quests||!b.progress){e="Required SAME initialized regular/volatile QuestSavegame";return false;}
 for(unsigned tier=0;tier<3;++tier){const auto count=(*b.quests)[tier].size();if(count>UINT32_MAX){e="Source saved quest vector count exceeded";return false;}
  if(!out.write_u32(std::uint32_t(count),e))return false;
  for(std::size_t i=0;i<count;++i){if(!out.write_u32(std::uint32_t(i),e))return false;const auto quest=(*b.quests)[tier][i];
   if(!quest||!b.save_quest){e="Required actual Quest::_saveQuestData after source index prefix";return false;}
   if(!b.save_quest(quest,out,e))return false;
  }
  if(!out.write_u32(std::uint32_t(b.progress->current_quest[tier]),e)||
     !out.write_u32(std::uint32_t(b.progress->primary_quest[tier]),e)||
     !out.write_u32(std::uint32_t(b.progress->current_act[tier]),e))return false;
 }return true;
}
bool player_save_quests_v45(std::int32_t mode,const QuestSaveCollectionBorrowV45& regular,
 const QuestSaveCollectionBorrowV45& volatile_quests,SavegameStreamV2& out,std::string& e){return quest_save_collection_v45((std::uint32_t(mode)&1u)?regular:volatile_quests,out,e);}
}
