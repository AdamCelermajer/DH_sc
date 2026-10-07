#include "character_model_name_v62.hpp"
namespace dh2::character {
bool character_model_name_v62(std::int32_t id,std::int16_t property_id,const data::Dictionary& models,const CharacterModelNameServicesV62& s,const char*& out,std::string& e){
 out=nullptr;if(id==-1)return true; //3a54f4 source early guard.
 if(!s.receiver||!s.is_faery){e="Required source GetCharModelName IsFaery receiver";return false;}
 bool faery{};if(!s.is_faery(faery,e))return false;
 if(faery){
  if(!s.faery_master418){e="Required SAME faery master418 field";return false;}std::uintptr_t master{};if(!s.faery_master418(master,e))return false;
  if(master){if(!s.current_faery||!s.faery_model){e="Required actual SG_GetCurrentFaery(-1)/GetFaeryData";return false;}std::int32_t index{};if(!s.current_faery(master,index,e)||!s.faery_model(master,index,id,e))return false;}
 }else{
  if(!s.is_player){e="Required source Character virtual IsPlayer";return false;}bool player{};if(!s.is_player(player,e))return false;
  if(player){if(!s.high_performance){e="Required actual Device.IsHighPerformance";return false;}bool high{};if(!s.high_performance(high,e))return false;
   if(!high){if(!s.is_local_player){e="Required actual PM.IsLocalPlayer for model selection";return false;}bool local{};if(!s.is_local_player(local,e))return false;
    if(!local){std::uint32_t offset{};const auto p=std::int32_t(property_id);
     if(std::uint32_t(p-290)<=2u)offset=0x38c;
     else if(std::uint32_t(p-325)<=2u)offset=0x398;
     else if(std::uint32_t(p-263)<=2u)offset=0x3a4;
     if(offset){if(!s.low_performance_model){e="Required actual low-performance class model CString";return false;}return s.low_performance_model(offset,out,e);}
     if(!s.invalid_low_performance_class){e="Required original invalid low-performance property assertion";return false;}if(!s.invalid_low_performance_class(e))return false;
    }
   }
  }
 }
 if(id>=0&&std::size_t(id)<models.values.size())out=models.values[id].c_str();return true;
}
}
