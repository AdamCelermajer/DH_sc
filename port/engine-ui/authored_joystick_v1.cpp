#include "authored_joystick_v1.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
namespace dh2::ui {namespace {
float f(std::uint32_t bits){float out;std::memcpy(&out,&bits,4);return out;}
std::int32_t trunc_source(float x){if(std::isnan(x))return 0;if(x>=2147483648.f)return INT32_MAX;if(x<=-2147483648.f)return INT32_MIN;return static_cast<std::int32_t>(x);}
float mul(float a,float b){volatile float v=a*b;return v;}
float sub(float a,float b){volatile float v=a-b;return v;}
float add(float a,float b){volatile float v=a+b;return v;}
float div(float a,float b){volatile float v=a/b;return v;}
}
bool authored_joystick_initialize_v1(AuthoredJoystickStateV1& s,float width,std::string& e){
 if(!std::isfinite(width)||width<=0){e="Required actual authored Joystick.bg width";return false;}
 auto radius=trunc_source(mul(div(width,20.f),.5f));if(radius<=0){e="Source joystick radius is zero";return false;}
 s.radius_x=s.radius_y=radius;return true;
}
bool authored_joystick_press_v1(AuthoredJoystickStateV1& s,float x,float y,std::string& e){
 if(!std::isfinite(x)||!std::isfinite(y)){e="Malformed source joystick event coordinates";return false;}
 s.center_x=trunc_source(div(x,20.f));s.center_y=trunc_source(div(y,20.f));return true;
}
bool authored_joystick_drag_v1(AuthoredJoystickStateV1& s,float x,float y,float tx,float ty,const AuthoredJoystickServicesV1& services,std::string& e){
 if(s.radius_x<=0||s.radius_y<=0||!std::isfinite(x)||!std::isfinite(y)||!std::isfinite(tx)||!std::isfinite(ty)){
  e="Required initialized joystick radius and actual source event matrix";return false;}
 auto dx=trunc_source(sub(div(add(x,tx),20.f),static_cast<float>(s.center_x)));
 auto dy=trunc_source(sub(div(add(y,ty),20.f),static_cast<float>(s.center_y)));
 const float angle=std::atan2(static_cast<float>(dy),static_cast<float>(dx));
 auto sx=std::clamp(dx,-s.radius_x,s.radius_x),sy=std::clamp(dy,-s.radius_y,s.radius_y);
 // Original y square is signed32 MUL before conversion. Valid authored domain
 // is small; unsigned wrapping preserves the original operation at extremes.
 const auto y2=static_cast<std::int32_t>(static_cast<std::uint32_t>(sy)*static_cast<std::uint32_t>(sy));
 volatile float squared=mul(static_cast<float>(sx),static_cast<float>(sx))+static_cast<float>(y2);
 s.magnitude=div(std::sqrt(squared),static_cast<float>(s.radius_x));
 if(s.magnitude>1.f){sy=trunc_source(mul(static_cast<float>(s.radius_y),std::sin(angle)));sx=trunc_source(mul(static_cast<float>(s.radius_x),std::cos(angle)));}
 if(!services.position_stick){e="Required source RenderFX.SetPosition joystick backend";return false;}
 if(!services.position_stick(services.context,sx,sy,e))return false;
 if(!services.controller_allowed){e="Required source Character.CTRLIsAllowed joystick backend";return false;}
 bool allowed{};if(!services.controller_allowed(services.context,allowed,e))return false;
 if(!allowed)return true;
 s.direction[0]=1.f;s.direction[1]=-1.f;s.direction[2]=0.f;
 volatile float degrees=mul(angle,f(0xc2652ee0u))+90.f;
 if(!services.rotate_direction){e="Required source normalized Point3D.rotateXYBy joystick backend";return false;}
 const float base[3]{1,-1,0};if(!services.rotate_direction(services.context,base,degrees,s.direction,e))return false;
 s.active=1;return true;
}
bool authored_joystick_release_v1(AuthoredJoystickStateV1& s,bool player,const AuthoredJoystickServicesV1& services,std::string& e){
 s.center_x=s.center_y=0;
 if(!services.position_stick){e="Required source joystick release SetPosition";return false;}
 if(!services.position_stick(services.context,0,0,e))return false;
 s.active=0;if(!player)return true;
 if(!services.stop){e="Required source joystick release Cmd_Stop";return false;}
 return services.stop(services.context,e);
}
bool authored_joystick_update_v1(const AuthoredJoystickStateV1& s,const AuthoredJoystickServicesV1& services,std::string& e){
 if(!s.active)return true;
 if(!services.controller_allowed){e="Required source joystick Update CTRLIsAllowed";return false;}
 bool allowed{};if(!services.controller_allowed(services.context,allowed,e))return false;if(!allowed)return true;
 const float v[3]{mul(s.direction[0],s.magnitude),mul(s.direction[1],s.magnitude),mul(s.direction[2],s.magnitude)};
 if(!services.head_towards){e="Required source joystick Update Cmd_HeadTowards";return false;}
 return services.head_towards(services.context,v,e);
}
}
