#include "component_applicator.hpp"
#include "animation_blend.hpp"
#include <cstring>
#include <limits>

namespace {
bool type_valid(std::uint32_t type){return (type>=2&&type<=4)||(type>=11&&type<=13);}
bool span(const void* pointer,std::size_t bytes){const auto a=reinterpret_cast<std::uintptr_t>(pointer);return pointer&&a%alignof(float)==0&&a<=std::numeric_limits<std::uintptr_t>::max()-bytes;}
bool overlaps(const void* a,std::size_t as,const void* b,std::size_t bs){const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<y+bs&&y<x+as;}
bool reads(const void* destination,std::size_t bytes,const float* values,const float* weights,std::int32_t count){
    if(count<0||count>dh2::animation::maximum_blend_slots)return false;
    if(!count)return true;
    const auto value_bytes=static_cast<std::size_t>(count)*12;
    if(!span(values,value_bytes)||overlaps(destination,bytes,values,value_bytes))return false;
    const auto weight_bytes=static_cast<std::size_t>(count)*4;
    return count==1||(span(weights,weight_bytes)&&!overlaps(destination,bytes,weights,weight_bytes));
}
void apply(dh2::animation::ComponentNodeState& node,std::uint32_t type,const float* value){
    std::memcpy(type<=4?node.position:node.scale,value,12);node.dirty|=type<=4?8u:2u;
}
}
extern "C" int dh2_animation_component_blend(float* out,std::uint32_t type,const float* values,const float* weights,std::int32_t count){
    return type_valid(type)?dh2_animation_blend_vector3(out,values,weights,count):1;
}
extern "C" int dh2_animation_component_apply(dh2::animation::ComponentNodeState* node,std::uint32_t type,const float* value){
    if(!type_valid(type)||!span(node,sizeof(*node))||!reads(node,sizeof(*node),value,nullptr,1))return 1;
    apply(*node,type,value);return 0;
}
extern "C" int dh2_animation_component_apply_blended(dh2::animation::ComponentNodeState* node,std::uint32_t type,const float* values,const float* weights,std::int32_t count){
    if(!type_valid(type)||!span(node,sizeof(*node))||!reads(node,sizeof(*node),values,weights,count))return 1;
    float result[3];if(dh2_animation_blend_vector3(result,values,weights,count))return 1;
    apply(*node,type,result);return 0;
}
namespace dh2::animation {
namespace {
ComponentNodeState snapshot(const scene::Node& node,std::uint32_t dirty){ComponentNodeState state;std::memcpy(state.position,node.translation,12);std::memcpy(state.scale,node.scale,12);state.dirty=dirty;return state;}
void commit(scene::Node& node,std::uint32_t& dirty,const ComponentNodeState& state,std::uint32_t type){std::memcpy(type<=4?node.translation:node.scale,type<=4?state.position:state.scale,12);dirty=state.dirty;}
bool graph_reads(const scene::Node& node,std::uint32_t& dirty,const float* values,const float* weights,std::int32_t count){
    return !overlaps(&node,sizeof(node),&dirty,sizeof(dirty))&&reads(&node,sizeof(node),values,weights,count)&&reads(&dirty,sizeof(dirty),values,weights,count);
}
}
int apply_component(scene::Node& node,std::uint32_t& dirty,std::uint32_t type,const float* value){
    if(!type_valid(type)||!graph_reads(node,dirty,value,nullptr,1))return 1;
    auto state=snapshot(node,dirty);const int result=dh2_animation_component_apply(&state,type,value);if(!result)commit(node,dirty,state,type);return result;
}
int apply_blended_component(scene::Node& node,std::uint32_t& dirty,std::uint32_t type,const float* values,const float* weights,std::int32_t count){
    if(!type_valid(type)||!graph_reads(node,dirty,values,weights,count))return 1;
    auto state=snapshot(node,dirty);const int result=dh2_animation_component_apply_blended(&state,type,values,weights,count);if(!result)commit(node,dirty,state,type);return result;
}
}
