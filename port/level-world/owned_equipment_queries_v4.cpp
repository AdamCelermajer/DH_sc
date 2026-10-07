#include "owned_equipment_queries_v4.hpp"
namespace dh2::player {
bool owned_equipment_queries_v4(EquipmentQueries12V1& out,
 const data::FreshInventoryOwnedV4& inventory,std::uintptr_t character,
 const std::int32_t* resolved,std::string& error){
 if(!character||inventory.character()!=character||!inventory.properties()||
    inventory.properties()->resolved.data()!=resolved){
  error="Required same Character owned equipment/property authority";return false;
 }
 const auto selected=inventory.current_equipment();
 if(selected<0||selected>=2){error="Required actual selected equipment set";return false;}
 const auto& set=inventory.equipment()[selected];
 const data::ItemRecord164* weapons[2]{};
 for(unsigned i=0;i<2;++i){
  const auto* slot=set[i+1];if(!slot)continue;
  if(!slot->item){error="Required actual equipped ItemInstance";return false;}
  const auto* metadata=data::item(inventory.table(),slot->item->id);
  if(!metadata){error="Required equipped item metadata from same table";return false;}
  weapons[i]=&metadata->record;
 }
 EquipmentQueries12V1 result{};
 if(dh2_equipment_queries_v1(&result,weapons[0],weapons[1],resolved[203])){
  error="Malformed source owned equipment query";return false;
 }
 out=result;error.clear();return true;
}
}
