#include "../character_interactive.hpp"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>
namespace k=dh2::character_interactive;
constexpr std::uintptr_t C=0x10014000,AI=C+0x3c8,P=0x10020000;
struct Call {unsigned query;std::uintptr_t subject,other;unsigned value;};
struct Fixture {
    k::Character character{C,AI,0x2380,0,1,1,0};
    unsigned dead=0,friend_value=0,type=4,mutation=0,fail=0,throws=0;
    std::vector<Call> calls;k::Result result{};k::Services services{this,invoke};
    void mutate(unsigned query) {
        if(mutation==1 && calls.size()==1)dead=!dead;
        if(mutation==2 && query==1)character.deleted_81=1;
        if(mutation==3 && query==2)character.enabled_8a=0;
        if(mutation==4 && query==3)dead=1;
        if(mutation==5 && query==4)dead=1;
        if(mutation==6 && query==0 && calls.size()>1)character.flags_520=0;
        if(mutation==7 && query==0 && calls.size()>1)character.interactive_415=255;
        if(mutation==8 && query==3){character.flags_520=0;character.interactive_415=17;}
        if(mutation==9 && query==1){character.enabled_8a=0;character.flags_520=0;}
        if(mutation==10 && calls.size()==1)character.enabled_8a=0;
        if(mutation==11 && query==1)type=1;
        if(mutation==12 && query==3)type=5;
        if(mutation==13 && query==4)character.flags_520=0x2000;
        if(mutation==14 && calls.size()==1)services.invoke=nullptr;
    }
    static int invoke(void* raw,k::Character* character,k::Query query,std::uintptr_t subject,
                      std::uintptr_t other,unsigned* value) {
        auto& f=*static_cast<Fixture*>(raw);assert(character==&f.character);
        const unsigned op=static_cast<unsigned>(query);
        assert(subject==(query==k::Query::is_friend?AI:C));
        assert(other==(query==k::Query::is_friend?P:0));
        unsigned output=0;
        if(query==k::Query::is_dead)output=f.dead;
        else if(query==k::Query::is_friend)output=f.friend_value;
        else if(query==k::Query::is_monster)output=f.type==4;
        else if(query==k::Query::is_faerie)output=f.type==3;
        else if(query==k::Query::is_summoned)output=f.type==5;
        f.calls.push_back({op,subject,other,output});
        if(f.fail==f.calls.size())return 9;
        if(f.throws==f.calls.size())throw std::runtime_error("provider");
        *value=output;f.mutate(op);return 0;
    }
    k::Status run(std::uintptr_t interactor=P) {return k::evaluate(&character,interactor,&services,&result);}
};
void dump(const Fixture& f,k::Status status) {
    std::cout<<"{\"status\":"<<unsigned(status)<<",\"value\":"<<f.result.value
        <<",\"flags\":"<<f.character.flags_520<<",\"deleted\":"<<unsigned(f.character.deleted_81)
        <<",\"enabled\":"<<unsigned(f.character.enabled_8a)<<",\"interactive\":"<<unsigned(f.character.interactive_415)
        <<",\"dead\":"<<f.dead<<",\"type\":"<<f.type<<",\"calls\":[";
    bool first=true;for(auto c:f.calls){if(!first)std::cout<<',';first=false;
        std::cout<<'['<<c.query<<','<<c.subject<<','<<c.other<<','<<c.value<<']';}
    std::cout<<"]}\n";
}
int main(int argc,char** argv) {
    if(argc>1) {
        assert(argc==12);unsigned x[11];for(unsigned i=0;i<11;++i)x[i]=std::strtoul(argv[i+1],nullptr,0);
        Fixture f;f.character.flags_520=x[0];f.character.deleted_81=x[1];f.character.enabled_8a=x[2];
        f.character.interactive_415=x[3];f.dead=x[4];f.friend_value=x[5];f.type=x[6];f.mutation=x[8];f.fail=x[9];f.throws=x[10];
        const auto status=f.run(x[7]?P:0);dump(f,status);return 0;
    }
    unsigned cases=0;
    {Fixture f;assert(f.run()==k::Status::complete && f.result.value==1 && f.calls.size()==4);++cases;}
    for(unsigned raw:{0u,1u,7u,255u}){Fixture f;f.character.interactive_415=raw;assert(f.run()==k::Status::complete && f.result.value==raw);++cases;}
    for(unsigned flags:{0u,0x100u,0x241u,0x2000u,0x2380u,0xffffffffu}) {
        Fixture f;f.character.flags_520=flags;assert(f.run()==k::Status::complete && f.result.value==unsigned(bool(flags&0x2000)));++cases;
    }
    for(unsigned type:{0u,1u,2u,3u,4u,5u,6u,7u,8u,9u,0xffffffffu}) {
        Fixture f;f.type=type;assert(f.run()==k::Status::complete && f.result.value==unsigned(type!=3 && type!=5));++cases;
        Fixture g;g.type=type;g.dead=1;g.friend_value=7;g.character.deleted_81=1;g.character.enabled_8a=0;g.character.flags_520=0;
        assert(g.run()==k::Status::complete && g.result.value==unsigned(type!=4) && g.calls.size()==3);++cases;
    }
    {Fixture f;f.dead=1;f.friend_value=1;f.type=1;assert(f.run(0)==k::Status::complete && !f.result.value && f.calls.size()==4);++cases;}
    {Fixture f;f.character.deleted_81=7;assert(f.run()==k::Status::complete && !f.result.value && f.calls.size()==1);++cases;}
    {Fixture f;f.character.enabled_8a=0;assert(f.run()==k::Status::complete && !f.result.value && f.calls.size()==1);++cases;}
    {Fixture f;f.character.enabled_8a=255;assert(f.run()==k::Status::complete && f.result.value==1);++cases;}
    for(unsigned mutation=1;mutation<=14;++mutation) {
        Fixture f;f.mutation=mutation;if(mutation==2 || mutation==3 || mutation==9 || mutation==11){f.dead=1;f.friend_value=1;}
        assert(f.run()==k::Status::complete);++cases;
    }
    for(unsigned index=1;index<=4;++index) {
        Fixture f;f.fail=index;f.mutation=7;assert(f.run()==k::Status::service_failed && f.calls.size()==index && !f.result.value);++cases;
        Fixture g;g.throws=index;assert(g.run()==k::Status::service_failed && g.calls.size()==index);++cases;
    }
    for(unsigned index=1;index<=3;++index) {
        Fixture f;f.dead=1;f.friend_value=1;f.fail=index;assert(f.run()==k::Status::service_failed && f.calls.size()==index);++cases;
    }
    {Fixture f;f.mutation=8;f.fail=3;assert(f.run()==k::Status::service_failed && f.character.flags_520==0 && f.character.interactive_415==17 && f.calls.size()==3);++cases;}
    {Fixture f;f.mutation=8;f.throws=3;assert(f.run()==k::Status::service_failed && f.character.flags_520==0 && f.character.interactive_415==17 && f.calls.size()==3);++cases;}
    {Fixture f;f.services.invoke=nullptr;assert(f.run()==k::Status::service_unavailable && f.calls.empty());++cases;}
    {Fixture f;f.character.identity=0;assert(f.run()==k::Status::invalid_argument && f.calls.empty());++cases;}
    {Fixture f;f.character.ai=0;assert(f.run()==k::Status::invalid_argument && f.calls.empty());++cases;}
    {Fixture f;assert(k::evaluate(&f.character,P,&f.services,reinterpret_cast<k::Result*>(&f.character))==k::Status::invalid_argument && f.calls.empty());++cases;}
    {Fixture f;assert(k::evaluate(&f.character,P,&f.services,reinterpret_cast<k::Result*>(&f.services))==k::Status::invalid_argument && f.calls.empty());++cases;}
    {Fixture f;assert(k::evaluate(&f.character,P,reinterpret_cast<k::Services*>(&f.character),&f.result)==k::Status::invalid_argument && f.calls.empty());++cases;}
    {Fixture f;alignas(k::Character) unsigned char bytes[sizeof(k::Character)+1]{};
        assert(k::evaluate(reinterpret_cast<k::Character*>(bytes+1),P,&f.services,&f.result)==k::Status::invalid_argument);++cases;}
    {Fixture f;alignas(k::Result) unsigned char bytes[sizeof(k::Result)+1]{};
        assert(k::evaluate(&f.character,P,&f.services,reinterpret_cast<k::Result*>(bytes+1))==k::Status::invalid_argument);++cases;}
    {Fixture f;f.services.invoke=[](void* raw,k::Character* c,k::Query q,std::uintptr_t subject,std::uintptr_t other,unsigned* out)->int {
        auto& outer=*static_cast<Fixture*>(raw);
        if(outer.calls.empty()){Fixture nested;assert(nested.run()==k::Status::complete && nested.result.value==1);}
        return Fixture::invoke(raw,c,q,subject,other,out);
      };assert(f.run()==k::Status::complete && f.result.value==1);++cases;
    }
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<",\"dead_friend_exception\":true,\"fresh_dead_and_fields\":true,\"summoned_not_invisible\":true,\"raw_final_byte\":true,\"failure_alias_lifetime_guards\":true}\n";
}
