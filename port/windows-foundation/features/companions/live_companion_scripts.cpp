#include "live_companions.hpp"
#include "../../../level-world/object_identity.hpp"
#include <algorithm>
#include <cstring>
namespace dh::foundation::companions {
namespace {
const dh2::data::AiProps* live_row(const LiveWorldBindings& b,ActorId id) {
 const auto* props=b.world?b.world->combat_properties(id):nullptr;
 return props?dh2::data::ai_props(b.world->factions(),props->sheets.resolved[1]):nullptr;
}
}
bool call_live_script(const LiveWorldBindings& b,ActorId id,const char* event,ActorId payload,std::string& error) {
    LiveActorOwner actual;if(!borrow_live_owner(b,id,actual,error))return false;
    const auto* row=live_row(b,id);
    if(!row||(row->script!="rene"&&row->script!="follower"&&row->script!="attacking_follower")){
        error="Required actual selected external companion AI script";return false;
    }
    if(!event||(!actual.script_owner&&!actual.script_session)){error="Required actual retained Character script callback receiver";return false;}
    dh2::character::ScriptSessionView view{};
    const bool active=actual.script_owner?actual.script_owner->active(view):actual.script_session->owner().active(view);
    if(!active||!view.identity||!view.vm||!view.aliases||
       !view.constructor_fields||view.constructor_fields->character!=actual.character_identity){
        error="Required SAME active Character-owned Lua session/alias receiver";return false;
    }
    if(!dh2_script_alias_contains(view.aliases,event)){error="Actual companion source callback is not registered";return false;}
    dh2_script_value argument{};std::uint32_t count=0;
    if(payload!=invalid_actor_id) {
        LiveActorOwner peer;if(!borrow_live_owner(b,payload,peer,error))return false;
        argument.type=DH2_SCRIPT_SOURCE_OBJECT;argument.identity=peer.character_identity;count=1;
    }
    std::uint32_t source_status{};
    const int status=actual.script_owner?
        actual.script_owner->call_discard(view.identity,event,count?&argument:nullptr,count,source_status):
        actual.script_session->owner().call_discard(view.identity,event,count?&argument:nullptr,count,source_status);
    const auto& owner_error=actual.script_owner?actual.script_owner->error():actual.script_session->error();
    if(status||source_status){error="Actual companion Lua callback failed (native "+std::to_string(status)+", source "+std::to_string(source_status)+"): "+owner_error;return false;}
    return true;
}
bool update_live_rene(const LiveWorldBindings& b,ActorId id,std::string& error) {
    const auto* row=live_row(b,id);
    if(!row||row->script!="rene"){error="Required actual selected live rene AI row";return false;}
    return call_live_script(b,id,"OnUpdate",invalid_actor_id,error);
}
namespace {
bool coherent_target_projection(const LiveWorldBindings& b,const LiveActorOwner& owner,std::string& error){
    if(!owner.target_bindings||!owner.target_bindings->state||!owner.target_bindings->state->owner||
       owner.target_bindings->state->owner->identity!=owner.character_identity||!b.actor_from_identity){
        error="Required same-Character TargetBindings and native identity resolver";return false;
    }
    const auto native=owner.target_bindings->state->target;
    ActorId selected=invalid_actor_id;
    if(native&&!b.actor_from_identity(native,selected,error))return false;
    if(owner.actor->target_id!=selected){error="TargetBindings target differs from same ActorState target projection";return false;}
    return true;
}
struct SourceCallback {std::uint32_t event;const char* name;std::uint32_t callback_flag;};
constexpr SourceCallback master_callbacks[]={
    {18,"OnMasterDied",0},{19,"OnMasterRevived",0},{20,"OnMasterOutOfSight",0},
    {21,"OnMasterInSight",0},{22,"OnMasterOutOfRange",1u<<6},
    {23,"OnMasterInRangedRange",1u<<7},{24,"OnMasterInCloseRange",1u<<8},
    {25,"OnMasterInMeleeRange",1u<<9}
};
}
bool dispatch_live_rene_character_event(const LiveWorldBindings& b,ActorId id,
    std::uint32_t event,std::uintptr_t subject,std::string& error){
    error.clear();LiveActorOwner owner;
    if(!borrow_live_owner(b,id,owner,error))return false;
    const auto* row=live_row(b,id);
    if(!row||row->script!="rene"||!owner.script_session||owner.script_owner||!owner.ai_fields){
        error="Required same-Character V1 rene ScriptOwner and CharAI master fields";return false;
    }
    dh2::character::ScriptSessionView view{};
    if(!owner.script_session->owner().active(view)||view.kind!=dh2::character::script_external||
       !view.vm||!view.aliases||!view.constructor_fields||view.constructor_fields->character!=owner.character_identity){
        error="Required active same-Character AISExternal callback owner";return false;
    }
    if(event==dh2::object_identity::enemy_spotted||
       (event>=0xa&&event<=0x11)){
        if(!coherent_target_projection(b,owner,error))return false;
        std::uint32_t target_event{};
        switch(event){
        case 9:target_event=dh2::object_identity::enemy_spotted;break;
        case 0xa:target_event=dh2::object_identity::target_died;break;
        case 0xc:target_event=dh2::object_identity::target_out_of_sight;break;
        case 0xd:target_event=dh2::object_identity::target_in_sight;break;
        case 0xe:target_event=dh2::object_identity::target_out_of_range;break;
        case 0xf:target_event=dh2::object_identity::target_in_ranged_range;break;
        case 0x10:target_event=dh2::object_identity::target_in_close_range;break;
        case 0x11:target_event=dh2::object_identity::target_in_melee_range;break;
        case 0xb:
            // CharAI::OnTargetRevived has no ObjectIdentity adapter event. The
            // selected external owner still supplies the genuine optional alias.
            if(!dh2_script_alias_contains(view.aliases,"OnTargetRevived"))return true;
            if(!call_live_script(b,id,"OnTargetRevived",invalid_actor_id,error))return false;
            return coherent_target_projection(b,owner,error);
        default:error="Unsupported original companion target event";return false;
        }
        if(event==9&&!subject){error="OnEnemySpotted requires the original nonnull Character payload";return false;}
        if(event!=9&&subject){error="Target range/death event has no source object payload";return false;}
        const int status=owner.script_session->dispatch_target(target_event,event==9?subject:0);
        if(status){error=owner.script_session->error();if(error.empty())error="Original Rene target callback failed";return false;}
        return coherent_target_projection(b,owner,error);
    }
    for(const auto& callback:master_callbacks)if(callback.event==event){
        if(!owner.ai_fields->master50||subject!=owner.ai_fields->master50){
            error="Master AI event subject differs from same CharAI+0x50 identity";return false;
        }
        if(callback.callback_flag&&!(view.callback_flags&callback.callback_flag))return true;
        if(!dh2_script_alias_contains(view.aliases,callback.name))return true;
        return call_live_script(b,id,callback.name,invalid_actor_id,error);
    }
    error="Character event is outside the source Rene target/master callback domain";return false;
}
namespace {
struct Binding {const char* name;Operation operation;bool boolean,object;};
const Binding bindings[]={{"HasMaster",Operation::has_master,true,false},
    {"HasTarget",Operation::has_target,true,false},{"GetTarget",Operation::get_target,false,true},
    {"GetHostPlayer",Operation::host_player,false,true},{"IsMasterHostPlayer",Operation::is_master_host_player,true,false},
    {"HasPath",Operation::has_path,true,false},{"GetState",Operation::state,false,false},
    {"SetMaster",Operation::set_master,false,false},{"SetTarget",Operation::set_target,false,false},
    {"ClearTarget",Operation::clear_target,false,false},{"MoveTo",Operation::move_to,false,false},
    {"WarpTo",Operation::warp_to_actor,false,false},{"WarpBehind",Operation::warp_behind,false,false},
    {"Stop",Operation::stop,false,false},{"Attack",Operation::attack_current_target,false,false}};
int native(void* raw,const dh2_script_value* args,std::uint32_t count,dh2_script_value* results,
           std::uint32_t capacity,std::uint32_t* result_count,char* message,std::size_t message_capacity) {
    auto& context=*static_cast<NativeBindingContext*>(raw);std::string error;
    auto fail=[&] {if(message&&message_capacity){const auto n=std::min(message_capacity-1,error.size());std::memcpy(message,error.data(),n);message[n]=0;}return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;};
    try {
        LiveActorOwner actual;if(!borrow_live_owner(*context.world,context.actor,actual,error))return fail();
        Request request{context.operation,context.actor};
        if(context.operation==Operation::set_master) {
            SourceMasterBorrow master;if(!borrow_source_master(*context.world,context.actor,master,error))return fail();
            if(!source_set_master_values(master,args,count,error))return fail();
            *result_count=0;return 0;
        }
        const auto* binding=std::find_if(std::begin(bindings),std::end(bindings),[&](const Binding& b){return b.operation==context.operation&&std::strcmp(b.name,context.name)==0;});
        if(count&&(args[0].type==DH2_SCRIPT_SOURCE_OBJECT||args[0].type==DH2_SCRIPT_IDENTITY)) {
            if(!context.world->actor_from_identity){error="Required SAME native Character object resolver";return fail();}
            if(!context.world->actor_from_identity(args[0].identity,request.argument,error))return fail();
            LiveActorOwner peer;if(!borrow_live_owner(*context.world,request.argument,peer,error))return fail();
            if(peer.character_identity!=args[0].identity){error="Companion source object resolver identity mismatch";return fail();}
            if(request.argument==invalid_actor_id){error="Companion Lua object does not name SAME live Character";return fail();}
            if(request.operation==Operation::attack_current_target)request.operation=Operation::attack;
        }
        Response response;if(!live_invoke(*context.world)(*actual.actor,request,response,error))return fail();
        *result_count=0;
        if(binding->boolean||binding->object||binding->operation==Operation::state) {
            if(!capacity){error="Companion source result capacity unavailable";return fail();}
            results[0]={};*result_count=1;
            if(binding->boolean){results[0].type=DH2_SCRIPT_BOOLEAN;results[0].boolean=response.boolean;}
            else if(binding->object) {
                if(response.actor==invalid_actor_id)results[0].type=DH2_SCRIPT_NIL;
                else {LiveActorOwner peer;if(!borrow_live_owner(*context.world,response.actor,peer,error))return fail();
                    results[0].type=DH2_SCRIPT_SOURCE_OBJECT;results[0].identity=peer.character_identity;}
            } else {results[0].type=DH2_SCRIPT_NUMBER;results[0].number=static_cast<float>(response.integer);}
        }
        return 0;
    }catch(const std::exception& e){error=e.what();return fail();}
}
}
bool bind_live_native(const std::shared_ptr<LiveWorldBindings>& world,ActorId actor,dh2_script_vm* vm,
    const char* name,NativeBindingOwner& owner,bool& handled,std::string& error) {
    handled=false;if(!name){error="Missing source native binding name";return false;}
    const auto* found=std::find_if(std::begin(bindings),std::end(bindings),[&](const Binding& b){return std::strcmp(b.name,name)==0;});
    if(found==std::end(bindings))return true;
    handled=true;if(!world||!vm){error="Required actual companion VM/world registration";return false;}
    LiveActorOwner actual;if(!borrow_live_owner(*world,actor,actual,error))return false;
    if(!actual.script_owner&&!actual.script_session){error="Required actual Character ScriptOwner registration receiver";return false;}
    dh2::character::ScriptSessionView view{};
    auto matches=[&](auto& script_owner){
        bool same=script_owner.pending(view)&&view.vm==vm&&view.constructor_fields&&view.constructor_fields->character==actual.character_identity;
        if(!same)same=script_owner.active(view)&&view.vm==vm&&view.constructor_fields&&view.constructor_fields->character==actual.character_identity;
        return same;
    };
    const bool same=actual.script_owner?matches(*actual.script_owner):matches(actual.script_session->owner());
    if(!same){error="Companion native registration VM is not SAME pending/active source owner";return false;}
    auto context=std::make_unique<NativeBindingContext>();context->world=world;context->actor=actor;context->name=found->name;context->operation=found->operation;
    if(dh2_script_vm_bind_source_objects(vm,name,native,context.get())){error="Actual companion Lua native registration failed";return false;}
    owner.callbacks.push_back(std::move(context));return true;
}
}
