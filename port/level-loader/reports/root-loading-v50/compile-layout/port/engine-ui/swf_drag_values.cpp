#include "swf_drag_values.hpp"
#include "swf_input_geometry.hpp"
#include <cstring>
#include <cfloat>
namespace {
float mul(float a,float b){return a*b;}float sub(float a,float b){return a-b;}
float finite(float x){return x>=-FLT_MAX&&x<=FLT_MAX?x:0.f;}
float bound(float x,float low,float high){if(!(high>x))x=high;if(!(low<x))x=low;return finite(x);}
}
extern "C" int dh2_ui_swf_drag_values(dh2::ui::SwfDragResult44*out,const dh2::ui::SwfDragValues108*in) noexcept {
 using namespace dh2::ui;if(!out||!in||reinterpret_cast<std::uintptr_t>(out)%alignof(SwfDragResult44)||reinterpret_cast<std::uintptr_t>(in)%alignof(SwfDragValues108)||in->reserved)return -1;
 auto a=reinterpret_cast<std::uintptr_t>(out),b=reinterpret_cast<std::uintptr_t>(in);if(a>UINTPTR_MAX-sizeof(*out)||b>UINTPTR_MAX-sizeof(*in)||(a<b+sizeof(*in)&&b<a+sizeof(*out)))return -1;
 SwfDragResult44 result{};std::memcpy(result.matrix,in->local,24);std::memcpy(result.offset,in->offset,8);result.initialized=in->initialized;
 float point[2]{mul(static_cast<float>(in->mouse[0]),20.f),mul(static_cast<float>(in->mouse[1]),20.f)};
 dh2_ui_swf_inverse_point(result.local_mouse,in->world,point);
 float x,y;
 if(in->lock_center){float local[2];dh2_ui_swf_inverse_point(local,in->parent_world,point);x=finite(local[0]);y=finite(local[1]);}
 else {if(!in->initialized){result.offset[0]=sub(point[0],in->local[2]);result.offset[1]=sub(point[1],in->local[5]);result.initialized=1;}x=finite(sub(point[0],result.offset[0]));y=finite(sub(point[1],result.offset[1]));}
 if(in->bounded){x=bound(x,mul(in->bounds[0],20.f),mul(in->bounds[1],20.f));y=bound(y,mul(in->bounds[2],20.f),mul(in->bounds[3],20.f));}
 result.matrix[2]=x;result.matrix[5]=y;std::memcpy(out,&result,sizeof(result));return 0;
}
