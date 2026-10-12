#include "game_object_relative_box_v3.hpp"
namespace dh2::world {
bool game_object_relative_box_v3(CanonicalGameObjectBaseOwnerV1& base,const float* input,
 const std::function<bool(std::string&)>& update,std::string& error){
 error.clear();if(!input){error="Required actual relative AABB input";return false;}
 auto* box=base.relative_aabb144();
 // Ordered reads/stores preserve exact input==destination aliasing.
 for(unsigned i=0;i<6;++i)box[i]=input[i];
 volatile float width=box[3]-box[0];
 if(width==0.f){volatile float height=box[4]-box[1];if(height==0.f){auto* flat=base.byte(0x2f9);if(!flat){error="Required SAME flat-byte constructor field";return false;}*flat=1;}}
 if(width<10.f){box[0]=box[0]-5.f;box[3]=box[3]+5.f;}
 volatile float height=box[4]-box[1];if(height<10.f){box[1]=box[1]-5.f;box[4]=box[4]+5.f;}
 base.update_absolute_aabb();
 if(!update){error="Required actual UpdatePFObject after SetRelativeAABB prefix";return false;}
 return update(error);
}
}
