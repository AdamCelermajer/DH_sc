#include "cached_level_file_v1.hpp"
#include "module_load_v1.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2;
using namespace dh2::loader;
namespace {
void require(bool ok,const std::string& reason){if(!ok)throw std::runtime_error(reason);}
std::uint32_t number() {
    unsigned char bytes[4];std::cin.read(reinterpret_cast<char*>(bytes),4);require(bool(std::cin),"Incomplete input");
    return std::uint32_t(bytes[0])|(std::uint32_t(bytes[1])<<8)|(std::uint32_t(bytes[2])<<16)|(std::uint32_t(bytes[3])<<24);
}
std::string blob() {
    const auto size=number();require(size<16*1024*1024,"Input domain exceeded");std::string out(size,'\0');
    std::cin.read(out.data(),size);require(bool(std::cin),"Incomplete input blob");return out;
}
std::string quote(const std::string& text) {
    std::string out="\"";const char* hex="0123456789abcdef";
    for(unsigned char c:text) {
        if(c=='"'||c=='\\'){out+='\\';out+=static_cast<char>(c);}
        else if(c<32){out+="\\u00";out+=hex[c>>4];out+=hex[c&15];}
        else out+=static_cast<char>(c);
    }return out+'"';
}
std::string project(const XmlDocumentV1::Borrow& doc,std::uint32_t index) {
    const auto& node=doc.elements().at(index);std::string out="{\"tag\":"+quote(node.tag)+",\"attributes\":{";
    for(std::size_t i=0;i<node.attributes.size();++i) {
        if(i)out+=',';
        out+=quote(node.attributes[i].first)+':'+quote(node.attributes[i].second);
    }out+="},\"children\":[";
    for(std::size_t i=0;i<node.children.size();++i){if(i)out+=',';out+=project(doc,node.children[i]);}
    return out+"]}";
}
assets::ZipAssetPackV1 archive(const char* path,std::shared_ptr<bool> failing={}) {
    auto file=std::make_shared<std::ifstream>(path,std::ios::binary|std::ios::ate);
    require(bool(*file),"Archive unavailable");const auto length=file->tellg();require(length>=0,"Archive length unavailable");
    assets::ZipBackingV1 backing;backing.owner=file;backing.bytes=static_cast<std::uint64_t>(length);
    backing.read=[file,failing](std::uint64_t at,void* dst,std::size_t count,std::string& error) {
        if(failing&&*failing){error="Explicit unavailable backing read";return false;}
        file->clear();file->seekg(static_cast<std::streamoff>(at));file->read(static_cast<char*>(dst),static_cast<std::streamsize>(count));
        if(!*file){error="Archive read failed";return false;}return true;
    };
    assets::ZipAssetPackV1 pack;std::string error;
    require(pack.mount(std::move(backing),"com.gameloft.android.GAND.GloftD2SS/files/",error),error);return pack;
}
struct Services:LevelFileWalkServicesV1 {
    std::vector<std::string> events;
    std::vector<ObjectEntryV1> declarations; // Inspection only; no runtime objects.
    std::string failing;
    unsigned call{},parses{},releases{};
    bool parsed{};
    bool status(const char* operation,std::string& error) {
        if(failing==operation){error="Required service unavailable";return false;}return true;
    }
    bool parse_result(bool success,std::string& error)override {
        ++parses;parsed=success;
        events.push_back("{\"call\":"+std::to_string(call)+",\"kind\":\"parse_result\",\"success\":"+(success?"true":"false")+'}');
        return status("parse_result",error);
    }
    bool load_element(const XmlDocumentV1::Borrow& doc,std::uint32_t index,std::string& error)override {
        events.push_back("{\"call\":"+std::to_string(call)+",\"kind\":\"load_element\",\"element\":"+project(doc,index)+'}');
        if(!status("load_element",error))return false;
        // Retain/classify rather than claim successful construction.
        ObjectEntryV1 entry;
        if(!prepare_object_entry_v1(doc,index,ObjectEntryRouteV1::level,{},entry,error))return false;
        declarations.push_back(std::move(entry));return true;
    }
    bool release_load_state(std::string& error)override {
        ++releases;events.push_back("{\"call\":"+std::to_string(call)+",\"kind\":\"release_load_state\"}");
        return status("release_load_state",error);
    }
};
void finish(CachedLevelFileV1& file,const std::string& uri,Services& services) {
    for(unsigned i=0;i<65536;++i) {
        const auto result=file.step(uri,"Module",services);require(result!=LevelFileWalkStepV1::failed,file.error());
        if(result==LevelFileWalkStepV1::complete)return;
    }throw std::runtime_error("Request did not complete");
}
struct ModuleServices:ModuleLoadServicesV1 {
    CachedLevelFileV1 file;Services consumer;std::int32_t id{-1};std::array<float,3> offset{};
    std::string gameplay="fixture/single.xml",visual="fixture/comments.xml";
    explicit ModuleServices(assets::ZipAssetPackV1 pack):file(std::move(pack)){}
    bool set_module_id(std::int32_t value,std::string&)override{id=value;return true;}
    bool set_module_offset(const std::array<float,3>& value,std::string&)override{offset=value;return true;}
    bool choose_xmls(const ObjectEntryV1&,std::string& gp,std::string& vp,std::string&)override{gp=gameplay;vp=visual;return true;}
    ModuleFileStepV1 load_file(const std::string& uri,const char* root,std::string& error)override {
        require(id==42&&offset==std::array<float,3>{1,2,3},"Pending module lost bound context");
        switch(file.step(uri,root,consumer)) {
        case LevelFileWalkStepV1::pending:return ModuleFileStepV1::pending;
        case LevelFileWalkStepV1::complete:return ModuleFileStepV1::complete;
        case LevelFileWalkStepV1::failed:error=file.error();return ModuleFileStepV1::failed;
        }throw std::runtime_error("Invalid file status");
    }
    bool discard_file(std::string& error)override{return file.discard(consumer,error);}
};
ModuleLoadV1 module() {
    XmlDocumentV1 doc;std::string error,raw="<Level><GameObject name=\"module\" gametype=\"Module\"/></Level>";
    require(doc.capture("module-fixture",{raw.begin(),raw.end()},error),error);ObjectEntryV1 entry;ModuleLoadV1 out;
    require(prepare_object_entry_v1(doc.borrow(),1,ObjectEntryRouteV1::level,{},entry,error),error);
    require(prepare_module_load_v1(std::move(entry),42,{1,2,3},out,error),error);return out;
}
unsigned adapter_checks(const char* fixture_path) {
    unsigned checked{};std::string error;
    for(bool changed_root:{false,true}) {
        CachedLevelFileV1 file(archive(fixture_path));Services services;
        require(file.step("fixture/single.xml","Module",services)==LevelFileWalkStepV1::pending,"Request not pending");
        require(file.step(changed_root?"fixture/single.xml":"fixture/comments.xml",changed_root?"Level":"Module",services)==LevelFileWalkStepV1::failed,"Changed pending request accepted");
        const auto count=services.events.size();require(file.source()&&file.requested_uri()=="fixture/single.xml","Changed request lost original source");
        require(file.step("fixture/single.xml","Module",services)==LevelFileWalkStepV1::failed&&services.events.size()==count,"Failed request restarted");
        services.failing="release_load_state";require(!file.discard(services,error)&&file.source(),"Failed discard lost source");
        services.failing.clear();require(file.discard(services,error)&&!file.source()&&!file.active(),error);++checked;
    }
    {
        CachedLevelFileV1 file(archive(fixture_path));Services services;
        require(file.step("__missing__.xml","Module",services)==LevelFileWalkStepV1::failed&&services.events.empty(),"Missing XML processed elements");
        require(file.active()&&!file.source(),"Missing request lost diagnostic state");
        require(file.discard(services,error),error);finish(file,"fixture/single.xml",services);++checked;
    }
    {
        auto failing=std::make_shared<bool>(false);CachedLevelFileV1 file(archive(fixture_path,failing));Services services;*failing=true;
        require(file.step("fixture/single.xml","Module",services)==LevelFileWalkStepV1::failed&&services.events.empty(),"Backing failure processed XML");
        *failing=false;require(file.step("fixture/single.xml","Module",services)==LevelFileWalkStepV1::failed,"Read failure silently restarted");
        require(file.discard(services,error),error);finish(file,"fixture/single.xml",services);++checked;
    }
    {
        CachedLevelFileV1 file(archive(fixture_path));Services services;
        require(file.step("fixture/unsupported_nul.xml","Module",services)==LevelFileWalkStepV1::failed,"Unsupported raw bytes accepted");
        require(!file.raw_source().empty()&&file.raw_source().at(8)==0&&!file.source()&&services.events.empty(),"Rejected raw bytes lost");
        require(file.discard(services,error)&&file.raw_source().empty(),error);++checked;
    }
    {
        CachedLevelFileV1 file(archive(fixture_path));Services services;services.failing="load_element";
        require(file.step("fixture/single.xml","Module",services)==LevelFileWalkStepV1::pending,"Element fixture not pending");
        require(file.step("fixture/single.xml","Module",services)==LevelFileWalkStepV1::failed&&file.source(),"Unavailable entry service accepted");
        services.failing.clear();require(file.discard(services,error),error);++checked;
    }
    {
        CachedLevelFileV1 file(archive(fixture_path));Services services;
        require(file.step("fixture/bad_close.xml","Module",services)==LevelFileWalkStepV1::failed&&!services.parsed&&services.declarations.empty()&&file.source(),"Partial parse tree processed");
        require(file.discard(services,error),error);++checked;
    }
    {
        CachedLevelFileV1 file(archive(fixture_path));Services services;
        finish(file,"fixture/single.xml",services);finish(file,"fixture/single.xml",services);
        require(services.parses==2&&services.declarations.size()==2&&services.releases==2,"Repeated occurrence collapsed");++checked;
    }
    {
        Services services;
        {CachedLevelFileV1 file(archive(fixture_path));finish(file,"debug/alias.mgp",services);
         require(file.resolved_uri()=="data/scene/alias.mgp","Compiled-path fallback failed");}
        require(services.declarations.size()==1&&services.declarations[0].document.parsed(),"Recipient lost source after cache/file facade destruction");++checked;
    }
    {
        auto state=module();ModuleServices services(archive(fixture_path));
        for(unsigned i=0;i<1000&&!state.completed;++i)require(step_module_load_v1(state,services)!=ModuleLoadStepV1::failed,state.error);
        require(state.completed&&services.consumer.declarations.size()==3&&services.consumer.parses==2,"Real module/file pipeline failed");
        require(services.id==-1&&services.offset==std::array<float,3>{0,0,0},"Completed pipeline left context");
        require(services.consumer.declarations[0].document.uri()=="fixture/single.xml"&&services.consumer.declarations[1].document.uri()=="fixture/comments.xml","Visual preceded gameplay");++checked;
    }
    {
        auto state=module();ModuleServices services(archive(fixture_path));services.gameplay="fixture/bad_close.xml";
        require(step_module_load_v1(state,services)==ModuleLoadStepV1::failed&&services.consumer.parses==1&&services.consumer.declarations.empty(),"Failed gameplay file reached visual/partial elements");
        require(discard_module_load_v1(state,services,error)&&services.id==-1&&!services.file.source(),error);++checked;
    }
    {
        auto state=module();ModuleServices services(archive(fixture_path));
        require(step_module_load_v1(state,services)==ModuleLoadStepV1::pending,"Module cancellation not pending");
        services.consumer.failing="release_load_state";
        require(!discard_module_load_v1(state,services,error)&&state.entry.document&&services.file.source()&&services.id==42,"Failed file cleanup cleared bound context");
        services.consumer.failing.clear();require(discard_module_load_v1(state,services,error)&&services.id==-1&&!services.file.source()&&!state.entry.document,error);++checked;
    }
    return checked;
}
}
int main(int argc,char** argv) {
    if(argc!=3)return 2;
    try {
        const std::array<assets::ZipAssetPackV1,2> packs{archive(argv[1]),archive(argv[2])};
        const auto count=number();require(count<10000,"Too many cases");std::cout<<"{\"cases\":[";
        for(unsigned i=0;i<count;++i) {
            const auto pack=number();require(pack<2,"Invalid archive index");const auto label=blob(),uri=blob(),root=blob(),raw=blob();
            CachedLevelFileV1 file(packs[pack]);Services services;std::vector<std::string> calls;bool unsafe{},preserved{};
            for(services.call=0;services.call<65536;++services.call) {
                const auto start=services.events.size();const auto result=file.step(uri,root,services);
                if(services.call==0){require(std::string(file.raw_source().begin(),file.raw_source().end())==raw,"Cache raw source changed");preserved=true;}
                unsafe=file.failed()&&file.error().find("Matching top-level node")!=std::string::npos;
                if(unsafe)break;
                calls.push_back(std::string("{\"returned\":")+(result==LevelFileWalkStepV1::pending?"false":"true")
                    +",\"event_count\":"+std::to_string(services.events.size()-start)+",\"load_state_present\":"+(file.source()?"true":"false")+'}');
                if(result!=LevelFileWalkStepV1::pending)break;
            }
            require(file.failed()||!file.active(),"Cache file did not finish");const auto events=services.events;std::string cleanup="null";
            if(file.failed()&&!unsafe) {
                services.call=static_cast<unsigned>(calls.size());const auto start=services.events.size();std::string error;
                require(file.discard(services,error),error);cleanup="{\"returned\":true,\"events\":[";
                for(std::size_t j=start;j<services.events.size();++j){if(j>start)cleanup+=',';cleanup+=services.events[j];}
                cleanup+="],\"load_state_present\":false}";
            }
            if(i)std::cout<<',';
            std::cout<<"{\"label\":"<<quote(label)<<",\"parse_success\":"<<(services.parsed?"true":"false")
                <<",\"unsafe\":"<<(unsafe?"true":"false")<<",\"calls\":[";
            for(std::size_t j=0;j<calls.size();++j){if(j)std::cout<<',';std::cout<<calls[j];}
            std::cout<<"],\"events\":[";
            for(std::size_t j=0;j<events.size();++j){if(j)std::cout<<',';std::cout<<events[j];}
            std::cout<<"],\"failure_cleanup_poll\":"<<cleanup<<",\"raw_source_preserved\":"<<(preserved?"true":"false")<<'}';
        }
        std::cout<<"],\"adapter_checks\":"<<adapter_checks(argv[2])<<"}\n";
    }catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}
}
