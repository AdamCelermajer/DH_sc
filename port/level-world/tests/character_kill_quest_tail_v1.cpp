#include "../character_kill_quest_tail_v1.hpp"

#include <array>
#include <cassert>
#include <cstdio>
#include <stdexcept>
#include <vector>

namespace k=dh2::character_kill_quest_tail_v1;
struct Fixture {
    std::vector<std::string> calls;
    std::vector<k::Event> events;
    k::Runtime* runtime=nullptr;
    std::uintptr_t level=0x7001;
    std::uintptr_t word25=0x2001;
    std::int16_t half2532=0x123,half2533=0x234;
    unsigned fail_level=0,fail_constant=0,fail_raise=0,level_calls=0,
        constant_calls=0,raise_calls=0;
    bool mutate_template=true;
    k::Status nested=k::Status::complete;
    std::string error;
};
bool current_level(void* raw,std::uintptr_t& level,std::string& error){
    auto& f=*static_cast<Fixture*>(raw);++f.level_calls;f.calls.emplace_back("current_level");
    if(f.fail_level){error="level fail";return false;}level=f.level;return true;
}
bool word25(void* raw,std::uintptr_t character,std::uintptr_t& value,std::string& error){
    (void)error;
    auto& f=*static_cast<Fixture*>(raw);assert(character==0x1001);
    f.calls.emplace_back("word25");value=f.word25;return true;
}
bool halfword(void* raw,std::uintptr_t character,std::uint32_t index,std::int16_t& value,std::string& error){
    (void)error;
    auto& f=*static_cast<Fixture*>(raw);assert(character==0x1001);
    f.calls.emplace_back(index==2532?"half2532":"half2533");
    if(index==2532)value=f.half2532;else{assert(index==2533);value=f.half2533;}return true;
}
bool constant(void* raw,const char* group,const char* key,std::int32_t& value,std::string& error){
    auto& f=*static_cast<Fixture*>(raw);++f.constant_calls;
    assert(std::string(group)=="v2QuestObjectiveType");
    f.calls.emplace_back(std::string("constant:")+key);
    if(f.runtime){k::Result r{};f.nested=f.runtime->run(&r,error);}
    if(f.constant_calls==f.fail_constant){error="constant fail";return false;}
    value=100+std::int32_t(f.constant_calls);return true;
}
bool raise(void* raw,std::uintptr_t level,k::Event& event,std::string& error){
    auto& f=*static_cast<Fixture*>(raw);++f.raise_calls;
    f.calls.emplace_back("raise");assert(level==0x7001);
    assert(event.source_subject==-1&&event.flag0==0&&event.flag1==0);
    f.events.push_back(event);event.flag0=1;
    if(f.raise_calls==2&&f.mutate_template){f.word25=0x3001;f.half2533=0x245;}
    if(f.raise_calls==f.fail_raise){error="raise fail";return false;}
    return true;
}
k::Bindings bindings(bool gate=false,bool templated=true){
    (void)templated;return {0x1001,0x1002,gate?1u:0u,0};
}
 k::Services services(Fixture& f){return {&f,current_level,word25,halfword,constant,raise};}
void check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
int main(){
    unsigned cases=0;
    {
        Fixture f;k::Runtime run(bindings(),services(f));f.runtime=&run;
        k::Result result{};std::string error;
        check(run.run(&result,error)==k::Status::complete,"full tail failed");
        check(result.events_attempted==4&&result.events_raised==4,"full event count");
        const std::array<k::Kind,4> kinds={k::Kind::kill_enemies,k::Kind::clear_enemies,
            k::Kind::kill_enemy_template,k::Kind::clear_enemy_template};
        const std::array<const char*,4> names={"constant:KillXEnemies","constant:ClearEnemies",
            "constant:KillEnemyTemplate","constant:ClearEnemyTemplate"};
        check(f.events.size()==4,"full event payload count");
        for(unsigned i=0;i<4;++i){
            check(f.events[i].kind==kinds[i],"source event order");
            check(f.events[i].killer==0x1002&&f.events[i].character_word_25==(i<2?0x2001:0x3001),
                  "killer or character word payload");
            check(f.events[i].source_word_24==(i<2?0x123:0x245),"source +24 payload");
            const unsigned offset=i<2?3+i*2:9+(i-2)*2;
            check(f.calls[offset]==names[i]&&f.calls[offset+1]=="raise","constant/raise order");
        }
        check(f.nested==k::Status::busy,"provider reentry not rejected");
        check(f.calls[0]=="current_level"&&f.calls[1]=="word25"&&f.calls[2]=="half2532",
              "source field/current-Level read order");
        check(run.run(&result,error)==k::Status::consumed,"event tail retried");
        cases+=4;
    }
    {
        Fixture f;f.half2533=-1;f.mutate_template=false;
        k::Runtime run(bindings(false,false),services(f));
        k::Result result{};std::string error;
        check(run.run(&result,error)==k::Status::complete&&result.events_raised==2,
              "untagged actor should emit only two common events");
        check(f.events[0].kind==k::Kind::kill_enemies&&f.events[1].kind==k::Kind::clear_enemies,
              "common event order");++cases;
    }
    for(unsigned gate=0;gate<2;++gate){
        Fixture f;auto b=bindings(gate==0);if(gate)b.suppress_byte_5348=1;
        k::Runtime run(b,services(f));k::Result result{};std::string error;
        check(run.run(&result,error)==k::Status::complete&&result.skipped==1&&f.calls.empty(),
              "source kill-tail gate");++cases;
    }
    for(unsigned mode=1;mode<=2;++mode){
        Fixture f;f.fail_constant=mode==1?2:0;f.fail_raise=mode==2?2:0;
        k::Runtime run(bindings(),services(f));k::Result result{};std::string error;
        check(run.run(&result,error)==k::Status::provider_failed,"provider failure not surfaced");
        check(result.events_raised==1&&result.events_attempted==2,"failure prefix changed");
        check(run.run(&result,error)==k::Status::consumed,"failed episode retried");cases+=2;
    }
    {
        Fixture f;f.level=0;k::Runtime run(bindings(),services(f));
        k::Result result{};std::string error;
        check(run.run(&result,error)==k::Status::provider_failed&&f.calls.size()==1&&
              f.calls[0]=="current_level",
              "missing reached Level was accepted");++cases;
    }
    std::printf("PASS: Character::Kill quest tail ordering/gates/prefixes cases=%u\n",cases);
}
