#include "character_timers.hpp"
#include <array>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
namespace {
void check(bool yes){if(!yes)throw std::runtime_error("timer mismatch");}
template<class T>T read(std::ifstream& in){T x{};in.read(reinterpret_cast<char*>(&x),sizeof x);check(bool(in));return x;}
struct Event {std::int32_t event;std::uint32_t id;std::uintptr_t ref;};static_assert(sizeof(Event)==16);
struct Fixture {TimerStore32* store;TimerServices32* services;std::vector<Event> events;int action=-1;std::vector<Timer32> grown;};
int grow(void* context,TimerStore32* store,unsigned requested){auto& f=*static_cast<Fixture*>(context);check(!store->update_depth);f.grown.assign(store->slots,store->slots+store->count);f.grown.resize(requested);store->slots=f.grown.data();store->capacity=requested;return 1;}
void expired(void* context,std::uintptr_t owner,std::int32_t event,Timer32* timer){
 check(reinterpret_cast<std::uintptr_t>(context)>0xffffffffu&&owner==0xabcdef0123456789ull);
 auto& f=*static_cast<Fixture*>(context);f.events.push_back({event,timer->id,timer->user_ref});int action=f.action;f.action=-1;
 if(action==0)check(dh2_character_timer_stop(f.store,timer->id)==1);
 if(action==1)check(dh2_character_timer_pause(f.store,timer->id,1)==1);
 if(action==2){timer->repeat=0;timer->duration_ms=30;timer->elapsed_ms=0;timer->active=1;}
 if(action==3)check(dh2_character_timer_start(f.store,10,0,0x2b,0x12345678,f.services)==1);
 if(action==4)check(dh2_character_timer_start(f.store,10,0,0x2b,0x12345678,f.services)==0);
}
void compare_events(std::ifstream& in,const std::vector<Event>& events){auto count=read<std::uint32_t>(in);check(events.size()==count);for(unsigned i=0;i<count;++i){auto expected=read<Event>(in);check(std::memcmp(&expected,&events[i],16)==0);}}
}
int main(int argc,char** argv){try{
 check(argc==2);std::ifstream in(argv[1],std::ios::binary);check(bool(in));check(read<std::uint32_t>(in)==0x31524d54);auto count=read<std::uint32_t>(in);
 std::array<Timer32,20> slots{};TimerStore32 store{slots.data(),1,20,0xabcdef0123456789ull,0,0};Fixture fixture{&store,nullptr,{},-1,{}};TimerServices32 services{&fixture,expired,nullptr,0};fixture.services=&services;
 for(unsigned i=0;i<count;++i){Timer32 timer;timer.repeat=read<std::int32_t>(in);timer.duration_ms=read<std::uint32_t>(in);timer.elapsed_ms=read<std::uint32_t>(in);timer.active=std::uint8_t(read<std::uint32_t>(in));timer.paused=std::uint8_t(read<std::uint32_t>(in));auto dt=read<std::uint32_t>(in);timer.event=read<std::int32_t>(in);timer.user_ref=0x12345678;auto expected=read<Timer32>(in);slots[0]=timer;fixture.events.clear();check(dh2_character_timers_update(&store,dt,0,&services)==1);check(std::memcmp(&slots[0],&expected,32)==0&&store.update_depth==0);compare_events(in,fixture.events);}
 auto reentry=read<std::uint32_t>(in);
 for(unsigned i=0;i<reentry;++i){fixture.action=int(read<std::uint32_t>(in));store.count=2;slots[0]={0,-1,10,0,1,0,0,0x2a,0x12345678};slots[1]={1,0,10,0,0,0,0,0x2a,0x12345678};std::array<Timer32,2> expected{read<Timer32>(in),read<Timer32>(in)};fixture.events.clear();check(dh2_character_timers_update(&store,35,0,&services)==1);check(std::memcmp(slots.data(),expected.data(),64)==0);compare_events(in,fixture.events);}
 check(in.peek()==std::ifstream::traits_type::eof());unsigned malformed=0;store.count=1;slots[0]={};TimerStore32 before=store;Timer32 timer_before=slots[0];
 auto reject=[&](TimerStore32 bad,const TimerServices32* callback){fixture.events.clear();check(dh2_character_timers_update(&bad,20,0,callback)==-1);check(std::memcmp(&slots[0],&timer_before,32)==0&&fixture.events.empty());++malformed;};
 auto bad=store;bad.reserved=1;reject(bad,&services);bad=store;bad.count=21;reject(bad,&services);bad=store;bad.slots=nullptr;reject(bad,&services);reject(store,nullptr);auto missing=services;missing.expired=nullptr;reject(store,&missing);missing=services;missing.reserved=1;reject(store,&missing);
 slots[0].reserved=1;timer_before=slots[0];reject(store,&services);slots[0]={};slots[0].id=7;timer_before=slots[0];reject(store,&services);slots[0]={};
 check(dh2_character_timer_pause(&store,0,2)==-1);++malformed;
 store.count=0;check(dh2_character_timer_start(&store,10,0,0x2a,0xfedcba9876543210ull,&services)==0);check(slots[0].user_ref==0xfedcba9876543210ull);
 slots[0].elapsed_ms=4;check(dh2_character_timer_pause(&store,0,1)==1);unsigned elapsed=0,duration=0;check(dh2_character_timer_time_left(&elapsed,&duration,&store,0)==1&&elapsed==4&&duration==10);
 check(dh2_character_timer_time_left(nullptr,&duration,&store,0)==-1);++malformed;
 fixture.events.clear();check(dh2_character_timers_update(&store,100,1,&services)==1&&fixture.events.empty()&&slots[0].elapsed_ms==4);
 check(dh2_character_timer_stop(&store,0)==1);check(dh2_character_timer_start(&store,20,2,-1,77,&services)==0);store.capacity=1;before=store;timer_before=slots[0];check(dh2_character_timer_start(&store,10,0,0x2a,0,&services)==-2&&std::memcmp(&slots[0],&timer_before,32)==0);
 store.update_depth=1;check(dh2_character_timer_start(&store,10,0,0x2a,0,&services)==-3);store.update_depth=0;
 check(dh2_character_timers_stop_all(&store)==1&&!slots[0].active);check(dh2_character_timer_stop(&store,99)==0&&dh2_character_timer_pause(&store,99,1)==0);check(dh2_character_timer_time_left(&elapsed,&duration,&store,0)==0);
 // One-shot deactivation permits Start to reuse that same slot synchronously.
 slots[0]={0,0,10,0,1,0,0,0x2a,0};fixture.action=4;fixture.events.clear();check(dh2_character_timers_update(&store,10,0,&services)==1&&fixture.events.size()==1&&slots[0].active&&slots[0].elapsed_ms==0&&slots[0].event==0x2b);
 services.grow=grow;check(dh2_character_timer_start(&store,22,0,0x2a,0,&services)==1&&store.count==2&&store.capacity==2&&store.slots!=slots.data()&&store.slots[0].event==0x2b);
 std::printf("{\"original_reference_cases\":%u,\"source_reentry_cases\":%u,\"malformed_no_mutation_cases\":%u,\"owner_and_user_ref_above4GiB\":true,\"expiry_growth_rejected\":true,\"one_shot_start_reentry_passed\":true,\"caller_growth_outside_update_passed\":true,\"mismatches\":0}\n",count,reentry,malformed);return 0;
}catch(const std::exception& e){std::fprintf(stderr,"timer audit: %s\n",e.what());return 1;}}
