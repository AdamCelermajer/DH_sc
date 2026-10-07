// Source init including actual box0 or sphere1 generation prefix.
#include "particle_resource_init_v2.hpp"
#include <cstring>
namespace dh2::animation {
namespace {
std::uint32_t read(const std::vector<std::uint8_t>& b,std::uint32_t p){std::uint32_t x;std::memcpy(&x,b.data()+p,4);return x;}
bool span(const std::vector<std::uint8_t>& b,std::uint32_t p,std::size_t n){return p<=b.size()&&n<=b.size()-p;}
void store(ParticleGenerationOwner& o,const char* key,std::uint32_t value){o.set_word(ParticleGenerationOwner::hash_name(key),value);}
}
int initialize_particle_resource_v2(ParticleGenerationOwner& owner,const ParticleEmitterInput& input,
 const ParticleGenerationInitServices& generation,const ParticleResourceInitServicesV2& services){
 if(!input.resource||input.record[2]>1||input.record[0x48/4]!=1||input.record[0x88/4]!=0)return -1;
 const auto& bytes=*input.resource;const auto direction=input.record[0x4c/4];
 if(!direction||!span(bytes,direction,16))return -1;
 const int prefix=initialize_particle_generation(owner,input,generation);if(prefix)return prefix;
 const char* initial[]{"Life","LifeVariation","TargetSize","SizeVariation","SizeGrowthTime","SizeFadeTime","Speed","SpeedVariation"};
 for(unsigned i=0;i<8;++i)store(owner,initial[i],input.record[0x28/4+i]);
 if(!services.vector_parameter)return -2;
 std::uint32_t vector[3]{read(bytes,direction),read(bytes,direction+4),read(bytes,direction+8)};
 int status=services.vector_parameter(services.context,owner,"Direction",vector);if(status)return status;
 store(owner,"DirectionVariation",read(bytes,direction+12));
 store(owner,"AnimKeyMappingType",input.record[0x64/4]);
 const char* animation[]{"AnimOffset","AnimOffsetVariation","AnimLength","AnimLengthVariation","AnimScaleMultiplier","AnimScaleMultiplierVariation"};
 const unsigned offsets[]{0x5c,0x60,0x68,0x6c,0x70,0x74};
 for(unsigned i=0;i<6;++i)store(owner,animation[i],input.record[offsets[i]/4]);
 const char* spin[]{"SpinTime","SpinVariation","SpinPhase","SpinPhaseVariation","SpinAxisType"};
 for(unsigned i=0;i<5;++i)store(owner,spin[i],input.record[0x78/4+i]);
 const std::uint32_t zero[3]{};
 status=services.vector_parameter(services.context,owner,"SpinAxis",zero);if(status)return status;
 store(owner,"SpinAxisVariation",0);
 if(!services.render_initialize)return -2;
 return services.render_initialize(services.context,owner,input);
}
}
