#include "faery_gameplay_v1.hpp"
#include "character_skill_info_session_v3.hpp"
#include "character_skill_callback_session_v3.hpp"
namespace dh2::android_ui {
namespace {
using namespace dh2::character::skills;
int selected(const model_renderer::PlayerGameplayBinding& p,int& id,std::string& e){
 if(!p.active||!p.character||!p.skills||!p.save||p.difficulty<0||p.difficulty>2){e="Same player skills/Save/difficulty unavailable";return -2;}
 const auto& live=p.skills->session().property_view();
 if(!p.properties||p.save->character()!=p.character||p.skills->state().owner!=p.character||live.resolved!=p.properties->resolved||live.saved!=p.properties->saved||p.skills->native_savegame()!=p.save){e="Faery authorities do not belong to the same player";return -1;}
 id=p.save->current_faery(unsigned(p.difficulty));return 0;
}
int instance(const model_renderer::PlayerGameplayBinding& p,const Instance32*& out,std::string& e){
 int id;auto code=selected(p,id,e);if(code)return code;
 const auto& slots=p.skills->state().spells;
 if(id<0||unsigned(id)>=slots.count||!slots.items){e="Selected source spell assertion domain invalid";return -1;}
 out=slots.items[id];return 0;
}
}
int faery_spell_info_v1(const model_renderer::PlayerGameplayBinding& p,float* fraction,std::string& e){
 if(!fraction)return -1;const Instance32* spell;auto code=instance(p,spell,e);if(code)return code;
 if(!spell){*fraction=0;return 0;}
 if(!p.skills->ready()){e=p.skills->error();return -2;}
 SkillInfoSessionsV3 sessions{const_cast<model_renderer::PlayerGameplayBinding*>(&p),[](void* c,std::uintptr_t id)->dh2::character::CharacterScriptSessionV3*{
  auto& b=*static_cast<model_renderer::PlayerGameplayBinding*>(c);return id==b.character?&b.skills->session():nullptr;},{}};
 const SkillInfoServicesV1 service{&sessions,skill_info_session_invoke_v3};
 // A borrowed read-only vector projection into original GetInfo coordinator,
 // not new spell storage. AI_SpellInfo calls exactly GetInfo(level0).
 State40 view{p.character,p.skills->state().spells,{}};int id;code=selected(p,id,e);if(code)return code;
 code=character_skill_info_v1(&view,unsigned(id),0,fraction,&service);e=sessions.error;return code;
}
int faery_spell_callback_v1(const model_renderer::PlayerGameplayBinding& p,unsigned op,unsigned* result,std::string& e){
 if(!result||op>4)return -1;const Instance32* spell;auto code=instance(p,spell,e);if(code)return code;
 if(!spell){*result=0;return 0;}if(!p.skills->ready()){e=p.skills->error();return -2;}
 return skill_callback_session_v3(p.skills->session(),spell,op,result,e);
}
int faery_spell_usable_v1(const model_renderer::PlayerGameplayBinding& p,bool* usable,std::string& e){
 if(!usable||!p.state||!p.skills)return -1;
 // Original ordered early exits: same FSM UsingSkill6, Casting7, then loaded.
 if(p.state->current==6||p.state->current==7||p.skills->session().owner().lifecycle().load_step<=6){*usable=false;return 0;}
 unsigned result=0;auto code=faery_spell_callback_v1(p,skill_check_usable_v3,&result,e);if(!code)*usable=result!=0;return code;
}
int faery_hud_v1(const model_renderer::PlayerGameplayBinding& p,FaeryHudV1& out,std::string& e){
 out={};int id;auto code=selected(p,id,e);if(code)return code;out.selected=id;
 if(id<0||id>=5||!p.save->faeries_initialized()[unsigned(p.difficulty)]){e="Original selected faery Save rows unavailable";return -1;}
 out.level=p.save->faery_level(unsigned(id),unsigned(p.difficulty));
 out.unlocked=p.save->faeries()[unsigned(p.difficulty)][unsigned(id)].state!=0;
 if(!out.unlocked)return 0;
 code=faery_spell_info_v1(p,&out.fraction,e);if(code)return code;
 return faery_spell_usable_v1(p,&out.usable,e);
}
int faery_hud_use_v1(const model_renderer::PlayerGameplayBinding& p,const FaeryWorldServicesV1& s,std::string& e){
 if(!p.active||!p.character)return -1;bool allowed=false;
 if(!s.controller_allowed||s.controller_allowed(s.context,p.character,&allowed)){e="Original CTRLIsAllowed producer unavailable";return -2;}
 if(!allowed)return 0;
 if(!s.begin_cast||s.begin_cast(s.context,p.character,false)){e="Original Cmd_BeginCast producer failed";return -2;}
 if(!s.end_cast||s.end_cast(s.context,p.character,false)){e="Original Cmd_EndCast producer failed after BeginCast";return -2;}
 return 0;
}
int faery_select_v1(const model_renderer::PlayerGameplayBinding& p,int id,const FaeryWorldServicesV1& s,std::string& e){
 int old;auto code=selected(p,old,e);if(code)return code;
 if(id<0||id>=5||!p.save->faeries_initialized()[unsigned(p.difficulty)]||!p.save->faeries()[unsigned(p.difficulty)][unsigned(id)].state){e="Faery has not been unlocked in the real campaign Save";return -1;}
 if(!p.save->set_current_faery(unsigned(id),unsigned(p.difficulty),e))return -1;
 if(p.skills->update()!=1){e=p.skills->error();return -2;}
 std::uintptr_t faery=0;if(!s.faery_character||s.faery_character(s.context,p.character,&faery)){e="Actual Character faery identity unavailable";return -2;}
 if(faery){
  if(!s.refresh_model||s.refresh_model(s.context,faery)){e="Original faery model refresh failed";return -2;}
  if(!s.add_animation_set||s.add_animation_set(s.context,faery)){e="Original faery animator set refresh failed";return -2;}
 }
 // NativeHUDSetActiveFaery calls actual current Level placement after Change.
 if(!s.place_faery_and_followers||s.place_faery_and_followers(s.context,p.character)){e="Actual Level faery placement unavailable";return -2;}
 return 0;
}
}
