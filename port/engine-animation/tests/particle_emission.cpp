#include "../particle_emission.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::animation;
namespace {
std::uint32_t w(const std::uint8_t* p){std::uint32_t v;std::memcpy(&v,p,4);return v;}
void write(std::uint8_t*& p,const void* v,std::size_t n){std::memcpy(p,v,n);p+=n;}
void u(std::uint8_t*& p,std::uint32_t v){write(p,&v,4);}
}
extern "C" int dh2_particle_emission_test(std::uint32_t op,const std::uint8_t* input,std::uint32_t size,std::uint8_t* output){
 if(!input||!output)return -99;auto* out=output;
 if(op==0){if(size!=132)return -99;ParticleEmission32 s;ParticleSeed100 seed;std::memcpy(&s,input,32);std::memcpy(seed.data(),input+32,100);ParticleEmissionRange8 r;int status=dh2_particle_emission(&s,&r);if(status)return status;dh2_particle_emission_template(&seed);write(out,&s,32);write(out,&r,8);write(out,seed.data(),100);}
 else if(op==1){if(size<16||size-16!=w(input+12))return -99;std::string error;auto resource=ParticleAnimationResource::create(input+16,size-16,error);if(!resource)return -98;auto cursor=std::int32_t(w(input+8));float value=0;int status=resource->sample(w(input),std::int32_t(w(input+4)),cursor,value);if(status)return status;write(out,&cursor,4);write(out,&value,4);}
 else if(op==2){if(size<104)return -99;const auto n=w(input);if(size!=104+16*n)return -99;ParticleSeed100 seed;std::memcpy(seed.data(),input+4,100);auto owner=ParticleGenerationOwner::create({});ParticleEmissionOwner emission(owner);if(emission.initialize())return -97;
  for(std::uint32_t i=0;i<n;++i){const auto* p=input+104+16*i;owner->set_word(owner->hash_name("BirthRate"),w(p));owner->set_word(owner->hash_name("MaxParticles"),w(p+4));float now,last;std::memcpy(&now,p+8,4);std::memcpy(&last,p+12,4);ParticleEmissionRange8 range;if(emission.generate(now,last,seed,range))return -96;write(out,&owner->generation(),16);write(out,&range,8);u(out,std::uint32_t(emission.particles().size()));for(const auto& particle:emission.particles())write(out,particle.data(),100);}
 }else return -99;return int(out-output);
}
#ifndef DH2_PARTICLE_EMISSION_ORACLE
int main(int argc,char** argv){try{if(argc!=3)return 2;std::ifstream gold(argv[1],std::ios::binary);auto read=[&](void* p,std::size_t n){if(!gold.read(static_cast<char*>(p),n))throw std::runtime_error("short gold");};auto word=[&](){std::uint32_t v;read(&v,4);return v;};if(word()!=0x314d4550)throw std::runtime_error("magic");const auto cases=word();
 for(unsigned i=0;i<cases;++i){auto op=word(),n=word();std::vector<std::uint8_t> input(n);read(input.data(),n);auto length=word();std::vector<std::uint8_t> expected(length),actual(length);read(expected.data(),length);int observed=dh2_particle_emission_test(op,input.data(),n,actual.data());if(observed!=int(length)||actual!=expected)throw std::runtime_error("gold mismatch "+std::to_string(i));}
 std::ifstream file(argv[2],std::ios::binary);std::vector<std::uint8_t> raw{std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>()};std::string error;auto resource=ParticleAnimationResource::create(raw.data(),raw.size(),error);if(!resource||resource->track_count()!=2)throw std::runtime_error(error);auto retained=resource;raw.assign(raw.size(),0);
 std::ifstream emitter_file(argv[2],std::ios::binary);auto emitter_bytes=std::make_shared<const std::vector<std::uint8_t>>(std::istreambuf_iterator<char>(emitter_file),std::istreambuf_iterator<char>());auto generation=ParticleGenerationOwner::create({});ParticleEmitterInput emitter;if(!decode_particle_emitter(emitter_bytes,0,emitter,error))throw std::runtime_error(error);generation->set_word(generation->hash_name("MaxParticles"),emitter.record[6]);ParticleEmissionOwner emission(generation);if(emission.initialize())throw std::runtime_error("initialize");ParticleBirthRateBinding binding{resource,generation->parameter("BirthRate"),0,0};
 auto dust_generation=ParticleGenerationOwner::create({});if(!decode_particle_emitter(emitter_bytes,1,emitter,error))throw std::runtime_error(error);dust_generation->set_word(dust_generation->hash_name("MaxParticles"),emitter.record[6]);ParticleEmissionOwner dust_emission(dust_generation);if(dust_emission.initialize())throw std::runtime_error("dust initialize");ParticleBirthRateBinding dust_binding{resource,dust_generation->parameter("BirthRate"),1,0};dust_generation.reset();
 std::weak_ptr<const ParticleAnimationResource> weak_resource=resource;std::weak_ptr<ParticleGenerationOwner> weak_generation=generation;resource.reset();generation.reset();ParticleSeed100 seed;seed.fill(0xcd);float last=0;unsigned emitted=0,dust_emitted=0;
 for(int ms=0;ms<=2000;ms+=10){if(binding.sample_apply(ms)||dust_binding.sample_apply(ms))throw std::runtime_error("sample apply");ParticleEmissionRange8 range;if(emission.generate(float(ms)/1000,last,seed,range))throw std::runtime_error("emission");if(range.end>=range.first)emitted+=range.end-range.first;if(dust_emission.generate(float(ms)/1000,last,seed,range))throw std::runtime_error("dust emission");if(range.end>=range.first)dust_emitted+=range.end-range.first;last=float(ms)/1000;}
 if(weak_resource.expired()||weak_generation.expired()||!emitted||!dust_emitted||emission.particles().empty()||dust_emission.particles().empty()||emission.particle(0)==dust_emission.particle(0))throw std::runtime_error("retained actual FX path");
 auto before=emission.particles();auto fields=emission.generation_owner()->generation();ParticleEmissionRange8 rejected{0x1234,0x5678};float bad;std::uint32_t infinity=0x7f800000;std::memcpy(&bad,&infinity,4);if(emission.generate(bad,last,seed,rejected)!=-1||emission.particles()!=before||std::memcmp(&fields,&emission.generation_owner()->generation(),16)||rejected.first!=0x1234)throw std::runtime_error("atomic bad frame");
 float value=1;auto cursor=binding.cursor;if(binding.resource->sample(99,0,cursor,value)!=-1||value!=1||cursor!=binding.cursor)throw std::runtime_error("sample guard");binding={};dust_binding={};retained.reset();if(!weak_resource.expired())throw std::runtime_error("resource leak");
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<cases<<",\"actual_fx_frames\":402,\"emitted_particles\":["<<emitted<<","<<dust_emitted<<"],\"retained_resource_and_generation\":true,\"atomic_guards\":2}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
#endif
