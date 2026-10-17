#include "../character_ai_skill_dispatch_v1.hpp"
#include "../character_coordinator.hpp"
#include "../player_skill_use_session_v1.hpp"
#include "../player_hud_skill_slot_resolution_v1.hpp"
#define main retained_session_fixture_main
#include "player_skill_session_v1.cpp"
#undef main

namespace x=dh2::character_ai_skill_dispatch_v1;
namespace sq=dh2::character_skill_state_queries;
namespace u=dh2::player_skill_use_session_v1;
// Reused real constants fixture from the frozen use test; original Lua bytes
// and source stores are unchanged, and the selected shared VM is retained.
int dispatch_constants(void* raw,const dh2_script_value* args,std::uint32_t count,dh2_script_value* out,
                       std::uint32_t cap,std::uint32_t* returned,char*,std::size_t) noexcept {try{
    auto& f=*static_cast<Fixture*>(raw);check(count==2,"source constants arity");const auto category=text(args[0]),key=text(args[1]);
    const auto& input=category=="AIStates" || category.rfind("AITargetList_",0)==0?f.catalogue.ai_constants:f.catalogue.design;
    dh2_pycst_view view{};dh2_pycst_result value{};
    check(!dh2_pycst_open(&view,input.data(),std::uint32_t(input.size())) && !dh2_pycst_get(&view,category.data(),std::uint32_t(category.size()),key.data(),std::uint32_t(key.size()),&value)&&value.found,"actual constant missing");
    result(out,cap,returned,float(value.value));return 0;
}catch(...){return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}}
void prepare_dispatch(Fixture& f){
    f.common();p::source::Result result{};check(f.owner->prepare(&result)==p::source::Status::complete,"actual source preparation failed");
    check(!dh2_script_vm_bind_source_values(f.session->vm(),"GetPyCst",dispatch_constants,&f),"actual constant provider binding failed");
}
struct DispatchFixture {
    std::int32_t phase=7,id=3,alternate_id=7;
    std::uint32_t flags=0,alternate_flags=0x8000,current=0,level=0;
    sq::Machine machine{&id},alternate_machine{&alternate_id};
    x::Character character{0x100000001ull,&machine,&flags},alternate{0x200000001ull,&alternate_machine,&alternate_flags};
    std::uintptr_t slots[4]{0x300000001ull,0x300000002ull,0x300000003ull,0x300000004ull};
    std::uintptr_t alternate_slots[4]{0x400000001ull,0x400000002ull,0x400000003ull,0x400000004ull};
    x::Skills skills{slots,slots+1};
    x::State state{0x500000001ull,&character,&phase,&skills,&current,&level};
    std::uint32_t using_word=0,casting_word=0,tail=0,mutation=0;
    int failure=-1;bool throws=false,real_queries=false,nest=false;
    x::Result* alias_output=nullptr;x::Services* mutable_services=nullptr;
    std::vector<std::pair<unsigned,unsigned>> trace;
    static int invoke(void* raw,x::State* state,const x::Request* q,x::Response* r){
        auto& f=*static_cast<DispatchFixture*>(raw);check(state==&f.state,"dispatch receiver changed");
        unsigned who=q->subject==f.character.identity?1:q->subject==f.alternate.identity?2:q->subject==f.state.ai?9:
            q->subject==f.slots[0]?11:q->subject==f.slots[1]?12:q->subject==f.slots[2]?13:
            q->subject==f.alternate_slots[0]?21:q->subject==f.alternate_slots[1]?22:q->subject==f.alternate_slots[2]?23:99;
        f.trace.emplace_back(unsigned(q->service),who);
        if(q->service==x::Service::using_skill){
            r->word=f.using_word;
            if(f.real_queries){sq::Result result{};check(sq::is_using_skill(q->machine,&result)==sq::Status::complete,"UsingSkill provider failed");r->word=result.value;}
            if(f.mutation==1)state->character=&f.alternate;
            if(f.mutation==5)state->character=nullptr;
            if(f.mutation==7)f.flags=0;
            if(f.mutation==9)state->load_phase_28=reinterpret_cast<const std::int32_t*>(f.alias_output);
            if(f.mutable_services)f.mutable_services->invoke=nullptr;
        }else if(q->service==x::Service::casting){
            r->word=f.casting_word;
            if(f.real_queries){sq::Result result{};check(sq::is_casting(q->machine,&result)==sq::Status::complete,"Casting provider failed");r->word=result.value;}
            if(f.mutation==2)f.phase=7;
            if(f.mutation==6)f.phase=6;
        }else if(q->service==x::Service::assertion_log){
            check(q->line==181 && std::string(q->format)=="ASSERT(%s) FAILED: %s:%d\n" &&
                  std::string(q->expression)=="skillId < m_skillScripts.size()" &&
                  std::string(q->filename)=="..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Characters\\AI\\CharAI_Skills.cpp","source assertion args changed");
            if(f.mutation==3)f.skills={f.alternate_slots,f.alternate_slots+3};
            if(f.mutation==8){f.alternate_slots[2]=0;f.skills={f.alternate_slots,f.alternate_slots+3};}
        }else {
            check(!q->machine,"skill tail gained a machine argument");r->word=f.tail;
            if(f.mutation==4){f.current=3;f.skills={f.alternate_slots,f.alternate_slots+4};}
            if(f.nest){f.nest=false;x::Result nested{};check(x::execute(state,x::Operation::loaded,0,nullptr,&nested)==x::Status::complete && nested.value==std::uint32_t(f.phase>6),"independent-output callback reentry failed");}
        }
        if(f.failure==int(q->service)){if(f.throws)throw std::runtime_error("provider failed after source effect");return -1;}
        return 0;
    }
    x::Services services(){return {this,invoke};}
};
std::uint32_t number(const char* s){return std::uint32_t(std::stoull(s));}
int dispatch_oracle(int argc,char** argv){
    check(argc==13,"dispatch oracle arg count");DispatchFixture f;
    const auto operation=number(argv[2]);std::uint32_t bits=number(argv[3]);std::memcpy(&f.phase,&bits,4);
    f.using_word=number(argv[4]);f.casting_word=number(argv[5]);f.flags=number(argv[6]);f.current=number(argv[7]);
    auto count=number(argv[8]);check(count<=4,"fixture table count");f.skills.end=f.slots+count;
    if(number(argv[9]))for(auto& slot:f.slots)slot=0;
    f.level=number(argv[10]);f.mutation=number(argv[11]);f.tail=number(argv[12]);auto services=f.services();x::Result r{};
    const auto status=x::execute(&f.state,static_cast<x::Operation>(operation),f.current,&services,&r);
    std::cout<<"{\"status\":"<<int(status)<<",\"value\":"<<r.value<<",\"trace\":[";
    for(std::size_t i=0;i<f.trace.size();++i){if(i)std::cout<<',';std::cout<<'['<<f.trace[i].first<<','<<f.trace[i].second<<']';}
    std::cout<<"]}\n";return 0;
}
struct RealDispatch {
    u::Runtime& runtime;p::Owner& owner;unsigned calls=0;std::string error;
    static int invoke(void* raw,x::State*,const x::Request* q,x::Response* r){
        auto& f=*static_cast<RealDispatch*>(raw);++f.calls;
        if(q->service==x::Service::using_skill || q->service==x::Service::casting){
            if(q->subject!=CHAR)return -1;
            sq::Result result{};
            auto kind=q->service==x::Service::using_skill?sq::Query::using_skill:sq::Query::casting;
            if(sq::query(kind,q->machine,&result)!=sq::Status::complete)return -1;
            r->word=result.value;return 0;
        }
        const auto& slots=f.owner.slots(u::List::skill);
        if(q->slot>=slots.size() || slots[q->slot]!=q->subject)return -1;
        u::Result out{};int status;
        if(q->service==x::Service::check_usable)status=f.runtime.check(u::List::skill,q->slot,u::Check::usable,out,f.error);
        else if(q->service==x::Service::pre || q->service==x::Service::use || q->service==x::Service::post){
            auto callback=q->service==x::Service::pre?u::Callback::pre:q->service==x::Service::use?u::Callback::use:u::Callback::post;
            status=f.runtime.invoke(u::List::skill,q->slot,callback,out,f.error);
        }else return -1;
        r->word=out.value;return status;
    }
};
void hud_skill_slot_resolution_regression(){
    using namespace dh2::player_hud_skill_slot_resolution_v1;
    constexpr std::uintptr_t character=0x600000001ull;
    dh2::data::SkillTables tables;
    tables.skill_lists.resize(2);
    tables.skill_lists[0].members={4,5,6};
    tables.skill_lists[1].members={17,23,29};
    tables.skills.resize(30);
    tables.skills[23].table_name="SelectedSkill23";
    dh2::data::PlayerSavegameV1 save;
    save.set_character(character);
    std::string error;
    check(save.initialize_skills_from_character_list(tables.skill_lists[1].members,error),
          "HUD resolver Save rows failed to initialize");
    const dh2::data::SavedSkillUpdateServicesV1 update{
        nullptr,[](void*,std::uintptr_t owner,std::string&){return owner==character;}};
    check(save.set_skill_in_slot(2,1,update,error),"HUD resolver Save slot failed to initialize");
    const std::vector<std::uintptr_t> scripts{0x710000001ull,0x710000002ull,0x710000003ull};
    Result resolved{};
    check(resolve(save,character,2,1,tables,&scripts,&resolved)==Status::complete&&
          resolved.hud_slot==2&&resolved.skill_index==1&&resolved.skill_id==23&&
          resolved.skill_row==&tables.skills[23]&&resolved.script_identity==scripts[1],
          "HUD argument did not resolve through Save row to the matching selected skill/script");

    const auto unchanged=resolved;
    check(resolve(save,character,1,1,tables,&scripts,&resolved)==Status::empty_hud_slot&&
          resolved.script_identity==unchanged.script_identity,
          "unassigned HUD slot fabricated a skill or changed output");
    check(resolve(save,character,2,0,tables,&scripts,&resolved)==Status::missing_skill_row&&
          resolved.script_identity==unchanged.script_identity,
          "mismatched selected SkillList/Save row was accepted");
    check(resolve(save,character,2,2,tables,&scripts,&resolved)==Status::invalid_source_fact,
          "invalid explicit source SkillList selector was accepted");
    check(resolve(save,character+1,2,1,tables,&scripts,&resolved)==Status::invalid_source_fact,
          "different Character Save was accepted");
    check(resolve(save,character,2,1,tables,nullptr,&resolved)==Status::missing_prepared_script,
          "missing prepared vector fabricated a script");
    const std::vector<std::uintptr_t> short_scripts{scripts[0]};
    check(resolve(save,character,2,1,tables,&short_scripts,&resolved)==Status::missing_prepared_script,
          "short prepared vector was indexed out of range");
    auto null_scripts=scripts;null_scripts[1]=0;
    check(resolve(save,character,2,1,tables,&null_scripts,&resolved)==Status::missing_prepared_script,
          "null prepared script was accepted");
    check(resolve(save,character,-1,1,tables,&scripts,&resolved)==Status::invalid_argument,
          "negative NativeHUDSkill argument was accepted");
}
int main(int argc,char** argv){try{
    if(argc>1 && std::string(argv[1])=="--oracle")return dispatch_oracle(argc,argv);
    hud_skill_slot_resolution_regression();
    check(argc==3,"cache/debug args required");unsigned functional=0,guards=0,failures=0,actual=0;
    for(auto id:{-1,0,3,6,7,42})for(auto phase:{-1,0,5,6,7,8})for(auto flags:{0u,0x8000u}){
        DispatchFixture f;f.id=id;f.phase=phase;f.flags=flags;f.tail=0xdeadbeef;f.real_queries=true;auto services=f.services();x::Result r{};
        check(x::execute(&f.state,x::Operation::usable,0,&services,&r)==x::Status::complete,"functional query failed");
        const bool reaches=(id!=6 || (flags&0x8000)) && id!=7 && phase>6;
        check(r.value==(reaches?f.tail:0),"borrowed real FSM/phase predicate differs");++functional;
    }
    for(auto op:{x::Operation::focus,x::Operation::event,x::Operation::blur}){
        DispatchFixture f;f.phase=0;f.tail=99;f.mutation=4;f.nest=true;auto services=f.services();x::Result r{};
        check(x::execute(&f.state,op,0,&services,&r)==x::Status::complete && r.value==0 && r.skill==f.slots[0] && f.trace.size()==1 && f.current==3,
              "dispatcher invented phase gate/replayed changed slot/kept ignored return");++functional;
    }
    for(auto op:{x::Service::using_skill,x::Service::casting,x::Service::check_usable,x::Service::assertion_log,x::Service::pre,x::Service::use,x::Service::post})for(bool throws:{false,true}){
        DispatchFixture f;f.failure=int(op);f.throws=throws;f.mutation=4;
        x::Operation operation=op==x::Service::pre?x::Operation::focus:op==x::Service::use?x::Operation::event:op==x::Service::post?x::Operation::blur:x::Operation::usable;
        if(op==x::Service::assertion_log){f.level=1;f.skills.end=f.slots;f.mutation=3;}
        auto services=f.services();x::Result r{};check(x::execute(&f.state,operation,0,&services,&r)==x::Status::service_failed && r.last_service==op && !f.trace.empty(),"failure skipped stop/prefix");
        if(op==x::Service::assertion_log)check(f.skills.begin==f.alternate_slots,"failed log mutation rolled back");
        if(op==x::Service::pre || op==x::Service::use || op==x::Service::post)check(f.current==3,"failed tail mutation rolled back");
        ++failures;
    }
    {
        DispatchFixture f;auto services=f.services();x::Result r{};
        f.mutable_services=&services;f.tail=17;
        check(x::execute(&f.state,x::Operation::usable,0,&services,&r)==x::Status::complete && r.value==17 && r.service_calls==3 && !services.invoke,"service binding snapshot changed midcall");++functional;
        f.mutable_services=nullptr;services=f.services();f.mutation=5;
        check(x::execute(&f.state,x::Operation::usable,0,&services,&r)==x::Status::invalid_source_fact && r.service_calls==1 && !f.state.character,"missing fresh owner did not preserve failure prefix");++failures;
        f.state.character=&f.character;f.mutation=9;f.alias_output=&r;
        check(x::execute(&f.state,x::Operation::usable,0,&services,&r)==x::Status::invalid_source_fact && r.service_calls==1 && f.state.load_phase_28==reinterpret_cast<const std::int32_t*>(&r),"callback alias accepted or rolled back");++failures;
    }
    {
        DispatchFixture f;auto services=f.services();x::Result r{};r.value=99;auto before=r;
        auto unchanged=[&](x::Status status){check(status==x::Status::invalid_argument && !std::memcmp(&r,&before,sizeof(r)),"guard touched output");++guards;};
        unchanged(x::execute(nullptr,x::Operation::usable,0,&services,&r));
        unchanged(x::execute(&f.state,static_cast<x::Operation>(99),0,&services,&r));
        alignas(x::Result) unsigned char misaligned[sizeof(x::Result)+alignof(x::Result)]{};
        auto initial=std::vector<unsigned char>(std::begin(misaligned),std::end(misaligned));
        check(x::execute(&f.state,x::Operation::usable,0,&services,reinterpret_cast<x::Result*>(misaligned+1))==x::Status::invalid_argument && std::equal(initial.begin(),initial.end(),std::begin(misaligned)),"misaligned output changed bytes");++guards;
        f.state.ai=0;unchanged(x::execute(&f.state,x::Operation::usable,0,&services,&r));f.state.ai=0x500000001ull;
        check(x::execute(&f.state,x::Operation::usable,0,&services,reinterpret_cast<x::Result*>(&f.state))==x::Status::invalid_argument,"output/state alias");++guards;
        check(x::execute(&f.state,x::Operation::usable,0,&services,reinterpret_cast<x::Result*>(&services))==x::Status::invalid_argument,"output/services alias");++guards;
        f.state.load_phase_28=reinterpret_cast<const std::int32_t*>(&r);unchanged(x::execute(&f.state,x::Operation::loaded,0,nullptr,&r));f.state.load_phase_28=&f.phase;
        f.state.character=reinterpret_cast<x::Character*>(reinterpret_cast<std::uintptr_t>(&f.character)+1);unchanged(x::execute(&f.state,x::Operation::usable,0,&services,&r));f.state.character=&f.character;
        f.skills.end=f.slots+1;f.skills.begin=reinterpret_cast<const std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(f.slots)+1);unchanged(x::execute(&f.state,x::Operation::usable,0,&services,&r));f.skills.begin=f.slots;
        f.skills.end=f.slots-0;f.skills.begin=f.slots+1;unchanged(x::execute(&f.state,x::Operation::usable,0,&services,&r));f.skills={f.slots,f.slots+1};
        f.state.current_slot_cc=reinterpret_cast<std::uint32_t*>(UINTPTR_MAX-1);unchanged(x::execute(&f.state,x::Operation::focus,0,&services,&r));f.state.current_slot_cc=&f.current;
        services.invoke=nullptr;check(x::execute(&f.state,x::Operation::usable,0,&services,&r)==x::Status::service_unavailable && r.service_calls==0,"missing query success");++guards;
        f.level=2;f.skills.end=f.slots;services=f.services();check(x::execute(&f.state,x::Operation::usable,0,&services,&r)==x::Status::unsupported_source_assertion && r.decision==x::Decision::fatal_assertion && f.trace.size()==2,"assert trap fabricated success");++guards;
        f.level=0;f.state.character=nullptr;check(x::execute(&f.state,x::Operation::focus,0,&services,&r)==x::Status::complete && r.decision==x::Decision::out_of_range,"dispatcher gained owner dereference");++guards;
        f.state.load_phase_28=nullptr;check(x::execute(&f.state,x::Operation::loaded,0,nullptr,&r)==x::Status::invalid_source_fact,"missing phase guessed");++guards;
    }
    Catalogue cat(argv[1]);Fixture f(argv[1],std::filesystem::path(argv[2])/"actual",cat,"KnightPlayerBase");prepare_dispatch(f);
    u::Runtime use(*f.session,*f.owner,CHAR);dh2::character::Coordinator coordinator(CHAR);coordinator.state.current=3;
    sq::Machine machine{&coordinator.state.current};x::Character character{CHAR,&machine,&coordinator.state.flags};
    const auto& slots=f.owner->slots(u::List::skill);x::Skills skills{slots.data(),slots.data()+slots.size()};
    std::int32_t phase=6;std::uint32_t current=0,level=0;x::State state{0x700000001ull,&character,&phase,&skills,&current,&level};
    RealDispatch bridge{use,*f.owner,0,{}};x::Services services{&bridge,RealDispatch::invoke};x::Result r{};
    const auto vm=f.session->vm();auto before=f.native_names.size();
    check(x::execute(&state,x::Operation::usable,0,&services,&r)==x::Status::complete && !r.value && r.decision==x::Decision::not_loaded && f.native_names.size()==before,"loaded Lua guessed source phase7");++actual;
    phase=7;check(x::execute(&state,x::Operation::usable,7,&services,&r)==x::Status::complete && !r.value && r.decision==x::Decision::dispatched,"actual passive usable differs");++actual;
    check(x::execute(&state,x::Operation::usable,0,&services,&r)==x::Status::service_failed && f.native_names.back()=="HasMana","unbound real mana check accepted");++actual;
    phase=0;check(x::execute(&state,x::Operation::event,0,&services,&r)==x::Status::complete && r.decision==x::Decision::dispatched && !r.value,"source nil-target use tail gained loaded gate");++actual;
    check(x::execute(&state,x::Operation::focus,0,&services,&r)==x::Status::service_failed && f.native_names.back()=="SetTargetListCharacterFilter","unbound actual target search accepted");++actual;
    current=15;check(x::execute(&state,x::Operation::blur,0,&services,&r)==x::Status::complete && r.decision==x::Decision::null_skill,"null source slot dispatched");++actual;
    check(f.session->vm()==vm,"dispatch replaced retained VM");f.session.reset();
    std::cout<<"{\"validation\":\"PASS\",\"functional_cases\":"<<functional<<",\"actual_script_cases\":"<<actual<<",\"guards\":"<<guards<<",\"failure_cases\":"<<failures<<",\"Player_AIS_phase_published\":false,\"CSSkill_activation\":false}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
