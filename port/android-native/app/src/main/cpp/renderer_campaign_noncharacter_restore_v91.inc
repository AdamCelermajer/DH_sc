// Include INSIDE model_renderer after renderer_campaign_noncharacter_v69.inc
// and renderer_campaign_noncharacter_save_v89.inc (its EXACT lender typedef).
// Global headers: canonical_character_candidate_v60.hpp, native_conditions_v69.hpp,
// application_player_manager_bootstrap_v59.hpp, renderer_character_campaign_v62.hpp,
// production_noncharacter_composition_v67.hpp and noncharacter_save_connection_v89.hpp.
namespace {
template<class Record>bool borrow_restore_record_v91(
 const std::shared_ptr<RendererCampaignNonCharacterV69>& self,const char* name,
 std::uintptr_t id,std::shared_ptr<Record>& out,std::string& e){
 if(!self||!self->current(e))return false;
 for(const auto& weak:self->auxiliary_journal){auto journal=weak.lock();
  if(!journal||!journal->class_name||std::strcmp(journal->class_name,name)||!journal->identity||journal->identity()!=id)continue;
  if(!journal->constructor_state||journal->constructor_state()!=dh2::loader::CanonicalConstructorStateV89::completed){e="Required completed SAME restore C1 record";return false;}
  // This exact factory-authored class tag proves the existing typed record.
  // No opaque receiver-pointer cast or second record directory is introduced.
  auto actual=std::static_pointer_cast<Record>(journal->record);
  if(!actual||!actual->owner||actual->owner->base().identity()!=id){e="Changed SAME native restore class record";return false;}
  out=std::move(actual);return true;
 }
 e="Required existing journal-admitted native restore receiver";return false;
}
template<class Record,class Method>auto restore_record_call_v91(
 std::weak_ptr<RendererCampaignNonCharacterV69> weak,std::weak_ptr<Record> record,Method method){
 return [weak,record,method](auto&&... args)->bool{
  auto self=weak.lock();auto actual=record.lock();
  auto& e=std::get<sizeof...(args)-1>(std::forward_as_tuple(args...));
  if(!self||!actual||!actual->owner||actual->constructor_state!=dh2::loader::CanonicalConstructorStateV89::completed||!self->current(e)){if(e.empty())e="Released SAME native restoration owner";return false;}
  if(!(actual->owner.get()->*method)(std::forward<decltype(args)>(args)...))return false;
  return self->current(e);
 };
}
}
bool make_campaign_noncharacter_restore_lender_v91(
 const RendererCampaignNonCharacterCompositionV69& composition,
 SourceNonCharacterRestoreLenderV89& out,std::string& e){
 if(!composition.services||!composition.services->files||!composition.services->current(e)){if(e.empty())e="Required existing native restore composition";return false;}
 const std::weak_ptr<RendererCampaignNonCharacterV69> weak=composition.services;
 const std::weak_ptr<dh2::loader::ContainerCatalogOwnerV67> containers=composition.owners.containers;
 out=[weak,containers](std::uintptr_t id,dh2::loader::NonCharacterRestoreServicesV89& out,std::string& e){
  auto self=weak.lock();std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
  if(!self||!self->current(e)||!borrow_noncharacter_saved_base_v92(self,id,pin,base,e)||!base||base->identity()!=id||!base->class_name20())return false;
  dh2::loader::NonCharacterRestoreServicesV89 s;s.owner=self->files;
  s.validate_current=[weak](std::uintptr_t id,std::string& e){auto self=weak.lock();std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};return self&&self->current(e)&&borrow_noncharacter_saved_base_v92(self,id,pin,base,e)&&base&&base->identity()==id;};
  const std::string name=base->class_name20();
  if(name=="Door"){
   std::shared_ptr<dh2::loader::CanonicalDoorRecordV27> record;if(!borrow_restore_record_v91(self,"Door",id,record,e)||&record->owner->base()!=base)return false;
   const std::weak_ptr<dh2::loader::CanonicalDoorRecordV27> typed=record;
   s.door_opened=restore_record_call_v91(weak,typed,&dh2::world::CanonicalDoorV27::source_opened_v91);
   s.door_closed=restore_record_call_v91(weak,typed,&dh2::world::CanonicalDoorV27::source_closed_v91);
  }else if(name=="TriggerObject"){
   std::shared_ptr<dh2::loader::CanonicalTriggerObjectRecordV28> record;if(!borrow_restore_record_v91(self,"TriggerObject",id,record,e)||&record->owner->base()!=base)return false;
   const std::weak_ptr<dh2::loader::CanonicalTriggerObjectRecordV28> typed=record;
   s.can_activate=restore_record_call_v91(weak,typed,&dh2::world::CanonicalTriggerObjectV28::source_can_activate_v91);
   s.test_interactive_condition=[weak,typed](std::string& e){
    auto self=weak.lock();auto record=typed.lock();if(!self||!record||!record->owner||!self->current(e)){if(e.empty())e="Released SAME native TriggerObject restore";return false;}
    dh2::world::TriggerObjectRestoreServicesV91 native;native.owner=self->files;native.condition_owner=self->native.condition_runtime;
    native.local_player=[weak](std::int32_t index,bool create,dh2::world::TriggerObjectLocalPlayerV91& out,std::string& e){
     auto self=weak.lock();auto app=self?self->application.lock():nullptr;auto pm=app?app->source_player_manager_v59():nullptr;
     if(!self||!self->current(e)||!app||!pm||!pm->belongs_to_application(app)||!pm->manager()){if(e.empty())e="Required SAME actual App40 PlayerManager";return false;}
     dh2::player::PlayerInfoFieldsV1* player{};if(!pm->get_local_player(index,create,player,e))return false;
     if(!player){e="Original TriggerObject GetLocalPlayer NULL before660 read";return false;}
     out={std::shared_ptr<void>(pm,player),reinterpret_cast<std::uintptr_t>(player),&player->character660};return true;
    };
    native.character_save=[weak](std::uintptr_t id,dh2::world::TriggerObjectSavedCharacterV91& out,std::string& e){
     auto self=weak.lock();auto world=self?self->scope.actual_world.lock():nullptr;SourceCampaignCharacterBorrowV62 actual;
     if(!self||!world||!self->current(e)||!borrow_source_campaign_character_v62(world,id,actual,e))return false;
     auto record=actual.character;
     if(!record||!record->actor||!record->actor->object||record->actor->object->identity!=id||!record->save_fields||record->save_fields->character()!=id){e="Required SAME actual local Character14e8 association";return false;}
     const auto* slot=record->save_fields->save_slot14e8();if(!slot){e="Required actual Character14e8 slot";return false;}
     out={record,id,slot,{}};
     if(!*slot){e.clear();return true;} // genuine NULL Save gate, no selected-profile fallback
     dh2::character::LootCharacterSaveBorrowV44 save;if(!record->save_fields->borrow_save(record,save,e))return false;
     if(!record->save||save.save!=record->save.get()||!save.save_lease||save.save_lease.owner_before(record->save)||record->save.owner_before(save.save_lease)){e="Required SAME positive native Save14e8 owner";return false;}
     out.save=record->save;e.clear();return true;
    };
    native.evaluate_condition=[weak,runtime=std::weak_ptr<dh2::world::NativeConditionRuntimeV69>(self->native.condition_runtime)](std::uintptr_t id,bool& value,std::string& e){
     auto self=weak.lock();auto runtime_owner=runtime.lock();
     if(!self||!runtime_owner||!self->current(e)||runtime_owner!=self->native.condition_runtime){if(e.empty())e="Required SAME Main native condition arena/Eval";return false;}
     return runtime_owner->evaluate(id,value,e); // whole original ConditionList; no fake evaluator
    };
    if(!record->owner->source_test_interactive_condition_v91(native,e))return false;return self->current(e);
   };
  }else if(name=="OpenableContainer"){
   auto owner=containers.lock();auto graph=owner?owner->openable(id):nullptr;
   if(!graph||&graph->receiver().base()!=base){e="Required SAME actual Openable graph/native restore receiver";return false;}
   const std::weak_ptr<dh2::world::CanonicalOpenableGraphV21> typed=graph;
   s.container_set_state=[weak,typed](std::uint8_t state,std::string& e){auto self=weak.lock();auto graph=typed.lock();return self&&graph&&self->current(e)&&graph->receiver().receiver().source_set_state_v91(state,e)&&self->current(e);};
   s.container_keep_physics=[weak,typed](bool& keep,std::string& e){auto self=weak.lock();auto graph=typed.lock();return self&&graph&&self->current(e)&&graph->receiver().receiver().source_keep_physics_v91(keep,e)&&self->current(e);};
   s.set_physical_null_false=[weak,typed](std::string& e){auto self=weak.lock();auto graph=typed.lock();return self&&graph&&self->current(e)&&graph->receiver().receiver().source_detach_physical_v91(e)&&self->current(e);};
  }else if(name=="DestructibleContainer"){
   auto owner=containers.lock();auto actual=owner?owner->destructible(id):nullptr;
   if(!actual||&actual->base()!=base){e="Required SAME actual Destructible native restore receiver";return false;}
   const std::weak_ptr<dh2::world::CanonicalDestructibleContainerV16> typed=actual;
   s.container_set_state=[weak,typed](std::uint8_t state,std::string& e){auto self=weak.lock();auto actual=typed.lock();return self&&actual&&self->current(e)&&actual->source_set_state_v91(state,e)&&self->current(e);};
   s.container_keep_physics=[weak,typed](bool& keep,std::string& e){auto self=weak.lock();auto actual=typed.lock();return self&&actual&&self->current(e)&&actual->source_keep_physics_v91(keep,e)&&self->current(e);};
   s.set_physical_null_false=[weak,typed](std::string& e){auto self=weak.lock();auto actual=typed.lock();return self&&actual&&self->current(e)&&actual->source_detach_physical_v91(e)&&self->current(e);};
  }
  // Other proven classes require only inherited GameObject payload. Existing
  // saved-object adapter supplies actual base visibility and animator methods.
  out=std::move(s);e.clear();return true;
 };e.clear();return true;
}
