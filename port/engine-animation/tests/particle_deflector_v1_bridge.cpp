#include "../particle_deflector_v1.hpp"
#include <cstring>
extern "C" unsigned dh2_particle_deflector_test_v1(const unsigned char* input,unsigned length,unsigned char* output){
 using namespace dh2::animation;
 if(!input||!output||length<168)return 0;
 unsigned n;std::int32_t seed;float dt;std::memcpy(&n,input,4);std::memcpy(&seed,input+4,4);std::memcpy(&dt,input+8,4);
 if(n>1024||length!=168+n*100)return 0;
 ParticleDeflectorParametersV1 parameters;std::memcpy(&parameters,input+12,28);
 dh2::math::Matrix4f previous{},current{};std::memcpy(previous.m,input+40,64);std::memcpy(current.m,input+104,64);
 previous.identity_hint=1;current.identity_hint=1;
 ParticleDeflectorModelV1 model;const int ctor=dh2_particle_deflector_construct_v1(&model,&parameters,&previous);if(ctor)return 0;
 // Output is aligned by the native harness and actual Particles are byte arrays.
 auto* particles=reinterpret_cast<ParticleSeed100*>(output+140);std::memcpy(particles,input+168,n*100);
 const int result=dh2_particle_deflector_apply_v1(particles,n,&model,&current,dt,&seed);
 std::memcpy(output,&result,4);std::memcpy(output+4,&seed,4);std::memcpy(output+8,&model.previous,65);std::memcpy(output+73,&current,65);
 output[138]=output[139]=0;return 140+n*100;
}
