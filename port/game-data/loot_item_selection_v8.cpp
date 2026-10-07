#include "loot_item_selection_v8.hpp"
#include <cstring>
namespace dh2::data {
namespace { std::int32_t signed_bits(std::uint32_t v){std::int32_t r;std::memcpy(&r,&v,4);return r;} }
bool LootItemSelectionV8::weighted_index(const std::vector<LootItemEntryV2>& list,std::uint32_t& out,std::string& e){
 std::uint32_t total{};for(const auto& row:list)total+=std::uint32_t(std::int32_t(row.probability));
 if(list.empty()||!total){e="Required original _GetRandomItem empty/zero-weight assertion continuation";return false;}
 std::int32_t draw{};if(dh2_loot_v2_random(&random_,signed_bits(total),&draw)){e="Required actual shared loot RNG";return false;}
 auto remaining=std::uint32_t(draw);
 for(std::size_t i=0;i<list.size();++i){auto weight=std::uint32_t(std::int32_t(list[i].probability));if(weight>remaining){out=std::uint32_t(i);return true;}remaining-=weight;}
 e="Required original _GetRandomItem exhausted-weight assertion continuation";return false;
}
bool LootItemSelectionV8::append(const LootEntry32V2& entry,const LootItemEntryV2& row,std::uint32_t caller,std::vector<LootItemInfoV8>& out,std::string& e){
 if(row.item<0||std::size_t(row.item)>=tables_.items().rows.size()){e="Required original _AddLootItems ItemTable index assertion";return false;}
 if(!entries_.trace(caller,e))return false;
 std::uint16_t bits=std::uint16_t(row.item);std::int16_t id;std::memcpy(&id,&bits,2);
 out.push_back({id,&entry,row.quantity,&tables_.items().rows[std::size_t(row.item)]});return true;
}
bool LootItemSelectionV8::expand(const std::vector<const LootEntry32V2*>& selected,bool give_all,std::vector<LootItemInfoV8>& out,std::string& e){
 e.clear();if(running_){e="Unsupported destructive loot item expansion reentry";return false;}running_=true;struct Reset{bool& v;~Reset(){v=false;}}reset{running_};
 if(!tables_){e="Required actual immutable loot tables";return false;}
 for(auto* entry:selected){if(!entry){e="Required actual selected loot entry";return false;}
  auto id=entry->words[0];if(id<0||std::size_t(id)>=tables_.item_lists().size()){e="Required original _AddLootItems ItemList index assertion";return false;}
  const auto& list=tables_.item_lists()[std::size_t(id)];
  if(give_all){for(const auto& row:list)if(!append(*entry,row,0x402f9c,out,e))return false;}
  else {std::uint32_t index{};if(!weighted_index(list,index,e)||!append(*entry,list[index],0x403118,out,e))return false;}
 }return true;
}
}
