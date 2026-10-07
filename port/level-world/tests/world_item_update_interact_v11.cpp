#include "item_frame_virtual_policy_v4.hpp"
#include "world_item_frame_v5.hpp"
#include "world.hpp"
#define DH2_ITEM_FACTORY_FIXTURE_ONLY
#include "canonical_item_factory_v2.cpp"
#include "../world_item_graph_v3.hpp"
#include "../world_loot_item_runtime_v1.hpp"
#include "../world_item_scene_services_v3.hpp"
#include "../world_item_pf_connection_v4.hpp"
#include "../world_item_physical_contact_v4.hpp"
struct PoolGraphFixture {
 ItemFactoryFixture factory_state;CanonicalItemFactoryV2* factory{};
 dh2::physical::NativeWorld physics;std::shared_ptr<void> lease;
 std::vector<std::uint8_t> visual_bytes;
 std::map<std::uintptr_t,std::unique_ptr<dh2::character::WorldItemGraphV3>> graphs;
 std::shared_ptr<GameObjectSceneRootRegistryV1> scene=std::make_shared<GameObjectSceneRootRegistryV1>();
 unsigned registers{},releases{},pf{},conditions{};std::function<bool(const dh2::character::WorldItemRequestV1&,std::int32_t&,std::string&)> update;
 dh2::navigation::CollisionWorld ctor_geometry{};
 dh2::navigation::ObstacleRegistry ctor_registry{};
 std::map<std::uintptr_t,std::unique_ptr<dh2::character::WorldItemPfConnectionV4>> pf_connections;
 static bool invoke(void* p,const dh2::character::WorldItemRequestV1& q,std::int32_t& out,std::string& error){
  auto& f=*static_cast<PoolGraphFixture*>(p);auto& graph=f.graphs[q.object];
  if(!graph){auto item=f.factory->find(q.object);check(bool(item));dh2::character::WorldItemGraphServicesV3 s;
   auto connection=std::make_unique<dh2::character::WorldItemPfConnectionV4>(*item,f.lease,&f.ctor_geometry,&f.ctor_registry);
   auto* same_pf=connection.get();f.pf_connections.emplace(q.object,std::move(connection));
   check(item->runtime().object.user==0);
   s.visual.owner=f.lease;s.visual.read_asset=[&f](const std::string& path,auto& bytes,bool& found,auto&){check(path=="data/3D/GameObjects/itemdrops.bdae");bytes=f.visual_bytes;found=true;return true;};
   s.visual=world_item_scene_services_v3(std::move(s.visual),f.scene,[&f,id=q.object](auto root){auto it=f.graphs.find(id);return it==f.graphs.end()?nullptr:it->second->visual().constructing_root_v3(root);});s.visual.update_pf=[&f,same_pf](auto& e){++f.pf;return same_pf->update(e);};
   s.initialization.owner=f.lease;s.initialization.condition_init=[item,&f](auto offset,auto&){++f.conditions;check(item->base().string(offset+4)->empty());check(*item->base().pointer(offset+0x1c)==0&&*item->base().byte(offset+0x20)==0);return true;};
   s.initialization.check_spawn_probability=[](auto& value,auto&){value=0;return true;};s.initialization.device_high_performance=[](auto& value,auto&){value=true;return true;};
   s.position.owner=f.lease;s.physical.owner=f.lease;s.physical.debug=[](const char* key,bool& value,auto&){check(std::string(key)=="MP_NoPhysics"||std::string(key)=="MP_NoCollisions");value=false;return true;};s.physical.update_pf=[&f,same_pf](auto& e){++f.pf;return same_pf->update(e);};
   graph=std::make_unique<dh2::character::WorldItemGraphV3>(item,f.physics,std::move(s));
   same_pf->physical(&graph->physical());
  }
  if(f.update&&(q.operation==dh2::character::WorldItemOperationV1::game_update||q.operation==dh2::character::WorldItemOperationV1::is_at_destination||q.operation==dh2::character::WorldItemOperationV1::stop))return f.update(q,out,error);
  bool handled=false;if(!graph->route(q,out,handled,error))return false;if(!handled){error="Unexpected required outer Item operation "+std::to_string(unsigned(q.operation));return false;}return true;
 }
 static bool present(void* p,std::uintptr_t id,bool& out,std::string& e){auto& f=*static_cast<PoolGraphFixture*>(p);auto it=f.graphs.find(id);if(it==f.graphs.end()){e="Missing same Item visual graph";return false;}return it->second->visual().present(out,e);}
 static bool spawn(void* p,const char* type,const char* name,bool deferred,bool network,std::shared_ptr<dh2::character::RetainedWorldItemObjectV1>& item,std::string& e){return static_cast<PoolGraphFixture*>(p)->factory->spawn(type,name,deferred,network,item,e);}
};

struct GenericItemFrameFixtureV4 {
 PoolGraphFixture& pool;dh2::character::WorldItemGraphV3& graph;
 std::uint32_t ms{},camera_queries{},fail_event{};std::string error;
 static bool world(void* p,std::string& e){auto& f=*static_cast<GenericItemFrameFixtureV4*>(p);auto v=f.graph.visual().visual();return v&&v->binding().update_world(v->scene(),e);}
 static bool rotation(void* p,const float* xyz,std::string& e){auto& f=*static_cast<GenericItemFrameFixtureV4*>(p);auto v=f.graph.visual().visual();if(!v){e="Missing actual Item root";return false;}return v->binding().set_rotation(xyz);}
 static std::uint32_t service(void* p,std::uint32_t event,float*){
  auto& f=*static_cast<GenericItemFrameFixtureV4*>(p);using namespace dh2::subobjects;
  if(event==f.fail_event)return ~0u;
  if(event==visual_update)return f.graph.visual().update_v3(f.ms,[&](auto&){f.pool.scene->notify_visibility_changed_v3();return true;},f.error)?1:~0u;
  if(event==visual_sync_scaling){auto v=f.graph.visual().visual();return v&&v->sync(f.error)?1:~0u;}
  if(event==camera_get){++f.camera_queries;return 0;} // Explicit absent-camera fixture.
  if(event==get_speed)return 0; // Observer of actual source3b0, not speed producer.
  f.error="Unexpected required fixture service "+std::to_string(event);return ~0u;
 }
};
int main(int argc,char** argv){try{
 check(argc==6);std::string error;LootTablesV2 tables;LootAudioVisualV8 av;
 auto b=file(std::string(argv[1])+"/loot_table_pyarray.bin"),n=file(std::string(argv[1])+"/loot_table_pyarraynames.bin"),s=file(std::string(argv[1])+"/loot_table_pystructnames.bin");check(tables.load(span(b),span(n),span(s),error),error);
 auto ab=file(std::string(argv[2])+"/loot_audiovisual_pyarray.bin"),an=file(std::string(argv[2])+"/loot_audiovisual_pyarraynames.bin"),as=file(std::string(argv[2])+"/loot_audiovisual_pystructnames.bin");check(av.load(span(ab),span(an),span(as),error),error);
 auto crypt=file(argv[4]),descriptor=file(argv[5]);dh2::resources::BresView view{};dh2::world::Level level;
 check(dh2_bres_open(&view,crypt.data(),crypt.size())==dh2::resources::BresError::ok);check(dh2::world::load(view,descriptor.data(),descriptor.size(),level,error),error);
 PoolGraphFixture f;f.lease=std::make_shared<int>(1);f.visual_bytes=file(std::string(argv[3])+"/itemdrops.bdae");float bounds[4]={-10000,-10000,10000,10000};f.physics.load(bounds);
 CanonicalItemFactoryServicesV2 fs{&f.factory_state,ItemFactoryFixture::resolve,ItemFactoryFixture::condition,ItemFactoryFixture::debug,{&f,PoolGraphFixture::invoke,{},{},nullptr,PoolGraphFixture::present}};
 CanonicalItemFactoryV2 factory(f.factory_state.manager,f.factory_state.map,f.lease,tables.borrow(),av.borrow(),fs);f.factory=&factory;f.factory_state.factory=&factory;
 std::shared_ptr<dh2::character::RetainedWorldItemObjectV1> item;check(factory.spawn("Item","ActualGenericFrameItem",false,true,item,error),error);check(item->init_once(0,error),error);
 auto& graph=*f.graphs.at(item->base().identity());
 dh2::character::WorldItemRequestV1 q;q.object=item->base().identity();q.operation=dh2::character::WorldItemOperationV1::set_position;q.position=level.spawn.data();q.flag=true;std::int32_t result{};bool handled{};check(graph.route(q,result,handled,error),error);
 q.operation=dh2::character::WorldItemOperationV1::enable;check(graph.route(q,result,handled,error),error);
 q.operation=dh2::character::WorldItemOperationV1::create_decor_physical;check(graph.route(q,result,handled,error),error);q.operation=dh2::character::WorldItemOperationV1::set_physical;check(graph.route(q,result,handled,error),error);
 auto& state=item->runtime();state.controller.destination[0]=level.spawn[0]+1000;state.controller.destination[1]=level.spawn[1];state.controller.destination[2]=level.spawn[2];
 dh2::navigation::ObstacleEntry entries[32]{};std::uint32_t floor_keys[16]{};dh2::navigation::ObstacleRegistry registry{entries,0,32,floor_keys,0,16};
 dh2::navigation::PathSegment scratch[8]{};dh2::navigation::AvoidanceActor actors[8]{};std::uint32_t floor_scratch[16]{};dh2::navigation::ControllerWorkspace workspace{scratch,8,0,actors,8,0,floor_scratch,16,0};
 dh2::navigation::MotionPolicy motion{};check(dh2_nav_motion_policy_defaults(&motion)==0);
 GenericItemFrameFixtureV4 context{f,graph,0,0,0,{}};const dh2::subobjects::Services services{&context,GenericItemFrameFixtureV4::service};
 dh2::actor::RuntimePolicy policy{};dh2::actor::GenericRuntimeRequestV4 request;check(dh2::character::item_frame_virtual_policy_v4(*item,request,policy,error),error);
 request.actor.state=&state;request.actor.native_body=&graph.physical().native();request.actor.geometry=&level.native_floor->collision_world;request.actor.graph=&level.native_floor->graph;request.actor.registry=&registry;request.actor.motion_policy=&motion;request.actor.workspace=&workspace;request.actor.services=&services;request.actor.key=item->base().identity();request.actor.dt_ms=16;
 auto visual=graph.visual().visual();request.visual={&context,&visual->binding().root,GenericItemFrameFixtureV4::world,GenericItemFrameFixtureV4::rotation};
 check(request.actor.resolved224==nullptr&&request.actor.binding==nullptr&&policy.virtual_speed==6.f&&request.actual_rotation_speed==-1.f);
 dh2::character::WorldItemFrameServicesV5 live_services{&level.native_floor->collision_world,&level.native_floor->graph,&registry,&motion,&workspace,nullptr,services,f.scene,&policy,nullptr,nullptr};
 unsigned begin_updates{},end_updates{},online_queries{};
 live_services.begin_update=[&](auto&,auto&){++begin_updates;return true;};live_services.end_update=[&](auto&,auto&){++end_updates;return true;}; // Explicit profiler/Debug/Application count fixture deliveries.
 live_services.online={&online_queries,[](void* p,bool& out,std::string&){++*static_cast<unsigned*>(p);out=false;return true;},nullptr,nullptr}; // Explicit actual offline query fixture.
 dh2::character::WorldItemFrameV5 live_frame(graph,live_services);dh2::character::WorldItemRequestV1 live_request;live_request.object=item->base().identity();live_request.operation=dh2::character::WorldItemOperationV1::game_update;live_request.integer=16;std::int32_t live_value{};bool live_handled{};
 dh2::actor::RuntimeResult output{};for(unsigned frame=1;frame<=8;++frame){context.ms=frame*16;f.physics.update(16);live_frame.absolute_time(context.ms);check(live_frame.route(live_request,live_value,live_handled,error)&&live_handled,error);check(graph.physical().native().body->GetLinearVelocity().x==6.f);check(visual->binding().root.position[0]==state.subobjects.position[0]);}
 check(state.subobjects.position[0]>level.spawn[0]&&state.controller.destination[0]==level.spawn[0]+1000);
 check(begin_updates==8&&end_updates==8&&online_queries==8);
 live_request.operation=dh2::character::WorldItemOperationV1::is_at_destination;check(live_frame.route(live_request,live_value,live_handled,error)&&live_handled&&live_value==0,error);
 live_request.operation=dh2::character::WorldItemOperationV1::stop;check(live_frame.route(live_request,live_value,live_handled,error)&&live_handled,error);check(graph.physical().native().body->GetLinearVelocity().x==0&&state.controller.heading.active==0&&state.controller.path_requested==0);for(unsigned i=0;i<3;++i)check(state.controller.destination[i]==state.subobjects.position[i]);

 // Actual retained Item inventory/body/base: only external Character cast and
 // IsPlayer deliveries below are explicit fixtures (source nonplayer branch).
 auto actual_item=std::make_unique<dh2::data::ItemInstanceV1>();actual_item->id=0;actual_item->quantity=1;
 auto* same_item=actual_item.get();check(item->inventory().store(actual_item,{nullptr,[](void*,const auto&,auto& out,auto&){out=0;return true;}},nullptr,nullptr,error),error);
 unsigned interaction_calls{};item->bind_interaction({&interaction_calls,[](void* raw,const auto& q,auto& out,auto& e){
  ++*static_cast<unsigned*>(raw);
  if(q.operation==dh2::character::LootInteractOperationV8::character_cast){out.character.identity=q.character;return true;}
  if(q.operation==dh2::character::LootInteractOperationV8::is_player){out.value=0;return true;}
  e="Unexpected required Item interaction fixture";return false;}, {}});
 auto* same_body=graph.physical().native().body;const auto same_identity=item->base().identity();
 check(!item->interact_from_update_v11(123,error)&&!item->failed());
 live_services.collision_interact=[item](auto peer,auto& e){return item->interact_from_update_v11(peer,e);};
 dh2::character::WorldItemFrameV5 nested_frame(graph,live_services);nested_frame.absolute_time(context.ms);
 f.update=[&](const auto& request,auto& value,auto& e){
  std::string public_error;check(!item->interact(123,public_error)&&!item->failed());
  bool reached{};return nested_frame.route(request,value,reached,e)&&reached;
 };
 *item->base().pointer(0x2e4)=123;check(item->update(16,0,error),error);
 check(interaction_calls==2&&*item->base().pointer(0x2e4)==0);
 check(item->inventory().peek()==same_item&&graph.physical().native().body==same_body&&item->base().identity()==same_identity);
 check(!item->interact_from_update_v11(123,error)&&!item->failed());
 // A real reached callback failure retains collision target and blocks retry.
 item->bind_interaction({nullptr,[](void*,const auto&,auto&,auto& e){e="Required actual Character cast unavailable fixture";return false;},{}});
 *item->base().pointer(0x2e4)=123;check(!item->update(16,0,error));
 check(item->failed()&&*item->base().pointer(0x2e4)==123&&item->inventory().peek()==same_item);
 const auto old_begin=begin_updates;check(!item->update(16,0,error)&&begin_updates==old_begin);
 const auto before=state;const auto calls=context.camera_queries;request.actual_virtual_policy.position_from_visual=2;check(dh2::actor::update_gameobject_v4(output,request,error)==1);check(!std::memcmp(&state,&before,sizeof state)&&context.camera_queries==calls);request.actual_virtual_policy.position_from_visual=0;
 context.fail_event=dh2::subobjects::visual_update;check(dh2::actor::update_gameobject_v4(output,request,error)==3&&output.failed_event==dh2::subobjects::visual_update);
 std::cout<<"PASS bounded Update Interact V11 on SAME Item inventory/base/Box2D plus reusable Item frame V5 over actual BRES/root/Box2D, eight frames speed6 + source Stop/IsAtDestination, no Character sheet/flags/animation-root, atomic policy reject and required callback prefix; camera/device/condition/Debug fixture declarations explicit; checks="<<checks<<"\n";
 }catch(const std::exception& e){std::cerr<<"check "<<checks<<" "<<e.what()<<'\n';return 1;}}
