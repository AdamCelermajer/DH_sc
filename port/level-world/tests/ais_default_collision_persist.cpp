#include "../ais_default_collision_persist.hpp"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>
namespace k=dh2::ais_default_collision_persist;
namespace u=dh2::ais_external_update;
constexpr std::uintptr_t AIS=0x10010000,A=0x10014000,B=0x10018000,P=0x10020000,Q=0x10024000,APP=0x10028000;
struct Call {unsigned op;std::uintptr_t subject,peer;unsigned argument;};
struct ViewCall {unsigned kind;std::uintptr_t identity;};
struct Fixture {
    u::State shared{AIS,A,1,180};k::State state{&shared,1};
    k::OwnerFacts a{A,Q,0,0},b{B,Q,Q,0};k::PeerFacts peer{P,2};k::Application app{APP,2};
    unsigned state_id=4,char1=0,char2=0,flag=1,player1=0,player2=0,enemy1=0,enemy2=0,dt=25,mutation=0;
    unsigned fail=0,throws=0,view_fail=0,view_alias=0,chars=0,players=0,enemies=0;
    k::Result result{};std::vector<Call> calls;std::vector<ViewCall> views;
    k::Services services{this,invoke,view};
    static int view(void* c,k::View kind,std::uintptr_t identity,const void** out) {
        auto& f=*static_cast<Fixture*>(c);f.views.push_back({unsigned(kind),identity});
        if(f.view_fail==f.views.size())return 9;
        if(f.view_alias==f.views.size()){*out=&f.result;return 0;}
        switch(kind) {
        case k::View::owner:*out=identity==A?&f.a:identity==B?&f.b:nullptr;break;
        case k::View::peer:*out=identity==P?&f.peer:nullptr;break;
        case k::View::application:*out=&f.app;break;
        }return 0;
    }
    static int invoke(void* c,k::State*,const k::Request* q,unsigned* out) {
        auto& f=*static_cast<Fixture*>(c);f.calls.push_back({unsigned(q->operation),q->subject,q->peer,q->argument});
        if(f.fail==f.calls.size())return 9;
        if(f.throws==f.calls.size())throw std::runtime_error("provider");
        switch(q->operation) {
        case k::Operation::state_is_moving:
            assert(q->argument==0);*out=f.state_id==4 || f.state_id==19;
            if(f.mutation==1)f.shared.owner=B;
            break;
        case k::Operation::is_character:
            *out=++f.chars==1?f.char1:f.char2;
            if(f.chars==1 && f.mutation==2)f.shared.owner=B;
            if(f.chars==1 && f.mutation==8)f.shared.counter_bc=42;
            if(f.chars==2 && f.mutation==10){f.shared.owner=B;f.b.byte_3e0=1;}
            break;
        case k::Operation::is_player:
            *out=++f.players==1?f.player1:f.player2;
            if(f.players==1 && f.mutation==3)f.shared.owner=B;
            if(f.players==2 && f.mutation==5)f.shared.owner=B;
            if(f.players==1 && f.mutation==11)f.a.master_418=Q;
            break;
        case k::Operation::is_enemy:
            *out=++f.enemies==1?f.enemy1:f.enemy2;
            if(f.enemies==1 && f.mutation==4)f.shared.owner=B;
            if(f.enemies==2 && f.mutation==6)f.shared.owner=B;
            if(f.enemies==1 && f.mutation==12)f.a.target_408=0;
            break;
        case k::Operation::set_target:assert(q->argument==0 && q->peer==P);break;
        case k::Operation::cancel_sneaking:if(f.mutation==7)f.shared.owner=B;break;
        case k::Operation::frame_delta:
            *out=f.dt;
            if(f.mutation==9){f.shared.counter_bc=999;f.state.frame_c0=999;f.app.frame_74=99;f.shared.owner=B;}
            if(f.mutation==13)f.shared.owner=B;
            break;
        }return 0;
    }
    k::Status run(std::uintptr_t identity=P){return k::persist(&state,identity,flag,&services,&result);}
};
void dump(const Fixture& f,k::Status status) {
    std::cout<<"{\"status\":"<<int(status)<<",\"counter\":"<<f.shared.counter_bc<<",\"frame\":"<<f.state.frame_c0
        <<",\"owner\":"<<f.shared.owner<<",\"target\":"<<f.a.target_408<<",\"master\":"<<f.a.master_418
        <<",\"app_frame\":"<<f.app.frame_74<<",\"calls\":[";
    bool first=true;for(auto c:f.calls){if(!first)std::cout<<',';first=false;std::cout<<'['<<c.op<<','<<c.subject<<','<<c.peer<<','<<c.argument<<']';}
    std::cout<<"],\"views\":[";first=true;for(auto v:f.views){if(!first)std::cout<<',';first=false;std::cout<<'['<<v.kind<<','<<v.identity<<']';}
    std::cout<<"]}\n";
}
std::uintptr_t identity(unsigned kind){return kind==0?0:kind==1?P:kind==2?Q:1;}
int main(int argc,char** argv) {
    if(argc>1) {
        assert(argc==18);unsigned x[17];for(unsigned i=0;i<17;++i)x[i]=std::strtoul(argv[i+1],nullptr,0);
        Fixture f;f.state_id=x[0];f.char1=x[1];f.char2=x[2];f.flag=x[3];f.peer.type_f4=x[4];f.player1=x[5];f.player2=x[6];
        f.enemy1=x[7];f.enemy2=x[8];f.a.target_408=identity(x[9]);f.a.master_418=identity(x[10]);f.a.byte_3e0=x[11];
        f.state.frame_c0=x[12];f.app.frame_74=x[13];f.shared.counter_bc=x[14];f.dt=x[15];f.mutation=x[16];
        const auto status=f.run();dump(f,status);return 0;
    }
    unsigned cases=0;
    {Fixture f;assert(f.run()==k::Status::complete && f.shared.counter_bc==205 && f.state.frame_c0==2 && f.result.counter_added);++cases;}
    for(unsigned state_id:{0u,3u,4u,19u,0xffffffffu}){Fixture f;f.state_id=state_id;
        assert(f.run()==k::Status::complete && f.result.counter_added==(state_id==4 || state_id==19));++cases;}
    {Fixture f;f.a.target_408=P;assert(f.run()==k::Status::complete && f.calls.size()==1 && !f.result.counter_added);++cases;}
    {Fixture f;f.a.target_408=0;assert(f.run(0)==k::Status::complete && f.calls.size()==1);++cases;}
    {Fixture f;f.flag=0;assert(f.run()==k::Status::complete && f.calls.size()==2);++cases;}
    {Fixture f;f.flag=0xffffffff;assert(f.run()==k::Status::complete && f.result.counter_added);++cases;}
    for(unsigned type:{0u,2u,21u,0xffffffffu}){Fixture f;f.peer.type_f4=type;
        assert(f.run()==k::Status::complete && f.result.counter_added==(type==2 || type==21));++cases;}
    {Fixture f;f.char2=7;f.peer.type_f4=0;assert(f.run()==k::Status::complete && f.result.counter_added);++cases;}
    {Fixture f;f.state.frame_c0=2;assert(f.run()==k::Status::complete && f.calls.size()==3 && f.shared.counter_bc==180);++cases;}
    {Fixture f;f.a.byte_3e0=255;assert(f.run()==k::Status::complete && f.state.frame_c0==1 && f.shared.counter_bc==180);++cases;}
    {Fixture f;f.shared.counter_bc=0xfffffff0u;f.dt=32;assert(f.run()==k::Status::complete && f.shared.counter_bc==16);++cases;}
    {Fixture f;f.char1=1;f.enemy1=1;assert(f.run()==k::Status::complete && f.result.set_target_calls==1 && !f.result.counter_added && f.calls.back().op==4);++cases;}
    {Fixture f;f.char1=1;f.a.master_418=Q;f.enemy1=1;assert(f.run()==k::Status::complete && !f.result.set_target_calls && f.result.counter_added);++cases;}
    {Fixture f;f.char1=1;f.player1=f.player2=f.enemy1=1;assert(f.run()==k::Status::complete && f.result.cancel_sneaking_calls==1 && f.result.counter_added);++cases;}
    for(unsigned mutation=1;mutation<=13;++mutation){Fixture f;f.mutation=mutation;
        if((mutation>=3 && mutation<=7) || mutation==11 || mutation==12){f.char1=1;f.player1=1;f.player2=1;f.enemy1=1;f.enemy2=1;}
        if(mutation==4 || mutation==11 || mutation==12){f.player1=0;f.enemy1=1;}
        if(mutation==6){f.player1=0;f.enemy1=0;}
        assert(f.run()==k::Status::complete);
        if(mutation==8)assert(f.shared.counter_bc==67);
        if(mutation==9)assert(f.shared.counter_bc==205 && f.state.frame_c0==999);
        if(mutation==10)assert(!f.result.counter_added);
        if(mutation==11 || mutation==12)assert(f.result.set_target_calls==1);
        ++cases;}
    for(unsigned failed=1;failed<=4;++failed){Fixture f;f.fail=failed;assert(f.run()==k::Status::service_failed && f.calls.size()==failed);
        if(failed==4)assert(f.state.frame_c0==2 && f.shared.counter_bc==180);
        ++cases;}
    for(unsigned failed=1;failed<=4;++failed){Fixture f;f.view_fail=failed;assert(f.run()==k::Status::service_failed && f.views.size()==failed);++cases;}
    for(unsigned alias=1;alias<=4;++alias){Fixture f;f.view_alias=alias;assert(f.run()==k::Status::invalid_source_fact);++cases;}
    {Fixture f;f.throws=4;assert(f.run()==k::Status::service_failed && f.state.frame_c0==2 && f.shared.counter_bc==180);++cases;}
    {Fixture f;f.services.invoke=nullptr;assert(f.run()==k::Status::service_unavailable);++cases;}
    {Fixture f;f.services.view=nullptr;assert(f.run()==k::Status::service_unavailable && f.calls.size()==1);++cases;}
    {Fixture f;f.a.byte_3e0=256;assert(f.run()==k::Status::invalid_source_fact);++cases;}
    {Fixture f;f.peer.identity=P+4;assert(f.run()==k::Status::invalid_source_fact);++cases;}
    {Fixture f;assert(k::persist(&f.state,P,1,&f.services,reinterpret_cast<k::Result*>(&f.shared))==k::Status::invalid_argument && f.calls.empty());++cases;}
    {Fixture f;alignas(k::State) unsigned char storage[sizeof(k::State)+1]{};
        assert(k::persist(reinterpret_cast<k::State*>(storage+1),P,1,&f.services,&f.result)==k::Status::invalid_argument);++cases;}
    // The genuine collision producer and frozen OnUpdate use the same +bc.
    {Fixture f;assert(f.run()==k::Status::complete && f.shared.counter_bc==205);
        unsigned calls=0;u::Services s{&calls,[](void* c,u::State*,const u::Request* q)->int {
            ++*static_cast<unsigned*>(c);if(q->operation==u::Operation::pause_character_ai)assert(q->argument==1000);return 0;
        }};u::Result result{};assert(u::update(&f.shared,&s,&result)==u::Status::complete && f.shared.counter_bc==0 && result.default_pause_due && calls==5);++cases;}
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<",\"shared_counter_composition\":true,\"counter_store_before_dt\":true,\"fresh_owner_and_peer_queries\":true,\"failure_alias_contracts\":true}\n";
}
