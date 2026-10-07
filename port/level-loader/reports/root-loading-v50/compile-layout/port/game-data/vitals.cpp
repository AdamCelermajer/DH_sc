#include "vitals.hpp"
#include <cstring>
namespace {
std::int32_t bits(std::uint32_t n){std::int32_t v;std::memcpy(&v,&n,4);return v;}
}
extern "C" unsigned dh2_vitals_regen(dh2::data::PropertyView* v,unsigned mana,std::int32_t amount,dh2::data::VitalsChange* out){
 if(!out||mana>1||dh2_property_validate(v))return 1;
 const unsigned current_id=mana?41:36,maximum_id=mana?43:38;
 const auto current=v->resolved[current_id],maximum=v->resolved[maximum_id];
 auto delta=amount<0?maximum:amount;
 if(bits(std::uint32_t(current)+std::uint32_t(delta))>maximum)delta=bits(std::uint32_t(maximum)-std::uint32_t(current));
 const auto positive=delta>0?delta:0;
 if(positive&&dh2_property_add(v,current_id,positive))return 1;
 *out={positive,current,v->resolved[current_id]};return 0;
}
extern "C" unsigned dh2_vitals_initialize(dh2::data::PropertyView* v,dh2::data::VitalsChange* hp,dh2::data::VitalsChange* mp){
 if(!hp||!mp||dh2_property_validate(v))return 1;
 return dh2_vitals_regen(v,0,-1,hp)||dh2_vitals_regen(v,1,-1,mp)?1:0;
}
extern "C" unsigned dh2_vitals_spawn_init(dh2::data::PropertyView* v,dh2::data::SpawnVitals* result){
 if(!result||dh2_property_validate(v))return 1;
 dh2::data::SpawnVitals candidate;
 if(dh2_vitals_initialize(v,&candidate.first_hp,&candidate.first_mp)||dh2_vitals_initialize(v,&candidate.second_hp,&candidate.second_mp))return 1;
 *result=candidate;return 0;
}
namespace dh2::data {
bool initialize_spawn_vitals(const PropertyRules& rules,PropertyState& state,SpawnVitals& result,std::string& error){
 error.clear();auto next=state;auto view=property_view(rules,next);SpawnVitals candidate;
 if(dh2_vitals_spawn_init(&view,&candidate)){error="Invalid original spawn-vitals property view";return false;}
 state=next;result=candidate;return true;
}
}
