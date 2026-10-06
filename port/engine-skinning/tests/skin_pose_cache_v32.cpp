#include "../visual_skin_owner_v6.hpp"
#include "../../engine-animation/animation.hpp"
#include "../../level-world/objects.hpp"
#include <fstream>
#include <iostream>
#include <chrono>
#include <cstring>
#include <limits>
#include <algorithm>
#include <cctype>
using namespace dh2;using namespace dh2::skinning;
static unsigned checks;
static void ck(bool value,const std::string& error){if(!value)throw std::runtime_error(error);++checks;}
static std::vector<std::uint8_t> read(const char* path){std::ifstream f(path,std::ios::binary);if(!f)throw std::runtime_error(path);return {std::istreambuf_iterator<char>(f),{}};}
static VisualAssetResultV6 weapon_asset(void*,const char* name,std::vector<std::uint8_t>& bytes,std::string& error){
 try{std::string uri=name;uri=uri.substr(uri.find_last_of('/')+1);for(auto& c:uri)c=char(std::tolower(static_cast<unsigned char>(c)));
  const auto path=".local-inputs/visual-skin-owner-v6/weapons/"+uri;bytes=read(path.c_str());return VisualAssetResultV6::found;
 }catch(const std::exception& e){error=e.what();return VisualAssetResultV6::failed;}
}
static void same(const std::vector<VisualDrawPartV6>& old,const std::vector<VisualDrawViewV32>& now){
 ck(old.size()==now.size(),"Part count differs");
 for(unsigned i=0;i<old.size();++i){const auto& a=old[i];const auto& b=now[i];
  ck(a.geometry==b.geometry&&a.material_table==b.material_table&&a.materials==b.materials&&
     a.category==b.category&&a.module==b.module&&a.weapon_slot==b.weapon_slot&&a.skinned==b.skinned,"Part identity differs");
  ck(a.world==b.world&&a.positions.size()==b.positions->size(),"Part matrix/dimensions differ");
  ck(!std::memcmp(a.positions.data(),b.positions->data(),a.positions.size()*sizeof(a.positions[0])),"Skin output is not bit-identical");
 }
}
int main(int argc,char** argv){try{
 ck(argc==3,"Usage model authored_clip");std::string error;
 VisualSkinResourcesV6 resource;ck(resource.load(read(argv[1]),error),error);
 auto scene=resource.borrow().factory_scene();
 VisualSkinOwnerV6 owner(resource.borrow(),scene,{nullptr,weapon_asset});ck(owner.initialize(error),error);
 animation::Player clip;auto bytes=read(argv[2]);ck(clip.load(bytes.data(),bytes.size(),scene,error,animation::MissingTargets::ignore),error);
 // Retain the legacy sampler as an independent in-build reference. Compare
 // every local/world value and instance, including clip endpoints and seeks.
 auto reference_scene=scene,reused_scene=scene;animation::PoseSampleWorkspaceV32 workspace;
 const auto* graph_storage=reused_scene.graph.data();const auto* material_storage=reused_scene.materials.data();
 const auto equal_scene=[&](const scene::Scene& a,const scene::Scene& b){
  ck(a.graph.size()==b.graph.size()&&a.instances.size()==b.instances.size(),"Pose dimensions differ");
  for(unsigned i=0;i<a.graph.size();++i){const auto& x=a.graph[i];const auto& y=b.graph[i];
   ck(x.id==y.id&&x.sid==y.sid&&x.parent==y.parent,"Pose metadata differs");
   ck(!std::memcmp(x.translation,y.translation,sizeof(x.translation))&&!std::memcmp(x.quaternion,y.quaternion,sizeof(x.quaternion))&&
      !std::memcmp(x.scale,y.scale,sizeof(x.scale))&&!std::memcmp(x.world.data(),y.world.data(),sizeof(x.world)),"Pose numeric bytes differ");
  }
  for(unsigned i=0;i<a.instances.size();++i)ck(a.instances[i].world==b.instances[i].world,"Instance world differs");
 };
 for(unsigned frame=0;frame<128;++frame){const auto ms=clip.start+(clip.end-clip.start)*(frame%65)/64;
  ck(clip.sample(reference_scene,ms,error),error);ck(clip.sample_reuse(reused_scene,ms,workspace,error),error);equal_scene(reference_scene,reused_scene);
 }
 ck(workspace.storage_growths==1&&workspace.calls==128,"Numeric sampler reallocates");
 ck(reused_scene.graph.data()==graph_storage&&reused_scene.materials.data()==material_storage,"Numeric sample replaces graph/material storage");
 const auto failed_sample=[&](scene::Scene bad){auto saved=bad;std::string expected,actual;
  auto oracle=bad;ck(!clip.sample(oracle,clip.start,expected),"Reference accepted malformed pose");
  ck(!clip.sample_reuse(bad,clip.start,workspace,actual)&&actual==expected,"Pose error branch differs");equal_scene(saved,bad);
 };
 auto malformed=scene;malformed.graph[0].parent=0;failed_sample(malformed);
 malformed=scene;malformed.graph[0].id="wrong";failed_sample(malformed);
 malformed=scene;malformed.instances[0].node_index=unsigned(scene.graph.size());failed_sample(malformed);
 // Whole object sampler retains its original explicit rigid fallback for an
 // unweighted fixture vertex; the immutable cache itself continues to return
 // the original software kernel's zero. It must never modify cached output.
 objects::Resource object;object.scene=scene;object.rest_scene=scene;
 ck(object.animation.load(bytes.data(),bytes.size(),object.scene,error,animation::MissingTargets::ignore),error);
 const auto& source_part=resource.borrow().categories()[0].modules[0].part;
 objects::Primitive primitive;primitive.node=resource.borrow().modular_node();primitive.skin=source_part.skin;
 primitive.rest_positions=source_part.geometry.positions;primitive.vertices.resize(primitive.rest_positions.size());
 primitive.skin.influences[0].weights.fill(0.f);object.primitives.push_back(std::move(primitive));
 auto object_reference=object.scene;
 for(unsigned frame=0;frame<32;++frame){const auto ms=clip.start+(clip.end-clip.start)*frame/32;
  ck(object.animation.sample(object_reference,ms,error),error);ck(objects::sample(object,ms,error),error);equal_scene(object_reference,object.scene);
  auto& p=object.primitives[0];std::vector<Matrix> matrices;std::vector<world::Point> output;
  ck(skinning::palette(p.skin,object_reference,matrices,error)&&skinning::positions(p.skin,matrices,p.rest_positions,output,error),error);
  for(unsigned i=0;i<output.size();++i){if(p.skin.influences[i].weights[0]==0){const auto& m=object_reference.graph[p.node].world;
    for(unsigned row=0;row<3;++row){output[i][row]=m[12+row];for(unsigned col=0;col<3;++col)output[i][row]+=m[col*4+row]*p.rest_positions[i][col];}}
   ck(!std::memcmp(output[i].data(),p.vertices[i].p,sizeof(p.vertices[i].p)),"Whole object sampler differs");
  }
  ck(p.pose_cache_v32.positions()[0]==world::Point{},"Rigid fallback corrupted cache output");
 }
 std::vector<VisualDrawPartV6> old;const std::vector<VisualDrawViewV32>* views=nullptr;
 const auto compare=[&](){ck(owner.draw_parts(old,error),error);ck(owner.draw_views(views,error)&&views,error);same(old,*views);};
 compare();const auto first=owner.pose_counters();ck(first.deformations==4,"First draw must deform all four modules");
 auto retained=old;const auto* stream=(*views)[0].positions->data();
 for(unsigned i=0;i<300;++i){ck(owner.draw_views(views,error),error);ck((*views)[0].positions->data()==stream,"Frozen pose buffer changed");}
 const auto hits=owner.pose_counters();ck(hits.deformations==first.deformations&&hits.hits==1200,"Frozen pose was recomputed");
 ck(hits.storage_growths==first.storage_growths,"Frozen pose allocated cache storage");
 // Every real module, with selected source pose and exact existing kernel as
 // the parity oracle. Includes unequip and same-module notification semantics.
 unsigned modules=0;
 for(unsigned c=0;c<resource.borrow().categories().size();++c){
  for(unsigned m=0;m<resource.borrow().categories()[c].modules.size();++m){
   ck(owner.set_modular(c,m,error),error);compare();++modules;
  }
 }
 ck(modules==172,"Actual module corpus differs");
 for(unsigned c=0;c<4;++c)ck(owner.set_modular(c,0,error),error);
 for(unsigned frame=0;frame<64;++frame){
  const auto ms=clip.start+(clip.end-clip.start)*frame/64;
  ck(clip.sample(scene,ms,error),error);compare();
 }
 const auto warm=owner.pose_counters();
 for(unsigned frame=0;frame<200;++frame){
  ck(clip.sample(scene,clip.start+(clip.end-clip.start)*(frame%64)/64,error),error);
  ck(owner.draw_views(views,error),error);
 }
 ck(owner.pose_counters().storage_growths==warm.storage_growths,"Animated pose grew warmed storage");
 // No frame-number shortcut: mutate one contributing world matrix directly.
 auto& matrix=scene.graph[resource.borrow().categories()[0].modules[0].part.skin.nodes[0]].world;
 const auto save_matrix=matrix;matrix[12]+=1.f;compare();matrix=save_matrix;compare();
 // Failures do not publish an invalid pose or fabricate a cache hit.
 const auto& part=resource.borrow().categories()[0].modules[0].part;
 SkinPoseCacheV32 cache;cache.bind(part.skin,part.geometry.positions);ck(cache.sample(scene,error),error);
 auto saved_positions=cache.positions();matrix[0]=std::numeric_limits<float>::infinity();
 ck(!cache.sample(scene,error)&&cache.positions()==saved_positions,"Failure lost prior positions");matrix=save_matrix;
 ck(cache.sample(scene,error)&&cache.positions()==saved_positions,"Recovery differs");
 auto bad=part.skin;bad.nodes[0]=unsigned(scene.graph.size());cache.bind(bad,part.geometry.positions);
 ck(!cache.sample(scene,error)&&error=="Skin joint out of range","Invalid node accepted");
 cache.reset();ck(!cache.sample(scene,error),"Unbound cache accepted");
 ck(retained[0].positions.size()>0,"Owned legacy snapshot lifetime changed");
 auto broken=scene;broken.graph[0].sid="mutated";std::swap(broken,scene);
 ck(!owner.draw_views(views,error)&&!views,"Invalid live graph published");std::swap(broken,scene);
 compare();
 ck(owner.set_weapon("mc_rweapon_longsword_01",1,1,error),error);compare();
 ck(views->size()>4,"Actual anchored weapon absent");
 const auto retained_weapon=views->back();
 ck(retained_weapon.positions==&retained_weapon.geometry->positions&&!retained_weapon.positions_changed,"Rigid weapon stream copied");
 for(unsigned frame=0;frame<8;++frame){ck(clip.sample(scene,clip.start+(clip.end-clip.start)*frame/8,error),error);compare();}
 ck(owner.set_weapon(nullptr,1,1,error),error);compare();
 ck(!retained_weapon.positions->empty()&&retained_weapon.geometry->positions.data()==retained_weapon.positions->data(),"Weapon retained geometry lifetime differs");
 // Representative CPU skin workload: actual four-part modular player, twelve
 // independently owned poses, changing authored clip every frame. Sampling is
 // outside the timed region in both cases. This is a CPU benchmark, not FPS.
 std::vector<scene::Scene> population(12,scene);std::vector<std::unique_ptr<VisualSkinOwnerV6>> actors;
 for(auto& pose:population){actors.push_back(std::make_unique<VisualSkinOwnerV6>(resource.borrow(),pose,VisualAssetServicesV6{}));ck(actors.back()->initialize(error),error);}
 using Clock=std::chrono::steady_clock;long long old_ns=0,new_ns=0,old_pose_ns=0,new_pose_ns=0;
 std::vector<animation::PoseSampleWorkspaceV32> workspaces(population.size());
 for(unsigned pass=0;pass<2;++pass)for(unsigned frame=0;frame<240;++frame){
  const auto ms=clip.start+(clip.end-clip.start)*(frame%64)/64;const auto start=Clock::now();
  for(unsigned i=0;i<population.size();++i){
   const bool ok=pass?clip.sample_reuse(population[i],ms,workspaces[i],error):clip.sample(population[i],ms,error);
   if(!ok)throw std::runtime_error(error);
  }
  const auto ns=std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now()-start).count();
  (pass?new_pose_ns:old_pose_ns)+=ns;
 }
 for(unsigned pass=0;pass<2;++pass)for(unsigned frame=0;frame<240;++frame){
  const auto ms=clip.start+(clip.end-clip.start)*(frame%64)/64;
  for(auto& pose:population)ck(clip.sample(pose,ms,error),error);
  const auto start=Clock::now();
  for(auto& actor:actors){if(!pass){if(!actor->draw_parts(old,error))throw std::runtime_error(error);}else if(!actor->draw_views(views,error))throw std::runtime_error(error);}
  const auto ns=std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now()-start).count();
  (pass?new_ns:old_ns)+=ns;
 }
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"actual_modules\":"<<modules
          <<",\"parity\":\"bit-identical\",\"numeric_pose_parity_frames\":128,\"whole_object_parity_frames\":32,\"frozen_hits\":"<<hits.hits
          <<",\"animated_postwarm_storage_growths\":0,\"benchmark_actors\":12,\"benchmark_frames\":240"
          <<",\"legacy_skin_ns\":"<<old_ns<<",\"cached_skin_ns\":"<<new_ns
          <<",\"cpu_ratio\":"<<double(old_ns)/double(new_ns)
          <<",\"legacy_pose_ns\":"<<old_pose_ns<<",\"reused_pose_ns\":"<<new_pose_ns
          <<",\"pose_cpu_ratio\":"<<double(old_pose_ns)/double(new_pose_ns)<<"}\n";
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
