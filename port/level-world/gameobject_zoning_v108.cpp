#include "gameobject_zoning_v108.hpp"
namespace dh2::world {namespace {
bool valid(const GameObjectZoningBorrowV108& f,std::string& e){
 if(f.owner&&f.identity&&f.zoning2ee&&f.inside2f0&&f.room2f4&&f.visual2d8)return true;
 e="Required SAME GameObject zoning2ee/2f0/2f4/2d8 receiver";return false;
}
bool updating(GameObjectZoningBorrowV108& f,const GameObjectZoningServicesV108& s,std::string& e){
 bool zonable{};if(!s.is_zonable||!s.is_zonable(zonable,e))return false;
 if(!s.set_updating){e="Required actual selected setUpdating3c";return false;}
 return s.set_updating(zonable&&*f.zoning2ee?*f.inside2f0:1,e);
}
}
//Whole38c600. Room2f4 and the receiver survive synchronous remove/add calls;
//the captured Room receiver is passed to RemoveObject before2ee is cleared.
bool gameobject_disable_zoning_v108(GameObjectZoningBorrowV108& f,const GameObjectZoningServicesV108& s,std::string& e){
 if(!valid(f,e))return false;
 if(*f.zoning2ee){
  const auto room=*f.room2f4;
  if(room&&(!s.room_remove||!s.room_remove(room,f.identity,e)))return false;
  if(!s.add_no_room){e="Required actual AddNoRoomObject344184";return false;}
  if(!s.add_no_room(e))return false;
  *f.zoning2ee=0;
 }
 return updating(f,s,e);
}
//Whole38c790. The2ee store precedes RemoveNoRoom, even on later failure.
//The qualified zone callback deliberately does not mean Enabled/Disabled.
bool gameobject_enable_zoning_v108(GameObjectZoningBorrowV108& f,const GameObjectZoningServicesV108& s,std::string& e){
 if(!valid(f,e))return false;
 if(!*f.zoning2ee){
  *f.zoning2ee=1;
  if(!s.remove_no_room){e="Required actual RemoveNoRoomObject3462b8";return false;}
  if(!s.remove_no_room(e))return false;
  const auto room=*f.room2f4;
  if(room){
   if(!s.room_add||!s.room_add(room,f.identity,e))return false;
   std::uint8_t active{};
   if(!s.room_active389||!s.room_active389(*f.room2f4,active,e))return false;
   if(!s.qualified_zone_event||!s.qualified_zone_event(active!=0,e))return false;
  }
 }
 bool zonable{};if(!s.is_zonable||!s.is_zonable(zonable,e))return false;
 const auto visual=*f.visual2d8;
 if(zonable&&visual){
  if(!s.sync_visibility){e="Required actual VisualObject.SyncVisibility4713d0";return false;}
  if(!s.sync_visibility(visual,e))return false;
 }
 return updating(f,s,e);
}
}
