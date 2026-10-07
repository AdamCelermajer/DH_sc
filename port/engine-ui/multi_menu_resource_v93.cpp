#include "multi_menu_resource_v93.hpp"
#include <utility>
namespace dh2::ui {
namespace {
template<class F,class... A>bool call(const F& f,const char* name,std::string& e,A&&... a){
 if(!f){e=std::string("Required actual MultiMenu resource provider: ")+name;return false;}
 return f(std::forward<A>(a)...,e);
}
bool retained(const MenuMovieBorrowV58& m,std::string& e){if(m.identity&&!m.actual_owner){e="Required retained actual current MenuFX slot";return false;}return true;}
}
bool multi_menu_unload_swf_v93(const MultiMenuResourceServicesV93& s,std::int32_t id,std::string& e){
 e.clear();if(!s.actual_manager||id<0||id>3){e="Required actual MultiMenuManager/four-slot resource id";return false;}
 const auto slot=std::uint32_t(id);MenuMovieBorrowV58 movie;
 if(!call(s.movie_slot,"nullable primary slot",e,slot,movie)||!retained(movie,e))return false;
 bool resumed{};
 if(s.resume_movie_clear_v98&&!s.resume_movie_clear_v98(slot,resumed,e))return false;
 if(!movie.identity&&!resumed)return true; // Genuine NULL without an admitted prefix still leaves orphaned camera untouched.
 if(!resumed){
 std::int32_t count{};if(!call(s.active_count,"active array count",e,count))return false;
 if(count<0){e="Malformed actual MultiMenu active array count";return false;}
 for(std::int32_t i=count-1;i>=0;--i){
  if(i!=count-1&&!call(s.movie_slot,"primary slot reread",e,slot,movie))return false;
  if(!retained(movie,e))return false;
  std::uintptr_t active{};if(!call(s.active_at,"active render at index",e,std::uint32_t(i),active))return false;
  if(active==movie.identity){
   if(!call(s.remove_active,"active array remove",e,std::uint32_t(i),active))return false;
   if(!call(s.movie_slot,"primary after remove",e,slot,movie)||!retained(movie,e))return false;
  }
 }
 if(!movie.identity){e="Source MenuFX disappeared before virtual Unload";return false;}
 if(!call(s.movie_virtual_c,"MenuFX virtual Unload",e,movie))return false;
 if(!call(s.movie_slot,"primary after Unload",e,slot,movie)||!retained(movie,e))return false;
 if(movie.identity&&!call(s.deleting_virtual4,"MenuFX deleting destructor",e,slot,movie))return false;
 if(!call(s.clear_movie_slot,"primary field clear",e,slot))return false;
 }
 MenuCameraBorrowV93 camera;if(!call(s.camera_slot,"nullable paired MenuFlash2DCamera",e,slot,camera))return false;
 if(camera.identity){
  if(!camera.actual_owner){e="Required retained actual paired camera owner";return false;}
  if(!call(s.deleting_camera_virtual4,"camera deleting destructor",e,slot,camera))return false;
 }
 return call(s.clear_camera_slot,"paired camera field clear",e,slot);
}
}
