#include "game_object_set_position_v2.hpp"
#include <cstring>
namespace dh2::world {
namespace {bool required(const char* name,std::string& e){e=std::string("Required source GameObject.SetPosition ")+name;return false;}}
bool game_object_set_position_v2(CanonicalGameObjectBaseOwnerV1& b,const float* p,bool destination,const GameObjectSetPositionServicesV2& s,std::string& e){
 if(!p)return required("position input",e);
 auto* current=b.vector3(0x160);auto* attached=b.pointer(0x2e0);auto* physical=b.pointer(0x2dc);auto* visual=b.pointer(0x2d8);
 if(!current||!attached||!physical||!visual)return required("same canonical fields",e);
 // Do not snapshot p: source rereads an alias after each attached-object store.
 if(*attached){
  if(!s.owner||!s.attached_position)return required("attached object +0c position",e);
  float* xyz=nullptr;if(!s.attached_position(*attached,xyz,e))return false;
  if(!xyz)return required("attached object retained position",e);
  const float dy=p[1]-current[1],dz=p[2]-current[2],dx=p[0]-current[0];
  xyz[0]=xyz[0]+dx;xyz[1]=xyz[1]+dy;xyz[2]=xyz[2]+dz;
 }
 current[0]=p[0];current[1]=p[1];current[2]=p[2];
 b.update_absolute_aabb();
 if(*physical){if(!s.owner||!s.physical_position)return required("actual physical.setPosition",e);if(!s.physical_position(*physical,current[0],current[1],e))return false;}
 if(*visual){if(!s.owner||!s.visual_sync)return required("actual visual.Sync",e);if(!s.visual_sync(*visual,e))return false;}
 if(destination){auto* target=b.runtime().controller.destination;target[0]=p[0];target[1]=p[1];target[2]=p[2];}
 return true;
}
}
