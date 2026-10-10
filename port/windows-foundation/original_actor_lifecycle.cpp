#include "original_actor_lifecycle.hpp"
#include <cmath>
#include <exception>
namespace dh::foundation {
bool OriginalActorLifecycle::call(Record&r,OriginalLifecycleOperation op,int previous,std::string&error){
 if(!services_.invoke){error="Original actor lifecycle effect provider unavailable";r.status.failed=true;return false;}
 OriginalLifecycleRequest request;request.operation=op;request.actor=r.actor;request.previous_state=previous;request.state=r.status.state;request.flags=r.status.flags;request.enabled=r.status.enabled;request.collisions_enabled=r.status.collisions_enabled;request.initial_transform=r.facts.initial_transform;
 try{if(services_.invoke(request,error))return true;}catch(const std::exception&e){error=e.what();}
 if(error.empty())error="Original actor lifecycle effect failed";r.status.failed=true;return false;
}
bool OriginalActorLifecycle::add(ActorState&actor,const OriginalLifecycleFacts&facts,std::string&error){
 if(actor.id==invalid_actor_id||records_.count(actor.id)){error="Invalid or duplicate lifecycle actor identity";return false;}
 const bool preset_pre_spawn=facts.preset_ai_state=="Limbus"||facts.preset_ai_state=="PreSpawn";
 if(preset_pre_spawn&&(!facts.pre_spawn_has_animation||!facts.pre_spawn_stay_enabled)){error="Original PreSpawn17 requires source animation availability and ai_state_visible facts";return false;}
 if(!services_.floor_height||!services_.invoke||!facts.initially_enabled){error="Original lifecycle requires floor/effect providers and source initial enabled fact";return false;}
 Record record;record.actor=&actor;record.facts=facts;record.status.enabled=*facts.initially_enabled;
 try{bool found=false;float height=0;auto p=facts.initial_transform.position;
  if(!std::isfinite(p[0])||!std::isfinite(p[1])||!std::isfinite(p[2])){error="Invalid original initial anchor";return false;}
  if(!services_.floor_height(p,found,height,error))return false;
  if(found){if(!std::isfinite(height)){error="Original floor height is nonfinite";return false;}record.facts.initial_transform.position[2]=height;}
 }catch(const std::exception&e){error=e.what();return false;}
 auto entry=records_.emplace(actor.id,std::move(record));auto&r=entry.first->second;
 // InitPost SetInitialPosition -> SetPosition(initial,true), snapshot rotation.
 if(!call(r,OriginalLifecycleOperation::restore_initial_position,-1,error))return false;
 if(!preset_pre_spawn&&!call(r,OriginalLifecycleOperation::clear_idle_suppressed,-1,error))return false;
 return change(r,preset_pre_spawn?17:3,error);
}
bool OriginalActorLifecycle::change(Record&r,int target,std::string&error){
 if(r.status.failed){error="Original lifecycle actor has a failed source prefix";return false;}
 if(target!=0&&target!=1&&target!=3&&target!=17){error="Unsupported original actor lifecycle state";return false;}
 const int previous=r.status.state;
 auto emit=[&](OriginalLifecycleOperation op){return call(r,op,previous,error);};
 // Source old-state OnBlur executes BEFORE publishing/focusing the new state.
 if(previous==0){
  r.status.enabled=true;if(!emit(OriginalLifecycleOperation::set_enabled)||!emit(OriginalLifecycleOperation::restore_initial_position)||
    !emit(OriginalLifecycleOperation::restore_initial_rotation)||!emit(OriginalLifecycleOperation::revive))return false;
 }else if(previous==17){r.status.enabled=true;r.status.collisions_enabled=true;if(!emit(OriginalLifecycleOperation::set_enabled)||!emit(OriginalLifecycleOperation::revive)||!emit(OriginalLifecycleOperation::set_collisions_enabled))return false;}
 else if(previous==1&&!(r.status.flags&0x2000))if(!emit(OriginalLifecycleOperation::init_physical))return false;
 if(previous==3&&!emit(OriginalLifecycleOperation::clear_idle_suppressed))return false;
 const bool carried_interactive=previous==17&&(r.status.flags&0x2000);
 r.status.state=target;
 if(target==0){
  r.status.flags=0;r.status.enabled=false;
  if(!emit(OriginalLifecycleOperation::set_flags)||!emit(OriginalLifecycleOperation::set_enabled))return false;
  if(r.facts.can_respawn&&r.facts.respawn_delay_ms>0){error="Original Limbus respawn timer/network owner not bound";r.status.failed=true;return false;}
  if(!emit(OriginalLifecycleOperation::clear_aggro))return false;
 }else if(target==17){
  r.status.flags=0x1300;if(!emit(OriginalLifecycleOperation::set_flags)||!emit(OriginalLifecycleOperation::select_state_animation))return false;
  if(!*r.facts.pre_spawn_has_animation&&!emit(OriginalLifecycleOperation::freeze_animation_speed))return false;
  if(!emit(OriginalLifecycleOperation::remove_physical))return false;
  if(!*r.facts.pre_spawn_stay_enabled){r.status.enabled=false;if(!emit(OriginalLifecycleOperation::set_enabled))return false;}
  r.status.collisions_enabled=false;if(!emit(OriginalLifecycleOperation::set_collisions_enabled))return false;
 }else if(target==1){
  r.status.flags=0x241;
  if(!emit(OriginalLifecycleOperation::set_flags)||!emit(OriginalLifecycleOperation::select_state_animation)||
     !emit(OriginalLifecycleOperation::clear_and_sync_target)||!emit(OriginalLifecycleOperation::cancel_sneaking))return false;
  if(carried_interactive){r.status.flags|=0x2000;if(!emit(OriginalLifecycleOperation::set_flags))return false;}
  // Source VisualObject::StartFadeIn470ce4 is empty; no alpha effect invented.
 }else{
  r.status.flags=0x2380;
  if(!emit(OriginalLifecycleOperation::set_flags)||!emit(OriginalLifecycleOperation::select_state_animation))return false;
 }
 // Source RaiseEvent1d carries PREVIOUS state after blur + focus.
 return emit(OriginalLifecycleOperation::notify_state_changed);
}
bool OriginalActorLifecycle::spawn(ActorId id,std::string&error){auto i=records_.find(id);if(i==records_.end()){error="Unknown original lifecycle actor";return false;}return change(i->second,1,error);}
bool OriginalActorLifecycle::put_idle(ActorId id,std::string&error){auto i=records_.find(id);if(i==records_.end()){error="Unknown original lifecycle actor";return false;}return change(i->second,3,error);}
bool OriginalActorLifecycle::put_limbus(ActorId id,std::string&error){auto i=records_.find(id);if(i==records_.end()){error="Unknown original lifecycle actor";return false;}return change(i->second,0,error);}
bool OriginalActorLifecycle::animation_finished(ActorId id,std::string&error){auto i=records_.find(id);if(i==records_.end()){error="Unknown original lifecycle actor";return false;}if(i->second.status.failed){error="Original lifecycle actor has a failed source prefix";return false;}if(i->second.status.state!=1){error.clear();return true;}return change(i->second,3,error);}
bool OriginalActorLifecycle::animation_event(ActorId id,const std::string&name,std::string&error){auto i=records_.find(id);if(i==records_.end()){error="Unknown original lifecycle actor";return false;}auto&r=i->second;if(r.status.failed){error="Original lifecycle actor has a failed source prefix";return false;}if((r.status.state==1||r.status.state==17)&&name=="is_interactive"){r.status.flags|=0x2000;if(!call(r,OriginalLifecycleOperation::set_flags,r.status.state,error)||!call(r,OriginalLifecycleOperation::init_physical,r.status.state,error))return false;}error.clear();return true;}
const OriginalLifecycleStatus* OriginalActorLifecycle::status(ActorId id)const{auto i=records_.find(id);return i==records_.end()?nullptr:&i->second.status;}
bool OriginalActorLifecycle::combat_enabled(ActorId id)const{auto*s=status(id);return s&&!s->failed&&s->enabled&&s->state==3;}
void OriginalActorLifecycle::remove(ActorId id){records_.erase(id);}
void OriginalActorLifecycle::clear(){records_.clear();}
}
