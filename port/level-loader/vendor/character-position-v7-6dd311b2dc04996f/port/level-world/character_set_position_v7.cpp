#include "character_set_position_v7.hpp"
#include <algorithm>
#include <cstring>
namespace dh2::character {
bool CharacterPositionFieldsV7::construct(actor::RuntimeState& runtime,std::string& error){
 if(constructed){error="Character inherited GameObject constructor fields already produced";return false;}constructed=true;
 // GameObjectC2 used by CharacterC1:38c530/534/538 and38c464..46c.
 physical2dc=0;attached2e0=0;std::fill_n(runtime.controller.destination,3,0.f);
 // Source38c5bc..5d0 writes c2c80000/42c80000, then38c5dc UpdateAbsoluteAABB.
 for(unsigned i=0;i<6;++i){runtime.subobjects.local_bounds[i]=i<3?-100.f:100.f;runtime.subobjects.absolute_bounds[i]=runtime.subobjects.local_bounds[i]+runtime.subobjects.position[i%3];}
 // Original nested PFObjectC1 at38c484. No placement/loader override yet.
 dh2_nav_object_defaults(&runtime.object);return true;
}
bool CharacterPositionFieldsV7::adopt(std::uintptr_t physical,std::uintptr_t attached,std::string& error){
 if(constructed){error="Character position source pointer storage already published";return false;}
 physical2dc=physical;attached2e0=attached;constructed=true;return true;
}
namespace {bool missing(const char* name,std::string& error){if(error.empty())error=std::string("Required same Character.SetPosition ")+name;return false;}}
bool character_set_position_v7(CharacterPositionResultV7& result,const CharacterPositionBorrowV7& b,const float* p,bool destination,const CharacterPositionServicesV7& s,std::string& error){
 error.clear();result={};if(!p||!b.receiver||!b.identity||!b.position160||!b.relative144||!b.absolute12c||!b.destination1a8||!b.attached2e0||!b.physical2dc||!b.visual2d8)return missing("source receiver/field borrows",error);
 auto* current=b.position160;
 if(*b.attached2e0){
  if(!s.world||!s.attached_position)return missing("attached+2e0 position producer",error);
  float* xyz=nullptr;if(!s.attached_position(*b.attached2e0,xyz,error))return false;if(!xyz)return missing("attached+0c XYZ",error);
  volatile float dy=p[1]-current[1],dz=p[2]-current[2],dx=p[0]-current[0];
  xyz[0]=xyz[0]+dx;xyz[1]=xyz[1]+dy;xyz[2]=xyz[2]+dz;
 }
 result.phase=CharacterPositionPhaseV7::attached;
 // Exact sequential reads, including p==attached.xyz or current storage.
 current[0]=p[0];current[1]=p[1];current[2]=p[2];
 if(b.publish_position)b.publish_position();result.phase=CharacterPositionPhaseV7::current;
 // Original captures all relative words, copies six, then adds ownerXYZ.
 float relative[6];std::copy_n(b.relative144,6,relative);std::copy_n(relative,6,b.absolute12c);
 for(unsigned i=0;i<6;++i)b.absolute12c[i]=relative[i]+current[i%3];
 result.phase=CharacterPositionPhaseV7::bounds;
 if(*b.physical2dc){if(!s.world||!s.physical_position)return missing("physical2dc.setPosition46ea80",error);if(!s.physical_position(*b.physical2dc,current[0],current[1],error))return false;}
 result.phase=CharacterPositionPhaseV7::physical;
 // Source rereads visual pointer after physical callback.
 if(*b.visual2d8){if(!s.world||!s.visual_sync_position)return missing("visual2d8.SyncPosition470cb8",error);if(!s.visual_sync_position(*b.visual2d8,error))return false;}
 result.phase=CharacterPositionPhaseV7::visual;
 if(destination){b.destination1a8[0]=p[0];b.destination1a8[1]=p[1];b.destination1a8[2]=p[2];result.phase=CharacterPositionPhaseV7::destination;}
 result.phase=CharacterPositionPhaseV7::complete;return true;
}
}
