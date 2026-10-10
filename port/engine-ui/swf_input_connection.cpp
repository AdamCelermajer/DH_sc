#include "swf_input_connection.hpp"
#include "swf_controller_storage_v91.hpp"
#include "swf_input_geometry.hpp"
#include "swf_event_core.hpp"
#include "gameswf/gameswf_root.h"
#include "gameswf/gameswf_sprite.h"
#include "gameswf/gameswf_button.h"
#include "gameswf/gameswf_text.h"
#include <map>
#include <vector>
#include <cstring>
#include <cmath>
#include <exception>
#include <stdexcept>
namespace dh2::ui {
namespace {
gameswf::character* character(std::uintptr_t p){return reinterpret_cast<gameswf::character*>(p);}
std::uintptr_t identity(gameswf::character*p){return reinterpret_cast<std::uintptr_t>(p);}
float mul(float a,float b){volatile float x=a*b;return x;}
std::int32_t trunc32(float a){if(std::isnan(a))return 0;if(a>=2147483648.f)return INT32_MAX;if(a<=-2147483648.f)return INT32_MIN;return static_cast<std::int32_t>(a);}
void matrix_words(const gameswf::matrix&m,float out[6]){for(unsigned j=0;j<6;++j)out[j]=m.m_[j/3][j%3];}
}
struct SwfInputConnection::State {
 SwfViewportLease lease;std::shared_ptr<SwfViewportConnection> viewport;std::shared_ptr<SwfInputHistory> history;SwfInputCoreServices services;
 gameswf::gc_ptr<gameswf::character> context_pin;
 std::shared_ptr<SwfControllerStorageV91> controllers;
 SwfInputState288& input;
 explicit State(std::shared_ptr<SwfControllerStorageV91> c):controllers(c?std::move(c):std::make_shared<SwfControllerStorageV91>()),input(controllers->fields()){}
 std::uint32_t*selection{};SwfInputServices16 flat{};std::string error;
 std::map<gameswf::character*,SwfInputCharacter32> projections;
 struct Scene {weak_ptr<gameswf::character> owner;std::uintptr_t identity{};};
 std::map<gameswf::character*,Scene> scenes;
 void retain(gameswf::character*c){controllers->retain(c);}
 void drop(gameswf::character*c){controllers->drop(c);}
 std::vector<std::uintptr_t> buttons;float raw_xy[2]{};std::int32_t raw_index{};
 // Shared facade constructor storage owns/drops the five slot refs.
 SwfInputHistoryFlags flags(gameswf::character*c){SwfInputHistoryFlags out{};if(!history->read(c,out,error))throw std::runtime_error(error);return out;}
 bool virtual_mouse(gameswf::character*c){if(!c)return false;if(c->is(gameswf::sprite_instance::m_class_id)){
   auto*s=static_cast<gameswf::sprite_instance*>(c);if(!s->is_enabled())return false;const auto f=flags(c);if(f.mouse9c)return true;return std::strstr(c->get_name().c_str(),"btn")?s->m_enabled:false;
  }return c->can_handle_mouse_event();
 }
 gameswf::character* topmost(gameswf::character*c,float x,float y){if(!c)return nullptr;
  if(c->is(gameswf::sprite_instance::m_class_id)){
   if(!c->get_visible())return nullptr;auto*s=static_cast<gameswf::sprite_instance*>(c);float point[2]{x,y};
   auto binding=scenes.find(c);if(binding!=scenes.end()&&binding->second.owner.get_ptr()==c&&binding->second.identity){if(!services.scene_local_mouse||!services.scene_local_mouse(services.context,binding->second.identity,c,point,error))throw std::runtime_error(error.empty()?"Required source scene-node local mouse service unavailable":error);}
   float m[6],local[2];matrix_words(c->get_matrix(),m);if(dh2_ui_swf_inverse_point(local,m,point))throw std::runtime_error("Malformed source hit matrix");
   const int n=s->m_display_list.size();gameswf::character*last=nullptr;bool found=false;
   for(int j=n-1;j>=0;--j){auto*child=s->m_display_list.get_character(j);if(!child||!child->get_visible())continue;
    last=topmost(child,local[0],local[1]);if(last){if(virtual_mouse(last))return virtual_mouse(c)?c:last;found=true;}
    if(!std::strcmp(child->get_name().c_str(),"hitzone"))break;
   }
   return found&&virtual_mouse(c)?c:last;
  }
  if((c->is(gameswf::edit_text_character::m_class_id)||dynamic_cast<gameswf::button_character_definition*>(c->get_character_def()))&&!c->get_visible())return nullptr;
  float m[6],point[2]{x,y},local[2];matrix_words(c->get_matrix(),m);if(dh2_ui_swf_inverse_point(local,m,point))throw std::runtime_error("Malformed source hit matrix");
  if(c->is(gameswf::edit_text_character::m_class_id)){
   if(!c->get_visible())return nullptr;auto*d=static_cast<gameswf::edit_text_character*>(c)->m_def.get_ptr();if(!d)throw std::runtime_error("Required source edit-text definition unavailable");const auto&r=d->m_rect;
   if(local[0]<r.m_x_min||local[0]>r.m_x_max||local[1]<r.m_y_min||local[1]>r.m_y_max)return nullptr;return c;
  }
  auto*d=c->get_character_def();if(!d)throw std::runtime_error("Required source character definition unavailable");
  if(auto*b=dynamic_cast<gameswf::button_character_definition*>(d)){
   if(!c->get_visible())return nullptr;for(int j=0;j<b->m_button_records.size();++j){auto&record=b->m_button_records[j];if(record.m_character_id<0||!record.m_hit_test)continue;
    float rm[6],rp[2];matrix_words(record.m_button_matrix,rm);if(dh2_ui_swf_inverse_point(rp,rm,local))throw std::runtime_error("Malformed source button matrix");
    if(!record.m_character_def)throw std::runtime_error("Required source button-record shape unavailable");if(record.m_character_def->point_test_local(rp[0],rp[1]))return c;
   }return nullptr;
  }
  return d->point_test_local(local[0],local[1])?c:nullptr;
 }
 void collect(gameswf::character*c,const char*name,int mask){if(!c)throw std::runtime_error("Required source collection context unavailable");const bool visible=(mask&1)?c->get_visible():true;const bool sprite=c->is(gameswf::sprite_instance::m_class_id);
  if(sprite&&(mask&2)&&!static_cast<gameswf::sprite_instance*>(c)->m_enabled)return;if(!visible)return;
  const auto*actual=c->get_name().c_str();if((!name||std::strstr(actual,name))&&(!(mask&4)||*actual))buttons.push_back(identity(c));
  if(sprite){auto*s=static_cast<gameswf::sprite_instance*>(c);for(int j=0;j<s->m_display_list.size();++j)collect(s->m_display_list.get_character(j),name,mask);}
 }
 static int invoke(void*p,SwfInputState288*,const SwfInputRequest64*q,SwfInputResponse56*out){auto&s=*static_cast<State*>(p);try{return s.request(*q,*out)?1:0;}catch(const std::exception&e){s.error=e.what();return 0;}catch(...){s.error="Native source input provider failed";return 0;}}
 bool request(const SwfInputRequest64&q,SwfInputResponse56&out){auto*c=character(q.character);
  switch(q.operation){
   case SwfInputOperation::retain:if(!c){error="Null source strong reference";return false;}retain(c);return true;
   case SwfInputOperation::drop:if(!c){error="Null source strong reference";return false;}drop(c);return true;
   case SwfInputOperation::character:{if(!c){error="Required source character unavailable";return false;}auto&v=projections[c];v.name=c->get_name().c_str();if(q.integer&2){v.is_sprite=c->is(gameswf::sprite_instance::m_class_id);v.sprite_ea=v.is_sprite?static_cast<gameswf::sprite_instance*>(c)->m_enabled:0;}if(q.integer&4)v.mouse9c=flags(c).mouse9c;v.visible=c->get_visible();out.character=&v;return true;}
   case SwfInputOperation::world_matrix:if(!c)return false;matrix_words(c->get_world_matrix(),out.values);return true;
   case SwfInputOperation::local_position:{if(!c)return false;float m[6],point[2]{mul(q.values[0],20.f),mul(q.values[1],20.f)};matrix_words(c->get_world_matrix(),m);return dh2_ui_swf_inverse_point(out.values,m,point)==0;}
   case SwfInputOperation::publish_raw_cursor:raw_xy[0]=q.values[0];raw_xy[1]=q.values[1];raw_index=static_cast<std::int32_t>(q.index);return true;
   case SwfInputOperation::screen_to_logical:out.values[0]=q.values[0];out.values[1]=q.values[1];return viewport->screen_to_logical(out.values,error);
   case SwfInputOperation::notify_mouse_state:return viewport->notify_mouse_state(trunc32(q.values[0]),trunc32(q.values[1]),q.integer,error);
   case SwfInputOperation::root_movie:out.identity=identity(lease.root->get_root_movie());return true;
   case SwfInputOperation::topmost:out.identity=identity(topmost(c,q.values[0],q.values[1]));return true;
   case SwfInputOperation::set_matrix:{if(!c)return false;gameswf::matrix m;for(unsigned j=0;j<6;++j)m.m_[j/3][j%3]=q.values[j];c->set_matrix(m);return true;}
   case SwfInputOperation::collect_buttons:buttons.clear();collect(c,q.name,q.integer);out.characters=buttons.data();out.count=static_cast<std::int32_t>(buttons.size());return true;
   case SwfInputOperation::play_animation:out.result=0;if(!c||!c->is(gameswf::sprite_instance::m_class_id))return true;if(!q.name)return false;if(c->goto_labeled_frame(q.name)){c->set_play_state(gameswf::character::PLAY);out.result=1;}return true;
   case SwfInputOperation::can_handle_event:{if(!q.event){error="Required native CanHandleEvent value";return false;}bool accepted=false;if(services.source_can_handle_event_v68){if(!services.source_can_handle_event_v68(*q.event,accepted,error))return false;}else {if(!services.can_handle_event){error="Required native CanHandleEvent receiver unavailable";return false;}if(!services.can_handle_event(services.context,*q.event,accepted,error))return false;}out.result=accepted;return true;}
   case SwfInputOperation::native_event:if(!q.event){error="Required native event value";return false;}if(services.source_native_event_v68)return services.source_native_event_v68(*q.event,error);if(!services.native_event){error="Required native event receiver unavailable";return false;}return services.native_event(services.context,*q.event,error);
   case SwfInputOperation::as_method:{bool invoked=false;return swf_event_method(lease,c,q.name,invoked,error);}
   case SwfInputOperation::play_state:if(!c)return false;out.result=c->get_play_state();return true;
   case SwfInputOperation::advance:if(!services.advance){error="Required original root/sprite advance backend unavailable";return false;}return services.advance(services.context,lease.root,q.values[0],q.integer!=0,error);
  }error="Unknown source input operation";return false;
 }
 bool finish(int status,std::string&out){if(status){if(error.empty())error=status==-1?"Malformed source input caller":"Required source input endpoint unavailable";out=error;return false;}out.clear();return true;}
};
SwfInputConnection::~SwfInputConnection(){release();}
bool SwfInputConnection::bind(SwfViewportLease lease,const ViewportState64&seed,const SwfViewportDriver&driver,std::shared_ptr<SwfInputHistory>history,gameswf::character*context,std::uint32_t flags,std::uint32_t&selection,const SwfInputCoreServices&services,std::string&error,std::shared_ptr<SwfControllerStorageV91> controllers,std::shared_ptr<SwfViewportConnection> viewport){
 if(state_||!lease.owner||!lease.root||!history||!services.owner){error="Malformed source input graph/owner binding";return false;}SwfInputHistoryFlags observed{};auto*movie=lease.root->get_root_movie();if(!movie||!history->read(movie,observed,error))return false;if(context&&context->get_player()!=movie->get_player()){error="Input context belongs to another player";return false;}
 if(viewport&&!viewport->owns_root(lease.root)){error="Input viewport belongs to another retained root";return false;}
 auto next=std::make_shared<State>(std::move(controllers));next->lease=lease;next->history=std::move(history);next->services=services;next->selection=&selection;
 // A RenderFX facade has one viewport for drawing and cursor projection.
 // Standalone connections still construct their own retained viewport.
 next->viewport=viewport?std::move(viewport):std::make_shared<SwfViewportConnection>();
 if(!next->viewport->bound()&&!next->viewport->bind(lease,seed,driver,error))return false;
 next->context_pin=context;next->input.root=reinterpret_cast<std::uintptr_t>(lease.root);next->input.context=identity(context);next->input.native_receiver=services.native_receiver;next->input.flags=flags;next->flat={next.get(),State::invoke};state_=std::move(next);error.clear();return true;
}
void SwfInputConnection::release()noexcept{state_.reset();}
bool SwfInputConnection::bound()const noexcept{return static_cast<bool>(state_);}
bool SwfInputConnection::focus(gameswf::character*c,std::uint32_t i,std::string&e){auto s=state_;if(!s){e="Input graph unbound";return false;}if(c){SwfInputHistoryFlags f{};if(!s->history->read(c,f,e))return false;}s->error.clear();return s->finish(dh2_ui_swf_set_focus(&s->input,identity(c),i,s->selection,&s->flat),e);}
bool SwfInputConnection::reset_controller_v91(std::uint32_t i,std::string& e){auto s=state_;if(!s){e="Input graph unbound";return false;}return s->controllers->reset_controller(i,e);}
bool SwfInputConnection::clear_context_v91(std::string& e){auto s=state_;if(!s){e="Input graph unbound";return false;}s->context_pin=nullptr;s->input.context=0;e.clear();return true;}
bool SwfInputConnection::reset_focus(std::uint32_t i,std::string&e){auto s=state_;if(!s){e="Input graph unbound";return false;}s->error.clear();return s->finish(dh2_ui_swf_reset_focus(&s->input,i,s->selection,&s->flat),e);}
bool SwfInputConnection::input(std::int32_t mask,std::uint32_t i,std::string&e){auto s=state_;if(!s){e="Input graph unbound";return false;}s->error.clear();return s->finish(dh2_ui_swf_update_input(&s->input,mask,i,s->selection,&s->flat),e);}
bool SwfInputConnection::cursor(const SwfCursor16&c,std::uint32_t i,std::string&e){auto s=state_;if(!s){e="Input graph unbound";return false;}s->error.clear();return s->finish(dh2_ui_swf_update_cursor(&s->input,&c,i,s->selection,&s->flat),e);}
bool SwfInputConnection::update(std::int32_t ms,bool flag,std::string&e){auto s=state_;if(!s){e="Input graph unbound";return false;}s->error.clear();return s->finish(dh2_ui_swf_update_pending(&s->input,ms,flag,s->selection,&s->flat),e);}
bool SwfInputConnection::graphic(gameswf::character*c,std::uint32_t i,std::string&e){auto s=state_;if(!s||i>=4){e="Malformed input graphic binding";return false;}if(c){SwfInputHistoryFlags f{};if(!s->history->read(c,f,e))return false;}auto&field=s->input.slots[i].graphic;if(field!=identity(c)){if(field)s->drop(character(field));field=identity(c);if(c)s->retain(c);}e.clear();return true;}
bool SwfInputConnection::enable(bool enabled,std::uint32_t i,std::string&e){auto s=state_;if(!s||i>=4){e="Malformed input cursor index";return false;}s->input.slots[i].enabled=enabled;e.clear();return true;}
bool SwfInputConnection::set_flags(std::uint32_t flags,std::string&e){auto s=state_;if(!s){e="Input graph unbound";return false;}s->input.flags=flags;e.clear();return true;}
bool SwfInputConnection::set_context(gameswf::character*c,std::string&e){
 auto s=state_;if(!s||!c||c->get_player()!=s->lease.root->get_root_movie()->get_player()){e="Input context belongs to another player or is absent";return false;}
 // RenderFX::SetContext 0x7a7ee8 only replaces the character pointer.
 // Keep the live cursor/focus state and pin the replacement in this adapter.
 s->context_pin=c;s->input.context=identity(c);e.clear();return true;
}
bool SwfInputConnection::scene_binding(gameswf::character*c,std::uintptr_t scene,std::string&e){auto s=state_;if(!s||!c||!c->is(gameswf::sprite_instance::m_class_id)){e="Malformed source sprite scene projection";return false;}SwfInputHistoryFlags observed{};if(!s->history->read(c,observed,e))return false;s->scenes[c]={c,scene};e.clear();return true;}
bool SwfInputConnection::snapshot(SwfInputState288&out,std::string&e)const{auto s=state_;if(!s){e="Input graph unbound";return false;}out=s->input;e.clear();return true;}
bool SwfInputConnection::raw_cursor(float xy[2],std::int32_t&i,std::string&e)const{auto s=state_;if(!s||!xy){e="Input graph unbound";return false;}xy[0]=s->raw_xy[0];xy[1]=s->raw_xy[1];i=s->raw_index;e.clear();return true;}
bool SwfInputConnection::viewport_rectangle(const std::int32_t xywh[4],std::string&e){
 auto s=state_;if(!s||!xywh){e="Input graph/viewport unbound";return false;}
 return s->viewport->set_viewport(xywh,e)&&s->viewport->set_bounds(xywh,0,e);
}
}
