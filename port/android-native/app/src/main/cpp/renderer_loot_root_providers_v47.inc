// Include after V44 loot composition, V31 actor helpers, BodyOwner and actual
// player/network/Gear/FX definitions. All headers are included at TU scope.
// No pool/frame/Save is created merely by construction; root retains this
// sibling beside its V44 connection and applies the exact publication hooks.
struct RendererLootRootContractsV47 {
 std::shared_ptr<void> application_lease;
 std::shared_ptr<dh2::character::LootPlayerFieldAssociationV47> player_fields;
 dh2::ui::HudTextV1* text{};dh2::ui::HudTextEnvironmentV1 text_environment;
 dh2::data::ItemTextServicesV5 item_text;
 void* context{};
 bool(*unknown_type_debug)(void*,const char*,std::string&){};
 bool(*null_handle_assertion)(void*,std::string&){};
 bool(*condition_is_true)(void*,std::uintptr_t,bool&,std::string&){};
 bool(*decor_peer)(void*,void*,dh2::character::LootPhysicalPeerBorrowV44&,std::string&){};
 bool(*decor_fields)(void*,std::uintptr_t,dh2::character::LootPhysicalPeerBorrowV44&,std::string&){};
 bool(*positive_frame)(void*,dh2::character::WorldItemGraphV3&,dh2::character::WorldItemFrameServicesV5&,std::string&){};
 bool(*pickup_remaining)(void*,const dh2::character::LootInteractRequestV8&,dh2::character::LootInteractResponseV8&,std::string&){};
};
class RendererLootRootProvidersV47 {
 PlayerSkillsRuntime& t_;RendererLootRootContractsV47 source_;
 RendererCanonicalLootConnectionV44* connection_{};
 std::unique_ptr<dh2::character::CharacterLootActorBindingV31> actors_;
 std::unique_ptr<dh2::character::LootPickupSourceLeavesV47> leaves_;
 std::unique_ptr<dh2::character::CharacterLootPickupFxV32> pickup_fx_;
 dh2::fx::CharacterMeshFxOwnerV4* pickup_fx_manager_{};
 std::map<std::uintptr_t,std::unique_ptr<dh2::character::LootItemFrameSourceBridgeV47>> frames_;
 bool connected_{};
 bool same(std::string& e){
  if(!t_.world||!t_.world->canonical_world||!t_.targets||!t_.player_manager||
     !equipment_platform||equipment_platform->world!=t_.world){
   e="Required SAME live registered World/PM/Gear loot root";return false;
  }return true;
 }
 static bool fields(void* raw,std::uintptr_t id,dh2::character::LootPhysicalPeerBorrowV44& out,std::string& e){
  auto& self=*static_cast<RendererLootRootProvidersV47*>(raw);if(!self.same(e))return false;
  auto& t=self.t_;const auto found=t.world_actors.find(id);
  if(found==t.world_actors.end()){
   if(!self.source_.decor_fields){e="Required canonical decor visible80 source owner";return false;}
   return self.source_.decor_fields(self.source_.context,id,out,e);
  }
  const auto& a=found->second;
  if(!a.object||a.object->identity!=id||!a.shared_handle){e="Required SAME registered Character peer";return false;}
  const auto* canonical=t.world->canonical_world->manager.object(a.shared_handle->key);
  if(!canonical||canonical->identity!=id||canonical->shared_handle!=a.shared_handle||!canonical->lease){
   e="Required SAME published Character physical peer";return false;
  }
  dh2::character::LootPhysicalPeerBorrowV44 next;next.object=canonical;next.receiver_lease=canonical->lease;
  if(t.world->player_object&&id==t.world->player_object->identity){
   if(!self.source_.player_fields||self.source_.player_fields->character()!=id||!t.world->player_canonical){
    e="Required actual player missing-field authority";return false;
   }
   next.visible80=self.source_.player_fields->visible80();
   next.character_ooi14a4=&t.world->player_canonical->ooi14a4;
  }else{
   if(!a.npc){e="Required actual retained NPC source fields";return false;}
   next.visible80=a.npc->source_bool_field(0x80);next.character_ooi14a4=&a.npc->source_ooi14a4;
  }
  if(!next.visible80){e="Required source PropertyMap/SetVisible byte80 publication; screen visibility is not this field";return false;}
  out=std::move(next);return true;
 }
 static bool peer(void* raw,void* address,dh2::character::LootPhysicalPeerBorrowV44& out,std::string& e){
  auto& self=*static_cast<RendererLootRootProvidersV47*>(raw);if(!self.same(e))return false;
  if(address==&prince_body_owner){
   if(!self.t_.world->player_object||prince_body_owner.native!=&prince_body||!prince_body.body){
    e="Required SAME current player body/native receiver";return false;
   }
   return fields(raw,self.t_.world->player_object->identity,out,e);
  }
  // Actual retained NPC PO receivers can be projected by their source physical
  // slot. Do not reinterpret body/FSM/decor addresses as this C++ class.
  for(const auto& row:self.t_.world_actors){
   const auto& actor=row.second;
   if(actor.npc&&actor.npc->position_fields_v7().physical2dc==reinterpret_cast<std::uintptr_t>(address)&&address)
    return fields(raw,row.first,out,e);
  }
  for(const auto& owner:decor_body_owners)if(address==owner.get()){
   if(!self.source_.decor_peer){e="Required actual decor PhysicalObject.owner8/canonical association";return false;}
   return self.source_.decor_peer(self.source_.context,address,out,e);
  }
  e="Unknown physical callback address; no guessed owner or cast";return false;
 }
 static bool save(void* raw,std::uintptr_t id,dh2::character::LootCharacterSaveBorrowV44& out,std::string& e){
  auto& self=*static_cast<RendererLootRootProvidersV47*>(raw);if(!self.same(e))return false;
  if(!self.t_.world->player_object||id!=self.t_.world->player_object->identity||
     !self.source_.player_fields||!self.t_.world->player_canonical){
   e="Required actual local player Character14e8 authority";return false;
  }
  return self.source_.player_fields->borrow_save(self.t_.world->player_canonical,out,e);
 }
 static bool save_fields(void* raw,std::uintptr_t id,dh2::character::LootCharacterSaveBorrowV44& out,
   const std::uintptr_t*& slot,std::string& e){
  auto& self=*static_cast<RendererLootRootProvidersV47*>(raw);
  if(!save(raw,id,out,e))return false;slot=self.source_.player_fields->save_slot14e8();return true;
 }
 static bool state(void* raw,std::uintptr_t id,const std::int32_t*& out,std::shared_ptr<void>& lease,std::string& e){
  auto& self=*static_cast<RendererLootRootProvidersV47*>(raw);if(!self.same(e))return false;
  const auto found=self.t_.world_actors.find(id);
  if(found==self.t_.world_actors.end()||!found->second.shared_handle){e="Required registered Character state producer";return false;}
  const auto* canonical=self.t_.world->canonical_world->manager.object(found->second.shared_handle->key);
  if(!canonical||canonical->identity!=id||!canonical->lease){e="Required SAME canonical state receiver";return false;}
  auto* owner=(self.t_.world->player_object&&id==self.t_.world->player_object->identity)?
   &prince_state_owner:found->second.npc&&found->second.npc->machine?&found->second.npc->machine->owner():nullptr;
  if(!owner){e="Required actual retained CharStateMachine owner";return false;}
  if(!dh2::character::loot_state_info_borrow_v47(*owner,id,out,e))return false;
  lease=canonical->lease;return true;
 }
 static bool actor_fields(void* raw,std::uintptr_t id,dh2::character::CharacterLootActorFieldsV31& out,std::string& e){
  auto& self=*static_cast<RendererLootRootProvidersV47*>(raw);if(!self.same(e))return false;
  const auto i=self.t_.world_actors.find(id);
  if(i==self.t_.world_actors.end()||!i->second.object||!i->second.shared_handle){e="Required actual registered loot Character";return false;}
  auto& a=i->second;const auto* c=self.t_.world->canonical_world->manager.object(a.shared_handle->key);
  if(!c||c->identity!=id||!c->lease){e="Required SAME canonical loot Actor Handle";return false;}
  out.canonical=*c;out.receiver_lease=c->lease;
  if(self.t_.world->player_object&&id==self.t_.world->player_object->identity){
   if(!self.t_.player||!self.t_.world->player_canonical){e="Required retained player loot properties";return false;}
   out.properties=&self.t_.player->session().property_view();out.object_of_interest14a4=&self.t_.world->player_canonical->ooi14a4;
   out.gear=player_equipment.get();
  }else{
   if(!a.npc||!a.npc->session){e="Required retained NPC loot properties";return false;}
   out.properties=&a.npc->session->property_view();out.object_of_interest14a4=&a.npc->source_ooi14a4;
  }return true;
 }
 static bool is_player(void* raw,std::uintptr_t id,bool& out,std::string& e){
  auto& self=*static_cast<RendererLootRootProvidersV47*>(raw);dh2::character::CharacterLootActorFieldsV31 b;
  if(!actor_fields(raw,id,b,e))return false;
  const auto i=self.t_.world_actors.find(id);
  if(i==self.t_.world_actors.end()||!i->second.object){e="Required SAME Character NAME CString30";return false;}
  return dh2::character::loot_character_is_player_v47(*self.t_.world->tables.ai(),*b.properties,i->second.object->name,out,e);
 }
 static bool pickup_actor(void* raw,std::uintptr_t id,dh2::character::LootPickupActorV23& out,std::string& e){
  dh2::character::CharacterLootActorFieldsV31 b;if(!actor_fields(raw,id,b,e))return false;
  bool player{};if(!is_player(raw,id,player,e))return false;
  out={id,b.receiver_lease,b.properties,b.object_of_interest14a4,b.gear,player};return true;
 }
 static bool local(void* raw,std::uintptr_t id,bool& out,std::string& e){
  auto& self=*static_cast<RendererLootRootProvidersV47*>(raw);
  if(!id){out=false;return true;}
  if(!self.same(e)||!self.t_.player_network_local){e="Required SAME source PlayerManager/network identity";return false;}
  dh2::player::PlayerInfoFieldsV1* info{};
  if(!self.t_.player_manager->manager().get_by_character(id,false,info,e))return false;
  if(!info){e="Required source IsLocal PlayerInfo receiver";return false;}
  return self.t_.player_network_local->is_local(*info,out,e);
 }
 static bool online(void* raw,bool& out,std::string& e){
  auto& self=*static_cast<RendererLootRootProvidersV47*>(raw);if(!self.same(e))return false;
  out=equipment_platform->online_mode;return true;
 }
 static bool online_frame(void* raw,bool& out,std::string& e){return online(raw,out,e);}
 static bool trophy(void* raw,std::uintptr_t id,const char* key,std::string& e){
  auto& self=*static_cast<RendererLootRootProvidersV47*>(raw);
  if(!self.same(e)||!key||!self.t_.trophy_bindings){e="Required SAME named trophy manager";return false;}
  dh2::character::CharacterLootActorFieldsV31 actor;if(!actor_fields(raw,id,actor,e))return false;
  if(self.t_.trophy_bindings->unlock_named(key)){e=self.t_.trophies->error();return false;}return true;
 }
 static bool item_actions(void* raw,std::uintptr_t id,dh2::ui::CharacterMenuItemActionsGraphV1& graph,std::string& e){
  auto& self=*static_cast<RendererLootRootProvidersV47*>(raw);dh2::character::LootPickupActorV23 actor;
  if(!pickup_actor(raw,id,actor,e)||!actor.source_is_player||!actor.gear){e="Required actual player Gear item action";return false;}
  if(!actor.gear->bind_menu_item_actions_v4(graph,e))return false;
  graph.transmute_multiplier=[&self](std::int32_t& value,std::string& error){
   return dh2::character::world_loot_design_constant_v44(self.t_.world->tables.design(),"CharacterDesign","TransmuteMultiplier",value,error);
  };
  graph.is_player=[&self,id](bool& v,std::string& error){return is_player(&self,id,v,error);};
  graph.is_local_player=[&self,id](bool& v,std::string& error){return local(&self,id,v,error);};
  graph.online=[&self](bool& v,std::string& error){return online(&self,v,error);};
  graph.achievement=[&self,id](const char* key,std::string& error){return trophy(&self,id,key,error);};
  return true;
 }
 static bool frame(void* raw,dh2::character::WorldItemGraphV3& graph,
   dh2::character::WorldItemFrameServicesV5& out,std::string& e){
  auto& self=*static_cast<RendererLootRootProvidersV47*>(raw);
  if(!self.same(e)||!level.native_floor||!self.t_.world->canonical_world->scene_roots_v20){
   e="Required SAME active floor/scene for Item frame";return false;
  }
  dh2::character::WorldItemFrameServicesV5 actual;
  actual.geometry=&level.native_floor->collision_world;actual.paths=&level.native_floor->graph;
  actual.obstacles=&live_registry;actual.motion=&live_motion_policy;actual.workspace=&live_workspace;
  actual.roots=self.t_.world->canonical_world->scene_roots_v20;actual.online={&self,online_frame,nullptr,nullptr};
  if(self.source_.positive_frame&&!self.source_.positive_frame(self.source_.context,graph,actual,e))return false;
  const auto id=graph.receiver_v4().base().identity();
  auto [it,added]=self.frames_.try_emplace(id);if(added)it->second=std::make_unique<dh2::character::LootItemFrameSourceBridgeV47>(graph);
  return it->second->bind(std::move(actual),out,e);
 }
 static bool pickup_remaining(void* raw,const dh2::character::LootInteractRequestV8& q,
   dh2::character::LootInteractResponseV8& out,std::string& e){
  auto& self=*static_cast<RendererLootRootProvidersV47*>(raw);bool handled{};
  if(!self.leaves_||!self.leaves_->route(q,out,handled,e))return false;if(handled)return true;
  if(q.operation==dh2::character::LootInteractOperationV8::loot_fx){
   auto* items=self.connection_&&self.connection_->gameplay()?self.connection_->gameplay()->owner()->items():nullptr;
   if(!items||!self.t_.combat_fx||!self.actors_){e="Required SAME initialized Item/FX/Actor owners";return false;}
   auto& manager=self.t_.combat_fx->manager();
   if(self.pickup_fx_manager_&&self.pickup_fx_manager_!=&manager){
    e="Actual FX owner changed; source pickup static lookup requires explicit retained rebind";return false;
   }
   if(!self.pickup_fx_){
    self.pickup_fx_=std::make_unique<dh2::character::CharacterLootPickupFxV32>(
     manager,self.t_.world->effects_owner->borrow(),*self.actors_,*items);self.pickup_fx_manager_=&manager;
   }
   return self.pickup_fx_->route(q,out,handled,e)&&handled;
  }
  if(!self.source_.pickup_remaining){e="Required reached actual pickup leaf "+std::to_string(unsigned(q.operation));return false;}
  return self.source_.pickup_remaining(self.source_.context,q,out,e);
 }
public:
 RendererLootRootProvidersV47(PlayerSkillsRuntime& t,RendererLootRootContractsV47 source)
  :t_(t),source_(std::move(source)){
  if(!source_.application_lease)throw std::invalid_argument("Required source loot root Application lifetime");
 }
 void bind_connection(RendererCanonicalLootConnectionV44& same){connection_=&same;}
 bool connect(RendererLootGameplayProvidersV23& p,dh2::character::WorldLootCanonicalServicesV44& canonical,std::string& e){
  if(connected_||!same(e))return false;connected_=true;
  actors_=std::make_unique<dh2::character::CharacterLootActorBindingV31>(
   *t_.targets,*t_.world->tables.ai(),dh2::character::CharacterLootActorServicesV31{source_.application_lease,this,actor_fields});
  dh2::character::LootPickupLeavesServicesV47 leaves;
  leaves.application_lease=source_.application_lease;leaves.context=this;leaves.actor=pickup_actor;
  leaves.named_is_player=is_player;leaves.is_local_player=local;leaves.online=online;leaves.save_fields=save_fields;
  leaves.achievement=trophy;leaves.bind_item_actions=item_actions;leaves.text=source_.text;
  leaves.text_environment=source_.text_environment;leaves.design=t_.world->tables.design();leaves.settings=t_.world->saved_options.get();
  leaves.item_text=source_.item_text;
  leaves.difficulty_global=dh2::player::process_player_save_difficulty_global_v29();
  leaves_=std::make_unique<dh2::character::LootPickupSourceLeavesV47>(std::move(leaves));
  p.source.context=this;p.source.frame_services=frame;
  p.source.tooltip_ooi=[](void* raw,std::uintptr_t id,std::uintptr_t& out,std::string& error){
   dh2::character::CharacterLootActorFieldsV31 fields;
   if(!actor_fields(raw,id,fields,error))return false;
   if(!fields.object_of_interest14a4){error="Required actual tooltip Character OOI14a4";return false;}
   out=*fields.object_of_interest14a4;return true;
  };
  p.source.loot.provider_lease=source_.application_lease;p.source.loot.context=this;
  p.source.loot.actor=[](void* raw,std::uintptr_t id,auto& out,std::string& error){
   auto& self=*static_cast<RendererLootRootProvidersV47*>(raw);return self.actors_->drop_actor(id,out,error);
  };
  p.source.loot.is_player=is_player;
  // Parent preserves/provides status callback separately; its prior context
  // must not be overwritten without that callback's explicit wrapper.
  auto prior_status=p.source.pickup.enqueue_status;auto prior_context=p.source.pickup.context;
  prior_status_=prior_status;prior_status_context_=prior_context;
  p.source.pickup.provider_lease=source_.application_lease;p.source.pickup.context=this;
  p.source.pickup.actor=pickup_actor;p.source.pickup.remaining=pickup_remaining;
  p.source.pickup.character_cast=[](void* raw,std::uintptr_t id,std::uintptr_t& out,std::string& error){
   auto& self=*static_cast<RendererLootRootProvidersV47*>(raw);return self.actors_->character_cast(id,out,error);
  };
  p.source.pickup.settings=t_.world->saved_options.get();
  p.source.pickup.enqueue_status=[](void* raw,const char* text,std::uint32_t meta,std::string& error){
   auto& self=*static_cast<RendererLootRootProvidersV47*>(raw);
   if(!self.prior_status_){error="Required SAME Application StatusMsg4 queue";return false;}
   return self.prior_status_(self.prior_status_context_,text,meta,error);
  };
  canonical.application_lease=source_.application_lease;canonical.context=this;
  canonical.physical_peer=peer;canonical.character_fields=fields;canonical.character_save=save;canonical.character_state=state;
  canonical.unknown_type_debug=[](void* raw,const char* name,std::string& error){auto& self=*static_cast<RendererLootRootProvidersV47*>(raw);if(!self.source_.unknown_type_debug){error="Required actual unknown type Debug";return false;}return self.source_.unknown_type_debug(self.source_.context,name,error);};
  canonical.null_handle_assertion=[](void* raw,std::string& error){auto& self=*static_cast<RendererLootRootProvidersV47*>(raw);if(!self.source_.null_handle_assertion){error="Required actual NULL Handle Debug";return false;}return self.source_.null_handle_assertion(self.source_.context,error);};
  canonical.condition_is_true=[](void* raw,std::uintptr_t id,bool& v,std::string& error){auto& self=*static_cast<RendererLootRootProvidersV47*>(raw);if(!self.source_.condition_is_true){error="Required actual compiled Condition";return false;}return self.source_.condition_is_true(self.source_.context,id,v,error);};
  return true;
 }
private:
 void* prior_status_context_{};
 bool(*prior_status_)(void*,const char*,std::uint32_t,std::string&){};
};
