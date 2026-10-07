// Include after WorldScriptContext and before renderer load_world methods.
// Add WorldScriptContext member: shared_ptr<PlayerCanonicalMetadataLiveV4>
// player_canonical; (forward declare this struct before WorldScriptContext).
// Caller retains its sole CanonicalObjectManager/PropertyMap/publication owner.
struct PlayerCanonicalMetadataLiveV4 {
 dh2::player::PlayerSpawnMetadataV4 spawn;
 // These are only previously missing canonical metadata fields. Actual player
 // object, runtime, properties, life, target and FSM remain borrowed.
 dh2::target_providers::Handle16 handle{0,UINT32_MAX,0};
 const std::uint32_t type_f4{0};
 std::int32_t room64{-1};
 // Source Character C1 fields, retained once; only genuine InitPre/OOI
 // producers may publish later values. Never derive these from a class row.
 std::int16_t properties13c8{-1};
 std::uintptr_t ooi14a4{};
 dh2::character::CharacterKillFieldsV21 kill_fields;
 // SAME missing player GameObject fields, C1 explicit stores once. Restore
 // retains this owner; no frame/refresh/default replay resets these bytes.
 std::uint8_t disabled81{},source_static84{};
 std::shared_ptr<dh2::character::LootPlayerFieldAssociationV47> loot_fields_v49;
 std::unique_ptr<dh2::character::PlayerSourceVisibilityV49> source_visibility_v49;
 // Sole inherited Character pointer slots. Fresh metadata C1 initializes
 // only these pointers; the already-live runtime is never reconstructed.
 dh2::character::CharacterPositionFieldsV7 position_fields;
 const char* class_name20{};
 std::string name,archetype;
 std::unique_ptr<dh2::world::CanonicalPlayerFacetV3> facet;
 bool bound{};
 static std::shared_ptr<PlayerCanonicalMetadataLiveV4> prepare(
   std::int32_t actual_internal_input,std::string& error){
  auto out=std::make_shared<PlayerCanonicalMetadataLiveV4>();
  if(!dh2::player::player_spawn_metadata_v4(actual_internal_input,out->spawn,error))return {};
  if(!out->position_fields.adopt(0,0,error))return {};
  if(!out->kill_fields.construct_fresh(error))return {};
  out->name=out->spawn.name;out->archetype=out->spawn.archetype;
  out->class_name20=out->spawn.factory->name;out->room64=out->spawn.room;return out;
 }
 dh2::character::CharacterPositionFieldsV7& position_fields_v7()noexcept{return position_fields;}
 const dh2::character::CharacterPositionFieldsV7& position_fields_v7()const noexcept{return position_fields;}
 // Call only after actual SetPhysical/NativeBody creation, with that SAME
 // receiver address. Passing zero is reserved for its real teardown store.
 bool publish_physical2dc_v20(std::uintptr_t actual_physical,std::string& error){
  if(!position_fields.constructed){error="Required produced player Character pointer fields";return false;}
  position_fields.physical2dc=actual_physical;return true;
 }
 bool bind(const std::shared_ptr<PlayerCanonicalMetadataLiveV4>& self,
   const std::shared_ptr<dh2::character::ScriptCharacterObject>& object,
   dh2::actor::RuntimeState& same_runtime,std::shared_ptr<void> world,
   const std::uint8_t* actual_across_rooms87,std::string& error){
  if(bound||!self||self.get()!=this||!object||object->name!=name){
   error="Required first same-player source-name canonical metadata binding";return false;
  }
  bound=true;handle.cached=object->identity;
  dh2::world::CanonicalPlayerFieldsV3 fields;
  fields.object=object;fields.runtime=&same_runtime;fields.handle=&handle;
  fields.type_f4=&type_f4;fields.room64=&room64;fields.across_rooms87=actual_across_rooms87;
  fields.class_name20=&class_name20;fields.name=&name;fields.archetype=&archetype;
  fields.world_lease=std::move(world);
  facet=std::make_unique<dh2::world::CanonicalPlayerFacetV3>(std::move(fields));return true;
 }
 bool publish(const std::shared_ptr<PlayerCanonicalMetadataLiveV4>& self,
   dh2::world::CanonicalExistingActorPublicationV3& publication,std::string& error){
  if(!facet){error="Required bound same-player canonical metadata facet";return false;}
  dh2::world::CanonicalExistingActorV3 borrow;
  if(!facet->borrow(self,borrow,error))return false;
  dh2::target_providers::Handle16 result{};
  return publication.publish(borrow,name.c_str(),archetype.c_str(),room64,spawn.network,result,error);
 }
 // Stop queries and destroy manager/receiver publication first. This breaks
 // the explicit World pin only after no manager callback can use the facet.
 void release_after_unpublication()noexcept{facet.reset();}
};
