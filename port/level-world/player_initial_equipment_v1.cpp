#include "player_initial_equipment_v1.hpp"
#include <stdexcept>

namespace dh2::player_initial_equipment_v1 {namespace {
struct Range{std::uintptr_t b,e;};
template<class T>bool range(const T* p,Range& r){const auto a=reinterpret_cast<std::uintptr_t>(p);if(!p||a%alignof(T)||a>UINTPTR_MAX-sizeof(T))return false;r={a,a+sizeof(T)};return true;}
bool overlap(Range a,Range b){return a.b<b.e&&b.b<a.e;}
bool valid(const Bindings& b){Range i,p,s;
 if(!b.character||!range(b.inventory,i)||!range(b.properties,p)||!range(b.equipment_services,s)||overlap(i,p)||overlap(i,s)||overlap(p,s))return false;
 const auto* state=b.inventory->properties();Range v;if(!range(state,v))return false;
 return b.inventory->character()==b.character&&b.properties->base==state->base.data()&&b.properties->saved==state->saved.data()&&
  b.properties->gear==state->gear.data()&&b.properties->resolved==state->resolved.data()&&!dh2_property_validate(b.properties);
}
bool separate(const Bindings& b,Range r){
 const auto clear=[&](const auto* p){Range c;return range(p,c)&&!overlap(r,c);};
 if(!clear(b.inventory)||!clear(b.inventory->properties())||!clear(b.properties)||!clear(b.equipment_services))return false;
 const auto& view=*b.properties;
 const std::int32_t* sheets[]{view.defaults,view.types,view.base,view.saved,view.gear,view.resolved};
 for(const auto* p:sheets){const auto at=reinterpret_cast<std::uintptr_t>(p);if(!p||at>UINTPTR_MAX-224*4||overlap(r,{at,at+224*4}))return false;}
 if(view.group_count>10000)return false;
 if(view.group_count){const auto at=reinterpret_cast<std::uintptr_t>(view.groups);const auto size=std::size_t(view.group_count)*sizeof(data::PropertyBuffGroup);
  if(!view.groups||at>UINTPTR_MAX-size||overlap(r,{at,at+size}))return false;
  for(std::uint32_t g=0;g<view.group_count;++g){const auto& group=view.groups[g];const auto address=reinterpret_cast<std::uintptr_t>(group.sheets);const auto bytes=std::size_t(group.count)*sizeof(group.sheets[0]);
   if(group.count>10000||(group.count&&(!group.sheets||address>UINTPTR_MAX-bytes||overlap(r,{address,address+bytes}))))return false;
   for(std::uint32_t j=0;j<group.count;++j){const auto sheet=reinterpret_cast<std::uintptr_t>(group.sheets[j]);if(!sheet||sheet>UINTPTR_MAX-224*4||overlap(r,{sheet,sheet+224*4}))return false;}
  }
 }
 for(const auto& slot:b.inventory->items())if(!slot||!slot->item||!clear(slot.get())||!clear(slot->item.get()))return false;
 return true;
}
enum SourceOperation:std::uint32_t {online,online_record,num_items,read_gold,loot_property,add_loot,is_equippable,auto_equip,skin};
struct Call {
 const Bindings& b;Result& out;std::string& error;
 int fail(const char* why){if(error.empty())error=why;return -2;}
 int backend(Operation operation,Reply& reply,const std::int32_t* args){
  Request q{};q.operation=operation;q.character=b.character;q.inventory=b.inventory;q.properties=b.properties;q.equipment_services=b.equipment_services;
  for(unsigned j=0;j<5;++j)q.arguments[j]=args[j];
  ++out.backend_calls;out.last_operation=std::uint32_t(operation);reply={};
  if(!b.backend.invoke)return fail("Initial equipment reached provider unavailable");
  try{if(b.backend.invoke(b.backend.context,&q,&reply,error))return fail("Initial equipment provider failed");}
  catch(...){return fail("Initial equipment provider exception");}
  return valid(b)?0:fail("Initial equipment borrowed owners changed");
 }
 int ask(SourceOperation op,std::int32_t& value,std::int32_t a=0,std::int32_t c=0,std::int32_t d=0,std::int32_t e=0,std::int32_t f=0){
  if(!valid(b))return fail("Initial equipment borrowed owners changed");
  const std::int32_t args[]{a,c,d,e,f};Reply reply{};
  switch(op){
  case online:++out.online_queries;if(backend(Operation::online,reply,args))return -2;if(reply.word>255)return fail("Initial equipment online byte unavailable");value=std::int32_t(reply.word);return 0;
  case online_record:++out.record_queries;if(backend(Operation::online_player_record,reply,args))return -2;if(!reply.record)return fail("Initial equipment reached null online record");value=reply.record_byte66c;return 0;
  case num_items:++out.item_count_reads;if(b.inventory->items().size()>65536)return fail("Initial equipment source count exceeds borrowed budget");value=std::int32_t(b.inventory->items().size());return 0;
  case read_gold:++out.gold_reads;value=b.inventory->gold();return 0;
  case loot_property:++out.loot_property_reads;value=b.properties->resolved[9];out.loot=value;return 0;
  case add_loot:++out.add_loot_calls;return backend(Operation::add_loot,reply,args);
  case is_equippable:{++out.equippable_queries;out.last_index=std::uint32_t(a);
   const auto index=std::uint32_t(a);if(a<0||index>=b.inventory->items().size()||!b.inventory->items()[index]||!b.inventory->items()[index]->item)return fail("Initial equipment source Item unavailable");
   const auto* row=data::item(b.inventory->table(),b.inventory->items()[index]->item->id);if(!row)return fail("Initial equipment source Item metadata unavailable");value=row->record.words[26]!=-1;return 0;
  }
  case auto_equip:++out.auto_equip_calls;
   try{if(!b.inventory->character_auto_equip(std::uint32_t(a),value,*b.equipment_services,error))return fail("Initial equipment CharacterAutoEquip failed");}
   catch(...){return fail("Initial equipment CharacterAutoEquip exception");}
   out.last_auto_equip=value;return valid(b)?0:fail("Initial equipment borrowed owners changed");
  case skin:{++out.skin_calls;
   if(!b.equipment_services->invoke)return fail("Initial equipment source Skin provider unavailable");
   const data::OwnedInventoryRequestV4 q{data::OwnedInventoryOperationV4::skin,0x3b3a3c,nullptr,nullptr,0,0};data::OwnedInventoryResponseV4 r{};
   try{if(!b.equipment_services->invoke(b.equipment_services->context,*b.inventory,q,r,error))return fail("Initial equipment source Skin failed");}
   catch(...){return fail("Initial equipment source Skin exception");}
   return valid(b)?0:fail("Initial equipment borrowed owners changed");
  }
  }
  return fail("Initial equipment source operation unavailable");
 }
 // Adam's exact _InitEquipment caller projection, scoped to this one body.
 // Preserve fresh reads, captured loop count, ignored virtual return and
 // normal online-record skip. Added native failures stop at reached prefixes.
 int execute(){
  std::int32_t v=0;
  if(ask(online,v))return -2;
  if(v){if(ask(online_record,v,0))return -2;if(v!=1){out.decision=Decision::online_record_skipped;return 0;}}
  if(ask(num_items,v))return -2;
  if(v){out.decision=Decision::existing_items_skin;return ask(skin,v);}
  if(ask(read_gold,v))return -2;
  if(v){out.decision=Decision::existing_gold_skin;return ask(skin,v);}
  if(ask(loot_property,v,9))return -2;
  const auto loot=v;if(ask(add_loot,v,loot,0,0,-1,0))return -2;
  if(ask(num_items,v))return -2;
  if(!v){out.decision=Decision::empty_after_loot;return 0;}
  if(v<0||v>65536)return fail("Initial equipment source count exceeds borrowed budget");
  out.captured_items=std::uint32_t(v);const auto count=v;
  for(std::int32_t i=0;i<count;++i){if(ask(is_equippable,v,i))return -2;if(v&&ask(auto_equip,v,i))return -2;}
  out.decision=Decision::equipped;return 0;
 }
};
}
Runtime::Runtime(Bindings b):bindings_(b){if(!valid(b))throw std::invalid_argument("Invalid borrowed initial equipment owners");}
Status Runtime::initialize(Result* out,std::string& error){
 if(busy_)return Status::busy;
 Range r,e,t;if(!valid(bindings_)||!range(out,r)||!range(&error,e)||!range(this,t)||overlap(r,e)||overlap(r,t)||overlap(e,t)||!separate(bindings_,r)||!separate(bindings_,e))return Status::invalid_argument;
 *out={};error.clear();busy_=true;struct Scope{bool& busy;~Scope(){busy=false;}}scope{busy_};Call call{bindings_,*out,error};
 try{return call.execute()?Status::failed:Status::complete;}catch(...){if(error.empty())error="Initial equipment provider exception";return Status::failed;}
}
}
