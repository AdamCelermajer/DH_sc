#include "menu_main_menu_state_v114.hpp"
#include <exception>
#include <utility>
namespace dh2::ui {
namespace {
template<class F> bool delivered(F&& f,std::string& first,std::string& e){
 if(!first.empty()){e=first;return false;}std::string local;
 bool ok=false;try{ok=f(local);}catch(const std::exception& x){local=x.what();}catch(...){local="Native menu source leaf threw";}
 if(!first.empty()){e=first;return false;}
 if(!ok){first=local.empty()?"Required actual menu source leaf":std::move(local);e=first;return false;}
 return true;
}
bool guard(const std::function<bool(std::string&)>& current,std::string& first,std::string& e){
 return delivered([&](auto& x){if(!current){x="Required SAME current process menu owner";return false;}return current(x);},first,e);
}
template<class F> bool leaf(F&& f,const std::function<bool(std::string&)>& current,std::string& first,std::string& e){
 if(!guard(current,first,e)||!delivered(std::forward<F>(f),first,e))return false;
 return guard(current,first,e);
}
}
bool GSFlashMenuEntryV114::enter(GSFlashMenuFieldsV114& f,const GSFlashMenuServicesV114& s,std::string& e){
 if(busy_){if(failure_.empty())failure_="Recursive GSFlashMenu.Ctor delivery";e=failure_;return false;}
 if(!failure_.empty()){e=failure_;return false;}
 if(complete_){phase_=0;complete_=false;}
 busy_=true;struct Busy{bool& b;~Busy(){b=false;}} busy{busy_};
 if(!s.owner){failure_="Required independent GSFlashMenu provider";e=failure_;return false;}
 for(;;){
  if(!guard(s.current,failure_,e))return false;
  switch(phase_){
  case 0:
   if(!leaf([&](auto& x){if(!s.debug_load){x="Required DebugSwitches.Load";return false;}const bool ok=s.debug_load(x);if(ok)phase_=1;return ok;},s.current,failure_,e))return false;
   phase_=1;break;
  case 1:{bool ignored{};
   if(!leaf([&](auto& x){if(!s.debug_switch){x="Required actual GSFlashMenu Debug switch";return false;}const bool ok=s.debug_switch("isTracingGSFlashMenu",ignored,x);if(ok)phase_=2;return ok;},s.current,failure_,e))return false;
   phase_=2;break;}
  case 2:{std::shared_ptr<void> pin;std::uintptr_t manager{};
   if(!leaf([&](auto& x){return s.menu_manager?s.menu_manager(pin,manager,x):(x="Required process MenuManager.GetInstance",false);},s.current,failure_,e))return false;
   if(!pin||!manager){failure_="Native MenuManager.GetInstance returned NULL";e=failure_;return false;}
   f.manager4=manager;phase_=3;break;}
  case 3:
   if(!f.source29){phase_=6;break;}
   if(!leaf([&](auto& x){if(!s.load_main_menu){x="Required genuine MenuManager.LoadMainMenu4324dc";return false;}const bool ok=s.load_main_menu(f.manager4,x);if(ok)phase_=4;return ok;},s.current,failure_,e))return false;
   phase_=4;break;
  case 4:{std::shared_ptr<void> pin;std::uintptr_t manager{};
   if(!leaf([&](auto& x){return s.menu_manager?s.menu_manager(pin,manager,x):(x="Required fresh process MenuManager.GetInstance",false);},s.current,failure_,e))return false;
   if(!pin||!manager){failure_="Native fresh MenuManager.GetInstance returned NULL";e=failure_;return false;}
   // The second GetInstance is used only for GetMenu. Original4 is not rewritten.
   std::uintptr_t menu{};
   if(!leaf([&](auto& x){return s.get_menu?s.get_menu(manager,"menu_MainMenu",menu,x):(x="Required MenuManager.GetMenuByName",false);},s.current,failure_,e))return false;
   f.request_c=menu;f.source29=0;phase_=6;break;}
  case 6:
   if(f.current8&&!leaf([&](auto& x){if(!s.push_menu){x="Required SAME MultiMenu.virtual34";return false;}const bool ok=s.push_menu(f.manager4,f.current8,x);if(ok)phase_=7;return ok;},s.current,failure_,e))return false;
   phase_=7;break;
  case 7:f.source28=0;complete_=true;e.clear();return true;
  default:failure_="Invalid retained GSFlashMenu entry continuation";e=failure_;return false;
  }
 }
}
bool GSFlashMenuEntryV114::arm_resume(std::string& e){if(busy_){e="Cannot arm active GS entry";return false;}failure_.clear();e.clear();return true;}
bool GSFlashMenuEntryV114::resume(GSFlashMenuFieldsV114& f,const GSFlashMenuServicesV114& s,std::string& e){
 if(busy_){e="Cannot resume active GSFlashMenu entry";return false;}
 if(failure_.empty()){e="No failed retained GSFlashMenu entry";return false;}
 failure_.clear();return enter(f,s,e);
}
bool gs_flash_menu_request_v114(GSFlashMenuFieldsV114& f,const GSFlashMenuServicesV114& s,std::string& e){
 std::string first;if(!s.owner||!guard(s.current,first,e)){if(e.empty())e="Required actual retained GSFlashMenu";return false;}
 if(!f.request_c||f.request_c==f.current8){e.clear();return true;}
 if(f.current8&&!leaf([&](auto& x){return s.pop_menu?s.pop_menu(f.manager4,f.current8,false,x):(x="Required SAME MultiMenu.virtual3c",false);},s.current,first,e))return false;
 // Authentic reload after old-menu Pop callback; no stale request snapshot.
 const auto next=f.request_c;f.request_c=0;f.current8=next;
 return leaf([&](auto& x){return s.push_menu?s.push_menu(f.manager4,next,x):(x="Required SAME MultiMenu.virtual34",false);},s.current,first,e);
}
bool MenuManagerResetV114::reset(const MenuManagerResetServicesV114& s,std::string& e){
 if(busy_){if(failure_.empty())failure_="Recursive MenuManager.Reset";e=failure_;return false;}
 if(!failure_.empty()){e=failure_;return false;}
 if(complete_&&!resume_receipt_){phase_=slot_=index_=0;count_=0;complete_=false;}
 busy_=true;struct Busy{bool& b;~Busy(){b=false;}} busy{busy_};
 if(!s.owner){failure_="Required independent MenuManager.Reset provider";e=failure_;return false;}
 if(resume_receipt_){if(!guard(s.current,failure_,e))return false;resume_receipt_=false;e.clear();return true;}
 for(;;){
  if(!guard(s.current,failure_,e))return false;
  if(phase_==0){
   if(slot_==4){phase_=1;continue;}
   MenuStackRenderV1* render{};std::shared_ptr<void> pin;
   if(!leaf([&](auto& x){return s.render_slot?s.render_slot(slot_,render,pin,x):(x="Required SAME MultiMenu134 slot",false);},s.current,failure_,e))return false;
   if(render){
    if(!pin){failure_="Positive Reset render lacks SAME resource pin";e=failure_;return false;}
    if(!leaf([&](auto& x){if(!s.pop_all){x="Required MenuFX.PopAll7abb20";return false;}const bool ok=s.pop_all(*render,x);if(ok)++slot_;return ok;},s.current,failure_,e))return false;
   }
   else ++slot_;continue;
  }
  if(phase_==1){
   if(!leaf([&](auto& x){return s.registry_count?s.registry_count(count_,x):(x="Required MenuManager.GetNumMenus",false);},s.current,failure_,e))return false;
   phase_=2;continue;
  }
  if(phase_==2){
   if(count_<=0||index_>=static_cast<unsigned>(count_)){phase_=3;continue;}
   MenuStackMenuV1* menu{};std::shared_ptr<void> pin;bool visible{};
   if(!leaf([&](auto& x){return s.registry_at?s.registry_at(index_,menu,pin,x):(x="Required live Reset registry64",false);},s.current,failure_,e))return false;
   if(!menu||!pin){failure_="Native Reset registry entry is invalid";e=failure_;return false;}
   if(!leaf([&](auto& x){return s.is_visible?s.is_visible(*menu,visible,x):(x="Required MenuBase.IsVisible74",false);},s.current,failure_,e))return false;
   if(visible){
    // Original reloads registry64[index] after IsVisible, then virtual10.
    menu=nullptr;pin.reset();
    if(!leaf([&](auto& x){return s.registry_at?s.registry_at(index_,menu,pin,x):(x="Required current Reset registry64",false);},s.current,failure_,e))return false;
    if(!menu||!pin){failure_="Native Reset visible registry entry replaced with NULL";e=failure_;return false;}
    if(!leaf([&](auto& x){if(!s.hide_virtual10){x="Required selected MenuBase.virtual10";return false;}const bool ok=s.hide_virtual10(*menu,x);if(ok)++index_;return ok;},s.current,failure_,e))return false;
   }
   else ++index_;continue;
  }
  if(phase_==3){
   if(!leaf([&](auto& x){if(!s.store_listener88){x="Required same manager listener88";return false;}const bool ok=s.store_listener88(x);if(ok)complete_=true;return ok;},s.current,failure_,e))return false;
   complete_=true;e.clear();return true;
  }
  failure_="Invalid retained MenuManager.Reset continuation";e=failure_;return false;
 }
}
bool MenuManagerResetV114::arm_resume(std::string& e){if(busy_){e="Cannot arm active Reset";return false;}resume_receipt_=complete_&&!failure_.empty();failure_.clear();e.clear();return true;}
bool MenuManagerResetV114::resume(const MenuManagerResetServicesV114& s,std::string& e){
 if(busy_){e="Cannot resume active MenuManager.Reset";return false;}
 if(failure_.empty()){e="No failed retained MenuManager.Reset";return false;}
 failure_.clear();if(complete_){if(!guard(s.current,failure_,e))return false;e.clear();return true;}return reset(s,e);
}
}
