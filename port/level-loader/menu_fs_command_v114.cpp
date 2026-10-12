#include "menu_fs_command_v114.hpp"
#include <cstring>
#include <exception>
#include <utility>
namespace dh2::ui { namespace {
struct Delivery {
 MenuFSCommandServicesV114& s;std::string& e;
 bool current(){if(!s.owner||!s.current){e="Required actual existing menu FS authority";return false;}return s.current(e);}
 template<class F,class... A>bool call(const char* name,const F& f,A&&... a){
  if(!current())return false;if(!f){e=name;return false;}
  bool ok{};try{ok=f(std::forward<A>(a)...,e);}catch(const std::exception& x){e=x.what();return false;}catch(...){e=name;return false;}
  return ok&&current();
 }
 // Native FS dispatch can legitimately switch the current GameState. Its
 // terminal call returns without any later old-World field access.
 template<class F,class... A>bool terminal(const char* name,const F& f,A&&... a){
  if(!current())return false;if(!f){e=name;return false;}
  try{return f(std::forward<A>(a)...,e);}catch(const std::exception& x){e=x.what();return false;}catch(...){e=name;return false;}
 }
 bool capture(std::uintptr_t id,std::shared_ptr<void>& pin){
  if(!id||!call("Required SAME captured roster MenuBase",s.capture_menu,id,pin))return false;
  if(!pin){e="Required genuine existing MenuBase lifetime pin";return false;}return true;
 }
};
}
bool render_fs_command_v114(std::uintptr_t handler,
 const std::function<bool(std::uintptr_t,const char*,const char*,std::string&)>& invoke,
 const char* command,const char* args,std::string& e){
 if(!handler){e.clear();return true;}if(!invoke){e="Required actual RenderFX.fc virtual4 handler";return false;}
 try{return invoke(handler,command,args,e);}catch(const std::exception& x){e=x.what();return false;}catch(...){e="RenderFX FS handler threw";return false;}
}
bool menu_fx_fs_command_v114(MenuStackRenderV1& render,const char* command,const char* args,MenuFSCommandServicesV114& s,std::string& e){
 Delivery d{s,e};if(!d.current())return false;
 std::int32_t count{};std::memcpy(&count,&render.count,4);
 if(count<=0){e.clear();return true;}
 if(!render.states||render.count>render.capacity){e="Required actual MenuFX states114/count118 storage";return false;}
 auto* menu=render.states[render.count-1];if(!menu){e="Original positive MenuFX top state is NULL";return false;}
 const auto id=menu->identity;std::shared_ptr<void> pin;if(!d.capture(id,pin))return false;
 return d.terminal("Required top MenuBase virtual30 OnFSCommand",s.virtual30,id,command,args);
}
bool menu_manager_fs_command_v114(MenuStackV1& stack,const char* command,const char* args,MenuFSCommandServicesV114& s,std::string& e){
 Delivery d{s,e};if(!d.call("Required original MenuManager FS trace",s.source_trace,true,command,args))return false;
 MenuStackMenuV1* menu{};if(dh2_menu_stack_find_v1(&stack,"menu_HUD_0",&menu)||!menu){e="Required actual MenuManager menu_HUD_0 FS receiver";return false;}
 const auto id=menu->identity;std::shared_ptr<void> pin;if(!d.capture(id,pin))return false;
 bool ignored{};return d.terminal("Required HUD MenuBase virtual38 MyFSCommand",s.virtual38,id,command,args,ignored);
}
bool menu_base_on_fs_command_v114(std::uintptr_t id,const char* command,const char* args,MenuFSCommandServicesV114& s,std::string& e){
 Delivery d{s,e};std::shared_ptr<void> pin;if(!d.capture(id,pin))return false;
 bool ignored{};return d.terminal("Required actual MenuBase virtual38",s.virtual38,id,command,args,ignored);
}
bool menu_base_my_fs_command_v114(MenuStackMenuV1& menu,const char* command,const char* args,MenuFSCommandServicesV114& s,bool& handled,std::string& e){
 Delivery d{s,e};if(!d.current())return false;handled=false;
 if(!command){handled=true;e.clear();return true;} //427a54 before source trace.
 const auto id=menu.identity;std::shared_ptr<void> pin;if(!d.capture(id,pin))return false;
 if(!d.call("Required original MenuBase FS trace",s.source_trace,false,command,args))return false;
 if(!std::strcmp(command,"LoadLevel")){
  if(!args){e="Native FS_LoadLevel NULL args outside source safe domain";return false;}
  if(!*args){e.clear();return true;}
  auto* render=menu.render;if(!render){e="Required actual MenuBase render4 for PopAll";return false;}
  // Original effects occur in this order. Completion is handled only after
  // the genuine application call; callback presence is not a completed load.
  if(!d.call("Required actual MenuFX.PopAll7abb20",s.pop_all_render,*render))return false;
  loader::ApplicationLoadLevelArgumentsV114 input{args,0,0,false,true,0,false,0,0};
  if(!d.terminal("Required actual Application.LoadLevel32bdc8 confirmation",s.application_load_level,input))return false;
  handled=true;e.clear();return true;
 }
 if(!std::strcmp(command,"LoadLevel2")){
  if(!args||!*args){e.clear();return true;}
  std::shared_ptr<void> tables_pin;const data::LevelTables* tables{};
  if(!d.call("Required actual LevelList for FS_LoadLevel2",s.level_tables,tables_pin,tables))return false;
  if(!tables_pin||!tables){e="Required SAME original LevelList lifetime";return false;}
  std::int32_t row{-1};const data::LevelRecord* record{};
  if(!loader::borrow_area_transition_level_v114(*tables,args,row,record,e))return false;
  if(row==-1||!record){e.clear();return true;}
  bool ignored{};
  if(!d.terminal("Required same MenuBase virtual38 LoadLevel forwarding",s.virtual38,id,"LoadLevel",record->file.c_str(),ignored))return false;
  handled=true;e.clear();return true; //Native outer returns1 independently.
 }
 // Existing source native-function table remains the authority for all other
 // names. Never report an unimplemented command as delivered/handled.
 return d.terminal("Required existing MenuBase FS native function table",s.remaining_my_fs,id,command,args,handled);
}
}
