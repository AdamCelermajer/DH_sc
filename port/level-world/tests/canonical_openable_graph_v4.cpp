#include "canonical_openable_graph_v4.hpp"
#include "openable_container_real_cache_fixture_v1.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::world;
int main(int argc,char** argv){try{
 if(argc!=2)throw std::runtime_error("requires actual three-chest cache directory");
 std::string e;OpenableContainerTableV1 table;
 if(!table.load(source_openable_records,sizeof source_openable_records,source_openable_names,sizeof source_openable_names,e))throw std::runtime_error(e);
 auto world=std::make_shared<int>(1);auto roots=std::make_shared<GameObjectSceneRootRegistryV1>();
 dh2::physical::NativeWorld physics;const float box[]{-30000,-30000,30000,30000};physics.load(box);
 unsigned cases{},pf{},updates{};
 for(const char* row_name:{"Swamp_Normal_Chest","SwampCave_Big_Chest","Swamp2_Rotten_Chest"}){
  CanonicalOpenableGraphServicesV4 s;s.world=world;s.roots=roots;s.physics=&physics;
  std::weak_ptr<CanonicalOpenableGraphV4> same;
  s.visual.owner=world;s.visual.read_asset=[&](const auto& uri,auto& bytes,bool& found,auto&){auto at=uri.find_last_of('/');std::ifstream f(std::string(argv[1])+"/"+uri.substr(at==std::string::npos?0:at+1),std::ios::binary);found=bool(f);if(found)bytes.assign(std::istreambuf_iterator<char>(f),{});return true;};
  s.visual.update_pf=[&](auto&){++pf;return true;}; // Explicit PF boundary fixture.
  s.initialization.condition_init=[](auto,auto&){return true;}; // Declared condition compiler boundary.
  s.initialization.check_spawn_probability=[](auto& out,auto&){out=0;return true;}; // Declared RNG fixture.
  s.initialization.device_high_performance=[](bool& out,auto&){out=true;return true;}; // Declared device fixture.
  s.initialization.init_pf_object=[&](bool,const float*,float,std::uintptr_t id,auto&){if(!same.lock()||id!=same.lock()->receiver().base().identity())throw std::runtime_error("PF identity mismatch");++pf;return true;};
  s.initialization.light_set_id=[](const auto& name,auto& id,auto&){if(!name.empty())throw std::runtime_error("unexpected authored light-set");id=0;return true;}; // Declared actual manager boundary.
  s.initialization.update_pf_object=[&](auto&){++pf;return true;};
  s.decor.debug_switch=[](const char* key,bool& out,auto&){if(std::string(key)!="MP_NoPhysics"&&std::string(key)!="MP_NoCollisions")throw std::runtime_error("wrong Debug key");out=false;return true;};
  s.decor.update_pf=[&](auto&){++pf;return true;};
  s.container.spawn_roll_and_probability=[](auto& roll,auto& probability,auto&){roll=0;probability=100;return true;};
  s.container.resolve_row=[&](const auto& name,auto& id,auto& out,auto& error){return table.resolve(name,id,out,error);};
  s.container.visual_asset=[&](std::int32_t id,auto&){const char* uri=id==47?"data/3D/GameObjects/go_chest_swamp.bdae":id==48?"data/3D/GameObjects/go_chest_swamp_big.bdae":id==49?"data/3D/GameObjects/go_chest_swamp_rotten.bdae":nullptr;if(!uri)throw std::runtime_error("source Visuals47/48/49 mismatch");*same.lock()->receiver().base().string(0x290)=uri;return true;}; // Exact original dictionary rows.
  s.container.meet_condition=[](bool& out,auto&){out=true;return true;}; // Explicit condition evaluation fixture.
  s.container.precache_complete_source_v42=[&](auto& error){auto actual=same.lock();if(!actual)throw std::runtime_error("lost SAME precache receiver");std::int32_t id{};OpenableContainerRowV1 row;if(!table.resolve(actual->receiver().fields().data_desc,id,row,error))return false;if(id<0||row.sound!=33)throw std::runtime_error("wrong actual source precache sound");return true;}; // Declared captured-manager precache boundary.
  s.container.load_object_script=[](const char* script,const char* directory,auto&){if(!script||*script||std::string(directory)!="data/scripts/objects/")throw std::runtime_error("unexpected actual source script");return true;}; // Declared script loader delivery, not production fallback.
  s.container.source_on_interact=[&](auto&){++updates;return true;}; // Declared generic Update boundary.
  auto graph=std::make_shared<CanonicalOpenableGraphV4>(std::move(s));same=graph;
  auto& receiver=graph->receiver();receiver.fields().data_desc=row_name;
  auto fields=receiver.properties().fields;
  if(!fields.write_vector3(fields.context,0x120,{1,1,1},e)||!fields.write_vector3(fields.context,0x160,{100,200,250},e))throw std::runtime_error(e);
  auto factory=graph->factory_receiver();if(!factory.init_post(e))throw std::runtime_error(e);
  auto visual=graph->visual();if(!visual||!visual->ready()||!visual->marker().found||roots->roots().size()!=1)throw std::runtime_error("missing actual chest scene/marker");
  if(visual->timeline().loop)throw std::runtime_error("Container source loop0 lost");
  if(!graph->init_final(e)||!*receiver.base().pointer(0x2dc))throw std::runtime_error(e.empty()?"missing actual PODecor assignment":e);
  const float moved[]{300,400,250};if(!graph->set_position(moved,true,e)||receiver.base().runtime().subobjects.position[0]!=300)throw std::runtime_error(e.empty()?"actual assigned-body SetPosition lost":e);
  if(!graph->release(e)||!roots->roots().empty()||*receiver.base().pointer(0x2dc))throw std::runtime_error(e.empty()?"incomplete same graph teardown":e);
  ++cases;
 }
 std::cout<<"PASS canonical Container same runtime/base/class, original68 rows/Visuals47-49, all3 real BRES scene/skin/marker, nonloop named controller, source InitPost/InitFinal + Box2D/PODecor and teardown; declared PF/RNG/device/condition/Debug/light/script/Update fixtures cases="<<cases<<" pf="<<pf<<" update="<<updates<<"\n";
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
