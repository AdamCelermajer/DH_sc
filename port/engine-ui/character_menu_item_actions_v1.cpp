#include "character_menu_item_actions_v1.hpp"
#include <cstring>
namespace dh2::ui {
namespace {
std::int32_t signed_word(std::uint32_t value){std::int32_t out;std::memcpy(&out,&value,4);return out;}
std::int32_t multiply(std::int32_t a,std::int32_t b){return signed_word(std::uint32_t(a)*std::uint32_t(b));}
struct Guard{bool& running;explicit Guard(bool& v):running(v){v=true;}~Guard(){running=false;}};
}
bool CharacterMenuItemActionsV1::ready(std::string& error)const{
 if(running_){error="Unsupported item action callback reentry";return false;}
 if(!graph_.owner||!graph_.inventory||!graph_.inventory->character()||!graph_.properties){error="Required actual Character inventory/property backing unavailable";return false;}
 auto& state=*graph_.inventory->properties();
 if(graph_.properties->saved!=state.saved.data()||graph_.properties->resolved!=state.resolved.data()){error="Item action property view is not the inventory backing";return false;}
 return true;
}
std::int32_t character_menu_transmute_value_v1(std::int32_t value,std::int32_t raw_bonus,std::int32_t multiplier)noexcept{
 auto bonus=signed_word(std::uint32_t(raw_bonus)+256u);
 auto result=multiply(multiplier,multiply(signed_word(std::uint32_t(value)<<8),bonus)>>8)>>16;
 return result<1?1:result;
}
bool CharacterMenuItemActionsV1::transmute(std::uint32_t index,bool preview,std::int32_t& value,std::string& error){
 value=0;if(!ready(error))return false;Guard guard(running_);auto& inv=*graph_.inventory;
 if(index>=inv.items().size()||!inv.items()[index]||!inv.items()[index]->item){error="Transmute source item unavailable";return false;}
 auto& item=*inv.items()[index]->item;std::int32_t multiplier;data::CharacterMenuInventoryMutationV1 mutation(inv);
 if(!graph_.transmute_multiplier||!mutation.callback([&](auto& e){return graph_.transmute_multiplier(multiplier,e);},error)){if(error.empty())error="Required CharacterDesign.TransmuteMultiplier unavailable";return false;}
 value=character_menu_transmute_value_v1(item.value,inv.properties()->resolved[197],multiplier);
 if(preview){error.clear();return true;}
 if(item.signed_quantity()>1){if(!mutation.add_quantity(item,-1,error))return false;}
 else if(!mutation.remove(index,graph_.inventory_services,error))return false;
 if(!inv.add_gold(value,graph_.inventory_services,error))return false;
 if(dh2_property_add(graph_.properties,213,256)){error="Source transmute property addition failed";return false;}
 bool local;if(!graph_.is_local_player||!mutation.callback([&](auto& e){return graph_.is_local_player(local,e);},error)){if(error.empty())error="Required IsLocalPlayer unavailable";return false;}
 if(local){std::int32_t count;if(dh2_property_resolve(graph_.properties,213,&count)){error="Transmute count query failed";return false;}
  if((count>>8)>=300){bool player;if(!graph_.is_player||!mutation.callback([&](auto& e){return graph_.is_player(player,e);},error)){if(error.empty())error="Required source IsPlayer unavailable";return false;}
   if(player&&(!graph_.achievement||!mutation.callback([&](auto& e){return graph_.achievement("gear_transmute",e);},error))){if(error.empty())error="Required gear_transmute achievement provider unavailable";return false;}}
 }
 if(!graph_.skin||!mutation.callback(graph_.skin,error)){if(error.empty())error="Required source Character::Skin unavailable";return false;}
 error.clear();return true;
}
bool CharacterMenuItemActionsV1::drop(std::uint32_t index,std::string& error){
 if(!ready(error))return false;Guard guard(running_);auto& inv=*graph_.inventory;
 if(index>=inv.items().size()||!inv.items()[index]||!inv.items()[index]->item){error="Drop source item unavailable";return false;}
 data::CharacterMenuInventoryMutationV1 mutation(inv);
 bool online;if(!graph_.online||!mutation.callback([&](auto& e){return graph_.online(online,e);},error)){if(error.empty())error="Required fresh online-session query unavailable";return false;}
 if(online&&(!graph_.drop_packet||!mutation.callback([&](auto& e){return graph_.drop_packet(inv.character(),*inv.items()[index]->item,e);},error))){if(error.empty())error="Required source online drop packet producer unavailable";return false;}
 std::unique_ptr<data::FreshInventoryOwnedV4> temporary;data::OwnedInventoryServicesV4 services;
 if(!graph_.drop_container||!mutation.callback([&](auto& e){return graph_.drop_container(temporary,services,e);},error)){if(error.empty())error="Required source empty drop inventory constructor unavailable";return false;}
 if(!temporary||temporary.get()==&inv||temporary->character()!=0||!temporary->items().empty()||temporary->potion()||temporary->gold()!=0||temporary->current_equipment()!=0||temporary->properties()!=inv.properties()||&temporary->table()!=&inv.table()){error="Drop temporary inventory must have original defaults and same genuine backing";return false;}
 for(const auto& set:temporary->equipment())for(auto* cell:set)if(cell){error="Drop default equipment is not empty";return false;}
 std::int32_t transferred;
 if(!mutation.transfer(index,*temporary,1,false,false,transferred,graph_.inventory_services,services,error))return false;
 if(!graph_.drop_world||!mutation.callback([&](auto& e){return graph_.drop_world(*temporary,inv.character(),inv.character(),0,e);},error)){if(error.empty())error="Required source ItemObject::DropInventory unavailable";return false;}
 error.clear();return true;
}
}
