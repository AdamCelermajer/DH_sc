#include "character_menu_save_actions_v1.hpp"
namespace dh2::ui {
bool CharacterMenuSaveActionsV1::award(std::uintptr_t character,const char* key,std::string& error){
 if(!graph_.achievement||!graph_.achievement(character,key,error)){if(error.empty())error="Required source achievement provider unavailable";return false;}return true;
}
bool CharacterMenuSaveActionsV1::save(std::string& error){
 if(running_){error="Unsupported NativeSaveGame callback reentry";return false;}
 if(!graph_.owner||!graph_.actions||!graph_.actions->validate_graph(true,error)){if(error.empty())error="Required same-Character save/skill backing unavailable";return false;}
 running_=true;struct Guard{bool& running;~Guard(){running=false;}}guard{running_};auto& g=graph_.actions->bindings();auto character=g.equipment->inventory()->character();
 if(!graph_.save||!graph_.save(*g.save,g.skills,character,*g.equipment->property_view(),error)){if(error.empty())error="Required genuine Character::SG_Save unavailable";return false;}
 if(!graph_.actions->validate_graph(true,error))return false;
 bool all_unlocked=true;
 // Source re-reads saved count after each reached effect and level twice for
 // nonzero rows. Never retain row/vector addresses across provider callbacks.
 for(std::uint32_t row=0;row<g.save->skills().size();++row){
  if(!g.save->skill_level(row)){all_unlocked=false;continue;}
  auto level=g.save->skill_level(row);std::int32_t maximum;
  if(!graph_.constant||!graph_.constant("CharacterDesign","MaxSkillLevelDVeryHard",maximum,error)){if(error.empty())error="Required MaxSkillLevelDVeryHard unavailable";return false;}
  if(level==maximum&&!award(character,"epic_maxskill",error))return false;
 }
 bool player;if(!graph_.is_player||!graph_.is_player(character,player,error)){if(error.empty())error="Required source IsPlayer unavailable";return false;}if(!player){error.clear();return true;}
 if(all_unlocked&&!award(character,"epic_all_skills_unlocked",error))return false;
 std::int32_t points;if(dh2_property_resolve(g.equipment->property_view(),157,&points)){error="Source saved skill-points query failed";return false;}if((points>>8)!=0){error.clear();return true;}
 std::int32_t level;if(dh2_property_resolve(g.equipment->property_view(),19,&level)){error="Source saved level query failed";return false;}
 std::int32_t maximum;if(!graph_.constant||!graph_.constant("CharacterDesign","MaxLevelDVeryHard",maximum,error)){if(error.empty())error="Required MaxLevelDVeryHard unavailable";return false;}
 if((level>>8)==maximum&&!award(character,"epic_spent_all_skill_points",error))return false;
 error.clear();return true;
}
}
