#include "native_skill_lua_services.hpp"
#include <cstdio>
#include <cstring>
namespace dh::foundation::skills_animation {
using namespace dh2::character;using namespace dh2::character::skills;
NativeSkillLuaServices::NativeSkillLuaServices(CharacterScriptSessionV3& session,CharacterSkillOwnerV6& instances,
 NativeFsm24& fsm,SkillAIContextV3& ai,SkillStateV4& state,SkillAIServices16V3 a,SkillStateServices16V4 s,
 std::function<bool(std::uint32_t&)> flags):session_(session),instances_(instances),fsm_(fsm),ai_(ai),state_(state),required_ai_(a),required_state_(s),flags_(std::move(flags)){}
bool NativeSkillLuaServices::coherent()const{
 return fsm_.state&&fsm_.character&&state_.state==fsm_.state&&state_.character==fsm_.character&&
  session_.timers().owner==fsm_.character&&instances_.state().owner==fsm_.character&&
  ai_.owner&&ai_.owner->character==fsm_.character&&ai_.slots==&instances_.state()&&ai_.fields&&!fsm_.reserved;
}
int NativeSkillLuaServices::ai_service(void* p,SkillAIContextV3* ai,const SkillAIRequest32V3* q,SkillAIResponse32V3* r){
 auto& self=*static_cast<NativeSkillLuaServices*>(p);if(ai!=&self.ai_||!self.coherent())return -1;
 int status=0;
 if(q->operation==skill_ai_using_v3||q->operation==skill_ai_casting_v3){std::int32_t current;
  if(dh2_character_native_fsm_get_integer(&current,&self.fsm_,0)!=1)return -1;r->word=current==(q->operation==skill_ai_using_v3?6:7);
 }else if(q->operation==skill_ai_row_v3){const auto* row=self.instances_.skill(q->index);if(!row)return -1;r->row=&row->scalar;
 }else if(q->operation==skill_ai_callback_v3){const auto& slots=self.instances_.state().skills;
  if(q->index>=slots.count||q->subject!=reinterpret_cast<std::uintptr_t>(slots.items[q->index]))return -1;
  status=self.callback_?self.callback_(slots.items[q->index],q->value,r->word,self.error_):
   skill_callback_session_v3(self.session_,slots.items[q->index],q->value,&r->word,self.error_);
 }else if(q->operation==skill_ai_player_v3){status=self.session_.source_is_player(r->word);
 }else if(q->operation==skill_ai_property_v3){if(q->index>=224||q->value>1)return -1;std::int32_t value;
  if(q->value)value=self.session_.property_view().resolved[q->index];else if(dh2_property_resolve(&self.session_.property_view(),q->index,&value))return -1;
  std::memcpy(&r->word,&value,4);
 }else status=self.required_ai_.invoke?self.required_ai_.invoke(self.required_ai_.context,ai,q,r):-1;
 ai->script_step=self.session_.owner().lifecycle().load_step;
 if(!self.flags_||!self.flags_(ai->owner->flags))return -1;
 return status;
}
int NativeSkillLuaServices::state_service(void* p,SkillStateV4* state,const SkillStateRequest32V4* q,SkillStateResponse16V4* r){
 auto& self=*static_cast<NativeSkillLuaServices*>(p);if(state!=&self.state_||!self.coherent())return -1;
 if(q->operation==skill_state_current_v4){std::int32_t current;if(dh2_character_native_fsm_get_integer(&current,&self.fsm_,0)!=1)return -1;r->word=std::uint32_t(current);return 0;}
 if(q->operation==skill_state_row_v4){const auto* row=self.instances_.skill(q->index);if(!row)return -1;r->identity=reinterpret_cast<std::uintptr_t>(&row->scalar);return 0;}
 if(q->operation==skill_state_constant_v4){std::int32_t value;if(self.session_.constant("AnimStancedAnim","SL__LIST_IPHONE",value))return -1;std::memcpy(&r->word,&value,4);return 0;}
 if(q->operation==skill_state_timer_v4)return self.session_.start_timer(q->value,0,std::int32_t(q->index))<0?-1:0;
 if(q->operation==skill_state_use_v4){std::uint32_t answer=0;return self.command(skill_ai_event_v3,0,answer);}
 return self.required_state_.invoke?self.required_state_.invoke(self.required_state_.context,state,q,r):-1;
}
int NativeSkillLuaServices::command(std::uint32_t operation,std::uint32_t index,std::uint32_t& answer){
 error_.clear();
 if(!coherent()){error_="Native skill Lua binding requires same source actor/FSM/VM/instances";return -1;}
 if(!flags_||!flags_(ai_.owner->flags)){error_="Native skill admission flags provider unavailable";return -2;}
 ai_.script_step=session_.owner().lifecycle().load_step;const auto services=ai_services();const int code=dh2_character_skill_ai_v3(&answer,&ai_,operation,index,&services);
 if(code&&error_.empty())error_="Required native skill/Lua provider failed";return code;
}
NativeSkillLuaCommands::NativeSkillLuaCommands(SkillLuaCommandServices services):services_(std::move(services)),commands_{{this,skill_ai_use_v3},{this,skill_ai_begin_v3},{this,skill_ai_end_v3}}{}
int NativeSkillLuaCommands::binding(void* p,std::uint32_t address,dh2_script_function* fn,void** context){
 if(!p||!fn||!context)return -1;auto& self=*static_cast<NativeSkillLuaCommands*>(p);unsigned index;
 switch(address){case 0x3b8bd8:index=0;break;case 0x3b8cb8:index=1;break;case 0x3b93cc:index=2;break;default:return 0;}
 *fn=invoke;*context=&self.commands_[index];return 1;
}
int NativeSkillLuaCommands::invoke(void* p,const dh2_script_value* args,std::uint32_t count,dh2_script_value* out,std::uint32_t capacity,std::uint32_t* written,char* error,std::size_t length){
 if(!p||!written||(!args&&count)||(!out&&capacity))return -1;auto& entry=*static_cast<Binding*>(p);auto& self=*entry.owner;*written=0;
 // All three original wrappers return no Lua values for rejected arguments.
 if(!count||args[0].type!=DH2_SCRIPT_NUMBER)return 0;
 std::uint32_t rangeIndex=0,sourceCount=0,answer=0;std::int32_t index=0;
 if(!self.services_.unsigned_value||!self.services_.source_list_count||!self.services_.number_integer||!self.services_.command){self.error_="Native skill Lua command producer unavailable";}
 else if(self.services_.unsigned_value(args[0],rangeIndex,self.error_)&&self.services_.source_list_count(sourceCount,self.error_)){
  if(rangeIndex>=sourceCount)return 0;
  if(self.services_.number_integer(args[0],index,self.error_)&&self.services_.command(entry.operation,std::uint32_t(index),answer,self.error_))return 0;
 }
 if(self.error_.empty())self.error_="Native skill Lua command required service failed";
 if(error&&length)std::snprintf(error,length,"%s",self.error_.c_str());return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
}
}
