// Include after NPC inventory and same World execution/attack geometry.
// TU includes character_melee_animation_event_v1.hpp. No observer replacement.
struct RendererNpcMeleeInputsV1 {
 void* context{};
 int(*remaining)(void*,MonsterScriptHandle&,const dh2::character::skills::MeleeAnimationRequestV1&,dh2::character::skills::MeleeAnimationResponseV1&,std::string&){};
 int(*activate)(void*,std::uintptr_t object,std::uintptr_t source){};
 dh2::character::NpcAnimationEventOwnerV1* animation_events{};
 dh2::fx::CharacterMeshFxOwnerV1* mesh_fx{};
 const dh2_script_callback_scope* const* current_scope{};
};
class RendererNpcMeleeEventV1 {
 MonsterScriptHandle& m_;PlayerSkillsRuntime& world_;RendererNpcInventoryV1& inventory_;
 RendererNpcMeleeInputsV1 input_;std::uint32_t source_on_attack_{};std::string error_;
 std::unique_ptr<dh2::character::skills::CharacterWorldMeleeAttackV1> attack_;
 static int activate(void* p,std::uintptr_t object,std::uintptr_t source){auto& t=*static_cast<RendererNpcMeleeEventV1*>(p);return t.input_.activate?t.input_.activate(t.input_.context,object,source):-1;}
 static int service(void* p,const dh2::character::skills::MeleeAnimationRequestV1* q,dh2::character::skills::MeleeAnimationResponseV1* out){
  using namespace dh2::character;using namespace dh2::character::skills;auto& t=*static_cast<RendererNpcMeleeEventV1*>(p);auto& m=t.m_;
  if(!q||!out||!m.animation||!m.machine||t.inventory_.owner.inventory().properties()!=m.object->properties)return -1;
  switch(q->operation){
   case melee_event_relay:
    if(!t.input_.animation_events){t.error_="Required NPC actual animation-event Lua/FX owner";return -1;}
    if(!t.input_.animation_events->relay(q->text,t.input_.current_scope?*t.input_.current_scope:nullptr)){t.error_=t.input_.animation_events->error();return -1;}return 0;
   case melee_event_mesh_fx:
    if(!t.input_.mesh_fx){t.error_="Required actual shared NPC animation FX manager";return -1;}
    return t.input_.mesh_fx->animation_event(q->text,m.object->position.data(),t.error_)?0:-1;
   case melee_event_step_index:out->value=std::int32_t(m.animation->playback().step_index());return 0;
   case melee_event_step_count:out->value=std::int32_t(m.animation->playback().step_count(actor_animation_tables));return 0;
   case melee_event_state:{std::int32_t state{};if(dh2_character_native_fsm_get_integer(&state,&m.machine->native_fsm(),0)!=1)return -1;out->value=state;return 0;}
   case melee_event_can_range:{
    int value{};if(!t.world_.attack_geometry||!t.world_.attack_geometry->read(m.object->identity,m.object->target.target,WorldAIAttackQueryV1::CharacterCanRangeAttack,value,t.error_))return -1;
    out->value=value;if(!value)return 0;
    // True source result requires its actual projectile out parameter.
    break;
   }
   case melee_event_attack:{
    if(!m.ai_events.ais_virtuals){t.error_="Required selected NPC AIS OnAttack source table";return -1;}
    t.source_on_attack_=std::uint32_t(m.ai_events.ais_virtuals[0xa8/4]);
    const int result=t.attack_->attack(m.animation_ai.attack_index,q->step,q->offhand);
    t.world_.execution->publish_compatibility_fields();if(result)t.error_=t.attack_->error();return result;
   }
   default:break;
  }
  if(!t.input_.remaining){t.error_="Required original NPC authored prefix/projectile/skill endpoint "+std::to_string(q->operation)+" "+(q->text?q->text:"");return -1;}
  return t.input_.remaining(t.input_.context,m,*q,*out,t.error_);
 }
public:
 RendererNpcMeleeEventV1(MonsterScriptHandle& m,PlayerSkillsRuntime& world,RendererNpcInventoryV1& inventory,RendererNpcMeleeInputsV1 input):m_(m),world_(world),inventory_(inventory),input_(input){
  using namespace dh2::character::skills;
  if(!world.execution||!world.targets||!world.attack_geometry||world.world!=m.context||&inventory.npc!=&m||!m.object)
   throw std::runtime_error("Required same NPC inventory/World melee execution owners");
  auto& object=*m.object;
  attack_=std::make_unique<CharacterWorldMeleeAttackV1>(world.execution->combat(),*world.targets,world.combat_context,combat_random,
   WorldMeleeAttackBorrowV1{object.identity,&object.target.target,&object.target.target,&m.ai_events.active,&source_on_attack_,&inventory.owner.inventory()},
   world.attack_geometry->queries(),WorldMeleeAttackBackendsV1{this,activate,nullptr});
 }
 int named(const char* name){
  using namespace dh2::character::skills;error_.clear();MeleeAnimationOutputV1 out{};
  const MeleeAnimationServicesV1 services{this,service,world_.execution->combat().application_services().debug};
  const int result=dh2_character_melee_animation_event_v1(&out,name,&services);
  if(result&&error_.empty())error_="NPC source named event failed phase "+std::to_string(out.phase);
  return result;
 }
 const std::string& error()const noexcept{return error_;}
 const dh2::data::CombatResult& result()const noexcept{return attack_->result();}
};
