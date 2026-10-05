#include "level_file_walk_v1.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh2::loader;
namespace {
std::string quote(const std::string& text) {
    std::string out="\"";const char* hex="0123456789abcdef";
    for(unsigned char c:text) {
        if(c=='"'||c=='\\'){out+='\\';out+=static_cast<char>(c);}
        else if(c<32){out+="\\u00";out+=hex[c>>4];out+=hex[c&15];}
        else out+=static_cast<char>(c);
    }return out+'"';
}
std::string project(const XmlDocumentV1::Borrow& doc,std::uint32_t index) {
    const auto& node=doc.elements().at(index);
    std::string out="{\"tag\":"+quote(node.tag)+",\"attributes\":{";
    for(std::size_t i=0;i<node.attributes.size();++i) {
        if(i)out+=',';
        out+=quote(node.attributes[i].first)+':'+quote(node.attributes[i].second);
    }out+="},\"children\":[";
    for(std::size_t i=0;i<node.children.size();++i) {if(i)out+=',';out+=project(doc,node.children[i]);}
    return out+"]}";
}
void require(bool ok,const std::string& reason) {if(!ok)throw std::runtime_error(reason);}
std::uint32_t number() {
    unsigned char bytes[4];std::cin.read(reinterpret_cast<char*>(bytes),4);
    require(static_cast<bool>(std::cin),"Incomplete binary input");
    return std::uint32_t(bytes[0])|(std::uint32_t(bytes[1])<<8)|(std::uint32_t(bytes[2])<<16)|(std::uint32_t(bytes[3])<<24);
}
std::string blob() {
    const auto size=number();require(size<16*1024*1024,"Input domain exceeded");
    std::string result(size,'\0');std::cin.read(result.data(),size);require(static_cast<bool>(std::cin),"Incomplete input blob");return result;
}
struct Services:LevelFileWalkServicesV1 {
    std::vector<std::string> events;
    unsigned call{};
    std::string failing;
    bool emit(const std::string& kind,const std::string& fields,std::string& error) {
        events.push_back("{\"call\":"+std::to_string(call)+",\"kind\":"+quote(kind)+fields+'}');
        if(failing==kind){error="Explicit unavailable service";return false;}return true;
    }
    bool parse_result(bool success,std::string& error)override {
        return emit("parse_result",std::string(",\"success\":")+(success?"true":"false"),error);
    }
    bool load_element(const XmlDocumentV1::Borrow& doc,std::uint32_t index,std::string& error)override {
        return emit("load_element",",\"element\":"+project(doc,index),error);
    }
    bool release_load_state(std::string& error)override{return emit("release_load_state","",error);}
};
LevelFileWalkV1 fixture(const char* bytes) {
    XmlDocumentV1 doc;std::string raw=bytes,error;LevelFileWalkV1 state;
    require(doc.capture_level_buffer("fixture",{raw.begin(),raw.end()},error),error);
    require(prepare_level_file_walk_v1(doc.borrow(),"Module",state,error),error);return state;
}
unsigned adapter_checks() {
    unsigned checked{};
    for(const auto& operation:{"parse_result","load_element","release_load_state"}) {
        auto state=fixture("<Module><GameObject/></Module>");Services services;services.failing=operation;
        for(unsigned i=0;i<20&&!state.failed;++i)step_level_file_walk_v1(state,services);
        require(state.failed&&!state.completed&&state.document,"Failed candidate lost source");
        const auto count=services.events.size();
        require(step_level_file_walk_v1(state,services)==LevelFileWalkStepV1::failed&&services.events.size()==count,"Failure replayed callbacks");
        services.failing.clear();std::string error;
        require(discard_level_file_walk_v1(state,services,error)&&!state.document&&state.released,"Failure discard did not release");++checked;
    }
    auto a=fixture("<Module><A/></Module>"),b=fixture("<Module><B/><C/></Module>");Services x,y;
    for(unsigned i=0;i<20&&(!a.completed||!b.completed);++i) {
        if(!a.completed)step_level_file_walk_v1(a,x);
        if(!b.completed)step_level_file_walk_v1(b,y);
    }
    require(a.completed&&b.completed&&x.events.size()==3&&y.events.size()==4,"Interleaved file cursors mixed");++checked;
    const auto count=x.events.size();
    require(step_level_file_walk_v1(a,x)==LevelFileWalkStepV1::complete&&x.events.size()==count,"Completion replayed release");++checked;
    auto state=fixture("<Module/>");Services services;services.failing="release_load_state";std::string error;
    require(!discard_level_file_walk_v1(state,services,error)&&state.document&&!state.released,"Failed discard lost source");
    services.failing.clear();require(discard_level_file_walk_v1(state,services,error)&&!state.document,"Discard retry failed");++checked;
    return checked;
}
}
int main() {
    try {
        const auto count=number();require(count<10000,"Too many cases");std::cout<<"{\"cases\":[";
        for(unsigned i=0;i<count;++i) {
            const auto label=blob(),requested=blob(),raw=blob();std::string error;
            XmlDocumentV1 doc;require(doc.capture_level_buffer(label,{raw.begin(),raw.end()},error),error);
            LevelFileWalkV1 state;require(prepare_level_file_walk_v1(doc.borrow(),requested,state,error),error);
            require(std::string(doc.borrow().source().begin(),doc.borrow().source().end())==raw,"Raw source changed");
            Services services;std::vector<std::string> calls;
            bool unsafe{};
            for(services.call=0;services.call<65536;++services.call) {
                const auto start=services.events.size();const auto result=step_level_file_walk_v1(state,services);
                unsafe=state.failed&&state.error.find("Matching top-level node")!=std::string::npos;
                if(unsafe)break; // No original return exists for the guarded unsafe branch.
                calls.push_back(std::string("{\"returned\":")+(result==LevelFileWalkStepV1::pending?"false":"true")
                    +",\"event_count\":"+std::to_string(services.events.size()-start)+",\"load_state_present\":"+(state.document?"true":"false")+'}');
                if(result!=LevelFileWalkStepV1::pending)break;
            }
            require(state.completed||state.failed,"File walk did not finish");
            const auto events=services.events;
            std::string cleanup="null";
            if(state.failed&&!unsafe) {
                services.call=static_cast<unsigned>(calls.size());const auto start=services.events.size();
                require(discard_level_file_walk_v1(state,services,error),error);
                cleanup="{\"returned\":true,\"events\":[";
                for(std::size_t j=start;j<services.events.size();++j){if(j>start)cleanup+=',';cleanup+=services.events[j];}
                cleanup+="],\"load_state_present\":false}";
            }
            if(i)std::cout<<',';
            std::cout<<"{\"label\":"<<quote(label)<<",\"parse_success\":"<<(doc.borrow().diagnostic().code==0?"true":"false")
                <<",\"unsafe\":"<<(unsafe?"true":"false")<<",\"calls\":[";
            for(std::size_t j=0;j<calls.size();++j){if(j)std::cout<<',';std::cout<<calls[j];}
            std::cout<<"],\"events\":[";
            for(std::size_t j=0;j<events.size();++j){if(j)std::cout<<',';std::cout<<events[j];}
            std::cout<<"],\"failure_cleanup_poll\":"<<cleanup<<",\"raw_source_preserved\":true}";
        }std::cout<<"],\"adapter_checks\":"<<adapter_checks()<<"}\n";
    }catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}
}
