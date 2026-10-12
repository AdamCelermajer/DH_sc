#include "character_pre_spawn.hpp"
#include <cstring>
namespace {
using namespace dh2::character;
bool valid(const PreSpawnState48* s,const PreSpawnServices16* c){return s&&s->state&&s->state->current==17&&s->character&&s->animation_rows&&s->animation_count&&!s->reserved&&s->stay_enabled<=255&&c&&c->invoke;}
int send(PreSpawnState48* s,const PreSpawnServices16* c,std::uintptr_t character,PreSpawnService service,std::uint32_t a=0,std::uint32_t b=0,std::uintptr_t payload=0,PreSpawnResponse8* out=nullptr){PreSpawnResponse8 ignored{};const PreSpawnRequest32 r{service,a,b,0,character,payload};return c->invoke(c->context,s,&r,out?out:&ignored)?-2:1;}
int query(PreSpawnState48* s,const PreSpawnServices16* c,std::uintptr_t character,PreSpawnService service,std::uint32_t& word){PreSpawnResponse8 response{};const auto r=send(s,c,character,service,0,0,0,&response);word=response.word;return r;}
}
extern "C" int dh2_character_pre_spawn_body(PreSpawnState48* s,std::uint32_t op,std::uint32_t event,std::uintptr_t payload,const PreSpawnServices16* c){
 if(!valid(s,c)||op>3)return -1;
 const auto character=s->character;const auto state=s->state;
 if(op==state_method_update)return dh2_character_state_empty_body(17,state_method_update,0x3c0070);
 if(op==state_method_focus){
  state->flags=0x1300;
  const auto initial_rows=s->animation_rows;const auto initial_count=s->animation_count;std::uint32_t index=0;auto result=query(s,c,character,pre_spawn_animation_index,index);if(result<0)return result;if(index>=initial_count)return -3;
  const bool has_clip=initial_rows[index][0x64/4]!=-1;
  // Source reloads rows BEFORE a second live index query on both branches.
  const auto rows=s->animation_rows;const auto count=s->animation_count;
  result=query(s,c,character,pre_spawn_animation_index,index);if(result<0)return result;if(!rows||index>=count)return -3;
  const auto base=static_cast<std::uint32_t>(rows[index][has_clip?0x64/4:0x80/4]);
  std::uint32_t sequence=base;
  if(!has_clip){std::uint32_t mask=0;result=query(s,c,character,pre_spawn_stance_mask,mask);if(result<0)return result;if(mask&1){std::uint32_t stance=0;result=query(s,c,character,pre_spawn_stance,stance);if(result<0)return result;sequence+=stance;}}
  result=send(s,c,character,pre_spawn_set_animation,sequence);if(result<0)return result;
  if(!has_clip){result=send(s,c,character,pre_spawn_set_speed,0);if(result<0)return result;}
  result=send(s,c,character,pre_spawn_remove_physical);if(result<0)return result;
  if(!s->stay_enabled){result=send(s,c,character,pre_spawn_enable,0);if(result<0)return result;}
  return send(s,c,character,pre_spawn_disable_collisions);
 }
 if(op==state_method_blur){auto result=send(s,c,character,pre_spawn_enable,1);if(result<0)return result;result=send(s,c,character,pre_spawn_revive);if(result<0)return result;return send(s,c,character,pre_spawn_enable_collisions);}
 if(event==0x28){if(!payload)return -3;if(std::strcmp(reinterpret_cast<const char*>(payload),"is_interactive"))return 1;state->flags|=0x2000;return send(s,c,character,pre_spawn_init_physical);}
 if(event!=9)return 1;
 PreSpawnResponse8 response{0,1};auto result=send(s,c,character,pre_spawn_predicate,event,17,payload,&response);if(result<0)return result;if(!response.word)return 1;
 if(response.next!=1){std::uint32_t policy=0;result=query(s,c,character,pre_spawn_assert_policy,policy);if(result<0)return result;if(policy==2)return -3;if(policy==1)return send(s,c,character,pre_spawn_assert_log,114);return 1;}
 result=send(s,c,character,pre_spawn_set_spawn,1,0);if(result<0)return result;
 if(s->ai_kind==3){if(!s->ai_word)return -3;*s->ai_word=0;}
 return 1;
}
