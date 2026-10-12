#pragma once
#include "script_manager_owner_v52.hpp"
#include "stage_loader_v51_config_scripts.hpp"
namespace dh2::loader {
// Main passes its ONE authoritative application-owned ScriptManager here.
// This creates no owner/global/VM, and does not supply missing command bodies.
inline ScriptManagerServicesV51 bind_script_manager_v52(const std::shared_ptr<ScriptManagerOwnerV52>& actual){
 if(!actual)return {};auto identity=actual->identity();return {actual,identity,
 [actual,identity](std::uintptr_t id,std::string& e){if(id!=identity){e="Foreign ScriptManager Unload identity";return false;}return actual->unload_all(e);},
 [actual,identity](std::uintptr_t id,const char* file,bool common,std::string& e){if(id!=identity){e="Foreign ScriptManager commands identity";return false;}return actual->load_commands(file,common,e);},
 [actual,identity](std::uintptr_t id,const char* file,bool common,std::string& e){if(id!=identity){e="Foreign ScriptManager names identity";return false;}return actual->load_names(file,common,e);}};
}
}
