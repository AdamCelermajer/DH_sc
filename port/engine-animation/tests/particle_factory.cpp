#include "../particle_factory.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::animation;
namespace {
std::uint32_t w(const std::uint8_t* p){std::uint32_t v;std::memcpy(&v,p,4);return v;}
void put(std::uint8_t*& p,std::uint32_t v){std::memcpy(p,&v,4);p+=4;}
std::uint32_t identity(void* p,const std::shared_ptr<ParticleGenerationOwner>& owner,std::uint32_t* storage){
 if(!p)return 0;if(p==&owner->generation().birth_rate)return 1;if(p==&owner->generation().max_particles)return 2;
 if(p==owner->parameter("AnimationDatabase").storage)return 3;
 for(unsigned i=0;i<8;++i)if(p==storage+i)return 4+i;throw std::runtime_error("parameter identity");
}
struct Services { std::vector<std::uint32_t> trace; };
int type(void* c,ParticleGenerationOwner& owner,std::uint32_t v){auto& s=*static_cast<Services*>(c);const auto h=owner.hash_name("EmitterType");s.trace.push_back(h);s.trace.push_back(v);owner.set_word(h,v);return 0;}
int shape(void* c,ParticleGenerationOwner& owner,const char* key,std::uint32_t v){auto& s=*static_cast<Services*>(c);const auto h=owner.hash_name(key);s.trace.push_back(h);s.trace.push_back(v);owner.set_word(h,v);return 0;}
int fail_type(void* c,ParticleGenerationOwner& owner,std::uint32_t v){type(c,owner,v);return -77;}
int fail_shape(void* c,ParticleGenerationOwner& owner,const char* key,std::uint32_t v){shape(c,owner,key,v);return -78;}
}
extern "C" int dh2_particle_factory_test(std::uint32_t op,const std::uint8_t* input,std::uint32_t size,std::uint8_t* output){
 if(!input||!output)return -99;
 if(op==1){if(!size||input[size-1]!=0)return -99;auto* p=output;put(p,ParticleGenerationOwner::hash_name(reinterpret_cast<const char*>(input)));return 4;}
 if(size<92)return -99;ParticleContextSeed92 seed;std::memcpy(seed.data(),input,92);auto owner=ParticleGenerationOwner::create(seed);std::uint8_t* out=output;
 std::uint32_t storage[8];for(unsigned i=0;i<8;++i)storage[i]=0x13500000+i;
 if(op==2){
  if(size<96)return -99;const auto count=w(input+92);auto* p=input+96;const auto* end=input+size;
  for(unsigned i=0;i<count;++i){if(end-p<12)return -99;const auto action=w(p),n=w(p+4),value=w(p+8);p+=12;if(n>unsigned(end-p)||!n||p[n-1]!=0)return -99;const char* name=reinterpret_cast<const char*>(p);p+=n;const auto h=owner->hash_name(name);std::uint32_t result;
   if(action==0){if(value!=0xffffffff&&value>=8)return -99;result=owner->register_parameter(h,value==0xffffffff?nullptr:storage+value);}
   else if(action==1)result=identity(owner->parameter(name).storage,owner,storage);
   else if(action==2){owner->set_word(h,value);result=0;}
   else return -99;put(out,result);put(out,std::uint32_t(owner->parameter_count()));
  }if(p!=end)return -99;
 }else if(op==3){
  if(size<100||size-100!=w(input+96))return -99;const auto index=w(input+92);auto raw=std::make_shared<const std::vector<std::uint8_t>>(input+100,input+size);ParticleEmitterInput emitter;std::string error;
  if(!decode_particle_emitter(raw,index,emitter,error))return -98;
  const char* keys[]{"EmitterType","RadiusLength","Width","Height"};for(unsigned i=0;i<4;++i)owner->register_parameter(owner->hash_name(keys[i]),storage+i);
  Services s;const int status=initialize_particle_generation(*owner,emitter,{&s,type,shape});if(status)return status;
  put(out,std::uint32_t(s.trace.size()));for(auto v:s.trace)put(out,v);
  put(out,emitter.record_offset);for(auto v:emitter.record)put(out,v);for(auto v:emitter.descriptor)put(out,v);put(out,std::uint32_t(emitter.shape.size()));for(auto v:emitter.shape)put(out,v);
 }else if(op!=0)return -99;
 const auto context=owner->context_projection();std::memcpy(out,context.data(),92);out+=92;std::memcpy(out,&owner->generation(),16);out+=16;std::memcpy(out,storage,32);out+=32;return int(out-output);
}
#ifndef DH2_PARTICLE_FACTORY_ORACLE
int main(int argc,char** argv){try{
 if(argc!=3)return 2;std::ifstream file(argv[1],std::ios::binary);if(!file)throw std::runtime_error("gold");auto read=[&](void* p,std::size_t n){if(!file.read(static_cast<char*>(p),n))throw std::runtime_error("short gold");};auto u=[&](){std::uint32_t v;read(&v,4);return v;};if(u()!=0x31434650)throw std::runtime_error("magic");const auto cases=u();
 for(unsigned i=0;i<cases;++i){const auto op=u(),n=u();if(n>1000000)throw std::runtime_error("input length");std::vector<std::uint8_t> input(n);read(input.data(),n);const auto length=u();if(length>65536)throw std::runtime_error("output length");std::vector<std::uint8_t> expected(length);read(expected.data(),length);std::uint8_t out[65536]{};const int observed=dh2_particle_factory_test(op,input.data(),n,out);if(observed!=int(length)||std::memcmp(out,expected.data(),length))throw std::runtime_error("gold mismatch "+std::to_string(i));}
 std::ifstream resource(argv[2],std::ios::binary);auto raw=std::make_shared<const std::vector<std::uint8_t>>(std::istreambuf_iterator<char>(resource),std::istreambuf_iterator<char>());ParticleEmitterInput emitter;std::string error;if(!decode_particle_emitter(raw,0,emitter,error))throw std::runtime_error(error);
 ParticleContextSeed92 seed{};auto owner=ParticleGenerationOwner::create(seed);auto lease=owner->parameter("BirthRate");std::weak_ptr<ParticleGenerationOwner> weak=owner;owner.reset();if(weak.expired()||!lease)throw std::runtime_error("lease prematurely released");const float value=2.0f;std::memcpy(lease.storage,&value,4);if(lease.owner->generation().birth_rate!=2.0f)throw std::runtime_error("retained field");lease={};if(!weak.expired())throw std::runtime_error("lease leak");
 owner=ParticleGenerationOwner::create(seed);auto missing=owner->parameter("Missing");if(missing||!missing.owner)throw std::runtime_error("missing lease");if(owner->register_parameter(owner->hash_name("Missing"),const_cast<float*>(&value)))throw std::runtime_error("null insertion replaced");
 auto source=std::make_shared<std::vector<std::uint8_t>>(*raw);ParticleEmitterInput copy;if(!decode_particle_emitter(source,0,copy,error))throw std::runtime_error(error);(*source)[copy.record_offset]=0;if(copy.record[0]!=w(copy.resource->data()+copy.record_offset))throw std::runtime_error("resource alias");source.reset();raw.reset();
 auto bad=copy;bad.record[8]^=1;const auto before=owner->generation();if(initialize_particle_generation(*owner,bad,{nullptr,type,shape})!=-1||std::memcmp(&before,&owner->generation(),16))throw std::runtime_error("malformed input changed owner");if(initialize_particle_generation(*owner,copy,{nullptr,nullptr,nullptr})!=-2)throw std::runtime_error("missing providers accepted");
 ParticleEmitterInput unchanged=copy;if(decode_particle_emitter(nullptr,0,unchanged,error)||unchanged.record!=copy.record)throw std::runtime_error("decode mutation");
 if(decode_particle_emitter(copy.resource,999,unchanged,error)||unchanged.record!=copy.record)throw std::runtime_error("decode range mutation");
 std::uint32_t type_field=99,shape_field=99;owner->register_parameter(owner->hash_name("EmitterType"),&type_field);owner->register_parameter(owner->hash_name("RadiusLength"),&shape_field);Services trace;
 if(initialize_particle_generation(*owner,copy,{&trace,fail_type,shape})!=-77||type_field!=copy.record[2]||shape_field!=99||trace.trace.size()!=2||std::memcmp(&before,&owner->generation(),16))throw std::runtime_error("type provider prefix");trace.trace.clear();
 if(initialize_particle_generation(*owner,copy,{&trace,type,fail_shape})!=-78||shape_field!=copy.shape[0]||trace.trace.size()!=4||std::memcmp(&before,&owner->generation(),16))throw std::runtime_error("shape provider prefix");
 const auto count=owner->parameter_count();if(owner->parameter(nullptr).owner||owner->parameter_count()!=count)throw std::runtime_error("null name effects");
 auto model=std::make_shared<float>(7);std::weak_ptr<float> model_weak=model;owner->register_parameter(owner->hash_name("ProvidedModelField"),model.get(),model);auto field=owner->parameter("ProvidedModelField");model.reset();owner.reset();if(model_weak.expired()||*static_cast<float*>(field.storage)!=7)throw std::runtime_error("provider model lifetime");missing={};field={};if(!model_weak.expired())throw std::runtime_error("provider model leak");
 std::cout<<"{\"validation\":\"PASS\",\"original_gold_cases\":"<<cases<<",\"ownership_checks\":11,\"additional_contract_checks\":4,\"mismatches\":0}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
#endif
