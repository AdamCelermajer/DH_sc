// Include in model_renderer namespace after renderer_character_campaign_lookup_v62.inc.
// Uses ONLY its existing checked current-world Character borrower and Save14e8.
namespace {
bool borrow_restore_save_v84(const SourceCampaignCharacterBorrowV62& actual,
 std::shared_ptr<dh2::data::PlayerSavegameV1>& save,std::string& e){
 save.reset();auto record=actual.character;
 if(!record||!record->actor||!record->actor->object||!record->save_fields||record->save_fields->character()!=record->actor->object->identity){
  e="Required SAME actual Character14e8 association";return false;
 }
 if(!*record->save_fields->save_slot14e8()){e.clear();return true;} // Original NULL BX before global/query.
 dh2::character::LootCharacterSaveBorrowV44 borrowed;
 if(!record->save_fields->borrow_save(record,borrowed,e))return false;
 if(!record->save||borrowed.save!=record->save.get()||!borrowed.save_lease||
    borrowed.save_lease.owner_before(record->save)||record->save.owner_before(borrowed.save_lease)){
  e="Required SAME retained Save14e8 owner, no selected-menu substitute";return false;
 }
 save=record->save;return true;
}
}
bool source_campaign_character_save_use_spawn_point_v84(const std::shared_ptr<void>& world,
 std::uintptr_t character,bool value,std::string& e){
 SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,character,actual,e))return false;
 std::shared_ptr<dh2::data::PlayerSavegameV1> save;if(!borrow_restore_save_v84(actual,save,e))return false;
 if(!save){e.clear();return true;}
 const auto global=actual.character->services.difficulty_global;
 if(!global){e="Required SAME process PlayerSavegame.m_difficultyLevel for SG_SetUseSpawnPoint";return false;}
 return save->source_store_use_spawn_point_v84(value,global->value(),e);
}
bool source_campaign_character_save_level_id_v84(const std::shared_ptr<void>& world,
 std::uintptr_t character,std::int32_t value,std::int32_t difficulty,std::string& e){
 SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,character,actual,e))return false;
 std::shared_ptr<dh2::data::PlayerSavegameV1> save;if(!borrow_restore_save_v84(actual,save,e))return false;
 if(!save){e.clear();return true;}
 if(difficulty==-1){
  const auto global=actual.character->services.difficulty_global;
  if(!global){e="Required SAME process PlayerSavegame.m_difficultyLevel for SG_SetLevelId(-1)";return false;}
  difficulty=global->value();
 }
 return save->source_store_level_id_v84(value,difficulty,e);
}
