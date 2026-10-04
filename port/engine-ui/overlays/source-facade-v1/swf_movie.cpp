#include "../../swf_movie.hpp"
#include "../../swf_source_movie_v1.hpp"
#include "../../swf_source_startup_v1.hpp"
#include "gameswf/gameswf.h"
#include "gameswf/gameswf_player.h"
#include "gameswf/gameswf_root.h"
#include "gameswf/gameswf_sprite.h"
#include "gameswf/gameswf_character.h"
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
 gameswf::gc_ptr<gameswf::root> root;std::vector<gameswf::gc_ptr<gameswf::root>> shared;
 std::vector<gameswf::gc_ptr<gameswf::sprite_instance>> hud_pins;
 std::vector<std::string> messages;std::string failure;SwfDraw state{};
 static Impl* active;
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
  Impl*p;bool entered;gameswf::glyph_provider* previous_glyphs{};
  Scope(Impl*i):p(i),entered(i&&active==nullptr){if(entered){active=p;p->failure.clear();previous_glyphs=gameswf::get_glyph_provider();gameswf::set_glyph_provider(p->service.glyphs);gameswf::set_render_handler(p);gameswf::register_file_opener_callback(open);gameswf::register_log_callback(log);gameswf::register_bitmap_substitution_callback(substitute);}}
  ~Scope(){if(entered){gameswf::set_glyph_provider(previous_glyphs);gameswf::register_bitmap_substitution_callback(nullptr);gameswf::set_render_handler(nullptr);active=nullptr;}}
 };
 void fail(const std::string&s){if(failure.empty())failure=s.empty()?"Required SWF provider rejected delivery":s;}
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
SwfMovie::SwfMovie():impl_(new Impl){}SwfMovie::~SwfMovie()=default;
SwfMovie::SwfMovie(SwfMovie&&)noexcept=default;SwfMovie&SwfMovie::operator=(SwfMovie&&)noexcept=default;
bool SwfMovie::load(const std::vector<std::string>&shared,const std::string&movie,const SwfServices&s,std::string&e){auto p=std::make_shared<Impl>();p->service=s;Impl::Scope scope(p.get());if(!scope.entered){e="SWF core busy";return false;}
 if(!s.read||!s.draw){e="SWF read/draw services required";return false;}p->player=new gameswf::player;p->player->set_separate_thread(false);gameswf::set_use_cache_files(false);
 p->player->get_global()->set_member("NativeGetStringFromSymbol",gameswf::as_value(Impl::native_string));
 if(!s.native_actions.empty()&&(!s.native_owner||!s.native_action)){e="Required owned native AS callback provider unavailable";return false;}
 if(s.graph_start&&!s.native_owner){e="Required owned SWF graph startup provider unavailable";return false;}
 for(const auto&name:s.native_actions)if(name=="NativeGetStringFromSymbol"){e="NativeGetStringFromSymbol already registered by movie facade";return false;}
 // Keep all original caller ownership/duplicate checks above this point.
 if(!swf_source_startup_owned_v1(s)){SwfServices defaults;if(!source_movie_services_v1(s,defaults,e))return false;p->service=std::move(defaults);}
 const auto&startup=p->service;
 auto as=std::make_shared<SwfAsGraph>();auto context=std::make_shared<Impl::AsContext>();
 context->graph=p;context->provider_owner=startup.native_owner;context->provider_context=startup.context;context->callback=startup.native_action;
 SwfAsServices as_services;as_services.context=context.get();as_services.owner=context;
 as_services.within_scope=Impl::AsContext::within;as_services.native_call=Impl::AsContext::native;as_services.failure=Impl::AsContext::failure;
 if(!as->bind({p,p->player.get_ptr(),nullptr},as_services,e)||!as->install_native(s.native_actions,e))return false;
 if(startup.graph_start&&!startup.graph_start(startup.context,{p,p->player.get_ptr(),nullptr},e))return false;
 for(const auto&file:shared){auto r=p->player->load_file(file.c_str());if(!r){e="SWF shared movie rejected: "+file;return false;}r->advance(0);p->shared.push_back(r);}
 p->root=p->player->load_file(movie.c_str());if(!p->root){e="SWF movie rejected: "+movie;return false;}
 if(!as->attach_root(p->root.get_ptr(),e)||!p->finish(e))return false;
 action_script_.reset();viewport_.reset();impl_=std::move(p);action_script_=std::move(as);return true;
}
bool SwfMovie::advance(float seconds,std::string&e){auto owner=impl_;if(!owner||!owner->root||!std::isfinite(seconds)||seconds<0){e="Invalid SWF advance";return false;}Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}owner->root->advance(seconds);return owner->finish(e);}
bool SwfMovie::display(std::int32_t x,std::int32_t y,std::int32_t w,std::int32_t h,std::string&e){auto owner=impl_;if(!owner||!owner->root||w<=0||h<=0){e="Invalid SWF viewport";return false;}Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}owner->root->set_display_viewport(x,y,w,h);owner->root->display();return owner->finish(e);}
bool SwfMovie::display_clip(const char*path,std::string&e){auto owner=impl_;if(!owner||!owner->root){e="SWF movie not loaded";return false;}auto&r=*owner->root;return display_clip(path,r.m_viewport_x0,r.m_viewport_y0,r.m_viewport_width,r.m_viewport_height,e);}
bool SwfMovie::display_clip(const char*path,std::int32_t x,std::int32_t y,std::int32_t w,std::int32_t h,std::string&e){auto owner=impl_;if(w<=0||h<=0){e="Invalid SWF viewport";return false;}Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}auto*c=owner->find(path);if(!c){e="SWF clip not found";return false;}auto&r=*owner->root;r.set_display_viewport(x,y,w,h);const auto&bounds=r.m_def->m_frame_size;owner->begin_display(r.m_background_color,x,y,w,h,bounds.m_x_min,bounds.m_x_max,bounds.m_y_min,bounds.m_y_max);c->display();owner->end_display();return owner->finish(e);}
bool SwfMovie::clip(const char*path,SwfClipInfo&out,std::string&e){auto owner=impl_;Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}auto*c=owner->find(path);if(!c){e="SWF clip not found";return false;}SwfClipInfo r;r.id=c->get_id();r.depth=c->get_depth();r.frame=c->get_current_frame();r.frames=c->get_frame_count();r.visible=c->get_visible();r.local=matrix(c->get_matrix());r.world=matrix(c->get_world_matrix());out=r;return owner->finish(e);}
bool SwfMovie::set_number(const char*path,double n,std::string&e){auto owner=impl_;Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}if(!owner->root||!path){e="Invalid SWF variable path";return false;}auto*env=owner->root->get_root_movie()->get_environment();if(!env){e="SWF environment missing";return false;}const ::array<gameswf::with_stack_entry> with;env->set_variable(path,gameswf::as_value(n),with);return owner->finish(e);}
bool SwfMovie::set_visible(const char*path,bool v,std::string&e){auto owner=impl_;Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}auto*c=owner->find(path);if(!c){e="SWF clip not found";return false;}c->set_visible(v);return owner->finish(e);}
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
gameswf::font*SwfMovie::borrowed_font(std::int32_t id)const{return impl_&&impl_->root?impl_->root->m_def->get_font(id):nullptr;}
const std::vector<std::string>&SwfMovie::diagnostics()const{return impl_->messages;}
} // namespace dh2::ui

