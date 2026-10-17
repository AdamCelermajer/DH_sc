// Replays real direct and recursive random LootTable rows through the existing
// V4/V5/V7 owners. Debug/text and AddPower transport are bounded fixtures.
#include "../fresh_inventory_owned_v4.hpp"
extern "C" {
#include "../../random/random.h"
}
#include <algorithm>
#include <array>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdexcept>
using namespace dh2::data;
using Raw=std::vector<std::uint8_t>;
static unsigned checks;
static void ck(bool ok,const std::string& why="check failed"){
 ++checks;if(!ok)throw std::runtime_error(why+" at check "+std::to_string(checks));
}
static Raw file(const std::string& path){std::ifstream f(path,std::ios::binary);ck(bool(f),path);return {std::istreambuf_iterator<char>(f),{}};}
static Bytes bytes(const Raw& raw){return {raw.data(),raw.size()};}
struct Reader{
 const Raw& raw;std::size_t at{};
 std::uint32_t u(){ck(at<=raw.size()&&raw.size()-at>=4,"truncated fixture");std::uint32_t v;std::memcpy(&v,raw.data()+at,4);at+=4;return v;}
 Raw block(){auto size=u();ck(size<=raw.size()-at,"truncated fixture block");Raw out(raw.begin()+at,raw.begin()+at+size);at+=size;return out;}
};
struct Random{
 dh2_random_state state{};unsigned draws{};
 Random(std::uint32_t seed,std::uint32_t calls):state{{seed,0x98765432u},{calls,37}}{}
 static bool next(void* p,std::int32_t bound,std::uint32_t stream,std::int32_t& out,std::string& error){
  auto& self=*static_cast<Random*>(p);if(stream!=0){error="unexpected source RNG stream";return false;}
  out=dh2_random_next(&self.state,std::uint32_t(bound),stream);++self.draws;return true;
 }
 InventoryRandomServiceV4 service(){return {this,next};}
};
struct Services{
 const ItemTable* items{};ItemPresentationOwnerV5* presentation{};unsigned adds{},queries{},gold_notifications{};
 static const Item* metadata(void* p,const ItemInstanceV1& i,std::string& error){auto& s=*static_cast<Services*>(p);auto* row=item(*s.items,i.id);if(!row)error="actual item metadata missing";return row;}
 static bool text(void*,ItemInstanceV1&,const ItemTextRequestV5& q,ItemTextResponseV5& r,std::string& out,std::string& error){
  switch(q.operation){case ItemTextOperationV5::constant:r.value=17;return true;
   case ItemTextOperationV5::integer_string:if(q.value==-1){error="missing description fixture";return false;}r.text="OID"+std::to_string(q.value);return true;
   case ItemTextOperationV5::class_name:r.value=q.value;return true;
   case ItemTextOperationV5::parse_varargs:case ItemTextOperationV5::parse_ex:out+=q.input?q.input:"";for(unsigned i=0;i<q.count;++i)out+=":"+std::to_string(q.arguments[i].integer);return true;}
  error="required text service fixture unavailable";return false;
 }
 ItemTextServicesV5 text_service(){return {this,metadata,text};}
 static bool inventory(void* p,FreshInventoryOwnedV4&,const OwnedInventoryRequestV4& q,OwnedInventoryResponseV4& r,std::string& error){
  auto& s=*static_cast<Services*>(p);auto text=s.text_service();
  switch(q.operation){case OwnedInventoryOperationV4::update_name:return item_update_name_v5(*q.item,text,error);
   case OwnedInventoryOperationV4::update_stats:return item_update_stats_v5(*q.item,text,error);
   case OwnedInventoryOperationV4::update_requirements:return item_update_requirements_v5(*q.item,text,error);
   case OwnedInventoryOperationV4::add_power:++s.adds;return s.presentation->add_power(*q.item,q.argument,std::int32_t(q.index),text,error);
   case OwnedInventoryOperationV4::player_count:r.value=1;return true;
   case OwnedInventoryOperationV4::current_player:r.identity=0;r.value=0;return true;
   case OwnedInventoryOperationV4::gold_notifications:++s.gold_notifications;return true;
   case OwnedInventoryOperationV4::debug_load:case OwnedInventoryOperationV4::debug_query:++s.queries;r.value=0;return true;
   case OwnedInventoryOperationV4::destroy_item:return s.presentation->forget(*q.item,error);
   default:error="required inventory continuation unavailable";return false;}
 }
 static void observe(void* p,FreshInventoryOwnedV4&,const OwnedInventoryRequestV4& q){
  auto& s=*static_cast<Services*>(p);if(q.operation==OwnedInventoryOperationV4::destroy_item){std::string error;if(!s.presentation->forget(*q.item,error))throw std::runtime_error(error);}
 }
 static bool power(void* p,const LootPowerRequestV7& q,std::int32_t& result,std::string& error){
  auto& s=*static_cast<Services*>(p);if(q.operation!=LootPowerOperationV7::add_power){++s.queries;result=0;return true;}
  ++s.adds;return s.presentation->add_power(*q.item,q.power,q.difficulty,s.text_service(),error);
 }
};
static int find_gold_loot(const std::string& cache){
 const auto records=file(cache+"/loot_table_pyarray.bin");
 const auto names=file(cache+"/loot_table_pyarraynames.bin");
 const auto schema=file(cache+"/loot_table_pystructnames.bin");
 LootTablesV2 tables;std::string error;ck(tables.load(bytes(records),bytes(names),bytes(schema),error),error);const auto view=tables.borrow();
 std::uint32_t matches=0;
 for(std::uint32_t root=0;root<view.loots().size();++root){
  std::vector<std::int32_t> active;
  std::function<void(std::int32_t,std::uint32_t)> walk=[&](std::int32_t id,std::uint32_t depth){
   if(id<0||std::size_t(id)>=view.loots().size()||depth>64||
      std::find(active.begin(),active.end(),id)!=active.end())return;
   active.push_back(id);const auto& table=view.loots()[std::size_t(id)];
   auto inspect=[&](const std::vector<LootEntry32V2>& entries,const char* kind){
    for(std::uint32_t entry_index=0;entry_index<entries.size();++entry_index){
     const auto& entry=entries[entry_index];const auto list_id=entry.words[0];
     if(list_id<0||std::size_t(list_id)>=view.item_lists().size())continue;
     const auto& list=view.item_lists()[std::size_t(list_id)];
     for(const auto& choice:list){const auto* row=item(view.items(),choice.item);
      if(row&&item_type(*row)==13){
       std::cout<<root<<'\t'<<view.loot_names()[root]<<'\t'<<id<<'\t'<<kind<<'\t'
        <<entry_index<<'\t'<<list_id<<'\t'<<view.item_list_names()[std::size_t(list_id)]<<'\t'
        <<choice.item<<'\t'<<row->name<<'\t'<<choice.probability<<'\t'
        <<unsigned(choice.quantity)<<'\t'<<entry.words[1]<<'\t'<<table.roll_type<<'\t'
        <<table.num_random_item_probs<<'\n';++matches;
      }
     }
    }
   };
   inspect(table.fixed_entries,"fixed");inspect(table.random_entries,"random");
   for(auto child:table.sub_loots)walk(child,depth+1);active.pop_back();
  };
  walk(std::int32_t(root),0);
 }
 std::cerr<<"gold loot paths: "<<matches<<'\n';return 0;
}
int main(int argc,char** argv){try{
 if(argc==3&&std::string(argv[1])=="--find-gold")return find_gold_loot(argv[2]);
 ck(argc==3,"expected original source fixture and canonical pydata directory");auto fixture=file(argv[1]);Reader input{fixture};
 ck(input.u()==0x37564c41,"invalid original AddLoot fixture");const auto loot_id=std::int32_t(input.u());const auto seed=input.u(),calls=input.u();const auto capacity=std::int8_t(input.u());
 const auto value_bonus=std::int32_t(input.u()),power_bonus=std::int32_t(input.u()),requested=std::int32_t(input.u()),difficulty=std::int32_t(input.u());
 const auto seed_after=input.u(),calls_after=input.u();auto source_snapshot=input.block();ck(input.at==fixture.size(),"trailing original source fixture");
 const char* names[]={"item_powers_pyarray.bin","item_powers_pyarraynames.bin","item_powers_pystructnames.bin","item_powers_monopoly_pyarray.bin","item_powers_monopoly_pyarraynames.bin","item_powers_monopoly_pystructnames.bin","loot_table_pyarray.bin","loot_table_pyarraynames.bin","loot_table_pystructnames.bin"};
 std::array<Raw,9> raw;for(unsigned i=0;i<9;++i)raw[i]=file(std::string(argv[2])+"/"+names[i]);
 ck(raw[6].size()==284104,"unexpected canonical loot/NumProbArray cache length");Raw quantities(raw[6].begin()+283108,raw[6].end());std::string error;
 LootPowerInputsV7 power_input{bytes(raw[0]),bytes(raw[1]),bytes(raw[2]),bytes(raw[3]),bytes(raw[4]),bytes(raw[5]),bytes(quantities),bytes(raw[7]),bytes(raw[8])};
 ItemPowerTablesV5 definitions;ck(definitions.load(power_input.powers,power_input.power_names,power_input.power_schema,error),error);
 LootPowerResourcesV7 resources;ck(resources.load(power_input,definitions.borrow(),error),error);auto resource_view=resources.borrow();
 LootTablesV2 tables;ck(tables.load(bytes(raw[6]),power_input.loot_names,power_input.loot_schema,error),error);auto loot_view=tables.borrow();
 ck(loot_id>=0&&std::size_t(loot_id)<loot_view.loots().size()&&
    (!loot_view.loots()[loot_id].random_entries.empty()||!loot_view.loots()[loot_id].fixed_entries.empty()||!loot_view.loots()[loot_id].sub_loots.empty()),
    "fixture no longer has source LootTable entries");
 if(loot_id==31)ck(loot_view.loots()[31].random_entries.empty()&&loot_view.loots()[31].sub_loots.size()==1&&
                   loot_view.loots()[31].sub_loots[0]==48&&loot_view.loots()[48].random_entries.size()==1,
                   "nested fixture no longer exercises a root-pooled child random row");
 Random random(seed,calls);auto rng=random.service();LootPowerCreationV7 creation(resource_view,rng);ItemPresentationOwnerV5 presentation(definitions.borrow());Services services{&loot_view.items(),&presentation};
 PropertyState properties;FreshInventoryOwnedV4 inventory(0x100000007,loot_view,rng,capacity,properties);OwnedInventoryServicesV4 inventory_services{&services,Services::inventory,Services::observe};
 OwnedLootEffectsV7 effects{&creation,definitions.borrow(),services.text_service(),value_bonus,power_bonus,requested,difficulty};LootEntrySelectionContextV1 selection{};
 std::unique_ptr<ItemInstanceV1> pending;ck(inventory.add_loot_table(loot_id,selection,RetainedItemSlotV4{&pending},inventory_services,effects,error),error);
 if(pending||inventory.properties()!=&properties)throw std::runtime_error("random AddLoot publication mismatch: pending="+std::to_string(bool(pending))+", items="+std::to_string(inventory.items().size())+", rng_draws="+std::to_string(random.draws)+", seed="+std::to_string(random.state.seeds[0])+", queries="+std::to_string(services.queries));++checks;
 ck(services.queries>0,"random AddLoot did not reuse its shared Debug provider");
 Reader expected{source_snapshot};auto compare=[&](std::uint32_t a,std::uint32_t b,const char* label){ck(a==b,std::string("source/selected ")+label+" differs");};
 compare(expected.u(),std::uint32_t(inventory.gold()),"gold");compare(expected.u(),std::uint32_t(inventory.current_equipment()),"equipment mode");const auto item_count=expected.u();compare(item_count,std::uint32_t(inventory.items().size()),"item count");if(item_count!=inventory.items().size())throw std::runtime_error("source/selected item count differs");
 for(std::uint32_t i=0;i<item_count;++i){const auto& slot=*inventory.items()[i];const auto& item_instance=*slot.item;
  compare(expected.u(),std::uint32_t(item_instance.id),"item id");compare(expected.u(),item_instance.quantity,"quantity");compare(expected.u(),std::uint32_t(item_instance.value),"item value");
  compare(expected.u(),item_instance.identified,"identified state");compare(expected.u(),std::uint8_t(slot.slots[0]),"left equipment slot");compare(expected.u(),std::uint8_t(slot.slots[1]),"right equipment slot");
  const auto power_count=expected.u();compare(power_count,std::uint32_t(item_instance.powers.size()),"power count");for(std::uint32_t j=0;j<power_count;++j)compare(expected.u(),std::uint32_t(item_instance.powers[j]),"power id");}
 std::uint32_t potion=UINT32_MAX;for(std::uint32_t i=0;i<inventory.items().size();++i)if(inventory.items()[i]->item.get()==inventory.potion())potion=i;compare(expected.u(),potion,"potion index");
 for(unsigned set=0;set<2;++set)for(unsigned slot=0;slot<9;++slot){std::uint32_t index=UINT32_MAX;auto* equipped=inventory.equipment()[set][slot];for(std::uint32_t i=0;i<inventory.items().size();++i)if(equipped==inventory.items()[i].get())index=i;compare(expected.u(),index,"equipment mapping");}
 ck(expected.at==source_snapshot.size(),"trailing original inventory snapshot");ck(random.state.seeds[0]==seed_after&&random.state.counters[0]==calls_after,"source/selected RNG state differs");ck(random.state.seeds[1]==0x98765432u&&random.state.counters[1]==37,"other source RNG stream changed");
 for(const auto& slot:inventory.items())ck(presentation.forget(*slot->item,error),error);
 Random drop_random(seed,calls);auto drop_rng=drop_random.service();LootPowerCreationV7 drop_creation(resource_view,drop_rng);ItemPresentationOwnerV5 drop_presentation(definitions.borrow());Services drop_services{&loot_view.items(),&drop_presentation};
 PropertyState drop_properties;FreshInventoryOwnedV4 drop_inventory(0x100000008,loot_view,drop_rng,capacity,drop_properties);OwnedInventoryServicesV4 drop_inventory_services{&drop_services,Services::inventory,Services::observe};OwnedLootEffectsV7 drop_effects{&drop_creation,definitions.borrow(),drop_services.text_service(),value_bonus,power_bonus,requested,difficulty};
 std::unique_ptr<ItemInstanceV1> drop_pending;ck(drop_inventory.add_world_loot_table(loot_id,selection,RetainedItemSlotV4{&drop_pending},drop_inventory_services,drop_effects,error),error);
 ck(!drop_pending&&drop_inventory.items().empty()&&drop_inventory.world_items().size()==item_count&&item_count>0,"source DropLoot published items into the player inventory or lost the drop prefix");
  auto* world_item=drop_inventory.world_items()[0]->item.get();auto* world_row=item(drop_inventory.table(),world_item->id);ck(world_row!=nullptr,"world item metadata missing before pickup");const auto world_value=world_item->value;const auto gold_before_pickup=drop_inventory.gold();const auto gold_notifications_before=drop_services.gold_notifications;const bool gold_drop=item_type(*world_row)==13;const auto queries_before_pickup=drop_services.queries;std::int32_t inventory_index=-1;
  ck(drop_inventory.pickup_world_item(0,inventory_index,drop_inventory_services,error),error);
  const auto expected_pickup_queries=gold_drop?0u:2u+(world_row->record.words[26]!=-1?2u:0u);ck(drop_services.queries-queries_before_pickup==expected_pickup_queries,"pickup queried inventory capacity outside the source equipment-item branch");
  ck(drop_inventory.world_items().size()+1==item_count,"ItemObject::Interact did not retire exactly one world item");
  if(gold_drop){ck(inventory_index==-1&&drop_inventory.items().empty()&&drop_inventory.gold()==gold_before_pickup+world_value&&drop_services.gold_notifications==gold_notifications_before+1,"gold pickup did not add its source value and retire the Item");}
  else ck(inventory_index>=0&&std::size_t(inventory_index)<drop_inventory.items().size()&&drop_inventory.items()[inventory_index]->item.get()==world_item,"ItemObject::Interact pickup did not transfer the same Item identity into V4");
  while(!drop_inventory.world_items().empty())ck(drop_inventory.retire_world_item(0,drop_inventory_services,error),error);
  for(const auto& slot:drop_inventory.items())ck(drop_presentation.forget(*slot->item,error),error);
 std::cout<<"{\"validation\":\"PASS\",\"source_host_parity\":true,\"loot_table_row\":"<<loot_id<<",\"item_count\":"<<item_count<<",\"gold_world_pickup\":"<<(gold_drop?"true":"false")<<",\"source_rng_after\":["<<seed_after<<","<<calls_after<<"],\"host_rng_after\":["<<random.state.seeds[0]<<","<<random.state.counters[0]<<"],\"world_drop_staging_and_pickup\":true,\"source_AddPower_body_executed\":false,\"controlled_services\":[\"DebugSwitches\",\"ItemInstance::AddPower\",\"Item text\",\"ItemInventory effects\"],\"full_AddLoot\":false,\"android_gameplay\":false,\"checks\":"<<checks<<"}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
