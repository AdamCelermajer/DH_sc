// Include after MonsterScriptHandle, PlayerSkillsRuntime, object_groups and
// player_aggro_callback_v2. Retain one instance per SAME NPC lifetime.
class RendererNpcDeathV2 {
 MonsterScriptHandle& m_;PlayerSkillsRuntime& world_;
 std::unique_ptr<dh2::character::skills::CharacterSkillSessionServices> script_;
 std::unique_ptr<dh2::character::skills::CharacterSkillOwner> skills_;
 dh2::character::AIDeathOwner24 actor_{};
 dh2::character::AIDeathState64 death_{};
 std::unique_ptr<dh2::character::NpcDeadStateV1> state_;
 std::unique_ptr<dh2::character::NpcDeathOwnerV2> owner_;
 std::unique_ptr<dh2::character::CharacterMenuRecalcOwnerV1> recalc_;
 dh2::character::BuffServices16 buff_services_{this,buff_service};
 dh2::character::BuffBindings32 buff_bindings_{};
 dh2::character::BuffOwner* buffs_{};
 std::string error_;
 const std::uintptr_t* source_ais_keys_{};
 static bool actor(void* p,std::uintptr_t id,dh2::character::AggroClearActorBorrowV2& out,std::string& error){
  auto& t=*static_cast<RendererNpcDeathV2*>(p);auto found=t.world_.world_actors.find(id);
  if(found==t.world_.world_actors.end()){error="Death reciprocal actor is absent from SAME World";return false;}
  AggroStorage* maps=nullptr;dh2::character::TargetBindings48* target=nullptr;
  if(t.world_.world->player_object&&id==t.world_.world->player_object->identity){maps=&prince_combat.aggro;target=&t.world_.world->player_object->binding;}
  else for(auto& group:object_groups)for(auto& item:group.instances)if(item.identity==id&&item.script){maps=&item.aggro;target=&item.script->object->binding;}
  if(!maps||!target){error="Required actual death reciprocal threat/target owner";return false;}
  out={id,&maps->outgoing_table,&maps->incoming_table,target};return true;
 }
 static bool notify(void* p,std::uintptr_t receiver,std::uintptr_t other,const dh2_script_callback_scope*,std::string& error){
  auto& t=*static_cast<RendererNpcDeathV2*>(p);
  // Existing source World AIS dispatcher performs real selected +3c dispatch,
  // including Player OnDeAggro. Never mutate its counters locally.
  if(player_aggro_callback_v2(&t.world_,dh2::data::aggro_notify_target_cleared,other,receiver)){error=t.world_.error;return false;}return true;
 }
 static bool ais(void* p,std::uintptr_t receiver,std::uintptr_t method,std::uintptr_t attacker,const dh2_script_callback_scope* scope,std::string& error){
  auto& t=*static_cast<RendererNpcDeathV2*>(p);
  return dh2::character::character_ais_death_vm_v2(t.m_.session->owner(),receiver,method,attacker,scope,error)==0;
 }
 static int constant(void* p,const char* group,const char* key,int* out){
  const auto* d=static_cast<RendererNpcDeathV2*>(p)->m_.context->tables.design();
  return d&&d->lookup?d->lookup(d->context,0,group,key,out):-1;
 }
 static int stance(void* p,std::uintptr_t id,int* out){
  auto& t=*static_cast<RendererNpcDeathV2*>(p);if(id!=t.m_.object->identity||!out)return -1;
  // GetAnimStance3a53e0 queries COUNT_IPHONE before its nonplayer return.
  int count{};if(constant(p,"AnimStances","COUNT_IPHONE",&count))return -1;
  auto queries=t.world_.targets->targets().query_services();std::uintptr_t player{};
  const dh2::target_providers::Request24 q{dh2::target_providers::virtual_player,0,id,0};
  if(!queries.invoke||queries.invoke(queries.context,&q,&player)||player)return -1;*out=0;return 0;
 }
 static int dead_constant(void* p,const char* group,const char* key,int* out){return constant(p,group,key,out);}
 bool drop(std::uintptr_t& handle){
  if(!handle)return true; // Whole source Character NULL gate.
  if(!world_.combat_fx){error_="Required same World VisualFXManager for positive NPC FX";return false;}
  return world_.combat_fx->manager().drop(handle,error_);
 }
 static int buff_service(void* p,dh2::data::PropertyView* view,const dh2::character::BuffRequest32* q,std::uintptr_t*){
  using namespace dh2::character;auto& t=*static_cast<RendererNpcDeathV2*>(p);
  if(!q||view!=&t.m_.session->property_view()||q->character!=t.m_.object->identity)return 0;
  if(q->service==buff_recalculate)return t.recalc_->recalculate(*view,1,t.error_)?1:0;
  if(q->service==buff_fx_release){auto handle=q->subject;return t.drop(handle)?1:0;}
  t.error_="Required original NPC buff FX producer "+std::to_string(q->service);return 0;
 }
public:
 RendererNpcDeathV2(MonsterScriptHandle& m,PlayerSkillsRuntime& world,std::uintptr_t source_group34):m_(m),world_(world){
  using namespace dh2::character;
  if(!m.session||!m.machine||!m.object||m.context!=world.world)throw std::runtime_error("Required SAME retained NPC death graph");
  source_ais_keys_=m.ai_events.ais_virtuals;
  script_=std::make_unique<skills::CharacterSkillSessionServices>(*m.session,m.context->effect_services,m.machine->native_fsm());
  skills_=std::make_unique<skills::CharacterSkillOwner>(m.object->identity,&m.session->property_view(),m.context->skills_owner->borrow(),m.context->script_files.faeries(),script_->services());
  if(skills_->configure()!=1)throw std::runtime_error("Source NPC Skill/Spell initialization: "+skills_->error()+"; "+script_->error());
  recalc_=std::make_unique<CharacterMenuRecalcOwnerV1>(m.context->design->borrow(),m.object->properties,m.session->property_view());
  buff_bindings_={&m.session->property_view(),&m.session->timers(),&m.session->native_timer_services(),&buff_services_};
  state_=std::make_unique<NpcDeadStateV1>(NpcDeadStateBorrowV1{m.machine.get(),&m.session->property_view(),&actor_animation_tables,this,dead_constant,stance});
  actor_={m.object->identity,reinterpret_cast<std::uintptr_t>(&m.machine->native_fsm()),&m.session->timers()};
  death_={m.ai_events.ai,&actor_,&m.object->target,source_group34,0,nullptr,UINT32_MAX,UINT32_MAX,0};
  AggroClearActorBorrowV2 relations{};if(!actor(this,m.object->identity,relations,error_))throw std::runtime_error(error_);
  owner_=std::make_unique<NpcDeathOwnerV2>(NpcDeathBorrowV2{&death_,&m.ai_events,&m.object->binding,relations,state_.get(),skills_.get()},NpcDeathServicesV2{{this,actor,notify},this,nullptr,ais});
  buffs_=dh2_character_buffs_create(&buff_bindings_);
  if(!buffs_)throw std::runtime_error("Required sole NPC BuffOwner over retained CharProperties/CharTimers");
 }
 ~RendererNpcDeathV2(){
  // Root clears these owners before Session/World teardown. No timer delivery
  // is allowed during this destructor; source buff D1 does not stop timers.
  if(buffs_&&dh2_character_buffs_destroy(buffs_)!=1)
   __android_log_print(ANDROID_LOG_ERROR,"DH2Native","NPC buff destructor required service: %s",error_.c_str());
  if(m_.ai_events.ais_virtuals==ais_keys_.data())m_.ai_events.ais_virtuals=source_ais_keys_;
 }
 // 1 handled,0 another source service,-1 required failure. Insert before
 // existing NPC state body dispatch. This executes source State12 FX/buffs
 // without changing state selection or synthesizing animation events.
 int body(dh2::character::State* state,const dh2::character::Request& q){
  using namespace dh2::character;
  if(state!=&m_.machine->state())return -1;
  bool okay{};
  switch(q.service){
   case remove_highlight:okay=drop(m_.source_highlight14a0());if(okay)m_.source_highlight14a0()=0;break;
   case disable_state_fx:okay=drop(m_.source_state_fx148c());break;
   case disable_self_fx:okay=drop(m_.source_self_fx1484());break;
   case remove_buffs:{BuffResult24 result{};okay=dh2_character_buffs_remove_all(&result,buffs_)==1;break;}
   case start_timer:
    if(q.argument[2]!=0x2e)return 0;
    {int delay{};if(constant(this,"CharacterDesign","Despawn_Delay",&delay))break;
     okay=dh2_character_timer_start(&m_.session->timers(),std::uint32_t(delay),q.argument[1],0x2e,q.identity,&m_.session->native_timer_services())>=0;break;}
   default:return 0;
  }
  if(!okay){if(error_.empty())error_="Required NPC DeathState12 service "+std::to_string(q.service);world_.error=error_;return -1;}
  return 1;
 }
 dh2::character::BuffOwner* buffs()noexcept{return buffs_;}
 int on_died(std::uintptr_t attacker,const dh2_script_callback_scope* scope){
  auto& lifecycle=m_.session->owner().lifecycle();
  death_.group=m_.source_ai_group34;
  std::uint32_t method{};
  if(m_.ai_events.active&&!m_.session->owner().source_death_callback(method)){
   error_="Required actual selected NPC AIS OnDied source method";world_.error=error_;return -1;
  }
  // Keep the source key table alive across synchronous AIS reselection.
  // Other slots remain the same retained dispatch table; only the newly
  // recovered +24 slot is published from its actual selected receiver.
  if(m_.ai_events.active){
   if(!m_.ai_events.ais_virtuals){error_="Required actual NPC AIS source key table";world_.error=error_;return -1;}
   if(m_.ai_events.ais_virtuals!=ais_keys_.data())source_ais_keys_=m_.ai_events.ais_virtuals;
   std::copy_n(m_.ai_events.ais_virtuals,ais_keys_.size(),ais_keys_.begin());
   ais_keys_[0x24/4]=method;m_.ai_events.ais_virtuals=ais_keys_.data();
  }
  death_.timer0=std::uint32_t(lifecycle.timer33);death_.timer1=std::uint32_t(lifecycle.timer34);
  dh2::character::AIDeathResult24 result{};const int status=owner_->on_died(result,attacker,scope);
  // Publish exact reset/failure prefix back to the sole Script/CharAI fields.
  std::memcpy(&lifecycle.timer33,&death_.timer0,4);std::memcpy(&lifecycle.timer34,&death_.timer1,4);
  if(status!=1){error_=owner_->error();world_.error=error_;return -1;}return 0;
 }
 dh2::character::skills::CharacterSkillOwner& skill_owner()noexcept{return *skills_;}
 const std::string& error()const noexcept{return error_;}
private:
 std::array<std::uintptr_t,51> ais_keys_{};
};
