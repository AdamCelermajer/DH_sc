#pragma once
#include <cstdint>
#include <functional>
#include <string>
#include <exception>
namespace dh2::world {
struct ExternalScriptServicesV52 {
 std::function<bool(std::uintptr_t,std::string&)> actual_deleting_destructor;
 // Enter original suffix38efac after old script destruction/field300 clear.
 // Main supplies actual Lua C1/actor binding/path/load body, same global cache.
 std::function<bool(const char*,const char*,std::string&)> actual_new_script_suffix;
};
// Exact38ef60..38efac control/stores. Empty names complete the whole original
// method only AFTER its prior script deleting destructor. This is no script VM.
inline bool game_object_external_script_prefix_v52(std::uintptr_t* same_script300,
 const char* name,const char* directory,const ExternalScriptServicesV52& services,
 bool& whole_method_completed,std::string& error){
 whole_method_completed=false;
 if(!same_script300){error="Required SAME actual GameObject script300 field";return false;}
 try {
  const auto previous=*same_script300;
  if(previous){
   if(!services.actual_deleting_destructor){error="Required original attached LuaScript deleting destructor before empty-name gate";return false;}
   if(!services.actual_deleting_destructor(previous,error))return false;
   *same_script300=0; // Original38ef90 overwrites even callback-changed field.
  }
  if(!name||!name[0]){whole_method_completed=true;error.clear();return true;}
  if(!services.actual_new_script_suffix){error="Required original LuaScript creation/load suffix38efac";return false;}
  if(!services.actual_new_script_suffix(name,directory,error))return false;
  whole_method_completed=true;error.clear();return true;
 }catch(const std::exception& e){error=e.what();return false;}
 catch(...){error="Original external script provider threw";return false;}
}
}
