#include "source_campaign_dispatch_v1.hpp"
#include <stdexcept>

namespace dh::foundation {
namespace {
bool required(const char* service,std::string& error){
    error=std::string("Required same authored campaign dispatch ")+service;
    return false;
}
}

SourceCampaignDispatchV1::SourceCampaignDispatchV1(SourceCampaignDispatchServicesV1 services)
    :services_(std::move(services)){}

bool SourceCampaignDispatchV1::bind(OriginalCampaignRuntime& runtime,
                                    OriginalCampaignServices existing,
                                    std::string& error){
    error.clear();
    if(!services_.skip||!services_.remaining){
        error="Campaign command dispatch requires same source skip and remaining command providers";
        return false;
    }
    if(!existing.admit_start){error="Campaign command dispatch requires existing source start admission";return false;}
    existing.command=[this](CampaignCommandPhase phase,
        const OriginalCampaignCommand& command,int module,bool& blocking,std::string& e){
        return this->command(phase,command,module,blocking,e);
    };
    runtime.bind(std::move(existing));
    return true;
}

bool SourceCampaignDispatchV1::command(CampaignCommandPhase phase,
    const OriginalCampaignCommand& command,int module,bool& blocking,std::string& error){
    error.clear();blocking=false;
    bool skip=false;
    if(!services_.skip(command,skip,error))return false;
    bool handled=false;
    const bool cinematic_kind=command.kind==1||command.kind==2||command.kind==4||
        command.kind==5||command.kind==6||command.kind==7||command.kind==8||
        command.kind==24||command.kind==25||command.kind==40||command.kind==41||
        command.kind==45||command.kind==46;
    if(cinematic_kind&&!services_.cinematics)
        return required("SourceCinematicCommands owner",error);
    if(services_.cinematics){
        if(!services_.cinematics(phase,command,module,skip,handled,blocking,error))return false;
        if(handled)return true;
    }
    if((command.kind==54||command.kind==55)&&!services_.doors)
        return required("SourceDoorCommands owner",error);
    if(services_.doors){
        if(!services_.doors(phase,command,module,skip,handled,blocking,error))return false;
        if(handled)return true;
    }
    if((command.kind==13||command.kind==14)&&!services_.audio)
        return required("canonical source audio command owner",error);
    if(services_.audio){
        if(command.kind==13||command.kind==14){
            SourceCampaignDispatchContextV1 context{};
            if(phase==CampaignCommandPhase::execute){
                if(!services_.context||!services_.context(command,context,error))
                    return required("actual receive flag/source frame pairing",error);
            }
            if(!services_.audio(phase,command,context.received,context.authored_event_ns,
                                handled,blocking,error))return false;
            if(handled)return true;
        }
    }
    return services_.remaining(phase,command,module,skip,blocking,error);
}

} // namespace dh::foundation
