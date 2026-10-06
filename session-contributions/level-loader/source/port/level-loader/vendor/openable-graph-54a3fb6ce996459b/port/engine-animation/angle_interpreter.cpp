#include "angle_interpreter.hpp"
#include <cstddef>
#include <cstdint>
#include <cstring>
namespace {
bool overlaps(std::uintptr_t a,std::size_t an,std::uintptr_t b,std::size_t bn){return a<=b?b-a<an:a-b<bn;}
int validate(dh2::math::Quaternion* out,const dh2::animation::AngleAccessor24* a,std::uint32_t key,std::uint32_t next){
 if(!out||!a||reinterpret_cast<std::uintptr_t>(out)%alignof(float)||reinterpret_cast<std::uintptr_t>(a)%alignof(dh2::animation::AngleAccessor24))return -1;
 if(overlaps(reinterpret_cast<std::uintptr_t>(out),16,reinterpret_cast<std::uintptr_t>(a),24))return -1;
 if(a->reserved||!a->values||!a->count||key>=a->count||next>=a->count||reinterpret_cast<std::uintptr_t>(a->values)%alignof(float))return -1;
 const auto address=reinterpret_cast<std::uintptr_t>(a->values);const auto length=std::size_t(a->count)*4;
 if(length>UINTPTR_MAX-address||overlaps(reinterpret_cast<std::uintptr_t>(out),16,address,length))return -1;
 if(!a->default_value)return -2;
 const auto defaults=reinterpret_cast<std::uintptr_t>(a->default_value);
 if(defaults%alignof(float)||defaults>UINTPTR_MAX-16||overlaps(reinterpret_cast<std::uintptr_t>(out),16,defaults,16))return -1;
 return 0;
}
int sample(dh2::math::Quaternion* out,const dh2::animation::AngleAccessor24* a,std::uint32_t key,std::uint32_t next,float fraction,bool between){
 const int status=validate(out,a,key,next);if(status)return status;
 dh2::math::Vector3f axis;std::memcpy(&axis,a->default_value,12);
 float angle=a->values[key];
 if(between){volatile float difference=a->values[next]-angle;volatile float product=fraction*difference;volatile float sum=angle+product;angle=sum;}
 dh2::math::Quaternion result;dh2_quat_from_angle_axis(&result,angle,&axis);std::memcpy(out,&result,16);return 0;
}
}
extern "C" int dh2_animation_angle_key(dh2::math::Quaternion* out,const dh2::animation::AngleAccessor24* a,std::uint32_t key){return sample(out,a,key,key,0,false);}
extern "C" int dh2_animation_angle_between(dh2::math::Quaternion* out,const dh2::animation::AngleAccessor24* a,std::uint32_t key,std::uint32_t next,float fraction){return sample(out,a,key,next,fraction,true);}
