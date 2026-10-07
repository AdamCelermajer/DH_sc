#include "combat_flash_swf_v1.hpp"
#include "swf_text_font_platform_v1.hpp"
#include "gameswf/gameswf_sprite.h"
#include "gameswf/gameswf_render.h"
#include <map>
#include <functional>
#include <cstring>
#include <cmath>
namespace dh2::ui {
struct CombatFlashSwfV1::Impl {
 SwfMovie* legacy_movie{};
 std::weak_ptr<SwfMovie> movie;
 std::weak_ptr<void> projection_owner;
 bool managed{},running{};
 std::uintptr_t movie_identity{},graph_identity{};
 std::uint64_t epoch{},scan_serial{};
 CombatFlashScanReceiptV92 receipt{};
 CombatFlashProjectionV1 projection{};CombatFlashQueueV1 queue;
 SwfAsGraph* graph{};gameswf::sprite_instance* root{};std::map<std::uintptr_t,SwfAsValue> pins;
 std::string error;std::function<int()> operation;float display_rect[4]{};int display_viewport[4]{};bool display_prepared{};
 Impl():queue({this,scan,clone,set_text,frames,project,scale,begin,draw,end}){}
 Impl(SwfMovie& m,CombatFlashProjectionV1 p):Impl(){legacy_movie=&m;projection=p;movie_identity=reinterpret_cast<std::uintptr_t>(&m);graph_identity=m.player_identity();epoch=1;}

 bool run(std::function<int()> op,std::string& out){
  if(running){out="Flash backend already executing this graph";return false;}
  auto held_movie=movie.lock();auto held_projection=projection_owner.lock();
  auto* actual=managed?held_movie.get():legacy_movie;
  if(!actual||(managed&&!held_projection)||!graph_identity||actual->player_identity()!=graph_identity){out="Required same live Flash HUD movie/projection generation";return false;}
  running=true;operation=std::move(op);error.clear();
  struct Finish {Impl& t;~Finish(){t.graph=nullptr;t.root=nullptr;t.operation={};t.running=false;}} finish{*this};
  //A genuine HUD native callback may enqueue Flash text synchronously. Borrow
  //the same scoped graph without advancing its movie; nested Scope preserves
  //the active player's providers and first failure. running still prevents
  //recursive mutation of this Flash queue while its own operation is active.
  return actual->menu_action_script(this,[](void* p,SwfAsGraph& g,std::string& e){auto& t=*static_cast<Impl*>(p);SwfAsValue value;gameswf::as_object* object{};
   if(!g.root_value(value,e)||!g.borrow_object(value,object,e)||!object||!object->is(gameswf::sprite_instance::m_class_id)){e="Required source combat root sprite";return false;}
   t.graph=&g;t.root=static_cast<gameswf::sprite_instance*>(object);const auto status=t.operation();
   if(status<0){e=t.error.empty()?"Required source combat SWF continuation":t.error;return false;}return true;
  },out);
 }
 gameswf::character* character(std::uintptr_t id){auto at=pins.find(id);gameswf::as_object* object{};if(at==pins.end()||!graph||!graph->borrow_object(at->second,object,error)||!object||!object->is(gameswf::character::m_class_id))return nullptr;return static_cast<gameswf::character*>(object);}
 std::uintptr_t pin(gameswf::character* c){if(!c||!graph)return 0;SwfAsValue value;if(!graph->retain_object(c,value,error))return 0;const auto id=reinterpret_cast<std::uintptr_t>(c);pins[id]=std::move(value);return id;}
 static void collect(gameswf::character* c,const char* substring,std::vector<gameswf::character*>& out){if(!c)return;if(std::strstr(c->get_name().c_str(),substring))out.push_back(c);if(c->is(gameswf::sprite_instance::m_class_id)){auto* sprite=static_cast<gameswf::sprite_instance*>(c);for(int n=0;n<sprite->m_display_list.size();++n)collect(sprite->m_display_list.get_character(n),substring,out);}}
 static std::uintptr_t text(Impl& t,gameswf::character* c){std::vector<gameswf::character*> found;collect(c,"_text",found);return found.empty()?0:t.pin(found.front());}
 static int scan(void* p,std::vector<CombatFlashStyleV1>& out){auto& t=*static_cast<Impl*>(p);if(!t.root)return -1;std::vector<gameswf::character*> found;collect(t.root,"anim_",found);for(auto* c:found){CombatFlashStyleV1 style;style.name=c->get_name().c_str();style.instances[0]={t.pin(c),text(t,c),false};if(!style.instances[0].clip)return -1;c->set_visible(false);out.push_back(std::move(style));}return 0;}
 static int clone(void* p,std::uintptr_t source,const char* name,CombatFlashInstanceV1* out){auto& t=*static_cast<Impl*>(p);auto* c=t.character(source);if(!c||!c->get_parent()||!c->get_parent()->is(gameswf::sprite_instance::m_class_id))return -1;auto* parent=static_cast<gameswf::sprite_instance*>(c->get_parent());auto* clone=c->clone_display_object(tu_string(name),parent->get_highest_depth());if(!clone)return -1;*out={t.pin(clone),text(t,clone),false};return out->clip?0:-1;}
 static int set_text(void* p,std::uintptr_t id,const char* value){auto& t=*static_cast<Impl*>(p);auto* c=t.character(id);return c&&c->set_member("text",gameswf::as_value(value))?0:-1;}
 static int frames(void* p,std::uintptr_t id,int* out){auto* c=static_cast<Impl*>(p)->character(id);if(!c||!out)return -1;*out=c->get_frame_count();return 0;}
 static int project(void* p,const float value[3],int* x,int* y){auto& t=*static_cast<Impl*>(p);return t.projection.world_to_screen&&t.projection.world_to_screen(t.projection.context,value,x,y,t.error)?0:-1;}
 static int scale(void* p,std::uintptr_t id,float* x,float* y){auto& t=*static_cast<Impl*>(p);auto* c=t.character(id);if(!c||!x||!y||!c->is(gameswf::sprite_instance::m_class_id))return -1;auto* root=static_cast<gameswf::sprite_instance*>(c)->get_root();if(!root||!root->m_def||!root->m_viewport_width||!root->m_viewport_height)return -1;const auto& b=root->m_def->m_frame_size;*x=((b.m_x_max-b.m_x_min)/20.f)/float(root->m_viewport_width);*y=((b.m_y_max-b.m_y_min)/20.f)/float(root->m_viewport_height);return 0;}
 static int begin(void* p){auto& t=*static_cast<Impl*>(p);if(!t.root||!t.display_prepared)return -1;auto platform=SwfTextFontPlatformV1::for_player(t.root->get_player());if(!platform){t.error="Required SAME source combat text font platform";return -1;}platform->policy().buffering=false;const auto* rect=t.display_rect;const auto* viewport=t.display_viewport;gameswf::render::begin_display(t.root->get_root()->m_background_color,viewport[0],viewport[1],viewport[2],viewport[3],rect[0],rect[1],rect[2],rect[3]);return 0;}
 static void position(gameswf::character* c,int x,int y){const auto old=c->get_matrix();gameswf::matrix next;next.m_[0][2]=float(x)*20.f;next.m_[1][2]=float(y)*20.f;next.set_scale_rotation(old.get_x_scale(),old.get_y_scale(),old.get_rotation());c->set_matrix(next);}
 static int draw(void* p,std::uintptr_t clip,std::uintptr_t text,const CombatFlashContextV1* q){auto& t=*static_cast<Impl*>(p);auto* c=t.character(clip);if(!c||!q)return -1;const auto old=c->get_matrix();const int x=int(old.m_[0][2]/20.f),y=int(old.m_[1][2]/20.f);position(c,x+q->x,y+q->y);c->goto_frame(q->frame);c->set_play_state(gameswf::character::STOP);
  if(text){auto* field=t.character(text);if(!field){position(c,x,y);c->set_visible(false);return -1;}gameswf::cxform transform;for(int n=0;n<3;++n)transform.m_[n][0]=0;transform.m_[3][0]=1;transform.m_[0][1]=float((std::uint32_t(q->color)>>16)&255);transform.m_[1][1]=float((std::uint32_t(q->color)>>8)&255);transform.m_[2][1]=float(std::uint32_t(q->color)&255);transform.m_[3][1]=float(std::uint32_t(q->color)>>24);field->set_cxform(transform);}
  c->set_visible(true);c->display();c->set_visible(false);position(c,x,y);return 0;
 }
 static int end(void* p){auto& t=*static_cast<Impl*>(p);auto platform=SwfTextFontPlatformV1::for_player(t.root->get_player());if(!platform)return -1;platform->policy().buffering=true;gameswf::render::end_display();return 0;}
};
CombatFlashSwfV1::CombatFlashSwfV1():impl_(new Impl){}
CombatFlashSwfV1::CombatFlashSwfV1(SwfMovie& m,CombatFlashProjectionV1 p):impl_(new Impl(m,p)){}
CombatFlashSwfV1::~CombatFlashSwfV1()=default;
bool CombatFlashSwfV1::scan(std::string& e){
 if(!impl_->run([&]{return impl_->queue.scan();},e))return false;
 impl_->receipt={reinterpret_cast<std::uintptr_t>(this),impl_->movie_identity,impl_->graph_identity,impl_->epoch,++impl_->scan_serial};return true;
}
bool CombatFlashSwfV1::scan_receipt_v92(CombatFlashScanReceiptV92& out,std::string& e){out={};if(!scan(e))return false;out=impl_->receipt;return true;}
bool CombatFlashSwfV1::validate_receipt_v92(const CombatFlashScanReceiptV92& r)const noexcept{
 const auto& a=impl_->receipt;auto m=impl_->movie.lock();auto owner=impl_->projection_owner.lock();
 return impl_->managed&&m&&owner&&m->player_identity()==impl_->graph_identity&&a.scan_serial&&
  r.backend==reinterpret_cast<std::uintptr_t>(this)&&r.movie==a.movie&&r.graph==a.graph&&r.binding_epoch==a.binding_epoch&&r.scan_serial==a.scan_serial;
}
bool CombatFlashSwfV1::bind_movie_v92(const std::shared_ptr<SwfMovie>& m,std::weak_ptr<void> owner,CombatFlashProjectionV1 projection,std::string& e){
 if(impl_->running||impl_->movie_identity||!impl_->pins.empty()||!impl_->queue.styles().empty()){e="Flash HUD replacement requires completed matching reset";return false;}
 for(const auto& c:impl_->queue.contexts())if(c.flags){e="Flash HUD replacement has uncleared contexts";return false;}
 if(!m||!m->player_identity()||owner.expired()){e="Required actual loaded HUD facade/projection owner";return false;}
 impl_->managed=true;impl_->legacy_movie=nullptr;impl_->movie=m;impl_->projection_owner=std::move(owner);impl_->projection=projection;
 impl_->movie_identity=reinterpret_cast<std::uintptr_t>(m.get());impl_->graph_identity=m->player_identity();++impl_->epoch;impl_->receipt={};e.clear();return true;
}
bool CombatFlashSwfV1::reset_scan_fields_v92(std::string& e){
 if(impl_->running){e="Flash field reset requires completed graph dispatch";return false;}
 impl_->queue.reset_scan();impl_->pins.clear();impl_->receipt={};e.clear();return true;
}
bool CombatFlashSwfV1::reset_movie_v92(std::uintptr_t expected,std::string& e){
 if(impl_->movie_identity!=expected){e="Flash reset does not match bound HUD facade";return false;}
 if(impl_->running){e="Flash reset requires completed source graph dispatch";return false;}
 if(!reset_scan_fields_v92(e))return false;impl_->graph=nullptr;impl_->root=nullptr;
 impl_->movie.reset();impl_->projection_owner.reset();impl_->projection={};impl_->legacy_movie=nullptr;
 impl_->movie_identity=impl_->graph_identity=0;impl_->display_prepared=false;e.clear();return true;
}
bool CombatFlashSwfV1::discard_unscanned_binding_v92(std::uintptr_t expected,std::string& e){
 if(impl_->receipt.scan_serial||!impl_->pins.empty()||!impl_->queue.styles().empty()){e="Cannot discard an actual scanned Flash prefix";return false;}
 return reset_movie_v92(expected,e);
}
std::uintptr_t CombatFlashSwfV1::bound_movie_v92()const noexcept{return impl_->movie_identity;}
bool CombatFlashSwfV1::busy_v92()const noexcept{return impl_->running;}
bool CombatFlashSwfV1::source_frame_rate_v92(float& out,std::string& e){return impl_->run([&]{out=impl_->root->get_root()->get_frame_rate();return 0;},e);}
bool CombatFlashSwfV1::update_interval_v92(std::uint32_t dt,std::int32_t interval,std::string& e){
 if(!impl_->movie_identity){const int r=impl_->queue.update(dt,interval);if(r<0){e="Detached Flash queue has invalid active context";return false;}e.clear();return true;}
 return impl_->run([&]{return impl_->queue.update(dt,interval);},e);
}
bool CombatFlashSwfV1::play(const char* s,const float p[3],const char* text,int number,int color,bool numeric,std::string& e){return impl_->run([&]{return impl_->queue.play(s,p,text,number,color,numeric);},e);}
bool CombatFlashSwfV1::update(std::uint32_t dt,std::int32_t phase,std::string& e){return impl_->run([&]{int interval=33;if(phase<2||phase>26){const auto fps=impl_->root->get_root()->get_frame_rate();if(!std::isfinite(fps)||fps<=0)return -1;interval=int(1000.f/fps);}return impl_->queue.update(dt,interval);},e);}
bool CombatFlashSwfV1::draw(bool disabled,std::string& e){
 // Read this movie's source rectangle before entering its graph Scope. The
 // facade's viewport connection uses that same Scope and cannot be nested.
 auto held_movie=impl_->movie.lock();auto held_projection=impl_->projection_owner.lock();
 if(impl_->managed&&(!held_movie||!held_projection||held_movie->player_identity()!=impl_->graph_identity)){e="Required same live Flash draw provider/HUD generation";return false;}
 impl_->display_prepared=false;
 if(!disabled){if(!impl_->projection.display_rectangle||!impl_->projection.display_rectangle(impl_->projection.context,impl_->display_rect,impl_->display_viewport,e))return false;impl_->display_prepared=true;}
 const bool result=impl_->run([&]{return impl_->queue.draw(disabled);},e);impl_->display_prepared=false;return result;
}
const CombatFlashQueueV1& CombatFlashSwfV1::queue()const{return impl_->queue;}
void CombatFlashSwfV1::stop_all(){impl_->queue.stop_all();}
}
