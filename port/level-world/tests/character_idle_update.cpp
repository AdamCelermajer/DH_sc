#include "../character_idle_update.hpp"
#include "../character_native_fsm.hpp"
#include "../character_target_providers.hpp"
#include <array>
#include <cstring>
#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
namespace {
unsigned checks=0;
void need(bool v){++checks;if(!v)throw std::runtime_error("Idle check "+std::to_string(checks));}
std::uint32_t word(std::istream&f){std::uint32_t v=0;f.read(reinterpret_cast<char*>(&v),4);need(bool(f));return v;}
float number(std::uint32_t v){float result;std::memcpy(&result,&v,4);return result;}
std::uint32_t bits(float v){std::uint32_t result;std::memcpy(&result,&v,4);return result;}
using Event=std::array<std::uint32_t,6>;
struct Replay {
 std::array<IdleCharacter40,6> actors{};
 std::array<unsigned char,6> controllers{};unsigned char manager{};
 std::array<std::uint32_t,54> x{};
 std::vector<Event> trace;std::vector<std::vector<Event>> nested;bool reentered=false;int fail=-1;
 unsigned identity(std::uintptr_t p){if(!p)return 0;for(unsigned i=0;i<6;++i){if(p==reinterpret_cast<std::uintptr_t>(&actors[i]))return 1+i;if(p==reinterpret_cast<std::uintptr_t>(&controllers[i]))return 11+i;}if(p==reinterpret_cast<std::uintptr_t>(&manager))return 20;throw std::runtime_error("identity");}
 void fixture(){trace.clear();nested.clear();reentered=false;for(unsigned i=0;i<6;++i){auto o=18+6*i;actors[i]={reinterpret_cast<std::uintptr_t>(&actors[i]),reinterpret_cast<std::uintptr_t>(&controllers[x[o]]),{number(x[o+1]),number(x[o+2]),number(x[o+3])},x[o+4],x[o+5],0};}}
 void mutate(int index){if(index!=std::int32_t(x[16]))return;for(unsigned i=0;i<6;++i){auto&a=actors[i];a.position[0]=.25f+.5f*i;a.position[1]=-.125f*i;a.position[2]=.125f*i;a.state_time_ms=x[2]+1;a.heading_active=0;a.controller=reinterpret_cast<std::uintptr_t>(&controllers[(i+1)%6]);}}
 static int service(void*p,IdleCharacter40*c,const IdleUpdateRequest32*r,IdleUpdateResponse16*out){auto&t=*static_cast<Replay*>(p);need(c==&t.actors[0]&&!r->reserved);const int index=int(t.trace.size());t.trace.push_back({r->service,r->argument,t.identity(r->subject),bits(r->point[0]),bits(r->point[1]),bits(r->point[2])});if(index==t.fail)return 1;t.mutate(index);
  if(index==std::int32_t(t.x[17])&&!t.reentered){t.reentered=true;auto outer=std::move(t.trace);t.trace.clear();t.actors[0].heading_active=1;auto services=t.services();need(dh2_character_idle_update(&t.actors[0],&services)==1);t.nested.push_back(std::move(t.trace));t.trace=std::move(outer);}
  switch(r->service){case idle_is_player:out->word=t.x[0];break;case idle_online_disabled:out->word=t.x[1];break;case idle_delay:out->word=t.x[2];break;case idle_player_count:out->word=t.x[3];out->identity=reinterpret_cast<std::uintptr_t>(&t.manager);break;case idle_player_character:{need(r->argument<5);auto id=t.x[5+r->argument];out->identity=id?reinterpret_cast<std::uintptr_t>(&t.actors[id-1]):0;break;}case idle_get_state:out->word=t.x[10+t.identity(r->subject)-1];break;case idle_distance:out->word=t.x[4];break;case idle_raise_event:case idle_move_to:break;default:throw std::runtime_error("service");}return 0;
 }
 IdleUpdateServices16 services(){return {this,&service};}
 std::array<std::uint32_t,36> snapshot(){std::array<std::uint32_t,36> out{};for(unsigned i=0;i<6;++i){auto&a=actors[i];auto p=out.data()+6*i;p[0]=identity(a.controller)-11;for(unsigned j=0;j<3;++j)p[j+1]=bits(a.position[j]);p[4]=a.state_time_ms;p[5]=a.heading_active;}return out;}
};
struct Composition {
 State state{};Facts facts{};NativeFsm24 view{&state,0xabcdef0123456789ull,1,0};IdleCharacter40 character{view.character,0,{1,2,3},0,0,0};
 CombatProperties896 props{};std::int32_t types[9]{0,1,2,3,4,5,0,0,0};dh2::target_providers::Types16 table{types,9,0};dh2::target_providers::Character32 receiver{view.character,&props,"Crypt_Skeleton",0,0,0,1,0};std::uint32_t dt=0,queries=0,idle_calls=0;
 static int idle(void*p,IdleCharacter40*,const IdleUpdateRequest32*r,IdleUpdateResponse16*out){auto&t=*static_cast<Composition*>(p);need(r->service==idle_is_player);std::int32_t result=-1;need(!dh2::target_providers::dh2_character_target_query(&result,dh2::target_providers::is_player,&t.receiver,nullptr,&t.table,nullptr));out->word=std::uint32_t(result);++t.queries;return 0;}
 static void state_service(void*p,State*s,const Request*r){auto&t=*static_cast<Composition*>(p);need(s==&t.state&&r->service==idle_common_update);t.character.heading_active=s->heading_active;t.character.state_time_ms=s->elapsed_ms;const IdleUpdateServices16 services{p,&idle};need(dh2_character_idle_update(&t.character,&services)==1);++t.idle_calls;}
 static int outer(void*p,NativeFsm24*f,const NativeFsmRequest32*r,std::uint32_t*out){auto&t=*static_cast<Composition*>(p);if(r->service==fsm_engine_dt)*out=t.dt;else if(r->service==fsm_current_update){const Services services{p,&state_service};need(dh2_character_native_fsm_bounded_tail(f,&t.facts,&services)==1);}else need(r->service==fsm_profile_begin||r->service==fsm_profile_end);return 0;}
};
}
int main(int argc,char**argv){try{
 need(argc==2);std::ifstream file(argv[1],std::ios::binary);char magic[4]{};file.read(magic,4);need(!std::memcmp(magic,"IDU1",4));auto cases=word(file);unsigned requests=0,reentries=0;
 for(unsigned i=0;i<cases;++i){Replay t;for(auto&v:t.x)v=word(file);auto count=word(file),nestedcount=word(file);std::array<std::uint32_t,36> final{};for(auto&v:final)v=word(file);std::vector<Event> expected(count);for(auto&e:expected)for(auto&v:e)v=word(file);std::vector<std::vector<Event>> nested(nestedcount);for(auto&list:nested){list.resize(word(file));for(auto&e:list)for(auto&v:e)v=word(file);}t.fixture();auto services=t.services();need(dh2_character_idle_update(&t.actors[0],&services)==1);need(t.trace==expected&&t.nested==nested&&t.snapshot()==final);requests+=count;for(auto&v:nested)requests+=v.size();reentries+=nestedcount;
 }need(file.peek()==EOF);
 Replay t;t.x={1,0,0,5,25,2,3,4,5,6,3,3,3,3,3,3,0xffffffffu,0xffffffffu};for(unsigned i=0;i<6;++i){auto o=18+6*i;t.x[o]=i;t.x[o+1]=bits(i*.25f);t.x[o+4]=100;}t.fixture();auto services=t.services();need(dh2_character_idle_update(&t.actors[0],&services)==1);auto baseline=t.trace;unsigned prefixes=0;
 for(unsigned i=0;i<baseline.size();++i){t.fixture();t.fail=int(i);need(dh2_character_idle_update(&t.actors[0],&services)==-2);need(t.trace==std::vector<Event>(baseline.begin(),baseline.begin()+i+1));++prefixes;}t.fail=-1;
 unsigned guards=0;t.fixture();auto before=t.actors;need(dh2_character_idle_update(nullptr,&services)==-1);++guards;need(dh2_character_idle_update(&t.actors[0],nullptr)==-1);++guards;
 {auto bad=services;bad.invoke=nullptr;need(dh2_character_idle_update(&t.actors[0],&bad)==-1);++guards;}
 for(unsigned op=0;op<3;++op){t.fixture();if(op==0)t.actors[0].reserved=1;if(op==1)t.actors[0].heading_active=256;if(op==2)t.actors[0].identity=0;auto copy=t.actors;need(dh2_character_idle_update(&t.actors[0],&services)==-1&&t.trace.empty()&&!std::memcmp(copy.data(),t.actors.data(),sizeof copy));++guards;}t.fixture();need(!std::memcmp(before.data(),t.actors.data(),sizeof before));
 unsigned compositions=0;for(auto dt:{0u,1u,17u,0xffffffffu})for(auto elapsed:{0u,100u,0xffffffffu}){Composition c;c.props.words[1]=4;c.state.current=3;c.state.flags=0x2380;c.state.elapsed_ms=elapsed;c.dt=dt;const NativeFsmServices16 outer{&c,&Composition::outer};need(dh2_character_native_fsm_update(&c.view,&outer)==1&&c.state.elapsed_ms==elapsed+dt&&c.character.state_time_ms==elapsed+dt&&c.queries==1&&c.idle_calls==1&&c.state.current==3&&c.state.flags==0x2380);++compositions;}
 Dl_info module{},world{},provider{};need(dladdr(reinterpret_cast<void*>(&dh2_character_idle_update),&module));need(dladdr(reinterpret_cast<void*>(&dh2_character_state_update),&world));need(dladdr(reinterpret_cast<void*>(&dh2::target_providers::dh2_character_target_query),&provider));
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"original_cases\":"<<cases<<",\"ordered_requests\":"<<requests<<",\"synchronous_reentries\":"<<reentries<<",\"failure_prefixes\":"<<prefixes<<",\"atomic_guards\":"<<guards<<",\"linked_FSM_State_IsPlayer_compositions\":"<<compositions<<",\"module_library\":\""<<module.dli_fname<<"\",\"world_library\":\""<<world.dli_fname<<"\",\"provider_library\":\""<<provider.dli_fname<<"\"}\n";
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
