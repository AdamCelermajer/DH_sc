#include "loot_inventory_source_v9.hpp"
namespace dh2::character {
namespace {
using namespace data;
template<class Tag,typename Tag::type Member>struct Expose {friend typename Tag::type access(Tag){return Member;}};
struct Items {using type=std::vector<std::unique_ptr<OwnedItemSlotV4>> FreshInventoryOwnedV4::*;friend type access(Items);};
struct Potion {using type=ItemInstanceV1* FreshInventoryOwnedV4::*;friend type access(Potion);};
struct Depth {using type=std::uint32_t FreshInventoryOwnedV4::*;friend type access(Depth);};
struct Allowed {using type=bool(FreshInventoryOwnedV4::*)(std::string&)const;friend type access(Allowed);};
template struct Expose<Items,&FreshInventoryOwnedV4::items_>;
template struct Expose<Potion,&FreshInventoryOwnedV4::potion_>;
template struct Expose<Depth,&FreshInventoryOwnedV4::callback_depth_>;
template struct Expose<Allowed,&FreshInventoryOwnedV4::mutation_allowed>;
}
bool fresh_inventory_loot_source_v9(data::FreshInventoryOwnedV4& inventory,
 std::shared_ptr<void> owner,LootInventorySourceV9& out,std::string& e){
 if(!owner||inventory.character()||inventory.potion_capacity_v4()!=-1||inventory.gold()!=0){e="Required original NULL-character world-drop source inventory";return false;}
 for(const auto& set:inventory.equipment())for(auto* slot:set)if(slot){e="Required original empty drop equipment vectors";return false;}
 LootInventorySourceV9 source;source.owner=std::move(owner);
 source.count=[&inventory]{return inventory.items().size();};
 source.item=[&inventory](std::uint32_t i){return i<inventory.items().size()&&inventory.items()[i]?inventory.items()[i]->item.get():nullptr;};
 source.metadata=[&inventory](const auto& item){return data::item(inventory.table(),item.id);};
 source.transfer=[&inventory](std::uint32_t index,bool force,bool convert,void* context,auto add,auto& result,auto& error){
  result=-1;if(!(inventory.*access(Allowed{}))(error))return false;
  auto& items=inventory.*access(Items{});
  if(index>=items.size()||!items[index]||!items[index]->item||!add){error="Required actual drop source item and destination AddItemInstance";return false;}
  auto* item=items[index]->item.get();if(item->signed_quantity()==0){result=0;return true;}
  if(item->signed_quantity()<0){error="Required original negative quantity transfer assertion";return false;}
  auto& potion=inventory.*access(Potion{});if(potion==item)potion=nullptr;
  auto& depth=inventory.*access(Depth{});++depth;struct Guard{std::uint32_t& value;~Guard(){--value;}}guard{depth};
  // Destination consumes the actual unique item. A reached failure retains
  // the actual consumed prefix; no item clone or dangling source alias exists.
  const bool delivered=add(context,items[index]->item,force,convert,result,error);
  if(!items[index]->item)items.erase(items.begin()+index);
  return delivered;
 };
 out=std::move(source);return true;
}
}
