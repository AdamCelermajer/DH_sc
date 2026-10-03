#include "../character_script_lifecycle.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
namespace {
void check(bool b,const char* m){if(!b)throw std::runtime_error(m);}
struct Reader {
 std::vector<unsigned char> bytes;std::size_t at=0;
 explicit Reader(const char* p){std::ifstream f(p,std::ios::binary);check(bool(f),"Gold missing");bytes={std::istreambuf_iterator<char>(f),{}};}
 template<class T>T get(){check(at+sizeof(T)<=bytes.size(),"Gold truncated");T r;std::memcpy(&r,bytes.data()+at,sizeof(T));at+=sizeof(T);return r;}
};
constexpr std::uintptr_t identities[7]={0,0x100001001ull,0x200002002ull,0x300003003ull,0x400004004ull,0x500005005ull,0x600006006ull};
std::uintptr_t canonical(std::uintptr_t p){for(unsigned i=0;i<7;++i)if(p==identities[i])return i;throw std::runtime_error("Unknown identity");}
ScriptLifecycleState64 encode(ScriptLifecycleState64 s){s.owner=identities[s.owner];s.active=identities[s.active];s.pending=identities[s.pending];s.external_name=identities[s.external_name];return s;}
ScriptLifecycleState64 decode(ScriptLifecycleState64 s){s.owner=canonical(s.owner);s.active=canonical(s.active);s.pending=canonical(s.pending);s.external_name=canonical(s.external_name);return s;}
using Call=std::array<unsigned char,96>;
struct Context {std::array<std::uint32_t,8> params;std::vector<Call> calls;};
void invoke(void* p,ScriptLifecycleState64* s,const ScriptLifecycleRequest32* r,ScriptLifecycleResponse16* response){
 auto& c=*static_cast<Context*>(p);check(!r->reserved,"Reserved request field");check(r->service<=script_ais_terminate,"Unknown service");
 auto normalized=*r;normalized.subject=canonical(r->subject);normalized.payload=canonical(r->payload);const auto snapshot=decode(*s);
 Call row{};std::memcpy(row.data(),&normalized,32);std::memcpy(row.data()+32,&snapshot,64);c.calls.push_back(row);
 if(r->service==c.params[6])switch(c.params[5]){
  case 1:s->pending=identities[5];break;case 2:s->pending=0;break;case 3:s->active=identities[5];break;
  case 4:s->external_name=0;break;case 5:s->delayed=0;break;case 6:s->owner=identities[6];break;
  case 7:s->timer34=-1;break;case 8:s->load_step=6;break;case 9:s->scripted=0;break;
 }
 if(r->service==script_create_step)s->pending=identities[3];
 response->word=0;response->reserved=0;response->identity=0;
 switch(r->service){
  case script_construct_iphone:response->identity=identities[3];break;
  case script_owner_is_character:response->word=c.params[0];break;
  case script_query_budget:response->word=c.params[1];break;
  case script_owner_is_dead:response->word=c.params[2];break;
  case script_design_tick:response->word=c.params[r->argument0==0x33?3:4];break;
  case script_timer_start:response->word=r->argument1==0x33?100:101;break;
 }
 if(c.params[7]&&(r->service==script_ai_init||r->service==script_ai_init_post||r->service==script_ai_init_final)){
  const ScriptLifecycleServices16 services{p,invoke};
  const auto op=r->service==script_ai_init?script_on_init:r->service==script_ai_init_post?script_on_init_post:script_on_init_final;
  check(dh2_character_script_lifecycle(s,op,0,&services)==1,"Nested source wrapper rejected");
 }
}
}
int main(int argc,char** argv){try{
 if(argc!=2)return 2;
 Reader reader(argv[1]);check(reader.get<unsigned>()==0x314c5341,"ASL1 magic differs");const auto cases=reader.get<unsigned>();unsigned requests=0;
 for(unsigned i=0;i<cases;++i){
  const auto op=reader.get<unsigned>(),arg=reader.get<unsigned>();const auto initial=reader.get<ScriptLifecycleState64>();Context c{reader.get<std::array<std::uint32_t,8>>(),{}};
  const auto result=reader.get<unsigned>();const auto expected=reader.get<ScriptLifecycleState64>();const auto n=reader.get<unsigned>();std::vector<Call> calls(n);for(auto& r:calls)r=reader.get<Call>();
  auto s=encode(initial);ScriptLifecycleServices16 services{&c,invoke};check(unsigned(dh2_character_script_lifecycle(&s,op,arg,&services))==result,"Source return differs");const auto actual=decode(s);
  if(std::memcmp(&actual,&expected,64)||c.calls!=calls){std::cerr<<"Mismatch case "<<i<<" op "<<op<<'\n';return 3;}requests+=n;
 }
 check(reader.at==reader.bytes.size(),"Trailing gold");Context c{};ScriptLifecycleState64 base{};base.owner=identities[1];ScriptLifecycleServices16 services{&c,invoke};unsigned rejects=0;
 auto reject=[&](ScriptLifecycleState64* s,unsigned op,unsigned arg,const ScriptLifecycleServices16* cb){const auto before=s?*s:base;const auto count=c.calls.size();check(dh2_character_script_lifecycle(s,op,arg,cb)==-1,"Malformed accepted");if(s)check(!std::memcmp(s,&before,64),"Malformed mutated");check(c.calls.size()==count,"Malformed called service");++rejects;};
 reject(nullptr,0,0,&services);reject(&base,0,0,nullptr);auto no_callback=services;no_callback.invoke=nullptr;reject(&base,0,0,&no_callback);reject(&base,0,0,reinterpret_cast<const ScriptLifecycleServices16*>(&base));reject(&base,10,0,&services);reject(&base,0,2,&services);
 for(unsigned i=0;i<7;++i){auto s=base;switch(i){case 0:s.owner=0;break;case 1:s.load_step=-1;break;case 2:s.delayed=256;break;case 3:s.scripted=256;break;case 4:s.reserved0=1;break;case 5:s.reserved1=1;break;case 6:s.reserved2=1;break;}reject(&s,0,0,&services);}
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<cases<<",\"ordered_service_requests\":"<<requests<<",\"atomic_rejection_checks\":"<<rejects<<",\"mismatches\":0}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}}
