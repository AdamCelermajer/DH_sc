#include "../character_skill_ai_v3.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::character::skills;
namespace {
unsigned checks{};void check(bool x,const char* message){++checks;if(!x)throw std::runtime_error(message);}
std::uint32_t word(std::istream& f){std::uint32_t x;check(bool(f.read(reinterpret_cast<char*>(&x),4)),"Truncated gold");return x;}
std::int32_t signed_word(std::uint32_t x){std::int32_t y;std::memcpy(&y,&x,4);return y;}
struct Fixture {
 std::array<std::uint32_t,16> input;std::array<std::uint32_t,5> answers;
 SkillAIStateV3 fields;SkillAIOwnerV3 owner{0xabcdef1234567890ull,0,0};Instance32 instance{owner.character,"borrowed",0,-1,0,0};const Instance32* item=&instance;
 State40 slots{owner.character,{&item,1,0},{}};SkillAIContextV3 state{&owner,&slots,&fields,0,0};dh2::data::SkillProjection76 row{};
 std::vector<std::string> names;std::vector<const char*> pointers;std::vector<std::array<unsigned,4>> trace;unsigned using_skill{};
 void prepare(){using_skill=input[0];owner.flags=input[4];fields={signed_word(input[5]),std::uint8_t(input[6]),std::uint8_t(input[7]),0};item=input[8]?&instance:nullptr;row.words[18]=input[9];row.words[2]=input[10];state.script_step=signed_word(input[3]);for(auto& n:names)pointers.push_back(n.c_str());}
 static int invoke(void* p,SkillAIContextV3* s,const SkillAIRequest32V3* q,SkillAIResponse32V3* r){auto& t=*static_cast<Fixture*>(p);check(s==&t.state&&q->character==t.owner.character,"Live source receiver");unsigned subject=q->subject==reinterpret_cast<std::uintptr_t>(&t.instance)?1:q->subject==0x111?2:q->subject==0x222?3:0;t.trace.push_back({q->operation,q->index,q->value,subject});
  switch(q->operation){case 1:r->word=t.using_skill;break;case 2:r->word=t.input[1];break;case 3:r->row=&t.row;break;
   case 4:if(t.input[15]==1&&q->value==3){t.fields={9,1,1,0};t.row.words[2]=77;}if(t.input[15]==3&&q->value==4)t.fields={13,1,1,0};r->word=t.answers.at(q->value);break;
   case 5:if(t.input[15]==2)t.fields={0,1,1,0};t.using_skill=t.input[2];break;
   case 6:r->word=t.input[11];break;case 7:r->word=q->value?t.input[12]:t.input[13];break;
   case 8:r->identity=0x111;break;case 9:r->word=t.input[14];break;case 10:r->names=t.pointers.data();r->count=t.pointers.size();break;case 11:case 12:break;default:return -1;
  }return 0;
 }
};
}
int main(int argc,char** argv){try{check(argc==2,"Usage: skill_ai gold");std::ifstream f(argv[1],std::ios::binary);check(word(f)==0x33494153,"Gold magic");unsigned cases=word(f),requests=0;
 for(unsigned c=0;c<cases;++c){auto size=word(f);auto start=f.tellg();auto operation=word(f);Fixture t;for(auto& x:t.input)x=word(f);for(auto& x:t.answers)x=word(f);auto names=word(f);while(names--){std::string n(word(f),0);check(bool(f.read(n.data(),n.size())),"Gold names");t.names.push_back(std::move(n));}auto expected=word(f);SkillAIStateV3 fields{signed_word(word(f)),std::uint8_t(word(f)),std::uint8_t(word(f)),0};auto count=word(f);std::vector<std::array<unsigned,4>> trace(count);for(auto& row:trace)for(auto& x:row)x=word(f);check(f.tellg()-start==std::streamoff(size),"Gold record size");t.prepare();SkillAIServices16V3 services{&t,Fixture::invoke};unsigned result=123;check(!dh2_character_skill_ai_v3(&result,&t.state,operation,0,&services),"Native coordinator");check(result==expected&&t.fields.current==fields.current&&t.fields.continued==fields.continued&&t.fields.last==fields.last,"Original answer/fields");check(t.trace==trace,"Original ordered requests");requests+=count;}
 check(f.peek()==EOF,"Gold trailing bytes");Fixture bad;bad.input.fill(0);bad.answers.fill(0);bad.prepare();SkillAIServices16V3 cb{&bad,Fixture::invoke};unsigned out=77,guards=0;
 for(unsigned op:{9u,UINT32_MAX}){check(dh2_character_skill_ai_v3(&out,&bad.state,op,0,&cb)==-1&&out==77&&bad.trace.empty(),"Atomic invalid operation");++guards;}
 auto* original=bad.state.owner;bad.state.owner=reinterpret_cast<SkillAIOwnerV3*>(reinterpret_cast<std::uintptr_t>(original)+1);check(dh2_character_skill_ai_v3(&out,&bad.state,0,0,&cb)==-1&&out==77,"Unaligned borrowed owner");++guards;bad.state.owner=original;
 check(dh2_character_skill_ai_v3(&out,&bad.state,1,1,&cb)==-2&&out==77&&bad.trace.empty(),"Active unsafe assertion domain");++guards;
 bad.slots.skills.reserved=1;check(dh2_character_skill_ai_v3(&out,&bad.state,0,0,&cb)==-1&&out==77&&bad.trace.empty(),"Reserved borrowed slots rejected atomically");++guards;
 std::cout<<"{\"validation\":\"PASS\",\"original_cases\":"<<cases<<",\"ordered_services\":"<<requests<<",\"atomic_and_required_guards\":"<<guards<<",\"checks\":"<<checks<<"}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
