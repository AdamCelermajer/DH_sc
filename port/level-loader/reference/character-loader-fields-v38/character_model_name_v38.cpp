#include "character_model_name_v38.hpp"
namespace dh2::character {
bool character_model_name_v38(const CharacterModelFieldsV38& f,const CharacterModelNamesV38& names,
 const CharacterModelServicesV38& s,CharacterModelResultV38& out,std::string& e){
 if(!f.receiver_lease||!f.identity||!f.properties||!f.properties->resolved||!names.dictionary_lease||!names.files){e="Required SAME actual Character/model dictionary field producers";return false;}
 CharacterModelResultV38 result;result.dictionary_lease=names.dictionary_lease;
 // Original GetCharModelId3a31e8: Character+1004 = CharProperties560 +
 // resolved-sheet a94 + sheet header4 + ModelFile index3*4.
 auto id=f.properties->resolved[3];
 if(id<0||static_cast<std::size_t>(id)>=names.files->size()){out=std::move(result);return true;}
 std::int32_t type{};if(!s.character_type){e="Required actual Character GetCharType3a3054";return false;}
 if(!s.character_type(f.identity,type,e))return false;
 if(type==3){
  if(!f.master418){e="Required produced SAME CharAI master50/Character418 field";return false;}
  const auto master=*f.master418;
  if(master){
   if(!s.saved_current_faery||!s.faery_model){e="Required actual live master Save/Faery model providers";return false;}
   std::int32_t faery{};if(!s.saved_current_faery(master,-1,faery,e)||!s.faery_model(master,faery,id,e))return false;
   result.branch=CharacterModelBranchV38::faery_owned;
  }else result.branch=CharacterModelBranchV38::faery_null_master;
 }else{
  bool player{};if(!s.is_player){e="Required actual Character IsPlayer virtual28";return false;}
  if(!s.is_player(f.identity,player,e))return false;
  result.branch=player?CharacterModelBranchV38::player_default:CharacterModelBranchV38::authored_nonplayer;
  if(player){
   bool high{};if(!s.high_performance){e="Required actual Device IsHighPerformance38174c";return false;}
   if(!s.high_performance(high,e))return false;
   if(!high){
    bool local{};if(!s.is_local_player){e="Required actual PlayerManager IsLocalPlayer36effc";return false;}
    if(!s.is_local_player(f.identity,local,e))return false;
    if(!local){
     if(!f.properties13c8){e="Required SAME produced Character properties13c8 cache";return false;}
     auto klass=*f.properties13c8;
     if(klass>=290&&klass<=292)id=75;
     else if(klass>=325&&klass<=327)id=76;
     else if(klass>=263&&klass<=265)id=77;
     else {if(!s.unknown_remote_class_debug){e="Required original unknown remote-player class Debug policy";return false;}if(!s.unknown_remote_class_debug(f.identity,klass,e))return false;}
     result.branch=CharacterModelBranchV38::player_low_remote;
    }
   }
  }
 }
 if(id>=0&&static_cast<std::size_t>(id)<names.files->size()){result.file=&(*names.files)[id];result.model_id=id;}
 out=std::move(result);return true;
}
}
