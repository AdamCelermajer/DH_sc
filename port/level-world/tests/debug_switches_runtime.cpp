#include "../debug_switches_runtime.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>

namespace k=dh2::debug_switches;
constexpr std::uintptr_t D=0x9a1d18,D2=0x10030000,FS=0x10040000,F=0x10050000;
struct Call {unsigned op;std::uintptr_t owner,filesystem,file;std::size_t size;unsigned tracing,tracing_file,stats;};
struct Fixture {
    k::Runtime first{D},second{D2};k::FileSystem filesystem{FS};k::Engine engine{0x10041000,&filesystem};k::Application application{0x10042000,&engine};k::Globals globals{0,&first,&application};k::Services services{this,invoke};
    std::vector<std::uint8_t> bytes;std::vector<Call> calls;unsigned mutation=0,fail=0;bool throws=false,missing=false;std::string* key=nullptr;
    static std::int32_t invoke(void* context,const k::Request* request,k::File* reply) {
        auto& f=*static_cast<Fixture*>(context);const auto op=unsigned(request->operation);const auto& m=request->owner->switches();
        f.calls.push_back({op,request->owner->identity(),request->filesystem,request->file?request->file->identity:0,m.size(),unsigned(m.count("isTracingDebugSwitches")),unsigned(m.count("isTracingDebugSwitchesFile")),unsigned(m.count("isTracingChar_Stats"))});
        if(op==0){assert(request->path && std::string(request->path)=="DebugSwitches.savegame");if(!f.missing)*reply={F,f.bytes.empty()?nullptr:f.bytes.data(),f.bytes.size()};}
        if(f.mutation==1 && op==0)f.globals.singleton=&f.second;
        if(f.mutation==2 && op==0){f.engine.files=nullptr;f.application.engine=nullptr;f.globals.application=nullptr;}
        if(f.mutation==3 && op==0 && f.key)*f.key="ChangedKey";
        if(f.mutation==4 && op==2 && f.key)*f.key="ChangedKey";
        if(f.mutation==5 && op==0)f.services.invoke=nullptr;
        if(f.mutation==6 && op==2 && !f.bytes.empty())f.bytes.back()=1;
        if(f.mutation==7 && op==2)for(const auto& c:f.calls)if(c.op==1)f.globals.singleton=&f.second;
        if(f.fail==f.calls.size()){if(f.throws)throw std::runtime_error("provider error");return 1;}
        return 0;
    }
};
std::vector<std::uint8_t> read(const char* path){std::ifstream f(path,std::ios::binary);if(!f)throw std::runtime_error("missing fixture");return {std::istreambuf_iterator<char>(f),{}};}
void word(std::vector<std::uint8_t>& b,std::uint32_t x){for(unsigned i=0;i<4;++i)b.push_back(std::uint8_t(x>>(8*i)));}
std::vector<std::uint8_t> config(const std::vector<std::pair<std::string,unsigned>>& rows,std::uint32_t version=0x20000,std::uint32_t modules=0){std::vector<std::uint8_t> b;word(b,0x44425357);word(b,version);if(version>=0x20000)word(b,modules);word(b,std::uint32_t(rows.size()));for(const auto& r:rows){word(b,std::uint32_t(r.first.size()));b.insert(b.end(),r.first.begin(),r.first.end());b.push_back(std::uint8_t(r.second));}return b;}
void map(const std::map<std::string,std::uint8_t>& rows){std::cout<<'{';bool first=true;for(const auto& row:rows){if(!first)std::cout<<',';first=false;std::cout<<'"'<<row.first<<"\":"<<unsigned(row.second);}std::cout<<'}';}
void emit(const Fixture& f,k::Status status,std::uint8_t value){std::cout<<"{\"status\":"<<unsigned(status)<<",\"value\":"<<unsigned(value)<<",\"loaded\":"<<unsigned(f.globals.loaded)<<",\"singleton\":"<<(f.globals.singleton?f.globals.singleton->identity():0)<<",\"position\":"<<f.first.last_file_position()<<",\"first\":";map(f.first.switches());std::cout<<",\"second\":";map(f.second.switches());std::cout<<",\"calls\":[";bool first=true;for(const auto& c:f.calls){if(!first)std::cout<<',';first=false;std::cout<<'['<<c.op<<','<<c.owner<<','<<c.filesystem<<','<<c.file<<','<<c.size<<','<<c.tracing<<','<<c.tracing_file<<','<<c.stats<<']';}std::cout<<"]}\n";}
int main(int argc,char** argv){try {
    if(argc>1){if(argc!=7)return 2;Fixture f;f.bytes=read(argv[1]);const auto mode=std::stoul(argv[2]);f.globals.loaded=std::uint8_t(std::stoul(argv[3]));f.mutation=std::stoul(argv[4]);f.missing=std::stoul(argv[5])!=0;std::string key=argv[6];f.key=&key;std::uint8_t value=77;k::Status status;
        if(mode==0)status=f.first.load(f.globals,f.services);else if(mode==1)status=f.first.get_switch(key,f.globals,f.services,value);else status=f.first.set_switch(key,std::uint8_t(mode==3),f.globals,f.services);emit(f,status,value);return 0;}
    unsigned cases=0;
    {Fixture f;f.bytes=config({{"Existing",1},{"isTracingDebugSwitches",1},{"isTracingDebugSwitchesFile",1}});assert(f.first.load(f.globals,f.services)==k::Status::complete && f.globals.loaded && f.first.switches().at("Existing")==1);const auto count=f.calls.size();assert(f.first.load(f.globals,f.services)==k::Status::complete && f.calls.size()==count);++cases;}
    {Fixture f;f.globals.loaded=1;std::uint8_t value=77;assert(f.first.get_switch("Missing",f.globals,f.services,value)==k::Status::complete && value==0 && f.first.switches().size()==2 && f.first.switches().at("isTracingDebugSwitches")==0 && f.calls.empty());++cases;}
    {Fixture f;f.globals.loaded=1;assert(f.first.set_switch("Missing",0,f.globals,f.services)==k::Status::complete && f.calls.empty());assert(f.first.set_switch("Missing",1,f.globals,f.services)==k::Status::complete && f.calls.size()==1);assert(f.first.set_switch("Missing",1,f.globals,f.services)==k::Status::complete && f.calls.size()==1);++cases;}
    for(unsigned version:{0x10000u,0x1ffffu,0x20000u,0x20001u}){Fixture f;f.bytes=config({{"Actual",9}},version);assert(f.first.load(f.globals,f.services)==k::Status::complete && f.first.switches().at("Actual")==1 && f.first.last_file_position()==f.bytes.size());++cases;}
    for(unsigned size=0;size<=11;++size){Fixture f;f.bytes.resize(size);assert(f.first.load(f.globals,f.services)==k::Status::complete && f.first.last_file_position()==0 && f.calls.size()==2);++cases;}
    {Fixture f;f.bytes=config({{"First",1},{"Later",0}});f.bytes.pop_back();assert(f.first.load(f.globals,f.services)==k::Status::invalid_argument && f.globals.loaded && f.first.switches().at("First")==1 && f.calls.back().op==2);++cases;}
    for(bool throws:{false,true})for(unsigned phase=1;phase<=3;++phase){Fixture f;f.bytes=config({{"Actual",1}});f.fail=phase;f.throws=throws;assert(f.first.load(f.globals,f.services)==k::Status::service_failed && f.globals.loaded && f.calls.size()==phase);if(phase>=2)assert(f.first.switches().at("Actual")==1);++cases;}
    for(unsigned mutation=1;mutation<=5;++mutation){Fixture f;f.bytes=config({{"Actual",1}});f.mutation=mutation;std::string key="OldKey";f.key=&key;std::uint8_t value=77;assert(f.first.get_switch(key,f.globals,f.services,value)==k::Status::complete && value==0);if(mutation==1)assert(f.second.switches().count("ConnectToAlphaServer") && f.first.switches().at("Actual")==1);if(mutation==2)assert(f.calls.back().op==1 && f.calls.back().filesystem==FS);if(mutation==3)assert(f.first.switches().count("OldKey") && f.first.switches().count("ChangedKey"));++cases;}
    {Fixture f;f.bytes=config({{"First",1},{"Later",0}});f.mutation=6;assert(f.first.load(f.globals,f.services)==k::Status::complete && f.first.switches().at("Later")==1 && f.calls.size()==4);++cases;}
    {Fixture f;f.bytes=config({{"IsDeactivatingFlashMenus",1},{"IsDeactivatingFlashMenusUpdate",1},{"IsDeactivatingFlashMenusRender",1}});f.mutation=7;assert(f.first.load(f.globals,f.services)==k::Status::complete && f.globals.singleton==&f.second && f.first.switches().at("IsDeactivatingFlashMenusUpdate")==0 && f.first.switches().at("IsDeactivatingFlashMenusRender")==0 && f.first.switches().count("ConnectToBetaServer") && !f.second.switches().count("ConnectToBetaServer"));++cases;}
    {Fixture f;f.missing=true;assert(f.first.load(f.globals,f.services)==k::Status::complete && f.calls.size()==1 && f.first.switches().count("ConnectToAlphaServer"));++cases;}
    {Fixture f;f.engine.files=nullptr;assert(f.first.load(f.globals,f.services)==k::Status::complete && f.calls.empty() && f.first.switches().size()==6);++cases;}
    for(unsigned mode=0;mode<3;++mode){Fixture f;f.bytes=config({},mode==0?0:0x20000,mode==1?1:0);if(mode==2)f.bytes[0]=0;assert(f.first.load(f.globals,f.services)==k::Status::unsupported_configuration && f.globals.loaded && f.calls.size()==1);++cases;}
    {Fixture f;f.services.invoke=nullptr;assert(f.first.load(f.globals,f.services)==k::Status::service_unavailable && f.globals.loaded);++cases;}
    {Fixture f;f.globals.application=nullptr;assert(f.first.load(f.globals,f.services)==k::Status::invalid_argument && f.globals.loaded && f.calls.empty());++cases;}
    {Fixture f;f.globals.loaded=1;f.services.invoke=nullptr;std::uint8_t value=77;assert(f.first.get_switch("Missing",f.globals,f.services,value)==k::Status::complete && value==0);assert(f.first.set_switch("Missing",1,f.globals,f.services)==k::Status::service_unavailable && f.first.switches().at("Missing")==1);++cases;}
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<",\"mismatches\":0}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}}
