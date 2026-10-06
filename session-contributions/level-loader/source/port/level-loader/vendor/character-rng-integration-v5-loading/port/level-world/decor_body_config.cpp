#include "decor_body_config.hpp"
#include <cstring>
using namespace dh2::physical;
extern "C" int dh2_decor_marker_mesh_box(float* out,const DecorMeshBoxInput* in){
 if(!out||!in)return -1;
 float scaled[6],transformed[6],result[6];
 for(unsigned endpoint=0;endpoint<2;++endpoint){
  for(unsigned i=0;i<3;++i)scaled[3*endpoint+i]=in->bounds[3*endpoint+i]*in->parent_scale[i];
  for(unsigned row=0;row<3;++row){
   float value=scaled[3*endpoint]*in->node_matrix[row];
   value=value+scaled[3*endpoint+1]*in->node_matrix[4+row];
   value=value+scaled[3*endpoint+2]*in->node_matrix[8+row];
   value=value+0.0f;transformed[3*endpoint+row]=value;
  }
 }
 for(unsigned i=0;i<3;++i){
  const float upper=transformed[3+i],lower=transformed[i];
  const float low=upper<lower?upper:lower,high=upper>lower?upper:lower;
  const float extent=(high-low)*0.5f;result[i]=0.0f-extent;result[3+i]=extent+0.0f;
 }
 std::memcpy(out,result,sizeof result);return 0;
}
extern "C" int dh2_decor_level_world_bounds(float* out){
 if(!out)return -1;
 const float bounds[4]={-2000,-2000,2000,2000};std::memcpy(out,bounds,sizeof bounds);return 0;
}
extern "C" int dh2_decor_body_config(DecorBodyConfig* out,const DecorBodyInput* in){
 if(!out||!in||!in->owner||in->visual_present>1||in->colbox_found>1||in->collision_group_override>1||in->disable_physical>1||in->previous_flat>255||(!in->visual_present&&in->colbox_found))return -1;
 if(in->visual_present&&in->colbox_found&&!in->new_physical)return -1;
 DecorBodyConfig result{};result.flat=in->previous_flat;
 if(in->visual_present){
  std::memcpy(result.relative_box,in->mesh_box,sizeof result.relative_box);
  const float width=in->mesh_box[3]-in->mesh_box[0];
  if(width==0&&(in->mesh_box[4]-in->mesh_box[1])==0)result.flat=1;
  if(width<10){result.relative_box[0]=in->mesh_box[0]-5;result.relative_box[3]=in->mesh_box[3]+5;}
  if((result.relative_box[4]-result.relative_box[1])<10){result.relative_box[1]=in->mesh_box[1]-5;result.relative_box[4]=in->mesh_box[4]+5;}
  for(unsigned i=0;i<3;++i){result.absolute_box[i]=result.relative_box[i]+in->position[i];result.absolute_box[i+3]=result.relative_box[i+3]+in->position[i];}
  result.mesh_pf_updates=1;
 }
 if(in->visual_present&&in->colbox_found){
  auto& physical=result.physical;auto request=[&](CharacterBodyService service){physical.requests[physical.request_count++]=service;};
  physical.enabled=1;physical.body.user_data=in->new_physical;
  physical.body.position[0]=in->position[0]*0.01f;physical.body.position[1]=in->position[1]*0.01f;
  physical.body.allow_sleep=1;physical.body.is_sleeping=1;physical.body.fixed_rotation=1;
  physical.shape.user_data=in->new_physical;physical.shape.kind=1;physical.shape.friction=1;
  physical.shape.group_index=in->collision_group_override?-666:0;physical.shape.category_bits=2;physical.shape.mask_bits=0xffff;
  const float width=(result.absolute_box[3]-result.absolute_box[0])*0.01f;
  const float height=(result.absolute_box[4]-result.absolute_box[1])*0.01f;
  physical.radius=(width<height?height:width)*0.5f;
  const float x=width*0.5f,y=height*0.5f;physical.shape.vertex_count=4;
  const float vertices[8]={-x,-y,x,-y,x,y,-x,y};std::memcpy(physical.shape.vertices,vertices,sizeof vertices);
  request(allocate_physical);request(create_body);request(create_shape);request(mass_from_shapes);
  if(in->disable_physical)request(destroy_physical);
  else{
   if(in->previous_physical!=in->new_physical){
    if(in->previous_physical)request(destroy_physical);
    request(assign_physical);
   }
   request(update_pf_object);
  }
 }
 *out=result;return 0;
}
