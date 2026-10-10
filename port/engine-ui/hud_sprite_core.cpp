#include "hud_sprite_core.hpp"
#include "hud_sprite_timeline.hpp"
#include "gameswf/gameswf_sprite.h"
#include <array>
#include <cstring>
namespace dh2::ui {
namespace {
constexpr const char* hud_sha="a4ffacd1abdf7c9b2ba19c46ebb81c60c100458731a4cdba5880391b9c11b238";
constexpr const char* base_hud_sha="3ab455733367acf657df6eaaec39c93fed34cbcf519255e02cf488c07a96f2bb";
int frames(int id,unsigned version){
 if(version==2){switch(id){case 96:case 151:return 100;case 156:return 101;case 119:case 37:return 103;default:return 0;}}
 if(version==1){switch(id){case 90:case 147:return 100;case 152:return 101;case 113:case 31:return 103;default:return 0;}}
 return 0;
}
bool bound(const HudSpriteCoreBindingV1& b){return (b.version==1||b.version==2)&&b.sprite&&b.definition&&b.root&&b.sprite->m_def.get_ptr()==b.definition&&b.sprite->m_root==b.root&&b.sprite->get_id()==b.id&&b.sprite->get_frame_count()==b.frames&&frames(b.id,b.version)==b.frames;}
struct Context {
 gameswf::sprite_instance* sprite;const HudSpriteCoreServices* services;std::string* error;
 bool base_hud{};
 std::array<std::uintptr_t,4096> pending{},queued{};
 unsigned capacity()const{return base_hud?4096u:16u;}
 bool pull(HudSpriteState64& s){const auto& p=sprite->m_action_list;const auto& q=sprite->m_goto_frame_action_list;if(unsigned(p.size())>capacity()||unsigned(q.size())>capacity()){*error="HUD action history exceeds supported storage";return false;}s.current_frame=sprite->m_current_frame;s.play_state=int(sprite->m_play_state);s.stream_sound_id=sprite->m_def->m_ss_id;s.pending.count=unsigned(p.size());s.goto_actions.count=unsigned(q.size());for(int i=0;i<p.size();++i)pending[unsigned(i)]=reinterpret_cast<std::uintptr_t>(p[i]);for(int i=0;i<q.size();++i)queued[unsigned(i)]=reinterpret_cast<std::uintptr_t>(q[i]);return true;}
 void push(const HudSpriteState64& s){sprite->m_current_frame=s.current_frame;sprite->m_play_state=gameswf::character::play_state(s.play_state);sprite->m_action_list.resize(int(s.pending.count));sprite->m_goto_frame_action_list.resize(int(s.goto_actions.count));for(unsigned i=0;i<s.pending.count;++i)sprite->m_action_list[int(i)]=reinterpret_cast<gameswf::action_buffer*>(pending[i]);for(unsigned i=0;i<s.goto_actions.count;++i)sprite->m_goto_frame_action_list[int(i)]=reinterpret_cast<gameswf::action_buffer*>(queued[i]);}
 static int invoke(void* ptr,HudSpriteState64* s,const HudSpriteRequest32* r,HudSpriteResponse16* out){auto& c=*static_cast<Context*>(ptr);c.push(*s);bool delivered=true;
  switch(r->operation){case HudSpriteOperation::frame_count:out->value=c.sprite->get_frame_count();break;
  case HudSpriteOperation::forward_tags:
   if(r->frame<0||r->frame>=c.sprite->get_frame_count()){*c.error="HUD frame tag index outside authored range";return 0;}
   c.sprite->execute_frame_tags(r->frame,r->state_only!=0);if(!c.base_hud&&c.sprite->m_action_list.size()!=0){*c.error="Unsupported HUD frame queued ActionScript";return 0;}break;
  case HudSpriteOperation::reverse_tags:
   if(r->frame<0||r->frame>=c.sprite->get_frame_count()){*c.error="HUD reverse tag index outside authored range";return 0;}
   c.sprite->execute_frame_tags_reverse(r->frame);if(!c.base_hud&&c.sprite->m_action_list.size()!=0){*c.error="Unsupported HUD reverse queued ActionScript";return 0;}break;
  case HudSpriteOperation::notify_advance:
   if(!c.services->notify_advance){*c.error="HUD source advance owner unavailable";return 0;}delivered=c.services->notify_advance(c.services->context,c.sprite,*c.error);break;
  case HudSpriteOperation::sound_handler:
   if(!c.services->sound_handler){*c.error="HUD source sound lookup unavailable";return 0;}delivered=c.services->sound_handler(c.services->context,out->identity,*c.error);break;
  case HudSpriteOperation::pause_sound:
   if(!c.services->pause_sound){*c.error="HUD source stream pause unavailable";return 0;}delivered=c.services->pause_sound(c.services->context,r->sound,r->frame,r->state_only!=0,*c.error);break;
  }
  return delivered&&c.pull(*s)?1:0;
 }
};
int apply(const HudSpriteCoreBindingV1& b,int value,const HudSpriteCoreServices& services,std::string& error,bool go){if(!bound(b)){error="HUD sprite binding/version mismatch";return -1;}
 // Keep the concrete clip alive through synchronous tag/provider callbacks.
 gameswf::gc_ptr<gameswf::sprite_instance> pin=b.sprite;
 if(go&&b.version==1&&(b.sprite->m_action_list.size()!=0||b.sprite->m_goto_frame_action_list.size()!=0)){error="Unsupported HUD pending ActionScript history";return -2;}
 if(go&&b.sprite->m_def->m_ss_id>=0){error="Unsupported HUD frame stream sound";return -2;}
 try{Context ctx{b.sprite,&services,&error,b.version==2};HudSpriteState64 s{0,0,-1,0,reinterpret_cast<std::uintptr_t>(b.sprite),reinterpret_cast<std::uintptr_t>(b.definition),{ctx.pending.data(),0,ctx.capacity()},{ctx.queued.data(),0,ctx.capacity()}};if(!ctx.pull(s))return -2;HudSpriteServices16 svc{&ctx,Context::invoke};const int result=go?dh2_ui_hud_sprite_goto_v1(&s,value,&svc):dh2_ui_hud_sprite_play_v1(&s,value,&svc);if(result>=0)ctx.push(s);if(result<0&&error.empty())error="HUD sprite source delivery failed";return result;}catch(const std::exception& ex){error=ex.what();return -2;}
}
}
bool bind_hud_sprite_v1(gameswf::character* c,const char* hash,HudSpriteCoreBindingV1& out,std::string& error){const unsigned version=hash&&!std::strcmp(hash,hud_sha)?1u:hash&&!std::strcmp(hash,base_hud_sha)?2u:0u;if(!c||!version||!c->is(gameswf::sprite_instance::m_class_id)){error="Unsupported HUD source/clip binding";return false;}auto* s=static_cast<gameswf::sprite_instance*>(c);const auto count=frames(s->get_id(),version);if(!count||s->get_frame_count()!=count||!s->m_def||!s->m_root){error="Unsupported HUD authored timeline";return false;}out={s,s->m_def.get_ptr(),s->m_root,s->get_id(),count,version};return true;}
int hud_core_goto_v1(const HudSpriteCoreBindingV1& b,std::int32_t frame,const HudSpriteCoreServices& services,std::string& error){return apply(b,frame,services,error,true);}
int hud_core_play_v1(const HudSpriteCoreBindingV1& b,std::int32_t play,const HudSpriteCoreServices& services,std::string& error){return apply(b,play,services,error,false);}
}
