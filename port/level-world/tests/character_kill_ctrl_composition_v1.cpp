#include "../character_kill_death_tail_v1.hpp"
#include "../character_kill_source_bridge_v1.hpp"
#include "../player_kill_continuation_v1.hpp"

#include <array>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace tail=dh2::character_kill_death_tail_v1;
namespace ctrl=dh2::player_kill_continuation_v1;
namespace bridge=dh2::character_kill_source_bridge_v1;
namespace {
constexpr std::uintptr_t character=0x100000001ull;
constexpr std::uintptr_t killer_object=0x200000002ull;
constexpr std::uintptr_t killer_character=0x300000003ull;
enum class Step {outer_is_dead,inner_is_dead,preflight,drop,credit,convert,xp,
                 virtual_54,field_14e4,objective_tail,event2};
void check(bool value,const char* why){if(!value)throw std::runtime_error(why);}

struct Fixture {
    std::vector<Step> trace;
    tail::Runtime* tail_runtime=nullptr;
    bool dead=false,inner_dead=false,fail_virtual=false,fail_event2=false;
    std::uint8_t source_script_created_539=0;
    unsigned drop_calls=0,credit_calls=0,xp_calls=0,objective_calls=0,
             event2_calls=0,kill_calls=0;

    static bool preflight(void* raw,const tail::Request&,std::string&){
        static_cast<Fixture*>(raw)->trace.push_back(Step::preflight);return true;
    }
    static bool drop(void* raw,std::uintptr_t,std::uintptr_t,std::string&){
        auto& f=*static_cast<Fixture*>(raw);f.trace.push_back(Step::drop);
        ++f.drop_calls;return true;
    }
    static bool credit(void* raw,std::uintptr_t,std::uintptr_t,std::string&){
        auto& f=*static_cast<Fixture*>(raw);f.trace.push_back(Step::credit);
        ++f.credit_calls;return true;
    }
    static bool convert(void* raw,std::uintptr_t,std::uintptr_t,
                        std::uintptr_t& killer,bool& award,std::string&){
        static_cast<Fixture*>(raw)->trace.push_back(Step::convert);
        killer=killer_character;award=true;return true;
    }
    static bool xp(void* raw,std::uintptr_t,std::uintptr_t,std::string&){
        auto& f=*static_cast<Fixture*>(raw);f.trace.push_back(Step::xp);++f.xp_calls;return true;
    }
    static bool virtual54(void* raw,std::uintptr_t,std::int32_t& value,std::string& error){
        auto& f=*static_cast<Fixture*>(raw);f.trace.push_back(Step::virtual_54);
        if(f.fail_virtual){error="virtual+0x54 injected failure";return false;}
        value=0;return true;
    }
    static bool field(void* raw,std::uintptr_t,std::uint8_t& value,std::string&){
        auto& f=*static_cast<Fixture*>(raw);f.trace.push_back(Step::field_14e4);
        value=f.source_script_created_539;return true;
    }
    static bool objective(void* raw,std::uintptr_t victim,std::uintptr_t killer,
                          std::int32_t virtual_result,std::uint8_t field,
                          std::string& error){
        auto& f=*static_cast<Fixture*>(raw);
        check(victim==character&&killer==killer_object&&virtual_result==0&&field==0,
              "Kill objective tail received ungated source facts");
        f.trace.push_back(Step::objective_tail);++f.objective_calls;
        error.clear();return true;
    }
    static int backend(void* raw,const ctrl::Request* request,ctrl::Reply* reply,
                       std::string& error){
        auto& f=*static_cast<Fixture*>(raw);
        check(request&&reply&&request->character==character&&
              request->killer==killer_object,"CtrlCaller identity changed");
        switch(request->operation){
        case ctrl::Operation::is_dead:
            f.trace.push_back(Step::outer_is_dead);reply->word=f.dead?1:0;return 0;
        case ctrl::Operation::kill:
            ++f.kill_calls;f.trace.push_back(Step::inner_is_dead);
            if(f.inner_dead)return 0;
            if(!f.tail_runtime){error="inner Character::Kill owner missing";return 1;}
            {
                tail::Result result{};
                const auto status=f.tail_runtime->run(&result,error);
                if(status==tail::Status::complete)f.dead=true;
                return status==tail::Status::complete?0:1;
            }
        case ctrl::Operation::event2:
            check(request->argument==2&&request->subject==character,
                  "CtrlCaller event2 arguments changed");
            f.trace.push_back(Step::event2);++f.event2_calls;
            if(f.fail_event2){error="RaiseEvent(2,killer) injected failure";return 1;}
            return 0;
        default:error="unexpected CtrlCaller operation";return 1;
        }
    }
};

void success_and_one_shot(){
    Fixture f;
    tail::Services services{&f,Fixture::preflight,Fixture::drop,Fixture::credit,
        Fixture::convert,Fixture::xp,Fixture::virtual54,Fixture::field,
        Fixture::objective};
    tail::Runtime tail_runtime({character,killer_object,0},services);
    f.tail_runtime=&tail_runtime;
    ctrl::CtrlCaller caller(character,{&f,Fixture::backend});
    bridge::Runtime shared(character,caller);bridge::Result result{};std::string error;
    bool rejected_mismatched_owner=false;
    try {
        ctrl::CtrlCaller wrong(character+1,{&f,Fixture::backend});
        bridge::Runtime invalid(character,wrong);
    } catch(const std::invalid_argument&) {rejected_mismatched_owner=true;}
    check(rejected_mismatched_owner,"shared bridge accepted another Character's CtrlCaller");
    const bridge::Request melee{character,killer_object,0,bridge::Source::player_melee};
    check(shared.dispatch(melee,&result,error)==ctrl::Status::complete&&
          result.ctrl.kill_completed&&result.ctrl.event2_completed&&f.xp_calls==1&&
          f.event2_calls==1&&f.kill_calls==1,
          "Ctrl_Kill did not run one inner tail then one event2");
    const std::vector<Step> expected={Step::outer_is_dead,Step::inner_is_dead,
        Step::preflight,Step::drop,Step::credit,Step::convert,Step::xp,
        Step::virtual_54,Step::field_14e4,Step::objective_tail,Step::event2};
    check(f.trace==expected,"inner Kill / outer Ctrl_Kill event order changed");
    for(const auto source:{bridge::Source::player_skill,bridge::Source::npc_attack,
                           bridge::Source::player_melee}){
        const auto calls=f.trace.size();error="sentinel";
        check(shared.dispatch({character,killer_object,0,source},&result,error)==
                  ctrl::Status::complete&&result.ctrl.skipped&&
              result.source==source&&f.trace.size()==calls+1&&f.xp_calls==1&&
              f.event2_calls==1&&f.kill_calls==1,
              "shared melee/skill/NPC ingress replayed a completed Kill effect");
    }
    const auto calls=f.trace.size();
    check(shared.dispatch({character,killer_object,0,
              bridge::Source::death_visual_completion},&result,error)==
              ctrl::Status::invalid_argument&&
          f.trace.size()==calls&&f.xp_calls==1&&f.event2_calls==1,
          "death visual completion was accepted as a second Kill ingress");

    // A real revive opens a new inner Character::Kill episode. Keep the same
    // bridge and outer CtrlCaller, but replace the consumed per-death tail.
    f.dead=false;f.source_script_created_539=1;
    tail::Runtime scripted_tail({character,killer_object,0},services);
    f.tail_runtime=&scripted_tail;
    const auto second_begin=f.trace.size();
    check(shared.dispatch({character,killer_object,0,bridge::Source::player_skill},
              &result,error)==ctrl::Status::complete&&
          result.ctrl.kill_completed&&result.ctrl.event2_completed&&
          scripted_tail.run(nullptr,error)==tail::Status::consumed&&
          f.xp_calls==2&&f.drop_calls==2&&f.credit_calls==2&&
          f.objective_calls==1&&f.event2_calls==2,
          "script-created Character+0x14e4 episode did not suppress only its objective tail");
    const std::vector<Step> scripted_expected={Step::outer_is_dead,
        Step::inner_is_dead,Step::preflight,Step::drop,Step::credit,Step::convert,
        Step::xp,Step::virtual_54,Step::field_14e4,Step::event2};
    check(std::vector<Step>(f.trace.begin()+std::ptrdiff_t(second_begin),f.trace.end())==
              scripted_expected,
          "script-suppressed second Kill episode changed source tail order");

    // A second revive, now entering through NPC damage, gets its own one-shot
    // tail while the outer caller remains retained across all producer paths.
    f.dead=false;f.source_script_created_539=0;
    tail::Runtime npc_tail({character,killer_object,0},services);
    f.tail_runtime=&npc_tail;
    const auto third_begin=f.trace.size();
    check(shared.dispatch({character,killer_object,0,bridge::Source::npc_attack},
              &result,error)==ctrl::Status::complete&&
          result.ctrl.kill_completed&&result.ctrl.event2_completed&&
          f.xp_calls==3&&f.drop_calls==3&&f.credit_calls==3&&
          f.objective_calls==2&&f.event2_calls==3&&f.kill_calls==3,
          "revived NPC damage did not start exactly one fresh Character::Kill episode");
    const std::vector<Step> npc_expected={Step::outer_is_dead,Step::inner_is_dead,
        Step::preflight,Step::drop,Step::credit,Step::convert,Step::xp,
        Step::virtual_54,Step::field_14e4,Step::objective_tail,Step::event2};
    check(std::vector<Step>(f.trace.begin()+std::ptrdiff_t(third_begin),f.trace.end())==
              npc_expected,
          "new NPC Kill episode failed to restore the authored objective tail");
}

void failure_prefixes(){
    {
        Fixture f;f.fail_virtual=true;
        tail::Runtime tail_runtime({character,killer_object,0},
            {&f,Fixture::preflight,Fixture::drop,Fixture::credit,Fixture::convert,
             Fixture::xp,Fixture::virtual54,Fixture::field,Fixture::objective});
        f.tail_runtime=&tail_runtime;ctrl::CtrlCaller caller(character,{&f,Fixture::backend});
        ctrl::Result result{};std::string error;
        check(caller.kill(killer_object,0,&result,error)==ctrl::Status::failed&&
              !result.event2_completed&&f.xp_calls==1&&f.event2_calls==0,
              "failed inner Kill crossed the outer event2 boundary");
        const auto calls=f.trace.size();f.fail_virtual=false;
        check(caller.kill(killer_object,0,&result,error)==ctrl::Status::consumed&&
              f.trace.size()==calls&&f.xp_calls==1&&f.event2_calls==0,
              "failed inner tail retried after XP");
    }
    {
        Fixture f;f.fail_event2=true;
        tail::Runtime tail_runtime({character,killer_object,0},
            {&f,Fixture::preflight,Fixture::drop,Fixture::credit,Fixture::convert,
             Fixture::xp,Fixture::virtual54,Fixture::field,Fixture::objective});
        f.tail_runtime=&tail_runtime;ctrl::CtrlCaller caller(character,{&f,Fixture::backend});
        ctrl::Result result{};std::string error;
        check(caller.kill(killer_object,0,&result,error)==ctrl::Status::failed&&
              result.kill_completed&&!result.event2_completed&&f.xp_calls==1&&
              f.event2_calls==1,"failed event2 lost completed Kill prefix");
        const auto calls=f.trace.size();f.fail_event2=false;
        check(caller.kill(killer_object,0,&result,error)==ctrl::Status::consumed&&
              f.trace.size()==calls&&f.xp_calls==1&&f.event2_calls==1,
              "failed event2 was replayed after completed Kill");
    }
}
}

int main(){try{success_and_one_shot();failure_prefixes();
    std::cout<<"CHARACTER CTRL KILL COMPOSITION PASS 3\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
