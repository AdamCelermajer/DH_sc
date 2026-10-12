#include "module_load_v1.hpp"
#include <cstring>
#include <iostream>
#include <stdexcept>
using namespace dh2::loader;
namespace {
void require(bool ok,const std::string& error) {if(!ok)throw std::runtime_error(error);}
std::uint32_t number() {
    unsigned char bytes[4];std::cin.read(reinterpret_cast<char*>(bytes),4);require(bool(std::cin),"Missing binary input");
    return std::uint32_t(bytes[0])|(std::uint32_t(bytes[1])<<8)|(std::uint32_t(bytes[2])<<16)|(std::uint32_t(bytes[3])<<24);
}
std::string blob() {
    const auto size=number();require(size<1048576,"Input domain exceeded");std::string text(size,'\0');
    std::cin.read(text.data(),size);require(bool(std::cin),"Missing binary blob");return text;
}
std::string quote(const std::string& text) {
    std::string out="\"";const char* hex="0123456789abcdef";
    for(unsigned char c:text) {
        if(c=='"'||c=='\\'){out+='\\';out+=static_cast<char>(c);}
        else if(c<32){out+="\\u00";out+=hex[c>>4];out+=hex[c&15];}
        else out+=static_cast<char>(c);
    }return out+'"';
}
std::string words(const std::array<float,3>& position) {
    std::array<std::uint32_t,3> value{};static_assert(sizeof(value)==sizeof(position));
    std::memcpy(value.data(),position.data(),sizeof(value));
    return '['+std::to_string(value[0])+','+std::to_string(value[1])+','+std::to_string(value[2])+']';
}
ObjectEntryV1 entry(const std::string& type="Module") {
    XmlDocumentV1 doc;std::string error,raw="<Level><GameObject gametype=\""+type+"\" name=\"retained\"/></Level>";
    require(doc.capture("fixture",{raw.begin(),raw.end()},error),error);ObjectEntryV1 out;
    require(prepare_object_entry_v1(doc.borrow(),1,ObjectEntryRouteV1::level,{},out,error),error);return out;
}
ModuleLoadV1 candidate(const std::string& type="Module") {
    ModuleLoadV1 state;std::string error;require(prepare_module_load_v1(entry(type),42,{1,2,3},state,error),error);return state;
}
struct Services:ModuleLoadServicesV1 {
    std::vector<std::string> events;
    std::string gameplay="gameplay.mgp",visual="visual.mvp",failing;
    std::array<unsigned,2> pending{},polls{};
    std::int32_t id{-7};std::array<float,3> offset{7,8,9};unsigned slot{};
    bool status(const std::string& operation,std::string& error) {
        if(failing==operation){error="Unavailable "+operation;return false;}return true;
    }
    bool set_module_id(std::int32_t value,std::string& error)override {
        events.push_back("{\"kind\":\"set_module_id\",\"value\":"+std::to_string(value)+'}');id=value;
        return status(value==-1?"clear_id":"set_id",error);
    }
    bool set_module_offset(const std::array<float,3>& value,std::string& error)override {
        events.push_back("{\"kind\":\"set_offset\",\"words\":"+words(value)+'}');offset=value;
        return status(words(value)=="[0,0,0]"?"clear_offset":"set_offset",error);
    }
    bool choose_xmls(const ObjectEntryV1& source,std::string& gp,std::string& vp,std::string& error)override {
        require(source.document&&*source.source().attribute("name")=="retained","Source owner unavailable after facade destruction");
        events.push_back("{\"kind\":\"choose\",\"module_id\":"+std::to_string(id)+",\"offset\":"+words(offset)+'}');
        gp=gameplay;vp=visual;slot=gp.empty()?1:0;return status("choose",error);
    }
    ModuleFileStepV1 load_file(const std::string& uri,const char* root,std::string& error)override {
        require(slot<2&&uri==(slot?visual:gameplay)&&std::string(root)=="Module","Changed pending file/root");
        const bool ready=polls[slot]>=pending[slot];++polls[slot];
        events.push_back("{\"kind\":\"load_file\",\"uri\":"+quote(uri)+",\"root\":"+quote(root)
            +",\"returned\":"+(ready?"true":"false")+",\"module_id\":"+std::to_string(id)+",\"offset\":"+words(offset)+'}');
        if(!status(slot?"load_visual":"load_gameplay",error))return ModuleFileStepV1::failed;
        if(ready)++slot;
        return ready?ModuleFileStepV1::complete:ModuleFileStepV1::pending;
    }
    bool discard_file(std::string& error)override {
        events.push_back("{\"kind\":\"discard_file\"}");return status("discard_file",error);
    }
};
unsigned adapter_checks() {
    unsigned checked{};
    for(const auto* operation:{"set_id","set_offset","choose","load_gameplay","load_visual","clear_offset","clear_id"}) {
        auto state=candidate();Services services;services.failing=operation;
        require(step_module_load_v1(state,services)==ModuleLoadStepV1::failed&&state.failed&&!state.completed,operation);
        require(bool(state.entry.document),"Failed module lost source");const auto count=services.events.size();
        require(step_module_load_v1(state,services)==ModuleLoadStepV1::failed&&services.events.size()==count,"Failed module replayed mutation");
        services.failing.clear();std::string error;require(discard_module_load_v1(state,services,error),error);
        require(!state.entry.document&&state.discarded&&services.id==-1&&words(services.offset)=="[0,0,0]","Failure cleanup left context/source");++checked;
    }
    for(const auto* operation:{"discard_file","clear_offset","clear_id"}) {
        auto state=candidate();Services services;services.pending[0]=2;std::string error;
        require(step_module_load_v1(state,services)==ModuleLoadStepV1::pending&&state.file_open,"Pending file not retained");
        services.failing=operation;
        require(!discard_module_load_v1(state,services,error)&&state.entry.document&&!state.discarded,"Failed cancellation lost retained source");
        services.failing.clear();require(discard_module_load_v1(state,services,error),error);
        require(services.id==-1&&words(services.offset)=="[0,0,0]"&&!state.entry.document,"Cancellation did not unwind");
        const auto count=services.events.size();require(discard_module_load_v1(state,services,error)&&services.events.size()==count,"Discard replayed services");++checked;
    }
    auto state=candidate();Services services;std::string error;
    require(!prepare_module_load_v1(entry("Character"),99,{0,0,0},state,error)&&state.runtime_module_id==42,"Rejected class replaced previous candidate");++checked;
    require(step_module_load_v1(state,services)==ModuleLoadStepV1::complete,"Completion failed");
    const auto count=services.events.size();require(step_module_load_v1(state,services)==ModuleLoadStepV1::complete&&services.events.size()==count,"Completion replayed services");++checked;
    return checked;
}
}
int main() {
    try {
        const auto count=number();require(count<10000,"Too many cases");std::cout<<"{\"cases\":[";
        for(unsigned i=0;i<count;++i) {
            const auto label=blob(),type=blob();const auto id_word=number();std::int32_t id{};std::memcpy(&id,&id_word,4);
            std::array<std::uint32_t,3> raw_position{number(),number(),number()};std::array<float,3> position{};
            std::memcpy(position.data(),raw_position.data(),sizeof(position));Services services;
            services.gameplay=blob();services.visual=blob();services.pending={number(),number()};
            ModuleLoadV1 state;std::string error;require(prepare_module_load_v1(entry(type),id,position,state,error),error);
            for(unsigned poll=0;poll<65536;++poll) {
                const auto result=step_module_load_v1(state,services);
                require(result!=ModuleLoadStepV1::failed,state.error);
                if(result==ModuleLoadStepV1::complete)break;
            }
            require(state.completed&&!state.file_open&&services.id==-1&&words(services.offset)=="[0,0,0]","Module did not complete");
            if(i)std::cout<<',';
            std::cout<<"{\"label\":"<<quote(label)<<",\"events\":[";
            for(std::size_t j=0;j<services.events.size();++j){if(j)std::cout<<',';std::cout<<services.events[j];}
            std::cout<<"],\"file_polls\":{\"gameplay\":"<<services.polls[0]<<",\"visual\":"<<services.polls[1]
                <<"},\"final_context\":{\"module_id\":"<<services.id<<",\"offset\":"<<words(services.offset)<<"}}";
        }
        std::cout<<"],\"adapter_checks\":"<<adapter_checks()<<"}\n";
    }catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}
}
