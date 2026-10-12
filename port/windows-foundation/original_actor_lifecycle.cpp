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
 if(preset_pre_spawn)pre_spawn_baseline_[actor.id]=record.facts; // B141: survives detach_for_restore()
 else idle_baseline_[actor.id]=record.facts; // B155: survives detach_for_restore() too (Idle3 campaign actors)
 auto entry=records_.emplace(actor.id,std::move(record));auto&r=entry.first->second;
 // InitPost SetInitialPosition -> SetPosition(initial,true), snapshot rotation.
 if(!call(r,OriginalLifecycleOperation::restore_initial_position,-1,error))return false;
 if(!preset_pre_spawn&&!call(r,OriginalLifecycleOperation::clear_idle_suppressed,-1,error))return false;
 return change(r,preset_pre_spawn?17:3,error);
}
bool OriginalActorLifecycle::change(Record&r,int target,std::string&error){
 if(r.status.failed){error="Original lifecycle actor has a failed source prefix";return false;}
 if(target!=0&&target!=1&&target!=2&&target!=3&&target!=17){error="Unsupported original actor lifecycle state";return false;}
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
 return focus(r,target,previous,carried_interactive,error);
}
// New-state OnFocus recipe + the RaiseEvent1d notification (everything after the old state's OnBlur in change()).
bool OriginalActorLifecycle::focus(Record&r,int target,int previous,bool carried_interactive,std::string&error){
 auto emit=[&](OriginalLifecycleOperation op){return call(r,op,previous,error);};
 r.status.state=target;
 if(target==0){
  r.status.flags=0;r.status.enabled=false;
  if(!emit(OriginalLifecycleOperation::set_flags)||!emit(OriginalLifecycleOperation::set_enabled))return false;
  // P16 DESPAWN: a Limbus actor leaves the physical world (receiver released, no collisions), so a despawned pool
  // actor cannot keep blocking or colliding while hidden. CSLimbus::OnFocus itself does not call SetPhysicalObject; this
  // release is the port's despawn decision for a summoned actor and is reported as such.
  r.status.collisions_enabled=false;
  if(!emit(OriginalLifecycleOperation::remove_physical)||!emit(OriginalLifecycleOperation::set_collisions_enabled))return false;
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
 }else if(target==2){
  // P16 DESPAWN: CSDespawn::OnFocus 0x3c32fc writes flags328 = 512 and selects the Despawn clip. Its end (event 34) is the
  // Limbus transition handled by animation_finished.
  r.status.flags=0x200;if(!emit(OriginalLifecycleOperation::set_flags)||!emit(OriginalLifecycleOperation::select_state_animation))return false;
 }else{
  r.status.flags=0x2380;
  if(!emit(OriginalLifecycleOperation::set_flags)||!emit(OriginalLifecycleOperation::select_state_animation))return false;
 }
 // Source RaiseEvent1d carries PREVIOUS state after blur + focus.
 return emit(OriginalLifecycleOperation::notify_state_changed);
}
bool OriginalActorLifecycle::spawn(ActorId id,std::string&error){auto i=records_.find(id);if(i==records_.end()){error="Unknown original lifecycle actor";return false;}return change(i->second,1,error);}
bool OriginalActorLifecycle::put_idle(ActorId id,std::string&error){auto i=records_.find(id);if(i==records_.end()){error="Unknown original lifecycle actor";return false;}return change(i->second,3,error);}
// P16 DESPAWN: CSDespawn entry from Idle (the dead actor keeps lifecycle Idle until its Despawn state; the combat session owns
// the Dead pose). Only an Idle actor may enter Despawn.
bool OriginalActorLifecycle::despawn(ActorId id,std::string&error){auto i=records_.find(id);if(i==records_.end()){error="Unknown original lifecycle actor";return false;}if(i->second.status.state!=3){error="Despawn requires an Idle lifecycle actor";return false;}return change(i->second,2,error);}
// P16 DESPAWN: CSDead::OnEvent event 34, SetPhysicalObject(nullptr): the body leaves the physical world at the death-animation end.
bool OriginalActorLifecycle::release_body(ActorId id,std::string&error){auto i=records_.find(id);if(i==records_.end()){error="Unknown original lifecycle actor";return false;}auto&r=i->second;if(r.status.failed){error="Original lifecycle actor has a failed source prefix";return false;}return call(r,OriginalLifecycleOperation::remove_physical,r.status.state,error);}
bool OriginalActorLifecycle::put_limbus(ActorId id,std::string&error){auto i=records_.find(id);if(i==records_.end()){error="Unknown original lifecycle actor";return false;}return change(i->second,0,error);}
bool OriginalActorLifecycle::animation_finished(ActorId id,std::string&error){auto i=records_.find(id);if(i==records_.end()){error="Unknown original lifecycle actor";return false;}if(i->second.status.failed){error="Original lifecycle actor has a failed source prefix";return false;}const int finished_state=i->second.status.state;if(finished_state==2)return change(i->second,0,error);if(finished_state!=1){error.clear();return true;}return change(i->second,3,error);}
bool OriginalActorLifecycle::animation_event(ActorId id,const std::string&name,std::string&error){auto i=records_.find(id);if(i==records_.end()){error="Unknown original lifecycle actor";return false;}auto&r=i->second;if(r.status.failed){error="Original lifecycle actor has a failed source prefix";return false;}if((r.status.state==1||r.status.state==17)&&name=="is_interactive"){r.status.flags|=0x2000;if(!call(r,OriginalLifecycleOperation::set_flags,r.status.state,error)||!call(r,OriginalLifecycleOperation::init_physical,r.status.state,error))return false;}error.clear();return true;}
const OriginalLifecycleStatus* OriginalActorLifecycle::status(ActorId id)const{auto i=records_.find(id);return i==records_.end()?nullptr:&i->second.status;}
bool OriginalActorLifecycle::combat_enabled(ActorId id)const{auto*s=status(id);return s&&!s->failed&&s->enabled&&s->state==3;}
void OriginalActorLifecycle::remove(ActorId id){records_.erase(id);}
void OriginalActorLifecycle::clear(){records_.clear();pre_spawn_baseline_.clear();idle_baseline_.clear();}
void OriginalActorLifecycle::detach_for_restore(){records_.clear();}
std::vector<ActorId> OriginalActorLifecycle::pre_spawn_baseline_ids()const{
 std::vector<ActorId> ids;ids.reserve(pre_spawn_baseline_.size());for(const auto&entry:pre_spawn_baseline_)ids.push_back(entry.first);return ids;
}
std::vector<ActorId> OriginalActorLifecycle::idle_baseline_ids()const{
 std::vector<ActorId> ids;ids.reserve(idle_baseline_.size());for(const auto&entry:idle_baseline_)ids.push_back(entry.first);return ids;
}
// B155: see the header. Saved Limbus/Despawn/Dead (0/2/12) -> Limbus0 (CSLimbus focus: flags 0, disabled, no collisions); any other
// non-PreSpawn saved state -> Idle3 (CSIdle focus flags 0x2380). No Blur/Focus effect runs: the restored actor data already has them.
bool OriginalActorLifecycle::restore_idle_or_limbus(ActorId id,ActorState&actor,bool enabled,bool limbus,std::string&error){
 const auto baseline=idle_baseline_.find(id);
 if(actor.id!=id||baseline==idle_baseline_.end()){error="Restored actor has no Idle-preset lifecycle baseline";return false;}
 if(records_.count(id)){error="Invalid or duplicate lifecycle actor identity";return false;}
 if(!services_.invoke){error="Original actor lifecycle effect provider unavailable";return false;}
 Record record;record.actor=&actor;record.facts=baseline->second;
 if(limbus){record.status.state=0;record.status.flags=0;record.status.enabled=false;record.status.collisions_enabled=false;}
 else{record.status.state=3;record.status.flags=0x2380;record.status.enabled=enabled;record.status.collisions_enabled=enabled;}
 records_.emplace(id,std::move(record));error.clear();return true;
}
// B142: Character::Deserialize 0x3a6210 on a LIVE Character whose saved state is Spawn(1)/PreSpawn(17) (the death-checkpoint reload)
// calls SM_SetPreSpawnState -> _SetState(17). The saved Character data (flags, position, vitals, dead bit, physical presence)
// has already replaced everything the old state's OnBlur would undo, so only CSPreSpawn::OnFocus (0x3c688c: PreSpawn pose,
// SetPhysicalObject(null), DisableCollisions) and the state notification run, through the same Focus recipe as the level start.
bool OriginalActorLifecycle::enter_restored_pre_spawn(ActorId id,ActorState&actor,bool enabled,int previous_state,std::string&error){
 const auto baseline=pre_spawn_baseline_.find(id);
 if(actor.id!=id||baseline==pre_spawn_baseline_.end()){error="Restored actor has no preset PreSpawn lifecycle baseline";return false;}
 if(records_.count(id)){error="Invalid or duplicate lifecycle actor identity";return false;}
 if(!services_.invoke){error="Original actor lifecycle effect provider unavailable";return false;}
 Record record;record.actor=&actor;record.facts=baseline->second;
 record.status.state=previous_state;record.status.enabled=enabled;
 auto entry=records_.emplace(id,std::move(record));
 if(!focus(entry.first->second,17,previous_state,false,error)){records_.erase(entry.first);return false;}
 return true;
}
// B145: true when the actor still stands on the authored initial anchor of its preset PreSpawn/Limbus baseline.
bool OriginalActorLifecycle::at_pre_spawn_anchor(const ActorState&actor,float tolerance)const{
 const auto baseline=pre_spawn_baseline_.find(actor.id);if(baseline==pre_spawn_baseline_.end())return false;
 for(int axis=0;axis<3;++axis){const float d=actor.transform.position[axis]-baseline->second.initial_transform.position[axis];if(!std::isfinite(d)||std::fabs(d)>tolerance)return false;}
 return true;
}
bool OriginalActorLifecycle::restore_pre_spawn(ActorId id,ActorState&actor,bool enabled,std::string&error){
 const auto baseline=pre_spawn_baseline_.find(id);
 if(actor.id!=id||baseline==pre_spawn_baseline_.end()){error="Restored actor has no preset PreSpawn lifecycle baseline";return false;}
 if(records_.count(id)){error="Invalid or duplicate lifecycle actor identity";return false;}
 if(!services_.invoke){error="Original actor lifecycle effect provider unavailable";return false;}
 Record record;record.actor=&actor;record.facts=baseline->second;
 record.status.state=17;record.status.flags=0x1300;record.status.enabled=enabled;record.status.collisions_enabled=false; // CSPreSpawn focus values (change() target 17)
 records_.emplace(id,std::move(record));return true;
}
}
