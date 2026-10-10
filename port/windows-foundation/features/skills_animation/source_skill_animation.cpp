#include "source_skill_animation.hpp"
#include <utility>
namespace dh2::foundation::skills_animation {
using namespace character::skills;
bool declarations(const data::SkillTables::Borrow& t,const std::string& name,std::vector<Declaration>& out,std::string& error){
 if(!t){error="Skill tables not loaded";return false;}
 const auto index=t.list_index(name.c_str());
 if(index<0){error="Unknown source SkillList: "+name;return false;}
 std::vector<Declaration> next;
 for(const auto id:t.lists()[index]){
  if(id<0||std::size_t(id)>=t.skills().size()){error="SkillList contains invalid row";return false;}
  const auto& r=t.skills()[id];const auto* w=r.scalar.words;
  next.push_back({id,t.skill_names()[id],r.script,r.icon,w[1],w[7],w[18],w[8],std::uint8_t(w[2]),std::uint8_t(w[11])});
 }
 out=std::move(next);error.clear();return true;
}
SourceSkillAnimation::SourceSkillAnimation(data::SkillTables::Borrow t,SkillAIContextV3& ai,SkillStateV4& state,std::function<bool(std::uint32_t,std::int32_t&)> resolver,SkillAIServices16V3 a,SkillStateServices16V4 s):tables_(std::move(t)),ai_(ai),state_(state),resolve_(std::move(resolver)),ai_services_(a),state_services_(s){}
bool SourceSkillAnimation::coherent()const{return tables_&&resolve_&&ai_.owner&&ai_.owner->character&&ai_.owner->character==state_.character&&state_.state&&ai_.slots&&ai_.slots->owner==state_.character;}
const data::SkillProjection76* SourceSkillAnimation::row(std::uint32_t slot){std::int32_t id=-1;if(!resolve_(slot,id)||id<0||std::size_t(id)>=tables_.skills().size())return nullptr;return &tables_.skills()[id].scalar;}
int SourceSkillAnimation::ai_service(void* p,SkillAIContextV3* state,const SkillAIRequest32V3* q,SkillAIResponse32V3* r){
 auto& self=*static_cast<SourceSkillAnimation*>(p);if(state!=&self.ai_||!self.coherent())return -1;
 if(q->operation==skill_ai_row_v3){r->row=self.row(q->index);return r->row?0:-1;}
 if(q->operation==skill_ai_set_state_v3)return self.state_operation(skill_state_select_v4,q->index,q->value);
 return self.ai_services_.invoke?self.ai_services_.invoke(self.ai_services_.context,state,q,r):-1;
}
int SourceSkillAnimation::state_service(void* p,SkillStateV4* state,const SkillStateRequest32V4* q,SkillStateResponse16V4* r){
 auto& self=*static_cast<SourceSkillAnimation*>(p);if(state!=&self.state_||!self.coherent())return -1;
 if(q->operation==skill_state_row_v4){const auto* row=self.row(q->index);r->identity=reinterpret_cast<std::uintptr_t>(row);return row?0:-1;}
 if(q->operation==skill_state_use_v4){std::uint32_t answer=0;return self.command(skill_ai_event_v3,0,answer);}
 return self.state_services_.invoke?self.state_services_.invoke(self.state_services_.context,state,q,r):-1;
}
int SourceSkillAnimation::command(std::uint32_t operation,std::uint32_t slot,std::uint32_t& answer){
 if(!coherent())return -1;const SkillAIServices16V3 services{this,ai_service};return dh2_character_skill_ai_v3(&answer,&ai_,operation,slot,&services);
}
int SourceSkillAnimation::state_operation(std::uint32_t operation,std::uint32_t index,std::uint32_t moving,std::uintptr_t payload,std::uint32_t force){
 if(!coherent())return -1;const SkillStateServices16V4 services{this,state_service};return dh2_character_skill_state_v4(&state_,operation,index,moving,payload,force,&services);
}
int SourceSkillAnimation::authored_event(const char* name){return state_operation(skill_state_ai_event_v4,0,0,reinterpret_cast<std::uintptr_t>(name));}
bool SourceSkillAnimation::animation_start(const data::AnimationTables& tables,data::AnimationRandom& random,data::AnimationStart& out,std::string& error,bool random_enabled)const{
 if(!coherent()){error="Skill animation borrowed owners are not coherent";return false;}
 return data::choose_animation_start(tables,state_.state->animation_override,random,out,error,random_enabled);
}
}
