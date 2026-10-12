#include "source_campaign_character_fsm_v101.hpp"
#include "gameobject_embedded_resources_v93.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "model_renderer.hpp"
#include "canonical_character_candidate_v60.hpp"
#include "source_campaign_animation_random_scope_v101.hpp"
#include "character_skill_target_binding_v46.hpp"
#include "character_script_source_virtuals_v101.hpp"
#include "character_ai_state_changed_vm.hpp"
#include "character_animation_event_owner_v1.hpp"
#include "npc_animation_event_owner_v1.hpp"
#include "character_melee_animation_event_v1.hpp"
#include "character_idle_update.hpp"
#include "character_heading_owner_v1.hpp"
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
#include <algorithm>
#include <cstring>
#include <stdexcept>
namespace model_renderer {namespace {
using namespace dh2::character;
using namespace dh2::character::skills;
namespace data=dh2::data;
namespace actor=dh2::actor;
namespace animation=dh2::animation;
namespace scene=dh2::scene;
using Record=dh2::world::CanonicalCharacterCandidateRecordV60;
class CampaignFsmV101 {
 Record& r_;
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> application_;
 std::shared_ptr<dh2::floors::World> floors_;
 std::shared_ptr<dh2::navigation::CampaignNavigationRegistryV64> navigation_;
 std::shared_ptr<dh2::world::CanonicalObjectManagerV1> objects_;
 std::unique_ptr<CharacterWorldAttackGeometryV1> geometry_;
 std::map<std::uintptr_t,std::unique_ptr<std::string>> debug_strings_;
 SkillStateV4 skill_{};
 AIEventServices24 events_{this,ai_service,63,0};
 SkillStateServices16V4 skill_services_{this,skill_service};
 TimerServices32 timer_expiry_v102_{this,[](void* raw,std::uintptr_t id,std::int32_t event,Timer32* timer){auto& t=*static_cast<CampaignFsmV101*>(raw);if(id!=t.r_.actor->object->identity||!timer||!t.raise(static_cast<unsigned>(event),reinterpret_cast<std::uintptr_t>(timer)))t.unavailable("Required SAME canonical CharTimer event delivery");},nullptr,0};
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
  s.body_present=a.position_fields_v7().physical2dc!=0;s.heading_active=a.runtime.controller.heading.active;
  ControllerCommandState32* command{};if(!controller(command))return false;s.controller_locked=command->locked;
  f.target=a.object->target.target;std::copy_n(a.runtime.controller.heading.direction,3,f.heading);
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
  if(status)return fail("Required canonical Character/CharAI event continuation");return true;
 }
 int skill_dispatch(unsigned operation,unsigned index,unsigned value,std::uintptr_t payload){
  auto* heading=r_.actor->source_heading_enabled412_v101();if(!heading)return fail("Required produced Character412 for SkillState"),-1;
  skill_.heading_enabled=*heading;
  const int result=character_skill_state_target_bound_v46(skill_,r_.actor->object->target,operation,index,value,payload,0,skill_services_);
  *heading=skill_.heading_enabled;return result;
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
 bool move(const float* point,bool command_gate=true){ControllerCommandState32* control{};if(!controller(control))return false;
  if(command_gate&&!control->forced&&(control->global_blocked||control->locked))return true;
  bool remotely_updated{};if(!remote(remotely_updated))return false;if(remotely_updated)return true;
  dh2::world::GameObjectInitializationFieldsV62 fields;if(!r_.actor->inherited_initialization_fields_v62(r_.actor,fields,r_.error))return false;
  auto* stat=fields.byte?fields.byte(0x84):nullptr;auto* limit=fields.integer?fields.integer(0x26c):nullptr;
  if(!stat||!limit)return fail("Required source PathTo static84/limit26c cells");auto& path=r_.actor->runtime.path;
  PathToState40 value{r_.actor->object->identity,*stat,path.count!=0,static_cast<unsigned>(*limit),0,{path.target[0],path.target[1],path.target[2]},0};
  const PathToServices16 callbacks{this,[](void* raw,const PathToRequest32* q,std::uint32_t* out){auto& t=*static_cast<CampaignFsmV101*>(raw);if(!q||!out||!t.floors_||!t.floors_->sewn||q->owner!=t.r_.actor->object->identity)return -1;
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
 int timer(unsigned duration,int repeat,int event){if(r_.player_script_owner_v62)return r_.player_script_owner_v62->session().start_timer(duration,repeat,event)>=0?0:-1;
  if(r_.actor->session)return dh2_character_timer_start(&r_.actor->session->timers(),duration,repeat,event,0,&r_.actor->session->timer_services())>=0?0:-1;return -1;
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
   case cancel_sneaking:okay=cancel_sneaking();break;
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
   }};
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
 static int skill_service(void* p,SkillStateV4* s,const SkillStateRequest32V4* q,SkillStateResponse16V4* out){
  if(!p||!s||!q||!out)return -1;auto& t=*static_cast<CampaignFsmV101*>(p);auto* heading=t.r_.actor->source_heading_enabled412_v101();if(!heading)return -1;
  *heading=s->heading_enabled;
  struct Reload{SkillStateV4& state;std::uint8_t* source;~Reload(){state.heading_enabled=*source;}}reload{*s,heading};
  return t.skill_operation(s,*q,*out);
 }
 static int remaining(void* p,StateOwnerMachine40* machine,const StateOwnerRequest48* q,StateOwnerResponse8*){
  auto& t=*static_cast<CampaignFsmV101*>(p);if(!q||machine!=&t.r_.actor->machine->owner().machine())return -1;
  if(q->operation==state_owner_character_event)return t.raise(q->event,q->payload)?0:-1;
  if(q->operation==state_owner_pin)return t.pin(true,q->character)?0:-1;
  if(q->state==6&&(q->operation==state_owner_focus||q->operation==state_owner_blur||q->operation==state_owner_event)){
   t.skill_.state=&t.state();t.skill_.character=t.r_.actor->object->identity;t.skill_.physical=t.r_.actor->position_fields_v7().physical2dc;
   return t.skill_dispatch(q->operation==state_owner_focus?skill_state_focus_v4:q->operation==state_owner_blur?skill_state_blur_v4:skill_state_event_v4,q->event,0,q->payload);
  }
  if(q->state==7&&(q->operation==state_owner_focus||q->operation==state_owner_blur||q->operation==state_owner_event))return t.cast_body(q->operation==state_owner_focus?0:q->operation==state_owner_blur?1:2);
  return t.r_.services.state_method_v101?t.r_.services.state_method_v101(t.r_,*q,t.r_.error)?0:-1:-1;
 }
 static int frame(void* p,NativeFsm24* f,const NativeFsmRequest32* q,std::uint32_t* out){auto& t=*static_cast<CampaignFsmV101*>(p);
  if(!q||!out||f!=&t.r_.actor->machine->native_fsm())return -1;
  if(q->service==fsm_engine_dt){auto app=t.application_.lock();if(!app)return -1;*out=app->source_loading_v55().dt8c;return 0;}
  if(q->service==fsm_profile_begin||q->service==fsm_profile_end){StateOwnerRequest48 source{};source.operation=q->service==fsm_profile_begin?state_owner_profile_begin:state_owner_profile_end;source.source_function=q->service==fsm_profile_begin?0x337888:0x318254;return t.r_.actor->diagnostics->invoke(source);}
  return t.r_.services.state_frame_v101?t.r_.services.state_frame_v101(t.r_,*q,*out,t.r_.error)?0:-1:-1;
 }
 static int prepare(void* p,const StateOwnerRequest48& q){auto& t=*static_cast<CampaignFsmV101*>(p);
  if(q.operation!=state_owner_focus&&q.operation!=state_owner_event)return 0;
  return t.refresh(q.state)?0:-1;
 }
 static int update_prepare(void* p){auto& t=*static_cast<CampaignFsmV101*>(p);return t.refresh(t.state().current)?0:-1;}
public:
 CampaignFsmV101(Record& record,const SourceCampaignCandidateBorrowV55& source):r_(record),application_(source.application),floors_(source.floors),navigation_(source.navigation_registry),objects_(source.objects){/*412 is borrowed at the actual method boundary.*/}
 ~CampaignFsmV101(){
  //Native scratch retirement after source unpublication/VM close. Do not
  //leave actor PF aliases pointing into the soon-to-be-destroyed route owner.
  if(r_.actor&&r_.actor->runtime.path.segments==route_segments_.data()){
   auto& path=r_.actor->runtime.path;dh2_nav_drop_path(&path);path.segments=nullptr;path.capacity=0;
  }
 }
 bool borrow_pf_path_storage_v93(dh2::world::PfPathStorageBorrowV93& out,std::string& error){
  if(!r_.actor){error="Actual canonical PF actor missing";return false;}auto& path=r_.actor->runtime.path;
  if(!path.segments||path.segments!=route_segments_.data()||path.capacity!=route_segments_.size()){error="PF path must borrow SAME actual FSM-owned route buffer";return false;}
  out={};out.owner=r_.fsm_context_v101;out.segments=&route_segments_;
  if(!out.owner){error="Actual canonical FSM owner missing";return false;}
  std::weak_ptr<void> weak=r_.fsm_context_v101;out.release_storage=[weak](std::string& error){auto pin=weak.lock();if(!pin){error="Actual PF path storage owner retired early";return false;}auto owner=std::static_pointer_cast<CampaignFsmV101>(pin);auto& path=owner->r_.actor->runtime.path;
   if(path.count||path.owned||path.segments!=owner->route_segments_.data()||path.capacity!=owner->route_segments_.size()){error="PF DropPath must complete before SAME buffer release";return false;}
   std::vector<dh2::navigation::PathSegment>{}.swap(owner->route_segments_);path.segments=nullptr;path.capacity=0;error.clear();return true;};
  error.clear();return true;
 }
 bool bind(WorldNpcStateServicesV1& out){
  if(!r_.actor||!r_.services.debug||!r_.services.debug_files||!floors_||!navigation_)return fail("Required actual canonical FSM resource graph");
  r_.actor->diagnostics=std::make_unique<StateOwnerDebugDiagnostics>(*r_.services.debug,*r_.services.debug_files);
  r_.actor->bodies={this,body_service};out.remaining_methods={this,remaining};out.outer={this,frame};
  out.remaining_updates={this,[](void* p,StateOwnerMachine40*,const StateOwnerUpdateRequest24* q){auto& t=*static_cast<CampaignFsmV101*>(p);return q&&t.r_.services.state_update_v101&&t.r_.services.state_update_v101(t.r_,*q,t.r_.error)?0:-1;}};
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
 bool bind_npc(CharacterScriptSessionInput& input){input.timer_services=&timer_expiry_v102_;return true;}
 bool update_timers(unsigned dt,unsigned blocked){
  if(r_.player_script_owner_v62){const int code=r_.player_script_owner_v62->update_timers(dt,blocked);if(code<0)r_.error=r_.player_script_owner_v62->error();return code==1;}
  if(!r_.actor->session)return fail("Required SAME active NPC timer receiver");const int code=r_.actor->session->update_timers(dt,blocked);if(code<0)r_.error=r_.actor->session->error();return code==1;
 }
 bool update_animator(){
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
 bool command_heading(const float* direction){ControllerCommandState32* control{};if(!direction||!controller(control))return false;
  dh2::physical::NativeBody* body=nullptr;const auto physical=r_.actor->position_fields_v7().physical2dc;
  if(physical){if(!r_.physical_owner_v62||physical!=reinterpret_cast<std::uintptr_t>(r_.physical_owner_v62.get()))return fail("Required SAME physical2dc heading receiver");body=&r_.physical_owner_v62->native();}
  const CharacterHeadingServicesV1 services{this,
   [](void* raw,std::uintptr_t id,const float*& out,std::string&){return static_cast<CampaignFsmV101*>(raw)->position(id,out);},
   [](void* raw,bool& out,std::string&){return static_cast<CampaignFsmV101*>(raw)->remote(out);},
   [](void* raw,bool& out,std::string&){out=(static_cast<CampaignFsmV101*>(raw)->state().flags&2)!=0;return true;},
   [](void* raw,unsigned event,std::string&){auto& t=*static_cast<CampaignFsmV101*>(raw);t.r_.actor->runtime.rotation.heading_angle=t.r_.actor->runtime.controller.heading.angle;t.state().heading_active=t.r_.actor->runtime.controller.heading.active;return t.raise(event,0);}};
  auto& a=*r_.actor;CharacterHeadingOwnerV1 owner(*control,state(),a.object->target,a.object->binding.services,a.runtime.controller,a.runtime.path,a.runtime.subobjects.position,a.runtime.subobjects.destination,body,services);
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
  }};
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
   case melee_event_skill:{if(t.r_.player_script_owner_v62){unsigned result{};return t.r_.player_script_owner_v62->native_skill_animation_event(&result);}return t.r_.services.melee_animation_v101&&t.r_.services.melee_animation_v101(t.r_,*q,*out,t.r_.error)?0:-1;}
   case melee_event_spell:return t.spell_callback(skill_use_v3);
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
  case skill_state_use_v4:{unsigned result{};if(!r_.player_script_owner_v62)return -1;const int status=r_.player_script_owner_v62->native_skill_animation_event(&result);if(status)r_.error=r_.player_script_owner_v62->error();return status;}
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
  const auto code=t.r_.actor->machine->event(static_cast<int>(q->event),payload);*out=static_cast<unsigned>(code);return code<0?-1:0;
 }
 if(q->service==ai_event_timer_id){if(q->callee!=0x3db288)return -1;const auto* store=t.r_.player_script_owner_v62?&t.r_.player_script_owner_v62->session().timers():t.r_.actor->session?&t.r_.actor->session->timers():nullptr;
  if(!store)return -1;for(unsigned i=0;i<store->count;++i)if(q->subject==reinterpret_cast<std::uintptr_t>(store->slots+i)){*out=store->slots[i].id;return 0;}return -1;
 }
 if(q->service==ai_event_virtual&&q->subject==ai->ai){
  if(q->operation==0x20&&q->callee==0x3d0bec)return dh2_character_ai_state_changed(ai,static_cast<int>(q->argument),static_cast<int>(static_cast<unsigned>(q->payload)),&t.events_);
  if(q->operation==0x98&&q->callee==0x3d0ce8)return dh2_character_ai_end_anim(ai,&t.events_);
  if(q->operation==0x90&&q->callee==0x3d0ca0){if(!t.selected())return -1;AIEventResult16 result;return dh2_character_ai_event_script_timer(&result,ai,q->argument,&t.events_)?-1:0;}
  if(q->operation==0x24&&q->callee==0x3d1000){if(!t.r_.death_v84)return t.fail("Required actual canonical OnDied owner"),-1;const auto* scope=t.r_.player_script_owner_v62?t.r_.player_script_owner_v62->session().current_skill_callback_scope():t.r_.actor->session?t.r_.actor->session->current_skill_callback_scope():nullptr;return t.r_.death_v84->raise(q->payload,scope,t.r_.error)?0:-1;}
 }
 if(q->service==ai_event_ais_virtual){
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
  if(q->operation==0x3d808c||q->operation==0x3d8b7c){if(!t.r_.player_script_owner_v62)return -1;unsigned value{};const int code=t.r_.player_script_owner_v62->skill_ai(q->operation==0x3d808c?skill_ai_focus_v3:skill_ai_blur_v3,0,&value);if(code)t.r_.error=t.r_.player_script_owner_v62->error();return code;}
  if(q->operation==0x3d8038||q->operation==0x3d8b28)return t.spell_callback(q->operation==0x3d8038?skill_pre_v3:skill_post_v3);
  if(q->operation==0x394a3c){
   if(q->subject!=t.r_.actor->object->identity)return -1;
   if(!t.collisions_){WorldNpcObjectServicesV1 services;services.context=&t;
    services.physical=[](void* raw)->dh2::physical::NativeBody*{auto& t=*static_cast<CampaignFsmV101*>(raw);if(!t.r_.actor->position_fields_v7().physical2dc)return nullptr;if(!t.r_.physical_owner_v62)t.unavailable("Required source physical2dc collision receiver");return &t.r_.physical_owner_v62->native();};
    services.method=[](void* raw,WorldNpcObjectMethodV1 method,std::string& error){auto& t=*static_cast<CampaignFsmV101*>(raw);if(!t.r_.physical_owner_v62)return -1;const bool okay=method==WorldNpcObjectMethodV1::EnablePhysicalFilter?t.r_.physical_owner_v62->enable_filter():method==WorldNpcObjectMethodV1::DisablePhysicalFilter?t.r_.physical_owner_v62->disable_filter():false;if(!okay)error=t.r_.physical_owner_v62->error();return okay?0:-1;};
    services=character_pf_services_v1(services);t.collisions_=std::make_unique<CharacterCollisionLifecycleV1>(t.r_.actor->runtime.object,&t.floors_->collision_world,t.navigation_->registry(),q->subject,services);
   }
   if(!t.collisions_->enable()){t.r_.error=t.collisions_->error();return -1;}return 0;
  }
 }
 if(t.r_.services.ai_event_v101)return t.r_.services.ai_event_v101(t.r_,*q,*out,t.r_.error)?0:-1;
 t.r_.error="Required canonical AI event source "+std::to_string(q->callee)+" service "+std::to_string(q->service);return -1;
}
}
bool bind_campaign_character_fsm_v101(Record& record,const SourceCampaignCandidateBorrowV55& source,WorldNpcStateServicesV1& output,std::string& error){
 if(record.fsm_context_v101||record.services.world!=source.actual_world||!source.application){error="Required once-only SAME canonical FSM construction";return false;}
 auto owner=std::make_shared<CampaignFsmV101>(record,source);record.fsm_context_v101=owner;if(!owner->bind(output)){error=record.error;return false;}return true;
}
bool bind_campaign_character_player_events_v101(Record& record,PlayerSkillGameplayServicesV3& out,std::string& error){auto owner=std::static_pointer_cast<CampaignFsmV101>(record.fsm_context_v101);if(!owner){error="Required actual canonical FSM before player VM binding";return false;}return owner->bind_player(out);}
bool bind_campaign_character_npc_events_v101(Record& record,CharacterScriptSessionInput& input,std::string& error){auto owner=std::static_pointer_cast<CampaignFsmV101>(record.fsm_context_v101);if(!owner){error="Required actual canonical FSM before NPC VM binding";return false;}return owner->bind_npc(input);}
bool source_campaign_character_timers_update_v102(const std::shared_ptr<void>& world,std::uintptr_t id,std::uint32_t dt,std::uint32_t blocked,std::string& error){SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,error)||!actual.character->fsm_context_v101)return false;auto owner=std::static_pointer_cast<CampaignFsmV101>(actual.character->fsm_context_v101);const bool okay=owner->update_timers(dt,blocked);if(!okay)error=actual.character->error;return okay;}
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
bool source_campaign_character_command_stop_v101(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& error){SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,error)||!actual.character->fsm_context_v101)return false;auto owner=std::static_pointer_cast<CampaignFsmV101>(actual.character->fsm_context_v101);const bool result=owner->command_stop();if(!result)error=actual.character->error;return result;}
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
bool borrow_source_campaign_character_pf_path_storage_v93(const std::shared_ptr<void>& world,std::uintptr_t id,
 dh2::world::PfPathStorageBorrowV93& out,std::string& error){
 SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,error)||!actual.character->actor||!actual.character->fsm_context_v101)return false;
 auto owner=std::static_pointer_cast<CampaignFsmV101>(actual.character->fsm_context_v101);return owner->borrow_pf_path_storage_v93(out,error);
}

}
