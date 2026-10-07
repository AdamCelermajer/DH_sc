#include "hud_attack_control_v46.hpp"
namespace dh2::ui {namespace {
bool missing(std::string& e,const char* text){if(e.empty())e=std::string("Required source HUD attack ")+text;return false;}
}
bool hud_attack_event_v46(HudAttackHeldFieldsV46& f,std::uint32_t event,bool& consumed,std::string& e){
 e.clear();consumed=true;
 if(event==4)f.held9=1;
 else if(event==6||event==7){f.pending_y80=-1;f.held9=0;f.pending_x7c=-1;}
 return true;
}
bool hud_attack_dispatch_v46(HudAttackHeldFieldsV46& f,const HudAttackActorBorrowV46& a,const HudAttackServicesV46& s,std::string& e){
 if(!f.held9)return true;
 if(!a.character||!a.object_of_interest14a4)return missing(e,"same Character/OOI borrow");
 if(*a.object_of_interest14a4){
  // Original passes NULL to Cmd_UseOOI: the controller reloads actual OOI.
  if(!a.controller378||!s.use_ooi||!s.use_ooi(s.context,a.controller378,0,e))return missing(e,"Cmd_UseOOI(NULL)");
 }else{
  if(!a.click413)return missing(e,"same CharAI click413 source field");
  *a.click413=0;
  if(!a.controller378||!s.attack||!s.attack(s.context,a.controller378,0,e))return missing(e,"Cmd_Attack(NULL)");
 }
 return true;
}
bool hud_attack_update_v46(HudAttackHeldFieldsV46& f,std::uint8_t& joystick,const HudAttackServicesV46& s,std::string& e){
 e.clear();std::uint8_t blocked{};
 if(!s.input_blocked30||!s.input_blocked30(s.context,blocked,e))return missing(e,"actual input byte30");
 if(!s.store_controller_blocked||!s.store_controller_blocked(s.context,blocked?1:0,e))return missing(e,"controller global block store");
 f.consumed84=0;
 std::uintptr_t level{};const std::uint8_t* enabled{};
 if(!s.current_level||!s.current_level(s.context,level,enabled,e))return missing(e,"Application CurrentLevel");
 if(level){
  if(!enabled)return missing(e,"same Level byte198");
  if(!*enabled){f.held9=0;joystick=0;return true;}
 }
 std::uintptr_t root{};
 if(!s.cached_root658||!s.cached_root658(s.context,root,e))return missing(e,"actual cached root658");
 if(!root)return true;
 std::uint8_t cached{};
 if(!s.cached_chars8||!s.cached_chars8(s.context,cached,e))return missing(e,"actual cache byte8");
 if(!cached){
  if(!s.init_cached_chars||!s.init_cached_chars(s.context,e))return missing(e,"initCachedChars");
  f.pending_y80=f.pending_x7c=-1;
 }
 HudAttackActorBorrowV46 actor{};
 if(!s.local_player||!s.local_player(s.context,0,false,actor,e))return missing(e,"PlayerManager GetLocalPlayer(0,false)");
 if(!actor.character)return true;
 return hud_attack_dispatch_v46(f,actor,s,e);
}
}
