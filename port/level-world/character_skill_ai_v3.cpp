#include "character_skill_ai_v3.hpp"
#include "character_skill_callbacks_v3.hpp"
#include <cstring>
namespace {using namespace dh2::character::skills;
bool aligned(const void* p,std::size_t n){return p&&reinterpret_cast<std::uintptr_t>(p)%n==0;}
struct Run {
 SkillAIContextV3& s;const SkillAIServices16V3& cb;
 bool valid(){return aligned(s.owner,alignof(SkillAIOwnerV3))&&s.owner->character&&!s.owner->reserved&&aligned(s.slots,alignof(State40))&&!s.slots->skills.reserved&&aligned(s.fields,alignof(SkillAIStateV3))&&!s.fields->reserved&&!s.reserved&&s.slots->skills.count<=1048576&&(!s.slots->skills.count||aligned(s.slots->skills.items,alignof(void*)));}
 bool call(unsigned op,unsigned index,unsigned value,std::uintptr_t subject,SkillAIResponse32V3& r){if(!valid())return false;r={};SkillAIRequest32V3 q{op,index,value,0,s.owner->character,subject};return !cb.invoke(cb.context,&s,&q,&r)&&valid();}
 bool query(unsigned op,unsigned index,unsigned value,unsigned& word){SkillAIResponse32V3 r;if(!call(op,index,value,0,r))return false;word=r.word;return true;}
 bool slot(unsigned index,const Instance32*& out,bool strict){if(!valid())return false;if(index>=s.slots->skills.count){out=nullptr;return !strict;}out=s.slots->skills.items[index];return !out||aligned(out,alignof(Instance32));}
 bool callback(unsigned index,unsigned op,unsigned& out){const Instance32* instance;if(!slot(index,instance,true)||!instance)return false;SkillAIResponse32V3 r;if(!call(skill_ai_callback_v3,index,op,reinterpret_cast<std::uintptr_t>(instance),r))return false;out=r.word;return true;}
 bool row(unsigned index,const dh2::data::SkillProjection76*& out){SkillAIResponse32V3 r;if(!call(skill_ai_row_v3,index,0,0,r)||!aligned(r.row,alignof(dh2::data::SkillProjection76)))return false;out=r.row;return true;}
 bool usable(unsigned index,unsigned& out){unsigned word;out=0;if(!query(skill_ai_using_v3,0,0,word))return false;if(word&&!(s.owner->flags&0x8000))return true;if(!query(skill_ai_casting_v3,0,0,word))return false;if(word||s.script_step<=6)return true;const Instance32* instance;if(!slot(index,instance,false))return false;if(!instance)return true;return callback(index,skill_check_usable_v3,out);}
 bool active(unsigned index,unsigned& out){const Instance32* instance;if(!slot(index,instance,true))return false;unsigned word;if(!query(skill_ai_using_v3,0,0,word))return false;if(word&&std::uint32_t(s.fields->current)==index){out=1;return true;}if(!slot(index,instance,true))return false;out=0;return !instance||callback(index,skill_check_active_v3,out);}
 bool begin(unsigned index,unsigned& out){const dh2::data::SkillProjection76* record;if(!row(index,record))return false;const auto type=record->words[18];unsigned answer;
  if(type==1){if(!active(index,answer))return false;if(answer){if(!callback(index,skill_pre_v3,answer))return false;out=1;return true;}}
  if(!usable(index,answer))return false;out=0;if(!answer)return true;
  // Original retains the row pointer from before all Check callbacks.
  s.fields->current=static_cast<std::int32_t>(index);s.fields->continued=s.fields->last=0;
  SkillAIResponse32V3 response;if(!call(skill_ai_set_state_v3,index,record->words[2]&255,0,response))return false;
  if(!query(skill_ai_player_v3,0,0,answer))return false;
  if(answer){if(!query(skill_ai_property_v3,216,1,answer))return false;
   if(!call(skill_ai_trophy_manager_v3,0,0,0,response))return false;const auto manager=response.identity;
   if(!query(skill_ai_property_v3,216,0,answer))return false;std::int32_t signed_value;std::memcpy(&signed_value,&answer,4);
   if(signed_value>199){if(!query(skill_ai_network_player_v3,0,0,answer))return false;if(answer){
    if(!call(skill_ai_trophy_catalog_v3,0,0,0,response)||response.count>1048576||(response.count&&!aligned(response.names,alignof(void*))))return false;
    auto names=response.names;auto count=response.count;unsigned trophy=UINT32_MAX;
    for(unsigned i=0;i<count;++i){if(!names[i])return false;if(!std::strcmp("epic_withskills",names[i])){trophy=i;break;}}
    if(!call(skill_ai_unlock_v3,trophy,0,manager,response))return false;
   }}
  }return query(skill_ai_using_v3,0,0,out);
 }
 bool end(unsigned index){unsigned answer;if(!query(skill_ai_using_v3,0,0,answer))return false;if(!answer)return true;const dh2::data::SkillProjection76* record;if(!row(index,record))return false;if(record->words[18]!=2)return true;if(!s.fields->continued){s.fields->last=1;return true;}SkillAIResponse32V3 response;return call(skill_ai_stop_loop_v3,0,1,0,response);}
 bool cancel(unsigned index){const Instance32* instance;if(!slot(index,instance,true))return false;if(!instance)return true;const dh2::data::SkillProjection76* record;if(!row(index,record))return false;if(record->words[18]!=1)return true;unsigned answer;if(!callback(index,skill_check_active_v3,answer))return false;return !answer||callback(index,skill_pre_v3,answer);}
 bool run(unsigned op,unsigned index,unsigned& out){out=0;switch(op){case skill_ai_usable_v3:return usable(index,out);case skill_ai_active_v3:return active(index,out);case skill_ai_begin_v3:return begin(index,out);case skill_ai_end_v3:return end(index);case skill_ai_use_v3:if(!begin(index,out))return false;return !out||end(index);case skill_ai_cancel_v3:return cancel(index);default:{index=std::uint32_t(s.fields->current);const Instance32* instance;if(!slot(index,instance,false))return false;if(!instance)return true;unsigned ignored;return callback(index,op==skill_ai_focus_v3?skill_pre_v3:op==skill_ai_event_v3?skill_use_v3:skill_post_v3,ignored);}}}
};}
extern "C" int dh2_character_skill_ai_v3(std::uint32_t* out,SkillAIContextV3* state,std::uint32_t op,std::uint32_t index,const SkillAIServices16V3* services){
 if(!aligned(out,alignof(std::uint32_t))||!aligned(state,alignof(SkillAIContextV3))||op>skill_ai_blur_v3||!aligned(services,alignof(SkillAIServices16V3))||!services->invoke)return -1;Run run{*state,*services};if(!run.valid())return -1;unsigned answer;if(!run.run(op,index,answer))return -2;*out=answer;return 0;
}
