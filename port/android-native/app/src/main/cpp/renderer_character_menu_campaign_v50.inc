// Staged root runtime supplement. Member:
// shared_ptr<dh2::character::CharacterMenuCampaignSaveV50> character_campaign_save_v50;
// No source profile is constructed here. Root's selected-slot startup owns
// source filename/factory/publication and the genuine private files/jobs.
static bool menu_campaign_writer_v50(const model_renderer::PlayerGameplayBinding& p,
 dh2::data::PlayerSaveWriteServicesV1& out,std::string& error){
 auto* t=player_skills_runtime.get();
 if(!t||t->world!=p.world_owner||!t->player_save_load||
    &t->player_save_load->save()!=p.save||!t->player||t->player->native_savegame()!=p.save){
  error="Required SAME current panel Save/profile/skills authority";return false;
 }
 // Genuine NULL-profile source SG_Save returns before calling services.
 // This does not turn slot-1 into a profile or claim disk persistence.
 if(!t->player_save_load->profile().identity){out={};return true;}
 if(!t->character_campaign_save_v50||!t->character_campaign_save_v50->ready()){
  error="Required bound original15-section Character campaign writer";return false;
 }
 out=t->character_campaign_save_v50->write_services();return true;
}
// Root connect_character_panel_runtime_impl:
// supplement_character_menu_mutations_v4(out,error,menu_campaign_writer_v50)
// Construct/bind CharacterMenuCampaignSaveV50 at actual published profile+8
// boundary, with an owning SaveLoad lease (never alias World to a dead runtime
// pointer), actual cache table-name borrows and receiver callback:
//   validate current t/player/Gear/Save; out.receiver=actual owning live lease;
//   out.properties=&t.player->session().property_view();
//   out.inventory=player_equipment->inventory();
// Keep old source registration/reader coordinator: on load_section PROP route
// to character_campaign_save_v50->load_services_v50(original_remaining).
// Attach through SAME authority.bind_services_v50; weak reader callback avoids
// SaveLoad -> services -> campaign writer -> SaveLoad lifetime cycles.
// Other source initialization/load masks still need their actual providers.
// Release writer before Profile/Save/Gear and before replacing table borrows.
