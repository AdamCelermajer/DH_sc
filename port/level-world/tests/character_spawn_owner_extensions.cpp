// Reuse the frozen source-body replay provider and gold format; never rewrite
// the historical test or corpus. Only its unused main is renamed.
#define main frozen_spawn_body_audit_main
#include "character_spawn_body.cpp"
#undef main
#include "../character_spawn_owner_extensions.hpp"
#include "../character_state_owner_behavior.hpp"
namespace {
unsigned checks=0;
void verify_at(bool value,unsigned line){++checks;if(!value)throw std::runtime_error("Spawn owner audit line "+std::to_string(line));}
#define verify(...) verify_at((__VA_ARGS__),__LINE__)
struct Owned {
 CharacterStateOwner owner{0xa123456789abcdefull};
 Facts facts{};StateOwnerBehaviorPredicate8 predicates{};
 std::int32_t rows[1][40]{};unsigned ai_word=73,dt=17;
 PreSpawnState48 pre{&owner.state(),owner.native_fsm().character,rows,1,0,3,0,&ai_word};
 PreSpawnServices16 pre_services{this,pre_call};
 StateOwnerExtensions48 base{&pre,&pre_services,{this,remaining},{this,other_update}};
 SpawnBody48 spawn{&owner.state(),owner.native_fsm().character,rows,0xb123456789abcdefull,1,0x3f800000,{0,0}};
 SpawnBodyServices16 spawn_services{this,spawn_call};
 dh2::data::CombatRandom random{};NativeSpawn24 selection{&owner.native_fsm(),&random,0,0};
 SpawnPermission16 permission{0,1,0};std::array<Timer32,4> slots{};
 TimerStore32 timers{slots.data(),0,4,owner.native_fsm().character,0,0};
 TimerServices32 timer_services{this,expiry,nullptr,0};
 StateOwnerServices16 outer{};
 SpawnOwnerExtensions72 extension{&owner.machine(),&base,&spawn,&spawn_services,&selection,&permission,&timers,&timer_services,&outer};
 StateOwnerServices16 methods{&extension,dh2_character_spawn_owner_extensions_method};
 StateOwnerUpdateServices16 updates{&extension,dh2_character_spawn_owner_extensions_update};
 Services bodies{this,body};StateOwnerBehaviorContext40 behavior{&facts,&bodies,&predicates,methods};
 NativeTargetDebug native;
 std::vector<unsigned> trace;
 unsigned notifications=0,expiries=0,physical=0,scene=0;
 int failure=-1;bool nested_marker=false,nested_idle=false;
 Owned(){rows[0][25]=777;rows[0][32]=800;facts.idle=115;verify(dh2_character_state_owner_behavior_bind(&outer,&behavior)==1);}
 static int pre_call(void* p,PreSpawnState48* s,const PreSpawnRequest32* r,PreSpawnResponse8* out){
  auto& f=*static_cast<Owned*>(p);f.trace.push_back(100+r->service);
  switch(r->service){
   case pre_spawn_animation_index:out->word=0;return 0;
   case pre_spawn_set_animation:s->state->current_animation=int(r->argument0);++f.scene;return 0;
   case pre_spawn_remove_physical:s->state->body_present=0;return 0;
   case pre_spawn_init_physical:s->state->body_present=1;++f.physical;return 0;
   case pre_spawn_enable:case pre_spawn_disable_collisions:case pre_spawn_enable_collisions:case pre_spawn_revive:++f.scene;return 0; // Explicit required scene/Revive fixtures.
   default:return 1; // Predicate and selection MUST be intercepted by native adapter.
  }
 }
 static int spawn_call(void* p,SpawnBody48* s,const SpawnBodyRequest32* r,unsigned* out){
  auto& f=*static_cast<Owned*>(p);f.trace.push_back(200+r->service);
  if(int(r->service)==f.failure)return 1;
  switch(r->service){
   case spawn_debug_load:return dh2_character_debug_load(f.native.debug,&f.native.files)==1?0:1;
   case spawn_debug_query:return dh2_character_debug_get(out,f.native.debug,r->argument0?"isTracingCSSpawn":"isTracingCharState",&f.native.files)==1?0:1;
   case spawn_animation_index:*out=0;return 0;
   case spawn_stance_mask:*out=1;return 0;
   case spawn_stance:*out=4;return 0;
   case spawn_set_animation:
    s->state->current_animation=int(r->argument0);++f.scene;
    if(f.nested_marker){f.nested_marker=false;verify(f.owner.event(0x28,reinterpret_cast<std::uintptr_t>("is_interactive"),f.outer)==0);}
    return 0;
   case spawn_set_target:return dh2_character_ai_set_target(&f.native.target,0,0,&f.native.target_services);
   case spawn_sync_last_target:return dh2_character_ai_sync_last_target(&f.native.target);
   case spawn_cancel_sneaking:++f.scene;return 0;
   case spawn_fade_in:return dh2_character_spawn_fade_in_empty()==1?0:1;
   case spawn_init_physical:s->state->body_present=1;++f.physical;return 0;
   default:return 1;
  }
 }
 static int remaining(void* p,StateOwnerMachine40*,const StateOwnerRequest48* r,StateOwnerResponse8*){
  auto& f=*static_cast<Owned*>(p);f.trace.push_back(300+r->operation);
  if(r->operation==state_owner_character_event){verify(r->event==0x1d&&r->source_function==0x3a4d5c);++f.notifications;
   if(f.nested_idle&&f.owner.state().current==1){f.nested_idle=false;verify(f.owner.transition(3,-1,0,f.outer)==1);}return 0;}
  return r->operation==state_owner_profile_begin||r->operation==state_owner_profile_end?0:1;
 }
 static int other_update(void*,StateOwnerMachine40*,const StateOwnerUpdateRequest24*){return 1;}
 static void body(void* p,State* s,const Request* r){auto& f=*static_cast<Owned*>(p);f.trace.push_back(400+r->service);if(r->service==set_animation)s->current_animation=r->argument[0];}
 static void expiry(void* p,std::uintptr_t id,int event,Timer32* timer){auto& f=*static_cast<Owned*>(p);verify(id==f.owner.native_fsm().character&&event==0x2d&&!timer->user_ref);++f.expiries;verify(f.owner.event(event,reinterpret_cast<std::uintptr_t>(timer),f.outer)==1);} // Explicit Character event provider, not full AI relay.
 static int frame_service(void* p,NativeFsm24*,const NativeFsmRequest32* r,unsigned* out){auto& f=*static_cast<Owned*>(p);if(r->service==fsm_engine_dt){*out=f.dt;return 0;}return r->service==fsm_profile_begin||r->service==fsm_profile_end?0:1;}
 void initialize(){verify(owner.initialize_level(17,outer)==1&&owner.state().current==17);trace.clear();}
 void stage(){owner.state()={};owner.state().current=17;owner.machine().current_index=17;owner.native_fsm().current_present=1;timers.count=0;slots={};random={};ai_word=73;trace.clear();} // Corpus fixture only.
};
}
int main(int argc,char** argv){try{
 verify(argc==4);
 // Original Spawn1 gold through the NEW metadata/owner adapter. Initial logical
 // machine storage is fixture staging; production transitions are tested below.
 Replay replay;CharacterStateOwner metadata{0xa123456789abcdefull};
 NativeFsm24 fsm{&replay.state,metadata.native_fsm().character,1,0};auto machine=metadata.machine();machine.fsm=&fsm;machine.current_index=1;
 PreSpawnState48 pre{&replay.state,fsm.character,replay.rows[0],2,0,0,0,nullptr};
 PreSpawnServices16 pre_services{nullptr,[](void*,PreSpawnState48*,const PreSpawnRequest32*,PreSpawnResponse8*){return 1;}};
 StateOwnerExtensions48 base{&pre,&pre_services,{},{} };dh2::data::CombatRandom random{};NativeSpawn24 selection{&fsm,&random,0,0};SpawnPermission16 permission{};
 std::array<Timer32,1> slots{};TimerStore32 timers{slots.data(),0,1,fsm.character,0,0};TimerServices32 timer_services{nullptr,[](void*,std::uintptr_t,int,Timer32*){},nullptr,0};
 StateOwnerServices16 outer{nullptr,[](void*,StateOwnerMachine40*,const StateOwnerRequest48*,StateOwnerResponse8*){return 1;}};
 SpawnOwnerExtensions72 extension{&machine,&base,&replay.view,&replay.services,&selection,&permission,&timers,&timer_services,&outer};
 std::ifstream corpus(argv[1],std::ios::binary);verify(read<unsigned>(corpus)==0x31425053);const auto count=read<unsigned>(corpus);unsigned ordered=0;const char* strings[]={"","is_interactive","Is_interactive","body","is_interactive_more"};
 for(unsigned i=0;i<count;++i){auto p=read<std::array<unsigned,16>>(corpus);const auto expected=read<State>(corpus);const auto projection=read<std::array<unsigned,4>>(corpus);auto n=read<unsigned>(corpus);std::vector<SpawnBodyRequest32> gold(n);for(auto& r:gold)r=read<SpawnBodyRequest32>(corpus);replay.init(p);
  if(p[0]==2){StateOwnerUpdateRequest24 r{0x3bfff0,1,fsm.character,replay.state.elapsed_ms,0};verify(dh2_character_spawn_owner_extensions_update(&extension,&machine,&r)==0);}
  else{const auto* info=metadata.info(1);const unsigned op=p[0]==0?state_owner_focus:p[0]==1?state_owner_blur:state_owner_event;StateOwnerRequest48 r{op,p[0]==0?info->focus:p[0]==1?info->blur:info->on_event,1,int(p[1]),p[13],0,fsm.character,p[13]==0x28?reinterpret_cast<std::uintptr_t>(strings[p[14]]):0xb123456789abcdefull,{0,0}};StateOwnerResponse8 response{73,99};verify(dh2_character_spawn_owner_extensions_method(&extension,&machine,&r,&response)==0&&response.next==73&&response.accepted==99);}
  const std::array<unsigned,4> got{unsigned(bool(replay.view.visual)),replay.view.fade_word,replay.target,replay.last};verify(!std::memcmp(&replay.state,&expected,56)&&got==projection&&replay.calls.size()==n);for(unsigned k=0;k<n;++k)verify(!std::memcmp(&gold[k],&replay.calls[k],32));ordered+=n;
 }
 verify(corpus.peek()==std::ifstream::traits_type::eof());
 Owned f;f.initialize();f.nested_marker=true;f.owner.state().elapsed_ms=999;f.owner.state().flags|=0x2000;
 verify(f.owner.event(9,0,f.outer)==0&&f.owner.state().current==1&&f.owner.machine().current_index==1&&f.owner.state().elapsed_ms==0&&f.owner.state().flags==0x2241&&f.owner.state().current_animation==804&&f.ai_word==0&&!f.timers.count&&f.physical==1);
 verify(f.trace==std::vector<unsigned>{100+pre_spawn_enable,100+pre_spawn_revive,100+pre_spawn_enable_collisions,200+spawn_debug_load,200+spawn_debug_query,200+spawn_debug_load,200+spawn_debug_query,200+spawn_animation_index,200+spawn_stance_mask,200+spawn_stance,200+spawn_set_animation,200+spawn_init_physical,200+spawn_set_target,200+spawn_sync_last_target,200+spawn_cancel_sneaking,200+spawn_fade_in,300+state_owner_character_event});
 verify(!f.native.target.candidate&&!f.native.target.target&&!f.native.target.last_target&&!f.native.owner.word14d0&&f.native.missing_file==1);
 StateOwnerFrameContext56 frame{&f.owner.machine(),&f.facts,&f.bodies,{&f,Owned::frame_service},f.updates};f.owner.state().elapsed_ms=0xfffffff8u;auto prior=f.trace.size();verify(dh2_character_state_owner_frame(&frame)==1&&f.owner.state().elapsed_ms==9&&f.trace.size()==prior);
 verify(f.owner.event(0x22,0,f.outer)==1&&f.owner.state().current==3&&f.owner.state().current_animation==115&&f.physical==1);
 // Genuine permission rejection for every raw byte and source group branch.
 std::ifstream permissions(argv[3],std::ios::binary);verify(read<unsigned>(permissions)==0x31505053);auto permission_count=read<unsigned>(permissions);
 for(unsigned i=0;i<permission_count;++i){const auto p=read<std::array<unsigned,3>>(permissions);f.stage();f.permission={p[0]?0xd123456789abcdefull:0,p[1],0};verify(f.owner.event(9,0,f.outer)==0&&f.owner.state().current==(p[2]?1:17)&&f.ai_word==(p[2]?0u:73u));}
 verify(permissions.peek()==std::ifstream::traits_type::eof());f.permission={0,1,0};
 // Original selection corpus now creates REAL timers or genuine owned state.
 std::ifstream selections(argv[2],std::ios::binary);verify(read<unsigned>(selections)==0x31535053);auto selection_count=read<unsigned>(selections);unsigned zero_timers=0;
 for(unsigned i=0;i<selection_count;++i){const auto p=read<std::array<unsigned,8>>(selections);const auto expected=read<std::array<unsigned,4>>(selections);const auto request=read<SpawnSelectRequest32>(selections);f.stage();f.selection.minimum_ms=int(p[2]);f.selection.maximum_ms=int(p[3]);f.random={p[4],p[5]};verify(dh2_character_spawn_owner_select(&f.extension,p[0],p[1])==1);
  verify((std::array<unsigned,4>{unsigned(f.selection.minimum_ms),unsigned(f.selection.maximum_ms),f.random.seed,f.random.calls})==expected);
  if(request.service==spawn_select_state)verify(f.owner.state().current==1&&!f.timers.count);
  else{verify(f.owner.state().current==17&&f.timers.count==1&&f.slots[0].duration_ms==request.argument0&&f.slots[0].repeat==0&&f.slots[0].event==0x2d&&!f.slots[0].user_ref);zero_timers+=!request.argument0;}
 }
 verify(selections.peek()==std::ifstream::traits_type::eof());
 f.stage();f.selection.minimum_ms=f.selection.maximum_ms=5;verify(f.owner.event(9,0,f.outer)==0&&f.owner.state().current==17&&f.timers.count==1&&f.ai_word==0);verify(dh2_character_timers_update(&f.timers,4,0,&f.timer_services)==1&&!f.expiries);verify(dh2_character_timers_update(&f.timers,1,0,&f.timer_services)==1&&f.expiries==1&&f.owner.state().current==1&&!f.slots[0].active);
 f.stage();f.selection.minimum_ms=0;f.selection.maximum_ms=1;verify(f.owner.event(9,0,f.outer)==0&&f.slots[0].duration_ms==0);verify(dh2_character_timers_update(&f.timers,1000,0,&f.timer_services)==1&&f.expiries==1&&f.owner.state().current==17&&f.slots[0].active);
 f.stage();f.selection.minimum_ms=f.selection.maximum_ms=0;f.nested_idle=true;verify(f.owner.event(9,0,f.outer)==0&&f.owner.state().current==3&&f.ai_word==0&&f.owner.state().elapsed_ms==0);
 f.stage();verify(dh2_character_spawn_owner_select(&f.extension,0,0)==1);f.owner.state().elapsed_ms=123;const auto notices=f.notifications;
 verify(dh2_character_spawn_owner_select(&f.extension,0,0)==1&&f.owner.state().current==1&&f.owner.state().elapsed_ms==123&&f.notifications==notices+1); // Source same-state Blur/Focus/notification, elapsed retained.
 f.stage();f.selection.minimum_ms=f.selection.maximum_ms=5;const auto storage=f.timers;f.timers.slots=nullptr;f.timers.capacity=0;
 verify(f.owner.event(9,0,f.outer)==-2&&f.owner.state().current==17&&f.ai_word==73&&!f.timers.count);f.timers=storage; // Unavailable caller storage fails, no fabricated timer.
 // Required provider failure retains source prefix and the already selected1.
 f.stage();f.selection.minimum_ms=f.selection.maximum_ms=0;f.failure=spawn_cancel_sneaking;verify(f.owner.event(9,0,f.outer)==-2&&f.owner.state().current==1&&f.ai_word==73);f.failure=-1;
 unsigned guards=0;auto bad=f.extension;const auto before=f.owner.state();const auto calls=f.trace.size();
 auto reject=[&](SpawnOwnerExtensions72 c){verify(dh2_character_spawn_owner_select(&c,0,0)==-1&&!std::memcmp(&before,&f.owner.state(),56)&&f.trace.size()==calls);++guards;};
 bad.machine=nullptr;reject(bad);bad=f.extension;bad.outer_methods=nullptr;reject(bad);bad=f.extension;bad.timer_services=nullptr;reject(bad);bad=f.extension;bad.permission=nullptr;reject(bad);bad=f.extension;bad.spawn=nullptr;reject(bad);bad=f.extension;bad.selection=nullptr;reject(bad);bad=f.extension;bad.remaining=nullptr;reject(bad);
 f.stage();StateOwnerResponse8 response{42,99};StateOwnerRequest48 r{state_owner_focus,0x3c35ec,1,17,0,0,f.owner.native_fsm().character,0,{0,0}};verify(dh2_character_spawn_owner_extensions_method(&f.extension,&f.owner.machine(),&r,&response)!=0&&f.trace.empty());++guards;
 f.owner.machine().current_index=1;f.owner.state().current=1;r.source_function^=4;verify(dh2_character_spawn_owner_extensions_method(&f.extension,&f.owner.machine(),&r,&response)!=0&&f.trace.empty());++guards;
 r.source_function^=4;r.reserved[1]=1;verify(dh2_character_spawn_owner_extensions_method(&f.extension,&f.owner.machine(),&r,&response)!=0&&f.trace.empty());++guards;r.reserved[1]=0;
 f.owner.machine().current_index=8;f.owner.state().current=8;r.state=8;r.source_function=f.owner.info(8)->focus;
 verify(dh2_character_spawn_owner_extensions_method(&f.extension,&f.owner.machine(),&r,&response)!=0&&f.trace==std::vector<unsigned>{300+state_owner_focus}&&response.next==42&&response.accepted==99);
 std::printf("{\"validation\":\"PASS\",\"checks\":%u,\"original_Spawn_body_cases\":%u,\"original_Spawn_body_requests\":%u,\"original_selection_cases\":%u,\"original_permission_cases\":%u,\"zero_duration_timer_cases\":%u,\"atomic_owner_guards\":%u,\"actual_PreSpawn_Spawn_Idle_transition\":true,\"nested_interactive_event\":true,\"nested_notification_transition\":true,\"elapsed_once\":true,\"genuine_target_debug\":true,\"missing_file_provider_is_fixture\":true,\"timer_expiry_AI_relay_is_fixture\":true,\"full_scene_backends\":false,\"mismatches\":0}\n",checks,count,ordered,selection_count,permission_count,zero_timers,guards);
 return 0;
}catch(const std::exception& e){std::fprintf(stderr,"%s (checks=%u)\n",e.what(),checks);return 1;}}
