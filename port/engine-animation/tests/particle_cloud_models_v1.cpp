#include "../particle_cloud_models_v1.hpp"
#include <cstring>
using namespace dh2::animation;
extern "C" std::uint32_t dh2_particle_cloud_models_test_v1(unsigned op,const unsigned char* input,unsigned bytes,unsigned char* output){
 if(!input||!output||bytes<204)return 0;unsigned count;std::int32_t seed;float dt;std::memcpy(&count,input,4);std::memcpy(&seed,input+4,4);std::memcpy(&dt,input+8,4);if(count>80||bytes!=204+count*100)return 0;
 ParticleSeed100 p[80];if(count)std::memcpy(p,input+204,count*100);int rc=-1;
 ParticleLifeModelV1 life;ParticleSizeModelV1 size;ParticleSpinModelV1 spin;ParticleMotionModelV1 motion;ParticleSphereV1 sphere;ParticleGravityV1 gravity;float matrix[16];std::memcpy(matrix,input+140,64);
 switch(op){case 0:std::memcpy(&life,input+12,sizeof life);rc=dh2_particle_life_init_v1(p,count,&life,dt,&seed);break;case 1:rc=dh2_particle_life_apply_v1(p,count,dt);break;case 2:std::memcpy(&size,input+12,sizeof size);rc=dh2_particle_size_init_v1(p,count,&size,&seed);break;case 3:std::memcpy(&size,input+12,sizeof size);rc=dh2_particle_size_apply_v1(p,count,&size);break;case 4:rc=dh2_particle_motion_apply_v1(p,count,dt);break;case 5:rc=dh2_particle_spin_apply_v1(p,count,dt);break;case 6:std::memcpy(&spin,input+12,sizeof spin);rc=dh2_particle_spin_init_v1(p,count,&spin,&seed);break;case 7:std::memcpy(&motion,input+12,sizeof motion);rc=dh2_particle_motion_init_v1(p,count,&motion,matrix,&seed);break;case 8:{float v[3];std::memcpy(&sphere,input+12,sizeof sphere);rc=dh2_particle_sphere_generate_v1(v,&sphere,&seed);std::memcpy(p[0].data(),v,12);break;}case 9:std::memcpy(&gravity,input+12,sizeof gravity);rc=dh2_particle_gravity_apply_v1(p,count,&gravity,matrix,dt);break;}
 std::memcpy(output,&rc,4);std::memcpy(output+4,&seed,4);if(count)std::memcpy(output+8,p,count*100);return 8+count*100;
}
