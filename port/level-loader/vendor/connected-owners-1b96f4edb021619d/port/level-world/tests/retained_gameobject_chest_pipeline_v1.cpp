#include "retained_gameobject_decor_v1.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <stdexcept>
int main(int argc,char** argv){try{
 using namespace dh2;using namespace dh2::world;
 if(argc!=2)throw std::runtime_error("requires exact actual chest cache directory");
 unsigned constructions=0,bodies=0;
 for(const char* name:{"go_chest_swamp.bdae","go_chest_swamp_big.bdae","go_chest_swamp_rotten.bdae"}){
  actor::RuntimeState runtime{};auto lease=std::make_shared<int>(1);
  CanonicalGameObjectBaseOwnerV1 base(0x7000+constructions,7,lease,runtime);
  base.class_name20()="OpenableContainer";auto fields=base.properties().fields;std::string error;
  assert(fields.write_vector3(fields.context,0x120,{1,1,1},error));
  assert(fields.write_vector3(fields.context,0x160,{100,200,0},error));
  assert(fields.write_vector3(fields.context,0x16c,{0,0,0},error));
  unsigned pf_calls=0,forces=0;std::vector<std::uintptr_t> registered;
  RetainedGameObjectVisualServicesV1 services;services.owner=lease;
  services.read_asset=[&](const std::string& model,auto& bytes,bool& found,auto&){std::ifstream f(std::string(argv[1])+"/"+model,std::ios::binary);found=bool(f);if(found)bytes.assign(std::istreambuf_iterator<char>(f),std::istreambuf_iterator<char>());return true;};
  // Declared SceneManager/PF fixture boundaries; actual BRES, graph, animation
  // and native body below are production implementations, not fake results.
  services.register_root=[&](auto root,auto&){registered.push_back(root);return true;};
  services.force_register=[&](auto,auto&){++forces;return true;};
  services.release_root=[&](auto root,auto&){assert(registered.size()==1&&registered.back()==root);registered.clear();return true;};
  services.update_pf=[&](auto&){++pf_calls;return true;};
  RetainedGameObjectVisualV1 visual(base,services);
  if(!visual.initialize(name,"",error))throw std::runtime_error(std::string(name)+": "+error);
  if(!(visual.ready()&&visual.root_identity()&&visual.marker().found&&forces==1&&pf_calls>0)){
   std::cerr<<name<<" constructor postcondition ready="<<visual.ready()<<" root="<<visual.root_identity()<<" marker="<<visual.marker().found<<" forces="<<forces<<" pf="<<pf_calls<<'\n';
   throw std::runtime_error("retained visual postcondition mismatch; test and library headers must match");
  }
  assert(visual.timeline().clip_index==0&&visual.timeline().loop==1);
  assert((visual.scene_flags()&0x200u)!=0);
  for(auto flags:visual.node_flags())assert((flags&0x200u)!=0);
  bool accepted=true;assert(visual.play("prespawn",false,accepted,error)&&!accepted);
  assert(visual.play("idle",true,accepted,error)&&accepted);
  assert(visual.update(100,error)&&visual.update(300,error));
  auto before_open=visual.scene().graph;
  assert(visual.play("activate",false,accepted,error)&&accepted);
  assert(visual.update(400,error)&&visual.update(2800,error)&&visual.timeline().ended);
  bool pose_changed=false;assert(before_open.size()==visual.scene().graph.size());
  for(std::size_t i=0;i<before_open.size();++i){auto& a=before_open[i];auto& b=visual.scene().graph[i];
   for(unsigned k=0;k<3;++k)pose_changed=pose_changed||a.translation[k]!=b.translation[k]||a.scale[k]!=b.scale[k];
   for(unsigned k=0;k<4;++k)pose_changed=pose_changed||a.quaternion[k]!=b.quaternion[k];
  }assert(pose_changed); // real authored activate clip must change actual mesh graph
  assert(visual.play("idleactive",true,accepted,error)&&accepted);
  physical::NativeWorld world;float limits[4];assert(dh2_decor_level_world_bounds(limits)==0);world.load(limits);
  bool no_physics=false;RetainedGameObjectDecorServicesV1 body_services;body_services.owner=lease;
  body_services.debug_switch=[&](const char* key,bool& value,auto&){assert(std::string(key)=="MP_NoCollisions"||std::string(key)=="MP_NoPhysics");value=std::string(key)=="MP_NoPhysics"&&no_physics;return true;};
  body_services.update_pf=[&](auto&){++pf_calls;return true;};
  std::uintptr_t physical_identity=0;
  body_services.peer_owner=[&](void* peer,std::uintptr_t& value,auto&){assert(reinterpret_cast<std::uintptr_t>(peer)==physical_identity);value=base.identity();return true;};
  body_services.peer_visible80=[&](auto identity,std::uint8_t& value,auto& e){assert(identity==base.identity());auto f=base.properties().fields;return f.read_bool(f.context,0x80,value,e);};
  RetainedGameObjectDecorV1 body(base,visual,world,body_services);
  physical_identity=reinterpret_cast<std::uintptr_t>(&body);
  if(!body.initialize(error))throw std::runtime_error(error);
  assert(body.assigned()&&body.native().body);
  assert(*base.pointer(0x2dc)==reinterpret_cast<std::uintptr_t>(&body));
  physical::NativeBodyObservation o{};assert(dh2_native_body_observe(&o,&body.native())==0);
  assert(o.mass==0&&o.position[0]==1&&o.position[1]==2);
  assert(body.native().body->GetShapeList()&&body.native().body->GetShapeList()->GetFilterData().categoryBits==2);
  assert(fields.write_bool(fields.context,0x80,1,error));
  physical::Filter filter{0,2,0xffff,1};auto& object=body.world_object();
  assert(object.test(object.context,object.context,&filter,&filter)==1);
  assert(fields.write_bool(fields.context,0x80,0,error));
  assert(object.test(object.context,object.context,&filter,&filter)==0);
  assert(fields.write_bool(fields.context,0x80,1,error));
  for(auto event:{physical::ContactEvent::add,physical::ContactEvent::persist,physical::ContactEvent::remove,physical::ContactEvent::result})object.contact(object.context,event,object.context,nullptr,0);
  world.update(16);++bodies;
  no_physics=true;assert(body.detach(error)&&body.native().body&&body.assigned());
  no_physics=false;assert(body.detach(error)&&!body.native().body&&!body.assigned()&&*base.pointer(0x2dc)==0);
  assert(body.release(error));world.clear();assert(visual.release(error)&&registered.empty()&&forces==2);++constructions;
 }
 std::cout<<"Actual chest BRES/complete-scene/clip/timeline + native polygon pipeline PASS visuals="<<constructions<<" bodies="<<bodies<<"; SceneManager/PF/Debug declared fixture boundaries\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
