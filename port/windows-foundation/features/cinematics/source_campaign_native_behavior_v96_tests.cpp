#include "source_campaign_native_behavior_v96.hpp"
#include "../../../level-loader/script_command_receivers_v59.hpp"
#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace dh2::loader;
using namespace dh::foundation::cinematics;

namespace {
void append_u32(std::vector<std::uint8_t>& bytes,std::uint32_t value){
    for(unsigned i=0;i<4;++i)bytes.push_back(std::uint8_t(value>>(8*i)));
}
}

int main(){
    std::string error;
    auto source_file=std::make_shared<std::vector<std::uint8_t>>();
    // One script with an actual dynamic IsBlocking PlayCamera and a Wait
    // command whose source Update is virtual and reaches the behavior hook.
    // Data fields: PlayCamera kind=5, animation=88, wait=1; Wait kind=26,
    // duration=10.
    for(auto word:{1u,2u,5u,88u})append_u32(*source_file,word);
    source_file->push_back(1);
    for(auto word:{26u,10u})append_u32(*source_file,word);
    auto manager_service_owner=std::make_shared<int>(1);
    auto app_owner=std::make_shared<int>(2);
    auto scheduler_owner=std::make_shared<int>(3);
    auto provider_owner=std::make_shared<int>(4);
    std::vector<std::string> order;
    std::uintptr_t receiver_identity{};
    std::uintptr_t data_identity{};
    std::uintptr_t wait_receiver_identity{};
    std::uintptr_t wait_data_identity{};
    bool saw_skip=false;
    std::int32_t saw_module=-1;

    ScriptCommandBehaviorV59 original;
    original.actual_owner=app_owner;
    original.execute=[&](const CheckedCommandBorrowV59& command,bool skip,
                         std::int32_t module,std::string& e){
        order.emplace_back("native.execute");
        assert(command.identity==command.actual_receiver->identity());
        assert(command.actual_data->identity()!=0);
        saw_skip=skip;saw_module=module;e.clear();return true;
    };
    original.blocking=[&](const CheckedCommandBorrowV59& command,bool& out,
                          std::string& e){
        order.emplace_back("native.blocking");
        assert(command.identity==command.actual_receiver->identity());
        assert(command.actual_data->identity()!=0);
        out=true;e.clear();return true;
    };
    original.update=[&](const CheckedCommandBorrowV59& command,std::string& e){
        order.emplace_back("native.update");
        assert(command.identity==command.actual_receiver->identity());
        assert(command.actual_data->identity()!=0);
        e.clear();return true;
    };

    ScriptManagerServicesV52 services;
    services.owner=manager_service_owner;
    services.open_file=[source_file](const std::string&,ScriptFileV52& out,
                                     std::string& e){
        out.owner=source_file;out.bytes=source_file->data();
        out.size=source_file->size();out.found=true;e.clear();return true;
    };
    services=bind_script_command_factories_v59(std::move(services),original);
    auto manager=std::make_shared<ScriptManagerOwnerV52>(std::move(services));
    if(!manager->load_commands("authored",false,error)){
        std::cerr<<"load_commands: "<<error<<"\n";return 1;
    }
    assert(manager->commands().size()==1&&manager->commands()[0].storage8);
    const auto& slot=(*manager->commands()[0].storage8)[0];
    const auto receiver=slot.command.canonical_receiver_v96;
    assert(receiver&&slot.command.retained_data&&*slot.command.retained_data);
    receiver_identity=receiver->identity();
    data_identity=(*slot.command.retained_data)->identity();
    const auto& wait_slot=(*manager->commands()[0].storage8)[1];
    const auto wait_receiver=wait_slot.command.canonical_receiver_v96;
    assert(wait_receiver&&wait_slot.command.retained_data&&*wait_slot.command.retained_data);
    wait_receiver_identity=wait_receiver->identity();
    wait_data_identity=(*wait_slot.command.retained_data)->identity();

    ScriptSchedulerServicesV96 scheduler;
    scheduler.owner=scheduler_owner;
    auto expected_scheduler=std::weak_ptr<void>(scheduler_owner);
    SourceCampaignNativeCommandHooksV96 hooks;
    hooks.owner=provider_owner;
    hooks.execute=[&](const CheckedCommandBorrowV59& command,bool skip,
        std::int32_t module,bool& handled,std::string& e){
        order.emplace_back("provider.execute");
        auto manager_command=manager->commands()[0].storage8->at(0).command.canonical_receiver_v96;
        auto scheduler_pin=expected_scheduler.lock();
        assert(command.actual_receiver==manager_command);
        assert(command.identity==receiver_identity&&
               command.actual_data->identity()==data_identity);
        assert(scheduler_pin&&scheduler_pin.get()==scheduler_owner.get());
        saw_skip=skip;saw_module=module;handled=false;e.clear();return true;
    };
    hooks.blocking=[&](const CheckedCommandBorrowV59& command,bool& handled,
                       bool& out,std::string& e){
        order.emplace_back("provider.blocking");
        assert(command.actual_receiver==receiver);
        assert(command.identity==receiver_identity&&
               command.actual_data->identity()==data_identity);
        assert(expected_scheduler.lock().get()==scheduler_owner.get());
        handled=false;out=false;e.clear();return true;
    };
    hooks.update=[&](const CheckedCommandBorrowV59& command,bool& handled,
                     std::string& e){
        order.emplace_back("provider.update");
        assert(command.actual_receiver==wait_receiver);
        assert(command.identity==wait_receiver_identity&&
               command.actual_data->identity()==wait_data_identity);
        assert(expected_scheduler.lock().get()==scheduler_owner.get());
        handled=false;e.clear();return true;
    };

    auto behavior=original;
    auto decorator=source_campaign_native_behavior_decorator_v96(hooks);
    assert(decorator(manager,scheduler,behavior,error));
    // Missing provider ownership/callbacks fail before parsed binding.
    auto missing_hooks=SourceCampaignNativeCommandHooksV96{};
    auto missing_behavior=original;
    assert(!decorate_source_campaign_native_behavior_v96(
        manager,scheduler,missing_hooks,missing_behavior,error));
    assert(error.find("provider owner")!=std::string::npos);
    missing_hooks.owner=provider_owner;
    assert(!decorate_source_campaign_native_behavior_v96(
        manager,scheduler,missing_hooks,missing_behavior,error));
    assert(error.find("Execute/IsBlocking/Update provider")!=std::string::npos);
    auto refusing_hooks=hooks;
    refusing_hooks.execute=[](const CheckedCommandBorrowV59&,bool,std::int32_t,
                              bool&,std::string& e){
        e="typed provider rejected command";return false;
    };
    auto refusing_behavior=original;
    assert(decorate_source_campaign_native_behavior_v96(
        manager,scheduler,refusing_hooks,refusing_behavior,error));
    CheckedCommandBorrowV59 checked;
    assert(receiver->checked_data_borrow(checked,error));
    const auto prior_calls=order.size();
    assert(!refusing_behavior.execute(checked,false,9,error));
    assert(error=="typed provider rejected command"&&order.size()==prior_calls);

    // The production binder uses this exact sequence on its same manager:
    // decorate behavior, attach it to its parsed receiver, then bind scheduler.
    assert(bind_parsed_script_execution_v96(*manager,behavior,error));
    assert(manager->bind_scheduler_v96(std::move(scheduler),error));
    auto execute=slot.command.execute;
    auto blocking=slot.command.blocking;
    auto update=wait_slot.command.update;
    assert(execute(true,77,error));
    assert((order==std::vector<std::string>{"provider.execute","native.execute"}));
    assert(saw_skip&&saw_module==77);
    bool is_blocking=false;
    assert(blocking(is_blocking,error)&&is_blocking);
    assert((order==std::vector<std::string>{"provider.execute","native.execute",
        "provider.blocking","native.blocking"}));
    assert(update(error));
    assert((order==std::vector<std::string>{"provider.execute","native.execute",
        "provider.blocking","native.blocking","provider.update","native.update"}));

    // A copied command callback cannot outlive the SAME manager/scheduler
    // lease and silently run on a detached receiver.
    manager.reset();
    assert(!execute(false,1,error));
    assert(error.find("same live ScriptManager")!=std::string::npos);
    return 0;
}
