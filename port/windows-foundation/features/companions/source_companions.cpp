#include "source_companions.hpp"

namespace dh::foundation::companions {
const dh2::data::AiProps* row(const Borrow& b) noexcept {
    if(!b.actor||!b.properties||!b.tables)return nullptr;
    return dh2::data::ai_props(*b.tables,b.properties->sheets.resolved[1]);
}
namespace {
struct Calls {
    ActorState& actor; const Invoke& invoke; std::string& error;
    bool call(Operation op,Response& out,ActorId subject=invalid_actor_id,
              ActorId argument=invalid_actor_id,std::array<float,3> vector={},std::int32_t integer=0,std::uintptr_t token=0) {
        if(!invoke){error="Required same-owner companion source service";return false;}
        out={}; Request request{op,subject==invalid_actor_id?actor.id:subject,argument,vector,integer,token};
        if(invoke(actor,request,out,error))return true;
        if(error.empty())error="Required companion source operation "+std::to_string(static_cast<int>(op));
        return false;
    }
    bool command(Operation op,ActorId argument=invalid_actor_id) {Response out;return call(op,out,actor.id,argument);}
    bool moving(bool& result) {
        Response state,constant;
        if(!call(Operation::state,state)||!call(Operation::move_state_constant,constant))return false;
        result=state.integer==constant.integer;return true;
    }
};
bool admit(const FaeryBorrow& b,std::string& error) {
    if(!b.actor||b.actor->id==invalid_actor_id||!b.master||!b.skin_id||!b.admit_native_script){
        error="Required same live native AISFaery receiver/master/skin/selector";return false;
    }
    if(b.admit_native_script(*b.actor,error))return true;
    if(error.empty())error="Required proved native AISFaery script admission";
    return false;
}
}
bool dispatch(const Borrow& b,Globals& g,Event event,ActorId payload,const Invoke& invoke,std::string& error) {
    error.clear();const auto* props=row(b);
    if(!b.actor||b.actor->id==invalid_actor_id||!props||
       (props->script!="follower"&&props->script!="attacking_follower")) {
        error="Required actual selected follower/attacking_follower AI script";return false;
    }
    const bool attacking=props->script=="attacking_follower";
    Calls c{*b.actor,invoke,error};Response r;bool moving;
    switch(event) {
    case Event::friend_spotted:
        if(!c.call(Operation::has_master,r))return false;
        if(r.boolean)return true;
        if(!c.command(Operation::set_master,payload))return false;
        g.master=payload;return true;
    case Event::enemy_spotted:
        if(!attacking||!g.can_attack)return true;
        if(!c.call(Operation::has_target,r))return false;
        if(r.boolean)return true;
        if(!c.command(Operation::set_target,payload))return false;
        g.target=payload;return true;
    case Event::master_in_sight:return true; // no callback registered in these two Lua scripts
    case Event::master_out_of_sight:
        if(attacking){g.can_attack=false;if(!c.command(Operation::clear_target))return false;}
        return c.command(Operation::warp_behind,g.master);
    case Event::master_out_of_range:
        if(!c.moving(moving))return false;
        if(!moving&&!c.command(Operation::move_to,g.master))return false;
        if(attacking)g.can_attack=false;
        return c.command(Operation::clear_target);
    case Event::master_in_ranged_range:
    case Event::master_in_close_range:
    case Event::master_in_melee_range:
        if(attacking&&g.can_attack)return true;
        if(!c.command(Operation::stop))return false;
        if(attacking)g.can_attack=true;
        return true;
    case Event::target_died:
    case Event::target_out_of_sight:
        return !attacking||c.command(Operation::clear_target);
    case Event::target_out_of_range:
        return !attacking||c.command(Operation::move_to,g.target);
    case Event::target_in_ranged_range:
        if(!attacking)return true;
        if(!c.call(Operation::can_attack_from_range,r))return false;
        if(r.boolean)return c.command(Operation::stop)&&c.command(Operation::attack,g.target);
        if(!c.moving(moving))return false;
        return moving||c.command(Operation::move_to,g.target);
    case Event::target_in_close_range:
        if(!attacking)return true;
        if(!c.moving(moving))return false;
        if(moving)return true;
        if(!c.call(Operation::can_attack_from_range,r))return false;
        if(r.boolean)return c.command(Operation::flee,g.target);
        if(!c.call(Operation::can_attack_in_melee,r))return false;
        return c.command(r.boolean?Operation::move_to:Operation::trace_unable_to_attack,g.target);
    case Event::target_in_melee_range:
        if(!attacking)return true;
        if(!c.call(Operation::can_attack_in_melee,r))return false;
        if(r.boolean)return c.command(Operation::stop)&&c.command(Operation::attack,g.target);
        if(!c.moving(moving))return false;
        return moving||c.command(Operation::flee,g.target);
    }
    error="Unknown companion event";return false;
}
namespace {
bool rene_admit(const Borrow& b,const ReneGlobals& g,std::string& error) {
    const auto* props=row(b);
    if(b.actor&&b.actor->id!=invalid_actor_id&&props&&props->script=="rene"&&g.move_state&&g.haste_id)return true;
    error="Required actual selected rene AI script and source CST_MOVE/OID_HASTE producers";return false;
}
bool rene_drop(Calls& c,ReneGlobals& g) {
    if(g.target==invalid_actor_id)return true;
    g.target=invalid_actor_id;return c.command(Operation::clear_target);
}
bool rene_caught(Calls& c,ReneGlobals& g) {
    if(!g.is_catching_up)return true;
    g.is_catching_up=false;g.catchup_timer=0;Response r;
    return c.call(Operation::remove_haste_buff,r,c.actor.id,invalid_actor_id,{},*g.haste_id)&&
        c.call(Operation::enable_collisions,r,c.actor.id,invalid_actor_id,{},1)&&c.command(Operation::stop);
}
bool rene_move(Calls& c,ReneGlobals& g) {
    if(g.master==invalid_actor_id)return true;
    if(!c.command(Operation::move_to,g.master))return false;
    Response r;if(!c.call(Operation::has_path,r))return false;
    return r.boolean||c.command(Operation::warp_to_actor,g.master);
}
}
bool dispatch_rene(const Borrow& b,ReneGlobals& g,Event event,ActorId payload,const Invoke& invoke,std::string& error) {
    error.clear();if(!rene_admit(b,g,error))return false;
    Calls c{*b.actor,invoke,error};Response r;
    switch(event) {
    case Event::friend_spotted:
        if(!c.call(Operation::has_master,r))return false;
        if(r.boolean)return true;
        if(!c.command(Operation::set_master,payload))return false;
        g.master=payload;return true;
    case Event::master_out_of_sight:
        if(!g.is_catching_up)g.is_far=true;
        return rene_drop(c,g);
    case Event::master_in_sight:
    case Event::master_in_ranged_range:
        if(g.target==invalid_actor_id&&!g.is_catching_up)g.is_far=true;
        return true;
    case Event::master_in_close_range:
        g.is_far=false;
        if(g.catchup_timer||!g.is_catching_up)return true;
        if(!c.call(Operation::start_caught_up_timer,r,b.actor->id,invalid_actor_id,{},200))return false;
        g.catchup_timer=r.token;return true;
    case Event::enemy_spotted:
        if(g.is_far)return true;
        if(!rene_caught(c,g))return false;
        if(g.is_far||g.is_catching_up||g.target!=invalid_actor_id)return true;
        g.target=payload;return c.command(Operation::set_target,payload);
    case Event::target_died:
    case Event::target_out_of_sight:return rene_drop(c,g);
    case Event::target_out_of_range:return c.command(Operation::move_to,g.target);
    case Event::target_in_ranged_range:
        if(!c.call(Operation::state,r))return false;
        if(r.integer==*g.move_state&&!c.command(Operation::stop))return false;
        return c.command(Operation::attack_current_target);
    case Event::target_in_close_range:
        return c.command(Operation::stop)&&c.command(Operation::attack_current_target);
    default:return true; // source rene registers no other callbacks
    }
}
bool update_rene(const Borrow& b,ReneGlobals& g,const Invoke& invoke,std::string& error) {
    error.clear();if(!rene_admit(b,g,error))return false;
    Calls c{*b.actor,invoke,error};Response r;bool acquire;
    if(!c.call(Operation::has_master,r))return false;
    acquire=!r.boolean;
    if(!acquire){if(!c.call(Operation::is_master_host_player,r))return false;acquire=!r.boolean;}
    if(acquire||g.master==invalid_actor_id) {
        if(!c.call(Operation::host_player,r))return false;
        g.master=r.actor;if(!c.command(Operation::set_master,g.master))return false;
    }
    if(g.is_catching_up){if(!rene_move(c,g))return false;}
    else if(g.is_far&&g.master!=invalid_actor_id) {
        g.is_catching_up=true;g.catch_time=0;
        if(!c.call(Operation::create_haste_buff,r,b.actor->id,invalid_actor_id,{},*g.haste_id))return false;
        const auto buff=r.token;
        if(!c.call(Operation::apply_haste_buff,r,b.actor->id,invalid_actor_id,{},*g.haste_id,buff)||
           !c.call(Operation::enable_collisions,r,b.actor->id,invalid_actor_id,{},0)||!rene_move(c,g))return false;
    }
    if(!c.call(Operation::get_target,r))return false;
    g.target=r.actor;return true;
}
bool rene_caught_up(const Borrow& b,ReneGlobals& g,const Invoke& invoke,std::string& error) {
    error.clear();if(!rene_admit(b,g,error))return false;
    Calls c{*b.actor,invoke,error};return rene_caught(c,g);
}
bool dispatch_faery(const FaeryBorrow& b,Event event,ActorId payload,const Invoke& invoke,std::string& error) {
    error.clear();if(!admit(b,error))return false;
    Calls c{*b.actor,invoke,error};Response r,p,q;
    switch(event) {
    case Event::friend_spotted:
        if(*b.master!=invalid_actor_id)return true;
        if(payload==invalid_actor_id){error="Required actual friend Character for AISFaery association";return false;}
        if(!c.call(Operation::faery_association,r,payload))return false;
        if(r.actor!=invalid_actor_id)return true;
        if(!c.command(Operation::set_master,payload))return false;
        *b.master=payload;
        if(!c.call(Operation::set_faery_association,r,payload,b.actor->id))return false;
        if(!c.call(Operation::current_faery_id,r,payload))return false;
        return c.call(Operation::change_faery,p,payload,invalid_actor_id,{},r.integer);
    case Event::master_in_ranged_range:return c.command(Operation::stop);
    case Event::master_in_close_range: // _ZTV8AISFaery+8+124 = 0x3de318
    case Event::master_in_melee_range:
        if(!c.call(Operation::is_idle,r))return false;
        if(!r.boolean)return true;
        if(*b.master==invalid_actor_id)return true;
        if(!c.call(Operation::is_idle,r,*b.master))return false;
        if(!r.boolean)return true;
        if(!c.call(Operation::look_at_vector,q,*b.master)||!c.call(Operation::target_position,p,*b.master))return false;
        for(unsigned i=0;i<3;++i)p.vector[i]+=(-q.vector[i])*200.0f;
        return c.call(Operation::head_to,r,b.actor->id,invalid_actor_id,p.vector);
    case Event::master_out_of_range:
        if(*b.master==invalid_actor_id){error="Required actual AISFaery master for HeadTowards";return false;}
        if(!c.call(Operation::target_position,p,*b.master)||!c.call(Operation::target_position,q))return false;
        for(unsigned i=0;i<3;++i)p.vector[i]-=q.vector[i];
        return c.call(Operation::head_towards,r,b.actor->id,invalid_actor_id,p.vector);
    case Event::master_out_of_sight:
        if(*b.master==invalid_actor_id){error="Required actual AISFaery master for WarpTo";return false;}
        if(!c.call(Operation::target_position,p,*b.master))return false;
        return c.call(Operation::warp_to,r,b.actor->id,invalid_actor_id,p.vector);
    default:return c.call(Operation::default_event,r,b.actor->id,payload,{},static_cast<std::int32_t>(event));
    }
}
bool update_faery(const FaeryBorrow& b,const Invoke& invoke,std::string& error) {
    error.clear();if(!admit(b,error))return false;
    Calls c{*b.actor,invoke,error};Response r,p;
    if(!c.command(Operation::default_update))return false;
    if(*b.master==invalid_actor_id)return true;
    if(!c.call(Operation::has_visual,r))return false;
    if(!r.boolean)return true;
    if(!c.call(Operation::current_faery_id,r,*b.master))return false;
    if(*b.skin_id==r.integer)return true;
    if(!c.call(Operation::current_faery_id,p,*b.master))return false;
    *b.skin_id=p.integer;
    return c.call(Operation::set_modular_skin,r,b.actor->id,invalid_actor_id,{},p.integer);
}
}
