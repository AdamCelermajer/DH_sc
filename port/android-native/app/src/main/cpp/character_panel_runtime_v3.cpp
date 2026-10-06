#include "character_panel_runtime_v3.hpp"
#include <stdexcept>
namespace dh2::android_ui {
CharacterPanelRuntimeV3::CharacterPanelRuntimeV3(std::shared_ptr<void> world,
 character::skills::CharacterPlayerSkillsV6& skills,player::PlayerEquipmentRenderOwnerV1& gear,
 player::PlayerManagerOwnerV1& manager,data::PlayerSaveLoadOwnerV1& saved,
 character::CharacterGameDesign::Borrow&& design,CharacterPanelMovieRuntimeV3 movie):
 world_(std::move(world)),skills_(skills),gear_(gear),manager_(manager),saved_(saved),
 recalc_(std::move(design),skills.session().properties(),skills.session().property_view()),movie_(std::move(movie)){
 if(!world_||skills.native_savegame()!=&saved.save()||gear.properties()!=skills.session().properties())
  throw std::invalid_argument("Panel runtime requires sole Save/property/Gear/skills graph");
}
bool CharacterPanelRuntimeV3::source(const model_renderer::PlayerGameplayBinding& p,
 const ui::MenuReloadRequest32V1& q,ui::MenuReloadResponse16V1& out,std::string& error){
 using namespace ui;
 if(p.skills!=&skills_||p.gear!=&gear_||p.save!=&saved_.save()){
  error="Panel reload replaced its retained authority";return false;
 }
 switch(q.service){
 case reload_remove_buffs_v1:{character::BuffResult24 result{};
  if(dh2_character_buffs_remove_all(&result,skills_.native_buffs())!=1){
   error="Source RemoveAllBuffs failed at phase "+std::to_string(result.phase);return false;
  }return true;
 }
 case reload_saved_skills_v1:return saved_.load(std::int32_t(q.argument),error);
 case reload_skill_instances_v1:
  if(skills_.native_initialize_skill_instances()!=1){error=skills_.error();return false;}return true;
 case reload_recalculate_v1:return recalc_.recalculate(skills_.session().property_view(),q.argument,error);
 case reload_check_items_v1:return gear_.check_item_requirements_v1(error);
 case reload_menu_fx_v1:
  if(movie_.current_menu_fx)return movie_.current_menu_fx(out.identity,error);
  error="Required current MultiMenuManager RenderFX lookup";return false;
 case reload_spec_prompt_v1:
  if(movie_.invoke)return movie_.invoke(q.subject,q.path,q.callback,q.argument!=0,error);
  error="Required scoped current RenderFX IsSpecTime callback";return false;
 default:error="Unowned Character panel reload operation "+std::to_string(q.service);return false;
 }
}
bool CharacterPanelRuntimeV3::bind(const model_renderer::PlayerGameplayBinding& p,
 ui::CharacterMenuReloadActionGraphV1& out,std::string& error){
 CharacterPanelReloadServicesV3 services;services.owner=world_;
 services.player=[this](std::int32_t index,bool remote,std::uintptr_t& identity,std::string& e){
  player::PlayerInfoFieldsV1* info{};if(!manager_.get_player(index,remote,info,e))return false;
  identity=info?info->character660:0;return true;
 };
 services.player_index=movie_.player_index;
 services.invoke=[this](const auto& player,const auto& q,auto& result,auto& e){return source(player,q,result,e);};
 return character_panel_reload_binding_v3(p,services,out,error);
}
}
