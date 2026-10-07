#include "source_campaign_death_rewards_v84.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_fx_v77.hpp"
#include "source_process_trophies_v100.hpp"
#include "source_campaign_save_objects_v86.hpp"
#include "source_campaign_character_fsm_v101.hpp"
#include "source_campaign_script_actor_v96.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "model_renderer.hpp"
#include <canonical_character_candidate_v60.hpp>
#include <application_player_manager_bootstrap_v59.hpp>
#include <application_services_owner_v5.hpp>
#include <world_loot_gameplay_v23.hpp>
#include <player_save_write_owner_v1.hpp>
#include <source_assertion_process_v76.hpp>
#include <character_design_services.hpp>
#include <character_world_aggro_event_v1.hpp>
#include <character_script_source_virtuals_v101.hpp>
#include <integration-v42/audio_application_manager_v42.hpp>
#include <vox_music_state_owner_v1.hpp>
#include <character_ai_groups_v87.hpp>
#include <character_world_clear_aggro_v40.hpp>
#include <script_manager_owner_v52.hpp>
#include <owned_hud_settings_v1.hpp>
#include <cstring>
#include <set>
#include <stdexcept>
namespace model_renderer {
class SourceCampaignDeathRewardsV84 {
 using Record=dh2::world::CanonicalCharacterCandidateRecordV60;
 std::weak_ptr<SourceWorldBorrowV61> world_;
 std::weak_ptr<dh2::loader::CanonicalLevelContextV1> level_;
 SourceDeathRewardsLeavesV84 leaves_;
 dh2::character::KillWorld16 globals_{};
 std::unique_ptr<dh2::character::CharacterKillProductionV23> kill_;
 std::unique_ptr<dh2::character::CharacterProgressionWorldV23> progression_;
 dh2::data::DesignSettingsOwner::Borrow settings_;
 std::shared_ptr<dh2::character::skills::CharacterWorldRuntimeV1> targets_;
 std::set<std::uintptr_t> enrolled_,enrolling_;
 std::map<std::uintptr_t,std::weak_ptr<Record>> deaths_;
 const dh2_script_callback_scope* scope_{};
 std::vector<dh2::android_ui::ProcessTrophyBorrowV100> trophy_pins_;
 std::shared_ptr<dh2::audio::AudioApplicationManagerV42> audio_pin_;
 std::string diagnostic_;
 bool current(std::shared_ptr<SourceWorldBorrowV61>& w,std::string& e)const{
  w=world_.lock();if(!w||!w->owner||!w->application||!w->player_manager||!w->player_manager->manager()){
   e="Released SAME campaign death World/App/PlayerManager";return false;}
  SourceCampaignCandidateBorrowV55 candidate;if(!borrow_source_campaign_candidate_v55(candidate,e))return false;
  auto level=level_.lock();if(!level||candidate.actual_world!=w->owner||candidate.level!=level||
   !w->canonical_world||candidate.objects!=w->canonical_world->manager_lease){e="Campaign death world/Level generation changed";return false;}return true;
 }
 bool record(std::uintptr_t id,std::shared_ptr<Record>& out,std::string& e)const{
  std::shared_ptr<SourceWorldBorrowV61> w;if(!current(w,e))return false;
  SourceCampaignCharacterBorrowV62 b;if(!borrow_source_campaign_character_v62(w->owner,id,b,e)||!b.character||!b.character->actor||
   !b.character->actor->object||b.character->actor->object->identity!=id){if(e.empty())e="Required SAME published canonical death Character";return false;}
  out=std::move(b.character);return true;
 }
 bool attacker_character(std::uintptr_t id,std::uintptr_t& character,std::string& e)const{
  character=0;if(!id)return true;std::shared_ptr<SourceWorldBorrowV61> w;if(!current(w,e))return false;
  std::int32_t key{};const dh2::world::CanonicalObjectBorrowV1* object{};
  bool more=w->canonical_world->manager.source_ordered_begin_v38(key,object);
  while(more){if(object&&object->identity==id){if(!object->as_character){e="Required actual attacker virtual24 Character cast";return false;}return object->as_character(object->context,character,e);}
   more=w->canonical_world->manager.source_ordered_next_v38(key,key,object);}
  e="Kill attacker is not a published SAME canonical object";return false;
 }
 bool debug(const char* name,bool& result,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> w;if(!current(w,e)||!w->debug||!w->debug_files){if(e.empty())e="Required actual death/XP Debug owner";return false;}
  std::uint32_t value{};if(dh2_character_debug_load(w->debug.get(),w->debug_files)!=1||
   dh2_character_debug_get(&value,w->debug.get(),name,w->debug_files)!=1){e="Actual death/XP Debug query failed";return false;}result=value!=0;return true;
 }
 bool constant(const char* group,const char* key,std::int32_t& out,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> w;if(!current(w,e)||!w->design)return false;
  auto design=w->design->borrow();const auto* lookup=design.design();
  if(!lookup||!lookup->lookup||!group||!key||lookup->lookup(lookup->context,0,group,key,&out)){e="Required actual campaign death/XP constant";return false;}return true;
 }
 static bool aggro_actor(void* raw,std::uintptr_t id,dh2::character::AggroClearActorBorrowV2& out,std::string& e){
  auto& self=*static_cast<SourceCampaignDeathRewardsV84*>(raw);std::shared_ptr<Record> r;if(!self.record(id,r,e))return false;
  auto* a=r->actor.get();auto* maps=a->source_aggro_v84();if(!maps||!maps->outgoing()||!maps->incoming()){
   e="Required SAME source C1/observed canonical CharAI relation maps";return false;}
  out={id,maps->outgoing(),maps->incoming(),&a->object->binding};return true;
 }
 static bool deaggro(void* raw,std::uintptr_t id,std::uintptr_t other,const dh2_script_callback_scope* scope,std::string& e){
  return dispatch_aggro(raw,id,other,scope,true,e);
 }
 static bool dispatch_aggro(void* raw,std::uintptr_t id,std::uintptr_t other,const dh2_script_callback_scope* scope,bool cleared,std::string& e){
  auto& self=*static_cast<SourceCampaignDeathRewardsV84*>(raw);std::shared_ptr<Record> r;if(!self.record(id,r,e))return false;
  if(scope&&!dh2_script_callback_scope_valid(scope)){e="Expired actual reciprocal DeAggro scope";return false;}
  auto& ai=r->actor->ai_events;
  std::shared_ptr<SourceWorldBorrowV61> w;if(!self.current(w,e)||!w->debug||!w->debug_files)return false;
  dh2::character::ScriptSessionView selected;
  if(r->player_script_owner_v62){
   auto& scripts=r->player_script_owner_v62->session().owner();ai.active=scripts.lifecycle().active;
   if(ai.active&&!scripts.active(selected)){e="Missing SAME player active AIS";return false;}
  }else{if(!r->actor->session){e="Required actual NPC selected AIS for DeAggro";return false;}
   auto& scripts=r->actor->session->owner();ai.active=scripts.lifecycle().active;
   if(ai.active&&!scripts.active(selected)){e="Missing SAME NPC active AIS";return false;}}
  if(ai.active)ai.ais_virtuals=dh2::character::character_script_source_virtuals_v101(selected.kind);
  self.diagnostic_.clear();
  const auto status=cleared?dh2::character::world_deaggro_event_v2(ai,other,w->debug.get(),w->debug_files,{&self,selected_deaggro},e):
   dh2::character::world_aggro_event_v1(ai,other,w->debug.get(),w->debug_files,{&self,selected_deaggro},e);
  if(status){if(!self.diagnostic_.empty())e+="; "+self.diagnostic_;return false;}return true;
 }
 static int selected_deaggro(void* raw,dh2::character::AIEventState64* ai,std::uintptr_t method,std::uintptr_t other){
  auto& self=*static_cast<SourceCampaignDeathRewardsV84*>(raw);if(!ai||!ai->owner||(method!=0x3dde48&&method!=0x3ddb70))return -1;
  std::shared_ptr<Record> r;if(!self.record(ai->owner->owner,r,self.diagnostic_)||&r->actor->ai_events!=ai||!r->player_script_owner_v62)return -1;
  std::shared_ptr<void> pin;std::uintptr_t selected{};dh2::character::PlayerAggroFieldsV1* fields{};
  if(!r->player_script_owner_v62->session().owner().source_player_aggro_v84(pin,selected,fields)||selected!=ai->active||!fields){self.diagnostic_="Required selected SAME AISPlayer constructor fields";return -1;}
  std::shared_ptr<SourceWorldBorrowV61> w;if(!self.current(w,self.diagnostic_)||!w->debug||!w->debug_files)return -1;
  dh2::character::CharacterPlayerAggroOwnerV1 receiver(selected,ai->owner->owner,*fields,*w->debug,*w->debug_files,{&self,player_aggro});
  const auto status=method==0x3dde48?receiver.on_deaggro(other):receiver.on_aggro(other);
  if(status!=0){if(self.diagnostic_.empty())self.diagnostic_=receiver.error();return -1;}return 0;
 }
 static int player_aggro(void* raw,const dh2::character::PlayerAggroRequestV1* q,dh2::character::PlayerAggroResponseV1* out){
  auto& self=*static_cast<SourceCampaignDeathRewardsV84*>(raw);if(!q||!out)return -1;*out={};
  std::shared_ptr<SourceWorldBorrowV61> w;if(!self.current(w,self.diagnostic_))return -1;
  using namespace dh2::character;
  if(q->service==player_aggro_online_v1){auto online=w->application->get_online_loading_v55();if(!online){self.diagnostic_="Required actual COnline owner";return -1;}out->word=online->byte5();return 0;}
  if(q->service==player_aggro_local_player_v1){bool value{};if(!w->player_manager->source_is_local_player_v61(q->player,value,self.diagnostic_))return -1;out->word=value;return 0;}
  if(q->service==player_aggro_current_level_v1){dh2::loader::CanonicalCurrentLevelBorrowV1 level;if(!borrow_current_native_level_v27(level,self.diagnostic_))return -1;
   if(!level){out->level={};return 0;}
   if(level.level()!=self.level_.lock()){self.diagnostic_="Aggro borrowed a different current Level";return -1;}
   auto fields=level.level()->config_fields();out->level={level.identity(),fields.config38,nullptr,fields.music11c};
   //OnDeAggro's first getter discards the result, and its later music branch
   //reads11c only. OnAggro additionally needs the real config's byte1c8; its
   //typed lender below supplies that field, never an authored/default guess.
   if(!self.leaves_.player_aggro)return 0;
   return self.leaves_.player_aggro(*q,*out,self.diagnostic_)?0:-1;
  }
  if(q->service==player_aggro_sound_v1){dh2::audio::AudioApplicationBorrowV42 audio;
   if(!borrow_actual_application_audio_v42(audio,self.diagnostic_)||!audio.manager){if(self.diagnostic_.empty())self.diagnostic_="Original positive VoxSoundManager getter unavailable";return -1;}
   self.audio_pin_=audio.manager;auto& fields=audio.manager->music_fields_on_producer();out->sound={audio.identity(),&fields.ambient_31,&fields.level_music_32};return 0;}
  if(q->service==player_aggro_set_music_state_v1){dh2::audio::AudioApplicationBorrowV42 audio;
   if(!q->name||!borrow_actual_application_audio_v42(audio,self.diagnostic_)||!audio.manager)return -1;
   self.audio_pin_=audio.manager;auto& fields=audio.manager->music_fields_on_producer();
   if(fields.current_music_24<0){dh2::sound::VoxMusicStateOwnerV1 source(fields,{});const auto status=source.set_music_state(q->name);if(status)self.diagnostic_=source.error();return status;}
  }
  if(q->service==player_aggro_other_weight_v1){std::shared_ptr<Record> other;if(!self.record(q->other,other,self.diagnostic_))return -1;
   const auto* rows=other->design.ai();const auto* row=rows?dh2::data::ai_props(*rows,other->properties->resolved[1]):nullptr;
   if(!row){self.diagnostic_="Required SAME Character GetCharAI row";return -1;}std::memcpy(&out->integer,&row->flags,4);return 0;}
  if(q->service==player_aggro_threshold_v1){const auto* word=self.settings_.word(0,4);if(!word){self.diagnostic_="Required actual EnemySpotted weight threshold";return -1;}std::memcpy(&out->integer,word,4);return 0;}
  if(!self.leaves_.player_aggro||!self.leaves_.player_aggro(*q,*out,self.diagnostic_)){if(self.diagnostic_.empty())self.diagnostic_="Required original campaign LevelConfig/Vox/trace aggro transport";return -1;}return 0;
 }
 bool refresh(std::uintptr_t id,std::string& e){
  std::shared_ptr<Record> r;if(!record(id,r,e)||!r->actor->controller||!r->actor->machine)return false;
  dh2::character::ControllerCommandState32 command{};
  if(!r->services.controller||!r->services.controller(command,*r,e))return false;
  auto& ai=r->actor->ai_events;if(!ai.owner){e="Required SAME canonical AI owner fields";return false;}
  ai.owner->forced=command.forced;ai.owner->locked=command.locked;ai.global_blocked=command.global_blocked;
  if(r->player_script_owner_v62)ai.active=r->player_script_owner_v62->session().owner().lifecycle().active;
  else if(r->actor->session)ai.active=r->actor->session->owner().lifecycle().active;
  else {e="Required actual selected canonical contributor ScriptOwner";return false;}return true;
 }
 bool ensure_death(const std::shared_ptr<Record>& r,std::string& e){
  if(r->death_v84)return true;
  auto* a=r->actor.get();if(!a||!a->object||!a->machine||(!r->player_script_owner_v62&&!a->session)){e="Required existing canonical FSM/private ScriptOwner before death transport";return false;}
  dh2::character::CanonicalCharacterDeathServicesV84 s;s.provider=leaves_.owner;s.aggro={this,aggro_actor,deaggro};
  s.group=[this](auto group,auto owner,auto attacker,auto& e){std::shared_ptr<Record> receiver;if(!record(owner,receiver,e))return false;
   const auto* actual_scope=scope_;return dh2::character::source_character_group_on_died_v87(*receiver,group,attacker,
    [this,actual_scope](Record& member,auto killer,auto& e){if(!member.actor||!member.actor->object||!member.actor->controller){e="Required SAME GroupInfo member controllable/controller";return false;}
     return command(member.actor->controller->identity(),member.actor->object->identity,killer,0,actual_scope,e);},e);};
  std::weak_ptr<SourceWorldBorrowV61> weak=world_;s.fx=[weak](auto& pin,auto*& manager,auto& e){auto w=weak.lock();if(!w){e="Released actual campaign FX lifetime";return false;}return borrow_source_campaign_fx_v77(w->owner,pin,manager,e);};
  std::weak_ptr<Record> actor=r;s.stance=[actor](auto& out,auto& e){auto r=actor.lock();if(!r){e="Released actual death stance receiver";return false;}
   bool player{};if(!r->is_player(player,e))return false;dh2::character::StanceFacts16 facts{};
   if(player){auto* gear=r->prepared_equipment_v60;if(!gear){e="Required SAME player Gear GetAnimStance";return false;}
    std::int32_t count{};auto design=r->design.design();if(!design||!design->lookup||design->lookup(design->context,0,"AnimStances","COUNT_IPHONE",&count)){e="Required original GetAnimStance count constant";return false;}
    if(!gear->stance_facts(true,count,facts,e))return false;}
   else {auto design=r->design.design();if(!design||!design->lookup||design->lookup(design->context,0,"AnimStances","COUNT_IPHONE",&facts.count)){e="Required original NPC GetAnimStance count constant";return false;}}
   if(dh2_character_anim_stance(&out,&facts)!=1){e="Actual GetAnimStance failed";return false;}return true;
  };
  s.buffs=[this,id=a->object->identity](auto*& buffs,auto& e){std::shared_ptr<SourceWorldBorrowV61> w;if(!current(w,e))return false;
   //Source save/object transport retains the sole NPC BuffOwner over this
   //same private Session/property view/timers. Never create one during death.
   return borrow_source_campaign_character_buffs_v86(w->owner,id,buffs,e);};
  try{r->death_v84=std::make_unique<dh2::character::CanonicalCharacterDeathV84>(*r,std::move(s));deaths_[a->object->identity]=r;return true;}
  catch(const std::exception& failure){e=failure.what();return false;}
 }
 bool enroll(std::uintptr_t id,std::string& e){
  if(enrolled_.count(id))return true;if(!enrolling_.insert(id).second){e="Canonical source master14d4 cycle";return false;}
  struct Pop{std::set<std::uintptr_t>& ids;std::uintptr_t id;~Pop(){ids.erase(id);}}pop{enrolling_,id};
  std::shared_ptr<Record> r;if(!record(id,r,e)||!refresh(id,e)||!ensure_death(r,e))return false;
  auto& a=*r->actor;auto* relations=a.source_aggro_v84();if(!relations||!relations->outgoing()){e="Required SAME canonical Kill source map";return false;}
  if(!r->services.world_targets||r->services.world_targets!=targets_||!a.controller||!a.kill_fields_v42().produced){e="Canonical Kill receiver has different targetWorld/unproduced C1";return false;}
  const std::int32_t* oid{};const std::int16_t* property{};std::uintptr_t* tracked{};a.kill_metadata_borrow_v23(oid,property,tracked);
  auto* view=r->player_script_owner_v62?&r->player_script_owner_v62->session().property_view():&a.session->property_view();
  dh2::character::CharacterKillLiveBorrowV21 borrow{id,a.controller->identity(),id,r,view,r->life.get(),&a.kill_fields_v42(),oid,property,tracked,relations->outgoing(),&a.shared_handle()};
  std::weak_ptr<Record> weak=r;dh2::character::KillContributorServicesV23 services{r,[weak](auto event,auto payload,const auto*,auto& e){auto r=weak.lock();if(!r||!r->actor||!r->actor->machine){e="Released SAME contributor FSM";return false;}if(r->actor->machine->event(event,payload)<0){e=r->actor->machine->error();return false;}return true;}};
  std::shared_ptr<dh2::character::CharacterKillContributorEventV23> contributor;
  try{if(r->player_script_owner_v62)contributor=std::make_shared<dh2::character::CharacterKillContributorEventV23>(a.ai_events,r->player_script_owner_v62->session().owner(),std::move(services));
   else contributor=std::make_shared<dh2::character::CharacterKillContributorEventV23>(a.ai_events,a.session->owner(),std::move(services));}
  catch(const std::exception& failure){e=failure.what();return false;}
  if(!kill_->add(std::move(borrow),std::move(contributor),e))return false;enrolled_.insert(id);
  if(a.kill_fields_v42().master14d4&&!enroll(a.kill_fields_v42().master14d4,e))return false;
  return true;
 }
 bool progression_actor(std::uintptr_t id,dh2::character::ProgressionActorV1& out,std::string& e){
  std::shared_ptr<Record> r;if(!record(id,r,e)||!r->properties||!r->life)return false;
  auto& a=*r->actor;auto* view=r->player_script_owner_v62?&r->player_script_owner_v62->session().property_view():a.session?&a.session->property_view():&r->view;
  bool player{},local{};if(!r->is_player(player,e))return false;std::shared_ptr<SourceWorldBorrowV61> w;if(!current(w,e)||!w->player_manager->source_is_local_player_v61(id,local,e))return false;
  const auto* remote=a.source_bool_field(0x118);const auto* actors=r->design.characters();const auto* classes=r->design.class_rows();const auto* source_position=a.runtime.subobjects.position;
  const std::int32_t* oid{};const std::int16_t* property{};std::uintptr_t* tracked{};a.kill_metadata_borrow_v23(oid,property,tracked);
  if(!remote||!actors||!classes||!property||!r->save_fields||
   *r->save_fields->save_slot14e8()!=reinterpret_cast<std::uintptr_t>(r->save.get())){e="Required SAME canonical source progression fields/Save14e8 association";return false;}
  dh2::loader::CanonicalCurrentLevelBorrowV1 level;if(!borrow_current_native_level_v27(level,e)||!level||level.level()!=level_.lock()||!level.kill_level()){if(e.empty())e="Required actual GS/current Level for XP";return false;}
  //GiveXP3bf5f4 compares SG_GetGameDifficultyUnlocked against the actual
  //current Level+118 word. It does NOT call SG_GetGameDifficulty/global PDFL
  //or the front-end Application settings getter at this reload point.
  const auto difficulty=level.level()->constructor_fields_v3().mode118;
  out={id,r->properties.get(),view,r->save.get(),actors,classes->data(),std::uint32_t(classes->size()),*property,
   source_position[0],source_position[1],player,*remote!=0,local,std::int32_t(level.kill_level()->loot_gate150),difficulty};return true;
 }
 struct RegenDebugBorrow {SourceCampaignDeathRewardsV84& self;std::map<std::uintptr_t,std::unique_ptr<std::string>> strings;};
 static int regen_debug(void* raw,const dh2::character::skills::SkillAttackNativeRequestV6* q,std::uintptr_t* out){
  auto& context=*static_cast<RegenDebugBorrow*>(raw);auto& self=context.self;if(!q||!out||q->reserved)return -1;
  using namespace dh2::character::skills;std::shared_ptr<SourceWorldBorrowV61> w;if(!self.current(w,self.diagnostic_)||!w->debug||!w->debug_files)return -1;
  switch(q->service){case skill_attack_debug_load_v6:return dh2_character_debug_load(w->debug.get(),w->debug_files)==1?0:-1;
   case skill_attack_string_construct_v6:{if(!q->name)return -1;auto key=std::make_unique<std::string>(q->name);*out=reinterpret_cast<std::uintptr_t>(key.get());context.strings.emplace(*out,std::move(key));return 0;}
   case skill_attack_debug_get_v6:{auto at=context.strings.find(q->subject);if(at==context.strings.end())return -1;std::uint32_t v{};if(dh2_character_debug_get(&v,w->debug.get(),at->second->c_str(),w->debug_files)!=1)return -1;*out=v;return 0;}
   case skill_attack_string_destroy_v6:return context.strings.erase(q->subject)==1?0:-1;default:return -1;}
 }
 bool remaining(dh2::character::KillActor56& actor,const dh2::character::KillRequest56& q,dh2::character::KillResponse16& out,std::string& e){
  using namespace dh2::character;std::shared_ptr<SourceWorldBorrowV61> w;if(!current(w,e))return false;
  if(q.service==kill_drop_loot){auto items=w->prepared_items_v88;if(!items||!items->factory_prepared_v88()||!items->ready()){e="Required SAME source Item145 pool from actual stage29 before DropLoot";return false;}bool handled{};return items->route(actor,q,out,handled,e)&&handled;}
  if(q.service==kill_distribute_xp){bool handled{};return progression_&&progression_->route(actor,q,out,handled,e)&&handled;}
  if(q.service==kill_is_local_player){bool local{};if(!w->player_manager->source_is_local_player_v61(q.subject,local,e))return false;out.word=local;return true;}
  if(q.service==kill_online){auto online=w->application->get_online_loading_v55();if(!online){e="Required SAME COnline source owner";return false;}out.word=online->byte5();return true;}
  if(q.service==kill_is_remotely_updated){std::shared_ptr<Record> r;if(!record(q.subject,r,e))return false;auto* remote=r->actor->source_bool_field(0x118);if(!remote){e="Required actual Character remote118 producer";return false;}out.word=*remote;return true;}
  if(q.service==kill_constant)return constant(q.name,q.key,out.word,e);
  if(q.service==kill_trophy_id){dh2::android_ui::ProcessTrophyBorrowV100 trophy;if(!dh2::android_ui::borrow_source_process_trophies_v100(w->application,trophy,e)||!trophy.manager||!q.name)return false;
   out.word=trophy.manager->catalog().index(q.name);trophy_pins_.push_back(std::move(trophy));return true;}
  if(q.service==kill_unlock_trophy){for(auto& trophy:trophy_pins_)if(trophy.manager&&reinterpret_cast<std::uintptr_t>(trophy.manager)==q.subject){if(trophy.manager->unlock(q.argument)!=0){e=trophy.manager->error();return false;}return true;}e="Required SAME source captured process Trophy singleton";return false;}
  e="Required original canonical Kill service "+std::to_string(q.service);return false;
 }
 bool initialize(const std::shared_ptr<Record>& seed,std::string& e){
  if(kill_)return true;std::shared_ptr<SourceWorldBorrowV61> w;if(!current(w,e)||!w->settings||!seed->services.world_targets)return false;
  settings_=w->settings->borrow();if(!settings_||settings_.rows().empty()){e="Required actual source DesignSettings row0";return false;}targets_=seed->services.world_targets;
  dh2::character::CharacterProgressionServicesV23 xp;xp.owner=leaves_.owner;xp.players=w->player_manager->manager();xp.settings=&settings_.rows()[0];
  xp.actor=[this](auto id,auto& out,auto& e){return progression_actor(id,out,e);};
  xp.source.constant=[this](auto key,auto& value,auto& e){return constant("CharacterDesign",key,value,e);};
  xp.source.debug=[this](auto key,auto& value,auto& e){return debug(key,value,e);};
  xp.source.negative_award_assert=[](auto& e){auto assertion=dh2::world::SourceAssertionProcessV76::borrow();const auto mode=*assertion->source_level();
   if(mode==2){e="Original GiveXP amount>=0 deliberate null-store assertion";return false;}
   if(mode==1)return assertion->report("..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Characters\\Character_Stats.cpp",350,"amount >= 0",e);
   e.clear();return true;};
  xp.source.regen_full=[this](auto& actor,bool mp,auto& e){RegenDebugBorrow context{*this,{}};const dh2::character::skills::SkillAttackNativeServicesV6 debug{&context,regen_debug};
   if(dh2::character::skills::dh2_character_skill_regen_v6(actor.properties,mp?1:0,-1,&debug)!=0){e=diagnostic_.empty()?"Required source full RegenHP/MP":diagnostic_;return false;}return true;};
  xp.source.save=[this](auto& actor,auto& e){std::shared_ptr<Record> r;if(!record(actor.identity,r,e)||!r->load||!r->profile_bootstrap||!r->profile_bootstrap->campaign_writer()){if(e.empty())e="Required SAME live player SG_Save writer";return false;}
   dh2::data::PlayerSaveWriteOwnerV1 save(r->load,r->profile_bootstrap->campaign_writer()->write_services());return save.save(e);};
  xp.source.level_presentation=leaves_.level_presentation;xp.source.xp_text=leaves_.xp_text;
  xp.source.statistics_player_lookup=[this](auto& actor,auto& e){std::shared_ptr<SourceWorldBorrowV61> w;if(!current(w,e))return false;dh2::player::PlayerInfoFieldsV1* info{};
   return w->player_manager->manager()->get_by_character(actor.identity,false,info,e);}; //IncreaseStat3790e0 itself literal BX LR.
  progression_=std::make_unique<dh2::character::CharacterProgressionWorldV23>(std::move(xp));
  dh2::character::CharacterKillProductionServicesV23 s;s.providers=leaves_.owner;s.scope=[this]{return scope_;};
  s.refresh_contributor=[this](auto id,auto& e){return refresh(id,e);};
  s.died=[this](auto id,auto attacker,const auto* scope,auto& e){std::shared_ptr<Record> r;return record(id,r,e)&&ensure_death(r,e)&&r->death_v84->raise(attacker,scope,e);};
  s.remaining=[this](auto& actor,const auto& q,auto& out,auto& e){return remaining(actor,q,out,e);};
  dh2::character::KillLevelProviderV23 level{leaves_.owner,[this](auto& current,auto& e){std::shared_ptr<SourceWorldBorrowV61> w;if(!this->current(w,e)||!borrow_current_native_level_v27(current,e))return false;
   if(current&&current.level()!=level_.lock()){e="Kill current GS Level belongs to a different campaign";return false;}return true;}};
  auto design=w->design->borrow();globals_.constants=reinterpret_cast<std::uintptr_t>(design.design());
  kill_=std::make_unique<dh2::character::CharacterKillProductionV23>(*targets_,*w->player_manager->manager(),globals_,std::move(level),std::move(s));return true;
 }
public:
 bool level_up_suffix(dh2::character::ProgressionActorV1& player,std::int32_t reached_level,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> w;std::shared_ptr<Record> r;
  if(!current(w,e)||!record(player.identity,r,e)||!player.properties||
   player.properties->resolved!=r->properties->resolved.data()||
   (player.properties->resolved[19]>>8)!=reached_level){e="LevelUp suffix lost SAME reached Character/property19";return false;}
  auto scripts=w->application->source_script_manager_v52();auto settings=w->application->source_settings4c_v67();
  if(!scripts||!settings){e="Required actual LevelUp ScriptManager/Application4c";return false;}
  auto start=[&](const char* name,std::string& e){const auto id=scripts->id_from_name(name,true);return id==-1||scripts->start_script_v96(id,-1,false,e);};
  auto online=[&](bool& value,std::string& e){auto actual=w->application->get_online_loading_v55();if(!actual){e="Required actual LevelUp COnline";return false;}value=actual->byte5()!=0;return true;};
  auto difficulty=[&](std::int32_t& value,std::string& e){if(!r->save_fields){e="Required actual LevelUp Character14e8 slot";return false;}
   return dh2::player::character_game_difficulty_v29({r,player.identity,r->save_fields->save_slot14e8(),r->services.difficulty_global},value,e);};
  //Source3bedc8/GetIDFromName(...,true), then StartScript(id,-1,false).
  if(!start("PlayerLevelUp",e))return false;
  std::shared_ptr<void> fx_pin;dh2::fx::CharacterMeshFxOwnerV4* fx{};
  if(!borrow_source_campaign_fx_v77(w->owner,fx_pin,fx,e)||!fx)return false;
  //Actual495f04 overload passes process Point3D::ZERO, Character anchor,
  //NULL AnimFXSetData. Set135 is the source literal, not a name substitution.
  const float zero[3]{0,0,0};if(!fx->play_set(0x87,zero,nullptr,player.identity,nullptr,e))return false;
  //The source rereads property19 AFTER script/FX callbacks, which can mutate
  //the same Character. Its captured comparison/trophy scalar is this reload.
  const auto source_level=player.properties->resolved[19]>>8;
  if(source_level==2){
   bool net{};std::int32_t mode{};
   if(!online(net,e))return false;
   if(!net){
    if(!difficulty(mode,e))return false;
    if(!mode&&settings->tutorials()[0x2e - 0x29]){
     if(!start("cinematic_Tuto_levelUp",e)||!settings->source_store_tutorial_v88(0x2e,0,e))return false;
     if(!leaves_.settings_job_start){e="Required original updateJob_thread.Start2 after LevelUp settings2e store";return false;}
     if(!leaves_.settings_job_start(e))return false;
    }
   }
   if(!online(net,e))return false;
   if(!net){if(!difficulty(mode,e))return false;
    if(!mode&&settings->tutorials()[0]&&!start("cinematic_Tuto_menuCharacterSheet",e))return false;}
   if(!online(net,e))return false;
   if(!net){if(!difficulty(mode,e))return false;
    if(!mode&&settings->tutorials()[0x2c-0x29]){
     const auto id=scripts->id_from_name("cinematic_Tuto_menuSkillSheet",true);
     if(id!=-1){if(!online(net,e))return false;
      //Original3bf1c0/1c4 branches AWAY when offline; preserve that source
      //quirk rather than correcting it to a guessed tutorial policy.
      if(net&&!scripts->start_script_v96(id,-1,false,e))return false;}
    }
   }
  }else if(source_level==12){if(!source_native_character_spec_time_v96(w->owner,true,e))return false;}
  bool local{};if(!w->player_manager->source_is_local_player_v61(player.identity,local,e))return false;
  if(local&&source_level>=10&&source_level<=100&&source_level%10==0){
   const std::string name="epic_lvl"+std::to_string(source_level);dh2::android_ui::ProcessTrophyBorrowV100 trophy;
   if(!dh2::android_ui::borrow_source_process_trophies_v100(w->application,trophy,e)||!trophy.native)return false;
   if(trophy.native->unlock_named(name.c_str())){e=trophy.manager?trophy.manager->error():"Original LevelUp named Trophy unavailable";return false;}
  }
  e.clear();return true;
 }
 SourceCampaignDeathRewardsV84(std::shared_ptr<SourceWorldBorrowV61> w,std::shared_ptr<dh2::loader::CanonicalLevelContextV1> level,SourceDeathRewardsLeavesV84 leaves):world_(w),level_(level),leaves_(std::move(leaves)){}
 ~SourceCampaignDeathRewardsV84(){std::string ignored;release(ignored);}
 bool command(std::uintptr_t controller,std::uintptr_t id,std::uintptr_t attacker,std::uint32_t force,const dh2_script_callback_scope* scope,std::string& e){
  if(scope&&!dh2_script_callback_scope_valid(scope)){e="Expired actual HitFor/Skill callback scope";return false;}
  std::shared_ptr<Record> victim;if(!record(id,victim,e)||!victim->actor->controller||victim->actor->controller->identity()!=controller||!initialize(victim,e)||!enroll(id,e))return false;
  std::uintptr_t character{};if(!attacker_character(attacker,character,e)||(character&&!enroll(character,e)))return false;
  // Original contributor iteration is dynamic. Enroll the actual current
  // source relation receivers, leaving their key/threat/count cells untouched.
  auto* map=victim->actor->source_aggro_v84()->outgoing();if(map->count>map->capacity||(map->count&&!map->entries)){e="Invalid actual Kill outgoing map";return false;}
  for(std::uint32_t i=0;i<map->count;++i)if(!enroll(std::uintptr_t(map->entries[i].character),e))return false;
  std::shared_ptr<SourceWorldBorrowV61> w;if(!current(w,e))return false;
  dh2::android_ui::ProcessTrophyBorrowV100 trophy;if(!dh2::android_ui::borrow_source_process_trophies_v100(w->application,trophy,e))return false;
  const auto pin_count=trophy_pins_.size();trophy_pins_.push_back(trophy);globals_.trophy_manager=reinterpret_cast<std::uintptr_t>(trophy.manager);
  const auto* previous=scope_;scope_=scope;struct Finish{SourceCampaignDeathRewardsV84& self;const dh2_script_callback_scope* old;std::size_t pins;~Finish(){self.scope_=old;self.trophy_pins_.resize(pins);}}finish{*this,previous,pin_count};
  if(kill_->command(controller,id,attacker,force)!=1){e=kill_->error();return false;}e.clear();return true;
 }
 bool give_xp(std::uintptr_t id,std::int32_t raw,bool stats,bool& accepted,std::string& e){
  std::shared_ptr<Record> r;if(!record(id,r,e)||!initialize(r,e))return false;
  dh2::character::ProgressionResultV1 result;if(!progression_->give_source_v108(id,raw,stats,result,e))return false;
  accepted=result.accepted;e.clear();return true;
 }
 bool set_dead(std::uintptr_t id,std::string& e){std::shared_ptr<Record> r;return record(id,r,e)&&ensure_death(r,e)&&r->death_v84->set_dead(e);}
 bool notify_aggro(std::uint32_t bit,std::uintptr_t owner,std::uintptr_t target,const dh2_script_callback_scope* scope,std::string& e){
  if(!owner||!target){e="Source Aggro notification requires actual owner/target";return false;}
  if(bit!=dh2::data::aggro_notify_target&&bit!=dh2::data::aggro_notify_target_cleared){e="Required different original Aggro continuation";return false;}
  std::shared_ptr<SourceWorldBorrowV61> w;if(!current(w,e)||!w->settings)return false;
  if(!settings_)settings_=w->settings->borrow();
  return dispatch_aggro(this,target,owner,scope,bit==dh2::data::aggro_notify_target_cleared,e);
 }
 bool forget(std::uintptr_t id,std::string& e){
  if(kill_&&!kill_->forget_after_unpublication(id,e))return false;
  auto found=deaths_.find(id);if(found!=deaths_.end()){if(auto record=found->second.lock())record->death_v84.reset();deaths_.erase(found);}
  enrolled_.erase(id);e.clear();return true;
 }
 bool clear_aggro(std::uintptr_t owner,std::uintptr_t other,const dh2_script_callback_scope* scope,std::string& e){
  dh2::character::WorldClearAggroServicesV40 s;s.context=this;
  s.actor=[](void* raw,std::uintptr_t id,dh2::character::WorldClearAggroActorV40& out,std::string& e){auto& self=*static_cast<SourceCampaignDeathRewardsV84*>(raw);std::shared_ptr<Record> r;
   if(!self.record(id,r,e)||!r->actor->controller)return false;auto& a=*r->actor;auto* maps=a.source_aggro_v84();if(!maps){e="Required SAME canonical AI_ClearAggro maps";return false;}
   out={id,maps->outgoing(),maps->incoming(),&a.object->binding,a.controller->identity(),r};return true;};
  s.on_deaggro=deaggro;
  s.command_stop=[](void* raw,std::uintptr_t id,std::uintptr_t controller,const dh2_script_callback_scope* scope,std::string& e){auto& self=*static_cast<SourceCampaignDeathRewardsV84*>(raw);
   std::shared_ptr<SourceWorldBorrowV61> world;if(!self.current(world,e))return false;
   std::shared_ptr<Record> r;if(!self.record(id,r,e)||!r->actor->controller||r->actor->controller->identity()!=controller||
    (scope&&!dh2_script_callback_scope_valid(scope))){e="Changed SAME source AI_ClearAggro Cmd_Stop controller/scope";return false;}
   return source_campaign_character_command_stop_v101(world->owner,id,e);};
  dh2::character::CharacterWorldClearAggroV40 source(s);if(source.clear(owner,other,scope)!=0){e=source.error();return false;}e.clear();return true;
 }
 bool release(std::string& e){
  // Restore borrowed callable tables before ANY canonical Session/FSM teardown.
  kill_.reset();progression_.reset();targets_.reset();trophy_pins_.clear();audio_pin_.reset();
  for(auto& entry:deaths_)if(auto r=entry.second.lock())r->death_v84.reset();
  deaths_.clear();enrolled_.clear();e.clear();return true;
 }
};
bool bind_source_campaign_death_rewards_v84(const SourceCampaignCandidateBorrowV55& candidate,SourceDeathRewardsLeavesV84 leaves,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;if(!borrow_source_campaign_condition_world_v70(candidate,world,e)||!leaves.owner)return false;
 if(world->death_rewards_v84){e="Campaign death/reward transport already published";return false;}
 world->death_rewards_v84=std::make_shared<SourceCampaignDeathRewardsV84>(world,candidate.level,std::move(leaves));e.clear();return true;
}
int source_campaign_hit_kill_v84(const std::shared_ptr<void>& world,dh2::character::HitActor32* actor,const dh2::character::HitRequest32* q,const dh2_script_callback_scope* scope,std::uintptr_t* out,std::string& e){
 if(!q||q->service!=dh2::character::hit_controller_kill)return 0;
 SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<SourceWorldBorrowV61> actual;
 if(!actor||!out||q->subject!=actor->controller||!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=world||
  !borrow_source_campaign_condition_world_v70(candidate,actual,e)||!actual->death_rewards_v84){if(e.empty())e="Required SAME canonical Hit8 Kill transport";return -1;}
 SourceCampaignCharacterBorrowV62 record;if(!borrow_source_campaign_character_v62(world,actor->identity,record,e)||!record.character||
  !actor->properties||actor->properties->resolved!=record.character->properties->resolved.data()){if(e.empty())e="Hit8 properties differ from actual canonical Kill owner";return -1;}
 if(!actual->death_rewards_v84->command(actor->controller,actor->identity,q->target,q->force,scope,e))return -1;*out=0;return 1;
}
bool source_campaign_character_ai_set_dead_v86(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<SourceWorldBorrowV61> actual;
 if(!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=world||!borrow_source_campaign_condition_world_v70(candidate,actual,e)||!actual->death_rewards_v84){if(e.empty())e="Required SAME saved dead-state transport";return false;}
 return actual->death_rewards_v84->set_dead(id,e);
}
bool source_campaign_cmd_kill_v84(const std::shared_ptr<void>& world,std::uintptr_t controller,std::uintptr_t character,std::uintptr_t attacker,std::uint32_t force,const dh2_script_callback_scope* scope,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<SourceWorldBorrowV61> actual;
 if(!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=world||!borrow_source_campaign_condition_world_v70(candidate,actual,e)||!actual->death_rewards_v84){if(e.empty())e="Required SAME direct source Cmd_Kill transport";return false;}
 return actual->death_rewards_v84->command(controller,character,attacker,force,scope,e);
}
bool source_campaign_character_aggro_event_v84(const std::shared_ptr<void>& world,std::uint32_t bit,std::uintptr_t owner,std::uintptr_t target,const dh2_script_callback_scope* scope,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<SourceWorldBorrowV61> actual;
 if(!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=world||!borrow_source_campaign_condition_world_v70(candidate,actual,e)||!actual->death_rewards_v84){if(e.empty())e="Required SAME canonical Aggro notification transport";return false;}
 return actual->death_rewards_v84->notify_aggro(bit,owner,target,scope,e);
}
bool source_campaign_forget_kill_actor_v84(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<SourceWorldBorrowV61> actual;
 if(!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=world||!borrow_source_campaign_condition_world_v70(candidate,actual,e))return false;
 if(actual->death_rewards_v84)return actual->death_rewards_v84->forget(id,e);e.clear();return true;
}
bool source_campaign_character_clear_aggro_v84(const std::shared_ptr<void>& world,std::uintptr_t owner,std::uintptr_t other,const dh2_script_callback_scope* scope,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<SourceWorldBorrowV61> actual;
 if(!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=world||!borrow_source_campaign_condition_world_v70(candidate,actual,e)||!actual->death_rewards_v84){if(e.empty())e="Required SAME target-event AI_ClearAggro transport";return false;}
 return actual->death_rewards_v84->clear_aggro(owner,other,scope,e);
}
bool source_campaign_give_xp_v108(const std::shared_ptr<void>& world,std::uintptr_t id,std::int32_t raw,bool stats,bool& accepted,std::string& e){
 SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
 if(!borrow_source_campaign_candidate_runtime_v61(c,e)||c.actual_world!=world||!borrow_source_campaign_condition_world_v70(c,w,e)||!w->death_rewards_v84){if(e.empty())e="Required SAME campaign GiveXP source transport";return false;}
 return w->death_rewards_v84->give_xp(id,raw,stats,accepted,e);
}
bool release_source_campaign_death_rewards_v84(const SourceCampaignCandidateBorrowV55& candidate,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;if(!borrow_source_campaign_condition_world_v70(candidate,world,e))return false;
 if(world->death_rewards_v84&&!world->death_rewards_v84->release(e))return false;world->death_rewards_v84.reset();e.clear();return true;
}
bool source_campaign_level_up_presentation_v88(const std::shared_ptr<void>& world,dh2::character::ProgressionActorV1& player,std::int32_t level,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<SourceWorldBorrowV61> actual;
 if(!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=world||!borrow_source_campaign_condition_world_v70(candidate,actual,e)||!actual->death_rewards_v84){if(e.empty())e="Required SAME canonical LevelUp presentation transport";return false;}
 return actual->death_rewards_v84->level_up_suffix(player,level,e);
}
}
