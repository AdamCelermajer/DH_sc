#include "../character_skill_fsm_callbacks_v1.hpp"
#include "../character_coordinator.hpp"
#include "../../android-native/app/src/main/cpp/native_debug_files.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>
namespace k=dh2::character_skill_fsm_callbacks_v1;
constexpr std::uintptr_t C=0x100000001ull,D=0x200000001ull,D2=0x200000002ull,P=0x300000001ull,P2=0x300000002ull,T=0x400000001ull;
void require(bool value,const char* message){if(!value)throw std::runtime_error(message);}
struct Fixture {
    std::uint32_t flags=0,gate=0,other_flags=0x11223344,other_gate=0x55667788;
    std::uint8_t ooi_intent=0xaa,moving=0,other_ooi_intent=7,other_moving=8;
    std::uintptr_t physical=P,other_physical=P2,target=0x500000001ull,last_target=0;
    k::Character character{C,C+0x3c8,C+0x4fc,C+0x49c,C+0x3b4,&flags,&gate,&ooi_intent,&moving,&physical};
    k::Character alternate{C+0x1000,C+0x13c8,C+0x14fc,C+0x149c,C+0x13b4,&other_flags,&other_gate,&other_ooi_intent,&other_moving,&other_physical};
    k::State state{&character};k::Globals globals{D};
    std::uint32_t monster=1,mini=0,boss=0,query_word=0,mutation=0;int failure=-1;
    bool throws=false,string_live=false,zero_string=false,nest=false,nested_completed=false;
    unsigned source_actions=0,stops=0,timer_attempts=0;
    std::vector<std::array<std::uint64_t,6>> trace;
    std::uintptr_t entered_debug=D;
    unsigned tag(std::uintptr_t id)const {
        if(!id)return 0;
        if(id==character.identity)return 1;
        if(id==character.ai)return 2;
        if(id==character.machine)return 3;
        if(id==character.animator)return 4;
        if(id==character.timers)return 5;
        if(id==entered_debug)return 6;
        if(id==D2)return 7;
        if(id==P)return 8;
        if(id==P2)return 9;
        if(id==T)return 10;
        return 99;
    }
    void record(const k::Request& q){
        require(q.character==C && q.payload==0,"captured Character/payload changed");
        trace.push_back({std::uint64_t(q.operation),tag(q.subject),q.argument0,q.argument1,q.argument2,q.string?10u:0u});
        if(mutation==1 && q.operation==k::Operation::debug_load){globals.debug_switches=D2;state.character=&alternate;}
        if(mutation==2 && q.operation==k::Operation::string_construct)gate=0x12345678;
        if(mutation==3 && q.operation==k::Operation::debug_query){gate=0xabcdefaa;flags=0x12340000;}
        if(mutation==4 && q.operation==k::Operation::string_destroy){gate=0xfffffff0;moving=255;physical=P2;}
        if(mutation==5 && q.operation==k::Operation::raise_event){gate^=0x100;physical=P2;flags=0x87650000;}
        if(mutation==6 && q.operation==k::Operation::set_animation){flags=0xfedcba98;moving=0;}
        if(mutation==7 && q.operation==k::Operation::set_speed)ooi_intent=255;
        if(mutation==8 && q.operation==k::Operation::cancel_sneaking){moving=255;physical=P2;gate=0x55550040;}
        if(mutation==9 && q.operation==k::Operation::sync_last_target){gate^=0x100;physical=P2;}
        if(mutation==10 && q.operation==k::Operation::stop){gate^=0x100;physical=P2;}
        if(mutation==11 && (q.operation==k::Operation::pin || q.operation==k::Operation::unpin)){flags=0xaaaa5555;physical=P2;}
        if(mutation==12 && q.operation==k::Operation::is_monster)flags=0x11110001;
        if(mutation==13 && q.operation==k::Operation::is_miniboss)flags=0x22220002;
        if(mutation==14 && q.operation==k::Operation::is_boss)flags=0x33330003;
    }
    void effect(k::State* owner,const k::Request& q,k::Response& out){
        out.word=0xffffffff;++source_actions;
        switch(q.operation){
        case k::Operation::string_construct:
            require(q.text && std::string(q.text)=="isTracingCharState","debug key changed");out.identity=zero_string?0:T;string_live=!zero_string;break;
        case k::Operation::debug_load:require(q.subject==entered_debug,"debug load not captured");break;
        case k::Operation::debug_query:require(q.subject==entered_debug && q.string==T && string_live,"debug query owner/string capture lost");out.word=query_word;break;
        case k::Operation::string_destroy:require(q.subject==T && string_live,"string destructor identity changed");string_live=false;break;
        case k::Operation::sync_last_target:require(q.subject==character.ai,"AI receiver changed");last_target=target;break;
        case k::Operation::stop:require(q.subject==C,"Stop owner changed");moving=0;++stops;break;
        case k::Operation::raise_event:
            require(q.subject==C && (q.argument0==0x1e || q.argument0==0x1f),"source event changed");
            if(nest){nest=false;auto services=this->services();k::Result nested{};nested_completed=k::execute(owner,k::Callback::blur,&globals,&services,&nested)==k::Status::complete;}
            break;
        case k::Operation::set_animation:require(q.subject==character.machine && q.argument0==UINT32_MAX,"SM_SetAnim receiver/value changed");break;
        case k::Operation::set_speed:require(q.subject==character.animator && q.argument0==0x3f800000,"speed changed");break;
        case k::Operation::cancel_sneaking:require(q.subject==C,"CancelSneaking receiver changed");break;
        case k::Operation::pin:case k::Operation::unpin:require(q.subject==P || q.subject==P2,"physical identity truncated");break;
        case k::Operation::start_timer:
            require(q.subject==character.timers && q.argument0==10 && q.argument1==0 && q.argument2==0x30,"source timer arguments changed");++timer_attempts;break;
        case k::Operation::is_monster:require(q.subject==C,"IsMonster receiver changed");out.word=monster;break;
        case k::Operation::is_miniboss:out.word=mini;break;
        case k::Operation::is_boss:out.word=boss;break;
        }
    }
    static int invoke(void* raw,k::State* state,const k::Request* q,k::Response* out){
        auto& f=*static_cast<Fixture*>(raw);f.record(*q);f.effect(state,*q,*out);
        if(f.failure==int(q->operation)){if(f.throws)throw std::runtime_error("source provider failure after effect");return -1;}return 0;
    }
    k::Services services(){return {this,invoke};}
    void print(k::Status status)const {
        std::cout<<"{\"status\":"<<int(status)<<",\"flags\":"<<flags<<",\"gate\":"<<gate<<",\"ooi_intent\":"<<unsigned(ooi_intent)<<",\"moving\":"<<unsigned(moving)
                 <<",\"physical\":"<<tag(physical)<<",\"debug_selection\":"<<(globals.debug_switches==D?6:7)<<",\"live_string\":"<<int(string_live)<<",\"trace\":[";
        for(std::size_t i=0;i<trace.size();++i){if(i)std::cout<<',';std::cout<<'[';for(unsigned n=0;n<6;++n){if(n)std::cout<<',';std::cout<<trace[i][n];}std::cout<<']';}std::cout<<"]}\n";
    }
};
std::uint32_t number(const char* p){return std::uint32_t(std::stoull(p));}
int oracle(int argc,char** argv){
    require(argc==13,"oracle args missing");Fixture f;const auto callback=number(argv[2]);f.flags=number(argv[3]);f.gate=number(argv[4]);f.ooi_intent=std::uint8_t(number(argv[5]));f.moving=std::uint8_t(number(argv[6]));
    f.physical=number(argv[7])?P:0;f.monster=number(argv[8]);f.mini=number(argv[9]);f.boss=number(argv[10]);f.query_word=number(argv[11]);f.mutation=number(argv[12]);
    auto services=f.services();k::Result out{};auto status=k::execute(&f.state,static_cast<k::Callback>(callback),&f.globals,&services,&out);f.print(status);return 0;
}
struct ActualDebugTimers {
    Fixture f;dh2::native::debug_files::Backend debug;dh2::character::Coordinator coordinator{C};
    std::map<std::uintptr_t,std::unique_ptr<std::string>> strings;unsigned constructed=0,destroyed=0;
    std::int32_t timer_id=-1;
    ActualDebugTimers(const std::filesystem::path& cache,const std::filesystem::path& directory){
        std::ifstream input(cache/"DebugSwitches.savegame",std::ios::binary);require(bool(input),"original Debug configuration missing");std::vector<std::uint8_t> bytes{std::istreambuf_iterator<char>(input),{}};
        std::filesystem::create_directories(directory);std::string error;require(debug.initialize(std::filesystem::absolute(directory),bytes.data(),bytes.size(),error),"actual Debug installation failed");
        f.entered_debug=f.globals.debug_switches=debug.runtime().identity();f.character.flags_520=&coordinator.state.flags;f.character.flags_528=&coordinator.state.attack_gate;
        dh2::character::CoordinatorBindings b;b.context=this;b.facts=[](void*){return dh2::character::Facts{};};
        b.services={this,[](void*,dh2::character::State*,const dh2::character::Request*){throw std::runtime_error("unbound source FSM service");}};coordinator.bind(b);
    }
    static int invoke(void* raw,k::State* state,const k::Request* q,k::Response* out){
        auto& a=*static_cast<ActualDebugTimers*>(raw);
        if(q->operation==k::Operation::debug_load || q->operation==k::Operation::string_construct || q->operation==k::Operation::debug_query || q->operation==k::Operation::string_destroy || q->operation==k::Operation::start_timer){
            a.f.record(*q);
            if(q->operation==k::Operation::debug_load){require(q->subject==a.debug.runtime().identity(),"actual Debug owner changed");return a.debug.runtime().load(a.debug.globals(),a.debug.services())==dh2::debug_switches::Status::complete?0:-1;}
            if(q->operation==k::Operation::string_construct){auto key=std::make_unique<std::string>(q->text);auto id=reinterpret_cast<std::uintptr_t>(key.get());a.strings.emplace(id,std::move(key));out->identity=id;++a.constructed;return 0;}
            if(q->operation==k::Operation::debug_query){auto it=a.strings.find(q->string);require(it!=a.strings.end(),"actual string not retained");std::uint8_t value=0;auto s=a.debug.runtime().get_switch(*it->second,a.debug.globals(),a.debug.services(),value);out->word=value;return s==dh2::debug_switches::Status::complete?0:-1;}
            if(q->operation==k::Operation::string_destroy){require(a.strings.erase(q->subject)==1,"actual source string destroy failed");++a.destroyed;return 0;}
            a.timer_id=a.coordinator.start_timer(q->argument0,std::int32_t(q->argument1),std::int32_t(q->argument2),q->payload);out->word=std::uint32_t(a.timer_id);return a.timer_id>=0?0:-1;
        }
        return Fixture::invoke(&a.f,state,q,out);
    }
};
int main(int argc,char** argv){try{
    if(argc>1 && std::string(argv[1])=="--oracle")return oracle(argc,argv);
    require(argc==3,"cache/debug directory required");unsigned functional=0,failures=0,guards=0;
    for(auto callback:{k::Callback::focus,k::Callback::blur})for(auto moving:{0u,1u,255u})for(bool physical:{false,true})for(auto classification:{0u,1u,2u}){
        Fixture f;f.flags=0xffffffff;f.gate=moving?0x140:0x40;f.moving=std::uint8_t(moving);f.physical=physical?P:0;f.monster=classification;auto services=f.services();k::Result out{};
        require(k::execute(&f.state,callback,&f.globals,&services,&out)==k::Status::complete && out.complete && !f.string_live,"functional callbacks failed");
        if(callback==k::Callback::focus)require(f.flags==(classification?0x16341u:0x6341u) && f.gate==(moving?0x100u:0u) && !f.ooi_intent,"source Focus flags/OOI intent differs");
        else require(f.flags==(classification?0xfffeffffu:0xffffffffu) && out.timer_attempted==bool(moving) && f.last_target==f.target,"source Blur timer/classification/sync differs");
        ++functional;
    }
    for(unsigned operation=0;operation<=unsigned(k::Operation::is_boss);++operation)for(bool throws:{false,true}){
        Fixture f;f.failure=int(operation);f.throws=throws;f.gate=0x100;
        auto callback=operation==unsigned(k::Operation::sync_last_target) || operation==unsigned(k::Operation::stop) || operation==unsigned(k::Operation::pin) || operation==unsigned(k::Operation::start_timer)?k::Callback::blur:k::Callback::focus;
        if(operation==unsigned(k::Operation::pin))f.gate=0;
        auto services=f.services();k::Result out{};
        require(k::execute(&f.state,callback,&f.globals,&services,&out)==k::Status::service_failed && out.last_operation==static_cast<k::Operation>(operation) && !out.complete && f.trace.size()==out.calls && f.source_actions==out.calls,"failure added cleanup/rolled back prefix");
        if(operation==unsigned(k::Operation::debug_query))require(f.string_live && !out.debug_destroyed,"failed query added destructor");
        if(operation>=unsigned(k::Operation::raise_event) && callback==k::Callback::focus)require(f.flags==0x6341,"failed source action rolled back flags");
        ++failures;
    }
    {
        Fixture f;auto services=f.services();k::Result out{};out.calls=77;auto before=out;
        auto unchanged=[&](k::Status status){require(status==k::Status::invalid_argument && !std::memcmp(&out,&before,sizeof(out)) && f.trace.empty(),"guard changed output/source");++guards;};
        unchanged(k::execute(nullptr,k::Callback::focus,&f.globals,&services,&out));unchanged(k::execute(&f.state,static_cast<k::Callback>(9),&f.globals,&services,&out));
        f.character.flags_528=&f.flags;unchanged(k::execute(&f.state,k::Callback::focus,&f.globals,&services,&out));f.character.flags_528=&f.gate;
        f.character.ooi_intent_412=reinterpret_cast<std::uint8_t*>(&out);unchanged(k::execute(&f.state,k::Callback::focus,&f.globals,&services,&out));f.character.ooi_intent_412=&f.ooi_intent;
        f.character.flags_520=reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uintptr_t>(&f.flags)+1);unchanged(k::execute(&f.state,k::Callback::focus,&f.globals,&services,&out));f.character.flags_520=&f.flags;
        f.character.physical_2dc=reinterpret_cast<const std::uintptr_t*>(UINTPTR_MAX-3);unchanged(k::execute(&f.state,k::Callback::focus,&f.globals,&services,&out));f.character.physical_2dc=&f.physical;
        f.state.character=nullptr;unchanged(k::execute(&f.state,k::Callback::focus,&f.globals,&services,&out));f.state.character=&f.character;
        f.character.identity=0;unchanged(k::execute(&f.state,k::Callback::focus,&f.globals,&services,&out));f.character.identity=C;
        f.globals.debug_switches=0;unchanged(k::execute(&f.state,k::Callback::focus,&f.globals,&services,&out));f.globals.debug_switches=D;
        require(k::execute(&f.state,k::Callback::focus,&f.globals,&services,reinterpret_cast<k::Result*>(&f.state))==k::Status::invalid_argument,"State/output alias accepted");++guards;
        require(k::execute(&f.state,k::Callback::focus,&f.globals,&services,reinterpret_cast<k::Result*>(&services))==k::Status::invalid_argument,"Services/output alias accepted");++guards;
        require(k::execute(&f.state,k::Callback::focus,&f.globals,&services,reinterpret_cast<k::Result*>(&f.character))==k::Status::invalid_argument,"Character/output alias accepted");++guards;
        services.invoke=nullptr;require(k::execute(&f.state,k::Callback::focus,&f.globals,&services,&out)==k::Status::service_unavailable && out.calls==0 && f.trace.empty(),"missing Debug service succeeded");++guards;
        services=f.services();f.zero_string=true;require(k::execute(&f.state,k::Callback::focus,&f.globals,&services,&out)==k::Status::invalid_source_fact && out.calls==2 && !out.debug_constructed && f.flags==0,"null source string advanced");++guards;
    }
    {
        Fixture f;f.nest=true;auto services=f.services();k::Result out{};
        require(k::execute(&f.state,k::Callback::focus,&f.globals,&services,&out)==k::Status::complete && f.nested_completed && f.flags==0x16341 && f.stops==1,"nested same-owner callback failed");++functional;
    }
    ActualDebugTimers actual(argv[1],std::filesystem::path(argv[2])/"source-debug");auto& f=actual.f;k::Services services{&actual,ActualDebugTimers::invoke};k::Result out{};
    f.moving=255;require(k::execute(&f.state,k::Callback::focus,&f.globals,&services,&out)==k::Status::complete && actual.coordinator.state.flags==0x16341 && actual.coordinator.state.attack_gate==0x100,"borrowed coordinator Focus storage differs");
    require(k::execute(&f.state,k::Callback::blur,&f.globals,&services,&out)==k::Status::complete && out.timer_attempted && actual.timer_id==0 && actual.coordinator.timers().count==1,"actual single timer owner failed");
    const auto& timer=actual.coordinator.timers().slots[0];require(timer.duration_ms==10 && timer.repeat==0 && timer.event==0x30 && !timer.user_ref && timer.active,"real source timer fields differ");
    require(actual.coordinator.update_timers(9,0)==1 && timer.elapsed_ms==9 && timer.active,"source timer duration replaced by animation timeout");
    require(actual.constructed==2 && actual.destroyed==2 && actual.strings.empty() && actual.debug.counters().read_opens==1 && actual.debug.runtime().switches().at("isTracingCharState")==0,"real Debug/string source effects absent");
    std::cout<<"{\"validation\":\"PASS\",\"functional_cases\":"<<functional<<",\"failure_cases\":"<<failures<<",\"guards\":"<<guards<<",\"real_debug_calls\":2,\"real_source_timers\":1,\"timer_duration_ms\":10,\"timer_before_expiry_ms\":9,\"same_coordinator\":true,\"full_skill_activation\":false}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
