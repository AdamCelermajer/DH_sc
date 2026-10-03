#include "../character_script_update.hpp"
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
constexpr std::uintptr_t identities[6]={0,0x100001001ull,0x200002002ull,0x300003003ull,0x400004004ull,0x500005005ull};
std::uintptr_t canonical(std::uintptr_t p){for(unsigned i=0;i<6;++i)if(p==identities[i])return i;throw std::runtime_error("Unknown identity");}
ScriptUpdateState48 encode(ScriptUpdateState48 s){s.active=identities[s.active];s.script_owner=identities[s.script_owner];s.ai_owner=identities[s.ai_owner];s.controller=identities[s.controller];return s;}
ScriptUpdateState48 decode(ScriptUpdateState48 s){s.active=canonical(s.active);s.script_owner=canonical(s.script_owner);s.ai_owner=canonical(s.ai_owner);s.controller=canonical(s.controller);return s;}
using Call=std::array<unsigned char,80>;
struct Context {std::array<std::uint32_t,2> params;std::vector<Call> calls;};
void invoke(void* p,ScriptUpdateState48* s,const ScriptUpdateRequest32* r){
 auto& c=*static_cast<Context*>(p);check(r->service<=script_update_controller_stop,"Unknown service");
 auto normalized=*r;normalized.subject=canonical(r->subject);normalized.user_ref=canonical(r->user_ref);const auto snapshot=decode(*s);
 Call row{};std::memcpy(row.data(),&normalized,32);std::memcpy(row.data()+32,&snapshot,48);c.calls.push_back(row);
 if(r->service==c.params[1])switch(c.params[0]){
  case 1:s->controller=identities[5];break;
  case 2:s->collision_ms=300;break;
  case 3:s->paused=0;break;
  case 4:s->script_owner=identities[4];s->controller=identities[5];break;
 }
}
}
int main(int argc,char** argv){try{
 if(argc!=2)return 2;
 Reader reader(argv[1]);check(reader.get<unsigned>()==0x31555341,"ASU1 magic differs");const auto cases=reader.get<unsigned>();unsigned requests=0;
 for(unsigned i=0;i<cases;++i){
  const auto dispatch=reader.get<unsigned>();const auto initial=reader.get<ScriptUpdateState48>();Context c{reader.get<std::array<std::uint32_t,2>>(),{}};
  const auto expected=reader.get<ScriptUpdateState48>();const auto n=reader.get<unsigned>();std::vector<Call> calls(n);for(auto& r:calls)r=reader.get<Call>();
  auto s=encode(initial);ScriptUpdateServices16 services{&c,invoke};check(dh2_character_script_update(&s,dispatch,&services)==1,"Source wrapper rejected");const auto actual=decode(s);
  if(std::memcmp(&actual,&expected,48)||c.calls!=calls){std::cerr<<"Mismatch case "<<i<<'\n';return 3;}requests+=n;
 }
 const auto expiries=reader.get<unsigned>();
 for(unsigned i=0;i<expiries;++i){auto s=encode(reader.get<ScriptUpdateState48>());const auto expected=reader.get<ScriptUpdateState48>();check(dh2_character_script_pause_expired(&s)==1,"Source pause expiry rejected");const auto actual=decode(s);check(!std::memcmp(&actual,&expected,48),"Pause expiry differed");}
 check(reader.at==reader.bytes.size(),"Trailing gold");Context c{};ScriptUpdateState48 base{};base.script_owner=base.ai_owner=identities[2];base.controller=identities[3];ScriptUpdateServices16 services{&c,invoke};unsigned rejects=0;
 auto reject=[&](ScriptUpdateState48* s,unsigned dispatch,const ScriptUpdateServices16* cb){const auto before=s?*s:base;const auto count=c.calls.size();check(dh2_character_script_update(s,dispatch,cb)==-1,"Malformed accepted");if(s)check(!std::memcmp(s,&before,48),"Malformed mutated");check(c.calls.size()==count,"Malformed called service");++rejects;};
 reject(nullptr,0,&services);reject(&base,0,nullptr);auto no_callback=services;no_callback.invoke=nullptr;reject(&base,0,&no_callback);reject(&base,0,reinterpret_cast<const ScriptUpdateServices16*>(&base));reject(&base,2,&services);
 for(unsigned i=0;i<6;++i){auto s=base;switch(i){case 0:s.script_owner=0;break;case 1:s.ai_owner=0;break;case 2:s.controller=0;break;case 3:s.paused=256;break;case 4:s.reserved0=1;break;case 5:s.reserved1=1;break;}reject(&s,0,&services);}
 unsigned expiry_rejects=0;check(dh2_character_script_pause_expired(nullptr)==-1,"Null expiry accepted");++expiry_rejects;
 for(unsigned i=0;i<6;++i){auto s=base;switch(i){case 0:s.script_owner=0;break;case 1:s.ai_owner=0;break;case 2:s.controller=0;break;case 3:s.paused=256;break;case 4:s.reserved0=1;break;case 5:s.reserved1=1;break;}const auto before=s;check(dh2_character_script_pause_expired(&s)==-1&&!std::memcmp(&s,&before,48),"Malformed expiry affected state");++expiry_rejects;}
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<cases<<",\"ordered_service_requests\":"<<requests<<",\"pause_expiry_comparisons\":"<<expiries<<",\"atomic_rejection_checks\":"<<rejects<<",\"pause_expiry_atomic_rejections\":"<<expiry_rejects<<",\"mismatches\":0}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}}
