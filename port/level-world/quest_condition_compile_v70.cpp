#include "quest_condition_compile_v70.hpp"
namespace dh2::world {
bool quest_condition_compile_v70(const QuestConditionCompileBorrowV70& fields,bool force,
 const QuestConditionCompileServicesV70& services,std::string& e){
 if(!fields.receiver||!fields.collection||!fields.character5c||!services.transport||!services.character_difficulty3bb8e4){e="Required SAME native QuestSavegame5c/cache28 and Character difficulty transport";return false;}
 const auto difficulty=[&](std::int32_t& out){
  if(!*fields.character5c){e="Original CompileQuests dereferences nullable Character5c";return false;}
  if(!services.character_difficulty3bb8e4(*fields.character5c,out,e))return false;
  if(out<0||out>=3||!fields.compiled28[static_cast<std::size_t>(out)]){e="Original Quest compile difficulty/cache indexes outside actual source cells";return false;}
  return true;
 };
 std::int32_t selected{};
 if(!force){if(!difficulty(selected))return false;if(*fields.compiled28[static_cast<std::size_t>(selected)]){e.clear();return true;}}
 if(!difficulty(selected))return false;
 *fields.compiled28[static_cast<std::size_t>(selected)]=1; //46b844 before count/walk
 if(!difficulty(selected))return false;
 const auto count=fields.collection->source_quests_v45()[static_cast<std::size_t>(selected)].size();
 for(std::size_t i=0;i<count;++i){
  if(!difficulty(selected))return false;
  const auto& current=fields.collection->source_quests_v45()[static_cast<std::size_t>(selected)];
  if(i>=current.size()||!current[i]){e="Original Quest.Compile reached a NULL/unavailable actual quest slot";return false;}
  if(!services.quest_compile480178||!services.quest_compile480178(current[i],e)){if(e.empty())e="Required whole actual Quest.Compile480178 Objective/Reward/marker providers";return false;}
 }
 e.clear();return true;
}
}
