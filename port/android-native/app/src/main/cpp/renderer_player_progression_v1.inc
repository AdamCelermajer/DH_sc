// New include; root owns insertion in renderer. Required: player_progression_v1.hpp,
// player_gameplay_binding.hpp. This creates a BORROW, never another player.
bool progression_player_borrow_v1(const model_renderer::PlayerGameplayBinding& binding,
 float source_x,float source_y,bool actual_remotely_updated,bool actual_local,
 std::int32_t actual_level_policy,dh2::character::ProgressionActorV1& out,std::string& error){
 if(!binding.gear||!binding.properties||!binding.character||!binding.design||
    !binding.gear->properties()||!binding.design.characters()||!binding.design.class_rows()){
  error="Required actual PlayerGameplayBinding/Gear/Class progression borrow";return false;
 }
 auto* state=binding.gear->properties().get();auto* view=binding.properties;
 if(view->base!=state->base.data()||view->saved!=state->saved.data()||
    view->gear!=state->gear.data()||view->resolved!=state->resolved.data()||
    (binding.save&&binding.save->character()!=binding.character)){
  error="XP property/save authority is not the same equipped player";return false;
 }
 const auto& rows=*binding.design.class_rows();
 out={binding.character,state,view,binding.save,binding.design.characters(),rows.data(),
  static_cast<std::uint32_t>(rows.size()),binding.actor_index,source_x,source_y,true,
  actual_remotely_updated,actual_local,actual_level_policy,binding.difficulty};
 return true;
}
