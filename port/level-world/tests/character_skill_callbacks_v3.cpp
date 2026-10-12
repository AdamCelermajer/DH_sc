#include "../character_skill_callbacks_v3.hpp"
#include "../character_skill_buff_bindings_v3.hpp"
#include <array>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::character::skills;
namespace {
unsigned checks{};void check(bool x,const char* e){++checks;if(!x)throw std::runtime_error(e);}
unsigned word(std::istream& f){unsigned x;check(bool(f.read(reinterpret_cast<char*>(&x),4)),"Truncated callback gold");return x;}
struct Fixture {
 std::array<unsigned,8> input{};Instance32 instance{0x111,"source",0,-1,0,0};
 std::vector<std::array<unsigned,4>> trace;unsigned fail{};
 static int invoke(void* p,const SkillCallbackRequest48V3* q,SkillCallbackResponse32V3* r){auto& t=*static_cast<Fixture*>(p);check(q->instance==&t.instance,"Retained live instance");auto owner=q->owner==0x111?1u:q->owner==0x222?2u:0u;auto active=q->active==0x1111?1u:q->active==0x2222?2u:0u;t.trace.push_back({q->operation,q->index,owner,active});if(q->operation==t.fail)return -1;
  switch(q->operation){case 1:r->active=owner==1?(t.input[1]?0x1111:0):0x2222;break;
   case 2:r->results=1;r->source_error=t.input[2];r->count=t.input[3];t.instance.owner=0x222;break;
   case 3:break;case 4:r->results=2;r->source_error=t.input[4];r->count=t.input[5];break;
   case 5:r->boolean=t.input.at(6+q->index);break;case 6:break;default:return -1;}return 0;
 }
};
}
int main(int argc,char** argv){try{check(argc==2,"Usage: callback gold");std::ifstream f(argv[1],std::ios::binary);check(word(f)==0x334c4b53,"Callback gold magic");auto cases=word(f);unsigned services=0;
 for(unsigned i=0;i<cases;++i){Fixture t;for(auto& x:t.input)x=word(f);auto expected=word(f),count=word(f);std::vector<std::array<unsigned,4>> trace(count);for(auto& row:trace)for(auto& x:row)x=word(f);SkillCallbackServices16V3 cb{&t,Fixture::invoke};unsigned out=77;check(!dh2_character_skill_callback_v3(&out,&t.instance,t.input[0],&cb),"Native callback completion");check(out==expected&&trace==t.trace,"Original return/cardinality/order/reload");services+=count;}
 check(f.peek()==EOF,"Callback trailing gold");unsigned guards=0;
 for(unsigned failure=1;failure<=6;++failure){Fixture t;t.input={0,1,0,1,0,1,1,1};t.fail=failure;SkillCallbackServices16V3 cb{&t,Fixture::invoke};unsigned out=77;check(dh2_character_skill_callback_v3(&out,&t.instance,0,&cb)==-2&&out==77&&t.trace.back()[0]==failure,"Required service preserves failed prefix and unpublished result");++guards;}
 Fixture malformed;SkillCallbackServices16V3 cb{&malformed,Fixture::invoke};unsigned out=77;malformed.instance.reserved=1;check(dh2_character_skill_callback_v3(&out,&malformed.instance,0,&cb)==-1&&out==77&&malformed.trace.empty(),"Atomic malformed callback");++guards;
 SkillBuffBindingsV3 missing_fx{reinterpret_cast<dh2::character::BuffOwner*>(0x111),1,UINT32_MAX,nullptr,nullptr};dh2_script_value args[5]{},value{};args[0].type=3;args[4].type=1;args[4].boolean=1;unsigned count=77;char error[128]{};check(skill_create_buff_v3(&missing_fx,args,5,&value,1,&count,error,sizeof(error))==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE&&count==0,"Unknown genuine FX catalog fails before fake no-FX creation");++guards;
 std::cout<<"{\"validation\":\"PASS\",\"original_cases\":"<<cases<<",\"ordered_services\":"<<services<<",\"atomic_and_required_guards\":"<<guards<<",\"checks\":"<<checks<<"}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
