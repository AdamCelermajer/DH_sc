// Include after the actual player aggro dispatcher/NPC death composition.
// Forward-declare helpers before character_service/event_service. This owns no
// Character, SkillOwner, VM, inventory, HP, physics body or substitute FSM.
class RendererPlayerDeathV56 {
 PlayerSkillsRuntime& t_;
 dh2::character::AIDeathOwner24 actor_{};
 dh2::character::AIDeathState64 death_{};
 std::unique_ptr<dh2::character::CharacterDeadSelectionOwnerV56> selector_;
 std::unique_ptr<dh2::character::CharacterDeathOwnerV56> owner_;
 std::array<std::uintptr_t,51> ais_keys_{};
 const std::uintptr_t* previous_ais_keys_{};
 const dh2_script_callback_scope* delivery_scope_{};bool delivery_active_{};
 std::string error_;
 static bool actor(void* p,std::uintptr_t id,dh2::character::AggroClearActorBorrowV2& out,std::string& e){
  auto& t=*static_cast<RendererPlayerDeathV56*>(p);
  if(!t.t_.world||!t.t_.world->player_object||t.t_.world_actors.find(id)==t.t_.world_actors.end()){e="Required registered SAME death relation receiver";return false;}
  if(id==t.t_.world->player_object->identity){out={id,&prince_combat.aggro.outgoing_table,&prince_combat.aggro.incoming_table,&t.t_.world->player_object->binding};return true;}
  for(auto& group:object_groups)for(auto& a:group.instances)if(a.identity==id&&a.script){out={id,&a.aggro.outgoing_table,&a.aggro.incoming_table,&a.script->object->binding};return true;}
  e="Required actual canonical death relation Aggro/Target receiver";return false;
 }
 static bool notify(void* p,std::uintptr_t receiver,std::uintptr_t other,const dh2_script_callback_scope*,std::string& e){
  auto& t=*static_cast<RendererPlayerDeathV56*>(p);
  if(player_aggro_callback_v2(&t.t_,dh2::data::aggro_notify_target_cleared,other,receiver)){e=t.t_.error;return false;}return true;
 }
 bool drop(std::uintptr_t& handle,std::string& e){
  if(!handle)return true; // Source DropAnimatedFX null-reference gate.
  if(!t_.combat_fx){e="Required SAME positive Character death VisualFXManager";return false;}
  return t_.combat_fx->manager().drop(handle,e);
 }
public:
 explicit RendererPlayerDeathV56(PlayerSkillsRuntime& t):t_(t){
  using namespace dh2::character;
  if(!t_.world||!t_.world->player_object||!t_.player||!t_.player->ready()||!t_.events.owner||
     t_.events.owner->owner!=t_.fsm.character||t_.fsm.character!=t_.world->player_object->identity)
   throw std::runtime_error("Required retained SAME ready player death graph");
  auto& fields=t_.missing_death_fields_v56;
  CharacterDeadSelectionServicesV56 ss;
  ss.stance_mask=[this](auto& value,auto& e){
   if(t_.player->session().constant("AnimStancedAnim","SL__LIST_IPHONE",value)){e="Required actual SM_SetDeadState stance mask";return false;}return true;
  };
  ss.stance=[this](auto& value,auto& e){
   if(!player_equipment){e="Required SAME retained player equipment stance";return false;}
   std::int32_t count{};dh2::character::StanceFacts16 facts{};
   if(t_.player->session().constant("AnimStances","COUNT_IPHONE",count)||
      !player_equipment->stance_facts(true,count,facts,e)||dh2_character_anim_stance(&value,&facts)!=1){
    if(e.empty())e="Required original Character.GetAnimStance";return false;
   }return true;
  };
  selector_=std::make_unique<CharacterDeadSelectionOwnerV56>(CharacterDeadSelectionBorrowV56{
   &prince_state_owner,&t_.player->session().property_view(),&actor_animation_tables,
   &t_.combat_fields.push_death,&fields.secondary38,prince_owner_services},std::move(ss));
  actor_={t_.fsm.character,t_.events.owner->state_machine,&t_.player->session().timers()};
  death_={t_.events.ai,&actor_,&t_.world->player_object->target,fields.group34,0,nullptr,UINT32_MAX,UINT32_MAX,0};
  AggroClearActorBorrowV2 relation{};if(!actor(this,t_.fsm.character,relation,error_))throw std::runtime_error(error_);
  CharacterDeathServicesV56 s;s.aggro={this,actor,notify};
  s.group=[](auto,auto,auto,auto& e){e="Required actual positive player CharAI GroupInfo.OnDied owner";return false;};
  s.ais=[this](auto receiver,auto method,auto attacker,const auto* scope,auto& e){
   return character_ais_death_vm_v56(t_.player->session().owner(),receiver,method,attacker,scope,e)==0;
  };
  s.set_dead=[this](bool mode,auto payload,bool force,auto& e){const int r=selector_->set(mode,payload,force);if(r<0){e=selector_->error();return false;}return true;};
  s.cleanup_skills=[this](auto& e){if(t_.player->cleanup_skills()!=1){e=t_.player->error();return false;}return true;};
  s.cleanup_spells=[this](auto& e){if(t_.player->cleanup_spells()!=1){e=t_.player->error();return false;}return true;};
  owner_=std::make_unique<CharacterDeathOwnerV56>(CharacterDeathBorrowV56{
   &death_,&t_.events,&t_.world->player_object->binding,relation,&t_.player->session().owner().lifecycle()},std::move(s));
 }
 ~RendererPlayerDeathV56(){if(t_.events.ais_virtuals==ais_keys_.data())t_.events.ais_virtuals=previous_ais_keys_;}
 int body(dh2::character::State* state,const dh2::character::Request& q,std::string& e){
  using namespace dh2::character;if(state!=&prince_state){e="Required SAME player State12 body";return -1;}
  auto& f=t_.missing_death_fields_v56;bool okay{};
  switch(q.service){
  case remove_highlight:okay=drop(f.highlight14a0,e);break;
  case disable_state_fx:okay=drop(f.state_fx148c,e);break;
  case disable_self_fx:okay=drop(f.self_fx1484,e);break;
  case remove_buffs:{BuffResult24 result{};auto* buffs=t_.player->native_buffs();okay=buffs&&dh2_character_buffs_remove_all(&result,buffs)==1;if(!okay)e="Required SAME player original PROPS_RemoveAllBuffs";break;}
  default:return 0;
  }
  if(!okay){if(e.empty())e="Required original player death body "+std::to_string(q.service);return -1;}return 1;
 }
 bool on_died(std::uintptr_t attacker,const dh2_script_callback_scope* scope,std::string& e){
  auto& lifecycle=t_.player->session().owner().lifecycle();
  t_.events.active=lifecycle.active;death_.group=t_.missing_death_fields_v56.group34;
  if(t_.events.active){
   std::uint32_t method{};
   if(!t_.player->session().owner().source_death_callback(method)||!t_.events.ais_virtuals){e="Required actual selected player AIS OnDied method/table";return false;}
   if(t_.events.ais_virtuals!=ais_keys_.data())previous_ais_keys_=t_.events.ais_virtuals;
   std::copy_n(t_.events.ais_virtuals,ais_keys_.size(),ais_keys_.begin());ais_keys_[9]=method;t_.events.ais_virtuals=ais_keys_.data();
  }
  dh2::character::AIDeathResult24 out{};
  if(owner_->on_died(out,attacker,scope)!=1){e=owner_->error();return false;}return true;
 }
 const dh2_script_callback_scope* current_scope()const noexcept{
  return delivery_active_?delivery_scope_:t_.player->session().current_skill_callback_scope();
 }
 bool raise(std::uintptr_t attacker,const dh2_script_callback_scope* scope,std::string& e){
  if(scope&&!dh2_script_callback_scope_valid(scope)){e="Required actual private death callback scope";return false;}
  const auto* previous=delivery_scope_;const bool active=delivery_active_;delivery_scope_=scope;delivery_active_=true;
  struct Restore{RendererPlayerDeathV56& t;const dh2_script_callback_scope* old;bool active;~Restore(){t.delivery_scope_=old;t.delivery_active_=active;}} restore{*this,previous,active};
  t_.events.active=t_.player->session().owner().lifecycle().active;
  t_.event_owner.forced=prince_controller_forced;t_.event_owner.locked=prince_state.controller_locked;t_.events.global_blocked=controller_global_blocked;
  dh2::character::AIEventResult16 result{};const dh2::character::AIEventPayload24 payload{attacker,0,0,0};
  const dh2::character::AIEventServices24 services{&t_,PlayerSkillsRuntime::event_service,63,0};
  if(dh2_character_ai_event(&result,&t_.events,2,&payload,&services)){e=t_.error.empty()?"Required source player RaiseAIEvent2":t_.error;return false;}return true;
 }
};
static RendererPlayerDeathV56& ensure_player_death_v56(PlayerSkillsRuntime& t){
 if(!t.death_v56)t.death_v56=std::make_unique<RendererPlayerDeathV56>(t);return *t.death_v56;
}
static int player_death_body_v56(PlayerSkillsRuntime& t,dh2::character::State* state,const dh2::character::Request& q,std::string& e){
 return ensure_player_death_v56(t).body(state,q,e);
}
static int player_death_ai_v56(PlayerSkillsRuntime& t,const dh2::character::AIEventRequest40& q){
 if(q.service!=dh2::character::ai_event_virtual||q.operation!=0x24||q.callee!=0x3d1000||q.subject!=t.events.ai)return 1;
 auto& owner=ensure_player_death_v56(t);return owner.on_died(q.payload,owner.current_scope(),t.error)?0:-1;
}
// Public Kill provider endpoint: whole original RaiseAIEvent2, THEN its actual
// FSM relay. Do not call OnDied and prince_event separately from reward code.
[[maybe_unused]] static bool renderer_player_raise_death_v56(PlayerSkillsRuntime& t,std::uintptr_t attacker,const dh2_script_callback_scope* scope,std::string& e){
 if(!t.player){e="Required SAME ready player death owner";return false;}
 return ensure_player_death_v56(t).raise(attacker,scope,e);
}
