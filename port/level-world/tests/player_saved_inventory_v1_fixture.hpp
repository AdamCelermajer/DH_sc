// Scoped test-only reuse of the actual-table/same-owner equipment fixture.
#define main existing_inventory_text_main
#include "../../engine-ui/tests/inventory_binding_text_queries_v1.cpp"
#undef main
#include "../player_initial_equipment_v1.hpp"
#include "../../game-data/player_equipment_live_services_v1.hpp"
#include <array>
#include <algorithm>
namespace initial=dh2::player_initial_equipment_v1;
using Trace=std::array<std::uint32_t,3>;
constexpr std::uintptr_t CHARACTER=0x100000001ull,RECORD=0x200000001ull;
static std::set<std::string> text_files;
struct Gold {std::array<std::uint32_t,7> in{};std::array<std::uint32_t,13> out{};std::vector<Trace> backend;};
template<class T>T read_gold(std::istream& f){T v{};f.read(reinterpret_cast<char*>(&v),sizeof(v));ck(bool(f),"truncated original equipment capture");return v;}
struct Tables {
 data::LootTablesV2 loot;data::ItemPowerTablesV5 powers;data::CharacterTable chars;
 data::ClassTables classes;data::PropertyRules rules;std::vector<data::ClassRow> rows;ui::HudTextV1 text;
 explicit Tables(const std::filesystem::path& cache){std::string e;
  auto table=[&](const char* name,auto load){auto a=file(cache/(std::string(name)+"_pyarray.bin")),b=file(cache/(std::string(name)+"_pyarraynames.bin")),c=file(cache/(std::string(name)+"_pystructnames.bin"));ck(load(a,b,c,e),e);};
  table("loot_table",[&](auto& a,auto& b,auto& c,auto& e){return loot.load(span(a),span(b),span(c),e);});
  table("item_powers",[&](auto& a,auto& b,auto& c,auto& e){return powers.load(span(a),span(b),span(c),e);});
  table("character_properties",[&](auto& a,auto& b,auto& c,auto& e){return data::load_characters(span(a),span(b),span(c),chars,e);});
  table("character_classes",[&](auto& a,auto& b,auto& c,auto& e){return data::load_classes(span(a),span(b),span(c),classes,e);});
  table("common_text",[&](auto& a,auto& b,auto& c,auto& e){return text.load({a.data(),a.size()},{b.data(),b.size()},{c.data(),c.size()},e);});
  ck(data::load_property_rules(chars,rules,e)&&text.switch_pack(0,false,e),e);
  for(auto& row:classes.rows)rows.push_back({row.data(),std::uint32_t(row.size())});
 }
};
struct World {
 Tables& tables;Context text;data::PropertyState properties{};data::PropertyView view{};
 dh2_random_state random{{991,887},{0,31}};
 std::unique_ptr<data::FreshInventoryOwnedV4> inventory;
 std::unique_ptr<ui::ItemTextOwnerV5> item_text;
 std::unique_ptr<data::ItemPresentationOwnerV5> presentation;
 data::EquipmentLiveHooksV1 hooks{};std::unique_ptr<data::PlayerEquipmentLiveServicesV1> equipment;
 data::OwnedInventoryServicesV4 delegate{},effects{};std::unique_ptr<initial::Runtime> runtime;
 Gold gold{};std::vector<Trace> trace;std::vector<unsigned> order;
 std::uintptr_t visual=0;std::uint32_t online=0;std::uint8_t record_byte=1;
 bool real_loot=false,real_text=false,attached=true,empty_record=false,throw_failure=false,reenter=false;
 unsigned calls=0,fail_call=0,fail_text=0,text_calls=0,fail_gear=0,gear_calls=0;
 unsigned updates=0,skin_calls=0,vitals=0,retirements=0;data::ItemInstanceV1* failed_item=nullptr;
 std::int32_t yes_item=-1,no_item=-1;
 World(Tables& t,const std::filesystem::path& cache,const char* name):tables(t){
  const auto at=std::find(t.chars.names.begin(),t.chars.names.end(),name);ck(at!=t.chars.names.end(),"actual class missing");
  data::reset_properties(t.rules,properties,&t.chars.rows.at(at-t.chars.names.begin()));view=data::property_view(t.rules,properties);
  ck(!dh2_class_recalc_base(t.rows.data(),t.rows.size(),properties.base.data(),&view),"actual class base calculation failed");
  inventory=std::make_unique<data::FreshInventoryOwnedV4>(CHARACTER,t.loot.borrow(),data::InventoryRandomServiceV4{&random,Context::random},-1,properties);
  text.root=cache.parent_path();text.constants_bytes=file(cache/"common_text_pycst.bin");text.colors_bytes=file(cache/"fonts_pycst.bin");
  ck(!dh2_pycst_open(&text.constants,text.constants_bytes.data(),text.constants_bytes.size())&&!dh2_pycst_open(&text.colors,text.colors_bytes.data(),text.colors_bytes.size()));
  item_text=std::make_unique<ui::ItemTextOwnerV5>(inventory->table(),t.chars,t.text,text.environment());text.item_text=item_text.get();
  presentation=std::make_unique<data::ItemPresentationOwnerV5>(t.powers.borrow());
  hooks={this,binding,{this,world_query},&visual,{this,skin},{this,required,observe}};
  equipment=std::make_unique<data::PlayerEquipmentLiveServicesV1>(*inventory,view,t.rows.data(),t.rows.size(),t.powers.borrow(),hooks);
  delegate=equipment->services();effects={this,effect,observed,true};
  runtime=std::make_unique<initial::Runtime>(initial::Bindings{CHARACTER,inventory.get(),&view,&effects,{this,backend}});
  for(unsigned id=0;id<inventory->table().rows.size();++id){const auto& row=inventory->table().rows[id];if(yes_item<0&&row.record.words[26]==1&&!std::uint8_t(row.record.words[7]))yes_item=id;
   if(no_item<0&&row.record.words[26]==-1&&data::item_type(row)!=13&&!std::uint8_t(row.record.words[7]))no_item=id;}
  ck(yes_item>=0&&no_item>=0,"real oracle metadata missing");
 }
 ~World(){text_files.insert(text.reached_files.begin(),text.reached_files.end());for(const auto& slot:inventory->items()){std::string e;ck(presentation->forget(*slot->item,e),e);}runtime.reset();equipment.reset();inventory.reset();}
 void probe(){if(!reenter)return;reenter=false;initial::Result out{};out.backend_calls=99;auto before=out;std::string e="sentinel";ck(runtime->initialize(&out,e)==initial::Status::busy&&!std::memcmp(&out,&before,sizeof out)&&e=="sentinel","initial equipment reentry changed output");}
 bool reject(){++calls;if(calls!=fail_call)return false;if(throw_failure)throw std::runtime_error("explicit equipment provider exception");return true;}
 static bool binding(void* raw,data::FreshInventoryOwnedV4& i,data::PropertyView& v,std::string& e){auto& s=*static_cast<World*>(raw);ck(&i==s.inventory.get()&&&v==&s.view,"equipment attachment changed authority");s.probe();if(!s.attached){e="explicit detached source visual";return false;}return true;}
 static bool world_query(void* raw,data::EquipmentWorldQueryV1 op,std::uintptr_t char_id,std::uintptr_t& id,std::int32_t& value,std::string&){auto& s=*static_cast<World*>(raw);ck(char_id==CHARACTER,"equipment Character identity truncated");id=0;value=0;
  if(op==data::EquipmentWorldQueryV1::online)value=s.online;
  if(op==data::EquipmentWorldQueryV1::online_player_record)id=RECORD;
  if(op==data::EquipmentWorldQueryV1::current_player)id=CHARACTER;
  if(op==data::EquipmentWorldQueryV1::player_count)value=1;
  return true;
 }
 static bool skin(void*,data::FreshInventoryOwnedV4&,const data::GearSkinRequestV5&,std::int32_t&,std::string& e){e="nonnull source Skin factory is not supplied by this bounded host fixture";return false;}
 static bool required(void* raw,data::FreshInventoryOwnedV4& i,const data::OwnedInventoryRequestV4& q,data::OwnedInventoryResponseV4& r,std::string& e){auto& s=*static_cast<World*>(raw);ck(&i==s.inventory.get(),"required Item owner differs");s.probe();using O=data::OwnedInventoryOperationV4;
  if(q.operation==O::update_name||q.operation==O::update_stats||q.operation==O::update_requirements){++s.text_calls;if(s.text_calls==s.fail_text){s.failed_item=q.item;e="explicit actual Item text failure";return false;}
   if(!s.real_text)return true;
   return Context::effect(&s.text,i,q,r,e);}
  if(q.operation==O::debug_load||q.operation==O::debug_query){r.value=0;return true;} // Declared Debug transport fixture.
  if(q.operation==O::inventory_full){r.value=0;return true;} // Exact bounded source external notification fixture.
  if(q.operation==O::gold_notifications)return true;
  e="required original equipment continuation absent";return false;
 }
 static void observe(void* raw,data::FreshInventoryOwnedV4& i,const data::OwnedInventoryRequestV4& q){auto& s=*static_cast<World*>(raw);ck(&i==s.inventory.get(),"Item retirement owner differs");if(q.operation==data::OwnedInventoryOperationV4::destroy_item){std::string e;ck(q.item&&s.presentation->forget(*q.item,e),e);++s.retirements;}}
 static bool effect(void* raw,data::FreshInventoryOwnedV4& i,const data::OwnedInventoryRequestV4& q,data::OwnedInventoryResponseV4& r,std::string& e){auto& s=*static_cast<World*>(raw);using O=data::OwnedInventoryOperationV4;
  if(q.operation==O::update_gear_properties){++s.updates;s.order.push_back(1);}
  if(q.operation==O::skin){++s.skin_calls;s.order.push_back(2);}
  if(q.operation==O::validate_hp_mp){++s.vitals;s.order.push_back(3);}
  if(q.operation==O::update_gear_properties||q.operation==O::skin||q.operation==O::validate_hp_mp){if(++s.gear_calls==s.fail_gear){e="explicit equipment effect rejection";return false;}}
  return s.delegate.invoke(s.delegate.context,i,q,r,e);
 }
 static void observed(void* raw,data::FreshInventoryOwnedV4& i,const data::OwnedInventoryRequestV4& q){auto& s=*static_cast<World*>(raw);s.delegate.observe_storage(s.delegate.context,i,q);}
 void insert(std::int32_t id){std::unique_ptr<data::ItemInstanceV1> item;std::string e;ck(inventory->create_item(id,1,item,effects,e),e);std::int32_t index=-1;ck(inventory->add_item(item,true,false,index,effects,e)&&!item,e);}
 static int backend(void* raw,const initial::Request* q,initial::Reply* r,std::string& e){auto& s=*static_cast<World*>(raw);
  ck(q->character==CHARACTER&&q->inventory==s.inventory.get()&&q->properties==&s.view&&q->equipment_services==&s.effects,"initial equipment authority differs");s.probe();
  if(s.reject()){e="explicit initial equipment provider rejection";return 1;}
  switch(q->operation){
  case initial::Operation::online:r->word=s.online;s.trace.push_back({0,0,s.online});return 0;
  case initial::Operation::online_player_record:ck(q->arguments[0]==0,"online selector changed");r->record=s.empty_record?0:RECORD;r->record_byte66c=s.record_byte;s.trace.push_back({1,0,s.record_byte});return 0;
  case initial::Operation::add_loot:
   ck(std::array<std::int32_t,4>{q->arguments[1],q->arguments[2],q->arguments[3],q->arguments[4]}==std::array<std::int32_t,4>{0,0,-1,0},"full AddLoot arguments changed");
   s.trace.push_back({5,std::uint32_t(q->arguments[0]),0});
   if(s.real_loot)return s.inventory->add_fixed_loot(q->arguments[0],s.effects,e)?0:1;
   for(unsigned i=0;i<s.gold.in[5];++i)s.insert((s.gold.in[6]>>i)&1?s.yes_item:s.no_item);
   return 0;
  }
  return 1;
 }
 void oracle(const Gold& g){gold=g;online=g.in[0];record_byte=std::uint8_t(g.in[1]);for(unsigned i=0;i<g.in[2];++i)insert(yes_item);std::string e;if(g.in[3])ck(inventory->set_gold(g.in[3],effects,e),e);
  // Intentionally inconsistent base/saved/gear/cache. _GetProperty reads only
  // cached source Loot and must not resolve/recalculate before AddLoot.
  properties.base[9]=777;properties.saved[9]=888;properties.gear[9]=999;properties.resolved[9]=std::int32_t(g.in[4]);
  trace.clear();calls=text_calls=updates=skin_calls=vitals=gear_calls=0;order.clear();
 }
 std::array<std::uint32_t,13> output(const initial::Result& r)const{return {std::uint32_t(r.decision),r.online_queries,r.record_queries,r.item_count_reads,r.gold_reads,r.loot_property_reads,r.add_loot_calls,r.equippable_queries,r.auto_equip_calls,r.skin_calls,r.captured_items,r.last_index,std::uint32_t(r.loot)};}
};
