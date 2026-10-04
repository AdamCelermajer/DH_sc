#include "../character_current_spell_v1.hpp"
using namespace dh2::character::skills;
struct OracleV1 {std::int32_t first,second,level;std::uint32_t calls,trace[16];};
static int invoke(void* p,const CurrentSpellRequest24V1* request,CurrentSpellResponse16V1* response){
 auto& s=*static_cast<OracleV1*>(p);const auto step=s.calls++;
 if(step>=4)return 1;
 s.trace[step*4]=request->operation;s.trace[step*4+1]=request->id;
 s.trace[step*4+2]=static_cast<std::uint32_t>(request->difficulty);s.trace[step*4+3]=request->character!=0;
 response->value=request->operation==current_spell_selected_faery?(step==0?s.first:s.second):request->operation==current_spell_saved_level?s.level:0;
 return 0;
}
extern "C" int dh2_current_spell_oracle_v1(std::int32_t* out,OracleV1* input){
 const CurrentSpellServices16V1 services{input,invoke};return dh2_character_current_spell_level_v1(out,0x100000001ull,&services);
}
