// Include in model_renderer after actual V62 Character lookup definitions.
bool bind_source_campaign_script_initialization_v116(const SourceCampaignCandidateBorrowV55& scope,std::string& e){
 const std::weak_ptr<void> weak=scope.actual_world;
 return bind_source_campaign_script_init_runtime_v62(scope.actual_world,scope.application,
  [weak](const dh2::loader::CheckedCommandBorrowV59& command,std::string& e){
   using namespace dh2;
   auto world=weak.lock();SourceCampaignCandidateBorrowV55 actual;
   if(!world||!borrow_source_campaign_candidate_v55(actual,e)||actual.actual_world!=world||!actual.objects||
      !command.actual_receiver||!command.actual_data||!command.kind8){if(e.empty())e="Required SAME canonical script Init owners";return false;}
   if(*command.kind8!=45||!std::dynamic_pointer_cast<loader::Script_PlayActorAnimReceiverV59>(command.actual_receiver)){
    e="Required recovered source Init for "+std::string(command.descriptor->class_name);return false;
   }
   //Whole45a2ec: retain the SAME Data before lookup; no persistent Handle
   //write here. Execute performs its own room-scoped lookup later.
   const auto data=command.actual_data;const char* name=data->cstring(0x18);
   if(!name){e="Required PlayActorAnim Data18 CString";return false;}
   target_providers::Handle16 handle;
   if(!actual.objects->by_name(name,-1,false,nullptr,handle,e))return false;
   const world::CanonicalObjectBorrowV1* object{};
   if(!actual.objects->resolve_handle_v4(handle,false,object,{},e))return false;
   if(!object){e.clear();return true;} //original GetObject(NULL) exit
   if(!object->as_character){e="Required actual selected ObjectHandle Character conversion";return false;}
   std::uintptr_t id{};if(!object->as_character(object->context,id,e))return false;
   if(!id){e.clear();return true;} //original wrong-type NULL exit
   SourceCampaignCharacterBorrowV62 character;if(!borrow_source_campaign_character_v62(world,id,character,e))return false;
   auto animator=character.character->visual?character.character->visual->animator():nullptr;
   if(!animator){e="Required actual Character4f8 CharAnimator before positive PlayActorAnim Init";return false;}
   for(const auto offset:{8u,12u}){
    const auto* field=data->scalar(offset);if(!field||field->width!=4){e="Required PlayActorAnim SAME Data animation dictionary word";return false;}
    std::int32_t animation;std::memcpy(&animation,&field->bits,4);
    if(!animator->source_add_animation_dict_v116(animation,e))return false;
   }
   e.clear();return true;
  },e);
}
