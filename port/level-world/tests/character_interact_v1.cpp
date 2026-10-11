#include "../character_interact_v1.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace {
using namespace dh2::character_interact_v1;
struct Fixture {
    std::vector<std::string> calls;
    bool fail_event{};
    bool fail_state{};
    bool fail_raise_event{};
    std::uint8_t target_marker{};
    std::uint8_t marker_after_state=0xff;
};
bool raise_talk(void* raw,
    dh2::level_world::current_level_quest_event_v1::Event& event,
    std::string& error) {
    auto& f=*static_cast<Fixture*>(raw);
    if(event.objective_type!=9||event.character!=0x100||
       event.network_id!=42||event.subject_id!=-1) {
        error="TalkToNPC event fields changed";return false;
    }
    f.calls.emplace_back("talk");
    if(f.fail_event){error="event failure";return false;}
    return true;
}
bool set_state(void* raw,std::uintptr_t target,std::int32_t animation,
    std::uint8_t suppressed,std::uintptr_t interactor,std::uint8_t direct,
    std::string& error) {
    auto& f=*static_cast<Fixture*>(raw);
    if(target!=0x200||animation!=77||suppressed!=1||interactor!=0x100||direct!=0) {
        error="SM_SetInteractState arguments changed";return false;
    }
    f.calls.emplace_back("state");
    if(f.marker_after_state!=0xff)f.target_marker=f.marker_after_state;
    if(f.fail_state){error="state failure";return false;}
    return true;
}
bool read_target_marker(void* raw,std::uintptr_t target,
    std::uint8_t& value,std::string& error) {
    auto& f=*static_cast<Fixture*>(raw);
    if(target!=0x200){error="target marker identity changed";return false;}
    value=f.target_marker;return true;
}
bool raise_event(void* raw,std::uintptr_t target,std::uint32_t event,
    std::uintptr_t interactor,std::string& error) {
    auto& f=*static_cast<Fixture*>(raw);
    if(target!=0x200||event!=5||interactor!=0x100) {
        error="Character::RaiseEvent arguments changed";return false;
    }
    f.calls.emplace_back("raise5");
    if(f.fail_raise_event){error="raise event failure";return false;}
    return true;
}
void require(bool ok,const char* message) {
    if(!ok){std::cerr<<message<<'\n';std::exit(1);}
}
}
int main() {
    Request request{0x200,0x100,9,42,77,0,0};
    Fixture fixture;
    Services services{&fixture,raise_talk,set_state,raise_event,read_target_marker};
    Result result{};std::string error;
    require(interact(request,services,result,error)==Status::complete,
            "fresh target interaction failed");
    require(fixture.calls==std::vector<std::string>{"talk","state","raise5"},
            "Character::Interact source call order changed");
    require(result.talk_event_raised&&result.state_set&&
            result.character_event_raised,"fresh interaction result incomplete");

    fixture.calls.clear();request.target_is_interacting=1;
    require(interact(request,services,result,error)==Status::complete,
            "already-interacting target failed its source state path");
    require(fixture.calls==std::vector<std::string>{"state","raise5"}&&
            !result.talk_event_raised,"already-interacting target raised TalkToNPC");

    fixture.calls.clear();fixture.fail_event=true;request.target_is_interacting=0;
    require(interact(request,services,result,error)==Status::provider_failed&&
            fixture.calls==std::vector<std::string>{"talk"}&&!result.state_set,
            "failed RaiseAsync crossed the source prefix into FSM mutation");
    fixture.fail_event=false;fixture.calls.clear();fixture.target_marker=0;
    fixture.marker_after_state=1;request.target_is_interacting=0;
    require(interact(request,services,result,error)==Status::complete&&
            fixture.calls==std::vector<std::string>{"talk","state"}&&
            result.state_set&&!result.character_event_raised,
            "Character::Interact did not read +0x2fa after callbacks changed it");
    std::cout<<"Character::Interact: source event/state/event order verified\n";
}
