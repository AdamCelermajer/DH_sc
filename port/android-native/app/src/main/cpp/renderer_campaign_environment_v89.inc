// Include in model_renderer namespace after V69, Arrays and public helpers.
namespace {
struct CampaignEnvironmentNameSnapshotV89 {
 std::vector<std::uint8_t> design_source,sound_source;
 std::vector<std::vector<std::string>> design_groups,sound_groups;
};
struct CampaignEnvironmentNamesV89 {
 std::weak_ptr<SourceWorldBorrowV61> world;
 std::weak_ptr<void> provider;
};
struct CampaignFramePinV89 {
 std::shared_ptr<SourceWorldBorrowV61> world;
 dh2::loader::GameObjectSourceFrameServicesV74 previous;
};
bool read_campaign_name_groups_v89(const std::shared_ptr<SourceWorldBorrowV61>& world,
 const char* uri,std::size_t expected,std::vector<std::uint8_t>& bytes,
 std::vector<std::vector<std::string>>& groups,std::string& e){
 bool found{};if(!world->read||!world->read(uri,found,bytes,e))return false;
 if(!found){e=std::string("Required original registered names resource ")+uri;return false;}
 std::size_t cursor{};
 const auto word=[&](std::uint32_t& value){if(cursor>bytes.size()||bytes.size()-cursor<4){e="Short original Arrays names word";return false;}
  value=std::uint32_t(bytes[cursor])|(std::uint32_t(bytes[cursor+1])<<8)|(std::uint32_t(bytes[cursor+2])<<16)|(std::uint32_t(bytes[cursor+3])<<24);cursor+=4;return true;};
 groups.resize(expected);
 for(auto& group:groups){std::uint32_t count{};if(!word(count)||count>(bytes.size()-cursor)/4){if(e.empty())e="Invalid original Arrays names count";return false;}
  group.reserve(count);for(std::uint32_t row=0;row<count;++row){std::uint32_t size{};if(!word(size)||size>bytes.size()-cursor){if(e.empty())e="Short original Arrays name CString";return false;}
   group.emplace_back(reinterpret_cast<const char*>(bytes.data()+cursor),size);cursor+=size;}}
 if(cursor!=bytes.size()){e="Unexpected original registered names stream tail";return false;}return true;
}
bool current_campaign_environment_v89(const std::weak_ptr<SourceWorldBorrowV61>& weak,
 std::shared_ptr<SourceWorldBorrowV61>& world,std::string& e){
 world=weak.lock();SourceCampaignCandidateBorrowV55 actual;
 if(!world||!borrow_source_campaign_candidate_v55(actual,e)||actual.actual_world.get()!=world->owner.get()||
    actual.actual_world.owner_before(world->owner)||world->owner.owner_before(actual.actual_world)||actual.application!=world->application){
  if(e.empty())e="Required SAME current environmental World/App source owner";return false;}return true;
}
}
static bool prepare_campaign_environment_v89(const SourceCampaignCandidateBorrowV55& actual,
 RendererCampaignNonCharacterInputsV69& inputs,std::string& e){
 using namespace dh2;using namespace dh2::loader;
 std::shared_ptr<SourceWorldBorrowV61> scope_world;
 if(!borrow_source_campaign_condition_world_v70(actual,scope_world,e)||!scope_world||!scope_world->application||!scope_world->canonical_world||!actual.roots||
    actual.roots!=scope_world->canonical_world->scene_roots_v20||actual.objects!=scope_world->canonical_world->manager_lease||
    !inputs.source.auxiliary.owner){if(e.empty())e="Required actual environment and independent producer authority";return false;}
 const std::weak_ptr<SourceWorldBorrowV61> weak=scope_world;
 auto names=std::make_shared<CampaignEnvironmentNamesV89>();names->world=scope_world;names->provider=inputs.source.auxiliary.owner;
 // Reuse the existing process immutable Arrays authority. Each name vector
 // aliases this ONE decoded snapshot, independent of Level and World lifetime.
 // Registration order is Design/GameDifficulties/GameOptions and
 // CharSounds/Listeners/SoundGroups/VolumeGroups/Sounds.
 if(!inputs.source.auxiliary.difficulty_names||!inputs.source.auxiliary.sound_names){
  auto tables=renderer_immutable_tables_v55;
  if(!tables){e="Required existing process immutable Arrays authority";return false;}
  if(!tables->campaign_names_v89){
   if(tables->campaign_names_attempted_v89){
    e=tables->campaign_names_failure_v89.empty()?"Incomplete original names snapshot":tables->campaign_names_failure_v89;return false;
   }
   tables->campaign_names_attempted_v89=true;
   auto snapshot=std::make_shared<CampaignEnvironmentNameSnapshotV89>();
   if(!read_campaign_name_groups_v89(scope_world,"data/pydata/design_pyarraynames.bin",3,snapshot->design_source,snapshot->design_groups,e)||
      !read_campaign_name_groups_v89(scope_world,"data/pydata/sounds_pyarraynames.bin",5,snapshot->sound_source,snapshot->sound_groups,e)){
    tables->campaign_names_failure_v89=e;return false;
   }
   tables->campaign_names_v89=std::move(snapshot);
  }
  auto snapshot=tables->campaign_names_v89;
  if(!inputs.source.auxiliary.difficulty_names)
   inputs.source.auxiliary.difficulty_names=std::shared_ptr<const std::vector<std::string>>(snapshot,&snapshot->design_groups[1]);
  if(!inputs.source.auxiliary.sound_names)
   inputs.source.auxiliary.sound_names=std::shared_ptr<const std::vector<std::string>>(snapshot,&snapshot->sound_groups[4]);
 }
 // ONE original name facet on SAME native scene/LightSetManager. Any later
 // complete LightSet implementation must adopt these exact fields, not clone.
 const auto light_names=actual.roots->source_light_names_v89();
 if(inputs.light_names&&(inputs.light_names.get()!=light_names.get()||inputs.light_names.owner_before(light_names)||light_names.owner_before(inputs.light_names))){e="Competing source LightSet name owner";return false;}
 inputs.light_names=light_names;
 if(!inputs.source.lights&&!prepare_source_campaign_light_environment_v113(actual,inputs.source.lights,e))return false;
 if(inputs.source.lights){
 auto lights=std::make_shared<LightEnvironmentLeavesV67>(*inputs.source.lights);
 if(!lights->owner)lights->owner=inputs.source.auxiliary.owner;
 if(lights->names&&(lights->names.get()!=light_names.get()||lights->names.owner_before(light_names)||light_names.owner_before(lights->names))){e="LightPoint leaves replaced actual LightSet names";return false;}
 lights->names=light_names;
 const auto previous_validate=lights->validate_current;
 lights->validate_current=[weak,previous_validate](const LightDeliveryV67& delivery,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 now;
  if(!current_campaign_environment_v89(weak,w,e)||!borrow_source_campaign_candidate_v55(now,e)||delivery.actual_world.get()!=now.actual_world.get()||
     delivery.actual_world.owner_before(now.actual_world)||now.actual_world.owner_before(delivery.actual_world)||delivery.level!=now.level||delivery.objects!=now.objects||delivery.properties!=now.properties){if(e.empty())e="Foreign LightPoint delivery";return false;}
  return !previous_validate||previous_validate(delivery,e);
 };
 if(!lights->debug_switch)lights->debug_switch=[weak](const LightDeliveryV67&,const char* name,bool& value,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> w;if(!current_campaign_environment_v89(weak,w,e)||!w->debug||!w->debug_files||!name)return false;
  std::uint32_t original{};const auto status=dh2_character_debug_get(&original,w->debug.get(),name,w->debug_files);
  if(status!=1){e="Actual LightPoint Debug.GetSwitch failed";return false;}value=original!=0;return true;
 };
 inputs.source.lights=std::move(lights); // journal hooks are appended NEXT.
 } // A missing genuine LightPoint node/provider packet stays NULL.
 if(!inputs.failed_spawn_mark)inputs.failed_spawn_mark=[weak](world::CanonicalGameObjectBaseOwnerV1& base,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> w;if(!current_campaign_environment_v89(weak,w,e)||!w->canonical_world)return false;
  return w->canonical_world->manager.source_mark_for_deletion_v89(base.identity(),e);
 };
 auto previous_frame=inputs.lend_frame_services;
 inputs.lend_frame_services=[weak,names,previous_frame](world::CanonicalGameObjectBaseOwnerV1& base,GameObjectSourceFrameServicesV74& out,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> w;if(!current_campaign_environment_v89(weak,w,e))return false;
  auto pin=std::make_shared<CampaignFramePinV89>();pin->world=w;
  if(previous_frame&&!previous_frame(base,pin->previous,e))return false;
  auto provider=names->provider.lock();if(!provider){e="Released independent environment provider";return false;}
  out=pin->previous;out.owner=provider;
  out.prefix.provider_lease=pin;out.prefix.context=pin.get();
  if(!out.prefix.debug){if(!w->debug||!w->debug_files){e="Required actual process Debug/file services for GameObject.Update";return false;}out.prefix.debug=w->debug.get();out.prefix.files=*w->debug_files;}
  out.prefix.manager38=[](void* raw,world::CanonicalObjectManagerV1*& out,std::string& e){
   auto& pin=*static_cast<CampaignFramePinV89*>(raw);auto w=pin.world;
   if(!w||!w->canonical_world){e="Released actual App38 frame receiver";return false;}
   if(pin.previous.prefix.manager38&&!pin.previous.prefix.manager38(pin.previous.prefix.context,out,e))return false;
   auto* actual=&w->canonical_world->manager;
   if(pin.previous.prefix.manager38&&out!=actual){e="Frame prefix replaced SAME App38 manager";return false;}out=actual;return true;
  };
  out.online.context=pin.get();
  out.online.online_byte5=[](void* raw,bool& out,std::string& e){auto& pin=*static_cast<CampaignFramePinV89*>(raw);
   if(pin.previous.online.online_byte5)return pin.previous.online.online_byte5(pin.previous.online.context,out,e);
   if(!pin.world||!pin.world->application){e="Required actual App GetOnline frame receiver";return false;}
   auto online=pin.world->application->get_online_loading_v55();if(!online){e="Required actual GetOnline owner";return false;}out=online->byte5()!=0;return true;
  };
  out.online.hosting=[](void* raw,bool& out,std::string& e){auto& pin=*static_cast<CampaignFramePinV89*>(raw);if(!pin.previous.online.hosting){e="Required reached actual online IsHosting";return false;}return pin.previous.online.hosting(pin.previous.online.context,out,e);};
  out.online.not_owned_virtual54=[](void* raw,std::uintptr_t id,bool& out,std::string& e){auto& pin=*static_cast<CampaignFramePinV89*>(raw);if(!pin.previous.online.not_owned_virtual54){e="Required reached SAME native network ownership virtual54";return false;}return pin.previous.online.not_owned_virtual54(pin.previous.online.context,id,out,e);};
  if(!out.idle_sound)out.idle_sound=[weak](world::CanonicalGameObjectBaseOwnerV1& actual,std::int16_t sound,std::string& e){
   std::shared_ptr<SourceWorldBorrowV61> w;if(!current_campaign_environment_v89(weak,w,e))return false;
   auto play=bind_container_play_v46(w->owner,actual.identity(),[sound](int& out,std::string& e){out=sound;e.clear();return true;},
    [&actual](std::array<float,3>& out,std::string& e){auto* position=actual.vector3(0x160);if(!position){e="Required actual GameObject160 idle sound position";return false;}out={position[0],position[1],position[2]};return true;});
   return play&&play(e);
  };
  e.clear();return true;
 };
 if(!inputs.placement.provider)inputs.placement.provider=inputs.source.auxiliary.owner;
 if(!inputs.placement.set_initial_position160)inputs.placement.set_initial_position160=[weak](std::uintptr_t id,std::string& e){std::shared_ptr<SourceWorldBorrowV61> w;return current_campaign_environment_v89(weak,w,e)&&source_campaign_character_set_initial_position160_v68(w->owner,id,e);};
 if(!inputs.placement.save_entrypoint)inputs.placement.save_entrypoint=[weak](std::uintptr_t id,std::int32_t value,std::int32_t difficulty,std::string& e){std::shared_ptr<SourceWorldBorrowV61> w;return current_campaign_environment_v89(weak,w,e)&&source_campaign_character_save_entrypoint_v68(w->owner,id,value,difficulty,e);};
 // SAME actual SpawnPoint borrowing is installed by V69 catalog composition.
 // Physics/visual policy facets remain the genuine native class lenders.
 e.clear();return true;
}
