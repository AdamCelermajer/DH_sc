#include "character_script_lifecycle.hpp"
#include <cstddef>
#include <cstring>
namespace {
using namespace dh2::character;
bool overlaps(const void* a,std::size_t n,const void* b,std::size_t m){auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<=y?y-x<n:x-y<m;}
std::int32_t signed_word(std::uint32_t u){std::int32_t r;std::memcpy(&r,&u,4);return r;}
struct Kernel {
 ScriptLifecycleState64& s;const ScriptLifecycleServices16& c;
 ScriptLifecycleResponse16 call(std::uint32_t service,std::uint32_t a=0,std::uint32_t b=0,std::uintptr_t subject=0,std::uintptr_t payload=0){
  ScriptLifecycleRequest32 request{service,a,b,0,subject,payload};ScriptLifecycleResponse16 out{};c.invoke(c.context,&s,&request,&out);return out;
 }
 void replace(){
  if(s.pending){call(script_ai_terminate);if(s.pending){call(script_destroy,0,0,s.pending);s.pending=0;}}
  const auto created=call(script_construct_iphone,0xd8,1).identity;
  s.pending=created;
 }
 void init_step(){call(script_ai_init);if(s.external_name)call(script_pending_init_vcb,0,0,s.pending);}
 void load(){
  if(s.load_step>6)return;
  std::int32_t remaining=6;
  if(s.delayed&&!call(script_owner_is_character,0,0,s.owner).word){
   const auto available=signed_word(call(script_query_budget,7u-std::uint32_t(s.load_step)).word);
   if(available<=0)return;
   remaining=available-1;
  }
  for(;;){
   switch(s.load_step){
    case 0:call(script_create_step);break;
    case 1:call(script_bind_functions,0,0,s.pending);break;
    case 2:call(script_set_character,0,0,s.pending,s.owner);break;
    case 3:if(s.scripted)call(script_load_common,0,0,s.pending);break;
    case 4:if(s.external_name)call(script_load_external,0,0,s.pending,s.external_name);break;
    case 5:init_step();break;
    case 6:s.active=s.pending;break;
    default:break; // Original assertion diagnostics have no projected effects.
   }
   s.load_step=signed_word(std::uint32_t(s.load_step)+1);
   if(!remaining)break;
   --remaining;
  }
 }
 void init_process(std::uint32_t final){
  call(script_refresh_vitals,0,0,s.owner);call(script_configure_skills);call(script_update_skills);
  call(script_ai_init_post);if(final)call(script_ai_init_final);
 }
 void timers(){
  if(call(script_owner_is_dead,0,0,s.owner).word)return;
  if(s.timer33!=-1)call(script_timer_stop,std::uint32_t(s.timer33),0,s.owner);
  const auto owner33=s.owner;const auto tick33=call(script_design_tick,0x33).word;
  s.timer33=signed_word(call(script_timer_start,tick33,0x33,owner33).word);
  if(s.timer34!=-1)call(script_timer_stop,std::uint32_t(s.timer34),0,s.owner);
  const auto owner34=s.owner;const auto tick34=call(script_design_tick,0x34).word;
  s.timer34=signed_word(call(script_timer_start,tick34,0x34,owner34).word);
 }
 void init(bool active){
  timers();
  if(active){if(s.active){call(script_ais_init,0,0,s.active);call(script_ais_init_post,0,0,s.active);call(script_ais_init_final,0,0,s.active);}}
  else if(s.pending)call(script_ais_init,0,0,s.pending);
 }
 void cleanup(){
  call(script_timer_stop,std::uint32_t(s.timer33),0,s.owner);
  call(script_timer_stop,std::uint32_t(s.timer34),0,s.owner);
  s.timer33=s.timer34=-1;
  if(s.active){call(script_skill_cleanup);call(script_spell_cleanup);call(script_ais_terminate,0,0,s.active);}
 }
 int execute(std::uint32_t op,std::uint32_t arg){
  switch(op){
   case script_replace_iphone:replace();break;
   case script_load_process:load();break;
   case script_init_step:init_step();break;
   case script_init_process:init_process(arg);break;
   case script_load_and_init:if(s.active)return 0;load();if(!s.active)return 0;init_process(arg);return 1;
   case script_on_init:init(false);break;
   case script_ai_script_init:init(true);break;
   case script_cleanup:cleanup();break;
   case script_on_init_post:if(s.active)call(script_ais_init_post,0,0,s.active);break;
   case script_on_init_final:if(s.active)call(script_ais_init_final,0,0,s.active);break;
  }
  return 1;
 }
};
}
extern "C" int dh2_character_script_lifecycle(dh2::character::ScriptLifecycleState64* s,std::uint32_t op,std::uint32_t arg,const dh2::character::ScriptLifecycleServices16* c){
 using namespace dh2::character;
 if(!s||!c||overlaps(s,sizeof(*s),c,sizeof(*c))||!c->invoke||!s->owner||s->delayed>255||s->scripted>255||s->reserved0||s->reserved1||s->reserved2||op>script_on_init_final||arg>1||s->load_step<0)return -1;
 return Kernel{*s,*c}.execute(op,arg);
}
