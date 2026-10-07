#pragma once
#include "script_manager_bind_v63.hpp"
namespace dh2::loader {
namespace complete_script_bind_detail {
inline bool missing(const char* required,std::string& error){
 error=std::string("Complete80 composition requires actual Main Init interface: ")+required;return false;
}
}
// Structural composition only: creates no manager/command/VM and calls no leaf.
// Use once at the SAME App-owned ScriptManager authority construction point.
// Each populated callback must contain the genuine reached Main engine body;
// presence of std::function is not evidence that its effects are implemented.
template<class Types>bool bind_complete_script_manager_services(
 ScriptManagerServicesV52 independent_cache,
 ScriptInitLeavesV62<Types> actual_main_init,
 ActualScriptExecuteLeafV63<Types> actual_main_execute,
 ScriptManagerServicesV52& out,std::string& error){
 // Original domain is exactly0..79. Loader now owns every original Data C1/read
 // schema. V52 prioritizes any installed data_factory over that storage, so an
 // old "unknown kind" extension cannot be silently preserved as complete80.
 // Retain the existing provider/authority elsewhere; this entry point rejects
 // the incompatible composition and never replaces or clears the callback.
 if(independent_cache.data_factory){
  error="Complete80 loader schemas require no overriding data_factory; retain the existing real provider and use its own explicit binding contract";
  return false;
 }
 using complete_script_bind_detail::missing;
 if(!actual_main_init.app_constants2c)return missing("Application.constants2c",error);
 if(!actual_main_init.get_constant)return missing("PyDataConstants.getConstant",error);
 if(!actual_main_init.current_fx_singleton)return missing("current VisualFXManager singleton projection",error);
 if(!actual_main_init.fx_register_set_to_load)return missing("RegisterFXSetToLoad",error);
 if(!actual_main_init.menu_get_instance)return missing("MenuManager.GetInstance",error);
 if(!actual_main_init.menu_by_name)return missing("MenuManager.GetMenuByName",error);
 if(!actual_main_init.app_objects38)return missing("Application.ObjectManager38",error);
 if(!actual_main_init.object_by_name)return missing("ObjectManager.GetObjectByName",error);
 if(!actual_main_init.handle_get_object)return missing("ObjectHandle.GetObject(false)",error);
 if(!actual_main_init.handle_as_character)return missing("ObjectHandle Character conversion",error);
 if(!actual_main_init.character_animator49c)return missing("SAME Character embedded Animator49c",error);
 if(!actual_main_init.animator_add_dict_to_set)return missing("AddAnimDictToSet",error);
 // V63 retains the existing command-factory rejection, independent cache owner
 // and weak App rules, required actual Execute leaf, and output preservation.
 // V62 retains authentic cached/current field loads and five Init body order.
 // Binding failure precedes manager construction; a reached actual leaf failure
 // still goes through V52/V59 original prefix retention and teardown.
 return bind_script_manager_services_v63<Types>(
  std::move(independent_cache),std::move(actual_main_init),
  std::move(actual_main_execute),out,error);
}
}
