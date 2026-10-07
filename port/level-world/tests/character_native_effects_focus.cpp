#include "character_native_effects.hpp"
#include "native_body.hpp"
#include "../game-data/animation_tables.hpp"
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
unsigned checks=0;void check(bool v){++checks;if(!v)throw std::runtime_error("native effect focus check "+std::to_string(checks));}
std::uint32_t word(std::istream& f){std::uint32_t v=0;f.read(reinterpret_cast<char*>(&v),4);check(bool(f));return v;}
using Event=std::array<std::uint32_t,5>;
struct Replay {
 State state{};NativeFsm24 view{&state,0xabcdef0123456789ull,1,0};std::array<std::int32_t,20> stun{},scare{};NativeEffects32 effects{&view,stun.data(),scare.data(),20,0};std::array<std::uint32_t,18> h{};std::vector<Event> trace;unsigned draw=0;int fail=-1;bool actual=false;std::uint32_t seed=1,calls=0;unsigned unpins=0;dh2::physical::NativeBody* native=nullptr;
 Replay(){for(unsigned i=0;i<20;++i){stun[i]=1000+10*i;scare[i]=2000+10*i;}}
 static int service(void* p,NativeEffects32* e,const NativeEffectRequest32* r,std::uint32_t* out){auto& t=*static_cast<Replay*>(p);check(e==&t.effects&&r->character==t.view.character&&!r->payload);Event event{r->service,0,0,0,0};*out=0;
  switch(r->service){
  case effect_animation_index:event[1]=t.h[3];event[2]=t.state.flags;*out=t.h[3];break;
  case effect_stance_bits:*out=t.h[4];break;
  case effect_anim_stance:*out=t.h[5];break;
  case effect_set_animation:event[1]=r->argument0;event[4]=1;break;
  case effect_is_player:event[4]=1;*out=t.h[6];break;
  case effect_cancel_sneaking:event[4]=1;if(t.h[9])t.state.body_present=0;break;
  case effect_unpin:event[4]=1;break;
  case effect_random:event[1]=r->argument0;*out=t.actual?dh2_animation_random(&t.seed,&t.calls,r->argument0):t.h[10+t.draw];event[2]=*out;++t.draw;break;
  case effect_heading_point:event={r->service,r->argument0,r->argument1,r->argument2,1};break;
  default:check(false);
  }
  t.trace.push_back(event);if(int(r->service)==t.fail)return 1;
  if(t.actual&&r->service==effect_unpin){check(t.native&&dh2_native_body_unpin(t.native)==0);++t.unpins;}
  return 0;
 }
 NativeEffectServices16 services(){return {this,&service};}
};
}
int main(int argc,char** argv){try{
 check(argc==2);std::ifstream gold(argv[1],std::ios::binary);char magic[4];gold.read(magic,4);check(!std::memcmp(magic,"NEF2",4));const auto count=word(gold);unsigned ordered=0;
 for(unsigned n=0;n<count;++n){Replay t;for(auto& v:t.h)v=word(gold);std::vector<Event> expected;for(unsigned j=0;j<t.h[17];++j){Event e{};for(auto& v:e)v=word(gold);expected.push_back(e);}
  t.state.flags=0x12345678;t.state.controller_locked=t.h[8];t.state.body_present=t.h[7];auto c=t.services();check((t.h[1]?dh2_character_native_effect_event(&t.effects,t.h[0],t.h[2],&c):dh2_character_native_effect_focus(&t.effects,t.h[0],&c))==1);check(t.trace==expected&&t.state.flags==t.h[14]&&t.state.controller_locked==t.h[15]&&t.state.body_present==t.h[16]);ordered+=expected.size();
 }check(gold.peek()==EOF);
 unsigned prefixes=0;
 for(int fail:{effect_animation_index,effect_stance_bits,effect_anim_stance,effect_set_animation,effect_random,effect_heading_point,effect_cancel_sneaking,effect_unpin}){Replay t;t.state.flags=0x12345678;t.state.body_present=1;t.h[4]=0x100;t.h[5]=3;t.h[10]=2;t.h[11]=3;t.h[12]=49;t.h[13]=50;t.fail=fail;auto c=t.services();check(dh2_character_native_effect_focus(&t.effects,1,&c)==-2&&t.trace.back()[0]==unsigned(fail)&&t.state.flags==0x2240);++prefixes;}
 Replay t;auto c=t.services();const auto before=t.state;unsigned guards=0;
 check(dh2_character_native_effect_focus(nullptr,0,&c)==-1);++guards;
 check(dh2_character_native_effect_focus(&t.effects,2,&c)==-1);++guards;
 check(dh2_character_native_effect_focus(&t.effects,0,nullptr)==-1);++guards;
 check(dh2_character_native_effect_event(nullptr,0,0,&c)==-1);++guards;
 check(dh2_character_native_effect_event(&t.effects,2,0x23,&c)==-1);++guards;
 check(!std::memcmp(&before,&t.state,sizeof before)&&t.trace.empty());
 b2AABB bounds;bounds.lowerBound.Set(-100,-100);bounds.upperBound.Set(100,100);b2World world(bounds,b2Vec2(0,0),true);b2BodyDef def;auto* body=world.CreateBody(&def);b2CircleDef shape;shape.radius=.36f;shape.localPosition.Set(.12f,-.08f);shape.density=1;body->CreateShape(&shape);body->SetMassFromShapes();dh2::physical::NativeBody native{body,.36f,0};check(!dh2_native_body_pin(&native));Replay physical;physical.actual=true;physical.native=&native;physical.state.body_present=1;physical.h[6]=1;auto pc=physical.services();check(dh2_character_native_effect_focus(&physical.effects,0,&pc)==1&&physical.unpins==1&&physical.state.flags==0x2202&&physical.state.controller_locked==1&&body->IsDynamic()&&!native.pinned&&body->GetLocalCenter().x==.12f&&body->GetLocalCenter().y==-.08f);
 check(!dh2_native_body_pin(&native));physical.trace.clear();check(dh2_character_native_effect_focus(&physical.effects,1,&pc)==1&&physical.unpins==2&&physical.state.flags==0x2240&&physical.calls==4&&physical.draw==4&&body->IsDynamic());const auto first=physical.trace[7];check(first[0]==effect_heading_point&&first[3]==0);physical.trace.clear();check(dh2_character_native_effect_event(&physical.effects,1,0x23,&pc)==1&&physical.calls==8&&physical.draw==8&&physical.trace.size()==5&&physical.trace.back()[0]==effect_heading_point&&physical.trace.back()!=first);
 Dl_info module{},body_lib{},rng{};check(dladdr(reinterpret_cast<void*>(&dh2_character_native_effect_focus),&module));check(dladdr(reinterpret_cast<void*>(&dh2_native_body_unpin),&body_lib));check(dladdr(reinterpret_cast<void*>(&dh2_animation_random),&rng));
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"original_cases\":"<<count<<",\"ordered_services\":"<<ordered<<",\"native_failure_prefixes\":"<<prefixes<<",\"atomic_guards\":"<<guards<<",\"actual_native_focus_unpins\":"<<physical.unpins<<",\"actual_source_RNG_calls\":"<<physical.calls<<",\"registered_transition_owner_bound\":false,\"module_library\":\""<<module.dli_fname<<"\",\"body_library\":\""<<body_lib.dli_fname<<"\",\"RNG_library\":\""<<rng.dli_fname<<"\"}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
