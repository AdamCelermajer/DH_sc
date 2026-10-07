#include "source_campaign_combat_v115.hpp"
#include "model_renderer.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "source_campaign_death_rewards_v84.hpp"
#include "source_campaign_fx_v77.hpp"
#include "source_campaign_character_fsm_v101.hpp"
#include "source_campaign_character_interaction_v114.hpp"
#include "source_process_trophies_v100.hpp"
#include <canonical_character_candidate_v60.hpp>
#include <canonical_object_manager_v1.hpp>
#include <canonical_character_save_v86.hpp>
#include <application_services_owner_v5.hpp>
#include <application_player_manager_bootstrap_v59.hpp>
#include <application_spawn_random_owner_v4.hpp>
#include <player_equipment_queries_v1.hpp>
#include <owned_hud_settings_v1.hpp>
#include <character_world_skill_combat_v6.hpp>
#include <character_combat_results_ai_v1.hpp>
#include <character_combat_hit_fx_v4.hpp>
#include <character_combat_follower_v1.hpp>
#include <character_world_attack_geometry_v1.hpp>
#include <character_script_source_virtuals_v101.hpp>
#include <character_skill_application_v116.hpp>
#include <player_save_difficulty_global_v29.hpp>
#include <script_manager_owner_v52.hpp>
#include <algorithm>
#include <cstring>
#include <map>
#include <cstdio>
#include <stdexcept>
namespace model_renderer {
using namespace dh2::character;
using namespace dh2::character::skills;
class SourceCampaignCombatV115 {
 using Record=dh2::world::CanonicalCharacterCandidateRecordV60;
 struct Actor {
  SourceCampaignCombatV115* owner{};std::weak_ptr<Record> record;
  dh2::data::CombatantView facts{};
  SkillAttackActorV6 attack{};SkillApplyActorV6 application{};HitActor32 hit{};
  std::uintptr_t controller{};std::int32_t previous_lifecycle{};
 };
 std::weak_ptr<SourceWorldBorrowV61> world_;
 std::shared_ptr<CharacterWorldRuntimeV1> targets_;
 std::unique_ptr<CharacterWorldSkillCombatV6> combat_;
 std::unique_ptr<CharacterWorldAttackGeometryV1> geometry_;
 std::map<std::uintptr_t,Actor> actors_;
 std::map<std::uintptr_t,std::string> strings_;
 std::uintptr_t serial_{};
 dh2::data::EffectsTables::Borrow effects_tables_;
 std::vector<const char*> class_names_v115_,effect_names_v115_;
 BuffDictionary16 dot_classes_v115_{},dot_effects_v115_{};
 SkillAttackNativeServicesV6 debug_{this,debug};
 SourceCombatPresentationV115 presentation_;
 CharacterGameDesign::Borrow design_;
 DotCombatContext32 context_{}; //single campaign projection, produced by CF_SetCombatants before consumption
 dh2::data::CombatRandom random_{};
 unsigned delivery_depth_{};
 const dh2_script_callback_scope* scope_{};
 std::string error_;
 struct CallbackRandom {SourceCampaignCombatV115& owner;bool ready;explicit CallbackRandom(SourceCampaignCombatV115& t):owner(t),ready(t.sync_random(true)){}~CallbackRandom(){if(ready)owner.sync_random(false);}};
 bool current(std::shared_ptr<SourceWorldBorrowV61>& out){
  out=world_.lock();SourceCampaignCandidateBorrowV55 candidate;
  if(!out||!out->owner||!out->application||!out->player_manager||!out->player_manager->manager()||
     !borrow_source_campaign_candidate_v55(candidate,error_)||candidate.actual_world!=out->owner){error_="Retired SAME canonical combat World/App/PlayerManager";return false;}return true;
 }
 bool record(std::uintptr_t id,std::shared_ptr<Record>& out){std::shared_ptr<SourceWorldBorrowV61> world;if(!current(world))return false;
  SourceCampaignCharacterBorrowV62 receipt;if(!borrow_source_campaign_character_v62(world->owner,id,receipt,error_)||!receipt.character||!receipt.character->actor||!receipt.character->actor->object||receipt.character->actor->object->identity!=id)return false;
  out=std::move(receipt.character);return true;
 }
 static int debug(void* raw,const SkillAttackNativeRequestV6* q,std::uintptr_t* out){
  if(!q||!out)return -1;auto& t=*static_cast<SourceCampaignCombatV115*>(raw);std::shared_ptr<SourceWorldBorrowV61> w;if(!t.current(w)||!w->debug||!w->debug_files)return -1;
  if(q->service==skill_attack_debug_load_v6)return dh2_character_debug_load(w->debug.get(),w->debug_files)==1?0:-1;
  if(q->service==skill_attack_string_construct_v6){if(!q->name)return -1;*out=++t.serial_;t.strings_.emplace(*out,q->name);return 0;}
  auto at=t.strings_.find(q->subject);if(at==t.strings_.end())return -1;
  if(q->service==skill_attack_string_destroy_v6){t.strings_.erase(at);return 0;}
  if(q->service==skill_attack_debug_get_v6){unsigned value{};if(dh2_character_debug_get(&value,w->debug.get(),at->second.c_str(),w->debug_files)!=1)return -1;*out=value;return 0;}return -1;
 }
 static int refresh(void* raw,WorldSkillCombatBorrowV6* out){
  if(!raw||!out)return -1;auto& a=*static_cast<Actor*>(raw);auto r=a.record.lock();if(!r||!r->actor||!r->actor->machine||!r->actor->controller||!r->life||!r->inventory37c)return -1;
  auto& actor=*r->actor;auto& machine=actor.machine->state();auto& fields=actor.machine->combat_fields();auto* aggro=actor.source_aggro_v84();
  if(!aggro||!aggro->outgoing()||!aggro->incoming()||r->inventory37c->character()!=actor.object->identity||r->inventory37c->properties()!=r->properties)return -1;
  a.facts={r->view.resolved,-1,-1,0,0,0,machine.current,fields.combo};
  const auto& slots=r->inventory37c->equipment()[r->inventory37c->current_equipment()];const dh2::data::ItemRecord164* weapon[2]{};
  for(unsigned hand=0;hand<2;++hand)if(slots[hand+1]){const auto* item=dh2::data::item(r->inventory37c->table(),slots[hand+1]->item->id);if(!item)return -1;weapon[hand]=&item->record;}
  dh2::player::EquipmentQueries12V1 equipment;if(dh2_equipment_queries_v1(&equipment,weapon[0],weapon[1],r->view.resolved[203]))return -1;
  a.facts.main_damage_class=equipment.main_category;a.facts.off_damage_class=equipment.off_category;
  a.facts.two_hander=(equipment.flags&dh2::player::query_two_raw)!=0;a.facts.dual_wield=(equipment.flags&dh2::player::query_dual)!=0;a.facts.shield=(equipment.flags&dh2::player::query_shield)!=0;
  a.controller=actor.controller->identity();a.previous_lifecycle=r->life->lifecycle;a.hit={actor.object->identity,&r->view,a.controller,r->life->lifecycle,0};
  BuffOwner* buffs{};if(r->player_script_owner_v62)buffs=r->player_script_owner_v62->native_buffs();else if(r->save_connection_v86&&!r->save_connection_v86->borrow_buffs(buffs,a.owner->error_))return -1;
  a.attack={actor.object->identity,&r->view,&a.facts};a.application={actor.object->identity,&r->view,buffs,&a.hit,nullptr,nullptr,&a.owner->dot_classes_v115_,&a.owner->dot_effects_v115_,&fields.combo,&fields.invulnerable,&fields.push_death,&fields.network_id};
  //Only native tree backing is admitted here; no source relation/count is
  //invented. The actual application performs reciprocal writes later.
  if(!aggro->prepare_outgoing_insert(aggro->outgoing()->count+1,a.owner->error_)||!aggro->prepare_incoming_insert(aggro->incoming()->count+1,a.owner->error_))return -1;
  *out={&a.attack,&a.application,r->life.get(),r->inventory37c,aggro->outgoing(),aggro->incoming()};return 0;
 }
 static int hit(void* raw,HitActor32* actor,const HitRequest32* q,std::uintptr_t* out){
  if(!actor||!q||!out)return -1;auto& t=*static_cast<SourceCampaignCombatV115*>(raw);std::shared_ptr<SourceWorldBorrowV61> w;if(!t.current(w))return -1;
  CallbackRandom random(t);if(!random.ready)return -1;
  if(q->service==hit_controller_kill){const auto status=source_campaign_hit_kill_v84(w->owner,actor,q,t.scope_,out,t.error_);return status==1?0:-1;}
  if(q->service==hit_main_player){const int result=w->player_manager->runtime()->hit(*q,out);return result==1?0:-1;}
  if(q->service==hit_local_player_v6){bool local{};if(!w->player_manager->source_is_local_player_v61(q->subject,local,t.error_))return -1;*out=local;return 0;}
  if(q->service==hit_online){auto online=w->application->get_online_loading_v55();if(!online)return -1;*out=online->byte5();return 0;}
  if(q->service==hit_application_switch){auto settings=w->application->source_settings4c_v67();if(!settings||!q->name)return -1;const auto* option=settings->descriptor(q->name);*out=settings->has_option(q->name)&&option&&option->type==0&&settings->option(q->name)==option->maximum;return 0;}
  if(q->service==hit_is_remotely_updated){std::shared_ptr<Record> r;if(!t.record(q->subject,r))return -1;auto* remote=r->actor->source_bool_field(0x118);if(!remote)return -1;*out=r->actor->machine->combat_fields().network_id!=-1||*remote;return 0;}
  if(q->service==hit_master_v116){std::shared_ptr<Record> r;if(!t.record(q->subject,r))return -1;auto* fields=r->actor->source_ai_pointers_v105();if(!fields)return -1;*out=fields->master50;return 0;}
  if(q->service==hit_trophy_manager_v6||q->service==hit_trophy_index_v6||q->service==hit_unlock_v6){dh2::android_ui::ProcessTrophyBorrowV100 trophy;if(!dh2::android_ui::borrow_source_process_trophies_v100(w->application,trophy,t.error_)||!trophy.native)return -1;return trophy.native->hit(q,out);}
  if(q->service==hit_major_enemy_v6||q->service==hit_minor_enemy_v6){std::shared_ptr<Record> r;if(!t.record(q->subject,r))return -1;const auto* row=r->design.ai()?dh2::data::ai_props(*r->design.ai(),r->properties->resolved[1]):nullptr;if(!row)return -1;*out=(row->flags>>(q->service==hit_major_enemy_v6?2:1))&1u;return 0;}
  t.error_="Required canonical HitFor source service "+std::to_string(q->service);return -1;
 }
 bool sync_random(bool publish){std::shared_ptr<SourceWorldBorrowV61> w;if(!current(w))return false;auto source=w->application->source_random_v62();if(!source)return false;auto& channel=source->channel(0);
  if(publish){channel.seed=random_.seed;channel.calls=random_.calls;}else{random_.seed=channel.seed;random_.calls=channel.calls;}return true;
 }
 bool enroll(std::uintptr_t id){if(actors_.count(id))return true;std::shared_ptr<Record> r;if(!record(id,r))return false;
  auto [at,inserted]=actors_.emplace(id,Actor{});auto& a=at->second;a.owner=this;a.record=r;
  if(combat_->add({id,&a,refresh})){actors_.erase(at);error_=combat_->error();return false;}return true;
 }
 static int aggro(void* raw,unsigned bit,std::uintptr_t actor,std::uintptr_t other){auto& t=*static_cast<SourceCampaignCombatV115*>(raw);std::shared_ptr<SourceWorldBorrowV61> w;if(!t.current(w))return -1;
  CallbackRandom random(t);if(!random.ready)return -1;
  return source_campaign_character_aggro_event_v84(w->owner,bit,actor,other,t.scope_,t.error_)?0:-1;
 }
 bool text(const dh2::data::CombatResult& result,std::uintptr_t attacker,std::uintptr_t target){
  if(!presentation_.owner||!presentation_.localized||!presentation_.enqueue){error_="Required actual canonical combat Flash/StringManager receiver";return false;}
  CombatTextServicesV1 s;s.context=this;
  s.follower=[](void* raw,std::uintptr_t id,bool* out){auto& t=*static_cast<SourceCampaignCombatV115*>(raw);WorldTargetActorBorrowV1 actor;if(!out||t.targets_->actor(id,&actor))return -1;return character_combat_follower_v1(out,actor,*t.design_.ai())==1?0:-1;};
  s.position=[](void* raw,std::uintptr_t id,float out[3]){auto& t=*static_cast<SourceCampaignCombatV115*>(raw);std::shared_ptr<Record> r;if(!out||!t.record(id,r))return -1;std::copy_n(r->actor->runtime.subobjects.position,3,out);return 0;};
  s.height=[](void* raw,std::uintptr_t id,float* out){auto& t=*static_cast<SourceCampaignCombatV115*>(raw);std::shared_ptr<Record> r;if(!out||!t.record(id,r)||!r->actor->source_bounds_ready)return -1;const auto* box=r->actor->runtime.subobjects.local_bounds;*out=box[5]-box[2];return 0;};
  s.property=[](void* raw,std::uintptr_t id,int index,int* out){auto& t=*static_cast<SourceCampaignCombatV115*>(raw);std::shared_ptr<Record> r;if(!out||index<0||index>=224||!t.record(id,r))return -1;*out=r->view.resolved[index];return 0;};
  s.dual_wield=[](void* raw,std::uintptr_t id,bool* out){auto& t=*static_cast<SourceCampaignCombatV115*>(raw);if(!out||!t.enroll(id))return -1;WorldSkillCombatBorrowV6 borrow;if(refresh(&t.actors_.at(id),&borrow))return -1;*out=borrow.attack->facts->dual_wield!=0;return 0;};
  s.is_player=[](void* raw,std::uintptr_t id,bool* out){auto& t=*static_cast<SourceCampaignCombatV115*>(raw);std::shared_ptr<Record> r;return out&&t.record(id,r)&&r->is_player(*out,t.error_)?0:-1;};
  s.constant=[](void* raw,const char* group,const char* name,int* out){auto& t=*static_cast<SourceCampaignCombatV115*>(raw);const auto* source=t.design_.design();return source&&source->lookup&&out?source->lookup(source->context,0,group,name,out):-1;};
  s.localized=[](void* raw,int id,const char** out){auto& t=*static_cast<SourceCampaignCombatV115*>(raw);return out&&t.presentation_.localized(id,*out,t.error_)?0:-1;};
  s.enqueue=[](void* raw,const CombatTextRequestV1* q){auto& t=*static_cast<SourceCampaignCombatV115*>(raw);return q&&t.presentation_.enqueue(*q,t.error_)?0:-1;};
  return character_combat_text_v1(result,attacker,target,s)==1;
 }
 static int application(void* raw,const SkillApplyRequestV6* q,SkillApplyResponseV6* out,dh2::data::CombatResult* result){
  if(!q||!out)return -1;auto& t=*static_cast<SourceCampaignCombatV115*>(raw);std::shared_ptr<SourceWorldBorrowV61> w;if(!t.current(w))return -1;
  CallbackRandom random(t);if(!random.ready)return -1;
  if(q->service==skill_apply_online_v6){auto online=w->application->get_online_loading_v55();if(!online)return -1;out->word=online->byte5();return 0;}
  if(q->service==skill_apply_party_count_v6){auto* count=w->player_manager->count_field();if(!count)return -1;out->word=static_cast<unsigned>(*count);return 0;}
  if(q->service==skill_apply_saved_option_v6){auto settings=w->application->source_settings4c_v67();if(!settings||!q->name)return -1;const auto* option=settings->descriptor(q->name);out->word=settings->has_option(q->name)&&option&&option->type==0&&settings->option(q->name)==option->maximum;return 0;}
  if(q->service==skill_apply_player_lookup_v6){dh2::player::PlayerInfoFieldsV1* info{};if(!w->player_manager->manager()->get_by_character(q->subject,false,info,t.error_)||!info)return -1;out->identity=reinterpret_cast<std::uintptr_t>(info);return 0;}
  if(q->service==skill_apply_trophy_manager_v116||q->service==skill_apply_trophy_index_v116||q->service==skill_apply_unlock_v116){dh2::android_ui::ProcessTrophyBorrowV100 trophy;if(!dh2::android_ui::borrow_source_process_trophies_v100(w->application,trophy,t.error_)||!trophy.native)return -1;
   HitRequest32 request{q->service==skill_apply_trophy_manager_v116?hit_trophy_manager_v6:q->service==skill_apply_trophy_index_v116?hit_trophy_index_v6:hit_unlock_v6,q->word,q->subject,0,q->name};std::uintptr_t result{};const int status=trophy.native->hit(&request,&result);out->identity=result;out->word=static_cast<unsigned>(result);return status;
  }
  if(q->service==skill_apply_local_player_v116){bool local{};if(!w->player_manager->source_is_local_player_v61(q->subject,local,t.error_))return -1;out->word=local;return 0;}
  if(q->service==skill_apply_hit_fx_v6){std::shared_ptr<void> pin;dh2::fx::CharacterMeshFxOwnerV4* fx;if(!result||!borrow_source_campaign_fx_v77(w->owner,pin,fx,t.error_)||!fx)return -1;
   CombatHitFxServicesV1 s{&t,[](void* raw,std::uintptr_t id,float out[3]){auto& t=*static_cast<SourceCampaignCombatV115*>(raw);std::shared_ptr<Record> r;if(!out||!t.record(id,r))return -1;std::copy_n(r->actor->runtime.rotation.rotation,3,out);return 0;}};
   return character_combat_hit_fx_v4(*t.targets_,w->effects->borrow(),fx,*result,q->target,s,t.error_)?0:-1;
  }
  if(q->service==skill_apply_scrolling_text_v6)return result&&t.text(*result,q->attacker,q->target)?0:-1;
  if(q->service==skill_apply_critical_camera_v6)return t.presentation_.owner&&t.presentation_.critical_camera&&t.presentation_.critical_camera(q->attacker,t.error_)?0:-1;
  if(q->service==skill_apply_ai_combat_v6){std::shared_ptr<Record> r;if(!result||!t.record(q->subject,r))return -1;CombatResultsAiBorrowV1 borrow;borrow.scope=t.scope_;
   if(r->player_script_owner_v62){auto& session=r->player_script_owner_v62->session();borrow.owner_v2=&session.owner();if(!session.owner().source_combat_results_callback(borrow.source_character_callback))return -1;}
   else if(r->actor->session){borrow.owner=&r->actor->session->owner();if(!borrow.owner->source_combat_results_callback(borrow.source_character_callback))return -1;}
   //NULL active AIS remains the actual source-null return inside this owner.
   return character_combat_results_ai_v1(borrow,q->attacker,q->target,static_cast<std::uint8_t>(result->outcomes),t.error_)>=0?0:-1;
  }
  const int handled=source_campaign_character_reaction_v115(w->owner,q->subject,*q,*out,t.error_);if(handled)return handled==1?0:-1;
  t.error_="Required canonical F_ApplyResult source service "+std::to_string(q->service);return -1;
 }
 static int whole_application(void* raw,SkillApplyOutputV6* out,dh2::data::CombatResult* result,SkillApplyActorV6* attacker,SkillApplyActorV6* target,const SkillApplyServicesV6* services){
  auto& t=*static_cast<SourceCampaignCombatV115*>(raw);std::shared_ptr<SourceWorldBorrowV61> w;std::shared_ptr<Record> r;if(!out||!result||!attacker||!target||!t.current(w)||!t.record(target->identity,r))return -1;
  struct Tail {SourceCampaignCombatV115& owner;std::shared_ptr<SourceWorldBorrowV61> world;std::shared_ptr<Record> target;} context{t,w,r};
  dh2::world::GameObjectInitializationFieldsV62 actual_fields;
  if(!r->actor->inherited_initialization_fields_v62(r,actual_fields,t.error_)||!actual_fields.byte)return -1;
  auto settings=w->application->source_settings4c_v67();HitPlayerTailBorrowV7 fields{target->identity,0,target->properties,actual_fields.byte(0x1448),settings?settings->source_tutorial_cell_v116(0x2d):nullptr};
  HitPlayerTailServicesV7 tail;tail.context=&context;
  tail.online=[](void* raw,bool* out){auto& c=*static_cast<Tail*>(raw);auto actual=c.world->application->get_online_loading_v55();if(!out||!actual)return -1;*out=actual->byte5()!=0;return 0;};
  tail.is_player=[](void* raw,std::uintptr_t id,bool* out){auto& c=*static_cast<Tail*>(raw);std::shared_ptr<Record> r;return out&&c.owner.record(id,r)&&r->is_player(*out,c.owner.error_)?0:-1;};
  tail.difficulty=[](void* raw,std::uintptr_t id,int* out){auto& c=*static_cast<Tail*>(raw);auto& r=*c.target;if(!out||id!=r.actor->object->identity||!r.save_fields)return -1;return dh2::player::character_game_difficulty_v29({r.actor,id,r.save_fields->save_slot14e8(),r.services.difficulty_global},*out,c.owner.error_)?0:-1;};
  tail.script_id=[](void* raw,const char* name,int* out){auto& c=*static_cast<Tail*>(raw);auto scripts=c.world->application->source_script_manager_v52();if(!out||!name||!scripts)return -1;*out=scripts->id_from_name(name,true);return 0;};
  tail.start_script=[](void* raw,int id,int actor,bool flag){auto& c=*static_cast<Tail*>(raw);auto scripts=c.world->application->source_script_manager_v52();return scripts&&scripts->start_script_v96(id,actor,flag,c.owner.error_)?0:-1;};
  tail.start_update_job=[](void* raw){auto& c=*static_cast<Tail*>(raw);if(!c.owner.presentation_.settings_job_start){c.owner.error_="Required actual settings updateJob.Start2 after tutorial2d store";return -1;}return c.owner.presentation_.settings_job_start(c.owner.error_)?0:-1;};
  tail.sound_index=[](void* raw,const char* name,int* out){auto& c=*static_cast<Tail*>(raw);if(!out||!c.owner.presentation_.sound_index){c.owner.error_="Required actual plain Vox low-health source index";return -1;}return c.owner.presentation_.sound_index(name,*out,c.owner.error_)?0:-1;};
  tail.play_sound=[](void* raw,int id,bool loop,int begin,int end,bool flag){auto& c=*static_cast<Tail*>(raw);if(!c.owner.presentation_.play_sound){c.owner.error_="Required actual Vox.Play(index,false,0,0,false)";return -1;}return c.owner.presentation_.play_sound(id,loop,begin,end,flag,c.owner.error_)?0:-1;};
  const CharacterHitPlayerServicesV116 hit{&fields,&tail};
  const DotPlayerReactionServicesV7 reaction{&context,[](void* raw,std::uintptr_t id,bool* out){auto& c=*static_cast<Tail*>(raw);if(!out||id!=c.target->actor->object->identity||!c.target->actor->machine)return -1;int current{};if(dh2_character_native_fsm_get_integer(&current,&c.target->actor->machine->native_fsm(),0)!=1)return -1;*out=current==3;return 0;}};
  const SkillApplyPlayerServicesV116 source{&hit,&reaction};return character_skill_apply_result_v116(out,result,attacker,target,services,&source);
 }
public:
 SourceCampaignCombatV115(std::shared_ptr<SourceWorldBorrowV61> world,std::shared_ptr<CharacterWorldRuntimeV1> targets):world_(world),targets_(std::move(targets)),design_(world->design->borrow()){
  effects_tables_=world->effects->borrow();for(const auto& name:design_.classes()->names)class_names_v115_.push_back(name.c_str());for(const auto& name:effects_tables_.set_names())effect_names_v115_.push_back(name.c_str());
  dot_classes_v115_={class_names_v115_.data(),static_cast<unsigned>(class_names_v115_.size()),0};dot_effects_v115_={effect_names_v115_.data(),static_cast<unsigned>(effect_names_v115_.size()),0};
  WorldSkillCombatBackendsV6 services;services.hit={this,hit};services.application={this,application,&debug_};services.aggro_context=this;services.aggro_event=aggro;services.whole_application_context=this;services.whole_application=whole_application;
  combat_=std::make_unique<CharacterWorldSkillCombatV6>(*targets_,*design_.ai(),debug_,services);
  WorldAttackGeometryServicesV1 queries;queries.context=this;
  queries.inventory=[](void* raw,std::uintptr_t id,WorldAttackInventoryBorrowV1* out){auto& t=*static_cast<SourceCampaignCombatV115*>(raw);std::shared_ptr<Record> r;if(!out||!t.record(id,r)||!r->inventory37c)return -1;out->owned=r->inventory37c;return 0;};
  queries.object_kind=[](void* raw,std::uintptr_t id,int* out){auto& t=*static_cast<SourceCampaignCombatV115*>(raw);std::shared_ptr<SourceWorldBorrowV61> w;if(!out||!t.current(w)||!w->canonical_world)return -1;const dh2::world::CanonicalObjectBorrowV1* object{};int key{};bool more=w->canonical_world->manager.source_ordered_begin_v38(key,object);while(more){if(object&&object->identity==id&&object->type_f4){*out=*object->type_f4;return 0;}more=w->canonical_world->manager.source_ordered_next_v38(key,key,object);}return -1;};
  geometry_=std::make_unique<CharacterWorldAttackGeometryV1>(*targets_,*design_.ai(),*world->debug,*world->debug_files,queries);
 }
 bool presentation(SourceCombatPresentationV115 input){if(presentation_.owner||!input.owner||!input.localized||!input.enqueue||!input.critical_camera||!input.sound_index||!input.play_sound||!input.settings_job_start){error_="Required once-only actual combat/text/camera/plain Vox/settings-job receivers";return false;}presentation_=std::move(input);return true;}
 const std::string& error()const noexcept{return error_;}
 bool forget(std::uintptr_t id){auto at=actors_.find(id);if(at==actors_.end())return true;if(delivery_depth_){error_="Combat actor retirement overlaps actual native delivery";return false;}if(combat_->remove(id))return false;actors_.erase(at);return true;}
 void publish(){for(auto& [id,a]:actors_){auto r=a.record.lock();if(!r||!r->actor||!r->actor->machine||!r->life)continue;const auto& fields=r->actor->machine->combat_fields();r->life->combo_hits=fields.combo;r->life->push_death=fields.push_death;}}
 bool melee(std::uintptr_t id,const MeleeAnimationRequestV1& q,MeleeAnimationResponseV1& out){
  std::shared_ptr<Record> r;if(!record(id,r)||!enroll(id))return false;
  if(q.operation==melee_event_can_range){int value{};if(!geometry_->read(id,r->actor->object->target.target,WorldAIAttackQueryV1::CharacterCanRangeAttack,value,error_))return false;out.value=value;return true;}
  if(q.operation!=melee_event_attack){error_="Required canonical authored combat operation "+std::to_string(q.operation);return false;}
  const auto target=r->actor->object->target.target;std::uintptr_t character{};std::shared_ptr<SourceWorldBorrowV61> world;if(!current(world)||!source_campaign_object_as_character_v114(world->owner,target,character,error_))return false;if(character&&!enroll(character))return false;
  auto& ai=r->actor->ai_events;const std::uint32_t address=ai.active&&ai.ais_virtuals?static_cast<std::uint32_t>(ai.ais_virtuals[0xa8/4]):0;
  WorldMeleeAttackBorrowV1 fields{id,&r->actor->object->target.target,&r->actor->object->target.target,&ai.active,&address,r->inventory37c};
  if(!sync_random(false))return false;++delivery_depth_;struct Exit{SourceCampaignCombatV115& t;~Exit(){t.sync_random(true);t.publish();--t.delivery_depth_;}}exit{*this};
  CharacterWorldMeleeAttackV1 attack(*combat_,*targets_,context_,random_,fields,geometry_->queries(),{});
  const int result=attack.attack(q.index,q.step,q.offhand);if(!sync_random(false))return false;if(result){error_=attack.error();return false;}return true;
 }
 bool roll(std::uintptr_t id,const dh2_script_value* args,unsigned count,dh2_script_value* values,unsigned capacity,unsigned& written){
  written=0;std::shared_ptr<Record> r;std::shared_ptr<SourceWorldBorrowV61> w;if(!current(w)||!record(id,r)||!enroll(id)||!r->services.skills)return false;
  const auto* previous_scope=scope_;scope_=r->player_script_owner_v62?r->player_script_owner_v62->session().current_skill_callback_scope():r->actor->session?r->actor->session->current_skill_callback_scope():nullptr;
  if(!sync_random(false)){scope_=previous_scope;return false;}++delivery_depth_;
  struct Exit{SourceCampaignCombatV115& t;const dh2_script_callback_scope* previous;~Exit(){t.sync_random(true);t.publish();--t.delivery_depth_;t.scope_=previous;}}exit{*this,previous_scope};
  struct Call {SourceCampaignCombatV115& owner;std::shared_ptr<Record> actor;dh2::target_providers::Handle16* handle{};dh2::target_providers::Registry24* registry{};dh2::target_providers::Handle16 local{};SkillCombatRowV6 row{};
   static int invoke(void* raw,const SkillCombatRequestV6* q,SkillCombatResponseV6* out,dh2::data::CombatResult* result){auto& c=*static_cast<Call*>(raw);auto& t=c.owner;if(!q||!out)return -1;const auto& tables=c.actor->services.skills;const int list=c.actor->view.resolved[28];
    switch(q->service){
     case skill_combat_list_v6:if(list<0||std::size_t(list)>=tables.lists().size())return -1;out->word=tables.lists()[list].size();return 0;
     case skill_combat_handle_v6:if(t.targets_->handle_borrow(q->target,&c.handle,&c.registry)||!c.handle||!c.registry)return -1;out->identity=reinterpret_cast<std::uintptr_t>(c.handle);return 0;
     case skill_combat_character_v6:if(q->target!=reinterpret_cast<std::uintptr_t>(c.handle))return -1;{const auto queries=t.targets_->targets().query_services();return dh2::target_providers::dh2_target_handle_character(&out->identity,&c.local,c.handle,c.registry,&queries)?-1:0;}
     case skill_combat_row_v6:if(list<0||std::size_t(list)>=tables.lists().size()||q->index>=tables.lists()[list].size())return -1;{const int id=tables.lists()[list][q->index];if(id<0||std::size_t(id)>=tables.skills().size())return -1;const auto& record=tables.skills()[id].scalar;c.row={static_cast<int>(record.words[6]),record.words[7]};out->row=&c.row;return 0;}
     case skill_combat_main_hand_v6:case skill_combat_off_hand_v6:out->word=c.actor->inventory37c->equipment()[c.actor->inventory37c->current_equipment()][q->service==skill_combat_main_hand_v6?1:2]!=nullptr;return 0;
     case skill_combat_calculate_v6:case skill_combat_apply_v6:{if(!result||!t.enroll(q->target))return -1;auto native=t.combat_->native_world();SkillAttackActorV6 *a{},*b{};SkillApplyActorV6 *aa{},*bb{};if(native.actor(native.context,q->attacker,&a,&aa)||native.actor(native.context,q->target,&b,&bb))return -1;
      if(q->service==skill_combat_calculate_v6){const int status=dh2_character_skill_attack_calculate_v6(result,&t.context_,&t.random_,a,b,c.actor->inventory37c,q->mask,q->element,&t.debug_);if(!t.sync_random(true))return -1;return status;}
      SkillApplyOutputV6 applied;const int status=t.combat_->apply(&applied,result,q->attacker,q->target);if(!t.sync_random(false))return -1;return status;}
     default:t.error_="Required original nonCharacter skill activation branch "+std::to_string(q->service);return -1;
    }
   }
  }call{*this,r};
  const SkillCombatServicesV6 services{&call,Call::invoke};SkillCombatOutputV6 result;
  if(dh2_character_skill_combat_roll_v6(&result,id,args,count,&services)){if(error_.empty())error_=combat_->error();if(error_.empty())error_="Required source _SkillCombatRoll phase "+std::to_string(result.phase);return false;}
  const auto needed=result.count+result.boolean_count;if(needed>capacity){error_="Source combat returns exceed actual VM return storage after effects";return false;}
  for(unsigned i=0;i<result.count;++i){values[i]={};values[i].type=DH2_SCRIPT_NUMBER;values[i].number=result.amount[i];}if(result.boolean_count){values[result.count]={};values[result.count].type=DH2_SCRIPT_BOOLEAN;values[result.count].boolean=0;}written=needed;return true;
 }
};
namespace {
bool borrow(const std::shared_ptr<void>& actual,std::shared_ptr<SourceWorldBorrowV61>& out,std::string& e){SourceCampaignCandidateBorrowV55 candidate;
 if(!actual||!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=actual||!borrow_source_campaign_condition_world_v70(candidate,out,e)){if(e.empty())e="Required SAME current canonical combat World";return false;}return true;
}
struct ScriptBindingV115 {
 std::weak_ptr<void> world;std::uintptr_t character{};void* previous_context{};
 int(*previous)(void*,std::uint32_t,dh2_script_function*,void**){};
 static int select(void* raw,unsigned address,dh2_script_function* function,void** context){if(!raw||!function||!context)return -1;auto& t=*static_cast<ScriptBindingV115*>(raw);
  if(address!=0x3b9fbc)return t.previous?t.previous(t.previous_context,address,function,context):0;
  *function=invoke;*context=&t;return 1;
 }
 static int invoke(void* raw,const dh2_script_value* args,unsigned count,dh2_script_value* values,unsigned capacity,unsigned* written,char* error,std::size_t size){
  if(!raw||!written||(count&&!args)||(capacity&&!values))return -1;*written=0;auto& t=*static_cast<ScriptBindingV115*>(raw);std::string diagnostic;
  try{auto actual=t.world.lock();std::shared_ptr<SourceWorldBorrowV61> w;
   if(borrow(actual,w,diagnostic)&&w->combat_v115){if(w->combat_v115->roll(t.character,args,count,values,capacity,*written))return 0;diagnostic=w->combat_v115->error();}
   else if(diagnostic.empty())diagnostic="Missing actual canonical combat owner at _SkillCombatRoll";
  }catch(const std::exception& failure){diagnostic=failure.what();}
  if(error&&size)std::snprintf(error,size,"%s",diagnostic.c_str());return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
 }
};
template<class Input>bool bind_input(const std::shared_ptr<void>& actual,std::uintptr_t id,Input& input,std::shared_ptr<void>& holder,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> w;if(holder||!id||!borrow(actual,w,e)||!w->combat_v115){if(e.empty())e="Required once-bound actual canonical combat script owner";return false;}
 auto binding=std::make_shared<ScriptBindingV115>();binding->world=actual;binding->character=id;binding->previous_context=input.gameplay_context;binding->previous=input.gameplay_binding;
 input.gameplay_context=binding.get();input.gameplay_binding=ScriptBindingV115::select;holder=std::move(binding);e.clear();return true;
}
}
bool bind_source_campaign_combat_v115(const SourceCampaignCandidateBorrowV55& candidate,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> w;std::shared_ptr<CharacterWorldRuntimeV1> targets;
 if(!borrow(candidate.actual_world,w,e)||w->combat_v115||!w->design||!w->debug||!w->debug_files||!w->application||!w->application->source_random_v62()||!borrow_source_campaign_character_targets_v88(candidate.actual_world,targets,e)||!targets){if(e.empty())e="Required once-produced actual canonical combat source authorities";return false;}
 auto design=w->design->borrow();if(!design.ai()||!design.classes()||!w->effects||!w->effects->borrow()){e="Required actual combat CharAI/Class/Effects tables";return false;}
 w->combat_v115=std::make_shared<SourceCampaignCombatV115>(w,std::move(targets));e.clear();return true;
}
bool bind_source_campaign_combat_presentation_v115(const std::shared_ptr<void>& actual,SourceCombatPresentationV115 input,std::string& e){std::shared_ptr<SourceWorldBorrowV61> w;if(!borrow(actual,w,e)||!w->combat_v115)return false;
 if(!w->combat_v115->presentation(std::move(input))){e=w->combat_v115->error();return false;}e.clear();return true;
}
bool source_campaign_melee_event_v115(const std::shared_ptr<void>& actual,std::uintptr_t id,const MeleeAnimationRequestV1& q,MeleeAnimationResponseV1& out,std::string& e){std::shared_ptr<SourceWorldBorrowV61> w;if(!borrow(actual,w,e)||!w->combat_v115)return false;
 if(!w->combat_v115->melee(id,q,out)){e=w->combat_v115->error();return false;}e.clear();return true;
}
bool bind_source_character_combat_natives_v115(const std::shared_ptr<void>& w,std::uintptr_t id,CharacterScriptSessionInput& input,std::shared_ptr<void>& pin,std::string& e){return bind_input(w,id,input,pin,e);}
bool bind_source_character_combat_natives_v115(const std::shared_ptr<void>& w,std::uintptr_t id,CharacterScriptSessionInputV3& input,std::shared_ptr<void>& pin,std::string& e){return bind_input(w,id,input,pin,e);}
bool source_campaign_forget_combat_actor_v115(const std::shared_ptr<void>& actual,std::uintptr_t id,std::string& e){std::shared_ptr<SourceWorldBorrowV61> w;if(!borrow(actual,w,e))return false;if(!w->combat_v115)return true;
 if(!w->combat_v115->forget(id)){e=w->combat_v115->error();return false;}return true;
}
}
