#include "level_faery_placement_v8.hpp"
namespace dh2::world {
namespace {bool missing(std::string& e,const char* text){if(e.empty())e=text;return false;}}
bool level_place_faery_followers_v8(std::uintptr_t selected,
 const LevelFaeryPlacementServicesV8& s,std::string& error){
 if(!selected)return missing(error,"Required null-player Level.Place online/local selection continuation");
 if(!s.owner||!s.objects||!s.char_type)return missing(error,"Required SAME canonical Character list and source GetCharType");
 const auto& characters=s.objects->characters();
 for(std::size_t index=0;index<characters.size();++index){
  const auto actor=characters[index];if(!actor)continue;
  std::int32_t type{};if(!s.char_type(actor,type,error))return false;
  // Original IsFaerie re-queries GetCharType. Followers use a second query,
  // so preserve live read ordering rather than caching inferred AI kinds.
  bool faery=type==3;
  if(!faery){if(!s.char_type(actor,type,error))return false;if(type!=2)continue;}
  std::array<float,3> direction{},position{};
  if(!s.look_at_vec||!s.look_at_vec(selected,direction,error))return missing(error,"Required source GetLookAtVec");
  if(!s.target_position||!s.target_position(selected,position,error))return missing(error,"Required source GetTargetPosition");
  for(unsigned axis=0;axis<3;++axis)direction[axis]=direction[axis]+position[axis];
  if(!s.set_position||!s.set_position(actor,direction,true,error))return missing(error,"Required source follower/faery SetPosition");
  if(!s.force_position||!s.force_position(actor,error))return missing(error,"Required source ForceUpdatePosition");
  if(!s.char_type(actor,type,error))return false;
  if(type!=3){if(!s.disable_zoning||!s.disable_zoning(actor,error))return missing(error,"Required source follower DisableZoning");continue;}
  std::int32_t count{};
  if(!s.source_player_count6c4||!s.source_player_count6c4(count,error))return missing(error,"Required SAME PlayerManager source count6c4");
  for(std::int32_t p=0;p<count;++p){
   std::uintptr_t character{};
   if(!s.player||!s.player(p,false,character,error))return missing(error,"Required source GetPlayer(index,false)");
   if(character){std::uintptr_t* field=nullptr;
    if(!s.faery420_field||!s.faery420_field(character,field,error)||!field)return missing(error,"Required source writable Character420 field");
    *field=actor;
   }
   if(!s.source_player_count6c4(count,error))return false;
  }
  std::uintptr_t master{},local{};
  if(!s.master50||!s.master50(actor,master,error))return missing(error,"Required SAME faery CharAI master50");
  if(!s.local_player||!s.local_player(0,true,local,error))return missing(error,"Required source GetLocalPlayer(0,true)");
  if(!s.set_master||!s.set_master(actor,local?local:selected,error))return missing(error,"Required source AI_SetMaster");
  std::uint32_t chosen{};
  if(!s.selected_faery||!s.selected_faery(local?local:selected,chosen,error))return missing(error,"Required SAME SG_GetCurrentFaerieId(-1)");
  if(!s.change_faery||!s.change_faery(selected,chosen,error))return missing(error,"Required source nested Character.ChangeFaery");
  if(!s.set_master(actor,master,error))return false;
 }
 error.clear();return true;
}
}
