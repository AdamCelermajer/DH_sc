#include "menu_fs_trace_v114.hpp"
#include <exception>
namespace dh2::ui {
bool menu_fs_source_trace_v114(bool manager,const char* command,const char*,
 const MenuFSTraceServicesV114& services,std::string& e)try{
 // MyFSCommand427a54: NULL command returns1 before original Debug.Load.
 // MenuManager has no corresponding NULL-command guard before its own trace.
 if(!manager&&!command){e.clear();return true;}
 auto debug=services.debug;auto files_pin=services.files_owner;
 const auto* files=services.files;
 if(!debug||!files_pin||!files||!services.current){e="Require SAME actual Debug/menu process and file providers";return false;}
 if(!services.current(e))return false;
 const auto loaded=dh2_character_debug_load(debug.get(),files);
 if(loaded<0){e="Original menu Debug.Load failed: "+std::to_string(loaded);return false;}
 if(!services.current(e))return false;
 int queried{};
 {
  // Same local C1/D1 scope as 42e7ac..7c0 /427a84..a98. It owns only this
  // literal key, never command/args storage or any Debug variable/value.
  const std::string key=manager?menu_manager_trace_key_v114:menu_base_trace_key_v114;
  std::uint32_t ignored{};
  queried=dh2_character_debug_get(&ignored,debug.get(),key.c_str(),files);
 }
 if(queried<0){e="Original menu Debug.GetSwitch failed: "+std::to_string(queried);return false;}
 if(!services.current(e))return false;
 e.clear();return true;
}catch(const std::exception& x){e=x.what();return false;}
catch(...){e="Actual menu Debug provider threw";return false;}
bool bind_menu_fs_source_trace_v114(const std::shared_ptr<const MenuFSTraceServicesV114>& actual,
 MenuFSCommandServicesV114& target,std::string& e){
 if(target.source_trace){e.clear();return true;}
 if(!actual||!actual->debug||!actual->files_owner||!actual->files||!actual->current){
  e="Require actual process-owned menu Debug packet before source trace enrollment";return false;
 }
 const std::weak_ptr<const MenuFSTraceServicesV114> weak=actual;
 target.source_trace=[weak](bool manager,const char* cmd,const char* args,std::string& error){
  // Keep the original base NULL return independent of provider admission.
  if(!manager&&!cmd){error.clear();return true;}
  auto locked=weak.lock();if(!locked){error="Expired actual menu Debug process provider";return false;}
  return menu_fs_source_trace_v114(manager,cmd,args,*locked,error);
 };
 e.clear();return true;
}
}
