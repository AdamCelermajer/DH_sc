// Include after RendererNpcCommandsV1 and PlayerSkillsRuntime. New ownership
// is orchestration only; State/Facts/Target/Timers/Animator are the same NPC.
struct RendererNpcStateAnimationInputsV1 {
 dh2::physical::NativeBody* body{};
 const dh2::character::TimerServices32* timers{};
 void* context{};
 // Whole selected AIS PreAttack override if it differs from proven default.
 int(*pre_attack)(void*,MonsterScriptHandle&,std::uintptr_t,std::uint32_t,std::string&){};
};
class RendererNpcStateAnimationV1 {
 MonsterScriptHandle& m_;PlayerSkillsRuntime& world_;RendererNpcCommandsV1& commands_;
 RendererNpcStateAnimationInputsV1 input_;std::string error_;
 int sequence(const char* name,int table){
  const auto& a=actor_animation_tables;const auto found=std::find(a.state_names.begin(),a.state_names.end(),name);
  if(found==a.state_names.end()||table<0||std::size_t(table)>=a.characters.size())throw std::runtime_error("Required actual NPC CharAnimTable "+std::string(name));
  const auto index=std::size_t(found-a.state_names.begin());
  if(index>=a.characters[table].fields.size()||a.characters[table].fields[index].size()!=1)throw std::runtime_error("Required scalar NPC animation field "+std::string(name));
  return a.characters[table].fields[index][0];
 }
 void coherent(){
  if(!m_.machine||!m_.object||!m_.session||!m_.animation||world_.world!=m_.context||!world_.targets)
   throw std::runtime_error("Required same NPC state/animation World graph");
  if(m_.machine->owner().machine().physical!=reinterpret_cast<std::uintptr_t>(input_.body))throw std::runtime_error("NPC state physical identity mismatch");
 }
 void fail(const char* text){throw std::runtime_error(error_.empty()?text:error_);}
 bool look(int mode,std::uintptr_t target){
  if(mode==1)return world_.targets->targets().look_at(m_.object->identity,target)==0;
  if(mode!=0&&mode!=2)return false;if(!target)return true;
  const float* point{};if(!PlayerSkillsRuntime::heading_position(&world_,target,point,error_)||!point)return false;
  dh2::character::LookAtState16 value{{m_.object->position[0],m_.object->position[1],m_.object->position[2]},m_.runtime.rotation.heading_angle};
  if(dh2_character_look_at_point(&value,point))return false;m_.runtime.rotation.heading_angle=value.heading_angle;return true;
 }
 static void animation_service(void* p,dh2::character::AnimationAIState96* ai,const dh2::character::AnimationAIRequest32* q,dh2::character::AnimationAIResponse16* out){
  using namespace dh2::character;auto& t=*static_cast<RendererNpcStateAnimationV1*>(p);auto& m=t.m_;
  if(!q||!out||ai!=&m.animation_ai)throw std::runtime_error("NPC animation AI receiver mismatch");
  auto& playback=m.animation->playback();
  switch(q->service){
   case ai_animation_state:{std::int32_t state{};if(dh2_character_native_fsm_get_integer(&state,&m.machine->native_fsm(),0)!=1)t.fail("Required original NPC current-state getter");out->word=std::uint32_t(state);return;}
   case ai_animation_step_index:out->word=playback.step_index();return;
   case ai_animation_step_count:out->word=playback.step_count(actor_animation_tables);return;
   case ai_has_combo_attack:{
    const auto* props=m.session->property_view().resolved;const auto table=dh2_character_animation_table_id(props[2],std::int32_t(actor_animation_tables.characters.size()));
    const int sequence=t.sequence("Attack",table);int type{};
    if(sequence>=0&&std::size_t(sequence)<actor_animation_tables.sequences.size())type=actor_animation_tables.sequences[sequence].type;
    out->word=dh2_character_animation_has_combo(sequence,std::int32_t(actor_animation_tables.sequences.size()),type);return;
   }
   case ai_target_position:{const float* point{};if(!PlayerSkillsRuntime::heading_position(&t.world_,q->subject,point,t.error_)||!point)t.fail("Required actual NPC animation target position");std::copy(point,point+3,out->position);return;}
   case ai_melee_radius_squared:{float radius{};if(!t.world_.attack_geometry||!t.world_.attack_geometry->melee_radius(m.object->identity,radius,t.error_))t.fail("Required actual NPC melee radius");radius*=radius;std::memcpy(&out->word,&radius,4);return;}
   case ai_controller_move_to:if(t.commands_.move_object(q->payload))t.fail("NPC animation Cmd_MoveTo failed");return;
   case ai_controller_look_at:if(!t.look(1,q->payload))t.fail("NPC animation Cmd_LookAt failed");return;
   case ai_owner_can_range_attack:{int value{};if(!t.world_.attack_geometry||!t.world_.attack_geometry->read(m.object->identity,0,WorldAIAttackQueryV1::CharacterCanRangeAttack,value,t.error_))t.fail("Required NPC actual ranged inventory query");out->word=std::uint32_t(value);return;}
   case ai_target_is_dead:{skills::WorldTargetActorBorrowV1 target{};if(t.world_.targets->actor(q->subject,&target)||!target.life)t.fail("Required NPC actual target life");out->word=target.life->dead;return;}
   case ai_clear_nonsticky_target:if(!ai->target_sticky&&dh2_character_clear_target(&m.object->target,&m.object->binding.services))t.fail("NPC source ClearTarget failed");ai->target=m.object->target.target;return;
   case ai_animation_set_step:playback.set_step(q->argument);return;
   case ai_animation_skip_next_step:playback.skip_next_step();return;
   case ai_animation_stop_loop:playback.stop_loop(q->argument!=0);return;
   case ai_character_event:{
    const AIEventPayload24 payload{q->payload,0,0,0};AIEventResult16 result{};const AIEventServices24 services{&m,MonsterScriptHandle::animation_service,63,0};
    if(dh2_character_ai_event(&result,&m.ai_events,q->argument,&payload,&services))t.fail("Required NPC attack Character event");return;
   }
   case ai_pre_attack_virtual:{
    bool can{};if(!t.world_.attack_geometry||!world_ai_can_attack_v1(m.object->identity,0,m.object->target.target,t.world_.attack_geometry->queries(),can,t.error_))t.fail("Required NPC actual PreAttack gate");
    if(!can)return;ScriptSessionView selected{};if(!m.session->owner().active(selected))return;
    if(selected.identity!=m.ai_events.active||!m.ai_events.ais_virtuals)t.fail("NPC selected AIS PreAttack identity mismatch");
    const auto source=m.ai_events.ais_virtuals[0xa4/4];
    if(source==0x3dbef8)return; // Exact selected AISDefault::OnPreAttack bx-lr.
    if(!t.input_.pre_attack||t.input_.pre_attack(t.input_.context,m,source,q->argument,t.error_))t.fail("Required actual NPC selected AIS PreAttack");return;
   }
   default:throw std::runtime_error("Required NPC animation service "+std::to_string(q->service));
  }
 }
public:
 RendererNpcStateAnimationV1(MonsterScriptHandle& m,PlayerSkillsRuntime& world,RendererNpcCommandsV1& commands,RendererNpcStateAnimationInputsV1 input):m_(m),world_(world),commands_(commands),input_(input){coherent();}
 void refresh_facts(){
  using namespace dh2::character;coherent();auto& f=m_.facts;const auto* p=m_.session->property_view().resolved;
  if(!p)fail("Required NPC resolved property owner");
  const auto queries=world_.targets->targets().query_services();std::uintptr_t player{};
  const dh2::target_providers::Request24 q{dh2::target_providers::virtual_player,0,m_.object->identity,0};
  if(!queries.invoke||queries.invoke(queries.context,&q,&player)||player)fail("NPC state source IsPlayer branch mismatch");
  int count{};const auto* d=m_.context->tables.design();
  if(!d||!d->lookup||d->lookup(d->context,0,"AnimStances","COUNT_IPHONE",&count))fail("Required source NPC GetAnimStance constant");
  f.is_player=0;f.stance=0; // Complete nonplayer GetAnimStance branch3a53e0.
  f.target=m_.object->target.target;std::copy(m_.runtime.controller.heading.direction,m_.runtime.controller.heading.direction+3,f.heading);
  const auto table=dh2_character_animation_table_id(p[2],std::int32_t(actor_animation_tables.characters.size()));
  f.idle=sequence("Idle",table);f.walk=sequence("Walk",table);f.attack_static=sequence("AttackStatic",table);f.attack_moving=sequence("Attack",table);
  dh2::move::Speed speed{};const float one=1;
  if(dh2_move_speed(&speed,p,&one)||dh2_character_attack_speed(&f.attack_speed,p)!=1)fail("Required source NPC movement/attack properties");
  f.walk_speed=speed.walk_multiplier;
  const auto* ai_table=m_.context->tables.ai();if(!ai_table)fail("Required source NPC AI table");
  int ai{};if(dh2_character_target_ai_id(&ai,p,std::uint32_t(ai_table->rows.size())))fail("Required source NPC AI table ID");
  const auto* row=dh2::data::ai_props(*ai_table,ai);if(!row)fail("Required original NPC AI fallback row8");f.attack_delay=std::uint32_t(row->attack_delay);
  f.following_path=m_.runtime.path.count!=0;f.is_at_destination=dh2_nav_is_at_destination(&m_.runtime.controller,&m_.runtime.path)==1;
  m_.machine->state().heading_active=m_.runtime.controller.heading.active;
 }
 void body(dh2::character::State* state,const dh2::character::Request& q){
  using namespace dh2::character;coherent();if(state!=&m_.machine->state())fail("NPC state body receiver mismatch");
  auto& playback=m_.animation->playback();bool okay=true;
  switch(q.service){
   case stop:okay=commands_.object_stop(error_);break;
   case pin:okay=input_.body&&dh2_native_body_pin(input_.body)==0;break;
   case unpin:okay=input_.body&&dh2_native_body_unpin(input_.body)==0;break;
   case set_animation:okay=MonsterScriptHandle::select_animation(&m_,state,q.argument[0])==0;break;
   case set_speed:okay=playback.set_speed(q.scalar,error_);break;
   case swap_animation:okay=playback.swap(actor_animation_tables,q.argument[0],q.argument[1],actor_random,m_.animation->resources().clips(),m_.animation->visual(),m_.animation->scene(),state->cached_speed,error_);break;
   case stop_loop:playback.stop_loop(false);break;
   case start_timer:okay=dh2_character_timer_start(&m_.session->timers(),std::uint32_t(q.argument[0]),q.argument[1],q.argument[2],0,input_.timers)>=0;break;
   case cancel_sneaking:okay=m_.injury&&m_.injury->cancel_sneaking()==0;break;
   case look_at:okay=look(q.argument[0],q.identity);break;
   case set_heading:{float direction[3];std::memcpy(direction,q.argument,12);okay=commands_.character_heading(direction,error_);break;}
   case raise_event:{const AIEventPayload24 payload{q.identity,0,0,0};AIEventResult16 result{};const AIEventServices24 services{&m_,MonsterScriptHandle::animation_service,63,0};okay=dh2_character_ai_event(&result,&m_.ai_events,std::uint32_t(q.argument[0]),&payload,&services)==0;break;}
   case idle_common_update:okay=MonsterScriptHandle::idle_update(&m_)==0;break;
   default:throw std::runtime_error("NPC Walk/Attack adapter does not own source service "+std::to_string(q.service));
  }
  if(!okay)fail("Required NPC Walk/Attack service");refresh_facts();
 }
 const std::string& error()const noexcept{return error_;}
 int step(std::uint32_t operation){
  refresh_facts();auto& ai=m_.animation_ai;ai.animation_depth=std::int32_t(m_.animation->playback().animation_depth());
  // Character+408 IS embedded CharAI(+3c8)+40, the current target.
  ai.owner_flags=m_.machine->state().flags;ai.target=m_.object->target.target;ai.look_target=m_.object->target.target;
  std::copy(m_.object->position.begin(),m_.object->position.end(),ai.owner_position);
  const dh2::character::AnimationAIServices16 services{this,animation_service};
  return dh2_character_animation_ai(&ai,operation,&services);
 }
 // 1 handled;0 not ours;-1 genuine failure. Insert BEFORE the frozen restricted
 // Idle/Injured helper branch. Does not install a second event observer.
 int ai_helper(dh2::character::AIEventState64* ai,const dh2::character::AIEventRequest40& q,std::uint32_t* out){
  using namespace dh2::character;
  if(ai!=&m_.ai_events||!out)return -1;
  if(q.service!=ai_event_helper||q.subject!=ai->ai)return 0;
  std::uint32_t operation{};
  if(q.operation==0x3d4204)operation=ai_step_begin;
  else if(q.operation==0x3d3ff8)operation=ai_step_end;
  else return 0;
  auto& binding=m_.object->binding;const auto* previous=binding.scope;
  struct Restore {TargetBindings48& binding;const dh2_script_callback_scope* scope;~Restore(){binding.scope=scope;}} restore{binding,previous};
  const auto* source_scope=commands_.owner().bindings().scope;if(source_scope)binding.scope=source_scope;
  const int result=step(operation);if(result!=1)return -1;*out=1;return 1;
 }
};
