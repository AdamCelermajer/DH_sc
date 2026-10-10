#include "source_campaign_character_fsm_v101.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "model_renderer.hpp"
#include "canonical_character_candidate_v60.hpp"
#include "source_campaign_animation_random_scope_v101.hpp"
#include "character_skill_target_binding_v46.hpp"
#include "character_script_source_virtuals_v101.hpp"
#include "character_ai_state_changed_vm.hpp"
#include "character_animation_selection_debug_v126.hpp"
#include "character_animation_event_owner_v1.hpp"
#include "npc_animation_event_owner_v1.hpp"
#include "character_melee_animation_event_v1.hpp"
#include "character_idle_update.hpp"
#include "character_heading_owner_v1.hpp"
#include "character_script_commands.hpp"
#include "navigation_objects.hpp"
#include "navigation_heading.hpp"
#include "character_collision_lifecycle_v1.hpp"
#include "campaign_navigation_registry_v64.hpp"
#include "application_player_manager_bootstrap_v59.hpp"
#include "canonical_point3d_globals_v1.hpp"
#include "character_cancel_sneaking.hpp"
#include "source_campaign_fx_v77.hpp"
#include "canonical_character_death_v84.hpp"
#include "visual_fx_manager_libraries_v63.hpp"
#include "character_candidate_cache_v62.hpp"
#include "audio_named_animation_sound_v38.hpp"
#include "faery_cast_state_v2.hpp"
#include "character_script_call_timer.hpp"
#include "character_world_attack_geometry_v1.hpp"
#include "npc_attack_command_owner_v1.hpp"
#include "character_design_services.hpp"
#include "character_ai_groups_v87.hpp"
#include "canonical_character_spawn_select_v87.hpp"
#include "canonical_character_pf_v62.hpp"
#include "source_campaign_script_actor_v96.hpp"
#include "source_campaign_script_ai_v118.hpp"
#include "character_ai_frame.hpp"
#include "character_ai_update.hpp"
#include "character_world_target_frame_v2.hpp"
#include "character_update_master_v108.hpp"
#include "source_campaign_ai_queue_v105.hpp"
#include "source_campaign_character_zoning_v108.hpp"
#include "world_click_target_v1.hpp"
#include "character_update_aggro_v108.hpp"
#include "character_skill_aggro_v6.hpp"
#include "source_campaign_death_rewards_v84.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "player_save_difficulty_global_v29.hpp"
#include "source_object_handle_assertion_v105.hpp"
#include "source_campaign_character_interaction_v114.hpp"
#include "canonical_gameobject_base_owner_v1.hpp"
#include "player_injure_state_v7.hpp"
#include "character_defensive_state_v1.hpp"
#include "canonical_npc_skills_v84.hpp"
#include "character_skill_callback_session_v3.hpp"
#include "character_native_effects.hpp"
#include "character_head_object_v2.hpp"
#include "character_slow_reaction_v1.hpp"
#include "canonical_character_save_v86.hpp"
#include "character_knockback_reaction_v1.hpp"
#include <design_settings.hpp>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <stdexcept>
namespace model_renderer {namespace {
using namespace dh2::character;
using namespace dh2::character::skills;
namespace data=dh2::data;
namespace actor=dh2::actor;
namespace animation=dh2::animation;
namespace scene=dh2::scene;
namespace target_search=dh2::target_search;
namespace target_providers=dh2::target_providers;
using Record=dh2::world::CanonicalCharacterCandidateRecordV60;
class CampaignFsmV101 {
 Record& r_;
 CharacterAnimationSelectionDebugV126 animation_debug_v126_{r_.services.debug,r_.services.debug_files};
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> application_;
 std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> backend_application_lease_v1_;
 std::shared_ptr<dh2::floors::World> floors_;
 std::shared_ptr<dh2::navigation::CampaignNavigationRegistryV64> navigation_;
 std::shared_ptr<void> process_pf_lease_;
 // Clean-backend campaign binding pins the same native World separately from
 // the record so its borrowed PF registry/context remain alive with this FSM.
 std::shared_ptr<void> backend_world_lease_v1_;
 const dh2::navigation::CollisionWorld* pf_geometry_{};
 dh2::navigation::ObstacleRegistry* pf_registry_{};
 std::shared_ptr<dh2::world::CanonicalObjectManagerV1> objects_;
 std::unique_ptr<CharacterWorldAttackGeometryV1> geometry_;
 std::unique_ptr<NpcAttackCommandOwnerV1> npc_attack_;
 ControllerCommandState32* npc_attack_controller_{};
 std::unique_ptr<CharacterWorldTargetFrameV2> target_frame_v108_;
 std::map<std::uintptr_t,std::unique_ptr<std::string>> debug_strings_;
 SkillStateV4 skill_{};
 // Refreshed loan view over the actor's actual SkillAI owner, slots and AI
 // fields. The original source context is a per-call local, so this stable
 // member carries only its exact current projections while a strong FSM loan
 // is held by SourceCampaignCharacterSkillContextBorrowV1.
 SkillAIContextV3 skill_ai_context_v3_{};
 PreSpawnState48 pre_spawn_{};
 std::vector<std::array<std::int32_t,40>> pre_spawn_rows_v108_;
 PreSpawnServices16 pre_spawn_services_v108_{this,pre_spawn_service_v108};
 std::vector<std::int32_t> stunned_v115_,scared_v115_;
 NativeEffects32 effects_v115_{};
 NativeEffectServices16 effect_services_v115_{this,effect_service_v115};
 AIEventServices24 events_{this,ai_service,63,0};
 SkillStateServices16V4 skill_services_{this,skill_service};
 TimerServices32 timer_expiry_v102_{this,[](void* raw,std::uintptr_t id,std::int32_t event,Timer32* timer){auto& t=*static_cast<CampaignFsmV101*>(raw);if(id!=t.r_.actor->object->identity||!timer||!t.raise(static_cast<unsigned>(event),reinterpret_cast<std::uintptr_t>(timer)))t.unavailable("Required SAME canonical CharTimer event delivery");},nullptr,0};
 ScriptCommandState48 script_command_state_{};
 ScriptCommandBindings40 script_commands_{};
 struct NpcScriptBinding {CampaignFsmV101* owner{};std::uint32_t address{};};
 std::map<std::uint32_t,NpcScriptBinding> npc_script_bindings_;
 std::vector<target_search::Target24> npc_script_target_heap_;
 target_search::List40 npc_script_targets_{};
 std::uint32_t npc_script_character_filter_{},npc_script_object_filter_{};
 void* previous_gameplay_context_{};
 int(*previous_gameplay_binding_)(void*,std::uint32_t,dh2_script_function*,void**){};
 std::array<float,3> script_look_{};
 std::unique_ptr<CharacterCollisionLifecycleV1> collisions_;
 std::unique_ptr<CharacterAnimationEventOwnerV1> player_events_;
 std::unique_ptr<NpcAnimationEventOwnerV1> npc_events_;
 std::shared_ptr<void> fx_pin_;
 std::int32_t lag_{};
 std::uintptr_t selected_{};
 std::uint32_t root_{UINT32_MAX};
 std::uintptr_t visual_{};
 const scene::Scene* scene_{};
 AnimationFloorBorrowV1 cached_floor_{};
 std::vector<dh2::navigation::PathSegment> route_segments_;
 std::vector<std::uint32_t> route_ids_;
 bool fail(const char* text){if(r_.error.empty())r_.error=text;return false;}
 [[noreturn]] void unavailable(const char* text){fail(text);throw std::runtime_error(r_.error);}
 State& state(){return r_.actor->machine->state();}
 actor::BlendedPlayback& playback(){if(!r_.visual||!r_.visual->animator())unavailable("Required actual canonical CharAnimator");return r_.visual->animator()->playback();}
 bool constant(const char* group,const char* name,std::int32_t& out){const auto* d=r_.design.design();return d&&d->lookup&&d->lookup(d->context,0,group,name,&out)==0;}
 bool controller(ControllerCommandState32*& out){ControllerCommandState32 projection;
  if(!r_.services.controller||!r_.services.controller(projection,r_,r_.error)||!r_.actor->controller)return false;
  out=r_.actor->controller->command_state(projection.global_blocked);return out&&out->owner==r_.actor->object->identity;
 }
 bool position(std::uintptr_t id,const float*& out){WorldTargetActorBorrowV1 actual;
  if(!r_.services.world_targets||r_.services.world_targets->actor(id,&actual)||!actual.position||!actual.target_node)return fail("Required canonical target-position receiver");
  if(*actual.target_node&&(!actual.target_enabled||(*actual.target_enabled&&!actual.cached_target_position)))return fail("Required canonical target-node position producer");
  out=dh2_world_target_position_v1(actual.position,actual.cached_target_position,*actual.target_node,actual.target_enabled?*actual.target_enabled:0);return out!=nullptr;
 }
 bool geometry(){
  if(geometry_)return true;
  if(!r_.services.world_targets||!r_.design.ai()||!r_.services.debug||!r_.services.debug_files||!objects_)return fail("Required SAME canonical attack geometry producers");
  WorldAttackGeometryServicesV1 services;services.context=this;
  services.inventory=[](void* raw,std::uintptr_t id,WorldAttackInventoryBorrowV1* out){auto& t=*static_cast<CampaignFsmV101*>(raw);SourceCampaignCharacterBorrowV62 actual;
   if(!out||!borrow_source_campaign_character_v62(t.r_.services.world,id,actual,t.r_.error)||!actual.character->inventory37c)return -1;
   out->owned=actual.character->inventory37c;return 0;};
  services.object_kind=[](void* raw,std::uintptr_t id,std::int32_t* out){auto& t=*static_cast<CampaignFsmV101*>(raw);if(!out)return -1;std::int32_t key{};const dh2::world::CanonicalObjectBorrowV1* object{};
   bool found=t.objects_->source_ordered_begin_v38(key,object);while(found){if(object&&object->identity==id&&object->type_f4){*out=static_cast<std::int32_t>(*object->type_f4);return 0;}found=t.objects_->source_ordered_next_v38(key,key,object);}return -1;};
  geometry_=std::make_unique<CharacterWorldAttackGeometryV1>(*r_.services.world_targets,*r_.design.ai(),*r_.services.debug,*r_.services.debug_files,services);return true;
 }
 static bool attack_online(void* raw,bool& online,std::string& error){
  auto& t=*static_cast<CampaignFsmV101*>(raw);
  if(!t.r_.services.online_byte5){error="Required actual same-World online byte+5";return false;}
  return t.r_.services.online_byte5(online,error);
 }
 static bool attack_melee_radius(void* raw,std::uintptr_t id,float& radius,std::string& error){
  auto& t=*static_cast<CampaignFsmV101*>(raw);
  return t.geometry()&&t.geometry_->melee_radius(id,radius,error);
 }
 static int attack_backend(void* raw,const AttackRequest32* q,AttackResponse16* out,
  const dh2_script_callback_scope*,std::string& error){
  auto& t=*static_cast<CampaignFsmV101*>(raw);
  if(!q||!out||!t.r_.actor||!t.r_.actor->object){error="Malformed same-Character NPC attack request";return -1;}
  switch(q->service){
   case attack_frontal_angle:{
    std::int32_t angle{};
    if(!t.constant("CharacterDesign","Attack_FrontalAngle",angle)){
     error="Required actual CharacterDesign/Attack_FrontalAngle";return -1;
    }
    std::memcpy(&out->word,&angle,sizeof(angle));return 0;
   }
   case attack_diagnostic:{
    const char* key=nullptr;
    switch(q->argument0){
     case 0x3d0294:case 0x3d03c0:key="isTracingCharAICommands";break;
     case 0x3d0354:case 0x3d0630:case 0x3d06b4:case 0x3d0538:key="isTracingChar_MeleePotentialTarget";break;
     default:error="Unknown original NPC attack diagnostic call site";return -1;
    }
    if(!t.r_.services.debug||!t.r_.services.debug_files||
       dh2_character_debug_load(t.r_.services.debug,t.r_.services.debug_files)!=1||
       dh2_character_debug_get(&out->word,t.r_.services.debug,key,t.r_.services.debug_files)!=1){
     error="Required same-World NPC attack DebugSwitches query";return -1;
    }
    return 0;
   }
   case attack_range_redirect:
    error="Required same-Character NPC AI_DoRangeAttack owner";return -1;
   case attack_network_send:
    error="Required same-World CMsgControllerAction network send owner";return -1;
   default:
    error="Required same-Character NPC attack continuation service "+std::to_string(q->service);return -1;
  }
 }
 bool ensure_npc_attack(){
  ControllerCommandState32* command{};
  if(!controller(command)||!geometry()||!r_.actor->machine||!r_.services.world_targets||!r_.actor->object)
   return fail("Required same-Character NPC attack World/Target/FSM/controller/geometry");
  auto& actor=*r_.actor;const auto id=actor.object->identity;
  auto* heading=actor.source_heading_enabled412_v101();auto* click=actor.source_click_fields_v101();
  if(!heading||!click)return fail("Required actual Character412/413 attack AI fields");
  actor.animation_ai.owner=id;actor.animation_ai.controller=command->controller;
  actor.animation_ai.target=actor.object->target.target;actor.animation_ai.look_target=actor.object->target.target;
  actor.animation_ai.owner_flags=state().flags;actor.animation_ai.seeking=*heading;
  actor.animation_ai.target_sticky=click->click_target413;
  if(!id||command->owner!=id||!actor.object->binding.state||!actor.object->binding.state->owner||
     actor.object->binding.state->owner->identity!=id||actor.machine->native_fsm().character!=id||
     actor.animation_ai.owner!=id)
   return fail("NPC attack requires identical same-Character World/Target/controller/FSM/AI owners");
  command->locked=state().controller_locked;
  if(npc_attack_&&npc_attack_controller_==command)return true;
  npc_attack_.reset();npc_attack_controller_=nullptr;
  NpcAttackCommandBorrowV1 borrow{r_.services.world_targets.get(),&actor.object->binding,
   actor.machine.get(),command,&actor.animation_ai,&actor.source_ooi14a4,nullptr};
  NpcAttackCommandServicesV1 services{this,attack_online,attack_backend,geometry_->queries(),attack_melee_radius};
  try{npc_attack_=std::make_unique<NpcAttackCommandOwnerV1>(borrow,services);}
  catch(const std::exception& e){return fail(e.what());}
  npc_attack_controller_=command;return true;
 }
 bool command_attack(std::uintptr_t requested){
  if(!ensure_npc_attack())return false;
  const auto* scope=r_.player_script_owner_v62
   ?r_.player_script_owner_v62->session().current_skill_callback_scope()
   :r_.actor->session?r_.actor->session->current_skill_callback_scope():nullptr;
  if(npc_attack_->command(requested,scope)==0)return true;
  if(!npc_attack_->error().empty())r_.error=npc_attack_->error();
  else fail("Canonical same-Character NPC attack command failed");
  return false;
 }
 bool selected(){auto& a=*r_.actor;ScriptSessionView v;
  const bool exists=r_.player_script_owner_v62?r_.player_script_owner_v62->session().owner().active(v):a.session&&a.session->owner().active(v);
  const auto previous=a.ai_events.active;const auto* published=a.ai_events.ais_virtuals;
  a.ai_events.active=exists?v.identity:0;
  //A real same-selected OnDied delivery can temporarily publish its scoped
  //native callable wrapper. Reentry must preserve that captured receiver.
  a.ai_events.ais_virtuals=exists?(previous==v.identity&&published?published:character_script_source_virtuals_v101(v.kind)):nullptr;selected_=a.ai_events.active;
  ControllerCommandState32* command{};if(!controller(command))return fail("Required SAME source controller at Character event");
  a.ai_owner.controller=command->controller;a.ai_owner.forced=command->forced;a.ai_owner.locked=command->locked;a.ai_events.global_blocked=command->global_blocked;
  return !exists||a.ai_events.ais_virtuals;
 }
 bool refresh(int id){
  auto& a=*r_.actor;auto& f=a.facts;auto& s=state();
  effects_v115_.fsm=&a.machine->native_fsm(); //published only after genuine machine C1; never dereferenced during service construction
  s.body_present=a.position_fields_v7().physical2dc!=0;s.heading_active=a.runtime.controller.heading.active;
  ControllerCommandState32* command{};if(!controller(command))return false;s.controller_locked=command->locked;
  f.target=a.object->target.target;std::copy_n(a.runtime.controller.heading.direction,3,f.heading);
  if(id==17){
   const auto tables=r_.services.animation_tables;const auto stay=a.source_bool_field(0x13e4);
   if(!tables||!stay)return fail("Required actual PreSpawn CharAnimTable/stay13e4");
   if(pre_spawn_rows_v108_.empty()){
    for(const auto& row:tables->characters){std::array<std::int32_t,40> projected;projected.fill(INT32_MIN);
     std::size_t word=1;for(std::size_t field=0;field<row.fields.size();++field){
      //Source row160 contains a dispatch tag plus two eight-byte array
      //headers. Their pointer words are not consumed by this body. Poison
      //unread native projection cells; publish only actual scalar words.
      if(field==15||field==31){word+=2;continue;}
      if(word>=projected.size()||row.fields[field].size()!=1)return fail("Required scalar CharAnim field for PreSpawn source row");
      projected[word++]=row.fields[field][0];
     }
     if(word!=40)return fail("Invalid source CharAnim160 layout");pre_spawn_rows_v108_.push_back(projected);
    }
   }
   pre_spawn_={&s,a.object->identity,reinterpret_cast<const std::int32_t(*)[40]>(pre_spawn_rows_v108_.data()),
    static_cast<std::uint32_t>(pre_spawn_rows_v108_.size()),*stay,static_cast<std::uint32_t>(a.source_ai_group_role38),0,
    r_.ai_group_v87?reinterpret_cast<std::uint32_t*>(r_.ai_group_v87->saved_fields().field24):nullptr};
   return true;
  }
  if(id!=3&&id!=4&&id!=5)return true;
  if(id==3&&s.idle_suppressed)return true; // Original byte538 early return.
  bool player{};if(!r_.is_player(player,r_.error))return false;f.is_player=player;
  const auto* tables=r_.services.animation_tables;if(!tables)return fail("Required actual CharAnimTable");
  const auto table=dh2_character_animation_table_id(r_.properties->resolved[2],static_cast<int>(tables->characters.size()));
  if(table<0||std::size_t(table)>=tables->characters.size())return fail("Required authored CharAnimTable fallback17");
  auto sequence=[&](const char* name,int& out){auto at=std::find(tables->state_names.begin(),tables->state_names.end(),name);
   if(at==tables->state_names.end())return false;const auto& field=tables->characters[table].fields[at-tables->state_names.begin()];if(field.size()!=1)return false;out=field[0];return true;};
  std::int32_t mask{},count{};if(!constant("AnimStancedAnim","SL__LIST_IPHONE",mask)||!constant("AnimStances","COUNT_IPHONE",count))return fail("Required actual animation stance constants");
  f.stance_mask=static_cast<unsigned>(mask);f.stance=0;
  if(player){StanceFacts16 facts;if(!r_.prepared_equipment_v60||!r_.prepared_equipment_v60->stance_facts(true,count,facts,r_.error)||dh2_character_anim_stance(&f.stance,&facts)!=1)return false;
   f.has_ranged_weapon=(facts.predicates&(stance_has_bow|stance_has_staff))!=0;
  }
  if(id==3)return sequence("Idle",f.idle)||fail("Required actual Idle sequence");
  if(id==4){
   if(!sequence("Walk",f.walk)||!sequence("Run",f.run))return fail("Required actual Walk/Run sequences");
   dh2::move::Speed speed;const float one=1;if(dh2_move_speed(&speed,r_.properties->resolved.data(),&one))return false;f.walk_speed=speed.walk_multiplier;
   const int destination=dh2_nav_is_at_destination(&a.runtime.controller,&a.runtime.path);if(destination<0)return false;f.is_at_destination=destination;f.following_path=a.runtime.path.count!=0;
   if(player){if(!r_.services.state_thresholds_v101||!r_.services.state_thresholds_v101(f.walk_threshold,f.run_threshold,r_.error))return false;}
  }else{
   if(!sequence("Attack",f.attack_moving)||!sequence("AttackStatic",f.attack_static))return fail("Required actual Attack sequences");
   if(dh2_character_attack_speed(&f.attack_speed,r_.properties->resolved.data())!=1)return false;
   const auto* ai=dh2::data::ai_props(*r_.design.ai(),r_.properties->resolved[1]);if(!ai)return false;f.attack_delay=static_cast<unsigned>(ai->attack_delay);
  }return true;
 }
 bool animation(int sequence){
  if(sequence==-1){sequence=state().animation_override;state().animation_override=-1;}
  if(!r_.visual||!r_.visual->animator()||!r_.services.animation_tables||!r_.services.random)return fail("Required SAME animation/table/Random owner");
  playback().selection_policy_v126=animation_debug_v126_.services();
  auto& source=*r_.services.random;data::AnimationRandom random{source.seed,source.calls};auto* previous=r_.animation_random_inflight_v101;r_.animation_random_inflight_v101=&random;
  struct Commit{Record& r;data::AnimationRandom& random;data::AnimationRandom* previous;~Commit(){r.services.random->seed=random.seed;r.services.random->calls=random.calls;r.animation_random_inflight_v101=previous;}}commit{r_,random,previous};
  if(!r_.visual->animator()->start(*r_.services.animation_tables,sequence,random,1.f,r_.error))return false;state().current_animation=sequence;return true;
 }
 bool raise(unsigned event,std::uintptr_t payload){
  auto* heading=r_.actor->source_heading_enabled412_v101();if(!heading)return fail("Required produced Character412 for source event");
  if(!selected())return false;r_.actor->ai_events.seeking=*heading;AIEventResult16 result;const AIEventPayload24 source{payload,event==0x35?0x3db288u:0u,0,0};
  const int status=dh2_character_ai_event(&result,&r_.actor->ai_events,event,&source,&events_);
  //Only the actual SetSeeking event owns this store; nested Focus/UseOOI
  //callbacks can publish412 independently during every other event.
  if(event==0x32)*heading=static_cast<std::uint8_t>(r_.actor->ai_events.seeking);
  r_.actor->ai_events.seeking=*heading;
  if(status){
   if(r_.error.empty())r_.error=std::string("Required canonical Character/CharAI event continuation: event ")+
    std::to_string(event)+" status "+std::to_string(status)+" phase "+std::to_string(result.phase)+
    " service "+std::to_string(result.last_service)+" calls "+std::to_string(result.service_calls);
   return false;
  }
  return true;
 }
 PlayerInjureServicesV7 injury_services_v115(){
  PlayerInjureServicesV7 source;source.context=this;
  source.is_player=[](void* raw,std::uintptr_t id,bool* out){auto& t=*static_cast<CampaignFsmV101*>(raw);return out&&id==t.r_.actor->object->identity&&t.r_.is_player(*out,t.r_.error)?0:-1;};
  source.animation_table=[](void* raw,std::uintptr_t id,int* out){auto& t=*static_cast<CampaignFsmV101*>(raw);if(!out||id!=t.r_.actor->object->identity||!t.r_.services.animation_tables)return -1;*out=dh2_character_animation_table_id(t.r_.properties->resolved[2],t.r_.services.animation_tables->characters.size());return 0;};
  source.injure_animation=[](void* raw,int table,bool* found,int* out){auto& t=*static_cast<CampaignFsmV101*>(raw);if(!found||!out||!t.r_.services.animation_tables)return -1;const auto& rows=t.r_.services.animation_tables->characters;*found=table>=0&&std::size_t(table)<rows.size();if(!*found)return 0;const auto& field=rows[table].fields[14];if(field.size()!=1)return -1;*out=field[0];return 0;};
  source.constant=[](void* raw,const char* group,const char* name,int* out){return out&&static_cast<CampaignFsmV101*>(raw)->constant(group,name,*out)?0:-1;};
  source.stance=[](void* raw,std::uintptr_t id,int* out){auto& t=*static_cast<CampaignFsmV101*>(raw);bool player{};int count{};if(!out||id!=t.r_.actor->object->identity||!t.r_.is_player(player,t.r_.error)||!t.constant("AnimStances","COUNT_IPHONE",count))return -1;if(!player){*out=0;return 0;}StanceFacts16 facts;return t.r_.prepared_equipment_v60&&t.r_.prepared_equipment_v60->stance_facts(true,count,facts,t.r_.error)&&dh2_character_anim_stance(out,&facts)==1?0:-1;};
  source.event=[](void* raw,int event,std::uintptr_t payload){return static_cast<CampaignFsmV101*>(raw)->r_.actor->machine->event(event,payload)>=0?0:-1;};
  source.transition=[](void* raw,int next,int event,std::uintptr_t payload){return static_cast<CampaignFsmV101*>(raw)->r_.actor->machine->transition(next,event,payload)>=0?0:-1;};
  source.debug=[](void* raw,const char* name){auto& t=*static_cast<CampaignFsmV101*>(raw);unsigned ignored{};return dh2_character_debug_load(t.r_.services.debug,t.r_.services.debug_files)==1&&dh2_character_debug_get(&ignored,t.r_.services.debug,name,t.r_.services.debug_files)==1?0:-1;};
  source.set_animation=[](void* raw,int sequence){return static_cast<CampaignFsmV101*>(raw)->animation(sequence)?0:-1;};
  source.cancel_sneaking=[](void* raw,std::uintptr_t id){auto& t=*static_cast<CampaignFsmV101*>(raw);return id==t.r_.actor->object->identity&&t.cancel_sneaking()?0:-1;};
  source.look_at_source_408=[](void* raw,std::uintptr_t id){auto& t=*static_cast<CampaignFsmV101*>(raw);if(id!=t.r_.actor->object->identity)return -1;return source_campaign_character_command_look_v114(t.r_.services.world,id,t.r_.actor->object->target.target,t.r_.error)?0:-1;};return source;
 }
 bool injury_borrow_v115(PlayerInjureBorrowV7& out){auto* fields=r_.actor->source_frame_fields_v106();if(!fields)return fail("Required actual Character14fc injury gate");out={r_.actor->object->identity,&state(),&fields->state_fx_delay14fc};return true;}
 bool publish_lock_v115(){ControllerCommandState32* control{};if(!controller(control))return false;control->locked=state().controller_locked;return true;}
 bool reload_lock_v115(){ControllerCommandState32* control{};if(!controller(control))return false;state().controller_locked=control->locked;return true;}
 KnockbackReactionServicesV1 knockback_services_v115(){const auto common=injury_services_v115();KnockbackReactionServicesV1 source;source.context=this;
  source.constant=common.constant;source.stance=common.stance;source.event=common.event;source.transition=common.transition;source.debug=common.debug;source.animation=common.set_animation;
  source.filter=[](void* raw,int group,int category,int mask,bool secondary){auto& t=*static_cast<CampaignFsmV101*>(raw);if(!t.publish_lock_v115()||secondary||!t.r_.physical_owner_v62||t.r_.actor->position_fields_v7().physical2dc!=reinterpret_cast<std::uintptr_t>(t.r_.physical_owner_v62.get()))return -1;return t.r_.physical_owner_v62->source_death_filter_v84(static_cast<std::int16_t>(group),static_cast<std::uint16_t>(category),static_cast<std::uint16_t>(mask))?0:-1;};
  source.reset_filter=[](void* raw){auto& t=*static_cast<CampaignFsmV101*>(raw);return t.publish_lock_v115()&&t.r_.physical_owner_v62&&t.r_.actor->position_fields_v7().physical2dc==reinterpret_cast<std::uintptr_t>(t.r_.physical_owner_v62.get())&&t.r_.physical_owner_v62->source_reset_filter_v84()?0:-1;};
  source.pin=[](void* raw){auto& t=*static_cast<CampaignFsmV101*>(raw);return t.publish_lock_v115()&&t.pin(true)?0:-1;};source.unpin=[](void* raw){auto& t=*static_cast<CampaignFsmV101*>(raw);return t.publish_lock_v115()&&t.pin(false)?0:-1;};
  source.look_at=[](void* raw,std::uintptr_t target){auto& t=*static_cast<CampaignFsmV101*>(raw);if(!t.publish_lock_v115())return -1;const bool okay=source_campaign_character_command_look_v114(t.r_.services.world,t.r_.actor->object->identity,target,t.r_.error);t.reload_lock_v115();return okay?0:-1;};
  source.cancel_sneaking=[](void* raw){auto& t=*static_cast<CampaignFsmV101*>(raw);if(!t.publish_lock_v115())return -1;const bool okay=t.cancel_sneaking();t.reload_lock_v115();return okay?0:-1;};
  source.is_dead=[](void* raw,bool* out){auto& t=*static_cast<CampaignFsmV101*>(raw);if(!out||!t.r_.life)return -1;*out=t.r_.life->dead!=0;return 0;};
  source.stop_animation=[](void* raw){auto& t=*static_cast<CampaignFsmV101*>(raw);return t.playback().stop_immediate_v1(true,t.r_.error)?0:-1;};
  source.set_dead=[](void* raw,bool mode,std::uintptr_t payload,bool force){auto& t=*static_cast<CampaignFsmV101*>(raw);return t.r_.death_v84&&t.r_.death_v84->source_set_dead_state_v115(mode,payload,force,t.r_.error)?0:-1;};return source;
 }
 KnockbackReactionBorrowV1 knockback_borrow_v115(){return {&r_.actor->machine->native_fsm(),&r_.view,r_.design.ai(),r_.services.animation_tables};}
 static int effect_service_v115(void* raw,NativeEffects32* actual,const NativeEffectRequest32* q,unsigned* out){auto& t=*static_cast<CampaignFsmV101*>(raw);if(!q||!out||actual!=&t.effects_v115_||q->character!=t.r_.actor->object->identity||!t.publish_lock_v115())return -1;
  struct Reload{CampaignFsmV101& owner;~Reload(){owner.reload_lock_v115();}}reload{t};
  switch(q->service){
   case effect_ai_flags:{const auto* row=t.r_.design.ai()?dh2::data::ai_props(*t.r_.design.ai(),t.r_.properties->resolved[1]):nullptr;if(!row)return -1;*out=row->flags;return 0;}
   case effect_animation_index:*out=static_cast<unsigned>(t.r_.properties->resolved[2]);return 0;
   case effect_start_timer:{int timer{};if(t.r_.player_script_owner_v62)timer=t.r_.player_script_owner_v62->session().start_timer(q->argument0,static_cast<int>(q->argument1),static_cast<int>(q->argument2));else if(t.r_.actor->session)timer=dh2_character_timer_start(&t.r_.actor->session->timers(),q->argument0,static_cast<int>(q->argument1),static_cast<int>(q->argument2),q->payload,&t.r_.actor->session->native_timer_services());else return -1;if(timer< -1)return -1;*out=static_cast<unsigned>(timer);return 0;}
   case effect_stance_bits:{int value{};if(!t.constant("AnimStancedAnim","SL__LIST_IPHONE",value))return -1;*out=value;return 0;}
   case effect_anim_stance:{const auto source=t.injury_services_v115();int value{};if(source.stance(source.context,q->character,&value))return -1;*out=value;return 0;}
   case effect_force_state:return t.r_.actor->machine->transition(static_cast<int>(q->argument0),static_cast<int>(q->argument1),q->payload)<0?-1:0;
   case effect_state_event:return t.r_.actor->machine->event(static_cast<int>(q->argument0),q->payload)<0?-1:0;
   case effect_set_animation:return t.animation(static_cast<int>(q->argument0))?0:-1;
   case effect_cancel_sneaking:return t.cancel_sneaking()?0:-1;
   case effect_pin:return t.pin(true)?0:-1;case effect_unpin:return t.pin(false)?0:-1;
   case effect_is_player:{bool player{};if(!t.r_.is_player(player,t.r_.error))return -1;*out=player;return 0;}
   case effect_stop_loop:t.playback().stop_loop(false);return 0;
   case effect_random:if(!t.r_.services.random||q->argument0>INT32_MAX)return -1;*out=dh2_animation_random(&t.r_.services.random->seed,&t.r_.services.random->calls,q->argument0);return 0;
   case effect_heading_point:{float direction[3];const unsigned bits[]{q->argument0,q->argument1,q->argument2};std::memcpy(direction,bits,12);return t.heading_v115(direction,1,0)?0:-1;}
   case effect_heading_object:return t.heading_v115(nullptr,2,q->payload)?0:-1;
   default:return -1;
  }
 }
 int skill_dispatch(unsigned operation,unsigned index,unsigned value,std::uintptr_t payload){
  auto* heading=r_.actor->source_heading_enabled412_v101();if(!heading)return fail("Required produced Character412 for SkillState"),-1;
  skill_.heading_enabled=*heading;
  const int result=character_skill_state_target_bound_v46(skill_,r_.actor->object->target,operation,index,value,payload,0,skill_services_);
  *heading=skill_.heading_enabled;return result;
 }
 int npc_skill_ai_v115(unsigned operation,unsigned index,unsigned& result){
  if(!r_.npc_skills_v84||!r_.actor->session)return fail("Required SAME NPC skill owner/Session"),-1;
  auto* fields=r_.npc_skills_v84->source_ai_fields_v115();if(!fields)return fail("Required observed NPC CharAI CC/D0/D1 source fields"),-1;
  r_.skill_owner_view_v68.character=r_.actor->object->identity;r_.skill_owner_view_v68.flags=state().flags;
  SkillAIContextV3 source{&r_.skill_owner_view_v68,const_cast<State40*>(&r_.npc_skills_v84->owner().state()),fields,r_.actor->session->owner().lifecycle().load_step,0};
  const SkillAIServices16V3 services{this,[](void* raw,SkillAIContextV3* actual,const SkillAIRequest32V3* q,SkillAIResponse32V3* out){auto& t=*static_cast<CampaignFsmV101*>(raw);if(!q||!out||!actual||q->character!=t.r_.actor->object->identity)return -1;
   switch(q->operation){
    case skill_ai_using_v3:case skill_ai_casting_v3:{int current{};if(dh2_character_native_fsm_get_integer(&current,&t.r_.actor->machine->native_fsm(),0)!=1)return -1;out->word=current==(q->operation==skill_ai_using_v3?6:7);return 0;}
    case skill_ai_row_v3:{t.skill_.state=&t.state();t.skill_.character=q->character;SkillStateRequest32V4 request{skill_state_row_v4,0,q->index,0,0,0};SkillStateResponse16V4 response;if(t.skill_operation(&t.skill_,request,response))return -1;out->row=reinterpret_cast<const dh2::data::SkillProjection76*>(response.identity);return 0;}
    case skill_ai_callback_v3:{unsigned value{};const int status=skill_callback_session_v3(*t.r_.actor->session,reinterpret_cast<const Instance32*>(q->subject),q->value,&value,t.r_.error);out->word=value;return status;}
    case skill_ai_set_state_v3:t.skill_.state=&t.state();t.skill_.character=q->character;t.skill_.physical=t.r_.actor->position_fields_v7().physical2dc;return t.skill_dispatch(skill_state_select_v4,q->index,q->value,0);
    case skill_ai_player_v3:{bool player{};if(!t.r_.is_player(player,t.r_.error))return -1;out->word=player;return 0;}
    case skill_ai_stop_loop_v3:t.playback().stop_loop(q->value!=0);return 0;
    default:return t.fail("Required positive NPC SkillAI source service"),-1;
   }
  }};
  return dh2_character_skill_ai_v3(&result,&source,operation,index,&services);
 }
 bool stop(){auto& a=*r_.actor;
  if(dh2_nav_drop_path(&a.runtime.path))return false;
  std::copy_n(a.runtime.subobjects.position,3,a.runtime.controller.destination);
  a.runtime.controller.heading.active=0;a.runtime.controller.path_requested=0;
  std::copy_n(a.runtime.subobjects.position,3,a.runtime.subobjects.destination);
  const auto& origin=dh2::world::canonical_vec3_origin_v1();std::copy(origin.begin(),origin.end(),a.runtime.controller.heading.direction);state().heading_active=0;
  const auto physical=a.position_fields_v7().physical2dc;if(!physical)return true;
  if(!r_.physical_owner_v62||physical!=reinterpret_cast<std::uintptr_t>(r_.physical_owner_v62.get()))return fail("Required SAME physical2dc Stop receiver");
  if(!(state().flags&2))return true;return dh2_native_body_stop(&r_.physical_owner_v62->native(),a.runtime.subobjects.position)==0;
 }
 bool pin(bool pinned,std::uintptr_t captured=0){auto id=r_.actor->position_fields_v7().physical2dc;if(captured&&id!=captured)return fail("Captured physical2dc changed before source pin");
  if(!id)return fail("Required positive physical2dc pin receiver");if(!r_.physical_owner_v62||id!=reinterpret_cast<std::uintptr_t>(r_.physical_owner_v62.get()))return false;
  return (pinned?dh2_native_body_pin(&r_.physical_owner_v62->native()):dh2_native_body_unpin(&r_.physical_owner_v62->native()))==0;
 }
 bool look(std::uintptr_t target){if(!target)return true;const float* p{};if(!position(target,p))return false;
  LookAtState16 source{{r_.actor->runtime.subobjects.position[0],r_.actor->runtime.subobjects.position[1],r_.actor->runtime.subobjects.position[2]},r_.actor->runtime.rotation.heading_angle};
  if(dh2_character_look_at_point(&source,p))return false;r_.actor->runtime.rotation.heading_angle=source.heading_angle;return true;
 }
 bool remote(bool& out){const auto* raw=r_.actor->source_bool_field(0x118);if(!raw)return fail("Required source ObjectBase remotely-updated118");out=r_.actor->machine->combat_fields().network_id!=-1||*raw;return true;}
 bool move(const float* point,bool command_gate=true){
  if(command_gate){ControllerCommandState32* control{};if(!controller(control))return false;
   if(!control->forced&&(control->global_blocked||control->locked))return true;}
  bool remotely_updated{};if(!remote(remotely_updated))return false;if(remotely_updated)return true;
  dh2::world::GameObjectInitializationFieldsV62 fields;if(!r_.actor->inherited_initialization_fields_v62(r_.actor,fields,r_.error))return false;
  auto* stat=fields.byte?fields.byte(0x84):nullptr;auto* limit=fields.integer?fields.integer(0x26c):nullptr;
  if(!stat||!limit)return fail("Required source PathTo static84/limit26c cells");auto& path=r_.actor->runtime.path;
  PathToState40 value{r_.actor->object->identity,*stat,path.count!=0,static_cast<unsigned>(*limit),0,{path.target[0],path.target[1],path.target[2]},0};
  const PathToServices16 callbacks{this,[](void* raw,const PathToRequest32* q,std::uint32_t* out){auto& t=*static_cast<CampaignFsmV101*>(raw);if(!q||!out||q->owner!=t.r_.actor->object->identity)return -1;
   if(!t.floors_||!t.floors_->sewn)return t.fail("Required actual sewn floor graph at Character.FindPath"),-1;
   auto& runtime=t.r_.actor->runtime;if(runtime.object.user!=q->owner){t.fail("Required actual InitPFObject before source FindPath");return -1;}
   const auto capacity=std::uint64_t(t.floors_->graph.node_count)+1;if(capacity>65536){t.fail("Canonical route storage admission exceeded");return -1;}
   if(!runtime.path.segments){if(runtime.path.count)return -1;t.route_segments_.resize(capacity);runtime.path.segments=t.route_segments_.data();runtime.path.capacity=static_cast<unsigned>(capacity);}
   t.route_ids_.resize(capacity);dh2::navigation::RouteResult result{};result.search.path=t.route_ids_.data();result.search.path_capacity=static_cast<unsigned>(capacity);
   runtime.path.route.flags=runtime.object.motion.flags;runtime.path.route.radius=runtime.object.radius;std::copy_n(runtime.subobjects.position,3,runtime.path.position);
   dh2::navigation::FindRequest request{&t.floors_->route_world,&t.floors_->collision_world,&runtime.path,&result,&t.floors_->route_workspace,{q->target[0],q->target[1],q->target[2]},q->limit,0,0};
   const dh2::navigation::FindSourceServicesV1 source{&t,[](void* raw,std::uint32_t* out){auto& t=*static_cast<CampaignFsmV101*>(raw);return out&&t.r_.services.path_policy_v101&&t.r_.services.path_policy_v101(*out,t.r_.error)?0:-1;}};
   const int status=dh2_nav_find_path_source_v1(&request,&source);if(status){t.r_.error="Canonical FindPath status "+std::to_string(status)+"; "+t.r_.error;return -1;}*out=result.found;return 0;}};
  PathToResult16 result;return dh2_character_path_to(&result,&value,point,&callbacks)==0;
 }
 bool click_point_v120(const float* point,bool released){
  if(!point)return fail("Required actual Ctrl_Click floor point");
  // Ctrl_Click3addc8 dispatches +0xe4 on press to Ctrl_HeadTo(Point)3adac8
  // and +0xec on release to Ctrl_MoveTo(Point)3ada30. Preserve the recovered
  // HeadTo body order: remote gate, heading vector, canonical destination,
  // then Character event0. The release continuation is remote then PathTo.
  if(released)return move(point,false); // Ctrl_MoveTo(Point): remote then PathTo, no Cmd_* gate.
  bool remotely_updated{};if(!remote(remotely_updated))return false;if(remotely_updated)return true;
  auto& actor=*r_.actor;const float* origin=actor.runtime.subobjects.position;
  const float direction[3]{point[0]-origin[0],point[1]-origin[1],point[2]-origin[2]};
  dh2::navigation::set_heading_unchecked(actor.runtime.controller.heading,direction,1);
  actor.runtime.rotation.heading_angle=actor.runtime.controller.heading.angle;
  state().heading_active=actor.runtime.controller.heading.active;
  // GameObject+1a8 is controller.destination. Character frame projection
  // refreshes subobjects.destination from this canonical source cell.
  std::copy_n(point,3,actor.runtime.controller.destination);
  return raise(0,0); // SetHeadingDirection, SetDestination, Character.RaiseEvent(0).
 }
 int timer(unsigned duration,int repeat,int event){if(r_.player_script_owner_v62)return r_.player_script_owner_v62->session().start_timer(duration,repeat,event)>=0?0:-1;
  if(r_.actor->session)return dh2_character_timer_start(&r_.actor->session->timers(),duration,repeat,event,0,&r_.actor->session->native_timer_services())>=0?0:-1;return -1;
 }
 bool cancel_sneaking(){if(r_.player_script_owner_v62)return r_.player_script_owner_v62->native_cancel_sneaking(&r_.target_character_v62.interactive415)==0;
  sneaking::Character48 value{};value.identity=r_.actor->object->identity;value.resolved={r_.view.resolved,224,0};value.changed415=r_.target_character_v62.interactive415;
  const sneaking::Services16 source{this,[](void* raw,const sneaking::Request24* q,std::uint32_t* out){auto& t=*static_cast<CampaignFsmV101*>(raw);if(!q||!out||q->operation!=sneaking::is_player)return -1;bool player{};if(!t.r_.is_player(player,t.r_.error))return -1;*out=player;return 0;}};
  if(r_.view.resolved[198]<=0){const auto result=dh2_character_cancel_sneaking(&value,&source);r_.target_character_v62.interactive415=value.changed415;return result==0;}
  if(!r_.services.cancel_sneaking_v101)return fail("Required source NPC active sneaking skill continuation");return r_.services.cancel_sneaking_v101(r_,r_.error);
 }
 int debug_request(const SkillAttackNativeRequestV6* q,std::uintptr_t* out){if(!q||!out)return -1;
  if(q->service==skill_attack_debug_load_v6)return dh2_character_debug_load(r_.services.debug,r_.services.debug_files)==1?0:-1;
  if(q->service==skill_attack_string_construct_v6){if(!q->name)return -1;auto text=std::make_unique<std::string>(q->name);const auto id=reinterpret_cast<std::uintptr_t>(text.get());debug_strings_.emplace(id,std::move(text));*out=id;return 0;}
  auto at=debug_strings_.find(q->subject);if(at==debug_strings_.end())return -1;
  if(q->service==skill_attack_string_destroy_v6){debug_strings_.erase(at);return 0;}
  if(q->service==skill_attack_debug_get_v6){unsigned value{};if(dh2_character_debug_get(&value,r_.services.debug,at->second->c_str(),r_.services.debug_files)!=1)return -1;*out=value;return 0;}return -1;
 }
 void body(State* actual,const Request& q){if(actual!=&state())unavailable("Wrong canonical State body receiver");bool okay=false;
  switch(q.service){
   case dh2::character::stop:okay=stop();break;case dh2::character::pin:okay=pin(true);break;case dh2::character::unpin:okay=pin(false);break;
   case set_animation:okay=animation(q.argument[0]);break;case set_speed:okay=playback().set_speed(q.scalar,r_.error);break;
   case stop_loop:playback().stop_loop(false);okay=true;break;
   case look_at:{ControllerCommandState32* command{};if(q.argument[0]==1){okay=controller(command);if(okay&&!command->forced&&(command->global_blocked||command->locked))break;}okay=look(q.identity);break;}
   case start_timer:okay=timer(static_cast<unsigned>(q.argument[0]),q.argument[1],q.argument[2])==0;break;
   case dh2::character::cancel_sneaking:okay=cancel_sneaking();break;
   case raise_event:okay=raise(static_cast<unsigned>(q.argument[0]),q.identity);break;
   case idle_common_update:okay=idle();break;
   default:if(r_.services.state_body_v101)okay=r_.services.state_body_v101(r_,q,r_.error);break;
  }
  if(!okay)unavailable("Required canonical state body continuation");
 }
 bool idle();
 bool animation_consumer(unsigned operation);
 void publish_animation_flags(){auto& a=*r_.actor;a.predicates.can_interrupt=static_cast<std::uint8_t>(a.animation_ai.attack_last);state().stop_attack_allowed=a.animation_ai.attack_finisher;
  if(r_.player_script_owner_v62){auto& fields=r_.player_script_owner_v62->skill_ai();fields.continued=static_cast<std::uint8_t>(a.animation_ai.skill_started);fields.last=static_cast<std::uint8_t>(a.animation_ai.skill_stop_requested);}}
 void reload_animation_flags(){auto& a=*r_.actor;if(auto* field=a.source_heading_enabled412_v101())a.animation_ai.seeking=*field;if(auto* click=a.source_click_fields_v101())a.animation_ai.target_sticky=click->click_target413;
  if(r_.player_script_owner_v62){const auto& fields=r_.player_script_owner_v62->skill_ai();a.animation_ai.skill_started=fields.continued;a.animation_ai.skill_stop_requested=fields.last;}}
 bool relay(const char*);
 int authored(const animation::TriggeredEvent&);
 int skill_operation(SkillStateV4*,const SkillStateRequest32V4&,SkillStateResponse16V4&);
 int spell_callback(unsigned operation){
  if(!r_.player_script_owner_v62||!r_.save||!r_.services.difficulty_global)return fail("Required SAME source Spell/Save/current difficulty"),-1;
  const int difficulty=r_.services.difficulty_global->value();if(difficulty<0||difficulty>2)return -1;
  const int selected=r_.save->current_faery(static_cast<unsigned>(difficulty));const auto& slots=r_.player_script_owner_v62->state().spells;
  if(selected<0||static_cast<unsigned>(selected)>=slots.count||!slots.items||!slots.items[selected])return 0;
  unsigned result{};const auto code=r_.player_script_owner_v62->native_instance_callback_v68(1,static_cast<unsigned>(selected),operation,&result);if(code)r_.error=r_.player_script_owner_v62->error();return code;
 }
 int cast_body(unsigned operation){
  const dh2::android_ui::CastStateServicesV2 services{this,[](void* raw,State* s,const dh2::android_ui::CastStateRequestV2* q){auto& t=*static_cast<CampaignFsmV101*>(raw);if(!q||s!=&t.state()||q->character!=t.r_.actor->object->identity)return -1;
   switch(q->operation){
    case dh2::android_ui::cast_raise_v2:return t.raise(q->value,0)?0:-1;
    case dh2::android_ui::cast_animation_v2:return t.animation(-1)?0:-1;
    case dh2::android_ui::cast_speed_v2:return t.playback().set_speed(1.f,t.r_.error)?0:-1;
    case dh2::android_ui::cast_heading_v2:{auto* heading=t.r_.actor->source_heading_enabled412_v101();if(!heading)return -1;*heading=0;return 0;} //CSCast3c3a6c writes412, not movement1b5.
    case dh2::android_ui::cast_cancel_sneaking_v2:return t.cancel_sneaking()?0:-1;
    default:return -1;
   }}};
  return dh2_faery_cast_body_v2(&state(),r_.actor->object->identity,operation,&services);
 }
 int call(std::uintptr_t identity,const char* name,const dh2_script_value* args,unsigned count){
  ScriptSessionView selected;std::uint32_t lua_error{};
  const auto* scope=r_.player_script_owner_v62?r_.player_script_owner_v62->session().current_skill_callback_scope():r_.actor->session?r_.actor->session->current_skill_callback_scope():nullptr;
  if(r_.player_script_owner_v62){auto& owner=r_.player_script_owner_v62->session().owner();if(!owner.find(identity,selected))return -1;
   if(!scope)return owner.call_discard(identity,name,args,count,lua_error);
  }else{if(!r_.actor->session||!r_.actor->session->owner().find(identity,selected))return -1;if(!scope)return r_.actor->session->owner().call_discard(identity,name,args,count,lua_error);}
  if(scope->vm!=selected.vm||!dh2_script_callback_scope_valid(scope))return -1;
  const auto* alias=dh2_script_alias_resolve(selected.aliases,name);return dh2_script_callback_call_discard_source_objects(scope,alias,args,count)<0?-1:0;
 }
 static void body_service(void* p,State* s,const Request* q){if(!p||!q)throw std::runtime_error("Malformed canonical body request");static_cast<CampaignFsmV101*>(p)->body(s,*q);}
 static int ai_service(void*,AIEventState64*,const AIEventRequest40*,std::uint32_t*);
 static int pre_spawn_service_v108(void* raw,PreSpawnState48* source,const PreSpawnRequest32* q,PreSpawnResponse8* out){
  auto& t=*static_cast<CampaignFsmV101*>(raw);if(!q||!out||q->character!=t.r_.actor->object->identity)return -1;
  auto& r=t.r_;auto& e=r.error;
  if(!source)return -1;
  struct Reload{PreSpawnState48& source;Record& r;~Reload(){
   if(auto stay=r.actor->source_bool_field(0x13e4))source.stay_enabled=*stay;
   source.ai_kind=static_cast<unsigned>(r.actor->source_ai_group_role38);
   source.ai_word=r.ai_group_v87?reinterpret_cast<unsigned*>(r.ai_group_v87->saved_fields().field24):nullptr;
  }}reload{*source,r};
  switch(q->service){
   case pre_spawn_animation_index:{const auto tables=r.services.animation_tables;if(!tables)return -1;const auto id=dh2_character_animation_table_id(r.properties->resolved[2],static_cast<int>(tables->characters.size()));if(id<0)return -1;out->word=static_cast<unsigned>(id);return 0;}
   case pre_spawn_stance_mask:{int value{};if(!t.constant("AnimStancedAnim","SL__LIST_IPHONE",value))return -1;out->word=static_cast<unsigned>(value);return 0;}
   case pre_spawn_stance:{if(!t.refresh(3))return -1;out->word=static_cast<unsigned>(r.actor->facts.stance);return 0;}
   case pre_spawn_set_animation:return t.animation(static_cast<int>(q->argument0))?0:-1;
   case pre_spawn_set_speed:{float speed;std::memcpy(&speed,&q->argument0,4);return t.playback().set_speed(speed,e)?0:-1;}
   case pre_spawn_remove_physical:{
    //Whole SetPhysical(NULL,false) Debug prefix. A true MP_NoPhysics returns
    //before touching an existing body or PF, even with a NULL new argument.
    unsigned no_physics{};
    if(!r.services.debug||!r.services.debug_files||dh2_character_debug_load(r.services.debug,r.services.debug_files)<0||
      dh2_character_debug_get(&no_physics,r.services.debug,"MP_NoPhysics",r.services.debug_files)<0)return -1;
    if(no_physics)return 0;
    if(r.actor->position_fields_v7().physical2dc){if(!r.physical_owner_v62||!r.physical_owner_v62->release()){if(r.physical_owner_v62)e=r.physical_owner_v62->error();return -1;}r.physical_owner_v62.reset();}
    return dh2::world::canonical_character_update_pf_v62(r,*t.pf_geometry_,*t.pf_registry_,e)?0:-1;
   }
   case pre_spawn_enable:return source_campaign_character_set_visible_v96(r.services.world,q->character,q->argument0!=0,e)?0:-1;
   case pre_spawn_revive:return r.revive_v70(0,0,e)?0:-1;
   case pre_spawn_init_physical:return r.initialize_physical(e)?0:-1;
   case pre_spawn_predicate:return source_character_spawn_predicate_v87(r,17,out->word,e)?0:-1;
   case pre_spawn_set_spawn:{if(!source_character_select_spawn_v87(r,q->argument0!=0,q->argument1!=0,e))return -1;return t.refresh(17)?0:-1;}
   case pre_spawn_disable_collisions:case pre_spawn_enable_collisions:{
    AIEventRequest40 request{};request.service=ai_event_helper;request.operation=q->service==pre_spawn_enable_collisions?0x394a3c:0x3949b0;request.subject=q->character;
    unsigned value{};return ai_service(&t,&r.actor->ai_events,&request,&value);
   }
   default:return t.fail("Required original PreSpawn assertion policy/log"),-1;
  }
 }
 static int skill_service(void* p,SkillStateV4* s,const SkillStateRequest32V4* q,SkillStateResponse16V4* out){
  if(!p||!s||!q||!out)return -1;auto& t=*static_cast<CampaignFsmV101*>(p);
  if(t.r_.fsm_context_v101.get()!=p||!t.r_.actor||!t.r_.actor->object||!t.r_.actor->machine||
     s!=&t.skill_||s->state!=&t.state()||s->character!=t.r_.actor->object->identity)return -1;
  auto* heading=t.r_.actor->source_heading_enabled412_v101();if(!heading)return -1;
  *heading=s->heading_enabled;
  struct Reload{SkillStateV4& state;std::uint8_t* source;~Reload(){state.heading_enabled=*source;}}reload{*s,heading};
  return t.skill_operation(s,*q,*out);
 }
 static int skill_ai_loan_service(void* p,SkillAIContextV3* actual,const SkillAIRequest32V3* q,SkillAIResponse32V3* out){
  auto& t=*static_cast<CampaignFsmV101*>(p);
  if(!q||!out||actual!=&t.skill_ai_context_v3_||!t.r_.actor||!t.r_.actor->object||
     t.r_.fsm_context_v101.get()!=p||
     q->character!=t.r_.actor->object->identity||actual->owner!=&t.r_.skill_owner_view_v68||
     !actual->slots||!actual->fields)return -1;
  if(t.r_.player_script_owner_v62){
   auto* instances=t.r_.player_script_owner_v62->native_skill_owner();
   if(!instances||actual->slots!=&instances->state()||actual->fields!=&t.r_.player_script_owner_v62->skill_ai()||
      actual->script_step!=t.r_.player_script_owner_v62->session().owner().lifecycle().load_step)return -1;
  }else if(!t.r_.npc_skills_v84||actual->slots!=&t.r_.npc_skills_v84->owner().state()||
    actual->fields!=t.r_.npc_skills_v84->source_ai_fields_v115()||!t.r_.actor->session||
    actual->script_step!=t.r_.actor->session->owner().lifecycle().load_step)return -1;
  switch(q->operation){
   case skill_ai_using_v3:case skill_ai_casting_v3:{
    if(!t.r_.actor->machine)return -1;int current{};
    if(dh2_character_native_fsm_get_integer(&current,&t.r_.actor->machine->native_fsm(),0)!=1)return -1;
    out->word=current==(q->operation==skill_ai_using_v3?6:7);return 0;
   }
   case skill_ai_row_v3:{
    t.skill_.state=&t.state();t.skill_.character=q->character;
    SkillStateRequest32V4 request{skill_state_row_v4,0,q->index,0,0,0};
    SkillStateResponse16V4 response{};if(t.skill_operation(&t.skill_,request,response))return -1;
    out->row=reinterpret_cast<const dh2::data::SkillProjection76*>(response.identity);return 0;
   }
   case skill_ai_callback_v3:{
    unsigned value{};int status=-1;
    if(t.r_.player_script_owner_v62)status=skill_callback_session_v3(t.r_.player_script_owner_v62->session(),
     reinterpret_cast<const Instance32*>(q->subject),q->value,&value,t.r_.error);
    else if(t.r_.actor->session)status=skill_callback_session_v3(*t.r_.actor->session,
     reinterpret_cast<const Instance32*>(q->subject),q->value,&value,t.r_.error);
    out->word=value;return status;
   }
   case skill_ai_set_state_v3:{
    t.skill_.state=&t.state();t.skill_.character=q->character;
    t.skill_.physical=t.r_.actor->position_fields_v7().physical2dc;
    return t.skill_dispatch(skill_state_select_v4,q->index,q->value,0);
   }
   case skill_ai_player_v3:{bool player{};if(!t.r_.is_player(player,t.r_.error))return -1;out->word=player;return 0;}
   case skill_ai_stop_loop_v3:t.playback().stop_loop(q->value!=0);return 0;
   default:
    if(t.r_.services.skill_ai_v101&&t.r_.services.skill_ai_v101(t.r_,*q,*out,t.r_.error))return 0;
    return t.fail("Required positive canonical SkillAI source service"),-1;
  }
 }
 static int remaining(void* p,StateOwnerMachine40* machine,const StateOwnerRequest48* q,StateOwnerResponse8*){
  auto& t=*static_cast<CampaignFsmV101*>(p);if(!q||machine!=&t.r_.actor->machine->owner().machine())return -1;
  t.effects_v115_.fsm=&t.r_.actor->machine->native_fsm();
  if(q->operation==state_owner_character_event)return t.raise(q->event,q->payload)?0:-1;
  if(q->operation==state_owner_pin)return t.pin(true,q->character)?0:-1;
  if(q->state==0)return source_campaign_limbus_body_v118(t.r_,*q,t.r_.error);
  if(q->state==14){
   auto debug=[&](const char* name){unsigned ignored{};return dh2_character_debug_load(t.r_.services.debug,t.r_.services.debug_files)==1&&dh2_character_debug_get(&ignored,t.r_.services.debug,name,t.r_.services.debug_files)==1;};
   if(q->operation==state_owner_focus&&q->source_function==0x3c324c){const auto* flag=t.r_.actor->source_bool_field(0x541);if(!flag||!debug("isTracingCharState"))return -1;t.state().flags=*flag?8768u:9024u;return t.animation(-1)?0:-1;}
   if(q->operation==state_owner_blur&&q->source_function==0x3c2dd0)return debug("isTracingCharState")?0:-1;
   if(q->operation==state_owner_event&&q->source_function==0x3c1a14){if(q->event!=34)return 0;const auto* flag=t.r_.actor->source_bool_field(0x540);if(!flag)return -1;return !*flag||t.set_idle_state_v116(false)?0:-1;}
  }
  if(q->state==6&&(q->operation==state_owner_focus||q->operation==state_owner_blur||q->operation==state_owner_event)){
   t.skill_.state=&t.state();t.skill_.character=t.r_.actor->object->identity;t.skill_.physical=t.r_.actor->position_fields_v7().physical2dc;
   return t.skill_dispatch(q->operation==state_owner_focus?skill_state_focus_v4:q->operation==state_owner_blur?skill_state_blur_v4:skill_state_event_v4,q->event,0,q->payload);
  }
  if(q->state==7&&(q->operation==state_owner_focus||q->operation==state_owner_blur||q->operation==state_owner_event))return t.cast_body(q->operation==state_owner_focus?0:q->operation==state_owner_blur?1:2);
  if(q->state==11){PlayerInjureBorrowV7 borrow;if(!t.injury_borrow_v115(borrow))return -1;const auto source=t.injury_services_v115();
   if(q->operation==state_owner_focus&&q->source_function==0x3c33e8)return player_injure_focus_v7(&borrow,&source)==1?0:-1;
   if(q->operation==state_owner_blur&&q->source_function==0x3c4ba4)return player_injure_blur_v7(&borrow,&source)==1?0:-1;
   if(q->operation==state_owner_event&&q->source_function==0x3c0044)return player_injure_empty_v7(q->source_function)==1?0:-1;
  }
  if(q->state==8||q->state==9){const unsigned kind=q->state==8?1:0;int status=-1;
   if(q->operation==state_owner_focus&&q->source_function==(kind?0x3c45c4u:0x3c3cc0u))status=dh2_character_native_effect_focus(&t.effects_v115_,kind,&t.effect_services_v115_);
   else if(q->operation==state_owner_blur&&q->source_function==(kind?0x3c4834u:0x3c3b4cu))status=dh2_character_native_effect_body(&t.effects_v115_,kind,1,&t.effect_services_v115_);
   else if(q->operation==state_owner_event&&q->source_function==(kind?0x3c2b28u:0x3c0030u))status=dh2_character_native_effect_event(&t.effects_v115_,kind,q->event,&t.effect_services_v115_);
   if(status>=0)return t.publish_lock_v115()?0:-1;
  }
  if(q->state==10){const auto borrow=t.knockback_borrow_v115();const auto source=t.knockback_services_v115();int status=-1;
   if(q->operation==state_owner_focus&&q->source_function==0x3c4a48)status=character_knockback_focus_v1(borrow,q->payload,source);
   else if(q->operation==state_owner_blur&&q->source_function==0x3c48e0)status=character_knockback_blur_v1(borrow,source);
   else if(q->operation==state_owner_event&&q->source_function==0x3c5ab0)status=character_knockback_event_v1(borrow,q->event,source);
   if(status>=0)return t.publish_lock_v115()?0:-1;
  }
  if(q->state==13){
   const auto* row=t.r_.design.ai()?dh2::data::ai_props(*t.r_.design.ai(),t.r_.properties->resolved[1]):nullptr;
   if(!row)return t.fail("Required actual CSInteract AI predicates"),-1;
   auto debug=[&](const char* name){unsigned ignored{};return dh2_character_debug_load(t.r_.services.debug,t.r_.services.debug_files)==1&&dh2_character_debug_get(&ignored,t.r_.services.debug,name,t.r_.services.debug_files)==1;};
   if(q->operation==state_owner_focus){
    if(!debug("isTracingCharState")||!debug("isTracingCSInteract"))return -1;
    if(row->type==4){t.state().idle_suppressed=0;return t.r_.actor->machine->transition(3,-1,0)>=0?0:-1;}
    t.state().flags=0x63c1;if(!t.animation(-1))return -1;
    auto* repeated=t.r_.actor->source_bool_field(0x548);if(!repeated)return -1;
    if(!(row->flags&8)&&*repeated&&!source_campaign_character_command_look_v114(t.r_.services.world,t.r_.actor->object->identity,q->payload,t.r_.error))return -1;
    return t.cancel_sneaking()?0:-1;
   }
   if(q->operation==state_owner_blur){
    if(!debug("isTracingCSInteract"))return -1;
    if(!(row->flags&8)&&(row->type==6||row->type==7||row->type==8)){
     dh2::world::GameObjectInitializationFieldsV62 fields;if(!t.r_.actor->inherited_initialization_fields_v62(t.r_.actor,fields,t.r_.error))return -1;
     auto* initial=fields.vector3?fields.vector3(0x145c):nullptr;auto* rotation=fields.vector3?fields.vector3(0x16c):nullptr;if(!initial||!rotation)return -1;
     std::copy_n(initial,3,rotation);t.r_.actor->runtime.rotation.heading_angle=initial[2];
      if(t.r_.actor->source_visual()){auto visual=t.r_.visual?t.r_.visual->visual():nullptr;if(!visual||!visual->sync_rotation_v86(t.r_.error))return -1;}
    }
    const auto kind=t.r_.actor->predicates.interaction;if(kind!=4&&kind!=5)return 0;
    if(q->other>=0&&q->other<=19&&(794624u&(1u<<q->other)))return 0;
    //Positive held Liftable releases belong to that selected receiver; retain
    //the reached source prefix rather than clear its ownership speculatively.
    dh2::world::GameObjectInitializationFieldsV62 fields;if(!t.r_.actor->inherited_initialization_fields_v62(t.r_.actor,fields,t.r_.error))return -1;
    auto* target=fields.pointer?fields.pointer(0x54c):nullptr;if(!target)return -1;
    if(*target)return t.fail("Required selected Liftable CSInteract Drop ownership query"),-1;*target=0;return 0;
   }
   if(q->operation==state_owner_event){
    if(q->event!=0x28)return 0;dh2::world::GameObjectInitializationFieldsV62 fields;
    if(!t.r_.actor->inherited_initialization_fields_v62(t.r_.actor,fields,t.r_.error))return -1;
    auto* target=fields.pointer?fields.pointer(0x54c):nullptr;if(!target)return -1;if(!*target)return 0;
    const auto* text=reinterpret_cast<const char*>(q->payload);if(!text)return -1;
    const bool release=std::strcmp(text,"release")==0,pick=std::strcmp(text,"pick_up")==0;if(!release&&!pick)return 0;
    const dh2::world::CanonicalObjectBorrowV1* receiver{};std::int32_t key{};bool next=t.objects_->source_ordered_begin_v38(key,receiver);
    while(next&&(!receiver||receiver->identity!=*target))next=t.objects_->source_ordered_next_v38(key,key,receiver);
    if(!next||!receiver||!receiver->type_f4)return t.fail("Required captured CSInteract object receiver"),-1;
    if(*receiver->type_f4==6)return t.fail("Required selected Liftable Release/PickUp body"),-1;
    if(release)*target=0;return 0;
   }
  }
  return t.r_.services.state_method_v101?t.r_.services.state_method_v101(t.r_,*q,t.r_.error)?0:-1:-1;
 }
 static int frame(void* p,NativeFsm24* f,const NativeFsmRequest32* q,std::uint32_t* out){auto& t=*static_cast<CampaignFsmV101*>(p);
  if(!q||!out||f!=&t.r_.actor->machine->native_fsm())return -1;
  if(q->service==fsm_engine_dt){auto app=t.application_.lock();if(!app)return -1;*out=app->source_loading_v55().dt8c;return 0;}
  if(q->service==fsm_profile_begin||q->service==fsm_profile_end){StateOwnerRequest48 source{};source.operation=q->service==fsm_profile_begin?state_owner_profile_begin:state_owner_profile_end;source.source_function=q->service==fsm_profile_begin?0x337888:0x318254;return t.r_.actor->diagnostics->invoke(source);}
  if(q->service==fsm_set_stun||q->service==fsm_set_scare){t.effects_v115_.fsm=f;if(!t.reload_lock_v115())return -1;const int status=dh2_character_native_effect_set(&t.effects_v115_,q->service==fsm_set_scare?1:0,q->argument0,q->argument1,q->payload,q->force,&t.effect_services_v115_);if(status<0||!t.publish_lock_v115())return -1;*out=status;return 0;}
  return t.r_.services.state_frame_v101?t.r_.services.state_frame_v101(t.r_,*q,*out,t.r_.error)?0:-1:-1;
 }
 static int prepare(void* p,const StateOwnerRequest48& q){auto& t=*static_cast<CampaignFsmV101*>(p);
  if(q.operation!=state_owner_focus&&q.operation!=state_owner_event)return 0;
  if(t.refresh(q.state))return 0;
  if(t.r_.error.empty())t.r_.error="Required canonical FSM prepare: state "+std::to_string(q.state)+
   " operation "+std::to_string(q.operation)+" source "+std::to_string(q.source_function)+
   " event "+std::to_string(q.event)+" equipment "+std::to_string(t.r_.prepared_equipment_v60!=nullptr);
  return -1;
 }
 static int update_prepare(void* p){auto& t=*static_cast<CampaignFsmV101*>(p);return t.refresh(t.state().current)?0:-1;}
public:
 bool borrow_skill_context(const std::shared_ptr<Record>& record_lease,SourceCampaignCharacterSkillContextBorrowV1& out,std::string& error){
  out={};auto actor=r_.actor;
  if(!record_lease||record_lease.get()!=&r_||r_.fsm_context_v101.get()!=this||!actor||!actor->object||!actor->machine||!r_.properties||
     !actor->object->identity||actor->machine->native_fsm().character!=actor->object->identity){
   error="Required SAME canonical Character/FSM/Session skill context";return false;
  }
  const auto id=actor->object->identity;
  auto* ai_owner=static_cast<SkillAIOwnerV3*>(nullptr);auto* slots=static_cast<dh2::character::skills::State40*>(nullptr);
  auto* fields=static_cast<SkillAIStateV3*>(nullptr);CharacterSkillOwner* instances_v1=nullptr;
  CharacterSkillOwnerV6* instances_v3=nullptr;std::int32_t script_step{};
  dh2::character::CharacterScriptSession* session_v1=nullptr;
  dh2::character::CharacterScriptSessionV3* session_v3=nullptr;
  dh2::data::PropertyView* property_view=nullptr;
  if(r_.player_script_owner_v62){
   auto& player=*r_.player_script_owner_v62;instances_v3=player.native_skill_owner();
   if(!instances_v3){error="Required actual same-player SkillOwnerV6";return false;}
   session_v3=&player.session();property_view=&session_v3->property_view();
   if(session_v3->timers().owner!=id||session_v3->properties()!=r_.properties||
      property_view->resolved!=r_.view.resolved||property_view->resolved!=r_.properties->resolved.data()){
    error="Required same-player Session/PropertyView aliases";return false;
   }
   ai_owner=&r_.skill_owner_view_v68;slots=const_cast<dh2::character::skills::State40*>(&instances_v3->state());
   fields=&player.skill_ai();script_step=session_v3->owner().lifecycle().load_step;
  }else{
   if(!r_.npc_skills_v84||!actor->session){error="Required actual same-NPC SkillOwner/Session";return false;}
   session_v1=actor->session.get();property_view=&session_v1->property_view();
   if(session_v1->timers().owner!=id||session_v1->properties()!=r_.properties||
      property_view->resolved!=r_.view.resolved||property_view->resolved!=r_.properties->resolved.data()){
    error="Required same-NPC Session/PropertyView aliases";return false;
   }
   instances_v1=&r_.npc_skills_v84->owner();slots=const_cast<dh2::character::skills::State40*>(&instances_v1->state());
   fields=r_.npc_skills_v84->source_ai_fields_v115();
   if(!fields){error="Required source-produced same-NPC SkillAI fields";return false;}
   script_step=session_v1->owner().lifecycle().load_step;
  }
  if(!ai_owner||!slots||!fields||ai_owner->character!=id||ai_owner->reserved||
     slots->owner!=id||!r_.services.skills||!property_view||
     ((instances_v1!=nullptr)==(instances_v3!=nullptr))||
     ((session_v1!=nullptr)==(session_v3!=nullptr))||
     !actor->object->binding.state||!actor->object->binding.state->owner||
     actor->object->binding.state->owner->identity!=id){
   error="SkillAI context fields do not belong to the same canonical Character";return false;
  }
  ai_owner->flags=actor->machine->state().flags;skill_ai_context_v3_={ai_owner,slots,fields,script_step,0};
  auto* heading=actor->source_heading_enabled412_v101();
  if(!heading){error="Required actual same-Character heading field for SkillStateV4";return false;}
  skill_.state=&actor->machine->state();skill_.character=id;skill_.physical=actor->position_fields_v7().physical2dc;
  skill_.target=actor->object->target.target;skill_.last_target=actor->object->target.last_target;skill_.heading_enabled=*heading;
  out.record_lease=std::static_pointer_cast<void>(record_lease);
  out.context_lease=r_.fsm_context_v101;out.character=id;out.machine=&actor->machine->native_fsm();
  out.session_v1=session_v1;out.session_v3=session_v3;out.property_view=property_view;out.skill_tables=r_.services.skills;
  out.instances_v1=instances_v1;out.instances_v3=instances_v3;out.ai=&skill_ai_context_v3_;out.state=&skill_;
  out.ai_services={this,skill_ai_loan_service};out.state_services=skill_services_;return true;
 }
 bool set_idle_state_v116(bool mode){state().idle_suppressed=mode?1:0;return r_.actor->machine->transition(3,-1,0)>=0;}
 bool set_anim_state_v116(int animation,bool event_end,bool update_end){r_.actor->publish_anim_state_flags_v116(event_end,update_end);state().animation_override=animation;return r_.actor->machine->transition(14,-1,0)>=0;}
 bool source_click_point_v120(const float* point,bool released){return click_point_v120(point,released);}
 bool command_move_v116(std::uintptr_t target){const float* p{};return position(target,p)&&move(p);}
 bool control_stop_v116(){bool is_remote{};if(!remote(is_remote))return false;if(is_remote)return true;return stop()&&raise(0x3f,0);}
 bool set_limbus_v118(bool mode){r_.actor->publish_limbus_mode_v118(mode);int current{};if(dh2_character_native_fsm_get_integer(&current,&r_.actor->machine->native_fsm(),0)!=1)return false;if(current==0||current==17||current==16)state().elapsed_ms=0;return r_.actor->machine->transition(0,-1,0)>=0;}
 bool set_interact_v114(int kind,bool repeat,std::uintptr_t object,bool force){
  const auto* tables=r_.services.animation_tables;if(!tables)return fail("Required actual Interact CharAnimTable");
  const int table=dh2_character_animation_table_id(r_.properties->resolved[2],static_cast<int>(tables->characters.size()));
  if(table<0||std::size_t(table)>=tables->characters.size())return true;
   dh2::world::GameObjectInitializationFieldsV62 fields;
   if(!r_.actor->inherited_initialization_fields_v62(r_.actor,fields,r_.error))return false;
   auto* repeated=fields.byte?fields.byte(0x548):nullptr;
  auto* target=fields.pointer?fields.pointer(0x54c):nullptr;if(!repeated||!target)return fail("Required source CSInteract constructor cells");
  unsigned event=0xc353;int next=13;
  if(kind==8){event=0xc354;next=5;}else if(kind==10){event=0xc357;next=15;}else{
   const auto& row=tables->characters[table];if(row.fields.size()<=15)return fail("Required source CharAnim Interact array");
   if(kind<0||std::size_t(kind)>=row.fields[15].size())return true;
   std::int32_t mask{};if(!constant("AnimStancedAnim","SL__LIST_IPHONE",mask))return false;int stance=0;
   if(static_cast<unsigned>(mask)&0x800000u){bool player{};if(!r_.is_player(player,r_.error))return false;if(player){std::int32_t count{};StanceFacts16 facts;if(!constant("AnimStances","COUNT_IPHONE",count)||!r_.prepared_equipment_v60||!r_.prepared_equipment_v60->stance_facts(true,count,facts,r_.error)||dh2_character_anim_stance(&stance,&facts)!=1)return false;}}
   state().animation_override=static_cast<std::int32_t>(static_cast<std::uint32_t>(row.fields[15][kind])+static_cast<std::uint32_t>(stance));
  }
  r_.actor->predicates.interaction=kind;*repeated=repeat;*target=object;
  return (force?r_.actor->machine->transition(next,event,object):r_.actor->machine->event(event,object))>=0;
 }
 bool raise_v114(unsigned event,std::uintptr_t payload){return raise(event,payload);}
 int reaction_v115(const SkillApplyRequestV6& request,SkillApplyResponseV6& out){
  if(request.subject!=r_.actor->object->identity||request.target!=request.subject)return -1;
  effects_v115_.fsm=&r_.actor->machine->native_fsm();
  if(request.service==skill_apply_cancel_sneaking_v6){if(!cancel_sneaking())return -1;out={};return 1;}
  if(request.service==skill_apply_stun_v6||request.service==skill_apply_scare_v6){if(!reload_lock_v115())return -1;const int status=dh2_character_native_effect_set(&effects_v115_,request.service==skill_apply_scare_v6?1:0,request.word,1,request.attacker,request.service==skill_apply_scare_v6?request.flags:0,&effect_services_v115_);if(status<0||!publish_lock_v115())return -1;out={};return 1;}
  if(request.service==skill_apply_push_v6){if(!reload_lock_v115())return -1;const auto borrow=knockback_borrow_v115();const auto source=knockback_services_v115();if(character_set_knockback_v1(borrow,request.word!=0,request.attacker,request.flags!=0,source)<0||!publish_lock_v115())return -1;out={};return 1;}
  if(request.service==skill_apply_slow_v6){BuffOwner* buffs{};if(r_.player_script_owner_v62)buffs=r_.player_script_owner_v62->native_buffs();else if(r_.save_connection_v86&&!r_.save_connection_v86->borrow_buffs(buffs,r_.error))return -1;
   auto app=application_.lock();auto libraries=app?app->source_fx_libraries_v63():nullptr;const SlowReactionBorrowV1 borrow{&r_.view,buffs,r_.design.ai(),r_.design.classes(),libraries?libraries->source_tables():dh2::data::EffectsTables::Borrow{}};BuffResult24 result;if(character_slow_reaction_v1(borrow,request.word,result,r_.error)!=1)return -1;out={};return 1;
  }
  if(request.service!=skill_apply_injure_v6&&request.service!=skill_apply_block_v6&&request.service!=skill_apply_dodge_v6)return 0;
  PlayerInjureBorrowV7 borrow;if(!injury_borrow_v115(borrow))return -1;const auto common=injury_services_v115();int status{};
  if(request.service==skill_apply_injure_v6)status=player_set_injure_v7(&borrow,request.attacker,request.flags!=0,&common);
  else{const DefensiveStateServicesV1 source{&common,[](void* raw,int table,DefensiveAnimationV1 kind,bool* found,int* animation){auto& t=*static_cast<CampaignFsmV101*>(raw);if(!found||!animation||!t.r_.services.animation_tables)return -1;const auto& rows=t.r_.services.animation_tables->characters;*found=table>=0&&std::size_t(table)<rows.size();if(!*found)return 0;const auto& field=rows[table].fields[kind==DefensiveAnimationV1::blocking?2:7];if(field.size()!=1)return -1;*animation=field[0];return 0;}};
   status=character_defensive_state_v1(&borrow,request.attacker,request.flags!=0,request.service==skill_apply_block_v6?DefensiveAnimationV1::blocking:DefensiveAnimationV1::dodging,&source);
  }if(status!=1)return -1;out={};return 1;
 }
 CampaignFsmV101(Record& record,const SourceCampaignCandidateBorrowV55& source):r_(record),application_(source.application),floors_(source.floors),navigation_(source.navigation_registry),objects_(source.objects){
  if(floors_)pf_geometry_=&floors_->collision_world;
  if(navigation_)pf_registry_=&navigation_->registry();
 }
 CampaignFsmV101(Record& record,const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
  const std::shared_ptr<void>& lease,const dh2::navigation::CollisionWorld& geometry,dh2::navigation::ObstacleRegistry& registry)
  :r_(record),application_(app),process_pf_lease_(lease),pf_geometry_(&geometry),pf_registry_(&registry),objects_(record.services.canonical_objects){}
 CampaignFsmV101(Record& record,const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
  const std::shared_ptr<void>& world_lease,const std::shared_ptr<dh2::floors::World>& floors,
  dh2::navigation::ObstacleRegistry& registry)
  :r_(record),application_(app),backend_application_lease_v1_(app),floors_(floors),backend_world_lease_v1_(world_lease),
   pf_geometry_(floors?&floors->collision_world:nullptr),pf_registry_(&registry),
   objects_(record.services.canonical_objects){}
 ~CampaignFsmV101(){
  //Native scratch retirement after source unpublication/VM close. Do not
  //leave actor PF aliases pointing into the soon-to-be-destroyed route owner.
  if(r_.actor&&r_.actor->runtime.path.segments==route_segments_.data()){
   auto& path=r_.actor->runtime.path;dh2_nav_drop_path(&path);path.segments=nullptr;path.capacity=0;
  }
 }
 bool bind(WorldNpcStateServicesV1& out){
  if(!r_.actor||!r_.services.debug||!r_.services.debug_files||!pf_geometry_||!pf_registry_)return fail("Required actual canonical FSM resource graph");
  r_.actor->diagnostics=std::make_unique<StateOwnerDebugDiagnostics>(*r_.services.debug,*r_.services.debug_files);
  r_.actor->bodies={this,body_service};out.remaining_methods={this,remaining};out.outer={this,frame};
  if(!r_.services.animation_tables)return fail("Required actual effect CharAnim tables");for(const auto& row:r_.services.animation_tables->characters){if(row.fields.size()<=32||row.fields[32].size()!=1||row.fields[29].size()!=1)return fail("Required authored Stunned/Scared scalar rows");stunned_v115_.push_back(row.fields[32][0]);scared_v115_.push_back(row.fields[29][0]);}
  effects_v115_={nullptr,stunned_v115_.data(),scared_v115_.data(),static_cast<unsigned>(stunned_v115_.size()),0};
  out.pre_spawn=&pre_spawn_;out.spawn_services=&pre_spawn_services_v108_;
  out.remaining_updates={this,[](void* p,StateOwnerMachine40*,const StateOwnerUpdateRequest24* q){auto& t=*static_cast<CampaignFsmV101*>(p);if(q&&q->state==0&&q->source_function==0x3bffe4)return 0; /* literal CSLimbus.OnUpdate */ if(q&&q->state==14&&q->source_function==0x3c1a3c){const auto* active=t.r_.actor->source_bool_field(0x1480);const auto* flag=t.r_.actor->source_bool_field(0x541);if(!active||!flag)return -1;return *active||!*flag||t.set_idle_state_v116(false)?0:-1;}if(q&&((q->state==13&&q->source_function==0x3c0080)||(q->state==11&&q->source_function==0x3c0040)))return 0; //Actual Interact/Injured BXLR Update methods.
   if(q&&q->state==10&&q->source_function==0x3c0038)return 0; //CSKnockedBack source literal Update.
   if(q&&((q->state==8&&q->source_function==0x3c4788)||(q->state==9&&q->source_function==0x3c549c))){const int status=dh2_character_native_effect_body(&t.effects_v115_,q->state==8?1:0,0,&t.effect_services_v115_);return status>=0&&t.publish_lock_v115()?0:-1;}
   return q&&t.r_.services.state_update_v101&&t.r_.services.state_update_v101(t.r_,*q,t.r_.error)?0:-1;}};
  out.diagnostics=r_.actor->diagnostics.get();out.diagnostics_required=1;out.prepare_context=this;out.prepare_method=prepare;out.prepare_update=update_prepare;return true;
 }
 void observe(actor::BlendedPlayback& actual,const actor::BlendedPlaybackEvent& event){if(&actual!=&playback())unavailable("Wrong canonical animation observer");CampaignAnimationRandomScopeV101 random_scope(r_);lag_=event.event.handoff.lag_ms;
  const animation::TriggeredEvent trigger{lag_,event.event.handoff.payload};
  if(!raise(event.event.handoff.event_id,event.event.handoff.event_id==0x28?reinterpret_cast<std::uintptr_t>(&trigger):0))unavailable("Required canonical authored animation event");
 }
 bool bind_player(PlayerSkillGameplayServicesV3& out){out.ai=&r_.actor->ai_events;out.events=events_;
  out.skill_services={this,[](void* p,SkillAIContextV3*,const SkillAIRequest32V3* q,SkillAIResponse32V3* out){auto& t=*static_cast<CampaignFsmV101*>(p);if(!q||!out||q->character!=t.r_.actor->object->identity)return -1;
   if(q->operation!=skill_ai_set_state_v3)return t.r_.services.skill_ai_v101?t.r_.services.skill_ai_v101(t.r_,*q,*out,t.r_.error)?0:-1:-1;
   t.skill_.state=&t.state();t.skill_.character=q->character;t.skill_.physical=t.r_.actor->position_fields_v7().physical2dc;
   return t.skill_dispatch(skill_state_select_v4,q->index,q->value,0);}};return true;
 }
 static int script_command_invoke(void* raw,ScriptCommandState48* state,const ScriptCommandRequest40* q,const float** out){
  auto& t=*static_cast<CampaignFsmV101*>(raw);if(!state||state!=&t.script_command_state_||!q||!out)return -1;
  *out=nullptr;
  switch(q->service){
   case script_controller_stop:return t.command_stop()?0:-1;
   case script_controller_move_object:return t.command_move_v116(q->target)?0:-1;
   case script_controller_head_point:return t.command_heading(q->point)?0:-1;
   case script_controller_move_point:return t.move(q->point)?0:-1;
   // v2Controller::Cmd_Attack against this candidate's same World/Target/FSM.
   case script_controller_attack:return t.command_attack(q->target)?0:-1;
   case script_target_position:return t.position(q->subject,*out)?0:-1;
   case script_look_vector:{const float angle=t.r_.actor->runtime.rotation.rotation[2];t.script_look_={std::sin(angle),-std::cos(angle),0.f};*out=t.script_look_.data();return 0;}
   default:return -1;
  }
 }
 static int script_command_number(void*,const dh2_script_value* value,float* out){
  if(!value||!out)return -1;if(value->type==DH2_SCRIPT_NUMBER){*out=value->number;return 0;}
  if(value->type==DH2_SCRIPT_BOOLEAN){*out=value->boolean?1.f:0.f;return 0;}
  if(value->type==DH2_SCRIPT_NIL){*out=0.f;return 0;}return -1;
 }
 static int script_motion_capability(void* raw,std::uint32_t address,std::uint32_t* value){
  if(!raw||!value||*value>1)return -1;auto& t=*static_cast<CampaignFsmV101*>(raw);
  if(!t.r_.actor)return -1;auto& object=t.r_.actor->runtime.object;
  if(address==0x390900u)return dh2_nav_object_set_flying(&object,*value);
  if(address==0x39088cu)return dh2_nav_object_set_swimming(&object,*value);
  if(address==0x38ea94u){const int result=dh2_nav_object_is_flying(&object);if(result<0)return -1;*value=static_cast<std::uint32_t>(result);return 0;}
  if(address==0x38ea74u){const int result=dh2_nav_object_is_swimming(&object);if(result<0)return -1;*value=static_cast<std::uint32_t>(result);return 0;}
  return -1;
 }
 static int script_register_animation_dict(void* raw,std::int32_t id){
  if(!raw)return -1;auto& t=*static_cast<CampaignFsmV101*>(raw);
  auto* animator=t.r_.visual?t.r_.visual->animator():nullptr;
  if(!animator){t.fail("Required SAME Character CharAnimator for RegisterAnim");return -1;}
  std::string error;if(!animator->source_add_animation_dict_v116(id,error)){
   t.r_.error=error.empty()?"Actual Character CharAnimator AddAnimDictToSet failed":error;return -1;
  }
  return 0;
 }
 static int npc_script_gameplay_binding(void*,std::uint32_t,dh2_script_function*,void**);
 static int npc_script_gameplay_call(void*,const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
 static int script_commands_refresh(void* raw){
  auto& t=*static_cast<CampaignFsmV101*>(raw);ControllerCommandState32* command{};
  if(!t.controller(command)||!t.r_.actor->object->target.owner||t.r_.actor->object->target.owner->identity!=t.r_.actor->object->identity)return -1;
  auto& actor=*t.r_.actor; t.state().controller_locked=command->locked;
  t.script_command_state_={actor.object->identity,command->controller,actor.object->target.target,
   actor.runtime.subobjects.position,dh2::world::canonical_vec3_k_v1().data(),actor.runtime.path.count,0};
  return 0;
 }
 bool bind_npc(CharacterScriptSessionInput& input){
  if(!script_commands_refresh(this)){input.commands=&script_commands_;input.commands_refresh_context=this;input.commands_refresh=script_commands_refresh;
   script_commands_={&script_command_state_,{this,script_command_invoke,script_command_number},nullptr};
   input.motion_capabilities={this,script_motion_capability};
   input.animation_registration={this,script_register_animation_dict};
   previous_gameplay_context_=input.gameplay_context;previous_gameplay_binding_=input.gameplay_binding;
   input.gameplay_context=this;input.gameplay_binding=npc_script_gameplay_binding;
   input.timer_services=&timer_expiry_v102_;return true;}
  return fail("Required SAME canonical controller/target for NPC script commands");
 }
 int npc_script_bind(std::uint32_t address,dh2_script_function* function,void** context){
  static constexpr std::uint32_t callbacks[]{0x390690u,0x390624u,0x390548u,0x38fcf0u,0x38e970u,0x38eb68u,0x38fbb8u,0x3b8ed8u,0x3b8098u,0x3b8bd8u};
  for(auto candidate:callbacks)if(candidate==address){auto& binding=npc_script_bindings_[address];binding={this,address};*function=npc_script_gameplay_call;*context=&binding;return 1;}return 0;
 }
 int npc_script_call(std::uint32_t address,const dh2_script_value* args,std::uint32_t count,dh2_script_value* out,std::uint32_t capacity,std::uint32_t* written){
  auto need=[&](std::uint32_t n){if(!written||capacity<n)return false;*written=n;return true;};
  auto number=[&](std::uint32_t i,float& value){if(i>=count||args[i].type!=DH2_SCRIPT_NUMBER)return false;value=args[i].number;return true;};
  if(!written||(!args&&count)||(!out&&capacity))return -1;*written=0;
  switch(address){
   case 0x390690u:case 0x390624u:{float value{};if(!number(0,value))return 0;if(address==0x390690u)npc_script_character_filter_=static_cast<std::uint32_t>(value);else npc_script_object_filter_=static_cast<std::uint32_t>(value);return 0;}
   case 0x390548u:{float value{};if(!number(0,value))return 0;const auto mode=static_cast<std::int32_t>(value);npc_script_targets_.sort=mode==1?1u:mode==2?2u:0u;npc_script_targets_.count=0;return 0;}
   case 0x38fcf0u:{float radius{};if(!number(0,radius))return 0;
    // The authored SwampKing path sets Player/NONE/ClosestFirst and calls the
    // one-radius overload. Other overloaded TargetListSearch forms remain strict.
    if(count!=1||npc_script_character_filter_!=128u||npc_script_object_filter_!=2u)return -1;
    if(!r_.services.world_targets||!objects_||r_.services.world_targets->refresh())return -1;
    WorldTargetActorBorrowV1 owner{};if(r_.services.world_targets->actor(r_.actor->object->identity,&owner)||!owner.search)return -1;
    if(npc_script_target_heap_.empty()){
     const auto capacity=std::max<std::size_t>(objects_->characters().size(),1u);if(capacity>65536)return -1;
     npc_script_target_heap_.resize(capacity);
     const auto services=r_.services.world_targets->targets().search_services();
     if(target_search::dh2_target_list_init(&npc_script_targets_,npc_script_target_heap_.data(),static_cast<std::uint32_t>(capacity),owner.search,npc_script_targets_.sort,&services))return -1;
    }
    const auto services=r_.services.world_targets->targets().search_services();
    if(target_search::dh2_target_search_policy_v108(&npc_script_targets_,&r_.services.world_targets->registry(),radius,3.1415927410125732421875f,128u,2u,&services)){r_.error=r_.services.world_targets->targets().error();return -1;}
    return 0;
   }
   case 0x38e970u:if(!need(1))return -1;out[0]={};out[0].type=DH2_SCRIPT_BOOLEAN;out[0].boolean=npc_script_targets_.count==0;return 0;
   case 0x38eb68u:{if(!npc_script_targets_.count){if(!need(1))return -1;out[0]={};return 0;}
    if(!need(5))return -1;const auto& top=npc_script_targets_.heap[0];out[0]={};out[0].type=DH2_SCRIPT_SOURCE_OBJECT;out[0].identity=top.identity;
    out[1]={};out[1].type=DH2_SCRIPT_NUMBER;out[1].number=top.distance;std::uint32_t degree_bits=0x42652ee0u;float degrees{};std::memcpy(&degrees,&degree_bits,4);
    out[2]={};out[2].type=DH2_SCRIPT_NUMBER;out[2].number=top.angle*degrees;out[3]={};out[3].type=DH2_SCRIPT_BOOLEAN;out[3].boolean=top.flags&1u;out[4]={};out[4].type=DH2_SCRIPT_NUMBER;return 0;
   }
   case 0x38fbb8u:if(!npc_script_targets_.count)return 0;{target_search::Target24 popped{};return target_search::dh2_target_pop(&npc_script_targets_,&popped)?-1:0;}
   case 0x3b8ed8u:{if(!count||(args[0].type!=DH2_SCRIPT_IDENTITY&&args[0].type!=DH2_SCRIPT_SOURCE_OBJECT))return 0;
    return source_campaign_character_command_look_v114(r_.services.world,r_.actor->object->identity,args[0].identity,r_.error)?0:-1;}
   case 0x3b8098u:{if(count<2||(args[0].type!=DH2_SCRIPT_IDENTITY&&args[0].type!=DH2_SCRIPT_SOURCE_OBJECT)||args[1].type!=DH2_SCRIPT_NUMBER)return 0;
    return add_aggro_v108(args[0].identity,args[1].number)?0:-1;}
   case 0x3b8bd8u:{if(!count||args[0].type!=DH2_SCRIPT_NUMBER)return 0;const float requested=args[0].number;
    if(!std::isfinite(requested)||requested<0.f||static_cast<double>(requested)>static_cast<double>(UINT32_MAX))return -1;
    unsigned ignored{};return npc_skill_ai_v115(skill_ai_use_v3,static_cast<unsigned>(requested),ignored);}
   default:return 0;
  }
 }
 bool borrow_path(const std::shared_ptr<Record>& record_lease,SourceCharacterPathBorrowV105& out){
  out={};
  auto actor=r_.actor;
  if(!record_lease||record_lease.get()!=&r_||r_.fsm_context_v101.get()!=this||
     !actor||!actor->object||!actor->machine||!actor->object->identity||
     actor->machine->native_fsm().character!=actor->object->identity)
   return fail("Required same-record CampaignFsmV101 path owner");
  auto& path=r_.actor->runtime.path;
  if(path.count>path.capacity||path.owned>1||path.owned>path.count||path.reserved)return fail("Malformed SAME native Character path prefix");
  if(path.segments){if(path.segments!=route_segments_.data()||path.capacity!=route_segments_.size())return fail("Character PF path is not the actual retained route scratch");}
  else if(path.count||path.capacity||path.owned)return fail("Character PF path lost its actual caller storage");
  const auto identity=actor->object->identity;
  out.owner=std::static_pointer_cast<void>(record_lease);
  out.record_lease=std::static_pointer_cast<void>(record_lease);
  out.context_lease=r_.fsm_context_v101;out.character=identity;
  out.machine=&actor->machine->native_fsm();out.path=&path;return true;
 }
 bool borrow_path(SourceCharacterPathBorrowV105& out){
  auto record_lease=r_.weak_from_this().lock();
  if(!record_lease){out={};return fail("Required retained canonical record lease for Character path");}
  return borrow_path(record_lease,out);
 }
 bool update_timers(unsigned dt,unsigned blocked){
  if(r_.player_script_owner_v62){const int code=r_.player_script_owner_v62->update_timers(dt,blocked);if(code<0)r_.error=r_.player_script_owner_v62->error();return code==1;}
  if(!r_.actor->session){if(r_.actor->source_ctor_empty_timers_v111())return true;return fail("Required SAME active NPC timer receiver");}const int code=r_.actor->session->update_timers(dt,blocked);if(code<0)r_.error=r_.actor->session->error();return code==1;
 }
 bool update_master_v108(){
  auto* fields=r_.actor->source_ai_pointers_v105();
  if(!fields)return fail("Required actual CharAI master50/54/55 constructor fields");
  MasterServicesV108 services;
  services.raise=[this](unsigned event,std::uintptr_t payload,std::string&){return raise(event,payload);};
  services.query=[this](MasterQueryV108 q,std::uintptr_t target,unsigned& out,std::string& e){
   const auto owner=r_.actor->object->identity;std::int32_t id{};
   if(q==MasterQueryV108::OwnerAiId||q==MasterQueryV108::Sight){
    auto* ai=r_.design.ai();if(!ai||dh2_character_target_ai_id(&id,r_.properties->resolved.data(),ai->rows.size()))return fail("Required actual master GetCharAIId");
    if(q==MasterQueryV108::OwnerAiId){out=static_cast<unsigned>(id);return true;}
    const auto* row=data::ai_props(*ai,id);const float *a,*b;
    return row&&position(owner,a)&&position(target,b)&&dh2_character_target_sight(&out,a,b,row->view_radius)==0;
   }
   if(q==MasterQueryV108::Dead){
    if(!r_.services.world_targets)return fail("Required SAME master virtual IsDead receiver");
    auto query=r_.services.world_targets->targets().search_services();target_search::Response16 response;
    const target_search::Request24 request{target_search::is_dead,0,target,0};
    if(query.invoke(query.context,&request,&response)){e=r_.services.world_targets->targets().error();return false;}out=static_cast<unsigned>(response.word);return true;
   }
   if(q==MasterQueryV108::MyTurn){bool value{};if(!source_campaign_ai_is_my_turn_v105(r_.services.world,r_.actor->ai_events.ai,value,e))return false;out=value;return true;}
   if(!geometry())return false;std::int32_t value{};
   if(q==MasterQueryV108::CloseRange){if(!geometry_->close_range_v38(owner,target,r_.actor->object->target.target,value,e))return false;}
   else{const auto operation=q==MasterQueryV108::CanRange?WorldAIAttackQueryV1::CharacterCanRangeAttack:
     q==MasterQueryV108::RangedRange?WorldAIAttackQueryV1::IsInRange:WorldAIAttackQueryV1::IsInMeleeRange;
    if(!geometry_->read(owner,target,operation,value,e))return false;}
   out=static_cast<unsigned>(value);return true;
  };
  return character_update_master_v108(*fields,services,r_.error);
 }
 bool add_aggro_v108(std::uintptr_t id,float amount){
  SourceCampaignCharacterBorrowV62 other;if(!borrow_source_campaign_character_v62(r_.services.world,id,other,r_.error))return false;
  auto own=r_.actor->source_aggro_v84(),peer=other.character->actor->source_aggro_v84();
  if(!own||!peer||!own->outgoing()||!peer->incoming())return fail("Required SAME constructor-produced reciprocal aggro trees");
  if(!own->prepare_outgoing_insert(id,r_.error)||!peer->prepare_incoming_insert(r_.actor->object->identity,r_.error))return false;
  struct Context{CampaignFsmV101& self;SourceCampaignCharacterBorrowV62& other;};Context context{*this,other};
  const SkillAggroServicesV6 services{&context,[](void* raw,const SkillAggroRequestV6* q,unsigned* out){auto& c=*static_cast<Context*>(raw);auto& t=c.self;if(!q||!out)return -1;*out=0;
   switch(q->service){
    case skill_aggro_owner_player_v6:{bool player{};if(!t.r_.is_player(player,t.r_.error))return -1;*out=player;return 0;}
    case skill_aggro_owner_dead_v6:if(!t.r_.life)return -1;*out=t.r_.life->dead;return 0;
    case skill_aggro_target_dead_v6:if(!c.other.character->life)return -1;*out=c.other.character->life->dead;return 0;
     case skill_aggro_target_on_aggro_v6:return source_campaign_character_aggro_event_v84(t.r_.services.world,data::aggro_notify_target,q->other,q->subject,nullptr,t.r_.error)?0:-1;
     default:return -1;
   }}};
  unsigned bits;std::memcpy(&bits,&amount,4);SkillAggroOwnerV6 actor{r_.actor->object->identity,own->outgoing()};SkillAggroTargetV6 target{id,peer->incoming()};SkillAggroOutputV6 result;
  if(dh2_character_skill_aggro_v6(&result,&actor,&target,bits,1,&services))return fail("Actual source AddAggro delivery failed");
  float added{};std::memcpy(&added,&result.returned_bits,4);
  // Character::_AddAggro loads and queries this ignored diagnostic only after
  // the actual AI_AddAggro result compares strictly greater than zero.
  if(added>0.f&&r_.services.debug&&r_.services.debug_files){
   (void)dh2_character_debug_load(r_.services.debug,r_.services.debug_files);
   std::uint32_t ignored{};(void)dh2_character_debug_get(&ignored,r_.services.debug,"isTracingThreatChange",r_.services.debug_files);
  }
  return true;
 }
 bool search_aggro_v108(bool tracked,float radius,unsigned flags,std::vector<AggroFrameTargetV108>& out){
  if(!objects_||!r_.services.world_targets)return fail("Required SAME source Character list/target directory");
  std::vector<std::uintptr_t> ids;
  if(tracked){auto& list=objects_->source_characters70_v102();ids.assign(list.begin(),list.end());}
  else ids=objects_->characters(); //Actual Add-produced source60 order.
  if(ids.size()>65536)return fail("Source TargetList admission exceeds retained bounded capacity");
  std::vector<target_search::Entry16> entries(ids.size());target_search::Entry16 sentinel{};sentinel.next=entries.empty()?&sentinel:&entries[0];
  auto observe=[&](std::uintptr_t id,WorldTargetActorBorrowV1& loan){
   if(r_.services.world_targets->actor(id,&loan)||!loan.search||!loan.position||!loan.target_node)return false;
   std::copy_n(loan.position,3,loan.search->position);loan.search->has_target_position=0;
   if(*loan.target_node){if(!loan.target_enabled)return false;if(*loan.target_enabled){if(!loan.cached_target_position)return false;std::copy_n(loan.cached_target_position,3,loan.search->target_position);loan.search->has_target_position=1;}}
   if(loan.heading_angle)loan.search->rotation=*loan.heading_angle;return true;
  };
  WorldTargetActorBorrowV1 owner;if(!observe(r_.actor->object->identity,owner))return fail("Required actual SearchEff owner position/heading");
  for(std::size_t i=0;i<ids.size();++i){WorldTargetActorBorrowV1 loan;if(!observe(ids[i],loan))return fail("Required actual source Character list element");entries[i]={i+1<entries.size()?&entries[i+1]:&sentinel,loan.search};}
  target_search::Room16 end{},room{};end.next=&room;room={&end,&sentinel};target_search::Registry8 registry{&end};
  std::vector<target_search::Target24> heap(std::max<std::size_t>(ids.size(),1));target_search::List40 list{};const auto services=r_.services.world_targets->targets().search_services();
  if(target_search::dh2_target_list_init(&list,heap.data(),heap.size(),owner.search,1,&services)||
    target_search::dh2_target_search_policy_v108(&list,&registry,radius,6.283185482025146484375f,flags,2,&services)){r_.error=r_.services.world_targets->targets().error();return fail("Actual source Character TargetList Search failed");}
  out.clear();out.reserve(list.count);while(list.count){target_search::Target24 value;if(target_search::dh2_target_pop(&list,&value))return false;out.push_back({value.identity,value.flags});}return true;
 }
 bool update_aggro_v108(){
  auto fields=r_.actor->source_ai_pointers_v105();auto aggro=r_.actor->source_aggro_v84();
  if(!fields||!aggro||!aggro->outgoing()||!aggro->incoming())return fail("Required SAME CharAI Aggro C1 fields/maps");
  auto row=[&]()->const data::AiProps*{int id{};auto ai=r_.design.ai();return ai&&dh2_character_target_ai_id(&id,r_.properties->resolved.data(),ai->rows.size())==0?data::ai_props(*ai,id):nullptr;};
  AggroFrameServicesV108 s;
  s.query=[&](AggroFrameQueryV108 q,bool& out,std::string& e){
   if(q==AggroFrameQueryV108::Player)return r_.is_player(out,e);
   if(q==AggroFrameQueryV108::MyTurn)return source_campaign_ai_is_my_turn_v105(r_.services.world,r_.actor->ai_events.ai,out,e);
   if(q==AggroFrameQueryV108::Remote)return remote(out);
   if(q==AggroFrameQueryV108::HasAggro){out=aggro->outgoing()->count!=0;return true;}
   if(q==AggroFrameQueryV108::AwaitingSpawn){int state;if(dh2_character_native_fsm_get_integer(&state,&r_.actor->machine->native_fsm(),0)!=1)return false;out=state==17;return true;}
   const auto ai=row();if(!ai)return fail("Required actual Aggro GetCharType");out=q==AggroFrameQueryV108::Npc?(ai->type==6||ai->type==7||ai->type==8):q==AggroFrameQueryV108::Monster?ai->type==4:ai->type==3;return true;
  };
  s.debug=[&](const char* name,bool& out,std::string&){unsigned word;if(!r_.services.debug||!r_.services.debug_files||dh2_character_debug_load(r_.services.debug,r_.services.debug_files)!=1||dh2_character_debug_get(&word,r_.services.debug,name,r_.services.debug_files)!=1)return false;out=word!=0;return true;};
  s.dt=[&](unsigned& out,std::string&){auto app=application_.lock();if(!app)return fail("Expired actual App.GetDt aggro receiver");out=app->source_loading_v55().dt8c;return true;};
  s.random200=[&](unsigned& out,std::string&){if(!r_.services.random)return fail("Required actual App Random0 aggro seed");out=dh2_animation_random(&r_.services.random->seed,&r_.services.random->calls,200);return true;};
  s.highest=[&](std::uintptr_t& out,std::string&){data::AggroQuery result;if(dh2_aggro_query(&result,aggro->outgoing(),r_.actor->object->identity,aggro->incoming()->count))return false;out=result.highest;return true;};
  s.threat=[&](std::uintptr_t target,float& out,std::string&){data::AggroQuery result;if(dh2_aggro_query(&result,aggro->outgoing(),target,aggro->incoming()->count))return false;std::memcpy(&out,&result.threat_bits,4);return true;};
  s.current_character=[&](std::uintptr_t& out,std::string& e){target_providers::Handle16 handle;if(!r_.services.world_targets||r_.services.world_targets->get_handle(r_.actor->object->target.target,&handle))return false;
   const dh2::world::CanonicalObjectBorrowV1* object{};if(!objects_->resolve_handle_v4(handle,false,object,dh2::world::source_object_handle_null_assertion_v105,e))return false;if(!object){out=0;return true;}return object->as_character&&object->as_character(object->context,out,e);};
  auto settings=[&](unsigned field,float& value,std::string& e){SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<SourceWorldBorrowV61> world;
   if(!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=r_.services.world||!borrow_source_campaign_condition_world_v70(candidate,world,e)||!world->settings)return false;
   auto data=world->settings->borrow();const auto word=data?data.word(0,field):nullptr;if(!word)return fail("Required actual DesignSettings source first row");std::memcpy(&value,word,4);return true;};
  s.switch_factor=[&](float& out,std::string& e){return settings(1,out,e);};s.spotted_amount=[&](float& out,std::string& e){return settings(11,out,e);};
  s.view_radius=[&](float& out,std::string&){const auto ai=row();if(!ai)return false;out=ai->view_radius;return true;};s.no_aggro_radius=[&](float& out,std::string&){const auto ai=row();if(!ai)return false;out=ai->view_radius_no_aggro;return true;};
  s.spawn_radius=[&](float& out,std::string& e){dh2::world::GameObjectInitializationFieldsV62 f;if(!r_.actor->inherited_initialization_fields_v62(r_.actor,f,e))return false;auto scalar=f.scalar(0x143c);if(!scalar)return fail("Required actual authored Character spawn radius143c");out=*scalar;return true;};
  s.local_player=[&](std::uintptr_t& out,std::string& e){auto app=application_.lock();auto pm=app?app->source_player_manager_v59():nullptr;dh2::player::PlayerInfoFieldsV1* player{};if(!pm||!pm->get_local_player(0,true,player,e)||!player)return false;out=player->character660;return true;};
  s.has_relation=[&](std::uintptr_t id,bool& out,std::string&){out=false;for(unsigned i=0;i<aggro->outgoing()->count;++i)if(aggro->outgoing()->entries[i].character==id){out=true;break;}return true;};
  s.relationship=[&](AggroRelationV108 q,std::uintptr_t id,bool& out,std::string&){std::uintptr_t value{};auto& world=*r_.services.world_targets;
   const auto code=q==AggroRelationV108::Neutral?world.neutral(r_.actor->object->identity,id,&value):world.relationship(r_.actor->object->identity,id,q==AggroRelationV108::Enemy,&value);if(code){r_.error=world.error();return false;}out=value!=0;return true;};
  s.search=[&](bool tracked,float radius,unsigned flags,auto& out,std::string&){return search_aggro_v108(tracked,radius,flags,out);};
  s.clear=[&](auto id,std::string& e){return source_campaign_character_clear_aggro_v84(r_.services.world,r_.actor->object->identity,id,nullptr,e);};
  s.add=[&](auto id,float amount,std::string&){return add_aggro_v108(id,amount);};
  s.set_target=[&](auto id,std::string&){return dh2_character_ai_set_target(&r_.actor->object->target,id,0,&r_.actor->object->binding.services)==0;};
  s.raise=[&](unsigned event,auto id,std::string&){return raise(event,id);};
  s.noncharacter_assert=[&](std::string&){return fail("Original Character list60 yielded nonCharacter; actual assert selector required");};
  return character_update_aggro_v108(*fields,r_.actor->object->target,s,r_.error);
 }
 bool default_script_update_v108(){
  //Actual AISDefault3dc798: counter store, AI pause/timer, then Cmd_Stop.
  if(r_.collision_fields_v68.collision_ms<=199u)return true;
  r_.collision_fields_v68.collision_ms=0;
  r_.actor->ai_events.paused=1;
  if(timer(1000,0,0x31))return fail("Required SAME CharTimer AI_PauseUpdate delivery");
  return command_stop();
 }
 bool selected_script_update_v108(std::uintptr_t captured){
  ScriptSessionView view{};auto& a=*r_.actor;
  const bool found=r_.player_script_owner_v62?r_.player_script_owner_v62->session().owner().find(captured,view):a.session&&a.session->owner().find(captured,view);
  if(!found||!view.constructor_fields)return fail("Required captured SAME AIS OnUpdate receiver");
  const auto* table=character_script_source_virtuals_v101(view.kind);
  if(!table)return fail("Required actual constructor-selected AIS OnUpdate table");
  if(table[6]==0x3dc798u)return default_script_update_v108();
  if(table[6]==0x3dd588u){
   //Whole AISMonster.OnUpdate: qualified Default prefix, idle/no-target
   //gate, actual initial1450 AABB proximity and authored leash distance.
   if(!default_script_update_v108())return false;
   int current{};if(dh2_character_native_fsm_get_integer(&current,&a.machine->native_fsm(),0)!=1)return false;
   if((current!=3&&current!=13&&current!=18)||a.object->target.target)return true;
   dh2::world::GameObjectInitializationFieldsV62 f;if(!a.inherited_initialization_fields_v62(r_.actor,f,r_.error))return false;
   const auto initial=f.vector3(0x1450);if(!initial)return fail("Required SAME initial1450 for Monster leash");
   if(dh2::world::world_click_nearby_v1(a.runtime.subobjects.absolute_bounds,initial,1.f))return true;
   const float* point{};if(!position(a.object->identity,point))return false;
   //Preserve the captured initial/current components across GetCharAIId.
   const float x=initial[0]-point[0],y=initial[1]-point[1],z=initial[2]-point[2];
   int ai{};auto* rows=r_.design.ai();if(!rows||dh2_character_target_ai_id(&ai,r_.properties->resolved.data(),rows->rows.size()))return false;
   const auto row=data::ai_props(*rows,ai);if(!row)return fail("Required actual Monster leash AI row");
   const float threshold=row->leash_distance*row->leash_distance;
   const float distance=((x*x)+(y*y))+(z*z);
   return threshold<distance?move(initial):true;
  }
    if(table[6]==0x3dce64u){
   struct Calls {CampaignFsmV101& owner;std::uintptr_t captured;};Calls scope{*this,captured};
   const ScriptStatesCalls24 callbacks{&scope,
    [](void* raw,const char* name){auto& c=*static_cast<Calls*>(raw);return c.owner.call(c.captured,name,nullptr,0);},
    [](void* raw){return static_cast<Calls*>(raw)->owner.default_script_update_v108()?0:-1;}};
   const int code=r_.player_script_owner_v62?r_.player_script_owner_v62->session().owner().source_states_update_v108(captured,callbacks):a.session->owner().source_states_update_v108(captured,callbacks);
     return code==1||fail("Actual External.OnUpdate state/VM continuation failed");
    }
    if(table[6]==0x3de544u){
     //Whole AISFaery.OnUpdate. Default runs first; master and Visual2d8
     //are then captured. The source queries current faery TWICE on change.
     if(!default_script_update_v108())return false;
     auto pointers=a.source_ai_pointers_v105();if(!pointers)return fail("Required actual Faery master418/CharAI50");
     const auto master=pointers->master50,visual_identity=a.source_visual();if(!master||!visual_identity)return true;
     auto visual=r_.visual?r_.visual->visual():nullptr;
     if(!visual||reinterpret_cast<std::uintptr_t>(visual.get())!=visual_identity)return fail("Required SAME captured Faery Visual2d8");
     std::shared_ptr<void> session;std::int32_t* cached{};
     const bool borrowed=r_.player_script_owner_v62?r_.player_script_owner_v62->session().owner().source_faery_skin_cache_v111(captured,session,cached):
      a.session&&a.session->owner().source_faery_skin_cache_v111(captured,session,cached);
     if(!borrowed||!cached)return fail("Required actual AISFaery C1 skin-cachec4");
     const auto previous=*cached;
     auto selected=[&](std::int32_t& out){SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(r_.services.world,master,actual,r_.error))return false;auto owner=actual.character;
      if(!owner->save_fields)return fail("Required actual master Character14e8");const auto* cell=owner->save_fields->save_slot14e8();
      if(!cell)return fail("Required actual master Save pointer cell");if(!*cell){out=-1;return true;}
      if(!owner->save||*cell!=reinterpret_cast<std::uintptr_t>(owner->save.get()))return fail("Required SAME positive master Save14e8");
      std::int32_t tier{};if(!dh2::player::character_game_difficulty_v29({owner->actor,master,cell,owner->services.difficulty_global},tier,r_.error))return false;
      out=owner->save->current_faery(tier);return true;};
     std::int32_t current{};if(!selected(current))return false;if(previous==current)return true;
     if(!selected(current))return false;*cached=current; //3de578 source store BEFORE SetModularSkin
     return visual->source_set_modular_skin_v114(0,current,r_.error);
    }
  if(r_.services.ai_event_v101){AIEventRequest40 q{ai_event_ais_virtual,0x18,0,0,captured,table[6],0};unsigned ignored{};return r_.services.ai_event_v101(r_,q,ignored,r_.error);}
  return fail("Required selected AIS OnUpdate override");
 }
 bool on_update_v108(){
  if(!selected())return false;
  AIUpdateState80 projection{};
  auto refresh=[&](std::string& e){dh2::world::GameObjectInitializationFieldsV62 f;
   if(!r_.actor->inherited_initialization_fields_v62(r_.actor,f,e))return false;
   const auto* master=r_.actor->source_ai_pointers_v105();auto zoning=f.byte(0x2ee),updating=f.byte(0x85);auto position=f.vector3(0x1450);
   if(!master||!zoning||!updating||!position)return fail("Required SAME CharAI.OnUpdate zoning/master/initial1450 fields");
   projection.active=r_.actor->ai_events.active;projection.owner=r_.actor->object->identity;
   projection.target408=r_.actor->object->target.target;projection.target418=master->master50;
   projection.machine_present=r_.actor->machine!=nullptr;projection.state=-1;
   if(projection.machine_present&&dh2_character_native_fsm_get_integer(&projection.state,&r_.actor->machine->native_fsm(),0)!=1)return false;
   projection.visual=r_.actor->source_visual();projection.visual_node=r_.visual&&r_.visual->visual()?r_.visual->visual()->root_identity():0;
   projection.zoned=*zoning;projection.flag85=*updating;std::copy_n(position,3,projection.saved_position);return true;};
  if(!refresh(r_.error))return false;
  struct Context {CampaignFsmV101& owner;decltype(refresh)& synchronize;};Context context{*this,refresh};
  const AIUpdateServices24 services{&context,[](void* raw,AIUpdateState80*,const AIUpdateRequest32* q,unsigned* out){
   auto& c=*static_cast<Context*>(raw);auto& t=c.owner;if(!q||!out)return -1;*out=0;bool okay=false;
   switch(q->service){
    case ai_update_active:okay=t.selected_script_update_v108(q->subject);break;
    case ai_update_disable_zoning:case ai_update_enable_zoning:okay=source_campaign_character_zoning_v108(t.r_.services.world,q->subject,q->service==ai_update_enable_zoning,t.r_.error);break;
    case ai_update_sync_visibility:if(t.r_.actor->source_visual()!=q->subject)return -1;okay=source_campaign_character_sync_visibility_v86(t.r_.services.world,t.r_.actor->object->identity,t.r_.error);break;
    case ai_update_is_zonable:{bool value{};okay=source_campaign_character_is_zonable_v104(t.r_.services.world,q->subject,value,t.r_.error);*out=value;break;}
    case ai_update_is_dead:if(!t.r_.life)return -1;*out=t.r_.life->dead;okay=true;break;
    case ai_update_set_position:okay=t.r_.set_position({q->position[0],q->position[1],q->position[2]},q->argument!=0,t.r_.error);break;
    case ai_update_node_set_position:{auto visual=t.r_.visual?t.r_.visual->visual():nullptr;if(!visual||visual->root_identity()!=q->subject)return -1;
     //Whole ISceneNode.setPosition59712c: relative translation+flags11c|8.
     //No animation sample, root absolute refresh or scene tick is introduced.
     std::copy_n(q->position,3,visual->binding().root.position);visual->scene_flags()|=8u;okay=true;break;}
    default:return -1;
   }
   return okay&&c.synchronize(t.r_.error)?0:-1;
  },255u,0};
  AIUpdateResult16 result{};const auto status=dh2_character_ai_update(&result,&projection,&services);
  return status==0||fail("Actual CharAI.OnUpdate ordered source delivery failed");
 }
 bool update_ai_v108(){
  AIFrameOwner48 owner{};AIFrameState32 frame{};
  auto refresh=[&]{ControllerCommandState32* control{};if(!controller(control))return false;
   auto zoning=r_.actor->source_bool_field(0x2ee),inside=r_.actor->source_bool_field(0x2f0),updated=r_.actor->source_bool_field(0x88);
   if(!zoning||!inside||!updated)return fail("Required actual CharAI frame2ee/2f0/88 cells");
   owner={r_.actor->object->identity,control->controller,state().flags,control->forced,control->locked,*zoning,*inside,*updated,0,0};
   frame={r_.actor->ai_events.ai,&owner,r_.actor->ai_events.paused,control->global_blocked,0,0};return true;};
  if(!refresh())return false;
  struct Context {CampaignFsmV101& t;AIFrameOwner48& owner;decltype(refresh)& synchronize;};Context context{*this,owner,refresh};
  const AIFrameServices24 services{&context,[](void* raw,AIFrameState32*,const AIFrameRequest16* q,unsigned* out){
   auto& c=*static_cast<Context*>(raw);auto& t=c.t;if(!q||!out)return -1;*out=0;
   dh2::world::GameObjectInitializationFieldsV62 fields;
   if(!t.r_.actor->inherited_initialization_fields_v62(t.r_.actor,fields,t.r_.error))return -1;
   auto* updated=fields.byte?fields.byte(0x88):nullptr;if(!updated)return -1;
   //The coordinator's source88=1 prefix becomes visible before _UpdateTarget
   //and its nested Character events, not at the end of the frame.
   *updated=static_cast<std::uint8_t>(c.owner.updated88);bool okay=false;
   switch(q->service){
    case ai_frame_is_zonable:{bool value{};okay=source_campaign_character_is_zonable_v104(t.r_.services.world,q->subject,value,t.r_.error);*out=value;break;}
    case ai_frame_update_target:okay=t.update_target_v108();break;
    case ai_frame_update_master:okay=t.update_master_v108();break;
    case ai_frame_update_aggro:okay=t.update_aggro_v108();break;
    case ai_frame_on_update:okay=t.on_update_v108();break;
    default:return -1;
   }
   return okay&&c.synchronize()?0:-1;
  },31,0};
  AIFrameResult16 result;const auto status=dh2_character_ai_frame(&result,&frame,&services);
  return status==0||fail("Actual whole CharAI.Update ordered delivery failed");
 }
 bool update_target_v108(){
  if(!geometry())return false;
  if(!target_frame_v108_){WorldTargetFrameServicesV2 services;services.context=this;
   services.machine_state=[](void* raw,std::uintptr_t id,unsigned* out){auto& t=*static_cast<CampaignFsmV101*>(raw);SourceCampaignCharacterBorrowV62 actor;
    if(!out||!borrow_source_campaign_character_v62(t.r_.services.world,id,actor,t.r_.error)||!actor.character->actor->machine)return -1;
    std::int32_t state{};if(dh2_character_native_fsm_get_integer(&state,&actor.character->actor->machine->native_fsm(),0)!=1)return -1;*out=static_cast<unsigned>(state);return 0;};
   services.raise_event=[](void* raw,std::uintptr_t id,unsigned event,std::uintptr_t payload){auto& t=*static_cast<CampaignFsmV101*>(raw);return id==t.r_.actor->object->identity&&t.raise(event,payload)?0:-1;};
   services.close_range=[](void* raw,std::uintptr_t id,std::uintptr_t target,unsigned* out){auto& t=*static_cast<CampaignFsmV101*>(raw);int value{};
    if(!out||!t.geometry_||!t.geometry_->close_range_v38(id,target,t.r_.actor->object->target.target,value,t.r_.error))return -1;*out=static_cast<unsigned>(value);return 0;};
   target_frame_v108_=std::make_unique<CharacterWorldTargetFrameV2>(*r_.services.world_targets,*geometry_,*r_.design.ai(),services);
  }
  if(target_frame_v108_->update(r_.actor->object->target)){r_.error=target_frame_v108_->error();return false;}return true;
 }
 bool update_animator(){
  //Actual CharAnimator.Update3caf70..cc toggles its source5c users latch
  //from flags520 bit200 BEFORE any scheduler work; Scene time is untouched.
  const bool enabled=(state().flags&0x200u)!=0;
  if(r_.visual&&r_.visual->animator()){bool actual{};if(!r_.visual->animator()->source_update_user_state_v107(state().flags,actual,r_.error))return false;}
  if(!enabled)return true;
  if(!r_.visual||!r_.visual->animator()||!r_.services.animation_tables||!r_.services.random)return fail("Required SAME canonical CharAnimator update");
  auto& source=*r_.services.random;data::AnimationRandom random{source.seed,source.calls};auto* previous=r_.animation_random_inflight_v101;r_.animation_random_inflight_v101=&random;
  struct Commit{Record& r;data::AnimationRandom& value;data::AnimationRandom* previous;~Commit(){r.services.random->seed=value.seed;r.services.random->calls=value.calls;r.animation_random_inflight_v101=previous;}}commit{r_,random,previous};
  return r_.visual->animator()->animator_phase(*r_.services.animation_tables,random,1.f,r_.error);
 }
 bool enabled_event(bool enabled){
  dh2::world::GameObjectInitializationFieldsV62 f;if(!r_.actor->inherited_initialization_fields_v62(r_.actor,f,r_.error))return false;
  auto update=f.byte(0x85),disabled=f.byte(0x373);if(!update||!disabled)return fail("Required source enabled85/disabled373 cells");
  if(enabled){if(!source_campaign_character_set_visible_v96(r_.services.world,r_.actor->object->identity,true,r_.error))return false;*update=1;r_.actor->runtime.object.motion.object_flags|=8;}
  else{*update=0;if(!source_campaign_character_set_visible_v96(r_.services.world,r_.actor->object->identity,false,r_.error))return false;r_.actor->runtime.object.motion.object_flags&=~8u;}
  const auto physical=r_.actor->position_fields_v7().physical2dc;
  if(physical){if(!r_.physical_owner_v62||physical!=reinterpret_cast<std::uintptr_t>(r_.physical_owner_v62.get()))return fail("Required SAME physical2dc enable receiver");
   if(!(enabled?r_.physical_owner_v62->enable_filter():r_.physical_owner_v62->disable_filter())){r_.error=r_.physical_owner_v62->error();return false;}}
  *disabled=enabled?0:1;return enabled||stop();
 }
 bool zone_entered(){
  dh2::world::GameObjectInitializationFieldsV62 f;if(!r_.actor->inherited_initialization_fields_v62(r_.actor,f,r_.error))return false;
  auto entered=f.byte(0x2f0),zoning=f.byte(0x2ee),updating=f.byte(0x85);if(!entered||!zoning||!updating)return fail("Required SAME source ZoneEntered cells");
  *entered=1;
  if(*zoning&&r_.actor->source_visual()){auto visible=f.byte(0x80);if(!visible)return fail("Required actual source visible80 before ZoneEntered");if(*visible){
    if(!source_campaign_character_sync_visibility_v86(r_.services.world,r_.actor->object->identity,r_.error))return false;}}
  bool zonable{};if(!source_campaign_character_is_zonable_v104(r_.services.world,r_.actor->object->identity,zonable,r_.error))return false;
  // The actual Character vptr inherits ObjectBase.setUpdating33dcf0 at3c.
  // Its captured callable is applied after virtualc4, not inferred from2ed.
  *updating=(zonable&&*zoning)?*entered:1;return true;
 }
 bool zone_exited(){
  dh2::world::GameObjectInitializationFieldsV62 f;if(!r_.actor->inherited_initialization_fields_v62(r_.actor,f,r_.error))return false;
  auto entered=f.byte(0x2f0),zoning=f.byte(0x2ee),updating=f.byte(0x85);if(!entered||!zoning||!updating)return fail("Required SAME source ZoneExited cells");
  *entered=0;if(*zoning&&r_.actor->source_visual()&&!source_campaign_character_sync_visibility_v86(r_.services.world,r_.actor->object->identity,r_.error))return false;
  bool zonable{};if(!source_campaign_character_is_zonable_v104(r_.services.world,r_.actor->object->identity,zonable,r_.error))return false;
  *updating=(zonable&&*zoning)?*entered:1;return true;
 }
 bool command_stop(){ControllerCommandState32* control{};if(!controller(control))return false;if(!control->forced&&(control->global_blocked||control->locked))return true;bool is_remote{};if(!remote(is_remote))return false;if(is_remote)return true;return stop()&&raise(0x3f,0);}
 bool command_heading(const float* direction){return heading_v115(direction,0,0);}
 bool heading_v115(const float* direction,unsigned kind,std::uintptr_t target){ControllerCommandState32* control{};if((kind!=2&&!direction)||!controller(control))return false;
  dh2::physical::NativeBody* body=nullptr;const auto physical=r_.actor->position_fields_v7().physical2dc;
  if(physical){if(!r_.physical_owner_v62||physical!=reinterpret_cast<std::uintptr_t>(r_.physical_owner_v62.get()))return fail("Required SAME physical2dc heading receiver");body=&r_.physical_owner_v62->native();}
  const CharacterHeadingServicesV1 services{this,
   [](void* raw,std::uintptr_t id,const float*& out,std::string&){return static_cast<CampaignFsmV101*>(raw)->position(id,out);},
   [](void* raw,bool& out,std::string&){return static_cast<CampaignFsmV101*>(raw)->remote(out);},
   [](void* raw,bool& out,std::string&){out=(static_cast<CampaignFsmV101*>(raw)->state().flags&2)!=0;return true;},
   [](void* raw,unsigned event,std::string&){auto& t=*static_cast<CampaignFsmV101*>(raw);t.r_.actor->runtime.rotation.heading_angle=t.r_.actor->runtime.controller.heading.angle;t.state().heading_active=t.r_.actor->runtime.controller.heading.active;return t.raise(event,0);}};
  auto& a=*r_.actor;CharacterHeadingOwnerV1 owner(*control,state(),a.object->target,a.object->binding.services,a.runtime.controller,a.runtime.path,a.runtime.subobjects.position,a.runtime.subobjects.destination,body,services);
  if(kind==1)return owner.character_head_towards(direction,r_.error);
  if(kind==2){const CharacterHeadObjectBorrowV2 source{control,&owner,&a.runtime.controller.heading.active,dh2::world::canonical_vec3_origin_v1().data(),services};return character_command_head_object_v2(source,target,r_.error);}
  return owner.command_head_towards(direction,r_.error);
 }
};
bool CampaignFsmV101::idle(){
 auto& a=*r_.actor;IdleCharacter40 self{a.object->identity,a.controller->identity(),{a.runtime.subobjects.position[0],a.runtime.subobjects.position[1],a.runtime.subobjects.position[2]},state().elapsed_ms,state().heading_active,0};
 struct Context{CampaignFsmV101& t;std::map<std::uintptr_t,IdleCharacter40> peers;};Context delivery{*this,{}};
 const IdleUpdateServices16 services{&delivery,[](void* raw,IdleCharacter40*,const IdleUpdateRequest32* q,IdleUpdateResponse16* out){auto& d=*static_cast<Context*>(raw);auto& t=d.t;if(!q||!out)return -1;
  auto app=t.application_.lock();auto pm=app?app->source_player_manager_v59():nullptr;
  switch(q->service){
   case idle_raise_event:return t.raise(q->argument,0)?0:-1;
   case idle_is_player:{bool player{};if(!t.r_.is_player(player,t.r_.error))return -1;out->word=player;return 0;}
   case idle_online_disabled:{bool online{};if(!t.r_.services.online_byte5||!t.r_.services.online_byte5(online,t.r_.error))return -1;out->word=online;return 0;}
   case idle_delay:case idle_distance:{int value{};if(!t.constant("CharacterDesign",q->service==idle_delay?"PlayerInterPenetration_Delay":"PlayerInterPenetration_Dist",value))return -1;out->word=static_cast<unsigned>(value);return 0;}
   case idle_player_count:{if(!pm||!pm->manager())return -1;int count{};if(!pm->manager()->num_players(count,t.r_.error))return -1;out->word=static_cast<unsigned>(count);out->identity=reinterpret_cast<std::uintptr_t>(pm->manager());return 0;}
   case idle_player_character:{if(!pm||q->subject!=reinterpret_cast<std::uintptr_t>(pm->manager()))return -1;dh2::player::PlayerInfoFieldsV1* info{};if(!pm->manager()->get_player(static_cast<int>(q->argument),false,info,t.r_.error)||!info)return -1;
    if(!info->character660){out->identity=0;return 0;}SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(t.r_.services.world,info->character660,actual,t.r_.error))return -1;
    auto& r=*actual.character;auto& a=*r.actor;auto& peer=d.peers[info->character660];peer={a.object->identity,a.controller->identity(),{a.runtime.subobjects.position[0],a.runtime.subobjects.position[1],a.runtime.subobjects.position[2]},a.machine->state().elapsed_ms,a.machine->state().heading_active,0};out->identity=reinterpret_cast<std::uintptr_t>(&peer);return 0;}
   case idle_get_state:{SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(t.r_.services.world,q->subject,actual,t.r_.error))return -1;int state{};if(dh2_character_native_fsm_get_integer(&state,&actual.character->actor->machine->native_fsm(),0)!=1)return -1;out->word=static_cast<unsigned>(state);return 0;}
   case idle_move_to:{if(q->subject==t.r_.actor->controller->identity())return t.move(q->point)?0:-1;
    for(const auto& peer:d.peers)if(peer.second.controller==q->subject){SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(t.r_.services.world,peer.first,actual,t.r_.error))return -1;auto other=std::static_pointer_cast<CampaignFsmV101>(actual.character->fsm_context_v101);return other&&other->move(q->point)?0:-1;}return -1;}
    default:return -1;
  }}};
  return dh2_character_idle_update(&self,&services)==1;
}
bool CampaignFsmV101::animation_consumer(unsigned operation){
 auto& a=*r_.actor;auto& ai=a.animation_ai;ControllerCommandState32* command{};if(!controller(command))return false;
 ai.owner=a.object->identity;ai.controller=command->controller;ai.target=a.object->target.target;ai.look_target=a.object->target.target;
 auto* heading=a.source_heading_enabled412_v101();auto* click=a.source_click_fields_v101();if(!heading||!click)return fail("Required actual Character412/413 animation fields");
 ai.owner_flags=state().flags;ai.seeking=*heading;ai.target_sticky=click->click_target413;ai.animation_depth=static_cast<int>(playback().animation_depth());std::copy_n(a.runtime.subobjects.position,3,ai.owner_position);
 reload_animation_flags();
 const AnimationAIServices16 services{this,[](void* raw,AnimationAIState96* actual,const AnimationAIRequest32* q,AnimationAIResponse16* out){auto& t=*static_cast<CampaignFsmV101*>(raw);if(!q||!out||actual!=&t.r_.actor->animation_ai)t.unavailable("Wrong canonical animation consumer receiver");bool okay=true;
  t.publish_animation_flags();struct Reload{CampaignFsmV101& owner;~Reload(){owner.reload_animation_flags();}}reload{t};
  switch(q->service){
   case ai_animation_state:{int state{};okay=dh2_character_native_fsm_get_integer(&state,&t.r_.actor->machine->native_fsm(),0)==1;out->word=static_cast<unsigned>(state);break;}
   case ai_animation_step_index:out->word=t.playback().step_index();break;
   case ai_animation_step_count:out->word=t.playback().step_count(*t.r_.services.animation_tables);break;
   case ai_has_combo_attack:{const auto& tables=*t.r_.services.animation_tables;const auto table=dh2_character_animation_table_id(t.r_.properties->resolved[2],static_cast<int>(tables.characters.size()));const auto at=std::find(tables.state_names.begin(),tables.state_names.end(),"Attack");
    if(table<0||std::size_t(table)>=tables.characters.size()||at==tables.state_names.end())t.unavailable("Required actual combo Attack table");const auto& field=tables.characters[table].fields[at-tables.state_names.begin()];if(field.size()!=1)t.unavailable("Required scalar combo Attack sequence");const int index=field[0];out->word=dh2_character_animation_has_combo(index,static_cast<int>(tables.sequences.size()),index>=0&&std::size_t(index)<tables.sequences.size()?tables.sequences[index].type:0);break;}
   case ai_target_position:{const float* p{};okay=t.position(q->subject,p);if(okay)std::copy_n(p,3,out->position);break;}
   case ai_target_is_dead:{WorldTargetActorBorrowV1 target;okay=t.r_.services.world_targets&&t.r_.services.world_targets->actor(q->subject,&target)==0&&target.life;if(okay)out->word=target.life->dead;break;}
   case ai_melee_radius_squared:{float radius{};okay=t.geometry()&&t.geometry_->melee_radius(q->subject,radius,t.r_.error);if(okay){radius*=radius;std::memcpy(&out->word,&radius,4);}break;}
   case ai_owner_can_range_attack:{std::int32_t value{};okay=t.geometry()&&t.geometry_->read(q->subject,actual->target,WorldAIAttackQueryV1::CharacterCanRangeAttack,value,t.r_.error);out->word=static_cast<unsigned>(value);break;}
   case ai_pre_attack_virtual:okay=t.selected()&&t.selected_&&t.r_.actor->ai_events.ais_virtuals&&t.r_.actor->ai_events.ais_virtuals[0xa4/4]==0x3dbef8;/*Captured inherited PreAttack is a literal return; no new range gate.*/break;
   case ai_controller_look_at:{ControllerCommandState32* command{};okay=t.controller(command);if(okay&&(command->forced||(!command->global_blocked&&!command->locked)))okay=t.look(q->payload);break;}
   case ai_controller_move_to:{const float* p{};okay=t.position(q->payload,p)&&t.move(p);break;}
   case ai_clear_nonsticky_target:okay=actual->target_sticky||dh2_character_clear_target(&t.r_.actor->object->target,&t.r_.actor->object->binding.services)==0;actual->target=t.r_.actor->object->target.target;break;
   case ai_animation_set_step:t.playback().set_step(q->argument);break;
   case ai_animation_skip_next_step:t.playback().skip_next_step();break;
   case ai_animation_stop_loop:t.playback().stop_loop(q->argument!=0);break;
   case ai_character_event:okay=t.raise(q->argument,q->payload);break;
   default:okay=t.r_.services.animation_consumer_v101&&t.r_.services.animation_consumer_v101(t.r_,*q,*out,t.r_.error);break;
  }
  if(!okay)t.unavailable("Required actual canonical animation consumer leaf");}};
 const int result=dh2_character_animation_ai(&ai,operation,&services);publish_animation_flags();return result==1;
}
bool CampaignFsmV101::relay(const char* text){
 if(!selected()||!text)return false;if(!selected_)return true;
 std::shared_ptr<void> pin;dh2::fx::CharacterMeshFxOwnerV4* fx{};if(!borrow_source_campaign_fx_v77(r_.services.world,pin,fx,r_.error)||!fx)return false;fx_pin_=std::move(pin);
 auto visual=r_.visual?r_.visual->visual():nullptr;visual_=r_.actor->source_visual();scene_=visual?&visual->scene():nullptr;
 cached_floor_={floors_.get(),&r_.actor->runtime.object.motion.floor};
 if(r_.player_script_owner_v62){auto& session=r_.player_script_owner_v62->session();
  if(!player_events_){AnimationEventBorrowV1 borrow{r_.actor->object->identity,&session.owner(),&selected_,&lag_,&r_.view,r_.services.world_targets.get(),&visual_,scene_,&root_,&cached_floor_,animation_floor_type_v1};player_events_=std::make_unique<CharacterAnimationEventOwnerV1>(r_.actor->ai_events,borrow,fx->source_libraries_v63()->source_tables(),nullptr);player_events_->bind_mesh_v4(*fx);}
  if(!player_events_->relay(text,session.current_skill_callback_scope())){r_.error=player_events_->error();return false;}return true;
 }
 if(!r_.actor->session)return fail("Required actual NPC selected animation ScriptOwner");
 if(!npc_events_){NpcAnimationEventBorrowV1 borrow{&r_.actor->ai_events,&r_.actor->session->owner(),r_.actor->object->identity,&lag_,&r_.view,r_.services.world_targets.get(),&visual_,scene_,&root_,&cached_floor_,animation_floor_type_v1};npc_events_=std::make_unique<NpcAnimationEventOwnerV1>(borrow,fx->source_libraries_v63()->source_tables(),nullptr);npc_events_->bind_mesh_v4(*fx);}
 if(!npc_events_->relay(text,r_.actor->session->current_skill_callback_scope())){r_.error=npc_events_->error();return false;}return true;
}
int CampaignFsmV101::authored(const animation::TriggeredEvent& trigger){
 if(!trigger.name)return -1;lag_=trigger.lag_ms;
 const MeleeAnimationServicesV1 services{this,[](void* raw,const MeleeAnimationRequestV1* q,MeleeAnimationResponseV1* out){auto& t=*static_cast<CampaignFsmV101*>(raw);if(!q||!out)return -1;
  switch(q->operation){
   case melee_event_step_index:out->value=static_cast<int>(t.playback().step_index());return 0;
   case melee_event_step_count:out->value=static_cast<int>(t.playback().step_count(*t.r_.services.animation_tables));return 0;
   case melee_event_state:out->value=t.state().current;return 0;
   case melee_event_relay:return t.relay(q->text)?0:-1;
   case melee_event_mesh_fx:{const float* position{};std::shared_ptr<void> pin;dh2::fx::CharacterMeshFxOwnerV4* fx{};const std::string authored="fx_"+std::string(q->text);return t.position(t.r_.actor->object->identity,position)&&borrow_source_campaign_fx_v77(t.r_.services.world,pin,fx,t.r_.error)&&fx->animation_event(authored.c_str(),position,t.r_.error)?0:-1;}
   case melee_event_sound_fx:return t.r_.services.named_sound_v101&&t.r_.services.named_sound_v101(t.r_,q->text,t.r_.error)?0:-1;
   case melee_event_object_animation:{const auto& tables=*t.r_.services.animation_tables;const auto at=std::find(tables.sequence_names.begin(),tables.sequence_names.end(),q->text);return at==tables.sequence_names.end()?0:t.animation(static_cast<int>(at-tables.sequence_names.begin()))?0:-1;}
   case melee_event_skill:{unsigned result{};if(t.r_.player_script_owner_v62)return t.r_.player_script_owner_v62->native_skill_animation_event(&result);return t.npc_skill_ai_v115(skill_ai_event_v3,0,result);}
   case melee_event_spell:return t.spell_callback(skill_use_v3);
   case melee_event_interact:{
    if(!t.selected())return -1;AIEventRequest40 request{};request.service=ai_event_virtual;request.operation=0xa0;request.subject=t.r_.actor->ai_events.ai;request.callee=0x3d0d38;
    unsigned result{};return ai_service(&t,&t.r_.actor->ai_events,&request,&result);
   }
   case melee_event_can_range:{std::int32_t result{};if(!t.geometry()||!t.geometry_->read(t.r_.actor->object->identity,t.r_.actor->object->target.target,WorldAIAttackQueryV1::CharacterCanRangeAttack,result,t.r_.error))return -1;out->value=result;return 0;}
   default:return t.r_.services.melee_animation_v101&&t.r_.services.melee_animation_v101(t.r_,*q,*out,t.r_.error)?0:-1;
  }},nullptr};
 //The dispatcher owns the reached Debug CString/Get/Destroy prefix. Adapt
 //the actual retained Debug owner rather than replacing it with log output.
 SkillAttackNativeServicesV6 debug{this,[](void* raw,const SkillAttackNativeRequestV6* q,std::uintptr_t* out){auto& t=*static_cast<CampaignFsmV101*>(raw);return t.debug_request(q,out);}};
 auto actual=services;actual.debug=&debug;MeleeAnimationOutputV1 out;const int status=dh2_character_melee_animation_event_v1(&out,trigger.name,&actual);return status? -1:0;
}
int CampaignFsmV101::skill_operation(SkillStateV4* s,const SkillStateRequest32V4& q,SkillStateResponse16V4& out){
 if(s!=&skill_||s->state!=&state()||s->character!=r_.actor->object->identity)return -1;
 switch(q.operation){
  case skill_state_debug_load_v4:{const SkillAttackNativeRequestV6 source{skill_attack_debug_load_v6,0,0,nullptr};std::uintptr_t result{};return debug_request(&source,&result);}
  case skill_state_debug_construct_v4:{const SkillAttackNativeRequestV6 source{skill_attack_string_construct_v6,0,0,reinterpret_cast<const char*>(q.payload)};return debug_request(&source,&out.identity);}
  case skill_state_debug_get_v4:case skill_state_debug_destroy_v4:{const SkillAttackNativeRequestV6 source{q.operation==skill_state_debug_get_v4?skill_attack_debug_get_v6:skill_attack_string_destroy_v6,0,q.subject,nullptr};std::uintptr_t result{};const int status=debug_request(&source,&result);out.word=static_cast<unsigned>(result);return status;}
  case skill_state_stop_v4:return stop()?0:-1;
  case skill_state_raise_v4:return raise(q.value,0)?0:-1;
  case skill_state_animation_v4:{int sequence{};std::memcpy(&sequence,&q.value,4);return animation(sequence)?0:-1;}
  case skill_state_speed_v4:{float speed{};std::memcpy(&speed,&q.value,4);return playback().set_speed(speed,r_.error)?0:-1;}
  case skill_state_pin_v4:return pin(true,q.subject)?0:-1;
  case skill_state_unpin_v4:return pin(false,q.subject)?0:-1;
  case skill_state_cancel_sneaking_v4:return cancel_sneaking()?0:-1;
  case skill_state_timer_v4:return timer(q.value,0,static_cast<int>(q.index));
  case skill_state_monster_v4:case skill_state_miniboss_v4:case skill_state_boss_v4:{const auto* ai=r_.design.ai()?dh2::data::ai_props(*r_.design.ai(),r_.properties->resolved[1]):nullptr;if(!ai)return -1;out.word=q.operation==skill_state_monster_v4?ai->type==4:q.operation==skill_state_miniboss_v4?(ai->flags>>1)&1:(ai->flags>>2)&1;return 0;}
  case skill_state_row_v4:{if(!r_.services.skills)return -1;const auto& lists=r_.services.skills.lists();int selected=r_.properties->resolved[28];if(selected<0||std::size_t(selected)>=lists.size())selected=3;
   if(std::size_t(selected)>=lists.size()||q.index>=lists[selected].size())return -1;const auto id=lists[selected][q.index];const auto& rows=r_.services.skills.skills();if(id<0||std::size_t(id)>=rows.size())return -1;out.identity=reinterpret_cast<std::uintptr_t>(&rows[id].scalar);return 0;}
  case skill_state_constant_v4:{int value{};if(!constant("AnimStancedAnim","SL__LIST_IPHONE",value))return -1;out.word=static_cast<unsigned>(value);return 0;}
  case skill_state_stance_v4:{if(!refresh(5))return -1;out.word=static_cast<unsigned>(r_.actor->facts.stance);return 0;}
  case skill_state_state_event_v4:return r_.actor->machine->event(static_cast<int>(q.value),q.payload)<0?-1:0;
  case skill_state_transition_v4:return r_.actor->machine->transition(static_cast<int>(q.index),static_cast<int>(q.value),q.payload)<0?-1:0;
  case skill_state_step_index_v4:out.word=playback().step_index();return 0;
  case skill_state_step_count_v4:out.word=playback().step_count(*r_.services.animation_tables);return 0;
  case skill_state_current_v4:{int current{};if(dh2_character_native_fsm_get_integer(&current,&r_.actor->machine->native_fsm(),0)!=1)return -1;out.word=static_cast<unsigned>(current);return 0;}
  case skill_state_use_v4:{unsigned result{};if(!r_.player_script_owner_v62)return npc_skill_ai_v115(skill_ai_event_v3,0,result);const int status=r_.player_script_owner_v62->native_skill_animation_event(&result);if(status)r_.error=r_.player_script_owner_v62->error();return status;}
  case skill_state_named_prefix_v4:{const auto* name=reinterpret_cast<const char*>(q.payload);if(!name)return -1;
   if(q.value==0)return relay(name)?0:-1;
   if(q.value==1){const auto& names=r_.services.animation_tables->sequence_names;const auto at=std::find(names.begin(),names.end(),name);if(at==names.end())return fail("Required actual named animation sequence"),-1;return animation(static_cast<int>(at-names.begin()))?0:-1;}
   if(q.value==2){std::shared_ptr<void> pin;dh2::fx::CharacterMeshFxOwnerV4* fx{};const float* p{};const std::string authored="fx_"+std::string(name);return position(r_.actor->object->identity,p)&&borrow_source_campaign_fx_v77(r_.services.world,pin,fx,r_.error)&&fx->animation_event(authored.c_str(),p,r_.error)?0:-1;}
   if(q.value==3)return r_.services.named_sound_v101&&r_.services.named_sound_v101(r_,name,r_.error)?0:-1;return -1;
  }
  case skill_state_other_event_v4:{animation::TriggeredEvent trigger{lag_,reinterpret_cast<const char*>(q.payload)};return authored(trigger);}
  default:return -1;
 }
}
int CampaignFsmV101::ai_service(void* raw,AIEventState64* ai,const AIEventRequest40* q,std::uint32_t* out){
 auto& t=*static_cast<CampaignFsmV101*>(raw);if(!q||!out||ai!=&t.r_.actor->ai_events)return -1;*out=0;
 if(q->service==ai_event_state_getter){if(q->subject!=reinterpret_cast<std::uintptr_t>(&t.r_.actor->machine->native_fsm()))return -1;int state{};if(dh2_character_native_fsm_get_integer(&state,&t.r_.actor->machine->native_fsm(),0)!=1)return -1;*out=static_cast<unsigned>(state);return 0;}
 if(q->service==ai_event_state_event){if(q->subject!=reinterpret_cast<std::uintptr_t>(&t.r_.actor->machine->native_fsm()))return -1;auto payload=q->payload;
  //The logical CSSkill body consumes the authored string; ARM OnEvent gets
  //the TriggeredEvent record and loads its string member. Keep that producer.
  if(q->event==0x28&&payload)payload=reinterpret_cast<std::uintptr_t>(reinterpret_cast<const animation::TriggeredEvent*>(payload)->name);
  const auto code=t.r_.actor->machine->event(static_cast<int>(q->event),payload);*out=static_cast<unsigned>(code);
  if(code<0&&t.r_.error.empty()){
   const auto& machine=t.r_.actor->machine->owner().machine();
   const auto* current=machine.current_index>=0&&static_cast<unsigned>(machine.current_index)<machine.state_count?&machine.states[machine.current_index]:nullptr;
   t.r_.error="Required canonical FSM event: code "+std::to_string(code)+" event "+std::to_string(q->event)+
    " state "+std::to_string(current?current->id:-1)+" source "+std::to_string(current?current->on_event:0);
  }
  return code<0?-1:0;
 }
 if(q->service==ai_event_timer_id){if(q->callee!=0x3db288)return -1;const auto* store=t.r_.player_script_owner_v62?&t.r_.player_script_owner_v62->session().timers():t.r_.actor->session?&t.r_.actor->session->timers():nullptr;
  if(!store)return -1;for(unsigned i=0;i<store->count;++i)if(q->subject==reinterpret_cast<std::uintptr_t>(store->slots+i)){*out=store->slots[i].id;return 0;}return -1;
 }
 if(q->service==ai_event_virtual&&q->subject==ai->ai){
  if((q->operation==0x84&&q->callee==0x3d0c34)||(q->operation==0x88&&q->callee==0x3d0c58)){
   if(!ai->active)return 0;if(!ai->ais_virtuals)return -1;AIEventRequest40 relay=*q;relay.service=ai_event_ais_virtual;relay.subject=ai->active;relay.callee=ai->ais_virtuals[q->operation/4];return ai_service(&t,ai,&relay,out);
  }
  if(q->operation==0xa0&&q->callee==0x3d0d38){
   //CharAI.OnInteract snapshots the selected active receiver; NULL returns.
   if(!ai->active)return 0;if(!ai->ais_virtuals)return -1;
   AIEventRequest40 selected=*q;selected.service=ai_event_ais_virtual;selected.subject=ai->active;selected.callee=ai->ais_virtuals[0xa0/4];
   return ai_service(&t,ai,&selected,out);
  }
  if(q->operation==0x20&&q->callee==0x3d0bec)return dh2_character_ai_state_changed(ai,static_cast<int>(q->argument),static_cast<int>(static_cast<unsigned>(q->payload)),&t.events_);
  if(q->operation==0x98&&q->callee==0x3d0ce8)return dh2_character_ai_end_anim(ai,&t.events_);
  if(q->operation==0x90&&q->callee==0x3d0ca0){if(!t.selected())return -1;AIEventResult16 result;return dh2_character_ai_event_script_timer(&result,ai,q->argument,&t.events_)?-1:0;}
  if(q->operation==0x24&&q->callee==0x3d1000){if(!t.r_.death_v84)return t.fail("Required actual canonical OnDied owner"),-1;const auto* scope=t.r_.player_script_owner_v62?t.r_.player_script_owner_v62->session().current_skill_callback_scope():t.r_.actor->session?t.r_.actor->session->current_skill_callback_scope():nullptr;return t.r_.death_v84->raise(q->payload,scope,t.r_.error)?0:-1;}
 }
 if(q->service==ai_event_ais_virtual){
  //These exact AISDefault methods are literal BXLR source bodies. An
  //override is dispatched by its captured identity and never bypassed here.
  if((q->callee>=0x3dbe78&&q->callee<=0x3dbeec&&!(q->callee&3))||
     (q->callee>=0x3dbef8&&q->callee<=0x3dbf04&&!(q->callee&3)))return 0;
  if(q->callee==0x3dbef0){*out=1;return 0;}
  if(q->callee==0x3dc1b0){
   auto& target=t.r_.actor->object->target;if(!target.target)return 0;
   std::uintptr_t character{};if(!source_campaign_object_as_character_v114(t.r_.services.world,target.target,character,t.r_.error))return -1;
   if(character){SourceCampaignCharacterBorrowV62 other;if(!borrow_source_campaign_character_v62(t.r_.services.world,character,other,t.r_.error))return -1;
    const auto* row=other.character->design.ai()?dh2::data::ai_props(*other.character->design.ai(),other.character->properties->resolved[1]):nullptr;if(!row)return -1;if(row->type==4)return 0;
   }
   const auto captured=target.target;std::uintptr_t actual{};if(!source_campaign_object_as_character_v114(t.r_.services.world,captured,actual,t.r_.error))return -1;
   if(!(actual?source_campaign_character_interact_v114(t.r_.services.world,actual,t.r_.actor->object->identity,t.r_.error):source_campaign_noncharacter_interact_v114(t.r_.services.world,captured,t.r_.actor->object->identity,t.r_.error)))return -1;
   if(!target.target)return t.fail("Source AISDefault.OnInteract target cleared during selected virtual98"),-1;
   if(!source_campaign_object_as_character_v114(t.r_.services.world,target.target,actual,t.r_.error))return -1;
   if(actual){SourceCampaignCharacterBorrowV62 other;if(!borrow_source_campaign_character_v62(t.r_.services.world,actual,other,t.r_.error))return -1;
    auto* talk=other.character->actor->source_bool_field(0x2fa);if(!talk)return -1;if(*talk)return 0;
    const auto* row=other.character->design.ai()?dh2::data::ai_props(*other.character->design.ai(),other.character->properties->resolved[1]):nullptr;if(!row)return -1;if(row->type==7)return 0;
   }else{
    SourceCampaignCandidateBorrowV55 candidate;if(!borrow_source_campaign_candidate_v55(candidate,t.r_.error)||candidate.actual_world!=t.r_.services.world)return -1;
    std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
    if(!borrow_source_campaign_object_base_v77(candidate,target.target,pin,base,t.r_.error)||!base)return -1;
    auto* talk=base->byte(0x2fa);if(!talk)return t.fail("Required selected nonCharacter TalkToNPC2fa projection"),-1;if(*talk)return 0;
   }
   auto& binding=t.r_.actor->object->binding;
   if(dh2_character_ai_set_target(&target,0,0,&binding.services))return -1;
   return dh2_character_ai_sync_last_target(&target)==0?0:-1;
  }
  if(q->operation==0x98&&q->callee==0x3dccd0)return t.call(q->subject,"OnEndOfAnim",nullptr,0);
  if(q->operation==0x90&&q->callee==0x3dcc80){dh2_script_value argument{};argument.type=DH2_SCRIPT_NUMBER;argument.number=static_cast<float>(static_cast<int>(q->argument));return t.call(q->subject,"OnTimer",&argument,1);}
 }
 if(q->service==ai_event_helper){
  if(q->operation==0x3d3d4c||q->operation==0x3d3d30){int current{};if(dh2_character_native_fsm_get_integer(&current,&t.r_.actor->machine->native_fsm(),0)!=1)return -1;*out=1;return 0;}
  if(q->operation==0x3d4204||q->operation==0x3d3ff8){const bool result=t.animation_consumer(q->operation==0x3d4204?ai_step_begin:ai_step_end);*out=result;return result?0:-1;}
  if(q->operation==0x3d4434){const auto* trigger=reinterpret_cast<const animation::TriggeredEvent*>(q->payload);if(!trigger||!trigger->name)return -1;t.lag_=trigger->lag_ms;
   int code{};if(t.state().current==6){t.skill_.state=&t.state();t.skill_.character=t.r_.actor->object->identity;code=t.skill_dispatch(skill_state_ai_event_v4,0,0,reinterpret_cast<std::uintptr_t>(trigger->name));}else code=t.authored(*trigger);
   *out=1;return code? -1:0;
  }
  if(q->operation==0x3d808c||q->operation==0x3d8b7c){unsigned value{};if(!t.r_.player_script_owner_v62)return t.npc_skill_ai_v115(q->operation==0x3d808c?skill_ai_focus_v3:skill_ai_blur_v3,0,value);const int code=t.r_.player_script_owner_v62->skill_ai(q->operation==0x3d808c?skill_ai_focus_v3:skill_ai_blur_v3,0,&value);if(code)t.r_.error=t.r_.player_script_owner_v62->error();return code;}
  if(q->operation==0x3d8038||q->operation==0x3d8b28)return t.spell_callback(q->operation==0x3d8038?skill_pre_v3:skill_post_v3);
  if(q->operation==0x394a3c||q->operation==0x3949b0){
   if(q->subject!=t.r_.actor->object->identity)return -1;
   if(!t.collisions_){WorldNpcObjectServicesV1 services;services.context=&t;
    services.physical=[](void* raw)->dh2::physical::NativeBody*{auto& t=*static_cast<CampaignFsmV101*>(raw);if(!t.r_.actor->position_fields_v7().physical2dc)return nullptr;if(!t.r_.physical_owner_v62)t.unavailable("Required source physical2dc collision receiver");return &t.r_.physical_owner_v62->native();};
    services.method=[](void* raw,WorldNpcObjectMethodV1 method,std::string& error){auto& t=*static_cast<CampaignFsmV101*>(raw);if(!t.r_.physical_owner_v62)return -1;const bool okay=method==WorldNpcObjectMethodV1::EnablePhysicalFilter?t.r_.physical_owner_v62->enable_filter():method==WorldNpcObjectMethodV1::DisablePhysicalFilter?t.r_.physical_owner_v62->disable_filter():false;if(!okay)error=t.r_.physical_owner_v62->error();return okay?0:-1;};
    services=character_pf_services_v1(services);t.collisions_=std::make_unique<CharacterCollisionLifecycleV1>(t.r_.actor->runtime.object,t.pf_geometry_,*t.pf_registry_,q->subject,services);
   }
   if(!(q->operation==0x394a3c?t.collisions_->enable():t.collisions_->disable())){t.r_.error=t.collisions_->error();return -1;}return 0;
  }
 }
 if(t.r_.services.ai_event_v101)return t.r_.services.ai_event_v101(t.r_,*q,*out,t.r_.error)?0:-1;
 t.r_.error="Required canonical AI event source "+std::to_string(q->callee)+" service "+std::to_string(q->service);return -1;
}
}
int CampaignFsmV101::npc_script_gameplay_binding(void* raw,std::uint32_t address,dh2_script_function* function,void** context){
 if(!raw||!function||!context)return -1;auto& owner=*static_cast<CampaignFsmV101*>(raw);
 if(owner.previous_gameplay_binding_){const int selected=owner.previous_gameplay_binding_(owner.previous_gameplay_context_,address,function,context);if(selected<0||selected>1)return -1;if(selected)return 1;}
 return owner.npc_script_bind(address,function,context);
}
int CampaignFsmV101::npc_script_gameplay_call(void* raw,const dh2_script_value* args,std::uint32_t count,dh2_script_value* out,std::uint32_t capacity,std::uint32_t* written,char* error,std::size_t error_capacity){
 if(!raw)return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;auto& binding=*static_cast<NpcScriptBinding*>(raw);if(!binding.owner)return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
 const int status=binding.owner->npc_script_call(binding.address,args,count,out,capacity,written);
 if(!status)return 0;if(error&&error_capacity){const auto& message=binding.owner->r_.error;const char* text=message.empty()?"Required same-World NPC script target/search service":message.c_str();std::snprintf(error,error_capacity,"%s",text);}return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
}
bool bind_campaign_character_fsm_v101(Record& record,const SourceCampaignCandidateBorrowV55& source,WorldNpcStateServicesV1& output,std::string& error){
 if(record.fsm_context_v101||record.services.world!=source.actual_world||!source.application){error="Required once-only SAME canonical FSM construction";return false;}
 if(!source.floors||!source.navigation_registry){error="Required actual campaign FSM floor/navigation graph";return false;}
 auto owner=std::make_shared<CampaignFsmV101>(record,source);record.fsm_context_v101=owner;if(!owner->bind(output)){error=record.error;return false;}return true;
}
bool bind_process_character_fsm_v121(Record& record,const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 const std::shared_ptr<void>& lease,const dh2::navigation::CollisionWorld& geometry,dh2::navigation::ObstacleRegistry& registry,
 WorldNpcStateServicesV1& output,std::string& error){
 if(record.fsm_context_v101||!app||!lease||!record.services.world||!record.services.canonical_objects){error="Required once-only SAME process Character FSM construction";return false;}
 auto owner=std::make_shared<CampaignFsmV101>(record,app,lease,geometry,registry);record.fsm_context_v101=owner;
 if(!owner->bind(output)){error=record.error;return false;}return true;
}
bool bind_backend_character_fsm_v1(Record& record,
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 const std::shared_ptr<void>& world_lease,
 const std::shared_ptr<dh2::floors::World>& floors,
 dh2::navigation::ObstacleRegistry& registry,
 WorldNpcStateServicesV1& output,std::string& error){
 if(record.fsm_context_v101||!app||!world_lease||!record.services.world||
    record.services.world.get()!=world_lease.get()||!floors||!floors->sewn||
    !record.services.canonical_objects||!record.actor||!record.actor->object||
    !record.actor->machine||!record.actor->controller||
    record.actor->machine->native_fsm().character!=record.actor->object->identity){
  error="Required once-only same-world campaign backend Character FSM inputs";return false;
 }
 auto owner=std::make_shared<CampaignFsmV101>(record,app,world_lease,floors,registry);
 record.fsm_context_v101=owner;
 if(!owner->bind(output)){error=record.error;return false;}
 error.clear();return true;
}
bool borrow_source_campaign_character_skill_context_v1(const std::shared_ptr<void>& world,std::uintptr_t id,SourceCampaignCharacterSkillContextBorrowV1& out,std::string& error){
 out={};SourceCampaignCharacterBorrowV62 actual;
 if(!borrow_source_campaign_character_v62(world,id,actual,error))return false;
 return borrow_source_campaign_character_skill_context_v1(actual.character,out,error);
}
bool borrow_source_campaign_character_skill_context_v1(const std::shared_ptr<Record>& record,SourceCampaignCharacterSkillContextBorrowV1& out,std::string& error){
 out={};if(!record||!record->actor||!record->actor->object||!record->fsm_context_v101){
  error="Required retained canonical Character record/actor/FSM";return false;
 }
 auto owner=std::static_pointer_cast<CampaignFsmV101>(record->fsm_context_v101);
 if(!owner){error="Required typed stored canonical CampaignFsmV101";return false;}
 return owner->borrow_skill_context(record,out,error);
}
bool bind_campaign_character_player_events_v101(Record& record,PlayerSkillGameplayServicesV3& out,std::string& error){auto owner=std::static_pointer_cast<CampaignFsmV101>(record.fsm_context_v101);if(!owner){error="Required actual canonical FSM before player VM binding";return false;}return owner->bind_player(out);}
bool bind_campaign_character_npc_events_v101(Record& record,CharacterScriptSessionInput& input,std::string& error){auto owner=std::static_pointer_cast<CampaignFsmV101>(record.fsm_context_v101);if(!owner){error="Required actual canonical FSM before NPC VM binding";return false;}return owner->bind_npc(input);}
bool source_campaign_character_timers_update_v102(const std::shared_ptr<void>& world,std::uintptr_t id,std::uint32_t dt,std::uint32_t blocked,std::string& error){SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,error)||!actual.character->fsm_context_v101)return false;auto owner=std::static_pointer_cast<CampaignFsmV101>(actual.character->fsm_context_v101);const bool okay=owner->update_timers(dt,blocked);if(!okay)error=actual.character->error;return okay;}
bool source_campaign_character_target_update_v108(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& error){SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,error)||!actual.character->fsm_context_v101)return false;auto owner=std::static_pointer_cast<CampaignFsmV101>(actual.character->fsm_context_v101);const bool okay=owner->update_target_v108();if(!okay)error=actual.character->error;return okay;}
bool source_campaign_character_master_update_v108(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& error){SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,error)||!actual.character->fsm_context_v101)return false;auto owner=std::static_pointer_cast<CampaignFsmV101>(actual.character->fsm_context_v101);const bool okay=owner->update_master_v108();if(!okay)error=actual.character->error;return okay;}
bool source_campaign_character_ai_on_update_v108(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& error){SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,error)||!actual.character->fsm_context_v101)return false;auto owner=std::static_pointer_cast<CampaignFsmV101>(actual.character->fsm_context_v101);const bool okay=owner->on_update_v108();if(!okay)error=actual.character->error;return okay;}
bool source_campaign_character_ai_update_v108(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& error){SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,error)||!actual.character->fsm_context_v101)return false;auto owner=std::static_pointer_cast<CampaignFsmV101>(actual.character->fsm_context_v101);const bool okay=owner->update_ai_v108();if(!okay)error=actual.character->error;return okay;}
bool source_campaign_character_add_aggro_v108(const std::shared_ptr<void>& world,std::uintptr_t id,std::uintptr_t other,float amount,std::string& error){SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,error)||!actual.character->fsm_context_v101)return false;auto owner=std::static_pointer_cast<CampaignFsmV101>(actual.character->fsm_context_v101);const bool okay=owner->add_aggro_v108(other,amount);if(!okay)error=actual.character->error;return okay;}
bool source_campaign_character_animator_update_v102(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& error){SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,error)||!actual.character->fsm_context_v101)return false;auto owner=std::static_pointer_cast<CampaignFsmV101>(actual.character->fsm_context_v101);const bool okay=owner->update_animator();if(!okay)error=actual.character->error;return okay;}
bool source_campaign_character_enabled_event_v102(const std::shared_ptr<void>& world,std::uintptr_t id,bool enabled,std::string& error){SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,error)||!actual.character->fsm_context_v101)return false;auto owner=std::static_pointer_cast<CampaignFsmV101>(actual.character->fsm_context_v101);const bool okay=owner->enabled_event(enabled);if(!okay)error=actual.character->error;return okay;}
bool source_campaign_character_zone_entered_v102(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& error){SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,error)||!actual.character->fsm_context_v101)return false;auto owner=std::static_pointer_cast<CampaignFsmV101>(actual.character->fsm_context_v101);const bool okay=owner->zone_entered();if(!okay)error=actual.character->error;return okay;}
bool source_campaign_character_zone_exited_v102(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& error){SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,error)||!actual.character->fsm_context_v101)return false;auto owner=std::static_pointer_cast<CampaignFsmV101>(actual.character->fsm_context_v101);const bool okay=owner->zone_exited();if(!okay)error=actual.character->error;return okay;}
bool source_campaign_character_is_zonable_v104(const std::shared_ptr<void>& world,std::uintptr_t id,bool& value,std::string& error){
 SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,error)||!actual.character->actor)return false;auto& r=*actual.character;bool player{};if(!r.is_player(player,error))return false;
 if(player){value=false;return true;}const auto* rows=r.design.ai();std::int32_t ai{};if(!rows||dh2_character_target_ai_id(&ai,r.properties->resolved.data(),static_cast<std::uint32_t>(rows->rows.size()))){error="Required actual Character.IsFaerie AI row";return false;}
 const auto* row=dh2::data::ai_props(*rows,ai);if(!row){error="Required source Character.IsFaerie row";return false;}
 //3a36e4: player0, Faerie0, otherwise qualified MeetCondition38ab60=1.
 value=row->type!=3;return true;
}
void source_campaign_character_animation_event_v101(void* raw,actor::BlendedPlayback& playback,const actor::BlendedPlaybackEvent& event){auto* record=static_cast<Record*>(raw);if(!record||!record->fsm_context_v101)throw std::runtime_error("Required actual canonical animation event owner");auto owner=std::static_pointer_cast<CampaignFsmV101>(record->fsm_context_v101);owner->observe(playback,event);}
bool source_campaign_character_fsm_update_v101(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& error){SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,error)||!actual.character->fsm_context_v101||!actual.character->actor->machine)return false;const int result=actual.character->actor->machine->update();if(result<0){error=actual.character->error.empty()?actual.character->actor->machine->error():actual.character->error;return false;}return true;}
bool source_campaign_character_set_interact_v114(const std::shared_ptr<void>& world,std::uintptr_t id,int kind,bool repeat,std::uintptr_t other,bool force,std::string& error){
 SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,error)||!actual.character->fsm_context_v101)return false;
 auto owner=std::static_pointer_cast<CampaignFsmV101>(actual.character->fsm_context_v101);if(!owner->set_interact_v114(kind,repeat,other,force)){error=actual.character->error;return false;}return true;
}
bool source_campaign_character_raise_event_v114(const std::shared_ptr<void>& world,std::uintptr_t id,unsigned event,std::uintptr_t other,std::string& error){
 SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,error)||!actual.character->fsm_context_v101)return false;
 auto owner=std::static_pointer_cast<CampaignFsmV101>(actual.character->fsm_context_v101);if(!owner->raise_v114(event,other)){error=actual.character->error;return false;}return true;
}
int source_campaign_character_reaction_v115(const std::shared_ptr<void>& world,std::uintptr_t id,const SkillApplyRequestV6& request,SkillApplyResponseV6& response,std::string& error){
 SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,error)||!actual.character->fsm_context_v101)return -1;
 auto owner=std::static_pointer_cast<CampaignFsmV101>(actual.character->fsm_context_v101);const int result=owner->reaction_v115(request,response);if(result<0)error=actual.character->error;return result;
}
bool borrow_source_campaign_character_path_v105(const std::shared_ptr<void>& world,std::uintptr_t id,SourceCharacterPathBorrowV105& out,std::string& e){
 SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,e)||!actual.character->fsm_context_v101)return false;
 auto owner=std::static_pointer_cast<CampaignFsmV101>(actual.character->fsm_context_v101);if(!owner->borrow_path(out)){e=actual.character->error;return false;}return true;
}
bool borrow_source_campaign_character_path_v105(const std::shared_ptr<Record>& record,SourceCharacterPathBorrowV105& out,std::string& e){
 out={};if(!record||record->failed||!record->actor||!record->actor->object||!record->actor->machine||!record->fsm_context_v101){e="Required retained canonical Character record/actor/FSM path";return false;}
 auto owner=std::static_pointer_cast<CampaignFsmV101>(record->fsm_context_v101);
 if(!owner||!owner->borrow_path(record,out)){e=record->error.empty()?"Required typed same-record CampaignFsmV101 path":record->error;return false;}
 e.clear();return true;
}
bool source_campaign_character_drop_path_v105(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& e){
 SourceCharacterPathBorrowV105 actual;if(!borrow_source_campaign_character_path_v105(world,id,actual,e))return false;
 if(dh2_nav_drop_path(actual.path)){e="Actual Character PFObject.DropPath rejected retained storage";return false;}return true;
}
bool source_campaign_character_command_stop_v101(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& error){SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,error)||!actual.character->fsm_context_v101)return false;auto owner=std::static_pointer_cast<CampaignFsmV101>(actual.character->fsm_context_v101);const bool result=owner->command_stop();if(!result)error=actual.character->error;return result;}
bool source_campaign_character_set_anim_state_v116(const std::shared_ptr<void>& world,std::uintptr_t id,std::int32_t animation,bool event_end,bool update_end,std::string& error){SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,error)||!actual.character->fsm_context_v101)return false;auto owner=std::static_pointer_cast<CampaignFsmV101>(actual.character->fsm_context_v101);if(!owner->set_anim_state_v116(animation,event_end,update_end)){error=actual.character->error;return false;}return true;}
bool source_campaign_character_command_move_v116(const std::shared_ptr<void>& world,std::uintptr_t id,std::uintptr_t target,std::string& error){SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,error)||!actual.character->fsm_context_v101)return false;auto owner=std::static_pointer_cast<CampaignFsmV101>(actual.character->fsm_context_v101);if(!owner->command_move_v116(target)){error=actual.character->error;return false;}return true;}
bool source_campaign_character_click_point_v120(const std::shared_ptr<void>& world,std::uintptr_t id,const float* point,bool released,std::string& error){
 SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,error)||!actual.character->fsm_context_v101)return false;
 auto owner=std::static_pointer_cast<CampaignFsmV101>(actual.character->fsm_context_v101);
 if(!owner->source_click_point_v120(point,released)){error=actual.character->error;return false;}return true;
}
bool source_campaign_character_control_stop_v116(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& error){SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,error)||!actual.character->fsm_context_v101)return false;auto owner=std::static_pointer_cast<CampaignFsmV101>(actual.character->fsm_context_v101);if(!owner->control_stop_v116()){error=actual.character->error;return false;}return true;}
bool source_campaign_character_set_limbus_v118(const std::shared_ptr<void>& world,std::uintptr_t id,bool mode,std::string& error){SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,error)||!actual.character->fsm_context_v101)return false;auto owner=std::static_pointer_cast<CampaignFsmV101>(actual.character->fsm_context_v101);if(!owner->set_limbus_v118(mode)){error=actual.character->error;return false;}return true;}
bool source_campaign_character_command_heading_v101(const std::shared_ptr<void>& world,std::uintptr_t id,const float* direction,std::string& error){SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,error)||!actual.character->fsm_context_v101)return false;auto owner=std::static_pointer_cast<CampaignFsmV101>(actual.character->fsm_context_v101);const bool result=owner->command_heading(direction);if(!result)error=actual.character->error;return result;}
bool bind_campaign_character_sound_names_v101(const std::shared_ptr<const CharacterCandidateCacheV62>& cache,dh2::world::CanonicalCharacterCandidateServicesV60& services,std::string& error){
 if(!cache||!cache->ready()||!cache->files().read){error="Required SAME immutable canonical animation sound cache";return false;}
 auto names=std::make_shared<dh2::audio::AudioSourceBindingsV38>();std::vector<std::uint8_t> records,strings;bool found{};
 if(!cache->files().read(dh2::audio::AudioSourceBindingsV38::records_uri,found,records,error)||!found){if(error.empty())error="Missing actual generated sound records";return false;}
 if(!cache->files().read(dh2::audio::AudioSourceBindingsV38::names_uri,found,strings,error)||!found){if(error.empty())error="Missing actual generated sound names";return false;}
 if(!names->load(records.data(),records.size(),strings.data(),strings.size(),error))return false;
 services.named_sound_v101=[cache,names](Record& record,const char* suffix,std::string& error){
  struct Delivery{Record& record;std::string& error;dh2::audio::AudioApplicationBorrowV42 manager;};Delivery delivery{record,error,{}};
  dh2::audio::AudioNamedAnimationSoundServicesV38 s;s.context=&delivery;s.actual_character=record.actor->object->identity;
  s.manager=[](void* raw,std::uintptr_t& identity){auto& d=*static_cast<Delivery*>(raw);if(!borrow_actual_application_audio_v42(d.manager,d.error))return -1;identity=d.manager.identity();return 0;};
  s.target_position=[](void* raw,std::uintptr_t id,std::array<float,3>& out){auto& d=*static_cast<Delivery*>(raw);WorldTargetActorBorrowV1 actor;
   if(!d.record.services.world_targets||d.record.services.world_targets->actor(id,&actor)||!actor.position||!actor.target_node)return -1;
   if(*actor.target_node&&(!actor.target_enabled||(*actor.target_enabled&&!actor.cached_target_position)))return -1;
   const auto* p=dh2_world_target_position_v1(actor.position,actor.cached_target_position,*actor.target_node,actor.target_enabled?*actor.target_enabled:0);if(!p)return -1;std::copy_n(p,3,out.begin());return 0;};
  s.play=[](void* raw,const dh2::character::CombatSoundPlayV1& request){auto& d=*static_cast<Delivery*>(raw);bool player{};if(!d.record.is_player(player,d.error))return -1;
   return submit_campaign_audio_v46(player?dh2::audio::AudioCategoryV46::attack:dh2::audio::AudioCategoryV46::monster,d.manager,d.record.services.world,request,d.error)?0:-1;};
  dh2::audio::AudioNamedAnimationSoundResultV38 result;const int code=dh2::audio::audio_named_animation_sound_v38(suffix,*names,s,result);
  if(code&&error.empty())error=result.required?result.required:"Required source named animation sound";return code==0;
 };
 return true;
}
}
