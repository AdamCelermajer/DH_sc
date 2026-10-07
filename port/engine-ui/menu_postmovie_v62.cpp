#include "menu_postmovie_v62.hpp"
#include <exception>
#include <utility>
namespace dh2::ui {namespace {
bool missing(std::string& e,const char* endpoint){if(e.empty())e=std::string("Required actual postmovie ")+endpoint;return false;}
}
bool menu_currently_in_game_view_v62(const MenuCurrentLevelV62& query,bool& out,std::string& e){
 out=false;if(!query)return missing(e,"Application current-Level");
 MenuLevelBorrowV58 first;if(!query(first,e))return false;
 if(!first.identity){e.clear();return true;}
 first={};MenuLevelBorrowV58 second;if(!query(second,e))return false;
 if(!second.identity||!second.actual_owner||!second.byte198)return missing(e,"reloaded SAME current-Level198");
 out=*second.byte198!=0;e.clear();return true;
}
bool MenuInfoHudOwnerV62::run(HudManagerEntry entry,const HudManagerServices& s,std::string& e){
 if(!s.invoke)return missing(e,"InfoHUD service transport");
 const auto result=dh2_ui_hud_manager_v1(&state_,static_cast<std::uint32_t>(entry),&s);
 if(result){if(e.empty())e="Original InfoHUD delivery failed ("+std::to_string(result)+")";return false;}
 e.clear();return true;
}
bool hud_controls_update_v62(HudAttackHeldFieldsV46& f,AuthoredJoystickStateV1& j,const HudControlsServicesV62& s,std::string& e){
 const auto& p=s.prefix;std::uint8_t blocked{};
 if(!p.input_blocked30||!p.input_blocked30(p.context,blocked,e))return missing(e,"input byte30");
 if(!p.store_controller_blocked||!p.store_controller_blocked(p.context,blocked?1:0,e))return missing(e,"controller blocked global");
 f.consumed84=0;
 std::uintptr_t level{};const std::uint8_t* enabled{};
 if(!p.current_level||!p.current_level(p.context,level,enabled,e))return missing(e,"current-Level");
 if(level){if(!enabled)return missing(e,"same Level198");if(!*enabled){f.held9=0;j.active=0;e.clear();return true;}}
 std::uintptr_t root{};
 if(!p.cached_root658||!p.cached_root658(p.context,root,e))return missing(e,"HUDControls cached root658");
 if(!root){e.clear();return true;}
 std::uint8_t cached{};
 if(!p.cached_chars8||!p.cached_chars8(p.context,cached,e))return missing(e,"cached chars8");
 if(!cached){if(!p.init_cached_chars||!p.init_cached_chars(p.context,e))return missing(e,"initCachedChars419b4c");f.pending_y80=f.pending_x7c=-1;}
 HudAttackActorBorrowV46 a;
 if(!p.local_player||!p.local_player(p.context,0,false,a,e))return missing(e,"PM.GetLocalPlayer(0,false)");
 if(!a.character){e.clear();return true;}
 if(!hud_attack_dispatch_v46(f,a,p,e))return false;
 if(j.active&&!authored_joystick_update_v1(j,s.joystick,e))return false;
 if(f.held9||f.pending_x7c<=0||f.pending_y80<=0){e.clear();return true;}
 //Original Int32ToFloat30e964 uses signed input and IEEE float conversion.
 const float screen[2]{static_cast<float>(f.pending_x7c),static_cast<float>(f.pending_y80)};
 float position[3]{};bool hit{};
 if(!s.screen_hit525884||!s.screen_hit525884(a.character,screen,position,hit,e))return missing(e,"SceneManager screen-hit525884");
 if(!hit){e.clear();return true;}
 if(!s.click_effect149c||!s.click_effect149c(a.character,position,e))return missing(e,"same optional Character149c effect sequence");
 if(!a.controller378||!s.cmd_head_to4054e4||!s.cmd_head_to4054e4(a.controller378,position,e))return missing(e,"Cmd_HeadTo4054e4");
 e.clear();return true;
}
bool MenuMapIconsV62::source_append(std::uint32_t type,MenuMapIconV62 icon,std::string& e){
 if(clearing_||type>=icons_.size()||!icon.weak_owner||!icon.borrow)return missing(e,"same map icon weak producer/type");
 icons_[type].push_back(std::move(icon));e.clear();return true;
}
bool MenuMapIconsV62::clear_all(const Parent& parent,const Remove& remove,std::string& e){
 if(clearing_)return missing(e,"non-reentrant map icons");
 clearing_=true;struct End{bool& flag;~End(){flag=false;}}end{clearing_};
 for(auto& family:icons_){
  for(auto& weak:family){std::uintptr_t icon{},owner{};
   const auto& actual_parent=weak.parent?weak.parent:parent;
   const auto& actual_remove=weak.remove?weak.remove:remove;
   if(!weak.borrow||!weak.borrow(icon,e))return missing(e,"map weak receiver");
   if(!icon)continue;
   if(!actual_parent||!actual_parent(icon,owner,e))return missing(e,"actual icon parent40");
   if(!owner)continue;
   //The source reloads the weak pointer before its second parent lookup.
   if(!weak.borrow(icon,e)||!icon||!actual_parent(icon,owner,e))return missing(e,"reloaded map weak/parent receiver");
   if(!owner||!actual_remove||!actual_remove(owner,icon,e))return missing(e,"display parent virtualA8 remove icon");
  }
  family.clear(); //retains capacity, like the original end=begin store
 }
 e.clear();return true;
}
MenuMapOwnerV62::MenuMapOwnerV62(){fields_.identity=reinterpret_cast<std::uintptr_t>(this);fields_.name="menu_MapSheet";state_.fields=&fields_;}
bool MenuMapOwnerV62::construct(MenuBaseFSRegistryV62& registry,const MenuMapServicesV62& s,std::string& e){
 if(attempted_){e=failure_.empty()?"Map C1 cannot replay its reached prefix":failure_;return false;}
 attempted_=true;auto fail=[&](){failure_=e;return false;};bool ignored{};
 if(!s.actual_manager||!s.debug_load||!s.debug_load(e)||!s.debug_switch||!s.debug_switch("isTracingMenuBase",ignored,e))return fail();
 registry.source_menu_base_c2_registration();
 if(!source_initialize_v67(s,e))return fail();
 constructed_=true;e.clear();return true;
}
bool MenuMapOwnerV62::source_initialize_v67(const MenuMapServicesV62& s,std::string& e){
 if(!s.register_menu||!s.register_menu(fields_,state_,projection_,e))return false;
 constexpr const char* paths[]{"MapIconsDummy","y","Map"};
 for(unsigned i=0;i<3;++i)if(!s.find_from||!s.find_from(fields_,state_,paths[i],caches_f8_[i],e))return false;
 e.clear();return true;
}
}
