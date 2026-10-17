#include "../player_skill_use_session_v1.hpp"
#include "../player_skill_property_services_v1.hpp"
#include "../character_clear_target_v1.hpp"
#include "../character_skill_cooldown_services.hpp"
#include "../player_skill_timer_provider_v1.hpp"
#include "../player_script_timer_dispatch_v1.hpp"
#include "../character_player_buffs_v1.hpp"
#include "../character_coordinator.hpp"
#define main retained_session_fixture_main
#include "player_skill_session_v1.cpp"
#undef main

namespace u=dh2::player_skill_use_session_v1;
namespace nb=dh2::character_native_bindings;
namespace timer_dispatch=dh2::player_script_timer_dispatch_v1;
namespace buff_owner=dh2::character_player_buffs_v1;
struct SkillCombatRollProbe {unsigned calls=0;std::uint32_t slot=0;std::uintptr_t target=0;};
void test_player_skill_timer_provider() {
    using namespace dh2;
    constexpr std::uintptr_t owner_id=0x100000001ull;
    character::Coordinator coordinator(owner_id,2);
    character::CoordinatorBindings coordinator_bindings{};
    coordinator_bindings.facts=[](void*){return character::Facts{};};
    coordinator_bindings.services={nullptr,[](void*,character::State*,const character::Request*){}};
    coordinator.bind(coordinator_bindings);
    player_skill_timer_provider_v1::Bindings provider{};
    check(player_skill_timer_provider_v1::bind(&provider,&coordinator,owner_id),
          "Player timer provider rejected its canonical Character Coordinator");

    dh2_script_value args[2]{};args[0].type=DH2_SCRIPT_NUMBER;args[0].number=12.75f;
    args[1].type=DH2_SCRIPT_BOOLEAN;args[1].boolean=1;
    dh2_script_value result{};std::uint32_t returned=99;char error[128]{};
    check(player_skill_timer_provider_v1::start(&provider,args,2,&result,1,&returned,
          error,sizeof(error))==0&&returned==1&&result.type==DH2_SCRIPT_NUMBER&&
          result.number==0.0f,"Player StartTimer failed through the shared Coordinator");
    const auto& timer=coordinator.timers().slots[0];
    check(timer.active&&timer.duration_ms==12&&timer.repeat==-1&&timer.event==0x35&&
          timer.user_ref==0,"StartTimer did not preserve source duration/repeat/event/ref");

    dh2_script_value timer_id{};timer_id.type=DH2_SCRIPT_NUMBER;timer_id.number=0;
    returned=99;
    check(player_skill_timer_provider_v1::stop(&provider,&timer_id,1,nullptr,0,&returned,
          error,sizeof(error))==0&&returned==0&&!coordinator.timers().slots[0].active,
          "Player StopTimer did not stop the same Coordinator timer");

    player_skill_timer_provider_v1::Bindings unbound{};
    character::Coordinator unavailable(owner_id,2);
    check(player_skill_timer_provider_v1::bind(&unbound,&unavailable,owner_id),
          "unbound Coordinator identity fixture rejected");
    returned=99;error[0]=0;
    check(player_skill_timer_provider_v1::start(&unbound,args,2,&result,1,&returned,
          error,sizeof(error))==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE&&returned==0&&error[0],
          "unavailable timer owner did not fail closed without throwing");
}

struct ScriptTimerExpiryFixture {
    static constexpr std::uintptr_t character=0x100000001ull;
    static constexpr std::uintptr_t ai_identity=0x200000001ull;
    static constexpr std::uintptr_t ais_identity=0x300000001ull;
    dh2::character::Coordinator coordinator{character,4};
    dh2::character_ai_initialization::State ai{};
    dh2_script_vm* vm=nullptr;
    timer_dispatch::Bindings dispatch{};
    unsigned delivered=0;
    ScriptTimerExpiryFixture() {
        ai.identity=ai_identity;ai.owner_04=character;ai.active_ais_1c=ais_identity;
        vm=dh2_script_vm_create(1024*1024);check(vm!=nullptr,"timer VM allocation failed");
        const char* source="hits=0;last=-1;function OnTimer(id) hits=hits+1;last=id end";
        check(dh2_script_vm_load(vm,source,std::strlen(source),"@timer-dispatch") == 0,
              "timer VM script load failed");
        dispatch={character,ai_identity,ais_identity,0x400000001ull,
                  0x500000001ull,&ai,&coordinator,vm};
        dh2::character::CoordinatorBindings bindings{};
        bindings.context=this;
        bindings.facts=[](void*){return dh2::character::Facts{};};
        bindings.services={nullptr,[](void*,dh2::character::State*,const dh2::character::Request*){}};
        bindings.before_timer_event=before;
        coordinator.bind(bindings);
    }
    ~ScriptTimerExpiryFixture(){if(vm)dh2_script_vm_destroy(vm);}
    static void before(void* raw,dh2::character::Coordinator& owner,std::int32_t event,
                       dh2::character::Timer32& timer,std::uint32_t) {
        auto& f=*static_cast<ScriptTimerExpiryFixture*>(raw);
        if(&owner!=&f.coordinator||event!=0x35)throw std::runtime_error("wrong timer owner/event");
        std::string error;
        const auto status=timer_dispatch::dispatch(
            f.dispatch,timer.id,error);
        if(status==timer_dispatch::Status::delivered){++f.delivered;return;}
        if(status==timer_dispatch::Status::inactive_ais)return;
        throw std::runtime_error(error.empty()?"timer dispatch rejected":error);
    }
    float global(const char* name){
        dh2_script_value value{};
        check(dh2_script_vm_get_global(vm,name,&value)==0&&value.type==DH2_SCRIPT_NUMBER,
              "timer callback global unavailable");
        return value.number;
    }
};

void test_player_script_timer_expiry_dispatch() {
    ScriptTimerExpiryFixture fixture;
    check(fixture.coordinator.start_timer(5,0,0x35,0)==0,
          "timer expiry fixture allocation failed");
    check(fixture.coordinator.update_timers(5,0)==1&&fixture.delivered==1&&
          fixture.global("hits")==1&&fixture.global("last")==0,
          "Coordinator event-0x35 expiry missed retained AIS OnTimer");

    fixture.ai.active_ais_1c=0;
    check(fixture.coordinator.start_timer(5,0,0x35,0)==0&&
          fixture.coordinator.update_timers(5,0)==1&&fixture.delivered==1&&
          fixture.global("hits")==1,
          "CharAI inactive-AIS gate did not suppress OnTimer");

    fixture.ai.active_ais_1c=ScriptTimerExpiryFixture::ais_identity+1;
    std::string error;
    check(timer_dispatch::dispatch(fixture.dispatch,0,error)==
          timer_dispatch::Status::unavailable&&
          error.find("active AIS")!=std::string::npos,
          "foreign active AIS was dispatched into the retained Player VM");
}

int probe_skill_combat_roll(void* raw,const dh2_script_value* args,std::uint32_t count,
    dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,char*,std::size_t) noexcept {
    auto& probe=*static_cast<SkillCombatRollProbe*>(raw);
    if(!args||count!=2||!out||!capacity||!returned||
       args[0].type!=DH2_SCRIPT_NUMBER||args[0].number!=42.0f||
       args[1].type!=DH2_SCRIPT_IDENTITY||args[1].identity!=0x123456789abcdef0ull)
        return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
    ++probe.calls;probe.slot=std::uint32_t(args[0].number);probe.target=args[1].identity;
    out[0]={};out[0].type=DH2_SCRIPT_NUMBER;out[0].number=73.0f;*returned=1;return 0;
}
struct BashdownSourceProbe {std::vector<std::string> events;std::uintptr_t target=0x9988776655443322ull;std::uint32_t skill_id=0;};
struct BashdownNativeBinding {BashdownSourceProbe* probe;const char* name;};
int bashdown_source_native(void* raw,const dh2_script_value* args,std::uint32_t count,
    dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,char*,std::size_t) noexcept {
    try {
        auto& binding=*static_cast<BashdownNativeBinding*>(raw);auto& probe=*binding.probe;
        if(!returned||(count&&!args))return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
        *returned=0;probe.events.emplace_back(binding.name);
        const std::string name=binding.name;
        if(name=="TargetListSearch")return count==1&&args[0].type==DH2_SCRIPT_NUMBER&&args[0].number==160.0f?0:DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
        if(name=="GetCurrentSkillInfo__"){
            if(count!=1||args[0].type!=DH2_SCRIPT_NUMBER||!out||!capacity)return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
            probe.skill_id=std::uint32_t(args[0].number);out[0]={};out[0].type=DH2_SCRIPT_NUMBER;out[0].number=2.0f;*returned=1;return 0;
        }
        if(name=="GetProp"){
            if(!out||!capacity)return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
            out[0]={};out[0].type=DH2_SCRIPT_NUMBER;out[0].number=5.0f;*returned=1;return 0;
        }
        if(name=="UseMana")return count==1&&args[0].type==DH2_SCRIPT_NUMBER&&args[0].number==5.0f?0:DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
        if(name=="IsTargetListEmpty"){
            if(!out||!capacity)return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
            out[0]={};out[0].type=DH2_SCRIPT_BOOLEAN;out[0].boolean=0;*returned=1;return 0;
        }
        if(name=="GetTargetListTop"){
            if(!out||!capacity)return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
            out[0]={};out[0].type=DH2_SCRIPT_IDENTITY;out[0].identity=probe.target;*returned=1;return 0;
        }
        if(name=="SkillCombatRoll__"){
            if(count!=2||args[0].type!=DH2_SCRIPT_NUMBER||args[1].type!=DH2_SCRIPT_IDENTITY||args[1].identity!=probe.target||!out||!capacity)
                return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
            probe.skill_id=std::uint32_t(args[0].number);out[0]={};out[0].type=DH2_SCRIPT_NUMBER;out[0].number=73.0f;*returned=1;return 0;
        }
        if(name=="LookAt" && (count!=1||args[0].type!=DH2_SCRIPT_IDENTITY||args[0].identity!=probe.target))return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
        return 0;
    }catch(...){return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
}
void fixture_load(Fixture&,const std::string&);
void test_bashdown_callback_order(Fixture& f) {
    static const char* names[]={"GetCurrentSkillInfo__","SetProp","ClearProps","ApplyPropClass","GetProp","SetTargetListCharacterFilter","SetTargetListObjectFilter","SetTargetListSorting","TargetListSearch","IsTargetListEmpty","GetTargetListTop","LookAt","UseMana","SetProp","ApplyPropClass","SkillCombatRoll__","ClearTarget"};
    BashdownSourceProbe probe{};BashdownNativeBinding binding[sizeof(names)/sizeof(names[0])]{};
    for(std::size_t i=0;i<sizeof(names)/sizeof(names[0]);++i){binding[i]={&probe,names[i]};check(!dh2_script_vm_bind_source_values(f.session->vm(),names[i],bashdown_source_native,&binding[i]),"Bashdown native binding override failed");}
    auto load_overlay=[&](const char* path,const std::string& source){s::LoadResult loaded{};std::string error;f.overlay(path,source);check(!f.session->load_resolved(path,&loaded,error)&&loaded.source_success,"Bashdown source fixture failed to load");};
    const auto common_bytes=read(f.cache/"data/scripts/skills/_commons.luac");
    load_overlay("fixture/use/bashdown-canonical-commons",std::string(common_bytes.begin(),common_bytes.end()));
    const auto bashdown_bytes=read(f.cache/"data/scripts/skills/prince_warrior_bashdown.luac");
    load_overlay("fixture/use/bashdown-canonical-skill","DeclareSkill('prince_warrior_bashdown',42);"+std::string(bashdown_bytes.begin(),bashdown_bytes.end()));
    fixture_load(f,"function ActivateBashdown() SetSkill('prince_warrior_bashdown') end");
    Return call{};
    auto call_source=[&](const char* name){std::string error;const int status=f.session->call(name,nullptr,0,0,observe,&call,error);if(status){for(const auto& event:probe.events)std::cerr<<event<<" ";std::cerr<<std::endl;throw std::runtime_error(std::string(name)+": "+error);}};
    call_source("ActivateBashdown");call_source("OnSkillUpdate");call_source("OnPreSkill");call_source("OnSkill");call_source("OnPostSkill");
    const std::vector<std::string> expected={"GetCurrentSkillInfo__","SetProp","ClearProps","SetProp","ApplyPropClass","GetProp","SetTargetListCharacterFilter","SetTargetListObjectFilter","SetTargetListSorting","TargetListSearch","IsTargetListEmpty","GetTargetListTop","LookAt","UseMana","SetProp","ApplyPropClass","SkillCombatRoll__","ClearTarget"};
    if(probe.events!=expected||probe.skill_id==0){for(const auto& event:probe.events)std::cerr<<event<<" ";std::cerr<<" skill="<<probe.skill_id<<std::endl;throw std::runtime_error("Bashdown source callback ordering or SkillCombatRoll__ injection differs");}
}
struct GroundSlamProbe {
    std::vector<std::string> events;
    std::uintptr_t target=0xaabbccddeeff0011ull;
    std::uint32_t target_count=1;
    unsigned get_prop_calls=0,combat_rolls=0;
    float mana_cost=0,search_radius=0,damage_probe=0;
    std::int32_t cooldown_index=-1;
    std::int32_t mana_cost_property_id=-1;
    std::int32_t range_property_id=-1;
    dh2::player_skill_property_services_v1::Bindings* properties=nullptr;
    dh2::character_player_skills_preparation_v3::Owner* preparation=nullptr;
    dh2::character_player_skills_preparation_v3::Owner::TimerFieldLease* timer_fields=nullptr;
    dh2::character::Coordinator* coordinator=nullptr;
    dh2::player_skill_timer_provider_v1::Bindings timer_bindings{};
    dh2::character_skill_cooldown_services::Services cooldown{};
    timer_dispatch::Bindings timer_dispatch_bindings{};
    unsigned timer_expiries=0;
    dh2::character_ai_initialization::State source_ai{};
    dh2::character::set_target::OwnerFacts target_owner{};
    unsigned clear_target_calls=0,target_debug_calls=0;
};
void ground_slam_timer_expired(void* raw,dh2::character::Coordinator& coordinator,
    std::int32_t event,dh2::character::Timer32& timer,std::uint32_t) {
    auto& probe=*static_cast<GroundSlamProbe*>(raw);
    if(&coordinator!=probe.coordinator||event!=0x35)
        throw std::runtime_error("Ground Slam timer routed from the wrong Character/event");
    std::string error;
    const auto status=timer_dispatch::dispatch(probe.timer_dispatch_bindings,timer.id,error);
    if(status!=timer_dispatch::Status::delivered)
        throw std::runtime_error(error.empty()?"Ground Slam OnTimer was not delivered":error);
    ++probe.timer_expiries;
}
std::int32_t ground_slam_skill_list_count(void* raw,std::uintptr_t character,
    std::uint32_t kind,std::uint32_t* count) {
    auto& probe=*static_cast<GroundSlamProbe*>(raw);
    if(!count||!probe.preparation||character!=CHAR||kind!=0)return 1;
    const auto& vector=probe.preparation->state().skills;
    if(!vector.begin||!vector.end||vector.end<vector.begin||
       std::size_t(vector.end-vector.begin)>UINT32_MAX)return 1;
    *count=std::uint32_t(vector.end-vector.begin);return 0;
}
std::int32_t ground_slam_skill_slot(void* raw,std::uintptr_t character,
    std::uint32_t kind,std::uint32_t index,
    dh2::character_skill_cooldown_services::Slot* slot) {
    auto& probe=*static_cast<GroundSlamProbe*>(raw);
    if(!slot||!probe.timer_fields||character!=CHAR||kind!=0)return 1;
    dh2::character_player_skills_preparation_v3::Owner::TimerFieldSlot found{};
    if(!probe.timer_fields->slot(character,
          dh2::character_ai_set_skills_and_spells::List::skill,index,found))return 1;
    *slot={found.instance,found.field18};return 0;
}
std::int32_t ground_slam_unsupported_number(void*,const dh2_script_value*,float*) {
    return 1;
}
int clear_target_debug(void* raw,const dh2::character::set_target::Request* request,
                       dh2::character::set_target::Response* response) {
    auto& probe=*static_cast<GroundSlamProbe*>(raw);
    using namespace dh2::character::set_target;
    if(!request||!response||request->ai_identity!=probe.source_ai.identity||
       request->owner_identity||request->target_identity)return 1;
    ++probe.target_debug_calls;*response={};
    if(request->operation==debug_switches_load)return 0;
    if(request->operation==debug_switch_lookup&&request->key==trace_target_changes)return 0;
    return 1;
}
struct GroundSlamBinding {GroundSlamProbe* probe;const char* name;};
int ground_slam_typed_native(void* raw,const dh2_script_value* args,std::uint32_t count,
    dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,
    char*,std::size_t) noexcept {
    try {
        auto& binding=*static_cast<GroundSlamBinding*>(raw);auto& probe=*binding.probe;
        const std::string name=binding.name;
        if(!returned||(count&&!args))return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
        *returned=0;probe.events.push_back(name);
        if(name=="GetCurrentSkillInfo__"){
            if(count!=1||args[0].type!=DH2_SCRIPT_NUMBER||!out||!capacity)
                return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
            out[0]={};out[0].type=DH2_SCRIPT_NUMBER;out[0].number=2.0f;*returned=1;return 0;
        }
        if(name=="GetProp"){
            if(!probe.properties)return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
            ++probe.get_prop_calls;
            const auto status=dh2::player_skill_property_services_v1::invoke(probe.properties,CHAR,
                dh2::character_native_bindings::Function::character_get_prop,args,count,
                out,capacity,returned,nullptr,0);
            if(!status&&count&&args[0].type==DH2_SCRIPT_NUMBER&&
               std::int32_t(args[0].number)==probe.mana_cost_property_id&&*returned==1&&
               out[0].type==DH2_SCRIPT_NUMBER)probe.mana_cost=out[0].number;
            return status;
        }
        if(name=="IsTargetListEmpty"){
            if(!out||!capacity)return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
            out[0]={};out[0].type=DH2_SCRIPT_BOOLEAN;out[0].boolean=probe.target_count==0;*returned=1;return 0;
        }
        if(name=="GetTargetListTop"){
            if(!probe.target_count||!out||!capacity)return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
            out[0]={};out[0].type=DH2_SCRIPT_IDENTITY;out[0].identity=probe.target;*returned=1;return 0;
        }
        if(name=="TargetListSearch"){
            if(count!=1||args[0].type!=DH2_SCRIPT_NUMBER||!std::isfinite(args[0].number)||args[0].number<0)
                return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
            probe.search_radius=args[0].number;probe.target_count=1;return 0;
        }
        if(name=="PopTargetList"){
            if(count||!probe.target_count)return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
            --probe.target_count;return 0;
        }
        if(name=="UseMana"){
            if(count!=1||args[0].type!=DH2_SCRIPT_NUMBER||args[0].number!=probe.mana_cost)
                return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
            return 0; // typed debit probe only; no production mana mutation
        }
        if(name=="StartTimer"){
            return dh2::player_skill_timer_provider_v1::start(
                &probe.timer_bindings,args,count,out,capacity,returned,nullptr,0);
        }
        if(name=="SetSkillCooldownTimerId__"){
            if(count&&args&&args[0].type==DH2_SCRIPT_NUMBER&&std::isfinite(args[0].number))
                probe.cooldown_index=std::int32_t(args[0].number);
            return dh2::character_skill_cooldown_services::skill(
                &probe.cooldown,args,count,out,capacity,returned,nullptr,0);
        }
        if(name=="SkillCombatRoll__"){
            if(count!=2||args[0].type!=DH2_SCRIPT_NUMBER||args[1].type!=DH2_SCRIPT_IDENTITY||
               args[1].identity!=probe.target)return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
            probe.damage_probe=13.0f;++probe.combat_rolls;return 0; // typed combat probe only
        }
        if(name=="ClearProps"||name=="SetProp"||name=="ApplyPropClass"){
            if(!probe.properties)return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
            const auto function=name=="ClearProps"?dh2::character_native_bindings::Function::character_clear_props:
                name=="SetProp"?dh2::character_native_bindings::Function::character_set_prop:
                dh2::character_native_bindings::Function::character_apply_prop_class;
            return dh2::player_skill_property_services_v1::invoke(probe.properties,CHAR,function,
                args,count,out,capacity,returned,nullptr,0);
        }
        if(name=="ClearTarget"){
            ++probe.clear_target_calls;
            const dh2::character::set_target::Services services{
                &probe,0,clear_target_debug};
            const auto status=dh2::character_clear_target_v1::clear(
                &probe.source_ai,&probe.target_owner,CHAR,&services);
            return status==dh2::character_clear_target_v1::Status::complete
                ?0:DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
        }
        if(name=="SetTargetListCharacterFilter"||name=="SetTargetListObjectFilter"||
           name=="SetTargetListSorting"||name=="LookAt")
            return 0; // explicit typed orchestration probes, not live gameplay providers
        return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
    }catch(...){return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
}
void test_ground_slam_session_orchestration(Fixture& f,u::Runtime& runtime) {
    const auto vm=f.session->vm();
    s::LoadResult common{};std::string error;
    check(!f.load("data/scripts/skills/_commons.luac",common)&&common.source_success,
          "retained skills common failed to load");
    const auto& slots=f.owner->slots(p::source::List::skill);
    std::uint32_t slot=UINT32_MAX;const p::constructor::State* instance=nullptr;
    for(std::uint32_t i=0;i<slots.size();++i)if(slots[i]){
        const auto* candidate=f.owner->instance(slots[i]);
        if(candidate&&candidate->script_name&&
           std::string(candidate->script_name)=="prince_warrior_ground_slam"){
            slot=i;instance=candidate;break;
        }
    }
    check(instance&&slot!=UINT32_MAX,"prepared Warrior Ground Slam instance is absent");
    const auto source=read(f.cache/"data/scripts/skills/prince_warrior_ground_slam.luac");
    const std::string bytes(source.begin(),source.end());
    const std::string fixture="DeclareSkill('prince_warrior_ground_slam',51);"+bytes;
    f.overlay("fixture/use/warrior-ground-slam",fixture);
    s::LoadResult loaded{};
    check(!f.load("fixture/use/warrior-ground-slam",loaded)&&loaded.source_success,
          "unchanged Ground Slam script fixture failed to load in retained VM");

    static const char* names[]={"GetCurrentSkillInfo__","GetProp","UseMana","StartTimer",
        "SetSkillCooldownTimerId__",
        "SkillCombatRoll__","SetProp","ClearProps","ApplyPropClass",
        "SetTargetListCharacterFilter","SetTargetListObjectFilter","SetTargetListSorting",
        "TargetListSearch","IsTargetListEmpty","GetTargetListTop","LookAt",
        "PopTargetList","ClearTarget"};
    d::PropertySheet shared_temp=f.properties.resolved;
    dh2::player_skill_property_services_v1::Bindings property_owner{
        CHAR,&f.catalogue.rules,&f.catalogue.classes,&f.properties,&shared_temp,false,&f.view};
    GroundSlamProbe probe{};probe.properties=&property_owner;
    probe.preparation=f.owner.get();
    auto timer_fields=f.owner->lease_timer_fields(CHAR);
    check(bool(timer_fields),"Ground Slam canonical skill timer-field lease is absent");
    probe.timer_fields=&*timer_fields;
    dh2::character::Coordinator coordinator(CHAR,32);
    dh2::character::CoordinatorBindings coordinator_bindings{};
    coordinator_bindings.facts=[](void*){return dh2::character::Facts{};};
    coordinator_bindings.services={nullptr,[](void*,dh2::character::State*,const dh2::character::Request*){}};
    coordinator_bindings.context=&probe;
    coordinator_bindings.before_timer_event=ground_slam_timer_expired;
    coordinator.bind(coordinator_bindings);
    probe.coordinator=&coordinator;
    check(dh2::player_skill_timer_provider_v1::bind(&probe.timer_bindings,&coordinator,CHAR),
          "Ground Slam canonical timer binding rejected Character identity");
    probe.cooldown={&probe,CHAR,ground_slam_skill_list_count,
                    ground_slam_skill_slot,ground_slam_unsupported_number};
    probe.source_ai.identity=0x221100;probe.source_ai.owner_04=CHAR;
    probe.source_ai.active_ais_1c=AIS;
    probe.source_ai.requested_target_3c=0x55;probe.source_ai.target_40=probe.target;
    probe.source_ai.last_target_44=probe.target;probe.source_ai.alive_48=1;
    probe.source_ai.sight_49=1;probe.source_ai.sticky_4c=1;
    probe.timer_dispatch_bindings={CHAR,probe.source_ai.identity,AIS,
        reinterpret_cast<std::uintptr_t>(&probe),reinterpret_cast<std::uintptr_t>(&probe),
        &probe.source_ai,&coordinator,f.session->vm()};
    probe.target_owner={CHAR,0,0x1234,0};
    {
        const dh2::character::set_target::Services services{
            &probe,0,clear_target_debug};
        probe.target_owner.identity=CHAR+1;
        check(dh2::character_clear_target_v1::clear(&probe.source_ai,
                  &probe.target_owner,CHAR,&services)==
                  dh2::character_clear_target_v1::Status::invalid_argument&&
              probe.source_ai.target_40==probe.target&&probe.target_debug_calls==0,
              "ClearTarget adapter accepted a different canonical Character owner");
        probe.target_owner.identity=CHAR;
    }
    const auto mana_field=std::find(f.catalogue.characters.fields.begin(),
                                    f.catalogue.characters.fields.end(),"SnS_ManaCost");
    check(mana_field!=f.catalogue.characters.fields.end(),"source SnS_ManaCost property id is absent");
    probe.mana_cost_property_id=std::int32_t(mana_field-f.catalogue.characters.fields.begin());
    const auto range_field=std::find(f.catalogue.characters.fields.begin(),
                                     f.catalogue.characters.fields.end(),"TempProp1");
    check(range_field!=f.catalogue.characters.fields.end(),"source TempProp1 property id is absent");
    probe.range_property_id=std::int32_t(range_field-f.catalogue.characters.fields.begin());
    dh2_script_value identity_probe_arg{};identity_probe_arg.type=DH2_SCRIPT_NUMBER;
    identity_probe_arg.number=float(probe.mana_cost_property_id);
    dh2_script_value identity_probe_out{};std::uint32_t identity_probe_returned=99;
    check(dh2::player_skill_property_services_v1::invoke(&property_owner,CHAR+0x100,
          dh2::character_native_bindings::Function::character_get_prop,&identity_probe_arg,1,
          &identity_probe_out,1,&identity_probe_returned,nullptr,0)==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE&&
          identity_probe_returned==0,"skill property adapter accepted a different Character");
    GroundSlamBinding bindings[sizeof(names)/sizeof(names[0])]{};
    for(std::size_t i=0;i<sizeof(names)/sizeof(names[0]);++i){bindings[i]={&probe,names[i]};
        check(!dh2_script_vm_bind_source_values(f.session->vm(),names[i],
            ground_slam_typed_native,&bindings[i]),"Ground Slam typed probe bind failed");}
    dh2_script_value selection[2]{};selection[0]=f.string(instance->script_name);
    selection[1].type=DH2_SCRIPT_NUMBER;selection[1].number=float(instance->skill_index_14);
    Return update{};
    check(!f.session->call("SetSkill",selection,2,0,observe,&update,error)&&!update.count,
          "Ground Slam SetSkill selection did not use the prepared source instance");
    if(f.session->call("OnSkillUpdate",nullptr,0,0,observe,&update,error))
        throw std::runtime_error("Ground Slam source OnSkillUpdate failed: "+error);
    if(probe.get_prop_calls!=4){std::string trace="Ground Slam GetProp count "+std::to_string(probe.get_prop_calls)+" native:";
        for(const auto& call:f.native_names)trace+=" "+call;
        trace+=" probe:";
        for(const auto& event:probe.events)trace+=" "+event;
        throw std::runtime_error(trace);}
    for(const auto callback:{u::Callback::pre,u::Callback::use,u::Callback::post}){
        u::Result result{};
        if(runtime.invoke(u::List::skill,slot,callback,result,error)){
            std::string trace="Ground Slam retained PlayerSkillUseSession callback "+std::to_string(int(callback))+" failed: "+error+" events:";
            for(const auto& event:probe.events)trace+=" "+event;
            throw std::runtime_error(trace);
        }
        if(!(result.destroyed&&result.last_lua_status==0)) throw std::runtime_error("Ground Slam callback lifecycle: callback="+std::to_string(int(callback))+" destroyed="+std::to_string(result.destroyed)+" status="+std::to_string(result.last_lua_status)+" decision="+std::to_string(int(result.decision))+" error="+error);
    }
    const auto has=[&](const char* name){return std::find(probe.events.begin(),probe.events.end(),name)!=probe.events.end();};
    const float source_range=std::trunc(float(shared_temp[std::size_t(probe.range_property_id)])/256.0f);
    if(!(probe.mana_cost>0&&probe.search_radius==source_range&&probe.combat_rolls==1&&
          probe.damage_probe==13.0f&&has("UseMana")&&has("PopTargetList")&&has("SkillCombatRoll__")&&
          probe.target_count==0&&probe.clear_target_calls==1&&probe.target_debug_calls==2&&
          probe.source_ai.requested_target_3c==0&&probe.source_ai.target_40==0&&
          probe.source_ai.last_target_44==probe.target&&probe.source_ai.sticky_4c==1&&
          probe.target_owner.target_change_marker_14d0==0&&f.session->vm()==vm)){
        throw std::runtime_error("Ground Slam probe final mismatch mana="+std::to_string(probe.mana_cost)+
            " range="+std::to_string(probe.search_radius)+" source_range="+std::to_string(source_range)+
            " rolls="+std::to_string(probe.combat_rolls)+" damage="+std::to_string(probe.damage_probe)+
            " targets="+std::to_string(probe.target_count)+" vm="+std::to_string(f.session->vm()==vm));
    }
    dh2::character_player_skills_preparation_v3::Owner::TimerFieldSlot cooldown_field{};
    if(probe.cooldown_index<0||
       !timer_fields->slot(CHAR,p::source::List::skill,std::uint32_t(probe.cooldown_index),cooldown_field)||
       !cooldown_field.instance||!cooldown_field.field18||*cooldown_field.field18<0){
        std::string trace="Ground Slam cooldown missing: events=";
        for(const auto& event:probe.events)trace+=event+",";
        trace+=" timers="+std::to_string(coordinator.timers().count);
        for(std::uint32_t i=0;i<coordinator.timers().count;++i){const auto& t=coordinator.timers().slots[i];trace+=" ["+std::to_string(t.id)+":"+std::to_string(t.active)+":"+std::to_string(t.event)+"]";}
        trace+=" slots=";
        for(std::uint32_t i=0;i<f.owner->slots(p::source::List::skill).size();++i){
            p::Owner::TimerFieldSlot field{};
            if(timer_fields->slot(CHAR,p::source::List::skill,i,field))trace+=" "+std::to_string(i)+"/"+std::to_string(field.instance)+"/"+(field.field18?std::to_string(*field.field18):"null");
        }
        throw std::runtime_error(trace);
    }
    const auto timer_id=std::uint32_t(*cooldown_field.field18);
    const auto& timers=coordinator.timers();
    const auto* timer=std::find_if(timers.slots,timers.slots+timers.count,
        [&](const auto& candidate){return candidate.id==timer_id;});
    check(timer!=timers.slots+timers.count&&timer->active&&timer->event==0x35&&
          timer->duration_ms>0&&cooldown_field.instance==instance->identity&&
          std::uint32_t(probe.cooldown_index)==slot,
          "Ground Slam cooldown field is not the same prepared skill/active Character timer");
    const auto duration=timer->duration_ms;
    check(coordinator.update_timers(duration,0)==1&&probe.timer_expiries==1&&
          f.session->vm()==vm,
          "Ground Slam cooldown expiry did not reach OnTimer in the same Player VM");
    check(timer_fields->slot(CHAR,p::source::List::skill,slot,cooldown_field)&&
          cooldown_field.instance==instance->identity&&*cooldown_field.field18==-1,
          "Ground Slam OnTimer did not clear cooldown on its retained source skill instance");
}

struct QuicknessBuffCycle {
    Fixture* fixture=nullptr;
    dh2::character::Coordinator* coordinator=nullptr;
    buff_owner::Owner* owner=nullptr;
    dh2::player_skill_timer_provider_v1::Bindings player_timers{};
    buff_owner::CallbackBindings buff_callbacks{};
    dh2::player_script_timer_dispatch_v1::Bindings dispatch{};
    dh2::character_ai_initialization::State ai{};
    dh2::player_skill_property_services_v1::Bindings property_services{};
    d::PropertySheet shared_temp{};
    std::uint32_t fx_id=0;
    unsigned create_calls=0,apply_calls=0,buff_expiries=0,script_expiries=0,remove_calls=0;
};
int quickness_buff_service(void* raw,d::PropertyView* view,
    const dh2::character_player_buffs_v1::Request* q,
    dh2::character_player_buffs_v1::Response* out) {
    auto& p=*static_cast<QuicknessBuffCycle*>(raw);
    if(!q||!out||view!=&p.fixture->view||q->character!=CHAR)return 1;
    switch(q->operation){
    case buff_owner::Operation::timer_start:
        if(q->event!=0x36||q->repeat)return 1;
        out->word=p.coordinator->start_timer(q->duration,q->repeat,q->event,q->subject);
        return out->word< -1?1:0;
    case buff_owner::Operation::timer_stop:
        return q->id<0?0:p.coordinator->stop_timer(std::uint32_t(q->id))<0?1:0;
    case buff_owner::Operation::timer_time_left:{
        const auto& timers=p.coordinator->timers();
        if(q->id<0||std::uint32_t(q->id)>=timers.count)return 1;
        out->elapsed=timers.slots[q->id].elapsed_ms;
        out->duration=timers.slots[q->id].duration_ms;return 0;
    }
    case buff_owner::Operation::fx_load:
        if(q->id!=std::int32_t(p.fx_id))return 1;
        out->identity=0x700000001ull;return 0; // Typed fixture handle; no rendered FX claim.
    case buff_owner::Operation::fx_release:return 0;
    case buff_owner::Operation::apply_class:{
        std::int32_t* owned=nullptr;
        if(!p.owner->owned_sheet(q->subject,&owned)||owned!=q->sheet||q->id<0||
           std::size_t(q->id)>=p.fixture->catalogue.classes.rows.size())return 1;
        std::vector<d::ClassRow> rows;
        for(const auto& row:p.fixture->catalogue.classes.rows)
            rows.push_back({row.data(),std::uint32_t(row.size())});
        return dh2_class_apply(rows.data(),std::uint32_t(rows.size()),q->id,
                               q->sheet,view->resolved);
    }
    case buff_owner::Operation::recalculate:{
        std::vector<d::ClassRow> rows;
        for(const auto& row:p.fixture->catalogue.classes.rows)
            rows.push_back({row.data(),std::uint32_t(row.size())});
        return dh2_class_recalc_base(rows.data(),std::uint32_t(rows.size()),
                                     p.fixture->properties.base.data(),view);
    }
    default:return 1;
    }
}
void quickness_timer_event(void* raw,dh2::character::Coordinator& coordinator,
    std::int32_t event,dh2::character::Timer32& timer,std::uint32_t) {
    auto& p=*static_cast<QuicknessBuffCycle*>(raw);
    if(&coordinator!=p.coordinator)throw std::runtime_error("Quickness timer used another Character Coordinator");
    if(event==0x36){
        dh2::character_player_buffs_v1::Result result{};
        if(p.owner->expired(&timer,&result)!=dh2::character_player_buffs_v1::Status::complete)
            throw std::runtime_error("Quickness buff event 0x36 did not expire its owned instance");
        ++p.buff_expiries;return;
    }
    if(event==0x35){
        std::string error;
        if(dh2::player_script_timer_dispatch_v1::dispatch(p.dispatch,timer.id,error)!=
           dh2::player_script_timer_dispatch_v1::Status::delivered)
            throw std::runtime_error(error.empty()?"Quickness OnTimer was not delivered":error);
        ++p.script_expiries;return;
    }
    throw std::runtime_error("Quickness emitted an unexpected timer event");
}
struct QuicknessNativeBinding {QuicknessBuffCycle* cycle=nullptr;const char* name=nullptr;};
void prepare(Fixture&);
int quickness_native(void* raw,const dh2_script_value* args,std::uint32_t count,
    dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,
    char* error,std::size_t error_capacity) noexcept {
    try {
        auto& b=*static_cast<QuicknessNativeBinding*>(raw);auto& p=*b.cycle;
        if(!returned||(count&&!args))return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
        *returned=0;const std::string name=b.name;
        if(name=="CreateBuff"){
            ++p.create_calls;return dh2::character_player_buffs_v1::create_buff(
                &p.buff_callbacks,args,count,out,capacity,returned,error,error_capacity);
        }
        if(name=="ApplyBuff"){
            ++p.apply_calls;return dh2::character_player_buffs_v1::apply_buff(
                &p.buff_callbacks,args,count,out,capacity,returned,error,error_capacity);
        }
        if(name=="RemoveBuff"){
            ++p.remove_calls;return dh2::character_player_buffs_v1::remove_buff(
                &p.buff_callbacks,args,count,out,capacity,returned,error,error_capacity);
        }
        if(name=="StartTimer")return dh2::player_skill_timer_provider_v1::start(
            &p.player_timers,args,count,out,capacity,returned,error,error_capacity);
        if(name=="StopTimer")return dh2::player_skill_timer_provider_v1::stop(
            &p.player_timers,args,count,out,capacity,returned,error,error_capacity);
        if(name=="GetCurrentSkillInfo__"){
            if(count!=1||args[0].type!=DH2_SCRIPT_NUMBER||!out||!capacity)return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
            out[0]={};out[0].type=DH2_SCRIPT_NUMBER;out[0].number=1.0f;*returned=1;return 0;
        }
        if(name=="GetProp"){
            if(count<1||args[0].type!=DH2_SCRIPT_NUMBER||!out||!capacity)return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
            // Quickness sees zero SNS duration and follows its authored 4s + 1s/level fallback.
            out[0]={};out[0].type=DH2_SCRIPT_NUMBER;out[0].number=0.0f;*returned=1;return 0;
        }
        if(name=="SetProp"||name=="SetBuffProp"||name=="ClearProps"){
            if(!p.fixture)return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
            if(name=="ClearProps")return count>1?DH2_SCRIPT_REQUIRED_SERVICE_FAILURE:
                dh2::player_skill_property_services_v1::invoke(&p.property_services,CHAR,
                    dh2::character_native_bindings::Function::character_clear_props,
                    args,count,out,capacity,returned,error,error_capacity);
            if(count<2||count>3)return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
            return dh2::player_skill_property_services_v1::invoke(
                &p.property_services,CHAR,
                dh2::character_native_bindings::Function::character_set_prop,
                args,count,out,capacity,returned,error,error_capacity);
        }
        if(name=="ApplyPropClass")return 0; // OnSkillUpdate's temporary calculation is observed, not a game mutation.
        if(name=="UseMana")return count==1?0:DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
        return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
    }catch(...){return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
}
void test_authored_quickness_buff_cycle(const std::filesystem::path& cache,
    const std::filesystem::path& temp,Catalogue& cat) {
    using namespace dh2::character_player_buffs_v1;
    Fixture f(cache,temp,cat,"RoguePlayerBase");f.common();prepare(f);
    const auto vm=f.session->vm();auto* slots=&f.owner->slots(p::source::List::skill);
    std::uint32_t slot=UINT32_MAX;const p::constructor::State* skill=nullptr;
    for(std::uint32_t i=0;i<slots->size();++i)if((*slots)[i]){
        const auto* candidate=f.owner->instance((*slots)[i]);
        if(candidate&&candidate->script_name&&std::string(candidate->script_name)=="prince_rogue_quickness"){
            slot=i;skill=candidate;break;
        }
    }
    check(skill&&slot!=UINT32_MAX,"authored Rogue Quickness is absent from prepared Character");
    s::LoadResult loaded{};std::string error;
    check(!f.load("data/scripts/skills/_commons.luac",loaded)&&loaded.source_success,
          "actual skill common failed to load for Quickness");
    QuicknessBuffCycle cycle{};cycle.fixture=&f;
    dh2_pynames_view effect_names{};std::int32_t effect_id=-1;
    check(!dh2_pynames_open(&effect_names,cat.names["AnimatedEffectTable"].data(),
          std::uint32_t(cat.names["AnimatedEffectTable"].size()))&&
          !dh2_pynames_get(&effect_names,"spell_dh2_prince_rogue_quickness",32,&effect_id)&&
          effect_id>=0,"Quickness authored FX OID is absent");
    cycle.fx_id=std::uint32_t(effect_id);
    cycle.ai.identity=0x221100;cycle.ai.owner_04=CHAR;cycle.ai.active_ais_1c=AIS;
    dh2::character::Coordinator coordinator(CHAR,8);cycle.coordinator=&coordinator;
    dh2::character::CoordinatorBindings timer_bindings{};timer_bindings.context=&cycle;
    timer_bindings.facts=[](void*){return dh2::character::Facts{};};
    timer_bindings.services={nullptr,[](void*,dh2::character::State*,const dh2::character::Request*){}};
    timer_bindings.before_timer_event=quickness_timer_event;coordinator.bind(timer_bindings);
    check(dh2::player_skill_timer_provider_v1::bind(&cycle.player_timers,&coordinator,CHAR),
          "Quickness player timer did not bind to the Character Coordinator");
    cycle.buff_callbacks.owner=nullptr;
    auto owner=Owner::create({CHAR,&f.view,{&cycle,quickness_buff_service},
        std::uint32_t(cat.classes.rows.size()),
        effect_names.count});
    check(bool(owner),"Quickness sole buff owner creation failed");cycle.owner=owner.get();cycle.buff_callbacks.owner=owner.get();
    cycle.dispatch={CHAR,cycle.ai.identity,AIS,reinterpret_cast<std::uintptr_t>(&cycle),
        reinterpret_cast<std::uintptr_t>(&f.properties),&cycle.ai,&coordinator,vm};
    cycle.shared_temp=f.properties.resolved;
    cycle.property_services={CHAR,&cat.rules,&cat.classes,&f.properties,&cycle.shared_temp,
        false,&f.view};
    static const char* names[]={"CreateBuff","ApplyBuff","RemoveBuff","StartTimer","StopTimer",
        "GetCurrentSkillInfo__","GetProp","SetProp","SetBuffProp","ClearProps","ApplyPropClass","UseMana"};
    QuicknessNativeBinding bindings[sizeof(names)/sizeof(names[0])]{};
    for(std::size_t i=0;i<std::size(names);++i){bindings[i]={&cycle,names[i]};
        check(!dh2_script_vm_bind_source_values(vm,names[i],quickness_native,&bindings[i]),
              "Quickness same-VM native owner binding failed");}
    dh2_script_value selection[2]{};selection[0]=f.string(skill->script_name);
    selection[1].type=DH2_SCRIPT_NUMBER;selection[1].number=float(skill->skill_index_14);
    Return selected;check(!f.call("SetSkill",{selection[0],selection[1]},selected)&&!selected.count,
          "Quickness source skill selection failed");
    Return updated;
    if(f.session->call("OnSkillUpdate",nullptr,0,0,observe,&updated,error))
        throw std::runtime_error("Quickness authored OnSkillUpdate did not compute its source duration: "+error);
    u::Runtime runtime(*f.session,*f.owner,CHAR);u::Result used{};
    if(runtime.invoke(u::List::skill,slot,u::Callback::use,used,error)||
          !used.destroyed||used.last_lua_status!=0||f.session->vm()!=vm)
        throw std::runtime_error("Quickness authored OnSkill callback failed in its retained Player VM: "+error);
    Snapshot buff_snapshot{};
    check(cycle.create_calls==1&&cycle.apply_calls==1&&owner->count()==1&&
          f.view.group_count==1&&owner->snapshot(0,&buff_snapshot)&&coordinator.timers().count==2&&
          coordinator.timers().slots[0].event==0x36&&coordinator.timers().slots[1].event==0x35&&
          coordinator.timers().slots[0].duration_ms==5000&&
          coordinator.timers().slots[1].duration_ms==5000&&
          coordinator.timers().slots[0].user_ref==buff_snapshot.instance&&
          coordinator.timers().slots[1].user_ref==0,
          "Quickness did not create one buff expiry and one script callback in the same Coordinator");
    const auto duration=coordinator.timers().slots[0].duration_ms;
    const auto update_status=coordinator.update_timers(duration,0);
    if(!(update_status==1&&cycle.buff_expiries==1&&cycle.script_expiries==1&&
          owner->count()==0&&f.view.group_count==0&&f.session->vm()==vm))
        throw std::runtime_error("Quickness expiry mismatch status="+std::to_string(update_status)+
          " buff="+std::to_string(cycle.buff_expiries)+" script="+std::to_string(cycle.script_expiries)+
          " owned="+std::to_string(owner->count())+" vm="+std::to_string(f.session->vm()==vm));
    u::Result pre{};check(!runtime.invoke(u::List::skill,slot,u::Callback::pre,pre,error)&&
          pre.destroyed&&cycle.remove_calls==0&&owner->count()==0,
          "Quickness BuffExpired callback did not clear its source-local buff handle");
    Result retired{};check(owner->retire(&retired)==Status::complete,
          "Quickness buff owner retirement failed");coordinator.stop_timers();
}
void test_skill_combat_roll_wrapper(Fixture& f) {
    std::size_t binding_count=0;const auto* bindings=nb::character_own_bindings(&binding_count);
    unsigned function_entries=0,method_entries=0;
    for(std::size_t i=0;i<binding_count;++i)if(std::string(bindings[i].name)=="SkillCombatRoll__" && bindings[i].function==nb::Function::character_skill_combat_roll){
        if(bindings[i].kind==nb::Kind::function && bindings[i].context==nb::Context::character)++function_entries;
        if(bindings[i].kind==nb::Kind::method && bindings[i].context==nb::Context::none)++method_entries;
    }
    check(binding_count==179&&function_entries==1&&method_entries==1&&
          std::string(nb::provider_name(nb::Function::character_skill_combat_roll))=="character_skill_combat_roll",
          "SkillCombatRoll function/method registration contract differs");
    // The retained real skills _commons owns the public helper; this tiny
    // registered source skill exercises its call into the exact native name.
    const char source[]=
        "DeclareSkill('wrapper_probe',42);"
        "RegisterSkill(function() end,function() end,function() end,"
        "function() end,function() end,nil,nil);"
        "SetSkill('wrapper_probe');"
        "function CallSkillCombatRoll(target) return SkillCombatRoll(target) end";
    s::LoadResult loaded{};std::string error;
    const auto path="fixture/use/skill-combat-roll-wrapper";
    f.overlay(path,source);
    check(!f.session->load_resolved(path,&loaded,error)&&loaded.source_success,
          "source wrapper fixture did not load");
    SkillCombatRollProbe probe{};
    check(!dh2_script_vm_bind_source_values(f.session->vm(),"SkillCombatRoll__",
          probe_skill_combat_roll,&probe),"could not instrument the registered native callback");
    dh2_script_value target{};target.type=DH2_SCRIPT_IDENTITY;
    target.identity=0x123456789abcdef0ull;
    Return result{};
    check(!f.call("CallSkillCombatRoll",{target},result)&&probe.calls==1&&
          probe.slot==42&&probe.target==target.identity&&result.type==DH2_SCRIPT_NUMBER&&
          result.number==73.0f,
          "skills _commons did not inject SKILL_ID before forwarding the target userdata");
}
struct Probe {u::Runtime* runtime;unsigned calls=0;};
int probe(void* raw,const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,
          std::uint32_t* returned,char*,std::size_t) noexcept {try{
    auto& p=*static_cast<Probe*>(raw);u::Result out{};out.value=99;auto before=out;std::string error="sentinel";
    check(p.runtime->check(u::List::skill,7,u::Check::usable,out,error)==-1 && !std::memcmp(&out,&before,sizeof(out)) && error=="sentinel","same Runtime check reentry changed output");
    check(p.runtime->invoke(u::List::skill,7,u::Callback::pre,out,error)==-1 && !std::memcmp(&out,&before,sizeof(out)) && error=="sentinel","same Runtime use reentry changed output");
    ++p.calls;*returned=0;return 0;
}catch(...){return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
}
int actual_constants(void* raw,const dh2_script_value* args,std::uint32_t count,dh2_script_value* out,
                     std::uint32_t cap,std::uint32_t* returned,char*,std::size_t) noexcept {try{
    auto& f=*static_cast<Fixture*>(raw);check(count==2,"source constants arity");const auto category=text(args[0]),key=text(args[1]);
    const auto& input=category=="AIStates" || category.rfind("AITargetList_",0)==0?f.catalogue.ai_constants:f.catalogue.design;
    dh2_pycst_view view{};dh2_pycst_result value{};
    check(!dh2_pycst_open(&view,input.data(),std::uint32_t(input.size())) && !dh2_pycst_get(&view,category.data(),std::uint32_t(category.size()),key.data(),std::uint32_t(key.size()),&value)&&value.found,"actual constant missing");
    result(out,cap,returned,float(value.value));return 0;
}catch(...){return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
}
void prepare(Fixture& f){f.common();p::source::Result result{};check(f.owner->prepare(&result)==p::source::Status::complete,"actual source preparation failed");
    // Real parser service fixture adds the AI constants namespace needed by
    // Bashdown. This changes no original script bytes or property/timer store.
    check(!dh2_script_vm_bind_source_values(f.session->vm(),"GetPyCst",actual_constants,&f),"actual constant provider binding failed");}
void fixture_load(Fixture& f,const std::string& source){
    static unsigned count=0;auto path="fixture/use/"+std::to_string(count++);f.overlay(path.c_str(),source);s::LoadResult r{};check(!f.load(path,r)&&r.source_success,"callback fixture load failed");
}
float global(Fixture& f,const char* name){dh2_script_value v{};check(!dh2_script_vm_get_global(f.session->vm(),name,&v)&&v.type==DH2_SCRIPT_NUMBER,"missing fixture counter");return v.number;}
int oracle_case(int argc,char** argv){
    check(argc==10,"oracle requires cache temp phase set-error set-count call-error count type");
    auto phase=std::stoi(argv[4]),set_error=std::stoi(argv[5]),set_count=std::stoi(argv[6]),call_error=std::stoi(argv[7]),count=std::stoi(argv[8]),type=std::stoi(argv[9]);
    Catalogue cat(argv[2]);Fixture f(argv[2],std::filesystem::path(argv[3])/"oracle",cat,"KnightPlayerBase");prepare(f);
    std::string result=type==0?"nil":type==1?"false":type==2?"true":type==3?"0":type==4?"-0":type==5?"0/0":type==6?"''":"'0'";
    std::string set="function SetSkill() set_calls=(set_calls or 0)+1; "+std::string(set_error?"error('set'); ":"")+std::string(set_count?"return 1,2":"")+" end; ";
    std::string body=call_error?"error('callback')":count?"return "+result+(count==2?",false":""):"";
    fixture_load(f,set+"function OnPreSkill() "+body+" end; OnSkill=OnPreSkill; OnPostSkill=OnPreSkill;");
    u::Runtime runtime(*f.session,*f.owner,CHAR);u::Result r{};std::string error;
    check(!runtime.invoke(u::List::skill,0,static_cast<u::Callback>(phase),r,error),"oracle fixture failed");
    std::cout<<"{\"value\":"<<r.value<<",\"calls\":"<<r.call_count<<",\"erased\":"<<r.erased<<",\"destroyed\":"<<r.destroyed<<",\"retained\":"<<runtime.retained_failed_returns()<<"}\n";
    f.session.reset();return 0;
}
int main(int argc,char** argv){try{
    if(argc>1 && std::string(argv[1])=="--oracle")return oracle_case(argc,argv);
    test_player_skill_timer_provider();
    test_player_script_timer_expiry_dispatch();
    check(argc==3,"cache and real Debug directory required");Catalogue cat(argv[1]);Fixture f(argv[1],std::filesystem::path(argv[2])/"actual",cat,"KnightPlayerBase");prepare(f);
    test_authored_quickness_buff_cycle(argv[1],std::filesystem::path(argv[2])/"quickness",cat);
    test_skill_combat_roll_wrapper(f);
    u::Runtime runtime(*f.session,*f.owner,CHAR);u::Result r{};std::string error;unsigned actual=0,protocol=0,guards=0;
    const auto vm=f.session->vm();
    {
        dh2::ais_player_init_vcb::State alternate_ais{AIS+0x100,0xffffffff};
        s::Configuration config;config.character=CHAR+0x100;config.ais=&alternate_ais;config.tables=cat.tables;
        auto foreign=s::Session::adopt(s::Vm(dh2_script_vm_create_deferred(1024*1024)),std::move(config),error);
        check(bool(foreign),"foreign ownership fixture adoption failed");
        bool rejected=false;try{u::Runtime mismatched(*foreign,*f.owner,CHAR);}catch(const std::invalid_argument&){rejected=true;}
        check(rejected && foreign->stage()==s::Stage::created && foreign->character_identity()==CHAR+0x100 && f.session->vm()==vm,
              "Runtime accepted another Character's retained VM");++guards;
    }
    check(!runtime.check(u::List::skill,7,u::Check::usable,r,error)&&!r.value&&r.source_check.destroyed&&r.call_count==2,"authored passive usable differs");++actual;
    check(!runtime.check(u::List::skill,7,u::Check::active,r,error)&&r.value&&r.source_check.value_count==2&&r.call_count==2,"authored passive active lost second return");++actual;
    check(runtime.check(u::List::skill,0,u::Check::usable,r,error)==-1&&r.last_lua_status==-5&&!r.destroyed&&f.native_names.back()=="HasMana","uninitialized Bashdown mana accepted");++actual;
    check(runtime.invoke(u::List::skill,0,u::Callback::pre,r,error)==-1&&r.last_lua_status==-5&&!r.destroyed&&f.native_names.back()=="SetTargetListCharacterFilter","unbound source target filtering accepted");++actual;
    check(!runtime.invoke(u::List::skill,0,u::Callback::use,r,error)&&r.value&&r.decision==u::Decision::empty_success&&r.destroyed,"authored initial nil-target use differs");++actual;
    check(runtime.invoke(u::List::skill,0,u::Callback::post,r,error)==-1&&r.last_lua_status==-5&&!r.destroyed&&f.native_names.back()=="ClearTarget","unbound post target clear accepted");++actual;
    check(runtime.retained_failed_returns()==3,"failed source returns unexpectedly destroyed");
    test_ground_slam_session_orchestration(f,runtime);

    fixture_load(f,"set_calls=0; check_calls=0; function SetSkill() set_calls=set_calls+1; return 'old',false end; function OnSkillCheck() check_calls=check_calls+1; return false,true,'third' end; function OnPreSkill() return '' end; function OnSkill() return 0 end; function OnPostSkill() error('post effect retained') end;");
    check(!runtime.check(u::List::skill,0,u::Check::usable,r,error)&&!r.value&&r.erased&&r.return_count==3,"all source returns/erase usable differs");++protocol;
    check(!runtime.check(u::List::skill,0,u::Check::active,r,error)&&r.value&&r.source_check.value_count==3,"active check projected wrong return");++protocol;
    check(global(f,"check_calls")==2&&global(f,"set_calls")==2,"checks replayed to inspect second return");++protocol;
    check(!runtime.invoke(u::List::skill,0,u::Callback::pre,r,error)&&r.value&&r.erased&&r.destroyed,"source empty string getBool was generic false");++protocol;
    check(!runtime.invoke(u::List::skill,0,u::Callback::use,r,error)&&!r.value&&r.decision==u::Decision::converted&&r.destroyed,"source number0 getBool differs");++protocol;
    check(!runtime.invoke(u::List::skill,0,u::Callback::post,r,error)&&r.last_lua_status>0&&r.destroyed&&r.decision==u::Decision::post_discard,"normal Post error skipped source destroy");++protocol;
    fixture_load(f,"function OnPreSkill() return end; function OnSkill() return -0 end; function OnSkillCheck() return 0/0, '' end");
    check(!runtime.invoke(u::List::skill,0,u::Callback::pre,r,error)&&r.value&&r.decision==u::Decision::empty_success,"Pre empty return is false");++protocol;
    check(!runtime.invoke(u::List::skill,0,u::Callback::use,r,error)&&!r.value,"source signed zero was true");++protocol;
    check(!runtime.check(u::List::skill,0,u::Check::usable,r,error)&&r.value,"source NaN was false");++protocol;
    check(!runtime.check(u::List::skill,0,u::Check::active,r,error)&&r.value,"source string temporary Lua bool differs");++protocol;
    fixture_load(f,"function SetSkill() set_effect=23; error('set') end; function OnPreSkill() error('unreached') end");
    check(!runtime.invoke(u::List::skill,0,u::Callback::pre,r,error)&&!r.value&&r.call_count==1&&r.destroyed&&r.decision==u::Decision::set_skill_error&&global(f,"set_effect")==23,"normal SetSkill failure lost effects/cleanup");++protocol;
    fixture_load(f,"function SetSkill() end; function OnPreSkill() pre_effect=31; error({}) end");
    auto failures=runtime.retained_failed_returns();check(runtime.invoke(u::List::skill,0,u::Callback::pre,r,error)==-1&&r.last_lua_status==-4&&!r.destroyed&&runtime.retained_failed_returns()==failures+1&&global(f,"pre_effect")==31,"unsupported error lost effects/prefix");++protocol;
    fixture_load(f,"function OnPreSkill() pcall(function() GetHostPlayer() end); return true end");
    failures=runtime.retained_failed_returns();check(runtime.invoke(u::List::skill,0,u::Callback::pre,r,error)==-1&&r.last_lua_status==-5&&!r.destroyed&&runtime.retained_failed_returns()==failures+1&&r.return_count==1,"caught required failure became success");++protocol;
    Probe controls{&runtime};check(!dh2_script_vm_bind(f.session->vm(),"fixture_reentry",probe,&controls),"probe binding failed");
    fixture_load(f,"function OnPreSkill() fixture_reentry(); return false end");check(!runtime.invoke(u::List::skill,0,u::Callback::pre,r,error)&&!r.value&&controls.calls==1,"Runtime reentry was accepted");guards+=2;
    r.value=99;auto before=r;error="sentinel";
    check(runtime.check(u::List::skill,0,static_cast<u::Check>(9),r,error)==-1&&!std::memcmp(&r,&before,sizeof(r))&&error=="sentinel","invalid check mutated output");++guards;
    check(runtime.invoke(u::List::skill,0,static_cast<u::Callback>(9),r,error)==-1&&!std::memcmp(&r,&before,sizeof(r))&&error=="sentinel","invalid callback mutated output");++guards;
    check(runtime.invoke(u::List::skill,15,u::Callback::use,r,error)==-1&&!std::memcmp(&r,&before,sizeof(r)),"source null slot manufactured use");++guards;
    const auto id=f.session->vm();
    check(runtime.invoke(u::List::skill,0,u::Callback::pre,*reinterpret_cast<u::Result*>(f.session.get()),error)==-1&&f.session->vm()==id,"Result aliases retained VM wrapper");++guards;
    check(runtime.invoke(u::List::skill,0,u::Callback::pre,*reinterpret_cast<u::Result*>(&runtime),error)==-1,"Result aliases Runtime wrapper");++guards;
    check(runtime.invoke(u::List::skill,0,u::Callback::pre,*reinterpret_cast<u::Result*>(f.owner.get()),error)==-1&&f.owner->state().owner==CHAR,"Result aliases prepared Owner");++guards;
    check(f.session->vm()==vm,"skill invocation replaced Player VM");
    test_bashdown_callback_order(f);
    f.session.reset(); // Close probe's Lua closure before borrowed Runtime/context.
    std::cout<<"{\"validation\":\"PASS\",\"actual_script_cases\":"<<actual<<",\"protocol_cases\":"<<protocol<<",\"guards\":"<<guards<<",\"same_vm\":true,\"ground_slam_source_cooldown_cycle\":true,\"quickness_authored_buff_event36_cycle\":true,\"coordinator_timer_expiry_to_OnTimer\":true,\"inactive_AIS_gate\":true,\"foreign_AIS_rejected\":true,\"Bashdown_target_mana_combat_complete\":false,\"FSM_activation\":false}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
