#include "menu_manager_unload_v58.hpp"
#include <exception>
#include <utility>
namespace dh2::ui {
namespace {
template<class F,class... Args> bool reached(const char* name,const F& fn,
 std::string& error,Args&&... args){
 if(!fn){error=std::string("Required actual MenuManager.UnloadMenu provider: ")+name;return false;}
 if(!fn(std::forward<Args>(args)...,error)){
  if(error.empty())error=std::string("Actual MenuManager.UnloadMenu failed: ")+name;
  return false;
 }
 return true;
}
}
MenuManagerUnloadV58::MenuManagerUnloadV58(MenuManagerUnloadServicesV58 s):services_(std::move(s)){}
bool MenuManagerUnloadV58::unload(std::int32_t id,std::string& error){
 if(busy_||!services_.actual_manager){error="Required nonreentrant live MenuManager unload owner";return false;}
 struct Guard {bool& busy;explicit Guard(bool& b):busy(b){busy=true;}~Guard(){busy=false;}} guard(busy_);
 try{
  auto& s=services_;
  if(id==1&&!reached("MenuCharMenu_Map.ClearAllIcons",s.clear_map_icons,error))return false;
  MenuMovieBorrowV58 movie;
  // Original cmp/bls is unsigned: negative IDs do not borrow a movie slot.
  if(static_cast<std::uint32_t>(id)<=3&&
     !reached("MultiMenuManager primary movie slot",s.movie_slot,error,static_cast<std::uint32_t>(id),movie))return false;
  if(movie.identity){
   if(!movie.actual_owner){error="Required retained actual movie slot";return false;}
   std::uint32_t index=0;
   for(;;){
    std::uint32_t count{};
    if(!reached("live registry end",s.registry_count,error,count))return false;
    if(index>=count)break;
    MenuReceiverBorrowV58 menu;
    if(!reached("live registry receiver",s.registry_at,error,index,menu))return false;
    if(!menu.identity){error="Required actual nonnull registry MenuBase";return false;}
    if(menu.render!=movie.identity){++index;continue;}
    const auto identity=menu.identity;
    // 42d540 clears +4 BEFORE 42d548 reads ownership and calls virtual4.
    if(!reached("MenuBase.render4=0",s.clear_render,error,identity))return false;
    std::uint8_t owned{};
    if(!reached("same MenuBase.owned7d",s.read_owned7d,error,identity,owned))return false;
    if(owned&&!reached("same MenuBase deleting virtual4",s.deleting_virtual4,error,identity))return false;
    // Deletion may append entries. Erase rereads the current directory and
    // end, then the next iteration stays at the same index (42d564..598).
    if(!reached("live registry erase",s.erase_registry,error,index,identity))return false;
   }
  }
  // Both calls are reached even when the primary movie is NULL/out of range.
  return reached("FlashAnimManager.ResetScanForAnims",s.reset_scan_for_anims,error,movie)&&
         reached("MultiMenuManager.UnloadSWFFile",s.unload_swf_file,error,id);
 }catch(const std::exception& failure){error=failure.what();return false;}
 catch(...){error="Actual MenuManager unload provider threw";return false;}
}
}
