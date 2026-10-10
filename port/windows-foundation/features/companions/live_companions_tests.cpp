#include "live_companions.hpp"
#include "../../../level-world/navigation_path.hpp"
#include <cassert>
#include <iostream>
using namespace dh::foundation;
using namespace dh::foundation::companions;
int main() {
    std::string error;dh2::data::CombatRandom random{};
    dh2::data::AiTables tables;tables.rows.resize(51);tables.factions.resize(11);tables.rows[50].script="rene";
    PlayableActorWorld world(tables,random);ActorState actor;actor.id=19;actor.definition_id="actual-source";actor.class_id="source-class";
    actor.health=actor.max_health=1; // explicit fixture vitals, not gameplay defaults
    actor.faction_id=0;actor.source_target_node180=0;actor.transform.position={7,8,9};
    OriginalCombatProperties props;props.sheets.resolved[0]=0;props.sheets.resolved[1]=50;
    assert(world.bind_actor(actor,props,{},error));
    actor.id=21;actor.transform.position={90,80,70};assert(world.bind_actor(actor,props,{},error));
    auto lease=std::make_shared<int>(1);dh2::character::CharacterAiPointerFieldsV105 fields;fields.master50=1021;dh2::navigation::PathObject path{};
    ActorPopulation population;PopulationActor enabled_actor;enabled_actor.definition.stableId=19;
    population.actors().push_back(std::move(enabled_actor));
    LiveWorldBindings bindings;bindings.lease=lease;bindings.world=&world;bindings.population=&population;
    bindings.owner=[&](ActorId id,LiveActorOwner& out,std::string&) {
        out.lease=lease;out.actor=world.find_actor(id);out.ai_fields=&fields;out.path=&path;out.character_identity=id+1000;return true;
    };
    bindings.host_player=[](ActorId& out,std::string&){out=21;return true;};
    bindings.native_is_dead=[](std::uintptr_t,std::uint32_t& out,std::string&){out=0;return true;};
    bindings.actor_from_identity=[](std::uintptr_t native,ActorId& out,std::string&){out=native-1000;return true;};
    auto invoke=live_invoke(bindings);Response out;Request request{Operation::has_master,19};
    assert(invoke(*world.find_actor(19),request,out,error)&&out.boolean);
    request.operation=Operation::is_master_host_player;assert(invoke(*world.find_actor(19),request,out,error)&&out.boolean);
    request.operation=Operation::has_path;assert(invoke(*world.find_actor(19),request,out,error)&&!out.boolean);
    path.count=3;assert(invoke(*world.find_actor(19),request,out,error)&&out.boolean);
    std::array<float,3> position{};assert(live_target_position(bindings,19,position,error));
    assert((position==std::array<float,3>{7,8,9}));
    auto* live=world.find_actor(19);live->source_target_node180=88;live->source_target_position184=std::array<float,3>{1,2,3};
    assert(live_target_position(bindings,19,position,error));assert((position==std::array<float,3>{1,2,3}));
    population.actors()[0].enabled=false;assert(live_target_position(bindings,19,position,error));assert((position==std::array<float,3>{7,8,9}));
    population.actors()[0].enabled=true;live->source_target_position184.reset();assert(!live_target_position(bindings,19,position,error));
    live->source_target_node180=0;request.operation=Operation::set_target;request.argument=21;
    assert(!invoke(*live,request,out,error)&&live->target_id==invalid_actor_id);
    bindings.source_operation=[](ActorState&,const Request&,Response&,std::string&){return true;};
    invoke=live_invoke(bindings);assert(!invoke(*live,request,out,error)); // successful no-op forbidden
    bindings.source_operation=[&](ActorState& same,const Request& r,Response&,std::string&){
        assert(&same==world.find_actor(19));
        if(r.operation==Operation::set_target)same.target_id=r.argument;
        if(r.operation==Operation::clear_target)same.target_id=invalid_actor_id;
        return true;
    };
    invoke=live_invoke(bindings);assert(invoke(*live,request,out,error)&&live->target_id==21);
    request.operation=Operation::clear_target;assert(invoke(*live,request,out,error)&&live->target_id==invalid_actor_id);
    request.operation=Operation::set_master;request.argument=19;assert(invoke(*live,request,out,error)&&fields.master50==1019);
    ActorState detached=*live;request.operation=Operation::has_master;assert(!invoke(detached,request,out,error));
    bindings.owner=[&](ActorId,LiveActorOwner& owner,std::string&){owner.lease=lease;owner.actor=&detached;owner.character_identity=1019;return true;};
    invoke=live_invoke(bindings);assert(!invoke(*live,request,out,error));
    std::cout<<"companion SAME live world/owner/path/publication/target-node tests passed\n";
}
