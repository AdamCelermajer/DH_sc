#include "source_companions.hpp"
#include <cassert>
#include <iostream>
using namespace dh::foundation;
using namespace dh::foundation::companions;
int main() {
    ActorState actor;actor.id=17;
    OriginalCombatProperties props;props.sheets.resolved[1]=0;
    dh2::data::AiTables tables;tables.rows.resize(1);tables.rows[0].script="follower";
    Borrow borrow{&actor,&props,&tables};Globals globals;std::string error;
    std::vector<Request> calls;bool moving=false,has_master=false,can_range=true,idle=true;
    Operation failure=Operation::trace_unable_to_attack;
    Invoke invoke=[&](ActorState& receiver,const Request& request,Response& out,std::string&) {
        assert(&receiver==&actor);calls.push_back(request);
        if(request.operation==failure)return false;
        switch(request.operation) {
        case Operation::has_master:out.boolean=has_master;break;
        case Operation::state:out.integer=moving?7:3;break;
        case Operation::move_state_constant:out.integer=7;break;
        case Operation::set_target:actor.target_id=request.argument;break;
        case Operation::clear_target:actor.target_id=invalid_actor_id;break;
        case Operation::has_target:out.boolean=actor.target_id!=invalid_actor_id;break;
        case Operation::can_attack_from_range:out.boolean=can_range;break;
        case Operation::can_attack_in_melee:out.boolean=true;break;
        case Operation::is_idle:out.boolean=idle;break;
        case Operation::target_position:out.vector=request.subject==22?std::array<float,3>{100,200,300}:std::array<float,3>{1,2,3};break;
        case Operation::look_at_vector:out.vector={0,1,0};break;
        case Operation::current_faery_id:out.integer=4;break;
        case Operation::has_visual:out.boolean=true;break;
        case Operation::is_master_host_player:out.boolean=true;break;
        case Operation::host_player:out.actor=22;break;
        case Operation::get_target:out.actor=actor.target_id;break;
        case Operation::create_haste_buff:out.token=991;break;
        case Operation::start_caught_up_timer:out.token=771;break;
        default:break;
        }return true;
    };
    assert(dispatch(borrow,globals,Event::friend_spotted,22,invoke,error));
    assert(globals.master==22&&calls.size()==2&&calls.back().operation==Operation::set_master);
    has_master=true;calls.clear();assert(dispatch(borrow,globals,Event::friend_spotted,99,invoke,error));
    assert(globals.master==22&&calls.size()==1);
    calls.clear();assert(dispatch(borrow,globals,Event::master_out_of_range,0,invoke,error));
    assert(calls.size()==4&&calls[2].operation==Operation::move_to&&calls[2].argument==22&&calls[3].operation==Operation::clear_target);
    moving=true;calls.clear();assert(dispatch(borrow,globals,Event::master_out_of_range,0,invoke,error));
    assert(calls.size()==3&&calls.back().operation==Operation::clear_target);
    calls.clear();assert(dispatch(borrow,globals,Event::master_out_of_sight,0,invoke,error));
    assert(calls.size()==1&&calls[0].operation==Operation::warp_behind);
    for(Event event:{Event::master_in_ranged_range,Event::master_in_close_range,Event::master_in_melee_range}){
        calls.clear();assert(dispatch(borrow,globals,event,0,invoke,error));assert(calls.size()==1&&calls[0].operation==Operation::stop);
    }
    tables.rows[0].script="attacking_follower";moving=false;
    calls.clear();assert(dispatch(borrow,globals,Event::enemy_spotted,45,invoke,error));
    assert(globals.target==45&&actor.target_id==45&&calls.back().operation==Operation::set_target);
    calls.clear();assert(dispatch(borrow,globals,Event::target_in_ranged_range,0,invoke,error));
    assert(calls.size()==3&&calls[1].operation==Operation::stop&&calls[2].operation==Operation::attack&&calls[2].argument==45);
    calls.clear();assert(dispatch(borrow,globals,Event::master_out_of_sight,0,invoke,error));
    assert(!globals.can_attack&&actor.target_id==invalid_actor_id&&calls.size()==2);
    calls.clear();assert(dispatch(borrow,globals,Event::enemy_spotted,99,invoke,error));assert(calls.empty());
    failure=Operation::stop;calls.clear();assert(!dispatch(borrow,globals,Event::master_in_close_range,0,invoke,error));
    assert(!globals.can_attack&&!error.empty());
    failure=Operation::trace_unable_to_attack;assert(dispatch(borrow,globals,Event::master_in_close_range,0,invoke,error));assert(globals.can_attack);
    calls.clear();assert(dispatch(borrow,globals,Event::master_in_close_range,0,invoke,error));assert(calls.empty());
    failure=Operation::clear_target;globals.can_attack=true;calls.clear();
    assert(!dispatch(borrow,globals,Event::master_out_of_sight,0,invoke,error));
    assert(!globals.can_attack&&calls.size()==1&&!error.empty()); // source prefix retained
    tables.rows[0].script="monster";calls.clear();assert(!dispatch(borrow,globals,Event::friend_spotted,22,invoke,error));assert(calls.empty());
    tables.rows[0].script="follower";assert(!dispatch(borrow,globals,Event::friend_spotted,22,{},error));
    failure=Operation::trace_unable_to_attack;
    ActorId master=22;std::int32_t skin=1;
    FaeryBorrow faery{&actor,&master,&skin,[](ActorState&,std::string&){return true;}};
    calls.clear();assert(dispatch_faery(faery,Event::master_in_close_range,0,invoke,error));
    assert((calls.size()==5&&calls.back().operation==Operation::head_to&&calls.back().vector==std::array<float,3>({100,0,300})));
    calls.clear();assert(dispatch_faery(faery,Event::master_out_of_range,0,invoke,error));
    assert((calls.size()==3&&calls.back().operation==Operation::head_towards&&calls.back().vector==std::array<float,3>({99,198,297})));
    idle=false;calls.clear();assert(dispatch_faery(faery,Event::master_in_melee_range,0,invoke,error));assert(calls.size()==1);
    calls.clear();assert(update_faery(faery,invoke,error));assert(skin==4&&calls.size()==5&&calls.back().operation==Operation::set_modular_skin);
    calls.clear();assert(update_faery(faery,invoke,error));assert(calls.size()==3);
    master=invalid_actor_id;calls.clear();assert(!dispatch_faery(faery,Event::master_out_of_sight,0,invoke,error));assert(calls.empty());
    faery.admit_native_script={};assert(!update_faery(faery,invoke,error));
    tables.rows[0].script="rene";ReneGlobals rene;rene.move_state=7;rene.haste_id=19;
    has_master=false;calls.clear();assert(update_rene(borrow,rene,invoke,error));
    assert(rene.master==22&&calls.size()==4&&calls[1].operation==Operation::host_player);
    has_master=true;rene.target=45;actor.target_id=45;calls.clear();
    assert(dispatch_rene(borrow,rene,Event::master_out_of_sight,0,invoke,error));
    assert(rene.is_far&&rene.target==invalid_actor_id&&actor.target_id==invalid_actor_id);
    calls.clear();assert(update_rene(borrow,rene,invoke,error));
    assert(rene.is_catching_up&&calls.size()==9);
    assert(calls[2].operation==Operation::create_haste_buff&&calls[2].integer==19);
    assert(calls[3].operation==Operation::apply_haste_buff&&calls[3].token==991);
    assert(calls[4].operation==Operation::enable_collisions&&calls[4].integer==0);
    assert(calls[5].operation==Operation::move_to&&calls[6].operation==Operation::has_path&&calls[7].operation==Operation::warp_to_actor);
    calls.clear();assert(dispatch_rene(borrow,rene,Event::master_in_close_range,0,invoke,error));
    assert(!rene.is_far&&rene.catchup_timer==771&&calls.size()==1&&calls[0].integer==200);
    calls.clear();assert(dispatch_rene(borrow,rene,Event::master_in_close_range,0,invoke,error));assert(calls.empty());
    calls.clear();assert(rene_caught_up(borrow,rene,invoke,error));
    assert(!rene.is_catching_up&&!rene.catchup_timer&&calls.size()==3&&calls[1].integer==1&&calls.back().operation==Operation::stop);
    failure=Operation::set_target;calls.clear();assert(!dispatch_rene(borrow,rene,Event::enemy_spotted,88,invoke,error));
    assert(rene.target==88&&calls.size()==1); // Lua caches before SetTarget
    failure=Operation::trace_unable_to_attack;moving=true;calls.clear();
    assert(dispatch_rene(borrow,rene,Event::target_in_ranged_range,0,invoke,error));
    assert(calls.size()==3&&calls[1].operation==Operation::stop&&calls[2].operation==Operation::attack_current_target);
    std::cout<<"source companion event/branch/shared-owner/failure-prefix tests passed\n";
}
