#include "../character_kill_event2_gate_v1.hpp"
#include <array>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace dh2::character_kill_event2_gate_v1;
namespace {
void check(bool value,const char* why){if(!value)throw std::runtime_error(why);}
constexpr std::uintptr_t character=0x100000001ull,killer=0x200000002ull;
struct Fixture {
    std::vector<unsigned> trace;
    unsigned calls=0;
    bool fail=false;
    static int raise(void* raw,std::uintptr_t c,std::uintptr_t k){
        auto& f=*static_cast<Fixture*>(raw);
        check(c==character&&k==killer,"RaiseEvent2 identity/payload changed");
        ++f.calls;
        // CharAI::OnDied invokes active AIS OnDied and AI_SetDead before the
        // same event is forwarded to the FSM.
        f.trace.insert(f.trace.end(),{2,3,4});
        return f.fail?1:0;
    }
};
Readiness ready(){return {1,1,1,1,1,1,1,1,1,1,1,1};}
}
int main(){try{
    constexpr std::array<std::uint32_t,12> bits={missing_kill_tail,
        missing_character_ai,missing_active_ais,missing_script_session,
        missing_state_machine,missing_animation,missing_timers,missing_target,
        missing_relations,missing_skill_cleanup,missing_spell_cleanup,
        missing_group_info};
    for(unsigned i=0;i<bits.size();++i){
        Episode episode{character};Request request{character,killer,1,ready()};
        std::uint32_t* fields[]={&request.readiness.kill_tail,
            &request.readiness.character_ai,&request.readiness.active_ais,
            &request.readiness.script_session,&request.readiness.state_machine,
            &request.readiness.animation,&request.readiness.timers,
            &request.readiness.target,&request.readiness.relations,
            &request.readiness.skill_cleanup,&request.readiness.spell_cleanup,
            &request.readiness.group_info};
        *fields[i]=0;Fixture f;Backend backend{&f,Fixture::raise};Result out{};
        check(dispatch(&episode,&request,&backend,&out)==Status::missing_owner&&
              out.missing==bits[i]&&!out.callback_called&&!episode.event2_attempted&&
              f.calls==0,"unbound source dependency crossed event2 gate");
    }
    {
        Episode episode{character};Request request{character,killer,0,ready()};
        Fixture f;Backend backend{&f,Fixture::raise};Result out{};
        check(dispatch(&episode,&request,&backend,&out)==Status::missing_owner&&
              !episode.event2_attempted&&!f.calls,
              "event2 ran before full Character::Kill completion");
    }
    {
        Episode episode{character};Request request{character,killer,1,ready()};
        Fixture f;f.fail=true;Backend backend{&f,Fixture::raise};Result out{};
        check(dispatch(&episode,&request,&backend,&out)==Status::failed&&
              episode.event2_attempted&&episode.failed&&out.callback_called&&
              !out.delivered&&f.calls==1,"reached failure lost source prefix state");
        f.fail=false;
        check(dispatch(&episode,&request,&backend,&out)==Status::consumed&&
              f.calls==1,"failed event2 prefix was replayed");
    }
    {
        Episode episode{character};Request request{character,killer,1,ready()};
        Fixture f;Backend backend{&f,Fixture::raise};Result out{};
        check(dispatch(&episode,&request,&backend,&out)==Status::complete&&
              episode.event2_attempted&&!episode.failed&&out.delivered&&
              f.trace==std::vector<unsigned>({2,3,4}),
              "completed event2 did not preserve source dispatch order");
        check(dispatch(&episode,&request,&backend,&out)==Status::consumed&&
              f.calls==1,"completed Kill episode replayed event2");
    }
    {
        Episode episode{character};Request request{character,killer,1,ready()};
        request.readiness.active_ais=2;Fixture f;Backend backend{&f,Fixture::raise};Result out{};
        check(dispatch(&episode,&request,&backend,&out)==Status::invalid_argument&&
              !episode.event2_attempted&&!f.calls,"malformed readiness mutated episode");
    }
    std::cout<<"KILL EVENT2 GATE PASS 16\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
