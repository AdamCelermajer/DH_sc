#pragma once
#include "menu_stack_owner_v1.hpp"
#include <functional>
namespace dh2::ui {
//Borrowed actual menu identity. Never looks the name up again for Push/Pop.
//The caller supplies the real receiver/context lifetime and IsVisible getter;
//adapter projection storage alone does not own the MenuBase/UI backend.
struct CapturedMenuLeaseV101 {
 std::shared_ptr<MenuStackOwnerV1> manager;
 std::shared_ptr<void> menu_storage,render_storage,receiver_owner,context_owner;
 MenuStackMenuV1* menu{};MenuStackServicesV1 services{};
 std::function<bool(std::uintptr_t,bool&,std::string&)> source_is_visible;
 std::uintptr_t identity()const noexcept{return menu?menu->identity:0;}
 bool visible(bool& out,std::string& error)const{
  if(!menu||!receiver_owner||!context_owner||!source_is_visible){error="Required actual captured MenuBase.IsVisible receiver";return false;}
  return source_is_visible(menu->identity,out,error);
 }
 bool push(std::string& error)const{return invoke(false,error);}
 bool pop(std::string& error)const{return invoke(true,error);}
private:
 bool invoke(bool pop,std::string& error)const{
  if(!manager||!manager->view()||!menu||!menu_storage||!receiver_owner||!context_owner||!services.invoke){
   error="Required SAME captured menu/manager/native stack backend";return false;
  }
  const int status=pop?dh2_menu_stack_manager_pop_v1(manager->view(),menu,&services):dh2_menu_stack_manager_push_v1(manager->view(),menu,&services);
  if(status){error="Captured source menu stack operation failed: "+std::to_string(status);return false;}
  error.clear();return true;
 }
};
inline bool capture_menu_lease_v101(std::shared_ptr<MenuStackOwnerV1> manager,const char* name,
 std::shared_ptr<void> receiver,std::shared_ptr<void> context,MenuStackServicesV1 services,
 std::function<bool(std::uintptr_t,bool&,std::string&)> visible,CapturedMenuLeaseV101& out,std::string& error){
 out={};if(!manager||!manager->view()||!name){error="Required actual MenuManager.GetMenuByName directory";return false;}
 auto* menu=manager->menu(name);if(!menu){error.clear();return true;} //Genuine observed NULL.
 if(!receiver||!context||!services.invoke||!visible){error="Required native captured menu receiver/backend lifetimes";return false;}
 CapturedMenuLeaseV101 next;next.manager=std::move(manager);next.menu=menu;next.receiver_owner=std::move(receiver);
 next.context_owner=std::move(context);next.services=services;next.source_is_visible=std::move(visible);
 if(!next.manager->capture_storage_v101(menu,next.menu_storage,next.render_storage,error))return false;
 out=std::move(next);error.clear();return true;
}
}
