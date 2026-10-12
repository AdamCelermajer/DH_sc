#include "loot_entry_selection_v8.hpp"
#include <cstring>
namespace dh2::data {
namespace {std::int32_t word(std::uint32_t v){std::int32_t r;std::memcpy(&r,&v,4);return r;}}
bool LootEntrySelectionV8::request(LootEntryOperationV8 op,std::uint32_t caller,const char* key,std::int32_t& out,std::string& e){
 if(!services_.invoke){e="Required original loot Debug/PlayerManager continuation";return false;}
 if(!services_.invoke(services_.context,{op,caller,key},out,e)){if(e.empty())e="Required source loot entry service "+std::to_string(std::uint32_t(op));return false;}return true;
}
bool LootEntrySelectionV8::debug(std::uint32_t caller,const char* key,std::int32_t& out,std::string& e){std::int32_t ignored{};return request(LootEntryOperationV8::debug_load,caller,nullptr,ignored,e)&&request(LootEntryOperationV8::debug_query,caller,key,out,e);}
bool LootEntrySelectionV8::trace(std::uint32_t caller,std::string& e){std::int32_t ignored{};return debug(caller,"isTracingItemInventory_Loot",ignored,e);}
bool LootEntrySelectionV8::uses_percent(const LootEntry32V2& entry,bool& out,std::string& e){std::int32_t infinite{};if(!debug(0x4027a4,"InfiniteLootDrops",infinite,e))return false;out=infinite!=0||std::uint32_t(entry.words[4])<=100u;return true;}
bool LootEntrySelectionV8::effective_weight(const LootEntry32V2& entry,std::int32_t& out,std::string& e){bool percent{};if(!uses_percent(entry,percent,e))return false;if(percent){out=0;return true;}
 // Original captures all four entry weights before its three player queries.
 const auto warrior=std::uint32_t(entry.words[7]),base=std::uint32_t(entry.words[5]),mage=std::uint32_t(entry.words[2]),rogue=std::uint32_t(entry.words[6]);
 std::int32_t m{},r{},w{};if(!request(LootEntryOperationV8::mage_count,0x402898,nullptr,m,e)||!request(LootEntryOperationV8::rogue_count,0x4028a8,nullptr,r,e)||!request(LootEntryOperationV8::warrior_count,0x4028b0,nullptr,w,e))return false;
 out=word(base+mage*std::uint32_t(m)+rogue*std::uint32_t(r)+warrior*std::uint32_t(w));return true;
}
bool LootEntrySelectionV8::percent_roll(const LootEntry32V2& entry,bool& out,std::string& e){std::int32_t infinite{};if(!debug(0x402bdc,"InfiniteLootDrops",infinite,e))return false;if(infinite){out=true;return true;}bool percent{};if(!uses_percent(entry,percent,e))return false;if(!percent){out=false;return true;}
 std::int32_t draw{};if(dh2_loot_v2_random(&random_,100,&draw)){e="Required source percentage RNG continuation";return false;}
 if(draw>entry.words[4]){out=false;return true;}std::int32_t tracing{};if(!debug(0x402c94,"isTracingItemPctRoll",tracing,e))return false;out=true;return true;
}
bool LootEntrySelectionV8::weighted_index(const std::vector<const LootEntry32V2*>& entries,std::uint32_t& out,bool vector_overload,std::string& e){
 std::uint32_t total=0;for(auto* entry:entries){if(!entry){e="Malformed actual source loot entry pointer";return false;}bool percent{};if(!uses_percent(*entry,percent,e))return false;if(!percent){std::int32_t weight{};if(!effective_weight(*entry,weight,e))return false;total+=std::uint32_t(weight);}}
 if(!total){out=0;return true;}std::int32_t selected{};if(dh2_loot_v2_random(&random_,word(total),&selected)){e="Required original loot weighted RNG/assertion";return false;}auto remaining=std::uint32_t(selected);
 for(std::uint32_t i=0;i<entries.size();++i){bool percent{};if(!uses_percent(*entries[i],percent,e))return false;if(!percent){std::int32_t weight{};if(!effective_weight(*entries[i],weight,e))return false;if(std::uint32_t(weight)>remaining){out=i;return true;}remaining-=std::uint32_t(weight);}}
 // Source assertion mode determines whether this fallback returns or faults.
 // Its actual Application diagnostic receiver is required when reached.
 std::int32_t ignored{};if(!request(LootEntryOperationV8::assertion,vector_overload?0x4029ec:0x402b68,"weighted loot index exhausted",ignored,e))return false;out=0;return true;
}
}
