#include "../character_skill_state_dispatch_v1.hpp"
#include "../character_coordinator.hpp"

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace k = dh2::character_skill_state_dispatch_v1;
namespace f = dh2::character_skill_fsm_callbacks_v1;
constexpr std::uintptr_t C = 0x123456789abcdef0ull, D = C+100, P = C+200, T = C+300;
void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}
struct Fixture {
    dh2::character::Coordinator coordinator{C};
    std::uint8_t ooi_intent = 255, moving = 1;
    std::uintptr_t physical = P;
    f::Character character{C,C+0x3c8,C+0x4fc,C+0x49c,C+0x3b4,
        &coordinator.state.flags,&coordinator.state.attack_gate,&ooi_intent,&moving,&physical};
    f::State callback_state{&character}; f::Globals globals{D};
    f::Services services{this,invoke};
    k::Projection projection{&coordinator.state,&callback_state,&globals,&services};
    std::vector<f::Operation> calls;
    std::uint32_t monster=0,mini=0,boss=0;
    int fail=-1; bool throws=false;
    std::int32_t timer_id=-1;
    unsigned stops=0, animation_calls=0, pre=0, post=0;
    Fixture() {
        coordinator.state.current=6;
        dh2::character::CoordinatorBindings bindings{}; bindings.context=this;
        bindings.facts=[](void*) { return dh2::character::Facts{}; };
        bindings.services={this,[](void*,dh2::character::State*,const dh2::character::Request*) {
            throw std::runtime_error("unexpected unrelated state provider");
        }};
        coordinator.bind(bindings);
    }
    static std::int32_t invoke(void* raw,f::State*,const f::Request* q,f::Response* out) {
        auto& self=*static_cast<Fixture*>(raw); self.calls.push_back(q->operation);
        require(q->character==C,"source Character identity truncated");
        switch(q->operation) {
        case f::Operation::string_construct:
            require(q->text && !std::strcmp(q->text,"isTracingCharState"),"source debug key differs"); out->identity=T; break;
        case f::Operation::debug_query: require(q->subject==D && q->string==T,"source debug owner/string differs"); break;
        case f::Operation::string_destroy: require(q->subject==T,"source string lifetime differs"); break;
        case f::Operation::raise_event:
            if(q->argument0==0x1e)++self.pre;
            else if(q->argument0==0x1f)++self.post;
            else throw std::runtime_error("unexpected Character event");
            break;
        case f::Operation::set_animation:
            require(q->subject==self.character.machine && q->argument0==UINT32_MAX,"animation receiver/override differs");
            self.coordinator.state.current_animation=self.coordinator.state.animation_override;
            self.coordinator.state.animation_override=-1; ++self.animation_calls; break;
        case f::Operation::set_speed: require(q->argument0==0x3f800000,"source skill speed differs"); break;
        case f::Operation::stop: self.moving=0; ++self.stops; break;
        case f::Operation::start_timer:
            require(q->argument0==10 && !q->argument1 && q->argument2==0x30 && !q->payload,"source Blur timer differs");
            self.timer_id=self.coordinator.start_timer(q->argument0,0,std::int32_t(q->argument2),0);
            out->word=std::uint32_t(self.timer_id); break;
        case f::Operation::is_monster: out->word=self.monster; break;
        case f::Operation::is_miniboss: out->word=self.mini; break;
        case f::Operation::is_boss: out->word=self.boss; break;
        default: break;
        }
        if(self.fail==int(q->operation)) {
            if(self.throws) throw std::runtime_error("provider failure after source effects");
            return -1;
        }
        return 0;
    }
};

int oracle(int argc,char** argv) {
    require(argc==7,"oracle arguments missing");
    dh2::character::State state{};
    state.current=std::stoi(argv[2]); state.flags=std::uint32_t(std::stoull(argv[3]));
    state.elapsed_ms=93; k::Result result{};
    const auto status=k::event(&state,std::uint32_t(std::stoull(argv[4])),
                               std::stoi(argv[5])?argv[6]:nullptr,&result);
    std::cout<<"{\"status\":"<<int(status)<<",\"next\":"<<result.next
             <<",\"flags\":"<<state.flags<<",\"writes\":"<<result.on_event_writes
             <<",\"predicate\":"<<result.predicate<<",\"registered\":"<<result.registered
             <<",\"current\":"<<state.current<<",\"elapsed\":"<<state.elapsed_ms<<"}\n";
    return 0;
}
int main(int argc,char** argv) { try {
    if(argc>1 && std::string(argv[1])=="--oracle") return oracle(argc,argv);
    unsigned functional=0, failures=0, guards=0;
    for(auto flags:{0u,0x8000u,0x10000u,0x18000u,0xffffffffu})
        for(auto event:{0x22u,0xc351u,0xc354u,0xc355u,0xc358u,0xc35au,0xc35bu,0xc35cu,0xc35du,0x30u}) {
            dh2::character::State state{}; state.current=6;state.flags=flags;state.elapsed_ms=91;
            k::Result result{};const auto status=k::event(&state,event,nullptr,&result);
            require(status==k::Status::complete || status==k::Status::unsupported_source_state,"state event rejected valid source input");
            require(state.current==6 && state.elapsed_ms==91 && state.flags==flags,"projection took FSM/elapsed ownership");
            if(event==0x30)require(result.next==-1 && !result.registered,"Blur timer invented state10");
            ++functional;
        }
    for(auto label:{"is_stoppable","is_stoppableX","is_stop","","IS_STOPPABLE","do_skill"}) {
        dh2::character::State state{};state.current=6;state.flags=0x6341;
        k::Result result{};require(k::event(&state,0x28,label,&result)==k::Status::complete,"animation label rejected");
        require(state.flags==(!std::strcmp(label,"is_stoppable")?0xe341u:0x6341u) && result.next==-1,"OnEvent28 inferred a label or transition");++functional;
    }
    for(auto current:{3,4,5}) {
        dh2::character::State state{};state.current=current;k::Result result{};
        require(k::event(&state,0xc355,nullptr,&result)==k::Status::complete && result.next==6 && result.registered && !result.predicate,"source entry registration lost");++functional;
    }
    {
        Fixture self;self.coordinator.state.animation_override=0x12345678;
        self.coordinator.state.attack_gate=0x140;self.monster=2;k::Result result{};
        require(k::callback(&self.projection,f::Callback::focus,&result)==k::Status::complete && result.callback.complete,"delegated Focus failed");
        require(self.coordinator.state.current==6 && self.coordinator.state.flags==0x16341 && self.coordinator.state.attack_gate==0x100 && !self.ooi_intent && self.pre==1 && self.animation_calls==1 && self.coordinator.state.current_animation==0x12345678 && self.coordinator.state.animation_override==-1,"Focus did not borrow single FSM/animation fields");
        require(k::callback(&self.projection,f::Callback::blur,&result)==k::Status::complete && self.post==1 && self.stops==1 && self.timer_id==0 && self.coordinator.timers().count==1,"Blur did not use single timer owner");
        const auto& timer=self.coordinator.timers().slots[0];
        require(timer.duration_ms==10 && timer.event==0x30 && !timer.repeat && !timer.user_ref && timer.active,"real timer fields differ");
        require(self.coordinator.update_timers(9,0)==1 && timer.elapsed_ms==9 && timer.active,"real timer duration differs");
        require(k::update(&self.coordinator.state,&result)==k::Status::complete && self.coordinator.state.current==6 && result.next==-1,"skill update invented timeout");++functional;
    }
    for(auto operation:{f::Operation::raise_event,f::Operation::set_animation,f::Operation::set_speed,f::Operation::cancel_sneaking,f::Operation::is_monster})
        for(bool throws:{false,true}) {
            Fixture self;self.fail=int(operation);self.throws=throws;k::Result result{};
            require(k::callback(&self.projection,f::Callback::focus,&result)==k::Status::service_failed && result.callback.last_operation==operation && result.callback.calls==self.calls.size() && !result.callback.complete,"provider failure lost source prefix");
            require(self.coordinator.state.flags==0x6341 && self.coordinator.state.current==6,"failure rolled back FSM flags or transitioned");++failures;
        }
    {
        Fixture self;k::Result result{};result.next=77;const auto prior=result;
        auto unchanged=[&](k::Status status) {require(status==k::Status::invalid_argument && !std::memcmp(&result,&prior,sizeof(result)) && self.calls.empty(),"malformed projection changed source/output");++guards;};
        unchanged(k::callback(nullptr,f::Callback::focus,&result));
        auto* saved=self.character.flags_520;std::uint32_t other=0;self.character.flags_520=&other;
        unchanged(k::callback(&self.projection,f::Callback::focus,&result));self.character.flags_520=saved;
        unchanged(k::event(nullptr,0x28,"is_stoppable",&result));
        unchanged(k::event(&self.coordinator.state,0x28,nullptr,&result));
        unchanged(k::event(&self.coordinator.state,0x28,reinterpret_cast<const char*>(&result),&result));
        self.character.machine_moving_58=reinterpret_cast<const std::uint8_t*>(&result);
        unchanged(k::callback(&self.projection,f::Callback::focus,&result));
    }
    std::cout<<"{\"validation\":\"PASS\",\"functional_cases\":"<<functional
             <<",\"failure_cases\":"<<failures<<",\"guards\":"<<guards
             <<",\"single_coordinator_state\":true,\"single_timer_owner\":true"
             <<",\"timer_event\":48,\"timer_duration_ms\":10,\"full_player_AIS_loaded\":false}\n";
    return 0;
} catch(const std::exception& error) { std::cerr<<error.what()<<'\n';return 1; } }
