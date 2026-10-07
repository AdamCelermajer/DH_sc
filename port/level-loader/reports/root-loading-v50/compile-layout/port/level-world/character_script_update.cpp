#include "character_script_update.hpp"
#include <cstddef>
namespace {
bool overlap(const void* a,std::size_t n,const void* b,std::size_t m){auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<=y?y-x<n:x-y<m;}
}
extern "C" int dh2_character_script_update(dh2::character::ScriptUpdateState48* s,std::uint32_t dispatch,const dh2::character::ScriptUpdateServices16* c){
 using namespace dh2::character;
 if(!s||!c||overlap(s,sizeof(*s),c,sizeof(*c))||!c->invoke||dispatch>1||!s->script_owner||!s->ai_owner||!s->controller||s->paused>255||s->reserved0||s->reserved1)return -1;
 if(dispatch&&!s->active)return 1;
 if(s->collision_ms<=199)return 1;
 s->collision_ms=0;
 s->paused=1;
 const ScriptUpdateRequest32 start{script_update_timer_start,1000,0,0x31,s->ai_owner,0};
 c->invoke(c->context,s,&start);
 const ScriptUpdateRequest32 stop{script_update_controller_stop,0,0,0,s->controller,0};
 c->invoke(c->context,s,&stop);
 return 1;
}
extern "C" int dh2_character_script_pause_expired(dh2::character::ScriptUpdateState48* s){
 if(!s||!s->script_owner||!s->ai_owner||!s->controller||s->paused>255||s->reserved0||s->reserved1)return -1;
 s->paused=0;
 return 1;
}
