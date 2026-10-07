#include "retained_gameobject_visual_asset_connection_v1.hpp"
#include "game_object_initialization_owner_v1.hpp"
#include "canonical_point3d_globals_v1.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::world;
int main(int argc,char** argv){try{
 if(argc!=2)throw std::runtime_error("requires actual chest cache directory");
 unsigned count=0;
 for(const char* model:{"go_chest_swamp.bdae","go_chest_swamp_big.bdae","go_chest_swamp_rotten.bdae"}){
  auto lease=std::make_shared<int>(1);dh2::actor::RuntimeState runtime{};
  CanonicalGameObjectBaseOwnerV1 base(0x7900+count,7,lease,runtime);
  // This audits the inherited GameObject lifecycle, not the derived network
  // constructor. Class defaults below are genuine recovered base declarations.
  base.class_name20()="GameObject";auto actor=base.properties();std::string error;
  CanonicalPropertyMapV1 properties({nullptr,&canonical_vec3_origin_v1(),nullptr});
  assert(properties.init_properties(actor,error)&&properties.load_defaults(actor,error));
  assert(properties.set_property(actor,"dae",model,error));
  assert(properties.set_property(actor,"position","100,200,0",error));
  unsigned registrations=0,releases=0,forces=0,pf=0,conditions=0;
  RetainedGameObjectVisualServicesV1 visual_services;visual_services.owner=lease;
  visual_services.read_asset=[&](const std::string& name,auto& bytes,bool& found,auto&){std::ifstream f(std::string(argv[1])+"/"+name,std::ios::binary);found=bool(f);if(found)bytes.assign(std::istreambuf_iterator<char>(f),{});return true;};
  // SceneManager, condition, device, PF and light catalog are explicitly
  // declared service fixtures. Resource construction and lifecycle are real.
  visual_services.register_root=[&](auto root,auto&){assert(root);++registrations;return true;};
  visual_services.release_root=[&](auto root,auto&){assert(root);++releases;return true;};
  visual_services.force_register=[&](auto,auto&){++forces;return true;};
  visual_services.update_pf=[&](auto&){++pf;return true;};
  auto connection=std::make_shared<RetainedGameObjectVisualAssetConnectionV1>(base,visual_services);
  GameObjectVisualAssetOwnerV1 assets(base,connection->services(connection));
  std::vector<std::string> names;
  GameObjectInitializationServicesV1 s;s.owner=lease;s.difficulty_names=&names;s.sound_names=&names;
  s.condition_init=[&](auto offset,auto&){assert(offset==0x8c||offset==0xb0);++conditions;return true;};
  s.check_spawn_probability=[](auto& roll,auto&){roll=-2;return true;};
  s.set_position=[&](const float* p,bool destination,auto&){assert(p==base.vector3(0x160)&&destination);base.update_absolute_aabb();return true;};
  s.device_high_performance=[](bool& high,auto&){high=true;return true;};
  s.load_visual=[&](auto& e){return assets.load_visual(e);};
  s.visual_sync=[&](auto id,auto& e){auto visual=connection->lookup(id);return visual&&visual->sync(e);};
  s.set_visible=[&](bool visible,auto& e){auto f=base.properties().fields;return f.write_bool(f.context,0x80,visible?1:0,e);};
  s.init_pf_object=[&](bool stat,const float* p,float radius,auto id,auto&){assert(!stat&&p==runtime.subobjects.position&&radius>0&&id==base.identity());++pf;return true;};
  s.light_set_id=[](const std::string& name,auto& id,auto&){assert(name=="PlayerLight");id=-1;return true;};
  s.visual_set_light_set=[&](auto id,auto light,auto&){auto visual=connection->lookup(id);assert(visual);visual->store_light_set(light);return true;};
  s.visual_root=[&](auto id,auto& root,auto&){auto visual=connection->lookup(id);assert(visual);root=visual->root_identity();return true;};
  s.node_from_name=[&](auto root,const char* name,auto& node,auto& e){auto visual=connection->attached();assert(visual&&visual->root_identity()==root);return visual->node_from_name(name,node,e);};
  s.update_pf_object=[&](auto&){++pf;return true;};
  GameObjectInitializationOwnerV1 initialization(base,s);bool eligible=false;
  if(!initialization.init_post(eligible,error)||!eligible)throw std::runtime_error(error);
  auto visual=connection->attached();assert(visual&&visual->root_game_object()==base.identity());
  auto id=*base.pointer(0x2d8);assert(id&&conditions==2&&registrations==1&&forces==1);
  if(!initialization.init_final(eligible,error)||!eligible)throw std::runtime_error(error);
  assert(connection->attached()==visual&&*base.pointer(0x2d8)==id&&pf>=3);
  std::uintptr_t expected_node=0;assert(visual->node_from_name("target_node",expected_node,error));
  assert(*base.pointer(0x180)==expected_node);
  bool accepted=false;assert(visual->play("activate",false,accepted,error)&&accepted);
  assert(visual->update(100,error)&&visual->update(3000,error)&&visual->timeline().ended);
  // A genuine file-open miss produces root0; source SetVisual keeps old graph.
  assert(assets.set_visual("absent-resource-for-lifecycle-audit.bdae","",true,error));
  assert(*base.pointer(0x2d8)==id&&connection->attached()==visual&&registrations==1&&releases==0);
  assert(assets.set_visual(std::uintptr_t{0},error));
  assert(!connection->attached()&&*base.pointer(0x2d8)==0&&releases==1&&forces==3);
  ++count;
 }
 std::cout<<"Actual chest asset connection + generic InitPost/InitFinal PASS models="<<count<<"; declared SceneManager/condition/device/PF/light fixtures\n";
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
