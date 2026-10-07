#pragma once
#include "script_command_init_v62.hpp"
#include <exception>
#include <utility>
namespace dh2::loader {
// Main supplies its genuine functional Execute body on SAME class/Data/context.
// Its callback closure must capture App weakly; this binder adds no engine state.
template<class Types>using ActualScriptExecuteLeafV63=std::function<bool(
 typename Types::Application&,const CheckedCommandBorrowV59&,bool,std::int32_t,std::string&)>;

// Compose services BEFORE constructing the ONE actual App-owned V52 manager.
// Allocates no manager/App/VM/global and invokes no engine leaf during binding.
// Existing real command factories are never replaced. Failure preserves out.
template<class Types>bool bind_script_manager_services_v63(
 ScriptManagerServicesV52 independent_cache,
 ScriptInitLeavesV62<Types> actual_main_init,
 ActualScriptExecuteLeafV63<Types> actual_main_execute,
 ScriptManagerServicesV52& out,std::string& error){
 using App=typename Types::Application;
 if(!independent_cache.owner||!independent_cache.open_file){error="Required independent actual cache/stream owner and open_file";return false;}
 if(independent_cache.command_factory){error="Retain existing genuine command_factory; V63 cannot replace it";return false;}
 auto app=actual_main_init.application.lock();if(!app){error="Required SAME actual Application authority at binding";return false;}
 // This catches direct and aliasing ownership cycles. C++ cannot inspect a
 // cache object's internal graph or a std::function's captures; Main must keep
 // those independent/weak as specified in the handoff.
 if(independent_cache.owner.get()==static_cast<void*>(app.get())||
    (!independent_cache.owner.owner_before(app)&&!app.owner_before(independent_cache.owner))){
  error="Cache services.owner must not own or alias the Application that owns ScriptManager";return false;
 }
 if(!actual_main_execute){error="Required genuine Main command Execute leaf";return false;}
 std::weak_ptr<App> weak_app=actual_main_init.application;
 try{
  auto behavior=script_init_behavior_v62(std::move(actual_main_init));
  behavior.execute=[weak_app,execute=std::move(actual_main_execute)](
    const CheckedCommandBorrowV59& same,bool skip,std::int32_t context,std::string& e)->bool {
   auto actual=weak_app.lock();if(!actual){e="Actual Application expired before Main Execute";return false;}
   // V59 supplies checked SAME receiver/header/Data and already pins this SAME
   // weak App authority. Pass original args/result straight to Main's body.
   return execute(*actual,same,skip,context,e);
  };
  auto candidate=bind_script_command_factories_v59(std::move(independent_cache),std::move(behavior));
  out=std::move(candidate);error.clear();return true;
 }catch(const std::exception& e){error=e.what();return false;}
 catch(...){error="Native script service composition failed";return false;}
}
}
