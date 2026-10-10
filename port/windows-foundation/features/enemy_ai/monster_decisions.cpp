#include "monster_decisions.hpp"

namespace dh::foundation::enemy_ai {
const dh2::data::AiProps* row(const Borrow& b) noexcept {
    if(!b.actor||!b.properties||!b.tables)return nullptr;
    return dh2::data::ai_props(*b.tables,b.properties->sheets.resolved[1]);
}
MonsterMeleeAction choose_monster_melee_action(float skill_tree_id,
                                               std::int32_t source_roll) noexcept {
    return skill_tree_id > -1.0f && source_roll < 40
        ? MonsterMeleeAction::do_skill_zero : MonsterMeleeAction::attack;
}
bool search_sneak_gate(const OriginalCombatProperties& owner,const OriginalCombatProperties& candidate) noexcept {
    return owner.sheets.resolved[199]>=candidate.sheets.resolved[198];
}
bool source_target_event(std::uint32_t event,MonsterEvent& result) noexcept {
    switch(event) {
    case 9:result=MonsterEvent::enemy_spotted;return true;
    case 10:result=MonsterEvent::target_died;return true;
    case 12:result=MonsterEvent::target_out_of_sight;return true;
    case 13:result=MonsterEvent::target_in_sight;return true;
    case 14:result=MonsterEvent::target_out_of_range;return true;
    case 15:result=MonsterEvent::target_in_ranged_range;return true;
    case 16:result=MonsterEvent::target_in_close_range;return true;
    case 17:result=MonsterEvent::target_in_melee_range;return true;
    default:return false;
    }
}
namespace {
bool valid(const Borrow& b,std::string& e) {
    if(!b.actor||b.actor->id==invalid_actor_id||!b.properties||!b.tables){e="Required same live enemy actor/properties/AI tables";return false;}
    const auto* r=row(b);
    if(!r||r->script!="monster"){e="Required actual selected monster script event provider";return false;}
    return true;
}
struct Calls {
    const Borrow& b;MonsterGlobals& g;const Invoke& invoke;std::string& error;
    bool call(Operation operation,Response& response,ActorId target=invalid_actor_id,
              const std::string& name={},std::int32_t a=0,std::int32_t c=0,std::uintptr_t token=0) {
        if(!invoke){error="Required original monster script service";return false;}
        response={};const Request request{operation,target,name,a,c,token};
        if(invoke(*b.actor,request,response,error))return true;
        if(error.empty())error="Required original monster service operation "+std::to_string(static_cast<int>(operation));
        return false;
    }
    bool command(Operation op,ActorId target=invalid_actor_id) {Response r;return call(op,r,target);}
    bool skill_or_attack() {
        Response r;
        if(g.skill_tree_id>-1)return call(Operation::do_skill,r,invalid_actor_id,{},0);
        return command(Operation::attack,b.actor->target_id);
    }
    bool remove_buff() {
        if(!g.buff)return true;
        Response r;
        if(!call(Operation::remove_buff,r,invalid_actor_id,{},g.buff_speed_id,0,g.buff))return false;
        g.buff=0;return true;
    }
};
}
bool dispatch_monster_event(const Borrow& b,MonsterGlobals& globals,MonsterEvent event,
    ActorId payload,ActorId defender,const Invoke& invoke,std::string& error) {
    error.clear();if(!valid(b,error))return false;
    Calls c{b,globals,invoke,error};Response r;
    switch(event) {
    case MonsterEvent::enemy_spotted:
    case MonsterEvent::target_hit:
        if(event==MonsterEvent::target_hit&&defender!=b.actor->id)return true;
        if(b.actor->target_id!=invalid_actor_id)return true;
        if(!c.command(Operation::set_target,payload))return false;
        return c.command(Operation::head_to,payload);
    case MonsterEvent::target_died:
        return c.command(Operation::stop)&&c.command(Operation::clear_target);
    case MonsterEvent::target_out_of_sight:
        if(!c.command(Operation::stop))return false;
        // Source captures GetTarget AFTER synchronous Stop.
        if(b.actor->target_id!=invalid_actor_id&&!c.command(Operation::clear_aggro,b.actor->target_id))return false;
        return c.command(Operation::clear_target);
    case MonsterEvent::target_in_sight:return true; // Original body only traces(false).
    case MonsterEvent::target_out_of_range: {
        if(!c.call(Operation::state,r))return false;const auto state=r.integer;
        if(!c.call(Operation::state_constant,r,invalid_actor_id,"Idle"))return false;
        if(state!=r.integer)return true;
        if(!c.call(Operation::has_path,r))return false;
        return r.integer!=0||c.command(Operation::move_to,b.actor->target_id);
    }
    case MonsterEvent::target_in_ranged_range:
        if(globals.buff){if(!c.remove_buff())return false;globals.flee_flag=true;}
        return c.command(Operation::stop)&&c.skill_or_attack();
    case MonsterEvent::target_in_melee_range: {
        if(!c.command(Operation::stop))return false;
        std::int32_t source_roll=40;
        if(globals.skill_tree_id>-1) {
            if(!c.call(Operation::random,r,invalid_actor_id,{},0,100))return false;
            source_roll=r.integer;
        }
        if(choose_monster_melee_action(globals.skill_tree_id,source_roll)==MonsterMeleeAction::do_skill_zero)
            return c.call(Operation::do_skill,r,invalid_actor_id,{},0);
        return c.command(Operation::attack,b.actor->target_id);
    }
    case MonsterEvent::target_in_close_range: {
        if(!c.call(Operation::state,r))return false;const auto state=r.integer;
        if(!globals.buff) {
            if(!c.call(Operation::create_buff,r,invalid_actor_id,{},globals.buff_speed_id))return false;
            globals.buff=r.token; // Lua assigns before ApplyBuff, even on later failure.
            if(!c.call(Operation::apply_buff,r,invalid_actor_id,{},globals.buff_speed_id,0,globals.buff))return false;
        }
        if(!c.call(Operation::position,r))return false;const auto x=r.x,y=r.y;
        if(!globals.flee_flag){if(!c.skill_or_attack())return false;}
        else {
            bool stuck=false;
            if(x==globals.saved_x&&y==globals.saved_y) {
                if(!c.call(Operation::state_constant,r,invalid_actor_id,"Move"))return false;
                if(state==r.integer) {
                    if(!c.call(Operation::state_time,r))return false;
                    stuck=r.time_ms>500;
                }
            }
            if(stuck){globals.flee_flag=false;if(!c.command(Operation::stop))return false;}
            else if(!c.command(Operation::flee,b.actor->target_id))return false;
        }
        globals.saved_x=x;globals.saved_y=y;return true;
    }
    case MonsterEvent::flee:globals.flee_flag=false;return true;
    case MonsterEvent::died:
    case MonsterEvent::terminate:return c.remove_buff();
    }
    error="Unsupported original monster event";return false;
}
} // namespace dh::foundation::enemy_ai

