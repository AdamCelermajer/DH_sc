// Include inside model_renderer after generated rooms/save/restore/release
// adapters and before renderer_character_campaign_v62.inc. No new owner TU.
static bool bind_campaign_source_owner_connections_v93(const SourceCampaignCandidateBorrowV55& actual,
 const std::shared_ptr<SourceWorldBorrowV61>& source,const RendererCampaignNonCharacterCompositionV69& composition,
 std::string& e){
 if(!source||!composition.services||!source->noncharacter_release_journal_v89){e="Required actual loader owners before source connection";return false;}
 if(source->noncharacter_release_provider_v92&&!source->noncharacter_release_provider_v92->attach(actual,composition,e))return false;
 if(!source->generated_room_native_v92){e="Required actual RoomZone native behavior provider before Module XML";return false;}
 CampaignGeneratedRoomNativeV92 native;
 if(!source->generated_room_native_v92(actual,native,e)||
  !bind_campaign_generated_rooms_complete_v92(actual,composition,std::weak_ptr<SourceWorldBorrowV61>(source),
   std::move(native),source->noncharacter_release_journal_v89,source->module_room_zones_v91,e))return false;
 SourceNonCharacterRestoreLenderV89 restore;
 if(!make_campaign_noncharacter_restore_lender_v91(composition,restore,e))return false;
 return bind_campaign_noncharacter_saved_objects_v89(actual,composition,
  source->noncharacter_release_journal_v89,std::move(restore),e,source->module_room_zones_v91);
}
