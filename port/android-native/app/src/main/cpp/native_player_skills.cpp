#include "native_player_skills.hpp"
#include "../../../../../level-world/character_clear_target_v1.hpp"
#include "player_gameplay_audio.hpp"
#include "player_skill_session_v1.hpp"
#include "player_skill_update_session_v1.hpp"
#include "player_skill_use_session_v1.hpp"
#include "player_skill_cleanup_session_v1.hpp"
#include "character_dead_focus_services_v1.hpp"
#include "animation_tables.hpp"
#include "ai.hpp"
#include "player_skill_property_services_v1.hpp"
#include "player_savegame_v1.hpp"
#include "player_save_load_owner_v1.hpp"
#include "savegame_options_v1.hpp"
#include "character_mana_services_v1.hpp"
#include "character_current_spell_v1.hpp"
#include "character_player_scalar_services_v1.hpp"
#include "character_equipped_faery_element_v1.hpp"
#include "character_current_equipped_faery_v1.hpp"
#include "character_player_buffs_v1.hpp"
#include "player_skill_progression_v1.hpp"
#include "player_initial_skill_grants_v1.hpp"
#include "fresh_inventory_owned_v4.hpp"
#include "character_skill_cooldown_services.hpp"
#include "character_coordinator.hpp"
#include "character_ai_skill_commands_v1.hpp"
#include "character_ai_skill_machine_projection_v1.hpp"
#include "trophy_manager_owner_v1.hpp"
#include "player_ais_lifecycle_v1.hpp"
#include "ais_external_init_callbacks.hpp"
#include "player_ai_timer_events_v1.hpp"
#include "native_debug_files.hpp"
#include "mod_assets.hpp"
#include "../../../../../../port/adam-script-runtime/script_game_bindings.h"
#include "../../../../../../port/level-world/player_skill_timer_provider_v1.hpp"
#include "../../../../../../port/level-world/player_script_timer_dispatch_v1.hpp"
extern "C" {
#include "../../../../../../port/pydata-names/names.h"
#include "../../../../../../port/pydata-constants/constants.h"
}
#include <android/log.h>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <exception>
#include <map>
#include <stdexcept>

namespace dh2::native::player_skills {
// One native Player AIS/skill owner. Source load and InitProcess drive the same
// Session/preparation/property/Coordinator graph. Full Character InitPost,
// profile/equipment/grants, frame AI and skill activation remain separate.
namespace {
struct InitialVirtuals {void(*init)();void(*post)();void(*final)();};
// Native dispatch identities for the proven inherited initial virtuals. These
// do not represent a complete original Binder or reconstructed C++ vtable.
const InitialVirtuals base_initials{dh2::ais_external_init_callbacks::default_init,
 dh2::ais_external_init_callbacks::default_post,dh2::ais_external_init_callbacks::default_final};
const InitialVirtuals player_initials=base_initials,iphone_initials=base_initials;
}
struct Runtime::Impl {
 Bindings bindings;
 AAssetManager* assets=nullptr;
 dh2::ais_player_init_vcb::State ais{};
 dh2::data::AiProps declaration;
 dh2::player_ais_lifecycle_v1::PlayerFields source_fields{};
 dh2::player_ais_lifecycle_v1::Tables source_tables{};
 dh2::player_skill_session_v1::Session* session_slot=nullptr;
 dh2::character_player_skills_preparation_v3::Owner* preparation_slot=nullptr;
 dh2::player_skill_update_session_v1::Runtime* update_slot=nullptr;
 dh2::player_skill_use_session_v1::Runtime* use_slot=nullptr;
 dh2::character_level_runtime::Runtime vitals;
 dh2::character_level_runtime::DesignBinding design_binding{};
 dh2::character_level_runtime::Storage vitals_storage{};
 std::unique_ptr<dh2::player_ais_lifecycle_v1::Runtime> lifecycle;
 std::unique_ptr<dh2::player_ai_timer_events_v1::Runtime> ai_ticks;
 unsigned regen_ticks=0,dot_ticks=0;
 dh2::player_ais_lifecycle_v1::Result loaded{},initialized_result{};
 dh2::data::PropertyView property_view{};
 std::vector<dh2::data::ClassRow> class_rows;
 std::unique_ptr<dh2::character_player_buffs_v1::Owner> buffs;
 dh2::character_player_buffs_v1::CallbackBindings buff_callbacks{};
 std::unique_ptr<dh2::character_player_skills_preparation_v3::Owner> preparation;
 std::optional<dh2::character_player_skills_preparation_v3::Owner::TimerFieldLease> timer_fields;
 std::unique_ptr<dh2::player_skill_session_v1::Session> session;
 std::unique_ptr<dh2::player_skill_update_session_v1::Runtime> updates;
 std::unique_ptr<dh2::player_skill_use_session_v1::Runtime> uses;
 std::unique_ptr<dh2::player_skill_cleanup_session_v1::Runtime> cleanups;
 std::unique_ptr<dh2::player_ai_death_v1::Runtime> death;
 std::unique_ptr<dh2::character_dead_focus_services_v1::Adapter> focus_services;
 dh2::character::set_target::Services target_services{};
 std::unique_ptr<dh2::character_ai_skill_machine_projection_v1::Projection> skill_machine;
 std::uintptr_t skill_animation_owner=0;
 std::unique_ptr<dh2::character_cast_lifecycle_v1::Projection> cast_state_projection;
 dh2::player_ai_death_v1::Result died_result{};
 dh2::character_skill_cooldown_services::Services cooldown{};
 dh2::player_skill_property_services_v1::Bindings property_services{};
 dh2::character_mana_services_v1::State mana_state{};
 dh2::character_mana_services_v1::Globals mana_globals{};
 dh2::character_mana_services_v1::Services mana_services{};
 dh2::character_mana_services_v1::CallbackContext mana_callbacks{};
 const dh2::data::PlayerSavegameV1* saved_slot=nullptr;
 dh2::character_faery_selection::Globals faery_globals{};
 dh2::character_faery_selection::Services faery_services{};
 dh2::character_current_spell_v1::SavedBindings spell_saved{};
 dh2::character_current_spell_v1::Bindings spell_callback{};
 dh2::character_equipped_faery_element_v1::Bindings element_callback{};
 dh2::character_current_equipped_faery_v1::Bindings equipped_callback{};
 // LuaScript+0x1c belongs to this AIS and its retained VM. It is a distinct
 // source component, not the Character property sheet or a global dictionary.
 dh2::character_player_scalar_services_v1::IntegerMap script_integers;
 dh2::character_player_scalar_services_v1::State scalar_state{};
 dh2::character_player_scalar_services_v1::Bindings scalar_callbacks{};
 dh2::player_skill_timer_provider_v1::Bindings timer_bindings{};
 dh2::player_script_timer_dispatch_v1::Bindings timer_dispatch{};
 std::map<std::string,std::vector<std::uint8_t>> named_bytes;
 std::map<std::string,dh2_pynames_view> named_views;
 std::vector<std::uint8_t> animation_constants_bytes;
 dh2_pycst_view animation_constants{};
 dh2::character_ai_set_skills_and_spells::Result prepared{};
 dh2::player_skill_update_session_v1::Result updated{};
 dh2::player_skill_update_session_v1::SelectedFaeryResult selected_updated{};
 unsigned update_attempts=0,timer_callbacks=0;
 bool update_blocked=false,initialized=false,deserialize_lifecycle_busy=false;
 bool terminal_cleanup_attempted=false,terminal_cleanup_complete=false;
 std::string terminal_cleanup_failure;
 std::string error;
 explicit Impl(Bindings b):bindings(std::move(b)),assets(bindings.assets),declaration(*bindings.declaration){
  // Retain the immutable authored row used by this AIS. GL reload replaces
  // the global AI table backing; it must not invalidate this live borrower.
  bindings.declaration=&declaration;
 }
 ~Impl(){
  // The Coordinator borrows this projection. Explicitly detach it before
  // destruction; never synthesize a state transition to make teardown pass.
  if(cast_state_projection){
   if(!bindings.coordinator||
      !bindings.coordinator->unbind_cast_projection(cast_state_projection.get()))
    std::terminate();
   cast_state_projection.reset();
  }
  // Prevent callbacks to retiring instances. The retained Coordinator remains
  // the sole timer owner; unrelated Character timers are not removed here.
  const auto stop_owned=[&](std::uint32_t id,std::int32_t event,std::uintptr_t ref){
   const auto& timers=bindings.coordinator->timers();
   if(id<timers.count){const auto& timer=timers.slots[id];
    if(timer.active&&timer.id==id&&timer.event==event&&timer.user_ref==ref)
     bindings.coordinator->stop_timer(id);
   }
  };
  if(bindings.source_ai){stop_owned(bindings.source_ai->word_10,0x33,0);stop_owned(bindings.source_ai->word_14,0x34,0);}
  if(timer_fields&&preparation){
   using List=dh2::character_ai_set_skills_and_spells::List;
   for(const auto list:{List::skill,List::faery})
    for(std::uint32_t index=0;index<preparation->slots(list).size();++index){
     dh2::character_player_skills_preparation_v3::Owner::TimerFieldSlot slot{};
     if(timer_fields->slot(bindings.character,list,index,slot)&&slot.field18&&*slot.field18>=0)
      stop_owned(std::uint32_t(*slot.field18),0x35,0);
    }
  }
  if(buffs){dh2::character_player_buffs_v1::Snapshot row{};
   for(std::uint32_t i=0;buffs->snapshot(i,&row);++i){
    const auto& timers=bindings.coordinator->timers();
    if(row.timer>=0&&std::uint32_t(row.timer)<timers.count){
     const auto& timer=timers.slots[row.timer];
     if(timer.active&&timer.id==std::uint32_t(row.timer)&&timer.event==0x36&&timer.user_ref==row.instance)
      bindings.coordinator->stop_timer(std::uint32_t(row.timer));
    }
   }
  }
  death.reset();focus_services.reset();cleanups.reset();ai_ticks.reset();lifecycle.reset();uses.reset();updates.reset();session.reset();
  if(buffs){dh2::character_player_buffs_v1::Result result{};
   if(buffs->retire(&result)!=dh2::character_player_buffs_v1::Status::complete)
    __android_log_print(ANDROID_LOG_ERROR,"DH2Native","Native Player buff retirement provider failed");
  }
  buffs.reset();timer_fields.reset();preparation.reset();
 }
 void refresh_properties(){
  property_view=dh2::data::property_view(*bindings.rules,*bindings.properties);
  if(buffs&&buffs->attach(&property_view)!=dh2::character_player_buffs_v1::Status::complete)
   throw std::runtime_error("Native Player buff property groups lost");
 }
 void refresh_class_rows(){
  // Level reload replaces the catalogue's vector backing. Keep only borrowed
  // descriptors, and rebind them before any retained buff can recalculate.
  class_rows.clear();class_rows.reserve(bindings.classes->rows.size());
  for(const auto& row:bindings.classes->rows)class_rows.push_back({row.data(),std::uint32_t(row.size())});
  design_binding={reinterpret_cast<std::uintptr_t>(bindings.design),bindings.design};
  vitals_storage={bindings.character,reinterpret_cast<std::uintptr_t>(bindings.properties),
   bindings.properties->base.data(),&property_view,class_rows.data(),std::uint32_t(class_rows.size()),
   &design_binding,1,&bindings.debug->globals(),&bindings.debug->services()};
 }
 void log_buffs(const char* phase){
  refresh_properties();
  dh2::character_player_buffs_v1::Snapshot row{};
  for(std::uint32_t i=0;buffs->snapshot(i,&row);++i){
   std::uint64_t hash=14695981039346656037ull;
   for(unsigned p=0;p<224;++p){std::uint32_t word;std::memcpy(&word,&row.sheet[p],4);
    for(unsigned byte=0;byte<4;++byte){hash^=(word>>(byte*8))&255u;hash*=1099511628211ull;}}
   __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native Player buff snapshot | phase %s | count %u | groups %u | id %d | instance %p | timer %d | strength %u | sheet %016llx | same VM/properties/Coordinator",
    phase,buffs->count(),property_view.group_count,row.id,reinterpret_cast<void*>(row.instance),row.timer,row.strength,static_cast<unsigned long long>(hash));
   for(std::uint32_t p=0;p<bindings.fields->size();++p)
    if((*bindings.fields)[p].find("Resistance")!=std::string::npos)
     __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native Player buff property | phase %s | name %s | index %u | buff %d | resolved %d",
      phase,(*bindings.fields)[p].c_str(),p,row.sheet[p],bindings.properties->resolved[p]);
  }
 }
 void log_ai_timers(const char* phase){
  const auto& timers=bindings.coordinator->timers();
  for(const auto event:{0x33,0x34}){
   const auto id=event==0x33?bindings.source_ai->word_10:bindings.source_ai->word_14;
   if(id==UINT32_MAX){
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native Player AI timer detached | phase %s | event %x | source id %u | no source timer association",phase,event,id);
    continue;
   }
   if(id>=timers.count)throw std::runtime_error("Native Player AI timer slot unavailable");
   const auto& timer=timers.slots[id];
   if(timer.id!=id||timer.event!=event||timer.user_ref!=0)
    throw std::runtime_error("Native Player AI timer owner differs");
   __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native Player AI timer snapshot | phase %s | event %x | slot %u | duration %u | repeat %d | elapsed %u | active %u | paused %u | ref %zu | delivered %u | HP %d / %d | MP %d / %d",
    phase,event,id,timer.duration_ms,timer.repeat,timer.elapsed_ms,unsigned(timer.active),unsigned(timer.paused),std::size_t(timer.user_ref),
    event==0x33?regen_ticks:dot_ticks,bindings.properties->resolved[36],bindings.properties->resolved[38],bindings.properties->resolved[41],bindings.properties->resolved[43]);
  }
  if(bindings.coordinator->state.current==12){
   __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native Player dead snapshot | phase %s | state %d | HP %d | dead %u | buffs %u | groups %u | update attempts %u | VM %p | AIS %p | source timer33 %u | source timer34 %u",
    phase,bindings.coordinator->state.current,bindings.properties->resolved[36],unsigned(*bindings.dead),buffs->count(),property_view.group_count,update_attempts,static_cast<void*>(session->vm()),reinterpret_cast<void*>(ais.ais),bindings.source_ai->word_10,bindings.source_ai->word_14);
   for(std::uint32_t i=0;i<timers.count;++i){const auto& timer=timers.slots[i];
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native Player dead timer | phase %s | slot %u | event %x | active %u | paused %u | elapsed %u | ref %zu",phase,i,unsigned(timer.event),unsigned(timer.active),unsigned(timer.paused),timer.elapsed_ms,std::size_t(timer.user_ref));
   }
  }
 }
 static auto& self(void* raw){return *static_cast<Impl*>(raw);}
 static int discard_kill_return(void*,const dh2_script_first_return_v1*,char*,std::size_t){return 0;}
 static int kill_credit_backend(void* raw,dh2::character::AIEventState64* state,
          const dh2::character::AIEventRequest40* request,std::uint32_t* value){
  auto& s=self(raw);
  if(!state||!state->owner||!request||!value||!s.initialized||!s.session||
     !s.bindings.source_ai||!s.bindings.coordinator)return 1;
  const auto& owner=*state->owner;
  if(state->ai!=s.bindings.ai||state->active!=s.bindings.source_ai->active_ais_1c||
     owner.owner!=s.bindings.character||owner.controller!=s.bindings.controller||
     owner.state_machine!=reinterpret_cast<std::uintptr_t>(&s.bindings.coordinator->state)||
     owner.properties!=reinterpret_cast<std::uintptr_t>(s.bindings.properties)||
     request->event!=4||!request->payload)return 1;
  if(request->service==dh2::character::ai_event_state_event){
   if(request->operation||request->subject!=reinterpret_cast<std::uintptr_t>(&s.bindings.coordinator->state)||request->callee)return 1;
   return s.bindings.coordinator->event(4,request->payload)<0?1:0;
  }
  if(request->service!=dh2::character::ai_event_ais_virtual||request->operation!=0xb0||
     request->subject!=s.ais.ais||request->callee!=dh2::player_enemy_kill_credit_v1::ais_player_on_kill_identity||
     !state->active||state->active!=s.ais.ais)return 1;
  // AISPlayer::OnKill uses its initialization-time VCB bit before calling
  // LuaScript::Call. An absent bit is the original no-op, not a missing hook.
  if(!(s.ais.flags_b8&0x400u))return 0;
  dh2_script_value argument{};argument.type=DH2_SCRIPT_IDENTITY;argument.identity=request->payload;
  std::string call_error;
  const int status=s.session->call("OnKill",&argument,1,0,discard_kill_return,nullptr,call_error);
  if(status<0){
   __android_log_print(ANDROID_LOG_ERROR,"DH2Native","Native Player AIS OnKill provider failed | victim %zu | status %d | %s | Character::Kill continuation remains source ordered",
    std::size_t(request->payload),status,call_error.c_str());
   return 1;
  }
  if(status>0)
   __android_log_print(ANDROID_LOG_WARN,"DH2Native","Native Player AIS OnKill Lua returned ordinary error | victim %zu | status %d | %s | source void caller ignores return",
    std::size_t(request->payload),status,call_error.c_str());
  return 0;
 }
 static int revived_backend(void* raw,
     const dh2::character_ai_on_revived_v1::Request* request,
     dh2::character_ai_on_revived_v1::Reply* reply,std::string& error){
  auto& s=self(raw);
  using namespace dh2::character_ai_on_revived_v1;
  if(!request||!reply||!s.initialized||!s.bindings.coordinator||
     !s.bindings.source_ai||request->character!=s.bindings.character||
     request->ai!=s.bindings.ai||request->active_ais!=s.ais.ais||
     s.bindings.source_ai->active_ais_1c!=s.ais.ais){
   error="Player CharAI::OnRevived crossed its retained CharAI/Coordinator/AIS";
   return 1;
  }
  *reply={};reply->timer_id=-1;
  switch(request->operation){
  case Operation::stop_all_timers:
   return s.bindings.coordinator->stop_timers()==1?0:1;
  case Operation::get_ai_tick:
  case Operation::get_dot_tick:{
   if(!s.bindings.design){error="Player CharAI::OnRevived has no CharacterDesign constants";return 1;}
   const char* key=request->operation==Operation::get_ai_tick?"AI_Tick":"DoT_Tick";
   dh2_pycst_result value{};
   if(dh2_pycst_get(s.bindings.design,"CharacterDesign",15,key,
         std::uint32_t(std::strlen(key)),&value)){
    error=std::string("Player CharAI::OnRevived constant lookup failed: ")+key;
    return 1;
   }
   // PyDataConstants::getConstant returns zero for an absent name.
   reply->duration_ms=value.found&&value.value>0?
       static_cast<std::uint32_t>(value.value):0u;
   return 0;
  }
  case Operation::start_ai_tick:
  case Operation::start_dot_tick:{
   const auto expected=request->operation==Operation::start_ai_tick?51:52;
   if(request->event!=expected||request->repeat!=-1){
    error="Player CharAI::OnRevived timer arguments differ from source";return 1;
   }
   reply->timer_id=s.bindings.coordinator->start_timer(
       request->duration_ms,request->repeat,request->event,0);
   if(reply->timer_id<0){error="Player CharAI::OnRevived timer allocation failed";return 1;}
   return 0;
  }
  case Operation::active_ais_on_revived:
   // The canonical Player AIS inherits source AISDefault::OnRevived, a
   // four-byte bx-lr leaf at 0x3dbe94. No Lua callback is present here.
   if(request->active_ais!=s.ais.ais){
    error="Player AIS::OnRevived does not belong to the retained Player AIS";
    return 1;
   }
   return 0;
  }
  error="Unexpected Player CharAI::OnRevived operation";return 1;
 }
 static int target_service(void* raw,const dh2::character::set_target::Request* q,dh2::character::set_target::Response* r){
  auto& s=self(raw);using namespace dh2::character::set_target;
  if(!q||!r||q->ai_identity!=s.bindings.ai)return 1;
  auto& debug=*s.bindings.debug;using Status=dh2::debug_switches::Status;
  if(q->operation==debug_switches_load)return debug.runtime().load(debug.globals(),debug.services())==Status::complete?0:1;
  if(q->operation==debug_switch_lookup){
   const char* key=q->key==trace_target_changes?"IsTracingCharAITarget":q->key==trace_target_details?"isTracingCharAITarget":nullptr;
   if(!key)return 1;std::uint8_t value=0;
   if(debug.runtime().get_switch(key,debug.globals(),debug.services(),value)!=Status::complete)return 1;
   r->word=value;r->reserved=0;return 0;
  }
  // A null death target takes the genuine source branch without these
  // services. Nonnull target/sight providers are not manufactured here.
  return 1;
 }
 static int death_service(void* raw,const dh2::player_ai_death_v1::Request* q,dh2::player_ai_death_v1::Reply* r,std::string& why){
  auto& s=self(raw);using namespace dh2::player_ai_death_v1;
  if(!q||!r||q->ai!=s.bindings.ai||q->character!=s.bindings.character){why="Player death identity differs";return 1;}
  switch(q->operation){
  case Operation::anim_stance:{
   if(q->subject!=s.bindings.character||!s.bindings.death_anim_stance.get_anim_stance){
    why="Player death reached unbound source GetAnimStance";return 1;
   }
   std::int32_t stance=0;
   if(s.bindings.death_anim_stance.get_anim_stance(
        s.bindings.death_anim_stance.context,q->subject,&stance)!=0){
    why="Player death source GetAnimStance provider failed";return 1;
   }
   r->word=stance;return 0;
  }
  case Operation::relations:case Operation::retain_peer:case Operation::contains_mirror:
  case Operation::erase_mirror:case Operation::clear_relations:case Operation::notify_deaggro:
  case Operation::release_peer:
   if(s.bindings.death_aggro.invoke)
    return s.bindings.death_aggro.invoke(s.bindings.death_aggro.context,q,r,why);
   if(q->operation!=Operation::relations&&q->operation!=Operation::clear_relations){
    why="Player death reached unbound aggro relation provider";return 1;
   }
   {
   auto& tree=q->direction==Direction::outgoing?s.bindings.source_ai->tree_7c:s.bindings.source_ai->tree_94;
   const auto header=reinterpret_cast<std::uintptr_t>(&tree);
   if(tree.count||tree.parent||tree.left!=header||tree.right!=header){why="Player death reached unbound nonempty source aggro graph";return 1;}
   r->count=0;r->peers=nullptr;return 0;
   }
  case Operation::skill_cleanup:case Operation::spell_cleanup:{
   using List=dh2::character_player_skills_preparation_v3::source::List;
   dh2::player_skill_cleanup_session_v1::Result result{};
   if(!s.cleanups||s.cleanups->cleanup(q->operation==Operation::skill_cleanup?List::skill:List::faery,result,why))return 1;
   __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native Player source cleanup | list %u | examined %u | callbacks %u | completed %u | lua errors %u | same VM/instances; no blanket timer stop",unsigned(result.list),result.examined,result.cleanup_calls,result.completed,result.lua_errors);
   return 0;
  }
  default:why="Player death reached unbound group/AIS/nonempty aggro provider";return 1;
  }
 }
 static int fail(char* message,std::size_t size,const char* name){
  if(message&&size)std::snprintf(message,size,"unresolved native Player skill provider: %s",name?name:"unknown");
  return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
 }
 static bool integer(const dh2_script_value& value,std::int32_t& out){
  if(value.type!=DH2_SCRIPT_NUMBER||!std::isfinite(value.number)||value.number<-2147483648.f||value.number>=2147483648.f)return false;
  out=static_cast<std::int32_t>(value.number);return true;
 }
 static void number(dh2_script_value& value,float n){value={};value.type=DH2_SCRIPT_NUMBER;value.number=n;}
 static std::int32_t list_count(void* raw,std::uintptr_t character,std::uint32_t kind,std::uint32_t* out){
  auto& s=self(raw);if(!out||!s.bindings.character||character!=s.bindings.character||kind>1||!s.bindings.tables)return 1;
  s.refresh_properties();
  std::int32_t selector=0;
  if(dh2_property_resolve(&s.property_view,kind?29:28,&selector))return 1;
  const auto* list=kind?s.bindings.tables->faery_list(selector):s.bindings.tables->skill_list(selector);
  if(!list||list->members.size()>UINT32_MAX)return 1;
  *out=std::uint32_t(list->members.size());return 0;
 }
 static std::int32_t slot(void* raw,std::uintptr_t character,std::uint32_t kind,std::uint32_t index,
                        dh2::character_skill_cooldown_services::Slot* out){
  auto& s=self(raw);if(!out||kind>1||!s.timer_fields)return 1;
  dh2::character_player_skills_preparation_v3::Owner::TimerFieldSlot found{};
  const auto list=kind?dh2::character_ai_set_skills_and_spells::List::faery:dh2::character_ai_set_skills_and_spells::List::skill;
  if(!s.timer_fields->slot(character,list,index,found))return 1;
  *out={found.instance,found.field18};return 0;
 }
 static std::int32_t unsupported_number(void*,const dh2_script_value*,float*){return 1;}
 static int buff_service(void* raw,dh2::data::PropertyView* view,
               const dh2::character_player_buffs_v1::Request* q,
               dh2::character_player_buffs_v1::Response* out){
  auto& s=self(raw);if(!q||!out||view!=&s.property_view||q->character!=s.bindings.character)return 1;
  using Op=dh2::character_player_buffs_v1::Operation;
  switch(q->operation){
   case Op::timer_start:
    out->word=s.bindings.coordinator->start_timer(q->duration,q->repeat,q->event,q->subject);
    return out->word< -1?1:0;
   case Op::timer_stop:return s.bindings.coordinator->stop_timer(std::uint32_t(q->id))<0?1:0;
   case Op::timer_time_left:return dh2_character_timer_time_left(&out->elapsed,&out->duration,&s.bindings.coordinator->timers(),std::uint32_t(q->id))==1?0:1;
   case Op::apply_class:{
    std::int32_t* sheet=nullptr;
    if(!s.buffs||!s.buffs->owned_sheet(q->subject,&sheet)||sheet!=q->sheet)return 1;
    return int(dh2_class_apply(s.class_rows.data(),std::uint32_t(s.class_rows.size()),q->id,sheet,view->resolved));
   }
   case Op::recalculate:return int(dh2_class_recalc_base(s.class_rows.data(),std::uint32_t(s.class_rows.size()),s.bindings.properties->base.data(),view));
   case Op::fx_load:case Op::fx_release:case Op::fx_object:case Op::fx_enable:
    // The real VisualFXManager service is borrowed from the renderer. A
    // reached effect operation fails explicitly when that owner is absent.
    return s.bindings.buff_fx.invoke?
      s.bindings.buff_fx.invoke(s.bindings.buff_fx.context,q,out):1;
  }
  return 1;
 }
 static int resolve(void* raw,const std::string& path,dh2::player_skill_session_v1::Resource& output,std::string& error){
  auto& s=self(raw);
  try{
   if(!s.assets||path.compare(0,5,"data/")||!dh2::mods::valid_path(path))throw std::runtime_error("invalid Player skill asset path");
   auto bytes=s.bindings.read(s.assets,path.substr(5));
   output={path,std::make_shared<const std::vector<std::uint8_t>>(std::move(bytes))};return 0;
  }catch(const std::exception& e){error=e.what();return -1;}
 }
 static std::int32_t faery_constant(void* raw,dh2::character_faery_selection::Character* character,
                   const dh2::character_faery_selection::Request* q,dh2::character_faery_selection::Response* out){
  auto& s=self(raw);if(!character||!q||!out||!s.bindings.character||character->identity!=s.bindings.character)return 1;
  dh2_pycst_result value{};
  if(dh2_pycst_get(s.bindings.faery_constants,q->category,std::strlen(q->category),q->key,std::strlen(q->key),&value)||!value.found)return 1;
  out->word=value.value;return 0;
 }
 static std::int32_t faery_assert(void*,dh2::character_faery_selection::Character*,const dh2::character_faery_selection::Request*){return 1;}
 static std::int32_t mana_service(void* raw,const dh2::character_mana_services_v1::Request* q,
                               dh2::character_mana_services_v1::Reply* out){
  auto& s=self(raw);if(!q||!out)return 1;
  using Operation=dh2::character_mana_services_v1::Operation;
  if(q->operation==Operation::get_online){
   if(!s.bindings.online||!s.bindings.online_identity)return 1;
   out->identity=s.bindings.online_identity;out->word=*s.bindings.online;return 0;
  }
  // Full COnline and ObjectBase::IsRemotelyUpdated are not attached to this
  // offline session. A reached online branch is an explicit missing provider.
  if(q->operation==Operation::is_remotely_updated)return 1;
  if(q->operation==Operation::application_is_saved_option_on){
   if(!s.bindings.application_singleton||q->subject!=*s.bindings.application_singleton||!s.bindings.saved_options)return 1;
   const dh2::data::savegame_options_v1::Application app{q->subject,s.bindings.saved_options};
   bool on=false;
   if(dh2::data::savegame_options_v1::is_saved_option_on(&app,q->text,&on)!=dh2::data::savegame_options_v1::Status::complete)return 1;
   out->word=on?1u:0u;return 0;
  }
  return 1;
 }
 static int native(void* raw,const dh2::player_skill_session_v1::NativeRequest& q,const dh2_script_value* a,std::uint32_t count,
          dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,char* text,std::size_t bytes){
  auto& s=self(raw);if(!returned||!s.bindings.character||q.character!=s.bindings.character||(!a&&count))return fail(text,bytes,q.name);
  *returned=0;
  if(q.domain==dh2::player_skill_session_v1::Domain::ais){
   using Fn=dh2::ais_native_bindings::Function;
   if(q.ais_function==Fn::play_sound){
    if(q.userdata!=s.ais.ais||count!=4||a[0].type!=DH2_SCRIPT_STRING||!a[0].text||
       a[0].text_bytes>256)return fail(text,bytes,q.name);
    bool looping=false,stop_music=false;float fade_ms=0.f;
    if(count>1){if(a[1].type!=DH2_SCRIPT_BOOLEAN)return fail(text,bytes,q.name);looping=a[1].boolean!=0;}
    if(count>2){if(a[2].type!=DH2_SCRIPT_NUMBER||!std::isfinite(a[2].number))return fail(text,bytes,q.name);fade_ms=a[2].number;}
    if(count>3){if(a[3].type!=DH2_SCRIPT_BOOLEAN)return fail(text,bytes,q.name);stop_music=a[3].boolean!=0;}
    // The current Android owner has no source-matched fade or gameplay-music
    // implementation. Do not reinterpret those arguments as simple WAV FX.
    if(fade_ms!=0.f||stop_music||!dh2::player_gameplay_audio::enqueue(a[0].text,a[0].text_bytes,looping))
     return fail(text,bytes,"unsupported Player PlaySound format/flags");
    return 0;
   }
   if(q.ais_function==Fn::get_int||q.ais_function==Fn::set_int){
    if(q.userdata!=s.ais.ais)return fail(text,bytes,q.name);
    return q.ais_function==Fn::get_int?
     dh2::character_player_scalar_services_v1::get_int(&s.scalar_callbacks,a,count,out,capacity,returned,text,bytes):
     dh2::character_player_scalar_services_v1::set_int(&s.scalar_callbacks,a,count,out,capacity,returned,text,bytes);
   }
   if(q.ais_function==Fn::trace)return dh2_script_game_trace(nullptr,a,count,out,capacity,returned,text,bytes);
   if(q.ais_function==Fn::get_py_struct||q.ais_function==Fn::get_py_oid||q.ais_function==Fn::get_py_cst){
    if(count<2||a[0].type!=DH2_SCRIPT_STRING||a[1].type!=DH2_SCRIPT_STRING||!a[0].text||!a[1].text||!out||!capacity)return fail(text,bytes,q.name);
    std::int32_t value=-1;
    if(q.ais_function==Fn::get_py_struct){
     if(std::strcmp(a[0].text,"CharacterProperties"))return fail(text,bytes,q.name);
     auto it=std::find(s.bindings.fields->begin(),s.bindings.fields->end(),a[1].text);
     if(it!=s.bindings.fields->end())value=std::int32_t(it-s.bindings.fields->begin());
    }else if(q.ais_function==Fn::get_py_oid){
     if(!std::strcmp(a[0].text,"ClassTable")){
      auto it=std::find(s.bindings.classes->names.begin(),s.bindings.classes->names.end(),a[1].text);
      if(it!=s.bindings.classes->names.end())value=std::int32_t(it-s.bindings.classes->names.begin());
     }else if(!std::strcmp(a[0].text,"SkillTable")){
      const auto& rows=s.bindings.tables->skills().skills;
      auto it=std::find_if(rows.begin(),rows.end(),[&](const auto& row){return row.table_name==a[1].text;});
      if(it!=rows.end())value=std::int32_t(it-rows.begin());
     }else if(!std::strcmp(a[0].text,"SkillListTable")){
      const auto& rows=s.bindings.tables->skills().skill_lists;
      auto it=std::find_if(rows.begin(),rows.end(),[&](const auto& row){return row.name==a[1].text;});
      if(it!=rows.end())value=std::int32_t(it-rows.begin());
     }else{
      const auto it=s.named_views.find(a[0].text);
      if(it==s.named_views.end()||a[1].text_bytes>UINT32_MAX||dh2_pynames_get(&it->second,a[1].text,std::uint32_t(a[1].text_bytes),&value))return fail(text,bytes,q.name);
     }
    }else{
     dh2_pycst_result result{};
     if(dh2_pycst_get(s.bindings.design,a[0].text,a[0].text_bytes,a[1].text,a[1].text_bytes,&result))return fail(text,bytes,q.name);
     if(!result.found&&dh2_pycst_get(s.bindings.ai_constants,a[0].text,a[0].text_bytes,a[1].text,a[1].text_bytes,&result))return fail(text,bytes,q.name);
     if(result.found)value=result.value;
    }
    number(out[0],static_cast<float>(value));*returned=1;return 0;
   }
  }else{
   using Fn=dh2::character_native_bindings::Function;
   if(q.character_function==Fn::character_spell_combat_roll){
    if(!s.bindings.spell_combat_roll_native||!s.bindings.skill_gameplay_context)
     return fail(text,bytes,q.name);
    return s.bindings.spell_combat_roll_native(s.bindings.skill_gameplay_context,
        static_cast<std::uint32_t>(q.character_function),a,count,out,capacity,
        returned,text,bytes,q.name);
   }
   const auto skill_gameplay_call=[&](){
    if(!s.bindings.skill_gameplay_native||!s.bindings.skill_gameplay_context)
     return fail(text,bytes,q.name);
    return s.bindings.skill_gameplay_native(s.bindings.skill_gameplay_context,
        static_cast<std::uint32_t>(q.character_function),a,count,out,capacity,
        returned,text,bytes,q.name);
   };
   if(q.character_function==Fn::character_clear_target){
    if(!s.bindings.source_ai||s.bindings.source_ai->identity!=s.bindings.ai)
     return fail(text,bytes,q.name);
    const auto status=dh2::character_clear_target_v1::clear(
      s.bindings.source_ai,s.bindings.target_owner,s.bindings.character,
      &s.target_services);
    return status==dh2::character_clear_target_v1::Status::complete
      ?0:fail(text,bytes,q.name);
   }
   switch(q.character_function){
   case Fn::game_object_set_target_list_character_filter:
   case Fn::game_object_set_target_list_object_filter:
   case Fn::game_object_set_target_list_sorting:
   case Fn::game_object_target_list_search:
   case Fn::game_object_target_list_search_rect:
   case Fn::game_object_target_list_resort:
   case Fn::game_object_target_list_backup:
   case Fn::game_object_is_target_list_empty:
   case Fn::game_object_get_target_list_size:
   case Fn::game_object_get_target_list_top:
   case Fn::game_object_pop_target_list:
   case Fn::character_look_at:
   // Charge's authored OnPostSkill reaches TargetInMeleeRange before its
   // source ClearTarget tail. The gameplay adapter owns current-target range.
   case Fn::character_target_in_melee_range:
   // Charge's authored OnSkillInfo/OnSkill branches call HasShield. Forward
   // through the existing gameplay adapter so it can query the canonical V4
   // inventory; do not infer shield state from Save/profile data here.
   case Fn::character_has_shield:
   case Fn::character_spawn_skill_projectile:
   case Fn::character_set_projectile_target:
   case Fn::character_skill_combat_roll:
    return skill_gameplay_call();
   default:break;
   }
   if(q.character_function==Fn::character_set_skill_cooldown_timer_id)
    return dh2::character_skill_cooldown_services::skill(&s.cooldown,a,count,out,capacity,returned,text,bytes);
   if(q.character_function==Fn::character_set_spell_cooldown_timer_id)
    return dh2::character_skill_cooldown_services::spell(&s.cooldown,a,count,out,capacity,returned,text,bytes);
   if(q.character_function==Fn::character_start_timer)
    return dh2::player_skill_timer_provider_v1::start(&s.timer_bindings,a,count,out,capacity,returned,text,bytes);
   if(q.character_function==Fn::character_stop_timer)
    return dh2::player_skill_timer_provider_v1::stop(&s.timer_bindings,a,count,out,capacity,returned,text,bytes);
   if(q.character_function==Fn::character_create_buff)
    return dh2::character_player_buffs_v1::create_buff(&s.buff_callbacks,a,count,out,capacity,returned,text,bytes);
   if(q.character_function==Fn::character_remove_buff)
    return dh2::character_player_buffs_v1::remove_buff(&s.buff_callbacks,a,count,out,capacity,returned,text,bytes);
   if(q.character_function==Fn::character_apply_prop_class&&count>1&&a[1].type==DH2_SCRIPT_IDENTITY)
    return dh2::character_player_buffs_v1::apply_buff(&s.buff_callbacks,a,count,out,capacity,returned,text,bytes);
   if(q.character_function==Fn::character_get_prop||q.character_function==Fn::character_set_prop||q.character_function==Fn::character_apply_prop_class||q.character_function==Fn::character_clear_props)
    return dh2::player_skill_property_services_v1::invoke(&s.property_services,q.character,q.character_function,a,count,out,capacity,returned,text,bytes);
   if(q.character_function==Fn::character_get_current_skill_info||q.character_function==Fn::character_get_skill_id_from_oid){
    std::int32_t index=0;
    if(!count)return 0;
    if(a[0].type!=DH2_SCRIPT_NUMBER)return fail(text,bytes,q.name);
    if(!integer(a[0],index)||!out||!capacity||s.bindings.savegame->character()!=q.character)return fail(text,bytes,q.name);
    s.refresh_properties();
    using namespace dh2::player_saved_skill_callbacks_v1;
    const auto value=q.character_function==Fn::character_get_current_skill_info?
     get_current_skill_info(&s.property_view,&s.bindings.tables->skills(),s.bindings.savegame.get(),q.character,index):
     get_skill_id_from_oid(&s.property_view,&s.bindings.tables->skills(),index);
    if(value.disposition==Disposition::no_return)return 0;
    if(value.disposition!=Disposition::append_integer)return fail(text,bytes,q.name);
    number(out[0],static_cast<float>(value.integer));*returned=1;return 0;
   }
   if(q.character_function==Fn::character_get_current_spell_info){
    s.saved_slot=s.bindings.savegame.get();
    return dh2::character_current_spell_v1::current_spell_info_v1(&s.spell_callback,a,count,out,capacity,returned,text,bytes);
   }
   if(q.character_function==Fn::character_get_equipped_faery_element){
    s.saved_slot=s.bindings.savegame.get();
    return dh2::character_equipped_faery_element_v1::equipped_faery_element_v1(&s.element_callback,a,count,out,capacity,returned,text,bytes);
   }
   if(q.character_function==Fn::character_get_current_equipped_faery_id||q.character_function==Fn::character_get_current_equipped_faery_level){
    s.saved_slot=s.bindings.savegame.get();
    return q.character_function==Fn::character_get_current_equipped_faery_id?
     dh2::character_current_equipped_faery_v1::current_equipped_faery_id_v1(&s.equipped_callback,a,count,out,capacity,returned,text,bytes):
     dh2::character_current_equipped_faery_v1::current_equipped_faery_level_v1(&s.equipped_callback,a,count,out,capacity,returned,text,bytes);
   }
   if(q.character_function==Fn::character_has_mana||q.character_function==Fn::character_use_mana){
    s.refresh_properties();
    return q.character_function==Fn::character_has_mana?
     dh2::character_mana_services_v1::has_mana_callback(&s.mana_callbacks,a,count,out,capacity,returned,text,bytes):
     dh2::character_mana_services_v1::use_mana_callback(&s.mana_callbacks,a,count,out,capacity,returned,text,bytes);
   }
  }
  return fail(text,bytes,q.name);
 }
 static int lifecycle_backend(void* raw,const dh2::player_ais_lifecycle_v1::Request* q,std::string& error){
  auto& s=self(raw);
  if(!q||q->character!=s.bindings.character||q->ais!=s.ais.ais){error="Native Player lifecycle identity differs";return 1;}
  if(q->operation==dh2::player_ais_lifecycle_v1::Operation::construct_vm){
   if(q->allocation_bytes!=0xd8||q->skip_bind!=1||s.session){error="Native Player deferred constructor differs";return 1;}
   dh2::player_skill_session_v1::Configuration config{};
   config.character=s.bindings.character;config.ais=&s.ais;config.tables=s.bindings.tables;
   config.debug=&s.bindings.debug->globals();config.debug_services=s.bindings.debug->services();
   config.providers.context=&s;config.providers.resolve=resolve;config.providers.native=native;
   config.providers.faery={&s,faery_constant,faery_assert};
   dh2::player_skill_session_v1::Vm vm(dh2_script_vm_create_deferred(16*1024*1024));
   s.session=dh2::player_skill_session_v1::Session::adopt(std::move(vm),std::move(config),error);
   s.session_slot=s.session.get();
   return s.session&&s.session->stage()==dh2::player_skill_session_v1::Stage::created?0:1;
  }
  if(q->operation!=dh2::player_ais_lifecycle_v1::Operation::configure_skills||!s.session||s.preparation||
     s.bindings.source_ai->active_ais_1c!=s.ais.ais||s.bindings.source_ai->pointer_28!=7){
   error="Native Player source configure prefix differs";return 1;
  }
  s.refresh_properties();
  const dh2::character_player_skills_preparation_v3::Inputs inputs{q->character,s.bindings.source_ai->active_ais_1c,&s.property_view,0};
  s.preparation=dh2::character_player_skills_preparation_v3::Owner::create(s.bindings.tables,inputs,s.session->preparation_services(),error);
  s.preparation_slot=s.preparation.get();
  if(!s.preparation||s.preparation->prepare(&s.prepared)!=dh2::character_ai_set_skills_and_spells::Status::complete){
   error="Player original skill preparation: "+s.session->last_error();return 1;
  }
  s.timer_fields=s.preparation->lease_timer_fields(q->character);
  if(!s.timer_fields){error="Player skill field18 lease unavailable";return 1;}
  s.updates=std::make_unique<dh2::player_skill_update_session_v1::Runtime>(*s.session,*s.preparation,s.bindings.ai,q->character,s.bindings.coordinator->state.current);
  s.uses=std::make_unique<dh2::player_skill_use_session_v1::Runtime>(*s.session,*s.preparation,q->character);
  s.update_slot=s.updates.get();s.use_slot=s.uses.get();return 0;
 }
 void initialize(){
  if(!bindings.character||!bindings.tables||!bindings.debug)throw std::runtime_error("Native Player skill owners missing");
  animation_constants_bytes=bindings.read(assets,"data/animations_pycst.bin");
  if(animation_constants_bytes.size()>UINT32_MAX||dh2_pycst_open(&animation_constants,animation_constants_bytes.data(),std::uint32_t(animation_constants_bytes.size())))
   throw std::runtime_error("Native Player animation constants rejected");
  for(const auto& pair:std::vector<std::pair<std::string,std::string>>{{"AnimatedEffectTable","effects"},{"ProjectileTable","projectiles"}}){
   auto bytes=bindings.read(assets,"data/"+pair.second+"_pyarraynames.bin");
   const auto word=[&](std::size_t at){if(at>bytes.size()||bytes.size()-at<4)throw std::runtime_error("Player names truncated");std::uint32_t n;std::memcpy(&n,bytes.data()+at,4);return n;};
   std::size_t end=4;const auto count=word(0);
   if(count>65536)throw std::runtime_error("Player names count exceeds bound");
   for(std::uint32_t i=0;i<count;++i){const auto n=word(end);end+=4;if(n>bytes.size()-end)throw std::runtime_error("Player names length rejected");end+=n;}
   bytes.resize(end);auto& retained=named_bytes[pair.first];retained=std::move(bytes);
   if(dh2_pynames_open(&named_views[pair.first],retained.data(),std::uint32_t(retained.size())))throw std::runtime_error("Player dictionary names rejected");
  }
  ais={reinterpret_cast<std::uintptr_t>(&source_fields),0};
  source_fields.script.identity=ais.ais;
  source_fields.script.binder_identity=reinterpret_cast<std::uintptr_t>(&session_slot);
  source_fields.script.path_storage_identity=reinterpret_cast<std::uintptr_t>(&session);
  source_tables={reinterpret_cast<std::uintptr_t>(&base_initials),reinterpret_cast<std::uintptr_t>(&player_initials),reinterpret_cast<std::uintptr_t>(&iphone_initials)};
  scalar_state={ais.ais,&script_integers};scalar_callbacks={&scalar_state,nullptr};
  property_services={bindings.character,bindings.rules,bindings.classes,bindings.properties,bindings.shared_property_temp,false,&property_view};
  mana_state={bindings.character,bindings.mana_exempt_14f0,&property_view};
  mana_globals={bindings.application_singleton,&bindings.debug->globals(),&bindings.debug->services()};
  mana_services={this,mana_service};mana_callbacks={&mana_state,&mana_globals,&mana_services};
  saved_slot=bindings.savegame.get();
  faery_globals={&bindings.tables->source_faeries(),0};
  faery_services={this,faery_constant,faery_assert};
  spell_saved={bindings.character,&saved_slot,bindings.current_difficulty,&bindings.properties->resolved[29],&faery_globals,&faery_services};
  spell_callback={bindings.character,dh2::character_current_spell_v1::saved_services(&spell_saved)};
  element_callback={bindings.character,dh2::character_equipped_faery_element_v1::saved_services(&spell_saved)};
  equipped_callback={bindings.character,dh2::character_current_spell_v1::saved_services(&spell_saved)};
  cooldown={this,bindings.character,list_count,slot,unsupported_number};
  if(!dh2::player_skill_timer_provider_v1::bind(
       &timer_bindings,bindings.coordinator,bindings.character))
   throw std::runtime_error("Native Player timer provider rejected Character Coordinator identity");
  refresh_properties();
  refresh_class_rows();
  buffs=dh2::character_player_buffs_v1::Owner::create({bindings.character,&property_view,{this,buff_service},std::uint32_t(class_rows.size()),bindings.fx_count});
  if(!buffs)throw std::runtime_error("Native Player buff owner rejected current property graph");
  buff_callbacks={buffs.get()};
  dh2::player_ai_timer_events_v1::Bindings tick_bindings{};
  tick_bindings.ai=bindings.source_ai;tick_bindings.coordinator=bindings.coordinator;tick_bindings.object=bindings.object;
  tick_bindings.controller=bindings.controller;tick_bindings.properties=reinterpret_cast<std::uintptr_t>(bindings.properties);
  tick_bindings.view=&property_view;tick_bindings.dead=bindings.dead;
  tick_bindings.debug_globals=&bindings.debug->globals();tick_bindings.debug_services=&bindings.debug->services();
  // Positive DoT combat remains an explicit reached-provider failure until
  // genuine Player F_DotAttack/F_ApplyResult storage is attached. Zero DoT is
  // the real cached-property path, never a forced or fabricated suppression.
  ai_ticks=std::make_unique<dh2::player_ai_timer_events_v1::Runtime>(tick_bindings);
  dh2::player_ais_lifecycle_v1::Bindings life{bindings.source_ai,&source_fields,&ais,&source_tables,&declaration,"Player",
   &session_slot,&preparation_slot,&update_slot,&use_slot,bindings.savegame.get(),bindings.coordinator,
   bindings.dead,bindings.design,&vitals,&vitals_storage,{this,lifecycle_backend}};
  lifecycle=std::make_unique<dh2::player_ais_lifecycle_v1::Runtime>(life);
  // Character profile/equipment/slot-grant integration belongs between these
  // original phases. The Crypt development entry has no such provider yet.
  if(lifecycle->load(&loaded,error)!=dh2::player_ais_lifecycle_v1::Status::complete||bindings.source_ai->pointer_28!=7)
   throw std::runtime_error("Native Player source load: "+error);
  if(lifecycle->initialize_process(1,&initialized_result,error)!=dh2::player_ais_lifecycle_v1::Status::complete)
   throw std::runtime_error("Native Player source InitProcess: "+error);
  if(initialized_result.init_phase_mask!=31||bindings.source_ai->active_ais_1c!=ais.ais||bindings.source_ai->alternate_ais_20!=ais.ais)
   throw std::runtime_error("Native Player completed InitProcess owners differ");
  timer_dispatch={bindings.character,bindings.ai,ais.ais,bindings.controller,
                  reinterpret_cast<std::uintptr_t>(bindings.properties),
                  bindings.source_ai,bindings.coordinator,session->vm()};
  cleanups=std::make_unique<dh2::player_skill_cleanup_session_v1::Runtime>(&session_slot,*preparation,bindings.character);
  target_services={this,std::int32_t(bindings.ai_tables->rows.size()),target_service};
  dh2::player_ai_death_v1::Bindings death_bindings{};
  death_bindings.ai=bindings.source_ai;death_bindings.coordinator=bindings.coordinator;
  death_bindings.target_owner=bindings.target_owner;death_bindings.target_services=&target_services;
  death_bindings.dead_state={bindings.coordinator,{bindings.character,&property_view},
      bindings.animation_tables,&animation_constants,bindings.death_anim_stance};
  death_bindings.group_identity=bindings.group_identity;
  death_bindings.player_ais_identity=ais.ais;death_bindings.backend={this,death_service};
  death=std::make_unique<dh2::player_ai_death_v1::Runtime>(death_bindings);
  dh2::character_dead_focus_services_v1::Bindings focus{};
  focus.character=bindings.character;focus.properties=&property_view;focus.buffs=buffs.get();
  focus.byte415=&bindings.source_ai->targetable_4d;
  focus.self_fx=bindings.self_fx;focus.state_fx=bindings.state_fx;focus.highlight=bindings.highlight_fx;
  focus.debug_globals=&bindings.debug->globals();focus.debug_services=&bindings.debug->services();
  focus.skills=&bindings.tables->skills();focus.preparation=preparation.get();focus.skill_calls=uses.get();
  focus.services={bindings.character_queries_context,bindings.is_player,nullptr};
  focus_services=std::make_unique<dh2::character_dead_focus_services_v1::Adapter>(focus);
  initialized=true;updated=initialized_result.update;++update_attempts;
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native Player skill preparation | skills %zu | faeries %zu | loaded paths %zu | declarations %u | flags %x | one VM; Player lifecycle and skill-use providers pending",
   preparation->slots(dh2::character_ai_set_skills_and_spells::List::skill).size(),preparation->slots(dh2::character_ai_set_skills_and_spells::List::faery).size(),session->loaded_path_count(),session->statistics().declarations,ais.flags_b8);
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native Player AIS initialized | AI %p | AIS %p | active %p | pending %p | phase %zu | VM %p | init mask %x | HP %d / %d | MP %d / %d | timer33 %u | timer34 %u | one source InitProcess; Character profile/grants/frame/activation pending",
   reinterpret_cast<void*>(bindings.ai),reinterpret_cast<void*>(ais.ais),reinterpret_cast<void*>(bindings.source_ai->active_ais_1c),
   reinterpret_cast<void*>(bindings.source_ai->alternate_ais_20),std::size_t(bindings.source_ai->pointer_28),static_cast<void*>(session->vm()),initialized_result.init_phase_mask,
   bindings.properties->resolved[36],bindings.properties->resolved[38],bindings.properties->resolved[41],bindings.properties->resolved[43],bindings.source_ai->word_10,bindings.source_ai->word_14);
  log_ai_timers("initial");
  log_initial_update();
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native Player saved skills ready | owner %p | rows %zu | slot0 level %d | source _InitSkills; starter grant/profile load pending",
   static_cast<void*>(bindings.savegame.get()),bindings.savegame->skills().size(),bindings.savegame->skill_level(0));
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native Player saved faeries ready | difficulty %d | selected %d | level %d | rows 5 5 5 | source constructor and _InitFaeries; same save owner",
   *bindings.current_difficulty,bindings.savegame->current_faery(*bindings.current_difficulty),bindings.savegame->faery_level(0,*bindings.current_difficulty));
 }
 void log_initial_update(){
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native Player skill update complete | attempt %u | callbacks %u | skill slots %u | faery slots %u | buffs %u | groups %u | one VM/save/property store; source InitProcess complete, activation pending",update_attempts,updated.callbacks,updated.source.skill_slots,updated.source.faery_slots,buffs->count(),property_view.group_count);
  log_buffs("initial");
 }
 bool update_after_saved_skill_slot_write(std::string& output_error){
  if(!initialized||update_blocked||!updates){output_error="Native Player UpdateSkills owner is unavailable or blocked";return false;}
  try{refresh_properties();}
  catch(const std::exception& e){output_error=e.what();return false;}
  ++update_attempts;
  // SG_SetSkillInSlot ends at CharAI::UpdateSkills: saved slot keys plus the
  // current difficulty's one Faery, through this Save/Character/VM owner.
  if(updates->update_current_faery(*bindings.savegame,*bindings.current_difficulty,selected_updated,error)){
   update_blocked=true;
   __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native Player UpdateSkills blocked | attempt %u | callbacks %u | VM status %d | faery %u | %s | source effects retained; remaining providers pending",update_attempts,selected_updated.callbacks,selected_updated.last_lua_status,selected_updated.source.faery_index,error.c_str());
   output_error=error;return false;
  }
  output_error.clear();return true;
 }
 bool update_after_source_skill_inputs_changed(std::string& output_error){
  if(!initialized||update_blocked||!updates){output_error="Native Player UpdateAllSkills owner is unavailable or blocked";return false;}
  try{refresh_properties();}
  catch(const std::exception& e){output_error=e.what();return false;}
  ++update_attempts;
  // ChangeFaery's source tail is UpdateAllSkills and stays distinct above.
  if(updates->update(updated,error)){
   update_blocked=true;
   __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native Player UpdateAllSkills blocked | attempt %u | callbacks %u | VM status %d | %s | source effects retained; remaining providers pending",update_attempts,updated.callbacks,updated.last_lua_status,error.c_str());
   output_error=error;return false;
  }
  output_error.clear();return true;
 }
 struct TrainContext {Impl* owner;dh2::data::FreshInventoryOwnedV4* inventory;};
 static int train_current_save(void* raw,std::uintptr_t character,dh2::data::PlayerSavegameV1** output){
  auto& context=*static_cast<TrainContext*>(raw);auto& owner=*context.owner;
  if(!output||character!=owner.bindings.character||!owner.bindings.savegame||
     owner.bindings.savegame->character()!=character)return 1;
  *output=owner.bindings.savegame.get();return 0;
 }
 static int train_service(void* raw,const dh2::player_skill_progression_v1::Request* request,
                          dh2::player_skill_progression_v1::Response* response){
  if(!raw||!request||!response)return 1;
  auto& context=*static_cast<TrainContext*>(raw);auto& owner=*context.owner;*response={};
  if(request->character!=owner.bindings.character||!owner.bindings.savegame||
     owner.bindings.savegame->character()!=request->character)return 1;
  using Operation=dh2::player_skill_progression_v1::Operation;
  switch(request->operation){
   case Operation::skill_limit:{
    static constexpr const char* keys[]={"MaxSkillLevelBNormal","MaxSkillLevelCHard","MaxSkillLevelDVeryHard"};
    const auto difficulty=request->arguments[0];if(difficulty<0||difficulty>2||!owner.bindings.design)return 1;
    dh2_pycst_result value{};const auto* key=keys[difficulty];
    if(dh2_pycst_get(owner.bindings.design,"CharacterDesign",15,key,
                     std::uint32_t(std::strlen(key)),&value))return 1;
    // PyDataConstants::getConstant returns zero for a missing entry.
    response->word=value.found?std::uint32_t(value.value):0u;return 0;
   }
   case Operation::unlocked_difficulty:
    if(request->savegame&&request->savegame!=owner.bindings.savegame.get())return 1;
    response->word=std::uint32_t(owner.bindings.savegame->unlocked_difficulty());return 0;
   case Operation::add_property:{
    if(request->arguments[0]!=157||request->arguments[1]!=-1||dh2_property_validate(&owner.property_view))return 1;
    const std::uint32_t bits=std::uint32_t(request->arguments[1])<<8;std::int32_t fixed{};
    std::memcpy(&fixed,&bits,sizeof(fixed));
    return dh2_property_add(&owner.property_view,request->arguments[0],fixed)?1:0;
   }
   case Operation::update_all_skills:{
    std::string error;return owner.update_after_source_skill_inputs_changed(error)?0:1;
   }
   case Operation::recalculate_properties:
    if(request->arguments[0]!=1||!owner.bindings.classes||owner.class_rows.empty()||
       dh2_property_validate(&owner.property_view))return 1;
    return int(dh2_class_recalc_base(owner.class_rows.data(),std::uint32_t(owner.class_rows.size()),
                                    owner.bindings.properties->base.data(),&owner.property_view));
   case Operation::set_potion_capacity:
    if(!context.inventory||context.inventory->character()!=request->character||
       context.inventory->properties()!=owner.bindings.properties)return 1;
    context.inventory->project_potion_capacity(std::int8_t(std::uint8_t(request->arguments[0])));return 0;
   case Operation::debug_load:
    if(!owner.bindings.debug||owner.bindings.debug->runtime().load(owner.bindings.debug->globals(),
        owner.bindings.debug->services())!=dh2::debug_switches::Status::complete)return 1;
    return 0;
   case Operation::debug_query:{
    if(!owner.bindings.debug)return 1;std::uint8_t ignored=0;
    return owner.bindings.debug->runtime().get_switch("isTracingChar_Stats",owner.bindings.debug->globals(),
        owner.bindings.debug->services(),ignored)==dh2::debug_switches::Status::complete?0:1;
   }
   default:return 1;
  }
 }
 bool train_skill(std::uint32_t skill_index,bool test_only,dh2::data::FreshInventoryOwnedV4& inventory,
                  std::uint32_t& source_return,std::string& output_error){
  source_return=0;output_error.clear();
  if(!initialized||update_blocked||!bindings.savegame||!bindings.tables||!bindings.design||!bindings.debug||
     !bindings.properties||!bindings.classes||inventory.character()!=bindings.character||
     inventory.properties()!=bindings.properties){output_error="Native skill training requires the active Save, Player update, design/debug and canonical V4 owners";return false;}
  try{refresh_properties();refresh_class_rows();}
  catch(const std::exception& e){output_error=e.what();return false;}
  TrainContext context{this,&inventory};
  const dh2::player_skill_progression_v1::Services services{&context,train_current_save,train_service};
  dh2::player_skill_progression_v1::Result result{};
  const auto status=dh2::player_skill_progression_v1::increment_skill(&property_view,
      &bindings.tables->skills(),bindings.character,skill_index,test_only,&services,&result);
  if(status!=dh2::player_skill_progression_v1::Status::complete){
   output_error="Source Character::IncSkill provider failed at operation "+std::to_string(result.last_operation);
   return false;
  }
  source_return=result.source_return;return true;
 }
 static bool initial_grant_update(void* raw,std::uintptr_t character,std::string& output_error){
  if(!raw)return false;auto& context=*static_cast<TrainContext*>(raw);
  if(!context.owner||character!=context.owner->bindings.character){
   output_error="Initial skill grant slot update Character mismatch";return false;
  }
  return context.owner->update_after_saved_skill_slot_write(output_error);
 }
 static int initial_grant_recalculate(void* raw,dh2::data::PropertyView* view,
                                      bool full,std::string& output_error){
  if(!raw)return 1;auto& owner=*static_cast<Impl*>(raw);
  if(view!=&owner.property_view||!full){
   output_error="Initial skill grant requires the same full-recalculation PropertyView";return 1;
  }
  if(!owner.initialized||!owner.bindings.classes||!owner.bindings.properties||
     owner.class_rows.empty()){
   output_error="Initial skill grant full recalculation owner is unavailable";return 1;
  }
  try{owner.refresh_properties();owner.refresh_class_rows();}
  catch(const std::exception& failure){output_error=failure.what();return 1;}
  if(dh2_property_validate(&owner.property_view)||
     dh2_class_recalc_base(owner.class_rows.data(),
        std::uint32_t(owner.class_rows.size()),owner.bindings.properties->base.data(),
        &owner.property_view)!=0){
   output_error="Same-owner initial skill full property recalculation failed";return 1;
  }
  output_error.clear();return 0;
 }
 bool initialize_initial_skill_grants(dh2::data::FreshInventoryOwnedV4& inventory,
                                      std::string& output_error){
  output_error.clear();
  if(!initialized||update_blocked||!bindings.savegame||!bindings.tables||
     !bindings.rules||!bindings.properties||!bindings.classes||class_rows.empty()||
     inventory.character()!=bindings.character||inventory.properties()!=bindings.properties){
   output_error="Initial skill grants require the active Save, Player runtime, class/property graph and canonical V4";
   return false;
  }
  try{refresh_properties();refresh_class_rows();}
  catch(const std::exception& failure){output_error=failure.what();return false;}
  auto* save=bindings.savegame.get();
  TrainContext context{this,&inventory};
  dh2::player_initial_skill_grants_v1::Bindings grants{};
  grants.character=bindings.character;grants.current_savegame=&save;
  grants.rules=bindings.rules;grants.properties=bindings.properties;
  grants.view=&property_view;grants.tables=&bindings.tables->skills();
  grants.inventory=&inventory;
  grants.slot_updates={&context,initial_grant_update};
  grants.full_recalculation={this,initial_grant_recalculate};
  grants.effects={&context,train_current_save,train_service};
  dh2::player_initial_skill_grants_v1::Result result{};
  const auto status=dh2::player_initial_skill_grants_v1::initialize(&grants,&result,output_error);
  if(status!=dh2::player_initial_skill_grants_v1::Status::complete){
   if(output_error.empty())output_error="Source initial skill-slot initialization failed with status "+
       std::to_string(static_cast<unsigned>(status));
   return false;
  }
  return true;
 }
 int skill_info(std::uint32_t skill_index,std::int32_t level,
                std::vector<std::int32_t>& output,std::string& output_error){
  output.clear();output_error.clear();
  if(!initialized||update_blocked||!session||!preparation||!bindings.shared_property_temp||
     session->character_identity()!=bindings.character||
     session->stage()!=dh2::player_skill_session_v1::Stage::character_bound){
   output_error="Native Player skill-info owners are unavailable or blocked";return -1;
  }
  using List=dh2::character_ai_set_skills_and_spells::List;
  const auto& slots=preparation->slots(List::skill);
  if(skill_index>=slots.size()){
   output_error="Source AI_SkillInfo skill-list index is outside the prepared vector";return -1;
  }
  if(!slots[skill_index])return 1; // Source null-instance early exit; no sheet write.
  const auto* instance=preparation->instance(slots[skill_index]);
  if(!instance||!instance->script_name||!*instance->script_name){
   output_error="Source AI_SkillInfo prepared instance is unavailable";return -1;
  }
  // Adam's GetInfo kernel confirms the source sequence: select this exact
  // retained script/instance, then call OnSkillInfo(level). Both calls use the
  // sole Player VM, and Character GetProp/SetProp already target shared_temp.
  dh2_script_value arguments[2]{};
  arguments[0].type=DH2_SCRIPT_STRING;
  arguments[0].text=instance->script_name;
  arguments[0].text_bytes=std::strlen(instance->script_name);
  number(arguments[1],static_cast<float>(instance->skill_index_14));
  const auto discard=[](void*,const dh2_script_first_return_v1*,char*,std::size_t){return 0;};
  int status=session->call("SetSkill",arguments,2,0,discard,nullptr,output_error);
  if(status<0){if(output_error.empty())output_error="Source skill SetSkill provider failed";return -1;}
  if(status>0)return 1; // Original Lua error is an ordinary source result.
  dh2_script_value level_argument{};
  number(level_argument,static_cast<float>(level));
  status=session->call("OnSkillInfo",&level_argument,1,0,discard,nullptr,output_error);
  if(status<0){if(output_error.empty())output_error="Source skill OnSkillInfo provider failed";return -1;}
  if(status>0)return 1;
  output.assign(bindings.shared_property_temp->begin(),bindings.shared_property_temp->end());
  return 0;
 }
 bool hud_info(bool faery,std::uint32_t index,bool refresh_usable,
               std::uint32_t& usable,float& cooldown_fraction,
               std::string& output_error){
  cooldown_fraction=0.f;output_error.clear();
  if(!initialized||update_blocked||!preparation||!uses||!timer_fields||
     !bindings.savegame||bindings.savegame->character()!=bindings.character||
     !bindings.coordinator||!session||session->character_identity()!=bindings.character){
   output_error="HUD query requires the initialized same Character, Save, Player VM and timer-field lease";return false;
  }
  using List=dh2::character_ai_set_skills_and_spells::List;
  using UseList=dh2::player_skill_use_session_v1::List;
  using Check=dh2::player_skill_use_session_v1::Check;
  const auto list=faery?List::faery:List::skill;
  const auto use_list=faery?UseList::faery:UseList::skill;
  const auto& prepared=preparation->slots(list);
  if(index>=prepared.size()){
   output_error="HUD query index is outside the retained prepared skill/faery vector";return false;
  }
  if(!prepared[index]){usable=0;return true;} // Original null script has no usable/cooldown state.
  const auto* instance=preparation->instance(prepared[index]);
  if(!instance||instance->character!=bindings.character||instance->identity!=prepared[index]){
   output_error="HUD query prepared script no longer belongs to the active Character";return false;
  }
  if(refresh_usable){
   dh2::player_skill_use_session_v1::Result check{};
   if(uses->check(use_list,index,Check::usable,check,output_error)!=0){
    if(output_error.empty())output_error="Same-VM HUD OnSkillCheck_Usable query failed";
    return false;
   }
   usable=check.value?1u:0u;
  }
  dh2::character_player_skills_preparation_v3::Owner::TimerFieldSlot field{};
  if(!timer_fields->slot(bindings.character,list,index,field)||field.instance!=prepared[index]||!field.field18){
   output_error="HUD query could not borrow the same prepared instance timer field18";return false;
  }
  const auto timer_id=*field.field18;
  if(timer_id<0){
   if(timer_id!=-1){output_error="HUD query source cooldown field18 is outside the -1 sentinel domain";return false;}
   return true;
  }
  std::uint32_t elapsed=0,duration=0;
  const auto present=dh2_character_timer_time_left(&elapsed,&duration,&bindings.coordinator->timers(),
                                                     static_cast<std::uint32_t>(timer_id));
  if(present<0){output_error="HUD query failed to read the retained Character timer store";return false;}
  if(present==1){
   if(!duration){output_error="Active HUD cooldown timer has zero source duration";return false;}
   cooldown_fraction=1.f-static_cast<float>(elapsed)/static_cast<float>(duration);
  }
  return true;
 }
 void update(){
  if(!initialized||update_blocked||!updates)return;
  std::string ignored;update_after_saved_skill_slot_write(ignored);
 }
 void timer(std::uint32_t id){
  const auto status=dh2::player_script_timer_dispatch_v1::dispatch(
      timer_dispatch,id,error);
  if(status==dh2::player_script_timer_dispatch_v1::Status::inactive_ais)return;
  if(status!=dh2::player_script_timer_dispatch_v1::Status::delivered)
   throw std::runtime_error("Player source OnScriptTimer dispatch failed: "+error);
  ++timer_callbacks;
  dh2::character_player_skills_preparation_v3::Owner::TimerFieldSlot found{};
  if(timer_fields->slot(bindings.character,dh2::character_ai_set_skills_and_spells::List::skill,0,found)&&found.field18)
   __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native Player skill timer callback | timer %u | callbacks %u | slot0 field18 %d | same Prince timer store and original Lua callbacks",id,timer_callbacks,*found.field18);
 }
};

std::string Runtime::cooldown_probe(std::uint32_t delay){
 if(!impl_->initialized)return "Player skill initialization incomplete";
 if(!delay||delay>60000)return "Player skill cooldown probe rejected";
 auto& s=*impl_;
 const auto& slots=s.preparation->slots(dh2::character_ai_set_skills_and_spells::List::skill);
 if(slots.empty()||!slots[0])return "Player skill slot0 unavailable";
 const auto* instance=s.preparation->instance(slots[0]);
 dh2::character_player_skills_preparation_v3::Owner::TimerFieldSlot field{};
 if(!instance||!s.timer_fields->slot(s.bindings.character,dh2::character_ai_set_skills_and_spells::List::skill,0,field)||!field.field18||*field.field18!=-1)return "Player skill cooldown already active";
 dh2_script_value arguments[2]{};arguments[0].type=DH2_SCRIPT_STRING;arguments[0].text=instance->script_name;arguments[0].text_bytes=std::strlen(instance->script_name);
 arguments[1].type=DH2_SCRIPT_NUMBER;arguments[1].number=static_cast<float>(instance->skill_index_14);
 const auto discard=[](void*,const dh2_script_first_return_v1*,char*,std::size_t){return 0;};
 if(s.session->call("SetSkill",arguments,2,0,discard,nullptr,s.error))return "Player skill selection failed: "+s.error;
 arguments[0]={};arguments[0].type=DH2_SCRIPT_NUMBER;arguments[0].number=static_cast<float>(delay);
 if(s.session->call("SetSkillCooldown",arguments,1,0,discard,nullptr,s.error))return "Player skill cooldown failed: "+s.error;
 const auto id=*field.field18;const auto& timers=s.bindings.coordinator->timers();
 if(id<0||std::uint32_t(id)>=timers.count||!timers.slots[id].active||timers.slots[id].event!=0x35)return "Player skill cooldown timer publication failed";
 dh2_script_first_return_v1 lua_cooldown{};
 const auto observe=[](void* raw,const dh2_script_first_return_v1* value,char*,std::size_t){*static_cast<dh2_script_first_return_v1*>(raw)=*value;return 0;};
 if(s.session->call("HasSkillCooldown",nullptr,0,0,observe,&lua_cooldown,s.error)||lua_cooldown.type!=DH2_SCRIPT_NUMBER||lua_cooldown.number!=static_cast<float>(id))return "Player skill Lua/native cooldown ID differs";
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native Player skill cooldown probe | script %s | slot0 field18 %d | timer %u | duration %u | same Prince timer store; shell fixture, not skill use",instance->script_name,id,timers.slots[id].id,timers.slots[id].duration_ms);
 return "Original skill cooldown callback armed";
}

std::string Runtime::check_probe(std::uint32_t slot){
 if(!impl_->initialized)return "Player skill initialization incomplete";
 auto& s=*impl_;
 using List=dh2::player_skill_use_session_v1::List;
 using Check=dh2::player_skill_use_session_v1::Check;
 const auto& slots=s.preparation->slots(List::skill);
 if(slot>=slots.size()||!slots[slot])return "Player skill check slot unavailable";
 const auto* instance=s.preparation->instance(slots[slot]);
 if(!instance)return "Player skill check instance unavailable";
 dh2::player_skill_use_session_v1::Result usable{},active{};
 // These are two distinct original callers. Each invokes OnSkillCheck once
 // and converts its own return index from that call's complete result vector.
 if(s.uses->check(List::skill,slot,Check::usable,usable,s.error)||
    s.uses->check(List::skill,slot,Check::active,active,s.error)){
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native Player skill check blocked | slot %u | %s",slot,s.error.c_str());
  return "Original skill check blocked: "+s.error;
 }
 const auto mana=s.bindings.properties->resolved[41];
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native Player skill check probe | script %s | slot %u | saved level %d | usable %u | active %u | usable returns %u | active returns %u | MP %d | SnS_Level %d | temp ManaCost %d | same Prince VM/properties/save; inner caller shell fixture, full activation pending",
  instance->script_name,slot,s.bindings.savegame->skill_level(slot),usable.value,active.value,usable.return_count,active.return_count,mana,
  s.bindings.properties->resolved[172],(*s.bindings.shared_property_temp)[173]);
 return "Original skill check completed";
}

std::string Runtime::mana_probe(std::uint32_t amount){
 if(!impl_->initialized)return "Player skill initialization incomplete";
 auto& s=*impl_;
 // This debug command traverses the same retained Player VM and original
 // native callback wrappers. It spends real MP; it is not a skill activation.
 if(amount>0x7fffffffu)return "Mana probe amount outside source domain";
 dh2_script_value argument{};Impl::number(argument,static_cast<float>(amount));
 dh2_script_first_return_v1 has{},used{};
 const auto capture=[](void* p,const dh2_script_first_return_v1* value,char*,std::size_t){*static_cast<dh2_script_first_return_v1*>(p)=*value;return 0;};
 const auto before=s.bindings.properties->resolved[41];
 if(s.session->call("HasMana",&argument,1,0,capture,&has,s.error)||
    s.session->call("UseMana",&argument,1,0,capture,&used,s.error)||
    has.type!=DH2_SCRIPT_BOOLEAN||used.type!=DH2_SCRIPT_BOOLEAN)return "Original mana callback blocked: "+s.error;
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native Player mana probe | amount %u | has %u | used %u | MP before %d | MP after %d | exempt14f0 %u | options %zu | same Prince VM/properties; debug debit, not skill activation",
  amount,has.boolean,used.boolean,before,s.bindings.properties->resolved[41],unsigned(*s.bindings.mana_exempt_14f0),s.bindings.saved_options->size());
 return "Original mana callbacks completed";
}

std::string Runtime::scalar_probe(std::int32_t value,bool write){
 if(!impl_->initialized)return "Player skill initialization incomplete";
 auto& s=*impl_;
 // A debug fixture calls the original globals through this same Player VM.
 // It gives reload tests an observable nonzero entry without altering authored
 // faery cooldown values or loading a second script/VM.
 constexpr const char* key="Native_Dictionary_Probe";
 dh2_script_value arguments[2]{};
 arguments[0].type=DH2_SCRIPT_STRING;arguments[0].text=key;arguments[0].text_bytes=std::strlen(key);
 Impl::number(arguments[1],static_cast<float>(value));
 const auto before=s.script_integers.size();
 const auto discard=[](void*,const dh2_script_first_return_v1*,char*,std::size_t){return 0;};
 if(write&&s.session->call("SetInt",arguments,2,0,discard,nullptr,s.error))return "Original scalar write blocked: "+s.error;
 dh2_script_first_return_v1 observed{};
 const auto capture=[](void* p,const dh2_script_first_return_v1* result,char*,std::size_t){*static_cast<dh2_script_first_return_v1*>(p)=*result;return 0;};
 if(s.session->call("GetInt",arguments,1,0,capture,&observed,s.error)||observed.type!=DH2_SCRIPT_NUMBER)return "Original scalar read blocked: "+s.error;
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native Player scalar probe | write %u | input %d | observed %.0f | entries before %zu | entries after %zu | AIS %p | VM %p | same LuaScript dictionary; debug fixture",
  unsigned(write),value,double(observed.number),before,s.script_integers.size(),reinterpret_cast<void*>(s.ais.ais),static_cast<void*>(s.session->vm()));
 return "Original scalar callbacks completed";
}

Runtime::Runtime(std::unique_ptr<Impl> p):impl_(std::move(p)){}
Runtime::~Runtime()=default;
std::unique_ptr<Runtime> Runtime::create(Bindings b,std::string& error){
 if(!b.character||!b.ai||!b.ai_lifetime||!b.source_ai||b.source_ai->identity!=b.ai||b.source_ai->owner_04!=b.character||
    !b.target_owner||!b.group_identity||!b.ai_tables||!b.animation_tables||!b.self_fx||!b.state_fx||!b.highlight_fx||!b.is_player||!b.declaration||b.declaration->script!="__player__"||!b.dead||!b.object||b.object->identity!=b.character||!b.controller||
    !b.tables||!b.catalogue_lifetime||!b.rules||!b.properties||!b.shared_property_temp||!b.savegame||b.savegame->character()!=b.character||!b.savegame->skills_initialized()||!b.application_singleton||!*b.application_singleton||!b.saved_options||!b.online||!b.online_identity||!b.mana_exempt_14f0||!b.current_difficulty||!b.classes||!b.fields||!b.design||!b.ai_constants||!b.faery_constants||!b.coordinator||!b.debug||!b.assets||!b.read||b.coordinator->owner()!=b.character){error="invalid native Player skill owner";return {};}
 std::unique_ptr<Runtime> owner;
 try{owner=std::unique_ptr<Runtime>(new Runtime(std::make_unique<Impl>(std::move(b))));}
 catch(const std::exception& e){error=e.what();return {};}
 auto& p=owner->impl_;
 // Allocate the outer owner before source publication. A later allocation
 // failure must not retire an AIS already borrowed by source_ai.
 try{p->initialize();}
 catch(const std::exception& e){
  p->update_blocked=true;
  // Source publication can precede a required InitProcess failure. Retain the
  // AIS/Session/instances/timers and reached effects until explicit teardown.
  __android_log_print(ANDROID_LOG_ERROR,"DH2Native","Native Player AIS failure retained | phase %zu | active %p | pending %p | VM %p | %s | no automatic initialization retry",
   std::size_t(p->bindings.source_ai->pointer_28),reinterpret_cast<void*>(p->bindings.source_ai->active_ais_1c),
   reinterpret_cast<void*>(p->bindings.source_ai->alternate_ais_20),p->session?static_cast<void*>(p->session->vm()):nullptr,e.what());
  // Diagnostics may allocate too. They cannot retire a published owner when
  // the source failure itself came from exhausted allocation.
  try{p->error=e.what();}catch(...){ }
 }
 try{error=p->error;}catch(...){error.clear();}
 return owner;
}
bool Runtime::initialized()const noexcept{return impl_->initialized;}
std::uintptr_t Runtime::character_identity()const noexcept{
 return impl_->initialized?impl_->bindings.character:0;
}
data::PropertyView* Runtime::combat_status_properties() noexcept {
 if(!impl_||!impl_->initialized||!impl_->buffs||
    !impl_->bindings.properties||
    impl_->buffs->character_identity()!=impl_->bindings.character||
    impl_->property_view.resolved!=impl_->bindings.properties->resolved.data())
  return nullptr;
 return &impl_->property_view;
}
character_player_buffs_v1::Owner* Runtime::combat_status_buffs() noexcept {
 if(!impl_||!impl_->initialized||!impl_->buffs||
    !impl_->bindings.properties||
    impl_->buffs->character_identity()!=impl_->bindings.character||
    impl_->property_view.resolved!=impl_->bindings.properties->resolved.data())
  return nullptr;
 return impl_->buffs.get();
}
void Runtime::update(){impl_->update();}
bool Runtime::update_after_saved_skill_slot_write(std::string& error){
 return impl_->update_after_saved_skill_slot_write(error);
}
bool Runtime::update_after_source_skill_inputs_changed(std::string& error){
 return impl_->update_after_source_skill_inputs_changed(error);
}
bool Runtime::reload_skill_instances(data::PlayerSaveLoadOwnerV1& save_loader,
                                     std::string& error){
 auto& s=*impl_;error.clear();
 if(!s.initialized||s.update_blocked||!s.preparation||!s.session||!s.updates||
    !s.bindings.savegame||&save_loader.save()!=s.bindings.savegame.get()||
    save_loader.save().character()!=s.bindings.character||
    !s.bindings.tables||!s.bindings.source_ai||
    s.bindings.source_ai->active_ais_1c!=s.ais.ais){
  error="AI_ReloadSkills requires the initialized same Player/Save/VM/preparation owner";
  return false;
 }
 s.timer_fields.reset();
 const auto restore_timer_lease=[&](){
  if(s.timer_fields||!s.preparation)return;
  auto lease=s.preparation->lease_timer_fields(s.bindings.character);
  if(lease)s.timer_fields=std::move(*lease);
 };
 const auto skills_list=dh2::character_ai_set_skills_and_spells::List::skill;
 try{
  const auto count=s.preparation->slots(skills_list).size();
  for(std::uint32_t i=0;i<count;++i){
   const auto instance=s.preparation->slots(skills_list)[i];
   if(instance&&!s.preparation->delete_skill_instance(i,instance,error)){
    restore_timer_lease();error="AI_ReloadSkills deleting destructor failed at slot "+std::to_string(i)+": "+error;return false;
   }
  }
  if(!s.preparation->reset_skill_end(error)){
   restore_timer_lease();error="AI_ReloadSkills failed to publish skill end=begin: "+error;return false;
  }
  s.refresh_properties();
  const auto* selected=s.bindings.tables->skill_list(s.property_view.resolved[28]);
  if(!selected){restore_timer_lease();error="AI_ReloadSkills selected Character SkillList is unavailable";return false;}
  if(!s.bindings.savegame->reload_skills_from_character_list(selected->members,error)){
   restore_timer_lease();return false;
  }
  if(!save_loader.load(8,error)){restore_timer_lease();return false;}

  s.refresh_properties();
  dh2::character_ai_set_skills_and_spells::Result prepared{};
  const auto status=s.preparation->prepare(&prepared);
  s.prepared=prepared;
  if(status!=dh2::character_ai_set_skills_and_spells::Status::complete){
   const auto detail=s.session->last_error();
   error="Same-VM skill preparation failed during AI_ReloadSkills";
   if(!detail.empty())error += ": "+detail;
   else error += " at a required source provider";
   return false;
  }
  // Adam's AI_ReloadSkills V6 sequence ends with the same existing native
  // skill update owner. NativeReloadSkills then has its distinct following
  // UpdateSkills service, so keep that next coordinator phase separate.
  if(!s.update_after_saved_skill_slot_write(error)){restore_timer_lease();return false;}
  s.timer_fields=s.preparation->lease_timer_fields(s.bindings.character);
  if(!s.timer_fields){error="AI_ReloadSkills rebuilt skills but could not reacquire the same timer-field lease";return false;}
  error.clear();return true;
 }catch(const std::exception& failure){
  restore_timer_lease();error=std::string("Same-owner AI_ReloadSkills provider failed: ")+failure.what();return false;
 }catch(...){
  restore_timer_lease();error="Same-owner AI_ReloadSkills provider failed";return false;
 }
}
bool Runtime::remove_all_buffs(std::string& error){
 auto& s=*impl_;error.clear();
 if(!s.initialized||!s.buffs||s.buffs->character_identity()!=s.bindings.character){
  error="NativeReloadSkills RemoveAllBuffs requires the retained same-Character BuffOwner";return false;
 }
 try{s.refresh_properties();}catch(const std::exception& failure){error=failure.what();return false;}
 dh2::character_player_buffs_v1::Result result{};
 const auto status=s.buffs->remove_all(&result);
 if(status!=dh2::character_player_buffs_v1::Status::complete){
  error="Same-owner RemoveAllBuffs failed at provider operation "+std::to_string(std::uint32_t(result.last_operation));
  return false;
 }
 error.clear();return true;
}
bool Runtime::cleanup_ai_scripts(std::string& error){
 auto& s=*impl_;error.clear();
 if(s.terminal_cleanup_complete)return true;
 if(s.terminal_cleanup_attempted){
  error=s.terminal_cleanup_failure.empty()?"CharAI terminal cleanup already failed":s.terminal_cleanup_failure;
  return false;
 }
 if(!s.cleanups)return true;
 s.terminal_cleanup_attempted=true;
 dh2::player_skill_cleanup_session_v1::Result result{};
 if(s.cleanups->cleanup_all(result,error)!=0){
  if(error.empty())error="CharAI AI_ScriptCleanUp skill/faery cleanup failed";
  s.terminal_cleanup_failure=error;
  return false;
 }
 s.terminal_cleanup_complete=true;
 __android_log_print(ANDROID_LOG_INFO,"DH2Native",
  "Player CharAI terminal cleanup | skill slots %u | faery slots %u | callbacks %u | completed %u | same retained VM/Character timers",
  result.skill_slots,result.faery_slots,result.cleanup_calls,result.completed);
 return true;
}
bool Runtime::cancel_sneaking(std::uint8_t* character_byte_415,
                              std::string& error){
 auto& s=*impl_;error.clear();
 if(!character_byte_415||!s.initialized||s.update_blocked||!s.buffs||
    s.buffs->character_identity()!=s.bindings.character||!s.bindings.is_player||
    !s.bindings.tables||!s.preparation||!s.uses||!s.session||
    !s.bindings.source_ai||s.bindings.source_ai->owner_04!=s.bindings.character||
    s.bindings.source_ai->active_ais_1c!=s.ais.ais||
    s.session->character_identity()!=s.bindings.character){
  error="Character::CancelSneaking requires the initialized same Player, BuffOwner, SkillList, AIS and VM";
  return false;
 }
 try{s.refresh_properties();}
 catch(const std::exception& failure){error=failure.what();return false;}
 std::uint32_t is_player=0;
 if(s.bindings.is_player(s.bindings.character_queries_context,
       s.bindings.character,&is_player,error)){
  if(error.empty())error="Character::CancelSneaking IsPlayer provider failed";
  return false;
 }
 if(!is_player){error.clear();return true;}

 // Character::CancelSneaking (IDA 0x3bc6b8) removes only buff 146, then
 // writes Character+0x415 before querying the post-removal property 198.
 // This is the existing canonical BuffOwner and does not clear other buffs.
 dh2::character_player_buffs_v1::Result removed{};
 if(s.buffs->remove(146,0,&removed)!=
       dh2::character_player_buffs_v1::Status::complete){
  error="Character::CancelSneaking PROPS_DelBuff(146) failed at BuffOwner operation "+
        std::to_string(std::uint32_t(removed.last_operation));
  return false;
 }
 *character_byte_415=1;
 const auto sneak_property=s.property_view.resolved[198];
 if(sneak_property<=0){error.clear();return true;}

 // IsSneaking is computed from property 198 (IDA 0x3bc690). The source then
 // scans this Character's selected SkillList for flag 0x02000000 and calls
 // AI_CancelSkill at the matching list slot. AI_CancelSkill runs the same
 // retained OnSkillCheck_Active and, only when true, OnPreSkill.
 auto selector=s.property_view.resolved[28];
 if(selector<0||std::size_t(selector)>=s.bindings.tables->skills().skill_lists.size())
  selector=3;
 const auto* list=s.bindings.tables->skill_list(selector);
 if(!list){error="Character::CancelSneaking selected source SkillList is unavailable";return false;}
 const auto& table=s.bindings.tables->skills().skills;
 const auto& scripts=s.preparation->slots(
     dh2::character_ai_set_skills_and_spells::List::skill);
 for(std::size_t slot=0;slot<list->members.size();++slot){
  const auto row_index=list->members[slot];
  if(row_index<0||std::size_t(row_index)>=table.size()){
   error="Character::CancelSneaking selected SkillList contains an invalid SkillTable row";
   return false;
  }
  if((std::uint32_t(table[std::size_t(row_index)].flags)&0x02000000u)==0)continue;
  // Character::CancelSneaking selects the first flagged row and calls
  // AI_CancelSkill(slot). AI_CancelSkill does not continue to another row:
  // it returns for a null script or when GetCharSkill(slot).Type != 1.
  if(slot>=scripts.size()||!scripts[slot]||table[std::size_t(row_index)].type!=1){
   error.clear();return true;
  }
  std::uint32_t active=0;
  if(skill_check(std::uint32_t(slot),true,active,error)<0){
   if(error.empty())error="Character::CancelSneaking OnSkillCheck_Active failed";
   return false;
  }
  if(!active){error.clear();return true;}
  SkillCallbackResult callback{};
  if(invoke_skill_callback(std::uint32_t(slot),SkillCallback::pre,
        callback,error)<0){
   if(error.empty())error="Character::CancelSneaking OnPreSkill failed";
   return false;
  }
  error.clear();return true;
 }
 error.clear();return true;
}
bool Runtime::recalculate_properties(bool source_argument,std::string& error){
 auto& s=*impl_;error.clear();
 if(!source_argument||!s.initialized||!s.bindings.classes||!s.bindings.properties||s.class_rows.empty()){
  error="NativeReloadSkills requires RecalcProperties(true) on the same class/property owner";return false;
 }
 try{s.refresh_properties();s.refresh_class_rows();}
 catch(const std::exception& failure){error=failure.what();return false;}
 if(dh2_property_validate(&s.property_view)||
    dh2_class_recalc_base(s.class_rows.data(),std::uint32_t(s.class_rows.size()),
                          s.bindings.properties->base.data(),&s.property_view)!=0){
  error="Same-owner Character RecalcProperties(true) failed";return false;
 }
 error.clear();return true;
}
bool Runtime::train_skill(std::uint32_t skill_index,bool test_only,data::FreshInventoryOwnedV4& inventory,
                          std::uint32_t& source_return,std::string& error){
 return impl_->train_skill(skill_index,test_only,inventory,source_return,error);
}
bool Runtime::initialize_initial_skill_grants(data::FreshInventoryOwnedV4& inventory,
                                               std::string& error){
 return impl_->initialize_initial_skill_grants(inventory,error);
}
int Runtime::skill_info(std::uint32_t skill_index,std::int32_t level,
                        std::vector<std::int32_t>& shared_temp,std::string& error){
 return impl_->skill_info(skill_index,level,shared_temp,error);
}
int Runtime::projectile_skill_callback(const char* callback_name,
    std::uintptr_t target_identity,std::uintptr_t projectile_identity,
    std::uintptr_t skill_script_identity,
    dh2_script_returns_observer_v1 observer,void* context,std::string& error){
 auto& s=*impl_;error.clear();
 if(!s.initialized||s.update_blocked||!s.preparation||!s.session||
    !s.bindings.tables||!s.bindings.savegame||
    !s.bindings.savegame->skills_initialized()||
    s.bindings.savegame->character()!=s.bindings.character||
    s.session->character_identity()!=s.bindings.character||
    !s.bindings.coordinator||s.bindings.coordinator->owner()!=s.bindings.character||
    !s.bindings.source_ai||s.bindings.source_ai->owner_04!=s.bindings.character||
    s.bindings.source_ai->active_ais_1c!=s.ais.ais||!s.ais.ais||
    !target_identity||!projectile_identity||!skill_script_identity){
  error="projectile callback requires the initialized same Player, Save, AIS, Coordinator and retained VM";
  return -1;
 }
 const auto* retained=s.preparation->instance_arguments(skill_script_identity);
 if(!retained||retained->identity!=skill_script_identity+0x0c||
    retained->values.size()!=2||
    retained->values[0].type!=dh2::character_player_skills_preparation_v3::Value::Type::string||
    retained->values[0].text.empty()||
    (retained->values[1].type!=dh2::character_player_skills_preparation_v3::Value::Type::integer&&
     retained->values[1].type!=dh2::character_player_skills_preparation_v3::Value::Type::number)){
  error="projectile skill identity has no retained Player Arguments owner at script+0x0c";
  return -1;
 }
 dh2_script_value args[2]{};
 const auto& name=retained->values[0].text;
 args[0].type=DH2_SCRIPT_STRING;args[0].text=name.data();args[0].text_bytes=name.size();
 args[1].type=DH2_SCRIPT_NUMBER;
 if(retained->values[1].type==dh2::character_player_skills_preparation_v3::Value::Type::integer)
  args[1].number=static_cast<float>(retained->values[1].word);
 else std::memcpy(&args[1].number,&retained->values[1].word,sizeof(args[1].number));
 return s.session->call_projectile_skill(callback_name,args,2,target_identity,
     projectile_identity,observer,context,error);
}
Runtime::ProjectileSourceState Runtime::projectile_skill_callback_source_state(
    std::uintptr_t skill_script_identity,std::string& error)const{
 const auto& s=*impl_;error.clear();
 if(!skill_script_identity||!s.initialized||s.update_blocked||!s.preparation){
  error="projectile callback source-state query has no live prepared Player owner";
  return ProjectileSourceState::invalid;
 }
 const auto* retained=s.preparation->instance_arguments(skill_script_identity);
 if(!retained||retained->identity!=skill_script_identity+0x0c||
    retained->values.size()!=2||
    retained->values[0].type!=dh2::character_player_skills_preparation_v3::Value::Type::string||
    retained->values[0].text.empty()||
    (retained->values[1].type!=dh2::character_player_skills_preparation_v3::Value::Type::integer&&
     retained->values[1].type!=dh2::character_player_skills_preparation_v3::Value::Type::number)){
  error="projectile source-state identity has no retained Player Arguments owner at script+0x0c";
  return ProjectileSourceState::invalid;
 }
 if(!s.bindings.tables||!s.bindings.savegame||
    !s.bindings.savegame->skills_initialized()||
    s.bindings.savegame->character()!=s.bindings.character||
    !s.bindings.coordinator||s.bindings.coordinator->owner()!=s.bindings.character||
    !s.bindings.source_ai||s.bindings.source_ai->owner_04!=s.bindings.character||
    s.bindings.source_ai->active_ais_1c!=s.ais.ais||!s.ais.ais){
  error="projectile source-state query does not match the retained Player Character/AIS owner";
  return ProjectileSourceState::invalid;
 }
 if(!s.session||s.session->character_identity()!=s.bindings.character){
  error="projectile source-state query has no canonical Player LuaScript owner";
  return ProjectileSourceState::invalid;
 }
 return s.session->has_source_lua_script()
     ? ProjectileSourceState::lua_script_present
     : ProjectileSourceState::lua_script_absent;
}
bool Runtime::hud_info(bool faery,std::uint32_t index,bool refresh_usable,
                       std::uint32_t& usable,float& cooldown_fraction,
                       std::string& error){
 return impl_->hud_info(faery,index,refresh_usable,usable,cooldown_fraction,error);
}
const std::vector<std::uintptr_t>* Runtime::prepared_skill_scripts()const noexcept{
 const auto& s=*impl_;
 if(!s.initialized||s.update_blocked||!s.preparation)return nullptr;
 return &s.preparation->slots(dh2::character_ai_set_skills_and_spells::List::skill);
}
bool Runtime::resolve_hud_skill_slot(std::int32_t hud_slot,
    std::int32_t skill_list_selector,
    player_hud_skill_slot_resolution_v1::Result& output,
    std::string& error)const{
 auto& s=*impl_;error.clear();
 if(!s.initialized||s.update_blocked||!s.preparation||!s.bindings.tables||
    !s.bindings.savegame||!s.bindings.savegame->skills_initialized()||
    s.bindings.savegame->character()!=s.bindings.character||!s.session||
    s.session->character_identity()!=s.bindings.character||
    !s.bindings.coordinator||s.bindings.coordinator->owner()!=s.bindings.character||
    !s.bindings.source_ai||s.bindings.source_ai->owner_04!=s.bindings.character||
    s.bindings.source_ai->active_ais_1c!=s.ais.ais){
  error="NativeHUDSkill resolution requires the initialized same Character, Save, AIS, Coordinator and retained Player VM";
  return false;
 }
 const auto& scripts=s.preparation->slots(
     dh2::character_ai_set_skills_and_spells::List::skill);
 player_hud_skill_slot_resolution_v1::Result value{};
 const auto status=player_hud_skill_slot_resolution_v1::resolve(
     *s.bindings.savegame,s.bindings.character,hud_slot,skill_list_selector,
     s.bindings.tables->skills(),&scripts,&value);
 if(status!=player_hud_skill_slot_resolution_v1::Status::complete){
  error="NativeHUDSkill source slot could not resolve to a prepared selected-skill script (status "+
        std::to_string(static_cast<std::int32_t>(status))+")";
  return false;
 }
 const auto* instance=s.preparation->instance(value.script_identity);
 if(!instance||instance->character!=s.bindings.character||
    instance->identity!=value.script_identity){
  error="NativeHUDSkill prepared script no longer belongs to the active Player Character";
  return false;
 }
 output=value;error.clear();return true;
}
bool Runtime::resolve_hud_skill_slot(std::int32_t hud_slot,
    player_hud_skill_slot_resolution_v1::Result& output,
    std::string& error)const{
 auto& s=*impl_;error.clear();
 if(!s.bindings.tables){
  error="NativeHUDSkill resolution requires the active retained SkillTables";
  return false;
 }
 auto selector=s.property_view.resolved[28];
 if(selector<0||std::size_t(selector)>=s.bindings.tables->skills().skill_lists.size())
  selector=3;
 if(std::size_t(selector)>=s.bindings.tables->skills().skill_lists.size()){
  error="NativeHUDSkill source fallback SkillList 3 is unavailable";
  return false;
 }
 return resolve_hud_skill_slot(hud_slot,selector,output,error);
}
bool Runtime::resolve_character_skill_row(std::uint32_t skill_index,
    std::int32_t skill_list_selector,
    character_ai_skill_commands_v1::SkillRow& output,
    std::string& error)const{
 auto& s=*impl_;error.clear();
 if(!s.initialized||s.update_blocked||!s.bindings.tables||
    !s.bindings.character||!s.bindings.savegame||
    s.bindings.savegame->character()!=s.bindings.character||!s.session||
    s.session->character_identity()!=s.bindings.character||
    !s.bindings.source_ai||s.bindings.source_ai->owner_04!=s.bindings.character||
    s.bindings.source_ai->active_ais_1c!=s.ais.ais){
  error="Character::GetCharSkill row requires the active same Character, AIS and retained SkillTables";
  return false;
 }
 const auto* row=s.bindings.tables->skill(skill_list_selector,skill_index);
 if(!row){
  error="Character::GetCharSkill index is outside its selected SkillList or SkillTable";
  return false;
 }
 character_ai_skill_commands_v1::SkillRow value{};
 value.identity=reinterpret_cast<std::uintptr_t>(row);
 value.animation_04=&row->anim;
 value.moving_08=&row->anim_is_moving;
 value.type_48=&row->type;
 if(!value.identity||!value.animation_04||!value.moving_08||!value.type_48){
  error="Character::GetCharSkill decoded SkillRow projection is incomplete";
  return false;
 }
 output=value;error.clear();return true;
}
bool Runtime::resolve_character_skill_row(std::uint32_t skill_index,
    character_ai_skill_commands_v1::SkillRow& output,
    std::string& error)const{
 auto& s=*impl_;error.clear();
 if(!s.bindings.tables){
  error="Character::GetCharSkill requires the active retained SkillTables";
  return false;
 }
 auto selector=s.property_view.resolved[28];
 if(selector<0||std::size_t(selector)>=s.bindings.tables->skills().skill_lists.size())
  selector=3;
 if(std::size_t(selector)>=s.bindings.tables->skills().skill_lists.size()){
  error="Character::GetCharSkill source fallback SkillList 3 is unavailable";
  return false;
 }
 return resolve_character_skill_row(skill_index,selector,output,error);
}
bool Runtime::end_skill(std::uint32_t skill_index,
    std::uintptr_t animation_owner_identity,void* animation_context,
    bool (*stop_loop)(void*,bool,std::string&),
    character_ai_skill_commands_v1::Result& output,std::string& error){
 namespace commands=character_ai_skill_commands_v1;
 auto& s=*impl_;error.clear();
 if(!animation_owner_identity||!s.skill_machine||
    animation_owner_identity!=s.skill_animation_owner||
    !s.initialized||s.update_blocked||!s.preparation||
    !s.bindings.tables||!s.bindings.source_ai||!s.bindings.coordinator||
    !s.bindings.coordinator->bound()||
    s.bindings.coordinator->owner()!=s.bindings.character||
    s.bindings.source_ai->owner_04!=s.bindings.character||
    s.bindings.source_ai->active_ais_1c!=s.ais.ais||
    !s.bindings.source_ai->identity||s.bindings.ai!=s.bindings.source_ai->identity){
  error="AI_EndSkill requires the initialized same Character, AIS, Coordinator, SkillTables and retained Player VM";
  return false;
 }
 struct Provider {
  Runtime::Impl* runtime;
  std::uintptr_t animation_owner;
  void* animation_context;
  bool (*stop_loop)(void*,bool,std::string&);
  std::string error;
 } provider{&s,animation_owner_identity,animation_context,stop_loop,{}};
 auto invoke=[](void* raw,commands::State*,const commands::Request* request,
                commands::Response* response)->std::int32_t {
  auto& p=*static_cast<Provider*>(raw);auto& owner=*p.runtime;
  if(!request||!response)return -1;
  switch(request->operation){
  case commands::Operation::get_skill_row:{
   if(request->subject!=owner.bindings.character||!owner.bindings.tables||
      !owner.bindings.source_ai||owner.bindings.source_ai->owner_04!=request->subject){
    p.error="AI_EndSkill GetCharSkill crossed the retained Character/SkillTables";return -1;
   }
   auto selector=owner.property_view.resolved[28];
   if(selector<0||std::size_t(selector)>=owner.bindings.tables->skills().skill_lists.size())selector=3;
   const auto* row=owner.bindings.tables->skill(selector,request->index);
   if(!row){p.error="AI_EndSkill GetCharSkill index is outside the selected SkillList";return -1;}
   response->row.identity=reinterpret_cast<std::uintptr_t>(row);
   response->row.animation_04=&row->anim;
   response->row.moving_08=&row->anim_is_moving;
   response->row.type_48=&row->type;
   if(!response->row.identity){p.error="AI_EndSkill selected SkillTable row has no native identity";return -1;}
   return 0;
  }
  case commands::Operation::stop_skill_loop:
   if(request->subject!=p.animation_owner||request->related!=owner.bindings.character||
      request->value!=1||!p.stop_loop){
    p.error="AI_EndSkill source StopLoop has no matching retained animation owner/provider";return -1;
   }
   if(!p.stop_loop(p.animation_context,true,p.error)){
    if(p.error.empty())p.error="AI_EndSkill animation StopLoop provider failed";
    return -1;
   }
   return 0;
  default:
   p.error="AI_EndSkill reached an operation outside its verified source provider set";return -1;
  }
 };

 // Reuse the same persistent logical Character/skill-machine projection that
 // Begin wrote. It borrows the Coordinator's sole state ID and live CharAI
 // bytes; it owns no FSM, timer, VM, animation or skill row.
 commands::OwnerSlot owner_slot{s.skill_machine->character()};
 commands::Fields fields{&s.bindings.source_ai->word_cc,
                         &s.bindings.source_ai->byte_d0,
                         &s.bindings.source_ai->byte_d1};
 commands::State state{};
 state.ai=s.bindings.ai;state.owner_04=&owner_slot;state.fields=&fields;
 state.assertion_level=0;
 const commands::Services services{&provider,invoke};
 commands::Result result{};
 const auto status=commands::execute(&state,commands::Command::end,skill_index,
                                     &services,&result);
 if(status!=commands::Status::complete){
  error=provider.error.empty()?"Source CharAI::AI_EndSkill command failed (status "+
        std::to_string(static_cast<std::int32_t>(status))+")":provider.error;
  output=result;return false;
 }
 output=result;error.clear();return true;
}

bool Runtime::bind_skill_state_callbacks(
    std::uintptr_t animation_owner_identity,
    const SkillStateServices& external,std::string& error){
 auto& s=*impl_;error.clear();
 if(!animation_owner_identity||!s.initialized||s.update_blocked||
    !s.preparation||!s.bindings.tables||!s.bindings.source_ai||
    !s.bindings.coordinator||!s.bindings.coordinator->bound()||
    s.bindings.coordinator->owner()!=s.bindings.character||
    s.bindings.source_ai->owner_04!=s.bindings.character||
    s.bindings.source_ai->active_ais_1c!=s.ais.ais||
    !s.bindings.source_ai->identity||s.bindings.ai!=s.bindings.source_ai->identity){
  error="CSSkill callback binding requires the initialized same Character, AIS, Coordinator, SkillTables and retained Player VM";
  return false;
 }
 if(s.skill_machine&&s.skill_animation_owner!=animation_owner_identity){
  error="CSSkill callback animation owner differs from the retained Character+0x49c projection";
  return false;
 }
 if(!s.skill_machine){
  try{
   s.skill_machine=std::make_unique<dh2::character_ai_skill_machine_projection_v1::Projection>(
       *s.bindings.coordinator,s.bindings.character,animation_owner_identity);
   s.skill_animation_owner=animation_owner_identity;
  }catch(const std::exception& failure){error=failure.what();return false;}
 }
 return s.skill_machine->bind_skill_state_callbacks(s.bindings.ai,
     external.debug_switches_identity,external.ooi_intent_412,
     external.physical_2dc,external.callbacks,error);
}

bool Runtime::unbind_skill_state_callbacks(std::string& error){
 auto& s=*impl_;error.clear();
 if(!s.skill_machine){error="CSSkill callback binding has no retained skill-machine projection";return false;}
 return s.skill_machine->unbind_skill_state_callbacks(error);
}
bool Runtime::retire_skill_state_callbacks(std::string& error){
 auto& s=*impl_;error.clear();
 if(!s.skill_machine){error="CSSkill callback binding has no retained skill-machine projection";return false;}
 return s.skill_machine->retire_skill_state_callbacks(error);
}
bool Runtime::skill_state_callbacks_bound()const noexcept{
 const auto& s=*impl_;
 return s.skill_machine&&s.skill_machine->skill_state_callbacks_bound();
}

bool Runtime::bind_cast_state_callbacks(std::uintptr_t animation_owner_identity,
    const CastStateServices& external,std::string& error){
 auto& s=*impl_;error.clear();
 auto* coordinator=s.bindings.coordinator;
 if(!animation_owner_identity||!s.initialized||s.update_blocked||
    !s.preparation||!s.uses||!s.session||!s.session->vm()||
    !s.bindings.tables||!s.bindings.source_ai||!coordinator||!coordinator->bound()||
    coordinator->owner()!=s.bindings.character||
    s.session->character_identity()!=s.bindings.character||
    s.bindings.source_ai->owner_04!=s.bindings.character||
    s.bindings.source_ai->active_ais_1c!=s.ais.ais||!s.ais.ais||
    !s.bindings.source_ai->identity||s.bindings.ai!=s.bindings.source_ai->identity){
  error="CSCast callback binding requires the initialized same Character, AIS, Coordinator and retained Player VM";
  return false;
 }
 if(!s.skill_machine){
  error="CSCast callback binding requires the existing retained skill-machine projection";
  return false;
 }
 auto* machine=s.skill_machine->machine();
 auto* character=s.skill_machine->character();
 const auto* machine_state=s.skill_machine->state_query();
 if(!machine||!character||!machine_state||!machine->identity||
    machine->owner_04!=s.bindings.character||
    machine->state_query!=machine_state||
    machine_state->current_state_id!=&coordinator->state.current||
    !machine->animation_28||character->identity!=s.bindings.character||
    character->skill_machine_4fc!=machine||
    character->stop_skill_loop_receiver_49c!=animation_owner_identity||
    s.skill_animation_owner!=animation_owner_identity){
  error="CSCast callback binding differs from the retained Character/Coordinator/machine/animator owners";
  return false;
 }
 if(!external.debug_switches_identity||!external.ooi_intent_412||
    !external.callbacks.invoke){
  error="CSCast callback binding requires DebugSwitches, OOI intent and every real source service";
  return false;
 }

 if(s.cast_state_projection){
  const auto& current=*s.cast_state_projection;
  const bool same=current.character.identity==s.bindings.character&&
      current.character.coordinator_state==&coordinator->state&&
      current.character.flags_520==&coordinator->state.flags&&
      current.character.machine==machine->identity&&
      current.character.animator==animation_owner_identity&&
      current.character.ooi_intent_412==external.ooi_intent_412&&
      current.globals.debug_switches==external.debug_switches_identity&&
      current.services.context==external.callbacks.context&&
      current.services.invoke==external.callbacks.invoke;
  if(!same){error="CSCast callback graph is already bound to different borrowed owners";return false;}
  if(!coordinator->bind_cast_projection(s.cast_state_projection.get())){
   error="CSCast callback projection no longer belongs to the active Coordinator";return false;
  }
  return true;
 }

 std::unique_ptr<dh2::character_cast_lifecycle_v1::Projection> projection;
 try{
  projection=std::make_unique<dh2::character_cast_lifecycle_v1::Projection>();
 }catch(const std::exception& failure){error=failure.what();return false;}
 projection->character={s.bindings.character,&coordinator->state,
     &coordinator->state.flags,machine->identity,animation_owner_identity,
     external.ooi_intent_412};
 projection->globals={external.debug_switches_identity};
 projection->services=external.callbacks;
 if(!coordinator->bind_cast_projection(projection.get())){
  error="Coordinator rejected the CSCast projection or state7 is already active";
  return false;
 }
 s.cast_state_projection=std::move(projection);
 return true;
}

bool Runtime::unbind_cast_state_callbacks(std::string& error){
 auto& s=*impl_;error.clear();
 if(!s.cast_state_projection){error="CSCast callback projection is not bound";return false;}
 if(!s.bindings.coordinator||
    !s.bindings.coordinator->unbind_cast_projection(s.cast_state_projection.get())){
  error="CSCast callback projection cannot detach during state7 or active Coordinator dispatch";
  return false;
 }
 s.cast_state_projection.reset();
 return true;
}

bool Runtime::retire_cast_state_callbacks(std::string& error){
 auto& s=*impl_;error.clear();
 if(!s.cast_state_projection){error="CSCast callback projection is not bound";return false;}
 if(!s.bindings.coordinator||
    !s.bindings.coordinator->retire_cast_projection(s.cast_state_projection.get())){
  error="CSCast callback projection cannot retire during active Coordinator dispatch";
  return false;
 }
 s.cast_state_projection.reset();
 return true;
}

bool Runtime::cast_state_callbacks_bound()const noexcept{
 const auto& s=*impl_;
 if(!s.cast_state_projection||!s.bindings.coordinator||!s.skill_machine)return false;
 const auto* machine=s.skill_machine->machine();
 return machine&&machine->identity&&
     s.cast_state_projection->character.coordinator_state==&s.bindings.coordinator->state&&
     s.cast_state_projection->character.flags_520==&s.bindings.coordinator->state.flags&&
     s.cast_state_projection->character.identity==s.bindings.character&&
     s.cast_state_projection->character.machine==machine->identity&&
     s.cast_state_projection->character.animator==s.skill_animation_owner;
}

bool Runtime::cast_state_machine_identity(std::uintptr_t& identity)const noexcept{
 identity=0;const auto& s=*impl_;
 if(!s.skill_machine||!s.bindings.coordinator)return false;
 const auto* machine=s.skill_machine->machine();
 const auto* character=s.skill_machine->character();
 const auto* state=s.skill_machine->state_query();
 if(!machine||!character||!state||!machine->identity||
    machine->owner_04!=s.bindings.character||machine->state_query!=state||
    state->current_state_id!=&s.bindings.coordinator->state.current||
    character->identity!=s.bindings.character||character->skill_machine_4fc!=machine)
  return false;
 identity=machine->identity;return true;
}

bool Runtime::select_skill_animation(std::uintptr_t animation_owner_identity,
    std::int32_t requested_animation,std::int32_t& selected_animation,
    std::string& error){
 auto& s=*impl_;error.clear();
 if(!animation_owner_identity||!s.initialized||s.update_blocked||
    !s.skill_machine||s.skill_animation_owner!=animation_owner_identity||
    !s.skill_machine->skill_state_callbacks_bound()){
  error="SM_SetAnim requires the bound retained Character skill-machine and animation owners";
  return false;
 }
 auto* machine=s.skill_machine->machine();
 if(!machine||machine->owner_04!=s.bindings.character||!machine->identity||
    !machine->animation_28){
  error="SM_SetAnim retained source animation field is unavailable";return false;
 }
 // IDA CharStateMachine::SM_SetAnim (0x3c0b50): if the remembered animation
 // is not -1, store -1 and replay the remembered animation; if it is -1,
 // pass the caller's value through and leave the sentinel in place.
 auto selected=*machine->animation_28;
 if(selected!=-1)*machine->animation_28=-1;
 else selected=requested_animation;
 selected_animation=selected;error.clear();return true;
}

bool Runtime::select_cast_animation(std::int32_t animation_table_id,
    std::int32_t saved_faery_slot,
    std::uintptr_t animation_owner_identity,
    std::int32_t& selected_animation,std::string& error){
 auto& s=*impl_;error.clear();
 const auto& b=s.bindings;
 if(!animation_owner_identity||!s.initialized||s.update_blocked||
    !b.coordinator||!b.coordinator->bound()||
    b.coordinator->owner()!=b.character||!b.animation_tables||!b.properties){
  error="SM_SetCastState requires the active Character, Coordinator, animation table and properties";
  return false;
 }

 const auto& animation_tables=*b.animation_tables;
 if(animation_tables.characters.empty()){
  error="SM_SetCastState CharAnimTable has no decoded rows";return false;
 }
 // Character::GetCharAnimTableId (IDA 0x3a3228) reads the active resolved
 // property and falls back to row 17 when it is outside CharAnimTable.
 auto active_table=b.properties->resolved[2];
 if(active_table<0||std::size_t(active_table)>=animation_tables.characters.size())
  active_table=17;
 if(std::size_t(active_table)>=animation_tables.characters.size()||
    animation_table_id!=active_table){
  error="SM_SetCastState animation-table ID differs from the active Character source row";
  return false;
 }

 if(saved_faery_slot<0){
  error="SM_SetCastState faery animation slot is negative";return false;
 }

 // SM_SetCastState (IDA 0x3c6394) directly indexes the active
 // CharAnimTable.Spells[faeryId]. It does not resolve the FaeryList, inspect
 // FaeryTable, or require a prepared spell script; those owners are consulted
 // by AI_BeginSpell before this source operation. Keep this animation helper
 // faithful to its own boundary so valid CharAnimTable entries are not
 // rejected by unrelated inventory/script projections.
 const auto* sequence=dh2::data::animation_state(animation_tables,
     animation_table_id,"Spells",saved_faery_slot);
 if(!sequence){
  error="SM_SetCastState CharAnimTable Spells entry is unavailable or outside the sequence table";
  return false;
 }
 const auto sequence_id=sequence-animation_tables.sequences.data();
 if(sequence_id<0||sequence_id>INT32_MAX){
  error="SM_SetCastState Spells sequence ID is outside the source integer domain";
  return false;
 }

 // The verified Android animations_pycst asset has
 // AnimStancedAnim/SL__LIST_IPHONE = 210 (0xd2). IDA tests bit 0x400000;
 // it is clear, so GetAnimStance is not reached and the selected sequence is
 // used unchanged. Preserve an explicit failure if a different asset reaches
 // that source branch without a live stance provider.
 constexpr char stance_group[]="AnimStancedAnim";
 constexpr char stance_key[]="SL__LIST_IPHONE";
 dh2_pycst_result stance_mask{};
 if(dh2_pycst_get(&s.animation_constants,stance_group,sizeof(stance_group)-1,
       stance_key,sizeof(stance_key)-1,&stance_mask)||!stance_mask.found){
  error="SM_SetCastState Android stance constant is unavailable";return false;
 }
 if((static_cast<std::uint32_t>(stance_mask.value)&0x400000u)!=0){
  error="SM_SetCastState reached source GetAnimStance without a live stance provider";
  return false;
 }

 if(s.skill_machine&&s.skill_animation_owner!=animation_owner_identity){
  error="SM_SetCastState animation owner differs from the retained Character machine projection";
  return false;
 }
 if(!s.skill_machine){
  try{
   s.skill_machine=std::make_unique<dh2::character_ai_skill_machine_projection_v1::Projection>(
       *b.coordinator,b.character,animation_owner_identity);
   s.skill_animation_owner=animation_owner_identity;
  }catch(const std::exception& failure){error=failure.what();return false;}
 }
 if(!s.skill_machine){
  error="SM_SetCastState has no retained Character skill-machine projection";
  return false;
 }
 auto* machine=s.skill_machine->machine();
 if(!machine||machine->owner_04!=b.character||!machine->identity||
    !machine->animation_28||s.skill_animation_owner!=animation_owner_identity){
  error="SM_SetCastState retained Character machine animation field is unavailable";
  return false;
 }
 *machine->animation_28=static_cast<std::int32_t>(sequence_id);
 selected_animation=static_cast<std::int32_t>(sequence_id);
 error.clear();return true;
}

bool Runtime::begin_spell(std::uintptr_t animation_owner_identity,
    SpellOperationResult& output,std::string& error){
 auto& s=*impl_;const auto& b=s.bindings;error.clear();output={};
 if(!animation_owner_identity||!s.initialized||s.update_blocked||
    !s.preparation||!s.uses||!s.session||!s.session->vm()||
    !b.savegame||b.savegame->character()!=b.character||!b.current_difficulty||
    !b.tables||!b.properties||!b.animation_tables||!b.online||!b.online_identity||
    !b.source_ai||b.source_ai->owner_04!=b.character||
    b.source_ai->active_ais_1c!=s.ais.ais||!b.coordinator||
    !b.coordinator->bound()||b.coordinator->owner()!=b.character||
    !s.skill_machine||s.skill_animation_owner!=animation_owner_identity||
    !cast_state_callbacks_bound()){
  error="AI_BeginSpell requires the active same Character, offline session, Save, AIS, Coordinator, cast lifecycle and retained faery VM/animation owners";
  return false;
 }
 // AI_IsSpellUsable (IDA 0x3d80b4) first rejects SM_IsUsingSkill/state 6,
 // SM_IsCasting/state 7 and an unloaded CharAI script process (>6 is loaded).
 // Match those source guards before reading the current saved faery.
 const auto state=b.coordinator->state.current;
 if(state==6||state==7||b.source_ai->pointer_28<=6){
  error.clear();return true;
 }
 if(*b.online){
  error="AI_BeginSpell reached online controller messaging without a source messaging provider";
  return false;
 }
 if(!b.properties||!b.savegame||!b.tables||!b.current_difficulty){
  error="AI_BeginSpell active Save/property/faery table owners disappeared";return false;
 }
 try{s.refresh_properties();}
 catch(const std::exception& failure){error=failure.what();return false;}
 const auto difficulty=*b.current_difficulty;
 if(difficulty<0||difficulty>=3||
    !b.savegame->faeries_initialized()[static_cast<std::size_t>(difficulty)]){
  error="AI_BeginSpell SG_GetCurrentFaerieId has no initialized active difficulty";
  return false;
 }
 const auto slot=b.savegame->current_faery(static_cast<std::uint32_t>(difficulty));
 if(slot<0){error="AI_BeginSpell SG_GetCurrentFaerieId returned an invalid saved faery slot";return false;}
 output.saved_faery_slot=slot;

 // GetCharFaery uses the slot as a position in the selected FaeryList and
 // validates FaeryRow.Type (+32) against that saved position. SpellType (+28)
 // is a separate field and controls the AI_BeginSpell/AI_EndSpell branches.
 const auto& faeries=b.tables->faeries();
 auto list_id=b.properties->resolved[29];
 if(list_id<0||static_cast<std::size_t>(list_id)>=faeries.faery_lists.size())list_id=0;
 if(static_cast<std::size_t>(list_id)>=faeries.faery_lists.size()){
  error="AI_BeginSpell selected FaeryList fallback row is unavailable";return false;
 }
 const auto& members=faeries.faery_lists[static_cast<std::size_t>(list_id)].members;
 const auto slot_index=static_cast<std::size_t>(slot);
 const auto& scripts=s.preparation->slots(
      dh2::character_ai_set_skills_and_spells::List::faery);
 if(slot_index>=members.size()||slot_index>=scripts.size()){
  error="AI_BeginSpell current faery slot is outside GetCharFaery or m_spellScripts";
  return false;
 }
 const auto row_id=members[slot_index];
 if(row_id<0||static_cast<std::size_t>(row_id)>=faeries.faeries.size()||
    faeries.faeries[static_cast<std::size_t>(row_id)].type!=slot){
  error="AI_BeginSpell GetCharFaery row/type validation failed for the saved slot";
  return false;
 }
 const auto& row=faeries.faeries[static_cast<std::size_t>(row_id)];
 output.spell_type=row.spell_type;
 // IDA CharAI::AI_IsSpellActive (0x3d7d9c) is `return 0` in this ELF.
 // Therefore the SpellType==1 OnPreSkill/online special arm at 0x3d82a8 is
 // unreachable; source falls through to AI_IsSpellUsable for every row.
 if(row.spell_type==1){ /* AI_IsSpellActive is a verified constant false. */ }
 if(!scripts[slot_index]){error.clear();return true;}
 const auto* instance=s.preparation->instance(scripts[slot_index]);
 if(!instance||instance->character!=b.character||instance->identity!=scripts[slot_index]){
  error="AI_BeginSpell current faery script is stale or belongs to another Character";
  return false;
 }
 // AI_IsSpellUsable's final source operation is the prepared faery's
 // OnSkillCheck_Usable on the existing Player VM. A false result stops before
 // the CharAI byte writes, animation selection and C356 dispatch.
 dh2::player_skill_use_session_v1::Result usable{};
 const auto check_status=s.uses->check(dh2::player_skill_use_session_v1::List::faery,
     slot_index,dh2::player_skill_use_session_v1::Check::usable,usable,error);
 if(check_status){
  if(error.empty())error="AI_BeginSpell OnSkillCheck_Usable failed in the retained faery VM";
  return false;
 }
 output.usable=usable.value;
 if(!output.usable){error.clear();return true;}

 // Source CharAI::AI_BeginSpell (IDA 0x3d81c0) clears these AI bytes only
 // after AI_IsSpellUsable succeeds, then calls SM_SetCastState and event C356.
 b.source_ai->byte_d0=0;b.source_ai->byte_d1=0;
 auto active_table=b.properties->resolved[2];
 if(active_table<0||static_cast<std::size_t>(active_table)>=b.animation_tables->characters.size())
  active_table=17;
 std::int32_t selected_animation=-1;
 if(!select_cast_animation(active_table,slot,animation_owner_identity,
       selected_animation,error))return false;
 const auto event_status=b.coordinator->event(50006u);
 if(event_status<0){
  error="AI_BeginSpell SM_SetCastState C356 dispatch failed after the source byte/animation prefix";
  return false;
 }
 output.is_casting=b.coordinator->state.current==7?1u:0u;
 error.clear();return true;
}

bool Runtime::end_spell(std::uintptr_t animation_owner_identity,
    void* animation_context,bool (*stop_loop)(void*,bool,std::string&),
    SpellOperationResult& output,std::string& error){
 auto& s=*impl_;const auto& b=s.bindings;error.clear();output={};
 if(!animation_owner_identity||!s.initialized||s.update_blocked||
    !b.savegame||b.savegame->character()!=b.character||!b.current_difficulty||
    !b.tables||!b.properties||!b.online||!b.online_identity||!b.source_ai||
    b.source_ai->owner_04!=b.character||b.source_ai->active_ais_1c!=s.ais.ais||
    !b.coordinator||!b.coordinator->bound()||b.coordinator->owner()!=b.character||
    !s.skill_machine||s.skill_animation_owner!=animation_owner_identity||
    !cast_state_callbacks_bound()){
  error="AI_EndSpell requires the active same Character, offline session, Save, AIS, Coordinator, cast lifecycle and retained animation owners";
  return false;
 }
 // AI_EndSpell starts with SM_IsCasting and is otherwise a source no-op.
 if(b.coordinator->state.current!=7){error.clear();return true;}
 output.is_casting=1;
 if(*b.online){
  error="AI_EndSpell reached online controller messaging without a source messaging provider";
  return false;
 }
 const auto difficulty=*b.current_difficulty;
 if(difficulty<0||difficulty>=3||
    !b.savegame->faeries_initialized()[static_cast<std::size_t>(difficulty)]){
  error="AI_EndSpell SG_GetCurrentFaerieId has no initialized active difficulty";
  return false;
 }
 const auto slot=b.savegame->current_faery(static_cast<std::uint32_t>(difficulty));
 if(slot<0){error="AI_EndSpell SG_GetCurrentFaerieId returned an invalid saved faery slot";return false;}
 output.saved_faery_slot=slot;
 const auto& faeries=b.tables->faeries();
 auto list_id=b.properties->resolved[29];
 if(list_id<0||static_cast<std::size_t>(list_id)>=faeries.faery_lists.size())list_id=0;
 if(static_cast<std::size_t>(list_id)>=faeries.faery_lists.size()){
  error="AI_EndSpell selected FaeryList fallback row is unavailable";return false;
 }
 const auto& members=faeries.faery_lists[static_cast<std::size_t>(list_id)].members;
 const auto slot_index=static_cast<std::size_t>(slot);
 if(slot_index>=members.size()){
  error="AI_EndSpell current faery slot is outside GetCharFaery";return false;
 }
 const auto row_id=members[slot_index];
 if(row_id<0||static_cast<std::size_t>(row_id)>=faeries.faeries.size()||
    faeries.faeries[static_cast<std::size_t>(row_id)].type!=slot){
  error="AI_EndSpell GetCharFaery row/type validation failed for the saved slot";
  return false;
 }
 const auto spell_type=faeries.faeries[static_cast<std::size_t>(row_id)].spell_type;
 output.spell_type=spell_type;
 // IDA CharAI::AI_EndSpell (0x3d7f60) only changes loop/AI state for
 // SpellType==2. It raises no FSM event in the supported offline path.
 if(spell_type==2){
  if(b.source_ai->byte_d0){
   if(!stop_loop||!animation_context){
    error="AI_EndSpell SpellType==2 requires the real retained CharAnimator::ANIM_StopLoop provider";
    return false;
   }
   if(!stop_loop(animation_context,true,error)){
    if(error.empty())error="AI_EndSpell CharAnimator::ANIM_StopLoop(true) provider failed";
    return false;
   }
  }else{
   b.source_ai->byte_d1=1;
  }
 }
 error.clear();return true;
}

bool Runtime::begin_skill(std::uint32_t skill_index,
    std::uintptr_t animation_owner_identity,const BeginSkillServices& external,
    character_ai_skill_commands_v1::Result& output,std::string& error){
 namespace commands=character_ai_skill_commands_v1;
 auto& s=*impl_;error.clear();output={};output.trophy_index=-1;
 if(!animation_owner_identity||!s.initialized||s.update_blocked||!s.preparation||
    !s.uses||!s.session||!s.bindings.tables||!s.bindings.source_ai||
    !s.bindings.coordinator||!s.bindings.coordinator->bound()||
    s.bindings.coordinator->owner()!=s.bindings.character||
    s.session->character_identity()!=s.bindings.character||
    s.bindings.source_ai->owner_04!=s.bindings.character||
    s.bindings.source_ai->active_ais_1c!=s.ais.ais||
    !s.bindings.source_ai->identity||s.bindings.ai!=s.bindings.source_ai->identity){
  error="AI_BeginSkill requires the initialized same Character, AIS, Coordinator, SkillTables and retained Player VM";
  return false;
 }
 if(s.skill_machine&&s.skill_animation_owner!=animation_owner_identity){
  error="AI_BeginSkill animation owner differs from the retained Character+0x49c projection";
  return false;
 }
 if(!s.skill_machine){
  try{
   s.skill_machine=std::make_unique<dh2::character_ai_skill_machine_projection_v1::Projection>(
       *s.bindings.coordinator,s.bindings.character,animation_owner_identity);
   s.skill_animation_owner=animation_owner_identity;
  }catch(const std::exception& failure){error=failure.what();return false;}
 }
 try{s.refresh_properties();}
 catch(const std::exception& failure){error=failure.what();return false;}

 struct Provider{
  Runtime::Impl* runtime;
  const BeginSkillServices* external;
  std::vector<const char*> trophy_names;
  std::string error;
 } provider{&s,&external,{},{}};
 auto invoke=[](void* raw,commands::State* state,const commands::Request* request,
                commands::Response* response)->std::int32_t{
  auto& p=*static_cast<Provider*>(raw);auto& owner=*p.runtime;
  if(!state||!request||!response)return -1;
  auto& b=owner.bindings;
  switch(request->operation){
  case commands::Operation::get_skill_row:{
   if(request->subject!=b.character||!b.tables||!b.source_ai||
      b.source_ai->owner_04!=request->subject||
      b.source_ai->active_ais_1c!=owner.ais.ais){
    p.error="AI_BeginSkill GetCharSkill crossed the active Character/AIS/SkillTables";return -1;
   }
   commands::SkillRow row{};
   if(!owner.initialized||!b.tables){p.error="AI_BeginSkill selected SkillTables are unavailable";return -1;}
   auto selector=owner.property_view.resolved[28];
   if(selector<0||std::size_t(selector)>=b.tables->skills().skill_lists.size())selector=3;
   const auto* source_row=b.tables->skill(selector,request->index);
   if(!source_row){p.error="AI_BeginSkill GetCharSkill index is outside the selected SkillList";return -1;}
   row.identity=reinterpret_cast<std::uintptr_t>(source_row);
   row.animation_04=&source_row->anim;row.moving_08=&source_row->anim_is_moving;
   row.type_48=&source_row->type;
   response->row=row;return 0;
  }
  case commands::Operation::check_active:
  case commands::Operation::check_usable:{
   const auto* scripts=owner.preparation?&owner.preparation->slots(
       dh2::character_ai_set_skills_and_spells::List::skill):nullptr;
   if(!scripts||request->index>=scripts->size()||!(*scripts)[request->index]){
    p.error="AI_BeginSkill Active/Usable index has no retained Player script";return -1;
   }
   const auto expected_subject=request->operation==commands::Operation::check_active?
       (*scripts)[request->index]:b.ai;
   if(request->subject!=expected_subject||request->related!=b.character){
    p.error="AI_BeginSkill Active/Usable query does not name the selected retained Player owner";return -1;
   }
   dh2::player_skill_use_session_v1::Result result{};std::string why;
   const auto check=request->operation==commands::Operation::check_active?
       dh2::player_skill_use_session_v1::Check::active:
       dh2::player_skill_use_session_v1::Check::usable;
   if(owner.uses->check(dh2::player_skill_use_session_v1::List::skill,
       request->index,check,result,why)){
    p.error=why.empty()?"AI_BeginSkill retained Player VM query failed":why;return -1;
   }
   response->word=result.value;return 0;
  }
  case commands::Operation::pre:{
   const auto* scripts=owner.preparation?&owner.preparation->slots(
       dh2::character_ai_set_skills_and_spells::List::skill):nullptr;
   if(!scripts||request->index>=scripts->size()||!(*scripts)[request->index]||
      request->subject!=(*scripts)[request->index]||request->related!=b.character){
    p.error="AI_BeginSkill OnPreSkill does not name the selected retained Player script";return -1;
   }
   dh2::player_skill_use_session_v1::Result result{};std::string why;
   if(owner.uses->invoke(dh2::player_skill_use_session_v1::List::skill,
       request->index,dh2::player_skill_use_session_v1::Callback::pre,result,why)){
    p.error=why.empty()?"AI_BeginSkill retained Player OnPreSkill failed":why;return -1;
   }
   response->word=result.value;return 0;
  }
  case commands::Operation::get_constant:{
   if(request->subject!=b.character||!request->text){p.error="AI_BeginSkill animation constant request is invalid";return -1;}
   const char* slash=std::strchr(request->text,'/');
   if(!slash||slash==request->text||!slash[1]){p.error="AI_BeginSkill animation constant path is malformed";return -1;}
   dh2_pycst_result value{};
   if(dh2_pycst_get(&owner.animation_constants,request->text,
       std::uint32_t(slash-request->text),slash+1,
       std::uint32_t(std::strlen(slash+1)),&value)||!value.found){
    p.error="Player AnimStancedAnim/SL__LIST_IPHONE constant is unavailable";return -1;
   }
   response->word=static_cast<std::uint32_t>(value.value);return 0;
  }
  case commands::Operation::get_anim_stance:{
   if(request->subject!=b.character||!p.external->get_anim_stance){
    p.error="AI_BeginSkill reached source GetAnimStance without the active Character provider";return -1;
   }
   if(p.external->get_anim_stance(p.external->context,request->subject,
       response->signed_word,p.error)){
    if(p.error.empty())p.error="AI_BeginSkill GetAnimStance provider failed";return -1;
   }
   return 0;
  }
  case commands::Operation::raise_state_event:{
   if(request->subject!=owner.skill_machine->machine()->identity||
      request->index!=0xc355||request->related!=0){
    p.error="AI_BeginSkill state event differs from source C355 dispatch";return -1;
   }
   if(!b.coordinator||b.coordinator->event(request->index,request->related)<0){
    p.error="AI_BeginSkill C355 reached the retained Coordinator without its live CSSkill dispatcher/animation services";return -1;
   }
   return 0;
  }
  case commands::Operation::set_state:
   p.error="AI_BeginSkill unexpectedly requested forced state installation";return -1;
  case commands::Operation::is_player:{
   if(request->subject!=b.character||!b.is_player){p.error="AI_BeginSkill Player classification provider is unavailable";return -1;}
   std::uint32_t value=0;
   if(b.is_player(b.character_queries_context,request->subject,&value,p.error)){
    if(p.error.empty())p.error="AI_BeginSkill IsPlayer provider failed";return -1;
   }
   response->word=value;return 0;
  }
  case commands::Operation::property_add_int:{
   if(request->subject!=b.character||request->index!=216||request->value>INT32_MAX/256u){
    p.error="AI_BeginSkill Player property AddInt request is invalid";return -1;
   }
   if(dh2_property_add(&owner.property_view,216,std::int32_t(request->value*256u))){
    p.error="AI_BeginSkill live Player property 216 AddInt failed";return -1;
   }
   return 0;
  }
  case commands::Operation::trophy_manager:{
   auto* manager=p.external->trophy_manager;
   if(request->subject!=b.ai||!manager||!manager->initialized()){
    p.error="AI_BeginSkill captured source TrophyManager singleton is unavailable or uninitialized";return -1;
   }
   response->identity=reinterpret_cast<std::uintptr_t>(manager);return 0;
  }
  case commands::Operation::property_get_int:{
   if(request->subject!=b.character||request->index!=216){p.error="AI_BeginSkill property GetInt receiver differs from captured Character";return -1;}
   std::int32_t fixed=0;
   if(dh2_property_resolve(&owner.property_view,216,&fixed)){
    p.error="AI_BeginSkill live Player property 216 GetInt failed";return -1;
   }
   response->signed_word=fixed/256;return 0;
  }
  case commands::Operation::is_local_player:{
   if(request->subject!=b.character||!p.external->is_local_player){
    p.error="AI_BeginSkill reached PlayerManager::IsLocalPlayer without its active service";return -1;
   }
   if(p.external->is_local_player(p.external->context,request->subject,
       response->word,p.error)){
    if(p.error.empty())p.error="AI_BeginSkill IsLocalPlayer provider failed";return -1;
   }
   return 0;
  }
  case commands::Operation::trophy_names:{
   auto* manager=p.external->trophy_manager;
   if(!manager||request->related!=reinterpret_cast<std::uintptr_t>(manager)){
    p.error="AI_BeginSkill TrophyTable lookup does not use the captured source manager";return -1;
   }
   const auto& names=manager->table_names();p.trophy_names.clear();
   p.trophy_names.reserve(names.size());
   for(const auto& name:names)p.trophy_names.push_back(name.c_str());
   response->trophy_names={p.trophy_names.empty()?nullptr:p.trophy_names.data(),
       static_cast<std::uint32_t>(p.trophy_names.size())};
   return 0;
  }
  case commands::Operation::unlock_trophy:{
   auto* manager=p.external->trophy_manager;
   if(!manager||request->subject!=reinterpret_cast<std::uintptr_t>(manager)||
      request->signed_value<0||manager->find_id_by_name("epic_withskills")!=request->signed_value){
    p.error="AI_BeginSkill UnlockTrophy index is not the exact source epic_withskills row";return -1;
   }
   const auto status=manager->unlock(request->signed_value);
   if(status!=dh2::data::TrophyUnlockStatusV1::completed&&
      status!=dh2::data::TrophyUnlockStatusV1::already_unlocked&&
      status!=dh2::data::TrophyUnlockStatusV1::already_unlocking){
    p.error="AI_BeginSkill source TrophyManager::UnlockTrophy failed";return -1;
   }
   return 0;
  }
  case commands::Operation::stop_skill_loop:
   p.error="AI_BeginSkill unexpectedly reached AI_EndSkill StopLoop";return -1;
  case commands::Operation::assertion_log:
   p.error="AI_BeginSkill reached a source assertion without the native assertion logger";return -1;
  }
  p.error="AI_BeginSkill reached an unsupported source provider operation";return -1;
 };

 const auto& scripts=s.preparation->slots(dh2::character_ai_set_skills_and_spells::List::skill);
 commands::SkillVector vector{};
 if(!scripts.empty())vector={scripts.data(),scripts.data()+scripts.size()};
 commands::OwnerSlot owner_slot{s.skill_machine->character()};
 commands::Fields fields{&s.bindings.source_ai->word_cc,
                         &s.bindings.source_ai->byte_d0,
                         &s.bindings.source_ai->byte_d1};
 commands::State state{};state.ai=s.bindings.ai;state.owner_04=&owner_slot;
 state.skill_vector_b4=&vector;state.fields=&fields;state.assertion_level=0;
 const commands::Services services{&provider,invoke};
 commands::Result result{};
 const auto status=commands::execute(&state,commands::Command::begin,skill_index,
                                     &services,&result);
 output=result;
 if(status!=commands::Status::complete){
  error=provider.error.empty()?"Source CharAI::AI_BeginSkill command failed (status "+
        std::to_string(static_cast<std::int32_t>(status))+", phase "+
        std::to_string(static_cast<std::uint32_t>(result.phase))+")":provider.error;
  return false;
 }
 error.clear();return true;
}

int Runtime::skill_check(std::uint32_t skill_slot,bool active,
                         std::uint32_t& value,std::string& error){
 auto& s=*impl_;error.clear();value=0;
 if(!s.initialized||s.update_blocked||!s.uses||!s.preparation||!s.session||
    !s.bindings.coordinator||!s.bindings.source_ai||
    s.bindings.coordinator->owner()!=s.bindings.character||
    s.session->character_identity()!=s.bindings.character||
    s.bindings.source_ai->owner_04!=s.bindings.character||
    s.bindings.source_ai->active_ais_1c!=s.ais.ais){
  error="Player skill check requires the initialized same Character, AIS, Coordinator and retained VM";
  return -1;
 }
 using List=dh2::player_skill_use_session_v1::List;
 using Check=dh2::player_skill_use_session_v1::Check;
 dh2::player_skill_use_session_v1::Result result{};
 const int status=s.uses->check(List::skill,skill_slot,
     active?Check::active:Check::usable,result,error);
 if(status==0)value=result.value;
 return status;
}
bool Runtime::combat_owner(std::uintptr_t& ais,const std::uint32_t*& flags)const noexcept{
 const auto& s=*impl_;
 if(!s.initialized||!s.session||!s.bindings.source_ai||
    !s.ais.ais||s.session->ais_identity()!=s.ais.ais||
    s.bindings.source_ai->active_ais_1c!=s.ais.ais)return false;
 ais=s.ais.ais;flags=&s.ais.flags_b8;return true;
}
bool Runtime::aggro_music_owner(std::uintptr_t ais,std::int32_t*& aggro_count,
                                std::int32_t*& aggro_for_music)noexcept{
 aggro_count=nullptr;aggro_for_music=nullptr;
 auto& s=*impl_;
 std::uintptr_t active=0;const std::uint32_t* flags=nullptr;
 if(!ais||!combat_owner(active,flags)||active!=ais||
    s.bindings.source_ai->owner_04!=s.bindings.character||
    s.source_fields.script.identity!=ais)return false;
 // PlayerFields retains these source words as uint32_t for its captured
 // constructor image. The original Player callbacks load/store them as signed
 // 32-bit integers; corresponding signed/unsigned types may alias the same
 // object representation, and no other owner copies or rewrites these fields.
 aggro_count=reinterpret_cast<std::int32_t*>(&s.source_fields.skill_d0);
 aggro_for_music=reinterpret_cast<std::int32_t*>(&s.source_fields.skill_d4);
 return true;
}
bool Runtime::target_in_melee_range(void* provider_context,
    std::int32_t (*provider)(void*,dh2::character_ai_melee_range::State*,
        const dh2::character_ai_melee_range::Request*,
        dh2::character_ai_melee_range::Response*),
    std::uint32_t& value,std::string& error)const{
 value=0;error.clear();
 if(!impl_){error="Player melee-range runtime is absent";return false;}
 const auto& s=*impl_;const auto& b=s.bindings;
 if(!s.initialized||!s.session||!b.source_ai||!b.character||!b.ai||
    b.source_ai->identity!=b.ai||b.source_ai->owner_04!=b.character){
  error="Player melee-range requires the same initialized Character and CharAI";
  return false;
 }
 dh2::character_ai_melee_range::State state{
     b.ai,b.character,b.source_ai->target_40};
 const dh2::character_ai_melee_range::Services services{provider_context,provider};
 dh2::character_ai_melee_range::Result result{};
 const auto status=dh2::character_ai_melee_range::evaluate_object(
     &state,0,&services,&result);
 if(status!=dh2::character_ai_melee_range::Status::complete){
  switch(status){
  case dh2::character_ai_melee_range::Status::service_unavailable:
   error="Source CharAI melee-range provider is unavailable";break;
  case dh2::character_ai_melee_range::Status::service_failed:
   error="Source CharAI melee-range provider failed";break;
  case dh2::character_ai_melee_range::Status::invalid_source_fact:
   error="Source CharAI melee-range facts are invalid";break;
  case dh2::character_ai_melee_range::Status::invalid_argument:
   error="Source CharAI melee-range state is invalid";break;
  case dh2::character_ai_melee_range::Status::complete:break;
  }
  return false;
 }
 value=result.value;return true;
}
int Runtime::dispatch_combat_result(std::uintptr_t ais,
    ais_combat_result_dispatch_v1::Callback callback,
    std::uintptr_t attacker,std::uintptr_t defender,std::string& error){
 auto& s=*impl_;error.clear();
 if(!s.initialized||!s.session||!s.bindings.source_ai||
    !s.bindings.coordinator||!s.bindings.coordinator->bound()||
    s.bindings.coordinator->owner()!=s.bindings.character||
    s.session->character_identity()!=s.bindings.character||
    s.session->ais_identity()!=s.ais.ais||ais!=s.ais.ais||
    s.bindings.source_ai->owner_04!=s.bindings.character||
    s.bindings.source_ai->active_ais_1c!=s.ais.ais||
    (attacker!=s.bindings.character&&defender!=s.bindings.character)){
  error="Player combat dispatch requires its active retained AIS VM and participating Character";return -1;
 }
 const std::uint32_t membership=callback==ais_combat_result_dispatch_v1::Callback::target_hit?0x800u:
     callback==ais_combat_result_dispatch_v1::Callback::target_missed?0x1000u:0u;
 if(!membership){error="unsupported Player combat callback";return -1;}
 if(!(s.ais.flags_b8&membership)){error="requested Player combat callback is absent from the active VCB";return -1;}
 return s.session->dispatch_combat_result(ais,callback,attacker,defender,error);
}
int Runtime::invoke_skill_callback(std::uint32_t skill_slot,
    SkillCallback callback,SkillCallbackResult& result,std::string& error){
 auto& s=*impl_;error.clear();
 player_skill_use_session_v1::Callback source_callback{};
 switch(callback){
 case SkillCallback::pre:source_callback=player_skill_use_session_v1::Callback::pre;break;
 case SkillCallback::use:source_callback=player_skill_use_session_v1::Callback::use;break;
 case SkillCallback::post:source_callback=player_skill_use_session_v1::Callback::post;break;
 default:error="Player skill callback phase is invalid";return -1;
 }
 if(!s.initialized||s.update_blocked||!s.uses||!s.preparation||!s.session||
    !s.bindings.coordinator||!s.bindings.source_ai||
    s.bindings.coordinator->owner()!=s.bindings.character||
    s.session->character_identity()!=s.bindings.character||
    s.bindings.source_ai->owner_04!=s.bindings.character||
    s.bindings.source_ai->active_ais_1c!=s.ais.ais){
  error="Player skill callback requires the initialized same Character, AIS, Coordinator and retained VM";
  return -1;
 }
 const auto& slots=s.preparation->slots(dh2::character_ai_set_skills_and_spells::List::skill);
 if(skill_slot>=slots.size()){
  error="Player skill callback slot is outside the retained prepared skill vector";
  return -1;
 }
 if(!slots[skill_slot]){
  error="Player skill callback slot has no retained source script instance";
  return -1;
 }
 const auto* instance=s.preparation->instance(slots[skill_slot]);
 if(!instance||instance->character!=s.bindings.character||instance->identity!=slots[skill_slot]){
  error="Player skill callback instance no longer belongs to the active Character";
  return -1;
 }
 player_skill_use_session_v1::Result source_result{};
 const auto status=s.uses->invoke(player_skill_use_session_v1::List::skill,
                                  skill_slot,source_callback,source_result,error);
 if(status==0){
  result.value=source_result.value;result.call_count=source_result.call_count;
  result.last_lua_status=source_result.last_lua_status;
 }
 return status;
}
int Runtime::invoke_faery_callback(std::uint32_t faery_slot,
    SkillCallback callback,SkillCallbackResult& result,std::string& error){
 auto& s=*impl_;error.clear();
 player_skill_use_session_v1::Callback source_callback{};
 switch(callback){
 case SkillCallback::pre:source_callback=player_skill_use_session_v1::Callback::pre;break;
 case SkillCallback::use:source_callback=player_skill_use_session_v1::Callback::use;break;
 case SkillCallback::post:source_callback=player_skill_use_session_v1::Callback::post;break;
 default:error="Player faery callback phase is invalid";return -1;
 }
 if(!s.initialized||s.update_blocked||!s.uses||!s.preparation||!s.session||
    !s.bindings.coordinator||!s.bindings.source_ai||
    s.bindings.coordinator->owner()!=s.bindings.character||
    s.session->character_identity()!=s.bindings.character||
    s.bindings.source_ai->owner_04!=s.bindings.character||
    s.bindings.source_ai->active_ais_1c!=s.ais.ais){
  error="Player faery callback requires the initialized same Character, AIS, Coordinator and retained VM";
  return -1;
 }
 using SourceList=dh2::character_ai_set_skills_and_spells::List;
 const auto& slots=s.preparation->slots(SourceList::faery);
 if(faery_slot>=slots.size()){
  // CharAI::_SpellFocus/_SpellBlur/_SpellEvent guard the current ID against
  // m_spellScripts.size() and return normally when it is outside the vector.
  result={};error.clear();return 0;
 }
 if(!slots[faery_slot]){
  // Those source helpers also skip a null script entry without invoking Lua.
  result={};error.clear();return 0;
 }
 const auto* instance=s.preparation->instance(slots[faery_slot]);
 if(!instance||instance->character!=s.bindings.character||instance->identity!=slots[faery_slot]){
  error="Player faery callback instance no longer belongs to the active Character";
  return -1;
 }
 // IDA CSCast entry/exit raises Character events 32/33, which dispatch
 // _SpellFocus/_SpellBlur; the state-7 "do_spell" marker dispatches
 // _SpellEvent. All three invoke the same Player script methods as skills,
 // but on CharAI's separate faery-script vector (List::faery).
 player_skill_use_session_v1::Result source_result{};
 const auto status=s.uses->invoke(player_skill_use_session_v1::List::faery,
                                  faery_slot,source_callback,source_result,error);
 if(status==0){
  result.value=source_result.value;result.call_count=source_result.call_count;
  result.last_lua_status=source_result.last_lua_status;
 }
 return status;
}
void Runtime::state_service(std::uint32_t service){
 auto& s=*impl_;if(!s.focus_services)throw std::runtime_error("Native Character cleanup owner unavailable");
 s.refresh_properties();dh2::character_dead_focus_services_v1::Result result{};
 if(s.focus_services->deliver(dh2::character::Service(service),&result,s.error)!=dh2::character_dead_focus_services_v1::Status::complete)
  throw std::runtime_error("Native Character cleanup failed: "+s.error);
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native Character cleanup service | service %u | calls %u | sneak %d | byte415 %u | buffs %u | groups %u | same source Character/buff/property owners",service,result.calls,result.sneak,unsigned(s.bindings.source_ai->targetable_4d),s.buffs->count(),s.property_view.group_count);
}
void Runtime::died(std::uintptr_t killer){
 auto& s=*impl_;if(!s.initialized||!s.death)throw std::runtime_error("Player source death owners unavailable");
 s.refresh_properties();s.bindings.target_owner->character_ai_id=s.bindings.properties->resolved[1];
 s.target_services.ai_property_count=std::int32_t(s.bindings.ai_tables->rows.size());
 if(s.death->died(killer,&s.died_result,s.error)!=dh2::player_ai_death_v1::Status::complete)
  throw std::runtime_error("Native Player source death failed: "+s.error);
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native Player source death complete | state %d | payload null | target %zu | last target %zu | timer stops %u | timer33 %u | timer34 %u | outgoing %u | incoming %u | skill %u | faery %u | buffs %u | groups %u | VM %p | AIS %p | update attempts %u | HP %d | same retained owners; Character Kill continuation pending",
  s.bindings.coordinator->state.current,std::size_t(s.bindings.source_ai->target_40),std::size_t(s.bindings.source_ai->last_target_44),s.died_result.timer_stops,s.bindings.source_ai->word_10,s.bindings.source_ai->word_14,s.died_result.outgoing_completed,s.died_result.incoming_completed,s.died_result.skill_completed,s.died_result.spell_completed,s.buffs->count(),s.property_view.group_count,static_cast<void*>(s.session->vm()),reinterpret_cast<void*>(s.ais.ais),s.update_attempts,s.bindings.properties->resolved[36]);
 s.log_ai_timers("dead");
}
bool Runtime::on_revived(std::string& error){
 auto& s=*impl_;error.clear();
 if(!s.initialized||!s.session||!s.bindings.source_ai||
    !s.bindings.coordinator||s.bindings.source_ai->identity!=s.bindings.ai||
    s.bindings.source_ai->owner_04!=s.bindings.character||
    s.bindings.source_ai->active_ais_1c!=s.ais.ais||!s.ais.ais){
  error="Player CharAI::OnRevived requires the initialized same Player AIS";
  return false;
 }
 const dh2::character_ai_on_revived_v1::Services services{
     &s,&Impl::revived_backend};
 dh2::character_ai_on_revived_v1::Result result{};
 const auto status=dh2::character_ai_on_revived_v1::execute(
     s.bindings.source_ai,s.bindings.coordinator,s.bindings.character,
     s.bindings.ai,s.ais.ais,services,&result,error);
 if(status!=dh2::character_ai_on_revived_v1::Status::complete){
  if(error.empty())error="Player CharAI::OnRevived failed";
  return false;
 }
 s.log_ai_timers("revived");
 error.clear();return true;
}
bool Runtime::deserialize_script_lifecycle(std::string& error){
 auto& s=*impl_;error.clear();
 if(!s.initialized||s.deserialize_lifecycle_busy||s.terminal_cleanup_attempted||
    !s.cleanups||!s.bindings.source_ai||!s.bindings.coordinator||
    !s.bindings.dead||!s.bindings.design||!s.bindings.source_ai->identity||
    s.bindings.source_ai->owner_04!=s.bindings.character||
    s.bindings.source_ai->active_ais_1c!=s.ais.ais||!s.ais.ais||
    !s.bindings.coordinator->bound()||
    s.bindings.coordinator->owner()!=s.bindings.character||
    s.bindings.declaration->script!="__player__"||*s.bindings.dead>1){
  error="Player Character::Deserialize AIS cleanup/init requires the live same Player AIS, timers, and script owners";
  return false;
 }
 s.deserialize_lifecycle_busy=true;
 struct ResetGuard {bool& busy;~ResetGuard(){busy=false;}} guard{s.deserialize_lifecycle_busy};
 auto& ai=*s.bindings.source_ai;
 // CharAI::AI_ScriptCleanUp (0x3cfd7c) stops the two source timer IDs, writes
 // both fields to -1, cleans the existing skill and spell vectors, then
 // dispatches AIS::OnTerminate. For this exact IPhone Player, vtable 0x966cd8
 // binds that virtual to AISDefault::OnTerminate (0x3dbe84), an observed BX LR.
 if(s.bindings.coordinator->stop_timer(ai.word_10)<0||
    s.bindings.coordinator->stop_timer(ai.word_14)<0){
  error="Player Character::Deserialize could not stop its same-Coordinator AIS timers";
  return false;
 }
 ai.word_14=UINT32_MAX;ai.word_10=UINT32_MAX;
 dh2::player_skill_cleanup_session_v1::Result cleanup{};
 if(s.cleanups->cleanup_all(cleanup,error)!=0){
  if(error.empty())error="Player Character::Deserialize AI_ScriptCleanUp failed";
  return false;
 }
 // CharAI::AI_ScriptInit (0x3cfde4) restarts AI_Tick/DoT_Tick only while the
 // same Character is alive. The IPhone Player's OnInit, OnInitPost and
 // OnInitFinal vtable slots resolve to the observed empty AISDefault leaves
 // at 0x3dbe78/7c/80, so no second VM callback is appropriate here.
 if(!*s.bindings.dead){
  const auto duration=[&](const char* key,std::uint32_t& out){
   dh2_pycst_result value{};
   if(dh2_pycst_get(s.bindings.design,"CharacterDesign",15,key,
         static_cast<std::uint32_t>(std::strlen(key)),&value))return false;
   out=value.found&&value.value>0?static_cast<std::uint32_t>(value.value):0u;
   return true;
  };
  std::uint32_t ai_tick=0,dot_tick=0;
  if(!duration("AI_Tick",ai_tick)||!duration("DoT_Tick",dot_tick)){
   error="Player Character::Deserialize AI_ScriptInit CharacterDesign lookup failed";
   return false;
  }
  const auto ai_timer=s.bindings.coordinator->start_timer(ai_tick,-1,0x33,0);
  if(ai_timer<0){error="Player Character::Deserialize AI_Tick timer allocation failed";return false;}
  ai.word_10=static_cast<std::uint32_t>(ai_timer);
  const auto dot_timer=s.bindings.coordinator->start_timer(dot_tick,-1,0x34,0);
  if(dot_timer<0){error="Player Character::Deserialize DoT_Tick timer allocation failed";return false;}
  ai.word_14=static_cast<std::uint32_t>(dot_timer);
 }
 error.clear();return true;
}
bool Runtime::deserialize_clear_target(std::string& error){
 auto& s=*impl_;error.clear();
 if(!s.initialized||!s.bindings.source_ai||!s.bindings.target_owner||
    !s.bindings.source_ai->identity||
    s.bindings.source_ai->identity!=s.bindings.ai||
    s.bindings.source_ai->owner_04!=s.bindings.character||
    s.bindings.target_owner->identity!=s.bindings.character||
    s.bindings.source_ai->active_ais_1c!=s.ais.ais){
  error="Player Character::Deserialize target clear lost its same-Character CharAI owner";
  return false;
 }
 const auto status=dh2::character_clear_target_v1::clear(
     s.bindings.source_ai,s.bindings.target_owner,s.bindings.character,
     &s.target_services);
 if(status!=dh2::character_clear_target_v1::Status::complete){
  error="Player Character::Deserialize AI_SetTarget(nullptr,false) failed";
  return false;
 }
 // AI_SyncLastTarget is the source assignment CharAI+0x44 = CharAI+0x40.
 s.bindings.source_ai->last_target_44=s.bindings.source_ai->target_40;
 error.clear();return true;
}
player_enemy_kill_credit_v1::Status Runtime::credit_enemy_kill(
 std::uintptr_t victim,std::uintptr_t killer,std::uint32_t kill_force,
 const data::AggroTable& victim_outgoing,std::uint32_t controller_forced,
 std::uint32_t global_blocked,void* renderer_context,
 int (*clear_renderer_selection)(void*,std::uintptr_t),
 player_enemy_kill_credit_v1::Result* output,std::string& error){
 namespace credit=player_enemy_kill_credit_v1;
 auto& s=*impl_;
 if(!output||!victim){error="Invalid reached Player Character::Kill inputs";return credit::Status::invalid_argument;}
 if(!killer||kill_force)return credit::Status::ineligible_kill;
 if(!s.initialized||!s.session||!s.bindings.source_ai||!s.bindings.coordinator||
    !s.bindings.properties||!s.bindings.rules||!s.bindings.source_ai->identity||
    !s.bindings.coordinator->bound()||s.bindings.coordinator->owner()!=s.bindings.character){
  error="Native Player Kill credit owners are unavailable";return credit::Status::invalid_argument;
 }
 const auto active=s.bindings.source_ai->active_ais_1c;
 if(active&&active!=s.ais.ais){error="Native Player active AIS differs from its retained Session";return credit::Status::invalid_argument;}
 try{s.refresh_properties();}
 catch(const std::exception& exception){error=exception.what();return credit::Status::invalid_argument;}
 credit::Bindings bindings{};
 bindings.character=s.bindings.character;
 bindings.char_ai=s.bindings.source_ai->identity;
 bindings.active_ais=active;
 bindings.controller=s.bindings.controller;
 bindings.state_machine=reinterpret_cast<std::uintptr_t>(&s.bindings.coordinator->state);
 bindings.property_owner=reinterpret_cast<std::uintptr_t>(s.bindings.properties);
 // This is the controller's force bit from RaiseAIEvent. It remains distinct
 // from Character::Kill's force argument passed separately below.
 bindings.forced=controller_forced;
 bindings.locked=s.bindings.coordinator->state.controller_locked;
 bindings.paused=s.bindings.source_ai->paused_18;
 bindings.global_blocked=global_blocked;
 bindings.victim_outgoing=&victim_outgoing;
 bindings.properties=&s.property_view;
 bindings.current_target=&s.bindings.source_ai->target_40;
 bindings.target_context=renderer_context;
 bindings.clear_matching_target=clear_renderer_selection;
 const std::uint32_t state_event_bit=1u<<character::ai_event_state_event;
 const std::uint32_t ais_virtual_bit=1u<<character::ai_event_ais_virtual;
 bindings.backend={&s,Impl::kill_credit_backend,
  state_event_bit|(active?ais_virtual_bit:0u),0};
 credit::Runtime episode(bindings);
 return episode.after_loot_attempt(victim,killer,kill_force,output,error);
}
void Runtime::timer(std::uint32_t id){impl_->timer(id);}
void Runtime::buff_expired(const character::Timer32& timer){
 auto& s=*impl_;s.refresh_properties();dh2::character_player_buffs_v1::Result result{};
 if(s.buffs->expired(&timer,&result)!=dh2::character_player_buffs_v1::Status::complete)
  throw std::runtime_error("Native Player buff expiry provider failed");
}
bool Runtime::ai_timer(std::int32_t event,const character::Timer32& timer){
 auto& s=*impl_;if(!s.ai_ticks||(event!=0x33&&event!=0x34))return false;
 const auto hp_before=s.bindings.properties->resolved[36],mp_before=s.bindings.properties->resolved[41];
 s.refresh_properties();dh2::player_ai_timer_events_v1::Result result{};
 const auto status=s.ai_ticks->deliver(event,&timer,&result,s.error);
 if(status!=dh2::player_ai_timer_events_v1::Status::complete){
  __android_log_print(ANDROID_LOG_ERROR,"DH2Native","Native Player AI timer failed | event %x | slot %u | %s | source effects retained",event,timer.id,s.error.c_str());return false;
 }
 auto& count=event==0x33?s.regen_ticks:s.dot_ticks;++count;
 if(event==0x33||count<=3||count%64==0)
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native Player AI timer delivered | event %x | slot %u | count %u | combat %u | remote %u | HP rate %u | MP rate %u | HP %d / %d | MP %d / %d | DoT attacks %u | HP before %d | MP before %d | no extra FSM event",
   event,timer.id,count,result.in_combat,result.remote_word,result.regen.hp_amount,result.regen.mp_amount,
   s.bindings.properties->resolved[36],s.bindings.properties->resolved[38],s.bindings.properties->resolved[41],s.bindings.properties->resolved[43],result.dots.attacks,hp_before,mp_before);
 return true;
}
void Runtime::restore(AAssetManager* assets,const void* ai,const void* catalogue){
 auto& s=*impl_;
 if(s.bindings.ai_lifetime.get()!=ai||s.bindings.catalogue_lifetime.get()!=catalogue)throw std::runtime_error("Retained Player skill owner differs");
 s.assets=assets;
 s.refresh_class_rows();
 s.refresh_properties();if(s.buffs)s.log_buffs("restore");
 if(s.initialized)s.log_ai_timers("restore");
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native Player skills retained | VM %p | paths %zu | update attempts %u | timer callbacks %u | no preparation/OnInit replay",s.session?static_cast<void*>(s.session->vm()):nullptr,s.session?s.session->loaded_path_count():0,s.update_attempts,s.timer_callbacks);
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native Player AIS retained | initialized %u | AI %p | AIS %p | active %p | pending %p | phase %zu | timer33 %u | timer34 %u | same owner/failure latch; no load/InitProcess replay",
  unsigned(s.initialized),reinterpret_cast<void*>(s.bindings.ai),reinterpret_cast<void*>(s.ais.ais),reinterpret_cast<void*>(s.bindings.source_ai->active_ais_1c),
  reinterpret_cast<void*>(s.bindings.source_ai->alternate_ais_20),std::size_t(s.bindings.source_ai->pointer_28),s.bindings.source_ai->word_10,s.bindings.source_ai->word_14);
}
}
