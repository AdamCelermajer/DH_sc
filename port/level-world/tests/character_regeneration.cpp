#include "../character_regeneration.hpp"
#include "../../game-data/vitals.hpp"
#include <array>
#include <cassert>
#include <cstring>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

namespace k=dh2::character_regeneration;
constexpr std::uintptr_t C=0x10014000,P=C+0x560,S=C+0xff4,D=0x9a1d18,D2=0x10030000,T=0x10025000;
struct Call {unsigned op;std::uintptr_t subject,sheet;std::uint32_t property,amount,word;std::uintptr_t identity;};
struct Fixture {
    k::State state{C,P,S};k::Globals globals{D};k::Services services{this,invoke};k::Result result{};
    std::uint32_t current=10,maximum=100,query=0,mutation=0,reads=0;unsigned fail=0;bool throws=false,null_string=false,live_string=false;
    std::vector<Call> calls;Fixture* nested=nullptr;bool entered=false;dh2::data::PropertyView* property_view=nullptr;
    static std::int32_t invoke(void* context,const k::Request* request,k::Reply* reply) {
        auto& f=*static_cast<Fixture*>(context);const auto op=unsigned(request->operation);reply->word=0;reply->identity=0;
        if(op==0)reply->word=(f.reads++==0?f.current:f.maximum);
        else if(op==2){assert(request->text && std::string(request->text)=="isTracingChar_Stats");reply->identity=f.null_string?0:T;f.live_string=!f.null_string;}
        else if(op==3){assert(f.live_string && request->sheet==T);reply->word=f.query;}
        else if(op==4){assert(f.live_string && request->subject==T);f.live_string=false;}
        f.calls.push_back({op,request->subject,request->sheet,request->property,request->amount,reply->word,reply->identity});
        if(f.mutation==1 && op==0 && f.reads==1)f.current=800;
        if(f.mutation==2 && op==0 && f.reads==2){f.current=1000;f.maximum=2000;}
        if(f.mutation==3 && op==1){f.globals.debug_switches=D2;f.current=4000;f.maximum=1;}
        if(f.mutation==4 && op==2){f.globals.debug_switches=D2;f.current=20;}
        if(f.mutation==5 && op==3){f.current=3000;f.maximum=5;}
        if(f.mutation==6 && op==4){f.state.properties=0x10031000;f.state.resolved_sheet=0x10032000;f.current=1234;}
        if(f.mutation==7 && op==0 && f.reads==1)f.globals.debug_switches=D2;
        if(f.mutation==8 && op==0 && f.reads==2){f.state.properties=0x10031000;f.state.resolved_sheet=0x10032000;}
        if(f.mutation==9 && op==0)f.services.invoke=nullptr;
        if(f.mutation==10 && op==5)f.current=77;
        if(f.nested && !f.entered && op==1){f.entered=true;assert(f.nested->run(0,UINT32_MAX)==k::Status::complete);}
        if(f.fail==f.calls.size()){if(f.throws)throw std::runtime_error("provider error");return 1;}
        if(op==5){
            if(f.property_view){if(dh2_property_add(f.property_view,std::int32_t(request->property),signed_word(request->amount)))return 1;f.current=std::uint32_t(f.property_view->resolved[request->property]);}
            else f.current+=request->amount;
        }
        return 0;
    }
    static std::int32_t signed_word(std::uint32_t n){std::int32_t x;std::memcpy(&x,&n,4);return x;}
    k::Status run(unsigned mana,std::uint32_t amount){return mana?k::regen_mp(&state,&globals,amount,&services,&result):k::regen_hp(&state,&globals,amount,&services,&result);}
};
void emit(const Fixture& f,k::Status status) {
    std::cout<<"{\"status\":"<<unsigned(status)<<",\"current\":"<<f.result.current<<",\"maximum\":"<<f.result.maximum
     <<",\"positive_amount\":"<<f.result.positive_amount<<",\"added\":"<<f.result.added<<",\"side_current\":"<<f.current
     <<",\"side_maximum\":"<<f.maximum<<",\"debug_selection\":"<<f.globals.debug_switches<<",\"live_string\":"<<(f.live_string?1:0)<<",\"calls\":[";
    bool first=true;for(const auto& c:f.calls){if(!first)std::cout<<',';first=false;std::cout<<'['<<c.op<<','<<c.subject<<','<<c.sheet<<','<<c.property<<','<<c.amount<<','<<c.word<<','<<c.identity<<']';}std::cout<<"]}\n";
}
int main(int argc,char** argv) {
    if(argc>1){if(argc!=7)return 2;Fixture f;const auto mana=std::stoul(argv[1]);f.current=std::stoul(argv[2]);f.maximum=std::stoul(argv[3]);const auto amount=std::uint32_t(std::stoul(argv[4]));f.query=std::stoul(argv[5]);f.mutation=std::stoul(argv[6]);emit(f,f.run(unsigned(mana),amount));return 0;}
    unsigned cases=0;
    for(unsigned mana:{0u,1u}) {
        {Fixture f;assert(f.run(mana,UINT32_MAX)==k::Status::complete && f.result.positive_amount==90 && f.current==100 && f.calls.size()==7 && !f.live_string);++cases;}
        {Fixture f;f.current=100;assert(f.run(mana,UINT32_MAX)==k::Status::complete && !f.result.positive_amount && f.calls.size()==2);++cases;}
        {Fixture f;f.current=UINT32_MAX;f.maximum=12160;assert(f.run(mana,UINT32_MAX)==k::Status::complete && f.current==12159 && f.result.positive_amount==12160);++cases;}
        for(unsigned mutation=1;mutation<=9;++mutation){Fixture f;f.mutation=mutation;assert(f.run(mana,UINT32_MAX)==k::Status::complete && f.result.positive_amount==90 && f.result.added && f.calls.size()==7);assert(f.calls[2].subject==(mutation==7?D2:D) && f.calls[4].subject==f.calls[2].subject && f.calls.back().subject==P);++cases;}
        for(std::uint32_t query:{0u,1u,UINT32_MAX}){Fixture f;f.query=query;assert(f.run(mana,UINT32_MAX)==k::Status::complete && f.current==100);++cases;}
        for(unsigned phase=1;phase<=7;++phase)for(bool throws:{false,true}){Fixture f;f.fail=phase;f.throws=throws;assert(f.run(mana,UINT32_MAX)==k::Status::service_failed && f.calls.size()==phase && !f.result.added && f.current==10);assert(f.live_string==(phase==4 || phase==5));++cases;}
        for(bool throws:{false,true})for(unsigned effect=0;effect<3;++effect){Fixture f;f.throws=throws;f.mutation=effect==0?4u:(effect==1?5u:10u);f.fail=effect==0?4u:(effect==1?5u:7u);assert(f.run(mana,UINT32_MAX)==k::Status::service_failed && !f.result.added && f.result.positive_amount==90 && f.calls.size()==f.fail && f.current==(effect==0?20u:(effect==1?3000u:77u)));++cases;}
        {Fixture f;f.null_string=true;assert(f.run(mana,UINT32_MAX)==k::Status::service_failed && f.calls.size()==4 && f.current==10);++cases;}
        {Fixture f;f.globals.debug_switches=0;assert(f.run(mana,UINT32_MAX)==k::Status::invalid_argument && f.calls.size()==2 && f.result.positive_amount==90);++cases;}
        {Fixture f;f.current=100;f.globals.debug_switches=0;assert(f.run(mana,UINT32_MAX)==k::Status::complete && f.calls.size()==2);++cases;}
    }
    {Fixture f;f.services.invoke=nullptr;assert(f.run(0,0)==k::Status::service_unavailable && f.result.calls==0);++cases;}
    {Fixture outer,inner;inner.state={C+0x40000,P+0x40000,S+0x40000};outer.nested=&inner;assert(outer.run(0,UINT32_MAX)==k::Status::complete && outer.current==100 && inner.current==100);++cases;}
    for(unsigned missing=0;missing<3;++missing){Fixture f;if(missing==0)f.state.character=0;else if(missing==1)f.state.properties=0;else f.state.resolved_sheet=0;f.result.added=7;assert(f.run(0,0)==k::Status::invalid_argument && f.calls.empty() && f.result.added==7);++cases;}
    for(unsigned missing=0;missing<4;++missing){Fixture f;f.result.added=7;assert(k::regen_hp(missing==0?nullptr:&f.state,missing==1?nullptr:&f.globals,0,missing==2?nullptr:&f.services,missing==3?nullptr:&f.result)==k::Status::invalid_argument && f.result.added==7 && f.calls.empty());++cases;}
    for(unsigned control=0;control<4;++control){Fixture f;alignas(k::State) std::array<unsigned char,128> unaligned{};const auto p=unaligned.data()+1;assert(k::regen_hp(control==0?reinterpret_cast<const k::State*>(p):&f.state,control==1?reinterpret_cast<const k::Globals*>(p):&f.globals,0,control==2?reinterpret_cast<const k::Services*>(p):&f.services,control==3?reinterpret_cast<k::Result*>(p):&f.result)==k::Status::invalid_argument && f.calls.empty());++cases;}
    for(unsigned i=0;i<4;++i)for(unsigned j=0;j<i;++j){Fixture f;alignas(k::State) std::array<unsigned char,512> storage{};const void* p[4]={storage.data(),storage.data()+128,storage.data()+256,storage.data()+384};p[i]=p[j];assert(k::regen_hp(static_cast<const k::State*>(p[0]),static_cast<const k::Globals*>(p[1]),0,static_cast<const k::Services*>(p[2]),static_cast<k::Result*>(const_cast<void*>(p[3])))==k::Status::invalid_argument && f.calls.empty());++cases;}
    // Reuse the maintained live property adder and independently verified vitals
    // kernel on real owner sheets. No new direct stat writes implement the add.
    std::mt19937 rng(0x3bdca4);unsigned reuse_cases=0;
    for(unsigned i=0;i<512;++i) {
        dh2::data::PropertyRules rules;rules.defaults.fill(-1);rules.types.fill(16);rules.types[36]=rules.types[41]=32;
        dh2::data::PropertyState left,right;dh2::data::reset_properties(rules,left);left.saved[36]=left.resolved[36]=Fixture::signed_word(rng());left.saved[41]=left.resolved[41]=Fixture::signed_word(rng());left.base[38]=left.resolved[38]=Fixture::signed_word(rng());left.base[43]=left.resolved[43]=Fixture::signed_word(rng());right=left;
        const unsigned mana=i%2,current_id=mana?41:36,maximum_id=mana?43:38;const auto amount=rng();auto lv=dh2::data::property_view(rules,left),rv=dh2::data::property_view(rules,right);Fixture f;f.current=std::uint32_t(left.resolved[current_id]);f.maximum=std::uint32_t(left.resolved[maximum_id]);f.property_view=&lv;dh2::data::VitalsChange expected;
        assert(f.run(mana,amount)==k::Status::complete && dh2_vitals_regen(&rv,mana,Fixture::signed_word(amount),&expected)==0);
        assert(left.base==right.base && left.saved==right.saved && left.gear==right.gear && left.resolved==right.resolved && f.result.positive_amount==std::uint32_t(expected.raw_add));++reuse_cases;
    }
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<",\"live_property_reuse_cases\":"<<reuse_cases<<",\"mismatches\":0}\n";
}
