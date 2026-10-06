#include "retained_scene_visual_connection_v3.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::world;
int main(int argc,char** argv){try{
 if(argc!=2)throw std::runtime_error("requires actual chest cache directory");
 unsigned count=0;
 auto manager=std::make_shared<GameObjectSceneRootRegistryV1>();
 for(const char* model:{"go_chest_swamp.bdae","go_chest_swamp_big.bdae","go_chest_swamp_rotten.bdae"}){
  auto lease=std::make_shared<int>(1);dh2::actor::RuntimeState runtime{};
  CanonicalGameObjectBaseOwnerV1 base(0x8800+count,7,lease,runtime);std::string error;
  auto fields=base.properties().fields;assert(fields.write_vector3(fields.context,0x120,{1,1,1},error));
  assert(fields.write_vector3(fields.context,0x160,{100,200,0},error));assert(fields.write_vector3(fields.context,0x16c,{0,0,0},error));
  bool pf_fail=false;unsigned pf=0;RetainedGameObjectVisualServicesV1 s;s.owner=lease;
  s.read_asset=[&](const std::string& name,auto& bytes,bool& found,auto&){std::ifstream f(std::string(argv[1])+"/"+name,std::ios::binary);found=bool(f);if(found)bytes.assign(std::istreambuf_iterator<char>(f),{});return true;};
  s.update_pf=[&](auto& e){++pf;if(pf_fail){e="declared actual PF unavailable";return false;}return true;};
  // SceneManager/visibility/animator endpoints deliberately NOT supplied:
  // V3 supplies actual retained owners, not successful empty callbacks.
  auto bridge=std::make_shared<RetainedSceneVisualConnectionV3>(base,s,manager);
  GameObjectVisualAssetOwnerV1 assets(base,bridge->services(bridge));
  if(!assets.set_visual(model,"",true,error))throw std::runtime_error(error);
  auto visual=bridge->attached();assert(visual&&visual->root_animator_present()&&visual->animation_track_count()>0&&pf==1);
  assert(manager->roots()==std::vector<std::uintptr_t>{visual->root_identity()});
  assert(visual->root_parent_identity()==manager->identity());
  assert(manager->fields().force448&&manager->fields().hierarchy_dirty288&&manager->fields().render_dirty289);
  assert(visual->notify_root_visibility(false,error)&&!(visual->scene_flags()&1u));
  for(auto flags:visual->node_flags())assert(!(flags&1u));
  std::uintptr_t node=0;assert(!visual->scene().graph.empty());
  assert(visual->node_from_name(visual->scene().graph.front().name.c_str(),node,error)&&node);
  assert(visual->set_node_local_visibility(node,false,error));
  assert(visual->notify_root_visibility(true,error)&&(visual->scene_flags()&1u));
  assert(!(visual->node_flags().front()&1u)); // local hidden survives parent restore
  assert(visual->set_node_local_visibility(node,true,error));
  for(auto flags:visual->node_flags())assert(flags&1u);
  bool accepted=false;assert(visual->play("activate",false,accepted,error)&&accepted);
  assert(visual->update(100,error)&&visual->update(600,error));
  auto sampled=visual->scene().graph.front().world;
  // Exercise the genuine release receiver before ordinary visual destruction.
  assert(manager->release_visual_root(visual->root_identity(),error));
  assert(!visual->root_parent_identity()&&!visual->root_animator_present()&&visual->animation_track_count()==0);
  assert(visual->scene().graph.front().world==sampled&&manager->roots().empty());
  assert(visual->update(10000,error)&&visual->scene().graph.front().world==sampled);
  // Source remove(NULL-parent) is a real empty-parent leaf; the visual still
  // drops its own root and ForceRegisters when destroyed after prior detach.
  assert(assets.set_visual(std::uintptr_t{0},error)&&bridge->retained_count()==0);
  assert(!visual->root_identity()&&manager->roots().empty());
  // Reached PF failure occurs after actual attachment. Discard must deliver
  // source animator removal/unlink/drop even before animation setup completed.
  pf_fail=true;assert(!assets.set_visual(model,"",true,error));
  assert(bridge->retained_count()==1&&manager->roots().size()==1);
  auto diagnostic=error;assert(bridge->discard_failed(error)&&error==diagnostic);
  assert(bridge->retained_count()==0&&manager->roots().empty());
  ++count;
 }
 std::cout<<"Actual chest scene-root/visibility/animator removal PASS models="<<count<<"; assetread and PF are declared boundary providers\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
