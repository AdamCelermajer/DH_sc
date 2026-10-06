#include "authored_menu_deadzones_v3.hpp"
#include <cstring>
#include <algorithm>
namespace dh2::ui {
namespace {
bool collect(AuthoredMenuCharacterBorrowV3* c,const char* match,std::uint32_t flags,
 std::vector<AuthoredMenuCharacterBorrowV3*>& out,
 std::vector<std::uintptr_t>& ancestors,std::string& error) {
 if(!c||!c->identity||!c->name){error="Required live source menu character/name";return false;}
 if(std::find(ancestors.begin(),ancestors.end(),c->identity)!=ancestors.end()){
  error="Invalid cyclic source menu display list";return false;
 }
 // Original 7a8acc: sprite focus gate precedes visibility gate and stops the
 // entire subtree. Name filtering applies to insertion, not descent.
 if(c->sprite&&(flags&2)&&!c->focus_enabled)return true;
 if((flags&1)&&!c->visible)return true;
 if((!match||std::strstr(c->name,match))&&(!(flags&4)||*c->name))out.push_back(c);
 if(!c->sprite)return true;
 ancestors.push_back(c->identity);
 for(auto* child:c->children)if(!collect(child,match,flags,out,ancestors,error)){
  ancestors.pop_back();return false;
 }
 ancestors.pop_back();return true;
}
}
bool authored_menu_collect_characters_v3(AuthoredMenuCharacterBorrowV3* root,
 const char* match,std::uint32_t flags,std::vector<AuthoredMenuCharacterBorrowV3*>& out,std::string& error){
 std::vector<std::uintptr_t> ancestors;
 return collect(root,match,flags,out,ancestors,error);
}
AuthoredMenuDeadZoneV3 authored_menu_absolute_rectangle_v3(
 const AuthoredMenuDeadZoneV3& r,float x,float y){
 return {(x+r.xmin)/20.f,(x+r.xmax)/20.f,(y+r.ymin)/20.f,(y+r.ymax)/20.f};
}
bool authored_menu_register_deadzones_v3(std::uint8_t& registered,
 AuthoredMenuCharacterBorrowV3* root,const AuthoredMenuDeadZoneServicesV3& services,
 std::vector<AuthoredMenuDeadZoneV3>& out,std::string& error){
 if(registered||!root)return true;
 registered=1;
 if(!services.debug||!services.debug(error)){
  if(error.empty())error="Required MenuBase dead-zone Debug service";return false;
 }
 std::vector<AuthoredMenuCharacterBorrowV3*> found;
 if(!authored_menu_collect_characters_v3(root,"deadzone_",0,found,error))return false;
 for(auto* c:found){
  AuthoredMenuDeadZoneV3 rectangle;
  if(!services.bounds||!services.bounds(*c,rectangle,error)){
   if(error.empty())error="Required source absolute dead-zone rectangle";return false;
  }
  out.push_back(rectangle);
 }
 return true;
}
}
