#define main old_table_main
#include "../../game-data/tests/loot_table_selection_v8.cpp"
#undef main
#include "../character_loot_item_manager_v8.hpp"
#include <list>
using namespace dh2::character;
struct Fixture {
 struct Object {std::uintptr_t id{};std::int16_t category{-1};std::uint8_t enabled{1};bool visible{};float position[3]{},destination[3]{};std::unique_ptr<ItemInstanceV1> item;};
 std::list<Object> objects;std::vector<unsigned> operations;bool fail_enable{};unsigned reuse{},removed{};
 static bool entry(void*,const LootEntryRequestV8& q,int& out,std::string&){out=0;return q.operation==LootEntryOperationV8::debug_load||(q.operation==LootEntryOperationV8::debug_query&&!std::strcmp(q.key,"InfiniteInventory"));}
 static bool spawn(void* p,const char* type,const char* name,bool first,bool second,LootItemObjectBorrowV8& out,std::string&){auto& f=*static_cast<Fixture*>(p);check(!std::strcmp(type,"Item")&&!first&&second);unsigned c,n;check(std::sscanf(name,"ItemObject_%u_%u",&c,&n)==2);f.objects.emplace_back();auto& o=f.objects.back();o.id=0x200000001+f.objects.size();out={o.id,3,&o.category,&o.enabled};return true;}
 static bool invoke(void* p,const LootItemRequestV8& q,std::string& e){auto& f=*static_cast<Fixture*>(p);f.operations.push_back(unsigned(q.operation));Object* o{};for(auto& row:f.objects)if(row.id==q.object->identity)o=&row;check(o!=nullptr);
  switch(q.operation){case LootItemOperationV8::init_once:check(q.audiovisual&&!q.audiovisual->visual.empty());o->category=std::int16_t(q.index);return true;
   case LootItemOperationV8::enable:if(f.fail_enable){e="Declared reached Enable fixture failure";return false;}o->visible=q.flag;return true;
   case LootItemOperationV8::remove_all:check(q.flag);if(o->item){++f.removed;o->item.reset();}return true;
   case LootItemOperationV8::physical:check(!q.flag);return true;
   case LootItemOperationV8::position:check(q.flag&&q.vector);std::copy(q.vector,q.vector+3,o->position);return true;
   case LootItemOperationV8::destination:check(q.vector);std::copy(q.vector,q.vector+3,o->destination);return true;
   case LootItemOperationV8::init_again:check(q.inventory&&q.index<q.inventory->items().size()&&q.audiovisual);++f.reuse;return q.inventory->release_spawned(*q.inventory->items()[q.index]->item,o->item,e);
  }return false;
 }
};
int main(int argc,char** argv){try{check(argc==3);std::string error,cache=argv[1],avcache=argv[2];auto b=file(cache+"/loot_table_pyarray.bin"),n=file(cache+"/loot_table_pyarraynames.bin"),s=file(cache+"/loot_table_pystructnames.bin");LootTablesV2 table;check(table.load(span(b),span(n),span(s),error),error);auto ab=file(avcache+"/loot_audiovisual_pyarray.bin"),an=file(avcache+"/loot_audiovisual_pyarraynames.bin"),as=file(avcache+"/loot_audiovisual_pystructnames.bin");LootAudioVisualV8 audiovisual;check(audiovisual.load(span(ab),span(an),span(as),error),error);check(audiovisual.borrow().rows().size()==29);
 auto av_gold=file("port/game-data/reference/loot-audiovisual-v8/fixtures.bin");Reader ar{av_gold};check(ar.block(4)==Raw({'L','A','V','8'}));check(ar.u()==29);for(auto& row:audiovisual.borrow().rows()){check(ar.u()==unsigned(row.audio_drop));check(ar.u()==unsigned(row.audio_pickup));auto bytes=ar.block(ar.u());check(std::string(bytes.begin(),bytes.end())==row.visual);}check(ar.at==av_gold.size());
 Fixture f;CharacterLootItemManagerV8 manager(audiovisual.borrow(),{&f,Fixture::spawn,Fixture::invoke});check(manager.precache(error),error);check(f.objects.size()==145);check(!manager.precache(error));for(auto& o:f.objects)check(!o.visible&&o.enabled==0&&o.category>=0);
 unsigned spawned{};float position[]={1,2,3},destination[]={5,6,7};
 for(unsigned category=0;category<29;++category){int id=-1;for(unsigned i=0;i<table.borrow().items().rows.size();++i)if(table.borrow().items().rows[i].record.words[21]==int(category)){id=int(i);break;}if(id<0)continue;
  for(unsigned j=0;j<7;++j){LootTemporaryInventoryV8 inventory(table.borrow());auto item=std::make_unique<ItemInstanceV1>();item->id=id;item->quantity=1;check(inventory.store(item,{nullptr,Fixture::entry},nullptr,nullptr,error),error);LootItemObjectBorrowV8 out;check(manager.spawn(inventory,0,0x100000005,position,destination,0x100000001,out,error),error);check(out.identity&&inventory.items().empty()&&*out.enabled85==1);++spawned;}
 }
 check(f.reuse==spawned&&f.removed==spawned/7*2);check(manager.despawn(f.objects.front().id,error));check(f.objects.front().enabled==0);check(manager.flush(error));check(!manager.ready());
 std::cout<<"{\"validation\":\"PASS\",\"actual_audiovisual_categories\":29,\"retained_object_fixture_count\":145,\"spawn_lifecycle_fixture_cases\":"<<spawned<<",\"reused_item_removals\":"<<f.removed<<",\"checks\":"<<checks<<",\"world_scene_audio_body_provider_fixture\":true,\"production_world_spawn\":false}\n";return 0;
 }catch(const std::exception& e){std::cerr<<"check "<<checks<<" "<<e.what()<<'\n';return 1;}}
