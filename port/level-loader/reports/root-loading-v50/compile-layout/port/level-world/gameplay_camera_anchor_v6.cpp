#include "gameplay_camera_anchor_v6.hpp"
namespace dh2::camera {
bool source_camera_anchor_v6(const CameraAnchorBorrowV6& b,PointV2& out,std::string& e){
 if(!b.actor_lease||!b.identity||!b.anchor2e0){e="Required same actor camera anchor slot2e0";return false;}const float* position=b.position160;
 if(*b.anchor2e0){if(!b.anchor_position){e="Required actual retained AnchorBase positionc";return false;}if(!b.anchor_position(*b.anchor2e0,position,e))return false;}
 if(!position){e="Required source camera anchor position";return false;}for(unsigned n=0;n<3;++n)out[n]=position[n];return true;
}
bool source_set_camera_anchor_v6(CameraAnchorBorrowV6& b,std::uintptr_t value,std::string& e){
 if(!b.actor_lease||!b.identity||!b.anchor2e0){e="Required same actor camera anchor slot2e0";return false;}if(*b.anchor2e0==value)return true;
 if(*b.anchor2e0){if(!b.delete_anchor){e="Required original camera anchor deleting destructor";return false;}if(!b.delete_anchor(*b.anchor2e0,e))return false;*b.anchor2e0=0;}
 *b.anchor2e0=value;return true;
}
}
