#include "monster_decisions.hpp"
#include "../../asset_catalog.hpp"
#include "../../playable_actor_world.hpp"
#include "../../../script-runtime/script_scalar_bindings.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
using namespace dh::foundation::enemy_ai;
namespace {
void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
float source_skill_tree(const std::int32_t raw) {
    dh2_script_value input{};input.type=DH2_SCRIPT_NUMBER;input.number=static_cast<float>(raw);
    dh2_script_value output[2]{};std::uint32_t returned=0;char error[128]{};
    check(dh2_script_scalar_from_fixed(nullptr,&input,1,output,2,&returned,error,sizeof(error))==0 && returned==2,
          "Actual FromFixed source scalar rejected SkillTree");
    return output[0].number;
}
struct Services {
    ActorState* same{};std::vector<Operation> calls;int state=3,roll=40;
    bool path=false;std::uint32_t time=500;float x=0,y=0;
    bool fail=false;Operation missing=Operation::flee;
    ActorId stop_replacement=invalid_actor_id;
    Invoke provider(){return [this](ActorState& actor,const Request& q,Response& r,std::string& e){
        check(&actor==same,"Decision used a copied actor");calls.push_back(q.operation);
        if(fail&&q.operation==missing){e.clear();return false;}
        switch(q.operation){
        case Operation::state:r.integer=state;break;
        case Operation::state_constant:check(q.name=="Idle"||q.name=="Move","Invented AI state constant");r.integer=q.name=="Idle"?3:4;break;
        case Operation::has_path:r.integer=path;break;
        case Operation::state_time:r.time_ms=time;break;
        case Operation::position:r.x=x;r.y=y;break;
        case Operation::random:check(q.argument0==0&&q.argument1==100,"Random bounds changed");r.integer=roll;break;
        case Operation::set_target:actor.target_id=q.target;break;
        case Operation::clear_target:actor.target_id=invalid_actor_id;break;
        case Operation::stop:if(stop_replacement!=invalid_actor_id)actor.target_id=stop_replacement;break;
        case Operation::create_buff:r.token=0x1234;break;
        case Operation::attack:case Operation::move_to:case Operation::flee:case Operation::clear_aggro:
            check(q.target==actor.target_id,"Command captured stale actor target");break;
        default:break;
        }return true;
    };}
};
void expect(const Services& s,std::initializer_list<Operation> wanted,const char* label){check(s.calls==std::vector<Operation>(wanted),label);}
void decisions(const Borrow& b,MonsterGlobals g){
    auto& a=*b.actor;Services s;s.same=&a;std::string e;auto invoke=s.provider();
    auto run=[&](MonsterEvent event){s.calls.clear();return dispatch_monster_event(b,g,event,22,a.id,invoke,e);};
    a.target_id=invalid_actor_id;check(run(MonsterEvent::enemy_spotted),e);
    expect(s,{Operation::set_target,Operation::head_to},"EnemySpotted order");
    check(a.target_id==22,"EnemySpotted target not shared");
    check(run(MonsterEvent::enemy_spotted)&&s.calls.empty(),"Existing target replaced by spotted enemy");
    check(run(MonsterEvent::target_out_of_range),e);
    expect(s,{Operation::state,Operation::state_constant,Operation::has_path,Operation::move_to},"Idle no-path chase order");
    s.path=true;check(run(MonsterEvent::target_out_of_range),e);
    expect(s,{Operation::state,Operation::state_constant,Operation::has_path},"Existing path changed");
    s.path=false;s.state=5;check(run(MonsterEvent::target_out_of_range),e);
    expect(s,{Operation::state,Operation::state_constant},"Attack state attempted chase");s.state=3;
    const bool skills=g.skill_tree_id>-1;
    check(run(MonsterEvent::target_in_melee_range),e);
    if(skills)expect(s,{Operation::stop,Operation::random,Operation::attack},"Roll40 boundary must attack");
    else expect(s,{Operation::stop,Operation::attack},"No-skill melee order");
    if(skills){s.roll=39;check(run(MonsterEvent::target_in_melee_range),e);expect(s,{Operation::stop,Operation::random,Operation::do_skill},"Roll39 must skill");}
    s.state=4;s.time=500;g.saved_x=s.x;g.saved_y=s.y;g.flee_flag=true;g.buff=0;
    check(run(MonsterEvent::target_in_close_range),e);
    expect(s,{Operation::state,Operation::create_buff,Operation::apply_buff,Operation::position,Operation::state_constant,Operation::state_time,Operation::flee},"Time500 must still flee");
    check(g.flee_flag&&g.buff==0x1234,"CloseRange source globals missing");
    s.time=501;check(run(MonsterEvent::target_in_close_range),e);
    expect(s,{Operation::state,Operation::position,Operation::state_constant,Operation::state_time,Operation::stop},"Time501 stuck must stop");
    check(!g.flee_flag,"Stuck did not clear flee flag");
    check(run(MonsterEvent::target_in_ranged_range),e);
    if(skills)expect(s,{Operation::remove_buff,Operation::stop,Operation::do_skill},"Ranged skill order");
    else expect(s,{Operation::remove_buff,Operation::stop,Operation::attack},"Ranged attack order");
    check(!g.buff&&g.flee_flag,"Ranged did not reset buff/flee globals");
    s.stop_replacement=33;check(run(MonsterEvent::target_out_of_sight),e);
    expect(s,{Operation::stop,Operation::clear_aggro,Operation::clear_target},"OutSight order/reload");
    check(a.target_id==invalid_actor_id,"OutSight did not clear same target");s.stop_replacement=invalid_actor_id;
    a.target_id=22;g.buff=0;g.flee_flag=true;g.saved_x=-1;s.x=1;s.fail=true;
    check(!run(MonsterEvent::target_in_close_range)&&!e.empty(),"Missing reached flee service accepted");
    check(g.buff==0x1234&&g.saved_x==-1,"Failed prefix rolled back buff or published late saved position");
    s.fail=false;check(run(MonsterEvent::terminate),e);expect(s,{Operation::remove_buff},"Terminate buff cleanup");
}
}
int main(int argc,char** argv){try {
    check(argc==2,"Actual original asset root required");AssetCatalog assets(argv[1]);std::string e;
    OriginalPropertyDatabase database;dh2::data::AiTables tables;
    check(load_original_property_tables(assets,"original-cache/data/pydata/",database,e),e);
    check(load_original_ai_tables(assets,"original-cache/data/pydata/",tables,e),e);
    const auto buff=std::find(database.classes.names.begin(),database.classes.names.end(),"Buff_Speed");
    check(buff!=database.classes.names.end(),"Original Buff_Speed missing");
    std::size_t rows=0,skills=0;
    for(const auto& name:database.characters.names){
        OriginalCombatProperties p;
        if(!build_original_combat_properties(database,name,{},{},{},p,e))continue;
        ActorState actor;actor.id=100+rows;actor.definition_id=name;
        Borrow borrow{&actor,&p,&tables};const auto* ai=row(borrow);
        if(!ai||ai->script!="monster")continue;
        MonsterGlobals globals;globals.skill_tree_id=source_skill_tree(p.sheets.resolved[28]);
        globals.buff_speed_id=static_cast<std::int32_t>(buff-database.classes.names.begin());
        check(search_sneak_gate(p,p)==(p.sheets.resolved[199]>=p.sheets.resolved[198]),"Original sneak/detection words changed");
        decisions(borrow,globals);++rows;if(globals.skill_tree_id>-1)++skills;
    }
    check(rows>10&&skills>0,"Original generic mob/skill coverage absent");
    MonsterEvent event;check(source_target_event(17,event)&&event==MonsterEvent::target_in_melee_range&&!source_target_event(11,event),"Source event mapping invented callback");
    std::cout<<"original monster decision tests passed rows="<<rows<<" skillRows="<<skills<<'\n';
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
