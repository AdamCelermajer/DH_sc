#include "session_skill_animation.hpp"
namespace dh::foundation::skills_animation {
using namespace dh2::character::skills;
SessionSkillAnimation::SessionSkillAnimation(CombatSession& session,ActorId id,SkillActorBorrow owner,
 dh2::data::SkillTables::Borrow tables,const SkillAnimationPrograms& programs,SessionSkillServices services)
 :session_(session),id_(id),owner_(owner),services_(std::move(services)),session_lease_(session.actor_binding_lease()),programs_(programs){
 if(owner_.ai&&owner_.state)source_=std::make_unique<dh2::foundation::skills_animation::SourceSkillAnimation>(
  std::move(tables),*owner_.ai,*owner_.state,[this](unsigned index,std::int32_t& row){return services_.row&&services_.row(id_,index,row,failure_);},
  services_.ai,SkillStateServices16V4{this,state_service});
}
bool SessionSkillAnimation::validate(std::string& error){
 if(!source_||!services_.borrow){error="Skill live owner/borrow provider unavailable";return false;}
 const auto originalLease=session_lease_.lock(),currentLease=session_.actor_binding_lease().lock();
 if(!originalLease||originalLease!=currentLease){error="Skill actor/property/native owners changed; explicit rebind required";return false;}
 SkillActorBorrow now;if(!services_.borrow(session_,id_,now,error))return false;
 // Check against the session's live registry before dereferencing a provider's
 // borrowed pointer. A retained callback may still return an old address after
 // detach/replacement; pointer identity in the provider alone is insufficient.
 if(!session_.retained_actor_pose(id_)||!now.actor||now.actor!=session_.actor(id_)||now.actor!=owner_.actor||!now.properties||!now.ai||!now.state||now.properties!=owner_.properties||now.ai!=owner_.ai||now.state!=owner_.state){error="Skill actor/property/native owners changed; explicit rebind required";return false;}
 if(now.actor->id!=id_){error="Skill actor/property/native owners changed; explicit rebind required";return false;}
 if(dh2_property_validate(now.properties)){error="Skill requires actual live property sheets";return false;}
 error.clear();return true;
}
int SessionSkillAnimation::state_service(void* p,SkillStateV4* state,const SkillStateRequest32V4* q,SkillStateResponse16V4* r){
 auto& self=*static_cast<SessionSkillAnimation*>(p);
 if(state!=self.owner_.state||!self.validate(self.failure_))return -1;
 if(q->operation==skill_state_animation_v4){
  std::int32_t root=std::int32_t(q->value);if(q->value==UINT32_MAX)root=state->state->animation_override;
  return self.select_animation(root,self.failure_)?0:-1;
 }
 return self.services_.state.invoke?self.services_.state.invoke(self.services_.state.context,state,q,r):-1;
}
bool SessionSkillAnimation::select_animation(std::int32_t root,std::string& error){
 if(!services_.selection||!services_.play||!services_.finished){error="Skill source selection/Session pose/whole-finished provider unbound";return false;}
 OriginalAttackSelection selection;if(!services_.selection(id_,root,selection,error))return false;
 if(selection.state!=skill_sequence_state(root)||selection.variant!=0||!programs_.plan.sequence(selection.state,0)){error="Skill ANIM_Set root does not match explicitly loaded original program";return false;}
 CombatSessionStateAnimationServices events;
 events.event=[this](ActorId actor,const RetainedAnimationEvent& event,std::string& e){if(actor!=id_){e="Foreign skill animation event";return false;}return authored_event(event,e);};
 events.finished=[this](ActorId actor,std::string& e){if(actor!=id_||!validate(e))return false;return services_.finished(session_,id_,e);};
 return services_.play(session_,id_,programs_.plan,programs_.policies,selection,std::move(events),error);
}
bool SessionSkillAnimation::command(std::uint32_t operation,std::uint32_t index,std::uint32_t& answer,std::string& error){
 if(!validate(error))return false;failure_.clear();const auto status=source_->command(operation,index,answer);
 if(status){error=failure_.empty()?"Original skill AI required provider failed (status "+std::to_string(status)+")":failure_;return false;}error.clear();return true;
}
bool SessionSkillAnimation::hotbar_command(std::uint32_t operation,std::size_t slot,std::uint32_t& answer,std::string& error){
 if(!validate(error))return false;if(!services_.hotbar){error="Actual saved hotbar provider unbound";return false;}
 std::vector<int> positions;if(!services_.hotbar(id_,positions,error))return false;
 if(slot>=positions.size()||positions[slot]<0){error="Hotbar slot is not bound to a source class skill position";return false;}
 return class_position_command(operation,positions[slot],answer,error);
}
bool SessionSkillAnimation::class_position_command(std::uint32_t operation,int position,std::uint32_t& answer,std::string& error){
 if(!validate(error))return false;if(position<0||!services_.native_index){error="Actual class-position to native skill index provider unbound";return false;}
 unsigned index;if(!services_.native_index(id_,position,index,error))return false;return command(operation,index,answer,error);
}
bool SessionSkillAnimation::state_operation(std::uint32_t operation,std::uint32_t index,std::uint32_t moving,std::uintptr_t payload,std::uint32_t force,std::string& error){
 if(!validate(error))return false;failure_.clear();const auto status=source_->state_operation(operation,index,moving,payload,force);
 if(status){error=failure_.empty()?"Original CSSkill required provider failed (status "+std::to_string(status)+")":failure_;return false;}error.clear();return true;
}
bool SessionSkillAnimation::authored_event(const RetainedAnimationEvent& event,std::string& error){
 if(!validate(error))return false;failure_.clear();const auto status=source_->authored_event(event.name.c_str());
 if(status){error=failure_.empty()?"Original skill animation event required provider failed (status "+std::to_string(status)+")":failure_;return false;}error.clear();return true;
}
}
