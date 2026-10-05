#include "../particle_cloud_runtime_v1.hpp"
#include "../particle_billboard_v1.hpp"
#include <cstring>
using namespace dh2::animation;
extern "C" int dh2_particle_cloud_graph_test_v1(const unsigned char* input,unsigned char* output){
 unsigned old_count,new_count;std::int32_t seed;std::memcpy(&old_count,input,4);std::memcpy(&new_count,input+4,4);std::memcpy(&seed,input+8,4);if(old_count+new_count>30)return -1;
 ParticleCloudModelsV1 models;static_assert(sizeof models==112);std::memcpy(&models,input+12,112);float world[16],camera[3];std::memcpy(world,input+124,64);std::memcpy(camera,input+188,12);std::uint32_t color;std::memcpy(&color,input+200,4);
 auto generation=ParticleGenerationOwner::create({});generation->set_word(generation->hash_name("MaxParticles"),30);ParticleEmissionOwner emission(generation);ParticleBillboardBoundsV1 bounds;
 ParticleCloudRuntimeV1 runtime(emission,models,seed,seed,{[](){return 0;},[&](ParticleSeed100* p,unsigned n){return dh2_particle_billboard_apply_v1(p,n,camera,world,false,&bounds);}});if(runtime.initialize())return -2;
 const auto set_birth=[&](unsigned n){float f=float(n);unsigned bits;std::memcpy(&bits,&f,4);generation->set_word(generation->hash_name("BirthRate"),bits);};set_birth(old_count);ParticleSeed100 untouched;std::memcpy(untouched.data(),input+204,100);ParticleEmissionRange8 range;if(emission.generate(1,0,untouched,range))return -3;
 for(unsigned i=0;i<old_count;++i)std::memcpy(emission.particle(i)->data(),input+304+100*i,100);set_birth(new_count);ParticleGravityV1 gravity{1,0,0};ParticleCloudFrameV1 frame{world,false,&color,{{&gravity,world}}};if(runtime.update(1,untouched,frame))return -4;
 auto count=unsigned(emission.particles().size());std::memcpy(output,&seed,4);std::memcpy(output+4,&count,4);std::memcpy(output+8,&bounds,24);for(unsigned i=0;i<count;++i)std::memcpy(output+32+100*i,emission.particle(i)->data(),100);return 32+100*count;
}
