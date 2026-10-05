#include "character_menu_faery_actions_v1.hpp"
namespace dh2::ui {
namespace {
struct CurrentFaery {using type=std::array<std::int32_t,3> data::PlayerSavegameV1::*;friend type access(CurrentFaery);};
template<class Tag,typename Tag::type Member>struct Expose{friend typename Tag::type access(Tag){return Member;}};
template struct Expose<CurrentFaery,&data::PlayerSavegameV1::current_faery_>;
}
bool character_menu_store_current_faery_v1(data::PlayerSavegameV1& save,std::uintptr_t character,std::uint32_t faery,std::int32_t difficulty,std::string& error){
 if(!character||save.character()!=character||difficulty<0||difficulty>=3||!save.faeries_initialized()[unsigned(difficulty)]||faery>=5){error="Unsupported source ChangeFaery assertion domain or save backing";return false;}
 // Exact SG_SetCurrentFaerie3bb9d8 field store, using the real SAME save.
 (save.*access(CurrentFaery{}))[unsigned(difficulty)]=std::int32_t(faery);error.clear();return true;
}
bool CharacterMenuFaeryActionsV1::set_active(std::uint32_t faery,std::string& error){
 if(running_){error="Unsupported ChangeFaery callback reentry";return false;}
 if(!graph_.owner||!graph_.actions||!graph_.actions->validate_graph(true,error)){if(error.empty())error="Required same-Character faery/skill/save backing unavailable";return false;}
 running_=true;struct Guard{bool& running;~Guard(){running=false;}}guard{running_};auto& g=graph_.actions->bindings();auto character=g.equipment->inventory()->character();
 std::int32_t difficulty;if(!graph_.difficulty||!graph_.difficulty(difficulty,error)){if(error.empty())error="Required fresh source difficulty unavailable";return false;}
 if(!character_menu_store_current_faery_v1(*g.save,character,faery,difficulty,error))return false;
 // This is the existing complete native UpdateAllSkills owner, with the same
 // session/save, actual scripts and live script services. No replacement rows.
 if(g.skills.update()!=1){error=g.skills.error();if(error.empty())error="Native UpdateAllSkills could not continue";return false;}
 std::uintptr_t actor;if(!graph_.faery_character||!graph_.faery_character(character,actor,error)){if(error.empty())error="Required actual faery Character query unavailable";return false;}
 if(actor&&(!graph_.retarget_effect||!graph_.retarget_effect(actor,error))){if(error.empty())error="Required genuine faery retarget/effect unavailable";return false;}
 std::uintptr_t hud;if(!graph_.current_hud_player||!graph_.current_hud_player(hud,error)){if(error.empty())error="Required fresh current HUD player unavailable";return false;}
 if(hud){if(!graph_.current_hud_player(hud,error))return false;if(!hud){error="Current HUD player disappeared before source interface delivery";return false;}
  if(!graph_.set_faery_interface||!graph_.set_faery_interface(hud,false,error)){if(error.empty())error="Required source SetFaeryInterface(false) unavailable";return false;}}
 error.clear();return true;
}
}
