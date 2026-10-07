#include "source_main_menu_process_v114.hpp"
#include "application_player_manager_bootstrap_v59.hpp"
namespace dh2::application {
namespace {
bool current(const MainMenuProcessLeavesV114& s,std::string& e){
 if(!s.provider||!s.current){e="Required retained original main-menu process leaves";return false;}return s.current(e);
}
}
bool source_send_score_v114(ApplicationServicesOwnerV5& app,const MainMenuProcessLeavesV114& s,std::string& e){
 if(!current(s,e))return false;auto pm=app.source_player_manager_v59();
 if(!pm||!pm->manager()){e="Required actual App40 PlayerManager for SendGLHiScore";return false;}
 player::PlayerInfoFieldsV1* info{};
 if(!pm->manager()->get_local_player(0,false,info,e)||!info)return false;
 if(!info->character660){e.clear();return true;} // original genuine NULL660
 std::shared_ptr<void> properties_pin;data::PropertyView* view{};
 if(!s.properties||!s.properties(info->character660,properties_pin,view,e)||!properties_pin||!view||!view->resolved){if(e.empty())e="Required same score Character properties560";return false;}
 const auto score=view->resolved[24]>>8; // CharProperties.GetInt(24,false)
 std::int32_t enabled{};if(!s.internet_access||!s.internet_access(enabled,e)||!current(s,e)){if(e.empty())e="Required actual nativeIsWifiAlive JNI integer";return false;}
 std::shared_ptr<void> cx_pin;std::uint8_t* state{};
 if(!s.cx_player_state5||!s.cx_player_state5(cx_pin,state,e)||!cx_pin||!state){if(e.empty())e="Required same CXPlayerManager source byte5";return false;}
 *state=enabled?1:0;
 if(!enabled){e.clear();return true;}
 bool uploaded{};if(!s.upload_my_score||!s.upload_my_score(score,uploaded,e)||!current(s,e)){if(e.empty())e="Required actual CXPlayerManager.UploadMyScore";return false;}
 if(uploaded&&dh2_property_set_int(view,24,0)){e="Actual score properties rejected SetInt24";return false;}
 e.clear();return true;
}
bool source_unlock_trophies_v114(ApplicationServicesOwnerV5&,const MainMenuProcessLeavesV114& s,std::string& e){
 if(!current(s,e))return false;std::shared_ptr<void> pin;trophies::TrophyManagerOwnerV1* manager{};const std::uint32_t* count{};
 if(!s.trophies||!s.trophies(pin,manager,count,e)||!pin||!manager||!count){if(e.empty())e="Required same process TrophyManager and Arrays TrophyTable count";return false;}
 // Original reads the source count at every condition, and data4 each time.
 for(std::uint32_t i=0;i<*count;++i){
  if(i>=manager->data().size()){e="Original trophy data/count domain mismatch";return false;}
  const auto& row=manager->data()[i];
  if(row.unlocked){
   if(!s.notify_trophy||!s.notify_trophy(row.gl_index,e)||!current(s,e)){if(e.empty())e="Required original Android NotifyTrophy";return false;}
  }
 }
 e.clear();return true;
}
bool source_menu_online_v114(ApplicationServicesOwnerV5& app,std::shared_ptr<SourceOnlineLoadingOwnerV55>& out,bool& enabled,std::string& e){
 out=app.get_online_loading_v55();if(!out){e="Required original lazy COnline instance";return false;}
 enabled=out->byte5()!=0;e.clear();return true;
}
}
