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
  if(!native.owner)native.owner=source->files_owner;
  const auto weak=world;
  if(!native.physical_filter)native.physical_filter=[weak](auto& base,std::uintptr_t physical,bool enabled,std::string& e){auto source=weak.lock();SourceCampaignCandidateBorrowV55 current;std::shared_ptr<void> pin;dh2::physical::NativePhysicalFilterBorrowV1 fields;
   if(!source||!borrow_source_campaign_candidate_v55(current,e)||current.actual_world!=source->owner){if(e.empty())e="Released actual Stage10 physical-filter scope";return false;}
   const auto* slot=base.pointer(0x2dc);if(!slot||*slot!=physical){e="Changed SAME Stage10 PhysicalObject2dc filter receiver";return false;}
   if(!borrow_source_campaign_physical_filter_v105(current,base.identity(),pin,fields,e)||!pin)return false;
   return dh2::physical::native_physical_filter_v1(fields,enabled,e);
  };
  if(!native.null_handle_assertion)native.null_handle_assertion=dh2::world::source_object_handle_null_assertion_v105;
  // This is a transient receipt of the existing services authority. It creates
  // no catalog, class, graph or owner and is not retained by the loading loop.
  RendererCampaignNonCharacterCompositionV69 retained;retained.services=services;
  return bind_campaign_object_loading_v95(actual,retained,rooms,std::move(native),inputs,e);
 };
 e.clear();return true;
}
