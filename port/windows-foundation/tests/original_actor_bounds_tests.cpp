#include "original_actor_bounds.hpp"
#include "original_character.hpp"
#include "modular_defaults.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
void check(bool value,const std::string& error){if(!value)throw std::runtime_error(error);}
int main(int argc,char**argv){try{
 check(argc==2,"Original asset root required");AssetCatalog assets(argv[1]);std::string error;
 const auto bytes=assets.read("models/prince_modular.bdae");std::vector<ModularDefaultCategory> categories;check(decode_modular_defaults(bytes,categories,error),error);
 ActorBoundsConfig cfg;cfg.model_path="models/prince_modular.bdae";
 for(const auto& category:categories)for(const auto& id:category.available_controller_ids)if(id.find("_default_warrior-mesh-skin")!=std::string::npos)cfg.components.push_back({id,category.node_id});
 check(cfg.components.size()==4,"Four original rest fixture warrior controllers required");OriginalActorBounds bounds;check(bounds.load(assets,cfg,error),error);
 ActorBoundsPlacement placement;placement.position={1090.75f,-212.202f,258};placement.base_scale={100,100,100};placement.collision_scale=85;
 OriginalActorBoundsResult rest;check(bounds.calculate(placement,nullptr,rest,error),error);check(!rest.marker,"Original warrior fixture unexpectedly has colbox marker");
 const float expected[]{113.699638f,30.755743f,150.669312f};for(unsigned k=0;k<3;++k){check(std::abs(rest.relative_box[k]+expected[k])<.001f&&std::abs(rest.relative_box[k+3]-expected[k])<.001f,"Actual source joint-box/rest owner extent differs from original fixture");}
 check(rest.pf_updates==1,"Source owner bound producer PF request lost");
 CharacterVisual visual;CharacterVisualConfig vc;vc.model_path=cfg.model_path;vc.template_clip_path="animations/prince_template_anim.bdae";vc.clips={{"idle","animations/prince_idle_shield.bdae"}};for(const auto& component:cfg.components)vc.controller_ids.push_back(component.controller_id);vc.motion_node_id="auto";vc.consume_root_motion=true;check(visual.load(assets,vc,error),error);
 SkeletalPose pose;check(visual.current_local_pose(pose,error),error);OriginalActorBoundsResult live;check(bounds.calculate(placement,&pose,live,error),error);
 check(visual.update(.5,error),error);SkeletalPose later;check(visual.current_local_pose(later,error),error);check(bounds.calculate(placement,&later,live,error),error);check(later.size()==pose.size(),"Published current pose graph changed");
 SkeletalPose published;check(visual.sample_local_pose("idle",250,published,error),error);check(visual.apply_local_pose(published,error),error);check(visual.current_local_pose(later,error),error);for(unsigned i=0;i<published.size();++i)check(later[i].translation==published[i].translation&&later[i].quaternion==published[i].quaternion&&later[i].scale==published[i].scale,"Retained blend pose publication absent from live cache");
 auto bad=later;bad.pop_back();check(!visual.apply_local_pose(bad,error),"Invalid current-pose publication accepted");SkeletalPose retained;check(visual.current_local_pose(retained,error),error);for(unsigned i=0;i<later.size();++i)check(retained[i].translation==later[i].translation&&retained[i].quaternion==later[i].quaternion&&retained[i].scale==later[i].scale,"Failed pose apply corrupted live cached SRT");
 OriginalActorBoundsResult moved=rest;check(OriginalActorBounds::update_absolute(moved,{1,2,3},error),error);for(unsigned k=0;k<6;++k)check(moved.absolute_box[k]==rest.relative_box[k]+float(k%3+1),"Source absolute AABB translation differs");check(moved.relative_box==rest.relative_box,"Position update changed cached relative bounds");
 auto invalid=cfg;invalid.components[0].group_node_id="missing-authored-node";check(!bounds.load(assets,invalid,error),"Missing original group accepted");OriginalActorBoundsResult preserved;check(bounds.calculate(placement,nullptr,preserved,error),error);check(preserved.relative_box==rest.relative_box,"Failed reload destroyed original cached bounds");
 std::cout<<"PASS original four selected modular joint-box/rest bounds, source scale/collision properties, absolute AABB position, live local-pose cache and failed mutation preservation\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
