#include "character_player_skills_v3.hpp"
#include "character_script_virtual.hpp"
#include "character_skill_callback_session_v3.hpp"
#include "character_skill_buff_bindings_v3.hpp"
#include "character_skill_cooldown_v3.hpp"
#include "character_skill_class_v3.hpp"
#include "character_script_call_timer.hpp"
#include "character_current_spell_v1.hpp"
#include "character_faery_element_v3.hpp"
#include <stdexcept>
#include <cstring>
namespace dh2::character::skills {
struct CharacterPlayerSkillsV3::Impl {
 const PlayerSkillInitServicesV3 init; // Copy descriptor; its context stays borrowed.
 const NativeFsm24& fsm;
 // Destruction is reverse declaration: owned instances/services die before
 // their actual private Session; all bound Session fields survive Lua close.
 std::unique_ptr<CharacterScriptSessionV3> session;
 std::unique_ptr<CharacterSkillSessionServicesV3> service;
 std::unique_ptr<CharacterSkillOwnerV3> owner;
 BuffOwner* buffs=nullptr;
 BuffServices16 buff_services{this,buff_effect};
 BuffBindings32 buff_bindings{};SkillBuffBindingsV3 buff_lua{};
 SkillCooldownBindingsV3 cooldown{};
 data::FaeryTables::Borrow faeries;
 std::shared_ptr<data::PlayerSavegameV1> saved;
 CurrentSpellBindingsV1 current_spell{};
 SkillAIStateV3 skill_ai{};
 TimerServices32 expiry{this,timer_expired,nullptr,0};
 void* original_context{};int(*original_binding)(void*,std::uint32_t,dh2_script_function*,void**){};
 void* original_property_context{};int(*original_external)(void*,std::uintptr_t,std::int32_t**){};
 const TimerServices32* original_expiry{};
 SkillInfoSessionsV3 info_sessions;
 bool ready=false,failed=false;std::string error;
 Impl(CharacterGameDesign::Borrow&& design,const CharacterScriptSessionInputV3& input,
  data::SkillTables::Borrow skills,data::FaeryTables::Borrow faeries,const fx::PreloadServices16& debug,
  const NativeFsm24& fsm,const PlayerSkillInitServicesV3& initial):init(initial),fsm(fsm),faeries(faeries),saved(input.savegame){
  if(!design||!skills||!faeries||!input.temporary||!init.vitals)throw std::invalid_argument("Player skill ownership/services incomplete");
  const auto* row=data::ai_props(*design.ai(),input.properties?input.properties->resolved[1]:-1);
  if(!row||row->script!="__player__")throw std::invalid_argument("Player skills require authored __player__ selection");
  original_context=input.gameplay_context;original_binding=input.gameplay_binding;
  original_property_context=input.property_context;original_external=input.external_property;original_expiry=input.timer_services;
  auto configured=input;configured.skill_tables=skills;configured.gameplay_context=this;configured.gameplay_binding=bind;
  configured.property_context=this;configured.external_property=external;configured.recalculate_properties=recalculate;
  configured.normal_property_class=normal_class;configured.timer_services=&expiry;
  session=CharacterScriptSessionV3::create(std::move(design),configured,error);
  if(!session)throw std::invalid_argument(error);
  service=std::make_unique<CharacterSkillSessionServicesV3>(*session,debug,fsm);
  owner=std::make_unique<CharacterSkillOwnerV3>(input.identity,&session->property_view(),std::move(skills),std::move(faeries),service->services());
  buff_bindings={&session->property_view(),&session->timers(),&session->timer_services(),&buff_services};
  buffs=dh2_character_buffs_create(&buff_bindings);if(!buffs)throw std::invalid_argument("Player Buff storage could not be retained");
  buff_lua={buffs,static_cast<unsigned>(session->classes().rows.size()),UINT32_MAX,input.skill_number_context,input.skill_number};
  cooldown={owner.get(),input.skill_number_context,input.skill_number};
  current_spell={input.identity,{this,spell_query}};
  info_sessions.context=this;info_sessions.resolve=[](void* p,std::uintptr_t id)->CharacterScriptSessionV3*{auto& t=*static_cast<Impl*>(p);return t.session->timers().owner==id?t.session.get():nullptr;};
 }
 ~Impl(){if(session)session->close();if(buffs)dh2_character_buffs_destroy(buffs);}
 static int external(void* p,std::uintptr_t id,std::int32_t** out){auto& t=*static_cast<Impl*>(p);
  if(t.buffs&&skill_buff_sheet_v3(t.buffs,id,out)==0)return 0;
  return t.original_external?t.original_external(t.original_property_context,id,out):-1;
 }
 static std::vector<data::ClassRow> rows(Impl& t){std::vector<data::ClassRow> out;for(const auto& r:t.session->classes().rows)out.push_back({r.data(),static_cast<unsigned>(r.size())});return out;}
 static int recalculate(void* p,data::PropertyView* v){auto& t=*static_cast<Impl*>(p);if(v!=&t.session->property_view())return -1;auto table=rows(t);return dh2_class_recalc_base(table.data(),table.size(),t.session->properties()->base.data(),v)?-1:0;}
 static int normal_class(void* p,data::PropertyView* v,std::int32_t id){auto& t=*static_cast<Impl*>(p);if(v!=&t.session->property_view())return -1;auto table=rows(t);return dh2_character_skill_class_v3(table.data(),table.size(),id,v)?-1:0;}
 static int buff_effect(void* p,data::PropertyView* v,const BuffRequest32* q,std::uintptr_t* out){auto& t=*static_cast<Impl*>(p);if(!q||!out)return 0;
  if(q->service==buff_recalculate)return recalculate(p,v)==0;
  // Original VisualFXManager::DropAnimatedFX494978 reads the reference and
  // returns at4949e4 when null, before any manager field/backend access.
  if(q->service==buff_fx_release&&!q->subject)return 1;
  return t.init.gameplay.effects.invoke?t.init.gameplay.effects.invoke(t.init.gameplay.effects.context,v,q,out):0;
 }
 static int bind(void* p,std::uint32_t address,dh2_script_function* fn,void** context){auto& t=*static_cast<Impl*>(p);
  if(address==0x3b6e30){*fn=current_spell_info_v1;*context=&t.current_spell;return 1;}
  if(address==0x3b6dc4){*fn=equipped_faery_element_v3;*context=&t.current_spell;return 1;}
  if(address==0x3b6df8){*fn=equipped_faery_level_v3;*context=&t.current_spell;return 1;}
  if(address==0x3b86a8){*fn=skill_create_buff_v3;*context=&t.buff_lua;return 1;}
  if(address==0x3b842c){*fn=skill_remove_buff_v3;*context=&t.buff_lua;return 1;}
  if(address==0x3b97e0){*fn=skill_set_cooldown_v3;*context=&t.cooldown;return 1;}
  if(address==0x3b90e4){*fn=spell_set_cooldown_v3;*context=&t.cooldown;return 1;}
  return t.original_binding?t.original_binding(t.original_context,address,fn,context):0;
 }
 static int spell_query(void* p,const CurrentSpellRequest24V1* q,CurrentSpellResponse16V1* r){auto& t=*static_cast<Impl*>(p);
  if(!q||!r||q->character!=t.session->timers().owner||q->difficulty!=-1)return -1;
  if(q->operation==current_spell_validate_faery||q->operation==current_spell_faery_element_v3){
   const auto& lists=t.faeries.lists();auto list=t.session->property_view().resolved[29];
   if(list<0||std::size_t(list)>=lists.size())list=0;if(lists.empty())return -1;
   // Both source GetPyCst calls are required, in their original order.
   std::int32_t first,second;if(t.session->constant("FaeryTypes","COUNT",first)||std::int32_t(q->id)<0||std::int32_t(q->id)>=first)return -1;
   if(t.session->constant("FaeryTypes","COUNT",second)||second<0||lists[list].size()!=std::size_t(second)||q->id>=lists[list].size())return -1;
   auto index=lists[list][q->id];if(index<0||std::size_t(index)>=t.faeries.faeries().size())return -1;
   if(t.faeries.faeries()[index].scalar.words[8]!=q->id)return -1;if(q->operation==current_spell_faery_element_v3)std::memcpy(&r->value,&t.faeries.faeries()[index].scalar.words[2],4);else r->value=index;return 0;
  }
  if(q->operation!=current_spell_selected_faery&&q->operation!=current_spell_saved_level)return -1;
  // Null SG accessors return source 0/-1 without reading global difficulty.
  if(!t.saved){r->value=q->operation==current_spell_selected_faery?0:-1;return 0;}
  std::int32_t difficulty=-1;if(!t.init.gameplay.difficulty||t.init.gameplay.difficulty(t.init.gameplay.difficulty_context,&difficulty)||difficulty<0||difficulty>2)return -1;
  if(q->operation==current_spell_selected_faery)r->value=t.saved->current_faery(std::uint32_t(difficulty));
  else {if(q->id>=5||!t.saved->faeries_initialized()[difficulty])return -1;r->value=t.saved->faery_level(q->id,std::uint32_t(difficulty));}return 0;
 }
 static int skill_service(void* p,SkillAIContextV3* s,const SkillAIRequest32V3* q,SkillAIResponse32V3* out){auto& t=*static_cast<Impl*>(p);if(!q||!out||!s||q->character!=t.session->timers().owner)return -1;int code=0;
  if(q->operation==skill_ai_using_v3||q->operation==skill_ai_casting_v3){std::int32_t current;if(dh2_character_native_fsm_get_integer(&current,&t.fsm,0)!=1)return -1;out->word=current==(q->operation==skill_ai_using_v3?6:7);}
  else if(q->operation==skill_ai_row_v3){auto* record=t.owner->skill(q->index);if(!record)return -1;out->row=&record->scalar;}
  else if(q->operation==skill_ai_callback_v3){const auto& slots=t.owner->state().skills;if(q->index>=slots.count||q->subject!=reinterpret_cast<std::uintptr_t>(slots.items[q->index]))return -1;code=skill_callback_session_v3(*t.session,slots.items[q->index],q->value,&out->word,t.error);}
  else if(q->operation==skill_ai_player_v3)code=t.session->source_is_player(out->word);
  else if(q->operation==skill_ai_property_v3){if(q->index>=224||q->value>1)return -1;std::int32_t value;if(q->value)value=t.session->property_view().resolved[q->index];else if(dh2_property_resolve(&t.session->property_view(),q->index,&value))return -1;std::memcpy(&out->word,&value,4);}
  else code=t.init.gameplay.skill_services.invoke?t.init.gameplay.skill_services.invoke(t.init.gameplay.skill_services.context,s,q,out):-1;
  s->script_step=t.session->owner().lifecycle().load_step;return code;
 }
 static int event_service(void* p,AIEventState64* state,const AIEventRequest40* q,std::uint32_t* out){auto& t=*static_cast<Impl*>(p);if(!q||!out)return -1;
  if(q->service==ai_event_timer_id){if(q->callee!=0x3db288)return -1;const auto& store=t.session->timers();bool owned=false;for(unsigned i=0;i<store.count;++i)if(q->subject==reinterpret_cast<std::uintptr_t>(store.slots+i)){owned=true;break;}if(!owned)return -1;*out=reinterpret_cast<Timer32*>(q->subject)->id;return 0;}
  if(q->service==ai_event_virtual&&q->operation==0x90){if(q->callee!=0x3d0ca0)return -1;AIEventResult16 r{};const AIEventServices24 services{p,event_service,63,0};ScriptSessionView view{};
   state->active=t.session->owner().active(view)?view.identity:0;
   return dh2_character_ai_event_script_timer(&r,state,q->argument,&services)?-1:0;
  }
  if(q->service==ai_event_ais_virtual&&q->operation==0x90){ScriptSessionView view{};
   if(q->callee!=0x3dcc80||!t.session->owner().find(q->subject,view)||view.kind!=script_player_iphone)return -1;
   const ScriptTimerCall16 call{view.vm,view.aliases};return dh2_character_script_call_timer(&call,q->argument);
  }
  return t.init.gameplay.events.invoke?t.init.gameplay.events.invoke(t.init.gameplay.events.context,state,q,out):-1;
 }
 static void timer_expired(void* p,std::uintptr_t character,std::int32_t event,Timer32* timer){auto& t=*static_cast<Impl*>(p);int code=-2;
  if(character!=t.session->timers().owner||!timer){t.failed=true;t.error="Player timer receiver malformed";return;}
  if(event==0x36){BuffResult24 result{};code=dh2_character_buff_expired(&result,t.buffs,timer)==1?0:-2;}
  else if(t.init.gameplay.ai){AIEventResult16 result{};AIEventPayload24 payload{reinterpret_cast<std::uintptr_t>(timer),0x3db288,0,0};const AIEventServices24 services{p,event_service,63,0};
   code=dh2_character_ai_event(&result,t.init.gameplay.ai,std::uint32_t(event),&payload,&services)?-2:0;
  }else if(t.original_expiry&&t.original_expiry->expired){t.original_expiry->expired(t.original_expiry->context,character,event,timer);code=0;}
  if(code){t.failed=true;t.error="Required player timer event delivery failed: "+std::to_string(event);}
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
CharacterPlayerSkillsV3::CharacterPlayerSkillsV3(std::unique_ptr<Impl> p):impl_(std::move(p)){}
CharacterPlayerSkillsV3::~CharacterPlayerSkillsV3()=default;
std::unique_ptr<CharacterPlayerSkillsV3> CharacterPlayerSkillsV3::create(CharacterGameDesign::Borrow&& d,const CharacterScriptSessionInputV3& in,data::SkillTables::Borrow s,data::FaeryTables::Borrow f,const fx::PreloadServices16& debug,const NativeFsm24& fsm,const PlayerSkillInitServicesV3& init,std::string& e){e.clear();try{return std::unique_ptr<CharacterPlayerSkillsV3>(new CharacterPlayerSkillsV3(std::make_unique<Impl>(std::move(d),in,std::move(s),std::move(f),debug,fsm,init)));}catch(const std::exception& x){e=x.what();return nullptr;}}
int CharacterPlayerSkillsV3::initialize(std::uint32_t final){auto& t=*impl_;if(final>1)return -1;auto& state=t.session->owner().lifecycle();if(state.active)return 0;if(t.failed)return -2;
 const auto code=t.session->advance();if(code){t.failed=true;t.error=t.session->error();return -2;}if(!state.active)return 0;
 ScriptSessionView active{};if(!t.session->owner().active(active)||active.kind!=script_player_iphone){t.failed=true;t.error="Unsupported player active kind";return -2;}
 const DeferredScriptServices16 services{&t,Impl::invoke};const auto result=dh2_character_deferred_script(&state,script_init_process,final,&services);
 if(result<0){t.failed=true;if(t.error.empty())t.error="Required player InitProcess delivery failed";return result;}
 t.ready=true;t.error.clear();return 1;
}
int CharacterPlayerSkillsV3::info(std::uint32_t index,std::uint32_t level,float* fraction){auto& t=*impl_;if(!t.ready||t.failed){t.error="Player skill startup incomplete";return -2;}const SkillInfoServicesV1 s{&t.info_sessions,skill_info_session_invoke_v3};
 // state() exposes a const view of a nonconst owned projection. The actual
 // GetInfo coordinator captures this same live owner/vector, not a copy.
 const auto code=character_skill_info_v1(const_cast<State40*>(&t.owner->state()),index,level,fraction,&s);t.error=t.info_sessions.error;return code;
}
int CharacterPlayerSkillsV3::update(){auto& t=*impl_;if(!t.ready||t.failed)return -2;const auto code=t.owner->update();t.error=t.owner->error()+t.service->error();return code;}
int CharacterPlayerSkillsV3::cleanup(){auto& t=*impl_;if(!t.ready||t.failed)return -2;auto code=t.owner->cleanup_skills();if(code==1)code=t.owner->cleanup_spells();t.error=t.owner->error()+t.service->error();return code;}
int CharacterPlayerSkillsV3::callback(std::uint32_t index,std::uint32_t op,std::uint32_t* result){auto& t=*impl_;if(op>4||!result)return -1;if(!t.ready||t.failed)return -2;const auto& s=t.owner->state().skills;if(index>=s.count||!s.items)return -1;if(!s.items[index]){*result=0;return 0;}return skill_callback_session_v3(*t.session,s.items[index],op,result,t.error);}
int CharacterPlayerSkillsV3::skill_ai(std::uint32_t op,std::uint32_t index,std::uint32_t* result){auto& t=*impl_;if(!t.ready||t.failed)return -2;if(!t.init.gameplay.skill_owner)return -2;SkillAIContextV3 state{t.init.gameplay.skill_owner,const_cast<State40*>(&t.owner->state()),&t.skill_ai,t.session->owner().lifecycle().load_step,0};const SkillAIServices16V3 services{&t,Impl::skill_service};auto code=dh2_character_skill_ai_v3(result,&state,op,index,&services);if(code)t.error="Required skill AI source service failed";return code;}
int CharacterPlayerSkillsV3::update_timers(std::uint32_t dt,std::uint32_t blocked){auto& t=*impl_;if(!t.ready||t.failed)return -2;const auto result=t.session->update_timers(dt,blocked);return t.failed?-2:result;}
std::uint32_t CharacterPlayerSkillsV3::buff_count()const noexcept{return dh2_character_buffs_count(impl_->buffs);}
int CharacterPlayerSkillsV3::buff_snapshot(BuffSnapshot48* out,std::uint32_t index)const{return dh2_character_buff_snapshot(out,impl_->buffs,index);}
CharacterScriptSessionV3& CharacterPlayerSkillsV3::session()noexcept{return *impl_->session;}
const State40& CharacterPlayerSkillsV3::state()const noexcept{return impl_->owner->state();}
SkillAIStateV3& CharacterPlayerSkillsV3::skill_ai()noexcept{return impl_->skill_ai;}
bool CharacterPlayerSkillsV3::ready()const noexcept{return impl_->ready&&!impl_->failed;}
const std::string& CharacterPlayerSkillsV3::error()const noexcept{return impl_->error;}
}
