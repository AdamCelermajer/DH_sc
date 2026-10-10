#define DH2_ITEM_FACTORY_FIXTURE_ONLY
#include "canonical_item_factory_v2.cpp"
#include "../world_item_graph_v3.hpp"
#include "../world_loot_item_runtime_v1.hpp"
#include "../world_item_scene_services_v3.hpp"
struct PoolGraphFixture {
 ItemFactoryFixture factory_state;CanonicalItemFactoryV2* factory{};
 dh2::physical::NativeWorld physics;std::shared_ptr<void> lease;
 std::vector<std::uint8_t> visual_bytes;
 std::map<std::uintptr_t,std::unique_ptr<dh2::character::WorldItemGraphV3>> graphs;
 std::shared_ptr<GameObjectSceneRootRegistryV1> scene=std::make_shared<GameObjectSceneRootRegistryV1>();
 unsigned registers{},releases{},pf{},conditions{};
 static bool invoke(void* p,const dh2::character::WorldItemRequestV1& q,std::int32_t& out,std::string& error){
  auto& f=*static_cast<PoolGraphFixture*>(p);auto& graph=f.graphs[q.object];
  if(!graph){auto item=f.factory->find(q.object);check(bool(item));dh2::character::WorldItemGraphServicesV3 s;
   s.visual.owner=f.lease;s.visual.read_asset=[&f](const std::string& path,auto& bytes,bool& found,auto&){check(path=="data/3D/GameObjects/itemdrops.bdae");bytes=f.visual_bytes;found=true;return true;};
   s.visual=world_item_scene_services_v3(std::move(s.visual),f.scene,[&f,id=q.object](auto root){auto it=f.graphs.find(id);return it==f.graphs.end()?nullptr:it->second->visual().constructing_root_v3(root);});s.visual.update_pf=[&f](auto&){++f.pf;return true;};
   s.initialization.owner=f.lease;s.initialization.condition_init=[item,&f](auto offset,auto&){++f.conditions;check(item->base().string(offset+4)->empty());check(*item->base().pointer(offset+0x1c)==0&&*item->base().byte(offset+0x20)==0);return true;};
   s.initialization.check_spawn_probability=[](auto& value,auto&){value=0;return true;};s.initialization.device_high_performance=[](auto& value,auto&){value=true;return true;};
   s.position.owner=f.lease;s.physical.owner=f.lease;s.physical.debug=[](const char* key,bool& value,auto&){check(std::string(key)=="MP_NoPhysics"||std::string(key)=="MP_NoCollisions");value=false;return true;};s.physical.update_pf=[&f](auto&){++f.pf;return true;};
   graph=std::make_unique<dh2::character::WorldItemGraphV3>(item,f.physics,std::move(s));
  }
  bool handled=false;if(!graph->route(q,out,handled,error))return false;if(!handled){error="Unexpected required outer Item operation "+std::to_string(unsigned(q.operation));return false;}return true;
 }
 static bool present(void* p,std::uintptr_t id,bool& out,std::string& e){auto& f=*static_cast<PoolGraphFixture*>(p);auto it=f.graphs.find(id);if(it==f.graphs.end()){e="Missing same Item visual graph";return false;}return it->second->visual().present(out,e);}
 static bool spawn(void* p,const char* type,const char* name,bool deferred,bool network,std::shared_ptr<dh2::character::RetainedWorldItemObjectV1>& item,std::string& e){return static_cast<PoolGraphFixture*>(p)->factory->spawn(type,name,deferred,network,item,e);}
};
int main(int argc,char** argv){try{check(argc==4);std::string error;LootTablesV2 tables;LootAudioVisualV8 av;
 auto b=file(std::string(argv[1])+"/loot_table_pyarray.bin"),n=file(std::string(argv[1])+"/loot_table_pyarraynames.bin"),s=file(std::string(argv[1])+"/loot_table_pystructnames.bin");check(tables.load(span(b),span(n),span(s),error),error);
 auto ab=file(std::string(argv[2])+"/loot_audiovisual_pyarray.bin"),an=file(std::string(argv[2])+"/loot_audiovisual_pyarraynames.bin"),as=file(std::string(argv[2])+"/loot_audiovisual_pystructnames.bin");check(av.load(span(ab),span(an),span(as),error),error);
 PoolGraphFixture f;f.lease=std::make_shared<int>(1);f.visual_bytes=file(std::string(argv[3])+"/itemdrops.bdae");float bounds[4]={-10000,-10000,10000,10000};f.physics.load(bounds);
 CanonicalItemFactoryServicesV2 fs{&f.factory_state,ItemFactoryFixture::resolve,ItemFactoryFixture::condition,ItemFactoryFixture::debug,{&f,PoolGraphFixture::invoke,{},{},nullptr,PoolGraphFixture::present}};
 CanonicalItemFactoryV2 factory(f.factory_state.manager,f.factory_state.map,f.lease,tables.borrow(),av.borrow(),fs);f.factory=&factory;f.factory_state.factory=&factory;
 dh2::character::WorldLootFactoryServicesV1 ls;ls.context=&f;ls.spawn=PoolGraphFixture::spawn;ls.pickup_override58=CanonicalItemFactoryV2::pickup_override58;
 dh2::character::WorldLootItemRuntimeV1 pool(av.borrow(),ls);check(pool.precache(error),error);
 check(f.graphs.size()==av.borrow().rows().size()*5&&f.graphs.size()==145);check(f.scene->roots().size()==145&&f.conditions==290);check(f.factory_state.manager.pending().size()==145);
 auto& first=*f.graphs.begin()->second;const auto id=f.graphs.begin()->first;dh2::character::WorldItemRequestV1 request;request.object=id;request.operation=dh2::character::WorldItemOperationV1::set_visible;request.flag=true;std::int32_t value=0;bool handled=false;check(first.route(request,value,handled,error)&&handled,error);check(!(first.visual().visual()->scene_flags()&1u));check(first.visual().update_v3(100,[&f](auto&){f.scene->notify_visibility_changed_v3();return true;},error),error);check(first.visual().visual()->scene_flags()&1u);
 request.operation=dh2::character::WorldItemOperationV1::create_decor_physical;check(first.route(request,value,handled,error),error);request.operation=dh2::character::WorldItemOperationV1::set_physical;check(first.route(request,value,handled,error),error);check(first.physical().native().body&&first.physical().secondary_shape());
 request.operation=dh2::character::WorldItemOperationV1::enable;request.flag=false;check(first.route(request,value,handled,error),error);check(first.physical().secondary_shape()->GetFilterData().categoryBits==0);
 request.flag=true;check(first.route(request,value,handled,error),error);check(first.physical().secondary_shape()->GetFilterData().categoryBits==0x40&&first.physical().secondary_shape()->GetFilterData().maskBits==4&&first.physical().secondary_shape()->GetFilterData().groupIndex==-3);
 request.flag=false;check(first.route(request,value,handled,error),error);request.operation=dh2::character::WorldItemOperationV1::set_physical;check(first.route(request,value,handled,error)&&!first.physical().native().body,error);
 for(auto& entry:f.graphs){auto item=factory.find(entry.first);check(bool(item)&&bool(item->base().lifecycle().enabled8a)==(entry.first!=id)&&!item->base().lifecycle().updating85&&item->inventory().items().empty());check(!*item->base().byte(0x80)&&item->base().lifecycle().disabled373==std::uint8_t(entry.first==id)&&bool(item->runtime().object.motion.object_flags&8u)==(entry.first!=id));check(entry.second->visual().visual()->ready()&&!(entry.second->visual().visual()->scene_flags()&1u));check(*item->base().pointer(0x2dc)==0);check(entry.second->visual().release(error),error);}
 check(f.scene->roots().empty());f.graphs.clear();f.physics.clear();std::cout<<"Actual145 canonical Item pool resource graph precache PASS checks="<<checks<<"; real SceneManager membership/release and inherited enable/deferred visibility/physical/filter routes; PF/device/condition/Debug endpoints explicitfixtures, drop/award notclaimed\n";return 0;
 }catch(const std::exception& e){std::cerr<<"check "<<checks<<" "<<e.what()<<'\n';return 1;}}
