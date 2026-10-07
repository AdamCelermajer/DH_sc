#include "../character_skill_info_v1.hpp"
#include <array>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <vector>
using namespace dh2::character::skills;
static void check(bool b,const char* m){if(!b)throw std::runtime_error(m);}
static float number(std::uint32_t v){float f;std::memcpy(&f,&v,4);return f;}
static std::uint32_t word(float f){std::uint32_t v;std::memcpy(&v,&f,4);return v;}
struct Fixture {std::array<std::uint32_t,16> v{};Instance32 instance{};State40 state{};Instance32* slots[2]{};std::vector<std::uint32_t> trace;};
static int invoke(void* p,const SkillInfoRequestV1* q,SkillInfoResponseV1* r){auto& c=*static_cast<Fixture*>(p);auto& v=c.v;*r={};c.trace.insert(c.trace.end(),{q->operation,static_cast<std::uint32_t>(q->owner),static_cast<std::uint32_t>(q->active),q->operation==3?q->level:q->operation==4?static_cast<std::uint32_t>(q->timer):0});
 if(q->operation==1)r->active=v[q->owner+1]?q->owner:0;
 else if(q->operation==2){r->source_error=v[4];r->return_count=v[5];if(v[13]&1)c.instance.owner=2;}
 else if(q->operation==3){r->source_error=v[6];r->return_count=v[7];r->first_type=v[8];r->first_number=number(v[9]);if(v[13]&2)c.instance.owner=2;}
 else if(q->operation==4){r->timer_found=v[10];r->elapsed=v[11];r->duration=v[14];}else return -1;
 return 0;
}
static std::uint32_t read(std::istream& f){std::uint32_t v;check(bool(f.read(reinterpret_cast<char*>(&v),4)),"Truncated skill gold");return v;}
int main(int argc,char** argv){try{check(argc==2,"Usage: skill_info_audit gold.bin");std::ifstream in(argv[1],std::ios::binary);check(read(in)==0x31494653,"Gold magic");auto count=read(in);std::size_t services=0;
 for(unsigned i=0;i<count;++i){Fixture c;check(read(in)==64,"Input size");for(auto& v:c.v)v=read(in);auto n=read(in);check(n>=8&&(n-8)%16==0,"Output size");auto expected=read(in),calls=read(in);check(n==8+calls*16,"Trace size");std::vector<std::uint32_t> trace(calls*4);for(auto& v:trace)v=read(in);
 c.instance={1,"skill",0,17,0,0};c.slots[0]=c.v[0]?&c.instance:nullptr;c.state.owner=1;c.state.skills={c.slots,2,0};float fraction=number(c.v[12]);SkillInfoServicesV1 svc{&c,invoke};check(character_skill_info_v1(&c.state,0,c.v[1],c.v[15]?&fraction:nullptr,&svc)==0,"Kernel failure");check(word(fraction)==expected||(std::isnan(fraction)&&std::isnan(number(expected))),"Fraction mismatch");check(c.trace==trace,"Ordered service mismatch");services+=calls;
 }
 check(in.peek()==EOF,"Gold suffix");Fixture c;c.slots[0]=&c.instance;c.state.skills={c.slots,1,0};float f=19;SkillInfoServicesV1 svc{&c,invoke};unsigned guards=0;
 check(character_skill_info_v1(nullptr,0,0,&f,&svc)==-1&&f==19,"Null state");++guards;
 check(character_skill_info_v1(&c.state,1,0,&f,&svc)==-1&&f==19,"Index bounds");++guards;
 c.slots[0]=reinterpret_cast<Instance32*>(reinterpret_cast<std::uintptr_t>(&c.instance)+1);check(character_skill_info_v1(&c.state,0,0,&f,&svc)==-1&&f==19,"Unaligned instance");++guards;
 std::printf("{\"validation\":\"PASS\",\"cases\":%u,\"ordered_services\":%zu,\"atomic_guards\":%u,\"mismatches\":0}\n",count,services,guards);return 0;
 }catch(const std::exception& e){std::fprintf(stderr,"%s\n",e.what());return 1;}}
