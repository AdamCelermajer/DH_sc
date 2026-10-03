#include "../character_animation_ai.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
namespace {
void check(bool b,const char* message){if(!b)throw std::runtime_error(message);}
struct Reader {
 std::vector<unsigned char> raw;std::size_t at=0;
 explicit Reader(const char* file){std::ifstream f(file,std::ios::binary);check(bool(f),"Gold missing");raw={std::istreambuf_iterator<char>(f),{}};}
 template<class T>T get(){check(at+sizeof(T)<=raw.size(),"Gold truncated");T r;std::memcpy(&r,raw.data()+at,sizeof(T));at+=sizeof(T);return r;}
};
using Call=std::array<unsigned char,128>;
constexpr std::uintptr_t identity[5]={0,0x100001001ull,0x200002002ull,0x300003003ull,0x400004004ull};
std::uintptr_t canonical(std::uintptr_t p){for(unsigned i=0;i<5;++i)if(identity[i]==p)return i;return p;}
AnimationAIState96 encode(AnimationAIState96 s){s.owner=identity[s.owner];s.controller=identity[s.controller];s.target=identity[s.target];s.look_target=identity[s.look_target];return s;}
AnimationAIState96 decode(AnimationAIState96 s){s.owner=canonical(s.owner);s.controller=canonical(s.controller);s.target=canonical(s.target);s.look_target=canonical(s.look_target);return s;}
struct Context {std::array<std::uint32_t,16> params;std::vector<Call> calls;};
void invoke(void* raw,AnimationAIState96* s,const AnimationAIRequest32* request,AnimationAIResponse16* response){
 auto& c=*static_cast<Context*>(raw);check(!request->reserved0&&!request->reserved1,"Reserved request bytes");
 auto normalized=*request;normalized.subject=canonical(request->subject);normalized.payload=canonical(request->payload);
 const auto snapshot=decode(*s);Call call{};std::memcpy(call.data(),&normalized,32);std::memcpy(call.data()+32,&snapshot,96);c.calls.push_back(call);
 if(request->service==c.params[8])switch(c.params[7]){
  case 1:s->animation_depth=7;s->attack_index=-7;break;
  case 2:s->owner_position[0]=9;s->owner_position[1]=-8;s->owner_position[2]=7;break;
  case 3:case 4:s->target=0;s->owner_byte14a8=c.params[7]==4?8:0;break;
  case 5:s->target=identity[4];break;
  case 6:s->attack_index=987;break;
  case 7:s->attack_continued=1;break;
 }
 const unsigned indices[16]={0,1,2,3,0,6,0,0,0,4,5,0,0,0,0,0};
 check(request->service<16,"Unknown request");
 response->word=(request->service<4||request->service==5||request->service==9||request->service==10)?c.params[indices[request->service]]:0;
 std::memcpy(response->position,c.params.data()+9,12);
}
}
int main(int argc,char** argv){try{
 if(argc!=2)return 2;
 Reader r(argv[1]);check(r.get<unsigned>()==0x31494141,"AAI1 magic differs");const auto count=r.get<unsigned>();unsigned requests=0;
 for(unsigned i=0;i<count;++i){
  const auto op=r.get<unsigned>();auto input=r.get<AnimationAIState96>();Context c{r.get<std::array<std::uint32_t,16>>(),{}};const auto expected=r.get<AnimationAIState96>();const auto n=r.get<unsigned>();std::vector<Call> calls(n);for(auto& row:calls)row=r.get<Call>();
  auto state=encode(input);const AnimationAIServices16 services{&c,invoke};check(dh2_character_animation_ai(&state,op,&services)==1,"Consumer rejected original fixture");const auto after=decode(state);
  if(std::memcmp(&after,&expected,96)||c.calls!=calls){std::cerr<<"Consumer mismatch case "<<i<<" op "<<op<<'\n';return 3;}requests+=n;
 }
 const auto scalars=r.get<unsigned>();
 for(unsigned i=0;i<scalars;++i){auto row=r.get<std::array<unsigned,5>>();const auto actual=row[0]?dh2_character_animation_table_id(std::int32_t(row[1]),std::int32_t(row[2])):dh2_character_animation_has_combo(std::int32_t(row[1]),std::int32_t(row[2]),std::int32_t(row[3]));check(unsigned(actual)==row[4],"Original scalar mismatch");}
 check(r.at==r.raw.size(),"Gold trailing bytes");
 Context c{};AnimationAIServices16 services{&c,invoke};AnimationAIState96 base{};base.owner=identity[1];base.controller=identity[2];unsigned rejects=0;
 auto reject=[&](AnimationAIState96* s,unsigned op,const AnimationAIServices16* callback){auto before=s?*s:base;auto n=c.calls.size();check(dh2_character_animation_ai(s,op,callback)==-1,"Malformed input accepted");if(s)check(!std::memcmp(&before,s,96),"Malformed input mutated state");check(n==c.calls.size(),"Malformed input invoked service");++rejects;};
 reject(nullptr,0,&services);reject(&base,0,nullptr);auto invalid_service=services;invalid_service.invoke=nullptr;reject(&base,0,&invalid_service);reject(&base,7,&services);
 reject(&base,0,reinterpret_cast<const AnimationAIServices16*>(&base));
 for(unsigned field=0;field<12;++field){auto state=base;
  switch(field){case 0:state.owner=0;break;case 1:state.controller=0;break;case 2:state.reserved0=1;break;case 3:state.reserved1=1;break;case 4:state.seeking=256;break;case 5:state.target_sticky=256;break;case 6:state.attack_continued=256;break;case 7:state.attack_last=256;break;case 8:state.attack_finisher=256;break;case 9:state.skill_started=256;break;case 10:state.skill_stop_requested=256;break;case 11:state.owner_byte14a8=128;break;}
  reject(&state,0,&services);
 }
 std::cout<<"{\"validation\":\"PASS\",\"consumer_comparisons\":"<<count<<",\"ordered_service_requests\":"<<requests<<",\"scalar_comparisons\":"<<scalars<<",\"atomic_rejection_checks\":"<<rejects<<",\"mismatches\":0}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}}
