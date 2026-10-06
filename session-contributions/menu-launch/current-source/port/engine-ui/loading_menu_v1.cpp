#include "loading_menu_v1.hpp"
#include <stdexcept>
#include <cstring>
namespace dh2::ui {
namespace {
struct Reader {
 LoadingMenuBytesV1 b;std::size_t at{};
 explicit Reader(LoadingMenuBytesV1 bytes):b(bytes){if(!b.data||b.size>16*1024*1024)throw std::runtime_error("Loading hints input outside bounds");}
 void require(std::size_t n){if(n>b.size-at)throw std::runtime_error("Truncated loading hints input");}
 std::uint32_t word(){require(4);auto p=b.data+at;at+=4;return std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);}
 std::uint32_t count(){auto n=word();if(n>65536)throw std::runtime_error("Loading hints count outside bounds");return n;}
 std::vector<std::string> strings(){auto n=count();std::vector<std::string> v;while(n--){auto size=word();require(size);v.emplace_back(reinterpret_cast<const char*>(b.data+at),size);at+=size;}return v;}
 void end(){if(at!=b.size)throw std::runtime_error("Loading hints trailing bytes");}
};
}
bool LoadingHintTableV1::load(LoadingMenuBytesV1 records,LoadingMenuBytesV1 names,LoadingMenuBytesV1 fields,std::string& error){
 try{
  Reader r(records),n(names),f(fields);
  if(f.strings()!=std::vector<std::string>{"HelpStringCategory","HelpStringText"}||f.strings()!=std::vector<std::string>{"HintAvatarID","HintStringID"})throw std::runtime_error("Loading hints schema differs");
  auto help=r.count();auto help_names=n.strings();if(help!=help_names.size())throw std::runtime_error("Help records/names differ");r.require(std::size_t(help)*8);r.at+=std::size_t(help)*8;
  auto count=r.count();auto hint_names=n.strings();if(count!=hint_names.size()||!count)throw std::runtime_error("Loading hints records/names differ or empty");
  std::vector<LoadingHintRowV1> next;next.reserve(count);while(count--)next.push_back({r.word(),r.word()});
  r.end();n.end();f.end();rows_=std::move(next);error.clear();return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
bool LoadingHintTableV1::next(LoadingHintRandomV1& random,std::uint32_t& id,std::string& error)const{
 if(rows_.empty()){error="Original loading hint table unavailable";return false;}
 // NativeGetLoadingTipStrID43cb24: ARM32 multiply/add wraps before modulus.
 random.seed=(random.seed*59051u+177149u)%14348907u;
 ++random.debug_calls;
 id=rows_[random.seed%rows_.size()].string_id;error.clear();return true;
}
std::int32_t loading_menu_progress_v1(bool present,std::uint32_t progress,std::uint32_t online){
 auto raw=present?progress:online==3?0u:100u;std::int32_t value;std::memcpy(&value,&raw,4);return value;
}
bool loading_menu_end_v1(bool present,std::uint32_t& state){if(!present||state!=36)return false;++state;return true;}
bool loading_menu_read_progress_v1(const LoadingMenuStateServicesV1& s,std::int32_t& progress,std::string& error){
 error.clear();
 if(!s.current_level){error="Loading progress requires retained Application::GetCurrentLevel";return false;}
 std::uintptr_t level{};if(!s.current_level(s.context,level,error))return false;
 std::uint32_t raw{},online{};
 if(level){
  if(!s.level_progress){error="Loading progress requires actual Level progress word";return false;}
  if(!s.level_progress(s.context,level,raw,error))return false;
 }else{
  if(!s.online_state){error="Loading progress without Level requires actual OnlineGameState";return false;}
  if(!s.online_state(s.context,online,error))return false;
 }
 progress=loading_menu_progress_v1(level!=0,raw,online);return true;
}
bool loading_menu_finish_v1(const LoadingMenuStateServicesV1& s,bool& advanced,std::string& error){
 error.clear();
 if(!s.current_level){error="Loading completion requires retained Application::GetCurrentLevel";return false;}
 std::uintptr_t level{};if(!s.current_level(s.context,level,error))return false;
 if(!level){advanced=false;return true;}
 if(!s.level_state){error="Loading completion requires actual Level readiness word";return false;}
 std::uint32_t state{};if(!s.level_state(s.context,level,state,error))return false;
 const auto expected=state;
 if(!loading_menu_end_v1(true,state)){advanced=false;return true;}
 if(!s.advance_level_state){error="Loading completion requires retained Level state commit";return false;}
 if(!s.advance_level_state(s.context,level,expected,state,error))return false;
 advanced=true;return true;
}

namespace {
template<class T> bool loading_read(void* context,bool (*provider)(void*,T&,std::string&),T& value,const char* owner,std::string& error){
 if(!provider){error=std::string("Loading requires retained ")+owner;return false;}
 return provider(context,value,error);
}
bool loading_player_field(const LoadingMenuMultiplayerServicesV1& s,bool (*provider)(void*,std::uintptr_t,std::uint8_t&,std::string&),std::uint8_t& value,const char* field,std::string& error){
 std::uintptr_t player{};
 if(!loading_read(s.context,s.hosting_player,player,"PlayerManager::GetHostingPlayer",error))return false;
 if(!player){error="Loading hosting player identity is absent";return false;}
 if(!provider){error=std::string("Loading requires retained hosting player ")+field;return false;}
 return provider(s.context,player,value,error);
}
}
bool loading_menu_multiplayer_completed_v1(const LoadingMenuMultiplayerServicesV1& s,bool& result,std::string& error){
 error.clear();bool enabled{},done{},ready{};
 if(!loading_read(s.context,s.enabled,enabled,"Online enabled byte",error))return false;
 if(!enabled){result=true;return true;}
 if(!loading_read(s.context,s.all_loading_done,done,"PlayerManager::AllLoadingDone",error))return false;
 if(!done){result=false;return true;}
 if(!loading_read(s.context,s.all_ready_to_roll,ready,"PlayerManager::AllReadyToRoll",error))return false;
 result=ready;return true;
}
bool loading_menu_multiplayer_host_v1(const LoadingMenuMultiplayerServicesV1& s,bool& result,std::string& error){
 error.clear();bool enabled{},host{};std::uint32_t state{};
 if(!loading_read(s.context,s.enabled,enabled,"Online enabled byte",error))return false;
 if(!enabled){result=true;return true;}
 if(!loading_read(s.context,s.online_state,state,"OnlineGameState state word",error))return false;
 // Original ARM unsigned (state-3)<=1 dispatches exactly states3 and4.
 if(state-3u<=1u){
  if(!loading_read(s.context,s.matching_is_host,host,"CMatchingGLLive::IsHost",error))return false;
 }else if(!loading_read(s.context,s.local_player_hosting,host,"PlayerManager::IsLocalPlayerHosting",error))return false;
 result=host;return true;
}
bool loading_menu_wait_for_host_v1(const LoadingMenuMultiplayerServicesV1& s,bool& result,std::string& error){
 error.clear();bool enabled{},local_host{};std::uint8_t first{},manager{},second{},ready{};
 if(!loading_read(s.context,s.enabled,enabled,"Online enabled byte",error))return false;
 if(!enabled){result=false;return true;}
 if(!loading_read(s.context,s.local_player_hosting,local_host,"PlayerManager::IsLocalPlayerHosting",error))return false;
 if(local_host){result=false;return true;}
 if(!loading_player_field(s,s.player_field_4e5,first,"field4e5",error))return false;
 if(!first){result=true;return true;}
 if(!loading_read(s.context,s.manager_field_71a,manager,"PlayerManager field71a",error))return false;
 if(!manager){result=false;return true;}
 // Original43b124 and43b138 call GetHostingPlayer again. Preserve each
 // fresh owner identity and the source's lazy reads; never cache a snapshot.
 if(!loading_player_field(s,s.player_field_4e5,second,"field4e5",error))return false;
 if(!second){result=false;return true;}
 if(!loading_player_field(s,s.player_field_505,ready,"field505",error))return false;
 result=ready!=0;return true;
}

namespace {
bool loading_hud_call(void* context,bool (*provider)(void*,std::string&),const char* owner,std::string& error){
 if(!provider){error=std::string("Loading HUD requires retained ")+owner;return false;}
 return provider(context,error);
}
}
bool loading_menu_back_to_hud_v1(const LoadingMenuHudServicesV1& s,std::string& error){
 error.clear();std::uintptr_t level{};
 if(!loading_read(s.context,s.current_level,level,"Application::GetCurrentLevel",error))return false;
 if(!level)return true; // Original4449b0..4449b4 does nothing for actual absence.
 if(!s.set_level_field_198){error="Loading HUD requires retained Level field198 commit";return false;}
 if(!s.set_level_field_198(s.context,level,1,error))return false;
 std::uint8_t script_flag{};
 if(!loading_read(s.context,s.script_manager_field_30,script_flag,"ScriptManager field30",error))return false;
 if(!script_flag){
  if(!loading_hud_call(s.context,s.display_right_hud,"HUD root DisplayRightHud",error))return false;
  std::uint32_t raw{};if(!loading_read(s.context,s.action_icon_id,raw,"MenuManager action icon word",error))return false;
  std::int32_t icon;std::memcpy(&icon,&raw,sizeof(icon));
  if(!s.fill_action_icon){error="Loading HUD requires actual HUD root FillActionIcon";return false;}
  if(!s.fill_action_icon(s.context,icon,error))return false;
 }
 if(!loading_hud_call(s.context,s.reset_finger_map,"ZoomHandler::ResetFingerMap",error)||
    !loading_hud_call(s.context,s.reset_update_cursor,"MultiMenuManager::ResetUpdateCursor",error)||
    !loading_hud_call(s.context,s.touch_hud_controls,"HUDControls::Touch",error))return false;
 std::uintptr_t character{};
 if(!loading_read(s.context,s.local_character,character,"PlayerManager local character",error))return false;
 if(!character)return true;
 // Original444a10..444a28 repeats GetLocalPlayer(0,true) before inventory.
 if(!loading_read(s.context,s.local_character,character,"PlayerManager local character",error))return false;
 if(!character){error="Loading HUD local character disappeared before armor check";return false;}
 if(!s.trophy_check_armor_set){error="Loading HUD requires actual ItemInventory::TrophyCheckArmorSet";return false;}
 return s.trophy_check_armor_set(s.context,character,error);
}
bool loading_menu_refresh_hud_v1(const LoadingMenuHudServicesV1& s,std::string& error){
 error.clear();
 if(!s.set_info_hud_field_4){error="Loading HUD refresh requires retained InfoHUDManager field4 commit";return false;}
 if(!s.set_info_hud_field_4(s.context,0,error))return false;
 return loading_hud_call(s.context,s.touch_hud_controls,"HUDControls::Touch",error);
}
}


