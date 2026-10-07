#include "loot_temporary_inventory_v8.hpp"
namespace dh2::data {
bool LootTemporaryInventoryV8::transfer_all_source_v10(bool force,bool convert,void* context,
 bool(*add)(void*,std::unique_ptr<ItemInstanceV1>&,bool,bool,std::int32_t&,std::string&),
 bool(*after)(void*,std::int32_t,std::string&),bool(*gold)(void*,std::int32_t,std::string&),std::string& e){
 e.clear();if(running_||transferred_slot_||destructive_failed_v2_){e="Unsupported whole loot transfer reentry/failed prefix";return false;}
 running_=true;struct Guard{bool& running;~Guard(){running=false;}}guard{running_};
 for(auto& slot:items_){
  if(!slot||!slot->item){destructive_failed_v2_=true;e="Required actual source TransferInventoryTo slot/item";return false;}
  auto* original=slot->item.get();const auto id=original->id;
  if(!add){destructive_failed_v2_=true;e="Required same destination AddItemInstance";return false;}
  transferred_slot_=slot.get();transferred_item_=original;
  std::int32_t index{};
  if(!add(context,slot->item,force,convert,index,e)){destructive_failed_v2_=true;return false;}
  if(slot->item){destructive_failed_v2_=true;e="Source destination AddItemInstance did not consume transferred ownership";return false;}
  if(potion_==original)potion_=nullptr;
  if(!after||!after(context,id,e)){destructive_failed_v2_=true;if(e.empty())e="Required source TransferInventoryTo quest notification receiver";return false;}
  transferred_slot_=nullptr;transferred_item_=nullptr;slot.reset();
 }
 items_.clear();
 if(!gold||!gold(context,0,e)){destructive_failed_v2_=true;if(e.empty())e="Required destination AddGold(0) source transfer tail";return false;}
 // Source AddGold(-0) to this constructor NULL-character/gold0 owner leaves
 // gold0, with no Character/trophy/notification receiver to deliver.
 return true;
}
}
