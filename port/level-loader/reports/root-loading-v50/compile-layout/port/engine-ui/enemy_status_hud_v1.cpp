#include "enemy_status_hud_v1.hpp"
#include "renderfx_text_connection.hpp"
#include "gameswf/gameswf.h"
#include "gameswf/gameswf_sound.h"
#include "gameswf/gameswf_sprite.h"
#include <cstring>
#include <algorithm>
namespace dh2::ui {
namespace {constexpr const char* sha="a4ffacd1abdf7c9b2ba19c46ebb81c60c100458731a4cdba5880391b9c11b238";}
EnemyStatusHudV1::EnemyStatusHudV1(SwfMovie& movie,EnemyHudTextServicesV1 text):movie_(movie),text_(text){}
EnemyStatusHudV1::~EnemyStatusHudV1()=default;
bool EnemyStatusHudV1::notify(void* p,gameswf::sprite_instance* s,std::string& e){return static_cast<EnemyStatusHudV1*>(p)->advance_.notify(s,e);}
bool EnemyStatusHudV1::sound(void*,std::uintptr_t& out,std::string&){out=reinterpret_cast<std::uintptr_t>(gameswf::get_sound_handler());return true;}
bool EnemyStatusHudV1::pause(void*,std::uintptr_t identity,std::int32_t id,bool paused,std::string& error){
 auto* handler=gameswf::get_sound_handler();
 if(!handler||reinterpret_cast<std::uintptr_t>(handler)!=identity){error="Enemy HUD sound owner changed";return false;}
 handler->pause(id,paused);return true;
}
int EnemyStatusHudV1::text_operation(void* p,const HudManagerRequest& q,HudManagerResponse&,std::string& error){
 auto& self=*static_cast<EnemyStatusHudV1*>(p);
 if(q.operation!=HudManagerOperation::text||!self.graph_){error="Enemy HUD text graph unavailable";return 0;}
 SwfAsValue value;
 if(!self.graph_->retain_object(reinterpret_cast<gameswf::as_object*>(q.subject),value,error)||
    !renderfx_set_plain_text(*self.graph_,value,q.text,q.value!=0,error))return 0;
 if(q.index==20)self.name_=q.text?q.text:"";
 if(q.index==21)self.level_=q.text?q.text:"";
 return 1;
}
int EnemyStatusHudV1::service(void* p,HudManagerState* state,const HudManagerRequest* q,HudManagerResponse* out){
 auto& self=*static_cast<EnemyStatusHudV1*>(p);
 if(!self.borrow_||!self.error_||!q||!out)return 0;
 const int core=self.core_.dispatch(*q,*out,*self.error_);
 if(core>=0){
  if(core==1&&q->operation==HudManagerOperation::visible)self.visible_=q->value!=0;
  if(core==1&&q->operation==HudManagerOperation::goto_frame&&q->index==22){
   // Source ignores invalid frames; retain the actual timeline result.
   auto* clip=reinterpret_cast<gameswf::character*>(q->subject);
   if(!clip||!clip->is(gameswf::sprite_instance::m_class_id)){*self.error_="Enemy HUD timeline receiver changed";return 0;}
   self.hp_frame_=static_cast<gameswf::sprite_instance*>(clip)->m_current_frame;
  }
  return core;
 }
 if(q->operation==HudManagerOperation::string_symbol){
  if(!self.text_.integer_string){*self.error_="Enemy HUD original string manager unavailable";return 0;}
  bool is_null=false;
  if(!self.text_.integer_string(self.text_.context,q->value,self.localized_,is_null,*self.error_))return 0;
  out->text=is_null?nullptr:self.localized_.c_str();return 1;
 }
 if(!self.borrow_->services.invoke){*self.error_="Enemy HUD world query provider unavailable";return 0;}
 return self.borrow_->services.invoke(self.borrow_->services.context,state,q,out);
}
bool EnemyStatusHudV1::apply(void* p,SwfAsGraph& graph,std::string& error){
 auto& self=*static_cast<EnemyStatusHudV1*>(p);self.graph_=&graph;self.error_=&error;
 if(!self.bound_){
  SwfAsValue value;gameswf::as_object* root=nullptr;
  if(!graph.root_value(value,error)||!graph.borrow_object(value,root,error))return false;
  if(!root||!root->is(gameswf::character::m_class_id)){error="Enemy HUD actual movie root unavailable";return false;}
  if(!self.core_.bind(static_cast<gameswf::character*>(root),sha,{&self,notify,sound,pause},error))return false;
  self.core_.required_operations(&self,text_operation);
  HudManagerResponse response;
   const auto menu="_root.menu_HUD_"+std::to_string(self.hud_style_);
   HudManagerRequest root_query{HudManagerOperation::root_lookup,0,0,0,0,0,menu.c_str(),nullptr,{0,0,0},0};
   if(self.core_.dispatch(root_query,response,error)!=1||!response.identity){error="Enemy HUD selected authored root unavailable";return false;}
  const auto base=response.identity;
  for(unsigned i=19;i<=22;++i){
    HudManagerRequest init{HudManagerOperation::cache_initialize,i,0,0,base,self.state_.render_fx,hud_manager_cache_path(i,self.hud_style_),nullptr,{0,0,0},0};
   if(self.core_.dispatch(init,response,error)!=1)return false;
  }
  self.bound_=true;
 }
 const HudManagerServices services{&self,service};
 const auto status=dh2_ui_hud_enemy_v1(&self.state_,self.borrow_->player,&services);
 if(status){if(error.empty())error="Original enemy HUD update failed: "+std::to_string(status);return false;}
 self.target_=self.state_.cached_target?self.state_.cached_target->identity:0;
 if(self.borrow_->presentation.target){SwfAsValue value;gameswf::as_object* object{};if(!graph.root_value(value,error)||!graph.borrow_object(value,object,error)||!object||!object->is(gameswf::sprite_instance::m_class_id)){error="Required live enemy viewport root";return false;}const auto* root=static_cast<gameswf::sprite_instance*>(object)->get_root();if(!root){error="Required live enemy viewport";return false;}self.viewport_pixels_[0]=float(root->m_viewport_x0);self.viewport_pixels_[1]=float(root->m_viewport_y0);self.viewport_pixels_[2]=float(root->m_viewport_x0)+float(root->m_viewport_width);self.viewport_pixels_[3]=float(root->m_viewport_y0)+float(root->m_viewport_height);}
 return true;
}
bool EnemyStatusHudV1::update(const EnemyHudWorldBorrowV1& borrow,std::string& error,std::int32_t hud_style){
 if(!borrow.world||!borrow.player||!borrow.services.invoke){error="Enemy HUD requires retained world/player/services";return false;}
  if(borrow_){error="Enemy HUD synchronous borrow reentered";return false;}
  if(hud_style<0||hud_style>3){error="Enemy HUD requires actual authored HUDStyle";return false;}
  if(hud_style_!=hud_style){bound_=false;state_.cached_target=nullptr;target_=0;hud_style_=hud_style;}
 if(world_!=borrow.world){
  // The source manager's initialization clears its previous target. Do not
  // dereference a character projection from a world that has been released.
  state_.cached_target=nullptr;world_=borrow.world;target_=0;
 }
 state_.render_fx=reinterpret_cast<std::uintptr_t>(&movie_);borrow_=&borrow;
 bool ok=movie_.action_script(this,apply,error);
 if(ok&&borrow.presentation.target){
  presentation_={};
  if(target_){
   ok=borrow.presentation.target(borrow.presentation.context,target_,presentation_,error);
   if(ok&&presentation_.identity!=target_){error="Enemy HUD presentation target identity differs from source selection";ok=false;}
   if(ok&&!presentation_.dead&&presentation_.raw_hp>0&&presentation_.raw_max_hp>0&&presentation_.on_screen){
    root_anchor_[0]=presentation_.screen_pixels[0];root_anchor_[1]=presentation_.screen_pixels[1];
    ok=movie_.screen_to_logical(root_anchor_,error);
    if(ok){root_anchor_[0]*=20.f;root_anchor_[1]*=20.f;float first[2]{viewport_pixels_[0],viewport_pixels_[1]},last[2]{viewport_pixels_[2],viewport_pixels_[3]};ok=movie_.screen_to_logical(first,error)&&movie_.screen_to_logical(last,error);if(ok){visible_root_[0]=std::min(first[0],last[0])*20.f;visible_root_[1]=std::max(first[0],last[0])*20.f;visible_root_[2]=std::min(first[1],last[1])*20.f;visible_root_[3]=std::max(first[1],last[1])*20.f;}}
   }
  }
  if(ok)ok=movie_.action_script(this,present,error);
 }
 borrow_=nullptr;graph_=nullptr;error_=nullptr;
 return ok;
}
bool EnemyStatusHudV1::present(void* p,SwfAsGraph&,std::string& error){
 auto& self=*static_cast<EnemyStatusHudV1*>(p);HudManagerResponse out;
 HudManagerRequest get{HudManagerOperation::cache_get,19,0,0,0,0,nullptr,nullptr,{0,0,0},0};
 if(self.core_.dispatch(get,out,error)!=1||!out.identity){error="Required authored enemy HUD clip";return false;}
 auto* clip=reinterpret_cast<gameswf::character*>(out.identity);
 const bool alive=self.target_&&!self.presentation_.dead&&self.presentation_.raw_hp>0&&self.presentation_.raw_max_hp>0&&self.presentation_.on_screen;
 if(!alive){clip->set_visible(false);self.visible_=false;self.presentation_hidden_=true;return true;}
 if(self.presentation_hidden_){HudManagerRequest show{HudManagerOperation::goto_label,19,0,0,out.identity,self.state_.render_fx,"Show",nullptr,{0,0,0},0};if(self.core_.dispatch(show,out,error)!=1)return false;self.presentation_hidden_=false;}
 clip->set_visible(true);self.visible_=true;
 get.index=22;if(self.core_.dispatch(get,out,error)!=1||!out.identity)return false;
 const auto frame=enemy_hud_hp_frame_v2(self.presentation_.raw_hp,self.presentation_.raw_max_hp);
 HudManagerRequest go{HudManagerOperation::goto_frame,22,frame,0,out.identity,self.state_.render_fx,nullptr,nullptr,{0,0,0},0};
 if(self.core_.dispatch(go,out,error)!=1)return false;
 auto* bar=reinterpret_cast<gameswf::sprite_instance*>(go.subject);self.hp_frame_=bar->m_current_frame;return enemy_hud_anchor_v2(clip,self.root_anchor_,error,self.visible_root_);
}
}
