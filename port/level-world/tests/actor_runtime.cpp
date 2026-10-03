#include "actor_runtime.hpp"
#include "world.hpp"
#include "Box2D.h"
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <vector>
namespace {
void require(bool condition,const char* message){if(!condition)throw std::runtime_error(message);}
std::vector<std::uint8_t> read(const char* path){std::ifstream stream(path,std::ios::binary);return {std::istreambuf_iterator<char>(stream),{}};}
struct Services {
 std::vector<std::uint32_t> events;
 std::uint32_t fail_event=0;
 static std::uint32_t invoke(void* opaque,std::uint32_t event,float*){
  auto& s=*static_cast<Services*>(opaque);s.events.push_back(event);
  if(event==s.fail_event)return ~0u;
  // Explicit fixture: absent camera/auxiliary, no extra visual update/scaling.
  return 0;
 }
};
}
int main(int argc,char** argv){
 if(argc!=5)return 2;
 try {
  using namespace dh2;using namespace actor;
  auto crypt=read(argv[1]),descriptor=read(argv[2]),prince=read(argv[3]),walk=read(argv[4]);
  resources::BresView world_view{},actor_view{};world::Level level;scene::Scene scene;std::string error;
  require(dh2_bres_open(&world_view,crypt.data(),crypt.size())==resources::BresError::ok,"Crypt BRES failed");
  if(!world::load(world_view,descriptor.data(),descriptor.size(),level,error))throw std::runtime_error(error);
  require(dh2_bres_open(&actor_view,prince.data(),prince.size())==resources::BresError::ok&&scene::load(actor_view,scene,error),"Prince scene failed");
  animation::Player clip;if(!clip.load(walk.data(),walk.size(),scene,error,animation::MissingTargets::ignore))throw std::runtime_error(error);
  visual::SceneBinding binding;if(!binding.bind(scene,error))throw std::runtime_error(error);
  RuntimeState state{};std::memcpy(state.subobjects.position,level.spawn.data(),12);std::memcpy(binding.root.position,level.spawn.data(),12);
  for(unsigned i=0;i<3;++i){state.subobjects.destination[i]=level.spawn[i];state.path.target[i]=level.spawn[i];}
  state.subobjects.destination[1]-=3000;state.path.target[1]-=3000;
  state.subobjects.local_bounds[0]=state.subobjects.local_bounds[1]=-36;state.subobjects.local_bounds[3]=state.subobjects.local_bounds[4]=36;state.subobjects.local_bounds[5]=72;
  auto& floor=*level.native_floor;navigation::ObjectInitRequest init{&floor.collision_world,&state.object,1,{},36,0,0};std::memcpy(init.position,level.spawn.data(),12);
  require(dh2_nav_init_object(&init)==0,"Source PF object init failed");
  navigation::ObstacleEntry entries[32]{};std::uint32_t floor_keys[16]{};navigation::ObstacleRegistry registry{entries,0,32,floor_keys,0,16};
  const navigation::ObstacleInitRequest obstacle{&floor.collision_world,&registry,&state.object,1,1,36,1,0};require(dh2_nav_init_obstacle(&obstacle)==0,"Source obstacle registration failed");
  navigation::PathSegment segments[8]{},scratch_segments[8]{};navigation::AvoidanceActor actors[8]{};std::uint32_t scratch_floors[16]{};
  state.path.segments=segments;state.path.capacity=8;state.controller.validate_boundary=1;
  navigation::ControllerWorkspace workspace{scratch_segments,8,0,actors,8,0,scratch_floors,16,0};
  navigation::MotionPolicy motion{};require(dh2_nav_motion_policy_defaults(&motion)==0,"Source floor policy failed");
  b2AABB bounds;bounds.lowerBound.Set(-2000,-2000);bounds.upperBound.Set(2000,2000);b2World physics(bounds,b2Vec2(0,0),true);
  b2BodyDef definition;definition.position.Set(level.spawn[0]*.01f,level.spawn[1]*.01f);definition.fixedRotation=true;definition.isBullet=true;
  auto* body=physics.CreateBody(&definition);b2CircleDef circle;circle.radius=.36f;circle.density=1;body->CreateShape(&circle);body->SetMassFromShapes();physical::NativeBody native{body,.36f,0};
  Services service_context;const subobjects::Services services{&service_context,Services::invoke};std::int32_t properties[224]{};
  RuntimePolicy policy{{0,0,0,1},1,0,0,0,base_virtual_speed};
  float target[3]{91,92,93};RuntimeRequest request{&state,&native,&binding,&scene,&floor.collision_world,&floor.graph,&registry,&motion,&workspace,nullptr,properties,&policy,&services,target,1,0x23c1,16};RuntimeResult result{};
  if(!binding.sample(scene,clip,clip.start,1,true,error))throw std::runtime_error(error);
  unsigned steps=0,frames=0;float traveled=0;
  for(unsigned tick=1;tick<=12;++tick){
   // Caller performs the recovered early scene phase then genuine world Step.
   if(!binding.sample(scene,clip,clip.start+std::int32_t(tick*16),tick*16+1,false,error))throw std::runtime_error(error);
   physics.Step(.016f,10);++steps;
   const float before[3]{state.subobjects.position[0],state.subobjects.position[1],state.subobjects.position[2]};
   const float previous_angle=state.subobjects.rotation;
   if(update_actor(result,request,error))throw std::runtime_error(error);
   ++frames;
   require(result.phase==completed&&!result.failed_event&&result.visual_rotation_requested,"Actor phase/result ordering failed");
   require(!std::memcmp(state.previous_position,before,12)&&state.previous_rotation[2]==previous_angle,"Pre-frame snapshot did not precede actor work");
   require(!std::memcmp(state.target_position,target,12),"Target-node absolute cache failed");
   require(state.controller.heading.active&&result.path.boundary_checked&&result.path.direction_valid,"Source path/heading/boundary did not execute");
   require(state.object.motion.floor!=~0u&&registry.count==1,"Real floor/registry handoff failed");
   require(!std::memcmp(state.subobjects.previous_position,state.object.motion.position,12),"PF rollback position did not refresh after floor validation");
   require(std::fabs(body->GetPosition().x*100-state.subobjects.position[0])<.01f&&std::fabs(body->GetPosition().y*100-state.subobjects.position[1])<.01f,"Native physical transform differs from actor synchronization");
   require(std::fabs(binding.root.position[0]-state.subobjects.position[0])<.001f&&std::fabs(binding.root.position[1]-state.subobjects.position[1])<.001f,"Genuine visual position synchronization failed");
   require(body->GetLinearVelocity().x==0&&body->GetLinearVelocity().y==0,"Move visual policy forced physics-position velocity");
   for(const auto& node:scene.graph)for(float value:node.world)require(std::isfinite(value),"Visual graph world matrix nonfinite");
   traveled+=std::hypot(state.subobjects.position[0]-before[0],state.subobjects.position[1]-before[1]);
  }
  require(traveled>80,"Authored walk root motion did not move actor");
  unsigned absent_camera_queries=0;for(auto event:service_context.events)absent_camera_queries+=event==subobjects::camera_get;
  require(absent_camera_queries==frames,"Explicit absent-camera service did not run before trailing floor validation");
  request.target_absolute_position=nullptr;const float retained_target[3]{state.target_position[0],state.target_position[1],state.target_position[2]};
  // An explicit physical-position policy and virtual speed use source units.
  request.character_flags=0x23c2;state.subobjects.destination[0]=state.subobjects.position[0]+1000;state.subobjects.destination[1]=state.subobjects.position[1];state.path.target[0]=state.subobjects.destination[0];state.path.target[1]=state.subobjects.destination[1];state.controller.validate_boundary=0;
  if(update_actor(result,request,error))throw std::runtime_error(error);
  ++frames;require(body->GetLinearVelocity().x==base_virtual_speed&&body->GetLinearVelocity().y==0,"Explicit original base virtual speed did not reach real body");
  const auto before_step=body->GetPosition();require(before_step.x==state.subobjects.position[0]*.01f,"Actor coordinator advanced an unrequested physics Step");physics.Step(.016f,10);++steps;require(body->GetPosition().x>before_step.x,"Caller physics Step did not advance source velocity");
  // Source Stop requires path_requested and arrival, then true physics policy.
  state.subobjects.position[0]=body->GetPosition().x*100;state.subobjects.position[1]=body->GetPosition().y*100;
  std::memcpy(state.subobjects.destination,state.subobjects.position,12);std::memcpy(state.path.target,state.subobjects.position,12);state.controller.path_requested=1;
  body->ApplyForce(b2Vec2(10,3),body->GetWorldCenter());if(update_actor(result,request,error))throw std::runtime_error(error);++frames;
  require(result.path.stopped&&result.path.physical_stop_requested&&result.physical_stop_applied&&body->IsSleeping(),"Controller-requested Stop did not reach genuine body");
  require(body->GetLinearVelocity().x==0&&body->GetLinearVelocity().y==0&&!state.controller.path_requested&&!state.controller.heading.active,"Stop physical/logical state failed");
  require(!std::memcmp(state.target_position,retained_target,12),"Absent target node overwrote target cache");
  body->WakeUp();physics.Step(.016f,10);++steps;require(body->GetLinearVelocity().x==0&&body->GetLinearVelocity().y==0,"Genuine Stop did not clear accumulated force");
  unsigned atomic=0;const auto valid_request=request;const auto valid_policy=policy;
  const auto reject=[&](){const auto before=state;physical::NativeBodyObservation old{},now{};dh2_native_body_observe(&old,&native);const auto calls=service_context.events.size();RuntimeResult output{};const auto output_before=output;require(update_actor(output,request,error)==1,"Malformed actor accepted");dh2_native_body_observe(&now,&native);require(!std::memcmp(&before,&state,sizeof state)&&!std::memcmp(&old,&now,sizeof old)&&!std::memcmp(&output,&output_before,sizeof output)&&calls==service_context.events.size(),"Malformed actor mutated state/body/output or invoked services");++atomic;};
  request.key=0;reject();request=valid_request;
  request.services=nullptr;reject();request=valid_request;
  policy.virtual_speed=std::numeric_limits<float>::infinity();reject();policy=valid_policy;
  policy.validating_camera=2;reject();policy=valid_policy;
  state.rotation.reserved=1;reject();state.rotation.reserved=0;
  service_context.fail_event=subobjects::visual_update;request.character_flags=0x23c1;require(update_actor(result,request,error)==3&&result.phase==subobjects_phase&&result.failed_event==subobjects::visual_update,"Explicit virtual service failure was not reported");
  std::cout<<"{\"actor_frames\":"<<frames<<",\"caller_world_steps\":"<<steps<<",\"crypt_graph_nodes\":"<<floor.graph.node_count<<",\"crypt_graph_edges\":"<<floor.graph.edge_count<<",\"prince_scene_nodes\":"<<scene.graph.size()<<",\"root_motion_distance\":"<<traveled<<",\"floor_registry_verified\":true,\"path_rotation_subobjects_order_verified\":true,\"controller_requested_genuine_stop\":true,\"explicit_absent_camera_queries\":"<<absent_camera_queries<<",\"atomic_rejection_checks\":"<<atomic<<",\"explicit_service_failure_checks\":1,\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}
}
