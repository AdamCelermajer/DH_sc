#include "../character_ai_on_revived_v1.hpp"
#include "../character_ai_pause_update.hpp"

#include <stdexcept>
#include <string>
#include <vector>

namespace {
using namespace dh2::character_ai_on_revived_v1;
void check(bool value,const char* message){if(!value)throw std::runtime_error(message);}

struct Fixture {
    dh2::character::Coordinator coordinator{0x410000001ull,1};
    dh2::character::AIFrameOwner48 owner{};
    dh2::character_ai_initialization::State ai{};
    std::vector<Operation> order;
    bool fail_dot_start=false;
    int next_timer=7;

    Fixture(){
        owner={coordinator.owner(),0x411,0,0,0,0,0,0,0,0};
        dh2::character::CoordinatorBindings bindings{};
        bindings.context=this;
        bindings.facts=read_facts;
        bindings.services={this,state_service};
        bindings.ai_identity=0x412;
        bindings.ai_owner_projection=&owner;
        bindings.controller_identity=owner.controller;
        coordinator.bind(bindings);
        ai.identity=0x412;ai.owner_04=coordinator.owner();
        ai.active_ais_1c=0x413;ai.word_10=99;ai.word_14=100;
    }
    static dh2::character::Facts read_facts(void*){return {};}
    static void state_service(void*,dh2::character::State*,
                              const dh2::character::Request*){}
    static int invoke(void* raw,const Request* request,Reply* reply,
                      std::string& error){
        auto& self=*static_cast<Fixture*>(raw);self.order.push_back(request->operation);
        switch(request->operation){
        case Operation::stop_all_timers:return 0;
        case Operation::get_ai_tick:reply->duration_ms=900;return 0;
        case Operation::start_ai_tick:
            check(request->repeat==-1&&request->event==51&&request->duration_ms==900,
                  "AI_Tick timer arguments changed");
            reply->timer_id=self.next_timer++;return 0;
        case Operation::get_dot_tick:reply->duration_ms=700;return 0;
        case Operation::start_dot_tick:
            check(request->repeat==-1&&request->event==52&&request->duration_ms==700,
                  "DoT_Tick timer arguments changed");
            if(self.fail_dot_start){error="expected failure";return 1;}
            reply->timer_id=self.next_timer++;return 0;
        case Operation::active_ais_on_revived:return 0;
        }
        return 1;
    }
};

void complete(){
    Fixture f;Services services{&f,Fixture::invoke};Result result{};std::string error;
    check(execute(&f.ai,&f.coordinator,f.coordinator.owner(),f.ai.identity,
        f.ai.active_ais_1c,services,&result,error)==Status::complete,
        "source OnRevived did not complete");
    const std::vector<Operation> expected{Operation::stop_all_timers,
        Operation::get_ai_tick,Operation::start_ai_tick,Operation::get_dot_tick,
        Operation::start_dot_tick,Operation::active_ais_on_revived};
    check(f.order==expected,"source OnRevived operation order changed");
    check(f.ai.word_10==7&&f.ai.word_14==8&&result.ai_tick_started&&
        result.dot_tick_started&&result.active_ais_called,
        "source timer IDs were not stored on the same CharAI owner");
}

void partial_failure(){
    Fixture f;f.fail_dot_start=true;Services services{&f,Fixture::invoke};
    Result result{};std::string error;
    check(execute(&f.ai,&f.coordinator,f.coordinator.owner(),f.ai.identity,
        f.ai.active_ais_1c,services,&result,error)==Status::service_failed,
        "DoT timer failure was not preserved");
    check(f.ai.word_10==7&&f.ai.word_14==100&&result.ai_tick_started&&
        !result.dot_tick_started&&!result.active_ais_called,
        "failure did not preserve exactly the reached source prefix");
}

void identity_failure(){
    Fixture f;Services services{&f,Fixture::invoke};Result result{};
    std::string error;
    check(execute(&f.ai,&f.coordinator,f.coordinator.owner()+1,f.ai.identity,
        f.ai.active_ais_1c,services,&result,error)==Status::invalid_argument&&
        f.order.empty(),"mismatched Character identity reached a provider");
}
}

int main(){complete();partial_failure();identity_failure();}
