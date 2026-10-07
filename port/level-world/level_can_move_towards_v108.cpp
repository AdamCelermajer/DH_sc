#include "level_can_move_towards_v108.hpp"
#include <cmath>
#include <cstring>
namespace dh2::world {namespace {
float multiply(float a,float b){volatile float out=a*b;return out;}
float add(float a,float b){volatile float out=a+b;return out;}
float subtract(float a,float b){volatile float out=a-b;return out;}
float length(const float* v){return std::sqrt(add(add(multiply(v[0],v[0]),multiply(v[1],v[1])),multiply(v[2],v[2])));}
float absolute(float a){std::uint32_t word;std::memcpy(&word,&a,4);word&=0x7fffffffu;std::memcpy(&a,&word,4);return a;}
}
//Whole Level.CanMoveTowards3f94f4. Single-player/online/nearzero-heading
//and absent camera-parent returns precede cooperative viewport dependencies.
bool level_can_move_towards_v108(const float* position,const float* heading,const LevelCanMoveServicesV108& s,bool& accepted,std::string& e){
 if(!position||!heading||!s.owner||!s.count6c4){e="Required actual Level.CanMoveTowards receiver/count";return false;}
 std::int32_t count{};if(!s.count6c4(count,e))return false;
 if(count==1){accepted=true;return true;}
 std::uint8_t online{};if(!s.online5||!s.online5(online,e))return false;
 if(online){accepted=true;return true;}
 const float squared=absolute(add(add(multiply(heading[0],heading[0]),multiply(heading[1],heading[1])),multiply(heading[2],heading[2])));
 if(squared<0x1.a36e2ep-14f){accepted=true;return true;}
 bool parent{};std::array<float,3> camera;
 if(!s.camera_parent4||!s.camera_parent4(parent,camera,e))return false;
 if(!parent){accepted=true;return true;}
 const float delta[]{subtract(camera[0],position[0]),subtract(camera[1],position[1]),subtract(camera[2],position[2])};
 const auto dot=add(add(multiply(heading[0],delta[0]),multiply(heading[1],delta[1])),multiply(heading[2],delta[2]));
 volatile float ratio=dot/multiply(length(heading),length(delta));
 const float angle=absolute(std::acos(ratio));
 if(angle<0x1.921fb6p-1f){accepted=true;return true;}
 std::array<float,3> limits;if(!s.limits||!s.limits(limits,e))return false;
 //Original checks side(+1c), top(+20), bottom(+18), in that order.
 const unsigned order[]{0,1,2};const char* names[]{"CamLimits_Sides > 0.0f","CamLimits_Top > 0.0f","CamLimits_Bottom > 0.0f"};
 for(const auto field:order)if(!(limits[field]>0.f)){
  if(!s.assertion||!s.assertion(96u+field,names[field],e))return false;
 }
 if(!s.player||!s.screen){e="Required actual PM.GetPlayer/screen projection";return false;}
 std::array<float,2> screen{};
 for(unsigned index=0;index<4;++index){
  LevelCanMovePartyV108 actor;if(!s.player(index,actor,e))return false;
  if(!actor.character||actor.dead)continue;
  if(!actor.receiver||!actor.position160){e="Required SAME live cooperative Character160";return false;}
  if(!s.screen(actor.position160,screen,e))return false;
  if(!(-limits[0]<screen[0])||!(limits[0]>screen[0])||!(-limits[2]<screen[1])||!(screen[1]<limits[1])){accepted=false;return true;}
 }
 accepted=true;return true;
}
}
