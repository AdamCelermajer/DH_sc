// Include after MonsterScriptHandle + shared floor/registry and actual source
// event providers. TU includes npc_script_commands_v1.hpp and canonical globals.
// Retain on MonsterScriptHandle BEFORE construct_script; destroy after session.
struct RendererNpcCommandInputsV1 {
 void* context{};
 int(*position)(void*,std::uintptr_t,const float*&){};
 bool(*raise)(void*,MonsterScriptHandle&,std::uint32_t,const dh2_script_callback_scope*,std::string&){};
 int(*attack)(void*,MonsterScriptHandle&,std::uintptr_t,const dh2_script_callback_scope*){};
 int(*number)(void*,const dh2_script_value*,float*){};
 dh2::physical::NativeBody* body{};
};
class RendererNpcCommandsV1 {
 MonsterScriptHandle& m_;RendererNpcCommandInputsV1 inputs_;
 dh2::physical::NativeBody* body_{};
 std::unique_ptr<dh2::character::CharacterHeadingOwnerV1> heading_;
 std::unique_ptr<dh2::character::NpcScriptCommandsV1> commands_;
 std::array<float,3> look_{};std::string error_;
 static int remote(void* p,bool& out){auto& t=*static_cast<RendererNpcCommandsV1*>(p);
  const auto* remote=t.m_.source_bool_field(0x118);if(!remote)return -1;
  out=t.m_.machine->combat_fields().network_id!=-1||*remote;return 0;
 }
 static bool heading_remote(void* p,bool& out,std::string&){return remote(p,out)==0;}
 static int position(void* p,std::uintptr_t id,const float*& out){auto& t=*static_cast<RendererNpcCommandsV1*>(p);return t.inputs_.position?t.inputs_.position(t.inputs_.context,id,out):-1;}
 static bool heading_position(void* p,std::uintptr_t id,const float*& out,std::string&){return position(p,id,out)==0;}
 static bool heading_physics(void* p,bool& out,std::string&){out=(static_cast<RendererNpcCommandsV1*>(p)->m_.machine->state().flags&2)!=0;return true;}
 static bool raise(void* p,std::uint32_t event,std::string& error){auto& t=*static_cast<RendererNpcCommandsV1*>(p);
  t.m_.machine->state().heading_active=t.m_.runtime.controller.heading.active;
  t.m_.runtime.rotation.heading_angle=t.m_.runtime.controller.heading.angle;
  return t.inputs_.raise&&t.inputs_.raise(t.inputs_.context,t.m_,event,t.commands_?t.commands_->bindings().scope:nullptr,error);
 }
 static int look(void* p,const float*& out){auto& t=*static_cast<RendererNpcCommandsV1*>(p);
  // Whole GameObject.GetLookAtVec393ae4: actual EulerZ+174, sin, negative
  // cos, Z0. Caller-owned output persists through subsequent conversions.
  const float angle=t.m_.runtime.rotation.rotation[2];t.look_={std::sin(angle),-std::cos(angle),0.f};out=t.look_.data();return 0;
 }
 static int attack(void* p,std::uintptr_t id,const dh2_script_callback_scope* scope){auto& t=*static_cast<RendererNpcCommandsV1*>(p);return t.inputs_.attack?t.inputs_.attack(t.inputs_.context,t.m_,id,scope):-1;}
 static int number(void* p,const dh2_script_value* value,float* out){auto& t=*static_cast<RendererNpcCommandsV1*>(p);
  // Actual native Value numeric/boolean/nil domains; string/object conversions
  // require the real source conversion provider, never Lua tonumber fallback.
  if(!value||!out)return -1;if(value->type==DH2_SCRIPT_NUMBER){*out=value->number;return 0;}
  if(value->type==DH2_SCRIPT_BOOLEAN){*out=value->boolean?1.f:0.f;return 0;}
  if(value->type==DH2_SCRIPT_NIL){*out=0;return 0;}
  return t.inputs_.number?t.inputs_.number(t.inputs_.context,value,out):-1;
 }
 static int path_policy(void* p,std::uint32_t* enabled){auto& t=*static_cast<RendererNpcCommandsV1*>(p);auto& world=*t.m_.context;
  std::uint32_t profile{};
  if(!enabled||dh2_character_debug_load(world.debug.get(),&world.debug_files)!=1||
   dh2_character_debug_get(&profile,world.debug.get(),"PF_ProfileSearches",&world.debug_files)!=1)return -1;
  // Source timing-ring branch requires the real os::Timer producer. Its
  // genuine missing-file default is0; do not mask an enabled profiling switch.
  if(profile)return -1;
  if(!world.effect_modules||dh2_fx_debug_preload_service(world.effect_modules.get(),dh2::fx::debug_load,nullptr,enabled))return -1;
  return dh2_fx_debug_module_get(enabled,world.effect_modules.get(),"PathFinding_Search")==1?0:-1;
 }
public:
 RendererNpcCommandsV1(MonsterScriptHandle& m,RendererNpcCommandInputsV1 inputs,dh2::floors::World* candidate_floor):m_(m),inputs_(inputs),body_(inputs.body){
  if(!m.object||!m.machine||!m.controller||!candidate_floor)throw std::runtime_error("Required actual NPC command graph/candidate floor");
  auto* command=m.controller->command_state(controller_global_blocked);
  if(m.machine->owner().machine().physical!=reinterpret_cast<std::uintptr_t>(body_))throw std::runtime_error("NPC command physical borrow mismatch");
  heading_=std::make_unique<dh2::character::CharacterHeadingOwnerV1>(*command,m.machine->state(),m.object->target,m.object->binding.services,m.runtime.controller,m.runtime.path,m.object->position.data(),m.runtime.subobjects.destination,body_,dh2::character::CharacterHeadingServicesV1{this,heading_position,heading_remote,heading_physics,raise});
  dh2::character::NpcScriptCommandBorrowV1 b{m.object->identity,command,&m.runtime,&m.object->target,m.object->position.data(),dh2::world::canonical_vec3_k_v1().data(),m.source_bool_field(0x84),&m.source_path_limit26c,heading_.get(),candidate_floor};
  commands_=std::make_unique<dh2::character::NpcScriptCommandsV1>(b,dh2::character::NpcScriptCommandServicesV1{this,remote,position,look,attack,number,path_policy});
 }
 static int refresh(void* p){auto& t=*static_cast<RendererNpcCommandsV1*>(p);
  auto* command=t.m_.controller->command_state(controller_global_blocked);if(!command)return -1;
  command->locked=t.m_.machine->state().controller_locked;return t.commands_->refresh()?0:-1;
 }
 void attach(dh2::character::CharacterScriptSessionInput& input){
  input.commands=&commands_->bindings();input.commands_refresh_context=this;input.commands_refresh=refresh;
 }
 bool initialize_pf(const dh2::navigation::ProducerFields& source_fields){return commands_->initialize_pf(live_registry,source_fields);}
 bool suspend_navigation(){return commands_->suspend_navigation();}
 bool rebind_floor(dh2::floors::World* actual_floor){return commands_->rebind_floor(actual_floor);}
 dh2::character::NpcScriptCommandsV1& owner(){return *commands_;}
 // Source FSM Stop bypasses the controller wrapper's Character event3f.
 bool object_stop(std::string& error){return heading_->object_stop(error);}
 bool character_heading(const float point[3],std::string& error){return heading_->character_head_towards(point,error);}
 int move_object(std::uintptr_t target){
  if(refresh(this))return -1;auto& b=commands_->bindings();const float* out{};
  const dh2::character::ScriptCommandRequest40 q{dh2::character::script_controller_move_object,0,b.state->controller,target,{},0};
  return b.services.invoke(b.services.context,b.state,&q,&out);
 }
};
