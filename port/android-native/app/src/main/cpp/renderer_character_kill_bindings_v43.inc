// Include AFTER renderer_character_kill_live_v21.inc and player-manager hit
// helpers. Requires retained_character_kill_borrow_v42.hpp,
// character_kill_production_v23.hpp, world_loot_gameplay_v23.hpp and
// character_progression_world_v23.hpp. Not an automatic Hit8 publication.
struct RendererKillProvidersV43 {
 std::shared_ptr<void> storage;
 dh2::character::KillLevelProviderV23 level;
 dh2::character::WorldLootGameplayV23* loot{};
 dh2::character::CharacterProgressionWorldV23* progression{};
 std::function<const dh2_script_callback_scope*()> scope;
 // The SAME player's fresh-only metadata backing/controller/PropertyView.
 std::function<bool(dh2::character::CharacterKillLiveBorrowV21&,std::string&)> player;
 // Whole source player death, including selected AIS/FSM, if player is killed.
 std::function<bool(std::uintptr_t,const dh2_script_callback_scope*,std::string&)> player_died;
};
class RendererCharacterKillBindingsV43 {
 PlayerSkillsRuntime& t_;RendererKillProvidersV43 services_;
 dh2::character::KillWorld16 globals_{};
 std::unique_ptr<dh2::character::CharacterKillProductionV23> owner_;
 bool bound_{};std::string error_;
 bool refresh(std::uintptr_t id,std::string& e){
  if(!t_.world||!t_.player||!t_.world->player_object){e="Required live Kill World/player";return false;}
  if(id==t_.world->player_object->identity){
   if(!t_.events.owner){e="Required SAME player CharAI event owner";return false;}
   t_.events.active=t_.player->session().owner().lifecycle().active;
   t_.events.owner->forced=prince_controller_forced;t_.events.owner->locked=prince_state.controller_locked;
   t_.events.global_blocked=controller_global_blocked;return true;
  }
  auto it=t_.world_actors.find(id);auto* m=it==t_.world_actors.end()?nullptr:it->second.npc;
  if(!m||!m->session||!m->controller||!m->machine){e="Required exact registered NPC contributor";return false;}
  const auto* command=m->controller->command_state(controller_global_blocked);
  if(!command){e="Required NPC contributor source controller globals";return false;}
  m->ai_owner.forced=command->forced;m->ai_owner.locked=command->locked;
  m->ai_events.global_blocked=command->global_blocked;
  m->ai_events.active=m->session->owner().lifecycle().active;return true;
 }
 bool remaining(dh2::character::KillActor56& a,const dh2::character::KillRequest56& q,
  dh2::character::KillResponse16& out,std::string& e){
  using namespace dh2::character;
  if(q.service==kill_drop_loot){bool handled{};if(!services_.loot->route(a,q,out,handled,e))return false;if(!handled){e="Required exact source DropLoot route";return false;}return true;}
  if(q.service==kill_distribute_xp){bool handled{};if(!services_.progression->route(a,q,out,handled,e))return false;if(!handled){e="Required exact source Kill XP route";return false;}return true;}
  if(q.service==kill_is_local_player){bool local{};if(!player_network_is_local_v4(t_,q.subject,local,e))return false;out.word=local;return true;}
  if(q.service==kill_online){std::uintptr_t id{};std::int32_t online{};if(!EquipmentPlatform::query(equipment_platform.get(),dh2::player::EquipmentWorldQueryV1::online,0,id,online,e))return false;out.word=online;return true;}
  if(q.service==kill_is_remotely_updated){
   if(q.subject==t_.fsm.character){bool remote{};if(!PlayerSkillsRuntime::heading_remote(&t_,remote,e))return false;out.word=remote;return true;}
   auto it=t_.world_actors.find(q.subject);auto* m=it==t_.world_actors.end()?nullptr:it->second.npc;
   const auto* byte=m?m->source_bool_field(0x118):nullptr;
   if(!byte){e="Required exact ObjectBase source remote118 byte";return false;}out.word=*byte;return true;
  }
  if(q.service==kill_constant){const auto* design=t_.world->tables.design();if(!q.name||!q.key||!design||!design->lookup||design->lookup(design->context,0,q.name,q.key,&out.word)){e="Required real Kill quest constant selector";return false;}return true;}
  if(q.service==kill_trophy_id){if(!q.name||!t_.trophies){e="Required SAME Kill TrophyManager/catalog";return false;}out.word=t_.trophies->catalog().index(q.name);return true;}
  if(q.service==kill_unlock_trophy){if(q.subject!=reinterpret_cast<std::uintptr_t>(t_.trophies.get())||!t_.trophies){e="Required captured SAME Kill TrophyManager";return false;}if(t_.trophies->unlock(q.argument)!=0){e=t_.trophies->error();return false;}return true;}
  e="Required unbound original Kill selector "+std::to_string(q.service);return false;
 }
public:
 RendererCharacterKillBindingsV43(PlayerSkillsRuntime& t,RendererKillProvidersV43 s):t_(t),services_(std::move(s)){}
 ~RendererCharacterKillBindingsV43(){release();}
 void release()noexcept{
  // Production destruction restores contributor source callable tables while
  // AI/Script/World/PM are still alive. Root calls before clearing actor maps.
  bound_=false;owner_.reset();
 }
 bool bind_impl(std::string& e){
  using namespace dh2::character;
  if(owner_||bound_||!services_.storage||!services_.level.current||services_.level.lifetime.expired()||
     !services_.loot||!services_.loot->ready()||!services_.progression||!services_.scope||!services_.player||!services_.player_died||
     !t_.world||!t_.targets||!t_.player_manager||!t_.player||!t_.trophies){
   e="Required actual Kill Level/ready145/progression/player death and provider storage before publication";return false;
  }
  dh2::loader::CanonicalCurrentLevelBorrowV1 current;
  if(!services_.level.current(current,e))return false;
  if(!current||!current.level()->constructor_owner_v3()||
     current.level()->constructor_owner_v3()->phase()!=dh2::loader::LevelConstructorPhaseV3::complete||
     !current.kill_level()||current.kill_level()->identity!=current.identity()){
   e="Required actual published completed GSLevel C1/gate150 before Kill bindings";return false;
  }
  globals_={reinterpret_cast<std::uintptr_t>(t_.trophies.get()),reinterpret_cast<std::uintptr_t>(t_.world->tables.design())};
  CharacterKillProductionServicesV23 s;s.providers=services_.storage;
  s.scope=services_.scope;
  s.refresh_contributor=[this](auto id,auto& error){return refresh(id,error);};
  s.died=[this](auto id,auto attacker,const auto* scope,auto& error){
   if(id==t_.fsm.character)return services_.player_died(attacker,scope,error);
   auto it=t_.world_actors.find(id);auto* m=it==t_.world_actors.end()?nullptr:it->second.npc;
   if(!m){error="Required exact victim NPC RaiseEvent2 receiver";return false;}
   return renderer_kill_npc_raise_v21(*m,2,attacker,scope,error);
  };
  s.remaining=[this](auto& actor,const auto& q,auto& out,auto& error){return remaining(actor,q,out,error);};
  owner_=std::make_unique<CharacterKillProductionV23>(*t_.targets,t_.player_manager->manager(),globals_,services_.level,std::move(s));
  for(const auto& entry:t_.world_actors){
   CharacterKillLiveBorrowV21 b;std::shared_ptr<CharacterKillContributorEventV23> contributor;
   if(entry.first==t_.fsm.character){
    if(!services_.player(b,e)||b.identity!=entry.first||!refresh(entry.first,e)){release();return false;}
    KillContributorServicesV23 cs{b.receiver,[](unsigned event,std::uintptr_t payload,const dh2_script_callback_scope*,std::string& error){if(prince_event(event,payload)<0){error="Required SAME player Kill contributor FSM event";return false;}return true;}};
    contributor=std::make_shared<CharacterKillContributorEventV23>(t_.events,t_.player->session().owner(),std::move(cs));
   }else{
    auto* m=entry.second.npc;if(!m)continue;
    AggroStorage* maps=nullptr;std::shared_ptr<void> lease;
    for(auto& group:object_groups)for(auto& actor:group.instances)if(actor.identity==entry.first&&actor.script.get()==m){maps=&actor.aggro;lease=actor.script;}
    if(!maps||!retained_character_kill_borrow_v42(*m,lease,maps->outgoing_table,b,e)||!refresh(entry.first,e)){release();return false;}
    KillContributorServicesV23 cs{lease,[m](unsigned event,std::uintptr_t payload,const dh2_script_callback_scope*,std::string& error){const int status=m->machine->event(event,payload);if(status<0){error=m->machine->error();return false;}return true;}};
    contributor=std::make_shared<CharacterKillContributorEventV23>(m->ai_events,m->session->owner(),std::move(cs));
   }
   if(!owner_->add(std::move(b),std::move(contributor),e)){release();return false;}
  }
  bound_=true;return true;
 }
 bool bind(std::string& error){
  try{return bind_impl(error);}catch(const std::exception& e){
   release();error=std::string("Required Kill source callable adoption: ")+e.what();return false;
  }
 }
 int hit(dh2::character::HitActor32* actor,const dh2::character::HitRequest32* q,std::uintptr_t* out){
  if(!q||q->service!=dh2::character::hit_controller_kill)return 0;
  if(!bound_||!owner_||!actor||!out){t_.error="Required fully bound original Kill V43";return -1;}
  auto it=t_.world_actors.find(actor->identity);
  if(it==t_.world_actors.end()||!it->second.object||!it->second.object->properties||!actor->properties||actor->properties->resolved!=it->second.object->properties->resolved.data()){
   t_.error="Required SAME Hit8 victim properties";return -1;
  }
  if(owner_->command(q->subject,actor->identity,q->target,q->force)!=1){error_=owner_->error();t_.error=error_;return -1;}
  *out=0;return 1;
 }
 const std::string& error()const noexcept{return error_;}
};
