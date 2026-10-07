// Include inside model_renderer, after renderer_campaign_generated_rooms_v92.inc
// and before renderer_source_boundary_v61.inc. See HANDOFF.md for global includes.
namespace {
struct CampaignGeneratedRoomBehaviorV93 {
 std::weak_ptr<SourceWorldBorrowV61> source;
 std::weak_ptr<dh2::loader::CanonicalLevelContextV1> level;
 std::weak_ptr<dh2::world::CanonicalObjectManagerV1> objects;
 CampaignGeneratedRoomNativeV92 incoming;
 static bool required(std::string& e,const char* leaf){if(e.empty())e=std::string("Required actual Room source behavior: ")+leaf;return false;}
 static bool same(const std::shared_ptr<void>& a,const std::shared_ptr<void>& b){return a&&b&&!a.owner_before(b)&&!b.owner_before(a);}
 bool current(std::shared_ptr<SourceWorldBorrowV61>& world,SourceCampaignCandidateBorrowV55& actual,std::string& e)const{
  world=source.lock();auto expected_level=level.lock();auto expected_objects=objects.lock();
  if(!world||!world->owner||!expected_level||!expected_objects||!borrow_source_campaign_candidate_v55(actual,e)||
   !same(actual.actual_world,world->owner)||actual.level!=expected_level||actual.objects!=expected_objects||
   actual.application!=world->application||!world->canonical_world||world->canonical_world->manager_lease!=actual.objects)
   return required(e,"SAME live World/Level/ObjectManager receipt");
  return true;
 }
 bool actor(std::uintptr_t id,std::shared_ptr<SourceWorldBorrowV61>& world,SourceCampaignCandidateBorrowV55& actual,
  dh2::world::ObjectUpdateActorV102& receiver,bool& character,std::string& e)const{
  character=false;if(!id||!current(world,actual,e))return false;
  //Classification itself must not demand a GameObject field projection: a
  //real Module can be non-zonable and exposes only its selected c4 receiver.
  const dh2::world::CanonicalObjectBorrowV1* entry{};std::int32_t key{};
  bool found=actual.objects->source_ordered_begin_v38(key,entry);
  while(found&&(!entry||entry->identity!=id))found=actual.objects->source_ordered_next_v38(key,key,entry);
  if(!found||!entry)return required(e,"published actual ObjectManager receiver");
  receiver.object=*entry;
  auto& object=receiver.object;std::uintptr_t selected{};
  if(!object.lease||!object.shared_handle||object.identity!=id||!object.as_character||!object.as_character(object.context,selected,e))return false;
  if(selected&&selected!=id)return required(e,"selected Character virtual receiver identity");
  character=selected!=0;return true;
 }
 bool zonable(std::uintptr_t id,bool& out,std::string& e)const{
  std::shared_ptr<SourceWorldBorrowV61> world;SourceCampaignCandidateBorrowV55 actual;dh2::world::ObjectUpdateActorV102 receiver;bool character{};
  if(!actor(id,world,actual,receiver,character,e))return false;
  // Player/Faerie c4 is Main's real derived method. No generic2ed inference.
  const bool okay=character?source_campaign_character_is_zonable_v104(actual.actual_world,id,out,e):
   (incoming.is_zonable&&incoming.is_zonable(id,out,e));
  if(!okay)return required(e,"non-Character selected IsZonable+c4");
  SourceCampaignCandidateBorrowV55 after;return current(world,after,e);
 }
 bool transition(std::uintptr_t id,bool entered,const dh2::world::RoomObjectBorrowV104* room,std::string& e)const{
  std::shared_ptr<SourceWorldBorrowV61> world;SourceCampaignCandidateBorrowV55 actual;dh2::world::ObjectUpdateActorV102 receiver;bool character{};
  if(!actor(id,world,actual,receiver,character,e))return false;
  if(room&&(!room->owner||room->identity!=id))return required(e,"SAME borrowed Room receiver");
  // Activate396d24/DeActivate396918 invoke ZoneEntered/ZoneExited, NOT
  // Enabled/Disabled. Original qualified body itself dispatches c4 and3c.
  bool okay{};
  if(character)okay=entered?source_campaign_character_zone_entered_v102(actual.actual_world,id,e):source_campaign_character_zone_exited_v102(actual.actual_world,id,e);
  else if(room)okay=incoming.zone_transition&&incoming.zone_transition(*room,entered,e);
  else okay=incoming.occupant_transition&&incoming.occupant_transition(id,entered,e);
  if(!okay)return required(e,"non-Character qualified Zone transition with selected c4/3c");
  SourceCampaignCandidateBorrowV55 after;return current(world,after,e);
 }
 struct EnableDelivery {
  std::shared_ptr<SourceWorldBorrowV61> world;
  SourceCampaignCandidateBorrowV55 actual;
  dh2::world::ObjectUpdateActorV102 receiver;
  std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> record;
  dh2::world::ObjectEnableConditionBorrowV2 fields;
  static bool local(void* opaque,bool& present,bool& saved,std::uint8_t& byte14,std::string& e){
   auto& d=*static_cast<EnableDelivery*>(opaque);present=false;saved=false;byte14=0;
   if(!d.world->application||!d.world->player_manager||d.world->player_manager!=d.world->application->source_player_manager_v59())
    return required(e,"SAME App40 PlayerManager for GetLocalPlayer(0,true)");
   dh2::player::PlayerInfoFieldsV1* info{};
   if(!d.world->player_manager->get_local_player(0,true,info,e)||!info)return required(e,"original positive local PlayerInfo assertion");
   if(!info->character660)return true;
   present=true;SourceCampaignCharacterBorrowV62 local;
   if(!borrow_source_campaign_character_v62(d.actual.actual_world,info->character660,local,e)||!local.character||!local.character->actor)return false;
   auto& record=local.character;
   if(!record->save_fields||record->save_fields->character()!=info->character660||!record->save_fields->save_slot14e8())
    return required(e,"SAME local Character.Save14e8 cell");
   if(!*record->save_fields->save_slot14e8())return true; // Genuine NULL Save early return in33e788.
   dh2::character::LootCharacterSaveBorrowV44 borrow;
   if(!record->save_fields->borrow_save(record,borrow,e)||!record->save||borrow.save!=record->save.get()||
    !same(borrow.save_lease,record->save)||*record->save_fields->save_slot14e8()!=reinterpret_cast<std::uintptr_t>(record->save.get()))
    return required(e,"SAME retained local Save14e8 owner");
   saved=true;byte14=borrow.save->source_quest_sync_ready14_v3();return true;
  }
  static bool current_level(void* opaque,bool& present,std::int32_t& difficulty,std::string& e){
   auto& d=*static_cast<EnableDelivery*>(opaque);present=false;difficulty=0;
   dh2::loader::CanonicalCurrentLevelBorrowV1 current;
   if(!borrow_current_native_level_v27(current,e))return false;
   if(!current)return true; // Native absent current-Level branch remains absent.
   if(current.level()!=d.actual.level)return required(e,"SAME actual current Level difficulty118");
   present=true;difficulty=current.level()->constructor_fields_v3().mode118;return true;
  }
  static bool condition(void* opaque,std::uintptr_t id,bool& out,std::string& e){
   auto& d=*static_cast<EnableDelivery*>(opaque);
   if(!d.world->conditions_v70||!d.fields.condition_a8||*d.fields.condition_a8!=id||!d.fields.tested_ac)
    return required(e,"SAME existing condition arena and Character.a8/ac");
   // Whole ConditionData.IsTrue over actual cells, not a new evaluator/table.
   dh2::world::NativeConditionDataBorrowV69 data{d.record,d.fields.condition_a8,d.fields.tested_ac};
   return d.world->conditions_v70->condition_data_is_true(data,out,e);
  }
  static bool enabled(void* opaque,bool value,std::string& e){
   auto& d=*static_cast<EnableDelivery*>(opaque);
   return source_campaign_character_enabled_event_v102(d.actual.actual_world,d.receiver.object.identity,value,e);
  }
 };
 bool test_enable(const dh2::world::CanonicalObjectBorrowV1& object,bool mark,std::string& e)const{
  EnableDelivery d;bool character{};
  if(!actor(object.identity,d.world,d.actual,d.receiver,character,e))return false;
  if(!object.lease||!same(object.lease,d.receiver.object.lease)||object.shared_handle!=d.receiver.object.shared_handle)
   return required(e,"SAME published TestEnableCondition receiver");
  if(!character){if(!incoming.test_enable_condition)return required(e,"actual non-Character TestEnableCondition + selected44/48");
   if(!incoming.test_enable_condition(d.receiver.object,mark,e))return false;
   SourceCampaignCandidateBorrowV55 after;return current(d.world,after,e);
  }
  //Only the reached Character condition body needs the real mutable fields.
  if(!borrow_source_campaign_object_update_actor_v104(d.actual,object.identity,d.receiver,e))return false;
  SourceCampaignCharacterBorrowV62 actual;
  if(!borrow_source_campaign_character_v62(d.actual.actual_world,object.identity,actual,e)||!actual.character||!actual.character->actor)return false;
  d.record=actual.character;
  if(!d.receiver.byte||!d.receiver.integer||!d.receiver.pointer)return required(e,"SAME Character enable field loan");
  d.fields={d.receiver.byte(0x8a),d.receiver.integer(0xec),d.receiver.byte(0xf1),d.receiver.pointer(0xa8),d.receiver.byte(0xac)};
  dh2::world::ObjectEnableConditionServicesV2 services{&d,EnableDelivery::local,EnableDelivery::current_level,EnableDelivery::condition,EnableDelivery::enabled};
  bool enabled{};
  if(!dh2::world::object_test_enable_condition_v2(d.fields,services,mark,enabled,e))return false;
  SourceCampaignCandidateBorrowV55 after;return current(d.world,after,e);
 }
 bool trace(const char* key,std::string& e)const{
  std::shared_ptr<SourceWorldBorrowV61> world;SourceCampaignCandidateBorrowV55 actual;
  if(!key||!current(world,actual,e)||!world->debug||!world->debug_files)return required(e,"fresh actual Room Debug receiver");
  std::uint32_t ignored{};
  if(dh2_character_debug_load(world->debug.get(),world->debug_files)!=1||dh2_character_debug_get(&ignored,world->debug.get(),key,world->debug_files)!=1)
   return required(e,"fresh original Room Debug load/GetSwitch");
  SourceCampaignCandidateBorrowV55 after;return current(world,after,e);
 }
};
}
static bool prepare_campaign_generated_room_behavior_v93(const SourceCampaignCandidateBorrowV55& actual,
 const std::shared_ptr<SourceWorldBorrowV61>& source,CampaignGeneratedRoomNativeV92& native,std::string& e){
 using B=CampaignGeneratedRoomBehaviorV93;
 if(!source||!actual.level||!actual.objects||!B::same(actual.actual_world,source->owner)||!native.owner||
  !native.is_zonable||!native.zone_transition||!native.occupant_transition||!native.room_objects||!native.test_enable_condition){
  return B::required(e,"genuine non-Character c4/Zone/enable virtual dispatch and ObjectManager Add/DelRoomObjects(list80)");
 }
 if(B::same(native.owner,actual.actual_world)||B::same(native.owner,actual.level)||B::same(native.owner,actual.objects)||B::same(native.owner,source))
  return B::required(e,"independent primitive authority without containing World/Level/manager cycle");
 auto state=std::make_shared<B>();state->source=source;state->level=actual.level;state->objects=actual.objects;state->incoming=std::move(native);
 CampaignGeneratedRoomNativeV92 out;out.owner=state;
 out.is_zonable=[state](auto id,bool& value,std::string& e){return state->zonable(id,value,e);};
 out.zone_transition=[state](const auto& object,bool entered,std::string& e){return state->transition(object.identity,entered,&object,e);};
 out.occupant_transition=[state](auto id,bool entered,std::string& e){return state->transition(id,entered,nullptr,e);};
 //Keep exact original list receiver address/iteration ordering. No copied list80.
 out.room_objects=[state](const auto& list,bool active,std::string& e){std::shared_ptr<SourceWorldBorrowV61> world;SourceCampaignCandidateBorrowV55 actual;
  if(!state->current(world,actual,e)||!state->incoming.room_objects(list,active,e))return false;
  SourceCampaignCandidateBorrowV55 after;return state->current(world,after,e);};
 out.trace=[state](const char* key,std::string& e){return state->trace(key,e);};
 out.test_enable_condition=[state](const auto& object,bool mark,std::string& e){return state->test_enable(object,mark,e);};
 out.null_handle_assertion=state->incoming.null_handle_assertion;
 out.destroy_source=state->incoming.destroy_source; //True Room/Zone D1 prefix remains Main, never generic D2 as whole D0.
 native=std::move(out);e.clear();return true;
}
//Enroll AFTER Main installs genuine native behavior provider and BEFORE XML.
//No candidate/Level is captured at World creation; lazy delivery borrows it.
bool install_campaign_generated_room_behavior_v93(const std::shared_ptr<SourceWorldBorrowV61>& source,std::string& e){
 if(!source||!source->owner)return CampaignGeneratedRoomBehaviorV93::required(e,"actual source World enrollment");
 if(!bind_native_generated_room_primitives_v105(source,e))return false;
 auto native=source->generated_room_native_v92;
 if(!native){e.clear();return true;} // Preserve absent provider; V93 startup reports it before XML.
 const auto weak=std::weak_ptr<SourceWorldBorrowV61>(source);
 source->generated_room_native_v92=[weak,native](const SourceCampaignCandidateBorrowV55& actual,CampaignGeneratedRoomNativeV92& out,std::string& e){
  auto source=weak.lock();if(!source||!native)return CampaignGeneratedRoomBehaviorV93::required(e,"Main's genuine Room primitive provider");
  CampaignGeneratedRoomNativeV92 bag;if(!native(actual,bag,e))return false;
  if(!prepare_campaign_generated_room_behavior_v93(actual,source,bag,e))return false;
  out=std::move(bag);return true;
 };
 e.clear();return true;
}


