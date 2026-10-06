// After PlayerSkillsRuntime and SAME world attack geometry definitions.
// This include adds no calls or mutations until root explicitly adopts it.
struct RendererNpcAttackInputsV1 {
 void* context{};
 const std::uintptr_t* object_of_interest14a4{};
 const std::uint8_t* network_enabled_a{};
 int(*remaining)(void*,MonsterScriptHandle&,const dh2::character::AttackRequest32*,
  dh2::character::AttackResponse16*,const dh2_script_callback_scope*,std::string&){};
};
class RendererNpcAttackCommandV1 {
 MonsterScriptHandle& m_;RendererNpcAttackInputsV1 inputs_;
 dh2::character::skills::CharacterWorldAttackGeometryV1& geometry_;
 std::unique_ptr<dh2::character::NpcAttackCommandOwnerV1> owner_;
 static bool online(void* p,bool& value,std::string& error){auto& t=*static_cast<RendererNpcAttackCommandV1*>(p);
  if(!equipment_platform||equipment_platform->world!=t.m_.context){error="Required same World online owner";return false;}
  value=equipment_platform->online_mode;return true;
 }
 static bool radius(void* p,std::uintptr_t id,float& value,std::string& error){return static_cast<RendererNpcAttackCommandV1*>(p)->geometry_.melee_radius(id,value,error);}
 static int backend(void* p,const dh2::character::AttackRequest32* q,
  dh2::character::AttackResponse16* out,const dh2_script_callback_scope* scope,std::string& error){
  using namespace dh2::character;auto& t=*static_cast<RendererNpcAttackCommandV1*>(p);
  if(!q||!out||!t.m_.context)return -1;auto& world=*t.m_.context;
  if(q->service==attack_frontal_angle){
   const auto* design=world.tables.design();std::int32_t value{};
   if(!design||!design->lookup||design->lookup(design->context,0,"CharacterDesign","Attack_FrontalAngle",&value))return -1;
   std::memcpy(&out->word,&value,4);return 0;
  }
  if(q->service==attack_diagnostic){
   const char* key=nullptr;
   switch(q->argument0){
    case 0x3d0294:case 0x3d03c0:key="isTracingCharAICommands";break;
    case 0x3d0354:case 0x3d0630:case 0x3d06b4:case 0x3d0538:key="isTracingChar_MeleePotentialTarget";break;
    default:return -1;
   }
   return dh2_character_debug_load(world.debug.get(),&world.debug_files)==1&&
    dh2_character_debug_get(&out->word,world.debug.get(),key,&world.debug_files)==1?0:-1;
  }
  if(!t.inputs_.remaining){error="Required whole source NPC ranged attack or network message owner";return -1;}
  return t.inputs_.remaining(t.inputs_.context,t.m_,q,out,scope,error);
 }
public:
 RendererNpcAttackCommandV1(MonsterScriptHandle& m,
  dh2::character::skills::CharacterWorldRuntimeV1& world,
  dh2::character::skills::CharacterWorldAttackGeometryV1& geometry,
  RendererNpcAttackInputsV1 inputs):m_(m),inputs_(inputs),geometry_(geometry){
  if(!m.object||!m.controller||!m.machine)throw std::runtime_error("Required retained NPC attack graph");
  auto* command=m.controller->command_state(controller_global_blocked);
  owner_=std::make_unique<dh2::character::NpcAttackCommandOwnerV1>(
   dh2::character::NpcAttackCommandBorrowV1{&world,&m.object->binding,m.machine.get(),command,
    &m.animation_ai,inputs.object_of_interest14a4,inputs.network_enabled_a},
   dh2::character::NpcAttackCommandServicesV1{this,online,backend,geometry.queries(),radius});
 }
 int command(std::uintptr_t target,const dh2_script_callback_scope* scope){
  auto* c=m_.controller->command_state(controller_global_blocked);if(!c)return -1;
  c->locked=m_.machine->state().controller_locked;
  return owner_->command(target,scope);
 }
 const std::string& error()const noexcept{return owner_->error();}
 const dh2_script_callback_scope* scope_borrow()const noexcept{return owner_->scope_borrow();}
};
