// Include INSIDE namespace model_renderer after actual V62/V70 public helpers.
// Global includes are listed in HANDOFF-request-v114.md.
bool project_source_campaign_area_transition_v114(const SourceCampaignCandidateBorrowV55& expected,
 const dh2::loader::AreaTransitionConfirmationV114& confirmed,
 dh2::loader::AreaTransitionRestoreReceiptV114 restore,
 dh2::loader::AreaTransitionProjectionServicesV114 leaves,
 std::shared_ptr<const dh2::loader::AreaTransitionRequestV114>& out,std::string& e){
 SourceCampaignCandidateBorrowV55 actual;
 if(!borrow_source_campaign_candidate_v55(actual,e)||!actual.level||!actual.actual_world||!actual.objects||
    actual.actual_world.get()!=expected.actual_world.get()||
    actual.actual_world.owner_before(expected.actual_world)||expected.actual_world.owner_before(actual.actual_world)||
    actual.level!=expected.level||actual.objects!=expected.objects||actual.level->constructor_fields_v3().field130!=38){
  if(e.empty())e="Require SAME active completed campaign at genuine menu confirmation";return false;
 }
 PlayerGameplayBinding player;
 if(!borrow_source_campaign_player_gameplay_v67(actual.actual_world,player,e))return false;
 SourceCampaignCharacterBorrowV62 selected;
 if(!player.active||!player.save||!player.character||!player.world_owner||
    player.world_owner.get()!=actual.actual_world.get()||player.world_owner.owner_before(actual.actual_world)||
    actual.actual_world.owner_before(player.world_owner)||
    !borrow_source_campaign_character_v62(actual.actual_world,player.character,selected,e)){
  if(e.empty())e="Require SAME current selected PM Character and live Save for transition";return false;
 }
 auto record=selected.character;
 if(!record||!record->save||record->save.get()!=player.save||!record->save_fields||
    !record->save_fields->save_slot14e8()||
    *record->save_fields->save_slot14e8()!=reinterpret_cast<std::uintptr_t>(record->save.get())){
  e="Require actual selected Character14e8 SAME Save owner";return false;
 }
 auto design=player.design.levels();
 if(!design){e="Require actual initialized Arrays::LevelList for confirmed transition";return false;}
 const std::weak_ptr<void> weak_world=actual.actual_world;
 const std::weak_ptr<dh2::loader::CanonicalLevelContextV1> weak_level=actual.level;
 const std::weak_ptr<dh2::world::CanonicalObjectManagerV1> weak_objects=actual.objects;
 const std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> weak_record=record;
 const auto actor=player.character;const auto difficulty=player.difficulty;
 auto incoming=std::move(leaves.current);
 // This guard exists only on the synchronous projection stack. Neither the
 // request nor restore bytes retain this closure/old World/old Character.
 leaves.current=[weak_world,weak_level,weak_objects,weak_record,actor,difficulty,incoming](std::string& error){
  if(!incoming){error="Require actual Main confirmation journal/current guard";return false;}
  if(!incoming(error))return false;
  auto world=weak_world.lock();auto level=weak_level.lock();auto objects=weak_objects.lock();auto previous=weak_record.lock();
  SourceCampaignCandidateBorrowV55 current;
  if(!world||!level||!objects||!previous||!borrow_source_campaign_candidate_v55(current,error)||
     current.actual_world.get()!=world.get()||current.actual_world.owner_before(world)||world.owner_before(current.actual_world)||
     current.level!=level||current.objects!=objects||level->constructor_fields_v3().field130!=38){
   if(error.empty())error="Changed current campaign during confirmed request projection";return false;
  }
  PlayerGameplayBinding live;
  if(!borrow_source_campaign_player_gameplay_v67(world,live,error))return false;
  SourceCampaignCharacterBorrowV62 same_actor;
  if(!live.active||live.character!=actor||live.difficulty!=difficulty||live.save!=previous->save.get()||
     !borrow_source_campaign_character_v62(world,actor,same_actor,error)||same_actor.character!=previous||
     !previous->save||previous->save->destroyed_v108()||!previous->save_fields||
     !previous->save_fields->save_slot14e8()||
     *previous->save_fields->save_slot14e8()!=reinterpret_cast<std::uintptr_t>(previous->save.get())){
   if(error.empty())error="Changed actual selected Character/Save during confirmed request projection";return false;
  }
  // Preserve Main's first failure/cancellation if the borrower reentered it.
  return incoming(error);
 };
 return dh2::loader::project_area_transition_request_v114(confirmed,*design,*record->save,
  actor,difficulty,actual.level->constructor_fields_v3().difficulty40,std::move(restore),std::move(leaves),out,e);
}
