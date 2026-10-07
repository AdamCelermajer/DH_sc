#include "item_body_config_v2.hpp"
#include <cstring>
namespace dh2::physical {
bool item_body_config_v2(CharacterBodyConfig& out,const ItemBodyInputV2& in)noexcept{
 if(!in.physical)return false;CharacterBodyConfig r{};
 r.enabled=1;r.body.user_data=in.physical;
 r.body.position[0]=in.position[0]*.01f;r.body.position[1]=in.position[1]*.01f;
 r.body.allow_sleep=1;r.body.is_sleeping=1;r.body.fixed_rotation=1;
 r.shape.user_data=in.physical;r.shape.kind=0;r.shape.sensor=1;r.shape.friction=1;
 const std::uint32_t density=0x4133d70a;std::memcpy(&r.shape.density,&density,4);
 const float width=(in.absolute_bounds[2]-in.absolute_bounds[0])*.01f;
 const float height=(in.absolute_bounds[3]-in.absolute_bounds[1])*.01f;
 r.radius=(width<height?height:width)*.5f;r.shape.radius=r.radius;
 r.shape.group_index=in.no_collisions?-666:-3;r.shape.category_bits=0x40;r.shape.mask_bits=4;
 r.request_count=4;r.requests[0]=allocate_physical;r.requests[1]=create_body;
 r.requests[2]=create_shape;r.requests[3]=mass_from_shapes;
 out=r;return true;
}
}
