#include "source_campaign_dispatch_v1.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace dh::foundation;
namespace {
void check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
}

int main(){try{
    std::string error;unsigned skip_reads=0,remaining_calls=0,audio_calls=0,cinematic_calls=0,door_calls=0;
    std::vector<std::string> order;
    auto remaining=[&](auto,const auto&,int,bool skip,bool& blocking,std::string&){
        order.push_back("remaining");++remaining_calls;check(skip,"remaining source command lost same sampled skip");blocking=false;return true;
    };
    // The callback seams stand in for existing typed adapters so this test
    // exercises only command ownership/order; their native component tests
    // remain the evidence for camera, Door, and V42 audio behavior.
    SourceCampaignDispatchServicesV1 routes;
    routes.skip=[&](const auto&,bool& skip,std::string&){++skip_reads;skip=true;return true;};
    routes.context=[](const auto&,SourceCampaignDispatchContextV1& out,std::string&){out.received=false;out.authored_event_ns=987654;return true;};
    routes.cinematics=[&](auto phase,const auto& command,int module,bool skip,bool& handled,bool& blocking,std::string&){
        order.push_back("cinematics");++cinematic_calls;if(command.kind!=5)return true;check(phase==CampaignCommandPhase::execute&&module==7&&skip,"cinematic dispatch context changed");handled=true;blocking=true;return true;
    };
    routes.doors=[&](auto phase,const auto& command,int module,bool skip,bool& handled,bool& blocking,std::string&){
        order.push_back("door");++door_calls;if(command.kind!=54)return true;check(phase==CampaignCommandPhase::is_blocking&&module==7&&skip,"Door dispatch context changed");handled=true;blocking=true;return true;
    };
    routes.audio=[&](auto phase,const auto& command,bool received,std::int64_t ns,bool& handled,bool& blocking,std::string&){
        order.push_back("audio");++audio_calls;check(command.kind==13&&phase==CampaignCommandPhase::execute&&!received&&ns==987654,"audio source context changed");handled=true;blocking=false;return true;
    };
    routes.remaining=remaining;
    SourceCampaignDispatchV1 mux(std::move(routes));
    bool blocking=false;
    OriginalCampaignCommand command;command.kind=5;
    check(mux.command(CampaignCommandPhase::execute,command,7,blocking,error)&&blocking&&cinematic_calls==1,"cinematic command was not owned by its adapter");
    command.kind=54;check(mux.command(CampaignCommandPhase::is_blocking,command,7,blocking,error)&&blocking&&door_calls==1,"Door blocking query was not owned by its adapter");
    command.kind=13;check(mux.command(CampaignCommandPhase::execute,command,7,blocking,error)&&!blocking&&audio_calls==1,"audio command was not owned by its adapter");
    command.kind=77;check(mux.command(CampaignCommandPhase::execute,command,7,blocking,error)&&remaining_calls==1,"unhandled command did not reach existing command provider");
    check(skip_reads==4,"source skip was not sampled once per command");
    check(order==std::vector<std::string>({"cinematics","cinematics","door","cinematics","door","audio","cinematics","door","remaining"}),
          "typed dispatch ordering or single remaining fallback changed");

    SourceCampaignDispatchServicesV1 missing_audio_services;
    missing_audio_services.skip=[](const auto&,bool& skip,std::string&){skip=false;return true;};
    missing_audio_services.remaining=remaining;
    SourceCampaignDispatchV1 missing_audio(std::move(missing_audio_services));
    command.kind=13;check(!missing_audio.command(CampaignCommandPhase::execute,command,7,blocking,error)&&
                          error.find("canonical source audio command owner")!=std::string::npos,
                          "missing mandatory campaign audio owner was accepted through fallback");

    // Rebind the same scheduler object: admission and source Random remain the
    // original functions while only its command callback is replaced.
    OriginalCampaignRuntime runtime;OriginalCampaignServices existing;
    unsigned admissions=0,random_calls=0;existing.admit_start=[&](int,int,bool,bool& allowed,std::string&){++admissions;allowed=true;return true;};
    existing.random=[&](std::uint32_t bound,std::uint32_t& result,std::string&){++random_calls;result=bound?0:0;return true;};
    SourceCampaignDispatchServicesV1 binder_services;
    binder_services.skip=[&](const auto&,bool& skip,std::string&){++skip_reads;skip=false;return true;};
    binder_services.remaining=remaining;SourceCampaignDispatchV1 binder(std::move(binder_services));
    check(binder.bind(runtime,std::move(existing),error),error.c_str());
    check(admissions==0&&random_calls==0,"binding invoked scheduler callbacks eagerly");
    std::cout<<"same-runtime campaign dispatch order PASS typed=cinematics,door,audio,remaining; source-time and skip retained; no secondary scheduler\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}return 0;}
