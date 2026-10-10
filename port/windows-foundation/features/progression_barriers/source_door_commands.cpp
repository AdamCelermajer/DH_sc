#include "source_door_commands.hpp"
#include <cstring>

namespace dh::foundation::progression_barriers {
namespace {
bool fail(const char* name,std::string& error){
    if(error.empty())error=std::string("Required source Door command ")+name;
    return false;
}
bool scalar(const OriginalCampaignCommand& command,unsigned offset,std::uint32_t& value,
            std::string& error){
    const auto found=command.scalars.find(offset);
    if(found==command.scalars.end()){
        error="Missing authored Door command scalar "+std::to_string(offset);return false;
    }
    value=found->second;return true;
}
bool text(const OriginalCampaignCommand& command,unsigned offset,const std::string*& value,
          std::string& error){
    const auto found=command.strings.find(offset);
    if(found==command.strings.end()){
        error="Missing authored Door command string "+std::to_string(offset);return false;
    }
    value=&found->second;return true;
}
}

SourceDoorCommands::SourceDoorCommands(DoorCommandServices services):services_(std::move(services)){}

bool SourceDoorCommands::command(CampaignCommandPhase phase,const OriginalCampaignCommand& command,
                                 int module,bool skip,bool& handled,bool& blocking,
                                 std::string& error){
    error.clear();handled=false;blocking=false;
    if(command.kind!=54&&command.kind!=55)return true;
    handled=true;
    if(phase==CampaignCommandPhase::update)return true; // ScriptCmd::Update is the source base no-op.

    auto executed=executed_.find(&command);
    if(phase==CampaignCommandPhase::is_blocking){
        if(executed==executed_.end()||!executed->second.receiver||!executed->second.wait)return true;
        const auto state=executed->second.receiver->source_state3a8();
        blocking=state!=1&&state!=3; // Exact Script_OpenDoor/CloseDoor::IsBlocking states.
        return true;
    }

    if(!services_.owner||!services_.trace)return fail("DebugSwitches Load/GetSwitch owner",error);
    if(!services_.trace(error))return false;
    const std::string* name=nullptr;
    std::uint32_t wait_word=0;
    if(!text(command,12,name,error)||!scalar(command,20,wait_word,error))return false;
    if(wait_word>1){error="Authored Door command wait byte is outside its source range";return false;}

    ExecutedDoor next;next.wait=wait_word!=0;
    auto manager=services_.objects.lock();
    if(!manager)return fail("SAME canonical ObjectManager",error);
    dh2::target_providers::Handle16 handle{};
    if(!manager->by_name(name->c_str(),module,false,nullptr,handle,error))return false;
    const dh2::world::CanonicalObjectBorrowV1* object=nullptr;
    if(!manager->resolve_handle_v4(handle,false,object,{},error))return false;
    // The source command clears its cached object for absent or non-Door names.
    if(object&&object->identity&&object->type_f4&&*object->type_f4==2){
        if(!services_.same_door)return fail("SAME CanonicalDoor receiver resolver",error);
        if(!services_.same_door(*object,next.receiver,error))return false;
        if(!next.receiver||next.receiver->base().identity()!=object->identity){
            error="Canonical Door resolver did not return the named SAME source receiver";return false;
        }
        if(command.kind==54){
            if(!next.receiver->source_opened_v91(skip,error))return false;
        }else if(!next.receiver->source_closed_v91(skip,error))return false;
    }
    executed_[&command]=std::move(next);
    return true;
}

} // namespace dh::foundation::progression_barriers
