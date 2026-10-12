#include "character_native_effects.hpp"
#include "character_timers.hpp"
#include "native_body.hpp"
#include <Box2D.h>
#include <array>
#include <cstring>
#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
namespace {
unsigned checks=0;void check(bool v){++checks;if(!v)throw std::runtime_error("native effects check "+std::to_string(checks));}
std::uint32_t word(std::istream& f){std::uint32_t v=0;f.read(reinterpret_cast<char*>(&v),4);check(bool(f));return v;}
using Event=std::array<std::uint32_t,7>;
struct Replay {
 State state{};NativeFsm24 view{&state,0xabcdef0123456789ull,1,0};std::array<std::int32_t,20> stun{},scare{};NativeEffects32 effects{&view,stun.data(),scare.data(),20,0};std::array<std::uint32_t,25> h{};std::vector<Event> trace;int fail=-1;bool actual=false;unsigned starts=0,pins=0;
 std::array<Timer32,4> slots{};TimerStore32 store{slots.data(),0,4,view.character,0,0};TimerServices32 timer_services{};dh2::physical::NativeBody* native=nullptr;
 Replay(){for(unsigned i=0;i<20;++i){stun[i]=1000+10*i;scare[i]=2000+10*i;}}
 static int service(void* p,NativeEffects32* e,const NativeEffectRequest32* r,std::uint32_t* out){auto& t=*static_cast<Replay*>(p);check(e==&t.effects&&r->character==t.view.character);Event event{r->service,0,0,0,0,0,0};*out=0;
  switch(r->service){
  case effect_ai_flags:event[5]=1;*out=t.h[2]*4;break;
  case effect_animation_index:event[1]=t.h[3];*out=t.h[3];break;
  case effect_start_timer:event={r->service,r->argument0,r->argument1,r->argument2,0,t.state.attack_gate,1};*out=0xffffffffu;break;
  case effect_stance_bits:event[5]=std::uint32_t(t.state.animation_override);*out=t.h[6];break;
  case effect_anim_stance:event[5]=1;*out=t.h[10];break;
  case effect_force_state:case effect_state_event:event={r->service,r->argument0,r->argument1,0,std::uint32_t(bool(r->payload)),t.h[1]?t.state.idle_suppressed:std::uint32_t(t.state.animation_override),t.h[1]?0:t.state.attack_gate};check(r->payload==(t.h[24]?0xa0123456fedcba98ull:0));break;
  case effect_stop_loop:check(!r->argument0&&!r->argument1&&!r->argument2&&!r->payload);event[5]=1;break;
  case effect_pin:check(!r->argument0&&!r->argument1&&!r->argument2&&!r->payload);event[5]=t.state.controller_locked;event[6]=1;break;
  case effect_heading_object:check(!r->argument0&&!r->argument1&&!r->argument2&&!r->payload);event[5]=1;break;
  default:check(false);
  }
  t.trace.push_back(event);if(int(r->service)==t.fail)return 1;
  if(t.actual){
   if(r->service==effect_start_timer){auto id=dh2_character_timer_start(&t.store,r->argument0,std::int32_t(r->argument1),std::int32_t(r->argument2),r->payload,&t.timer_services);check(id>=0);*out=std::uint32_t(id);++t.starts;}
   else if(r->service==effect_pin){check(t.native&&dh2_native_body_pin(t.native)==0);++t.pins;}
   else if(r->service==effect_force_state||r->service==effect_state_event)return 1; // Real state8/9 owner is not supplied, so no fabricated acceptance.
  }else if(t.h[9]){
   if(r->service==effect_start_timer)t.state.attack_gate=0x40000000;
   else if((r->service==effect_force_state||r->service==effect_state_event)&&!t.h[1])t.state.flags=0x100;
   else if(r->service==effect_heading_object)t.state.body_present=0;
  }
  return 0;
 }
 NativeEffectServices16 services(){return {this,&service};}
};
}
int main(int argc,char** argv){try{
 check(argc==2);std::ifstream gold(argv[1],std::ios::binary);char magic[4];gold.read(magic,4);check(!std::memcmp(magic,"NEF1",4));const auto count=word(gold);unsigned ordered=0;
 for(unsigned n=0;n<count;++n){Replay t;for(auto& v:t.h)v=word(gold);std::vector<Event> expected;for(unsigned j=0;j<t.h[23];++j){Event e{};for(auto& v:e)v=word(gold);expected.push_back(e);}
  t.effects.count=t.h[4];t.state.current=t.h[0]?8:9;t.state.attack_gate=t.h[5];t.state.flags=t.h[15];t.state.animation_override=std::int32_t(t.h[16]);t.state.controller_locked=t.h[12];t.state.idle_suppressed=t.h[13];t.state.body_present=t.h[14];auto c=t.services();int result;
  if(!t.h[1]){result=dh2_character_native_effect_set(&t.effects,t.h[0],t.h[11],t.h[7],t.h[24]?0xa0123456fedcba98ull:0,t.h[8],&c);std::int32_t id=std::int32_t(t.h[3]);if(id<0||id>=std::int32_t(t.h[4]))id=17;check(result==int(!t.h[2]&&std::uint32_t(id)<t.h[4]));}
  else check(dh2_character_native_effect_body(&t.effects,t.h[0],t.h[1]-1,&c)==1);
  check(t.trace==expected&&t.state.flags==t.h[17]&&std::uint32_t(t.state.animation_override)==t.h[18]&&t.state.attack_gate==t.h[19]&&t.state.controller_locked==t.h[20]&&t.state.idle_suppressed==t.h[21]&&t.state.body_present==t.h[22]);ordered+=expected.size();
 }check(gold.peek()==EOF);
 unsigned failure_prefixes=0;
 for(int fail=0;fail<=5;++fail){Replay t;t.h[6]=0x200;t.h[10]=3;t.h[24]=1;t.h[1]=0;t.state.flags=0;t.state.animation_override=-1;t.fail=fail;auto c=t.services();check(dh2_character_native_effect_set(&t.effects,0,250,1,0xa0123456fedcba98ull,1,&c)==-2&&t.trace.size()==unsigned(fail+1));check(t.state.attack_gate==unsigned(fail<=2?0:2));check(t.state.animation_override==(fail<=4?-1:1003)&&t.state.flags==0);++failure_prefixes;}
 unsigned guards=0;Replay t;auto c=t.services();State before=t.state;
 check(dh2_character_native_effect_set(nullptr,0,1,0,0,0,&c)==-1);++guards;
 check(dh2_character_native_effect_set(&t.effects,2,1,0,0,0,&c)==-1);++guards;
 check(dh2_character_native_effect_set(&t.effects,0,1,2,0,0,&c)==-1);++guards;
 check(dh2_character_native_effect_set(&t.effects,0,1,0,0,2,&c)==-1);++guards;
 check(dh2_character_native_effect_set(&t.effects,0,1,0,0,0,nullptr)==-1);++guards;
 check(dh2_character_native_effect_body(&t.effects,0,2,&c)==-1);++guards;
 t.effects.reserved=1;check(dh2_character_native_effect_set(&t.effects,0,1,0,0,0,&c)==-1);++guards;t.effects.reserved=0;
 check(!std::memcmp(&before,&t.state,sizeof before)&&t.trace.empty());
 // Genuine timer prefix, with explicit missing registered-state provider.
 Replay timer;timer.actual=true;timer.h[1]=0;unsigned expiries=0;timer.timer_services.context=&expiries;timer.timer_services.expired=[](void* context,std::uintptr_t,std::int32_t,Timer32*){++*static_cast<unsigned*>(context);};auto tc=timer.services();check(dh2_character_native_effect_set(&timer.effects,0,100,1,0,0,&tc)==-2&&timer.starts==1&&timer.store.count==1&&timer.state.attack_gate==2&&timer.state.flags==0);auto& slot=timer.slots[0];check(slot.active&&slot.duration_ms==100&&slot.repeat==0&&slot.event==0x2b&&!slot.user_ref);
 check(dh2_character_timers_update(&timer.store,25,0,&timer.timer_services)==1&&slot.elapsed_ms==25&&!expiries);check(dh2_character_native_effect_set(&timer.effects,0,999,1,0,0,&tc)==-2&&timer.starts==1&&slot.elapsed_ms==25&&slot.duration_ms==100);unsigned timer_checks=3;
 // Source Blur reaches the real native physical pin/mass/refilter bridge.
 b2AABB bounds;bounds.lowerBound.Set(-100,-100);bounds.upperBound.Set(100,100);b2World world(bounds,b2Vec2(0,0),true);b2BodyDef def;auto* body=world.CreateBody(&def);b2CircleDef shape;shape.radius=.36f;shape.localPosition.Set(.12f,-.08f);shape.density=1;body->CreateShape(&shape);body->SetMassFromShapes();dh2::physical::NativeBody native{body,.36f,0};Replay physical;physical.actual=true;physical.native=&native;physical.h[1]=2;physical.state.controller_locked=1;physical.state.body_present=1;auto pc=physical.services();check(dh2_character_native_effect_body(&physical.effects,0,1,&pc)==1&&physical.pins==1&&!physical.state.controller_locked&&body->IsStatic()&&native.pinned==1&&body->GetLocalCenter().x==.12f&&body->GetLocalCenter().y==-.08f);
 Dl_info module{},timer_lib{},body_lib{};check(dladdr(reinterpret_cast<void*>(&dh2_character_native_effect_set),&module));check(dladdr(reinterpret_cast<void*>(&dh2_character_timer_start),&timer_lib));check(dladdr(reinterpret_cast<void*>(&dh2_native_body_pin),&body_lib));
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"original_cases\":"<<count<<",\"ordered_services\":"<<ordered<<",\"native_failure_prefixes\":"<<failure_prefixes<<",\"atomic_guards\":"<<guards<<",\"genuine_timer_prefix_checks\":"<<timer_checks<<",\"genuine_native_body_pin\":true,\"missing_state_owner_rejected\":true,\"module_library\":\""<<module.dli_fname<<"\",\"timer_library\":\""<<timer_lib.dli_fname<<"\",\"body_library\":\""<<body_lib.dli_fname<<"\"}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
