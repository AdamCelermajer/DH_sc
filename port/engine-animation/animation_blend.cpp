#include "animation_blend.hpp"
#include "../engine-math/math.hpp"
#include <cstddef>
#include <cstring>
#include <limits>

namespace {
float add(float a, float b) { volatile float value=a+b; return value; }
float multiply(float a, float b) { volatile float value=a*b; return value; }
float divide(float a, float b) { volatile float value=a/b; return value; }
bool span(const void* pointer, std::size_t bytes) {
    const auto address=reinterpret_cast<std::uintptr_t>(pointer);
    return pointer && address%alignof(float)==0
        && address<=std::numeric_limits<std::uintptr_t>::max()-bytes;
}
bool overlaps(const void* a, std::size_t as, const void* b, std::size_t bs) {
    const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
    return x<y+bs && y<x+as;
}
bool valid(float* output, const float* values, const float* weights,
           std::int32_t count, unsigned components, bool uses_weights) {
    const auto result_bytes=components*sizeof(float);
    if(count<0 || count>dh2::animation::maximum_blend_slots || !span(output,result_bytes))return false;
    if(count==0)return true;
    const auto values_bytes=static_cast<std::size_t>(count)*result_bytes;
    if(!span(values,values_bytes) || overlaps(output,result_bytes,values,values_bytes))return false;
    const auto weights_bytes=static_cast<std::size_t>(count)*sizeof(float);
    return !uses_weights || (span(weights,weights_bytes) && !overlaps(output,result_bytes,weights,weights_bytes));
}
int weighted(float* output, const float* values, const float* weights,
             std::int32_t count, unsigned components) {
    if(!valid(output,values,weights,count,components,count>1))return 1;
    if(count==1){std::memcpy(output,values,components*sizeof(float));return 0;}
    float result[3]{0.0f,0.0f,0.0f};
    for(std::int32_t slot=0;slot<count;++slot)
        for(unsigned component=0;component<components;++component)
            result[component]=add(result[component],multiply(weights[slot],values[slot*components+component]));
    std::memcpy(output,result,components*sizeof(float));
    return 0;
}
}
extern "C" int dh2_animation_blend_scalar(float* output,const float* values,
                                          const float* weights,std::int32_t count) {
    return weighted(output,values,weights,count,1);
}
extern "C" int dh2_animation_blend_vector3(float* output,const float* values,
                                           const float* weights,std::int32_t count) {
    return weighted(output,values,weights,count,3);
}
extern "C" int dh2_animation_blend_quaternion(float* output,const float* values,
                                              const float* weights,std::int32_t count) {
    if(!valid(output,values,weights,count,4,count!=0))return 1;
    dh2::math::Quaternion result{0.0f,0.0f,0.0f,1.0f};
    std::int32_t first=0;
    while(first<count && weights[first]==0.0f)++first;
    if(first<count){
        std::memcpy(&result,values+first*4,sizeof(result));
        float accumulated=weights[first];
        if(accumulated==1.0f){std::memcpy(output,&result,sizeof(result));return 0;}
        for(std::int32_t slot=first+1;slot<count;++slot){
            const float weight=weights[slot];
            if(weight==0.0f)continue;
            accumulated=add(accumulated,weight);
            dh2::math::Quaternion next;
            std::memcpy(&next,values+slot*4,sizeof(next));
            dh2_quat_slerp(&result,&result,&next,divide(weight,accumulated));
        }
    }
    std::memcpy(output,&result,sizeof(result));
    return 0;
}
