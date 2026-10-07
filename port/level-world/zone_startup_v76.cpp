#include "zone_startup_v76.hpp"
#include <cstring>
namespace dh2::world {
namespace {
float flip_zone_sign_v76(float value){std::uint32_t bits;std::memcpy(&bits,&value,4);bits^=0x80000000u;std::memcpy(&value,&bits,4);return value;}
bool required_zone_v76(const char* name,std::string& e){e=std::string("Required actual Zone startup ")+name;return false;}
}
bool zone_init_post_v76(CanonicalGameObjectBaseOwnerV1& base,GameObjectInitializationOwnerV1& initialization,
 const GameObjectInitializationServicesV1& source,std::array<float,3>& dimensions,
 const bool& physical,const bool& trigger,std::uintptr_t& colzone,std::shared_ptr<void>& colzone_lease,
 const ZoneStartupServicesV76& leaves,std::string& e){
 e.clear();std::int32_t roll{};
 if(!source.owner||!source.check_spawn_probability)return required_zone_v76("CheckSpawnProbability38bd64",e);
 if(!source.check_spawn_probability(roll,e))return false;
 // Original rereads274 AFTER the callback, not a cached probability receipt.
 const auto* probability=base.integer(0x274);if(!probability)return required_zone_v76("SAME spawn probability274",e);
 if(roll>=*probability)return true;
 bool eligible{};if(!initialization.init_post(eligible,e))return false;
 // Original does not inspect the GameObject eligibility result here. Its
 // second cached roll and all of its reached stores remain the original body.
 auto* scale=base.vector3(0x120);auto* box=base.relative_aabb144();
 if(!scale||!box)return required_zone_v76("SAME scale120/relative144 storage",e);
 //397764/778/78c: finish ALL dimensions stores before the first half-box.
 dimensions[0]=dimensions[0]*scale[0];
 dimensions[1]=dimensions[1]*scale[1];
 dimensions[2]=dimensions[2]*scale[2];
 const float x=dimensions[0]*0.5f;box[0]=flip_zone_sign_v76(x);box[3]=x;
 const float y=dimensions[1]*0.5f;box[1]=flip_zone_sign_v76(y);box[4]=y;
 const float z=dimensions[2]*0.5f;box[5]=z;box[2]=flip_zone_sign_v76(z);
 if(!leaves.owner||!leaves.set_bounding_box)return required_zone_v76("virtual9c exact144,false",e);
 if(!leaves.set_bounding_box(base,box,false,e))return false;
 if(physical){
  if(!leaves.create_zone_physical)return required_zone_v76("genuine Zone physical C1/assignment",e);
  if(!leaves.create_zone_physical(base,trigger,e))return false;
 }
 if(colzone)return true;
 const auto* visual=base.pointer(0x2d8);if(!visual)return required_zone_v76("SAME visual2d8 slot",e);
 if(!*visual)return true;
 if(!leaves.bind_visual_collision_zone)return required_zone_v76("genuine _colzone node/mesh/selector resources",e);
 if(!leaves.bind_visual_collision_zone(base,"_colzone",colzone,colzone_lease,e))return false;
 if(colzone){if(!colzone_lease||colzone_lease.get()!=reinterpret_cast<void*>(colzone))return required_zone_v76("SAME published384 node lease",e);}
 else if(colzone_lease)return required_zone_v76("actual NULL _colzone lookup without foreign node lease",e);
 return true;
}
}
