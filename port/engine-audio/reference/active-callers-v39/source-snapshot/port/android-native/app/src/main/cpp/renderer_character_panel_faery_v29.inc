// Same graph supplemental owner for original character-menu callbacks.
// Source CharAI+58 is retained by World at the actual first construction;
// source GS current-Level BSS is read separately from the Crypt demo C1.
static bool supplement_native_character_panel_faery_v29(
 dh2::android_ui::CharacterPanelGameplayServicesV2& out,std::string& error){
 return supplement_character_menu_faery_v8(out,[](const auto& p,auto& source,auto& e){
  auto* runtime=player_skills_runtime.get();
  if(!runtime||runtime->world!=p.world_owner||!runtime->world->player_faery_v8){e="Required retained same player CharAI faery association";return false;}
  source.current_level=[](auto& identity,auto& error){
   dh2::loader::CanonicalCurrentLevelBorrowV1 current;
   if(!model_renderer::borrow_current_native_level_v27(current,error))return false;
   identity=current.identity();return true;
  };
  // Genuine ctor NULL avoids these calls. A positive faery must use its
  // actual renderer/animation owner; neither an actor nor unlock is invented.
  source.model_name=[](auto,const char*&,auto& error){error="Required actual faery Character.GetCharModelName owner";return false;};
  source.set_visual=[](auto,const char*,const char*,bool,auto& error){error="Required actual faery SetVisualObject receiver";return false;};
  source.add_animation_set=[](auto,auto& error){error="Required actual faery ANIM_AddSetToRenderObject receiver";return false;};
  dh2::world::LevelFaeryPlacementServicesV8 placement;
  // GS Level.Init/loader owns the complete source ObjectManager/faery fields.
  // The development World draw roster cannot stand in for that source list.
  if(!current_world_faery_services_v8(p,*runtime->world->player_faery_v8,std::move(source),std::move(placement),source,e))return false;
  source.place_faery_followers=[](auto,auto,auto& error){error="Required same GS Level.ObjectManager/faery placement initialization";return false;};
  return true;
 },error);
}
