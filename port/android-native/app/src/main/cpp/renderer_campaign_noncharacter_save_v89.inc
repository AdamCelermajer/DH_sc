// Include INSIDE model_renderer namespace AFTER renderer_campaign_noncharacter_v69.inc.
// Outside namespace include source_campaign_save_objects_v86.hpp,
// source_campaign_fx_v77.hpp, source_campaign_module_rooms_v91.hpp and
// noncharacter_save_connection_v89.hpp. Include renderer_campaign_module_rooms_v91.inc
// before this file; it supplies the actual Module seven-byte connection.
using SourceNonCharacterRestoreLenderV89=std::function<bool(std::uintptr_t,dh2::loader::NonCharacterRestoreServicesV89&,std::string&)>;
// Existing Module/Block records belong to ModuleGraph, rather than the
// nonCharacter auxiliary graph. Borrow that SAME actual receiver when needed.
static bool borrow_noncharacter_saved_base_v92(const std::shared_ptr<RendererCampaignNonCharacterV69>& self,
 std::uintptr_t id,std::shared_ptr<void>& pin,dh2::world::CanonicalGameObjectBaseOwnerV1*& base,std::string& e){
 if(!self||!self->current(e))return false;
 std::string auxiliary_error;if(self->borrow(id,pin,base,auxiliary_error)&&pin&&base)return true;
 auto world=self->scope.actual_world.lock();dh2::world::CanonicalModuleV1* module{};
 if(!world||!borrow_source_campaign_module_v91(world,id,pin,module,e)||!pin||!module){
  if(e.empty())e=auxiliary_error.empty()?"Required SAME existing save receiver":auxiliary_error;return false;
 }
 base=&module->base();if(base->identity()!=id){e="Changed SAME Module save receiver identity";return false;}
 return self->current(e);
}
struct NonCharacterGameObjectRestoreContextV89 {std::weak_ptr<RendererCampaignNonCharacterV69> renderer;std::uintptr_t identity{};};
struct NonCharacterSaveDispatchV89 {
 std::weak_ptr<RendererCampaignNonCharacterV69> renderer;
 std::weak_ptr<dh2::loader::SourceReleaseJournalV69> journal;
 std::weak_ptr<void> world;dh2::world::CanonicalObjectBorrowV1 actual;
 std::weak_ptr<dh2::loader::ModuleRoomZoneConnectionV91> rooms;
 SourceNonCharacterRestoreLenderV89 native;
 std::shared_ptr<dh2::loader::NonCharacterSaveConnectionV89> generic; // this SAME per-call context only
 bool connection(std::shared_ptr<dh2::loader::NonCharacterSaveConnectionV89>& out,std::string& e){
  auto self=renderer.lock();auto release=journal.lock();auto actual_world=world.lock();
  if(!self||!release||!actual_world||!self->current(e)){if(e.empty())e="Required SAME non-Character save composition lifetime";return false;}
  dh2::loader::NonCharacterRestoreServicesV89 services;if(native&&!native(actual.identity,services,e))return false;
  if(!services.owner)services.owner=self->files;
  auto independent=[&](const auto& containing){return !containing||services.owner.owner_before(containing)||containing.owner_before(services.owner);};
  if(!independent(actual_world)||!independent(self->scope.level.lock())||!independent(self->scope.objects.lock())){e="Restore service authority must not retain containing World/Level/manager";return false;}const auto identity=actual.identity;auto weak_renderer=renderer;auto weak_journal=journal;
  auto prior_current=services.validate_current;services.validate_current=[weak_renderer,weak_journal,prior_current](std::uintptr_t id,std::string& e){auto self=weak_renderer.lock();auto journal=weak_journal.lock();if(!self||!journal||!self->current(e))return false;if(journal->handles_identity(id)&&!journal->validate_serialization_identity(id,e))return false;return !prior_current||prior_current(id,e);};
  if(!services.level_flags)services.level_flags=[weak_renderer](auto& out,std::string& e){auto self=weak_renderer.lock();auto level=self?self->scope.level.lock():nullptr;if(!self||!level||!self->current(e)){if(e.empty())e="Required actual current Level flags for non-Character restore";return false;}auto fields=level->constructor_borrow_v3();if(!fields.owner||fields.identity!=level->identity()||!fields.fields){e="Required SAME Level constructor flags";return false;}out.owner=fields.owner;out.bytef3=&fields.fields->byte_f3;out.bytef4=&fields.fields->byte_f4;return true;};
  if(!services.gameobject.invoke){auto context=std::make_shared<NonCharacterGameObjectRestoreContextV89>();context->renderer=renderer;context->identity=identity;services.gameobject_context_owner=context;services.gameobject.context=context.get();services.gameobject.invoke=[](void* raw,const auto& request,std::string& e){auto& dispatch=*static_cast<NonCharacterGameObjectRestoreContextV89*>(raw);auto self=dispatch.renderer.lock();std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
    if(request.entry==0x38b954){
     if(!self||!self->current(e)||request.subject!=dispatch.identity){if(e.empty())e="Foreign source GameObject saved-roll assertion receiver";return false;}
     auto process=dh2::world::SourceAssertionProcessV76::borrow();if(!process){e="Required actual process source assertion mode";return false;}
     const auto mode=*process->source_level();if(mode==2){e="Original GameObject.Deserialize deliberate NULL-store assertion: spawnProbRolled == m_spawnProbRolled";return false;}
     if(mode==1)return process->report("..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\GameObject.cpp",0x11e,"spawnProbRolled == m_spawnProbRolled",e);
     e.clear();return true;
    }
    if(request.entry!=0x4713d0){e="Required different original GameObject restore continuation";return false;}
    if(!self||!self->current(e)||request.payload!=dispatch.identity||!borrow_noncharacter_saved_base_v92(self,dispatch.identity,pin,base,e)||!base||!base->pointer(0x2d8)||*base->pointer(0x2d8)!=request.subject){if(e.empty())e="Required SAME actual restored visual2d8";return false;}const char* name=base->class_name20();
    if(name&&(!std::strcmp(name,"Module")||!std::strcmp(name,"Block"))){auto world=self->scope.actual_world.lock();return world&&source_campaign_module_sync_visibility_v94(world,dispatch.identity,e);}
    return self->source_visibility(*base,e);};}
  if(!services.play_animation)services.play_animation=[weak_renderer](std::uintptr_t visual,const char* name,unsigned arg2,unsigned arg3,unsigned arg4,bool& result,std::string& e){auto self=weak_renderer.lock();std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> actual;if(arg3||arg4){e="Unrecovered nonzero native restore animator arguments";return false;}return self&&self->actual_visual(visual,actual,e)&&actual->play(name,arg2!=0,result,e);};
  if(release->handles_identity(identity))return release->save_connection(identity,std::move(services),out,e);
  if(actual.class_name20&&*actual.class_name20&&
   (!std::strcmp(*actual.class_name20,"Module")||!std::strcmp(*actual.class_name20,"Block"))){
   if(generic){out=generic;return true;}auto same_rooms=rooms.lock();
   if(!same_rooms){e="Required actual generated RoomZone sibling for Module OBJS";return false;}
   if(!make_campaign_module_saved_connection_v91(actual_world,identity,same_rooms,std::move(services),generic,e))return false;
   out=generic;return true;
  }
  // Existing Root-owned Module/Block/Config or optional generic family. Only
  // proven GameObject inherited payloads are supplied; unknown stays required
  // at actual Save/Load, never skipped from metadata/checkpoint filtering.
  if(generic){out=generic;return true;}
  dh2::world::CanonicalBaseBorrowV68 borrow=[weak_renderer,identity](auto& pin,auto*& base,std::string& e){auto self=weak_renderer.lock();return self&&self->current(e)&&self->borrow(identity,pin,base,e);};
  std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};if(!borrow(pin,base,e)||!base||!base->class_name20()){if(e.empty())e="Required actual generic sameclass serialization producer";return false;}
  const std::string name=base->class_name20();if(name!="RoomZone"&&name!="ColBox"&&name!="Floor"&&name!="GameObject"&&name!="TriggerTrap"){e="Required native serializer for actual nonjournal class "+name;return false;}
  if(!dh2::loader::make_noncharacter_base_save_v89(std::move(borrow),std::move(services),generic,e))return false;out=generic;return true;
 }
 bool level_config(std::shared_ptr<dh2::world::CanonicalLevelConfigV1>& out,std::string& e){
  SourceCampaignCandidateBorrowV55 current;std::shared_ptr<SourceWorldBorrowV61> scope;
  auto source=world.lock();auto self=renderer.lock();
  if(!source||!self||!self->current(e)||!borrow_source_campaign_candidate_runtime_v61(current,e)||current.actual_world!=source||
     !borrow_source_campaign_condition_world_v70(current,scope,e)||!scope||!scope->stage34_native_v80||!scope->stage34_native_v80->config){
   if(e.empty())e="Required SAME current typed LevelConfig borrower";return false;
  }
  if(!scope->stage34_native_v80->config(actual.identity,out,e)||!out||out->identity()!=actual.identity||
     !actual.lease||actual.lease.owner_before(out)||out.owner_before(actual.lease)||actual.context!=out.get()){
   if(e.empty())e="Replaced actual LevelConfig ObjectBase receiver";return false;
  }return true;
 }
 bool deliver(bool load,dh2::level::SavegameStreamV2& stream,std::string& e){
  if(actual.class_name20&&*actual.class_name20&&!std::strcmp(*actual.class_name20,"LevelConfig")){
   std::shared_ptr<dh2::world::CanonicalLevelConfigV1> config;if(!level_config(config,e))return false;
   dh2::world::ObjectSaveRestoreBorrowV3 fields;fields.identity=actual.identity;
   fields.visible80=config->source_save_byte_v91(0x80);fields.enabled8a=config->source_save_byte_v91(0x8a);
   fields.tested_ac=config->source_save_byte_v91(0xac);fields.tested_d0=config->source_save_byte_v91(0xd0);
   return load?dh2::world::object_base_deserialize_v3(stream,fields,e):dh2::world::object_base_serialize_v3(stream,fields,e);
  }
  std::shared_ptr<dh2::loader::NonCharacterSaveConnectionV89> body;if(!connection(body,e))return false;
  dh2::level::LevelSaveObjectBorrowV2 object;object.identity=reinterpret_cast<const void*>(actual.identity);object.context=this;
  object.is_character=[](void* raw,bool& value,std::string& e){auto& same=*static_cast<NonCharacterSaveDispatchV89*>(raw);std::uintptr_t result{};if(!same.actual.as_character||!same.actual.as_character(same.actual.context,result,e))return false;value=result!=0;return true;};
  object.is_player=[](void*,bool&,std::string& e){e="Native non-Character IsPlayer branch is not reached";return false;};
  if(!body->bind_methods(object,e))return false;return load?object.load(object.context,stream,e):object.save(object.context,stream,e);
 }
};
bool bind_campaign_noncharacter_saved_objects_v89(const SourceCampaignCandidateBorrowV55& actual,
 const RendererCampaignNonCharacterCompositionV69& composition,const std::shared_ptr<dh2::loader::SourceReleaseJournalV69>& journal,
 SourceNonCharacterRestoreLenderV89 native,std::string& e,
 std::weak_ptr<dh2::loader::ModuleRoomZoneConnectionV91> rooms={}){
 if(!actual.actual_world||!composition.services||!journal){e="Required actual V69 composition/journal before OBJS enrollment";return false;}
 std::weak_ptr<void> world=actual.actual_world;std::weak_ptr<RendererCampaignNonCharacterV69> renderer=composition.services;std::weak_ptr<dh2::loader::SourceReleaseJournalV69> weak_journal=journal;
 return bind_source_campaign_noncharacter_saved_objects_v86(actual.actual_world,journal,[world,renderer,weak_journal,rooms,native=std::move(native)](std::uintptr_t id,auto& out,std::string& e){
  auto actual_world=world.lock();auto self=renderer.lock();auto journal=weak_journal.lock();SourceCampaignCandidateBorrowV55 current;
  if(!actual_world||!self||!journal||!self->current(e)||!borrow_source_campaign_candidate_runtime_v61(current,e)||current.actual_world!=actual_world||!current.objects)return false;
  const dh2::world::CanonicalObjectBorrowV1* found{};std::int32_t key{};const dh2::world::CanonicalObjectBorrowV1* object{};bool more=current.objects->source_ordered_begin_v38(key,object);
  while(more){if(object&&object->identity==id){found=object;break;}more=current.objects->source_ordered_next_v38(key,key,object);}if(!found||!found->lease){e="Required SAME actual canonical non-Character map receiver";return false;}
  auto dispatch=std::make_shared<NonCharacterSaveDispatchV89>();dispatch->renderer=renderer;dispatch->journal=weak_journal;dispatch->world=world;dispatch->actual=*found;dispatch->native=native;dispatch->rooms=rooms;
  if(journal->handles_identity(id)){if(!journal->save_header(id,out,e))return false;}
  else if(found->class_name20&&*found->class_name20&&!std::strcmp(*found->class_name20,"LevelConfig")){
   std::shared_ptr<dh2::world::CanonicalLevelConfigV1> config;if(!dispatch->level_config(config,e))return false;
   out={};out.identity=reinterpret_cast<const void*>(id);out.checkpoint28=config->byte(0x28);
   out.gametype48=config->string(0x48);out.map_name=config->string(0x30);out.room64=found->room64;out.disabled81=config->byte(0x81);
  }
  else if(found->class_name20&&*found->class_name20&&
   (!std::strcmp(*found->class_name20,"Module")||!std::strcmp(*found->class_name20,"Block"))){
   std::shared_ptr<void> pin;dh2::world::CanonicalModuleV1* module{};
   if(!borrow_source_campaign_module_v91(actual_world,id,pin,module,e)||!pin||!module||
    found->lease.owner_before(pin)||pin.owner_before(found->lease)||found->context!=&module->base()){
    if(e.empty())e="Required SAME typed Module OBJS header";return false;
   }
   auto& base=module->base();out={};out.identity=reinterpret_cast<const void*>(id);
   out.checkpoint28=base.byte(0x28);out.gametype48=base.string(0x48);out.map_name=base.string(0x30);
   out.room64=found->room64;out.disabled81=base.byte(0x81);
  }
  else {std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};if(!borrow_source_campaign_object_base_v77(current,id,pin,base,e)||!base)return false;out={};out.identity=reinterpret_cast<const void*>(id);out.checkpoint28=base->byte(0x28);out.gametype48=base->string(0x48);out.map_name=base->string(0x30);out.room64=found->room64;out.disabled81=base->byte(0x81);}
  out.context=dispatch.get();out.receiver_lease_v86=dispatch;
  out.is_character=[](void* raw,bool& value,std::string& e){auto& same=*static_cast<NonCharacterSaveDispatchV89*>(raw);std::uintptr_t result{};if(!same.actual.as_character||!same.actual.as_character(same.actual.context,result,e))return false;value=result!=0;return true;};
  out.is_player=[](void*,bool&,std::string& e){e="Native non-Character IsPlayer branch is not reached";return false;};
  out.save=[](void* raw,auto& stream,std::string& e){return static_cast<NonCharacterSaveDispatchV89*>(raw)->deliver(false,stream,e);};out.load=[](void* raw,auto& stream,std::string& e){return static_cast<NonCharacterSaveDispatchV89*>(raw)->deliver(true,stream,e);};e.clear();return true;
 },e);
}
