// Include inside model_renderer after V69 and actual Character/source borrowers.
// Global: source_object_loading_v95.hpp, renderer_campaign_object_loading_v95.hpp,
// source_campaign_character_fsm_v101.hpp, renderer_native_gslevel_v27.hpp, source_campaign_module_rooms_v91.hpp.
struct CampaignObjectLoadingProviderV95 {
 std::weak_ptr<RendererCampaignNonCharacterV69> graph;
 std::weak_ptr<SourceWorldBorrowV61> source;
 CampaignObjectLoadingNativeV95 native;
 bool current(std::shared_ptr<RendererCampaignNonCharacterV69>& graph_out,std::shared_ptr<SourceWorldBorrowV61>& world_out,SourceCampaignCandidateBorrowV55& actual,std::string& e)const{
  graph_out=graph.lock();world_out=source.lock();
  if(!graph_out||!world_out||!graph_out->current(e)||!borrow_source_campaign_candidate_v55(actual,e)||actual.actual_world.get()!=world_out->owner.get()||actual.actual_world.owner_before(world_out->owner)||world_out->owner.owner_before(actual.actual_world)){if(e.empty())e="Required live SAME Stage10/17 campaign";return false;}return true;
 }
 bool visible(const dh2::world::CanonicalObjectBorrowV1& object,dh2::world::CanonicalGameObjectBaseOwnerV1& base,bool enabled,std::string& e){
  std::shared_ptr<RendererCampaignNonCharacterV69> graph;std::shared_ptr<SourceWorldBorrowV61> world;SourceCampaignCandidateBorrowV55 actual;if(!current(graph,world,actual,e))return false;
  if(!base.store_byte(0x80,enabled?base.lifecycle().enabled8a:0,e))return false;
  const auto* visual=base.pointer(0x2d8);if(!visual){e="Unproduced actual visual2d8";return false;}if(!*visual)return true;
  if(object.class_name20&&*object.class_name20&&(!std::strcmp(*object.class_name20,"Module")||!std::strcmp(*object.class_name20,"Block"))){
   if(!native.module_sync_visibility){e="Required SAME ModuleGraph SyncVisibility export";return false;}return native.module_sync_visibility(object.identity,e);
  }
  return graph->source_visibility(base,e);
 }
 bool enabled_event(const dh2::world::CanonicalObjectBorrowV1& object,const dh2::world::CanonicalObjectLoadingFieldsV95& fields,bool enabled,std::string& e){
  if(fields.objectbase_enabled_event)return fields.objectbase_enabled_event(enabled,e);
  std::shared_ptr<RendererCampaignNonCharacterV69> graph;std::shared_ptr<SourceWorldBorrowV61> world;SourceCampaignCandidateBorrowV55 actual;if(!current(graph,world,actual,e))return false;
  std::uintptr_t character{};if(!object.as_character||!object.as_character(object.context,character,e))return false;
  if(character){if(character!=object.identity){e="Changed source Character conversion";return false;}return source_campaign_character_enabled_event_v102(world->owner,character,enabled,e);}
  auto* base=fields.gameobject_base;if(!base||base->identity()!=object.identity){e="Required SAME GameObject Enabled/Disabled base";return false;}
  //ObjectBase33de04/34 then GameObject38ba38/04. Preserve original store/call order.
  if(!enabled)base->lifecycle().updating85=0;
  if(!visible(object,*base,enabled,e))return false;
  if(enabled)base->lifecycle().updating85=1;
  if(enabled)base->runtime().object.motion.object_flags|=8;else base->runtime().object.motion.object_flags&=~8u;
  const auto physical=base->pointer(0x2dc);if(!physical){e="Unproduced SAME physical2dc";return false;}
  if(*physical){if(!native.physical_filter){e="Required SAME PhysicalObject saved-filter/shape18/1c/disabled26 borrower";return false;}if(!native.physical_filter(*base,*physical,enabled,e))return false;}
  if(!base->store_byte(0x373,enabled?0:1,e))return false;
  //Door3e7bd8/94 adds genuine PFWorld.FlagFloorAsDeadEnd when state3a8!=1.
  if(object.class_name20&&*object.class_name20&&!std::strcmp(*object.class_name20,"Door")){
   for(const auto& weak:graph->auxiliary_journal){auto record=weak.lock();if(!record||!record->class_name||std::strcmp(record->class_name,"Door")||!record->identity||record->identity()!=object.identity)continue;
    auto same=std::static_pointer_cast<dh2::loader::CanonicalDoorRecordV27>(record->record);
    if(!same||!same->owner||&same->owner->base()!=base){e="Changed SAME Door enabled receiver";return false;}
    if(same->owner->source_state3a8()!=1){dh2::world::DoorServicesV27 services;dh2::loader::AuxiliaryDeliveryV67 delivery{actual.actual_world,actual.level,actual.objects,actual.properties};
     if(!graph->native.source.auxiliary.door||!graph->native.source.auxiliary.door(delivery,*same->owner,services,e)||!services.startup.flag_floor_dead_end){if(e.empty())e="Required actual Door PF dead-end primitive";return false;}
     if(!services.startup.flag_floor_dead_end(*same->owner,enabled,e))return false;
    }return true;
   }e="Required SAME admitted Door record";return false;
  }
  return true;
 }
 bool test(const dh2::world::CanonicalObjectBorrowV1& object,bool mark,std::string& e){
  using namespace dh2::world;
  std::shared_ptr<RendererCampaignNonCharacterV69> graph;std::shared_ptr<SourceWorldBorrowV61> world;SourceCampaignCandidateBorrowV55 actual;if(!current(graph,world,actual,e))return false;
  auto transport=actual.receiver_transport_v69.lock();const CanonicalClassReceiverV1* dispatch{};
  if(!transport||!transport->receiver(object,dispatch,e)||!dispatch||!dispatch->source_loading_fields_v95){if(e.empty())e="Required SAME loading receiver field producer";return false;}
  auto producer=dispatch->source_loading_fields_v95;CanonicalObjectLoadingFieldsV95 fields;
  if(!producer(fields,e)||!fields.receiver||fields.receiver.owner_before(object.lease)||object.lease.owner_before(fields.receiver)){if(e.empty())e="Foreign Stage10 ObjectBase field loan";return false;}
  struct Delivery {CampaignObjectLoadingProviderV95& self;const CanonicalObjectBorrowV1& object;CanonicalObjectLoadingFieldsV95& fields;std::shared_ptr<RendererCampaignNonCharacterV69> graph;std::shared_ptr<SourceWorldBorrowV61> world;} delivery{*this,object,fields,graph,world};
  ObjectEnableConditionServicesV2 services;services.context=&delivery;
  services.local_profile=[](void* raw,bool& character,bool& saved,std::uint8_t& ready,std::string& e){auto& d=*static_cast<Delivery*>(raw);character=false;saved=false;ready=0;
   if(!d.world->player_manager){e="Required actual App40 PlayerManager";return false;}dh2::player::PlayerInfoFieldsV1* info{};
   if(!d.world->player_manager->get_local_player(0,true,info,e)||!info){if(e.empty())e="Required actual GetLocalPlayer0 assertion branch";return false;}
   if(!info->character660)return true;character=true;SourceCampaignCharacterBorrowV62 same;
   if(!borrow_source_campaign_character_v62(d.world->owner,info->character660,same,e)||!same.character||!same.character->save_fields||!same.character->save_fields->save_slot14e8())return false;
   const auto identity=*same.character->save_fields->save_slot14e8();if(!identity)return true;
   if(!same.character->save||identity!=reinterpret_cast<std::uintptr_t>(same.character->save.get())){e="Required SAME actual Character Save14e8";return false;}
   saved=true;ready=same.character->save->source_quest_sync_ready14_v3();return true;
  };
  services.current_level=[](void*,bool& present,std::int32_t& mode118,std::string& e){dh2::loader::CanonicalCurrentLevelBorrowV1 current;if(!borrow_current_native_level_v27(current,e))return false;present=bool(current);if(present)mode118=current.level()->constructor_fields_v3().mode118;return true;};
  services.condition_is_true=[](void* raw,std::uintptr_t identity,bool& accepted,std::string& e){auto& d=*static_cast<Delivery*>(raw);
   auto runtime=d.graph->native.condition_runtime;if(!runtime||!d.fields.condition_a8||*d.fields.condition_a8!=identity||!d.fields.tested_ac){e="Required SAME native condition arena/a8/ac";return false;}
   NativeConditionDataBorrowV69 value{d.fields.receiver,d.fields.condition_a8,d.fields.tested_ac};return runtime->condition_data_is_true(value,accepted,e);
  };
  services.enabled_event=[](void* raw,bool enabled,std::string& e){auto& d=*static_cast<Delivery*>(raw);return d.self.enabled_event(d.object,d.fields,enabled,e);};
  ObjectEnableConditionBorrowV2 fields_borrow{fields.enabled8a,fields.minimum_ec,fields.disabled_f1,fields.condition_a8,fields.tested_ac};bool enabled{};
  return object_test_enable_condition_v2(fields_borrow,services,mark,enabled,e);
 }
};
static bool bind_campaign_object_loading_v95(const SourceCampaignCandidateBorrowV55& actual,
 const RendererCampaignNonCharacterCompositionV69& composition,const std::shared_ptr<dh2::loader::ModuleRoomZoneConnectionV91>& rooms,
 CampaignObjectLoadingNativeV95 native,dh2::loader::SourceLoadingInputsV43& inputs,std::string& e){
 auto graph=composition.services;auto transport=actual.receiver_transport_v69.lock();std::shared_ptr<SourceWorldBorrowV61> world;
 if(!graph||!graph->current(e)||!transport||!borrow_source_campaign_condition_world_v70(actual,world,e))return false;
 const auto same=[&](const auto& owner){return native.owner&&owner&&!native.owner.owner_before(owner)&&!owner.owner_before(native.owner);};
 if((native.physical_filter&&!native.owner)||same(actual.actual_world)||same(actual.level)||same(actual.objects)){e="Loading native resource leaves require independent authority/weak containing scope";return false;}
 if(!native.module_sync_visibility){const std::weak_ptr<void> weak=actual.actual_world;
  native.module_sync_visibility=[weak](std::uintptr_t id,std::string& e){auto world=weak.lock();if(!world){e="Released SAME Module visibility campaign";return false;}return source_campaign_module_sync_visibility_v94(world,id,e);};
 }
 auto provider=std::make_shared<CampaignObjectLoadingProviderV95>();provider->graph=graph;provider->source=world;provider->native=std::move(native);
 dh2::loader::SourceObjectLoadingLeavesV95 leaves;leaves.owner=provider;leaves.null_handle_assertion=provider->native.null_handle_assertion;
 leaves.current=[provider](std::string& e){std::shared_ptr<RendererCampaignNonCharacterV69> graph;std::shared_ptr<SourceWorldBorrowV61> world;SourceCampaignCandidateBorrowV55 actual;return provider->current(graph,world,actual,e);};
 leaves.test_enable=[provider](const auto& object,bool mark,std::string& e){return provider->test(object,mark,e);};
 return dh2::loader::SourceObjectLoadingV95::bind(actual.objects,transport,rooms,std::move(leaves),inputs,e);
}
