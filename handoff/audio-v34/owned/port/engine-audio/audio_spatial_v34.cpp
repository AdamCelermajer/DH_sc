#include "audio_spatial_v34.hpp"
#include <cmath>
#include <algorithm>
#include <limits>
namespace dh2::audio {namespace {
float mul(float a,float b){volatile float v=a*b;return v;}
float add(float a,float b){volatile float v=a+b;return v;}
float sub(float a,float b){volatile float v=a-b;return v;}
float div(float a,float b){volatile float v=a/b;return v;}
float dot(const float*a,const float*b){return add(add(mul(a[0],b[0]),mul(a[1],b[1])),mul(a[2],b[2]));}
bool quant(float x,int&out){const auto v=mul(x,16384.f);if(!std::isfinite(v)||v< -2147483648.f||v>=2147483648.f)return false;out=int(v);return true;}
}
bool audio_spatial_v34(const AudioListenerV34&listener,const AudioSpatialSourceV34&s,AudioSpatialResultV34&out)noexcept{
 const float*arrays[]{listener.position,listener.velocity,listener.front,listener.up,s.position,s.velocity};for(const auto*a:arrays)for(unsigned k=0;k<3;++k)if(!std::isfinite(a[k]))return false;
 for(float v:{s.reference_distance,s.maximum_distance,s.rolloff,s.doppler_factor,s.speed_over_doppler})if(!std::isfinite(v))return false;
 float delta[3];for(unsigned k=0;k<3;++k)delta[k]=s.relative?s.position[k]:sub(s.position[k],listener.position[k]);const float distance=std::sqrt(dot(delta,delta));float pan=0;
 if(distance>0){if(s.relative)pan=div(delta[0],distance);else{
  const float*front=listener.front;const float*up=listener.up;float right[3]{sub(mul(front[1],up[2]),mul(front[2],up[1])),sub(mul(front[2],up[0]),mul(front[0],up[2])),sub(mul(front[0],up[1]),mul(front[1],up[0]))};const float length=std::sqrt(dot(right,right));
  if(length>0){float d[3],r[3];for(unsigned k=0;k<3;++k){d[k]=div(delta[k],distance);r[k]=div(right[k],length);}pan=dot(d,r);}
 }}
 const float right=std::sqrt(mul(add(pan,1.f),.5f));const float left=std::sqrt(sub(1.f,mul(right,right)));AudioSpatialResultV34 result;if(!quant(left,result.left_q14)||!quant(right,result.right_q14))return false;
 float gain=1,d=distance;const float ref=s.reference_distance,max=s.maximum_distance,rolloff=s.rolloff;
 switch(s.distance_model){
 case 2:if(ref>d)d=ref;else if(max<d)d=max;[[fallthrough]];
 case 1:{const auto denominator=add(ref,mul(rolloff,sub(d,ref)));if(denominator>0)gain=div(ref,denominator);break;}
 case 4:if(ref>d)d=ref;else if(max<d)d=max;[[fallthrough]];
 case 3:{const auto span=sub(max,ref);if(span>0)gain=std::max(0.f,sub(1.f,div(mul(sub(d,ref),rolloff),span)));break;}
 case 6:if(rolloff<=0||ref<=0)break;if(ref>d)d=ref;else if(max<d)d=max;[[fallthrough]];
 case 5:if(rolloff>0&&ref>0)gain=std::pow(div(d,ref),-rolloff);break;
 default:break;
 }if(!quant(gain,result.distance_q14))return false;
 result.doppler_q14=16384;
 if(s.doppler_factor>0){float to_listener[3];for(unsigned k=0;k<3;++k)to_listener[k]=-delta[k];float listener_dot=s.relative?0:dot(to_listener,listener.velocity);const float source_dot=dot(to_listener,s.velocity);const float speed_distance=mul(distance,s.speed_over_doppler);if(listener_dot>speed_distance)listener_dot=speed_distance;
  const float denominator=sub(speed_distance,source_dot);if(denominator>0){const float pitch=add(div(sub(source_dot,listener_dot),denominator),1.f);if(pitch>2.9f)result.doppler_q14=47513;else if(pitch<.001f)result.doppler_q14=16;else if(!quant(pitch,result.doppler_q14))return false;}
 }
 out=result;return true;
}
}
