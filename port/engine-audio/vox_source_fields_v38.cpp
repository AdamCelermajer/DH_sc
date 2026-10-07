#include "vox_source_fields_v38.hpp"
#include <algorithm>
#include <cmath>
namespace dh2::audio {namespace {
float mul(float a,float b){volatile float v=a*b;return v;}
float add(float a,float b){volatile float v=a+b;return v;}
float sub(float a,float b){volatile float v=a-b;return v;}
float dot(const float*a,const float*b){return add(add(mul(a[0],b[0]),mul(a[1],b[1])),mul(a[2],b[2]));}
void cross(const float*a,const float*b,float*out){for(unsigned i=0;i<3;++i)out[i]=sub(mul(a[(i+1)%3],b[(i+2)%3]),mul(a[(i+2)%3],b[(i+1)%3]));}
bool normalize(float*v){const float length=std::sqrt(dot(v,v));if(!std::isfinite(length)||length<=0)return false;volatile float inverse=1.f/length;for(unsigned i=0;i<3;++i)v[i]=mul(v[i],inverse);return true;}
}
bool vox_source_emitter_fields_v38(const VoxListenerAuthorityV38&a,int type,
 const float*p,float ref,float max,AudioSpatialSourceV34&existing,std::string&error){
 if(type<0||type>2||!p||!std::isfinite(ref)||!std::isfinite(max)||!std::isfinite(a.rolloff)){error="Required finite original Vox emitter authority";return false;}
 auto next=existing;next.rolloff=a.rolloff;
 if(type==0){next.relative=true;existing=next;error.clear();return true;}
 for(unsigned i=0;i<3;++i){if(!std::isfinite(p[i])){error="Original source position range";return false;}next.position[i]=p[i];}
 const bool overrides=ref>=0&&max>=0;next.reference_distance=overrides?ref:float(a.reference_distance);next.maximum_distance=overrides?max:float(a.maximum_distance);
 if(type==2){float front[3],right[3],up[3],delta[3];std::copy_n(a.listener.front,3,front);if(!normalize(front)){error="Required actual finite listener front";return false;}cross(front,a.listener.up,right);if(!normalize(right)){error="Required actual listener basis";return false;}cross(right,front,up);if(!normalize(up)){error="Required actual listener up";return false;}for(unsigned i=0;i<3;++i)delta[i]=sub(p[i],a.listener.position[i]);next.position[0]=dot(right,delta);next.position[1]=dot(up,delta);next.position[2]=dot(front,delta);next.relative=true;}
 existing=next;error.clear();return true;
}
bool vox_source_spatial_command_v38(const VoxListenerAuthorityV38&a,const AudioSpatialSourceV34&s,float gain,float pitch,AudioCommandV34&command,std::string&error){
 if(!std::isfinite(gain)||gain<0||!std::isfinite(pitch)||pitch<=0){error="Required original source DSP gain/pitch";return false;}AudioSpatialResultV34 result;if(!audio_spatial_v34(a.listener,s,result)){error="Required recovered finite source spatial domain";return false;}
 const float distance=float(result.distance_q14)/16384.f;const float actual_pitch=mul(pitch,float(result.doppler_q14)/16384.f);if(actual_pitch>8){error="Required modern transport pitch capacity";return false;}
 command.left=mul(mul(gain,distance),float(result.left_q14)/16384.f);command.right=mul(mul(gain,distance),float(result.right_q14)/16384.f);command.pitch=actual_pitch;std::copy_n(s.position,3,command.source_emitter_position.begin());error.clear();return true;
}
}
