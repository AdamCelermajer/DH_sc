#include "original_actor_subobjects.hpp"
#include "playable_actor_bodies.hpp"
#include "playable_actor_world.hpp"
#include "original_actor_properties.hpp"
#include <cstring>
#include <algorithm>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
static void check(bool value,const std::string& error){if(!value)throw std::runtime_error(error);}
struct Fields {ActorState actor;std::array<float,3> destination{};std::uintptr_t attached=0,visual=73;};
// Explicit host visual-root fixture: real decoded character plus the concrete
// placement cells consumed by its renderer. It is not a recovered VisualObject
// implementation or an assertion that the live main has this source root.
struct Visual {CharacterVisual character;std::array<float,3> position{},rotation{},scale{1,1,1};Mat4 placement{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};unsigned updates=0,absolute_updates=0;};
struct Obstacles {std::array<dh2::navigation::ObstacleEntry,8> entries{};std::array<unsigned,8> floors{};dh2::navigation::ObstacleRegistry registry{entries.data(),0,8,floors.data(),0,8};};
int main(int argc,char**argv){try{
 check(argc==2,"Original shared assets root required");AssetCatalog assets(argv[1]);std::string error;
 auto db=std::make_shared<OriginalPropertyDatabase>();check(load_original_property_tables(assets,"original-cache/data/pydata/",*db,error),error);
 auto ai=std::make_shared<dh2::data::AiTables>();check(load_original_ai_tables(assets,"original-cache/data/pydata/",*ai,error),error);
 OriginalActorProperties resolved;check(resolve_original_actor_properties(db->characters,db->classes,"KnightPlayerBase",{256,true},resolved,error),error);
 auto props=std::make_shared<OriginalCombatProperties>();props->sheets=std::move(resolved.sheets);const auto originalSheets=props->sheets;
 auto fields=std::make_shared<Fields>();fields->actor.id=100;fields->actor.health=19;fields->actor.target_id=991;
 auto visual=std::make_shared<Visual>();CharacterVisualConfig config;config.model_path="models/prince_modular.bdae";config.use_authored_modular_defaults=true;check(visual->character.load(assets,config,error),error);
 const auto bytes=assets.read("original-cache/data/3d/modules/swamp/swamp.bdae");dh2::resources::BresView view{};check(dh2_bres_open(&view,bytes.data(),bytes.size())==dh2::resources::BresError::ok,"Floor BRES absent");dh2::scene::Scene scene;check(dh2::scene::load(view,scene,error),error);
 unsigned instance=UINT32_MAX;for(unsigned i=0;i<scene.instances.size();++i)if(scene.graph[scene.instances[i].node_index].name.find("floor")!=std::string::npos){instance=i;break;}check(instance!=UINT32_MAX,"Authored floor absent");
 auto floors=std::make_shared<dh2::floors::World>();OriginalSourceFloorBinding floor;floor.instance=instance;floor.room=0;floor.mesh_local_quaternion={0,0,0,1};floor.mesh_local_scale={1,1,1};check(append_original_module_floors(view,scene,{floor},*floors,error),error);check(dh2::floors::build_graph(*floors,error)&&dh2::floors::post_load(*floors,error),error);
 std::array<float,3> point{};bool found=false;for(const auto& tri:floors->records.front()->triangles){for(unsigned k=0;k<3;++k)point[k]=(tri.points[0][k]+tri.points[1][k]+tri.points[2][k])/3;float z=point[2];if(dh2::floors::height(*floors,point.data(),z)){point[2]=z;found=true;break;}}check(found,"Authored floor lacks diagnostic point");fields->actor.transform.position=point;
 OriginalActorBodyPlanInput q;q.visual=config;q.properties=&props->sheets;q.ai=ai.get();q.owner_identity=100;q.source_name="KnightPlayerBase";q.position={point[0],point[1],point[2]};OriginalActorBodyPlan plan;check(make_original_actor_body_plan(assets,q,plan,error),error);
 auto world=std::make_shared<dh2::physical::NativeWorld>();const float worldBounds[]{-1000,-1000,1000,1000};world->load(worldBounds);PlayableActorBodies pool;
 OriginalActorPhysicalBindings b;b.actor_lease=fields;b.world_lease=world;b.data_lease=props;b.world=world.get();b.ai=ai.get();b.destination1a8=fields->destination.data();b.attached2e0=&fields->attached;b.visual2d8=&fields->visual;
 b.position.visual_sync_position=[&](auto id,auto&){check(id==73,"SetPosition borrowed different visual");visual->position=fields->actor.transform.position;for(unsigned k=0;k<3;++k)visual->placement[12+k]=visual->position[k];return true;};
 b.static84=[](auto& out,auto&){out=0;return true;};b.is_player=[](auto type,auto& out,auto&){out=original_actor_source_is_player(type,"KnightPlayerBase");return true;};b.debug_switch=[](const char*,bool& out,auto&){out=false;return true;};
 b.filter=[](void*,const auto&,const auto&,bool&,auto& e){e="Unbound reached contact filter";return false;};b.contact=[](auto,void*,unsigned,auto& e){e="Unbound reached AIS contact";return false;};check(pool.bind(fields->actor,*props,plan,b,error),error);
 auto obstacles=std::make_shared<Obstacles>();check(pool.initialize_navigation(100,{floors,obstacles,floors.get(),&obstacles->registry},error),error);
 auto path=std::make_shared<dh2::navigation::PathObject>();OriginalActorSubobjectsInput input;input.actor=&fields->actor;check(pool.subobjects_borrow(100,input.owner,error),error);input.navigation_lease=input.owner.owner_lease;input.path_lease=path;input.pf=const_cast<dh2::navigation::NavigationObject*>(pool.navigation(100));input.path=path.get();input.floor_lease=floors;input.obstacle_lease=obstacles;input.floors=floors.get();input.obstacles=&obstacles->registry;input.validating_camera=false;
 std::vector<std::string> order;
 input.set_position=[&](const auto& xyz,bool destination,auto& e){order.push_back("applyPosition");return pool.set_position(100,xyz,destination,e);};
 input.visual=[&](auto id,OriginalActorSubobjectsVisual& out,auto&){check(id==73,"Different actual visual identity");out.receiver=visual;out.identity=73;out.position_ac=visual->position.data();out.update=[&](auto& e){order.push_back("update");++visual->updates;return visual->character.update(0,e);};out.update_absolute=[&](auto&){order.push_back("absolute");for(unsigned k=0;k<3;++k)visual->placement[12+k]=visual->position[k];++visual->absolute_updates;return true;};out.apply_rotation=[&](auto& actor,auto&){order.push_back("applyRotation");actor.transform.rotation=visual->rotation;return true;};out.sync_rotation=[&](const auto& actor,auto&){order.push_back("rotation");visual->rotation=actor.transform.rotation;return true;};out.sync_scaling=[&](const auto& actor,auto&){order.push_back("scale");visual->scale=actor.transform.scale;return true;};return true;};
 OriginalActorSubobjectsResult result;fields->actor.source_flags520=0x23c1;visual->position=point;visual->position[2]+=10;
 const auto before=fields->actor.transform.position;check(visual->position!=before,"Staged root already published");check(update_original_actor_subobjects(input,result,error),error);
 check(result.completed&&order.front()=="update"&&std::find(order.begin(),order.end(),"applyPosition")!=order.end(),"Visual ApplyPosition not reached in original sequence");check(fields->actor.transform.position==point&&visual->position==point&&input.pf->motion.position[2]==point[2],"Source trailing validation did not publish SAME actor/PF/visual ground point");check(visual->placement[14]==point[2],"Actual visual placement not refreshed");
 dh2::physical::NativeBodyObservation observed{};check(!dh2_native_body_observe(&observed,input.owner.native)&&observed.position[0]==point[0]*.01f&&observed.position[1]==point[1]*.01f,"Native position differs from SAME actor");
 // False source admission still publishes its mutated cached position.
 visual->position={99999,99999,point[2]};check(update_original_actor_subobjects(input,result,error),error);check(fields->actor.transform.position==point&&visual->position==point,"False source validation discarded clamp or retained invalid staged root");
 // Missing reached rotation service stops before scaling/floor continuation,
 // preserving the actual ApplyPosition prefix rather than rolling it back.
 auto services=input.visual;input.visual=[&](auto id,auto& out,auto& e){if(!services(id,out,e))return false;out.sync_rotation={};return true;};visual->position=point;visual->position[2]+=5;order.clear();check(!update_original_actor_subobjects(input,result,error)&&result.failed_event==dh2::subobjects::visual_sync_rotation,"Missing reached rotation callback became success");check(fields->actor.transform.position[2]==point[2]+5&&std::find(order.begin(),order.end(),"scale")==order.end(),"Reached prefix rolled back or continued after missing service");input.visual=services;
 // Removed body is actual slot0; the same actor/PF/root can still advance.
 check(pool.remove_physical(100,error)&&pool.subobjects_borrow(100,input.owner,error),error);check(!input.owner.native&&!*input.owner.physical2dc,"Absent body manufactured receiver");visual->position=point;check(update_original_actor_subobjects(input,result,error),error);check(fields->actor.transform.position==point,"Removed body lost actual actor visual publication");
 check(pool.initialize_physical(100,error)&&pool.subobjects_borrow(100,input.owner,error),error);check(input.owner.native&&*input.owner.physical2dc,"Reinitialized body absent");
 // Idle validates neither a fresh stage nor the trailing floor branch. Source
 // flags are explicit fixture cells, never inferred from a gameplay state ID.
 fields->actor.source_flags520=0x2380;visual->position[2]+=50;order.clear();input.floor_lease.reset();input.floors=nullptr;check(update_original_actor_subobjects(input,result,error),error);check(std::find(order.begin(),order.end(),"applyPosition")==order.end()&&visual->position==point,"Idle used unconditional stage/floor policy");
 fields->actor.source_flags520.reset();const auto preserved=fields->actor.transform.position;check(!update_original_actor_subobjects(input,result,error)&&fields->actor.transform.position==preserved,"Unknown actual flags silently admitted");
 check(fields->actor.health==19&&fields->actor.target_id==991&&!std::memcmp(&props->sheets,&originalSheets,sizeof(originalSheets)),"Adapter copied/mutated gameplay property authority");check(pool.clear(error)&&obstacles->registry.count==0,error);
 std::cout<<"PASS actual native/PF/actor authority; concrete decoded visual fixture; staged publication, source floor clamp, Idle policy, removed/reinitialized body, reached failure prefix\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}

