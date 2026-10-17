#include "debug_switches_runtime.hpp"
#include "../persistence/binary.h"
#include <cstring>
#include <initializer_list>

namespace dh2::debug_switches { namespace {
constexpr const char* path="DebugSwitches.savegame";
constexpr const char* tracing="isTracingDebugSwitches";
constexpr const char* tracing_file="isTracingDebugSwitchesFile";
Status invoke(const Services& services,const Request& request,File& reply) {
    if(!services.invoke)return Status::service_unavailable;
    reply={};
    try{if(services.invoke(services.context,&request,&reply))return Status::service_failed;}
    catch(...){return Status::service_failed;}
    return Status::complete;
}
struct Reader {
    const File& file;std::size_t& position;
    bool word(std::uint32_t& out){if(position>file.size || file.size-position<4)return false;out=dh2_save_read32(file.bytes+position);position+=4;return true;}
    bool byte(std::uint8_t& out){if(position>=file.size)return false;out=file.bytes[position++];return true;}
    bool name(std::string& out){std::uint32_t n=0;if(!word(n) || n>255 || position>file.size || n>file.size-position)return false;
        out.assign(reinterpret_cast<const char*>(file.bytes+position),n);position+=n;
        // Original ReadString stores a NUL-terminated buffer and subsequent
        // std::string(char*) truncates at the first NUL. Binary stream consumes n.
        const auto nul=out.find('\0');if(nul!=std::string::npos)out.resize(nul);return true;}
};
bool depth_valid(unsigned depth){return depth<128;}
std::int32_t signed_word(std::uint32_t n){std::int32_t out;std::memcpy(&out,&n,4);return out;}
}
Status Runtime::load(Globals& globals,const Services& services){const auto bound=services;try{return load_impl(globals,bound,0);}catch(...){return Status::service_failed;}}
Status Runtime::get_switch(const std::string& key,Globals& globals,const Services& services,std::uint8_t& value){const auto bound=services;try{return get_impl(key,globals,bound,value,0);}catch(...){return Status::service_failed;}}
Status Runtime::set_switch(const std::string& key,std::uint8_t value,Globals& globals,const Services& services){const auto bound=services;try{return set_impl(key,value,globals,bound,0);}catch(...){return Status::service_failed;}}
Status Runtime::trace(const char* key,Globals& globals,const Services& services,unsigned depth) {
    if(!depth_valid(depth))return Status::unsupported_configuration;
    auto* const selected=globals.singleton;
    if(!selected || !selected->identity_)return Status::invalid_argument;
    auto status=selected->load_impl(globals,services,depth+1);
    if(status!=Status::complete)return status;
    const std::string text(key);std::uint8_t ignored=0;
    return selected->get_impl(text,globals,services,ignored,depth+1);
}
Status Runtime::load_impl(Globals& globals,const Services& services,unsigned depth) {
    if(!identity_)return Status::invalid_argument;
    if(!depth_valid(depth))return Status::unsupported_configuration;
    if(globals.loaded)return Status::complete;
    globals.loaded=1;
    const auto* const application=globals.application;
    if(!application || !application->identity || !application->engine || !application->engine->identity)return Status::invalid_argument;
    const auto* const filesystem=application->engine->files;
    if(filesystem) {
        if(!filesystem->identity)return Status::invalid_argument;
        const auto fs_identity=filesystem->identity;File file{};
        auto status=invoke(services,{Operation::open_read,this,fs_identity,nullptr,path},file);
        if(status!=Status::complete)return status;
        if(file.identity) {
            if((file.size && !file.bytes) || file.size>16u*1024u*1024u)return Status::invalid_argument;
            status=read_configuration(file,globals,services,depth+1);
            if(status!=Status::complete)return status;
            File ignored{};status=invoke(services,{Operation::close,this,fs_identity,&file,nullptr},ignored);
            if(status!=Status::complete)return status;
        }
    }
    auto* const selected=globals.singleton;
    if(!selected || !selected->identity_)return Status::invalid_argument;
    for(const char* key:{"IsDeactivatingFlashMenus","IsDeactivatingFlashMenusUpdate","IsDeactivatingFlashMenusRender"}) {
        auto status=selected->load_impl(globals,services,depth+1);
        if(status!=Status::complete)return status;
        const std::string text(key);status=selected->set_impl(text,0,globals,services,depth+1);
        if(status!=Status::complete)return status;
    }
    for(const char* key:{"ConnectToAlphaServer","ConnectToBetaServer"}) {
        auto status=selected->load_impl(globals,services,depth+1);
        if(status!=Status::complete)return status;
        const std::string text(key);std::uint8_t ignored=0;
        status=selected->get_impl(text,globals,services,ignored,depth+1);
        if(status!=Status::complete)return status;
    }
    return Status::complete;
}
Status Runtime::get_impl(const std::string& key,Globals& globals,const Services& services,std::uint8_t& value,unsigned depth) {
    if(!identity_)return Status::invalid_argument;
    if(!depth_valid(depth))return Status::unsupported_configuration;
    if(switches_.find(key)==switches_.end()) {
        switches_[key]=0;
        auto status=trace(tracing,globals,services,depth+1);if(status!=Status::complete)return status;
    }
    value=switches_[key];return Status::complete;
}
Status Runtime::set_impl(const std::string& key,std::uint8_t value,Globals& globals,const Services& services,unsigned depth) {
    if(!identity_)return Status::invalid_argument;
    if(!depth_valid(depth))return Status::unsupported_configuration;
    if(switches_.find(key)==switches_.end()) {
        auto status=trace(tracing,globals,services,depth+1);if(status!=Status::complete)return status;
        switches_[key]=0;
    }
    if(switches_[key]!=value) {
        switches_[key]=value;File ignored{};
        return invoke(services,{Operation::save,this,0,nullptr,nullptr},ignored);
    }
    return Status::complete;
}
Status Runtime::read_configuration(const File& file,Globals& globals,const Services& services,unsigned depth) {
    last_file_position_=0;
    if(file.size<=11)return Status::complete;
    Reader reader{file,last_file_position_};std::uint32_t magic=0,version=0,count=0;
    if(!reader.word(magic) || magic!=0x44425357u || !reader.word(version))return Status::unsupported_configuration;
    if(signed_word(version)<0x10000)return Status::unsupported_configuration;
    if(signed_word(version)>=0x20000) {
        if(!reader.word(count))return Status::invalid_argument;
        if(signed_word(count)>0)return Status::unsupported_configuration;
    }
    if(!reader.word(count))return Status::invalid_argument;
    if(signed_word(count)<=0)return Status::complete;
    if(count>10000)return Status::unsupported_configuration;
    for(std::uint32_t i=0;i<count;++i) {
        std::string name;std::uint8_t raw=0;
        if(!reader.name(name) || !reader.byte(raw))return Status::invalid_argument;
        auto status=trace(tracing_file,globals,services,depth+1);if(status!=Status::complete)return status;
        status=set_impl(name,std::uint8_t(raw!=0),globals,services,depth+1);if(status!=Status::complete)return status;
    }
    return Status::complete;
}
} // namespace dh2::debug_switches
