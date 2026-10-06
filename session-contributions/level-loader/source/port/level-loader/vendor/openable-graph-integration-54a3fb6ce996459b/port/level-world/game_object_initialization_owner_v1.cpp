#include "game_object_initialization_owner_v1.hpp"
#include <cmath>
#include <cstring>
namespace dh2::world {
namespace {
float source_float(std::uint32_t bits){float f;std::memcpy(&f,&bits,4);return f;}
bool find_name(const std::vector<std::string>* names,const std::string& name,std::int32_t& id,std::string& e){
 if(!names){e="Required original shared string catalog";return false;}
 id=-1;for(std::size_t i=0;i<names->size();++i)if((*names)[i]==name){id=static_cast<std::int32_t>(i);break;}return true;
}
}
bool GameObjectInitializationOwnerV1::missing(const char* text,std::string& e)const{e=std::string("Required original GameObject ")+text;return false;}
bool GameObjectInitializationOwnerV1::spawn(std::int32_t& roll,std::string& e){
 if(!services_.owner||!services_.check_spawn_probability)return missing("CheckSpawnProbability owner",e);
 return services_.check_spawn_probability(roll,e);
}
bool GameObjectInitializationOwnerV1::object_base_init_post(std::string& e){
 e.clear();if(!services_.owner||!services_.condition_init)return missing("ConditionData::Init provider",e);
 if(!services_.condition_init(0x8c,e)||!services_.condition_init(0xb0,e))return false;
 auto* name=base_.string(0xd4);if(!name)return missing("min_difficulty source field",e);
 if(!name->empty()){
  std::int32_t id;if(!find_name(services_.difficulty_names,*name,id,e))return false;
  auto* target=base_.integer(0xec);if(!target)return missing("difficulty id+ec field",e);*target=id;
 }
 return true;
}
bool GameObjectInitializationOwnerV1::init_post(bool& eligible,std::string& e){
 eligible=false;if(!object_base_init_post(e))return false;
 std::int32_t roll;if(!spawn(roll,e))return false;
 auto* probability=base_.integer(0x274);if(!probability)return missing("spawn probability+274 field",e);
 if(roll>=*probability)return true;eligible=true;
 auto* physical=base_.pointer(0x2dc);auto* scale=base_.vector3(0x120);auto* rotation=base_.vector3(0x16c);
 auto* heading=base_.scalar(0x178);auto* position=base_.vector3(0x160);
 if(!physical||!scale||!rotation||!heading||!position)return missing("InitPost source transform fields",e);
 *physical=0;
 for(unsigned i=0;i<3;++i)if(std::fabs(scale[i])<source_float(0x38d1b717))scale[i]=1.f;
 for(unsigned i=0;i<3;++i)rotation[i]=rotation[i]*source_float(0x3c8efa35);
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
 if(*skip){auto* checkpoint=base_.byte(0x28);if(!checkpoint)return missing("checkpoint+28 field",e);*checkpoint=0;}
 auto* sound=base_.string(0x358);if(!sound)return missing("idle_sound+358 field",e);
 if(!sound->empty()){
  std::int32_t id;if(!find_name(services_.sound_names,*sound,id,e))return false;
  auto* target=base_.integer(0x370);if(!target)return missing("idle sound id+370 field",e);
  // Original stores a halfword, read subsequently as signed short.
  *target=static_cast<std::int16_t>(static_cast<std::uint16_t>(id));
 }
 return true;
}
bool GameObjectInitializationOwnerV1::init_final(bool& eligible,std::string& e){
 e.clear();eligible=false;std::int32_t roll;if(!spawn(roll,e))return false;
 auto* probability=base_.integer(0x274);if(!probability)return missing("spawn probability+274 field",e);
 if(roll>=*probability)return true;
 auto* disabled=base_.byte(0x81);if(!disabled)return missing("disabled+81 field",e);if(*disabled)return true;
 eligible=true;auto* always=base_.byte(0x2ed);if(!always)return missing("alwaysVisible+2ed field",e);
 if(*always){if(!services_.set_visible)return missing("SetVisible(true) virtual provider",e);if(!services_.set_visible(true,e))return false;}
 auto* box=base_.relative_aabb144();const float x=box[3]-box[0],y=box[4]-box[1];
 const float radius=x<y?y:x;auto* stat=base_.byte(0x84);auto* position=base_.vector3(0x160);
 if(!stat||!position||!services_.init_pf_object)return missing("PFWorld::InitObject provider and fields",e);
 if(!services_.init_pf_object(*stat!=0,position,radius,base_.identity(),e))return false;
 auto* archetype=base_.string(0x48);auto* pfname=base_.string(0x254);
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
