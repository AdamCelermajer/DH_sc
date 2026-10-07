// Include after renderer_canonical_world_v4.inc, in the same model_renderer
// namespace. This provider borrows the actual World/Scene/PF/physics/UI owners.
namespace {
struct RendererItemGraphRecordV4 {
 std::shared_ptr<dh2::character::RetainedWorldItemObjectV1> item;
 std::unique_ptr<dh2::character::WorldItemPfConnectionV4> pf;
 std::unique_ptr<dh2::character::WorldItemGraphV3> graph;
};
struct RendererItemGraphAdaptersV4 {
 std::shared_ptr<WorldScriptContext> world;
 std::shared_ptr<dh2::world::GameObjectSceneRootRegistryV1> roots;
 dh2::floors::World* floor{};
 dh2::navigation::ObstacleRegistry* obstacles{};
 std::map<std::uintptr_t,RendererItemGraphRecordV4> records;
 dh2::sound::VoxPlay3DOwnerV2 vox;
 explicit RendererItemGraphAdaptersV4(std::shared_ptr<WorldScriptContext> w,
  std::shared_ptr<dh2::world::GameObjectSceneRootRegistryV1> same_roots,
  dh2::floors::World* same_floor,dh2::navigation::ObstacleRegistry* same_obstacles)
  :world(std::move(w)),roots(std::move(same_roots)),floor(same_floor),obstacles(same_obstacles),
   vox({this,vox_service}){}
 ~RendererItemGraphAdaptersV4(){if(item_contact_context_v4==this){item_contact_context_v4=nullptr;item_contact_borrow_v4=nullptr;}}
 void bind_contacts(){item_contact_context_v4=this;item_contact_borrow_v4=[](void* p,void* peer,dh2::navigation::PhysicalContact& out,std::string& e){
  auto& self=*static_cast<RendererItemGraphAdaptersV4*>(p);
  for(auto& record:self.records)if(record.second.graph&&peer==&record.second.graph->physical())
   return dh2::character::world_item_physical_contact_v4(*record.second.graph,out,e);
  e="Required actual registered physical peer receiver; unknown context cannot be cast";return false;
 };}
 static int vox_service(void* p,const dh2::sound::VoxPlay3DRequestV2& q,
   dh2::sound::VoxPlay3DResponseV2& out){
  auto& self=*static_cast<RendererItemGraphAdaptersV4*>(p);
  if(!player_skills_runtime||player_skills_runtime->world!=self.world||!player_skills_runtime->vox_music||
      !q.play||q.play->manager!=player_skills_runtime->vox_music->identity())return -1;
  using O=dh2::sound::VoxPlay3DOperationV2;
  if(q.operation==O::disabled){bool disabled{};std::string error;
   if(!self.debug("IsDisablingSounds",disabled,error))return -1;out.value=disabled;return 0;}
  if(q.operation==O::current_level){out.identity=reinterpret_cast<std::uintptr_t>(&level);
   out.value=self.world->source_level_load_phase;return 0;}
  player_skills_runtime->error="Required actual Item Vox Play3D continuation "+std::to_string(unsigned(q.operation));return -1;
 }
 bool debug(const char* key,bool& value,std::string& error){
  std::uint32_t actual{};
  if(!world||!world->debug||dh2_character_debug_load(world->debug.get(),&world->debug_files)!=1||
      dh2_character_debug_get(&actual,world->debug.get(),key,&world->debug_files)!=1){
   error="Required same World Debug query for Item";return false;}
  value=actual!=0;return true;
 }
 static bool palette(void*,std::int32_t row,std::uint32_t& value,std::string& error){
  const dh2::ui::CharacterMenuFontPaletteV1* actual{};
  return model_renderer::borrow_actual_font_palette_v4(actual,error)&&actual&&actual->text_color(row,value,error);
 }
 bool make_services(const std::shared_ptr<dh2::character::RetainedWorldItemObjectV1>& item,
  dh2::character::WorldItemGraphServicesV3& services,std::string& error){
  using namespace dh2::character;
  if(!item||!world||!roots||!floor||!obstacles||!equipment_platform||equipment_platform->world!=world){
   error="Required same candidate World Item scene/PF/cache owners";return false;}
  services.visual.owner=world;
  services.visual.read_asset=[this](const std::string& uri,auto& bytes,bool& found,std::string& e){
   if(!equipment_platform||equipment_platform->world!=world){e="Required current same World Item cache";return false;}
   return equipment_platform->cache.read(uri.c_str(),found,bytes,e);
  };
  services.initialization.owner=world;
  services.initialization.condition_init=[this,item](std::uint32_t offset,std::string& e){
   // Genuine ctor/default empty condition returns before Arrays::Conditions.
   // Positive named conditions still demand the actual compiled-table owner.
   dh2::world::ConditionDataInitServicesV3 s;s.owner=world;
   return dh2::world::condition_data_init_v3(item->base(),offset,s,e);
  };
  services.initialization.check_spawn_probability=[this,item](std::int32_t& roll,std::string& e){
   dh2::data::LootTablesV2::Borrow tables;dh2::data::ItemPowerTablesV5::Borrow powers;
   dh2::data::ItemTextServicesV5 text;dh2::data::LootRandom8V2* random{};
   if(!player_equipment||!player_equipment->loot_sources_v8(tables,powers,text,random,e))return false;
   auto& base=item->base();dh2::world::GameObjectSpawnProbabilityBorrowV1 b;
   b.owner=world;b.cached_roll270=base.integer(0x270);b.probability274=base.integer(0x274);
   b.network_id108=base.integer(0x108);b.online_owner_fc=base.integer(0xfc);b.byte82=base.byte(0x82);b.random0=random;
   b.handle_as_player_character=[item](bool& out,std::string& e){auto borrow=item->canonical(item);
    std::uintptr_t character{};if(!borrow.as_character||!borrow.as_character(borrow.context,character,e))return false;
    if(character){e="Item source AsChar unexpectedly produced Character";return false;}out=false;return true;};
   b.online_byte5=[this](bool& out,std::string& e){if(!equipment_platform||equipment_platform->world!=world){e="Required same Item GetOnline";return false;}out=equipment_platform->online_mode;return true;};
   std::int32_t probability{};return dh2::world::game_object_check_spawn_probability_v1(b,roll,probability,e);
  };
  services.initialization.device_high_performance=[](bool& out,std::string& e){
   return model_renderer::borrow_actual_device_high_performance_v54(out,e);
  };
  services.position.owner=world;
  services.physical.owner=world;services.physical.debug=[this](const char* key,bool& value,std::string& e){return debug(key,value,e);};
  services.color.design=world->design->borrow();services.color.font_text_color=palette;
  services.vox=&vox;
  if(!player_skills_runtime||player_skills_runtime->world!=world||!player_skills_runtime->vox_music){error="Required same World Item Vox manager";return false;}
  services.vox_identity=player_skills_runtime->vox_music->identity();
  return true;
 }
 bool create(const std::shared_ptr<dh2::character::RetainedWorldItemObjectV1>& item,std::string& error){
  using namespace dh2::character;
  if(!item||!world||!roots||!floor||!obstacles||!equipment_platform||equipment_platform->world!=world){
   error="Required same candidate World Item scene/PF/cache owners";return false;}
  const auto id=item->base().identity();if(records.count(id)){error="Item graph already constructed";return false;}
  auto [it,inserted]=records.try_emplace(id);(void)inserted;auto& record=it->second;record.item=item;
  record.pf=std::make_unique<WorldItemPfConnectionV4>(*item,world,&floor->collision_world,obstacles);
  WorldItemGraphServicesV3 services;if(!make_services(item,services,error))return false;
  services.visual=dh2::world::world_item_scene_services_v3(std::move(services.visual),roots,
   [this,id](std::uintptr_t root){auto i=records.find(id);return i==records.end()||!i->second.graph?
     std::shared_ptr<dh2::world::RetainedGameObjectVisualV1>{}:i->second.graph->visual().constructing_root_v3(root);});
  services.visual.update_pf=[this,id](std::string& e){return records.at(id).pf->update(e);};
  services.physical.update_pf=[this,id](std::string& e){return records.at(id).pf->update(e);};
  record.graph=std::make_unique<WorldItemGraphV3>(item,actor_world,std::move(services));
  record.pf->physical(&record.graph->physical());return true;
 }
 void suspend()noexcept{floor=nullptr;obstacles=nullptr;for(auto& record:records)record.second.pf->suspend();}
 void rebind(dh2::floors::World& same,dh2::navigation::ObstacleRegistry& registry)noexcept{
  floor=&same;obstacles=&registry;for(auto& record:records)record.second.pf->rebind(&same.collision_world,&registry);
 }
};
} // anonymous retained World scope
