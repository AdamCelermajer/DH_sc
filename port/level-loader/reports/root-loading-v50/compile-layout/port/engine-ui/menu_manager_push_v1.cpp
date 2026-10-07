#include "menu_manager_push_v1.hpp"
#include <cstring>
namespace {
using namespace dh2::ui;
template<class T> bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
}
extern "C" dh2::ui::MenuManagerPushEntry24V1* dh2_menu_manager_find_v1(const dh2::ui::MenuManagerPushState32V1* state,const char* name) noexcept {
 if(!aligned(state)||!name||state->reserved||(state->count&&!aligned(state->entries)))return nullptr;
 for(std::uint32_t i=0;i<state->count;++i){
  auto* entry=state->entries[i];if(!aligned(entry)||!entry->name)return nullptr;
  if(std::strcmp(name,entry->name)==0)return entry;
 }
 return nullptr;
}
extern "C" int dh2_menu_manager_push_v1(dh2::ui::MenuManagerPushState32V1* state,dh2::ui::MenuManagerPushEntry24V1* menu,const dh2::ui::MenuManagerPushServices16V1* services) noexcept {
 using namespace dh2::ui;using Op=MenuManagerPushOperationV1;
 if(!aligned(state)||state->reserved)return -1;
 if(!menu)return 0;
 if(!aligned(menu)||!menu->identity||!state->manager||!state->multi_manager)return -1;
 if(!aligned(services)||!services->invoke)return -2;
 const auto provider=*services;
 const auto manager=state->manager,identity=menu->identity;
 bool rejected=false;
 auto call=[&](Op op,std::uintptr_t receiver,const char* text=nullptr){
  MenuManagerPushResponse16V1 out{};
  if(!rejected){const MenuManagerPushRequest32V1 q{op,0,menu,text,receiver};
   if(provider.invoke(provider.context,state,&q,&out)!=1)rejected=true;}
  return out;
 };
 const auto valid=call(Op::is_valid,identity).value;
 if(rejected)return -2;
 if(valid){
  const auto present=call(Op::is_in_stack,state->multi_manager).value;
  if(rejected)return -2;
  if(!present){
   call(Op::reset_touch,manager);
   call(Op::process_touch_events,manager);
   call(Op::multi_push,state->multi_manager);
   call(Op::debug_load,manager);
   // Original queries this switch after the push; its result is discarded.
   call(Op::debug_get,manager,"isTracingMenuManager");
   if(rejected)return -2;
  }
 }
 const auto renderer=menu->renderer;
 const auto hud=call(Op::hud_root,manager).identity;
 if(rejected)return -2;
 // Source re-reads MenuBase.renderer after push/debug callbacks.
 if(renderer==hud)call(Op::register_listener,manager);
 return rejected?-2:0;
}
