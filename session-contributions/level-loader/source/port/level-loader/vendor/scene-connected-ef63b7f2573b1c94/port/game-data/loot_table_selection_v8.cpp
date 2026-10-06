#include "loot_table_selection_v8.hpp"
#include <algorithm>
namespace dh2::data {
bool LootTableSelectionV8::quantity(std::int32_t id,std::int32_t& out,std::string& e){
 if(!powers_||id<0||std::size_t(id)>=powers_.quantities().size()){e="Required original NumProbArray index assertion";return false;}
 const auto& list=powers_.quantities()[std::size_t(id)];if(dh2_loot_quantity_v7(&out,&random_,list.data(),std::uint32_t(list.size()),0)){e="Required original NumProbArray weighted/assertion continuation";return false;}return true;
}
bool LootTableSelectionV8::rolls(const std::vector<const LootEntry32V2*>& list,std::int32_t count,bool vector_overload,std::vector<const LootEntry32V2*>& out,std::string& e){
 for(std::int32_t n=0;n<count;++n){std::uint32_t index{};if(!entries_.weighted_index(list,index,vector_overload,e)||!entries_.trace(vector_overload?0x403eb4:0x403fd0,e))return false;if(index>=list.size()){e="Required actual nonempty random loot entry";return false;}out.push_back(list[index]);}
 for(auto* entry:list){bool passed{};if(!entries_.percent_roll(*entry,passed,e))return false;if(passed)out.push_back(entry);}return true;
}
bool LootTableSelectionV8::table(std::int32_t id,std::vector<const LootEntry32V2*>& out,std::vector<const LootEntry32V2*>* collector,std::string& e){
 if(!tables_||id<0||std::size_t(id)>=tables_.loots().size()){e="Required original LootTable index assertion";return false;}
 if(std::find(recursion_.begin(),recursion_.end(),id)!=recursion_.end()){e="Actual loot cache contains unsupported recursive cycle";return false;}
 recursion_.push_back(id);struct Pop{std::vector<std::int32_t>& v;~Pop(){v.pop_back();}}pop{recursion_};
 if(!entries_.trace(0x403a04,e)||!entries_.trace(0x403a34,e)||!entries_.trace(0x403a78,e))return false;
 const auto& row=tables_.loots()[std::size_t(id)];for(const auto& fixed:row.fixed_entries){if(!entries_.trace(0x403b04,e))return false;out.push_back(&fixed);}
 std::vector<const LootEntry32V2*> own;for(const auto& entry:row.random_entries)own.push_back(&entry);
 if(own.empty()){if(!entries_.trace(0x403d28,e))return false;}
 else if(collector)collector->insert(collector->end(),own.begin(),own.end());
 else {std::int32_t count{};if(!quantity(row.num_random_item_probs,count,e)||!entries_.trace(0x403f40,e)||!rolls(own,count,false,out,e))return false;}
 std::vector<const LootEntry32V2*> nested;if(!entries_.trace(0x403c08,e))return false;
 for(auto child:row.sub_loots)if(!table(child,out,collector?collector:&nested,e))return false;
 if(!collector&&!nested.empty()){std::int32_t count{};if(!quantity(row.num_random_item_probs,count,e))return false;
  for(auto caller:{0x403d9cu,0x403dccu,0x403dfcu,0x403e28u})if(!entries_.trace(caller,e))return false;
  if(!rolls(nested,count,true,out,e))return false;
 }return true;
}
bool LootTableSelectionV8::select(std::int32_t id,std::vector<const LootEntry32V2*>& out,std::string& e){e.clear();if(running_){e="Unsupported destructive loot selection reentry";return false;}running_=true;struct Reset{bool& v;~Reset(){v=false;}}reset{running_};return table(id,out,nullptr,e);}
}
