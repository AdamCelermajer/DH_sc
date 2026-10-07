// Include after V47 loot provider and actual source held/controller declarations.
// Root retains this sibling, provides the SAME controller raw flags/slots and
// genuine positive online methods. No new controller bootstrap is performed.
struct RendererUseOoiContractsV47 {
 dh2::character::ControllerUseOoiFieldsV47 controller;
 dh2::character::ControllerUseOoiServicesV47 network;
};
class RendererUseOoiV47 {
 PlayerSkillsRuntime& t_;
 RendererCanonicalLootConnectionV44& loot_;
 std::unique_ptr<dh2::character::LootItemTargetSourceV47> item_targets_;
 std::unique_ptr<dh2::character::CharacterUseOoiV47> character_;
 std::unique_ptr<dh2::character::ControllerUseOoiV47> controller_;
 static bool remote(void* raw,std::uintptr_t id,bool& out,std::string& e){
  auto& self=*static_cast<RendererUseOoiV47*>(raw);
  if(!self.t_.world||!self.t_.world->player_object||id!=self.t_.world->player_object->identity){e="Required SAME Character Ctrl_UseOOI remote34";return false;}
  return PlayerSkillsRuntime::heading_remote(&self.t_,out,e);
 }
 static bool state(void* raw,std::uintptr_t id,std::int32_t& out,std::string& e){
  auto& self=*static_cast<RendererUseOoiV47*>(raw);const std::int32_t* info{};
  if(!self.t_.world||!self.t_.world->player_object||id!=self.t_.world->player_object->identity||
     !dh2::character::loot_state_info_borrow_v47(prince_state_owner,id,info,e))return false;
  out=info?*info:-1;return true;
 }
 static bool position(void* raw,std::uintptr_t id,const float*& out,std::string& e){
  auto& self=*static_cast<RendererUseOoiV47*>(raw);return PlayerSkillsRuntime::heading_position(&self.t_,id,out,e);
 }
 static bool online(void* raw,bool& out,std::string& e){
  auto& self=*static_cast<RendererUseOoiV47*>(raw);
  if(!equipment_platform||equipment_platform->world!=self.t_.world){e="Required SAME NetworkManager byte5 source owner";return false;}
  out=equipment_platform->online_mode;return true;
 }
 static bool control(void* raw,std::uintptr_t receiver,std::uintptr_t requested,std::string& e){
  auto& self=*static_cast<RendererUseOoiV47*>(raw);
  if(receiver!=self.t_.attack_controller.controllable||receiver!=reinterpret_cast<std::uintptr_t>(&prince_runtime.controller)||!self.character_){
   e="Required SAME verified Character controllable4 virtual50 receiver";return false;
  }
  return self.character_->control(requested,e);
 }
public:
 RendererUseOoiV47(PlayerSkillsRuntime& t,RendererCanonicalLootConnectionV44& loot):t_(t),loot_(loot){}
 bool bind(RendererUseOoiContractsV47 source,std::string& e){
  if(controller_||!t_.world||!t_.world->player_object||!t_.world->player_canonical||!t_.player){e="Required SAME unbound source UseOOI player";return false;}
  const auto id=t_.world->player_object->identity;
  if(!source.controller.actorC||!source.controller.controllable4||
     source.controller.actorC!=&t_.attack_controller.character||
     source.controller.controllable4!=&t_.attack_controller.controllable||
     *source.controller.actorC!=id||*source.controller.controllable4!=reinterpret_cast<std::uintptr_t>(&prince_runtime.controller)){
   e="Required existing controller actorC/controllable4 fields; no duplicate association";return false;
  }
  auto* items=loot_.gameplay()&&loot_.gameplay()->owner()?loot_.gameplay()->owner()->items():nullptr;
  if(!items){e="Required same stage29 Item145 owner before positive UseOOI binding";return false;}
  item_targets_=std::make_unique<dh2::character::LootItemTargetSourceV47>(*items,
   t_.world->canonical_world->manager,t_.world->player_object->binding.services,*t_.world->tables.ai(),
   t_.player->session().property_view(),this,position);
  character_=std::make_unique<dh2::character::CharacterUseOoiV47>(
   dh2::character::CharacterUseOoiFieldsV47{t_.world->player_object,id,&t_.world->player_object->target,
    &t_.world->player_canonical->ooi14a4,&t_.skill_state.heading_enabled},
   dh2::character::CharacterUseOoiServicesV47{this,remote,state,item_targets_->borrow()});
  // Network methods retain their original separate context. Do not overwrite
  // it to install the local virtual tail: use a retained forwarding adapter.
  network_=source.network;source.network.context=this;
  source.network.online5=online;source.network.control50=control;
  source.network.actor=[](void* raw,std::uintptr_t id,auto& out,std::string& e){auto& s=*static_cast<RendererUseOoiV47*>(raw);return s.network_.actor&&s.network_.actor(s.network_.context,id,out,e);};
  source.network.is_character24=[](void* raw,std::uintptr_t id,bool& out,std::string& e){auto& s=*static_cast<RendererUseOoiV47*>(raw);return s.network_.is_character24&&s.network_.is_character24(s.network_.context,id,out,e);};
  source.network.is_dead=[](void* raw,std::uintptr_t id,bool& out,std::string& e){auto& s=*static_cast<RendererUseOoiV47*>(raw);return s.network_.is_dead&&s.network_.is_dead(s.network_.context,id,out,e);};
  source.network.interaction90=[](void* raw,std::uintptr_t id,std::uintptr_t actor,std::int32_t& out,std::string& e){auto& s=*static_cast<RendererUseOoiV47*>(raw);return s.network_.interaction90&&s.network_.interaction90(s.network_.context,id,actor,out,e);};
  source.network.client_get=[](void* raw,std::uintptr_t& out,std::shared_ptr<void>& lease,std::string& e){auto& s=*static_cast<RendererUseOoiV47*>(raw);return s.network_.client_get&&s.network_.client_get(s.network_.context,out,lease,e);};
  source.network.message_create=[](void* raw,const char* name,bool flag,auto& out,std::string& e){auto& s=*static_cast<RendererUseOoiV47*>(raw);return s.network_.message_create&&s.network_.message_create(s.network_.context,name,flag,out,e);};
  source.network.message_send=[](void* raw,std::uintptr_t id,const auto& message,std::string& e){auto& s=*static_cast<RendererUseOoiV47*>(raw);return s.network_.message_send&&s.network_.message_send(s.network_.context,id,message,e);};
  controller_=std::make_unique<dh2::character::ControllerUseOoiV47>(std::move(source.controller),source.network);return true;
 }
 // Exact ui::HudAttackServicesV46::use_ooi endpoint; root context is this owner.
 static bool held(void* raw,std::uintptr_t controller,std::uintptr_t requested,std::string& e){
  auto& self=*static_cast<RendererUseOoiV47*>(raw);
  if(!self.controller_||self.controller_->identity()!=controller){e="Required SAME held controller378 UseOOI owner";return false;}
  return self.controller_->command(requested,e);
 }
private:
 dh2::character::ControllerUseOoiServicesV47 network_;
};
