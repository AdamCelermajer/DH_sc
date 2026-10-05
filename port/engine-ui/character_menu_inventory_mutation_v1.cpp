#include "character_menu_inventory_mutation_v1.hpp"
#include <algorithm>
namespace dh2::data {
namespace {
// [temp.explicit]: access checking is not performed on names used to specify
// explicit instantiations. Every address and member type is compiler checked.
// This produces real member pointers, never object-layout offsets or aliases.
template<class Tag,typename Tag::type Member> struct Expose {
 friend typename Tag::type access(Tag){return Member;}
};
struct Items {using type=std::vector<std::unique_ptr<OwnedItemSlotV4>> FreshInventoryOwnedV4::*;friend type access(Items);};
struct Character {using type=std::uintptr_t FreshInventoryOwnedV4::*;friend type access(Character);};
struct Tables {using type=LootTablesV2::Borrow FreshInventoryOwnedV4::*;friend type access(Tables);};
struct Random {using type=LootRandom8V2* FreshInventoryOwnedV4::*;friend type access(Random);};
struct Potion {using type=ItemInstanceV1* FreshInventoryOwnedV4::*;friend type access(Potion);};
struct CallbackDepth {using type=std::uint32_t FreshInventoryOwnedV4::*;friend type access(CallbackDepth);};
struct Equipment {using type=std::array<std::array<OwnedItemSlotV4*,9>,2> FreshInventoryOwnedV4::*;friend type access(Equipment);};
struct Allowed {using type=bool(FreshInventoryOwnedV4::*)(std::string&)const;friend type access(Allowed);};
struct Quantity {using type=bool(FreshInventoryOwnedV4::*)(ItemInstanceV1&,std::int32_t,std::string&);friend type access(Quantity);};
struct Destroy {using type=bool(FreshInventoryOwnedV4::*)(std::unique_ptr<ItemInstanceV1>&,const OwnedInventoryServicesV4&,std::uint32_t,std::string&);friend type access(Destroy);};
template struct Expose<Items,&FreshInventoryOwnedV4::items_>;
template struct Expose<Character,&FreshInventoryOwnedV4::character_>;
template struct Expose<Tables,&FreshInventoryOwnedV4::tables_>;
template struct Expose<Random,&FreshInventoryOwnedV4::random_>;
template struct Expose<Potion,&FreshInventoryOwnedV4::potion_>;
template struct Expose<CallbackDepth,&FreshInventoryOwnedV4::callback_depth_>;
template struct Expose<Equipment,&FreshInventoryOwnedV4::equipment_>;
template struct Expose<Allowed,&FreshInventoryOwnedV4::mutation_allowed>;
template struct Expose<Quantity,&FreshInventoryOwnedV4::add_quantity>;
template struct Expose<Destroy,&FreshInventoryOwnedV4::destroy>;
}
bool CharacterMenuInventoryMutationV1::make_drop_inventory(FreshInventoryOwnedV4& source,std::unique_ptr<FreshInventoryOwnedV4>& output,std::string& error){
 if(output||!source.character()||!(source.*access(Tables{}))||!(source.*access(Random{}))||!source.properties()){error="Required actual drop constructor providers unavailable";return false;}
 // Frozen V4 constructor only retains inputs and validates them; it invokes
 // no callbacks and mutates no Character/RNG/property state. Use the existing
 // real Character solely for that validation, then erase the field before any
 // observable delivery. Original default ItemInventory character is NULL.
 auto temporary=std::make_unique<FreshInventoryOwnedV4>(source.character(),source.*access(Tables{}),*(source.*access(Random{})),std::int8_t(-1),source.properties());
 temporary.get()->*access(Character{})=0;
 output=std::move(temporary);error.clear();return true;
}
bool CharacterMenuInventoryMutationV1::add_quantity(ItemInstanceV1& item,std::int32_t amount,std::string& error){
 if(!(inventory_.*access(Allowed{}))(error))return false;
 auto& items=inventory_.*access(Items{});
 if(std::none_of(items.begin(),items.end(),[&](const auto& p){return p&&p->item.get()==&item;})){error="Quantity item does not belong to this inventory";return false;}
 return (inventory_.*access(Quantity{}))(item,amount,error);
}
bool CharacterMenuInventoryMutationV1::callback(const std::function<bool(std::string&)>& fn,std::string& error){
 if(!(inventory_.*access(Allowed{}))(error))return false;
 if(!fn){error="Required inventory-bound callback unavailable";return false;}
 auto& depth=inventory_.*access(CallbackDepth{});++depth;
 struct Guard{std::uint32_t& depth;~Guard(){--depth;}}guard{depth};return fn(error);
}
bool CharacterMenuInventoryMutationV1::remove(std::uint32_t index,const OwnedInventoryServicesV4& services,std::string& error){
 if(!(inventory_.*access(Allowed{}))(error))return false;
 auto& items=inventory_.*access(Items{});
 if(index>=items.size()||!items[index]||!items[index]->item){error="RemoveItem assertion domain unsupported";return false;}
 auto* cell=items[index].get();
 // Original RemoveItem clears pointers directly; UnEquipSlot would merge stacks.
 auto& equipment=inventory_.*access(Equipment{});
 auto* row=item(inventory_.table(),cell->item->id);
 if(!row){error="RemoveItem metadata unavailable";return false;}
 auto target=row->record.words[26];if(target==-4||target==-3)target=1;else if(target==-2)target=5;
 auto clear=[&](){bool equipped;if(!inventory_.is_equipped(index,equipped,error))return false;if(!equipped)return true;
  unsigned set=(target<0||target==1||target==2)?unsigned(inventory_.current_equipment()):0u;
  auto slot=cell->slots[set];if(slot<0||slot>=9){error="Invalid RemoveItem equipment slot byte";return false;}
  equipment[set][unsigned(slot)]=nullptr;return true;};
 if(!clear())return false;
 inventory_.swap_equipment();bool cleared=clear();inventory_.swap_equipment();if(!cleared)return false;
 auto& potion=inventory_.*access(Potion{});if(potion==cell->item.get())potion=nullptr;
 if(!(inventory_.*access(Destroy{}))(cell->item,services,0x3fe558,error))return false;
 items.erase(items.begin()+index);error.clear();return true;
}
bool CharacterMenuInventoryMutationV1::transfer(std::uint32_t index,FreshInventoryOwnedV4& recipient,std::int32_t quantity,bool force,bool convert,std::int32_t& result,const OwnedInventoryServicesV4& source_services,const OwnedInventoryServicesV4& recipient_services,std::string& error){
 result=-1;if(!(inventory_.*access(Allowed{}))(error)||!(recipient.*access(Allowed{}))(error))return false;
 if(&recipient==&inventory_){error="Self TransferItem requires original assertion continuation";return false;}
 auto& items=inventory_.*access(Items{});
 if(index>=items.size()||!items[index]||!items[index]->item){error="TransferItem assertion domain unsupported";return false;}
 if(quantity<=0){error.clear();return true;}
 auto* cell=items[index].get();auto* identity=cell->item.get();auto available=identity->signed_quantity();
 if(available==0){result=0;error.clear();return true;}
 if(!recipient_services.invoke){error="Required recipient inventory services unavailable";return false;}
 if(available>quantity){std::unique_ptr<ItemInstanceV1> split;if(!inventory_.split_item(*identity,quantity,split,source_services,error))return false;if(!split){error="Transfer split requires original assertion continuation";return false;}return callback([&](std::string& e){return recipient.add_item(split,force,convert,result,recipient_services,e);},error);}
 for(unsigned set=0;set<2;++set){
  auto found=std::find_if(items.begin(),items.end(),[&](const auto& p){return p.get()==cell;});
  if(found==items.end()||!cell->item||cell->item.get()!=identity){error="Transfer unequip consumed its source cell";return false;}
  if(cell->slots[set]!=-1){auto slot=cell->slots[set];if(slot<0||slot>=9){error="Invalid TransferItem equipment slot byte";return false;}if(!inventory_.unequip_from_slot(unsigned(slot),int(set),source_services,error))return false;}
 }
 auto found=std::find_if(items.begin(),items.end(),[&](const auto& p){return p.get()==cell;});
 if(found==items.end()){error="Transfer unequip consumed its source cell";return false;}
 auto& potion=inventory_.*access(Potion{});if(potion==identity)potion=nullptr;
 // Move the unique owner itself. On a provider failure an unconsumed item stays
 // in the original source slot; a consumed prefix removes only that slot.
 auto& depth=inventory_.*access(CallbackDepth{});++depth;
 struct BorrowGuard{std::uint32_t& depth;~BorrowGuard(){--depth;}}guard{depth};
 bool ok=recipient.add_item(cell->item,force,convert,result,recipient_services,error);
 if(!cell->item)items.erase(found);
 return ok;
}
}
