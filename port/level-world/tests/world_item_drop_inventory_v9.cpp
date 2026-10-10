#include "world_item_drop_inventory_v9.hpp"
#include "world_loot_item_runtime_v1.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2;using namespace dh2::character;
namespace {unsigned checks{};void check(bool v){++checks;if(!v)throw std::runtime_error("source drop check "+std::to_string(checks));}
std::vector<std::uint8_t> file(const std::string& p){std::ifstream f(p,std::ios::binary);check(bool(f));return {std::istreambuf_iterator<char>(f),{}};}
data::Bytes span(const std::vector<std::uint8_t>& b){return {b.data(),b.size()};}
struct Fixture {
 data::LootTablesV2::Borrow tables;data::LootAudioVisualV8::Borrow av;std::shared_ptr<void> lease;
 std::map<std::uintptr_t,std::shared_ptr<RetainedWorldItemObjectV1>> objects;
 std::uintptr_t next{0x2000};std::vector<std::string> order;bool fail_sound{};unsigned init_once{},transfers{};const float origin[3]{100,200,250};
 static bool inventory_debug(void*,const data::LootEntryRequestV8& q,std::int32_t& out,std::string&){check(q.operation==data::LootEntryOperationV8::debug_load||q.operation==data::LootEntryOperationV8::debug_query);out=0;return true;}
 static bool operation(void* raw,const WorldItemRequestV1& q,std::int32_t&,std::string& e){auto& f=*static_cast<Fixture*>(raw);auto& item=*f.objects.at(q.object);
  switch(q.operation){
   case WorldItemOperationV1::game_init_post:++f.init_once;return true; // Declared absent visual InitPost fixture.
   case WorldItemOperationV1::set_visible:return item.base().store_byte(0x80,q.flag?item.base().lifecycle().enabled8a:0,e); // Declared source virtual40 fixture; manager owns byte85.
   case WorldItemOperationV1::remove_all:return item.inventory().remove_all_owned_v2(nullptr,nullptr,e);
   case WorldItemOperationV1::set_physical:return true; // Declared body endpoint.
   case WorldItemOperationV1::set_position:std::copy_n(q.position,3,item.base().vector3(0x160));std::copy_n(q.position,3,item.base().vector3(0x1a8));f.order.push_back("position");return true;
   case WorldItemOperationV1::set_destination:std::copy_n(q.position,3,item.base().vector3(0x1a8));f.order.push_back("destination");return true;
   case WorldItemOperationV1::drop_sound:f.order.push_back("sound");++f.transfers;if(f.fail_sound){e="declared audio delivery failure";return false;}return true;
   case WorldItemOperationV1::create_decor_physical:f.order.push_back("body");return true; // Declared body allocation boundary.
   default:e="Unexpected source Item endpoint";return false;
  }
 }
 static bool visual(void* raw,std::uintptr_t id,bool& present,std::string&){auto& f=*static_cast<Fixture*>(raw);present=*f.objects.at(id)->base().pointer(0x2d8)!=0;return true;}
 static bool sound_position(void* raw,std::uintptr_t id,const float*& out,std::string&){out=static_cast<Fixture*>(raw)->objects.at(id)->base().vector3(0x1a8);return true;}
 static bool spawn(void* raw,const char* type,const char* name,bool first,bool second,std::shared_ptr<RetainedWorldItemObjectV1>& out,std::string&){auto& f=*static_cast<Fixture*>(raw);check(std::string(type)=="Item"&&std::string(name).find("ItemObject_")==0&&!first&&second);
  WorldItemServicesV1 s;s.context=&f;s.invoke=operation;s.visual_present=visual;s.sound_position1a8=sound_position;s.inventory_debug={&f,inventory_debug};
  out=std::make_shared<RetainedWorldItemObjectV1>(f.next++,f.lease,f.tables,f.av,s);f.objects.emplace(out->base().identity(),out);return true; // Explicit canonical factory delivery fixture.
 }
};
bool owned_effect(void*,data::FreshInventoryOwnedV4&,const data::OwnedInventoryRequestV4& q,data::OwnedInventoryResponseV4& out,std::string& e){if(q.operation!=data::OwnedInventoryOperationV4::debug_load&&q.operation!=data::OwnedInventoryOperationV4::debug_query){e="Unexpected owned source item effect";return false;}out.value=0;return true;}
}
int main(int argc,char** argv){try{
 check(argc==3);std::string e;data::LootTablesV2 tables;data::LootAudioVisualV8 av;
 auto b=file(std::string(argv[1])+"/loot_table_pyarray.bin"),n=file(std::string(argv[1])+"/loot_table_pyarraynames.bin"),s=file(std::string(argv[1])+"/loot_table_pystructnames.bin");check(tables.load(span(b),span(n),span(s),e));
 b=file(std::string(argv[2])+"/loot_audiovisual_pyarray.bin");n=file(std::string(argv[2])+"/loot_audiovisual_pyarraynames.bin");s=file(std::string(argv[2])+"/loot_audiovisual_pystructnames.bin");check(av.load(span(b),span(n),span(s),e));
 Fixture f;f.tables=tables.borrow();f.av=av.borrow();f.lease=std::make_shared<int>(1);
 WorldLootFactoryServicesV1 factories;factories.context=&f;factories.spawn=Fixture::spawn;
 factories.drop.context=&f;factories.drop.position=[](void* p,auto id,const float*& out,auto&){check(id==0x1000);auto& f=*static_cast<Fixture*>(p);f.order.push_back("source-position");out=f.origin;return true;};
 factories.drop.random_drop_position=[](void* p,auto source,auto second,float out[3],auto&){check(source==0x1000&&second==0x1000);auto& f=*static_cast<Fixture*>(p);f.order.push_back("scatter");out[0]=200;out[1]=300;out[2]=250;return true;}; // Explicit scatter provider fixture.
 WorldLootItemRuntimeV1 runtime(av.borrow(),factories);check(runtime.precache(e)&&f.objects.size()==145&&f.init_once==145);
 auto properties=std::make_shared<data::PropertyState>();data::LootRandom8V2 random{1,0};data::OwnedInventoryServicesV4 effects{nullptr,owned_effect,nullptr};
 std::int32_t id=-1;const auto& rows=tables.borrow().items().rows;for(std::size_t i=0;i<rows.size();++i){const auto& row=rows[i];if(row.record.words[21]>=0&&row.record.words[21]<29&&data::item_type(row)!=13&&data::item_type(row)!=14){id=static_cast<std::int32_t>(i);break;}}check(id>=0);
 auto make=[&](){auto inv=data::FreshInventoryOwnedV4::create_drop_temporary_v4(tables.borrow(),random,properties);auto item=std::make_unique<data::ItemInstanceV1>();item->id=id;item->quantity=1;std::int32_t index{};check(inv->add_item(item,true,false,index,effects,e)&&!item&&index==0);return inv;};
 WorldItemDropInventoryServicesV9 drop;drop.context=&f;
 drop.is_character=[](void* raw,auto source,bool& out,auto&){check(source==0x1000);static_cast<Fixture*>(raw)->order.push_back("is-character");out=true;return true;};
 drop.friendly_index=[](void* raw,auto source,auto& out,auto&){check(source==0x1000);static_cast<Fixture*>(raw)->order.push_back("friendly");out=2;return true;}; // Explicit actual PlayerInfo projection fixture.
 auto inventory=make();auto* actual=inventory->items()[0]->item.get();LootInventorySourceV9 borrow;check(fresh_inventory_loot_source_v9(*inventory,f.lease,borrow,e));f.order.clear();
 check(runtime.drop_inventory_source_v9(borrow,0x1000,0x1000,0,drop,e)&&inventory->items().empty());
 RetainedWorldItemObjectV1* same{};for(auto& entry:f.objects)if(entry.second->inventory().peek()==actual)same=entry.second.get();
 check(same&&same->fields().lock3b8==5000&&same->fields().player_id3c0==2&&same->fields().owner3bc==0);
 check(f.order==std::vector<std::string>({"source-position","scatter","position","destination","sound","body","is-character","friendly"}));
 check(same->inventory().peek()==actual&&same->base().type_f4()==3&&runtime.manager().ready());
 auto failed=make();auto* failed_item=failed->items()[0]->item.get();check(fresh_inventory_loot_source_v9(*failed,f.lease,borrow,e));f.fail_sound=true;e.clear();check(!runtime.drop_inventory_source_v9(borrow,0x1000,0x1000,0,drop,e)&&e=="declared audio delivery failure"&&failed->items().empty());
 unsigned retained{};for(auto& entry:f.objects)retained+=entry.second->inventory().peek()==failed_item;check(retained==1);
 std::cout<<"PASS SAME145 manager Fresh NULL inventory source transfer (no clone), original DropInventory order/5000 lock/friendly678, actual item identity and audio failure mutation prefix; factory/visual/audio/body/scatter/PlayerInfo declared fixtures checks="<<checks<<"\n";
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
