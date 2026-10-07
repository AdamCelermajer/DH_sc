#include "particle_deflector_v1.hpp"
#include <cmath>
#include <cstring>
namespace dh2::animation {
namespace {
float add(float a,float b){volatile float v=a+b;return v;}
float sub(float a,float b){volatile float v=a-b;return v;}
float mul(float a,float b){volatile float v=a*b;return v;}
float div(float a,float b){volatile float v=a/b;return v;}
float dot(const float* a,const float* b){return add(add(mul(a[0],b[0]),mul(a[1],b[1])),mul(a[2],b[2]));}
float length(const float* v){return static_cast<float>(std::sqrt(static_cast<double>(dot(v,v))));}
float read(const ParticleSeed100& p,unsigned at){float v;std::memcpy(&v,p.data()+at,4);return v;}
void write(ParticleSeed100& p,unsigned at,float v){std::memcpy(p.data()+at,&v,4);}
float draw(std::int32_t* seed){return static_cast<float>(dh2_particle_random_v1(seed));}
}
extern "C" float dh2_particle_deflector_friction_v1(float coefficient,float normal_speed,float tangent_speed,float duration){
 if(coefficient==0.f)return 1.f;if(coefficient==1.f)return 0.f;
 const float keep=sub(1.f,coefficient);
 if(mul(tangent_speed,0.07f)<=normal_speed)return keep;
 const float temporal=std::exp(mul(duration,std::log(keep)));
 if(mul(tangent_speed,0.035f)>normal_speed)return temporal;
 const float blend=sub(div(div(normal_speed,tangent_speed),0.035f),1.f);
 return add(mul(keep,blend),mul(sub(1.f,blend),temporal));
}
extern "C" int dh2_particle_deflector_construct_v1(ParticleDeflectorModelV1* model,
 const ParticleDeflectorParametersV1* parameters,const math::Matrix4f* matrix){
 if(!model||!parameters||!matrix)return -1;
 model->parameters=parameters;std::memcpy(&model->previous,matrix,65);return 0;
}
extern "C" int dh2_particle_deflector_apply_v1(ParticleSeed100* particles,std::uint32_t count,
 ParticleDeflectorModelV1* model,math::Matrix4f* current,float dt,std::int32_t* seed){
 if(count>65536||(count&&!particles)||!model||!model->parameters||!current)return -1;
 const auto& p=*model->parameters;const float* m=current->m;const float* old=model->previous.m;
 float normal[3]={m[8],m[9],m[10]};math::Vector3f normalized{normal[0],normal[1],normal[2]};
 current->identity_hint=0;dh2_vec3_normalize(&normalized);normal[0]=normalized.x;normal[1]=normalized.y;normal[2]=normalized.z;
 const float negative_y[3]={-m[4],-m[5],-m[6]};
 const float half_height=mul(mul(length(negative_y),p.length),0.5f);
 const float half_width=mul(mul(length(m),p.width),0.5f);
 model->previous.identity_hint=0;const float amplitude=mul(p.bounce,p.bounce_variation);
 for(unsigned i=0;i<count;++i){
  float position[3],velocity[3],motion[3],relative[3];
  for(unsigned k=0;k<3;++k){position[k]=read(particles[i],4*k);velocity[k]=read(particles[i],12+4*k);motion[k]=mul(dt,velocity[k]);relative[k]=sub(m[12+k],position[k]);}
  const float denominator=dot(motion,m+8);if(denominator==0.f)continue;
  const float fraction=div(dot(relative,m+8),denominator);if(fraction<=0.f||fraction>1.f)continue;
  float intersection[3],local[3];for(unsigned k=0;k<3;++k){intersection[k]=add(position[k],mul(fraction,motion[k]));local[k]=sub(intersection[k],m[12+k]);}
  const float y=div(dot(negative_y,local),half_height);if(y>1.f||y<-1.f)continue;
  const float x=div(dot(m,local),half_width);if(x>1.f||x<-1.f)continue;
  if(!seed)return -2;
  const float variation=amplitude!=0.f?add(mul(amplitude,draw(seed)),mul(amplitude,-0.5f)):0.f;
  const float reflected_normal=-dot(normal,velocity);float tangent[3],result[3];
  for(unsigned k=0;k<3;++k)tangent[k]=add(velocity[k],mul(reflected_normal,normal[k]));
  const float bounce=mul(reflected_normal,add(variation,p.bounce));
  const float friction=dh2_particle_deflector_friction_v1(p.friction,bounce,length(tangent),mul(sub(1.f,fraction),dt));
  for(unsigned k=0;k<3;++k)result[k]=add(mul(bounce,normal[k]),mul(friction,tangent[k]));
  if(p.chaos>0.f){
   const float width=mul(p.chaos,180.f),low=mul(width,-0.5f);math::Vector3f value{result[0],result[1],result[2]},zero{0,0,0};
   const float xy=add(mul(width,draw(seed)),low);dh2_vec3_rotate_xy(&value,static_cast<double>(xy),&zero);
   const float yz=add(mul(width,draw(seed)),low);dh2_vec3_rotate_yz(&value,static_cast<double>(yz),&zero);
   const float xz=add(mul(width,draw(seed)),low);dh2_vec3_rotate_xz(&value,static_cast<double>(xz),&zero);
   result[0]=value.x;result[1]=value.y;result[2]=value.z;
   const float facing=dot(normal,result);if(facing<0.f)for(unsigned k=0;k<3;++k)result[k]=add(result[k],mul(mul(facing,-2.f),normal[k]));
  }
  for(unsigned k=0;k<3;++k)intersection[k]=add(m[12+k],local[k]);
  if(p.inherit_velocity>0.f){
   const float offset_y=mul(y,half_height),offset_x=mul(x,half_width);
   for(unsigned k=0;k<3;++k){const float old_point=add(add(old[12+k],mul(-old[4+k],offset_y)),mul(old[k],offset_x));
    result[k]=add(result[k],mul(p.inherit_velocity,sub(intersection[k],old_point)));}
  }
  for(unsigned k=0;k<3;++k){write(particles[i],4*k,add(intersection[k],mul(normal[k],0.3f)));write(particles[i],12+4*k,result[k]);}
 }
 std::memcpy(&model->previous,current,65);return 0;
}
}
