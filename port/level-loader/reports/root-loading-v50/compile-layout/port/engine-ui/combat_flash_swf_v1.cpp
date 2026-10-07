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
 SwfMovie& movie;CombatFlashProjectionV1 projection;CombatFlashQueueV1 queue;
 SwfAsGraph* graph{};gameswf::sprite_instance* root{};std::map<std::uintptr_t,SwfAsValue> pins;
 std::string error;std::function<int()> operation;float display_rect[4]{};int display_viewport[4]{};bool display_prepared{};
 explicit Impl(SwfMovie& m,CombatFlashProjectionV1 p):movie(m),projection(p),queue({this,scan,clone,set_text,frames,project,scale,begin,draw,end}){}
 bool run(std::function<int()> op,std::string& out){operation=std::move(op);error.clear();
  const bool result=movie.action_script(this,[](void* p,SwfAsGraph& g,std::string& e){auto& t=*static_cast<Impl*>(p);SwfAsValue value;gameswf::as_object* object{};
   if(!g.root_value(value,e)||!g.borrow_object(value,object,e)||!object||!object->is(gameswf::sprite_instance::m_class_id)){e="Required source combat root sprite";return false;}
   t.graph=&g;t.root=static_cast<gameswf::sprite_instance*>(object);const auto status=t.operation();t.graph=nullptr;t.root=nullptr;
   if(status<0){e=t.error.empty()?"Required source combat SWF continuation":t.error;return false;}return true;
  },out);operation={};return result;
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
CombatFlashSwfV1::CombatFlashSwfV1(SwfMovie& m,CombatFlashProjectionV1 p):impl_(new Impl(m,p)){}
CombatFlashSwfV1::~CombatFlashSwfV1()=default;
bool CombatFlashSwfV1::scan(std::string& e){return impl_->run([&]{return impl_->queue.scan();},e);}
bool CombatFlashSwfV1::play(const char* s,const float p[3],const char* text,int number,int color,bool numeric,std::string& e){return impl_->run([&]{return impl_->queue.play(s,p,text,number,color,numeric);},e);}
bool CombatFlashSwfV1::update(std::uint32_t dt,std::int32_t phase,std::string& e){return impl_->run([&]{int interval=33;if(phase<2||phase>26){const auto fps=impl_->root->get_root()->get_frame_rate();if(!std::isfinite(fps)||fps<=0)return -1;interval=int(1000.f/fps);}return impl_->queue.update(dt,interval);},e);}
bool CombatFlashSwfV1::draw(bool disabled,std::string& e){
 // Read this movie's source rectangle before entering its graph Scope. The
 // facade's viewport connection uses that same Scope and cannot be nested.
 impl_->display_prepared=false;
 if(!disabled){if(!impl_->projection.display_rectangle||!impl_->projection.display_rectangle(impl_->projection.context,impl_->display_rect,impl_->display_viewport,e))return false;impl_->display_prepared=true;}
 const bool result=impl_->run([&]{return impl_->queue.draw(disabled);},e);impl_->display_prepared=false;return result;
}
const CombatFlashQueueV1& CombatFlashSwfV1::queue()const{return impl_->queue;}
void CombatFlashSwfV1::stop_all(){impl_->queue.stop_all();}
}
