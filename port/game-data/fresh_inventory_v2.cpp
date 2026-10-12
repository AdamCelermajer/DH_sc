#include "fresh_inventory_v2.hpp"
#include <cstring>
#include <stdexcept>
namespace dh2::data {
FreshInventoryV2::FreshInventoryV2(std::uintptr_t id,LootTablesV2::Borrow b,LootRandom8V2& r,std::int8_t cap):tables_(std::move(b)),random_(&r),character_(id),potion_capacity_(cap){if(!id||!tables_)throw std::invalid_argument("Unbound fresh inventory owner");}
bool FreshInventoryV2::deliver(const FreshInventoryServicesV2& s,FreshInventoryOperationV2 op,std::uint32_t caller,ItemInstanceV1* item,const char* name,std::int32_t arg,std::uint32_t index,FreshInventoryResponseV2& out,std::string& e){out={};if(!s.invoke){e="Required fresh inventory service unavailable";return false;}if(!s.invoke(s.context,*this,{op,caller,item,name,arg,index},out,e)){if(e.empty())e="Required fresh inventory service failed at "+std::to_string(caller);return false;}return true;}
bool FreshInventoryV2::debug(const FreshInventoryServicesV2& s,std::uint32_t call,const char* name,std::int32_t& value,std::string& e){FreshInventoryResponseV2 out;auto load=call-0x1c;if(call==0x40411c||call==0x403d48||call==0x403138)load=call-0x20;else if(call==0x403c2c)load=call-0x24;if(!deliver(s,FreshInventoryOperationV2::debug_load,load,nullptr,nullptr,0,0,out,e)||!deliver(s,FreshInventoryOperationV2::debug_query,call,nullptr,name,0,0,out,e))return false;value=out.value;return true;}
bool FreshInventoryV2::add_fixed_loot(std::int32_t id,const FreshInventoryServicesV2& s,std::string& e){
 if(running_||!s.invoke){e="Malformed fresh loot service or unsupported owner reentry";return false;}if(id<0||std::size_t(id)>=tables_.loots().size()){e.clear();return true;}running_=true;struct Guard{bool& b;~Guard(){b=false;}}guard{running_};std::int32_t ignored=0;auto trace=[&](std::uint32_t at){return debug(s,at,"isTracingItemInventory_Loot",ignored,e);};
 if(!debug(s,0x40411c,"MP_MinimalRandoms",ignored,e))return false;if(ignored){e.clear();return true;}if(!trace(0x404160)||!trace(0x404190))return false;
 const auto& loot=tables_.loots()[id];if(!loot.random_entries.empty()||!loot.sub_loots.empty()){e="Required random/subloot source continuation unavailable";return false;}
 if(!trace(0x403a20)||!trace(0x403a50)||!trace(0x403a94))return false;
 for(const auto& entry:loot.fixed_entries){if(!trace(0x403b20))return false;auto list=entry.words[0];if(list<0||std::size_t(list)>=tables_.item_lists().size()){e="Loot ItemList requires unrecovered Debug continuation";return false;}if(entry.words[1]!=-1){e="Required ItemPowerList producer unavailable";return false;}}
 if(!trace(0x403d48)||!trace(0x403c2c)||!trace(0x404264)||!trace(0x404294))return false;
 struct Prepared {std::int32_t id;std::uint8_t quantity;const Item* row;};std::vector<Prepared> selected;
 for(const auto& entry:loot.fixed_entries){const auto& list=tables_.item_lists()[entry.words[0]];std::uint32_t sum=0;for(const auto& p:list)sum+=std::uint32_t(std::int32_t(p.probability));if(list.empty()||!sum){e="ItemList weighted selection requires source Debug continuation";return false;}
  std::int32_t bound;std::memcpy(&bound,&sum,4);std::int32_t draw;if(dh2_loot_v2_random(random_,bound,&draw)){e="Invalid borrowed source RNG";return false;}std::uint32_t remainder=std::uint32_t(draw);const LootItemEntryV2* chosen=nullptr;for(const auto& p:list){auto weight=std::uint32_t(std::int32_t(p.probability));if(remainder<weight){chosen=&p;break;}remainder-=weight;}if(!chosen){e="Weighted ItemList fell through required Debug continuation";return false;}
  auto* row=item(tables_.items(),chosen->item);if(!row){e="Selected ItemID exceeds genuine ItemTable";return false;}if(!trace(0x403138))return false;selected.push_back({chosen->item,chosen->quantity,row});
 }
 if(selected.empty()){if(!trace(0x404574)||!trace(0x4045a4)||!trace(0x4041f8)||!trace(0x404228))return false;e.clear();return true;}
 if(!trace(0x4042fc)||!trace(0x40432c))return false;FreshInventoryResponseV2 out;
 for(const auto& chosen:selected){auto type=item_type(*chosen.row);if(type==13){e="Required gold-loot valuation unavailable";return false;}std::int32_t repeats=1;auto distribution=chosen.row->record.words[4];if(distribution==2||distribution==3){if(!deliver(s,FreshInventoryOperationV2::player_count,0x4043a8,nullptr,nullptr,0,0,out,e))return false;repeats=out.value;if(repeats<=0)continue;if(repeats>1024){e="Source player count exceeds owned budget";return false;}}
  for(std::int32_t j=0;j<repeats;++j){if(!deliver(s,FreshInventoryOperationV2::current_player,0x4043cc,nullptr,nullptr,0,0,out,e))return false;if(out.identity){if(!deliver(s,FreshInventoryOperationV2::current_player,0x4043dc,nullptr,nullptr,1,0,out,e))return false;if(out.value!=0){e="Required difficulty-name item variant lookup unavailable";return false;}}
   auto owned=std::make_unique<ItemInstanceV1>();owned->id=chosen.id;owned->quantity=1;auto* instance=owned.get();for(auto op:{FreshInventoryOperationV2::update_name,FreshInventoryOperationV2::update_stats,FreshInventoryOperationV2::update_requirements})if(!deliver(s,op,std::uint32_t(op),instance,nullptr,0,0,out,e))return false;
   std::int8_t qty;std::memcpy(&qty,&chosen.quantity,1);if(qty==-2)qty=99;if(qty<0){e="SetQty requires original negative Debug continuation";return false;}instance->quantity=std::uint16_t(qty);
   auto* valued=item(tables_.items(),instance->id);if(!valued||item_type(*valued)==13){e="Item effects changed ID into unsupported valuation continuation";return false;}auto value=std::uint32_t(valued->record.words[27])*std::uint32_t(valued->record.words[28]);for(const auto& power:instance->powers){(void)power;e="Item effects supplied unrecovered powered valuation";return false;}std::memcpy(&instance->value,&value,4);if(!deliver(s,FreshInventoryOperationV2::update_name,0x402128,instance,nullptr,0,0,out,e))return false;
   auto* inserted=item(tables_.items(),instance->id);if(!inserted){e="Item effects changed ID outside genuine ItemTable";return false;}auto inserted_type=item_type(*inserted);
   if(potion_capacity_==0&&inserted_type==14){if(!deliver(s,FreshInventoryOperationV2::destroy_item,0x3ff70c,instance,nullptr,0,0,out,e))return false;continue;}
   if(!potion_&&inserted_type==14)potion_=instance;auto slot=std::make_unique<ItemSlotV1>();slot->item=std::move(owned);items_.push_back(std::move(slot));auto index=std::uint32_t(items_.size()-1);if(!deliver(s,FreshInventoryOperationV2::inventory_full,0x3ff6c4,instance,nullptr,0,index,out,e))return false;if(out.value&&!deliver(s,FreshInventoryOperationV2::full_notifications,0x3ff7a8,instance,nullptr,0,index,out,e))return false;
  }
 }
 e.clear();return true;
}
bool FreshInventoryV2::record_delivered_equipment(std::uint32_t set,std::uint32_t slot,std::uint32_t index,std::string& e){if(set>=2||slot>=9||index>=items_.size()){e="Invalid genuine delivered equipment identity";return false;}auto* ptr=items_[index].get();equipment_[set][slot]=ptr;ptr->equipment_set=std::int8_t(set);ptr->equipment_slot=std::int8_t(slot);e.clear();return true;}
std::int32_t FreshInventoryV2::num_potions()const noexcept{std::int16_t value=0;if(potion_)std::memcpy(&value,&potion_->quantity,2);return value;}
}
