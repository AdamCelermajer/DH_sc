#include "swf_input_session_v1.hpp"
#include "swf_frame_connection.hpp"
#include "gameswf/gameswf_sprite.h"
#include "gameswf/gameswf_render.h"
#include <functional>
#include <exception>
namespace dh2::ui {
struct SwfInputSessionV1::Control {
 std::shared_ptr<Generation> current;
 std::uint64_t revision{};
};
struct SwfInputSessionV1::Provider {
 SwfInputSessionConfigV1 config;
 std::weak_ptr<Generation> generation;
 std::shared_ptr<SwfInputHistory> history=std::make_shared<SwfInputHistory>();
 SwfFrameConnection frames;
 static Provider& self(void*p){return *static_cast<Provider*>(p);}
 static bool read(void*p,const char*n,std::vector<std::uint8_t>&b,std::string&e){auto&s=self(p).config.movie_services;return s.read&&s.read(s.context,n,b,e);}
 static bool texture(void*p,const char*n,std::int32_t w,std::int32_t h,SwfTexture&o,std::string&e){auto&s=self(p).config.movie_services;return s.texture&&s.texture(s.context,n,w,h,o,e);}
 static bool image(void*p,std::int32_t w,std::int32_t h,std::uint32_t c,const std::uint8_t*b,std::int32_t pitch,SwfTexture&o,std::string&e){auto&s=self(p).config.movie_services;return s.image&&s.image(s.context,w,h,c,b,pitch,o,e);}
 static bool draw(void*p,const SwfDraw&d,std::string&e){auto&s=self(p).config.movie_services;return s.draw&&s.draw(s.context,d,e);}
 static bool native(void*p,const char*n,const std::vector<SwfValue>&a,SwfValue&o,std::string&e){auto&s=self(p).config.movie_services;return s.native_call&&s.native_call(s.context,n,a,o,e);}
 static bool native_as(void*p,const char*n,const gameswf::fn_call&f,std::string&e){auto&s=self(p).config.movie_services;return s.native_action&&s.native_action(s.context,n,f,e);}
 static bool stencil(void*p,const float*b,std::uint8_t n,bool&o,std::string&e){auto&s=self(p).config.movie_services;return s.stencil&&s.stencil(s.context,b,n,o,e);}
 static void diagnostic(void*p,bool failed,const char*n){auto&s=self(p).config.movie_services;if(s.diagnostic)s.diagnostic(s.context,failed,n);}
 static bool start(void*p,const SwfAsLease&lease,std::string&e){auto&s=self(p);
  if(!s.history->bind(lease.player,e)||!s.frames.bind(lease.player,s.history,e))return false;
  auto&original=s.config.movie_services;
  return !original.graph_start||original.graph_start(original.context,lease,e);
 }
 static bool accept(void*p,SwfEvent48&ev,bool&accepted,std::string&e){auto&s=self(p).config.input_services;if(!s.can_handle_event){e="Required native CanHandleEvent receiver unavailable";return false;}return s.can_handle_event(s.context,ev,accepted,e);}
 static bool event(void*p,SwfEvent48&ev,std::string&e){auto&s=self(p).config.input_services;if(!s.native_event){e="Required native event receiver unavailable";return false;}return s.native_event(s.context,ev,e);}
 static bool advance(void*p,gameswf::root*r,float dt,bool flag,std::string&e){return self(p).frames.advance(r,dt,flag,e);}
 SwfServices services(){auto out=config.movie_services;out.context=this;
  //FSCommand captures use no rebased context; config pins their original owner.
  if(out.read)out.read=read;if(out.texture)out.texture=texture;if(out.image)out.image=image;
  if(out.draw)out.draw=draw;if(out.native_call)out.native_call=native;
  if(out.stencil)out.stencil=stencil;if(out.diagnostic)out.diagnostic=diagnostic;
  if(out.native_action)out.native_action=native_as;
  out.graph_start=start;return out;
 }
};
struct SwfInputSessionV1::Generation {
 // Reverse member destruction releases input lease before movie, and keeps
 // observers/providers alive through complete graph/AS teardown.
 std::shared_ptr<Provider> provider;
 SwfMovie movie;
 SwfInputConnectionV2 input;
 std::uint32_t selection{};
 FlashCamera40 camera{};
 SwfAsGraph* scoped_graph{};
};
namespace {
struct Active {
 const SwfInputSessionV1::Control*control;
 std::shared_ptr<SwfInputSessionV1::Generation> generation;
 Active*previous;
 static thread_local Active*current;
 Active(const SwfInputSessionV1::Control*c,std::shared_ptr<SwfInputSessionV1::Generation>g):control(c),generation(std::move(g)),previous(current){current=this;}
 ~Active(){current=previous;}
};
thread_local Active*Active::current=nullptr;
using Operation=std::function<bool(SwfInputSessionV1::Generation&,SwfAsGraph&,std::string&)>;
struct Batch {
 SwfInputSessionV1::Generation*generation;const Operation*operation;
 static bool apply(void*p,SwfAsGraph&graph,std::string&e){auto&b=*static_cast<Batch*>(p);auto*g=b.generation;auto*before=g->scoped_graph;g->scoped_graph=&graph;
  struct Restore {SwfInputSessionV1::Generation*g;SwfAsGraph*before;~Restore(){g->scoped_graph=before;}} restore{g,before};
  return (*b.operation)(*g,graph,e);
 }
};
bool run(std::shared_ptr<SwfInputSessionV1::Control>c,const Operation&operation,std::string&e){
 try{
  if(Active::current&&Active::current->control==c.get()){
   auto g=Active::current->generation;
   if(g->scoped_graph)return operation(*g,*g->scoped_graph,e);
   e="Input session graph construction is not yet bound";return false;
  }
  auto g=c->current;if(!g){e="Input session unbound";return false;}
  Active active(c.get(),g);Batch b{g.get(),&operation};return g->movie.action_script(&b,Batch::apply,e);
 }catch(const std::exception&x){e=x.what();return false;}catch(...){e="Native input session provider failed";return false;}
}
bool character(SwfAsGraph&as,const char*path,gameswf::character*&out,std::string&e){
 SwfAsValue root,value;if(!as.root_value(root,e))return false;
 if(!path||!*path)value=root;else if(!as.find_target(root,path,value,e))return false;
 gameswf::as_object*object=nullptr;if(!as.borrow_object(value,object,e))return false;
 if(!object||!object->is(gameswf::character::m_class_id)){e="Source input context is not a retained character";return false;}
 out=static_cast<gameswf::character*>(object);return true;
}
}
SwfInputSessionV1::SwfInputSessionV1():control_(std::make_shared<Control>()){}
SwfInputSessionV1::~SwfInputSessionV1(){release();}
void SwfInputSessionV1::release()noexcept{auto c=control_;++c->revision;c->current.reset();}
bool SwfInputSessionV1::bound()const noexcept{return control_->current&&control_->current->input.bound();}
bool SwfInputSessionV1::load(const SwfInputSessionConfigV1&config,std::string&e){auto c=control_;const auto revision=c->revision;
 if(config.movie_services.source_fscommand_v114&&!config.movie_services.native_owner){e="Required original owned FSCommand provider before InputSession wrapping";return false;}
 if(!config.provider_owner||!config.movie_services.read||!config.movie_services.draw||config.movie.empty()||!config.driver.orientation||!config.driver.dimensions||config.input_services.advance||config.input_services.scene_local_mouse){e="Malformed 2D source input session providers/policy";return false;}
 try{
  auto g=std::make_shared<Generation>();g->provider=std::make_shared<Provider>();g->provider->config=config;g->provider->generation=g;g->selection=config.selection;
  auto services=g->provider->services();services.native_owner=g->provider;
  Active active(c.get(),g);
  if(!g->movie.load(config.shared,config.movie,services,e))return false;
  Operation bind=[&](Generation&state,SwfAsGraph&as,std::string&error){
   SwfAsValue root;if(!as.root_value(root,error))return false;
   gameswf::character*root_character=nullptr,*context=nullptr;
   if(!character(as,nullptr,root_character,error)||!character(as,config.context_path.c_str(),context,error))return false;
   auto pin=std::make_shared<SwfAsValue>(root);
   SwfInputCoreServices input{state.provider,state.provider.get(),config.input_services.native_receiver,Provider::accept,Provider::event,Provider::advance,nullptr};
   if(!state.input.bind({pin,root_character->get_root()},config.viewport,config.driver,state.provider->history,context,config.flags,state.selection,input,error,state.movie.controller_storage_v91()))return false;
   state.movie.bind_controller_context_clear_v91([weak=std::weak_ptr<Generation>(g)](std::string& e){
    auto actual=weak.lock();if(!actual){e="Required same live InputSession generation for context release";return false;}
    return actual->input.clear_context_v91(e);
   });
   state.camera=config.camera;return state.input.camera_update(state.camera,error);
  };
  Batch b{g.get(),&bind};if(!g->movie.action_script(&b,Batch::apply,e))return false;
  if(c->revision!=revision){e="Input session candidate cancelled by owner release";return false;}
  c->current=std::move(g);++c->revision;e.clear();return true;
 }catch(const std::exception&x){e=x.what();return false;}catch(...){e="Native input session construction failed";return false;}
}
bool SwfInputSessionV1::cursor(const SwfCursor16&cursor,std::uint32_t index,std::string&e){return run(control_,[&](Generation&g,SwfAsGraph&,std::string&error){return g.input.cursor(cursor,index,error);},e);}
bool SwfInputSessionV1::input(std::int32_t mask,std::uint32_t index,std::string&e){return run(control_,[&](Generation&g,SwfAsGraph&,std::string&error){return g.input.input(mask,index,error);},e);}
bool SwfInputSessionV1::reset_focus(std::uint32_t index,std::string&e){return run(control_,[&](Generation&g,SwfAsGraph&,std::string&error){return g.input.reset_focus(index,error);},e);}
bool SwfInputSessionV1::focus(const char*path,std::uint32_t index,std::string&e){return run(control_,[&](Generation&g,SwfAsGraph&as,std::string&error){gameswf::character*c=nullptr;if(path&&!character(as,path,c,error))return false;return g.input.focus(c,index,error);},e);}
bool SwfInputSessionV1::enable(bool value,std::uint32_t index,std::string&e){return run(control_,[&](Generation&g,SwfAsGraph&,std::string&error){return g.input.enable(value,index,error);},e);}
bool SwfInputSessionV1::set_flags(std::uint32_t flags,std::string&e){return run(control_,[&](Generation&g,SwfAsGraph&,std::string&error){return g.input.set_flags(flags,error);},e);}
bool SwfInputSessionV1::update(std::int32_t ms,bool flag,std::string&e){return run(control_,[&](Generation&g,SwfAsGraph&,std::string&error){return g.input.update(ms,flag,error);},e);}
bool SwfInputSessionV1::camera_update(FlashCamera40&camera,std::string&e){return run(control_,[&](Generation&g,SwfAsGraph&,std::string&error){g.camera=camera;const bool delivered=g.input.camera_update(g.camera,error);camera=g.camera;return delivered;},e);}
bool SwfInputSessionV1::camera_state(FlashCamera40&camera,std::string&e)const{return run(control_,[&](Generation&g,SwfAsGraph&,std::string&error){camera=g.camera;error.clear();return true;},e);}
bool SwfInputSessionV1::screen_to_logical(float point[2],std::string&e){return run(control_,[&](Generation&g,SwfAsGraph&,std::string&error){return g.input.screen_to_logical(point,error);},e);}
bool SwfInputSessionV1::viewport_state(ViewportState64&state,std::string&e)const{return run(control_,[&](Generation&g,SwfAsGraph&,std::string&error){return g.input.viewport_state(state,error);},e);}
bool SwfInputSessionV1::snapshot(SwfInputState288&state,std::uint32_t&selection,std::string&e)const{return run(control_,[&](Generation&g,SwfAsGraph&,std::string&error){if(!g.input.snapshot(state,error))return false;selection=g.selection;return true;},e);}
bool SwfInputSessionV1::display(const char*path,std::string&e){return run(control_,[&](Generation&g,SwfAsGraph&as,std::string&error){
 gameswf::character*clip=nullptr;if(!character(as,path,clip,error))return false;
 ViewportState64 state{};float rect[4];if(!g.input.viewport_state(state,error)||!g.input.display_rectangle(rect,error))return false;
 auto*r=clip->get_root();gameswf::render::begin_display(r->m_background_color,state.viewport[0],state.viewport[1],state.viewport[2],state.viewport[3],rect[0],rect[1],rect[2],rect[3]);
 clip->display();gameswf::render::end_display();return true;
},e);}
bool SwfInputSessionV1::action_script(void*context,bool(*operation)(void*,SwfAsGraph&,std::string&),std::string&e){if(!operation){e="Missing connected graph operation";return false;}return run(control_,[&](Generation&,SwfAsGraph&as,std::string&error){return operation(context,as,error);},e);}
}

