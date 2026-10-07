#include "../character_state_owner_extensions.hpp"
#include "../character_state_owner_behavior.hpp"
#include <array>
#include <cstdio>
#include <cstring>
#include <dlfcn.h>
#include <fstream>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
namespace {
unsigned checks=0;
void check(bool value){++checks;if(!value)throw std::runtime_error("state extension composition mismatch");}
template<class T>T read(std::ifstream& f){T value{};f.read(reinterpret_cast<char*>(&value),sizeof value);check(bool(f));return value;}
// Declared host scene providers. No physics/Revive/animation backend claim.
struct Fixture {
 CharacterStateOwner owner{0xa123456789abcdefull};
 Facts facts{};StateOwnerBehaviorPredicate8 predicates{};
 std::int32_t rows[1][40]{};std::uint32_t ai_word=73,dt=17;
 PreSpawnState48 pre{&owner.state(),owner.native_fsm().character,rows,1,0,3,0,&ai_word};
 PreSpawnServices16 spawn{this,spawn_call};
 StateOwnerExtensions48 extensions{&pre,&spawn,{this,remaining},{this,update_remaining}};
 StateOwnerServices16 methods{&extensions,dh2_character_state_owner_extensions_method};
 StateOwnerUpdateServices16 updates{&extensions,dh2_character_state_owner_extensions_update};
 Services bodies{this,body};
 StateOwnerBehaviorContext40 behavior_context{&facts,&bodies,&predicates,methods};
 StateOwnerServices16 behavior{};
 std::vector<unsigned> spawn_calls,body_calls;
 unsigned remaining_calls=0,remaining_updates=0,notifications=0,nested=0;
 int failure=-1;bool reenter=false;
 Fixture(){rows[0][25]=777;rows[0][32]=778;facts.idle=115;check(dh2_character_state_owner_behavior_bind(&behavior,&behavior_context)==1);}
 static int spawn_call(void* p,PreSpawnState48* s,const PreSpawnRequest32* r,PreSpawnResponse8* out){
  auto& f=*static_cast<Fixture*>(p);f.spawn_calls.push_back(r->service);
  check(s==&f.pre&&s->state==&f.owner.state()&&r->character==f.owner.native_fsm().character);
  if(int(r->service)==f.failure)return 1;
  switch(r->service){
   case pre_spawn_animation_index:out->word=0;break;
   case pre_spawn_set_animation:
    s->state->current_animation=static_cast<std::int32_t>(r->argument0);
    if(f.reenter){f.reenter=false;++f.nested;check(f.owner.event(0x28,reinterpret_cast<std::uintptr_t>("is_interactive"),f.behavior)==0);}break;
   case pre_spawn_remove_physical:s->state->body_present=0;break;
   case pre_spawn_predicate:out->word=0;break; // Explicit rejected CSM_Spawn fixture.
   default:break;
  }
  return 0;
 }
 static int remaining(void* p,StateOwnerMachine40*,const StateOwnerRequest48* r,StateOwnerResponse8*){
  auto& f=*static_cast<Fixture*>(p);++f.remaining_calls;
  if(r->operation==state_owner_character_event){check(r->source_function==0x3a4d5c&&r->event==0x1d);++f.notifications;return 0;}
  if(r->operation==state_owner_profile_begin||r->operation==state_owner_profile_end)return 0;
  return 1;
 }
 static int update_remaining(void* p,StateOwnerMachine40*,const StateOwnerUpdateRequest24*){++static_cast<Fixture*>(p)->remaining_updates;return 1;}
 static void body(void* p,State* s,const Request* r){auto& f=*static_cast<Fixture*>(p);f.body_calls.push_back(r->service);if(r->service==set_animation)s->current_animation=r->argument[0];}
 static int outer(void* p,NativeFsm24*,const NativeFsmRequest32* r,std::uint32_t* out){
  auto& f=*static_cast<Fixture*>(p);
  if(r->service==fsm_engine_dt){*out=f.dt;return 0;}
  return r->service==fsm_profile_begin||r->service==fsm_profile_end?0:1;
 }
 // Metadata corpus staging is not a production transition implementation.
 void stage(int id){owner.state()=State{};owner.state().current=id;owner.machine().current_index=id;owner.native_fsm().current_present=1;}
};
}
int main(int argc,char** argv){try{
 check(argc==2);Fixture f;f.reenter=true;
 check(f.owner.initialize_level(17,f.behavior)==1);
 check(f.owner.state().current==17&&f.owner.state().flags==0x3300&&f.owner.state().current_animation==777);
 check(f.nested==1&&f.notifications==1&&f.ai_word==73&&f.body_calls.empty());
 check(f.spawn_calls==std::vector<unsigned>{pre_spawn_animation_index,pre_spawn_animation_index,
  pre_spawn_set_animation,pre_spawn_init_physical,pre_spawn_remove_physical,pre_spawn_enable,pre_spawn_disable_collisions});
 f.owner.state().elapsed_ms=0xfffffff8u;
 StateOwnerFrameContext56 frame{&f.owner.machine(),&f.facts,&f.bodies,{&f,Fixture::outer},f.updates};
 const auto calls=f.spawn_calls.size();check(dh2_character_state_owner_frame(&frame)==1);
 check(f.owner.state().elapsed_ms==9&&f.spawn_calls.size()==calls&&!f.remaining_updates);
 check(f.owner.event(9,0xb123456789abcdefull,f.behavior)==0&&f.owner.state().current==17);
 f.spawn_calls.clear();check(f.owner.transition(3,-1,0,f.behavior)==1);
 check(f.spawn_calls==std::vector<unsigned>{pre_spawn_enable,pre_spawn_revive,pre_spawn_enable_collisions});
 check(f.owner.state().current==3&&f.owner.state().elapsed_ms==0&&f.owner.state().current_animation==115);
 check(f.owner.transition(17,-1,0,f.behavior)==1);
 f.failure=pre_spawn_revive;f.spawn_calls.clear();const auto notifications=f.notifications;
 check(f.owner.transition(3,-1,0,f.behavior)==-2&&f.owner.state().current==17);
 check(f.spawn_calls==std::vector<unsigned>{pre_spawn_enable,pre_spawn_revive}&&f.notifications==notifications);
 f.failure=-1;
 unsigned empty=0,required=0,guards=0;
 std::ifstream corpus(argv[1],std::ios::binary);check(read<unsigned>(corpus)==0x314d4553);const auto count=read<unsigned>(corpus);
 for(unsigned i=0;i<count;++i){const auto id=read<unsigned>(corpus),op=read<unsigned>(corpus),source=read<unsigned>(corpus),is_empty=read<unsigned>(corpus);f.stage(id);
  const auto before=f.owner.state();const auto prior_methods=f.remaining_calls,prior_updates=f.remaining_updates;
  StateOwnerResponse8 response{42,99};
  if(op==state_method_update){
   StateOwnerUpdateRequest24 request{source,int(id),f.owner.native_fsm().character,f.owner.state().elapsed_ms,0};
   check(dh2_character_state_owner_extensions_update(&f.extensions,&f.owner.machine(),&request)==(is_empty?0:1));
   check(f.remaining_updates==prior_updates+(is_empty?0:1));request.source_function^=4;
   const auto after=f.remaining_updates;check(dh2_character_state_owner_extensions_update(&f.extensions,&f.owner.machine(),&request)!=0&&f.remaining_updates==after);++guards;
  }else{
   if(id==17)continue; // Complete nonempty PreSpawn methods were composed above.
   const unsigned operation=op==state_method_focus?state_owner_focus:op==state_method_blur?state_owner_blur:state_owner_event;
   StateOwnerRequest48 request{operation,source,int(id),0,0xcafe,0,f.owner.native_fsm().character,0,{0,0}};
   check(dh2_character_state_owner_extensions_method(&f.extensions,&f.owner.machine(),&request,&response)==(is_empty?0:1));
   check(f.remaining_calls==prior_methods+(is_empty?0:1)&&response.next==42&&response.accepted==99);
   request.source_function^=4;const auto after=f.remaining_calls;
   check(dh2_character_state_owner_extensions_method(&f.extensions,&f.owner.machine(),&request,&response)!=0&&f.remaining_calls==after);++guards;
  }
  check(!std::memcmp(&before,&f.owner.state(),sizeof before));if(is_empty)++empty;else ++required;
 }
 check(corpus.peek()==std::ifstream::traits_type::eof()&&empty==18&&required==43);
 f.stage(17);StateOwnerResponse8 response{42,99};const auto* info=f.owner.info(17);
 StateOwnerRequest48 request{state_owner_focus,info->focus,17,-1,0xffffffffu,0,f.owner.native_fsm().character,0,{0,0}};
 const auto before=f.owner.state();const auto prior=f.spawn_calls.size();
 f.pre.character=1;check(dh2_character_state_owner_extensions_method(&f.extensions,&f.owner.machine(),&request,&response)!=0&&f.spawn_calls.size()==prior);++guards;
 f.pre.character=f.owner.native_fsm().character;f.pre.state=nullptr;
 check(dh2_character_state_owner_extensions_method(&f.extensions,&f.owner.machine(),&request,&response)!=0&&f.spawn_calls.size()==prior);++guards;
 f.pre.state=&f.owner.state();request.character=1;
 check(dh2_character_state_owner_extensions_method(&f.extensions,&f.owner.machine(),&request,&response)!=0&&f.spawn_calls.size()==prior);++guards;
 request.character=f.owner.native_fsm().character;
 std::array<StateOwnerInfo40,20> altered{};
 const auto* original=f.owner.machine().states;
 std::memcpy(altered.data(),original,sizeof altered);altered[17].focus=0xdead;
 f.owner.machine().states=altered.data();request.source_function=0xdead;
 check(dh2_character_state_owner_extensions_method(&f.extensions,&f.owner.machine(),&request,&response)!=0&&f.spawn_calls.size()==prior);++guards;
 f.owner.machine().states=original;
 check(!std::memcmp(&before,&f.owner.state(),sizeof before)&&response.next==42&&response.accepted==99);
 Dl_info origin{};check(dladdr(reinterpret_cast<void*>(dh2_character_state_owner_extensions_method),&origin));
 std::printf("{\"validation\":\"PASS\",\"checks\":%u,\"genuine_empty_method_deliveries\":%u,\"required_nonempty_failure_prefixes\":%u,\"metadata_and_owner_guards\":%u,\"nested_PreSpawn_event\":1,\"actual_owner_PreSpawn_Idle_transition\":true,\"single_elapsed_frame\":true,\"module_library\":\"%s\",\"full_scene_backends\":false}\n",checks,empty,required,guards,origin.dli_fname);
 return 0;
}catch(const std::exception& e){std::fprintf(stderr,"%s\n",e.what());return 1;}}
