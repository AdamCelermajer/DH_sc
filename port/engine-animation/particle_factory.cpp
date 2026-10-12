#include "particle_factory.hpp"
#include "../engine-resources/resources.hpp"
#include <cstring>
namespace dh2::animation {
namespace {
std::uint32_t word(const std::uint8_t* p){std::uint32_t v;std::memcpy(&v,p,4);return v;}
void put(ParticleContextSeed92& a,unsigned offset,std::uint32_t value){std::memcpy(a.data()+offset,&value,4);}
bool span(const std::vector<std::uint8_t>& b,std::uint32_t at,std::uint32_t n){return at<=b.size()&&n<=b.size()-at;}
bool string(const std::vector<std::uint8_t>& b,std::uint32_t at,std::string& out){if(!at||at>=b.size())return false;const void* end=std::memchr(b.data()+at,0,b.size()-at);if(!end)return false;out.assign(reinterpret_cast<const char*>(b.data()+at),static_cast<const std::uint8_t*>(end)-b.data()-at);return true;}
bool decode_fields(const std::vector<std::uint8_t>& bytes,std::uint32_t index,ParticleEmitterInput& out,std::string& error){
 resources::BresView view{};if(dh2_bres_open(&view,bytes.data(),bytes.size())!=resources::BresError::ok||!span(bytes,view.root_offset,0x80)){error="particle BRES";return false;}
 const auto count=word(bytes.data()+view.root_offset+0x78),table=word(bytes.data()+view.root_offset+0x7c);
 if(index>=count||count>bytes.size()/0x90||!span(bytes,table,count*0x90)){error="particle emitter range";return false;}
 out.record_offset=table+index*0x90;std::memcpy(out.record.data(),bytes.data()+out.record_offset,0x90);
 if(!string(bytes,out.record[0],out.name)){error="particle emitter name";return false;}
 const auto descriptor=out.record[0x54/4];
 if(out.record[0x50/4]!=0||!descriptor||!span(bytes,descriptor,36)){error="unsupported particle factory mode";return false;}
 std::memcpy(out.descriptor.data(),bytes.data()+descriptor,36);
 if(out.descriptor[0]!=3||out.descriptor[1]!=0){error="unsupported particle descriptor";return false;}
 const auto type=out.record[8/4];if(type>2){error="unsupported particle emitter type";return false;}
 const unsigned size=type==0?3:type==1?1:2;const auto shape=out.record[12/4];
 if(!shape||!span(bytes,shape,size*4)){error="particle shape range";return false;}
 out.shape.resize(size);std::memcpy(out.shape.data(),bytes.data()+shape,size*4);return true;
}
}
std::uint32_t ParticleGenerationOwner::hash_name(const char* name){
 std::uint32_t hash=0;
 if(name)for(;*name;++name){const auto byte=std::int32_t(static_cast<std::int8_t>(static_cast<std::uint8_t>(*name)));hash^=std::uint32_t(byte)+0x9e3779b9u+(hash<<6)+(hash>>2);}
 return hash;
}
ParticleGenerationOwner::ParticleGenerationOwner(const ParticleContextSeed92& seed):context_(seed),generation_{1.0f,1,0,0}{
 // Source ctor stores: vtable, zero vector words, individual flags and map
 // header. Source +4/+20/+44/+55..58 remain untouched except stated bytes.
 put(context_,0,0);
 for(unsigned at:{8u,12u,16u,20u,24u,28u,36u,40u,44u,52u,64u,72u,76u,80u})put(context_,at,0);
 context_[0x21]=0;context_[0x30]=0;context_[0x54]=0;
 put(context_,0x38,0);put(context_,0x3c,0);
 register_parameter(hash_name("AnimationDatabase"),context_.data()+0x58);
 register_parameter(hash_name("BirthRate"),&generation_.birth_rate);
 register_parameter(hash_name("MaxParticles"),&generation_.max_particles);
}
std::shared_ptr<ParticleGenerationOwner> ParticleGenerationOwner::create(const ParticleContextSeed92& seed){return std::shared_ptr<ParticleGenerationOwner>(new ParticleGenerationOwner(seed));}
bool ParticleGenerationOwner::register_parameter(std::uint32_t hash,void* storage,std::shared_ptr<void> storage_owner){return parameters_.emplace(hash,Parameter{storage,std::move(storage_owner)}).second;}
void* ParticleGenerationOwner::lookup_hash(std::uint32_t hash){return parameters_[hash].storage;}
ParticleParameterLease ParticleGenerationOwner::parameter(const char* name){if(!name)return {};return {shared_from_this(),lookup_hash(hash_name(name))};}
void ParticleGenerationOwner::set_word(std::uint32_t hash,std::uint32_t value){if(void* pointer=lookup_hash(hash))std::memcpy(pointer,&value,4);}
ParticleContextSeed92 ParticleGenerationOwner::context_projection()const{auto out=context_;put(out,0x34,0);put(out,0x38,0);put(out,0x3c,0);put(out,0x40,std::uint32_t(parameters_.size()));return out;}
bool decode_particle_emitter(std::shared_ptr<const std::vector<std::uint8_t>> bytes,std::uint32_t index,ParticleEmitterInput& output,std::string& error){
 if(!bytes){error="null particle resource";return false;}ParticleEmitterInput result;
 if(!decode_fields(*bytes,index,result,error))return false;
 result.resource=std::make_shared<const std::vector<std::uint8_t>>(*bytes);output=std::move(result);return true;
}
int initialize_particle_generation(ParticleGenerationOwner& owner,const ParticleEmitterInput& input,const ParticleGenerationInitServices& services){
 if(!input.resource)return -1;
 resources::BresView view{};const auto& bytes=*input.resource;
 if(dh2_bres_open(&view,bytes.data(),bytes.size())!=resources::BresError::ok||!span(bytes,view.root_offset,0x80))return -1;
 const auto table=word(bytes.data()+view.root_offset+0x7c);if(input.record_offset<table||(input.record_offset-table)%0x90)return -1;
 ParticleEmitterInput expected;std::string error;
 if(!decode_fields(bytes,(input.record_offset-table)/0x90,expected,error)||expected.record!=input.record||expected.descriptor!=input.descriptor||expected.shape!=input.shape||expected.name!=input.name)return -1;
 if(!services.emitter_type||!services.shape_parameter)return -2;
 int status=services.emitter_type(services.context,owner,input.record[2]);if(status)return status;
 const char* box[]{"RadiusLength","Width","Height"};const char* cylinder[]{"RadiusLength","Height"};
 for(std::size_t i=0;i<input.shape.size();++i){const char* key=input.record[2]==0?box[i]:input.record[2]==1?"RadiusLength":cylinder[i];status=services.shape_parameter(services.context,owner,key,input.shape[i]);if(status)return status;}
 owner.set_word(ParticleGenerationOwner::hash_name("MaxParticles"),input.record[0x18/4]);
 owner.set_word(ParticleGenerationOwner::hash_name("BirthRate"),input.record[0x20/4]);return 0;
}
}
