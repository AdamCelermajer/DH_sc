#include "menu_fx_pop_all_v114.hpp"
namespace dh2::ui {
namespace {
bool current(const MenuFXPopAllServicesV114& s,std::string& e){
 if(!s.owner || !s.current){e="MenuFX.PopAll requires the actual retained render owner/current guard";return false;}
 return s.current(e);
}
bool top(MenuStackRenderV1& r,MenuStackMenuV1*& m,std::string& e){
 if(!r.count || r.count>r.capacity || !r.states || !r.states[r.count-1]){
  e="MenuFX.PopAll reached an invalid source states114/count118 domain";return false;
 }
 m=r.states[r.count-1];return true;
}
}
bool menu_fx_pop_all_v114(MenuStackRenderV1& r,const MenuFXPopAllServicesV114& s,std::string& e){
 if(!current(s,e))return false;
 while(static_cast<std::int32_t>(r.count)>0){
  MenuStackMenuV1* receiver{};
  if(!top(r,receiver,e))return false;
  if(!s.hide_virtual10){e="MenuFX.PopAll requires the reached original receiver virtual10";return false;}
  if(!s.hide_virtual10(*receiver,e) || !current(s,e))return false;
  // Hide can mutate the stack: native rereads both states114 and count118.
  MenuStackMenuV1* after{};
  if(!top(r,after,e))return false;
  after->status=2; // native state58, after Hide, before count decrement
  const auto next=r.count-1;
  if(!next){r.count=0;break;}
  if(next>r.capacity){
   if(!s.resize_states){e="MenuFX.PopAll reached the original states resize without its provider";return false;}
   if(!s.resize_states(r,next+next/2,e) || !current(s,e))return false;
  }
  r.count=next;
 }
 // SetContext is exactly a field40 store. Root is reread at the end.
 r.context=r.root;
 return true;
}
}
