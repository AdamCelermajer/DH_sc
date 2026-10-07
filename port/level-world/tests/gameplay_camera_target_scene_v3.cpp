#include "../gameplay_camera_scene_v3.hpp"
#include "../gameplay_camera_target_v2.hpp"
#include "../canonical_point3d_globals_v1.hpp"
#include <cassert>
#include <fstream>
#include <iterator>
#include <iostream>
using namespace dh2;
int main(int argc,char** argv){try{if(argc!=2)return 2;unsigned checks=0;std::string e;
 for(const char* name:{"cameratests.bdae","playercamera.bdae"}){
  std::ifstream file(std::string(argv[1])+"/"+name,std::ios::binary);if(!file)throw std::runtime_error("Required original camera file");std::vector<std::uint8_t> bytes{std::istreambuf_iterator<char>(file),{}};
  auto graph=std::make_shared<camera::GameplayCameraSceneV3>();if(!graph->load(std::move(bytes),e))throw std::runtime_error(e);std::uint32_t selected;if(!graph->select("PlayerCamera_Default",selected,e))throw std::runtime_error(e);
  const auto& c=graph->cameras().at(selected);assert(c.kind==0&&c.horizontal_fov_or_mag==45.f&&c.aspect==1.5f&&c.znear==600.f&&c.zfar==3800.f);++checks;
  const auto local=graph->graph().graph[c.node].translation[0];const auto original_camera=graph->graph().graph[c.node].world;const auto original_target=graph->graph().graph[c.target_node].world;
  const float translation[3]{11,22,33};assert(graph->set_root_position(translation,e));assert(graph->graph().graph[c.node].translation[0]==local);for(unsigned n=0;n<3;++n){assert(graph->graph().graph[c.node].world[12+n]==original_camera[12+n]+translation[n]);assert(graph->graph().graph[c.target_node].world[12+n]==original_target[12+n]+translation[n]);checks+=2;}
  camera::PointV2 anchor{100,200,300};std::uint32_t dt=16;unsigned writes=0,reads=0;camera::TargetServicesV2 services;services.scene_lease=graph;services.root_present=true;
  // World actor anchor/Application dt are explicit fixtures. Root methods use
  // the actual resource graph above, not an empty scene or copied prototype.
  services.actor_anchor=[&](auto id,auto& value,auto&){assert(id==7);value=anchor;return true;};services.root_position=[&](auto& value,auto& error){++reads;return graph->root_position(value.data(),error);};services.root_set_position=[&](const auto& value,auto& error){++writes;return graph->set_root_position(value.data(),error);};services.application_dt=[&](auto& value,auto&){value=dt;return true;};
  camera::GameplayCameraTargetV2 cold(services);assert(!cold.set_target(7,32,e)&&cold.fields().target==0&&e=="Required original camera transition Point3D ZERO");++checks;
  services.source_origin=&world::canonical_vec3_origin_v1();camera::GameplayCameraTargetV2 target(services);assert(target.set_target(7,0,e));target.fields().damping.velocity[0]=9;target.fields().ghost={1,2,3};assert(target.set_target(0,300,e)&&target.fields().target==7&&target.fields().ghost[0]==1);++checks;
  assert(target.set_target(7,0,e)&&target.fields().damping.velocity[0]==9&&target.fields().ghost[0]==0);assert(target.update(e)&&writes==1&&reads==0&&target.fields().transition_remaining==-16);checks+=2;
  anchor={110,210,310};assert(target.update(e)&&writes==2&&reads==2);++checks;
  assert(target.set_target(7,32,e));anchor={120,230,340};assert(target.update(e)&&target.fields().transition_remaining==16);float position[3];assert(graph->root_position(position,e));assert(position[0]==115&&position[1]==220&&position[2]==325);checks+=2;
  assert(target.update(e)&&target.fields().transition_remaining==0);assert(graph->root_position(position,e)&&position[0]==120);++checks;
  assert(target.update(e)&&target.fields().transition_remaining==-16);++checks;
  target.fields().offset_enabled=true;assert(!target.update(e)&&e=="Required actual CameraBase GetCenterOffset");++checks;
  std::cout<<name<<" actual camera/target nodes "<<c.node<<' '<<c.target_node<<"\n";
 }
 std::cout<<"Gameplay camera actual-cache graph/Target lifecycle PASS checks "<<checks<<"; external actor/dt fixtures; no CameraLevel AnimSet/render claim\n";
}catch(const std::exception& failure){std::cerr<<failure.what()<<'\n';return 1;}}
