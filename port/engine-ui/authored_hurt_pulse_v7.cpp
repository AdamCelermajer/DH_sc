#include "authored_hurt_pulse_v7.hpp"
#include "gameswf/gameswf_sprite.h"
#include <cmath>
#include <exception>
namespace dh2::ui {
namespace {
struct Call {std::uint32_t dt;bool tick;float remainder;AuthoredHurtPulseDiagnosticV7 out;};
bool apply(void* raw,SwfAsGraph& graph,std::string& error){
 auto& call=*static_cast<Call*>(raw);SwfAsValue root,value;gameswf::as_object* object{};
 if(!graph.root_value(root,error)||!graph.find_target(root,"_root.HurtCorners",value,error)||
    !graph.borrow_object(value,object,error))return false;
 if(!object||!object->is(gameswf::sprite_instance::m_class_id)){
  error="Required actual HurtCorners sprite unavailable";return false;
 }
 auto* outer=static_cast<gameswf::sprite_instance*>(object);
 if(outer->get_id()!=31||outer->get_frame_count()!=103||outer->m_display_list.size()!=1){
  error="Actual HurtCorners authored tree differs";return false;
 }
 auto* child=outer->m_display_list.get_character(0);
 if(!child||child->get_depth()!=1||child->get_id()!=30||
    !child->is(gameswf::sprite_instance::m_class_id)){
  error="Required actual HurtCorners pulse child differs";return false;
 }
 auto* pulse=static_cast<gameswf::sprite_instance*>(child);
 if(pulse->get_frame_count()!=22||pulse->get_play_state()!=gameswf::character::PLAY){
  error="Actual authored HurtCorners pulse is not playing its 22-frame timeline";return false;
 }
 auto* movie_root=pulse->get_root();
 const float cadence=movie_root?movie_root->m_frame_time:0.f;
 if(!std::isfinite(cadence)||cadence<=0){error="Required actual HUD movie frame time unavailable";return false;}
 // Source root scheduling with catch_up=false advances at most one frame and
 // fmods the remainder. No root listeners, other menu sprites, or native AS
 // callbacks are traversed by this bounded modern integration adapter.
 if(call.tick){
  const float delta=float(call.dt)*.001f;
  call.remainder=delta+call.remainder;
  if(call.remainder>=cadence){pulse->advance(delta);call.out.advanced=true;call.remainder-=cadence;
   call.remainder=std::fmod(call.remainder,cadence);}
 }
 call.out.outer_frame=outer->get_current_frame();call.out.pulse_frame=pulse->get_current_frame();
 call.out.pulse_frames=pulse->get_frame_count();call.out.frame_seconds=cadence;
 // Pulse30 animates the cxform of its child29; health31 animates pulse30.
 if(pulse->m_display_list.size()!=1){error="Actual pulse shape subtree differs";return false;}
 auto* shape= pulse->m_display_list.get_character(0);
 if(!shape||shape->get_id()!=29){error="Actual pulse authored shape wrapper differs";return false;}
 call.out.pulse_alpha=shape->get_cxform().m_[3][0];
 call.out.health_alpha=pulse->get_cxform().m_[3][0];call.out.remainder=call.remainder;
 error.clear();return true;
}
}
bool AuthoredHurtPulseV7::update(SwfMovie& movie,std::uint32_t dt,bool tick,
 AuthoredHurtPulseDiagnosticV7& out,std::string& error){
 const auto identity=movie.player_identity();
 if(!identity){error="Required retained HUD movie identity unavailable";return false;}
 if(player_!=identity){player_=identity;remainder_=0;}
 Call call{dt,tick,remainder_,{}};
 try {const bool ok=movie.action_script(&call,apply,error);remainder_=call.remainder;out=call.out;return ok;}
 catch(const std::exception& ex){remainder_=call.remainder;out=call.out;error=ex.what();return false;}
}
}
