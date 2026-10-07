#include "menu_manager_update_v58.hpp"
#include <exception>
#include <utility>
namespace dh2::ui {
namespace {
template<class F,class... Args> bool reached(const char* name,const F& fn,
 std::string& error,Args&&... args){
 if(!fn){error=std::string("Required actual MenuManager.Update provider: ")+name;return false;}
 if(!fn(std::forward<Args>(args)...,error)){
  if(error.empty())error=std::string("Actual MenuManager.Update failed: ")+name;
  return false;
 }
 return true;
}
}
MenuManagerUpdateV58::MenuManagerUpdateV58(MenuManagerUpdateServicesV58 s):services_(std::move(s)){}
bool MenuManagerUpdateV58::update(bool hud_only,std::string& error){
 if(busy_||!services_.actual_manager){error="Required nonreentrant live MenuManager update owner";return false;}
 struct Guard {bool& busy;explicit Guard(bool& b):busy(b){busy=true;}~Guard(){busy=false;}} guard(busy_);
 try{
  auto& s=services_;bool early_exit{};std::int32_t dt{};
  if(!reached("source PM/Debug/Application/listener prefix",s.source_prefix,error,early_exit,dt))return false;
  if(early_exit)return true;
  for(std::uint32_t slot=hud_only?3:0;slot<4;++slot){
   MenuMovieBorrowV58 movie;
   if(!reached("current primary movie slot",s.movie_slot,error,slot,movie))return false;
   if(!movie.identity)continue;
   if(!movie.actual_owner){error="Required retained actual MenuFX movie";return false;}
   MenuLevelBorrowV58 level;
   if(!reached("Application.GetCurrentLevel",s.current_level,error,level))return false;
   if(level.identity&&slot==3){
    // Source calls GetCurrentLevel AGAIN at42ee14, then reads byte198.
    level={};if(!reached("Application.GetCurrentLevel HUD reread",s.current_level,error,level))return false;
    if(!level.identity||!level.actual_owner||!level.byte198){error="Required live source Level+198 HUD gate";return false;}
    if(!*level.byte198)break;
   }
   // Original r2 is always0. hud_only chooses slots; it is NOT this flag.
   if(!reached("same MenuFX virtual10(dt,false)",s.movie_virtual10,error,movie,dt,false))return false;
  }
  bool in_game{};
  if(!reached("Application.IsCurrentlyInGameView",s.currently_in_game_view,error,in_game))return false;
  if(in_game&&(!reached("InfoHUD.Update",s.info_hud_update,error)||
               !reached("HUDControls.Update",s.hud_controls_update,error)))return false;
  if(!reached("FlashAnimManager.Update",s.flash_anim_update,error))return false;
  std::int32_t count{};
  if(!reached("MenuManager.GetNumMenus",s.get_num_menus,error,count))return false;
  // Source captures the count once, but rereads registry64 at EVERY index
  // and again after IsVisible before delivering its selected virtual1c.
  for(std::int32_t i=0;i<count;++i){
   std::uintptr_t menu{};bool visible{};
   if(!reached("registry receiver before IsVisible",s.registry_at,error,static_cast<std::uint32_t>(i),menu)||
      !reached("MenuBase.IsVisible",s.menu_is_visible,error,menu,visible))return false;
   if(!visible)continue;
   if(!reached("registry receiver before virtual1c",s.registry_at,error,static_cast<std::uint32_t>(i),menu)||
      !reached("same visible MenuBase virtual1c",s.menu_virtual1c,error,menu))return false;
  }
  return true;
 }catch(const std::exception& failure){error=failure.what();return false;}
 catch(...){error="Actual MenuManager update provider threw";return false;}
}
}
