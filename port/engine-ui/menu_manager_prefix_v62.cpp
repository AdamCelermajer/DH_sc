#include "menu_manager_prefix_v62.hpp"
#include <cstring>
#include <exception>
#include <utility>
namespace dh2::ui {
namespace {
struct FSCommand {const char* name;std::uintptr_t source;};
constexpr FSCommand commands[]{
 {"PushMenu",0x421334},{"PopMenu",0x421410},{"PopAllAbove",0x421350},{"PopAllMenu",0x4203e4},{"SwitchMenu",0x4212f8},{"PushState",0x421234},{"SetFocus",0x42121c},
 {"PlaySoundFX",0x421164},{"PlayMusic",0x4210a8},{"StopMusic",0x422304},{"StartGame",0x4220a0},{"ReturnToGame",0x420fac},{"ContinueGame",0x420e64},
 {"SetSaveSlot",0x42053c},{"SetPlayerClass",0x41f640},{"SetDifficulty",0x4222d8},{"SetPlayerName",0x422280},{"IsSaveSlotValid",0x420cfc},{"GetSaveSlot",0x420d7c},{"ResetSaveFile",0x420c98},
 {"SkipScript",0x420c68},{"GoToMainMenu",0x420c3c},{"LoadLevel",0x420bb8},{"LoadLevel2",0x41f578},{"EndLoading",0x420b48},{"LoadWorldMap",0x420b8c},{"IsMapLocLocked",0x4248dc},{"ExitGame",0x41f40c},
 {"SetBtnImage",0x42040c},{"GetCharProperty",0x421808},{"GetCharProp",0x4247c0},{"SetText",0x424698},{"SetText2",0x424540},{"GetString",0x420914},{"GetString2",0x420a1c},{"GetParsedString2",0x424ef0},
 {"GetPlayerName",0x420988},{"GetPlayerClass",0x421720},{"GetPlayerClass2",0x421980},{"GetHasTwoHandWeapon",0x421684},{"GetHasMainHandWeapon",0x4215ec},{"GetHasOffHandWeapon",0x421554},{"GetNumPotions",0x421aac},
 {"AssignPoint",0x4219b8},{"AutoEquipSlot",0x42143c},{"IncSkill",0x421ce0},{"ResetDraggablePosition",0x4208e4},{"IncOption",0x420804},{"DecOption",0x420728},{"ToggleOption",0x420624},{"SetOption",0x4205e0},{"GetOption",0x421b58},
 {"SetLanguage",0x42057c},{"SaveOptions",0x4204fc},{"LoadOptions",0x4204bc},{"PlayAnim",0x4243e0},{"GotoFrame",0x424288},{"LockCharacter",0x420344},{"UnlockCharacter",0x4202a8},{"PauseGameplay",0x41f444},{"ResumeGameplay",0x41f44c},{"StopDialog",0x427b08},{"LaunchTwitter",0x41f404}
};
template<class F,class... A> bool reached(const char* label,const F& fn,std::string& e,A&&... args){
 if(!fn){e=std::string("Required actual menu prefix provider: ")+label;return false;}
 if(!fn(std::forward<A>(args)...,e)){if(e.empty())e=std::string("Actual menu prefix failed: ")+label;return false;}return true;
}
}
void MenuBaseFSRegistryV62::source_menu_base_c2_registration(){
 initialized_source_global_=1;for(const auto& command:commands)callbacks_.emplace(command.name,command.source);
}
std::uintptr_t MenuBaseFSRegistryV62::callback(const std::string& name)const noexcept{auto at=callbacks_.find(name);return at==callbacks_.end()?0:at->second;}
MenuDebugHudOwnerV62::MenuDebugHudOwnerV62(){fields_.identity=reinterpret_cast<std::uintptr_t>(this);fields_.name="menu_DebugHUD";state_.fields=&fields_;}
bool MenuDebugHudOwnerV62::construct(MenuBaseFSRegistryV62& registry,const MenuDebugHudServicesV62& s,std::string& e){
 if(attempted_){e=failure_.empty()?"DebugHUD C1 cannot replay its source prefix":failure_;return false;}attempted_=true;
 auto fail=[&](){failure_=e;return false;};bool ignored{};
 if(!s.actual_manager||!reached("MenuBase C2 Debug.load",s.debug_load,e)||
    !reached("MenuBase C2 Debug.GetSwitch",s.debug_switch,e,"isTracingMenuBase",ignored))return fail();
 // Fields/state defaults are the actual MenuFX.State/MenuBase scalar C1
 // stores. Debug C1 adds c8/cc=0, its empty tree and five real cache owners.
 registry.source_menu_base_c2_registration();
 if(!source_initialize_v67(s,e))return fail();
 complete_=true;e.clear();return true;
}
bool MenuDebugHudOwnerV62::source_initialize_v67(const MenuDebugHudServicesV62& s,std::string& e){
 if(!reached("DebugHUD.Init RegisterMenu",s.register_menu,e,*this))return false;
 if(fields_.render){
  const char* keys[]{"text","times_loaded","times_loaded.text","seal_of_freshness","seal_of_freshness_fail"};
  constexpr unsigned order[]{0,2,1,3,4};
  for(unsigned i=0;i<caches_.size();++i)if(!reached("DebugCachedCharacter.RefreshCache",s.refresh_cache,e,*this,caches_[order[i]],keys[i]))return false;
  bool written{};if(!reached("DebugHUD.LoadFromFile",s.load_flags,e,flags_,written))return false;if(written)flags_produced_=true;
 }
 e.clear();return true;
}
bool MenuDebugHudOwnerV62::update(std::string& e)const{
 if(!complete_){e=failure_.empty()?"Required actual completed DebugHUD C1 before selected BXLR Update":failure_;return false;}
 e.clear();return true; // whole selected original429c18, no ignored constructor
}
bool MenuDebugHudOwnerV62::source_counter_store_v67(const std::string& name,std::int32_t value,std::string& e){
 if(!complete_){e="Required completed SAME DebugHUD before source counter publication";return false;}
 counters_c8_v67_[name]=value;tree_c8_.count=static_cast<std::uint32_t>(counters_c8_v67_.size());
 e.clear();return true;
}
bool MenuDebugHudOwnerV62::prepare_level_frame_v67(std::string& e){
 if(!complete_){e="Required SAME completed DebugHUD.GetInstance before Level.Update";return false;}
 if(tree_c8_.count!=counters_c8_v67_.size()){e="DebugHUD counter tree/storage ownership mismatch";return false;}
 // Original3f83ac..3f83d0 erases the actual tree only when countD8 is
 // nonzero, then resets count/parent and left/right header self pointers.
 if(tree_c8_.count){counters_c8_v67_.clear();tree_c8_.count=0;tree_c8_.parent=0;tree_c8_.left=&tree_c8_;tree_c8_.right=&tree_c8_;}
 e.clear();return true;
}
bool MenuManagerProcessV62::debug_hud(const MenuDebugHudServicesV62& services,MenuDebugHudOwnerV62*& out,std::string& e){
 if(!debug_hud_){debug_hud_=std::make_unique<MenuDebugHudOwnerV62>();out=debug_hud_.get();return debug_hud_->construct(fs_commands_,services,e);}
 out=debug_hud_.get();if(!out->constructed()){e=out->failure();if(e.empty())e="Retained DebugHUD constructor prefix incomplete";return false;}
 e.clear();return true; // source guard complete: no Init/C1/Update replay
}
MenuManagerPrefixV62::MenuManagerPrefixV62(MenuManagerPrefixServicesV62 s):services_(std::move(s)){}
bool MenuManagerPrefixV62::update(bool& early,std::int32_t& dt,std::string& e){
 early=false;if(busy_||!services_.actual_manager||!services_.process){e="Required idle actual MenuManager prefix/process owner";return false;}
 busy_=true;struct End{bool& busy;~End(){busy=false;}}end{busy_};
 try{
  auto& s=services_;
  if(*s.process->fill_leaderboard_byte()&&!reached("fillLeaderBoard441dcc",s.fill_leaderboard,e))return false;
  MenuLevelBorrowV58 level;if(!reached("Application.GetCurrentLevel",s.current_level,e,level))return false;
  if(level.identity){if(!level.actual_owner){e="Required actual current Level lease";return false;}MenuDebugHudOwnerV62* debug{};
   if(!s.process->debug_hud(s.debug_hud,debug,e)||!debug||!debug->update(e))return false;}
  std::uintptr_t player{};if(!reached("SAME App PM.GetLocalPlayer(0,true).Character660",s.local_player_character,e,player))return false;
  if(player){
   MenuPrefixCharacterBorrowV62 character;if(!reached("SAME Character14a8",s.character,e,player,character))return false;
   if(character.character!=player||!character.receiver||!character.signed_ooi_type14a8||
      *character.signed_ooi_type14a8<-128||*character.signed_ooi_type14a8>127){e="Required SAME real signed Character14a8 cell";return false;}
   const auto type=static_cast<std::int8_t>(*character.signed_ooi_type14a8);
   MenuPrefixHudBorrowV62 cached;if(!reached("SAME manager108 authored action cache",s.action_cache,e,cached))return false;
   if(!cached.receiver||!cached.cache108){e="Required actual retained authored manager108 cache";return false;}
   if(*cached.cache108!=AuthoredGameplayHudV1::action_icon(type)){
    MenuPrefixHudBorrowV62 hud;if(!reached("MenuManager.GetHUDRoot",s.hud_root,e,hud))return false;
    if(hud.facade){if(!hud.receiver||hud.cache108!=cached.cache108){e="HUD root/cache are different source owners";return false;}
     if(!hud.facade->update_action_icon(type,e))return false;}
   }
  }
  if(!reached("RenderFX.SetWireFrame(false)",s.set_wire_frame,e,false)||!reached("Debug.load wireframe",s.debug_load,e))return false;
  bool enabled{};if(!reached("IsDisplayingMenuWireframe",s.debug_switch,e,"IsDisplayingMenuWireframe",enabled))return false;
  if(enabled&&!reached("RenderFX.SetWireFrame(true)",s.set_wire_frame,e,true))return false;
  if(!reached("Debug.load menus",s.debug_load,e)||!reached("IsDeactivatingFlashMenus",s.debug_switch,e,"IsDeactivatingFlashMenus",enabled))return false;
  if(enabled){early=true;e.clear();return true;}
  if(!reached("Debug.load updates",s.debug_load,e)||!reached("IsDeactivatingFlashMenusUpdate",s.debug_switch,e,"IsDeactivatingFlashMenusUpdate",enabled))return false;
  if(enabled){early=true;e.clear();return true;}
  std::uint32_t bits{};if(!reached("actual Application.GetDt8c",s.application_dt8c,e,bits))return false;std::memcpy(&dt,&bits,sizeof(dt));
  AuthoredMenuApplicationFieldsV3* fields{};if(!reached("SAME listener map/88",s.listener_fields,e,fields)||!fields){if(e.empty())e="Required actual listener map/88";return false;}
  fields->source_update_listener_cleanup_v62();e.clear();return true;
 }catch(const std::exception& ex){e=ex.what();return false;}catch(...){e="Original menu prefix provider threw";return false;}
}
}
