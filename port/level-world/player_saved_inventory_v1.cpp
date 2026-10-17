#include "player_saved_inventory_v1.hpp"
#include <cstring>
#include <stdexcept>

namespace dh2::player_saved_inventory_v1 {namespace {
struct Range{std::uintptr_t b,e;};
template<class T>bool range(const T* p,Range& r){const auto a=reinterpret_cast<std::uintptr_t>(p);if(!p||a%alignof(T)||a>UINTPTR_MAX-sizeof(T))return false;r={a,a+sizeof(T)};return true;}
bool overlap(Range a,Range b){return a.b<b.e&&b.b<a.e;}
bool valid(const Bindings& b){Range s,i,p,e,a;
 if(!range(b.save,s)||!range(b.inventory,i)||!range(b.properties,p)||!range(b.equipment_services,e)||!range(b.incoming,a)||!b.powers)return false;
 const Range controls[]{s,i,p,e,a};for(unsigned j=0;j<5;++j)for(unsigned k=j+1;k<5;++k)if(overlap(controls[j],controls[k]))return false;
 const auto* state=b.inventory->properties();Range v;if(!range(state,v))return false;
 return b.properties->base==state->base.data()&&b.properties->saved==state->saved.data()&&b.properties->gear==state->gear.data()&&b.properties->resolved==state->resolved.data()&&!dh2_property_validate(b.properties);
}
bool separate(const Bindings& b,Range r){
 const auto clear=[&](const auto* p){Range c;return range(p,c)&&!overlap(r,c);};
 if(!clear(b.save)||!clear(b.inventory)||!clear(b.inventory->properties())||!clear(b.properties)||!clear(b.equipment_services)||!clear(b.incoming))return false;
 const auto& table=b.inventory->table();if(!clear(&table))return false;
 for(const auto& name:table.identifiers)if(!clear(&name))return false;
 for(const auto& field:table.fields)if(!clear(&field))return false;
 for(const auto& row:table.rows)if(!clear(&row))return false;
 for(const auto& name:b.powers.names())if(!clear(&name))return false;
 for(const auto& row:b.powers.rows())if(!clear(&row))return false;
 const auto& v=*b.properties;const std::int32_t* sheets[]{v.defaults,v.types,v.base,v.saved,v.gear,v.resolved};
 for(const auto* p:sheets){const auto at=reinterpret_cast<std::uintptr_t>(p);if(!p||at>UINTPTR_MAX-224*4||overlap(r,{at,at+224*4}))return false;}
 if(v.group_count>10000)return false;
 if(v.group_count){const auto at=reinterpret_cast<std::uintptr_t>(v.groups);const auto n=std::size_t(v.group_count)*sizeof(data::PropertyBuffGroup);if(!v.groups||at>UINTPTR_MAX-n||overlap(r,{at,at+n}))return false;
  for(std::uint32_t g=0;g<v.group_count;++g){const auto& group=v.groups[g];const auto a=reinterpret_cast<std::uintptr_t>(group.sheets);const auto n=std::size_t(group.count)*sizeof(group.sheets[0]);if(group.count>10000||(group.count&&(!group.sheets||a>UINTPTR_MAX-n||overlap(r,{a,a+n}))))return false;
   for(std::uint32_t j=0;j<group.count;++j){const auto s=reinterpret_cast<std::uintptr_t>(group.sheets[j]);if(!s||s>UINTPTR_MAX-224*4||overlap(r,{s,s+224*4}))return false;}
  }
 }
 for(const auto& slot:b.inventory->items())if(!slot||!slot->item||!clear(slot.get())||!clear(slot->item.get()))return false;
 return !*b.incoming||clear(b.incoming->get());
}
std::int32_t signed_word(std::uint32_t u){std::int32_t s;std::memcpy(&s,&u,4);return s;}
struct Call {
 const Bindings& b;data::Bytes bytes;Result& out;std::string& error;
 bool fail(const char* message){if(error.empty())error=message;return false;}
 void stage(Stage s,std::uint32_t at){out.stage=s;out.source_caller=at;}
 bool character(){if(!valid(b))return fail("Saved inventory borrowed owners changed");if(!b.save->character()||b.save->character()!=b.inventory->character())return fail("Saved inventory reached unavailable Character inventory");return true;}
 bool read(std::uint32_t n,const std::uint8_t*& p){++out.read_calls;if(out.consumed>bytes.size||n>bytes.size-out.consumed)return fail("Saved inventory reached truncated source stream");p=bytes.data+out.consumed;out.consumed+=n;return true;}
 bool word(std::uint32_t& value){const std::uint8_t* p;if(!read(4,p))return false;value=p[0]|std::uint32_t(p[1])<<8|std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24;return true;}
 bool text(std::string& value){++out.string_reads;std::uint32_t n;if(!word(n))return false;if(!n||n>1048576)return fail("Saved inventory source string assertion/budget boundary");const std::uint8_t* p;if(!read(n,p))return false;if(p[n-1])return fail("Saved inventory source string lacks terminator");value.assign(reinterpret_cast<const char*>(p),n-1);return true;}
 bool effect(data::OwnedInventoryOperationV4 op,std::uint32_t at,data::ItemInstanceV1* item,std::int32_t argument=0,std::uint32_t index=0){
  if(!b.inventory->saved_item_effect(op,at,item,argument,index,*b.equipment_services,error))return fail("Saved inventory mandatory Item provider failed");
  return valid(b)||fail("Saved inventory borrowed owners changed");
 }
 bool execute(){
  stage(Stage::character,0x46a3c0);if(!character())return false;
  std::uint32_t gold,selected,count;stage(Stage::header,0x46a408);if(!word(gold))return false;stage(Stage::header,0x46a414);if(!word(selected))return false;stage(Stage::header,0x46a420);if(!word(count))return false;out.declared_items=count;
  stage(Stage::gold,0x46a434);if(!character()||!b.inventory->set_gold(signed_word(gold),*b.equipment_services,error))return fail("Saved inventory SetGold failed");
  stage(Stage::selection,0x46a444);if(!character())return false;b.inventory->project_current_equipment(std::uint8_t(selected));++out.selection_stores;
  if(count>65536)return fail("Saved inventory item count exceeds native budget");
  for(std::uint32_t j=0;j<count;++j){
   std::string name;stage(Stage::item_name,0x46a4b0);if(!text(name))return false;
   out.item_id=-1;const auto& names=b.inventory->table().identifiers;for(std::size_t k=0;k<names.size();++k)if(!std::strcmp(name.c_str(),names[k].c_str())){out.item_id=std::int32_t(k);break;}
   std::uint32_t slot0,slot1,quantity,value,powers;const std::uint8_t* identified;
   stage(Stage::item_fields,0x46a508);if(!word(slot0))return false;stage(Stage::item_fields,0x46a514);if(!word(slot1))return false;stage(Stage::item_fields,0x46a520);if(!word(quantity))return false;stage(Stage::item_fields,0x46a52c);if(!word(value))return false;stage(Stage::item_fields,0x46a538);if(!read(1,identified))return false;stage(Stage::item_fields,0x46a544);if(!word(powers))return false;out.declared_powers=powers;out.completed_powers=0;
   const auto flag=*identified;stage(Stage::construct,0x46a560);++out.constructors;
   if(!b.inventory->create_item(out.item_id,quantity,data::RetainedItemSlotV4{b.incoming},*b.equipment_services,error))return fail("Saved inventory Item constructor failed");
   auto* const item=b.incoming->get();if(!item||!valid(b))return fail("Saved inventory Item constructor changed owners");
   stage(Stage::set_value,0x46a56c);++out.set_values;item->value=signed_word(value);
   if(!effect(data::OwnedInventoryOperationV4::update_name,0x3fbc5c,item)||b.incoming->get()!=item)return fail("Saved inventory SetValue changed incoming identity");
   stage(Stage::identified,0x46a57c);item->identified=flag!=0;++out.identified_stores;
   if(powers>65536)return fail("Saved inventory power count exceeds native budget");
   for(std::uint32_t k=0;k<powers;++k){stage(Stage::power_name,0x46a598);if(!text(name))return false;out.power_id=-1;
    const auto& names=b.powers.names();for(std::size_t p=0;p<names.size();++p)if(!std::strcmp(name.c_str(),names[p].c_str())){out.power_id=std::int32_t(p);break;}
    stage(Stage::add_power,0x46a5f4);++out.add_powers;const auto before=item->powers.size();
    if(!effect(data::OwnedInventoryOperationV4::add_power,0x46a5f4,item,out.power_id,UINT32_MAX))return false;
    if(b.incoming->get()!=item||item->powers.size()!=before+1)return fail("Saved inventory AddPower did not append to actual incoming Item");++out.completed_powers;
   }
   stage(Stage::add_item,0x46a620);++out.add_items;if(!character()||!b.inventory->add_item(*b.incoming,true,true,out.inserted_index,*b.equipment_services,error))return fail("Saved inventory AddItemInstance failed");
   if(*b.incoming)return fail("Saved inventory AddItemInstance did not transfer actual Item");
   const std::uint32_t slots[]{slot0,slot1};for(unsigned set=0;set<2;++set)if(slots[set]!=UINT32_MAX){
    stage(set?Stage::equip_second:Stage::equip_first,set?0x46a6a8:0x46a660);if(!character())return false;
    const auto previous=std::uint8_t(b.inventory->current_equipment());b.inventory->project_current_equipment(std::uint8_t(set));++out.selection_stores;++out.equips;
    if(!b.inventory->equip_to_slot(slots[set],std::uint32_t(out.inserted_index),true,data::RetainedItemSlotV4{b.incoming},*b.equipment_services,error))return fail("Saved inventory EquipItemToSlot failed");
    if(!character())return false;b.inventory->project_current_equipment(previous);++out.selection_stores;
   }
   ++out.completed_items;
  }
  stage(Stage::complete,0x46a6f8);return true;
 }
};
}
Runtime::Runtime(Bindings b):bindings_(std::move(b)){if(!valid(bindings_))throw std::invalid_argument("Invalid borrowed saved inventory owners");}
Status Runtime::load(data::Bytes bytes,Result* out,std::string& error){
 if(busy_)return Status::busy;
 Range r,e,t,input;const auto a=reinterpret_cast<std::uintptr_t>(bytes.data);
 if(!valid(bindings_)||!range(out,r)||!range(&error,e)||!range(this,t)||overlap(r,e)||overlap(r,t)||overlap(e,t)||!separate(bindings_,r)||!separate(bindings_,e)||*bindings_.incoming||(!bytes.data&&bytes.size)||bytes.size>67108864||a>UINTPTR_MAX-bytes.size)return Status::invalid_argument;
 input={a,a+bytes.size};if(bytes.size&&(overlap(input,r)||overlap(input,e)||overlap(input,t)||!separate(bindings_,input)))return Status::invalid_argument;
 *out={};error.clear();busy_=true;struct Scope{bool& b;~Scope(){b=false;}}scope{busy_};Call call{bindings_,bytes,*out,error};
 try{return call.execute()?Status::complete:Status::failed;}catch(const std::exception& x){if(error.empty())error=x.what();return Status::failed;}catch(...){if(error.empty())error="Saved inventory required provider exception";return Status::failed;}
}
}
