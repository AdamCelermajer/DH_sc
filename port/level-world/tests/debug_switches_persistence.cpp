#include "../debug_switches_persistence.hpp"
#include "../../persistence/binary.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>

namespace p=dh2::debug_switches_persistence;
namespace d=dh2::debug_switches;
constexpr std::uintptr_t D=0x9a1d18,D2=0x10030000,FS=0x10040000,F=0x10050000;
struct Call {unsigned op;std::uintptr_t owner,fs,stream;std::uint32_t value;std::string text;};
struct Fixture {
    d::Runtime first{D},second{D2};d::FileSystem filesystem{FS};d::Engine engine{0x10041000,&filesystem};d::Application application{0x10042000,&engine};d::Globals globals{1,&first,&application};
    d::Services debug{this,debug_call};p::Services services{this,io_call};
    std::map<std::uintptr_t,std::vector<std::uint8_t>> streams;std::vector<Call> calls;
    unsigned mutation=0,fail=0,opened=0;bool throws=false,missing=false,mutated=false;
    p::Map& switches(d::Runtime& r){return const_cast<p::Map&>(r.switches());}
    p::Map& modules(d::Runtime& r){return const_cast<p::Map&>(r.modules());}
    p::Owner owner(d::Runtime& r){return {r.identity(),&r.switches(),&r.modules()};}
    void change(const p::Request& request) {
        const auto op=request.operation;
        if(mutation==1 && op==p::Operation::open_write){engine.files=nullptr;application.engine=nullptr;globals.application=nullptr;}
        if(mutated)return;
        if(mutation==2 && op==p::Operation::write_word && request.word==0x44425357u){modules(first)["AddedModule"]=1;switches(first)["AddedSwitch"]=1;mutated=true;}
        if(mutation==3 && op==p::Operation::write_word && calls.size()==3){modules(first)["LaterModule"]=1;mutated=true;}
        if(mutation==4 && op==p::Operation::write_word && calls.size()==4){switches(first)["AfterCount"]=1;mutated=true;}
        if(mutation==5 && op==p::Operation::write_string){const std::string name(request.bytes,request.size);auto& s=switches(first);auto& m=modules(first);if(s.count(name))s[name]=7;else m[name]=7;mutated=true;}
        if(mutation==6 && op==p::Operation::write_byte){switches(first)["ZZInserted"]=1;mutated=true;}
        if(mutation==7 && op==p::Operation::write_string){globals.singleton=&second;mutated=true;}
        if(mutation==8 && op==p::Operation::write_word){services.invoke=nullptr;debug.invoke=nullptr;mutated=true;}
        if(mutation==10 && op==p::Operation::write_word && calls.size()==4){globals.singleton=&second;mutated=true;}
    }
    static std::int32_t io_call(void* context,const p::Request* request,p::Stream* reply) {
        auto& f=*static_cast<Fixture*>(context);const auto op=unsigned(request->operation);
        f.calls.push_back({op,request->owner,request->filesystem,request->stream,request->word,request->bytes?std::string(request->bytes,request->size?request->size:std::char_traits<char>::length(request->bytes)):std::string{}});
        if(request->operation==p::Operation::open_write){assert(request->word==1 && std::string(request->bytes)=="DebugSwitches.savegame");if(!f.missing){*reply={F+4*(++f.opened)};f.streams[reply->identity].clear();}}
        else if(request->operation==p::Operation::write_word){auto& bytes=f.streams[request->stream];const auto pos=bytes.size();bytes.resize(pos+4);dh2_save_write32(bytes.data()+pos,request->word);}
        else if(request->operation==p::Operation::write_byte)f.streams[request->stream].push_back(std::uint8_t(request->word));
        else if(request->operation==p::Operation::write_string){auto& bytes=f.streams[request->stream];const auto pos=bytes.size();bytes.resize(pos+4);dh2_save_write32(bytes.data()+pos,std::uint32_t(request->size));bytes.insert(bytes.end(),request->bytes,request->bytes+request->size);}
        f.change(*request);
        if(f.fail==f.calls.size()){if(f.throws)throw std::runtime_error("io error");return 1;}
        return 0;
    }
    static std::int32_t debug_call(void* context,const d::Request* request,d::File* reply) {
        auto& f=*static_cast<Fixture*>(context);
        if(request->operation==d::Operation::open_read){f.calls.push_back({5,request->owner->identity(),request->filesystem,0,0,"DebugSwitches.savegame"});*reply={};return 0;}
        if(request->operation==d::Operation::close)return 1;
        p::Result result{};const auto status=p::save(f.owner(*request->owner),f.globals,f.debug,f.services,result);
        return status==p::Status::complete?0:1;
    }
};
void word(std::vector<std::uint8_t>& bytes,std::uint32_t value){const auto pos=bytes.size();bytes.resize(pos+4);dh2_save_write32(bytes.data()+pos,value);}
std::string hex(const char* bytes,std::size_t size){std::string out;constexpr const char* digits="0123456789abcdef";for(std::size_t i=0;i<size;++i){const auto b=std::uint8_t(bytes[i]);out.push_back(digits[b>>4]);out.push_back(digits[b&15]);}return out;}
void map_json(const p::Map& rows){std::cout<<'{';bool first=true;for(const auto& row:rows){if(!first)std::cout<<',';first=false;std::cout<<'"'<<hex(row.first.data(),row.first.size())<<"\":"<<unsigned(row.second);}std::cout<<'}';}
void emit(const Fixture& f,p::Status status,const p::Result& result){
    std::cout<<"{\"status\":"<<unsigned(status)<<",\"loaded\":"<<unsigned(f.globals.loaded)<<",\"singleton\":"<<f.globals.singleton->identity()<<",\"modules\":";map_json(f.first.modules());std::cout<<",\"first\":";map_json(f.first.switches());std::cout<<",\"second\":";map_json(f.second.switches());
    std::cout<<",\"streams\":{";bool first=true;for(const auto& stream:f.streams){if(!first)std::cout<<',';first=false;std::cout<<'"'<<stream.first<<"\":\""<<hex(reinterpret_cast<const char*>(stream.second.data()),stream.second.size())<<'"';}std::cout<<"},\"calls\":[";first=true;for(const auto& c:f.calls){if(!first)std::cout<<',';first=false;std::cout<<'['<<c.op<<','<<c.owner<<','<<c.fs<<','<<c.stream<<','<<c.value<<",\""<<hex(c.text.data(),c.text.size())<<"\"]";}std::cout<<"],\"result\":["<<result.calls<<','<<result.module_count<<','<<result.switch_count<<','<<result.modules_written<<','<<result.switches_written<<"]}\n";
}
void setup(Fixture& f,const char* filename){std::ifstream file(filename,std::ios::binary);if(!file)throw std::runtime_error("fixture missing");std::vector<std::uint8_t> bytes{std::istreambuf_iterator<char>(file),{}};std::size_t pos=0;auto read_word=[&](){if(bytes.size()-pos<4)throw std::runtime_error("fixture word");const auto value=dh2_save_read32(bytes.data()+pos);pos+=4;return value;};if(read_word()!=0x44425357 || read_word()!=0x20000)throw std::runtime_error("fixture header");auto rows=[&](p::Map& out){const auto count=read_word();for(std::uint32_t i=0;i<count;++i){const auto len=read_word();if(bytes.size()-pos<=len)throw std::runtime_error("fixture row");std::string name(reinterpret_cast<const char*>(bytes.data()+pos),len);pos+=len;out[name]=bytes[pos++];}};rows(f.modules(f.first));rows(f.switches(f.first));if(pos!=bytes.size())throw std::runtime_error("fixture suffix");}
int main(int argc,char** argv){try {
    if(argc>1){if(argc!=8)return 2;Fixture f;setup(f,argv[1]);const auto mode=std::stoul(argv[2]);f.globals.loaded=std::uint8_t(std::stoul(argv[3]));f.mutation=std::stoul(argv[4]);f.missing=std::stoul(argv[5])!=0;if(std::stoul(argv[6]))f.engine.files=nullptr;const bool null_stream=std::stoul(argv[7])!=0;p::Result result{};const auto status=mode?p::save(f.owner(f.first),f.globals,f.debug,f.services,result):p::write(f.owner(f.first),{null_stream?0:F},f.globals,f.debug,f.services,result);emit(f,status,result);return 0;}
    unsigned cases=0;
    {Fixture f;p::Result result{};assert(p::write(f.owner(f.first),{F},f.globals,f.debug,f.services,result)==p::Status::complete && f.streams[F].size()==16 && result.switch_count==0 && f.calls.size()==4);++cases;}
    {Fixture f;f.switches(f.first)["A"]=1;p::Result result{};assert(p::write(f.owner(f.first),{F},f.globals,f.debug,f.services,result)==p::Status::complete && result.switch_count==1 && result.switches_written==3 && f.first.switches().count("isTracingDebugSwitchesFile") && f.first.switches().count("isTracingDebugSwitches"));++cases;}
    {Fixture f;f.modules(f.first)["Module"]=1;f.switches(f.first)["A"]=1;p::Result result{};assert(p::save(f.owner(f.first),f.globals,f.debug,f.services,result)==p::Status::complete && result.modules_written==1 && f.calls.front().op==0 && f.calls.back().op==4 && f.calls.back().fs==FS);++cases;}
    for(unsigned mutation:{1u,2u,3u,4u,5u,6u,7u,8u,10u}){Fixture f;f.switches(f.first)["A"]=1;f.switches(f.first)["B"]=0;f.mutation=mutation;p::Result result{};const auto status=mutation==1?p::save(f.owner(f.first),f.globals,f.debug,f.services,result):p::write(f.owner(f.first),{F},f.globals,f.debug,f.services,result);assert(status==p::Status::complete);if(mutation==1)assert(f.calls.back().fs==FS);if(mutation==3)assert(result.module_count==0 && result.modules_written==1);if(mutation==4)assert(result.switch_count==2 && result.switches_written==5);if(mutation==5)assert(f.switches(f.first).at("A")==7 && f.streams[F][21]==7);if(mutation==6)assert(f.switches(f.first).count("ZZInserted"));if(mutation==7)assert(f.globals.singleton==&f.second && f.second.switches().empty());if(mutation==10)assert(f.second.switches().count("isTracingDebugSwitchesFile") && !f.first.switches().count("isTracingDebugSwitchesFile"));++cases;}
    for(bool throws:{false,true})for(unsigned phase=1;phase<=12;++phase){Fixture f;f.switches(f.first)["A"]=1;f.fail=phase;f.throws=throws;p::Result result{};assert(p::save(f.owner(f.first),f.globals,f.debug,f.services,result)==p::Status::service_failed && f.calls.size()==phase);if(phase<12)assert(f.calls.back().op!=4);++cases;}
    {Fixture f;f.engine.files=nullptr;p::Result result{};assert(p::save(f.owner(f.first),f.globals,f.debug,f.services,result)==p::Status::complete && f.calls.empty());++cases;}
    {Fixture f;f.missing=true;p::Result result{};assert(p::save(f.owner(f.first),f.globals,f.debug,f.services,result)==p::Status::complete && f.calls.size()==1 && f.streams.empty());++cases;}
    {Fixture f;f.services.invoke=nullptr;f.globals.singleton=nullptr;p::Result result{};assert(p::write({D,nullptr,nullptr},{0},f.globals,f.debug,f.services,result)==p::Status::complete && f.calls.empty());++cases;}
    {Fixture f;f.services.invoke=nullptr;p::Result result{};assert(p::save(f.owner(f.first),f.globals,f.debug,f.services,result)==p::Status::service_unavailable && result.calls==0);++cases;}
    {Fixture f;f.globals.application=nullptr;p::Result result{};assert(p::save(f.owner(f.first),f.globals,f.debug,f.services,result)==p::Status::invalid_argument && f.calls.empty());++cases;}
    {Fixture f;f.globals.singleton=nullptr;f.switches(f.first)["A"]=1;p::Result result{};assert(p::write(f.owner(f.first),{F},f.globals,f.debug,f.services,result)==p::Status::invalid_argument && f.streams[F].size()==16);++cases;}
    {Fixture f;f.globals.loaded=0;f.switches(f.first)["A"]=1;p::Result result{};assert(p::write(f.owner(f.first),{F},f.globals,f.debug,f.services,result)==p::Status::complete && f.globals.loaded && f.calls[4].op==5 && result.switch_count==1 && result.switches_written==8);++cases;}
    {Fixture f;f.modules(f.first)["Module"]=1;f.mutation=5;p::Result result{};assert(p::write(f.owner(f.first),{F},f.globals,f.debug,f.services,result)==p::Status::complete && f.modules(f.first).at("Module")==7);++cases;}
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<",\"mismatches\":0}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}}
