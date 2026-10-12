// Inside model_renderer after renderer_campaign_object_loading_v95.inc,
// before the source owner assembly/Character connection. No new owner TU.
static bool install_campaign_object_loading_inputs_v95(const std::shared_ptr<SourceWorldBorrowV61>& source,
 const RendererCampaignNonCharacterCompositionV69& composition,
 const std::shared_ptr<dh2::loader::ModuleRoomZoneConnectionV91>& rooms,std::string& e){
 if(!source||!composition.services||!composition.services->current(e)||!rooms||source->source_object_loading_v95){
  if(e.empty())e="Required once-only actual Stage10/17 owner enrollment";return false;
 }
 const std::weak_ptr<SourceWorldBorrowV61> world=source;
 const std::weak_ptr<RendererCampaignNonCharacterV69> graph=composition.services;
 const std::weak_ptr<dh2::loader::ModuleRoomZoneConnectionV91> room_scope=rooms;
 source->source_object_loading_v95=[world,graph,room_scope](const SourceCampaignCandidateBorrowV55& actual,
  dh2::loader::SourceLoadingInputsV43& inputs,std::string& e){
  auto source=world.lock();auto services=graph.lock();auto rooms=room_scope.lock();
  if(!source||!services||!services->current(e)||!rooms||actual.actual_world.get()!=source->owner.get()||
   actual.actual_world.owner_before(source->owner)||source->owner.owner_before(actual.actual_world)){
   if(e.empty())e="Retired actual Stage10/17 loading owners";return false;
  }
  CampaignObjectLoadingNativeV95 native;
  if(source->object_loading_native_v95&&!source->object_loading_native_v95(actual,native,e))return false;
  // This is a transient receipt of the existing services authority. It creates
  // no catalog, class, graph or owner and is not retained by the loading loop.
  RendererCampaignNonCharacterCompositionV69 retained;retained.services=services;
  return bind_campaign_object_loading_v95(actual,retained,rooms,std::move(native),inputs,e);
 };
 e.clear();return true;
}
