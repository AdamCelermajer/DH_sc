#include "../character_script_set_level.hpp"
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>
namespace k=dh2::character_script_set_level;
constexpr std::uintptr_t C=0x10014000,ARGS=0x10018000,A1=0x10030000,A2=0x10031000,M1=0x10032000,M2=0x10033000,M3=0x10034000;
std::uint32_t bits(float value){std::uint32_t out;std::memcpy(&out,&value,4);return out;}
struct Call {unsigned op;std::uintptr_t subject;std::uint32_t argument,number,output,base_level;};
struct Fixture {
    k::Character character{C,C+0x560,0x12345678};k::Arguments arguments{ARGS,1,3};
    k::Application app1{A1,M1},app2{A2,M3};k::Globals globals{&app1};k::Services services{this,invoke};k::Result result{};
    std::uint32_t number1=bits(2560),number2=bits(2560),max1=100,max2=100,mutation=0,fail=0,throws=0;
    unsigned number_calls=0,design_calls=0;bool nested=false;std::vector<Call> calls;
    void mutate(unsigned op) {
        if(mutation==1 && op==0 && number_calls==1)globals.application=&app2;
        if(mutation==2 && op==1 && design_calls==1)globals.application=&app2;
        if(mutation==3 && op==1 && design_calls==1)app1.design_manager=M2;
        if(mutation==4 && op==1 && design_calls==1)number2=bits(-257.75f);
        if(mutation==5 && op==3)character.base_level_5b8=0xaabbccdd;
        if(mutation==6 && op==4)character.base_level_5b8=0xdeadbeef;
        if(mutation==7 && calls.size()==1)services.invoke=nullptr;
        if(nested && calls.size()==1){Fixture child;child.character.identity=C+0x1000;
            child.character.properties_identity=C+0x1560;child.arguments.identity=ARGS+0x1000;
            assert(child.run()==k::Status::complete);}
    }
    static int invoke(void* raw,k::Character* owner,const k::Request* request,std::uint32_t* word) {
        auto& f=*static_cast<Fixture*>(raw);assert(owner==&f.character);const auto op=unsigned(request->operation);std::uint32_t value=0;
        if(op==0){assert(request->subject==f.arguments.identity && request->argument==0);value=++f.number_calls==1?f.number1:f.number2;}
        if(op==1){assert(request->subject==M1 || request->subject==M2 || request->subject==M3);
            assert(std::strcmp(request->category,"CharacterDesign")==0 && std::strcmp(request->key,"MaxLevelDVeryHard")==0);
            value=++f.design_calls==1?f.max1:f.max2;}
        if(op==2){assert(!request->subject && !request->argument);float number;std::memcpy(&number,&request->number_bits,4);
            if(!std::isfinite(number) || number< -2147483648.0f || number>=2147483648.0f)return 29;
            const auto signed_value=std::int32_t(number);std::memcpy(&value,&signed_value,4);}
        if(op==3)assert(request->subject==f.character.properties_identity && request->argument==1);
        if(op==4 || op==5)assert(request->subject==f.character.identity && request->argument==UINT32_MAX);
        f.calls.push_back({op,request->subject,request->argument,request->number_bits,value,owner->base_level_5b8});
        f.mutate(op);*word=value;
        if(f.fail==f.calls.size())return 19;
        if(f.throws==f.calls.size())throw std::runtime_error("level provider");
        return 0;
    }
    k::Status run(){return k::set_level(&character,&arguments,&globals,&services,&result);}
};
void dump(const Fixture& f,k::Status status) {
    std::cout<<"{\"status\":"<<unsigned(status)<<",\"base_level\":"<<f.character.base_level_5b8<<",\"applied\":"<<f.result.applied
        <<",\"clamped\":"<<f.result.clamped<<",\"calls\":[";bool first=true;
    for(const auto& c:f.calls){if(!first)std::cout<<',';first=false;std::cout<<'['<<c.op<<','<<c.subject<<','<<c.argument<<','<<c.number<<','<<c.output<<','<<c.base_level<<']';}
    std::cout<<"]}\n";
}
int main(int argc,char** argv) {
    if(argc>1){assert(argc==9);Fixture f;std::uint32_t input[8];for(unsigned i=0;i<8;++i)input[i]=std::strtoul(argv[i+1],nullptr,0);
        f.arguments.count=input[0];f.arguments.first_type=input[1];f.number1=input[2];f.number2=input[3];f.max1=input[4];f.max2=input[5];f.mutation=input[6];f.character.base_level_5b8=input[7];
        const auto status=f.run();dump(f,status);return 0;}
    unsigned cases=0;
    {Fixture f;assert(f.run()==k::Status::complete && f.character.base_level_5b8==2560 && f.calls.size()==8 && f.number_calls==2 && f.design_calls==1);++cases;}
    {Fixture f;f.number1=bits(25601);f.max2=7;assert(f.run()==k::Status::complete && f.result.clamped && f.character.base_level_5b8==1792 && f.number_calls==1 && f.design_calls==2 && f.calls.size()==7);++cases;}
    {Fixture f;f.number1=bits(25600);f.number2=bits(99999);assert(f.run()==k::Status::complete && !f.result.clamped && f.character.base_level_5b8==99999);++cases;}
    for(float number:{0.f,-0.f,-1.f,-256.75f,-2147483648.f,2147483520.f}){Fixture f;f.number1=f.number2=bits(number);f.max1=0x007fffff;
        assert(f.run()==k::Status::complete && f.result.applied);++cases;}
    for(unsigned maximum:{0u,1u,100u,0xffffffffu,0x00800000u,0x01000000u,0x80000000u,0x7fffffffu}) {
        Fixture f;f.max1=maximum;f.max2=maximum^0x03000001;assert(f.run()==k::Status::complete && f.result.applied);++cases;}
    for(unsigned mutation=1;mutation<=7;++mutation)for(bool upper:{false,true}) {
        Fixture f;f.mutation=mutation;f.number1=bits(upper?25601:2560);assert(f.run()==k::Status::complete);
        if(mutation==1)assert(f.calls[1].subject==M3);
        if(mutation==2 && upper)assert(f.calls[3].subject==M1);
        if(mutation==3 && upper)assert(f.calls[3].subject==M2);
        if(mutation==4 && !upper)assert(f.character.base_level_5b8==std::uint32_t(-257));
        if(mutation==5)assert(f.character.base_level_5b8==0xaabbccdd);
        if(mutation==6)assert(f.character.base_level_5b8==0xdeadbeef);
        ++cases;
    }
    for(bool upper:{false,true})for(unsigned phase=1;phase<=(upper?7u:8u);++phase)for(bool throwing:{false,true}) {
        Fixture f;f.number1=bits(upper?25601:2560);if(throwing)f.throws=phase;else f.fail=phase;f.mutation=5;
        assert(f.run()==k::Status::service_failed && f.calls.size()==phase && f.result.calls==phase);
        const unsigned store_phase=upper?5:6;
        assert(f.result.applied==unsigned(phase>=store_phase));
        assert(f.character.base_level_5b8==(phase>=store_phase?0xaabbccddu:0x12345678u));++cases;
    }
    for(unsigned type:{0u,1u,2u,4u,5u,6u,7u,0xffffffffu}){Fixture f;f.arguments.first_type=type;f.services.invoke=nullptr;f.globals.application=nullptr;
        assert(f.run()==k::Status::complete && f.calls.empty() && f.character.base_level_5b8==0x12345678);++cases;}
    {Fixture f;f.arguments.count=0;f.globals.application=nullptr;assert(f.run()==k::Status::complete && f.calls.empty());++cases;}
    {Fixture f;f.services.invoke=nullptr;assert(f.run()==k::Status::service_unavailable && f.calls.empty());++cases;}
    for(float number:{std::numeric_limits<float>::infinity(),-std::numeric_limits<float>::infinity(),std::numeric_limits<float>::quiet_NaN(),2147483648.f}) {
        Fixture f;f.number1=bits(number);assert(f.run()==k::Status::service_failed && !f.result.applied && f.calls.size()==2 && f.result.calls==3);++cases;
    }
    {Fixture f;f.number2=bits(std::numeric_limits<float>::quiet_NaN());assert(f.run()==k::Status::service_failed && !f.result.applied && f.result.calls==5);++cases;}
    {Fixture f;f.globals.application=nullptr;assert(f.run()==k::Status::invalid_argument && f.calls.size()==1 && !f.result.applied);++cases;}
    {Fixture f;f.app1.design_manager=0;assert(f.run()==k::Status::invalid_argument && f.calls.size()==1);++cases;}
    {Fixture f;f.nested=true;assert(f.run()==k::Status::complete);++cases;}
    for(unsigned identity=0;identity<3;++identity){Fixture f;if(identity==0)f.character.identity=0;if(identity==1)f.character.properties_identity=0;if(identity==2)f.arguments.identity=0;
        assert(f.run()==k::Status::invalid_argument && f.calls.empty());++cases;}
    {Fixture f;assert(k::set_level(&f.character,&f.arguments,&f.globals,&f.services,reinterpret_cast<k::Result*>(&f.character))==k::Status::invalid_argument);++cases;}
    {Fixture f;assert(k::set_level(&f.character,&f.arguments,&f.globals,&f.services,reinterpret_cast<k::Result*>(&f.arguments))==k::Status::invalid_argument);++cases;}
    {Fixture f;assert(k::set_level(&f.character,&f.arguments,&f.globals,&f.services,reinterpret_cast<k::Result*>(&f.services))==k::Status::invalid_argument);++cases;}
    {Fixture f;assert(k::set_level(&f.character,&f.arguments,reinterpret_cast<k::Globals*>(&f.character),&f.services,&f.result)==k::Status::invalid_argument);++cases;}
    {Fixture f;f.globals.application=reinterpret_cast<k::Application*>(&f.arguments);assert(f.run()==k::Status::invalid_argument && f.calls.size()==1);++cases;}
    for(unsigned item=0;item<5;++item){Fixture f;alignas(k::Character) unsigned char bytes[sizeof(k::Services)+1]{};
        auto owner=&f.character;auto arguments=&f.arguments;auto globals=&f.globals;auto services=&f.services;auto result=&f.result;
        if(item==0)owner=reinterpret_cast<k::Character*>(bytes+1);
        if(item==1)arguments=reinterpret_cast<k::Arguments*>(bytes+1);
        if(item==2)globals=reinterpret_cast<k::Globals*>(bytes+1);
        if(item==3)services=reinterpret_cast<k::Services*>(bytes+1);
        if(item==4)result=reinterpret_cast<k::Result*>(bytes+1);
        assert(k::set_level(owner,arguments,globals,services,result)==k::Status::invalid_argument);++cases;
    }
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<",\"native_wired\":false}\n";
}
