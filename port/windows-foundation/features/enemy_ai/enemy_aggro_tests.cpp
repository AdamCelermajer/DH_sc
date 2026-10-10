#include "monster_decisions.hpp"
#include "../../asset_catalog.hpp"
#include "../../playable_actor_world.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
using namespace dh::foundation::enemy_ai;
using namespace dh2::character;
static void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
int main(int argc,char** argv){try {
    check(argc==2,"Actual original asset root required");AssetCatalog assets(argv[1]);std::string e;
    OriginalPropertyDatabase db;dh2::data::AiTables ai;
    check(load_original_property_tables(assets,"original-cache/data/pydata/",db,e),e);
    check(load_original_ai_tables(assets,"original-cache/data/pydata/",ai,e),e);
    OriginalCombatProperties props;check(build_original_combat_properties(db,"Swamp_LizadMan_Type1",{},{},{},props,e),e);
    ActorState actor;actor.id=1;actor.definition_id="Swamp_LizadMan_Type1";
    Borrow b{&actor,&props,&ai};const auto* authored=row(b);check(authored&&authored->script=="monster","Original script mismatch");
    TargetOwner16 owner{actor.id,0,0,0};TargetState48 target{};target.identity=actor.id;target.owner=&owner;
    CharacterAiPointerFieldsV105 fields;AggroFrameServicesV108 services;
    std::vector<std::string> calls;float radius=0;std::uint32_t mask=0;bool tracked=false;
    // Explicit original service fixtures: not substitutes for production owners.
    services.query=[&](AggroFrameQueryV108 q,bool& out,std::string&){
        calls.push_back("query:"+std::to_string(static_cast<int>(q)));
        out=q==AggroFrameQueryV108::MyTurn||q==AggroFrameQueryV108::Monster;return true;};
    services.debug=[&](const char* name,bool& out,std::string&){calls.push_back(name);out=true;return true;};
    services.search=[&](bool t,float r,std::uint32_t f,std::vector<AggroFrameTargetV108>& out,std::string&){tracked=t;radius=r;mask=f;out={{22,1}};calls.push_back("search");return true;};
    services.relationship=[&](AggroRelationV108 r,std::uintptr_t id,bool& out,std::string&){check(id==22,"Wrong searched relation identity");out=r==AggroRelationV108::Enemy;calls.push_back("relationship");return true;};
    services.raise=[&](std::uint32_t event,std::uintptr_t id,std::string& error){
        check(event==9&&id==22,"Invented acquisition event");calls.push_back("raise9");
        MonsterGlobals globals;globals.skill_tree_id=original_signed256(props.sheets.resolved[28]);
        return dispatch_monster_event(b,globals,MonsterEvent::enemy_spotted,id,0,
            [&](ActorState& same,const Request& q,Response&,std::string&){check(&same==&actor,"Acquisition copied actor");
                if(q.operation==Operation::set_target){same.target_id=q.target;target.target=q.target;calls.push_back("SetTarget");return true;}
                if(q.operation==Operation::head_to){calls.push_back("HeadTo");return true;}return false;},error);
    };
    check(update_aggro(b,fields,target,services,e),e);
    check(!tracked&&radius==authored->view_radius_no_aggro&&mask==0x80000001u,
        "Fresh source aggro search changed authored no-aggro radius/mask/list");
    check(actor.target_id==22&&target.target==22&&calls.back()=="HeadTo","Spotted source event did not publish same target");
    // Unknown required source services fail when reached, with no guessed timing.
    target.target=actor.target_id=0;services.search={};check(!update_aggro(b,fields,target,services,e)&&!e.empty(),"Missing real source search accepted");
    target.target=22;check(!update_aggro(b,fields,target,services,e)&&actor.target_id==0,"Divergent target authority accepted");
    std::cout<<"source enemy aggro tests passed aiRow="<<props.sheets.resolved[1]<<" radius="<<radius<<" flags="<<mask<<'\n';
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
