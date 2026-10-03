#include "character_body_config.hpp"
#include <cstring>
using namespace dh2::physical;
extern "C" int dh2_character_body_config(CharacterBodyConfig* out,const CharacterBodyInput* in){
 if(!out||!in||!in->owner||in->reserved||in->is_player>1||in->special_owner_byte>255||in->collision_group_override>1||in->disable_physical>1)return -1;
 CharacterBodyConfig result{};
 auto request=[&](CharacterBodyService service){result.requests[result.request_count++]=service;};
 bool special=false,bullet=false;std::int32_t group=0;std::uint32_t category=0,mask=0;
 if(in->character_type==9){category=0x100;result.enabled=1;}
 else if(in->special_owner_byte){category=2;mask=0xffff;special=true;result.enabled=1;}
 else if(in->character_type==6||in->character_type==7||in->character_type==8){group=-4;category=0x400;mask=0xd1f;result.enabled=1;}
 else if(in->is_player){group=-1;category=4;mask=0xd7f;bullet=true;result.enabled=1;}
 else if(in->character_type==2||in->character_type==5){group=-2;category=8;mask=0xd3b;result.enabled=1;}
 else if(in->character_type==3){category=0x100;result.enabled=1;}
 else if(in->character_type==4){group=2;category=0x10;mask=0xd3f;result.enabled=1;}
 if(result.enabled&&!in->new_physical)return -1;
 if(result.enabled){
  result.po_character=!special;
  result.body.user_data=in->new_physical;result.body.position[0]=in->position[0]*0.01f;result.body.position[1]=in->position[1]*0.01f;
  result.body.allow_sleep=1;result.body.is_sleeping=1;result.body.fixed_rotation=1;result.body.bullet=bullet;
  result.shape.user_data=in->new_physical;result.shape.kind=special?1:0;result.shape.friction=1;
  const std::uint32_t density=0x4133d70a;if(!special)std::memcpy(&result.shape.density,&density,4);
  result.shape.group_index=in->collision_group_override?-666:group;result.shape.category_bits=category;result.shape.mask_bits=mask;
  const float width=(in->absolute_bounds[2]-in->absolute_bounds[0])*0.01f;
  const float height=(in->absolute_bounds[3]-in->absolute_bounds[1])*0.01f;
  result.radius=(width<height?height:width)*0.5f;
  if(special){
   const float x=width*0.5f,y=height*0.5f;
   result.shape.vertex_count=4;
   const float vertices[8]={-x,-y,x,-y,x,y,-x,y};std::memcpy(result.shape.vertices,vertices,32);
  }else result.shape.radius=result.radius;
  request(allocate_physical);request(create_body);request(create_shape);request(mass_from_shapes);
  if(result.po_character){request(pin_zero_mass);result.pinned=1;}
 }
 if(in->disable_physical){if(result.enabled)request(destroy_physical);}
 else {
  if(in->previous_physical!=(result.enabled?in->new_physical:nullptr)){
   if(in->previous_physical)request(destroy_physical);
   request(assign_physical);
   if(result.enabled&&!result.pinned){request(pin_zero_mass);result.pinned=1;}
  }
  request(update_pf_object);
 }
 *out=result;return 0;
}
