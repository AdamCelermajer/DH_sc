#include "authored_status_timeline_v27.hpp"
#include "gameswf/gameswf_sprite.h"
#include <cmath>
namespace dh2::ui {namespace {
struct Call {const char* path;std::uint32_t dt;bool tick;float remainder;AuthoredStatusDiagnosticV27 out;};
bool apply(void* raw,SwfAsGraph& graph,std::string& error){
 auto& c=*static_cast<Call*>(raw);SwfAsValue root,value;gameswf::as_object* object{};
 if(!graph.root_value(root,error)||!graph.find_target(root,c.path,value,error)||
    !graph.borrow_object(value,object,error))return false;
 if(!object||!object->is(gameswf::sprite_instance::m_class_id)){error="Required original status movie clip";return false;}
 auto* clip=static_cast<gameswf::sprite_instance*>(object);
 auto* source=clip->get_root();
 // Validate the retained authored resource rather than an AS wrapper.
 auto* definition=clip->m_def.get_ptr();
 int scroll{};
 if(!source||!definition||clip->get_id()!=166||clip->get_frame_count()!=48||!definition->get_labeled_frame("scroll",&scroll)){
  error="Original 48-frame labeled status timeline differs: "+std::string(c.path);return false;
 }
 const float cadence=source->m_frame_time;
 if(!std::isfinite(cadence)||cadence<=0){error="Required source status movie cadence";return false;}
 c.out.visible=clip->get_visible();
 // Source catch_up=false scheduling: at most one authored frame per actual
 // application tick, carrying then fmodding the same movie cadence remainder.
 // Advancing this subtree executes its real last-frame ActionScript and the
 // synchronous NativeStopMessage callback; no queue timer is substituted.
 if(c.tick){c.remainder=float(c.dt)*.001f+c.remainder;
  if(c.remainder>=cadence){
   if(c.out.visible){clip->advance(float(c.dt)*.001f);c.out.advanced=true;}
   c.remainder=std::fmod(c.remainder-cadence,cadence);
  }
 }
 c.out.frame=clip->get_current_frame();c.out.frames=clip->get_frame_count();
 c.out.visible=clip->get_visible();c.out.cadence=cadence;c.out.remainder=c.remainder;return true;
}
}
bool AuthoredStatusTimelineV27::update(SwfMovie& movie,const AuthoredGameplayHudV1& hud,
 std::uint32_t dt,bool tick,AuthoredStatusDiagnosticV27& out,std::string& error){
 if(hud.style()<0){error="Required selected source HUD for status advance";return false;}
 const auto path=hud.elements_path()+".itemname_text";
 const auto identity=movie.player_identity();
 if(!identity){error="Required retained status movie identity";return false;}
 if(movie_!=identity||path_!=path){movie_=identity;path_=path;remainder_=0;}
 Call c{path_.c_str(),dt,tick,remainder_,{}};
 const bool ok=movie.menu_action_script(&c,apply,error);
 remainder_=c.remainder;out=c.out;return ok;
}
}
