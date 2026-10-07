#include "../character_spawn_select.hpp"
#include "../character_timers.hpp"
#include "../character_state_owner.hpp"
#include "../character_spawn_permission.hpp"
#include <array>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
static void check(bool v){if(!v)throw std::runtime_error("spawn selection audit failed");}
template<class T>T read(std::ifstream& f){T v{};f.read(reinterpret_cast<char*>(&v),sizeof v);check(bool(f));return v;}
struct Replay {
 State state{};NativeFsm24 fsm{&state,0xabcdef0123456789ull,1,0};dh2::data::CombatRandom rng{};NativeSpawn24 view{&fsm,&rng,0,0};SpawnSelectServices16 svc{this,invoke};std::vector<SpawnSelectRequest32> calls;unsigned response=0;bool fail=false;
 static int invoke(void* p,NativeSpawn24*,const SpawnSelectRequest32* r,unsigned* response){auto& f=*static_cast<Replay*>(p);f.calls.push_back(*r);*response=f.response;return f.fail?1:0;}
 void reset(const std::array<unsigned,8>& p){calls.clear();fail=false;response=p[6];view={&fsm,&rng,static_cast<int>(p[2]),static_cast<int>(p[3])};rng={p[4],p[5]};state={};state.current=17;fsm={&state,0xabcdef0123456789ull,1,0};}
};
struct OwnedFixture {
 CharacterStateOwner owner{0xc123456789abcdefull};dh2::data::CombatRandom rng{};NativeSpawn24 view{&owner.native_fsm(),&rng,0,0};std::array<Timer32,4> slots{};TimerStore32 timers{slots.data(),0,4,owner.native_fsm().character,0,0};TimerServices32 timer_services{this,expired,nullptr,0};StateOwnerServices16 methods{this,method};SpawnSelectServices16 services{this,invoke};unsigned behavior_fixtures=0,expiries=0;bool nested=false;std::vector<SpawnSelectRequest32> calls;
 // Remaining virtual behavior implementations are explicitly fixtures. The
 // actual owned registry/transition/event routing and TimerStore execute.
 static int method(void* p,StateOwnerMachine40*,const StateOwnerRequest48* r,StateOwnerResponse8*){auto& f=*static_cast<OwnedFixture*>(p);if(r->operation==state_owner_focus||r->operation==state_owner_blur||r->operation==state_owner_event)++f.behavior_fixtures;return 0;}
 static void expired(void* p,std::uintptr_t owner,int event,Timer32* timer){auto& f=*static_cast<OwnedFixture*>(p);check(owner==f.owner.native_fsm().character&&event==0x2d&&timer->user_ref==0);++f.expiries;check(f.owner.event(event,reinterpret_cast<std::uintptr_t>(timer),f.methods)>=0);}
 static int invoke(void* p,NativeSpawn24*,const SpawnSelectRequest32* r,unsigned* out){auto& f=*static_cast<OwnedFixture*>(p);check(r->character==f.owner.native_fsm().character&&!r->payload);f.calls.push_back(*r);
  if(r->service==spawn_select_state){check(r->argument0==1&&r->argument1==~0u&&!r->argument2);return f.owner.transition(1,-1,0,f.methods)==1?0:1;}
  check(r->service==spawn_select_timer&&!r->argument1&&r->argument2==0x2d);
  if(f.nested){f.nested=false;check(dh2_character_spawn_select(&f.view,0,~0u,&f.services)==1);}
  const auto id=dh2_character_timer_start(&f.timers,r->argument0,0,0x2d,0,&f.timer_services);if(id<0)return 1;*out=static_cast<unsigned>(id);return 0;
 }
 void init(){check(owner.initialize_level(17,methods)==1&&owner.state().current==17);}
};
int main(int argc,char** argv){try{
 check(argc==3);std::ifstream f(argv[1],std::ios::binary);check(read<unsigned>(f)==0x31535053);const auto count=read<unsigned>(f);Replay r;unsigned zero_timers=0;
 for(unsigned i=0;i<count;++i){const auto p=read<std::array<unsigned,8>>(f);const auto expected=read<std::array<unsigned,4>>(f);const auto request=read<SpawnSelectRequest32>(f);r.reset(p);check(dh2_character_spawn_select(&r.view,p[0],p[1],&r.svc)==1);const std::array<unsigned,4> got{static_cast<unsigned>(r.view.minimum_ms),static_cast<unsigned>(r.view.maximum_ms),r.rng.seed,r.rng.calls};check(got==expected&&r.calls.size()==1&&!std::memcmp(&r.calls[0],&request,32));zero_timers+=request.service==spawn_select_timer&&!request.argument0;}
 check(f.peek()==std::ifstream::traits_type::eof());const std::array<unsigned,8> p{1,1,~0u,1,1,0,7,0};unsigned guards=0;
 auto reject=[&](NativeSpawn24 v,const SpawnSelectServices16* s){r.reset(p);const auto rng=r.rng;check(dh2_character_spawn_select(&v,1,0,s)==-1&&r.calls.empty()&&r.rng.seed==rng.seed&&r.rng.calls==rng.calls);++guards;};
 auto v=r.view;reject(v,nullptr);r.reset(p);check(dh2_character_spawn_select(nullptr,1,0,&r.svc)==-1&&r.calls.empty());++guards;
 // The actual malformed checks use persistent structs; no output aliases or
 // borrowed temporary service contexts are passed.
 r.reset(p);v=r.view;v.fsm=nullptr;reject(v,&r.svc);v=r.view;v.random=nullptr;reject(v,&r.svc);
 r.reset(p);auto services=r.svc;services.invoke=nullptr;reject(r.view,&services);
 r.reset(p);r.fsm.state=nullptr;check(dh2_character_spawn_select(&r.view,1,0,&r.svc)==-1);++guards;
 r.reset(p);r.fsm.character=0;check(dh2_character_spawn_select(&r.view,1,0,&r.svc)==-1);++guards;
 r.reset(p);r.fsm.reserved=1;check(dh2_character_spawn_select(&r.view,1,0,&r.svc)==-1);++guards;
 r.reset(p);r.fsm.current_present=2;check(dh2_character_spawn_select(&r.view,1,0,&r.svc)==-1);++guards;
 r.reset(p);r.fail=true;check(dh2_character_spawn_select(&r.view,1,0,&r.svc)==-2&&r.view.minimum_ms==0&&r.view.maximum_ms==1&&r.rng.calls==1&&r.calls.size()==1);++guards;
 unsigned methods=0;
 {OwnedFixture x;x.init();x.view.minimum_ms=x.view.maximum_ms=5;check(dh2_character_spawn_select(&x.view,1,0,&x.services)==1&&x.owner.state().current==17&&x.timers.count==1);check(dh2_character_timers_update(&x.timers,4,0,&x.timer_services)==1&&x.expiries==0);check(dh2_character_timers_update(&x.timers,1,0,&x.timer_services)==1&&x.expiries==1&&x.owner.state().current==1&&!x.slots[0].active);methods+=x.behavior_fixtures;}
 {OwnedFixture x;x.init();x.view.minimum_ms=0;x.view.maximum_ms=1;check(dh2_character_spawn_select(&x.view,1,0,&x.services)==1&&x.timers.count==1&&x.slots[0].duration_ms==0);check(dh2_character_timers_update(&x.timers,1000,0,&x.timer_services)==1&&x.expiries==0&&x.slots[0].active&&x.owner.state().current==17);methods+=x.behavior_fixtures;}
 {OwnedFixture x;x.init();check(dh2_character_spawn_select(&x.view,0,1,&x.services)==1&&x.owner.state().current==1&&!x.timers.count);methods+=x.behavior_fixtures;}
 {OwnedFixture x;x.init();x.view.minimum_ms=x.view.maximum_ms=5;x.nested=true;check(dh2_character_spawn_select(&x.view,1,0,&x.services)==1&&x.calls.size()==2&&x.calls[0].service==spawn_select_timer&&x.calls[1].service==spawn_select_state&&x.owner.state().current==1);check(dh2_character_timers_update(&x.timers,5,0,&x.timer_services)==1&&x.expiries==1&&x.owner.state().current==1);methods+=x.behavior_fixtures;}
 std::ifstream permission_file(argv[2],std::ios::binary);check(read<unsigned>(permission_file)==0x31505053);const auto permissions=read<unsigned>(permission_file);unsigned permission_guards=0;
 for(unsigned i=0;i<permissions;++i){const auto row=read<std::array<unsigned,3>>(permission_file);const SpawnPermission16 v{row[0]?0xc123456789abcdefull:0,row[1],0};unsigned out=~0u;check(dh2_character_pre_spawn_permission(&out,&v)==1&&out==row[2]);}check(permission_file.peek()==std::ifstream::traits_type::eof());
 unsigned out=0x12345678;SpawnPermission16 permission{};check(dh2_character_pre_spawn_permission(&out,nullptr)==-1&&out==0x12345678);++permission_guards;check(dh2_character_pre_spawn_permission(nullptr,&permission)==-1);++permission_guards;permission.reserved=1;check(dh2_character_pre_spawn_permission(&out,&permission)==-1&&out==0x12345678);++permission_guards;permission={0,256,0};check(dh2_character_pre_spawn_permission(&out,&permission)==-1&&out==0x12345678);++permission_guards;permission={0,1,0};check(dh2_character_pre_spawn_permission(&permission.auto_spawn,&permission)==-1&&permission.auto_spawn==1);++permission_guards;
 std::printf("{\"validation\":\"PASS\",\"original_cases\":%u,\"ordered_requests\":%u,\"zero_duration_timer_gold_cases\":%u,\"guards_and_failure_prefix_checks\":%u,\"genuine_TimerStore_owned_StateInfo_compositions\":4,\"remaining_method_fixture_deliveries\":%u,\"synchronous_nested_spawn_request_checks\":1,\"permission_original_cases\":%u,\"permission_atomic_guards\":%u,\"mismatches\":0}\n",count,count,zero_timers,guards,methods,permissions,permission_guards);
 return 0;
 }catch(const std::exception& e){std::fprintf(stderr,"%s\n",e.what());return 1;}}
