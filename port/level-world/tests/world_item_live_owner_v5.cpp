#define DH2_ITEM_FACTORY_FIXTURE_ONLY
#include "canonical_item_factory_v2.cpp"
#include "../world_item_live_owner_v5.hpp"
using namespace dh2::character;
struct LiveFixtureV5 {
 ItemFactoryFixture canonical;
 dh2::physical::NativeWorld physics;
 dh2::navigation::CollisionWorld geometry{};dh2::navigation::ObstacleRegistry obstacles{};
 std::shared_ptr<void> lease=std::make_shared<int>(1);
 std::shared_ptr<GameObjectSceneRootRegistryV1> roots=std::make_shared<GameObjectSceneRootRegistryV1>();
 std::vector<std::uint8_t> bytes;
 unsigned conditions{},outer_calls{};
 static bool services(void* raw,RetainedWorldItemObjectV1& item,WorldItemGraphServicesV3& s,std::string&){
  auto& f=*static_cast<LiveFixtureV5*>(raw);s.visual.owner=f.lease;
  s.visual.read_asset=[&f](const std::string& uri,auto& bytes,bool& found,std::string&){check(uri=="data/3D/GameObjects/itemdrops.bdae");bytes=f.bytes;found=true;return true;};
  s.initialization.owner=f.lease;s.initialization.condition_init=[&f,&item](auto offset,auto&){++f.conditions;check(item.base().string(offset+4)->empty());check(!*item.base().pointer(offset+0x1c));return true;}; // Declared constructor-empty condition fixture.
  s.initialization.check_spawn_probability=[](auto& roll,auto&){roll=0;return true;}; // Declared source probability query fixture.
  s.initialization.device_high_performance=[](bool& value,auto&){value=true;return true;}; // Declared device fixture.
  s.position.owner=f.lease;s.physical.owner=f.lease;s.physical.debug=[](const char* key,bool& value,auto&){check(std::string(key)=="MP_NoPhysics"||std::string(key)=="MP_NoCollisions");value=false;return true;};
  return true;
 }
 static bool outer(void* raw,const WorldItemRequestV1&,std::int32_t&,std::string& e){++static_cast<LiveFixtureV5*>(raw)->outer_calls;e="actual required outer Item receiver fixture";return false;}
};
int main(int argc,char** argv){try{
 check(argc==4);std::string e;LootTablesV2 tables;LootAudioVisualV8 audiovisual;
 auto b=file(std::string(argv[1])+"/loot_table_pyarray.bin"),n=file(std::string(argv[1])+"/loot_table_pyarraynames.bin"),s=file(std::string(argv[1])+"/loot_table_pystructnames.bin");check(tables.load(span(b),span(n),span(s),e),e);
 b=file(std::string(argv[2])+"/loot_audiovisual_pyarray.bin");n=file(std::string(argv[2])+"/loot_audiovisual_pyarraynames.bin");s=file(std::string(argv[2])+"/loot_audiovisual_pystructnames.bin");check(audiovisual.load(span(b),span(n),span(s),e),e);
 LiveFixtureV5 f;f.bytes=file(std::string(argv[3])+"/itemdrops.bdae");float bounds[4]{-10000,-10000,10000,10000};f.physics.load(bounds);
 WorldItemLiveServicesV5 services;services.world=f.lease;services.roots=f.roots;services.context=&f;services.graph_services=LiveFixtureV5::services;services.outer_item=LiveFixtureV5::outer;
 services.factory.context=&f.canonical;services.factory.resolve=ItemFactoryFixture::resolve;services.factory.test_enable_condition=ItemFactoryFixture::condition;services.factory.unknown_type_debug=ItemFactoryFixture::debug;
 WorldItemLiveOwnerV5 owner(f.canonical.manager,f.canonical.map,f.physics,&f.geometry,&f.obstacles,tables.borrow(),audiovisual.borrow(),services);f.canonical.factory=&owner.factory();
 check(owner.precache(e),e);check(owner.retained_count()==145&&f.roots->roots().size()==145&&f.canonical.manager.pending().size()==145&&f.conditions==290);
 std::vector<std::uintptr_t> ids;
 for(auto identity:f.canonical.manager.pending()){auto item=owner.factory().find(identity);check(bool(item));ids.push_back(identity);check(item->base().shared_handle().key>0);check(f.canonical.manager.object(item->base().shared_handle().key)->identity==identity);auto* graph=owner.graph(identity);check(graph&&graph->receiver_v4().base().identity()==identity&&graph->visual().visual()->ready());check(!item->base().lifecycle().updating85&&!item->base().lifecycle().enabled8a&&item->inventory().items().empty());}
 check(owner.sample_visuals(100,e),e);auto* first=owner.graph(ids.front());WorldItemRequestV1 request;request.object=ids.front();request.operation=WorldItemOperationV1::create_decor_physical;std::int32_t out{};bool handled{};check(first->route(request,out,handled,e)&&handled,e);request.operation=WorldItemOperationV1::set_physical;check(first->route(request,out,handled,e),e);check(first->physical().native().body&&first->physical().secondary_shape());
 check(owner.detach_physics(e),e);check(!first->physical().native().body);check(!owner.update(ids.front(),16,0,e)&&e=="Required attached SAME Item frame owner");owner.rebind(&f.geometry,&f.obstacles);
 check(!owner.update(ids.front(),16,0,e)&&e=="actual required outer Item receiver fixture"&&f.outer_calls==1);
 check(!owner.erased(ids.front(),e)&&e=="Flush SAME145 ItemManager before canonical receiver removal");check(owner.pool().manager().flush(e),e);
 for(auto id:ids)check(owner.erased(id,e),e);check(owner.retained_count()==0&&f.roots->roots().empty());f.physics.clear();
 std::cout<<"PASS reusable SAME canonical145 Item/Scene/physical/PF graph, original key publication, body teardown/rebind, explicit required frame failure and ordered release; actual cache meshes with device/condition/Debug endpoints fixture-only checks="<<checks<<'\n';return 0;
 }catch(const std::exception& ex){std::cerr<<"check "<<checks<<" "<<ex.what()<<'\n';return 1;}}
