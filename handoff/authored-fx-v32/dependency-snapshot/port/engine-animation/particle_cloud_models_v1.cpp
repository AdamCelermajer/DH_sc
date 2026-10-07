#include "particle_cloud_models_v1.hpp"
#include "../engine-math/math.hpp"
#include <cstring>
#include <algorithm>
namespace dh2::animation {
namespace {
float get(const ParticleSeed100& p,unsigned at){float v;std::memcpy(&v,p.data()+at,4);return v;}
void put(ParticleSeed100& p,unsigned at,float v){std::memcpy(p.data()+at,&v,4);}
bool valid(ParticleSeed100* p,std::uint32_t n){return n<=65536&&(!n||p);}
float draw(std::int32_t* seed){return static_cast<float>(dh2_particle_random_v1(seed));}
float spread(float width,std::int32_t* seed){const float sample=width*draw(seed),low=width*-0.5f;return sample+low;}
void random_vec(float* v,std::int32_t* seed){const double a=dh2_particle_random_v1(seed),b=dh2_particle_random_v1(seed),c=dh2_particle_random_v1(seed);v[0]=static_cast<float>(a);v[1]=static_cast<float>(b);v[2]=static_cast<float>(c);}
void normalize(float* v){math::Vector3f vector{v[0],v[1],v[2]};dh2_vec3_normalize(&vector);v[0]=vector.x;v[1]=vector.y;v[2]=vector.z;}
void vary_direction(float* v,float variation,std::int32_t* seed){
 math::Vector3f value{v[0],v[1],v[2]},center{0,0,0};const float width=variation*180.f,low=width*-0.5f;
 const float xy=width*draw(seed)+low;dh2_vec3_rotate_xy(&value,static_cast<double>(xy),&center);
 const float yz=width*draw(seed)+low;dh2_vec3_rotate_yz(&value,static_cast<double>(yz),&center);
 const float xz=width*draw(seed)+low;dh2_vec3_rotate_xz(&value,static_cast<double>(xz),&center);
 v[0]=value.x;v[1]=value.y;v[2]=value.z;
}
}
extern "C" int dh2_particle_life_init_v1(ParticleSeed100* p,std::uint32_t n,const ParticleLifeModelV1* m,float dt,std::int32_t* seed){
 if(!valid(p,n)||!m||!seed)return -1;
 for(unsigned i=0;i<n;++i){const float random=draw(seed);put(p[i],0x3c,0.f);const float high=m->variation*random,low=m->variation*-0.5f;float life=(high+low)+m->life;put(p[i],0x40,life);if(life<dt&&dt<life*4.f){life=dt*1.5f;put(p[i],0x40,life);}}
 return 0;
}
extern "C" int dh2_particle_life_apply_v1(ParticleSeed100* p,std::uint32_t n,float dt){if(!valid(p,n))return -1;for(unsigned i=0;i<n;++i)put(p[i],0x3c,get(p[i],0x3c)+dt);return 0;}
extern "C" int dh2_particle_size_init_v1(ParticleSeed100* p,std::uint32_t n,const ParticleSizeModelV1* m,std::int32_t* seed){
 if(!valid(p,n)||!m||!seed)return -1;const float width=m->target*m->variation;
 for(unsigned i=0;i<n;++i){const float variation=spread(width,seed),value=variation+m->target;if(m->growth>0){put(p[i],0x44,0.f);put(p[i],0x48,value);}else{put(p[i],0x44,value);put(p[i],0x48,value);}}return 0;
}
extern "C" int dh2_particle_size_apply_v1(ParticleSeed100* p,std::uint32_t n,const ParticleSizeModelV1* m){
 if(!valid(p,n)||!m)return -1;for(unsigned i=0;i<n;++i){const float initial=get(p[i],0x48),age=get(p[i],0x3c);put(p[i],0x44,initial);if(m->growth>0&&m->growth>age)put(p[i],0x44,initial*(age/m->growth));if(m->fade>0){const float remaining=get(p[i],0x40)-age;if(m->fade>remaining)put(p[i],0x44,initial*(remaining/m->fade));}}return 0;
}
extern "C" int dh2_particle_motion_apply_v1(ParticleSeed100* p,std::uint32_t n,float dt){if(!valid(p,n))return -1;for(unsigned i=0;i<n;++i){const float y=dt*get(p[i],0x10),z=dt*get(p[i],0x14),x=dt*get(p[i],0xc);put(p[i],0,get(p[i],0)+x);put(p[i],4,get(p[i],4)+y);put(p[i],8,get(p[i],8)+z);}return 0;}
extern "C" int dh2_particle_spin_apply_v1(ParticleSeed100* p,std::uint32_t n,float dt){if(!valid(p,n))return -1;float tau;const std::uint32_t bits=0x40c90fdb;std::memcpy(&tau,&bits,4);for(unsigned i=0;i<n;++i){const float period=get(p[i],0x4c),angle=get(p[i],0x50);float increment=0;if(period!=0)increment=(tau/period)*dt;put(p[i],0x50,angle+increment);}return 0;}
extern "C" int dh2_particle_spin_init_v1(ParticleSeed100* p,std::uint32_t n,const ParticleSpinModelV1* m,std::int32_t* seed){
 if(!valid(p,n)||!m||!seed||m->axis_type>2||(m->axis_type&&m->axis_variation>0))return -1;
 const float time_width=m->time*m->variation,phase_width=m->phase*m->phase_variation;
 for(unsigned i=0;i<n;++i){const float t=time_width!=0?spread(time_width,seed):0.f;put(p[i],0x4c,t+m->time);const float phase=phase_width!=0?spread(phase_width,seed):0.f;put(p[i],0x50,phase+m->phase);float axis[3];if(!m->axis_type){random_vec(axis,seed);for(float& a:axis)a=a-0.5f;}else if(m->axis_type==2)for(unsigned k=0;k<3;++k)axis[k]=get(p[i],0xc+4*k);else std::copy_n(m->axis,3,axis);normalize(axis);for(unsigned k=0;k<3;++k)put(p[i],0x54+4*k,axis[k]);}return 0;
}
extern "C" int dh2_particle_motion_init_v1(ParticleSeed100* p,std::uint32_t n,const ParticleMotionModelV1* m,const float* world,std::int32_t* seed){
 if(!valid(p,n)||!m||!seed)return -1;const float width=m->speed*m->speed_variation;
 for(unsigned i=0;i<n;++i){const float variation=width!=0?spread(width,seed):0.f;float direction[3];std::copy_n(m->direction,3,direction);
  if(m->direction[0]!=0&&m->direction[1]!=0&&m->direction[2]!=0){random_vec(direction,seed);for(float& a:direction)a=a-0.5f;normalize(direction);}else{if(m->direction_variation>0)vary_direction(direction,m->direction_variation,seed);if(world){float transformed[3];for(unsigned row=0;row<3;++row){float a=direction[0]*world[row];a=a+direction[1]*world[row+4];a=a+direction[2]*world[row+8];transformed[row]=a;}std::copy_n(transformed,3,direction);}normalize(direction);}
  const float speed=variation+m->speed;const float y=speed*direction[1],z=speed*direction[2],x=speed*direction[0];put(p[i],0x10,y);put(p[i],0xc,x);put(p[i],0x14,z);
 }return 0;
}
extern "C" int dh2_particle_sphere_generate_v1(float* out,const ParticleSphereV1* m,std::int32_t* seed){
 if(!out||!m||!seed||m->fixed_radius>1)return -1;float v[3],length;
 do{random_vec(v,seed);for(float& a:v)a=a-0.5f;const float x=v[0]*v[0],y=v[1]*v[1],z=v[2]*v[2];length=(x+y)+z;}while(length>0.25f);
 normalize(v);const float radius=m->fixed_radius?m->radius:m->inner_radius+draw(seed)*m->radial_range;
 const float y=radius*v[1]+m->center[1],z=radius*v[2]+m->center[2],x=radius*v[0]+m->center[0];out[1]=y;out[0]=x;out[2]=z;return 0;
}
extern "C" int dh2_particle_sphere_construct_v1(ParticleSphereV1* out,const float* center,float first,float second){if(!out||!center)return -1;std::copy_n(center,3,out->center);if(first<second){out->radius=second;out->inner_radius=first;}else{out->radius=first;out->inner_radius=second;}out->fixed_radius=out->radius==out->inner_radius;out->radial_range=out->radius-out->inner_radius;return 0;}
extern "C" int dh2_particle_gravity_apply_v1(ParticleSeed100* p,std::uint32_t n,const ParticleGravityV1* m,const float* matrix,float dt){
 if(!valid(p,n)||!m||!matrix||m->point_mode||m->falloff>0)return -1;
 const float strength=m->strength*1000.f;
 for(unsigned i=0;i<n;++i){float direction[3]={matrix[8],matrix[9],matrix[10]};normalize(direction);const float scale=strength*dt;const float x=scale*direction[0],y=direction[1]*scale,z=direction[2]*scale;put(p[i],0xc,get(p[i],0xc)+x);put(p[i],0x10,get(p[i],0x10)+y);put(p[i],0x14,get(p[i],0x14)+z);}return 0;
}
extern "C" int dh2_particle_emitter_init_v1(ParticleSeed100* p,std::uint32_t n,const ParticleSphereV1* m,const float* matrix,bool local,std::int32_t* seed){
 if(!valid(p,n)||!m||!seed)return -1;ParticleSphereV1 domain=*m;if(!local&&matrix)std::copy_n(matrix+12,3,domain.center);
 for(unsigned i=0;i<n;++i){float v[3];const int rc=dh2_particle_sphere_generate_v1(v,&domain,seed);if(rc)return rc;for(unsigned k=0;k<3;++k)put(p[i],4*k,v[k]);}return 0;
}
extern "C" int dh2_particle_color_fallback_v1(ParticleSeed100* p,std::uint32_t n,const std::uint32_t* color){if(!valid(p,n)||!color)return -1;for(unsigned i=0;i<n;++i)std::memcpy(p[i].data()+0x18,color,4);return 0;}
}
