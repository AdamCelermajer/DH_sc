#define main existing_script_session_fixture_main
#include "character_script_session.cpp"
#undef main
#include "../character_world_npc_initialization_v1.hpp"
#include "../character_world_npc_physical_v1.hpp"
#include "../character_world_npc_object_v1.hpp"
#include "../character_world_npc_properties_v1.hpp"
#include "../world.hpp"
#include "../character_world_npc_init_final_v1.hpp"
#include "../character_world_npc_scene_v1.hpp"
#include "../character_world_npc_scene_bridge_v1.inc"
#include "../character_world_npc_bounds_v1.hpp"
#include <map>
#include "../character_world_npc_physical_lifecycle_v2.hpp"
#include "../character_world_peer_properties_v1.hpp"
#include "../character_world_physical_character_v1.hpp"
#define check(expression) do { if (!(expression)) { std::cerr << "physical graph check failed at line " << __LINE__ << ": " << #expression << '\n'; std::abort(); } ++checks; } while (0)
namespace {
namespace sk=dh2::character::skills;
struct Clock {unsigned frame=1,dt=16,calls=0;static bool get(void* p,unsigned& f,unsigned& d,std::string&){auto& c=*static_cast<Clock*>(p);f=c.frame;d=c.dt;++c.calls;return true;}};
struct PhysicalFixture {
 DebugSwitches* debug_owner{};const DebugFileServices24* debug_files{};unsigned debug_queries{};
 static bool debug(void* p,const char* key,bool& result,std::string& error){auto& f=*static_cast<PhysicalFixture*>(p);++f.debug_queries;return f.debug_owner&&f.debug_files&&character_npc_physical_debug_v1(*f.debug_owner,*f.debug_files,key,result,error);}
 std::map<std::uintptr_t,CharacterWorldNpcObjectV1*> pf;
 CharacterWorldPhysicalPeersV1* peers{};unsigned reads=0;bool enabled_available=true;
 static bool peer(void* p,void* context,std::uintptr_t& owner,std::string& error){return static_cast<PhysicalFixture*>(p)->peers->owner(context,owner,error);}
 static bool enabled(void* p,std::uintptr_t id,std::uint8_t& value,std::string& error){auto& f=*static_cast<PhysicalFixture*>(p);++f.reads;if(!f.enabled_available){error="explicit unavailable byte80 test";return false;}return f.peers->enabled(id,value,error);}
 static int update_pf(void* p,std::uintptr_t id,const dh2::physical::NativeBody*,const dh2::physical::NpcBodyProjection*,std::string& error){auto& f=*static_cast<PhysicalFixture*>(p);auto at=f.pf.find(id);if(at==f.pf.end()){error="missing same PF owner";return 1;}if(!at->second->update_pf()){error=at->second->error();return 1;}return 0;}
};
struct Npc {
 std::uintptr_t id{},node{},target_id{};std::uint8_t enabled=0;
 dh2::target_search::Object48 search{};sk::SkillTargetCharacterV6 character{};
 dh2::scene::Scene scene{};dh2::target_providers::Handle16 handle{};
 std::shared_ptr<dh2::data::PropertyState> properties;
 std::shared_ptr<dh2::data::CombatActorState> life;
 std::unique_ptr<CharacterScriptSession> session;
 std::unique_ptr<CharacterWorldNpcStateOwnerV1> owner;
 std::unique_ptr<CharacterWorldNpcControllerV1> controller;
 AIEventOwner48 ai_owner{};AIEventState64 ai{};
 std::unique_ptr<CharacterWorldNpcStateChangedV1> notifications;
 Facts facts{};StateOwnerBehaviorPredicate8 predicates{};Services bodies{};
 std::shared_ptr<ScriptCharacterObject> object;
 CharacterWorldNpcPhysicalV1* physical{};
 std::unique_ptr<CharacterWorldNpcPhysicalLifecycleV2> graph;std::uint8_t flat{};
 dh2::actor::RuntimeState runtime{};
 WorldNpcObjectFieldsV1 object_fields;
 CharacterWorldNpcObjectV1* object_lifecycle{};
 dh2::visual::SceneBinding visual;
 std::uint32_t light_field=0xa5;
 std::unique_ptr<CharacterWorldNpcSceneV1> scene_owner;
 sk::CharacterWorldRuntimeV1* world{};WorldNpcSpawnGlobalsV1* random{};WorldNpcNetworkModeV1* network{};
 const dh2::data::AiTables* ai_tables{};WorldNpcInitFinalFieldsV1 final_fields;
 std::unique_ptr<CharacterWorldNpcSceneBridgeV1> final_owner;
 dh2::actor::RotationState rotation{};
 WorldNpcAISCollisionFieldsV1 collisions;
 std::unique_ptr<CharacterWorldNpcCollisionV1> collision;
 unsigned animation_deliveries=0,event_deliveries=0;
 static dh2::physical::NativeBody* get_physical(void* p){auto& n=*static_cast<Npc*>(p);return n.physical&&n.physical->phase()>=3?&n.physical->native():nullptr;}
 static bool get_aabb(void* p,float* out,std::string& error){auto& n=*static_cast<Npc*>(p);if(!n.physical){error="required same physical projection";return false;}std::copy_n(n.physical->projection().bounds.absolute_box,6,out);return true;}
 static bool source_player(void* p,bool& out,std::string& error){auto& n=*static_cast<Npc*>(p);return character_npc_spawn_handle_player_v1(*n.world,n.id,out,error);}
 static bool network_mode(void* p,bool& out,std::string&){out=static_cast<Npc*>(p)->network->byte5!=0;return true;}
 static bool spawn(void* p,std::int32_t& out,std::string& error){auto& n=*static_cast<Npc*>(p);WorldNpcSpawnServicesV1 services{};services.context=&n;services.handle_is_player=source_player;services.network_enabled=network_mode;return character_check_spawn_probability_v1(*n.random,n.final_fields.cached270,n.final_fields.threshold274,n.final_fields.deleted82,*n.object_lifecycle,services,out,error);}
 static bool has_visual(void* p){return bool(static_cast<Npc*>(p)->scene_owner);}
 static bool sync_visual(void* p,std::string& error){auto& n=*static_cast<Npc*>(p);const auto& v=n.physical->projection().visual;return n.scene_owner->sync(n.object->position.data(),v.rotation_radians,v.effective_scale,error);}
 static bool set_light(void* p,unsigned value,std::string&){static_cast<Npc*>(p)->scene_owner->assign_light(value);return true;}
 static bool find_node(void* p,const char* name,std::uintptr_t& node,std::string& error){return static_cast<Npc*>(p)->scene_owner->find_node(name,node,error);}
 static bool type(void* p,std::int32_t& out,std::string&){auto& n=*static_cast<Npc*>(p);auto id=n.session->property_view().resolved[1];if(id<0||std::size_t(id)>=n.ai_tables->rows.size())id=8;out=n.ai_tables->rows[id].type;return true;}
 static bool init_ai(void* p,std::string& error){auto& n=*static_cast<Npc*>(p);return character_npc_external_init_final_v1(*n.session,n.ai.active,error);}
 static void body(void* p,State* s,const Request* r){auto& n=*static_cast<Npc*>(p);check(r->service==set_animation);s->current_animation=r->argument[0];++n.animation_deliveries;}
 static int remaining(void* p,StateOwnerMachine40*,const StateOwnerRequest48* r,StateOwnerResponse8*){auto& n=*static_cast<Npc*>(p);if(r->operation!=state_owner_character_event)return 1;check(r->event==0x1d);++n.event_deliveries;return n.notifications->notify(*r);}
 static int refresh(void* p,sk::WorldTargetActorBorrowV1* out){auto& n=*static_cast<Npc*>(p);*out={n.id,&n.search,&n.character,&n.scene,n.life.get(),&n.node,&n.enabled,nullptr,n.search.position,nullptr,nullptr,nullptr,nullptr};return 0;}
};
}
int main(int argc,char** argv){try{
 check(argc==2);const std::string root=argv[1];std::string error;
 Inputs raw((root+"/port/level-world/reference/character-game-design/real-cache-inputs.bin").c_str());CharacterGameDesign design;check(design.initialize(raw.input,error));auto d=design.borrow();
 auto dact=file((root+"/port/android-native/app/src/main/assets/worlds/crypt01.dact").c_str());auto authored=placements(dact);
 auto binary=file((root+"/port/level-world/reference/actor-initialization/crypt01-actor-initialization.bin").c_str());ActorInitializationDigest digest{};std::memcpy(digest.data(),binary.data()+64,32);
 std::vector<ActorInitializationKey> keys;for(std::size_t at=16;at<dact.size();at+=256){std::uint32_t kind,room;std::memcpy(&kind,dact.data()+at,4);std::memcpy(&room,dact.data()+at+4,4);if(kind==1)keys.push_back({room,reinterpret_cast<const char*>(dact.data()+at+8),reinterpret_cast<const char*>(dact.data()+at+72)});}
 ActorInitialization initialization;check(load_actor_initialization(binary.data(),binary.size(),digest,keys,initialization,error));check(keys.size()==11&&authored.size()==11);
 sk::CharacterWorldRuntimeV1 world(*d.ai());CharacterWorldNpcInitializationV1 loader(world,initialization);std::vector<std::unique_ptr<Npc>> actors;
 MissingSave missing;DebugFileServices24 files{&missing,MissingSave::open,MissingSave::close};std::unique_ptr<DebugSwitches,decltype(&dh2_character_debug_destroy)> debug(dh2_character_debug_create(),dh2_character_debug_destroy);check(debug!=nullptr);StateOwnerDebugDiagnostics diagnostics(*debug,files);
 CharacterScriptObjects objects(design.borrow(),debug.get(),&files);WorldNpcCollisionGlobalsV1 globals;Clock clock;
 Host h;h.rows.resize(d.levels()->levels.size());for(unsigned i=0;i<h.rows.size();++i)std::memcpy(&h.rows[i],d.levels()->levels[i].scalar.words+12,24);
 auto crypt=std::find(d.levels()->level_names.begin(),d.levels()->level_names.end(),"GOTHICUS_CRYPT_01");check(crypt!=d.levels()->level_names.end());h.level={std::int32_t(crypt-d.levels()->level_names.begin()),0};
 auto player=std::make_shared<dh2::data::PropertyState>();dh2::data::reset_properties(*d.rules(),*player,&d.characters()->rows[263]);check(dh2::data::recalc_properties_with_class(*d.classes(),*d.rules(),*player,error));auto pv=dh2::data::property_view(*d.rules(),*player);check(!dh2_character_host_context_sync_level(&h.player,&pv));HostContextBindings16 host{{&h,Host::invoke}};DebugLevelBinding16 db{debug.get(),&files};LevelServices16 level{&db,dh2_character_debug_level_service};
 auto common=file((root+"/port/android-native/app/src/main/assets/data/scripts/ai/_commons.luac").c_str()),monster=file((root+"/port/android-native/app/src/main/assets/data/scripts/ai/monster.luac").c_str());
 unsigned interactive=0;std::vector<std::int32_t> types;for(const auto& row:d.ai()->rows)types.push_back(row.type);dh2::target_providers::Types16 type_view{types.data(),std::uint32_t(types.size()),0};
 for(unsigned index=0;index<keys.size();++index){
  auto npc=std::make_unique<Npc>();auto& n=*npc;unsigned source_index=0;for(;source_index<(dact.size()-16)/256;++source_index){const auto* row=dact.data()+16+source_index*256;unsigned room;std::memcpy(&room,row+4,4);if(room==keys[index].room&&!std::strcmp(reinterpret_cast<const char*>(row+8),keys[index].name.c_str()))break;}check(source_index<(dact.size()-16)/256);n.id=0x100000002ull+source_index;n.properties=std::make_shared<dh2::data::PropertyState>();n.life=std::make_shared<dh2::data::CombatActorState>();
  auto found=std::find(d.characters()->names.begin(),d.characters()->names.end(),keys[index].character);check(found!=d.characters()->names.end());dh2::data::reset_properties(*d.rules(),*n.properties,&d.characters()->rows[found-d.characters()->names.begin()]);check(dh2::data::recalc_properties_with_class(*d.classes(),*d.rules(),*n.properties,error));
  n.facts.idle=215;n.bodies={&n,Npc::body};WorldNpcStateServicesV1 services{};services.facts=&n.facts;services.bodies=&n.bodies;services.predicates=&n.predicates;services.remaining_methods={&n,Npc::remaining};services.diagnostics=&diagnostics;services.diagnostics_required=1;n.owner=std::make_unique<CharacterWorldNpcStateOwnerV1>(n.id,services);
  n.object=objects.add(n.id,keys[index].name,n.properties,n.life,authored[index].position);check(n.object!=nullptr);
  CharacterScriptSessionInput input{};input.identity=n.id;input.name=keys[index].name;input.properties=n.properties;input.combat=n.life;input.source_is_character=1;input.target=&n.object->binding;input.objects=&objects.services();input.position=authored[index].position;input.common={common.data(),common.size()};input.external={monster.data(),monster.size()};input.host=&host;input.level=&level;input.state_machine=&n.owner->native_fsm();n.session=CharacterScriptSession::create(design.borrow(),input,error);check(n.session!=nullptr);check(n.session->start()==0&&n.session->error().empty());
  n.controller=std::make_unique<CharacterWorldNpcControllerV1>(n.id);auto* controller=n.controller->command_state(0);check(controller&&controller->owner==n.id&&!controller->locked&&!controller->forced);n.ai_owner={n.id,n.controller->identity(),reinterpret_cast<std::uintptr_t>(&n.owner->native_fsm()),reinterpret_cast<std::uintptr_t>(&n.session->property_view()),0,0,0,0};n.ai={reinterpret_cast<std::uintptr_t>(&n.ai),&n.ai_owner,character_idle_ai_keys(),0,character_idle_external_keys(),0,0,0,0,0,0};n.ai.active=active(*n.session).identity;
  n.collision=std::make_unique<CharacterWorldNpcCollisionV1>(world,*n.owner,*n.session,*n.object,n.ai,*controller,n.collisions,globals,debug.get(),&files,WorldNpcCollisionServicesV1{&clock,Clock::get});
  n.notifications=std::make_unique<CharacterWorldNpcStateChangedV1>(*n.owner,n.ai);
  n.search.identity=n.id;n.search.visible=1;n.character={n.id,n.properties->resolved.data(),keys[index].name.c_str(),0,0,0,1,1};check(world.add({n.id,int(n.id-0x100000000ull),&n,Npc::refresh,&n.owner->state(),&n.handle,&n.object->target.target})==0);
  const auto before=n.properties->resolved;auto queries=world.targets().query_services();std::uintptr_t flags=99;dh2::target_providers::Request24 request{dh2::target_providers::live_state_flags,0,n.id,0};check(!queries.invoke(queries.context,&request,&flags)&&flags==0);
  std::int32_t can_target=99;check(!sk::dh2_character_skill_target_query_live_flags_v6(&can_target,dh2::target_providers::is_interactive,&n.character,nullptr,&type_view,&queries)&&can_target==0);
  WorldNpcInitializationBorrowV1 in{n.id,keys[index].room,keys[index].name.c_str(),n.owner.get(),n.session.get()};check(loader.initialize(in)==1);check(n.owner->state().current==3&&n.owner->state().flags==0x2380);check(n.animation_deliveries==1&&n.event_deliveries==1);check(n.properties->resolved==before&&n.session->combat_state()==n.life);check(!queries.invoke(queries.context,&request,&flags)&&flags==0x2380);check(!sk::dh2_character_skill_target_query_live_flags_v6(&can_target,dh2::target_providers::is_interactive,&n.character,nullptr,&type_view,&queries)&&can_target==1);++interactive;
  auto bad=in;bad.name="absent";check(loader.initialize(bad)==-1&&n.animation_deliveries==1);actors.push_back(std::move(npc));
 }
 check(interactive==11);
 for(auto& actor:actors){auto& n=*actor;check(!character_npc_external_init_final_v1(*n.session,0xdead,error));check(n.session->combat_state()==n.life&&n.owner->state().current==3);}
 check(character_light_set_id_v1("PlayerLight")==0&&character_light_set_id_v1("SceneLight")==1&&character_light_set_id_v1("CameraLight")==2&&character_light_set_id_v1("MonsterLight")==3&&character_light_set_id_v1("absent")==0);
 auto object_properties=file((root+"/port/level-world/reference/character-world-npc-object-v1/crypt01-object-properties.bin").c_str());CharacterWorldNpcPropertiesV1 property_loader;
 check(property_loader.load(object_properties.data(),object_properties.size(),digest,keys));
 auto crypt_bytes=file((root+"/port/android-native/app/src/main/assets/worlds/crypt.bdae").c_str());
 auto descriptor=file((root+"/port/android-native/app/src/main/assets/worlds/crypt01.dwld").c_str());
 dh2::resources::BresView crypt_view{};dh2::world::Level crypt_world;
 check(dh2_bres_open(&crypt_view,crypt_bytes.data(),crypt_bytes.size())==dh2::resources::BresError::ok);
 check(dh2::world::load(crypt_view,descriptor.data(),descriptor.size(),crypt_world,error)&&crypt_world.native_floor);
 std::array<dh2::navigation::ObstacleEntry,32> obstacle_entries{};std::array<unsigned,32> obstacle_floors{};
 dh2::navigation::ObstacleRegistry obstacles{obstacle_entries.data(),0,32,obstacle_floors.data(),0,32};
 dh2::physical::NativeWorld physical_world;float world_bounds[4]={-2000,-2000,2000,2000};physical_world.load(world_bounds);
 std::array<dh2::physical::CharacterNpcBodyModel,3> models;const char* resources[]={"skeleton.bdae","slime_green_v2.bdae","ghost.bdae"};
 for(unsigned model=0;model<3;++model){auto bytes=file((root+"/port/android-native/app/src/main/assets/actors/"+resources[model]).c_str());check(models[model].initialize({bytes.data(),bytes.size()},nullptr,error));}
 CharacterWorldPhysicalPeersV1 production_peers(world);PhysicalFixture physical_fixture;physical_fixture.peers=&production_peers;physical_fixture.debug_owner=debug.get();physical_fixture.debug_files=&files;WorldNpcPhysicalServicesV1 physical_services{&physical_fixture,PhysicalFixture::peer,PhysicalFixture::enabled,nullptr,PhysicalFixture::debug};
 dh2::data::CombatRandom common_random{0,0};unsigned synced_seed=0,synced_calls=0;WorldNpcSpawnGlobalsV1 spawn_random{common_random.seed,synced_seed,common_random.calls,synced_calls};WorldNpcNetworkModeV1 network;
 for(unsigned index=0;index<actors.size();++index){auto& n=*actors[index];unsigned model=keys[index].character.find("Skeleton")!=std::string::npos?0:keys[index].character.find("Ghost")!=std::string::npos?2:1;
  check(!dh2_nav_object_defaults(&n.runtime.object));
  check(property_loader.initialize(keys[index].room,keys[index].name,keys[index].character,n.object_fields));
  n.scene=*models[model].complete_scene();check(n.visual.bind(n.scene,error));
  WorldNpcPhysicalGraphBorrowV2 b{world,physical_world,production_peers,models[model],*n.owner,*n.session,*n.object,n.ai,*n.controller->command_state(0),n.collisions,globals,n.object_fields,n.runtime,crypt_world.native_floor->collision_world,obstacles,*d.ai(),n.final_fields,spawn_random,network,n.visual,n.scene,n.light_field,n.node,n.flat,*debug,files};
  WorldNpcPhysicalGraphServicesV2 service{};service.collision={&clock,Clock::get,nullptr,nullptr,nullptr};
  // Current source Idle/domain initialization reaches no object method: the
  // already-enabled InitFinal gate does not invent visibility/Stop callbacks.
  n.graph=std::make_unique<CharacterWorldNpcPhysicalLifecycleV2>(b,service);
  n.physical=&n.graph->physical_owner();n.object_lifecycle=&n.graph->object_owner();physical_fixture.pf[n.id]=n.object_lifecycle;
  dh2::physical::NpcBodyRequest q{};q.properties=&n.session->property_view();q.ai=d.ai();
  for(unsigned at=0;at<(dact.size()-16)/256;++at){const auto* row=dact.data()+16+256*at;if(!std::strcmp(reinterpret_cast<const char*>(row+8),keys[index].name.c_str())){std::memcpy(q.rotation_degrees,row+212,12);break;}}
  check(n.graph->initialize(q,keys[index].name.c_str(),""));check(n.physical->phase()==4&&n.physical->native().body);
  check(n.runtime.object.motion.floor!=~0u&&n.runtime.object.user==n.id);
  const auto& projection=n.physical->projection();for(unsigned k=0;k<6;++k){check(n.runtime.subobjects.local_bounds[k]==projection.bounds.relative_box[k]);check(n.runtime.subobjects.absolute_bounds[k]==projection.bounds.absolute_box[k]);}
  check(n.runtime.rotation.heading_angle==projection.visual.rotation_radians[2]);
 }

 std::vector<dh2::objects::Record> actual_records;
 for(unsigned index=0;index<(dact.size()-16)/256;++index){const auto* row=dact.data()+16+256*index;dh2::objects::Record r;std::memcpy(&r.kind,row,4);std::memcpy(&r.room,row+4,4);r.name=reinterpret_cast<const char*>(row+8);r.model=reinterpret_cast<const char*>(row+136);std::memcpy(r.position.data(),row+200,12);std::memcpy(r.rotation_degrees.data(),row+212,12);std::memcpy(r.scale.data(),row+224,12);actual_records.push_back(r);}
 CharacterWorldPeerPropertiesV1 decor_properties;auto props=file((root+"/port/level-world/reference/character-world-physical-peers-v1/crypt01-decor-properties.bin").c_str());check(decor_properties.load(props.data(),props.size(),digest,actual_records));
 struct DecorPeer {std::uintptr_t id{},node{};std::uint32_t type{};WorldNpcObjectFieldsV1 fields;dh2::physical::NativeBody body{};dh2::target_providers::Handle16 handle{};std::unique_ptr<WorldPhysicalBaseActorV1> base;std::unique_ptr<CharacterWorldPhysicalReceiverV1> receiver;};
 std::vector<std::unique_ptr<DecorPeer>> decor_peers;unsigned decor_bodies=0;
 for(unsigned index=0;index<actual_records.size();++index){const auto& r=actual_records[index];if(r.kind!=2)continue;auto peer=std::make_unique<DecorPeer>();auto& p=*peer;p.id=0x100000002ull+index;check(decor_properties.initialize_decor(index,p.fields,p.type)&&p.type==0x14&&p.fields.static84==1&&p.fields.visible80==1);
  p.base=std::make_unique<WorldPhysicalBaseActorV1>(p.id,r.position.data(),p.fields,nullptr,p.node);check(world.add(p.base->registration(int(p.id-0x100000000ull),p.handle))==0);
  auto model_bytes=file((root+"/port/android-native/app/src/main/assets/actors/"+r.model).c_str());dh2::resources::BresView v{};check(dh2_bres_open(&v,model_bytes.data(),model_bytes.size())==dh2::resources::BresError::ok);dh2::physical::DecorSceneMarker marker;check(dh2::physical::decor_scene_marker(v,marker,error));
  if(marker.found){dh2::physical::DecorSceneInput placement{};std::copy(r.position.begin(),r.position.end(),placement.position);std::copy(r.rotation_degrees.begin(),r.rotation_degrees.end(),placement.rotation_degrees);std::copy(r.scale.begin(),r.scale.end(),placement.scale);std::copy_n(marker.bounds,6,placement.marker_bounds);std::copy_n(marker.parent_scale,3,placement.marker_parent_scale);dh2::physical::DecorSceneOutput out{};check(dh2_decor_scene(&out,&placement)==0);dh2::physical::DecorBodyInput input{};input.owner=reinterpret_cast<void*>(p.id);input.new_physical=&p.body;input.visual_present=input.colbox_found=1;std::copy_n(out.mesh_box,6,input.mesh_box);std::copy(r.position.begin(),r.position.end(),input.position);dh2::physical::DecorBodyConfig definition{};check(dh2_decor_body_config(&definition,&input)==0);
   p.receiver=std::make_unique<CharacterWorldPhysicalReceiverV1>(production_peers,p.body,p.id,WorldPhysicalReceiverV1::base_physical);check(production_peers.add({&p.receiver->world_object(),&p.body,p.id,&p.fields,&p.type}));p.body={physical_world.create_character(definition.physical,&p.receiver->world_object()),definition.physical.radius,definition.physical.pinned};check(p.body.body);++decor_bodies;
  }decor_peers.push_back(std::move(peer));
 }
 check(decor_peers.size()==84&&decor_bodies>0);

 struct PlayerPeer {std::uintptr_t id=0x100000001ull,node{};WorldNpcObjectFieldsV1 fields;dh2::physical::NativeBody body{};dh2::target_providers::Handle16 handle{};dh2::target_search::Object48 search{};sk::SkillTargetCharacterV6 character{};std::shared_ptr<dh2::data::PropertyState> properties;dh2::data::CombatActorState life{};CharacterStateOwner machine{id};const dh2::scene::Scene* pose{};
  static int refresh(void* p,sk::WorldTargetActorBorrowV1* out){auto& s=*static_cast<PlayerPeer*>(p);s.search.identity=s.id;s.character.identity=s.id;s.character.resolved=s.properties->resolved.data();s.character.name="PlayerCharacterPrince";*out={};out->identity=s.id;out->search=&s.search;out->character=&s.character;out->life=&s.life;out->target_node=&s.node;out->position=s.search.position;out->scene=s.pose;return 0;}
 } player_peer;player_peer.properties=player;check(character_object_default_properties_v1(player_peer.fields,error));std::copy(crypt_world.spawn.begin(),crypt_world.spawn.end(),player_peer.search.position);
 check(world.add({player_peer.id,1,&player_peer,PlayerPeer::refresh,&player_peer.machine.state(),&player_peer.handle})==0);
 CharacterWorldPhysicalCharacterV1 player_events({world,player_peer.id,player_peer.machine.native_fsm(),*debug,files,nullptr,nullptr});CharacterWorldPhysicalReceiverV1 player_receiver(production_peers,player_peer.body,player_peer.id,WorldPhysicalReceiverV1::character,player_events.services());check(production_peers.add({&player_receiver.world_object(),&player_peer.body,player_peer.id,&player_peer.fields,nullptr}));
 // Borrow existing genuine source player body definition producer; this host
 // deliberately leaves the real FSM at constructor-null (GetInteger=-1) and player AIS
 // unavailable. No fabricated Idle/AI success is used to enable contacts.
 auto prince_bytes=file((root+"/port/android-native/app/src/main/assets/models/prince_modular.bdae").c_str());dh2::resources::BresView prince_view{};dh2::scene::Scene prince_pose;check(dh2_bres_open(&prince_view,prince_bytes.data(),prince_bytes.size())==dh2::resources::BresError::ok&&dh2::scene::load(prince_view,prince_pose,error));dh2::physical::ObjectVisualTransformV1 tr{};float scale[3],angles[3]{};check(dh2_character_visual_scale(scale,player->base.data()+12)==0&&dh2_object_visual_transform_v1(&tr,player_peer.search.position,angles,scale)==0);dh2::physical::CharacterOwnerBounds prince_bounds{};check(character_npc_visual_bounds_v1(prince_view,prince_pose,tr.root_matrix,player_peer.search.position,player->resolved[16],0,prince_bounds,error));player_peer.pose=&prince_pose;auto queries=world.targets().query_services();std::uintptr_t is_player{};dh2::target_providers::Request24 player_query{dh2::target_providers::virtual_player,0,player_peer.id,0};check(queries.invoke(queries.context,&player_query,&is_player)==0&&is_player);
 dh2::physical::CharacterBodyInput player_input{};player_input.owner=reinterpret_cast<void*>(player_peer.id);player_input.new_physical=&player_peer.body;player_input.character_type=d.ai()->rows[player->resolved[1]].type;player_input.is_player=unsigned(is_player);player_input.special_owner_byte=player_peer.fields.static84;player_input.absolute_bounds[0]=prince_bounds.absolute_box[0];player_input.absolute_bounds[1]=prince_bounds.absolute_box[1];player_input.absolute_bounds[2]=prince_bounds.absolute_box[3];player_input.absolute_bounds[3]=prince_bounds.absolute_box[4];std::copy_n(player_peer.search.position,2,player_input.position);dh2::physical::CharacterBodyConfig player_config{};check(dh2_character_body_config(&player_config,&player_input)==0);player_peer.body={physical_world.create_character(player_config,&player_receiver.world_object()),player_config.radius,player_config.pinned};check(player_peer.body.body);
 bool player_allowed=true;check(player_events.filter(1,player_allowed,error)&&player_allowed);check(!player_events.event(dh2::physical::ContactEvent::add,actors.front()->id,1,error)&&!error.empty());
 auto& npc=*actors.front();npc.owner->state().current=3;
 for(const auto& p:decor_peers)if(p->body.body){auto* a=npc.physical->native().body->GetShapeList();auto* b=p->body.body->GetShapeList();const auto af=a->GetFilterData(),bf=b->GetFilterData();dh2::physical::WorldShape x{&npc.physical->world_object(),{af.groupIndex,af.categoryBits,af.maskBits,1}},y{&p->receiver->world_object(),{bf.groupIndex,bf.categoryBits,bf.maskBits,1}};std::uintptr_t owner{};check(production_peers.owner(y.owner->context,owner,error)&&owner==p->id);std::uint8_t enabled{};check(production_peers.enabled(owner,enabled,error)&&enabled==1);unsigned type{};check(production_peers.type(owner,type,error)&&type==20);
  dh2::physical::WorldContact contact{{x,y},{0,0}};check(dh2_physical_world_contact(&contact,0)==0&&globals.collisions==1);check(dh2_physical_world_contact(&contact,2)==0&&globals.collisions==0);p->fields.visible80=0;check(dh2_physical_world_should_collide(&x,&y)==0);p->fields.visible80=1;check(!production_peers.remove(y.owner->context));
 }
 check(obstacles.count==11);for(const auto& n:actors){check(n->runtime.object.motion.floor!=~0u&&(n->runtime.object.motion.object_flags&4));unsigned count=0;for(unsigned i=0;i<obstacles.count;++i)count+=obstacles.entries[i].object==n->id;check(count==1);}
 {
  auto& a=*actors[0];auto& b=*actors[1];
  auto shape=[&](Npc& n){auto* shape=n.physical->native().body->GetShapeList();const auto filter=shape->GetFilterData();return dh2::physical::WorldShape{&n.physical->world_object(),{filter.groupIndex,filter.categoryBits,filter.maskBits,1}};};
  auto first=shape(a),second=shape(b);check(dh2_physical_world_should_collide(&first,&second)==1);
  a.object_fields.visible_written=false;bool failed=false;try{dh2_physical_world_should_collide(&first,&second);}catch(const std::runtime_error&){failed=true;}check(failed);a.object_fields.visible_written=true;
  dh2::physical::WorldContact contact{{first,second},{0,0}};check(dh2_physical_world_contact(&contact,0)==0&&globals.collisions==2);
  check(dh2_physical_world_contact(&contact,2)==0&&globals.collisions==0);
  a.owner->state().current=b.owner->state().current=4;clock.frame=900;clock.dt=16;
  check(dh2_physical_world_contact(&contact,1)==0&&a.collisions.collision_ms==16&&b.collisions.collision_ms==0);
  a.owner->state().current=b.owner->state().current=3;a.collisions={};b.collisions={};
 }

 for(unsigned index=0;index<actors.size();++index){auto& n=*actors[index];auto peer=actors[(index+1)%actors.size()]->id;
  bool allowed=false;check(n.collision->permits_filter(1,allowed)&&allowed);check(n.collision->physical_event(WorldNpcPhysicalEventV1::result,peer,1));
  check(n.collision->physical_event(WorldNpcPhysicalEventV1::begin,peer,1));check(globals.collisions==1&&n.collisions.collision_ms==0);
  check(n.collision->physical_event(WorldNpcPhysicalEventV1::end,peer,1)&&globals.collisions==0);
  // Same FSM identity with explicit source-current-ID fixture for the moving
  // branch; no source Move Focus/physical movement implementation is claimed.
  n.owner->state().current=4;clock.frame=unsigned(index+10);clock.dt=16;
  check(n.collision->physical_event(WorldNpcPhysicalEventV1::persist,peer,1));check(n.collisions.collision_ms==16&&n.collisions.last_collision_frame==clock.frame);
  check(n.collision->physical_event(WorldNpcPhysicalEventV1::persist,peer,1)&&n.collisions.collision_ms==16);
  ++clock.frame;n.ai.paused=1;check(n.collision->physical_event(WorldNpcPhysicalEventV1::persist,peer,1)&&n.collisions.collision_ms==16);n.ai.paused=0;
  check(n.collision->physical_event(WorldNpcPhysicalEventV1::persist,peer,1)&&n.collisions.collision_ms==32);
  n.owner->state().current=0;check(n.collision->permits_filter(1,allowed)&&!allowed);
  n.owner->state().current=10;check(n.collision->permits_filter(4,allowed)&&!allowed);check(n.collision->permits_filter(1,allowed)&&allowed);n.owner->state().current=3;
  check(n.collision->physical_event(WorldNpcPhysicalEventV1::begin,0,1)&&globals.collisions==0);
  auto saved=n.ai.active;n.ai.active=0;check(n.collision->physical_event(WorldNpcPhysicalEventV1::begin,peer,1)&&globals.collisions==0);n.ai.active=saved;
 }
 const auto retained_floor_keys=obstacles.floor_count;
 physical_world.destroy(player_peer.body.body);check(production_peers.remove(player_receiver.world_object().context));check(world.remove(player_peer.id)==0);
 for(auto& n:actors){check(n->graph->close());check(!(n->runtime.object.motion.object_flags&4)&&n->runtime.object.obstacle_weight==0&&n->runtime.object.obstacle_extent==0);n->physical=nullptr;n->graph.reset();}for(auto& p:decor_peers){if(p->body.body){physical_world.destroy(p->body.body);check(production_peers.remove(p->receiver->world_object().context));}check(world.remove(p->id)==0);}check(production_peers.clear());check(physical_world.backend()->GetBodyCount()==1);check(obstacles.count==0&&obstacles.floor_count==retained_floor_keys);
 {
  auto& n=*actors.front();auto missing_tail=physical_services;missing_tail.update_pf=nullptr;
  CharacterWorldNpcPhysicalV1 failed(physical_world,models[0],*n.collision,*n.object,*n.session,missing_tail);check(production_peers.add({&failed.world_object(),&failed.native(),n.id,&n.object_fields,nullptr}));
  dh2::physical::NpcBodyRequest request{};request.properties=&n.session->property_view();request.ai=d.ai();std::copy(n.object->position.begin(),n.object->position.end(),request.position);
  check(!failed.initialize(request)&&failed.phase()==3&&failed.native().body&&!failed.error().empty());check(failed.release());check(production_peers.remove(failed.world_object().context));
 }
 auto& first=*actors[0];std::array<std::uintptr_t,51> unknown{};unknown[0xbc/4]=0xdead;first.ai.ais_virtuals=unknown.data();check(!first.collision->physical_event(WorldNpcPhysicalEventV1::begin,actors[1]->id,1)&&!first.collision->error().empty());first.ai.ais_virtuals=character_idle_external_keys();
 first.ai.active=0xdead;check(!first.collision->physical_event(WorldNpcPhysicalEventV1::persist,actors[1]->id,1));first.ai.active=active(*first.session).identity;
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks
 <<",\"actual_Crypt_actors\":11,\"actual_monster_VMs\":11,\"actual_BRES_models\":3,\"actual_decor_world_objects\":84,\"actual_decor_bodies\":"<<decor_bodies<<""
 <<",\"genuine_native_bodies_created_destroyed\":12,\"missing_PF_tail_preserves_body_prefix\":true"
 <<",\"source_InitPhysical_complete_actors\":11,\"full_InitFinal_current_Crypt_domain\":true"
 <<",\"actual_InitFinal_VM_dispatches\":11,\"source_scene_bridge\":true,\"common_Random_borrowed\":true"
 <<",\"same_FSM_property_life_target_AI_controller\":true,\"same_paused_byte\":true"
 <<",\"source_collision_clock_fixture\":true,\"production_peer_registry\":true,\"physical_lifecycle_v2\":true,\"byte80_explicit_fixture\":false"
 <<",\"source_visible_default_producer\":true,\"moving_state_ID_fixture\":true,\"full_NPC_AI\":false}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
