#pragma once
#include "game_object_initialization_services_v51.hpp"
#include <cmath>
#include <cstring>
namespace dh2::world {
// One source control kernel. Fields lends actual storage; it constructs no
// CanonicalBase, pose, visual, condition, save or eligibility/stage cache.
template<class Fields> class GameObjectInitializationKernelV51 {
 Fields& base_;const GameObjectInitializationServicesV1& services_;
 bool missing(const char*,std::string&)const;
 bool spawn(std::int32_t&,std::string&);
public:
 GameObjectInitializationKernelV51(Fields& f,const GameObjectInitializationServicesV1& s):base_(f),services_(s){}
 bool object_base_init_post(std::string&);
 bool init_post(bool&,std::string&);
 bool init_final(bool&,std::string&);
};
namespace parent_kernel_detail_v51 {
inline float source_float(std::uint32_t bits){float f;std::memcpy(&f,&bits,4);return f;}
inline bool find_name(const std::vector<std::string>* names,const std::string& name,std::int32_t& id,std::string& e){
 if(!names){e="Required original shared string catalog";return false;}
 id=-1;for(std::size_t i=0;i<names->size();++i)if((*names)[i]==name){id=static_cast<std::int32_t>(i);break;}return true;
}
}
template<class Fields>
bool GameObjectInitializationKernelV51<Fields>::missing(const char* text,std::string& e)const{e=std::string("Required original GameObject ")+text;return false;}
template<class Fields>
bool GameObjectInitializationKernelV51<Fields>::spawn(std::int32_t& roll,std::string& e){
 if(!services_.owner||!services_.check_spawn_probability)return missing("CheckSpawnProbability owner",e);
 return services_.check_spawn_probability(roll,e);
}
template<class Fields>
bool GameObjectInitializationKernelV51<Fields>::object_base_init_post(std::string& e){
 e.clear();if(!services_.owner||!services_.condition_init)return missing("ConditionData::Init provider",e);
 if(!services_.condition_init(0x8c,e)||!services_.condition_init(0xb0,e))return false;
 auto* name=base_.string(0xd4);if(!name)return missing("min_difficulty source field",e);
 if(!name->empty()){
  std::int32_t id;if(!parent_kernel_detail_v51::find_name(services_.difficulty_names,*name,id,e))return false;
  auto* target=base_.difficulty_ec();if(!target)return missing("difficulty id+ec field",e);*target=id;
 }
 return true;
}
template<class Fields>
bool GameObjectInitializationKernelV51<Fields>::init_post(bool& eligible,std::string& e){
 eligible=false;if(!object_base_init_post(e))return false;
 std::int32_t roll;if(!spawn(roll,e))return false;
 auto* probability=base_.probability274();if(!probability)return missing("spawn probability+274 field",e);
 if(roll>=*probability)return true;eligible=true;
 auto* physical=base_.pointer(0x2dc);auto* scale=base_.vector3(0x120);auto* rotation=base_.vector3(0x16c);
 auto* heading=base_.scalar(0x178);auto* position=base_.vector3(0x160);
 if(!physical||!scale||!rotation||!heading||!position)return missing("InitPost source transform fields",e);
 *physical=0;
 for(unsigned i=0;i<3;++i)if(std::fabs(scale[i])<parent_kernel_detail_v51::source_float(0x38d1b717))scale[i]=1.f;
 for(unsigned i=0;i<3;++i)rotation[i]=rotation[i]*parent_kernel_detail_v51::source_float(0x3c8efa35);
 *heading=rotation[2];
 if(!services_.set_position)return missing("SetPosition(true) provider",e);
 if(!services_.set_position(position,true,e))return false;
 auto* box=base_.relative_aabb144();for(unsigned i=0;i<6;++i)box[i]=box[i]*scale[i%3];
 base_.update_absolute_aabb();
 auto* visual=base_.pointer(0x2d8);if(!visual)return missing("VisualObject+2d8 field",e);
 if(!*visual){
  bool high;if(!services_.device_high_performance)return missing("Device::IsHighPerformance provider",e);
  if(!services_.device_high_performance(high,e))return false;
  bool omit=false;
  if(!high){auto* facultative=base_.byte(0x60);if(!facultative)return missing("facultative+60 field",e);
   if(*facultative){auto* optimization=base_.byte(0x10c);if(!optimization)return missing("optimization+10c field",e);omit=*optimization!=0;}}
  if(!omit){if(!services_.load_visual)return missing("LoadVisualObject provider",e);if(!services_.load_visual(e))return false;}
 }
 if(*visual){if(!services_.visual_sync)return missing("VisualObject::Sync provider",e);if(!services_.visual_sync(*visual,e))return false;}
 auto* skip=base_.byte(0x15c);if(!skip)return missing("skip_checkpoint+15c field",e);
 if(*skip){if(!base_.clear_checkpoint(e))return false;}
 auto* sound=base_.string(0x358);if(!sound)return missing("idle_sound+358 field",e);
 if(!sound->empty()){
  std::int32_t id;if(!parent_kernel_detail_v51::find_name(services_.sound_names,*sound,id,e))return false;
  // Adapter writes only the actual owner's original signed-halfword value.
  if(!base_.store_idle_sound16(id,e))return false;
 }
 return true;
}
template<class Fields>
bool GameObjectInitializationKernelV51<Fields>::init_final(bool& eligible,std::string& e){
 e.clear();eligible=false;std::int32_t roll;if(!spawn(roll,e))return false;
 auto* probability=base_.probability274();if(!probability)return missing("spawn probability+274 field",e);
 if(roll>=*probability)return true;
 auto* disabled=base_.byte(0x81);if(!disabled)return missing("disabled+81 field",e);if(*disabled)return true;
 eligible=true;auto* always=base_.byte(0x2ed);if(!always)return missing("alwaysVisible+2ed field",e);
 if(*always){if(!services_.set_visible)return missing("SetVisible(true) virtual provider",e);if(!services_.set_visible(true,e))return false;}
 auto* box=base_.relative_aabb144();const float x=box[3]-box[0],y=box[4]-box[1];
 const float radius=x<y?y:x;auto* stat=base_.byte(0x84);auto* position=base_.vector3(0x160);
 if(!stat||!position||!services_.init_pf_object)return missing("PFWorld::InitObject provider and fields",e);
 if(!services_.init_pf_object(*stat!=0,position,radius,base_.identity(),e))return false;
 auto* archetype=base_.string(0x48);auto* pfname=base_.pf_name254();
 if(!archetype||!pfname)return missing("PF name/archetype source fields",e);*pfname=*archetype;
 auto* visual=base_.pointer(0x2d8);if(!visual)return missing("VisualObject+2d8 field",e);
 if(*visual){
  if(!services_.visual_sync)return missing("VisualObject::Sync provider",e);if(!services_.visual_sync(*visual,e))return false;
  auto* light=base_.string(0x278);if(!light||!services_.light_set_id||!services_.visual_set_light_set)return missing("LightSetManager source provider",e);
  std::int32_t id;if(!services_.light_set_id(*light,id,e)||!services_.visual_set_light_set(*visual,id,e))return false;
  // The light-set callback may synchronously remove the visual; reread it.
  if(*visual){std::uintptr_t root;if(!services_.visual_root)return missing("VisualObject root field provider",e);
   if(!services_.visual_root(*visual,root,e))return false;
   if(root){std::uintptr_t node;if(!services_.node_from_name)return missing("SceneNode name lookup provider",e);
    if(!services_.node_from_name(root,"target_node",node,e))return false;
    auto* target=base_.pointer(0x180);if(!target)return missing("target_node+180 field",e);*target=node;}}
 }
 if(!services_.update_pf_object)return missing("UpdatePFObject provider",e);
 return services_.update_pf_object(e);
}
}
