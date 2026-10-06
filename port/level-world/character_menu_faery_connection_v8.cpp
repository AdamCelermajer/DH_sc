#include "character_menu_faery_connection_v8.hpp"
namespace dh2::character {
namespace {
struct Frame {
 std::shared_ptr<void> prior,world;CharacterMenuFaeryServicesV8 source;
 std::unique_ptr<ui::CharacterMenuFaeryActionsV1> actions;
};
}
bool bind_character_menu_faery_v8(ui::CharacterMenuActionsOwnerV1& actions,
 ui::CharacterMenuQueriesGraphV1& queries,skills::CharacterPlayerSkillsV6& skills,
 CharacterMenuFaeryServicesV8 source,std::string& error){
 const auto& actual=actions.bindings();
 if(!source.owner||!queries.owner||queries.actions!=&actions||!actual.save||
    skills.native_savegame()!=actual.save||skills.state().owner!=actual.save->character()){
  error="Required same Save/skill/menu graph for ChangeFaery";return false;
 }
 auto frame=std::make_shared<Frame>();frame->prior=queries.owner;frame->world=source.owner;frame->source=std::move(source);
 ui::CharacterMenuFaeryActionsGraphV1 graph;graph.owner=frame->world;graph.actions=&actions;
 graph.difficulty=frame->source.difficulty;graph.faery_character=frame->source.faery_character;
 // Compatibility entrypoints retain the frozen V1 ABI. The verified source
 // callee identities supersede its old position/retarget comments.
 graph.retarget_effect=[weak=std::weak_ptr<Frame>(frame)](auto faery,auto& error){
  auto f=weak.lock();if(!f){error="ChangeFaery provider lifetime expired";return false;}
  const char* model{};
  if(!f->source.model_name||!f->source.model_name(faery,model,error)){if(error.empty())error="Required source faery GetCharModelName";return false;}
  if(!f->source.set_visual||!f->source.set_visual(faery,model,nullptr,true,error)){if(error.empty())error="Required source faery SetVisualObject(model,NULL,true)";return false;}
  if(!f->source.add_animation_set||!f->source.add_animation_set(faery,error)){if(error.empty())error="Required source faery ANIM_AddSetToRenderObject";return false;}
  return true;
 };
 // NativeHUDSetActiveFaery's outer tail actually queries currentLevel twice,
 // then passes selected Character to PlaceFaeryAndFollowers. No HUD callback
 // or synthetic refresh is present in this original method.
 graph.current_hud_player=[weak=std::weak_ptr<Frame>(frame)](auto& level,auto& error){
  auto f=weak.lock();if(!f){error="ChangeFaery provider lifetime expired";return false;}
  if(!f->source.current_level||!f->source.current_level(level,error)){
   if(error.empty())error="Required actual Application.GetCurrentLevel for faery placement";return false;
  }return true;
 };
 const auto character=actual.save->character();
 graph.set_faery_interface=[weak=std::weak_ptr<Frame>(frame),character](auto level,bool,auto& error){
  auto f=weak.lock();if(!f){error="ChangeFaery provider lifetime expired";return false;}
  if(!f->source.place_faery_followers||!f->source.place_faery_followers(level,character,error)){
   if(error.empty())error="Required actual Level.PlaceFaeryAndFollowers";return false;
  }return true;
 };
 frame->actions=std::make_unique<ui::CharacterMenuFaeryActionsV1>(std::move(graph));
 queries.faery_actions=frame->actions.get();queries.owner=std::move(frame);error.clear();return true;
}
}
