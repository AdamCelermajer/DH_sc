#include "debug_switches_persistence.hpp"
#include <limits>

namespace dh2::debug_switches_persistence { namespace {
constexpr const char* filename="DebugSwitches.savegame";
constexpr const char* tracing_file="isTracingDebugSwitchesFile";
Status invoke(const Services& services,const Request& request,Stream& reply,Result& result) {
    if(!services.invoke)return Status::service_unavailable;
    ++result.calls;result.last_operation=request.operation;reply={};
    try{if(services.invoke(services.context,&request,&reply))return Status::service_failed;}
    catch(...){return Status::service_failed;}
    return Status::complete;
}
Status word(const Owner& owner,Stream stream,std::uint32_t value,const Services& services,Result& result) {
    Stream ignored{};return invoke(services,{Operation::write_word,owner.identity,0,stream.identity,value,nullptr,0},ignored,result);
}
Status row(const Owner& owner,Stream stream,const Map::value_type& entry,const Services& services,Result& result) {
    if(entry.first.size()>std::numeric_limits<std::uint32_t>::max())return Status::unsupported_configuration;
    Stream ignored{};
    auto status=invoke(services,{Operation::write_string,owner.identity,0,stream.identity,0,entry.first.data(),entry.first.size()},ignored,result);
    if(status!=Status::complete)return status;
    return invoke(services,{Operation::write_byte,owner.identity,0,stream.identity,entry.second,nullptr,0},ignored,result);
}
Status write_impl(const Owner& owner,Stream stream,debug_switches::Globals& globals,
                  const debug_switches::Services& debug_services,const Services& services,Result& result) {
    if(!owner.identity)return Status::invalid_argument;
    if(!stream.identity)return Status::complete;
    if(!owner.switches || !owner.modules || owner.switches==owner.modules)return Status::invalid_argument;
    auto status=word(owner,stream,0x44425357u,services,result);if(status!=Status::complete)return status;
    status=word(owner,stream,0x20000,services,result);if(status!=Status::complete)return status;
    if(owner.modules->size()>std::numeric_limits<std::uint32_t>::max())return Status::unsupported_configuration;
    result.module_count=static_cast<std::uint32_t>(owner.modules->size());
    status=word(owner,stream,result.module_count,services,result);if(status!=Status::complete)return status;
    for(auto it=owner.modules->begin();it!=owner.modules->end();++it) {
        status=row(owner,stream,*it,services,result);if(status!=Status::complete)return status;
        ++result.modules_written;
    }
    if(owner.switches->size()>std::numeric_limits<std::uint32_t>::max())return Status::unsupported_configuration;
    result.switch_count=static_cast<std::uint32_t>(owner.switches->size());
    status=word(owner,stream,result.switch_count,services,result);if(status!=Status::complete)return status;
    auto it=owner.switches->begin();
    if(it==owner.switches->end())return Status::complete;
    auto* const tracing=globals.singleton;
    if(!tracing || !tracing->identity())return Status::invalid_argument;
    for(;it!=owner.switches->end();++it) {
        status=tracing->load(globals,debug_services);if(status!=Status::complete)return status;
        const std::string key(tracing_file);std::uint8_t ignored=0;
        status=tracing->get_switch(key,globals,debug_services,ignored);if(status!=Status::complete)return status;
        status=row(owner,stream,*it,services,result);if(status!=Status::complete)return status;
        ++result.switches_written;
    }
    return Status::complete;
}
}
Status write(const Owner& owner,Stream stream,debug_switches::Globals& globals,
             const debug_switches::Services& debug_services,const Services& services,Result& result) {
    result={};const auto bound=services;const auto debug_bound=debug_services;
    try{return write_impl(owner,stream,globals,debug_bound,bound,result);}catch(...){return Status::service_failed;}
}
Status save(const Owner& owner,debug_switches::Globals& globals,
            const debug_switches::Services& debug_services,const Services& services,Result& result) {
    result={};const auto bound=services;const auto debug_bound=debug_services;
    try {
        if(!owner.identity)return Status::invalid_argument;
        const auto* const application=globals.application;
        if(!application || !application->identity || !application->engine || !application->engine->identity)return Status::invalid_argument;
        const auto* const filesystem=application->engine->files;
        if(!filesystem)return Status::complete;
        if(!filesystem->identity)return Status::invalid_argument;
        const auto fs=filesystem->identity;Stream stream{};
        auto status=invoke(bound,{Operation::open_write,owner.identity,fs,0,1,filename,0},stream,result);
        if(status!=Status::complete || !stream.identity)return status;
        status=write_impl(owner,stream,globals,debug_bound,bound,result);if(status!=Status::complete)return status;
        Stream ignored{};return invoke(bound,{Operation::close,owner.identity,fs,stream.identity,0,nullptr,0},ignored,result);
    }catch(...){return Status::service_failed;}
}
} // namespace dh2::debug_switches_persistence
