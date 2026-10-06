// Include after renderer_item_graph_v4.inc and source Kill production adapters.
// This constructs ONE V5 Item graph/pool; never also call old adapter.create.
// The provider request carries actual source callbacks, not ready flags.
struct RendererLootGameplayProvidersV23 {
 dh2::data::LootPowerResourcesV7::Borrow powers;
 dh2::data::LootAudioVisualV8::Borrow audiovisual;
 dh2::character::WorldLootGameplayServicesV23 source;
 dh2::navigation::ObstacleRegistry* obstacles{}; // SAME actual World registry
 void* physical_context{};
 bool(*physical_services)(void*,dh2::character::RetainedWorldItemObjectV1&,
  dh2::character::WorldItemPhysicalServicesV2&,std::string&){};
};
class RendererCharacterLootGameplayV23 {
 PlayerSkillsRuntime& t_;
 RendererLootGameplayProvidersV23 providers_;
 dh2::character::WorldItemLiveServicesV5 source_items_;
 std::unique_ptr<RendererItemGraphAdaptersV4> graph_services_;
 std::unique_ptr<dh2::character::WorldLootGameplayV23> owner_;
 static bool graph(void* raw,dh2::character::RetainedWorldItemObjectV1& receiver,
   dh2::character::WorldItemGraphServicesV3& out,std::string& e){
  auto& self=*static_cast<RendererCharacterLootGameplayV23*>(raw);
  auto* live=self.owner_?self.owner_->items():nullptr;
  auto item=live?live->factory().find(receiver.base().identity()):nullptr;
  if(!item||item.get()!=&receiver||!self.graph_services_){e="Required SAME canonical loot Item receiver/graph service";return false;}
  if(!self.graph_services_->make_services(item,out,e))return false;
  if(!self.providers_.physical_services){e="Required actual mixed-world PhysicalObject peer projections";return false;}
  return self.providers_.physical_services(self.providers_.physical_context,receiver,out.physical,e);
 }
 static bool outer(void* raw,const dh2::character::WorldItemRequestV1& q,
   std::int32_t& value,std::string& e){auto& self=*static_cast<RendererCharacterLootGameplayV23*>(raw);
  if(!self.source_items_.outer_item){e="Required actual Item Character-moving/local/tooltip continuation";return false;}
  return self.source_items_.outer_item(self.source_items_.context,q,value,e);
 }
 static bool manager(void* raw,dh2::world::CanonicalObjectManagerV1*& out,std::string& e){
  auto& self=*static_cast<RendererCharacterLootGameplayV23*>(raw);
  if(!self.t_.world||!self.t_.world->canonical_world){e="Required SAME Application ObjectManager38";return false;}
  out=&self.t_.world->canonical_world->manager;return true;
 }
public:
 RendererCharacterLootGameplayV23(PlayerSkillsRuntime& t,RendererLootGameplayProvidersV23 p)
  :t_(t),providers_(std::move(p)),source_items_(providers_.source.items){}
 bool bind(std::string& e){
  if(owner_||!t_.world||!t_.targets||!t_.player_manager||!t_.world->canonical_world||
     !t_.world->canonical_world->scene_roots_v20||!level.native_floor||!player_equipment||
     !actor_world.backend()||!providers_.obstacles){e="Required SAME ready World/PM/Gear/Scene/floor/PF/PhysicalWorld loot construction boundary";return false;}
  auto& canonical=*t_.world->canonical_world;
  auto services=std::move(providers_.source);
  // Callback/provider leases are supplied independently to avoid a World->
  // owner->World ownership cycle. Root retains this adapter in its runtime.
  services.gear=player_equipment.get();services.debug=t_.world->debug.get();services.debug_files=t_.world->debug_files;
  services.items.world=t_.world;services.items.roots=canonical.scene_roots_v20;
  graph_services_=std::make_unique<RendererItemGraphAdaptersV4>(t_.world,
   canonical.scene_roots_v20,level.native_floor.get(),providers_.obstacles);
  services.items.context=this;services.items.graph_services=graph;services.items.outer_item=outer;
  services.update_prefix.debug=t_.world->debug.get();services.update_prefix.files=t_.world->debug_files;
  services.update_prefix.context=this;services.update_prefix.manager38=manager;
  owner_=std::make_unique<dh2::character::WorldLootGameplayV23>(*t_.targets,
   t_.player_manager->manager(),canonical.manager,canonical.properties,actor_world,
   &level.native_floor->collision_world,providers_.obstacles,
   providers_.powers,providers_.audiovisual,std::move(services));
  return owner_->initialize(e);
 }
 // Root calls this from its actual pending-object InitFinal stage, once.
 bool initialize_final(std::string& e){if(!owner_){e="Required bound Item145 source stage";return false;}return owner_->initialize_final(e);}
 bool route(dh2::character::KillActor56& actor,const dh2::character::KillRequest56& q,
  dh2::character::KillResponse16& out,bool& handled,std::string& e){
  handled=q.service==dh2::character::kill_drop_loot;if(!handled)return true;
  if(!owner_){e="Required live positive DropLoot graph before Hit8";return false;}
  return owner_->route(actor,q,out,handled,e);
 }
 bool frame(std::uint32_t same_scene_ms,std::uint32_t source_dt_ms,std::string& e){
  if(!owner_||!owner_->update(same_scene_ms,source_dt_ms,e))return false;
  std::vector<dh2::character::LootDrawSceneV23> scenes;if(!owner_->draw_scenes(scenes,e))return false;
  std::vector<model_renderer::LootVisualDrawSourceV27> draws;draws.reserve(scenes.size());
  for(auto& entry:scenes)draws.push_back({entry.object,std::move(entry.visual)});
  return model_renderer::sync_loot_visual_draws_v27(draws,e);
 }
 dh2::character::WorldLootGameplayV23* owner()noexcept{return owner_.get();}
 bool release(std::string& e){
  if(!owner_){e="Required same loot owner before source release";return false;}
  if(!owner_->release(e))return false;
  return model_renderer::sync_loot_visual_draws_v27({},e);
 }
};
