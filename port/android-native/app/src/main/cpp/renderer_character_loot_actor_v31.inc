// Include after RendererCharacterLootGameplayV23, all SAME retained actor
// definitions, and character_loot_actor_binding_v31.hpp at global scope.
class RendererCharacterLootActorV31 {
 PlayerSkillsRuntime& t_;
 struct PickupCallbacks {void* context{};bool(*enqueue_status)(void*,const char*,std::uint32_t,std::string&){};bool(*remaining)(void*,const dh2::character::LootInteractRequestV8&,dh2::character::LootInteractResponseV8&,std::string&){};} pickup_;
 std::unique_ptr<dh2::character::CharacterLootActorBindingV31> actor_;
 static bool fields(void* raw,std::uintptr_t id,dh2::character::CharacterLootActorFieldsV31& out,std::string& e){
  auto& self=*static_cast<RendererCharacterLootActorV31*>(raw);auto& t=self.t_;
  if(!t.world||!t.world->canonical_world||!t.targets){e="Required actual canonical World before loot actor borrow";return false;}
  const auto i=t.world_actors.find(id);
  if(i==t.world_actors.end()||!i->second.object||!i->second.shared_handle){e="Required same registered Character loot receiver";return false;}
  auto& a=i->second;const auto* canonical=t.world->canonical_world->manager.object(a.shared_handle->key);
  if(!canonical||canonical->identity!=id||canonical->shared_handle!=a.shared_handle||!canonical->lease){e="Required SAME published canonical Character loot Handle/lease";return false;}
  out.canonical=*canonical;out.receiver_lease=canonical->lease;
  if(t.world->player_object&&id==t.world->player_object->identity){
   if(!t.player||!t.world->player_canonical){e="Required actual retained player loot properties/OOI";return false;}
   out.properties=&t.player->session().property_view();out.object_of_interest14a4=&t.world->player_canonical->ooi14a4;out.gear=player_equipment.get();
  }else{
   if(!a.npc||!a.npc->session){e="Required actual retained NPC session for loot";return false;}
   out.properties=&a.npc->session->property_view();out.object_of_interest14a4=&a.npc->source_ooi14a4;
  }
  return true;
 }
public:
 explicit RendererCharacterLootActorV31(PlayerSkillsRuntime& t):t_(t){}
 bool connect(RendererLootGameplayProvidersV23& p,std::string& e){
  if(actor_||!t_.targets||!t_.world||!t_.world->tables.ai()||!p.source.application_lease){e="Required first sameWorld loot actor connection";return false;}
  actor_=std::make_unique<dh2::character::CharacterLootActorBindingV31>(*t_.targets,*t_.world->tables.ai(),dh2::character::CharacterLootActorServicesV31{p.source.application_lease,this,fields});
  p.source.loot.provider_lease=p.source.application_lease;p.source.loot.context=this;
  p.source.loot.actor=[](void* raw,auto id,auto& out,auto& error){return static_cast<RendererCharacterLootActorV31*>(raw)->actor_->drop_actor(id,out,error);};
  p.source.loot.is_player=[](void* raw,auto id,auto& out,auto& error){return static_cast<RendererCharacterLootActorV31*>(raw)->actor_->is_player(id,out,error);};
  pickup_={p.source.pickup.context,p.source.pickup.enqueue_status,p.source.pickup.remaining};p.source.pickup.provider_lease=p.source.application_lease;p.source.pickup.context=this;
  p.source.pickup.actor=[](void* raw,auto id,auto& out,auto& error){return static_cast<RendererCharacterLootActorV31*>(raw)->actor_->pickup_actor(id,out,error);};
  p.source.pickup.character_cast=[](void* raw,auto id,auto& out,auto& error){return static_cast<RendererCharacterLootActorV31*>(raw)->actor_->character_cast(id,out,error);};
  p.source.pickup.enqueue_status=[](void* raw,const char* text,std::uint32_t metadata,std::string& error){auto& s=static_cast<RendererCharacterLootActorV31*>(raw)->pickup_;if(!s.enqueue_status){error="Required SAME Application status queue";return false;}return s.enqueue_status(s.context,text,metadata,error);};
  p.source.pickup.remaining=[](void* raw,const auto& q,auto& out,std::string& error){auto& s=static_cast<RendererCharacterLootActorV31*>(raw)->pickup_;if(!s.remaining){error="Required positive pickup continuation "+std::to_string(unsigned(q.operation));return false;}return s.remaining(s.context,q,out,error);};
  return true;
 }
};
