#include "../character_dot_attack.hpp"
#include "../../android-native/app/src/main/cpp/native_debug_files.hpp"
#include <array>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>

namespace da=dh2::character_dot_attack;
namespace ds=dh2::debug_switches;
namespace nf=dh2::native::debug_files;
namespace fs=std::filesystem;
constexpr std::uintptr_t A=0x10014000,B=0x10018000,D=0x9a1d18,D2=0x10030000;
using Bytes=std::vector<std::uint8_t>;
void require(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
Bytes read(const fs::path& path){std::ifstream input(path,std::ios::binary);require(bool(input),"file read");return Bytes(std::istreambuf_iterator<char>(input),{});}
struct Fixture {
    std::array<std::int32_t,224> first{},second{};
    std::array<da::Actor,2> actors{{{A,first.data()},{B,second.data()}}};
    da::CombatContext combat{};dh2::data::CombatRandom random{1234,77};
    ds::Runtime debug{D};ds::Globals globals{1,&debug,nullptr};ds::Services debug_services{};
    da::Storage storage{actors.data(),2,&combat,&random,&globals,&debug_services};
    da::Runtime runtime;da::Arguments arguments{A,B,1024,-1};
    dh2::data::CombatResult output{};da::Result result{};da::Status status=da::Status::complete;
    Fixture(){first[19]=1024;second[19]=512;}
    void run(){status=runtime.attack(&storage,&arguments,&output,&result);}
};
void print(const Fixture& f){
    const auto& o=f.output;const auto& c=f.combat;const auto& r=f.result;
    std::cout<<"{\"status\":"<<std::int32_t(f.status)<<",\"report\":["<<r.calls<<','<<r.last_operation<<','<<r.loads<<','<<r.constructed<<','<<r.queries<<','<<r.destroyed<<','<<r.calculated<<','<<r.level_reads
             <<"],\"output\":["<<o.amount<<','<<o.dot_element<<','<<o.dot_duration<<','<<o.dot_amount<<','<<o.hp_leech<<','<<o.mp_leech<<','<<o.outcomes<<','<<o.mask<<','<<o.weapon_category<<','<<o.element
             <<"],\"context\":["<<c.attacker<<','<<c.defender<<','<<c.level_delta<<','<<c.reverse_level_delta<<','<<c.element<<','<<unsigned(c.offhand)<<','<<unsigned(c.magic)<<','<<unsigned(c.blocked)<<','<<unsigned(c.critical)
             <<"],\"random\":["<<f.random.seed<<','<<f.random.calls<<"],\"retained_strings\":"<<f.runtime.retained_strings()<<"}\n";
}
struct Caller {
    da::Arguments arguments{A,B,0xffffffffu,4};da::Globals globals{D};da::Services services{this,invoke};
    dh2::data::CombatResult output{};da::Result result{};da::Status status{};
    std::map<std::uintptr_t,std::unique_ptr<std::string>> strings;
    std::vector<da::Operation> calls;
    unsigned fail=0,throws=0,mutation=0;std::uint32_t query_word=0;std::uintptr_t selected=0;
    static std::int32_t invoke(void* p,const da::Request* request,da::Reply* reply){
        auto& c=*static_cast<Caller*>(p);c.calls.push_back(request->operation);
        switch(request->operation){
        case da::Operation::debug_load:
            c.selected=request->subject;if(c.mutation==1)c.globals.debug_switches=D2;break;
        case da::Operation::string_construct:{
            require(std::string(request->text)=="isTracingChar_Attack","original key");
            auto text=std::make_unique<std::string>(request->text);const auto id=reinterpret_cast<std::uintptr_t>(text.get());reply->identity=id;c.strings.emplace(id,std::move(text));
            if(c.mutation==2)c.globals.debug_switches=D2;
            break;
        }
        case da::Operation::debug_query:
            require(request->subject==c.selected&&c.strings.count(request->string),"captured owner/string query");reply->word=c.query_word;
            if(c.mutation==3)c.globals.debug_switches=D2;
            break;
        case da::Operation::string_destroy:
            require(c.strings.erase(request->subject)==1,"real string release");break;
        case da::Operation::calculate_result:
            require(request->arguments.attacker==A&&request->arguments.defender==B&&request->arguments.amount==0xffffffffu&&request->arguments.element==4&&request->mask==0x20080000u&&request->weapon_category==-1,"exact calculation request");
            // Named calculation boundary fixture for caller-only error tests.
            // Positive native composition below uses the genuine Runtime path.
            request->output->amount=-1;break;
        }
        if(c.calls.size()==c.throws)throw std::runtime_error("provider throw");
        return c.calls.size()==c.fail?1:0;
    }
    void run(){status=da::execute(&arguments,&globals,&services,&output,&result);}
};
struct RealFixture:Fixture {
    nf::Backend backend;
    RealFixture(const fs::path& path,const Bytes* seed=nullptr){
        std::string error;require(backend.initialize(path,seed?seed->data():nullptr,seed?seed->size():0,error),error.c_str());
        storage.debug_globals=&backend.globals();storage.debug_services=&backend.services();
    }
};
struct CloseFailure {
    nf::Backend* backend;
    static std::int32_t invoke(void* p,const ds::Request* request,ds::File* reply){
        auto& f=*static_cast<CloseFailure*>(p);const auto& service=f.backend->services();
        const auto status=service.invoke(service.context,request,reply);
        return request->operation==ds::Operation::close?1:status;
    }
};
unsigned behavior=0,guards=0,failures=0,real_cases=0;
void host(const fs::path& configuration,const fs::path& parent){
    const auto seed=read(configuration);require(seed.size()==665,"original cache configuration");fs::create_directories(parent);
    auto folder=[&](){const auto path=parent/std::to_string(real_cases);require(!fs::exists(path),"fresh case directory");fs::create_directory(path);return path;};
    for(auto amount:{0u,1u,256u,0x7fffffffu,0x80000000u,0xffffffffu})for(int element=-1;element<=4;++element){
        Fixture f;f.arguments.amount=amount;f.arguments.element=element;f.run();std::int32_t expected;std::memcpy(&expected,&amount,4);
        require(f.status==da::Status::complete&&f.output.amount==expected&&f.output.mask==0x20080000u&&f.output.element==element&&f.result.loads==2&&f.result.queries==2&&f.result.calls==9&&f.result.level_reads==2&&f.result.calculated==1&&f.runtime.retained_strings()==0&&f.random.seed==1234&&f.random.calls==77,"direct result/two real map queries");++behavior;
    }
    for(unsigned mutation=0;mutation<4;++mutation)for(auto raw_query:{0u,1u,0x80000000u,0xffffffffu}){
        Caller c;c.mutation=mutation;c.query_word=raw_query;c.run();require(c.status==da::Status::complete&&c.calls.size()==5&&c.result.queries==1&&c.result.calculated==1&&c.strings.empty(),"caller capture/discard query");++behavior;
    }
    for(unsigned i=1;i<=5;++i)for(bool throwing:{false,true}){
        Caller c;if(throwing)c.throws=i;else c.fail=i;c.run();require(c.status==da::Status::service_failed&&c.calls.size()==i,"error/throw no extra operation");
        if(i==3)require(c.strings.size()==1&&c.result.destroyed==0,"failed query retains real string");
        ++failures;
    }
    {Fixture f;f.arguments.defender=A;f.run();require(f.status==da::Status::complete&&f.combat.level_delta==0&&f.combat.reverse_level_delta==0,"self identity");++behavior;}
    {Fixture f;f.first[19]=INT32_MIN;f.second[19]=INT32_MAX;f.run();require(f.status==da::Status::complete&&f.combat.level_delta==1&&f.combat.reverse_level_delta==0xffffffffu,"context wrap32");++behavior;}
    {Fixture f;f.actors[1].identity=A;f.run();require(f.status==da::Status::actor_unavailable&&f.result.loads==1&&f.output.mask==0x20080000u&&f.combat.attacker==A&&f.combat.defender==B,"duplicate/absent actor preserves prefix reset/context");++guards;}
    {Fixture f;f.arguments.attacker=0;f.run();require(f.status==da::Status::invalid_argument&&f.result.calls==0,"unsupported original null diagnostic");++guards;}
    {Fixture f;f.storage.actor_count=0;f.run();require(f.status==da::Status::invalid_argument&&f.result.calls==0,"empty actor registry");++guards;}
    {Fixture f;f.storage.actor_count=65537;f.run();require(f.status==da::Status::invalid_argument,"actor count bound");++guards;}
    {Fixture f;f.storage.debug_globals=nullptr;f.run();require(f.status==da::Status::invalid_argument,"debug control absent");++guards;}
    {Fixture f;f.storage.combat_context=nullptr;f.run();require(f.status==da::Status::invalid_argument,"context absent");++guards;}
    {Fixture f;f.storage.random=nullptr;f.run();require(f.status==da::Status::invalid_argument,"random absent");++guards;}
    {Fixture f;f.actors[0].resolved=reinterpret_cast<std::int32_t*>(&f.output);f.run();require(f.status==da::Status::invalid_argument,"sheet/output alias");++guards;}
    {Fixture f;f.storage.combat_context=reinterpret_cast<da::CombatContext*>(&f.output);f.run();require(f.status==da::Status::invalid_argument,"context/output alias");++guards;}
    {Fixture f;alignas(da::Result) unsigned char bytes[128]{};require(f.runtime.attack(&f.storage,&f.arguments,&f.output,reinterpret_cast<da::Result*>(bytes+1))==da::Status::invalid_argument,"report alignment");++guards;}
    {Fixture f;require(f.runtime.attack(nullptr,&f.arguments,&f.output,&f.result)==da::Status::invalid_argument,"storage null");++guards;}
    {Fixture f;require(f.runtime.attack(&f.storage,&f.arguments,nullptr,&f.result)==da::Status::invalid_argument,"output null");++guards;}
    {Caller c;c.services.invoke=nullptr;c.run();require(c.status==da::Status::service_unavailable&&c.result.calls==0,"reached missing service");++guards;}
    {Caller c;require(da::execute(&c.arguments,&c.globals,&c.services,reinterpret_cast<dh2::data::CombatResult*>(&c.services),&c.result)==da::Status::invalid_argument&&c.calls.empty(),"caller control alias");++guards;}
    {RealFixture f(folder(),&seed);f.run();const auto& n=f.backend.counters();
     require(f.status==da::Status::complete&&f.output.amount==1024&&f.result.loads==2&&f.result.queries==2&&f.runtime.retained_strings()==0&&n.read_opens==1&&n.read_closes==1&&n.save_completions==5&&n.write_closes==5&&f.backend.runtime().switches().size()==24&&f.backend.runtime().switches().at("isTracingChar_Attack")==0&&read(f.backend.filename())==seed,"actual two Debug phases/five nested live saves/result");++real_cases;}
    {RealFixture f(folder(),&seed);f.run();require(f.status==da::Status::complete,"initial real attack");
     require(f.backend.runtime().set_switch("isTracingChar_Attack",1,f.backend.globals(),f.backend.services())==ds::Status::complete,"real true switch persistence");
     const auto before=read(f.backend.filename());f.arguments.amount=2048;f.run();
     require(f.status==da::Status::complete&&f.output.amount==2048&&f.result.queries==2&&f.backend.runtime().switches().at("isTracingChar_Attack")==1&&f.backend.counters().save_completions==6&&read(f.backend.filename())==before,"true debug result discarded/persistence preserved");++real_cases;}
    {RealFixture f(folder());f.run();require(f.status==da::Status::complete&&f.output.amount==1024&&f.backend.counters().read_misses==1&&!f.backend.counters().save_attempts&&!fs::exists(f.backend.filename())&&f.backend.runtime().switches().count("isTracingChar_Attack"),"source ordinary missing configuration branch");++real_cases;}
    {const Bytes invalid{0xef,0xbe,0xad,0xde,0,0,2,0,0,0,0,0,0,0,0,0};RealFixture f(folder(),&invalid);f.output.amount=777;f.run();
     require(f.status==da::Status::debug_failed&&f.result.calls==1&&f.result.constructed==0&&f.output.amount==777&&f.backend.globals().loaded==1&&f.backend.active_reads()==1&&!f.backend.counters().read_closes,"real source load failure keeps prior file/guard and no reset");++real_cases;}
    {RealFixture f(folder(),&seed);CloseFailure bridge{&f.backend};ds::Services service{&bridge,CloseFailure::invoke};f.storage.debug_services=&service;f.output.amount=888;f.run();
     require(f.status==da::Status::debug_failed&&f.result.calls==1&&f.output.amount==888&&f.backend.counters().save_completions==5&&f.backend.counters().read_closes==1&&!f.backend.active_reads()&&read(f.backend.filename())==seed,"returned close error preserves actual completed saves/close with no reset");++real_cases;}
    std::cout<<"{\"validation\":\"PASS\",\"behavior_cases\":"<<behavior<<",\"guard_cases\":"<<guards<<",\"failure_cases\":"<<failures<<",\"real_file_cases\":"<<real_cases<<",\"mismatches\":0}\n";
}
int main(int argc,char** argv){try{
    if(argc==4&&std::string(argv[1])=="caller"){
        Caller c;c.mutation=unsigned(std::stoul(argv[2]));c.query_word=std::uint32_t(std::stoull(argv[3]));c.run();
        std::cout<<"{\"status\":"<<std::int32_t(c.status)<<",\"calls\":"<<c.result.calls<<",\"captured_debug\":"<<c.selected<<",\"selected_debug\":"<<c.globals.debug_switches<<",\"amount\":"<<c.output.amount<<",\"retained_strings\":"<<c.strings.size()<<"}\n";
    }else if(argc==8){Fixture f;f.arguments.amount=std::uint32_t(std::stoull(argv[1]));f.arguments.element=std::stoi(argv[2]);
        auto raw=std::uint32_t(std::stoull(argv[3]));std::memcpy(&f.first[19],&raw,4);raw=std::uint32_t(std::stoull(argv[4]));std::memcpy(&f.second[19],&raw,4);
        f.random.seed=std::uint32_t(std::stoull(argv[5]));f.random.calls=std::uint32_t(std::stoull(argv[6]));if(std::stoul(argv[7]))f.arguments.defender=A;f.run();print(f);
    }else{require(argc==3,"pass configuration and new output directory");host(fs::absolute(argv[1]),fs::absolute(argv[2]));}return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
