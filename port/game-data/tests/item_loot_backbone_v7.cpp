// Replays Adam's pinned original-derived cases and composes the new modules
// with our selected V4/V5 library. Text/debug services are explicit fixtures.
#include "../loot_power_creation_v7.hpp"
#include "../fresh_inventory_owned_v4.hpp"
extern "C" {
#include "../../random/random.h"
}
#include <array>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::data;
using Raw=std::vector<std::uint8_t>;
static unsigned checks;
static void ck(bool okay,const std::string& why="check failed") {
 ++checks;if(!okay)throw std::runtime_error(why+" at check "+std::to_string(checks));
}
static Raw file(const std::string& p){std::ifstream f(p,std::ios::binary);ck(bool(f),p);return {std::istreambuf_iterator<char>(f),{}};}
static Bytes bytes(const Raw& r){return {r.data(),r.size()};}
static void word(Raw& b,std::uint32_t x){auto* p=reinterpret_cast<const std::uint8_t*>(&x);b.insert(b.end(),p,p+4);}
struct Reader {
 const Raw& b;std::size_t at{};
 std::uint32_t u(){ck(at<=b.size()&&b.size()-at>=4,"truncated fixture");std::uint32_t n;std::memcpy(&n,b.data()+at,4);at+=4;return n;}
 Raw block(){auto n=u();ck(at<=b.size()&&n<=b.size()-at);Raw r(b.begin()+at,b.begin()+at+n);at+=n;return r;}
};
extern "C" std::uint32_t dh2_loot_power_creation_fixture_v7(const std::uint8_t*,std::uint8_t*);
struct Random {
 dh2_random_state live{};unsigned draws{};bool fail{},throws{};
 explicit Random(std::uint32_t seed=1,std::uint32_t calls=0):live{{seed,0x98765432u},{calls,37}}{}
 static bool next(void* p,std::int32_t bound,std::uint32_t stream,std::int32_t& out,std::string& error){
  auto& r=*static_cast<Random*>(p);ck(stream==0,"unexpected source RNG stream");
  out=dh2_random_next(&r.live,std::uint32_t(bound),stream);++r.draws;
  if(r.throws)throw std::runtime_error("declared RNG throw after draw");
  if(r.fail){error="declared RNG failure after draw";return false;}return true;
 }
 InventoryRandomServiceV4 service(){return {this,next};}
};
struct Context {
 const ItemTable* items{};ItemPresentationOwnerV5* presentation{};LootPowerCreationV7* creation{};
 bool fail_append{},empty_append{},reenter{},fail_text{},forget_reentry{},fail_text_after_append{},inside_power_callback{};FreshInventoryOwnedV4* guarded_inventory{};const OwnedInventoryServicesV4* guarded_services{};bool try_reentrant_add{},reentrant_add_attempted{},reentrant_add_rejected{};unsigned debug{},appends{};std::vector<std::string> flow;
 static const Item* metadata(void* p,const ItemInstanceV1& i,std::string& error){
  auto& c=*static_cast<Context*>(p);auto* found=item(*c.items,i.id);if(!found)error="actual item metadata missing";return found;
 }
 static bool text(void* p,ItemInstanceV1& i,const ItemTextRequestV5& q,ItemTextResponseV5& r,std::string& out,std::string& error){
  auto& c=*static_cast<Context*>(p);c.flow.push_back(std::string(c.inside_power_callback?"power_text:":"text:")+std::to_string(std::uint32_t(q.operation)));if(c.fail_text){error="declared text failure";return false;}
  if(c.try_reentrant_add&&!c.inside_power_callback){c.try_reentrant_add=false;c.reentrant_add_attempted=true;auto nested=std::make_unique<ItemInstanceV1>();nested->id=i.id;nested->quantity=1;std::int32_t index=-1;std::string nested_error;auto before=c.guarded_inventory->items().size();auto accepted=c.guarded_inventory->add_item(nested,true,false,index,*c.guarded_services,nested_error);c.reentrant_add_rejected=!accepted&&nested_error.find("reentry")!=std::string::npos&&nested&&c.guarded_inventory->items().size()==before;}
  if(c.forget_reentry){c.forget_reentry=false;std::string nested;ck(!c.presentation->forget(i,nested)&&nested.find("reentry")!=std::string::npos);}
  // Controlled service transport; this does not implement localization or
  // source varargs. Original text arithmetic is exercised by separate gold.
  switch(q.operation){
   case ItemTextOperationV5::constant:r.value=17;return true;
   case ItemTextOperationV5::integer_string:if(q.value==-1){error="required null description continuation";return false;}r.text="OID"+std::to_string(q.value);return true;
   case ItemTextOperationV5::class_name:r.value=q.value;return true;
   case ItemTextOperationV5::parse_varargs:case ItemTextOperationV5::parse_ex:
    out+=q.input?q.input:"";for(unsigned j=0;j<q.count;++j)out+=":"+std::to_string(q.arguments[j].integer);return true;
  }error="required formatter fixture unavailable";return false;
 }
 ItemTextServicesV5 text_service(){return {this,metadata,text};}
 static bool power(void* p,const LootPowerRequestV7& q,std::int32_t& result,std::string& error){
  auto& c=*static_cast<Context*>(p);
  if(q.operation!=LootPowerOperationV7::add_power){++c.debug;result=0;return true;}
  ++c.appends;if(c.fail_append){error="declared append failure";return false;}if(c.empty_append)return true;
  if(c.reenter){c.reenter=false;LootEntry32V2 entry{};entry.words[1]=0;std::string nested;ck(!c.creation->add_powers(entry,*q.item,0,0,0,{p,power},nested)&&nested.find("reentry")!=std::string::npos);}
  return c.presentation->add_power(*q.item,q.power,q.difficulty,c.text_service(),error);
 }
 static void observe(void* p,FreshInventoryOwnedV4&,const OwnedInventoryRequestV4& q){auto& c=*static_cast<Context*>(p);if(q.operation==OwnedInventoryOperationV4::inventory_full)c.flow.push_back("add_item");if(q.operation==OwnedInventoryOperationV4::destroy_item){std::string error;if(!c.presentation->forget(*q.item,error))throw std::runtime_error(error);}}
 static bool inventory(void* p,FreshInventoryOwnedV4&,const OwnedInventoryRequestV4& q,OwnedInventoryResponseV4& response,std::string& error){
  auto& c=*static_cast<Context*>(p);c.flow.push_back("effect:"+std::to_string(std::uint32_t(q.operation)));auto text=c.text_service();
  switch(q.operation){
   case OwnedInventoryOperationV4::update_name:return item_update_name_v5(*q.item,text,error);
   case OwnedInventoryOperationV4::update_stats:return item_update_stats_v5(*q.item,text,error);
   case OwnedInventoryOperationV4::update_requirements:return item_update_requirements_v5(*q.item,text,error);
   case OwnedInventoryOperationV4::add_power:{++c.appends;if(c.fail_append||c.appends==2){error="declared append failure";return false;}if(c.empty_append)return true;struct Flag{bool& value;bool before;explicit Flag(bool& v):value(v),before(v){value=true;}~Flag(){value=before;}} flag(c.inside_power_callback);auto ok=c.presentation->add_power(*q.item,q.argument,std::int32_t(q.index),text,error);if(ok&&c.fail_text_after_append)c.fail_text=true;return ok;}
   case OwnedInventoryOperationV4::player_count:response.value=1;return true;
   case OwnedInventoryOperationV4::current_player:response.identity=0;response.value=0;return true;
   case OwnedInventoryOperationV4::debug_load:case OwnedInventoryOperationV4::debug_query:++c.debug;response.value=0;return true;
   case OwnedInventoryOperationV4::destroy_item:return c.presentation->forget(*q.item,error);
   default:error="required inventory continuation unavailable";return false;
  }
 }
};
int main(int argc,char** argv){try{
 ck(argc==4,"expected golden fixture, canonical pydata, and original AddLoot fixture");std::string error;
 auto source_bytes=file(argv[3]);Reader source{source_bytes};ck(source.u()==0x37564c41,"invalid original AddLoot fixture");
 const auto source_loot=std::int32_t(source.u());const auto source_seed=source.u(),source_calls=source.u();
 const auto source_capacity=std::int8_t(source.u());const auto source_value_bonus=std::int32_t(source.u()),source_power_bonus=std::int32_t(source.u()),source_requested=std::int32_t(source.u()),source_difficulty=std::int32_t(source.u());
 const auto source_seed_after=source.u(),source_calls_after=source.u();auto source_snapshot=source.block();ck(source.at==source_bytes.size(),"trailing original AddLoot fixture data");
 const char* names[]={"item_powers_pyarray.bin","item_powers_pyarraynames.bin","item_powers_pystructnames.bin","item_powers_monopoly_pyarray.bin","item_powers_monopoly_pyarraynames.bin","item_powers_monopoly_pystructnames.bin","loot_table_pyarray.bin","loot_table_pyarraynames.bin","loot_table_pystructnames.bin"};
 std::array<Raw,9> raw;for(unsigned i=0;i<9;++i)raw[i]=file(std::string(argv[2])+"/"+names[i]);
 ck(raw[6].size()==284104,"unexpected canonical quantity section");Raw quantities(raw[6].begin()+283108,raw[6].end());
 LootPowerInputsV7 input{bytes(raw[0]),bytes(raw[1]),bytes(raw[2]),bytes(raw[3]),bytes(raw[4]),bytes(raw[5]),bytes(quantities),bytes(raw[7]),bytes(raw[8])};
 ItemPowerTablesV5 definitions;ck(definitions.load(input.powers,input.power_names,input.power_schema,error),error);
 LootPowerResourcesV7 resources;ck(resources.load(input,definitions.borrow(),error),error);auto pinned=resources.borrow();
 ck(pinned.lists().size()==121&&pinned.quantities().size()==39&&pinned.powers().rows().size()==937);
 ck(&pinned.powers().rows()==&definitions.borrow().rows(),"duplicated power authority");
 auto gold=file(argv[1]);Reader r{gold};ck(r.u()==0x3756504c);auto count=r.u();unsigned weighted=0,quantity=0,values=0;
 while(count--){auto command=r.block(),expected=r.block();Reader c{command};auto op=c.u();auto seed=c.u(),calls=c.u(),n=c.u();LootRandom8V2 fixture{seed,calls};Random live(seed,calls);auto service=live.service();std::int32_t legacy=0,actual=0;int a=-1,b=-1;
  if(op==0){std::vector<LootPowerChoiceV7> rows(n);for(auto& row:rows){row.power=static_cast<std::int32_t>(c.u());auto w=c.u();std::memcpy(&row.probability,&w,1);}std::uint32_t x=0,y=0;a=dh2_loot_power_select_v7(&x,&fixture,rows.data(),n);b=loot_power_select_v7(&y,service,rows.data(),n,error);std::memcpy(&legacy,&x,4);std::memcpy(&actual,&y,4);++weighted;}
  else if(op==1){auto bonus=static_cast<std::int32_t>(c.u());std::vector<LootQuantityChoiceV7> rows(n);for(auto& row:rows){auto w=c.u();std::memcpy(&row,&w,4);}a=dh2_loot_quantity_v7(&legacy,&fixture,rows.data(),n,bonus);b=loot_quantity_v7(&actual,service,rows.data(),n,bonus,error);++quantity;}
  else{ck(op==2);auto bonus=static_cast<std::int32_t>(c.u());ItemRecord164 record{};for(auto& w:record.words)w=static_cast<std::int32_t>(c.u());std::vector<LootPowerGold8V7> powers(n);for(auto& p:powers){p.multiplier=static_cast<std::int32_t>(c.u());p.bonus=static_cast<std::int32_t>(c.u());}a=dh2_loot_item_value_v7(&legacy,&fixture,&record,powers.data(),n,bonus);b=loot_value_v7(&actual,service,&record,powers.data(),n,bonus,error);++values;}
  ck(a==0&&b==0&&legacy==actual&&c.at==command.size(),error);Raw observed;word(observed,std::uint32_t(actual));word(observed,live.live.seeds[0]);word(observed,live.live.counters[0]);ck(observed==expected,"original RNG/value comparison failed");ck(fixture.seed==live.live.seeds[0]&&fixture.calls==live.live.counters[0]);ck(live.live.seeds[1]==0x98765432u&&live.live.counters[1]==37,"other RNG stream changed");
 }
 auto cases=r.u();Raw commands,expected;word(commands,cases);for(unsigned i=0;i<cases;++i){auto command=r.block(),result=r.block();commands.insert(commands.end(),command.begin(),command.end());word(expected,result.size());expected.insert(expected.end(),result.begin(),result.end());}ck(r.at==gold.size());
 Raw fixture;for(unsigned i=0;i<9;++i){const auto& data=i==6?quantities:raw[i];word(fixture,data.size());fixture.insert(fixture.end(),data.begin(),data.end());}fixture.insert(fixture.end(),commands.begin(),commands.end());Raw output(expected.size()+1024);auto size=dh2_loot_power_creation_fixture_v7(fixture.data(),output.data());ck(size==expected.size());output.resize(size);ck(output==expected,"original coordinator comparison failed");
 LootTablesV2 loots;ck(loots.load(bytes(raw[6]),input.loot_names,input.loot_schema,error),error);
 Random random;auto random_service=random.service();LootPowerCreationV7 creation(pinned,random_service);ItemPresentationOwnerV5 presentation(definitions.borrow());Context context{&loots.borrow().items(),&presentation,&creation};
 PropertyState properties;FreshInventoryOwnedV4 inventory(0x100000001,loots.borrow(),random_service,12,properties);ck(inventory.properties()==&properties);
 OwnedInventoryServicesV4 effects{&context,Context::inventory,Context::observe};LootPowerServicesV7 power_services{&context,Context::power};unsigned stored=0;
 // This explicit source-field fixture permits independent 121x3 creation
 // cases without substituting inventory fullness behaviour.
 inventory.project_unlimited(true);
 for(unsigned id=0;id<pinned.lists().size();++id)for(int difficulty=0;difficulty<3;++difficulty){
  std::unique_ptr<ItemInstanceV1> item;ck(inventory.create_item(664,1,RetainedItemSlotV4{&item},effects,error),error);auto* identity=item.get();LootEntry32V2 entry{};entry.words[1]=id;
  ck(creation.add_powers(entry,*item,0,1,difficulty,power_services,error),error);ck(loot_item_value_v7(*item,inventory.table(),definitions.borrow(),random_service,0,context.text_service(),error),error);
  ck(!item->name.empty());auto* powers=presentation.powers(*item);ck(powers&&powers->size()==item->powers.size());for(const auto& power:*powers)ck(!power.description.empty());
  std::int32_t index=-1;ck(inventory.add_item(item,true,false,index,effects,error),error);ck(!item&&index>=0&&inventory.items().at(index)->item.get()==identity);++stored;
 }
 unsigned guards=0;
 // Exercise the composed fixed-loot path on an actual canonical powered row.
 // The exact source inputs stay explicit test fixtures; no Crypt drop ID or
 // runtime caller values are inferred here.
 std::uint32_t integrated_loot=UINT32_MAX;
 for(std::uint32_t id=0;id<loots.borrow().loots().size()&&integrated_loot==UINT32_MAX;++id){const auto& loot=loots.borrow().loots()[id];if(!loot.random_entries.empty()||!loot.sub_loots.empty()||loot.fixed_entries.size()!=1)continue;const auto& fixed=loot.fixed_entries[0];if(fixed.words[1]<0||std::size_t(fixed.words[1])>=pinned.lists().size()||pinned.lists()[fixed.words[1]].empty()||fixed.words[0]<0||std::size_t(fixed.words[0])>=loots.borrow().item_lists().size())continue;bool safe=true;for(const auto& candidate:loots.borrow().item_lists()[fixed.words[0]]){auto* row=item(loots.borrow().items(),candidate.item);if(!row||item_type(*row)==13){safe=false;break;}}if(safe)integrated_loot=id;}
 ck(integrated_loot!=UINT32_MAX,"no canonical single-entry powered fixed-loot test row");
 ck(std::int8_t(source_capacity)==12&&source_loot>=0,"invalid original AddLoot inputs");Random integrated_random(source_seed,source_calls);auto integrated_rng=integrated_random.service();LootPowerCreationV7 integrated_creation(pinned,integrated_rng);ItemPresentationOwnerV5 integrated_presentation(definitions.borrow());Context integrated_context{&loots.borrow().items(),&integrated_presentation,&integrated_creation};PropertyState integrated_properties;FreshInventoryOwnedV4 integrated_inventory(0x100000002,loots.borrow(),integrated_rng,source_capacity,integrated_properties);integrated_inventory.project_current_equipment(0);OwnedInventoryServicesV4 integrated_services{&integrated_context,Context::inventory,Context::observe,true};OwnedLootEffectsV7 integrated_effects{&integrated_creation,definitions.borrow(),integrated_context.text_service(),source_value_bonus,source_power_bonus,source_requested,source_difficulty};std::unique_ptr<ItemInstanceV1> pending;
 ck(integrated_loot==std::uint32_t(source_loot),"original AddLoot loot row differs from discovered host row");integrated_context.guarded_inventory=&integrated_inventory;integrated_context.guarded_services=&integrated_services;integrated_context.try_reentrant_add=true;ck(integrated_inventory.add_fixed_loot(source_loot,RetainedItemSlotV4{&pending},integrated_services,integrated_effects,error),error);ck(integrated_context.reentrant_add_attempted&&integrated_context.reentrant_add_rejected,"post-value text callback mutated the V4 owner during AddLoot");ck(!pending&&integrated_inventory.items().size()==1&&integrated_inventory.properties()==&integrated_properties);auto* integrated_item=integrated_inventory.items()[0]->item.get();auto* integrated_power_state=integrated_presentation.powers(*integrated_item);ck(!integrated_item->name.empty()&&!integrated_item->powers.empty()&&integrated_power_state&&integrated_power_state->size()==integrated_item->powers.size());
 bool source_host_parity=true;unsigned source_host_differences=0,source_power_count=0,host_power_count=0;std::int32_t source_value=0;auto compare_source=[&](std::uint32_t expected,std::uint32_t actual){if(expected!=actual){source_host_parity=false;++source_host_differences;}};
 Reader source_result{source_snapshot};compare_source(source_result.u(),std::uint32_t(integrated_inventory.gold()));compare_source(source_result.u(),std::uint32_t(integrated_inventory.current_equipment()));auto source_item_count=source_result.u();ck(source_item_count==integrated_inventory.items().size(),"source/host AddLoot inventory item count differs");for(std::uint32_t i=0;i<source_item_count;++i){const auto& slot=*integrated_inventory.items()[i];const auto& actual=*slot.item;auto source_id=source_result.u(),source_quantity=source_result.u(),source_item_value=source_result.u();source_value=std::int32_t(source_item_value);compare_source(source_id,std::uint32_t(actual.id));compare_source(source_quantity,actual.quantity);compare_source(source_item_value,std::uint32_t(actual.value));compare_source(source_result.u(),actual.identified);compare_source(source_result.u(),std::uint8_t(slot.slots[0]));compare_source(source_result.u(),std::uint8_t(slot.slots[1]));auto count=source_result.u();source_power_count=count;host_power_count=unsigned(actual.powers.size());compare_source(count,host_power_count);for(std::uint32_t j=0;j<count;++j){auto power=source_result.u();if(j<actual.powers.size())compare_source(power,std::uint32_t(actual.powers[j]));else{source_host_parity=false;++source_host_differences;}}}auto source_potion=source_result.u();std::uint32_t host_potion=UINT32_MAX;for(std::uint32_t i=0;i<integrated_inventory.items().size();++i)if(integrated_inventory.items()[i]->item.get()==integrated_inventory.potion())host_potion=i;compare_source(source_potion,host_potion);for(unsigned set=0;set<2;++set)for(unsigned slot=0;slot<9;++slot){std::uint32_t host_index=UINT32_MAX;auto* equipped=integrated_inventory.equipment()[set][slot];for(std::uint32_t i=0;i<integrated_inventory.items().size();++i)if(equipped==integrated_inventory.items()[i].get())host_index=i;compare_source(source_result.u(),host_index);}ck(source_result.at==source_snapshot.size(),"trailing original inventory snapshot");const bool source_rng_match=integrated_random.live.seeds[0]==source_seed_after&&integrated_random.live.counters[0]==source_calls_after;compare_source(source_rng_match?1u:0u,1u);
 auto append=std::find(integrated_context.flow.begin(),integrated_context.flow.end(),"effect:"+std::to_string(std::uint32_t(OwnedInventoryOperationV4::add_power)));auto add_item=std::find(integrated_context.flow.begin(),integrated_context.flow.end(),"add_item");auto text_after_append=append==integrated_context.flow.end()?integrated_context.flow.end():std::find_if(append+1,integrated_context.flow.end(),[](const std::string& event){return event.rfind("text:",0)==0;});ck(append!=integrated_context.flow.end()&&text_after_append!=integrated_context.flow.end()&&add_item!=integrated_context.flow.end()&&append<text_after_append&&text_after_append<add_item,"AddPower -> post-power value text -> same-owner AddItem order differs");auto* value_row=item(loots.borrow().items(),integrated_item->id);std::uint32_t expected_value=std::uint32_t(value_row->record.words[27])*std::uint32_t(value_row->record.words[28]);for(auto power_id:integrated_item->powers){const auto& power=definitions.borrow().rows()[power_id].scalars;expected_value+=std::uint32_t(power.gold_multiplier)*std::uint32_t(power.gold_bonus);}std::int32_t expected_signed;std::memcpy(&expected_signed,&expected_value,4);ck(integrated_item->value==expected_signed,"integrated item missed source power-aware value");ck(integrated_presentation.forget(*integrated_item,error));
 // A source-compatible failure after the actual AddPower append but during
 // CalcLootItemValue.UpdateName preserves the caller-owned item/value/power
 // prefix and never publishes a half-finished item into inventory storage.
 Random failed_random;auto failed_rng=failed_random.service();LootPowerCreationV7 failed_creation(pinned,failed_rng);ItemPresentationOwnerV5 failed_presentation(definitions.borrow());Context failed_context{&loots.borrow().items(),&failed_presentation,&failed_creation};failed_context.fail_text_after_append=true;PropertyState failed_properties;FreshInventoryOwnedV4 failed_inventory(0x100000003,loots.borrow(),failed_rng,12,failed_properties);OwnedInventoryServicesV4 failed_services{&failed_context,Context::inventory,Context::observe,true};OwnedLootEffectsV7 failed_effects{&failed_creation,definitions.borrow(),failed_context.text_service(),0,0,1,0};std::unique_ptr<ItemInstanceV1> failed_item;ck(!failed_inventory.add_fixed_loot(std::int32_t(integrated_loot),RetainedItemSlotV4{&failed_item},failed_services,failed_effects,error)&&error=="declared text failure");ck(failed_item&&failed_item->value!=0&&failed_item->powers.size()==1&&failed_inventory.items().empty()&&failed_context.appends==1);auto* failed_power_state=failed_presentation.powers(*failed_item);ck(failed_power_state&&failed_power_state->size()==1&&failed_random.draws>0);ck(failed_presentation.forget(*failed_item,error));++guards;
 ck(random.draws>0&&random.live.seeds[1]==0x98765432u&&random.live.counters[1]==37);
 ItemInstanceV1 sample;sample.id=664;LootEntry32V2 entry{};entry.words[1]=0;
 context.empty_append=true;ck(!creation.add_powers(entry,sample,0,1,0,power_services,error)&&sample.powers.empty()&&error.find("append one")!=std::string::npos);context.empty_append=false;++guards;
 context.fail_append=true;ck(!creation.add_powers(entry,sample,0,1,0,power_services,error)&&sample.powers.empty());context.fail_append=false;++guards;
 context.reenter=true;context.forget_reentry=true;ck(creation.add_powers(entry,sample,0,1,0,power_services,error),error);ck(!sample.powers.empty());++guards;ck(presentation.forget(sample,error));sample.powers.clear();
 context.fail_text=true;ck(!creation.add_powers(entry,sample,0,1,0,power_services,error)&&sample.powers.size()==1&&presentation.powers(sample)->size()==1);context.fail_text=false;++guards;ck(presentation.forget(sample,error));sample.powers.clear();
 sample.value=-123;ck(!loot_item_value_v7(sample,inventory.table(),definitions.borrow(),random_service,0,{},error)&&sample.value!=-123);++guards;
 std::int32_t sentinel=42;auto before=random.draws;ck(loot_quantity_v7(&sentinel,random_service,nullptr,0,0,error)==-2&&sentinel==42&&before==random.draws);++guards;
 LootQuantityChoiceV7 row{1,100};random.fail=true;error.clear();ck(loot_quantity_v7(&sentinel,random_service,&row,1,0,error)==-2&&sentinel==42&&random.draws==before+1&&error=="declared RNG failure after draw");random.fail=false;++guards;
 random.throws=true;error.clear();ck(loot_quantity_v7(&sentinel,random_service,&row,1,0,error)==-2&&sentinel==42&&random.draws==before+2&&error.find("threw")!=std::string::npos);random.throws=false;++guards;
 inventory.project_unlimited(false);std::unique_ptr<ItemInstanceV1> full;ck(inventory.create_item(664,1,RetainedItemSlotV4{&full},effects,error),error);auto* identity=full.get();std::int32_t index=-1;ck(!inventory.add_item(full,true,false,index,effects,error)&&!full&&inventory.items().back()->item.get()==identity&&error=="required inventory continuation unavailable");++guards;
 for(const auto& slot:inventory.items())ck(presentation.forget(*slot->item,error));resources=LootPowerResourcesV7{};definitions=ItemPowerTablesV5{};for(auto& data:raw)data.clear();quantities.clear();ck(creation.resources().lists().size()==121&&pinned.powers().rows().size()==937);
 std::cout<<"{\"validation\":\""<<(source_host_parity?"PASS":"SOURCE_PARITY_GAP")<<"\",\"weighted_original_cases\":"<<weighted<<",\"quantity_original_cases\":"<<quantity<<",\"value_original_cases\":"<<values<<",\"coordinator_original_cases\":"<<cases<<",\"same_inventory_powered_items\":"<<stored<<",\"addloot_v4_v7_compositions\":1,\"addloot_fixture_id\":"<<integrated_loot<<",\"source_host_parity\":"<<(source_host_parity?"true":"false")<<",\"source_host_differences\":"<<source_host_differences<<",\"source_power_count\":"<<source_power_count<<",\"host_power_count\":"<<host_power_count<<",\"source_value\":"<<source_value<<",\"host_value\":"<<integrated_inventory.items()[0]->item->value<<",\"source_rng_seed\":"<<source_seed_after<<",\"host_rng_seed\":"<<integrated_random.live.seeds[0]<<",\"source_rng_calls\":"<<source_calls_after<<",\"host_rng_calls\":"<<integrated_random.live.counters[0]<<",\"addloot_failure_prefixes\":1,\"borrowed_rng_draws\":"<<random.draws+integrated_random.draws+failed_random.draws<<",\"required_prefix_guards\":"<<guards<<",\"checks\":"<<checks<<",\"mismatches\":0,\"same_live_properties_rng\":true,\"text_debug_services_are_fixtures\":true,\"native_gameplay\":false,\"full_AddLoot\":false}\n";return source_host_parity?0:2;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
