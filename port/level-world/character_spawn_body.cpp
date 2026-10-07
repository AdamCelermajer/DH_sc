#include "character_spawn_body.hpp"
#include <cstring>
namespace {
using namespace dh2::character;
bool valid(const SpawnBody48* s,const SpawnBodyServices16* c){return s&&s->state&&s->state->current==1&&s->character&&s->animation_rows&&s->animation_count&&!s->reserved[0]&&!s->reserved[1]&&c&&c->invoke;}
int send(SpawnBody48* s,const SpawnBodyServices16* c,std::uintptr_t character,SpawnBodyService service,std::uint32_t argument=0,std::uintptr_t payload=0,std::uint32_t* out=nullptr){std::uint32_t ignored=0;const SpawnBodyRequest32 r{service,argument,0,0,character,payload};return c->invoke(c->context,s,&r,out?out:&ignored)?-2:1;}
int debug(SpawnBody48* s,const SpawnBodyServices16* c,std::uintptr_t character,std::uint32_t query){const auto r=send(s,c,character,spawn_debug_load);return r<0?r:send(s,c,character,spawn_debug_query,query);}
}
extern "C" int dh2_character_spawn_body(SpawnBody48* s,std::uint32_t op,std::int32_t previous,std::uint32_t event,std::uintptr_t payload,const SpawnBodyServices16* c){
 if(!valid(s,c)||op>3)return -1;
 const auto character=s->character;const auto state=s->state;
 if(op==state_method_update)return dh2_character_state_empty_body(1,state_method_update,0x3bfff0);
 if(op==state_method_event){if(event!=0x28)return 1;if(!payload)return -3;if(std::strcmp(reinterpret_cast<const char*>(payload),"is_interactive"))return 1;state->flags|=0x2000;return send(s,c,character,spawn_init_physical);}
 auto result=debug(s,c,character,0);if(result<0)return result;
 if(op==state_method_blur)return state->flags&0x2000?1:send(s,c,character,spawn_init_physical);
 result=debug(s,c,character,1);if(result<0)return result;
 const bool carried=previous==17&&(state->flags&0x2000);
 state->flags=0x241;
 const auto rows=s->animation_rows;const auto count=s->animation_count;
 std::uint32_t index=0;result=send(s,c,character,spawn_animation_index,0,0,&index);if(result<0)return result;if(!rows||index>=count)return -3;
 const auto base=static_cast<std::uint32_t>(rows[index][32]);std::uint32_t mask=0,stance=0;
 result=send(s,c,character,spawn_stance_mask,0,0,&mask);if(result<0)return result;
 if(mask&1){result=send(s,c,character,spawn_stance,0,0,&stance);if(result<0)return result;}
 result=send(s,c,character,spawn_set_animation,base+stance);if(result<0)return result;
 result=send(s,c,character,spawn_set_target);if(result<0)return result;
 result=send(s,c,character,spawn_sync_last_target);if(result<0)return result;
 result=send(s,c,character,spawn_cancel_sneaking);if(result<0)return result;
 if(carried)state->flags|=0x2000;
 const auto visual=s->visual;if(visual)return send(s,c,character,spawn_fade_in,s->fade_word,visual);
 return 1;
}
extern "C" int dh2_character_spawn_fade_in_empty(){return 1;}
