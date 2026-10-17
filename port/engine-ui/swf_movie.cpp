#include "swf_movie.hpp"
#include "swf_frame_connection.hpp"
#include "authored_animation_pool_v1.hpp"
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
#include <algorithm>
#include <limits>
#include <exception>
#include <set>
#include <array>
#include <cstring>
#include <functional>
#include <mutex>
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
 gameswf::gc_ptr<gameswf::root> root;std::vector<gameswf::gc_ptr<gameswf::root>> shared;
 std::vector<gameswf::gc_ptr<gameswf::sprite_instance>> hud_pins;
 struct AuthoredAnimationStyle {
  std::string name;gameswf::gc_ptr<gameswf::sprite_instance> source;
  authored_animation_pool_v1::StylePool pool;
  std::array<gameswf::gc_ptr<gameswf::sprite_instance>,8> clips{};
  std::array<gameswf::gc_ptr<gameswf::edit_text_character>,8> text_fields{};
 };
 struct AuthoredPlayback {bool active{};std::size_t style{},clone{};float elapsed_ms{},x{},y{};std::uint32_t frame{};std::string text;std::uint8_t r{},g{},b{},a{255};};
 authored_animation_pool_v1::State authored_pool{};
 std::array<AuthoredPlayback,authored_animation_pool_v1::playback_context_count> authored_playbacks{};
 std::vector<AuthoredAnimationStyle> authored_styles;
 std::vector<std::string> messages;std::string failure;SwfDraw state{};
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
  Scope(Impl*i,bool menu_dispatch=false):gate(scope_gate,std::try_to_lock),p(i),
   entered(i&&gate.owns_lock()&&(!active||menu_dispatch)){
   if(entered){
    previous_active=active;previous_glyphs=gameswf::get_glyph_provider();
    previous_renderer=gameswf::get_render_handler();
    active=p;if(p->scope_depth++==0)p->failure.clear();
    gameswf::set_glyph_provider(p->service.glyphs);gameswf::set_render_handler(p);
    // These callbacks dispatch through active, so they remain the same
    // trampolines throughout a nested renderer chain.
    gameswf::register_file_opener_callback(open);gameswf::register_log_callback(log);
    gameswf::register_bitmap_substitution_callback(substitute);
   }
  }
  ~Scope(){if(entered){
   --p->scope_depth;active=previous_active;
   gameswf::set_glyph_provider(previous_glyphs);gameswf::set_render_handler(previous_renderer);
   gameswf::register_bitmap_substitution_callback(previous_active?substitute:nullptr);
  }}
 };
 void fail(const std::string&s){if(failure.empty())failure=s.empty()?"Required SWF provider rejected delivery":s;}
 void advance_authored_animations(float seconds){
  if(!root||!std::isfinite(seconds)||seconds<0)return;
  // The retained live-player HUD path runs during a source Level gameplay
  // session. FlashAnimManager::Update selects its fixed 33 ms cadence there.
  constexpr float frame_ms=33.f;
  for(std::size_t i=0;i<authored_playbacks.size();++i){auto& playback=authored_playbacks[i];
   if(!playback.active||playback.style>=authored_styles.size())continue;
   auto& style=authored_styles[playback.style];if(playback.clone>=style.clips.size())continue;
   auto* clip=style.clips[playback.clone].get_ptr();if(!clip)continue;
   playback.elapsed_ms+=seconds*1000.f;
   while(playback.elapsed_ms>frame_ms){playback.elapsed_ms-=frame_ms;++playback.frame;
    if(playback.frame>=static_cast<std::uint32_t>(std::max(0,clip->get_frame_count()))){
     authored_animation_pool_v1::stop(authored_pool,style.pool, i);playback.active=false;break;
    }
   }
  }
 }
 bool finish(std::string&e){if(!failure.empty()){e=failure;return false;}e.clear();return true;}
 static void log(bool error,const char*s){if(!active)return;active->messages.emplace_back(s?s:"");if(active->service.diagnostic)active->service.diagnostic(active->service.context,error,s?s:"");}
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
 void end_display()override{SwfDraw d;d.kind=SwfDraw::end;emit(d);}
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
SwfMovie::SwfMovie():impl_(new Impl){}SwfMovie::~SwfMovie()=default;
SwfMovie::SwfMovie(SwfMovie&&)noexcept=default;SwfMovie&SwfMovie::operator=(SwfMovie&&)noexcept=default;
bool SwfMovie::load(const std::vector<std::string>&shared,const std::string&movie,const SwfServices&s,std::string&e){auto p=std::make_shared<Impl>();p->service=s;Impl::Scope scope(p.get());if(!scope.entered){e="SWF core busy";return false;}
 if(!s.read||!s.draw){e="SWF read/draw services required";return false;}p->player=new gameswf::player;p->player->set_separate_thread(false);gameswf::set_use_cache_files(false);
 p->player->get_global()->set_member("NativeGetStringFromSymbol",gameswf::as_value(Impl::native_string));
 if(!s.native_actions.empty()&&(!s.native_owner||!s.native_action)){e="Required owned native AS callback provider unavailable";return false;}
 if(s.graph_start&&!s.native_owner){e="Required owned SWF graph startup provider unavailable";return false;}
 for(const auto&name:s.native_actions)if(name=="NativeGetStringFromSymbol"){e="NativeGetStringFromSymbol already registered by movie facade";return false;}
 auto as=std::make_shared<SwfAsGraph>();auto context=std::make_shared<Impl::AsContext>();
 context->graph=p;context->provider_owner=s.native_owner;context->provider_context=s.context;context->callback=s.native_action;
 SwfAsServices as_services;as_services.context=context.get();as_services.owner=context;
 as_services.within_scope=Impl::AsContext::within;as_services.native_call=Impl::AsContext::native;as_services.failure=Impl::AsContext::failure;
 if(!as->bind({p,p->player.get_ptr(),nullptr},as_services,e)||!as->install_native(s.native_actions,e))return false;
 if(s.graph_start&&!s.graph_start(s.context,{p,p->player.get_ptr(),nullptr},e))return false;
 for(const auto&file:shared){auto r=p->player->load_file(file.c_str());if(!r){e="SWF shared movie rejected: "+file;return false;}r->advance(0);gameswf::as_value value;bool global_com=p->player->get_global()->get_member("com",&value);bool root_com=r->get_root_movie()->get_member("com",&value);p->messages.push_back(std::string("Shared namespace: global com=")+(global_com?"yes":"no")+", root com="+(root_com?"yes":"no"));if(s.diagnostic)s.diagnostic(s.context,false,p->messages.back().c_str());gameswf::as_object* package=p->player->get_global();for(const char*part:{"com","gameloft","components","ui"}){gameswf::as_value item;bool found=package&&package->get_member(part,&item);package=found?item.to_object():nullptr;std::string diagnostic=std::string("Shared package ")+part+": "+(package?"object":"missing");if(package){for(auto iter=package->m_members.begin();iter!=package->m_members.end();++iter)diagnostic+=" "+std::string(iter->first.c_str());}if(s.diagnostic)s.diagnostic(s.context,false,diagnostic.c_str());}p->shared.push_back(r);}
 p->root=p->player->load_file(movie.c_str());if(!p->root){e="SWF movie rejected: "+movie;return false;}
 if(!as->attach_root(p->root.get_ptr(),e)||!p->finish(e))return false;
 input_.reset();action_script_.reset();viewport_.reset();impl_=std::move(p);action_script_=std::move(as);return true;
}
bool SwfMovie::advance(float seconds,std::string&e){auto owner=impl_;if(!owner||!owner->root||!std::isfinite(seconds)||seconds<0){e="Invalid SWF advance";return false;}Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}owner->root->advance(seconds);owner->advance_authored_animations(seconds);return owner->finish(e);}
bool SwfMovie::display(std::int32_t x,std::int32_t y,std::int32_t w,std::int32_t h,std::string&e){auto owner=impl_;if(!owner||!owner->root||w<=0||h<=0){e="Invalid SWF viewport";return false;}Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}owner->root->set_display_viewport(x,y,w,h);owner->root->display();return owner->finish(e);}
bool SwfMovie::display_clip(const char*path,std::string&e){auto owner=impl_;if(!owner||!owner->root){e="SWF movie not loaded";return false;}auto&r=*owner->root;return display_clip(path,r.m_viewport_x0,r.m_viewport_y0,r.m_viewport_width,r.m_viewport_height,e);}
bool SwfMovie::display_clip(const char*path,std::int32_t x,std::int32_t y,std::int32_t w,std::int32_t h,std::string&e){auto owner=impl_;if(w<=0||h<=0){e="Invalid SWF viewport";return false;}Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}auto*c=owner->find(path);if(!c){e="SWF clip not found";return false;}auto&r=*owner->root;r.set_display_viewport(x,y,w,h);const auto&bounds=r.m_def->m_frame_size;owner->begin_display(r.m_background_color,x,y,w,h,bounds.m_x_min,bounds.m_x_max,bounds.m_y_min,bounds.m_y_max);c->display();owner->end_display();return owner->finish(e);}
bool SwfMovie::clip(const char*path,SwfClipInfo&out,std::string&e){auto owner=impl_;Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}auto*c=owner->find(path);if(!c){e="SWF clip not found";return false;}SwfClipInfo r;r.id=c->get_id();r.depth=c->get_depth();r.frame=c->get_current_frame();r.frames=c->get_frame_count();r.visible=c->get_visible();r.local=matrix(c->get_matrix());r.world=matrix(c->get_world_matrix());out=r;return owner->finish(e);}
bool SwfMovie::set_number(const char*path,double n,std::string&e){auto owner=impl_;Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}if(!owner->root||!path){e="Invalid SWF variable path";return false;}auto*env=owner->root->get_root_movie()->get_environment();if(!env){e="SWF environment missing";return false;}const ::array<gameswf::with_stack_entry> with;env->set_variable(path,gameswf::as_value(n),with);return owner->finish(e);}
bool SwfMovie::set_visible(const char*path,bool v,std::string&e){auto owner=impl_;Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}auto*c=owner->find(path);if(!c){e="SWF clip not found";return false;}c->set_visible(v);return owner->finish(e);}
bool SwfMovie::authored_animation_style_id(const char* style,std::int32_t& id,std::string& e){
 auto owner=impl_;if(!owner||!owner->root||!style){e="Source FlashAnimManager style query is invalid";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 std::vector<std::string> names;
 std::function<void(gameswf::character*)> collect=[&](gameswf::character* c){
  if(!c)return;const char* name=c->get_name().c_str();if(std::strncmp(name,"anim_",5)==0)names.emplace_back(name);
  if(auto* sprite=dynamic_cast<gameswf::sprite_instance*>(c))
   for(int i=0;i<sprite->m_display_list.size();++i)collect(sprite->m_display_list.get_character(i));
 };
 collect(owner->root->get_root_movie());
 for(std::size_t i=0;i<names.size();++i)if(names[i]==style){
  if(i>std::size_t(std::numeric_limits<std::int32_t>::max())){e="Source animation id exceeds signed range";return false;}
  id=static_cast<std::int32_t>(i);return owner->finish(e);
 }
 e=std::string("Source FlashAnimManager animation style is absent: ")+style;return false;
}
bool SwfMovie::play_authored_animation_text(const char* style,std::uint32_t slot,
 float x,float y,const char* text,std::uint8_t r,std::uint8_t g,std::uint8_t b,
 std::uint8_t a,std::string& e){
 auto owner=impl_;if(!owner||!owner->root||!style||!text||
    (slot>=authored_animation_pool_v1::playback_context_count&&slot!=std::numeric_limits<std::uint32_t>::max())||
    !std::isfinite(x)||!std::isfinite(y)){e="Invalid authored animation request";return false;}
 static constexpr const char* styles[]={"anim_sct_normaldamage","anim_sct_xp","anim_sct_normaldamageleft",
  "anim_sct_normaldamageright","anim_sct_crit","anim_sct_critleft","anim_sct_critright",
  "anim_sct_block","anim_sct_stun","anim_sct_dot"};
 bool known=false;for(const auto* candidate:styles)known|=std::strcmp(style,candidate)==0;
 if(!known){e="Unknown source scrolling-combat-text style";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 auto* source=dynamic_cast<gameswf::sprite_instance*>(owner->find(style));
 if(!source){e=std::string("Source scrolling animation style instance unavailable: ")+style;return false;}
 std::size_t style_id=owner->authored_styles.size();
 for(std::size_t i=0;i<owner->authored_styles.size();++i)
  if(owner->authored_styles[i].name==style){style_id=i;break;}
 if(style_id==owner->authored_styles.size()){
  Impl::AuthoredAnimationStyle record;record.name=style;record.source=source;
  owner->authored_styles.push_back(std::move(record));
 }
 auto& animation_style=owner->authored_styles[style_id];
 if(animation_style.source.get_ptr()!=source){e="Source animation style identity changed while movie remained loaded";return false;}
 std::size_t context_id{},clone_id{};bool create_clone{};
 if(slot==std::numeric_limits<std::uint32_t>::max()){
  if(!authored_animation_pool_v1::acquire(owner->authored_pool,animation_style.pool,
       static_cast<std::uint32_t>(style_id),context_id,clone_id,create_clone)){
   e="Source FlashAnimManager has no free playback context";return false;
  }
 }else{
  context_id=slot;
  // The original manager chooses contexts internally; explicit indices are a
  // compatibility/testing seam. Clone selection still follows source policy.
  if(!authored_animation_pool_v1::acquire_at(owner->authored_pool,animation_style.pool,
       static_cast<std::uint32_t>(style_id),context_id,clone_id,create_clone)){
   e="Source FlashAnimManager has no free playback context";return false;
  }
 }
 auto& clip=animation_style.clips[clone_id];auto& field=animation_style.text_fields[clone_id];
 auto abandon=[&](){authored_animation_pool_v1::stop(owner->authored_pool,animation_style.pool,context_id);
  if(create_clone)animation_style.pool.clones[clone_id].created=false;};
 if(create_clone){
  const std::string name="_clone_"+std::to_string(clone_id);
  auto* parent=dynamic_cast<gameswf::sprite_instance*>(source->get_parent());
  if(!parent){abandon();e="Source animation style has no parent movieclip";return false;}
  auto* clone=dynamic_cast<gameswf::sprite_instance*>(source->clone_display_object(
      tu_string(name.c_str()),parent->get_highest_depth()+1));
  if(!clone){abandon();e="Source animation clone failed";return false;}
  auto* text_field=dynamic_cast<gameswf::edit_text_character*>(clone->find_target(gameswf::as_value("_text")));
  if(!text_field){if(auto* parent=dynamic_cast<gameswf::sprite_instance*>(clone->get_parent()))parent->remove_display_object(clone);abandon();e="Source scrolling animation _text field unavailable";return false;}
  clip=clone;field=text_field;
 }
 if(!clip||!field){abandon();e="Source animation clone record is inconsistent";return false;}
 auto& playback=owner->authored_playbacks[context_id];playback={};playback.active=true;
 playback.style=style_id;playback.clone=clone_id;playback.x=x;playback.y=y;
 playback.text=text;playback.r=r;playback.g=g;playback.b=b;playback.a=a;
 field->m_color.set(r,g,b,a);field->set_text_value(tu_string(text));
 return owner->finish(e);
}
bool SwfMovie::display_authored_animations(std::string& e){
 auto owner=impl_;auto connection=viewport_;
 if(!owner||!owner->root||!connection){e="Required retained combat-text movie/viewport unavailable";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 float rectangle[4];if(!connection->display_rectangle(rectangle,e))return false;
 const auto& v=connection->state().viewport;
 for(std::size_t i=0;i<owner->authored_playbacks.size();++i){
  const auto& playback=owner->authored_playbacks[i];if(!playback.active||playback.style>=owner->authored_styles.size())continue;
  const auto& style=owner->authored_styles[playback.style];if(playback.clone>=style.clips.size())continue;
  auto* clip=style.clips[playback.clone].get_ptr();auto* field=style.text_fields[playback.clone].get_ptr();if(!clip||!field)continue;
  if(auto* parent=dynamic_cast<gameswf::sprite_instance*>(clip->get_parent())){
   gameswf::matrix transform=style.source->get_matrix();transform.m_[0][2]+=playback.x;transform.m_[1][2]+=playback.y;
   gameswf::cxform color;parent->move_display_object(clip->get_depth(),false,color,true,transform,
       clip->get_ratio(),clip->get_clip_depth(),clip->get_blend_mode());
  }
  clip->goto_frame(static_cast<int>(playback.frame));field->m_color.set(playback.r,playback.g,playback.b,playback.a);
  field->set_text_value(tu_string(playback.text.c_str()));
  owner->begin_display(owner->root->m_background_color,v[0],v[1],v[2],v[3],rectangle[0],rectangle[1],rectangle[2],rectangle[3]);
  const bool was_visible=clip->get_visible();clip->set_visible(true);clip->display();clip->set_visible(was_visible);owner->end_display();
 }
 return owner->finish(e);
}
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
bool SwfMovie::display_source_clip(const char* path,std::string&e){
 auto owner=impl_;auto connection=viewport_;if(!owner||!connection){e="Required source viewport connection unavailable";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 auto* clip=owner->find(path);if(!clip){e="SWF clip not found";return false;}
 float rectangle[4];if(!connection->display_rectangle(rectangle,e))return false;
 const auto& v=connection->state().viewport;
 owner->begin_display(owner->root->m_background_color,v[0],v[1],v[2],v[3],rectangle[0],rectangle[1],rectangle[2],rectangle[3]);
 clip->display();owner->end_display();return owner->finish(e);
}
bool SwfMovie::screen_to_logical(float point[2],std::string&e){
 auto owner=impl_;auto connection=viewport_;if(!owner||!connection){e="Required source viewport connection unavailable";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 return connection->screen_to_logical(point,e)&&owner->finish(e);
}
bool SwfMovie::source_display_rectangle(float rectangle[4],std::int32_t viewport[4],std::string&e){
 auto owner=impl_;auto connection=viewport_;
 if(!owner||!owner->root||!connection||!rectangle||!viewport){e="Required retained source viewport unavailable";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 if(!connection->display_rectangle(rectangle,e))return false;
 const auto& source=connection->state().viewport;
 for(unsigned i=0;i<4;++i)viewport[i]=source[i];
 return owner->finish(e);
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
 if(!input->bind({owner,owner->root.get_ptr()},viewport_->state(),driver,std::move(history),context,flags,selection,services,e))return false;
 input_=std::move(input);return owner->finish(e);
}
bool SwfMovie::advance_frames(std::int32_t milliseconds,SwfFrameConnection& frames,std::string& e){
 auto owner=impl_;if(!owner||!owner->root||milliseconds<0){e="Required retained source frame/time unavailable";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 try{if(!frames.advance(owner->root.get_ptr(),float(milliseconds)*.001f,false,e))return false;owner->advance_authored_animations(float(milliseconds)*.001f);return owner->finish(e);}
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
 return input_cursor(cursor,0,e);
}
bool SwfMovie::input_cursor(const SwfCursor16& cursor,std::uint32_t cursor_index,std::string& e){
 auto owner=impl_;auto input=input_;if(!owner||!input){e="Source input owner unavailable";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 if(cursor_index>=4){e="Source input cursor index outside 0..3";return false;}
 try{return input->cursor(cursor,cursor_index,e)&&owner->finish(e);}catch(const std::exception& x){e=x.what();return false;}
}
bool SwfMovie::input_advance(std::int32_t ms,std::string& e){
 auto owner=impl_;auto input=input_;if(!owner||!input||ms<0){e="Source input owner/time unavailable";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 try{if(!input->update(ms,false,e))return false;owner->advance_authored_animations(float(ms)*.001f);return owner->finish(e);}catch(const std::exception& x){e=x.what();return false;}
}
bool SwfMovie::input_cancel(float x,float y,std::string& e){
 return input_cancel(x,y,0,e);
}
bool SwfMovie::input_cancel(float x,float y,std::uint32_t cursor_index,std::string& e){
 auto owner=impl_;auto input=input_;if(!owner||!input){e="Source input owner unavailable";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 if(cursor_index>=4){e="Source input cursor index outside 0..3";return false;}
 if(!input->enable(false,cursor_index,e))return false;
 const bool cleared=input->cursor({x,y,0.f,0},cursor_index,e)&&input->reset_focus(cursor_index,e);
 std::string restore;const bool enabled=input->enable(true,cursor_index,restore);
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
bool SwfMovie::input_key_event(std::uint32_t key_code,bool down,std::string& e){
 auto owner=impl_;if(!owner||!owner->player||key_code==gameswf::key::INVALID||
    key_code>=gameswf::key::KEYCOUNT){e="Malformed retained GameSWF key event";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 try{
  owner->player->notify_key_event(static_cast<gameswf::key::code>(key_code),down);
  return owner->finish(e);
 }catch(const std::exception& failure){e=failure.what();return false;}
}
gameswf::font*SwfMovie::borrowed_font(std::int32_t id)const{return impl_&&impl_->root?impl_->root->m_def->get_font(id):nullptr;}
const std::vector<std::string>&SwfMovie::diagnostics()const{return impl_->messages;}
std::uintptr_t SwfMovie::player_identity()const noexcept{auto owner=impl_;return owner?reinterpret_cast<std::uintptr_t>(owner->player.get_ptr()):0;}
} // namespace dh2::ui

