#include "world_touch_projection_v1.hpp"
#include <cmath>
namespace dh2::world {
bool world_touch_projection_v1(float x,float y,const WorldTouchProjectionServicesV1& s,bool& hit,std::array<float,3>& point,std::string& e){
 hit=false;if(!std::isfinite(x)||!std::isfinite(y)||double(x)<-2147483648.||double(x)>=2147483648.||double(y)<-2147483648.||double(y)>=2147483648.){e="required source screen conversion domain";return false;}
 if(!s.screen_ray){e="required actual camera screen ray";return false;}std::array<float,3> start,end;if(!s.screen_ray(static_cast<std::int32_t>(x),static_cast<std::int32_t>(y),start,end,e))return false;
 if(!s.room_at){e="required actual PFWorld room vector";return false;}
 for(std::uint32_t room_index=0;;++room_index){bool present=false;std::uintptr_t room=0;if(!s.room_at(room_index,present,room,e))return false;if(!present)return true;if(!room){e="required original PFRoom identity";return false;}
  if(!s.floor_at){e="required actual PFRoom floor vector";return false;}
  for(std::uint32_t index=0;;++index){WorldTouchFloorBorrowV1 floor;if(!s.floor_at(room,index,present,floor,e))return false;if(!present)break;if(!floor.identity||!floor.flags24){e="required actual PFFloor fields";return false;}
   // Source TranslateScreen passes includeSpecial=false. PFRoom5212e0 skips
   // floorflags0x03000000 before invoking actual floor collision.
   if(*floor.flags24&0x03000000u)continue;
   if(!s.floor_collision){e="required actual floor triangle-selector raycast";return false;}bool intersects=false;std::array<float,3> result;if(!s.floor_collision(floor.identity,start,end,intersects,result,e))return false;if(intersects){point=result;hit=true;return true;}
  }
 }
}
}
