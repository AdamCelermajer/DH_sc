#include "native_skill_lua_services.hpp"
#include <cstring>
namespace dh::foundation::skills_animation {
using namespace dh2::character;using namespace dh2::character::skills;
NativeSkillLuaServicesV1::NativeSkillLuaServicesV1(CharacterScriptSession& session,CharacterSkillOwner& instances,
 dh2::data::SkillTables::Borrow tables,NativeFsm24& fsm,SkillAIContextV3& ai,SkillStateV4& state,
 SkillAIServices16V3 a,SkillStateServices16V4 s,std::function<bool(std::uint32_t&)> flags)
 :session_(session),instances_(instances),tables_(std::move(tables)),fsm_(fsm),ai_(ai),state_(state),required_ai_(a),required_state_(s),flags_(std::move(flags)){}
bool NativeSkillLuaServicesV1::coherent()const{return tables_&&fsm_.state&&fsm_.character&&state_.state==fsm_.state&&
 state_.character==fsm_.character&&session_.timers().owner==fsm_.character&&instances_.state().owner==fsm_.character&&
 ai_.owner&&ai_.owner->character==fsm_.character&&ai_.slots==&instances_.state()&&ai_.fields&&!fsm_.reserved;}
const dh2::data::SkillRecord* NativeSkillLuaServicesV1::row(std::uint32_t index){
 // Exact existing CharacterSkillOwner V1 source property selection. Resolve
 // uncached property28 afresh; V3's resolved-property shortcut is not imported.
 std::int32_t list;if(dh2_property_resolve(&session_.property_view(),28,&list))return nullptr;
 if(list<0||std::size_t(list)>=tables_.lists().size())list=3;
 if(std::size_t(list)>=tables_.lists().size())return nullptr;
 const auto& entries=tables_.lists()[list];if(index>=entries.size())return nullptr;
 const auto id=entries[index];if(id<0||std::size_t(id)>=tables_.skills().size())return nullptr;return &tables_.skills()[id];
}
int NativeSkillLuaServicesV1::ai_service(void* p,SkillAIContextV3* ai,const SkillAIRequest32V3* q,SkillAIResponse32V3* r){
 auto& self=*static_cast<NativeSkillLuaServicesV1*>(p);if(ai!=&self.ai_||!self.coherent())return -1;int status=0;
 if(q->operation==skill_ai_using_v3||q->operation==skill_ai_casting_v3){std::int32_t current;if(dh2_character_native_fsm_get_integer(&current,&self.fsm_,0)!=1)return -1;r->word=current==(q->operation==skill_ai_using_v3?6:7);}
 else if(q->operation==skill_ai_row_v3){const auto* row=self.row(q->index);if(!row)return -1;r->row=&row->scalar;}
 else if(q->operation==skill_ai_callback_v3){const auto& slots=self.instances_.state().skills;
  if(q->index>=slots.count||q->subject!=reinterpret_cast<std::uintptr_t>(slots.items[q->index]))return -1;
  status=self.callback_?self.callback_(slots.items[q->index],q->value,r->word,self.error_):
   skill_callback_session_v3(self.session_,slots.items[q->index],q->value,&r->word,self.error_);
 }else if(q->operation==skill_ai_property_v3){if(q->index>=224||q->value>1)return -1;std::int32_t value;
  if(q->value)value=self.session_.property_view().resolved[q->index];else if(dh2_property_resolve(&self.session_.property_view(),q->index,&value))return -1;std::memcpy(&r->word,&value,4);
 }else status=self.required_ai_.invoke?self.required_ai_.invoke(self.required_ai_.context,ai,q,r):-1;
 ai->script_step=self.session_.owner().lifecycle().load_step;
 if(!self.flags_||!self.flags_(ai->owner->flags))return -1;return status;
}
int NativeSkillLuaServicesV1::state_service(void* p,SkillStateV4* state,const SkillStateRequest32V4* q,SkillStateResponse16V4* r){
 auto& self=*static_cast<NativeSkillLuaServicesV1*>(p);if(state!=&self.state_||!self.coherent())return -1;
 if(q->operation==skill_state_current_v4){std::int32_t current;if(dh2_character_native_fsm_get_integer(&current,&self.fsm_,0)!=1)return -1;r->word=std::uint32_t(current);return 0;}
 if(q->operation==skill_state_row_v4){const auto* row=self.row(q->index);if(!row)return -1;r->identity=reinterpret_cast<std::uintptr_t>(&row->scalar);return 0;}
 if(q->operation==skill_state_timer_v4)return dh2_character_timer_start(&self.session_.timers(),q->value,0,std::int32_t(q->index),0,&self.session_.native_timer_services())<0?-1:0;
 if(q->operation==skill_state_use_v4){unsigned answer=0;return self.command(skill_ai_event_v3,0,answer);}
 return self.required_state_.invoke?self.required_state_.invoke(self.required_state_.context,state,q,r):-1;
}
int NativeSkillLuaServicesV1::command(std::uint32_t operation,std::uint32_t index,std::uint32_t& answer){
 error_.clear();if(!coherent()){error_="NPC skill Lua binding requires same canonical actor/FSM/VM/instances";return -1;}
 if(!flags_||!flags_(ai_.owner->flags)){error_="NPC native admission flags provider unavailable";return -2;}
 ai_.script_step=session_.owner().lifecycle().load_step;const auto providers=ai_services();const int status=dh2_character_skill_ai_v3(&answer,&ai_,operation,index,&providers);
 if(status&&error_.empty())error_="Required NPC skill/Lua provider failed";return status;
}
}
