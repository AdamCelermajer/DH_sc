// Include INSIDE model_renderer after V69 inputs and before V89 environment.
// Global includes: retained_light_resources_v92.hpp, PlayerLightTweakerV90,
// renderer_character_campaign_v62.hpp. SAME Scene and weak campaign authority.
namespace {
struct CampaignLightProviderV92 {
 std::weak_ptr<SourceWorldBorrowV61> world;
 std::shared_ptr<dh2::world::SceneLightResourcesV92> resources;
 bool current(const dh2::loader::LightDeliveryV67& delivery,
  std::shared_ptr<SourceWorldBorrowV61>& out,SourceCampaignCandidateBorrowV55& actual,std::string& e)const{
  out=world.lock();if(!out||!out->application||!out->canonical_world||!resources||!resources->owning_thread()||
   !borrow_source_campaign_candidate_v55(actual,e)||actual.actual_world.get()!=out->owner.get()||actual.actual_world.owner_before(out->owner)||out->owner.owner_before(actual.actual_world)||
   actual.application!=out->application||actual.objects!=delivery.objects||actual.level!=delivery.level||actual.properties!=delivery.properties||
   actual.actual_world.get()!=delivery.actual_world.get()||actual.actual_world.owner_before(delivery.actual_world)||delivery.actual_world.owner_before(actual.actual_world)||
   !actual.roots||actual.roots->source_light_resources_v92()!=resources||!delivery.services_owner){if(e.empty())e="Required SAME current LightPoint App/World/Scene/resource delivery";return false;}return true;
 }
 bool node(const dh2::loader::LightDeliveryV67& d,std::shared_ptr<dh2::world::RetainedLightSceneNodeV92> native,
  dh2::world::LightNodeBorrowV53& out,std::string& e)const{
  std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 actual;if(!current(d,w,actual,e)||!d.receiver||!native||!native->light||native->actor_reference_live){if(e.empty())e="Required unique SAME actor LightNode reference";return false;}
  dh2::world::LightRuntimeStampV67 stamp{reinterpret_cast<std::uintptr_t>(actual.application.get()),reinterpret_cast<std::uintptr_t>(actual.level.get()),reinterpret_cast<std::uintptr_t>(actual.objects.get()),actual.roots->scene_manager_identity_v16(),resources->identity(),resources->generation()};
  dh2::world::LightNodeBorrowV53 result;result.owner=native;result.source_node_v92=native;result.identity=reinterpret_cast<std::uintptr_t>(native.get());result.actual_light_identity=reinterpret_cast<std::uintptr_t>(native->light.get());
  result.actor_reference=std::make_unique<dh2::world::RetainedLightActorReferenceV92>(native,resources,d.services_owner,stamp,d.receiver->identity());out=std::move(result);e.clear();return true;
 }
 bool refresh(const dh2::loader::LightDeliveryV67& d,const dh2::world::LightNodeBorrowV53& node,
  dh2::world::LightSetNameOwnerV3& names,std::string& e)const{
  std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 actual;if(!current(d,w,actual,e)||!d.receiver||&names!=resources->names().get()||!node.source_node_v92)return false;
  auto& receiver=*d.receiver;const auto name=receiver.string(0x30);auto* attached=receiver.source_string_v92(0x180,e);if(!name||!attached){if(e.empty())e="Required SAME LightPoint source name/attachment fields";return false;}
  // Exact strncmp length18, original literals8c7c00 /8c7c18.
  if(std::strncmp(name->c_str(),"_prim_PlayerLight_",18)==0){
   *attached="PlayerCharacter_0";if(!w->player_manager){e="Required actual App40 PlayerManager";return false;}
   dh2::player::PlayerInfoFieldsV1* local{};if(!w->player_manager->get_local_player(0,true,local,e)||!local){if(e.empty())e="Actual GetLocalPlayer0 NULL assertion branch";return false;}
   if(local->character660){SourceCampaignCharacterBorrowV62 character;if(!borrow_source_campaign_character_v62(w->owner,local->character660,character,e)||!character.character||!character.character->actor)return false;*attached=character.character->actor->source_name();}
   // Original AssignTweaker(0,0): SetLight BEFORE App58 slots/color stores.
   if(!resources->set_light(0,0,node.source_node_v92->light,e))return false;
   auto tweaker=w->application->source_tweaker58_v90(0);const auto attenuation=receiver.vector(0x134),ambient=receiver.vector(0x140),diffuse=receiver.vector(0x14c),specular=receiver.vector(0x158);
   if(!tweaker||!attenuation||!ambient||!diffuse||!specular){e="Required SAME App58 light publication/produced source RGB cells";return false;}
   if(!tweaker->source_assign_light_v92(0,receiver.identity(),*attenuation,*ambient,*diffuse,*specular,e))return false;
  }
  if(attached->empty()){e.clear();return true;}auto canonical=receiver.canonical(d.services_owner);if(!canonical.room64){e="Required SAME LightPoint room64";return false;}
  dh2::target_providers::Handle16 handle{};if(!actual.objects->by_name(attached->c_str(),*canonical.room64,false,nullptr,handle,e))return false;
  const dh2::world::CanonicalObjectBorrowV1* object{};if(!actual.objects->resolve_handle_v4(handle,false,object,{},e))return false;
  if(object)receiver.source_attached_handle198_v92()=handle;
  auto active=receiver.byte(0x1b0),added=receiver.byte(0x1b1);if(!active||!added){e="Required produced SAME LightPoint1b0/1b1";return false;}
  if(*active&&!*added){if(!w->debug||!w->debug_files){e="Required reached actual EnablePlayerHeadLight Debug provider";return false;}
   std::uint32_t enabled{};if(dh2_character_debug_get(&enabled,w->debug.get(),"EnablePlayerHeadLight",w->debug_files)!=1){e="Actual EnablePlayerHeadLight Debug.GetSwitch failed";return false;}
   if(!enabled){if(!resources->add_active(handle,node.source_node_v92,e))return false;auto* slot=receiver.source_byte_v92(0x1b1,e);if(!slot)return false;*slot=1;
    receiver.source_attached_handle198_v92()={0,UINT32_MAX,0};} // ObjectHandle(NULL) original40bc94..b4.
  }
  e.clear();return true;
 }
};
}
static bool prepare_campaign_light_resources_v92(const SourceCampaignCandidateBorrowV55& actual,
 const std::weak_ptr<SourceWorldBorrowV61>& weak,RendererCampaignNonCharacterInputsV69& inputs,std::string& e){
 using namespace dh2;using namespace dh2::loader;
 // Publishing this packet admits the complete LightPoint factory before XML.
 // Resource construction alone cannot supply the genuine ObjectBase D0 tail.
 // Leave the optional class unavailable until Main supplies that continuation;
 // non-LightPoint levels must still compose, and reached LightPoint fails via
 // the normal unsupported original-factory path rather than a fake callback.
 if(!inputs.source.lights||!inputs.source.lights->bind_objectbase_dtor){
  inputs.source.lights.reset();e.clear();return true;
 }
 auto world=weak.lock();if(!world||!actual.roots||!world->read||!inputs.source.auxiliary.owner){e="Required actual light resource producer/Scene/file reader";return false;}
 auto provider=std::make_shared<CampaignLightProviderV92>();provider->world=weak;provider->resources=actual.roots->source_light_resources_v92();
 auto lights=inputs.source.lights?std::make_shared<LightEnvironmentLeavesV67>(*inputs.source.lights):std::make_shared<LightEnvironmentLeavesV67>();
 if(!lights->owner)lights->owner=inputs.source.auxiliary.owner;lights->names=provider->resources->names();
 const auto validate=lights->validate_current;lights->validate_current=[provider,validate](const auto& d,std::string& e){std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 current;return provider->current(d,w,current,e)&&(!validate||validate(d,e));};
 if(!lights->construct_light_node)lights->construct_light_node=[provider](const auto& d,bool original,auto& out,std::string& e){
  if(!original){e="Required actual LightBase CLightSceneNode(true) branch";return false;}return provider->node(d,std::make_shared<dh2::world::RetainedLightSceneNodeV92>(),out,e);};
 if(!lights->first_scene_light)lights->first_scene_light=[provider](const auto& d,const std::string& file,const char* xref,bool first,bool second,auto& out,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 actual;if(!provider->current(d,w,actual,e)||!xref||*xref||first||second){if(e.empty())e="Unsupported original LightBase LoadScene parameter domain";return false;}
  std::vector<std::uint8_t> bytes;bool found{};if(!w->read(file,found,bytes,e))return false;if(!found){out={};e.clear();return true;}
  resources::BresView image;if(dh2_bres_open(&image,bytes.data(),bytes.size())!=resources::BresError::ok){e="Required actual LightBase authored BRES";return false;}
  auto prefix=std::make_shared<dh2::world::RetainedLightScenePrefixV92>();if(!dh2::world::RetainedLightSceneV92::load(image,prefix->root,e))return false;
  if(prefix->root->lights188.empty()){out={};e.clear();return true;} // True constructed scene's empty first-light list.
  if(!provider->node(d,prefix->root->lights188.front(),out,e))return false;out.load_prefix_v92=std::move(prefix);return true;
 };
 if(!lights->attach_root)lights->attach_root=[provider](const auto& d,const auto& n,std::string& e){std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 a;return provider->current(d,w,a,e)&&dh2::world::attach_retained_light_root_v92(*a.roots,n,e);};
 if(!lights->add_automatic)lights->add_automatic=[provider](const auto& d,std::uintptr_t id,const auto& n,std::string& e){std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 a;return provider->current(d,w,a,e)&&d.receiver&&d.receiver->identity()==id&&provider->resources->add_automatic(id,n.source_node_v92,e);};
 if(!lights->borrow_light_parameters)lights->borrow_light_parameters=[provider](const auto& d,const auto& n,auto& out,std::string& e){std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 a;return provider->current(d,w,a,e)&&dh2::world::borrow_retained_light_parameters_v92(n,out,e);};
 if(!lights->set_type)lights->set_type=[provider](const auto& d,const auto& n,std::uint16_t type,std::string& e){std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 a;
  if(!provider->current(d,w,a,e)||!n.source_node_v92||n.identity!=reinterpret_cast<std::uintptr_t>(n.source_node_v92.get()))return false;n.source_node_v92->light->type58=type;e.clear();return true;}; //40bcfc stores58 ONLY; no extra recalc.
 if(!lights->refresh_attachment)lights->refresh_attachment=[provider](const auto& d,std::uintptr_t id,const auto& n,auto& names,std::string& e){return d.receiver&&d.receiver->identity()==id&&provider->refresh(d,n,names,e);};
 // Preserve only a genuine incoming SAME actor ObjectBase continuation. Main
 // owns its native condition/handle destruction and real owning-thread lease.
 // Missing D0 remains absent, so CatalogEnvironment cannot fabricate readiness.
 inputs.source.lights=std::move(lights);e.clear();return true;
}

// Public typed bridge to the SAME App tweaker's positive LightBase setter
// branch. Borrow the existing manager entry; no new LightPoint directory/C1.
bool borrow_campaign_light_base_v92(const std::shared_ptr<SourceWorldBorrowV61>& source,std::uintptr_t id,
 dh2::application::TweakLightV90& out,std::string& e){
 SourceCampaignCandidateBorrowV55 actual;if(!source||!id||!borrow_source_campaign_candidate_v55(actual,e)||actual.actual_world.get()!=source->owner.get()||actual.actual_world.owner_before(source->owner)||source->owner.owner_before(actual.actual_world)||!actual.objects){if(e.empty())e="Required SAME current LightBase manager";return false;}
 std::int32_t key{};const dh2::world::CanonicalObjectBorrowV1* object{};
 bool more=actual.objects->source_ordered_begin_v38(key,object);
 for(;more;more=actual.objects->source_ordered_next_v38(key,key,object))if(object&&object->identity==id){
  if(!object->lease||!object->type_f4||*object->type_f4!=19||!object->class_name20||!*object->class_name20||std::strcmp(*object->class_name20,"LightPoint")||object->context!=reinterpret_cast<void*>(id)){e="Unadmitted original LightBase typed receiver domain";return false;}
  auto receiver=std::shared_ptr<dh2::world::CanonicalLightPointV53>(object->lease,static_cast<dh2::world::CanonicalLightPointV53*>(object->context));
  const std::weak_ptr<dh2::world::CanonicalLightPointV53> weak=receiver;const std::weak_ptr<SourceWorldBorrowV61> scope=source;
  const auto setter=[weak,scope,id](std::uint32_t offset){return [weak,scope,id,offset](const std::array<float,3>& value,std::string& e){auto receiver=weak.lock();auto source=scope.lock();SourceCampaignCandidateBorrowV55 actual;
   if(!receiver||!source||!borrow_source_campaign_candidate_v55(actual,e)||actual.actual_world.get()!=source->owner.get()||actual.actual_world.owner_before(source->owner)||source->owner.owner_before(actual.actual_world)||receiver->identity()!=id){if(e.empty())e="Retired actual App LightBase setter receiver";return false;}return receiver->source_set_light_vector_v92(offset,value,e);};};
  dh2::application::TweakLightV90 value;value.owner=receiver;value.identity=id;value.attenuation=setter(0x134);value.ambient=setter(0x140);value.diffuse=setter(0x14c);value.specular=setter(0x158);out=std::move(value);e.clear();return true;
 }
 e="App tweaker LightBase ID is not published on SAME manager";return false;
}
// Metadata only: Main supplies/holds the genuine owning-thread barrier and
// qualified ObjectBase continuation; this never invents or issues a lease.
bool borrow_campaign_light_stamp_v92(const SourceCampaignCandidateBorrowV55& actual,
 dh2::world::LightRuntimeStampV67& out,std::string& e){
 if(!actual.application||!actual.level||!actual.objects||!actual.roots){e="Required actual light runtime owners";return false;}auto resources=actual.roots->source_light_resources_v92();if(!resources||!resources->owning_thread()){e="Light Scene is not on its actual owning thread";return false;}
 out={reinterpret_cast<std::uintptr_t>(actual.application.get()),reinterpret_cast<std::uintptr_t>(actual.level.get()),reinterpret_cast<std::uintptr_t>(actual.objects.get()),actual.roots->scene_manager_identity_v16(),resources->identity(),resources->generation()};e.clear();return true;
}
