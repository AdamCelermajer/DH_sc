#include "../character_mana_services_v1.hpp"
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

namespace k=dh2::character_mana_services_v1;
namespace d=dh2::debug_switches;
namespace p=dh2::data;
constexpr std::uintptr_t C=0x10014000, APP=0x99f72c, ONLINE=0x10033000, DEBUG=0x9a1d18;

struct ServiceCall {unsigned op;std::uintptr_t subject;std::string text;std::uint32_t word;};
struct Fixture {
    p::PropertyRules rules{};p::PropertyState properties{};p::PropertyView view{};
    std::uint8_t mana_exempt=0;
    k::State state{C,&mana_exempt,&view};
    std::uintptr_t app=APP;
    d::Runtime debug_runtime{DEBUG},alternate_debug_runtime{DEBUG+0x100};
    d::FileSystem debug_filesystem{0x10055000};
    d::Engine debug_engine{0x10054000,&debug_filesystem};
    d::Application debug_application{0x10053000,&debug_engine};
    d::Globals debug_globals{1,&debug_runtime,&debug_application};
    d::Services debug_services{this,debug_invoke};
    k::Globals globals{&app,&debug_globals,&debug_services};
    k::Services services{this,invoke};
    std::uint8_t online_first=0,online_second=0;bool remote=false,saved=false;
    unsigned online_calls=0,service_calls=0,debug_saves=0,fail_service=0,throw_service=0;
    bool clear_debug_on_second_online=false,rebind_debug_on_open=false;
    std::vector<ServiceCall> calls;
    Fixture(std::int32_t mp=100,bool god=false,bool trace=false){
        rules.defaults.fill(0);rules.types.fill(16);rules.types[41]=32;
        properties.base[41]=mp;properties.saved[41]=0;properties.resolved[41]=mp;
        view=p::property_view(rules,properties);
        assert(debug_runtime.set_switch("GOD_MANA",god,debug_globals,debug_services)==d::Status::complete);
        assert(debug_runtime.set_switch("isTracingChar_Stats",trace,debug_globals,debug_services)==d::Status::complete);
        debug_saves=0;
    }
    static std::int32_t debug_invoke(void* context,const d::Request* request,d::File* reply){
        auto& f=*static_cast<Fixture*>(context);*reply={};
        if(request->operation==d::Operation::save)++f.debug_saves;
        if(request->operation==d::Operation::open_read&&f.rebind_debug_on_open)
            f.debug_globals.singleton=&f.alternate_debug_runtime;
        return 0;
    }
    static std::int32_t invoke(void* context,const k::Request* request,k::Reply* reply){
        auto& f=*static_cast<Fixture*>(context);*reply={};++f.service_calls;
        const auto op=static_cast<unsigned>(request->operation);
        if(op==unsigned(k::Operation::get_online)){
            const auto index=f.online_calls++;
            reply->identity=ONLINE+index*0x100;
            reply->word=index==0?f.online_first:f.online_second;
            if(f.clear_debug_on_second_online&&index==1)f.debug_globals.singleton=nullptr;
        }else if(op==unsigned(k::Operation::is_remotely_updated))reply->word=f.remote?1u:0u;
        else {assert(request->subject==f.app&&request->text&&std::string(request->text)=="GOD_MANA");reply->word=f.saved?1u:0u;}
        f.calls.push_back({op,request->subject,request->text?request->text:"",reply->word});
        if(f.throw_service==f.service_calls)throw std::runtime_error("fixture provider exception");
        if(f.fail_service==f.service_calls)return 1;
        return 0;
    }
    k::Status has(std::int32_t amount,bool& value,k::Result& details){return k::has_mana(&state,amount,&services,&value,&details);}
    k::Status use(std::int32_t amount,bool& value,k::Result& details){return k::use_mana(&state,&globals,amount,&services,&value,&details);}
};

void emit(const Fixture& f,k::Status status,bool value,const k::Result& r){
    std::cout<<"{\"status\":"<<static_cast<int>(status)<<",\"decision\":"<<(value?1:0)
      <<",\"mp\":"<<f.properties.resolved[41]<<",\"saved_mp\":"<<f.properties.saved[41]
      <<",\"service_calls\":"<<r.service_calls<<",\"online_queries\":"<<r.online_queries
      <<",\"remote_queries\":"<<r.remote_queries<<",\"application_is_saved_option_on_queries\":"<<r.application_is_saved_option_on_queries
      <<",\"debug_loads\":"<<r.debug_loads<<",\"debug_queries\":"<<r.debug_queries
      <<",\"has_calls\":"<<r.has_mana_calls<<",\"mana_before\":"<<r.mana_before
      <<",\"mana_exempt_14f0_byte\":"<<unsigned(r.mana_exempt_14f0_byte)<<",\"mana_added\":"<<unsigned(r.mana_added)
      <<",\"last_online\":"<<r.last_online_identity<<",\"last_online_byte\":"<<r.last_online_byte
      <<",\"calls\":[";
    bool first=true;for(const auto& c:f.calls){if(!first)std::cout<<',';first=false;std::cout<<'['<<c.op<<','<<c.subject<<",\""<<c.text<<"\","<<c.word<<']';}
    std::cout<<"]}\n";
}

void host_cases(){
    unsigned cases=0;
    {Fixture f(80);bool v=false;k::Result r{};assert(f.has(50,v,r)==k::Status::complete&&v&&r.mana_before==80&&r.online_queries==1&&r.remote_queries==0);++cases;}
    {Fixture f(49);bool v=true;k::Result r{};assert(f.has(50,v,r)==k::Status::complete&&!v&&r.mana_before==49);++cases;}
    {Fixture f(0);f.online_first=2;f.remote=true;bool v=false;k::Result r{};assert(f.has(99,v,r)==k::Status::complete&&v&&r.online_queries==1&&r.remote_queries==1&&r.mana_before==0);++cases;}
    {Fixture f(100);f.online_first=1;f.remote=false;bool v=false;k::Result r{};assert(f.has(100,v,r)==k::Status::complete&&v&&r.remote_queries==1&&r.last_remote_word==0);++cases;}
    {Fixture f(12);f.online_first=1;f.remote=true;bool v=false;k::Result r{};assert(f.use(900,v,r)==k::Status::complete&&v&&f.properties.resolved[41]==12&&f.calls.size()==2&&!r.application_is_saved_option_on_queries&&!r.debug_queries);++cases;}
    {Fixture f(12);f.saved=true;bool v=false;k::Result r{};assert(f.use(900,v,r)==k::Status::complete&&v&&f.properties.resolved[41]==12&&r.application_is_saved_option_on_queries==1&&!r.debug_queries);++cases;}
    {Fixture f(12,true);bool v=false;k::Result r{};assert(f.use(900,v,r)==k::Status::complete&&v&&f.properties.resolved[41]==12&&r.debug_loads==1&&r.debug_queries==1&&r.online_queries==1);++cases;}
    {Fixture f(12);f.mana_exempt=5;bool v=false;k::Result r{};assert(f.use(900,v,r)==k::Status::complete&&v&&f.properties.resolved[41]==12&&r.mana_exempt_14f0_byte==5&&r.has_mana_calls==0);++cases;}
    {Fixture f(12,false,true);bool v=true;k::Result r{};assert(f.use(13,v,r)==k::Status::complete&&!v&&f.properties.resolved[41]==12&&r.has_mana_calls==1&&r.debug_loads==1&&r.debug_queries==1);++cases;}
    {Fixture f(100,false,true);bool v=false;k::Result r{};assert(f.use(30,v,r)==k::Status::complete&&v&&f.properties.resolved[41]==70&&f.properties.saved[41]==-30&&r.mana_added&&r.online_queries==2&&r.has_mana_calls==1&&r.debug_loads==2&&r.debug_queries==2);assert(f.calls.size()==3&&f.calls[0].op==0&&f.calls[1].op==2&&f.calls[2].op==0);++cases;}
    {Fixture f(100);f.debug_globals.singleton=&f.alternate_debug_runtime;assert(f.alternate_debug_runtime.set_switch("GOD_MANA",1,f.debug_globals,f.debug_services)==d::Status::complete);assert(f.alternate_debug_runtime.set_switch("isTracingChar_Stats",1,f.debug_globals,f.debug_services)==d::Status::complete);f.debug_globals.singleton=&f.debug_runtime;f.debug_globals.loaded=0;f.rebind_debug_on_open=true;bool v=false;k::Result r{};assert(f.use(30,v,r)==k::Status::complete&&v&&f.properties.resolved[41]==70&&f.debug_globals.singleton==&f.alternate_debug_runtime&&r.debug_loads==2&&r.debug_queries==2);assert(f.debug_runtime.switches().at("GOD_MANA")==0&&f.alternate_debug_runtime.switches().at("GOD_MANA")==1);++cases;}
    {Fixture f(100);f.online_first=2;f.online_second=0;f.remote=false;bool v=false;k::Result r{};assert(f.use(30,v,r)==k::Status::complete&&v&&f.properties.resolved[41]==70&&r.online_queries==2&&r.remote_queries==1&&r.last_online_identity==ONLINE+0x100);++cases;}
    {Fixture f(100);f.online_first=0;f.online_second=1;f.remote=false;bool v=false;k::Result r{};assert(f.use(30,v,r)==k::Status::complete&&v&&f.properties.resolved[41]==70&&r.remote_queries==1&&r.mana_added);++cases;}
    {Fixture f(0);bool v=false;k::Result r{};assert(f.use(0,v,r)==k::Status::complete&&v&&r.mana_added&&f.properties.resolved[41]==0);++cases;}
    {Fixture f(100);f.clear_debug_on_second_online=true;bool v=false;k::Result r{};assert(f.use(20,v,r)==k::Status::complete&&v&&f.properties.resolved[41]==80&&r.mana_added&&r.debug_loads==2&&r.debug_queries==2);++cases;}
    for(unsigned fail=1;fail<=3;++fail){Fixture f(100);bool v=true;k::Result r{};f.fail_service=fail;const auto s=f.use(20,v,r);assert(s==k::Status::service_failed&&v&&f.properties.resolved[41]==100&&f.calls.size()==fail);++cases;}
    for(unsigned throwing=0;throwing<2;++throwing){Fixture f(100);bool v=true;k::Result r{};if(throwing)f.throw_service=2;else f.fail_service=2;assert(f.use(20,v,r)==k::Status::service_failed&&v&&f.properties.resolved[41]==100);++cases;}
    {Fixture f(100);bool v=true;k::Result r{};assert(k::use_mana(&f.state,nullptr,10,&f.services,&v,&r)==k::Status::service_unavailable&&v&&f.properties.resolved[41]==100);++cases;}
    {Fixture f(100);bool v=true;k::Result r{};assert(f.use(-1,v,r)==k::Status::unsupported_assertion_domain&&v&&f.calls.empty());++cases;}
    {Fixture f(100);bool v=true;k::Result r{};f.services.invoke=nullptr;assert(f.has(10,v,r)==k::Status::service_unavailable&&v&&f.calls.empty());++cases;}
    {Fixture f(100);k::CallbackContext c{&f.state,nullptr,&f.services};dh2_script_value in{};in.type=DH2_SCRIPT_STRING;dh2_script_value out{};std::uint32_t n=4;char e[128]{};assert(k::has_mana_callback(&c,&in,1,&out,1,&n,e,sizeof(e))==0&&n==0&&f.calls.empty());++cases;}
    {Fixture f(100);k::CallbackContext c{&f.state,&f.globals,&f.services};dh2_script_value out{};std::uint32_t n=4;char e[128]{};assert(k::use_mana_callback(&c,nullptr,0,&out,1,&n,e,sizeof(e))==0&&n==0&&f.calls.empty());++cases;}
    {Fixture f(100);k::CallbackContext c{&f.state,&f.globals,&f.services};dh2_script_value in[2]{};in[0].type=DH2_SCRIPT_NUMBER;in[0].number=1.0f;in[1].type=DH2_SCRIPT_STRING;dh2_script_value out{};std::uint32_t n=0;char e[128]{};assert(k::use_mana_callback(&c,in,2,&out,1,&n,e,sizeof(e))==0&&n==1&&out.type==DH2_SCRIPT_BOOLEAN&&out.boolean==1&&f.properties.resolved[41]==99);++cases;}
    {Fixture f(100);k::CallbackContext c{&f.state,&f.globals,&f.services};dh2_script_value in[2]{};in[0].type=DH2_SCRIPT_STRING;in[1].type=DH2_SCRIPT_NUMBER;in[1].number=1.0f;dh2_script_value out{};std::uint32_t n=4;char e[128]{};assert(k::use_mana_callback(&c,in,2,&out,1,&n,e,sizeof(e))==0&&n==0&&f.calls.empty());++cases;}
    {Fixture f(100);k::CallbackContext c{&f.state,&f.globals,&f.services};dh2_script_value in{};in.type=DH2_SCRIPT_NUMBER;in.number=1.9f;dh2_script_value out{};std::uint32_t n=0;char e[128]{};assert(k::use_mana_callback(&c,&in,1,&out,1,&n,e,sizeof(e))==0&&n==1&&out.type==DH2_SCRIPT_BOOLEAN&&out.boolean==1&&f.properties.resolved[41]==99);++cases;}
    {Fixture f(100);k::CallbackContext c{&f.state,&f.globals,&f.services};dh2_script_value in{};in.type=DH2_SCRIPT_NUMBER;in.number=-0.9f;dh2_script_value out{};std::uint32_t n=0;char e[128]{};assert(k::use_mana_callback(&c,&in,1,&out,1,&n,e,sizeof(e))==0&&n==1&&out.type==DH2_SCRIPT_BOOLEAN&&out.boolean==1&&f.properties.resolved[41]==100);++cases;}
    {Fixture f(100);k::CallbackContext c{&f.state,&f.globals,&f.services};dh2_script_value in{};in.type=DH2_SCRIPT_NUMBER;in.number=-0.0f;dh2_script_value out{};std::uint32_t n=0;char e[128]{};assert(k::has_mana_callback(&c,&in,1,&out,1,&n,e,sizeof(e))==0&&n==1&&out.type==DH2_SCRIPT_BOOLEAN&&out.boolean==1&&f.properties.resolved[41]==100);++cases;}
    {Fixture f(100);k::CallbackContext c{&f.state,&f.globals,&f.services};dh2_script_value in{};in.type=DH2_SCRIPT_NUMBER;in.number=-1.0f;dh2_script_value out{};std::uint32_t n=4;char e[128]{};assert(k::use_mana_callback(&c,&in,1,&out,1,&n,e,sizeof(e))==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE&&n==0&&f.calls.empty()&&f.properties.resolved[41]==100);++cases;}
    {Fixture f(100);k::CallbackContext c{&f.state,&f.globals,&f.services};dh2_script_value in{};in.type=DH2_SCRIPT_NUMBER;in.number=std::numeric_limits<float>::quiet_NaN();dh2_script_value out{};out.type=DH2_SCRIPT_STRING;std::uint32_t n=4;char e[128]{};assert(k::use_mana_callback(&c,&in,1,&out,1,&n,e,sizeof(e))==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE&&n==0&&out.type==DH2_SCRIPT_STRING&&f.calls.empty());++cases;}
    {Fixture f(100);bool v=true;k::Result r{};alignas(k::State) unsigned char bytes[sizeof(k::State)+1]{};auto* bad=reinterpret_cast<const k::State*>(bytes+1);assert(k::has_mana(bad,1,&f.services,&v,&r)==k::Status::invalid_argument&&v&&f.calls.empty());++cases;}
    {Fixture f(100);bool v=true;k::Result r{};struct Nested {Fixture* f; k::State* s; bool attempted=false; k::Status status=k::Status::complete;} n{&f,&f.state};k::Services svc{&n,[](void* ctx,const k::Request* q,k::Reply* o)->int{auto& n=*static_cast<Nested*>(ctx);if(!n.attempted){n.attempted=true;bool v=false;k::Result r{};n.status=k::has_mana(n.s,1,&n.f->services,&v,&r);}return Fixture::invoke(n.f,q,o);}};assert(k::has_mana(&f.state,1,&svc,&v,&r)==k::Status::complete&&n.status==k::Status::reentrant_owner);++cases;}
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<",\"source_property_add\":true,\"debug_runtime_reuse\":true}\n";
}

int main(int argc,char** argv){
    if(argc==11){Fixture f(static_cast<std::int32_t>(std::stol(argv[9])),std::stoul(argv[6])!=0,std::stoul(argv[7])!=0);const auto mode=std::stoul(argv[1]);f.online_first=std::uint8_t(std::stoul(argv[2]));f.online_second=std::uint8_t(std::stoul(argv[3]));f.remote=std::stoul(argv[4])!=0;f.saved=std::stoul(argv[5])!=0;f.mana_exempt=std::uint8_t(std::stoul(argv[8]));bool value=false;k::Result details{};k::Status status=k::Status::complete;
        if(mode==6){f.debug_globals.singleton=&f.alternate_debug_runtime;assert(f.alternate_debug_runtime.set_switch("GOD_MANA",1,f.debug_globals,f.debug_services)==d::Status::complete);assert(f.alternate_debug_runtime.set_switch("isTracingChar_Stats",1,f.debug_globals,f.debug_services)==d::Status::complete);f.debug_globals.singleton=&f.debug_runtime;f.debug_globals.loaded=0;f.rebind_debug_on_open=true;}
        if(mode<2||mode==6){const auto amount=static_cast<std::int32_t>(std::stol(argv[10]));status=mode==0?k::has_mana(&f.state,amount,&f.services,&value,&details):k::use_mana(&f.state,&f.globals,amount,&f.services,&value,&details);}
        else {k::CallbackContext context{&f.state,&f.globals,&f.services};dh2_script_value input[2]{};input[0].type=mode==5?DH2_SCRIPT_STRING:DH2_SCRIPT_NUMBER;input[0].number=std::stof(argv[10]);input[1].type=DH2_SCRIPT_STRING;dh2_script_value output{};std::uint32_t n=0;char error[128]{};const bool use=mode==3||mode==5||mode==7;const std::uint32_t count=mode==4?0u:(mode==7||mode==8?2u:1u);const auto callback_status=use?k::use_mana_callback(&context,count?input:nullptr,count,&output,1,&n,error,sizeof(error)):k::has_mana_callback(&context,count?input:nullptr,count,&output,1,&n,error,sizeof(error));if(callback_status!=0)status=k::Status::service_failed;else if(n==1)value=output.type==DH2_SCRIPT_BOOLEAN&&output.boolean!=0;}
        emit(f,status,value,details);return 0;}
    host_cases();return 0;
}
