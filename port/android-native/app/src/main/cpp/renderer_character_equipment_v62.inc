// Private concrete source Gear transport; include after V60 record types and
// before renderer_character_campaign_v62.inc. Same scene and C1 inventory.
struct RendererCanonicalEquipmentV62 {
 std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> actor;
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> application;
 std::shared_ptr<dh2::character::CharacterCandidateCacheV62> cache;
 std::shared_ptr<void> presentation;
 std::shared_ptr<void> inventory_services_v118;
 static dh2::skinning::VisualAssetResultV6 asset(void* raw,const char* uri,std::vector<std::uint8_t>& bytes,std::string& e){
  auto& self=*static_cast<RendererCanonicalEquipmentV62*>(raw);bool found{};
  if(!uri||!self.cache||!self.cache->files().read(uri,found,bytes,e))return dh2::skinning::VisualAssetResultV6::failed;
  return found?dh2::skinning::VisualAssetResultV6::found:dh2::skinning::VisualAssetResultV6::missing;
 }
 static bool world(void* raw,dh2::player::EquipmentWorldQueryV1 q,std::uintptr_t subject,std::uintptr_t& id,std::int32_t& value,std::string& e){
  auto& self=*static_cast<RendererCanonicalEquipmentV62*>(raw);auto record=self.actor.lock();auto app=self.application.lock();id=0;value=0;
  if(!record||!app){e="Expired actual canonical Gear platform";return false;}
  using Q=dh2::player::EquipmentWorldQueryV1;
  if(q==Q::online){auto online=app->get_online_loading_v55();if(!online){e="Required actual Gear GetOnline receiver";return false;}value=online->byte5()!=0;return true;}
  if(q==Q::player_count){auto manager=app->source_player_manager_v59();if(!manager||!manager->count_field()){e="Required actual Gear PM character_count6c4";return false;}value=*manager->count_field();return true;}
  if(subject!=record->actor->object->identity){e="Gear query requires its SAME canonical Character";return false;}
  if(q==Q::current_player){dh2::loader::CanonicalCurrentLevelBorrowV1 level;if(!model_renderer::borrow_current_native_level_v27(level,e))return false;id=level?level.identity():0;return true;}
  if(q==Q::current_difficulty){dh2::player::CharacterSaveDifficultyBorrowV29 borrow{record->actor,subject,record->save_fields->save_slot14e8(),record->services.difficulty_global};return dh2::player::character_game_difficulty_v29(borrow,value,e);}
  if(q==Q::remotely_updated){const auto* byte=record->actor->source_bool_field(0x118);if(!byte){e="Required same Character remote118 producer";return false;}value=*byte;return true;}
  e="Required actual online Gear PlayerInfo/network projection";return false;
 }
};
static bool make_canonical_equipment_inputs_v62(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 const std::shared_ptr<dh2::character::CharacterCandidateCacheV62>& cache,
 dh2::world::CanonicalCharacterCandidateRecordV60& record,
 dh2::player::PlayerEquipmentRenderInputsV1& out,std::string& e){
 if(!app||!cache||!record.visual||!record.visual->visual()||!record.actor||!record.profile_bootstrap){e="Required actual selected-profile Character visual before source SG_Load4 Gear creation";return false;}
 auto visual=record.visual->visual();
 if(record.actor->source_visual()!=reinterpret_cast<std::uintptr_t>(visual.get())){e="Gear attempted a second Character VisualObject";return false;}
 std::shared_ptr<dh2::ui::OwnedHudSettingsV1> settings;dh2::ui::HudTextEnvironmentV1 text;std::shared_ptr<void> presentation;
 if(!model_renderer::borrow_actual_gameplay_presentation_v67(record.services.world,settings,text,presentation,e))return false;
 if(!settings||settings->language()<0||settings->language()>8){e="Required actual selected StringManager language pack before Gear";return false;}
 const auto& bres=visual->bres();if(!bres.bytes||!bres.size){e="Required actual retained visual BRES for modular Skin factory";return false;}
 std::vector<std::uint8_t> bytes(bres.bytes,bres.bytes+bres.size);dh2::skinning::VisualSkinResourcesV6 resources;
 if(!resources.load(bytes,e))return false;
 auto platform=std::make_shared<RendererCanonicalEquipmentV62>();platform->actor=record.shared_from_this();platform->application=app;platform->cache=cache;platform->presentation=presentation;
 out.character=record.actor->object->identity;out.properties=record.properties;out.random=record.services.random;
 std::shared_ptr<dh2::character::SourceItemResourcesV88> items;
 if(!borrow_source_campaign_item_resources_v88(record.services.world,items,e)||!items||!items->ready())return false;
 if(!make_source_campaign_equipment_services_v118(app,record.shared_from_this(),items,out.required,platform->inventory_services_v118,e))return false;
 const auto release=record.services.release_player_equipment;
 if(!release){e="Required actual Character Gear release before inventory callback enrollment";return false;}
 record.services.release_player_equipment=[release,required=platform->inventory_services_v118](auto& actual,auto& error){
  if(!release_source_campaign_equipment_presentation_v118(required,actual,error))return false;
  return release(actual,error);
 };
 out.immutable_loot_v88=items->loot();out.immutable_powers_v88=items->definitions();
 out.resources=resources.borrow();out.live_scene=&visual->scene();out.language_pack=settings->language();out.text_environment=text;
 out.assets={platform.get(),RendererCanonicalEquipmentV62::asset};out.world={platform.get(),RendererCanonicalEquipmentV62::world};out.debug=record.services.debug;if(record.services.debug_files)out.debug_files=*record.services.debug_files;
 out.services_lease_v62=platform;return true;
}
