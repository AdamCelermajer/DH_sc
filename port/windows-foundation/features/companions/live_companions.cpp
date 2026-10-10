#include "live_companions.hpp"
#include "../../original_actor_target_position.hpp"
#include "../../../level-world/navigation_path.hpp"
#include <algorithm>
#include <cstring>

namespace dh::foundation::companions {
bool borrow_live_owner(const LiveWorldBindings& b,ActorId id,LiveActorOwner& out,std::string& error) {
    if(!b.lease||!b.world||!b.owner){error="Required SAME live companion world/owner lease";return false;}
    if(b.session&&b.session->world()!=b.world){error="Companion CombatSession differs from actual actor world";return false;}
    auto* actor=b.world->find_actor(id);
    if(!actor){error="Companion actor absent from SAME live world";return false;}
    out={};if(!b.owner(id,out,error))return false;
    if(!out.lease||out.actor!=actor||!out.character_identity){error="Companion owner does not borrow SAME live ActorState/Character";return false;}
    return true;
}
namespace {
bool enabled(const LiveWorldBindings& b,ActorId id,std::uint8_t& value,std::string& error) {
    if(b.population) {
        const auto& actors=b.population->actors();
        auto found=std::find_if(actors.begin(),actors.end(),[&](const PopulationActor& a){return a.definition.stableId==id;});
        if(found!=actors.end()){value=found->enabled?1:0;return true;}
    }
    if(b.enabled)return b.enabled(id,value,error);
    error="Required actual Character enabled80 producer";return false;
}
const dh2::data::AiProps* live_row(const LiveWorldBindings& b,ActorId id) {
    const auto* props=b.world?b.world->combat_properties(id):nullptr;
    return props?dh2::data::ai_props(b.world->factions(),props->sheets.resolved[1]):nullptr;
}
}
bool live_target_position(const LiveWorldBindings& b,ActorId id,std::array<float,3>& out,std::string& error) {
    LiveActorOwner actual;if(!borrow_live_owner(b,id,actual,error))return false;
    const OriginalTargetPosition* chosen{};
    if(!original_get_target_position(actual.actor->source_target_node180,actual.actor->source_target_position184,
        actual.actor->transform.position,[&](std::uint8_t& flag,std::string& e){return enabled(b,id,flag,e);},chosen,error))return false;
    out=*chosen;return true;
}
Invoke live_invoke(LiveWorldBindings bindings) {
    return [b=std::move(bindings)](ActorState& actor,const Request& request,Response& response,std::string& error) {
        LiveActorOwner actual;if(!borrow_live_owner(b,actor.id,actual,error))return false;
        if(actual.actor!=&actor){error="Companion operation received detached ActorState";return false;}
        response={};
        switch(request.operation) {
        case Operation::has_master:
            if(!actual.ai_fields){error="Required original CharAI pointer fields";return false;}
            response.boolean=actual.ai_fields->master50!=0;return true;
        case Operation::has_target:response.boolean=actor.target_id!=invalid_actor_id;return true;
        case Operation::get_target:response.actor=actor.target_id;return true;
        case Operation::host_player:
            if(!b.host_player){error="Required source GetHostPlayer producer";return false;}
            return b.host_player(response.actor,error);
        case Operation::is_master_host_player: {
            if(!actual.ai_fields){error="Required original CharAI pointer fields";return false;}
            if(!actual.ai_fields->master50){response.boolean=false;return true;}
            if(!b.host_player){error="Required host-player source producer";return false;}
            ActorId host{};if(!b.host_player(host,error))return false;
            LiveActorOwner host_owner;if(!borrow_live_owner(b,host,host_owner,error))return false;
            response.boolean=actual.ai_fields->master50==host_owner.character_identity;return true;
        }
        case Operation::set_master: {
            SourceMasterBorrow master;if(!borrow_source_master(b,actor.id,master,error))return false;
            std::uintptr_t peer_identity{};
            if(request.argument!=invalid_actor_id){LiveActorOwner peer;if(!borrow_live_owner(b,request.argument,peer,error))return false;peer_identity=peer.character_identity;}
            return set_source_master(master,peer_identity,error);
        }
        case Operation::state:
            if(!b.session){error="Required actual CombatSession source FSM state";return false;}
            response.integer=b.session->original_actor_state(actor.id);
            if(response.integer<0){error="Actual source FSM state is unbound";return false;}return true;
        case Operation::target_position:return live_target_position(b,request.subject,response.vector,error);
        case Operation::has_path:
            if(!actual.path){error="Required SAME live source PF path-list owner";return false;}
            response.boolean=actual.path->count!=0;return true;
        default:
            if(!b.source_operation){error="Required whole original companion controller/skill/timer/native operation";return false;}
            if(!b.source_operation(actor,request,response,error)){
                if(error.empty())error="Required whole original companion source operation "+std::to_string(static_cast<int>(request.operation));
                return false;
            }
            if(request.operation==Operation::set_target&&actor.target_id!=request.argument){error="Source SetTarget failed SAME actor target publication";return false;}
            if(request.operation==Operation::clear_target&&actor.target_id!=invalid_actor_id){error="Source ClearTarget failed SAME actor target publication";return false;}
            return true;
        }
    };
}
bool borrow_source_master(const LiveWorldBindings& b,ActorId id,SourceMasterBorrow& out,std::string& error) {
    LiveActorOwner actual;if(!borrow_live_owner(b,id,actual,error))return false;
    if(!actual.ai_fields){error="Required SAME original CharAI master/alive/sight fields";return false;}
    out={};out.fields=actual.ai_fields;out.same_character=actual.character_identity;out.tables=&b.world->factions();
    out.get_ai_id=[b,id](std::int32_t& ai_id,std::string& e){
        const auto* props=b.world->combat_properties(id);if(!props){e="Required actual resolved Character AI property";return false;}
        ai_id=props->sheets.resolved[1];return true;
    };
    out.is_dead=b.native_is_dead;
    out.target_position=[b](std::uintptr_t native,std::array<float,3>& position,std::string& e){
        if(!b.actor_from_identity){e="Required SAME native Character identity resolver";return false;}
        ActorId actor{};if(!b.actor_from_identity(native,actor,e))return false;
        LiveActorOwner owner;if(!borrow_live_owner(b,actor,owner,e))return false;
        if(owner.character_identity!=native){e="Source master identity resolver did not preserve SAME Character";return false;}
        return live_target_position(b,actor,position,e);
    };
    out.hosting_player_character=[b](std::uintptr_t& native,std::string& e){
        if(!b.host_player){e="Required original hosting player producer";return false;}
        ActorId host{};if(!b.host_player(host,e))return false;
        LiveActorOwner actual;if(!borrow_live_owner(b,host,actual,e))return false;
        native=actual.character_identity;return true;
    };
    return true;
}
}
