#include "monster_decisions.hpp"
#include "../../asset_catalog.hpp"
#include "../../playable_actor_world.hpp"
#include "../../../script-runtime/script_runtime.h"
#include "../../../script-runtime/script_scalar_bindings.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
using namespace dh::foundation::enemy_ai;
static void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
static float source_skill_tree(std::int32_t raw) {
    dh2_script_value input{};input.type=DH2_SCRIPT_NUMBER;input.number=static_cast<float>(raw);
    dh2_script_value output[2]{};std::uint32_t returned=0;char error[128]{};
    check(dh2_script_scalar_from_fixed(nullptr,&input,1,output,2,&returned,error,sizeof(error))==0 &&
          returned==2 && output[0].type==DH2_SCRIPT_NUMBER,
          "Actual FromFixed source scalar rejected SkillTree");
    return output[0].number;
}
struct Fixture {
    ActorState actor;std::vector<Operation> calls;int state=3,roll=40,skill_raw=-256,buff_id=7;
    bool path=false;std::uint32_t time=500;float x=1,y=2;
    Invoke provider(){return [this](ActorState& a,const Request& q,Response& r,std::string&){
        check(&a==&actor,"Native copied actor");calls.push_back(q.operation);
        switch(q.operation){
        case Operation::state:r.integer=state;break;
        case Operation::state_constant:r.integer=q.name=="Idle"?3:4;break;
        case Operation::has_path:r.integer=path;break;
        case Operation::state_time:r.time_ms=time;break;
        case Operation::position:r.x=x;r.y=y;break;
        case Operation::random:r.integer=roll;break;
        case Operation::set_target:a.target_id=q.target;break;
        case Operation::clear_target:a.target_id=invalid_actor_id;break;
        case Operation::create_buff:r.token=1234;break;
        default:break;
        }return true;
    };}
};
struct Binding {Fixture* f;const char* name;};
static int callback(void* raw,const dh2_script_value* a,std::uint32_t count,
    dh2_script_value* out,std::uint32_t capacity,std::uint32_t* n,char* error,std::size_t ec){
    try {
        auto& binding=*static_cast<Binding*>(raw);auto& f=*binding.f;const std::string name=binding.name;
        *n=0;auto number=[&](float v){check(*n<capacity,"Callback capacity");out[*n]={};out[*n].type=DH2_SCRIPT_NUMBER;out[(*n)++].number=v;};
        auto boolean=[&](bool v){check(capacity,"Callback capacity");out[0]={};out[0].type=DH2_SCRIPT_BOOLEAN;out[0].boolean=v;*n=1;};
        auto identity=[&](ActorId id){if(id)number(static_cast<float>(id));else{check(capacity,"Callback capacity");out[0]={};*n=1;}};
        if(name=="GetPyStruct"){number(28);return 0;}
        if(name=="GetProp"){number(static_cast<float>(f.skill_raw));return 0;}
        if(name=="FromFixed")return dh2_script_scalar_from_fixed(nullptr,a,count,out,capacity,n,error,ec);
        if(name=="AddToVFTable")return 0; // Registration transport fixture, no owner claim.
        if(name=="HasTarget"){boolean(f.actor.target_id!=invalid_actor_id);return 0;}
        if(name=="GetTarget"){identity(f.actor.target_id);return 0;}
        Request q;
        if(name=="GetState")q.operation=Operation::state;
        else if(name=="GetPyCst"){q.operation=Operation::state_constant;check(count==2,"GetPyCst args");q.name=a[1].text;}
        else if(name=="HasPath")q.operation=Operation::has_path;
        else if(name=="GetStateTime")q.operation=Operation::state_time;
        else if(name=="GetPosition")q.operation=Operation::position;
        else if(name=="GetRand"){q.operation=Operation::random;check(count==2&&a[0].number==0&&a[1].number==100,"Original random bounds");q.argument1=100;}
        else if(name=="Stop")q.operation=Operation::stop;
        else if(name=="SetTarget")q.operation=Operation::set_target;
        else if(name=="ClearTarget")q.operation=Operation::clear_target;
        else if(name=="HeadTo")q.operation=Operation::head_to;
        else if(name=="MoveTo")q.operation=Operation::move_to;
        else if(name=="Attack")q.operation=Operation::attack;
        else if(name=="DoSkill")q.operation=Operation::do_skill;
        else if(name=="Flee")q.operation=Operation::flee;
        else if(name=="ClearAggro")q.operation=Operation::clear_aggro;
        else if(name=="CreateBuff")q.operation=Operation::create_buff;
        else if(name=="ApplyBuff")q.operation=Operation::apply_buff;
        else if(name=="RemoveBuff")q.operation=Operation::remove_buff;
        else throw std::runtime_error("Unsupported actual Lua service "+name);
        if(count&&a[0].type==DH2_SCRIPT_NUMBER)q.target=static_cast<ActorId>(a[0].number);
        Response r;std::string e;check(f.provider()(f.actor,q,r,e),e);
        switch(q.operation){
        case Operation::state:case Operation::state_constant:case Operation::random:number(static_cast<float>(r.integer));break;
        case Operation::has_path:boolean(r.integer!=0);break;
        case Operation::state_time:number(static_cast<float>(r.time_ms));break;
        case Operation::position:number(r.x);number(r.y);break;
        case Operation::create_buff:number(static_cast<float>(r.token));break;
        default:break;
        }return 0;
    }catch(const std::exception& ex){if(ec)std::snprintf(error,ec,"%s",ex.what());return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
}
static dh2_script_value global(dh2_script_vm* vm,const char* name){dh2_script_value v{};check(!dh2_script_vm_get_global(vm,name,&v),"GetGlobal failed");return v;}
int main(int argc,char** argv){try {
    check(argc==3,"Original assets and actual monster.luac path required");AssetCatalog assets(argv[1]);
    std::ifstream input(argv[2],std::ios::binary);check(bool(input),"Original monster script missing");
    const std::string script((std::istreambuf_iterator<char>(input)),{});
    OriginalPropertyDatabase db;dh2::data::AiTables tables;OriginalCombatProperties p;std::string e;
    check(load_original_property_tables(assets,"original-cache/data/pydata/",db,e),e);
    check(load_original_ai_tables(assets,"original-cache/data/pydata/",tables,e),e);
    check(build_original_combat_properties(db,"Swamp_LizadMan_Type1",{},{},{},p,e),e);
    const std::pair<MonsterEvent,const char*> events[]={
        {MonsterEvent::enemy_spotted,"monster_OnEnemySpotted"},
        {MonsterEvent::target_died,"monster_OnTargetDied"},
        {MonsterEvent::target_out_of_sight,"monster_OnTargetOutOfSight"},
        {MonsterEvent::target_in_sight,"monster_OnTargetInSight"},
        {MonsterEvent::target_out_of_range,"monster_OnTargetOutOfRange"},
        {MonsterEvent::target_in_ranged_range,"monster_OnTargetInRangedRange"},
        {MonsterEvent::target_in_close_range,"monster_OnTargetInCloseRange"},
        {MonsterEvent::target_in_melee_range,"monster_OnTargetInMeleeRange"},
        {MonsterEvent::flee,"monster_Flee"},{MonsterEvent::died,"monster_OnDied"},
        {MonsterEvent::terminate,"monster_OnTerminate"}};
    const char* functions[]={"GetPyStruct","GetProp","FromFixed","AddToVFTable","HasTarget","GetTarget","GetState","GetPyCst","HasPath","GetStateTime","GetPosition","GetRand","Stop","SetTarget","ClearTarget","HeadTo","MoveTo","Attack","DoSkill","Flee","ClearAggro","CreateBuff","ApplyBuff","RemoveBuff"};
    unsigned cases=0;
    for(int fixture=0;fixture<128;++fixture)for(const auto& event:events){
        Fixture source;source.actor.id=1;source.actor.target_id=fixture&1?22:0;
        source.skill_raw=fixture&2?0:-256;source.state=fixture&4?4:3;
        source.path=fixture&8;source.time=fixture&16?501:500;source.roll=fixture&32?39:40;
        Fixture native=source;MonsterGlobals globals;
        globals.skill_tree_id=source_skill_tree(source.skill_raw);
        globals.buff_speed_id=7;globals.buff=fixture&64?1234:0;
        globals.saved_x=source.x;globals.saved_y=source.y;
        auto* vm=dh2_script_vm_create(4*1024*1024);check(vm,"Source VM allocation");
        std::vector<Binding> bindings;bindings.reserve(std::size(functions));
        for(const auto* fn:functions){bindings.push_back({&source,fn});check(!dh2_script_vm_bind(vm,fn,callback,&bindings.back()),"Source callback bind failed");}
        check(!dh2_script_vm_load(vm,script.data(),script.size(),"actual-cache-monster"),dh2_script_vm_error(vm));
        const std::string setup="m_flee_flag=true;BUFF_ID=7;buff="+std::string(globals.buff?"1234":"nil")+";saved_X=1;saved_Y=2";
        check(!dh2_script_vm_load(vm,setup.data(),setup.size(),"explicit-OnInit-result-fixture"),"Source fixture load");
        source.calls.clear();dh2_script_value argument{};argument.type=DH2_SCRIPT_NUMBER;argument.number=22;
        check(!dh2_script_vm_call_discard_source(vm,event.second,&argument,1),dh2_script_vm_error(vm));
        Borrow b{&native.actor,&p,&tables};check(dispatch_monster_event(b,globals,event.first,22,1,native.provider(),e),e);
        check(native.calls==source.calls,std::string("Original Lua order mismatch ")+event.second+" fixture="+std::to_string(fixture));
        check(native.actor.target_id==source.actor.target_id,"Original Lua target mismatch");
        const auto buff=global(vm,"buff");check(globals.buff==(buff.type==DH2_SCRIPT_NIL?0:static_cast<std::uintptr_t>(buff.number)),"Original Lua buff global mismatch");
        check(globals.flee_flag==bool(global(vm,"m_flee_flag").boolean),"Original Lua flee flag mismatch");
        check(globals.saved_x==global(vm,"saved_X").number&&globals.saved_y==global(vm,"saved_Y").number,"Original Lua saved position mismatch");
        dh2_script_vm_destroy(vm);++cases;
    }
    unsigned actual_lizard_melee_cases=0;
    const auto actual_skill_tree_raw=p.sheets.resolved[28];
    const auto actual_skill_tree=source_skill_tree(actual_skill_tree_raw);
    check(actual_skill_tree==-1.0f,"Actual Swamp lizard source SkillTree must use FromFixed's first return");
    for(const int roll:{39,40}) {
        Fixture source;source.actor.id=1;source.actor.target_id=22;
        source.skill_raw=actual_skill_tree_raw;source.roll=roll;
        Fixture native=source;MonsterGlobals globals;globals.skill_tree_id=actual_skill_tree;
        globals.buff_speed_id=7;globals.saved_x=source.x;globals.saved_y=source.y;
        auto* vm=dh2_script_vm_create(4*1024*1024);check(vm,"Actual-row melee VM allocation");
        std::vector<Binding> bindings;bindings.reserve(std::size(functions));
        for(const auto* fn:functions){bindings.push_back({&source,fn});check(!dh2_script_vm_bind(vm,fn,callback,&bindings.back()),"Actual-row source callback bind");}
        check(!dh2_script_vm_load(vm,script.data(),script.size(),"actual-row-monster"),dh2_script_vm_error(vm));
        const char* globals_script="m_flee_flag=true;BUFF_ID=7;buff=nil;saved_X=1;saved_Y=2";
        check(!dh2_script_vm_load(vm,globals_script,std::strlen(globals_script),"actual-row-globals"),"Actual-row globals");
        dh2_script_value target{};target.type=DH2_SCRIPT_NUMBER;target.number=22;
        check(!dh2_script_vm_call_discard_source(vm,"monster_OnTargetInMeleeRange",&target,1),dh2_script_vm_error(vm));
        Borrow borrow{&native.actor,&p,&tables};
        check(dispatch_monster_event(borrow,globals,MonsterEvent::target_in_melee_range,
              22,native.actor.id,native.provider(),e),e);
        check(native.calls==source.calls,"Actual Swamp lizard melee action order differs from monster.luac");
        const std::vector<Operation> expected{Operation::stop,Operation::attack};
        check(source.calls==expected,"Actual Swamp lizard melee roll boundary changed");
        check(choose_monster_melee_action(actual_skill_tree,roll)==
              MonsterMeleeAction::attack,
              "Reusable melee choice disagrees with actual source row/roll");
        dh2_script_vm_destroy(vm);++actual_lizard_melee_cases;
    }
    std::cout<<"actual monster Lua/native differential passed cases="<<cases
             <<" actualSwampLizardMeleeCases="<<actual_lizard_melee_cases
             <<" SkillTreeRaw="<<actual_skill_tree_raw<<" SkillTree="<<actual_skill_tree
             <<" rolls39/40=Attack noRandom=true\n";
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
