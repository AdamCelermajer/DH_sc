#include "../character_script_collision.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
namespace {
void check(bool v,const char* message){if(!v)throw std::runtime_error(message);}
struct Reader {
 std::vector<unsigned char> data;std::size_t at=0;
 explicit Reader(const char* path){std::ifstream f(path,std::ios::binary);check(bool(f),"Missing gold");data={std::istreambuf_iterator<char>(f),{}};}
 template<class T>T get(){check(at+sizeof(T)<=data.size(),"Truncated gold");T out;std::memcpy(&out,data.data()+at,sizeof(T));at+=sizeof(T);return out;}
};
using Event=std::array<unsigned char,128>;
constexpr std::uintptr_t key=0x100000000ull,collider=key+0x200c000,other=key+0x2006000;
struct Context {std::array<unsigned,8> params{};std::array<unsigned,3> indices{};std::vector<Event> events;unsigned depth=0;bool mutated=false;};
unsigned invoke(void* opaque,ScriptCollisionState72* s,ScriptCollisionObject16* o,const ScriptCollisionRequest32* r){
 auto& c=*static_cast<Context*>(opaque);check(r->service<=4,"Unknown service");unsigned response=0;
 if(r->service<3){const auto index=c.indices[r->service]++;response=c.params[1+r->service*2+(index>0?1:0)];}
 Event event{};std::memcpy(event.data(),r,32);std::memcpy(event.data()+32,s,72);std::memcpy(event.data()+104,o,16);std::memcpy(event.data()+120,&c.depth,4);std::memcpy(event.data()+124,&response,4);c.events.push_back(event);
 const auto m=c.params[7],service=r->service;
 if(!c.depth&&!c.mutated&&(((m==1||m==4||m==7)&&service==0)||(m==2&&service==1)||(m==3&&service==2)||((m==5||m==6||m==8)&&(service==3||service==4)))){
  c.mutated=true;
  switch(m){
   case 1:s->current_target=collider;break;
   case 2:s->preferred_target=s->current_target;break;
   case 3:s->collision_ms=0xfffffff0;s->application_frame+=1;s->dt_ms=32;break;
   case 4:s->owner=other;s->ai_owner=other+0x3c8;s->controller=other+0x4fc;s->paused=0;o->type=21;break;
   case 5:s->current_target=collider;break;
   case 6:s->paused=1;break;
   case 7:case 8:{++c.depth;const ScriptCollisionServices16 services{&c,invoke};check(dh2_character_script_collision(s,o,c.params[0],&services)==1,"Nested rejected");--c.depth;break;}
  }
 }
 return response;
}
}
int main(int argc,char** argv){try{
 if(argc!=2)return 2;
 Reader reader(argv[1]);check(reader.get<unsigned>()==0x31434353,"SCC1 magic differs");const auto count=reader.get<unsigned>();unsigned callbacks=0,nested=0;
 for(unsigned i=0;i<count;++i){auto state=reader.get<ScriptCollisionState72>();auto object=reader.get<ScriptCollisionObject16>();Context c;c.params=reader.get<std::array<unsigned,8>>();const auto final=reader.get<ScriptCollisionState72>();const auto final_object=reader.get<ScriptCollisionObject16>();const auto n=reader.get<unsigned>();std::vector<Event> events(n);bool has_nested=false;for(auto& event:events){event=reader.get<Event>();unsigned depth;std::memcpy(&depth,event.data()+120,4);has_nested|=depth!=0;}
  const ScriptCollisionServices16 services{&c,invoke};check(dh2_character_script_collision(&state,&object,c.params[0],&services)==1,"Fixture rejected");if(std::memcmp(&state,&final,72)||std::memcmp(&object,&final_object,16)||c.events!=events){std::cerr<<"Mismatch case "<<i<<'\n';return 3;}callbacks+=n;nested+=has_nested;
 }
 check(reader.at==reader.data.size(),"Trailing gold");Context c;ScriptCollisionState72 state{key+0x2001000,key+0x2002000,key+0x20023c8,key+0x20024fc,0,0,0,0,0,4,1,16};ScriptCollisionObject16 object{collider,2,0};ScriptCollisionServices16 services{&c,invoke};unsigned guards=0;
 auto reject=[&](ScriptCollisionState72* s,ScriptCollisionObject16* o,const ScriptCollisionServices16* cb){const auto before=state;const auto before_o=object;const auto calls=c.events.size();check(dh2_character_script_collision(s,o,1,cb)==-1,"Malformed accepted");check(!std::memcmp(&state,&before,72)&&!std::memcmp(&object,&before_o,16)&&c.events.size()==calls,"Malformed mutation");++guards;};
 reject(nullptr,&object,&services);reject(&state,nullptr,&services);reject(&state,&object,nullptr);auto empty=services;empty.invoke=nullptr;reject(&state,&object,&empty);reject(&state,reinterpret_cast<ScriptCollisionObject16*>(&state),&services);reject(&state,&object,reinterpret_cast<ScriptCollisionServices16*>(&state));reject(&state,&object,reinterpret_cast<ScriptCollisionServices16*>(&object));
 for(unsigned i=0;i<7;++i){const auto saved=state;const auto saved_o=object;switch(i){case 0:state.script_owner=0;break;case 1:state.owner=0;break;case 2:state.ai_owner=0;break;case 3:state.controller=0;break;case 4:state.paused=256;break;case 5:object.identity=0;break;case 6:object.reserved=1;break;}reject(&state,&object,&services);state=saved;object=saved_o;}
 // Actual counter lifecycle: same frame cannot accumulate twice; next frame can.
 c.params={1,0,0,0,0,0,0,0};state.collision_ms=0xfffffff0;state.dt_ms=32;check(dh2_character_script_collision(&state,&object,1,&services)==1&&state.collision_ms==16&&state.last_collision_frame==1,"Wrap failed");check(dh2_character_script_collision(&state,&object,1,&services)==1&&state.collision_ms==16,"Same frame accumulated");++state.application_frame;state.dt_ms=200;check(dh2_character_script_collision(&state,&object,1,&services)==1&&state.collision_ms==216,"Next frame failed");
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<count<<",\"ordered_callbacks\":"<<callbacks<<",\"nested_callback_cases\":"<<nested<<",\"atomic_rejection_checks\":"<<guards<<",\"counter_lifecycle_checks\":3,\"mismatches\":0}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}}
