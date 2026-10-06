// Root integration: include after PlayerSkillsRuntime/character panel helpers.
// Actual services callback supplies currentWorld's CharAI58/Level/visual graph.
// No raw ABI casts, faery spawn, unlock, timer or HUD copies are introduced.
// Call at the real player CharAI construction boundary once:
// t.faery_association_v8 = make_unique<PlayerFaeryAssociationV8>(t.fsm.character);
// Do not reset on menu open, world restore or GL surface resize. Source Level
// placement borrows and writes this SAME field, not a HUD selection mirror.
[[maybe_unused]] static bool current_world_faery_services_v8(
 const model_renderer::PlayerGameplayBinding& p,
 dh2::character::PlayerFaeryAssociationV8& association,
 dh2::character::CharacterMenuFaeryServicesV8 source,
 dh2::world::LevelFaeryPlacementServicesV8 placement,
 dh2::character::CharacterMenuFaeryServicesV8& out,std::string& error){
 auto* t=player_skills_runtime.get();
 if(!t||!t->world||t->world!=p.world_owner||t->player.get()!=p.skills||
    !t->world->canonical_world||!t->player_manager||association.character!=p.character){
  error="Required same retained CharAI/World/canonical manager for faery selection";return false;
 }
 auto same=[world=std::weak_ptr<WorldScriptContext>(t->world),skills=p.skills](std::string& e)->PlayerSkillsRuntime*{
  auto w=world.lock();auto* current=player_skills_runtime.get();
  if(!w||!current||current->world!=w||current->player.get()!=skills){e="Faery selected World/skill owner replaced";return nullptr;}return current;
 };
 source.owner=t->world;
 source.difficulty=[same](auto& value,auto& e){auto* current=same(e);if(!current)return false;
  value=current->world->host_level.difficulty;return true;
 };
 source.faery_character=[same,&association](auto character,auto& value,auto& e){auto* current=same(e);
  if(!current||character!=association.character||current->fsm.character!=character){if(e.empty())e="Required same Character420 association";return false;}
  value=association.faery420;return true;
 };
 placement.owner=t->world;placement.objects=&t->world->canonical_world->manager;
 placement.char_type=[same](auto id,auto& value,auto& e){auto* current=same(e);
  if(!current||!current->targets||!current->world->tables.ai())return false;
  dh2::character::skills::WorldTargetActorBorrowV1 actor{};
  if(current->targets->actor(id,&actor)||!actor.character||!actor.character->resolved){e="Required same canonical source Character properties";return false;}
  const auto* row=dh2::data::ai_props(*current->world->tables.ai(),actor.character->resolved[1]);
  if(!row){e="Required actual Character.GetCharAI fallback/table row";return false;}value=row->type;return true;
 };
 placement.source_player_count6c4=[same](auto& value,auto& e){auto* current=same(e);if(!current||!current->player_manager)return false;
  value=*current->player_manager->manager().character_count_field();return true;
 };
 placement.player=[same](auto index,bool flag,auto& identity,auto& e){auto* current=same(e);if(!current||!current->player_manager)return false;
  dh2::player::PlayerInfoFieldsV1* info{};if(!current->player_manager->manager().get_player(index,flag,info,e))return false;
  identity=info?info->character660:0;return true;
 };
 placement.local_player=[same](auto index,bool flag,auto& identity,auto& e){auto* current=same(e);if(!current||!current->player_manager)return false;
  dh2::player::PlayerInfoFieldsV1* info{};if(!current->player_manager->manager().get_local_player(index,flag,info,e))return false;
  identity=info?info->character660:0;return true;
 };
 // Current native player is the one real PlayerInfo. A genuine remote player's
 // separate retained CharAI field is mandatory when that branch is reached.
 placement.faery420_field=[same,&association](auto character,auto*& field,auto& e){
  if(!same(e))return false;if(character!=association.character){e="Required actual remote Character420 field";return false;}
  field=&association.faery420;return true;
 };
 placement.selected_faery=[same](auto character,auto& value,auto& e){auto* current=same(e);
  if(!current||!current->player||current->fsm.character!=character){e="Required actual selected Character Save";return false;}
  const auto* save=current->player->native_savegame();const auto tier=current->world->host_level.difficulty;
  if(!save||tier<0||tier>=3){e="Required current native Save/difficulty";return false;}
  value=std::uint32_t(save->current_faery(unsigned(tier)));return true;
 };
 // Keep actual Level borrow and positive visual/controller functions supplied
 // by root. NULL currentLevel is valid only when its genuine source getter
 // returnsNULL; absent getter fails at the original reached point.
 source.place_faery_followers=[same,placement=std::move(placement)](auto level,auto character,auto& e){
  if(!level||!same(e)){if(e.empty())e="Required actual current Level receiver";return false;}
  return dh2::world::level_place_faery_followers_v8(character,placement,e);
 };
 out=std::move(source);error.clear();return true;
}
[[maybe_unused]] static bool supplement_character_menu_faery_v8(
 dh2::android_ui::CharacterPanelGameplayServicesV2& out,
 std::function<bool(const model_renderer::PlayerGameplayBinding&,
  dh2::character::CharacterMenuFaeryServicesV8&,std::string&)> borrow,
 std::string& error){
 if(!out.owner||!borrow){error="Required retained faery menu services";return false;}
 auto preceding=out.queries;
 out.queries=[preceding,borrow=std::move(borrow)](const auto& p,auto& actions,auto& graph,auto& e){
  if(preceding&&!preceding(p,actions,graph,e))return false;
  auto* t=player_skills_runtime.get();
  if(!t||!p.skills||t->player.get()!=p.skills||t->world!=p.world_owner||
    p.skills->native_savegame()!=p.save){e="Faery menu same-player runtime replaced";return false;}
  dh2::character::CharacterMenuFaeryServicesV8 services;
  if(!borrow(p,services,e))return false;
  return dh2::character::bind_character_menu_faery_v8(actions,graph,*p.skills,std::move(services),e);
 };
 error.clear();return true;
}
