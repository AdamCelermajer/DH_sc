#include "source_campaign_native_behavior_v96.hpp"

namespace dh::foundation::cinematics {
namespace {
using namespace dh2::loader;

bool missing(const char* name,std::string& error){
    error=std::string("Required same native campaign ")+name;
    return false;
}

bool same_parsed_command(const ScriptManagerOwnerV52& manager,
                         const CheckedCommandBorrowV59& borrow,
                         std::string& error){
    if(!borrow.actual_receiver||!borrow.actual_data||
       borrow.identity!=borrow.actual_receiver->identity()||
       !borrow.descriptor||!borrow.skip4||!borrow.kind8||!borrow.data_c)
        return missing("checked parsed command borrow",error);

    CheckedCommandBorrowV59 actual;
    if(!borrow.actual_receiver->checked_data_borrow(actual,error))return false;
    if(actual.actual_receiver!=borrow.actual_receiver||
       actual.actual_data!=borrow.actual_data||actual.identity!=borrow.identity||
       actual.descriptor!=borrow.descriptor||actual.skip4!=borrow.skip4||
       actual.kind8!=borrow.kind8||actual.data_c!=borrow.data_c)
        return missing("unchanged canonical receiver/data borrow",error);

    for(const auto& script:manager.commands()){
        if(!script.storage8)continue;
        for(const auto& slot:*script.storage8){
            const auto& command=slot.command;
            if(slot.storage_released||!command.retained_data||!command.kind8||
               !command.skip4||!command.data_c||!command.actual_owner||
               !command.canonical_receiver_v96)continue;
            if(command.identity==borrow.identity&&
               command.actual_owner.get()==borrow.actual_receiver.get()&&
               command.canonical_receiver_v96==borrow.actual_receiver&&
               command.retained_data->get()==borrow.actual_data.get()&&
               command.skip4==borrow.skip4&&command.kind8==borrow.kind8&&
               command.data_c==borrow.data_c&&
               *command.kind8==borrow.actual_data->source_kind()&&
               *command.data_c==borrow.actual_data->identity()){
                error.clear();return true;
            }
        }
    }
    return missing("receiver/data assignment in the same ScriptManager",error);
}

struct LiveBorrowV96 {
    std::weak_ptr<ScriptManagerOwnerV52> manager;
    std::weak_ptr<void> scheduler_owner;
    std::uintptr_t manager_identity{};
    std::uintptr_t scheduler_identity{};
    std::weak_ptr<void> application_owner;

    bool check(const CheckedCommandBorrowV59& command,
               std::shared_ptr<ScriptManagerOwnerV52>& manager_pin,
               std::string& error)const{
        manager_pin=manager.lock();
        const auto scheduler=scheduler_owner.lock();
        const auto application=application_owner.lock();
        if(!manager_pin||manager_pin->identity()!=manager_identity)
            return missing("same live ScriptManager owner",error);
        if(!scheduler||reinterpret_cast<std::uintptr_t>(scheduler.get())!=scheduler_identity)
            return missing("same live scheduler owner",error);
        if(!application)return missing("same live Application command owner",error);
        return same_parsed_command(*manager_pin,command,error);
    }
};
}

bool decorate_source_campaign_native_behavior_v96(
    const std::shared_ptr<ScriptManagerOwnerV52>& manager,
    const ScriptSchedulerServicesV96& scheduler,
    const SourceCampaignNativeCommandHooksV96& hooks,
    ScriptCommandBehaviorV59& behavior,std::string& error){
    error.clear();
    if(!manager)return missing("Application ScriptManager",error);
    if(!scheduler.owner)return missing("source scheduler owner",error);
    if(!hooks.owner)return missing("native command provider owner",error);
    if(!hooks.execute&&!hooks.blocking&&!hooks.update)
        return missing("native Execute/IsBlocking/Update provider",error);
    const auto application=behavior.actual_owner.lock();
    if(!application||!behavior.execute||!behavior.blocking)
        return missing("existing native command behavior to decorate",error);

    const LiveBorrowV96 live{manager,scheduler.owner,manager->identity(),
        reinterpret_cast<std::uintptr_t>(scheduler.owner.get()),behavior.actual_owner};
    auto prior_execute=behavior.execute;
    auto prior_blocking=behavior.blocking;
    auto prior_update=behavior.update;
    const auto provider=hooks;

    behavior.execute=[live,provider,prior_execute](const auto& command,bool skip,
        std::int32_t module,std::string& e){
        std::shared_ptr<ScriptManagerOwnerV52> manager_pin;
        if(!live.check(command,manager_pin,e))return false;
        if(provider.execute){
            bool handled=false;
            if(!provider.execute(command,skip,module,handled,e))return false;
            if(!live.check(command,manager_pin,e))return false;
            if(handled){e.clear();return true;}
        }
        if(!prior_execute)return missing("existing reached Execute fallback",e);
        return prior_execute(command,skip,module,e);
    };
    behavior.blocking=[live,provider,prior_blocking](const auto& command,
        bool& blocking,std::string& e){
        std::shared_ptr<ScriptManagerOwnerV52> manager_pin;
        if(!live.check(command,manager_pin,e))return false;
        if(provider.blocking){
            bool handled=false;
            if(!provider.blocking(command,handled,blocking,e))return false;
            if(!live.check(command,manager_pin,e))return false;
            if(handled){e.clear();return true;}
        }
        if(!prior_blocking)return missing("existing reached IsBlocking fallback",e);
        return prior_blocking(command,blocking,e);
    };
    if(provider.update){
        behavior.update=[live,provider,prior_update](const auto& command,std::string& e){
            std::shared_ptr<ScriptManagerOwnerV52> manager_pin;
            if(!live.check(command,manager_pin,e))return false;
            bool handled=false;
            if(!provider.update(command,handled,e))return false;
            if(!live.check(command,manager_pin,e))return false;
            if(handled){e.clear();return true;}
            if(!prior_update)return missing("existing reached Update fallback",e);
            return prior_update(command,e);
        };
    }else if(prior_update){
        behavior.update=[live,prior_update](const auto& command,std::string& e){
            std::shared_ptr<ScriptManagerOwnerV52> manager_pin;
            return live.check(command,manager_pin,e)&&prior_update(command,e);
        };
    }
    error.clear();return true;
}

dh2::android_ui::SourceScriptBehaviorDecoratorV96
source_campaign_native_behavior_decorator_v96(
    SourceCampaignNativeCommandHooksV96 hooks){
    return [hooks=std::move(hooks)](const auto& manager,const auto& scheduler,
        auto& behavior,std::string& error){
        return decorate_source_campaign_native_behavior_v96(
            manager,scheduler,hooks,behavior,error);
    };
}

} // namespace dh::foundation::cinematics
