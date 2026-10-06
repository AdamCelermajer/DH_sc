#include "../../engine-animation/particle_billboard_v32.hpp"
#include <cstring>
using namespace dh2::animation;
extern "C" unsigned dh2_fx_source_v32(unsigned op,const unsigned char* in,unsigned char* out){
 unsigned n;std::memcpy(&n,in,4);
 if(n>256)return 0;
 ParticleSeed100 particles[256]{};
 if(op==0){
  std::memcpy(particles,in+84,n*100);float camera[3],world[16];unsigned local;
  std::memcpy(camera,in+4,12);std::memcpy(world,in+16,64);std::memcpy(&local,in+80,4);
  ParticleBillboardBoundsV1 bounds{};
  if(dh2_particle_billboard_apply_v32(particles,n,camera,world,local!=0,&bounds))return 0;
  std::memcpy(out,&bounds,24);std::memcpy(out+24,particles,n*100);return 24+n*100;
 }
 if(op==1){
  unsigned seed;std::memcpy(&seed,in+4,4);ParticleSpinModelV1 spin;
  std::memcpy(&spin,in+8,sizeof spin);std::memcpy(particles,in+8+sizeof spin,n*100);
  if(dh2_particle_spin_init_v1(particles,n,&spin,reinterpret_cast<int*>(&seed)))return 0;
  std::memcpy(out,&seed,4);std::memcpy(out+4,particles,n*100);return 4+n*100;
 }
 return 0;
}
