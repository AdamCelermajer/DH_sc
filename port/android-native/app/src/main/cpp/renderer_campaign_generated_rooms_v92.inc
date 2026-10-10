// Include inside model_renderer AFTER V69 and module rooms V91. Global
// includes: canonical_character_candidate_v60.hpp, renderer_character_campaign_v62.hpp,
// source_campaign_runtime_v61.hpp, source_campaign_object_update_actor_v104.hpp,
// renderer_campaign_generated_rooms_v92.hpp, module_room_zone_connection_v91.hpp.
// Main owns actual virtual behavior. This owner assembles its supplied leaves
// with the existing current object/Module/PM/camera/manager resource borrowers.
// Native behavior receipt is public in renderer_campaign_generated_rooms_v92.hpp.
struct CampaignGeneratedRoomProviderV92 {
 std::weak_ptr<RendererCampaignNonCharacterV69> graph;
 std::weak_ptr<SourceWorldBorrowV61> world;
 std::weak_ptr<dh2::loader::ModuleRoomZoneConnectionV91> rooms;
 CampaignGeneratedRoomNativeV92 native;
 bool current(std::shared_ptr<RendererCampaignNonCharacterV69>& out,
  std::shared_ptr<SourceWorldBorrowV61>& source,SourceCampaignCandidateBorrowV55& actual,std::string& e)const{
  out=graph.lock();source=world.lock();
  if(!out||!source||!out->current(e)||!borrow_source_campaign_candidate_v55(actual,e)||
   actual.actual_world.get()!=source->owner.get()||actual.actual_world.owner_before(source->owner)||source->owner.owner_before(actual.actual_world)){
   if(e.empty())e="Required SAME generated-room native campaign";return false;
  }return true;
 }
 bool object(std::uintptr_t id,dh2::world::RoomObjectBorrowV104& out,std::string& e)const{
  std::shared_ptr<RendererCampaignNonCharacterV69> graph;std::shared_ptr<SourceWorldBorrowV61> source;SourceCampaignCandidateBorrowV55 actual;
  if(!id||!current(graph,source,actual,e))return false;
  dh2::world::CanonicalObjectBorrowV1 canonical;bool found=false;
  std::int32_t key{};const dh2::world::CanonicalObjectBorrowV1* entry{};
  for(bool more=actual.objects->source_ordered_begin_v38(key,entry);more;more=actual.objects->source_ordered_next_v38(key,key,entry)){
   if(entry&&entry->identity==id){canonical=*entry;found=true;break;}
  }
  if(!found||!canonical.lease||!canonical.type_f4){e="RoomZone object is not published on SAME manager";return false;}
  dh2::world::RoomObjectBorrowV104 value;value.owner=canonical.lease;value.identity=id;
  const auto method=native.is_zonable;const auto scope=world;const auto graph_scope=this->graph;
  value.is_zonable_c4=[scope,graph_scope,method,id](bool& value,std::string& e){auto source=scope.lock();auto graph=graph_scope.lock();SourceCampaignCandidateBorrowV55 actual;
   if(!source||!graph||!graph->current(e)||!method||!borrow_source_campaign_candidate_v55(actual,e)||actual.actual_world.get()!=source->owner.get()||
    actual.actual_world.owner_before(source->owner)||source->owner.owner_before(actual.actual_world)){
    if(e.empty())e="Required actual current IsZonable virtualc4";return false;
   }return method(id,value,e)&&graph->current(e);
  };
  if(*canonical.type_f4==0){
   // Main's actual Character receipt supplies current source_position160_v7
   // and its real derived IsZonable c4 (Player/Faerie exceptions included).
   // It is defined in source_campaign_object_update_actor_v104.cpp, not FSM.
   if(!borrow_source_campaign_character_room_v104(actual,id,value,e)||!value.owner||value.identity!=id)return false;
  }else if(*canonical.type_f4==5){
   // The SAME Module graph owns this class's base and visual, independently
   // of V69. Positive inherited IsZonable still needs its actual room fields.
   std::shared_ptr<void> pin;dh2::world::CanonicalModuleV1* receiver{};
   if(!borrow_source_campaign_module_v91(actual.actual_world,id,pin,receiver,e)||!pin||!receiver||receiver->base().identity()!=id||
     pin.owner_before(canonical.lease)||canonical.lease.owner_before(pin)){if(e.empty())e="Required SAME published Module room receiver";return false;}
   auto& base=receiver->base();value.owner=pin;value.position160=base.vector3(0x160);value.room2f4=base.pointer(0x2f4);value.initial_room2ef=base.byte(0x2ef);
  }else{
   std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};std::string missing;
   if(graph->borrow(id,pin,base,missing)&&base){value.owner=pin;value.position160=base->vector3(0x160);value.room2f4=base->pointer(0x2f4);value.initial_room2ef=base->byte(0x2ef);}
   // The selected virtualc4 runs before room fields are required. A positive
   // result requires these genuine fields; no origin/room is manufactured.
  }
  out=std::move(value);e.clear();return true;
 }
};
static bool bind_campaign_generated_rooms_complete_v92(const SourceCampaignCandidateBorrowV55& actual,
 const RendererCampaignNonCharacterCompositionV69& composition,const std::weak_ptr<SourceWorldBorrowV61>& world,
 CampaignGeneratedRoomNativeV92 native,const std::shared_ptr<dh2::loader::SourceReleaseJournalV69>& journal,
 std::shared_ptr<dh2::loader::ModuleRoomZoneConnectionV91>& sibling,std::string& e){
 using namespace dh2;using namespace dh2::world;using namespace dh2::loader;
 auto source=world.lock();auto graph=composition.services;
 if(!source||!graph||!graph->current(e)||!native.owner||!native.is_zonable||!native.zone_transition||!native.occupant_transition||
  !native.room_objects||!native.trace||!native.test_enable_condition||!journal||sibling){
  if(e.empty())e="Required complete actual RoomZone virtual/diagnostic runtime leaves";return false;
 }
 const auto same=[](const auto& a,const auto& b){return a&&b&&!a.owner_before(b)&&!b.owner_before(a);};
 if(same(native.owner,actual.actual_world)||same(native.owner,actual.level)||same(native.owner,actual.objects)){
  e="RoomZone native leaves must not own containing World/Level/manager";return false;
 }
 auto provider=std::make_shared<CampaignGeneratedRoomProviderV92>();provider->graph=graph;provider->world=world;provider->native=std::move(native);
 ModuleRoomZoneLeavesV91 leaves;leaves.owner=graph;
 leaves.test_enable_condition=provider->native.test_enable_condition;leaves.null_handle_assertion=provider->native.null_handle_assertion;
 leaves.destroy_source=provider->native.destroy_source; // absence remains reached genuine D0 dependency
 leaves.lend=[provider](const std::shared_ptr<CanonicalRoomZoneRecordV3>& record,RoomZoneServicesV3& out,std::string& e){
  std::shared_ptr<RendererCampaignNonCharacterV69> graph;std::shared_ptr<SourceWorldBorrowV61> source;SourceCampaignCandidateBorrowV55 actual;
  if(!record||!provider->current(graph,source,actual,e))return false;
  const std::weak_ptr<CanonicalRoomZoneRecordV3> weak=record;const auto scope=provider->graph;
  auto get=[weak,scope](std::shared_ptr<void>& pin,CanonicalGameObjectBaseOwnerV1*& base,std::string& e){auto graph=scope.lock();auto record=weak.lock();
   if(!graph||!graph->current(e)||!record||!record->receiver){if(e.empty())e="Unproduced SAME generated RoomZone C1";return false;}
   pin=record;base=&record->receiver->base();return true;
  };
  out={};out.game_object.owner=graph->files;out.position.owner=graph->files;
  graph->bind_container_initialization_v89(get,out.game_object);
  const auto bind=[scope,get](auto member){return [scope,get,member](auto&&... args){auto tuple=std::forward_as_tuple(args...);auto& error=std::get<sizeof...(args)-1>(tuple);
   auto graph=scope.lock();std::shared_ptr<void> pin;CanonicalGameObjectBaseOwnerV1* base{};RendererCampaignNonCharacterV69::Bound* bound{};
   if(!graph||!get(pin,base,error)||!graph->ensure(*base,bound,error))return false;
   const auto& callback=bound->position.*member;if(!callback){error="Required reached actual generated RoomZone SetPosition leaf";return false;}
   return callback(std::forward<decltype(args)>(args)...);
  };};
  out.position.attached_position=bind(&GameObjectSetPositionServicesV2::attached_position);
  out.position.physical_position=bind(&GameObjectSetPositionServicesV2::physical_position);
  out.position.visual_sync=bind(&GameObjectSetPositionServicesV2::visual_sync);
  e.clear();return true;
 };
 leaves.enroll=[provider](const std::shared_ptr<CanonicalRoomZoneRecordV3>& record,std::string& e){
  std::shared_ptr<RendererCampaignNonCharacterV69> graph;std::shared_ptr<SourceWorldBorrowV61> source;SourceCampaignCandidateBorrowV55 actual;
  if(!provider->current(graph,source,actual,e)||!record||!record->constructor_completed_v91||!record->receiver){if(e.empty())e="Required completed SAME generated RoomZone C1 enrollment";return false;}
  const auto id=record->receiver->base().identity();const auto scope=provider->graph;const std::weak_ptr<CanonicalRoomZoneRecordV3> weak=record;
  CanonicalBaseBorrowV68 borrow=[scope,weak,id](auto& pin,auto*& base,std::string& e){auto graph=scope.lock();auto record=weak.lock();
   if(!graph||!graph->current(e)||!record||!record->receiver||record->receiver->base().identity()!=id){if(e.empty())e="Released SAME generated RoomZone base";return false;}
   pin=record;base=&record->receiver->base();return true;
  };
  graph->borrows.push_back(borrow);return graph->graph->observe_base_v78(id,std::move(borrow),e);
 };
 leaves.lend_runtime_v104=[provider](const std::shared_ptr<CanonicalRoomZoneRecordV3>& record,RoomZoneRuntimeServicesV104& out,std::string& e){
  std::shared_ptr<RendererCampaignNonCharacterV69> graph;std::shared_ptr<SourceWorldBorrowV61> source;SourceCampaignCandidateBorrowV55 actual;
  if(!record||!record->receiver||!provider->current(graph,source,actual,e))return false;
  out={};out.owner=graph->files;out.trace=provider->native.trace;out.zone_transition=provider->native.zone_transition;
  out.occupant_transition=provider->native.occupant_transition;out.room_objects=provider->native.room_objects;
  out.object=[provider](auto id,auto& object,std::string& e){return provider->object(id,object,e);};
  out.remove_from_room=[provider](auto id,auto object,std::string& e){auto rooms=provider->rooms.lock();if(!rooms){e="Required live SAME generated RoomZone RemoveObject owner";return false;}return rooms->remove_object(id,object,e);};
  out.camera_planes=[provider](auto& planes,std::string& e){std::shared_ptr<RendererCampaignNonCharacterV69> graph;std::shared_ptr<SourceWorldBorrowV61> source;SourceCampaignCandidateBorrowV55 actual;
   return provider->current(graph,source,actual,e)&&borrow_source_campaign_room_camera_v104(source->owner,planes,e);
  };
  out.local_player_position=[provider](auto& position,bool& present,std::string& e){std::shared_ptr<RendererCampaignNonCharacterV69> graph;std::shared_ptr<SourceWorldBorrowV61> source;SourceCampaignCandidateBorrowV55 actual;
   present=false;if(!provider->current(graph,source,actual,e))return false;
   if(!source->player_manager){e="Required SAME App40 PlayerManager for RoomZone visit update";return false;}
   dh2::player::PlayerInfoFieldsV1* player{};if(!source->player_manager->get_local_player(0,true,player,e)||!player){if(e.empty())e="Required original local PlayerInfo0 assertion branch";return false;}
   if(!player->character660){e.clear();return true;}
   SourceCampaignCharacterBorrowV62 character;if(!borrow_source_campaign_character_v62(source->owner,player->character660,character,e)||!character.character||!character.character->actor)return false;
   const auto* same=character.character->actor->source_position160_v7();if(!same){e="Required actual local Character position160";return false;}
   std::copy_n(same,3,position.begin());present=true;e.clear();return true;
  };
  out.count_visible_room=[provider](std::string& e){std::shared_ptr<RendererCampaignNonCharacterV69> graph;std::shared_ptr<SourceWorldBorrowV61> source;SourceCampaignCandidateBorrowV55 actual;
   if(!provider->current(graph,source,actual,e))return false;++actual.objects->source_visible_rooms_f8_v104();e.clear();return true;
  };
  // Connector installs the actual ModuleGraph3fc borrower once, after this.
  e.clear();return true;
 };
 ScopeV67 scope{actual.actual_world,actual.level,actual.objects,actual.properties,graph};
 const bool connected=bind_campaign_generated_module_rooms_v91(actual,std::move(scope),std::move(leaves),journal,sibling,e);
 provider->rooms=sibling; // preserve a failed connector prefix, never fresh C1 retry
 return connected;
}
