#include "../character_current_skill_v2.hpp"
#include "../character_script_player_vcb_v2.hpp"
#include "../../script-runtime/script_function_alias.h"
#include <array>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
extern "C" int dh2_script_alias_clear_contents(dh2_script_aliases*);
using namespace dh2::character::skills;
namespace {
unsigned checks=0;void check(bool b){++checks;if(!b)throw std::runtime_error("skill-session source check "+std::to_string(checks));}
std::uint32_t word(std::istream& f){std::uint32_t v;check(bool(f.read(reinterpret_cast<char*>(&v),4)));return v;}
float number(std::uint32_t v){float f;std::memcpy(&f,&v,4);return f;}
}
int main(int argc,char** argv){try{
check(argc==2);std::ifstream f(argv[1],std::ios::binary);check(word(f)==0x324b5350);auto vc=word(f),sc=word(f);auto* aliases=dh2_script_alias_create();check(aliases);unsigned owned_vcb=0;
const char* names[]={"OnTargetHit","OnTargetMissed","OnKill"};
for(unsigned i=0;i<vc;++i){auto flags=word(f),mask=word(f),mutation=word(f),expected=word(f);if(mutation)continue;check(!dh2_script_alias_clear_contents(aliases));for(unsigned n=0;n<3;++n)if(mask&(1u<<n))check(!dh2_script_alias_add(aliases,names[n],"MissingGlobalStillMember"));check(!dh2_character_script_player_vcb_v2(&flags,aliases)&&flags==expected);++owned_vcb;}
for(unsigned i=0;i<sc;++i){auto selected=word(f),count=word(f),index=word(f),has=word(f),present=word(f),expected=word(f);std::vector<dh2::data::SavedSkill8V1> rows(count);std::vector<std::int32_t> ids(count);for(unsigned n=0;n<count;++n){rows[n]={static_cast<int>(n),static_cast<std::uint16_t>(word(f)),0,0};ids[n]=n;}std::array<List16,8> lists;for(auto& l:lists)l={ids.data(),count,0};dh2::data::SavedSkillsView16V1 saved{rows.data(),count,0};CurrentSkillView32V2 v{lists.data(),8,20,static_cast<std::int32_t>(selected),0,has?&saved:nullptr};std::int32_t out=0x12345678;check(dh2_character_current_skill_level_v2(&out,&v,number(index),present)==int(present)&&std::uint32_t(out)==expected);}
check(f.peek()==EOF);unsigned guards=0;std::int32_t out=71,id=0;List16 lists[4];for(auto& l:lists)l={&id,1,0};dh2::data::SavedSkill8V1 row{0,65535,0,0};dh2::data::SavedSkillsView16V1 saved{&row,1,0};CurrentSkillView32V2 v{lists,4,1,3,0,&saved};
for(float index:{-1.f,1.f,INFINITY,-INFINITY}){check(dh2_character_current_skill_level_v2(&out,&v,index,1)==-2&&out==71);++guards;}
check(dh2_character_current_skill_level_v2(&out,nullptr,0,1)==-1&&out==71);++guards;v.list_count=3;check(dh2_character_current_skill_level_v2(&out,&v,0,1)==-1&&out==71);++guards;v.list_count=4;v.reserved=1;check(dh2_character_current_skill_level_v2(&out,&v,0,1)==-1&&out==71);++guards;v.reserved=0;id=-1;check(dh2_character_current_skill_level_v2(&out,&v,0,1)==-2&&out==71);++guards;id=0;saved.count=0;check(dh2_character_current_skill_level_v2(&out,&v,0,1)==-2&&out==71);++guards;
std::uint32_t flags=71;check(dh2_character_script_player_vcb_v2(&flags,nullptr)==-1&&flags==71);++guards;check(dh2_character_script_player_vcb_v2(nullptr,aliases)==-1);++guards;dh2_script_alias_destroy(aliases);
std::cout<<"{\"validation\":\"PASS\",\"original_numeric_skill_cases\":"<<sc<<",\"original_owned_alias_cases\":"<<owned_vcb<<",\"hooked_alias_mutation_cases_arm64_only\":"<<vc-owned_vcb<<",\"atomic_guards\":"<<guards<<",\"checks\":"<<checks<<",\"mismatches\":0}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
