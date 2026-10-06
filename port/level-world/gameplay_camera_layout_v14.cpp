#include "gameplay_camera_layout_v14.hpp"
#include <cmath>
#include <cstring>
extern "C" float dh2_camera_margin_v14(const float* screen,const float* bounds,std::uint32_t count){
 std::uint32_t raw=0x7f7fffff;float margin;std::memcpy(&margin,&raw,4);
 for(unsigned i=0;i<count;++i){const float x=screen[i*2],y=screen[i*2+1];const float sides=bounds[0]-std::fabs(x);if(sides<margin)margin=sides;const float top=bounds[1]-y;if(margin>top)margin=top;const float bottom=y+bounds[2];if(margin>bottom)margin=bottom;}return margin;
}
extern "C" float dh2_camera_centering_shift_v14(std::int32_t count,const std::uint8_t slots[4]){
 float shift=0;if(count==2){if(slots[0])shift=-.5f;if(slots[1])shift=shift+.5f;return shift;}
 if(count<=1)return shift;if(slots[0])shift=-.25f;if(slots[1])shift=shift+.25f;if(slots[2])shift=shift-.25f;if(count==4&&slots[3])shift=shift+.25f;return shift;
}
extern "C" void dh2_camera_autozoom_arithmetic_v14(float* zoom,const float* screen,const float* bounds,std::uint32_t count){
 const float margin=dh2_camera_margin_v14(screen,bounds,count);const float difference=std::fabs(bounds[3]-margin);
 if(bounds[3]>margin){const float delta=difference*bounds[4];*zoom=*zoom-delta;}
 else if(bounds[3]<margin){const float delta=difference*bounds[4];*zoom=*zoom+delta;}
}
namespace dh2::camera {
bool source_camera_autozoom_v14(float& zoom,const data::DesignSettingsOwner::Borrow& design,const CameraLayoutServicesV14& s,std::string& e){
 if(!s.world||!s.source_party_count6c4){e="Required same Application PlayerManager party-count6c4";return false;}std::int32_t count;if(!s.source_party_count6c4(count,e))return false;if(count==1)return true;
 if(!s.source_online5){e="Required actual Online byte5";return false;}std::uint8_t online;if(!s.source_online5(online,e))return false;if(online)return true;
 AutoZoomDesignV5 config;if(!source_autozoom_design_v5(design,config,e))return false;
 if(!(config.sides>=0.f&&config.top>=0.f&&config.bottom>=0.f)){e="Required original camera margin configuration assertions";return false;}
 if(!s.get_player||!s.get_screen_coord){e="Required same PlayerInfo/Character660/source driver projection";return false;}
 std::array<float,8> screens{};unsigned used=0;
 for(unsigned i=0;i<4;++i){CameraPartyActorV14 actor;if(!s.get_player(i,actor,e))return false;if(!actor.character||actor.dead)continue;std::array<float,2> point{};if(!s.get_screen_coord(actor.position160,point,e))return false;screens[used*2]=point[0];screens[used*2+1]=point[1];++used;}
 const float bounds[5]{config.sides,config.top,config.bottom,config.reference,config.step};dh2_camera_autozoom_arithmetic_v14(&zoom,screens.data(),bounds,used);return true;
}
bool source_camera_centering_v14(PointV2& p,const CameraLayoutServicesV14& s,std::string& e){
 if(!s.world||!s.has_current_level||!s.num_local_players){e="Required same currentLevel/local PlayerManager centering";return false;}
 bool level;if(!s.has_current_level(level,e))return false;std::int32_t count;if(!s.num_local_players(count,e))return false;
 if(!level){e="Required original currentLevel centering assertion";return false;}if(count<=1)return true;
 if(!s.level_slots190){e="Required actual Level player-slot bytes190..193";return false;}std::array<std::uint8_t,4> slots;if(!s.level_slots190(slots,e))return false;
 const float shift=dh2_camera_centering_shift_v14(count,slots.data());if(shift==0.f)return true;
 if(!s.get_world_coord){e="Required original active-camera viewport ray unprojection";return false;}PointV2 center{},offset{};
 if(!s.get_world_coord({0,0},p[2],center,e)||!s.get_world_coord({shift,0},p[2],offset,e))return false;
 const float dy=offset[1]-center[1],dz=offset[2]-center[2],dx=offset[0]-center[0];p[0]=p[0]+dx;p[1]=p[1]+dy;p[2]=p[2]+dz;return true;
}
}
