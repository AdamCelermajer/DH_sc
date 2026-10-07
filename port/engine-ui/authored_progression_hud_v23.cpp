#include "authored_progression_hud_v23.hpp"
#include "gameswf/gameswf_sprite.h"
#include <cmath>
namespace dh2::ui {namespace {
struct Call {const char* path;std::uint32_t dt;bool tick;float remainder;AuthoredProgressionHudDiagnosticV23 out;};
bool apply(void* raw,SwfAsGraph& graph,std::string& error){
 auto& c=*static_cast<Call*>(raw);SwfAsValue root,value;gameswf::as_object* object{};
 if(!graph.root_value(root,error)||!graph.find_target(root,c.path,value,error)||!graph.borrow_object(value,object,error))return false;
 if(!object||!object->is(gameswf::sprite_instance::m_class_id)){error="Required actual authored level-up sprite";return false;}
 auto* sprite=static_cast<gameswf::sprite_instance*>(object);
 if(sprite->get_id()!=162||sprite->get_frame_count()!=30){error="Actual level-up HUD timeline differs";return false;}
 auto* owner=sprite->get_root();const float cadence=owner?owner->m_frame_time:0;
 if(!std::isfinite(cadence)||cadence<=0){error="Required actual HUD cadence";return false;}
 c.out.visible=sprite->get_visible();
 if(c.tick){c.remainder=float(c.dt)*.001f+c.remainder;
  if(c.remainder>=cadence){
   // Hidden source animation stays hidden; no notification/point flag is
   // fabricated. Advancing the shown action-free subtree uses actual tags.
   if(c.out.visible){sprite->advance(float(c.dt)*.001f);c.out.advanced=true;}
   c.remainder=std::fmod(c.remainder-cadence,cadence);
  }
 }
 c.out.frame=sprite->get_current_frame();c.out.frames=sprite->get_frame_count();
 c.out.cadence=cadence;c.out.remainder=c.remainder;return true;
}
}
bool AuthoredProgressionHudV23::update(SwfMovie& movie,const AuthoredGameplayHudV1& hud,
 std::uint32_t dt,bool tick,AuthoredProgressionHudDiagnosticV23& out,std::string& error){
 if(hud.style()<0){error="Required selected authored HUD";return false;}
 const auto path=hud.control_path(AuthoredHudControlV1::character)+".anim_levelup";
 if(movie_!=movie.player_identity()||path_!=path){movie_=movie.player_identity();path_=path;remainder_=0;}
 Call c{path_.c_str(),dt,tick,remainder_,{}};const bool ok=movie.action_script(&c,apply,error);
 remainder_=c.remainder;out=c.out;return ok;
}
}
