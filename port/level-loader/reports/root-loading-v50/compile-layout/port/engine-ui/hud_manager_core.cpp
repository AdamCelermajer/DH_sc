#include "hud_manager_core.hpp"
#include "hud_sprite_timeline.hpp"
#include "gameswf/gameswf_sprite.h"
#include "gameswf/gameswf_text.h"
#include "gameswf/gameswf_action.h"
#include "gameswf/gameswf_function.h"
#include <array>
#include <cstring>
#include <cmath>
namespace dh2::ui { namespace {
constexpr const char* hud_sha="a4ffacd1abdf7c9b2ba19c46ebb81c60c100458731a4cdba5880391b9c11b238";
std::int32_t integer(double f){if(std::isnan(f))return 0;if(f>=2147483648.)return INT32_MAX;if(f<=-2147483648.)return INT32_MIN;return static_cast<std::int32_t>(f);}
struct Timeline {
 gameswf::sprite_instance* sprite;const HudSpriteCoreServices* services;std::string* error;
 std::array<std::uintptr_t,4096> pending{},queued{};
 bool pull(HudSpriteState64&s){auto&p=sprite->m_action_list;auto&q=sprite->m_goto_frame_action_list;if(p.size()>4096||q.size()>4096){*error="HUD pending actions exceed owned bounded storage";return false;}s.current_frame=sprite->m_current_frame;s.play_state=int(sprite->m_play_state);s.stream_sound_id=sprite->m_def->m_ss_id;s.pending.count=p.size();s.goto_actions.count=q.size();for(int i=0;i<p.size();++i)pending[i]=reinterpret_cast<std::uintptr_t>(p[i]);for(int i=0;i<q.size();++i)queued[i]=reinterpret_cast<std::uintptr_t>(q[i]);return true;}
 void push(const HudSpriteState64&s){sprite->m_current_frame=s.current_frame;sprite->m_play_state=gameswf::character::play_state(s.play_state);sprite->m_action_list.resize(s.pending.count);sprite->m_goto_frame_action_list.resize(s.goto_actions.count);for(unsigned i=0;i<s.pending.count;++i)sprite->m_action_list[i]=reinterpret_cast<gameswf::action_buffer*>(pending[i]);for(unsigned i=0;i<s.goto_actions.count;++i)sprite->m_goto_frame_action_list[i]=reinterpret_cast<gameswf::action_buffer*>(queued[i]);}
 static int invoke(void*ctx,HudSpriteState64*s,const HudSpriteRequest32*q,HudSpriteResponse16*out){auto&t=*static_cast<Timeline*>(ctx);t.push(*s);bool ok=true;
  switch(q->operation){case HudSpriteOperation::frame_count:out->value=t.sprite->get_frame_count();break;
  case HudSpriteOperation::forward_tags:t.sprite->execute_frame_tags(q->frame,q->state_only!=0);break;
  case HudSpriteOperation::reverse_tags:t.sprite->execute_frame_tags_reverse(q->frame);break;
  case HudSpriteOperation::notify_advance:if(!t.services->notify_advance){*t.error="HUD required advance owner unavailable";return 0;}ok=t.services->notify_advance(t.services->context,t.sprite,*t.error);break;
  case HudSpriteOperation::sound_handler:if(!t.services->sound_handler){*t.error="HUD required sound lookup unavailable";return 0;}ok=t.services->sound_handler(t.services->context,out->identity,*t.error);break;
  case HudSpriteOperation::pause_sound:if(!t.services->pause_sound){*t.error="HUD required stream pause unavailable";return 0;}ok=t.services->pause_sound(t.services->context,q->sound,q->frame,q->state_only!=0,*t.error);break;}
  return ok&&t.pull(*s)?1:0;
 }
};
int timeline(gameswf::sprite_instance*s,int v,bool go,const HudSpriteCoreServices&services,std::string&error){
 gameswf::gc_ptr<gameswf::sprite_instance> pin=s;if(!s->m_def||!s->m_root){error="HUD retained sprite definition/root missing";return 0;}
 if(go&&s->m_def->m_ss_id>=0){error="HUD source frame stream owner unavailable";return 0;}
 Timeline t{s,&services,&error};HudSpriteState64 state{0,0,-1,0,reinterpret_cast<std::uintptr_t>(s),reinterpret_cast<std::uintptr_t>(s->m_def.get_ptr()),{t.pending.data(),0,4096},{t.queued.data(),0,4096}};if(!t.pull(state))return 0;
 HudSpriteServices16 svc{&t,Timeline::invoke};auto rc=go?dh2_ui_hud_sprite_goto_v1(&state,v,&svc):dh2_ui_hud_sprite_play_v1(&state,v,&svc);if(rc>=0)t.push(state);if(rc<0&&error.empty())error="HUD source timeline delivery failed";return rc>=0?1:0;
}
}
struct HudManagerCore::Impl {
 struct Cache {std::string path;weak_ptr<gameswf::character> value;weak_ptr<gameswf::character> base;std::uintptr_t fx=0;};
 weak_ptr<gameswf::character> root;std::array<Cache,29> caches;HudSpriteCoreServices timeline_services{};
 bool bound=false;
 void* required_context=nullptr;RequiredOperation required=nullptr;
 gameswf::character* find(gameswf::character* base,const char*p){if(!base||!p)return nullptr;auto*o=base->find_target(gameswf::as_value(p));if(!o||!o->is(gameswf::character::m_class_id))return nullptr;return static_cast<gameswf::character*>(o);}
};
HudManagerCore::HudManagerCore():impl_(new Impl){}HudManagerCore::~HudManagerCore()=default;
void HudManagerCore::required_operations(void*context,RequiredOperation required){impl_->required_context=context;impl_->required=required;}
bool HudManagerCore::bind(gameswf::character*root,const char*sha,const HudSpriteCoreServices&services,std::string&error){if(!root||!sha||std::strcmp(sha,hud_sha)||!root->is(gameswf::sprite_instance::m_class_id)){error="HUD manager verified root binding rejected";return false;}if(impl_->bound&&impl_->root.get_ptr()!=root){error="HUD manager owner cannot change retained roots";return false;}impl_->root=root;impl_->bound=true;impl_->timeline_services=services;return true;}
int HudManagerCore::dispatch(const HudManagerRequest&q,HudManagerResponse&out,std::string&error){
 auto*root=impl_->root.get_ptr();if(!root){error="HUD manager retained movie owner expired";return 0;}
 auto*character=reinterpret_cast<gameswf::character*>(q.subject);gameswf::gc_ptr<gameswf::character> pin;
 using Op=HudManagerOperation;
 switch(q.operation){
 case Op::root_lookup:out.identity=reinterpret_cast<std::uintptr_t>(impl_->find(root,q.text));return 1;
 case Op::root_character:out.identity=reinterpret_cast<std::uintptr_t>(root);return 1;
 case Op::cache_initialize:{if(q.index>=29||!q.text){error="HUD cache projection malformed";return 0;}auto&c=impl_->caches[q.index];c.value=impl_->find(character?character:root,q.text);c.path=q.text;c.fx=q.render_fx;c.base=character;return 1;}
 case Op::cache_get:{if(q.index>=29){error="HUD cache index malformed";return 0;}auto*c=impl_->caches[q.index].value.get_ptr();out.identity=reinterpret_cast<std::uintptr_t>(c);if(c){auto*parent=c->get_parent();unsigned depth=0;while(parent){if(++depth>65536){error="HUD cache parent topology unsupported";return 0;}parent=parent->get_parent();}}return 1;}
 case Op::visible:if(!character){error="HUD source unchecked visibility target missing";return 0;}pin=character;character->set_visible(q.value!=0);return 1;
 case Op::sprite_type:out.value=character&&character->is(gameswf::sprite_instance::m_class_id);return 1;
 case Op::goto_frame:case Op::play_state:if(!character||!character->is(gameswf::sprite_instance::m_class_id)){error="HUD sprite virtual projection malformed";return 0;}return timeline(static_cast<gameswf::sprite_instance*>(character),q.value,q.operation==Op::goto_frame,impl_->timeline_services,error);
 case Op::as_slot_id:{if(!character){error="HUD list button missing";return 0;}pin=character;gameswf::as_value value;character->get_member("SlotId",&value);out.value=integer(value.to_number());return 1;}
 case Op::text:if(!character||!character->is(gameswf::edit_text_character::m_class_id))return 1;pin=character;if(!impl_->required){error="HUD source text provider unavailable";return 0;}return impl_->required(impl_->required_context,q,out,error);
 case Op::goto_label:{if(!character||!character->is(gameswf::sprite_instance::m_class_id))return 1;if(!q.text){error="HUD label missing";return 0;}auto*s=static_cast<gameswf::sprite_instance*>(character);pin=s;int frame=0;if(!s->m_def->get_labeled_frame(q.text,&frame))return 1;if(!timeline(s,frame,true,impl_->timeline_services,error))return 0;return timeline(s,q.value^1,false,impl_->timeline_services,error);}
 case Op::allies_callback:{if(!character||!character->is(gameswf::sprite_instance::m_class_id)){error="HUD callback sprite/weak-wrapper provider missing";return 0;}if(!q.payload||!q.text){error="HUD callback arguments missing";return 0;}pin=character;gameswf::as_value method;if(!character->get_member(q.text,&method)||!method.to_object()||!method.to_object()->is(gameswf::as_function::m_class_id)){error="Required authored HUD callback unavailable";return 0;}auto&a=*static_cast<const HudManagerAllies*>(q.payload);gameswf::as_value args[6];args[0]=gameswf::as_value(a.present);args[1]=gameswf::as_value(a.name?a.name:"");args[2]=gameswf::as_value(a.level);args[3]=gameswf::as_value(a.hp_frame);args[4]=gameswf::as_value(a.index);args[5]=gameswf::as_value(a.death_seconds);static_cast<gameswf::sprite_instance*>(character)->call_method(q.text,args,6);return 1;}
 default:return -1;
 }
}
}
