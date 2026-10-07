#include "world_item_pickup_inventory_v10.hpp"
namespace dh2::character {
bool WorldItemPickupInventoryV10::add(void* raw,std::unique_ptr<data::ItemInstanceV1>& item,bool force,bool convert,std::int32_t& index,std::string& e){return static_cast<Transfer*>(raw)->gear->loot_add_item_v8(item,force,convert,index,e);}
bool WorldItemPickupInventoryV10::after(void* raw,std::int32_t id,std::string& e){auto& t=*static_cast<Transfer*>(raw);auto& s=t.self->services_;if(!s.after_add_all){e="Required actual same player quest transfer tail";return false;}return s.after_add_all(s.context,t.character,id,e);}
bool WorldItemPickupInventoryV10::gold(void* raw,std::int32_t amount,std::string& e){return static_cast<Transfer*>(raw)->gear->loot_add_gold_v8(amount,e);}
bool WorldItemPickupInventoryV10::route(const LootInteractRequestV8& q,LootInteractResponseV8& out,bool& handled,std::string& e){
 using O=LootInteractOperationV8;handled=false;
 if(q.operation!=O::transfer_all&&q.operation!=O::transfer_one&&q.operation!=O::inventory_full&&q.operation!=O::num_potions&&q.operation!=O::potion_capacity)return true;
 handled=true;player::PlayerEquipmentRenderOwnerV1* gear{};
 if(!services_.player||!services_.player(services_.context,q.character,gear,e)||!gear||!gear->ready()||!gear->inventory()||gear->inventory()->character()!=q.character){if(e.empty())e="Required SAME live Character Gear inventory for pickup";return false;}
 out={};const auto* inventory=gear->inventory();
 if(q.operation==O::inventory_full){bool full{};if(!gear->loot_inventory_full_v10(full,e))return false;out.value=full;return true;}
 if(q.operation==O::num_potions){out.value=inventory->num_potions();return true;}
 if(q.operation==O::potion_capacity){out.value=inventory->potion_capacity_v4();return true;}
 if(!q.inventory){e="Required SAME world Item embedded inventory";return false;}
 Transfer transfer{this,gear,q.character};
 // Whole original Interact branches3ed480/3ed834: force0/convertGold1.
 if(q.operation==O::transfer_all)return q.inventory->transfer_all_source_v10(false,true,&transfer,add,after,gold,e);
 // Source TransferItemTo(0,destination,false,true); later transmuteIndex is
 // delivered separately by the existing whole CharacterLootInteract owner.
 return q.inventory->transfer(0,false,true,&transfer,add,out.value,e);
}
}
