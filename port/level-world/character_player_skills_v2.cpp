#include "character_player_skills_v2.hpp"
#include "character_script_virtual.hpp"
#include <stdexcept>
namespace dh2::character::skills {
struct CharacterPlayerSkillsV2::Impl {
 const PlayerSkillInitServicesV2 init; // Copy descriptor; its context stays borrowed.
 // Destruction is reverse declaration: owned instances/services die before
 // their actual private Session; all bound Session fields survive Lua close.
 std::unique_ptr<CharacterScriptSessionV2> session;
 std::unique_ptr<CharacterSkillSessionServicesV2> service;
 std::unique_ptr<CharacterSkillOwner> owner;
 SkillInfoSessionsV2 info_sessions;
 bool ready=false,failed=false;std::string error;
 Impl(CharacterGameDesign::Borrow&& design,const CharacterScriptSessionInputV2& input,
  data::SkillTables::Borrow skills,data::FaeryTables::Borrow faeries,const fx::PreloadServices16& debug,
  const NativeFsm24& fsm,const PlayerSkillInitServicesV2& initial):init(initial){
  if(!design||!skills||!faeries||!input.temporary||!init.vitals)throw std::invalid_argument("Player skill ownership/services incomplete");
  const auto* row=data::ai_props(*design.ai(),input.properties?input.properties->resolved[1]:-1);
  if(!row||row->script!="__player__")throw std::invalid_argument("Player skills require authored __player__ selection");
  auto configured=input;configured.skill_tables=skills;
  session=CharacterScriptSessionV2::create(std::move(design),configured,error);
  if(!session)throw std::invalid_argument(error);
  service=std::make_unique<CharacterSkillSessionServicesV2>(*session,debug,fsm);
  owner=std::make_unique<CharacterSkillOwner>(input.identity,&session->property_view(),std::move(skills),std::move(faeries),service->services());
  info_sessions.context=this;info_sessions.resolve=[](void* p,std::uintptr_t id)->CharacterScriptSessionV2*{auto& t=*static_cast<Impl*>(p);return t.session->timers().owner==id?t.session.get():nullptr;};
 }
 static int invoke(void* p,ScriptLifecycleState64* state,const ScriptLifecycleRequest32* q,ScriptLifecycleResponse16*){
  auto& t=*static_cast<Impl*>(p);if(state!=&t.session->owner().lifecycle()||!q)return -1;
  if(q->service==script_refresh_vitals)return t.init.vitals(t.init.context,*t.session,*q);
  if(q->service==script_configure_skills||q->service==script_update_skills){const auto code=q->service==script_configure_skills?t.owner->configure():t.owner->update();if(code!=1){t.error=t.owner->error()+"; "+t.service->error();return -1;}return 0;}
  if(q->service!=script_ai_init_post&&q->service!=script_ai_init_final)return -1;
  if(!state->active)return 0;
  ScriptSessionView view{};if(!t.session->owner().find(state->active,view)||view.kind!=script_player_iphone)return -1;
  const ScriptTimerCall16 call{view.vm,view.aliases};
  return dh2_character_script_initial_virtual(&call,view.kind,q->service==script_ai_init_post?12:16);
 }
};
CharacterPlayerSkillsV2::CharacterPlayerSkillsV2(std::unique_ptr<Impl> p):impl_(std::move(p)){}
CharacterPlayerSkillsV2::~CharacterPlayerSkillsV2()=default;
std::unique_ptr<CharacterPlayerSkillsV2> CharacterPlayerSkillsV2::create(CharacterGameDesign::Borrow&& d,const CharacterScriptSessionInputV2& in,data::SkillTables::Borrow s,data::FaeryTables::Borrow f,const fx::PreloadServices16& debug,const NativeFsm24& fsm,const PlayerSkillInitServicesV2& init,std::string& e){e.clear();try{return std::unique_ptr<CharacterPlayerSkillsV2>(new CharacterPlayerSkillsV2(std::make_unique<Impl>(std::move(d),in,std::move(s),std::move(f),debug,fsm,init)));}catch(const std::exception& x){e=x.what();return nullptr;}}
int CharacterPlayerSkillsV2::initialize(std::uint32_t final){auto& t=*impl_;if(final>1)return -1;auto& state=t.session->owner().lifecycle();if(state.active)return 0;if(t.failed)return -2;
 const auto code=t.session->advance();if(code){t.failed=true;t.error=t.session->error();return -2;}if(!state.active)return 0;
 ScriptSessionView active{};if(!t.session->owner().active(active)||active.kind!=script_player_iphone){t.failed=true;t.error="Unsupported player active kind";return -2;}
 const DeferredScriptServices16 services{&t,Impl::invoke};const auto result=dh2_character_deferred_script(&state,script_init_process,final,&services);
 if(result<0){t.failed=true;if(t.error.empty())t.error="Required player InitProcess delivery failed";return result;}
 t.ready=true;t.error.clear();return 1;
}
int CharacterPlayerSkillsV2::info(std::uint32_t index,std::uint32_t level,float* fraction){auto& t=*impl_;if(!t.ready||t.failed){t.error="Player skill startup incomplete";return -2;}const SkillInfoServicesV1 s{&t.info_sessions,skill_info_session_invoke_v2};
 // state() exposes a const view of a nonconst owned projection. The actual
 // GetInfo coordinator captures this same live owner/vector, not a copy.
 const auto code=character_skill_info_v1(const_cast<State40*>(&t.owner->state()),index,level,fraction,&s);t.error=t.info_sessions.error;return code;
}
int CharacterPlayerSkillsV2::update(){auto& t=*impl_;if(!t.ready||t.failed)return -2;const auto code=t.owner->update();t.error=t.owner->error()+t.service->error();return code;}
int CharacterPlayerSkillsV2::cleanup(){auto& t=*impl_;if(!t.ready||t.failed)return -2;auto code=t.owner->cleanup_skills();if(code==1)code=t.owner->cleanup_spells();t.error=t.owner->error()+t.service->error();return code;}
CharacterScriptSessionV2& CharacterPlayerSkillsV2::session()noexcept{return *impl_->session;}
const State40& CharacterPlayerSkillsV2::state()const noexcept{return impl_->owner->state();}
bool CharacterPlayerSkillsV2::ready()const noexcept{return impl_->ready&&!impl_->failed;}
const std::string& CharacterPlayerSkillsV2::error()const noexcept{return impl_->error;}
}
