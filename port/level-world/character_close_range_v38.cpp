#include "character_close_range_v38.hpp"
#include <cstring>
namespace dh2::character {
int character_close_distance_v38(const float* a,const float* b,std::int32_t minimum){
 if(!a||!b)return -1;
 volatile float x=a[0]-b[0],y=a[1]-b[1],z=a[2]-b[2];
 volatile float xx=x*x,yy=y*y,zz=z*z,xy=xx+yy,d=xy+zz;
 const auto raw=std::uint32_t(minimum)*std::uint32_t(minimum);std::int32_t square;std::memcpy(&square,&raw,4);
 return float(square)>d;
}
int character_close_range_v38(std::int32_t& out,std::uintptr_t owner,std::uintptr_t target,std::uintptr_t current,const CloseRangeServicesV38& s,std::string& error){
 error.clear();if(!owner){error="Required same Character close-range owner";return -1;}
 if(!target)target=current;if(!target){out=0;return 0;}
 auto call=[&](CloseRangeOperationV38 op,std::uintptr_t id,std::uintptr_t other,CloseRangeResponseV38& r,const char* name=nullptr){r={};if(!s.invoke||s.invoke(s.context,{op,id,other,name},r,error)){if(error.empty())error="Required close-range source operation "+std::to_string(unsigned(op));return false;}return true;};
 CloseRangeResponseV38 r;
 if(!call(CloseRangeOperationV38::resolve_character,target,0,r))return -1;
 bool enemy_character=r.character!=0;
 if(enemy_character){if(!call(CloseRangeOperationV38::object_kind,r.character,0,r))return -1;enemy_character=r.word==0;}
 if(enemy_character){if(!call(CloseRangeOperationV38::interaction_type,target,owner,r))return -1;enemy_character=r.word==8;}
 if(!enemy_character){if(!call(CloseRangeOperationV38::interaction_range,owner,target,r))return -1;out=r.word;return 0;}
 if(!call(CloseRangeOperationV38::range_parameters,owner,0,r))return -1;
 if(!r.word){out=0;return 0;}const auto minimum=r.limits[0];
 if(!call(CloseRangeOperationV38::position,owner,0,r))return -1;float a[3];std::memcpy(a,r.position,12);
 if(!call(CloseRangeOperationV38::position,target,0,r))return -1;float b[3];std::memcpy(b,r.position,12);
 if(!call(CloseRangeOperationV38::debug_load,0,0,r)||!call(CloseRangeOperationV38::debug_query,0,0,r,"IsTracingCharAITarget"))return -1;
 if(r.word&&(!call(CloseRangeOperationV38::debug_load,0,0,r)||!call(CloseRangeOperationV38::debug_query,0,0,r,"isTracingCharAITarget")))return -1;
 out=character_close_distance_v38(a,b,minimum);return 0;
}
}
