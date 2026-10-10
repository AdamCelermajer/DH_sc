#include "../../swf_movie.hpp"
#include <algorithm>
#include "swf_source_movie_v1.hpp"
#include "swf_frame_connection.hpp"
#include "swf_event_core.hpp"
#include "../../swf_controller_storage_v91.hpp"
#include "../../swf_menu_unload_v91.hpp"
#include "../../swf_frame_connection.hpp"
#include <cstring>
#include <functional>
#include <mutex>
#include "../../swf_source_movie_v1.hpp"
#include "../../swf_source_startup_v1.hpp"
#include "../../swf_text_font_platform_v1.hpp"
#include "gameswf/gameswf.h"
#include "gameswf/gameswf_player.h"
#include "gameswf/gameswf_root.h"
#include "gameswf/gameswf_sprite.h"
#include "gameswf/gameswf_character.h"
#include "gameswf/gameswf_text.h"
#include "gameswf/gameswf_types.h"
#include "gameswf/gameswf_movie_def.h"
#include "gameswf/gameswf_render.h"
#include "gameswf/gameswf_function.h"
#include "gameswf/gameswf_as_classes/as_array.h"
#include "base/tu_file.h"
#include "base/image.h"
#include <cmath>
#include <limits>
#include <exception>
#include <set>
#undef isfinite

namespace dh2::ui {
namespace {
SwfMatrix matrix(const gameswf::matrix& m){SwfMatrix r;for(int i=0;i<2;++i)for(int j=0;j<3;++j)r.value[i*3+j]=float(m.m_[i][j]);return r;}
SwfColorTransform cx(const gameswf::cxform& m){SwfColorTransform r;for(int i=0;i<4;++i)for(int j=0;j<2;++j)r.value[i*2+j]=float(m.m_[i][j]);return r;}
void rgba(std::uint8_t*o,const gameswf::rgba&c){o[0]=c.m_r;o[1]=c.m_g;o[2]=c.m_b;o[3]=c.m_a;}
void rect(float*o,const gameswf::rect&r){o[0]=r.m_x_min;o[1]=r.m_x_max;o[2]=r.m_y_min;o[3]=r.m_y_max;}
struct Bitmap:gameswf::bitmap_info {
 SwfTexture texture{};int width{},height{};
 explicit Bitmap(int w=0,int h=0):width(w),height(h){}
 int get_width()const override{return width;}int get_height()const override{return height;}
};
}
struct SwfMovie::Impl:gameswf::render_handler {
 SwfServices service{};gameswf::gc_ptr<gameswf::player> player;
 std::shared_ptr<SwfControllerStorageV91> controllers;
 Impl():controllers(std::make_shared<SwfControllerStorageV91>()){}
 explicit Impl(std::shared_ptr<SwfControllerStorageV91> existing):controllers(std::move(existing)){}
 std::string source_filename_v91;
 gameswf::gc_ptr<gameswf::root> root;std::vector<gameswf::gc_ptr<gameswf::root>> shared;
 std::vector<gameswf::gc_ptr<gameswf::sprite_instance>> hud_pins;
 std::vector<std::string> messages;std::string failure;SwfDraw state{};
 std::uintptr_t event_listenerfc_v68{};
 std::weak_ptr<void> event_listener_lease_v68;
 std::function<bool(SwfEvent48&,std::string&)> event_listener_v68;
 std::function<bool(SwfEvent48&,bool&,std::string&)> event_accept_v68;

 struct DisplayHook {
  Impl* owner{};gameswf::gc_ptr<gameswf::character> character;
  void* context{};bool (*draw)(void*,const SwfDraw&,std::string&){};
  static void display(void* raw){auto& h=*static_cast<DisplayHook*>(raw);
   if(active!=h.owner){h.owner->fail("Display callback outside retained movie scope");return;}
   try{gameswf::rect r;h.character->get_bound(&r);
    if(auto* p=h.character->get_parent())p->get_world_matrix().transform(&r);
    SwfDraw pane;pane.kind=SwfDraw::bitmap_quad;rect(pane.rect,r);
    std::string e;if(!h.draw||!h.draw(h.context,pane,e))h.owner->fail(e);
   }catch(const std::exception& e){h.owner->fail(e.what());}
  }
 };
 std::vector<std::unique_ptr<DisplayHook>> display_hooks;
 static Impl* active;
 static std::recursive_mutex scope_gate;
 unsigned scope_depth=0;
 struct AsContext {
  std::weak_ptr<Impl> graph;
  std::shared_ptr<void> provider_owner;
  void* provider_context{};
  bool (*callback)(void*,const char*,const gameswf::fn_call&,std::string&){};
  static bool within(void* ptr,const void* identity){auto& c=*static_cast<AsContext*>(ptr);auto p=c.graph.lock();return p&&p.get()==identity&&Impl::active==p.get();}
  static bool native(void* ptr,const char* name,const gameswf::fn_call& fn,std::string& error){auto& c=*static_cast<AsContext*>(ptr);return c.callback&&c.callback(c.provider_context,name,fn,error);}
  static void failure(void* ptr,const std::string& error){auto& c=*static_cast<AsContext*>(ptr);if(auto p=c.graph.lock())p->fail(error);}
 };
 ~Impl(){
   for(auto& hook:display_hooks)hook->character->set_display_callback(nullptr,nullptr);
   display_hooks.clear();
  if(player){
   // Quiescent native owner teardown. Upstream's tracked heap omits functions
   // created by DefineFunction, so retain the entire reachable AS graph first.
   // These are ownership operations only; no getter or ActionScript executes.
   std::vector<gameswf::gc_ptr<gameswf::as_object>> pins;
   std::set<gameswf::as_object*> seen;
   auto pin=[&](gameswf::as_object*o){if(o&&seen.insert(o).second)pins.push_back(o);};
   auto value=[&](const gameswf::as_value&v){if(v.is_object())pin(v.to_object());auto*p=v.to_property();if(p){pin(const_cast<gameswf::as_object*>(v.get_property_target()));pin(p->m_getter.get_ptr());pin(p->m_setter.get_ptr());}};
   pin(player->m_global.get_ptr());if(root)pin(root->get_root_movie());for(auto&r:shared)pin(r->get_root_movie());for(auto&clip:hud_pins)pin(clip.get_ptr());
   for(auto it=player->m_heap.begin();it!=player->m_heap.end();++it)pin(it->first.get_ptr());
   for(std::size_t i=0;i<pins.size();++i){auto*o=pins[i].get_ptr();pin(o->m_proto.get_ptr());for(auto it=o->m_members.begin();it!=o->m_members.end();++it)value(it->second);
    if(o->m_watch)for(auto it=o->m_watch->begin();it!=o->m_watch->end();++it){pin(it->second.m_func);value(it->second.m_user_data);}
    if(auto*a=dynamic_cast<gameswf::as_array*>(o))for(int k=0;k<a->m_array.size();++k)value(a->m_array[k]);
    if(auto*f=dynamic_cast<gameswf::as_s_function*>(o))for(int k=0;k<f->m_with_stack.size();++k)pin(f->m_with_stack[k].m_object.get_ptr());
    if(auto*env=o->get_environment()){pin(env->m_target.get_ptr());for(int k=0;k<env->get_stack_size();++k)value(env->bottom(k));for(int k=0;k<env->m_scope.size();++k)value(env->m_scope.bottom(k));for(auto&v:env->m_global_register)value(v);for(int k=0;k<env->m_local_register.size();++k)value(env->m_local_register[k]);for(int k=0;k<env->m_local_frames.size();++k)value(env->m_local_frames[k].m_value);}
   }
   for(auto&o:pins){o->m_proto=nullptr;o->m_members.clear();if(o->m_watch)o->m_watch->clear();
    if(auto*a=dynamic_cast<gameswf::as_array*>(o.get_ptr()))a->m_array.clear();
    if(auto*f=dynamic_cast<gameswf::as_s_function*>(o.get_ptr()))f->m_with_stack.clear();
    if(auto*env=o->get_environment()){env->m_target=nullptr;env->set_stack_size(0);env->m_scope.resize(0);for(auto&v:env->m_global_register)v.set_undefined();env->m_local_register.clear();env->m_local_frames.clear();}
   }
   player->clear_heap();root=nullptr;shared.clear();hud_pins.clear();
  }
 }
 struct Scope {
  std::unique_lock<std::recursive_mutex> gate;
  Impl*p;bool entered;Impl* previous_active{};
  gameswf::glyph_provider* previous_glyphs{};
  gameswf::render_handler* previous_renderer{};
  gameswf::fscommand_callback previous_fscommand_v114{};
  Scope(Impl*i,bool menu_dispatch=false):gate(scope_gate,std::try_to_lock),p(i),
   entered(i&&gate.owns_lock()&&(!active||menu_dispatch)){
   if(entered){
    previous_active=active;previous_glyphs=gameswf::get_glyph_provider();
    previous_renderer=gameswf::get_render_handler();
    previous_fscommand_v114=gameswf::get_fscommand_callback();
    active=p;if(p->scope_depth++==0)p->failure.clear();
    gameswf::set_glyph_provider(p->service.glyphs);gameswf::set_render_handler(p);
    // These callbacks dispatch through active, so they remain the same
    // trampolines throughout a nested renderer chain.
    gameswf::register_file_opener_callback(open);gameswf::register_log_callback(log);
    gameswf::register_bitmap_substitution_callback(substitute);
    gameswf::register_fscommand_callback(source_fscommand_v114);
   }
  }
  ~Scope(){if(entered){
   --p->scope_depth;active=previous_active;
   gameswf::set_glyph_provider(previous_glyphs);gameswf::set_render_handler(previous_renderer);
   gameswf::register_bitmap_substitution_callback(previous_active?substitute:nullptr);
   gameswf::register_fscommand_callback(previous_fscommand_v114);
  }}
 };
 void fail(const std::string&s){if(failure.empty())failure=s.empty()?"Required SWF provider rejected delivery":s;}
 bool finish(std::string&e){if(!failure.empty()){e=failure;return false;}e.clear();return true;}
 static void log(bool error,const char*s){if(!active)return;active->messages.emplace_back(s?s:"");if(active->service.diagnostic)active->service.diagnostic(active->service.context,error,s?s:"");}
 static void source_fscommand_v114(gameswf::character* origin,const char* command,const char* arg){
  auto* p=active;if(!p)return;
  if(!origin||!p->player||origin->get_player()!=p->player.get_ptr()){
   p->fail("FSCommand originated outside its actual retained movie player");return;
  }
  //RenderFX.FSCommand7a7e5c only calls handler_fc.virtual4 when nonNULL.
  const auto receiver=p->event_listenerfc_v68;if(!receiver)return;
  std::string error;
  try{
   auto listener=p->event_listener_lease_v68.lock();
   auto owner=p->service.native_owner;auto callback=p->service.source_fscommand_v114;
   auto continuation=p->service.source_continue_v98;
   if(!listener){p->fail("Expired original FSCommand listenerfc");return;}
   if(!callback||!owner){p->fail("Required owned FSCommand listenerfc virtual4 transport");return;}
   if(continuation&&!continuation(error)){p->fail(error);return;}
   //Captured fc and its actual handler stay pinned if dispatch changes the
   //current menu/listener or enters another movie. Never reroute by name.
   bool handled=false;const bool delivered=callback(receiver,command,arg,handled,error);
   if(!delivered)p->fail(error.empty()?std::string("Source FSCommand provider rejected ")+(command?command:"<NULL>"):error);
   if(continuation&&!continuation(error))p->fail(error);
   //The source virtual returns no AS value. Unknown commands remain unhandled;
   //they do not fall through to a callback belonging to another player.
  }catch(const std::exception& failure){p->fail(failure.what());}
  catch(...){p->fail("Source FSCommand provider threw");}
 }
 static tu_file*open(const char*uri){if(!active)return nullptr;auto&p=*active;std::vector<std::uint8_t>b;std::string e;
  if(!p.service.read||!p.service.read(p.service.context,uri,b,e)){p.fail(e.empty()?std::string("SWF file provider unavailable: ")+uri:e);return nullptr;}
  if(b.size()>std::size_t(std::numeric_limits<int>::max())){p.fail("SWF stream exceeds native reader length");return nullptr;}
  auto*f=new tu_file(tu_file::memory_buffer);if(!b.empty())f->write_bytes(b.data(),int(b.size()));f->set_position(0);return f;
 }
 static void substitute(const char*name,gameswf::bitmap_info*b){if(!active)return;auto&p=*active;auto*bitmap=dynamic_cast<Bitmap*>(b);std::string e;SwfTexture t;
  if(!bitmap||!p.service.texture||!p.service.texture(p.service.context,name,b->get_width(),b->get_height(),t,e)||!t.identity){p.fail(e.empty()?std::string("External SWF texture unavailable: ")+name:e);return;}
  bitmap->texture=t;bitmap->width=t.width;bitmap->height=t.height;
 }
 static void native_string(const gameswf::fn_call&f){if(!active)return;auto&p=*active;std::vector<SwfValue>args;
  for(int i=0;i<f.nargs;++i){const auto&v=f.arg(i);SwfValue a;if(v.is_string()){a.kind=SwfValue::text;a.string=v.to_string();}else if(v.is_bool()){a.kind=SwfValue::boolean;a.numeric=v.to_bool();}else if(!v.is_undefined()){a.kind=SwfValue::number;a.numeric=v.to_number();}args.push_back(std::move(a));}
  SwfValue out;std::string e;if(!p.service.native_call||!p.service.native_call(p.service.context,"NativeGetStringFromSymbol",args,out,e)){p.fail(e.empty()?"NativeGetStringFromSymbol unavailable":e);return;}
  if(!f.result)return;switch(out.kind){case SwfValue::text:f.result->set_string(out.string.c_str());break;case SwfValue::boolean:f.result->set_bool(out.numeric!=0);break;case SwfValue::number:f.result->set_double(out.numeric);break;default:f.result->set_undefined();}
 }
 void emit(SwfDraw d){std::string e;if(!service.draw||!service.draw(service.context,d,e))fail(e.empty()?"SWF draw sink unavailable":e);}
 Bitmap*image(int w,int h,std::uint32_t channels,const std::uint8_t*pixels,int pitch){auto*b=new Bitmap(w,h);std::string e;
  if(w>0&&h>0&&(!service.image||!service.image(service.context,w,h,channels,pixels,pitch,b->texture,e)||!b->texture.identity))fail(e.empty()?"Embedded SWF image upload unavailable":e);return b;
 }
 gameswf::bitmap_info*create_bitmap_info_empty()override{return new Bitmap;}
 gameswf::bitmap_info*create_bitmap_info_alpha(int w,int h,unsigned char*d)override{return image(w,h,1,d,w);}
 gameswf::bitmap_info*create_bitmap_info_rgb(image::rgb*i)override{return image(i->m_width,i->m_height,3,i->m_data,i->m_pitch);}
 gameswf::bitmap_info*create_bitmap_info_rgba(image::rgba*i)override{return image(i->m_width,i->m_height,4,i->m_data,i->m_pitch);}
 gameswf::video_handler*create_video_handler()override{fail("SWF video backend unavailable");return nullptr;}
 void begin_display(gameswf::rgba c,int x,int y,int w,int h,float x0,float x1,float y0,float y1)override{SwfDraw d;d.kind=SwfDraw::begin;d.viewport[0]=x;d.viewport[1]=y;d.viewport[2]=w;d.viewport[3]=h;d.bounds[0]=x0;d.bounds[1]=x1;d.bounds[2]=y0;d.bounds[3]=y1;rgba(d.background,c);emit(d);}
 void end_display()override{
  // Original root::display7755b8 -> flush_buffered_text7755c0 -> end7755cc.
  // Keep the queued fields inside this same live renderer/core Scope.
  if(root&&player){auto platform=SwfTextFontPlatformV1::for_player(player.get_ptr());std::string error;
   if(platform&&!platform->flush_buffered_text(error))fail(error.empty()?"Source buffered text flush failed":error);}
  SwfDraw d;d.kind=SwfDraw::end;emit(d);
 }
 void set_matrix(const gameswf::matrix&m)override{state.matrix=matrix(m);}void set_cxform(const gameswf::cxform&c)override{state.color_transform=cx(c);}
 void vertices(const void*v,int n,SwfDraw::Kind kind){if(n<0||(!v&&n)){fail("Malformed upstream draw span");return;}SwfDraw d=state;d.kind=kind;auto*p=static_cast<const coord_component*>(v);d.xy.reserve(std::size_t(n)*2);for(int i=0;i<n*2;++i)d.xy.push_back(float(p[i]));emit(std::move(d));}
 void draw_mesh_strip(const void*v,int n)override{vertices(v,n,SwfDraw::triangle_strip);}void draw_triangle_list(const void*v,int n)override{vertices(v,n,SwfDraw::triangles);}void draw_line_strip(const void*v,int n)override{vertices(v,n,SwfDraw::line_strip);}
 void fill_style_disable(int side)override{if(side==0)state.fill.kind=SwfFill::disabled;}
 void fill_style_color(int side,const gameswf::rgba&c)override{if(side==0){state.fill.kind=SwfFill::color;rgba(state.fill.rgba,c);}}
 void fill_style_bitmap(int side,gameswf::bitmap_info*b,const gameswf::matrix&m,bitmap_wrap_mode w,bitmap_blend_mode blend)override{if(side!=0)return;auto*i=dynamic_cast<Bitmap*>(b);if(!i||!i->texture.identity){fail("SWF fill references unresolved bitmap");return;}state.fill.kind=SwfFill::bitmap;state.fill.texture=i->texture;state.fill.uv=matrix(m);state.fill.wrap=w;state.fill.blend=blend;}
 void line_style_disable()override{state.line.kind=SwfFill::disabled;}void line_style_color(gameswf::rgba c)override{state.line.kind=SwfFill::color;rgba(state.line.rgba,c);}void line_style_width(float w)override{state.line_width=w;}
 void draw_bitmap(const gameswf::matrix&m,gameswf::bitmap_info*b,const gameswf::rect&r,const gameswf::rect&uv,gameswf::rgba color)override{SwfDraw d=state;d.kind=SwfDraw::bitmap_quad;d.matrix=matrix(m);d.fill.kind=SwfFill::bitmap;auto*i=dynamic_cast<Bitmap*>(b);if(!i||!i->texture.identity){fail("SWF bitmap quad references unresolved bitmap");return;}d.fill.texture=i->texture;rgba(d.fill.rgba,color);rect(d.rect,r);rect(d.uv_rect,uv);emit(std::move(d));}
 void set_antialiased(bool b)override{SwfDraw d;d.kind=SwfDraw::antialias;d.enabled=b;emit(d);}
 bool test_stencil_buffer(const gameswf::rect&r,Uint8 pattern)override{float b[4];rect(b,r);bool result=false;std::string e;if(!service.stencil||!service.stencil(service.context,b,pattern,result,e))fail(e.empty()?"SWF stencil query backend unavailable":e);return result;}
 void mask(SwfDraw::Kind k){SwfDraw d;d.kind=k;emit(d);}void begin_submit_mask()override{mask(SwfDraw::mask_begin);}void end_submit_mask()override{mask(SwfDraw::mask_end);}void disable_mask()override{mask(SwfDraw::mask_disable);}
 bool is_visible(const gameswf::rect&)override{return true;} // conservative submission, no visibility culling
 void open()override{}
 gameswf::character*find(const char*path){if(!root||!path)return nullptr;auto*o=root->get_root_movie()->find_target(gameswf::as_value(path));return o&&o->is(gameswf::character::m_class_id)?static_cast<gameswf::character*>(o):nullptr;}
};
SwfMovie::Impl*SwfMovie::Impl::active=nullptr;
std::recursive_mutex SwfMovie::Impl::scope_gate;
#include "../../swf_movie_combat_flash_v1.inc"
SwfMovie::SwfMovie():impl_(new Impl){}SwfMovie::~SwfMovie()=default;
std::shared_ptr<SwfControllerStorageV91> SwfMovie::controller_storage_v91()const noexcept{return impl_?impl_->controllers:nullptr;}
void SwfMovie::bind_controller_context_clear_v91(std::function<bool(std::string&)> fn){external_context_clear_v91_=std::move(fn);}
bool SwfMovie::unload_menu_resource_v91(SwfMenuUnloadServicesV91& services,std::string& e){
 std::unique_lock<std::recursive_mutex> gate(Impl::scope_gate,std::try_to_lock);
 if(!gate.owns_lock()||Impl::active){e="Source MenuFX unload requires quiescent facade dispatch";return false;}
 auto old=impl_;if(!old||!old->controllers||!services.owner){e="Required actual MenuFX constructor/resource owners";return false;}
 if(!services.renderer){e="Required RenderFX renderer singleton producer";return false;}
 // Prepare adapter backing before any destructive native callback. This
 // constructor borrows the SAME controller owner; it creates no extra C1.
 std::shared_ptr<Impl> empty;
 try{empty=std::make_shared<Impl>(old->controllers);
  //Original resource Unload does not clear receiver.fc; only its actual
  //SetEventListener producer may change the existing native handler slot.
  empty->event_listenerfc_v68=old->event_listenerfc_v68;
  empty->event_listener_lease_v68=old->event_listener_lease_v68;
  empty->event_listener_v68=old->event_listener_v68;empty->event_accept_v68=old->event_accept_v68;}
 catch(const std::exception& x){e=std::string("Unloaded facade preparation failed: ")+x.what();return false;}
 std::uintptr_t renderer{};if(!services.renderer(renderer,e))return false;
 if(renderer){if(!services.renderer_virtual_a4){e="Required actual renderer virtualA4 unload";return false;}if(!services.renderer_virtual_a4(renderer,e))return false;}
 // RenderFX::Unload's first four Controller.Reset calls.
 if(!old->controllers->release_all(e))return false;
 // Detach this facade's root/player graph. Backing leases intentionally keep
 // old alive until all source references and caller pins have unwound.
 // Prepared backing already retains the same cursor/enabled storage.
 if(action_script_)action_script_->release();action_script_.reset();viewport_.reset();impl_=std::move(empty);
 // Original tu_string.resize0 mutates this retained owner's filename.
 // A blank replacement facade alone does not perform this source commit.
 old->source_filename_v91.clear();
 old->controllers->fields().flags=(old->controllers->fields().flags&0xff000000u)|0xffffffu;
 if(input_){if(!input_->clear_context_v91(e))return false;}
 else if(old->controllers->fields().context){
  if(!external_context_clear_v91_){e="Required same external InputSession context release";return false;}
  if(!external_context_clear_v91_(e))return false;
 }
 old->controllers->fields().root=0;old->controllers->fields().context=0;
 if(!services.clear_render_context_and_flags){e="Required same RenderFX flags/context commit";return false;}
 if(!services.clear_render_context_and_flags(e))return false;
 // MenuFX::Unload repeats all four resets AFTER base graph/context unload.
 if(!old->controllers->release_all(e))return false;
 if(!services.clear_catalog_104){e="Required actual MenuFX catalog104 owner";return false;}
 if(!services.clear_catalog_104(e))return false;
 if(!services.clear_active_states_114){e="Required actual MenuFX active114 owner";return false;}
 if(!services.clear_active_states_114(e))return false;
 input_.reset();external_context_clear_v91_={};e.clear();return true;
}
SwfMovie::SwfMovie(SwfMovie&&)noexcept=default;SwfMovie&SwfMovie::operator=(SwfMovie&&)noexcept=default;
bool SwfMovie::load(const std::vector<std::string>&shared,const std::string&movie,const SwfServices&s,std::string&e){return load_resource_v98(shared,movie,s,false,e);}
bool SwfMovie::source_set_text_buffering_v98(bool enabled,std::string& e){
 auto p=impl_;if(!p||!p->player||!p->root){e="Required SAME native root text-buffering byte85 setter";return false;}
 Impl::Scope scope(p.get(),true);if(!scope.entered){e="SWF core busy";return false;}
 const auto& s=p->service;if(s.source_continue_v98&&!s.source_continue_v98(e))return false;
 if(s.source_text_buffering_v98&&!s.source_text_buffering_v98({p,p->player.get_ptr(),p->root.get_ptr()},enabled,e))return false;
 if(s.source_continue_v98&&!s.source_continue_v98(e))return false;
 p->root->source_set_text_buffering_v98(enabled);return true;
}
bool SwfMovie::load_source_resource_v98(const char* uri,const SwfServices& s,std::string& e){
 if(!uri||!*uri){e="Required original primary1 resource URI";return false;}
 return load_resource_v98({},uri,s,true,e);
}
bool SwfMovie::load_resource_v98(const std::vector<std::string>&shared,const std::string&movie,const SwfServices&s,bool source,std::string&e){
 if(source&&(!impl_||!impl_->controllers||impl_->player||impl_->root)){e="Original source movie C1 prefix requires genuine unload before replay";return false;}
 auto p=source?std::make_shared<Impl>(impl_->controllers):std::make_shared<Impl>();
 if(source){p->event_listenerfc_v68=impl_->event_listenerfc_v68;p->event_listener_lease_v68=impl_->event_listener_lease_v68;
  p->event_listener_v68=impl_->event_listener_v68;p->event_accept_v68=impl_->event_accept_v68;}
 p->service=s;p->source_filename_v91=movie;Impl::Scope scope(p.get());if(!scope.entered){e="SWF core busy";return false;}
 if(source&&s.source_continue_v98&&!s.source_continue_v98(e))return false;
 if(!s.read||!s.draw){e="SWF read/draw services required";return false;}p->player=new gameswf::player;p->player->set_separate_thread(false);gameswf::set_use_cache_files(false);
 if(source){
  //Original player publication precedes graph/startup/file callbacks. A later
  //failure retains this real prefix for the actual resource unload path.
  external_context_clear_v91_={};input_.reset();action_script_.reset();viewport_.reset();impl_=p;
  const auto slash=movie.find_last_of("/\\");if(slash!=std::string::npos)p->player->set_workdir(movie.substr(0,slash+1).c_str());
 }
 p->player->get_global()->set_member("NativeGetStringFromSymbol",gameswf::as_value(Impl::native_string));
 if(!s.native_actions.empty()&&(!s.native_owner||!s.native_action)){e="Required owned native AS callback provider unavailable";return false;}
 if(s.graph_start&&!s.native_owner){e="Required owned SWF graph startup provider unavailable";return false;}
 if(s.source_fscommand_v114&&!s.native_owner){e="Required owned source FSCommand provider unavailable";return false;}
 for(const auto&name:s.native_actions)if(name=="NativeGetStringFromSymbol"){e="NativeGetStringFromSymbol already registered by movie facade";return false;}
 // Keep all original caller ownership/duplicate checks above this point.
 if(!swf_source_startup_owned_v1(s)){SwfServices defaults;if(!source_movie_services_v1(s,defaults,e))return false;p->service=std::move(defaults);}
 const auto&startup=p->service;
 auto as=std::make_shared<SwfAsGraph>();if(source)action_script_=as;auto context=std::make_shared<Impl::AsContext>();
 context->graph=p;context->provider_owner=startup.native_owner;context->provider_context=startup.context;context->callback=startup.native_action;
 SwfAsServices as_services;as_services.context=context.get();as_services.owner=context;
 as_services.within_scope=Impl::AsContext::within;as_services.native_call=Impl::AsContext::native;as_services.failure=Impl::AsContext::failure;
 if(!as->bind({p,p->player.get_ptr(),nullptr},as_services,e)||!as->install_native(s.native_actions,e))return false;
 if(startup.graph_start&&!startup.graph_start(startup.context,{p,p->player.get_ptr(),nullptr},e))return false;
 if(!p->finish(e))return false;
 if(source&&s.source_continue_v98&&!s.source_continue_v98(e))return false;
 for(const auto&file:shared){auto r=p->player->load_file(file.c_str());if(!p->finish(e))return false;if(!r){e="SWF shared movie rejected: "+file;return false;}r->advance(0);if(!p->finish(e))return false;p->shared.push_back(r);}
 p->root=p->player->load_file(movie.c_str());if(!p->finish(e))return false;
 if(source&&s.source_continue_v98&&!s.source_continue_v98(e))return false;
 if(!p->root){e="SWF movie rejected: "+movie;return false;}
 if(!as->attach_root(p->root.get_ptr(),e)||!p->finish(e))return false;
 if(source&&s.source_continue_v98&&!s.source_continue_v98(e))return false;
 external_context_clear_v91_={};input_.reset();action_script_.reset();viewport_.reset();impl_=std::move(p);action_script_=std::move(as);return true;
}
bool SwfMovie::source_dispatch_pending_v115(bool& pending,std::string& e){
 std::unique_lock<std::recursive_mutex> gate(Impl::scope_gate,std::try_to_lock);
 if(!gate.owns_lock()){e="Movie retirement overlaps another actual provider thread";return false;}
 pending=Impl::active!=nullptr;e.clear();return true;
}
bool SwfMovie::source_set_event_listener_v68(std::uintptr_t receiver,std::weak_ptr<void> lease,
 std::function<bool(SwfEvent48&,std::string&)> callback,
 std::function<bool(SwfEvent48&,bool&,std::string&)> accept,std::string& e){
 auto owner=impl_;if(!owner){e="SetEventListener requires actual constructed RenderFX receiver";return false;}
 if(receiver&&(lease.expired()||!callback||!accept)){e="SetEventListener requires actual live native receiver";return false;}
 owner->event_listenerfc_v68=receiver;owner->event_listener_lease_v68=std::move(lease);owner->event_listener_v68=std::move(callback);owner->event_accept_v68=std::move(accept);e.clear();return true;
}
bool SwfMovie::source_raw_event_position_v68(std::int32_t cursor,int& x,int& y,std::string& e){
 auto owner=impl_;if(!owner||!owner->controllers||cursor<0||cursor>=4){e="Required actual source event cursor";return false;}
 if(input_)return input_raw_position(x,y,e);
 const auto& point=owner->controllers->fields().slots[cursor].cursor;
 if(!std::isfinite(point.x)||!std::isfinite(point.y)||point.x>=2147483648.f||point.x<-2147483648.f||point.y>=2147483648.f||point.y<-2147483648.f){e="Malformed actual source raw cursor";return false;}
 x=static_cast<int>(point.x);y=static_cast<int>(point.y);return true;
}
bool SwfMovie::source_event_focus_v68(std::uintptr_t id,std::uint32_t cursor,std::string& e){
 if(!input_){e="Required same source input SetFocus receiver";return false;}
 return input_->focus(reinterpret_cast<gameswf::character*>(id),cursor,e);
}
bool SwfMovie::advance(float seconds,std::string&e){auto owner=impl_;if(!owner||!owner->root||!std::isfinite(seconds)||seconds<0){e="Invalid SWF advance";return false;}Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}owner->root->advance(seconds);return owner->finish(e);}
bool SwfMovie::source_bind_fs_command_v114(decltype(SwfServices::source_fscommand_v114) callback,std::string& e){
 if(!impl_){e="FS handler binding requires actual constructed RenderFX";return false;}
 impl_->service.source_fscommand_v114=std::move(callback);e.clear();return true;
}
bool SwfMovie::display(std::int32_t x,std::int32_t y,std::int32_t w,std::int32_t h,std::string&e){auto owner=impl_;if(!owner||!owner->root||w<=0||h<=0){e="Invalid SWF viewport";return false;}Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}owner->root->set_display_viewport(x,y,w,h);owner->root->display();return owner->finish(e);}
bool SwfMovie::display_clip(const char*path,std::string&e){auto owner=impl_;if(!owner||!owner->root){e="SWF movie not loaded";return false;}auto&r=*owner->root;return display_clip(path,r.m_viewport_x0,r.m_viewport_y0,r.m_viewport_width,r.m_viewport_height,e);}
bool SwfMovie::display_clip(const char*path,std::int32_t x,std::int32_t y,std::int32_t w,std::int32_t h,std::string&e){auto owner=impl_;if(w<=0||h<=0){e="Invalid SWF viewport";return false;}Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}auto*c=owner->find(path);if(!c){e="SWF clip not found";return false;}auto&r=*owner->root;r.set_display_viewport(x,y,w,h);const auto&bounds=r.m_def->m_frame_size;owner->begin_display(r.m_background_color,x,y,w,h,bounds.m_x_min,bounds.m_x_max,bounds.m_y_min,bounds.m_y_max);c->display();owner->end_display();return owner->finish(e);}
bool SwfMovie::clip(const char*path,SwfClipInfo&out,std::string&e){auto owner=impl_;Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}auto*c=owner->find(path);if(!c){e="SWF clip not found";return false;}SwfClipInfo r;r.id=c->get_id();r.depth=c->get_depth();r.frame=c->get_current_frame();r.frames=c->get_frame_count();r.visible=c->get_visible();r.local=matrix(c->get_matrix());r.world=matrix(c->get_world_matrix());out=r;return owner->finish(e);}
bool SwfMovie::set_number(const char*path,double n,std::string&e){auto owner=impl_;Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}if(!owner->root||!path){e="Invalid SWF variable path";return false;}auto*env=owner->root->get_root_movie()->get_environment();if(!env){e="SWF environment missing";return false;}const ::array<gameswf::with_stack_entry> with;env->set_variable(path,gameswf::as_value(n),with);return owner->finish(e);}
bool SwfMovie::set_visible(const char*path,bool v,std::string&e){auto owner=impl_;Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}auto*c=owner->find(path);if(!c){e="SWF clip not found";return false;}c->set_visible(v);return owner->finish(e);}
bool SwfMovie::hide_menu_state_clips(std::vector<std::string>& names,std::string&e){
 auto owner=impl_;Impl::Scope scope(owner.get());
 if(!scope.entered){e="SWF core busy";return false;}
 if(!owner->root){e="Required menu state graph unavailable";return false;}
 std::vector<gameswf::character*> clips;
 // MenuManager::PostLoad finds all names containing menu_ with mask=0;
 // RegisterState hides each real character before state activation.
 std::function<void(gameswf::character*)> collect=[&](gameswf::character*c){
  if(std::strstr(c->get_name().c_str(),"menu_"))clips.push_back(c);
  if(c->is(gameswf::sprite_instance::m_class_id)){
   auto*s=static_cast<gameswf::sprite_instance*>(c);
   for(int i=0;i<s->m_display_list.size();++i)collect(s->m_display_list.get_character(i));
  }
 };
 collect(owner->root->get_root_movie());
 std::vector<std::string> result;
 for(auto*c:clips){result.emplace_back(c->get_name().c_str());c->set_visible(false);}
 if(!owner->finish(e))return false;
 names=std::move(result);return true;
}
bool SwfMovie::source_reset_fonts_v119(std::string& e){
 auto owner=impl_;if(!owner||!owner->root){e="Required loaded RenderFX root for ClearFonts";return false;}
 // NativeSetOptions can arrive from this movie's ActionScript dispatch;
 // ResetFonts synchronously re-enters the same RenderFX owner in source.
 Impl::Scope scope(owner.get(),true);if(!scope.entered){e="SWF core busy";return false;}
 std::function<void(gameswf::character*)> clear=[&](gameswf::character* c){
  if(c->is(gameswf::edit_text_character::m_class_id))
   static_cast<gameswf::edit_text_character*>(c)->set_text_value("");
  if(c->is(gameswf::sprite_instance::m_class_id)){
   auto* sprite=static_cast<gameswf::sprite_instance*>(c);
   for(int i=0;i<sprite->m_display_list.size();++i)clear(sprite->m_display_list.get_character(i));
  }
 };
 clear(owner->root->get_root_movie());
 if(auto platform=SwfTextFontPlatformV1::for_player(owner->player.get_ptr()))
  if(!platform->source_reset_fonts_v119(e))return false;
 return owner->finish(e);
}
bool SwfMovie::connect_viewport(const ViewportState64& seed,const SwfViewportDriver& driver,std::string&e){
 auto owner=impl_;if(!owner||!owner->root){e="SWF movie not loaded";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 auto connection=std::make_shared<SwfViewportConnection>();
 if(!connection->bind({owner,owner->root.get_ptr()},seed,driver,e))return false;
 viewport_=std::move(connection);return owner->finish(e);
}
bool SwfMovie::update_viewport(FlashCamera40& camera,std::string&e){
 auto owner=impl_;auto connection=viewport_;if(!owner||!connection){e="Required source viewport connection unavailable";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 return connection->camera_update(camera,e)&&owner->finish(e);
}
bool SwfMovie::set_source_bounds(const std::int32_t xywh[4],std::int32_t aspect_mode,std::string&e){
 auto owner=impl_;auto connection=viewport_;if(!owner||!connection){e="Required source viewport connection unavailable";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 return connection->set_bounds(xywh,aspect_mode,e)&&owner->finish(e);
}
bool SwfMovie::display_source_clip(const char* path,std::string&e){
 auto owner=impl_;auto connection=viewport_;if(!owner||!connection){e="Required source viewport connection unavailable";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 auto* clip=owner->find(path);if(!clip){e="SWF clip not found";return false;}
 float rectangle[4];if(!connection->display_rectangle(rectangle,e))return false;
 const auto& v=connection->state().viewport;
 owner->begin_display(owner->root->m_background_color,v[0],v[1],v[2],v[3],rectangle[0],rectangle[1],rectangle[2],rectangle[3]);
 clip->display();owner->end_display();return owner->finish(e);
}
#include "../../swf_movie_stage_clip_v5.inc"
bool SwfMovie::screen_to_logical(float point[2],std::string&e){
 auto owner=impl_;auto connection=viewport_;if(!owner||!connection){e="Required source viewport connection unavailable";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 return connection->screen_to_logical(point,e)&&owner->finish(e);
}
bool SwfMovie::hud_bind(const char* path,const char* digest,SwfHudClip& handle,std::string&e){
 auto owner=impl_;if(!owner||!owner->root){e="SWF movie not loaded";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 HudSpriteCoreBindingV1 candidate;
 if(!bind_hud_sprite_v1(owner->find(path),digest,candidate,e)||!owner->finish(e))return false;
 bool present=false;for(auto&clip:owner->hud_pins)if(clip.get_ptr()==candidate.sprite)present=true;
 if(!present)owner->hud_pins.emplace_back(candidate.sprite);
 handle.binding_=candidate;handle.owner_=owner;return true;
}
bool SwfMovie::hud_goto(const SwfHudClip& handle,std::int32_t frame,const HudSpriteCoreServices& services,std::string&e){
 const auto& binding=handle.binding_;
 auto owner=impl_;bool retained=false;if(owner)for(auto&clip:owner->hud_pins)if(clip.get_ptr()==binding.sprite)retained=true;
 if(!retained||handle.owner_.get()!=owner.get()||!owner->root||binding.root!=owner->root.get_ptr()){e="HUD binding belongs to a different retained movie";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 return hud_core_goto_v1(binding,frame,services,e)>=0&&owner->finish(e);
}
bool SwfMovie::hud_play(const SwfHudClip& handle,std::int32_t state,const HudSpriteCoreServices& services,std::string&e){
 const auto& binding=handle.binding_;
 auto owner=impl_;bool retained=false;if(owner)for(auto&clip:owner->hud_pins)if(clip.get_ptr()==binding.sprite)retained=true;
 if(!retained||handle.owner_.get()!=owner.get()||!owner->root||binding.root!=owner->root.get_ptr()){e="HUD binding belongs to a different retained movie";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 return hud_core_play_v1(binding,state,services,e)>=0&&owner->finish(e);
}
bool SwfMovie::action_script(void* context,bool (*apply)(void*,SwfAsGraph&,std::string&),std::string& e){
 auto owner=impl_;auto as=action_script_;if(!owner||!owner->root||!as||!apply){e="Required retained AS movie/batch unavailable";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 try{return apply(context,*as,e)&&owner->finish(e);}catch(const std::exception& exception){e=exception.what();return false;}
}
bool SwfMovie::connect_input(const char* path,std::shared_ptr<SwfInputHistory> history,std::uint32_t flags,
 std::uint32_t& selection,const SwfViewportDriver& driver,const SwfInputCoreServices& services,std::string& e){
 auto owner=impl_;if(!owner||!owner->root||!viewport_||input_){e="Input connection requires a retained viewport and fresh owner";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 auto* context=owner->find(path);if(!context){e="Source input context clip absent";return false;}
 auto input=std::make_shared<SwfInputConnection>();
 if(!input->bind({owner,owner->root.get_ptr()},viewport_->state(),driver,std::move(history),context,flags,selection,services,e,owner->controllers,viewport_))return false;
 input_=std::move(input);return owner->finish(e);
}
bool SwfMovie::advance_frames(std::int32_t milliseconds,SwfFrameConnection& frames,std::string& e){
 auto owner=impl_;if(!owner||!owner->root||milliseconds<0){e="Required retained source frame/time unavailable";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 try{return frames.advance(owner->root.get_ptr(),float(milliseconds)*.001f,false,e)&&owner->finish(e);}
 catch(const std::exception& exception){e=exception.what();return false;}
}
bool SwfMovie::menu_action_script(void* context,bool (*apply)(void*,SwfAsGraph&,std::string&),std::string& e){
 auto owner=impl_;auto as=action_script_;if(!owner||!owner->root||!as||!apply){e="Required retained menu AS movie/batch unavailable";return false;}
 Impl::Scope scope(owner.get(),true);if(!scope.entered){e="SWF core busy";return false;}
 try{return apply(context,*as,e)&&owner->finish(e);}catch(const std::exception& exception){e=exception.what();return false;}
}
bool SwfMovie::input_rectangle(const std::int32_t xywh[4],std::string& e){
 auto owner=impl_;auto input=input_;if(!owner||!input){e="Source input owner unavailable";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 return input->viewport_rectangle(xywh,e)&&owner->finish(e);
}
bool SwfMovie::menu_display_callback(const char* path,void* context,
 bool (*draw)(void*,const SwfDraw&,std::string&),std::string& e){
 auto owner=impl_;Impl::Scope scope(owner.get(),true);
 if(!scope.entered){e="SWF core busy";return false;}
 auto* character=owner->find(path);if(!character){e="Display callback character absent";return false;}
 for(auto& h:owner->display_hooks)if(h->character.get_ptr()==character){
  h->context=context;h->draw=draw;return owner->finish(e);
 }
 auto h=std::make_unique<Impl::DisplayHook>();h->owner=owner.get();h->character=character;
 h->context=context;h->draw=draw;character->set_display_callback(Impl::DisplayHook::display,h.get());
 owner->display_hooks.push_back(std::move(h));return owner->finish(e);
}
bool SwfMovie::menu_input_context(const char* path,std::string& e){
 auto owner=impl_;auto input=input_;if(!owner||!input){e="Source input owner unavailable";return false;}
 Impl::Scope scope(owner.get(),true);if(!scope.entered){e="SWF core busy";return false;}
 auto* context=owner->find(path);if(!context){e="Source input context clip absent";return false;}
 return input->set_context(context,e)&&owner->finish(e);
}
bool SwfMovie::menu_input_behavior(std::uint32_t flags,std::string& e){
 auto owner=impl_;auto input=input_;if(!owner||!input){e="Source input owner unavailable";return false;}
 Impl::Scope scope(owner.get(),true);if(!scope.entered){e="SWF core busy";return false;}
 return input->set_flags(flags,e)&&owner->finish(e);
}
bool SwfMovie::input_cursor(const SwfCursor16& cursor,std::string& e){
 return input_cursor_slot_v120(cursor,0,e);
}
bool SwfMovie::input_cursor_slot_v120(const SwfCursor16& cursor,std::uint32_t slot,std::string& e){
 auto owner=impl_;auto input=input_;if(!owner||!input){e="Source input owner unavailable";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 try{return input->cursor(cursor,slot,e)&&owner->finish(e);}catch(const std::exception& x){e=x.what();return false;}
}
bool SwfMovie::source_update_v93(std::int32_t ms,bool flag,std::string& e){
 auto owner=impl_;auto input=input_;if(!owner||!owner->root||!owner->controllers){e="Required same source RenderFX graph/controller owner";return false;}
 Impl::Scope scope(owner.get(),true);if(!scope.entered){e="SWF core busy";return false;}
 try{
  if(input)return input->update(ms,flag,e)&&owner->finish(e);
  SwfSourceFrameBorrowV1 frames;
  if(!source_movie_frame_borrow_v1(owner->service,frames,e)||!frames.frames||!frames.history){if(e.empty())e="Required actual retained source frame/history owner";return false;}
  struct Update {
   std::shared_ptr<Impl> lease;Impl& movie;SwfSourceFrameBorrowV1& frames;std::string& error;SwfInputCharacter32 character{};
   gameswf::gc_ptr<gameswf::root> temporary_root{};
   static int invoke(void* raw,SwfInputState288*,const SwfInputRequest64* q,SwfInputResponse56* out){
    auto& t=*static_cast<Update*>(raw);
    switch(q->operation){
     case SwfInputOperation::retain:case SwfInputOperation::drop:
      // update_pending borrows a temporary strong root. That identity denotes
      // gameswf::root, not character; pin it through the already retained graph.
      if(q->operation==SwfInputOperation::retain){
       if(t.temporary_root||q->character!=reinterpret_cast<std::uintptr_t>(t.movie.root.get_ptr())){t.error="Required actual source pending temporary root identity";return 0;}
       t.temporary_root=t.movie.root;
      }else{
       if(!t.temporary_root||q->character!=reinterpret_cast<std::uintptr_t>(t.temporary_root.get_ptr())){t.error="Required captured source pending root drop";return 0;}
       t.temporary_root=nullptr;
      }
      return 1;
     case SwfInputOperation::advance:return t.frames.frames->advance(t.movie.root.get_ptr(),q->values[0],q->integer!=0,t.error)?1:0;
     case SwfInputOperation::play_state:{auto* c=reinterpret_cast<gameswf::character*>(q->character);if(!c)return 0;out->result=c->get_play_state();return 1;}
     case SwfInputOperation::character:{auto* c=reinterpret_cast<gameswf::character*>(q->character);if(!c)return 0;t.character.name=c->get_name().c_str();out->character=&t.character;return 1;}
     case SwfInputOperation::native_event:t.error="Required actual MenuFX native pending-event receiver";return 0;
     case SwfInputOperation::as_method:{bool called{};return swf_event_method({t.lease,t.movie.root.get_ptr()},reinterpret_cast<gameswf::character*>(q->character),q->name,called,t.error)?1:0;}
     default:t.error="Required actual source pending-click operation";return 0;
    }
   }
  } update{owner,*owner,frames,e};
  auto& state=owner->controllers->fields();const auto old_root=state.root;
  state.root=reinterpret_cast<std::uintptr_t>(owner->root.get_ptr());
  struct Restore {std::uintptr_t& field;std::uintptr_t prior;~Restore(){field=prior;}} restore{state.root,old_root};
  std::uint32_t selection{};SwfInputServices16 services{&update,Update::invoke};
  const int result=dh2_ui_swf_update_pending(&state,ms,flag,&selection,&services);
  if(result){if(e.empty())e="Required source pending-click continuation";return false;}
  return owner->finish(e);
 }catch(const std::exception& x){e=x.what();return false;}
}
bool SwfMovie::input_advance(std::int32_t ms,std::string& e){
 auto owner=impl_;auto input=input_;if(!owner||!input||ms<0){e="Source input owner/time unavailable";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 try{return input->update(ms,false,e)&&owner->finish(e);}catch(const std::exception& x){e=x.what();return false;}
}
bool SwfMovie::input_cancel(float x,float y,std::string& e){
 auto owner=impl_;auto input=input_;if(!owner||!input){e="Source input owner unavailable";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 if(!input->enable(false,0,e))return false;
 const bool cleared=input->cursor({x,y,0.f,0},0,e)&&input->reset_focus(0,e);
 std::string restore;const bool enabled=input->enable(true,0,restore);
 if(!cleared)return false;if(!enabled){e=restore;return false;}
 return owner->finish(e);
}
bool SwfMovie::input_raw_position(int& x,int& y,std::string& e){
 // Called synchronously by the bound native event receiver inside this
 // movie's existing Scope. Entering a second facade Scope would be reentry.
 if(Impl::active!=impl_.get()||!input_){e="Raw cursor read outside retained input scope";return false;}
 float xy[2]{};std::int32_t index=0;if(!input_->raw_cursor(xy,index,e))return false;
 x=static_cast<int>(xy[0]);y=static_cast<int>(xy[1]);return true;
}
gameswf::font*SwfMovie::borrowed_font(std::int32_t id)const{return impl_&&impl_->root?impl_->root->m_def->get_font(id):nullptr;}
const std::vector<std::string>&SwfMovie::diagnostics()const{return impl_->messages;}
std::uintptr_t SwfMovie::player_identity()const noexcept{auto owner=impl_;return owner?reinterpret_cast<std::uintptr_t>(owner->player.get_ptr()):0;}
bool SwfMovie::source_register_native_actions_v119(const std::vector<std::string>& names,std::string& e){
 auto owner=impl_;if(!owner||!owner->player||!action_script_||!owner->service.native_owner||!owner->service.native_action){e="Required actual source native AS registration provider";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 std::vector<std::string> actual;
 for(const auto& name:names)if(name!="NativeGetStringFromSymbol"&&
    std::find(owner->service.native_actions.begin(),owner->service.native_actions.end(),name)==owner->service.native_actions.end())actual.push_back(name);
 //The facade's existing NativeGetStringFromSymbol is the real process text
 //transport, installed at player C1. Preserve it and the already installed
 //movie-bootstrap callbacks when original phase25 reaches the full name table.
 //Append only after successful installation so retries remain idempotent.
 if(!action_script_->install_native(actual,e)||!owner->finish(e))return false;
 owner->service.native_actions.insert(owner->service.native_actions.end(),actual.begin(),actual.end());
 return true;
}
bool SwfMovie::source_preload_glyphs_v119(const char* path,const char* codes,std::string& e){
 auto owner=impl_;if(!owner||!owner->root||!path||!codes){e="Required actual source glyph/movie request";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 auto* character=owner->find(path);
 if(!character||!character->is(gameswf::edit_text_character::m_class_id))return owner->finish(e); //41724c genuine miss/wrong-type.
 auto& field=*static_cast<gameswf::edit_text_character*>(character);
 auto platform=SwfTextFontPlatformV1::for_player(owner->player.get_ptr());if(!platform){e="Required SAME preload glyph platform";return false;}
 auto services=platform->layout_services({});if(!services.glyph){e="Required real preload glyph raster/cache";return false;}
 auto preload=[&](gameswf::font* font){
  if(!font){e="Required actual preload edit-text font";return false;}auto original=platform->font(font,e);if(!original)return false;
  std::shared_ptr<text_v1::Font> projection;
  //7aafac constructs a new descriptor from NAME/bold/italic rather than
  //retaining the field's embedded glyph/kerning-definition cache.
  if(!services.clone_font||!services.clone_font(original,projection,e)||!projection)return false;
  const auto size=static_cast<std::int32_t>(field.m_text_height/20.f);
  for(const unsigned char* p=reinterpret_cast<const unsigned char*>(codes);*p;++p){text_v1::Glyph glyph;bool found{};
   if(!services.glyph(projection,*p,size,glyph,found,e))return false;
  }return true;
 };
 //Native glyph materialization is shared with normal source text; no display
 //or authored field text mutation is needed for the shipping ASCII packs.
 for(int i=0;i<field.m_text_glyph_records.size();++i)if(!preload(field.m_text_glyph_records[i].m_style.m_font))return false;
 return preload(field.m_font.get_ptr())&&owner->finish(e);
}
} // namespace dh2::ui

