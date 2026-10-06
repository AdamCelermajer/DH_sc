#define main old_table_main
#include "../../game-data/tests/loot_table_selection_v8.cpp"
#undef main
#include "../canonical_item_factory_v2.hpp"
#include "../canonical_point3d_globals_v1.hpp"
using namespace dh2::world;
struct ItemFactoryFixture {
 CanonicalObjectManagerV1 manager;
 CanonicalPropertyMapV1 map{{nullptr,&canonical_vec3_origin_v1(),nullptr}};
 CanonicalItemFactoryV2* factory{};bool fail_condition{};
 unsigned conditions{},network_calls{},discarded{},item_callbacks{};
 ItemFactoryFixture():manager({this,nullptr,nullptr,nullptr,destroy,network,nullptr}){}
 static bool destroy(void* p,CanonicalObjectBorrowV1& object,std::string&){auto& t=*static_cast<ItemFactoryFixture*>(p);t.factory->erased(object.identity);++t.discarded;return true;}
 static bool network(void* p,CanonicalObjectBorrowV1&,std::string&){++static_cast<ItemFactoryFixture*>(p)->network_calls;return true;}
 static bool resolve(void* p,dh2::target_providers::Handle16& handle,bool,const CanonicalObjectBorrowV1*& out,std::string&){out=static_cast<ItemFactoryFixture*>(p)->manager.object(handle.key);return true;}
 static bool condition(void* p,const CanonicalObjectBorrowV1& object,bool tested,std::string& error){auto& t=*static_cast<ItemFactoryFixture*>(p);++t.conditions;check(tested&&t.manager.object(object.shared_handle->key)->identity==object.identity);if(t.fail_condition){error="Explicit required condition fixture";return false;}return true;}
 static bool debug(void*,const char*,std::string&){return true;}
 static bool item(void* p,const dh2::character::WorldItemRequestV1&,std::int32_t&,std::string&){++static_cast<ItemFactoryFixture*>(p)->item_callbacks;return false;}
};
#ifndef DH2_ITEM_FACTORY_FIXTURE_ONLY
int main(int argc,char** argv){try{
 check(argc==3);const std::string cache=argv[1],avcache=argv[2];std::string error;
 auto b=file(cache+"/loot_table_pyarray.bin"),n=file(cache+"/loot_table_pyarraynames.bin"),s=file(cache+"/loot_table_pystructnames.bin");LootTablesV2 tables;check(tables.load(span(b),span(n),span(s),error),error);
 auto ab=file(avcache+"/loot_audiovisual_pyarray.bin"),an=file(avcache+"/loot_audiovisual_pyarraynames.bin"),as=file(avcache+"/loot_audiovisual_pystructnames.bin");LootAudioVisualV8 av;check(av.load(span(ab),span(an),span(as),error),error);
 ItemFactoryFixture t;auto pin=std::make_shared<int>(1);
 CanonicalItemFactoryServicesV2 services{&t,ItemFactoryFixture::resolve,ItemFactoryFixture::condition,ItemFactoryFixture::debug,{&t,ItemFactoryFixture::item,{},{},nullptr,nullptr}};
 CanonicalItemFactoryV2 owner(t.manager,t.map,pin,tables.borrow(),av.borrow(),services);t.factory=&owner;
 std::shared_ptr<dh2::character::RetainedWorldItemObjectV1> item;
 check(owner.spawn("Item","NativeItem",false,true,item,error),error);check(bool(item));
 auto canonical=item->canonical(item);check(canonical.identity==reinterpret_cast<std::uintptr_t>(item.get()));
 check(t.manager.object(canonical.shared_handle->key)->identity==canonical.identity);
 check(t.conditions==1&&t.network_calls==1&&t.item_callbacks==0);
 check(t.manager.pending()==std::vector<std::uintptr_t>{canonical.identity});
 check(item->inventory().items().empty()&&item->fields().category3ac==-1);
 check(*item->base().byte(0x85)==1&&*item->base().byte(0x2ee)==0);
 std::shared_ptr<dh2::character::RetainedWorldItemObjectV1> duplicate;
 check(owner.spawn("Item","NativeItem",false,true,duplicate,error),error);check(duplicate==item&&t.discarded==1&&t.network_calls==1);
 check(t.manager.pending()==std::vector<std::uintptr_t>({canonical.identity,canonical.identity}));
 t.fail_condition=true;std::shared_ptr<dh2::character::RetainedWorldItemObjectV1> failed;
 check(!owner.spawn("Item","FailedItem",false,true,failed,error)&&!failed);
 dh2::target_providers::Handle16 failed_handle{};check(t.manager.by_name("FailedItem",-1,false,nullptr,failed_handle,error));
 auto* published=t.manager.object(failed_handle.key);check(published&&owner.find(published->identity)&&t.manager.source_count50()==2);
 check(t.manager.pending().size()==2&&t.item_callbacks==0);
 ItemInstanceV1 instance;const std::int16_t* actual{};check(CanonicalItemFactoryV2::pickup_override58(nullptr,instance,actual,error));check(actual==&instance.requirement&&*actual==-1);
 ItemRecord164 row{};row.words[3]=37;check(item_pickup_type_v2(instance,row)==37);instance.requirement=-2;check(item_pickup_type_v2(instance,row)==-2);instance.requirement=4;check(item_pickup_type_v2(instance,row)==4);
 std::cout<<"Canonical Item factory PASS actual cache, real Item ctor/base/embedded inventory, canonical Spawn/queue/duplicate/failed prefix and SAME pickup58; condition/network/Handle transports explicit fixtures; visual/body pool initialization not claimed; checks "<<checks<<'\n';return 0;
 }catch(const std::exception& e){std::cerr<<"check "<<checks<<" "<<e.what()<<'\n';return 1;}}
#endif
